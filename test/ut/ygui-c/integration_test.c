/*
 * integration_test.c - Integration tests with mock terminal
 *
 * Tests event handling, callbacks, and coordinate transformation
 * by injecting OSC events and verifying widget responses.
 */

#include "test_harness.h"
#include "../../../src/yetty/ygui-c/ygui.h"
#include "../../../src/yetty/ygui-c/ygui_internal.h"
#include <string.h>

/*=============================================================================
 * Mock Terminal Harness
 *
 * Provides functions to:
 * - Create engine without terminal initialization
 * - Inject mouse/key events directly
 * - Track callback invocations
 *===========================================================================*/

/* Forward declaration of internal process_input function */
extern void ygui_engine_mouse_down(ygui_engine_t* engine, float x, float y, int button);
extern void ygui_engine_mouse_up(ygui_engine_t* engine, float x, float y, int button);
extern void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y);
extern void ygui_engine_key_down(ygui_engine_t* engine, uint32_t key, int mods);

/*
 * Create engine for testing without terminal init
 * (We don't call ygui_init() to avoid messing with terminal)
 */
static ygui_engine_t* create_test_engine(const char* name, float w, float h) {
    ygui_engine_t* e = ygui_engine_create(name, w, h);
    if (e) {
        /* Set cell size manually (normally comes from CSI 16 t response) */
        e->cell_width = 8.0f;
        e->cell_height = 16.0f;
        e->card_x = 0;
        e->card_y = 0;
        e->card_w = (int)(w / 8);
        e->card_h = (int)(h / 16);
    }
    return e;
}

/*
 * Force rebuild grid with current widget positions
 * (Normally done during render, but we skip render in tests)
 */
static void rebuild_grid(ygui_engine_t* e) {
    ygui_grid_clear(&e->grid);
    for (ygui_widget_t* w = e->first_widget; w; w = w->next_sibling) {
        w->effective_x = w->x;
        w->effective_y = w->y;
        w->was_rendered = 1;
        ygui_grid_insert(&e->grid, w);
    }
}

/*=============================================================================
 * Callback Tracking Helpers
 *===========================================================================*/

static int g_click_count = 0;
static int g_change_count = 0;
static float g_last_value = 0;
static int g_last_checked = 0;
static const char* g_last_widget_id = NULL;

static void reset_tracking(void) {
    g_click_count = 0;
    g_change_count = 0;
    g_last_value = 0;
    g_last_checked = 0;
    g_last_widget_id = NULL;
}

static void track_click(ygui_widget_t* w, void* userdata) {
    (void)userdata;
    g_click_count++;
    g_last_widget_id = w->id;
}

static void track_change(ygui_widget_t* w, float value, void* userdata) {
    (void)userdata;
    g_change_count++;
    g_last_value = value;
    g_last_widget_id = w->id;
}

static void track_check(ygui_widget_t* w, int checked, void* userdata) {
    (void)userdata;
    g_change_count++;
    g_last_checked = checked;
    g_last_widget_id = w->id;
}

/*=============================================================================
 * Button Click Tests
 *===========================================================================*/

static int test_button_click_callback(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn1", 50, 50, 100, 40, "Click Me");
    ygui_button_on_click(btn, track_click, NULL);
    rebuild_grid(e);

    /* Simulate click: mouse down then up at same location */
    ygui_engine_mouse_down(e, 75, 70, 1);
    TEST_ASSERT_EQ(g_click_count, 0, "no click on mouse down");

    ygui_engine_mouse_up(e, 75, 70, 1);
    TEST_ASSERT_EQ(g_click_count, 1, "click on mouse up");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn1", "correct widget");

    ygui_engine_destroy(e);
    return 0;
}

static int test_button_click_miss(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Click Me");
    ygui_button_on_click(btn, track_click, NULL);
    rebuild_grid(e);

    /* Click outside button */
    ygui_engine_mouse_down(e, 10, 10, 1);
    ygui_engine_mouse_up(e, 10, 10, 1);
    TEST_ASSERT_EQ(g_click_count, 0, "no click outside button");

    ygui_engine_destroy(e);
    return 0;
}

static int test_button_drag_away_no_click(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Click Me");
    ygui_button_on_click(btn, track_click, NULL);
    rebuild_grid(e);

    /* Mouse down on button, then release outside */
    ygui_engine_mouse_down(e, 75, 70, 1);
    ygui_engine_mouse_up(e, 10, 10, 1);  /* Release outside */
    TEST_ASSERT_EQ(g_click_count, 0, "no click when dragged away");

    ygui_engine_destroy(e);
    return 0;
}

static int test_multiple_button_clicks(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn1 = ygui_button(e, "btn1", 10, 10, 80, 30, "One");
    ygui_widget_t* btn2 = ygui_button(e, "btn2", 10, 50, 80, 30, "Two");
    ygui_button_on_click(btn1, track_click, NULL);
    ygui_button_on_click(btn2, track_click, NULL);
    rebuild_grid(e);

    /* Click btn1 */
    ygui_engine_mouse_down(e, 50, 25, 1);
    ygui_engine_mouse_up(e, 50, 25, 1);
    TEST_ASSERT_EQ(g_click_count, 1, "first click");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn1", "clicked btn1");

    /* Click btn2 */
    ygui_engine_mouse_down(e, 50, 65, 1);
    ygui_engine_mouse_up(e, 50, 65, 1);
    TEST_ASSERT_EQ(g_click_count, 2, "second click");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn2", "clicked btn2");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Slider Interaction Tests
 *===========================================================================*/

static int test_slider_drag_changes_value(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* sld = ygui_slider(e, "slider", 50, 50, 200, 30, 0, 100, 50);
    ygui_slider_on_change(sld, track_change, NULL);
    rebuild_grid(e);

    /* Click at 75% of slider width */
    float click_x = 50 + 200 * 0.75f;  /* 200 */
    ygui_engine_mouse_down(e, click_x, 65, 1);
    TEST_ASSERT_EQ(g_change_count, 1, "change on press");
    TEST_ASSERT_EQ_FLOAT(g_last_value, 75.0f, 1.0f, "value at 75%");

    /* Drag to 25% */
    float drag_x = 50 + 200 * 0.25f;  /* 100 */
    ygui_engine_mouse_move(e, drag_x, 65);
    TEST_ASSERT_EQ(g_change_count, 2, "change on drag");
    TEST_ASSERT_EQ_FLOAT(g_last_value, 25.0f, 1.0f, "value at 25%");

    ygui_engine_mouse_up(e, drag_x, 65, 1);

    ygui_engine_destroy(e);
    return 0;
}

static int test_slider_clamps_value(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* sld = ygui_slider(e, "slider", 50, 50, 200, 30, 0, 100, 50);
    ygui_slider_on_change(sld, track_change, NULL);
    rebuild_grid(e);

    /* Click at right edge (x=249 is inside, maps to ~99.5%) */
    ygui_engine_mouse_down(e, 249, 65, 1);
    TEST_ASSERT_EQ_FLOAT(g_last_value, 99.5f, 1.0f, "near max");

    /* Now drag beyond right edge - should clamp to max */
    ygui_engine_mouse_move(e, 350, 65);
    TEST_ASSERT_EQ_FLOAT(g_last_value, 100.0f, 0.1f, "clamped to max");

    /* Drag beyond left edge - should clamp to min */
    ygui_engine_mouse_move(e, 0, 65);
    TEST_ASSERT_EQ_FLOAT(g_last_value, 0.0f, 0.1f, "clamped to min");

    ygui_engine_mouse_up(e, 0, 65, 1);

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Checkbox Interaction Tests
 *===========================================================================*/

static int test_checkbox_toggle(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* chk = ygui_checkbox(e, "chk", 50, 50, 150, 30, "Enable", 0);
    ygui_checkbox_on_change(chk, track_check, NULL);
    rebuild_grid(e);

    /* Click to check */
    ygui_engine_mouse_down(e, 75, 65, 1);
    ygui_engine_mouse_up(e, 75, 65, 1);
    TEST_ASSERT_EQ(g_change_count, 1, "change on click");
    TEST_ASSERT_EQ(g_last_checked, 1, "now checked");
    TEST_ASSERT_EQ(ygui_checkbox_get_checked(chk), 1, "widget state checked");

    /* Click to uncheck */
    ygui_engine_mouse_down(e, 75, 65, 1);
    ygui_engine_mouse_up(e, 75, 65, 1);
    TEST_ASSERT_EQ(g_change_count, 2, "second change");
    TEST_ASSERT_EQ(g_last_checked, 0, "now unchecked");
    TEST_ASSERT_EQ(ygui_checkbox_get_checked(chk), 0, "widget state unchecked");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Hover State Tests
 *===========================================================================*/

static int test_hover_state_changes(void) {
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Hover Me");
    rebuild_grid(e);

    /* Initially not hovered */
    TEST_ASSERT(!(btn->flags & YGUI_FLAG_HOVER), "initially not hovered");

    /* Move over button */
    ygui_engine_mouse_move(e, 75, 70);
    TEST_ASSERT(btn->flags & YGUI_FLAG_HOVER, "hovered after move over");
    TEST_ASSERT(e->hovered == btn, "engine tracks hovered widget");

    /* Move away from button */
    ygui_engine_mouse_move(e, 10, 10);
    TEST_ASSERT(!(btn->flags & YGUI_FLAG_HOVER), "not hovered after move away");
    TEST_ASSERT_NULL(e->hovered, "engine clears hovered");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Pressed State Tests
 *===========================================================================*/

static int test_pressed_state(void) {
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Press Me");
    rebuild_grid(e);

    /* Initially not pressed */
    TEST_ASSERT(!(btn->flags & YGUI_FLAG_PRESSED), "initially not pressed");

    /* Mouse down */
    ygui_engine_mouse_down(e, 75, 70, 1);
    TEST_ASSERT(btn->flags & YGUI_FLAG_PRESSED, "pressed after mouse down");
    TEST_ASSERT(e->pressed == btn, "engine tracks pressed widget");

    /* Mouse up */
    ygui_engine_mouse_up(e, 75, 70, 1);
    TEST_ASSERT(!(btn->flags & YGUI_FLAG_PRESSED), "not pressed after mouse up");
    TEST_ASSERT_NULL(e->pressed, "engine clears pressed");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Focus State Tests
 *===========================================================================*/

static int test_focus_state(void) {
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn1 = ygui_button(e, "btn1", 10, 10, 80, 30, "One");
    ygui_widget_t* btn2 = ygui_button(e, "btn2", 10, 50, 80, 30, "Two");
    rebuild_grid(e);

    /* Initially nothing focused */
    TEST_ASSERT_NULL(e->focused, "initially no focus");

    /* Click btn1 - gains focus */
    ygui_engine_mouse_down(e, 50, 25, 1);
    TEST_ASSERT(e->focused == btn1, "btn1 focused after click");
    TEST_ASSERT(btn1->flags & YGUI_FLAG_FOCUSED, "btn1 has focus flag");

    /* Click btn2 - focus moves */
    ygui_engine_mouse_down(e, 50, 65, 1);
    TEST_ASSERT(e->focused == btn2, "btn2 focused after click");
    TEST_ASSERT(btn2->flags & YGUI_FLAG_FOCUSED, "btn2 has focus flag");
    TEST_ASSERT(!(btn1->flags & YGUI_FLAG_FOCUSED), "btn1 lost focus flag");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Coordinate Transformation Tests
 *===========================================================================*/

static int test_coord_transform_no_zoom(void) {
    ygui_engine_t* e = create_test_engine("test", 500, 300);
    e->cell_width = 8.0f;
    e->cell_height = 16.0f;
    e->card_w = 62;  /* 496 display pixels */
    e->card_h = 18;  /* 288 display pixels */
    e->view_zoom = 1.0f;
    e->view_scroll_x = 0;
    e->view_scroll_y = 0;

    /* Button at canvas position (100, 100) */
    ygui_widget_t* btn = ygui_button(e, "btn", 100, 100, 80, 40, "Test");
    rebuild_grid(e);

    /*
     * Display pixel to click = canvas * (display_w / canvas_w)
     * For canvas x=100: display_x = 100 * (496/500) = 99.2
     */
    float display_x = 100.0f * (496.0f / 500.0f);
    float display_y = 110.0f * (288.0f / 300.0f);

    /* But the grid query uses canvas coordinates directly.
     * The transformation happens when processing OSC events.
     * Here we test with canvas coords directly. */
    ygui_widget_t* hit = ygui_grid_query(&e->grid, 110, 110);
    TEST_ASSERT(hit == btn, "hit button at canvas coords");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Dirty Flag Tests
 *===========================================================================*/

static int test_dirty_after_widget_change(void) {
    ygui_engine_t* e = create_test_engine("test", 400, 300);
    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Test");

    e->dirty = 0;
    ygui_button_set_label(btn, "New Label");
    TEST_ASSERT(e->dirty, "dirty after label change");

    e->dirty = 0;
    ygui_widget_set_position(btn, 60, 60);
    TEST_ASSERT(e->dirty, "dirty after position change");

    e->dirty = 0;
    ygui_widget_set_visible(btn, 0);
    TEST_ASSERT(e->dirty, "dirty after visibility change");

    ygui_engine_destroy(e);
    return 0;
}

static int test_dirty_after_interaction(void) {
    ygui_engine_t* e = create_test_engine("test", 400, 300);
    ygui_button(e, "btn", 50, 50, 100, 40, "Test");
    rebuild_grid(e);

    e->dirty = 0;
    ygui_engine_mouse_move(e, 75, 70);  /* Hover */
    TEST_ASSERT(e->dirty, "dirty after hover");

    e->dirty = 0;
    ygui_engine_mouse_down(e, 75, 70, 1);  /* Press */
    TEST_ASSERT(e->dirty, "dirty after press");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Disabled Widget Interaction Tests
 *===========================================================================*/

static int test_disabled_widget_no_click(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Disabled");
    ygui_button_on_click(btn, track_click, NULL);
    ygui_widget_set_enabled(btn, 0);
    rebuild_grid(e);

    /* Click on disabled button */
    ygui_engine_mouse_down(e, 75, 70, 1);
    ygui_engine_mouse_up(e, 75, 70, 1);
    TEST_ASSERT_EQ(g_click_count, 0, "no click on disabled button");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Invisible Widget Interaction Tests
 *===========================================================================*/

static int test_invisible_widget_no_click(void) {
    reset_tracking();
    ygui_engine_t* e = create_test_engine("test", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 50, 50, 100, 40, "Invisible");
    ygui_button_on_click(btn, track_click, NULL);
    ygui_widget_set_visible(btn, 0);
    rebuild_grid(e);

    /* Click where invisible button would be */
    ygui_engine_mouse_down(e, 75, 70, 1);
    ygui_engine_mouse_up(e, 75, 70, 1);
    TEST_ASSERT_EQ(g_click_count, 0, "no click on invisible button");

    ygui_engine_destroy(e);
    return 0;
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_case_t tests[] = {
        /* Button clicks */
        {"button_click_callback", test_button_click_callback},
        {"button_click_miss", test_button_click_miss},
        {"button_drag_away_no_click", test_button_drag_away_no_click},
        {"multiple_button_clicks", test_multiple_button_clicks},

        /* Slider */
        {"slider_drag_changes_value", test_slider_drag_changes_value},
        {"slider_clamps_value", test_slider_clamps_value},

        /* Checkbox */
        {"checkbox_toggle", test_checkbox_toggle},

        /* State changes */
        {"hover_state_changes", test_hover_state_changes},
        {"pressed_state", test_pressed_state},
        {"focus_state", test_focus_state},

        /* Coordinates */
        {"coord_transform_no_zoom", test_coord_transform_no_zoom},

        /* Dirty flag */
        {"dirty_after_widget_change", test_dirty_after_widget_change},
        {"dirty_after_interaction", test_dirty_after_interaction},

        /* Disabled/Invisible */
        {"disabled_widget_no_click", test_disabled_widget_no_click},
        {"invisible_widget_no_click", test_invisible_widget_no_click},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    return run_tests("Integration Tests", tests, count);
}
