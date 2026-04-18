/*
 * Unit tests for ygui-c coordinate transformation
 *
 * Tests the display-to-canvas coordinate scaling that happens when
 * mouse events arrive from yetty.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define ASSERT_NEAR(a, b, eps, msg) do { \
    float _a = (a), _b = (b), _eps = (eps); \
    if (fabs(_a - _b) > _eps) { \
        printf("FAIL: %s: expected %.2f, got %.2f (diff=%.4f)\n", msg, _b, _a, fabs(_a - _b)); \
        return 1; \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        printf("FAIL: %s\n", msg); \
        return 1; \
    } \
} while(0)

/*
 * Replicate the scale_coords formula from ygui_engine.c
 *
 * Original (no zoom/scroll):
 *   canvas_x = display_x * (canvas_w / display_w)
 *   display_w = card_cells * cell_pixels
 *
 * With zoom/scroll:
 *   visible_w = canvas_w / zoom
 *   canvas_x = scroll_x + (display_x / display_w) * visible_w
 */
static void scale_coords_test(
    float display_x, float display_y,
    float cell_w, float cell_h,
    int card_w_cells, int card_h_cells,
    float canvas_w, float canvas_h,
    float view_zoom, float scroll_x, float scroll_y,
    float *canvas_x, float *canvas_y)
{
    if (cell_w > 0 && cell_h > 0 && card_w_cells > 0 && card_h_cells > 0) {
        float display_w = card_w_cells * cell_w;
        float display_h = card_h_cells * cell_h;

        /* When zoomed, display shows only a portion of the canvas */
        float visible_w = canvas_w / view_zoom;
        float visible_h = canvas_h / view_zoom;

        /* Transform: display → visible canvas portion → canvas coords */
        *canvas_x = scroll_x + (display_x / display_w) * visible_w;
        *canvas_y = scroll_y + (display_y / display_h) * visible_h;
    } else {
        /* No transformation if cell size unknown */
        *canvas_x = display_x;
        *canvas_y = display_y;
    }
}

/*
 * Test 1: Basic scaling without zoom
 * Canvas: 500x300, Card: 62x18 cells, Cell: 8x16 pixels
 * Display: 496x288 pixels
 */
static int test_basic_scaling_no_zoom(void) {
    float canvas_x, canvas_y;

    /* Setup: 500x300 canvas, 62x18 cells, 8x16 cell size */
    float cell_w = 8.0f, cell_h = 16.0f;
    int card_w = 62, card_h = 18;
    float canvas_w = 500.0f, canvas_h = 300.0f;
    float display_w = card_w * cell_w;  /* 496 */
    float display_h = card_h * cell_h;  /* 288 */

    /* Click at display origin (0,0) -> canvas origin */
    scale_coords_test(0, 0, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, 0.0f, 0.1f, "origin x");
    ASSERT_NEAR(canvas_y, 0.0f, 0.1f, "origin y");

    /* Click at display center -> canvas center (roughly) */
    scale_coords_test(display_w / 2, display_h / 2, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, canvas_w / 2, 1.0f, "center x");
    ASSERT_NEAR(canvas_y, canvas_h / 2, 1.0f, "center y");

    /* Click at display (20, 50) for button at canvas (20, 50) */
    /* display_x = 20 -> canvas_x = 20 * (500/496) = 20.16 */
    float expected_x = 20.0f * (canvas_w / display_w);
    scale_coords_test(20, 50, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, expected_x, 0.1f, "button left edge x");

    /* Click at display right edge of button area */
    /* Button at canvas (20, 50) with w=200 has right edge at canvas x=220 */
    /* To hit canvas x=220, we need display_x = 220 * (496/500) = 217.6 */
    float display_for_220 = 220.0f * (display_w / canvas_w);
    scale_coords_test(display_for_220, 50, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, 220.0f, 0.1f, "button right edge x");

    printf("PASS: test_basic_scaling_no_zoom\n");
    return 0;
}

/*
 * Test 2: Button hit testing with transformed coordinates
 * Verify clicks at button boundaries
 */
static int test_button_hit_boundaries(void) {
    float canvas_x, canvas_y;

    /* Setup: same as demo */
    float cell_w = 8.0f, cell_h = 16.0f;
    int card_w = 62, card_h = 18;
    float canvas_w = 500.0f, canvas_h = 300.0f;
    float display_w = card_w * cell_w;  /* 496 */

    /* Button at canvas (20, 50, 200, 50) -> right edge at x=220 */
    float btn_left = 20.0f;
    float btn_right = 220.0f;

    /* Click just inside left edge: display_x for canvas x=21 */
    float display_inside_left = 21.0f * (display_w / canvas_w);
    scale_coords_test(display_inside_left, 75, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_TRUE(canvas_x >= btn_left && canvas_x < btn_right, "inside left edge should be in button");

    /* Click just outside left edge: display_x for canvas x=19 */
    float display_outside_left = 19.0f * (display_w / canvas_w);
    scale_coords_test(display_outside_left, 75, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_TRUE(canvas_x < btn_left, "outside left edge should NOT be in button");

    /* Click just inside right edge: display_x for canvas x=219 */
    float display_inside_right = 219.0f * (display_w / canvas_w);
    scale_coords_test(display_inside_right, 75, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_TRUE(canvas_x >= btn_left && canvas_x < btn_right, "inside right edge should be in button");

    /* Click just outside right edge: display_x for canvas x=221 */
    float display_outside_right = 221.0f * (display_w / canvas_w);
    scale_coords_test(display_outside_right, 75, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_TRUE(canvas_x >= btn_right, "outside right edge should NOT be in button");

    printf("PASS: test_button_hit_boundaries\n");
    return 0;
}

/*
 * Test 3: Zoom changes visible area
 */
static int test_zoom_scaling(void) {
    float canvas_x, canvas_y;

    float cell_w = 8.0f, cell_h = 16.0f;
    int card_w = 62, card_h = 18;
    float canvas_w = 500.0f, canvas_h = 300.0f;
    float display_w = card_w * cell_w;

    /* At 200% zoom, visible canvas is 250x150 */
    /* Click at display center (248, 144) should map to canvas (125, 75) */
    scale_coords_test(display_w / 2, 144, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 2.0f, 0, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, 125.0f, 1.0f, "2x zoom center x");

    /* With zoom=2 and scroll_x=100, display center should map to canvas 225 */
    scale_coords_test(display_w / 2, 144, cell_w, cell_h, card_w, card_h,
                      canvas_w, canvas_h, 2.0f, 100, 0, &canvas_x, &canvas_y);
    ASSERT_NEAR(canvas_x, 225.0f, 1.0f, "2x zoom scrolled center x");

    printf("PASS: test_zoom_scaling\n");
    return 0;
}

/*
 * Test 4: Default values when zoom is 1.0
 * This is the common case - should match original formula
 */
static int test_default_zoom_matches_original(void) {
    float canvas_x, canvas_y;
    float canvas_x_orig, canvas_y_orig;

    float cell_w = 8.0f, cell_h = 16.0f;
    int card_w = 62, card_h = 18;
    float canvas_w = 500.0f, canvas_h = 300.0f;
    float display_w = card_w * cell_w;
    float display_h = card_h * cell_h;

    /* Test multiple points */
    float test_points[][2] = {
        {0, 0}, {100, 50}, {200, 100}, {300, 150}, {400, 200}, {496, 288}
    };

    for (int i = 0; i < 6; i++) {
        float dx = test_points[i][0];
        float dy = test_points[i][1];

        /* New formula with zoom=1, scroll=0 */
        scale_coords_test(dx, dy, cell_w, cell_h, card_w, card_h,
                          canvas_w, canvas_h, 1.0f, 0, 0, &canvas_x, &canvas_y);

        /* Original formula */
        canvas_x_orig = dx * (canvas_w / display_w);
        canvas_y_orig = dy * (canvas_h / display_h);

        ASSERT_NEAR(canvas_x, canvas_x_orig, 0.001f, "formula match x");
        ASSERT_NEAR(canvas_y, canvas_y_orig, 0.001f, "formula match y");
    }

    printf("PASS: test_default_zoom_matches_original\n");
    return 0;
}

int main(void) {
    int failures = 0;

    failures += test_basic_scaling_no_zoom();
    failures += test_button_hit_boundaries();
    failures += test_zoom_scaling();
    failures += test_default_zoom_matches_original();

    printf("\n%d test(s) failed\n", failures);
    return failures;
}
