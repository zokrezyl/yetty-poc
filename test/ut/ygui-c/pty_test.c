/*
 * pty_test.c - PTY-based integration tests for ygui-c
 *
 * Tests the full libuv event loop with real PTY communication.
 * OSC sequences are written to the PTY master and processed by
 * the engine through the actual libuv callbacks.
 */

#include "pty_harness.h"
/* ygui_internal.h is included via pty_harness.h */

/*=============================================================================
 * Callback Tracking
 *===========================================================================*/

static int g_click_count = 0;
static int g_change_count = 0;
static int g_check_count = 0;
static float g_last_value = 0.0f;
static int g_last_checked = 0;
static const char* g_last_widget_id = NULL;

static void reset_tracking(void) {
    g_click_count = 0;
    g_change_count = 0;
    g_check_count = 0;
    g_last_value = 0.0f;
    g_last_checked = 0;
    g_last_widget_id = NULL;
}

static void track_click(ygui_widget_t* w, void* userdata) {
    (void)userdata;
    g_click_count++;
    g_last_widget_id = ygui_widget_id(w);
}

static void track_change(ygui_widget_t* w, float value, void* userdata) {
    (void)userdata;
    g_change_count++;
    g_last_value = value;
    g_last_widget_id = ygui_widget_id(w);
}

static void track_check(ygui_widget_t* w, int checked, void* userdata) {
    (void)userdata;
    g_check_count++;
    g_last_checked = checked;
    g_last_widget_id = ygui_widget_id(w);
}

/*=============================================================================
 * Button Tests via PTY
 *===========================================================================*/

static int test_pty_button_click(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    /* Create button */
    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Click Me");
    ygui_button_on_click(btn, track_click, NULL);
    pty_rebuild_grid(&ctx);

    /* Send mouse down via PTY */
    pty_send_click(&ctx, "test", 1, 1, 75, 70);  /* press at center */

    /* Run loop to process the event */
    pty_run_iterations(&ctx, 10);

    /* Send mouse up */
    pty_send_click(&ctx, "test", 1, 0, 75, 70);  /* release */
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 1, "button clicked via PTY");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn", "correct widget");

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_button_miss(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Test");
    ygui_button_on_click(btn, track_click, NULL);
    pty_rebuild_grid(&ctx);

    /* Click outside button */
    pty_send_click(&ctx, "test", 1, 1, 10, 10);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 10, 10);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 0, "no click when missing button");

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_multiple_buttons(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn1 = ygui_button(ctx.engine, "btn1", 10, 10, 80, 30, "One");
    ygui_widget_t* btn2 = ygui_button(ctx.engine, "btn2", 100, 10, 80, 30, "Two");
    ygui_button_on_click(btn1, track_click, NULL);
    ygui_button_on_click(btn2, track_click, NULL);
    pty_rebuild_grid(&ctx);

    /* Click btn1 */
    pty_send_click(&ctx, "test", 1, 1, 50, 25);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 50, 25);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 1, "first click");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn1", "btn1 clicked");

    /* Click btn2 */
    pty_send_click(&ctx, "test", 1, 1, 140, 25);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 140, 25);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 2, "second click");
    TEST_ASSERT_STR_EQ(g_last_widget_id, "btn2", "btn2 clicked");

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * Slider Tests via PTY
 *===========================================================================*/

static int test_pty_slider_drag(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* sld = ygui_slider(ctx.engine, "slider", 50, 50, 200, 30, 0, 100, 50);
    ygui_slider_on_change(sld, track_change, NULL);
    pty_rebuild_grid(&ctx);

    /* Press at 50% position */
    pty_send_click(&ctx, "test", 1, 1, 150, 65);  /* center = 50% */
    pty_run_iterations(&ctx, 10);
    TEST_ASSERT_EQ(g_change_count, 1, "change on press");

    /* Drag to 75% */
    float x_75 = 50 + 200 * 0.75f;  /* 200 */
    pty_send_move(&ctx, "test", 1, (int)x_75, 65);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_change_count, 2, "change on drag");
    TEST_ASSERT_EQ_FLOAT(g_last_value, 75.0f, 2.0f, "value at 75%");

    /* Release */
    pty_send_click(&ctx, "test", 1, 0, (int)x_75, 65);
    pty_run_iterations(&ctx, 10);

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_slider_clamp(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* sld = ygui_slider(ctx.engine, "slider", 50, 50, 200, 30, 0, 100, 50);
    ygui_slider_on_change(sld, track_change, NULL);
    pty_rebuild_grid(&ctx);

    /* Press inside slider */
    pty_send_click(&ctx, "test", 1, 1, 150, 65);
    pty_run_iterations(&ctx, 10);

    /* Drag way beyond right edge */
    pty_send_move(&ctx, "test", 1, 500, 65);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ_FLOAT(g_last_value, 100.0f, 0.1f, "clamped to max");

    /* Drag way before left edge */
    pty_send_move(&ctx, "test", 1, -100, 65);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ_FLOAT(g_last_value, 0.0f, 0.1f, "clamped to min");

    pty_send_click(&ctx, "test", 1, 0, -100, 65);
    pty_run_iterations(&ctx, 10);

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * Checkbox Tests via PTY
 *===========================================================================*/

static int test_pty_checkbox_toggle(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* chk = ygui_checkbox(ctx.engine, "chk", 50, 50, 150, 30, "Enable", 0);
    ygui_checkbox_on_change(chk, track_check, NULL);
    pty_rebuild_grid(&ctx);

    /* Click to check */
    pty_send_click(&ctx, "test", 1, 1, 75, 65);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 75, 65);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_check_count, 1, "change count");
    TEST_ASSERT_EQ(g_last_checked, 1, "now checked");
    TEST_ASSERT_EQ(ygui_checkbox_get_checked(chk), 1, "widget state");

    /* Click again to uncheck */
    pty_send_click(&ctx, "test", 1, 1, 75, 65);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 75, 65);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_check_count, 2, "change count");
    TEST_ASSERT_EQ(g_last_checked, 0, "now unchecked");

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * State Tests via PTY
 *===========================================================================*/

static int test_pty_hover_state(void) {
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Test");
    pty_rebuild_grid(&ctx);

    /* Move over button (no buttons pressed) */
    pty_send_move(&ctx, "test", 0, 75, 70);
    pty_run_iterations(&ctx, 10);

    uint32_t flags = ygui_widget_get_flags(btn);
    TEST_ASSERT(flags & YGUI_FLAG_HOVER, "hover flag set");

    /* Move away */
    pty_send_move(&ctx, "test", 0, 10, 10);
    pty_run_iterations(&ctx, 10);

    flags = ygui_widget_get_flags(btn);
    TEST_ASSERT(!(flags & YGUI_FLAG_HOVER), "hover flag cleared");

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_pressed_state(void) {
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Test");
    pty_rebuild_grid(&ctx);

    /* Press */
    pty_send_click(&ctx, "test", 1, 1, 75, 70);
    pty_run_iterations(&ctx, 10);

    uint32_t flags = ygui_widget_get_flags(btn);
    TEST_ASSERT(flags & YGUI_FLAG_PRESSED, "pressed flag set");

    /* Release */
    pty_send_click(&ctx, "test", 1, 0, 75, 70);
    pty_run_iterations(&ctx, 10);

    flags = ygui_widget_get_flags(btn);
    TEST_ASSERT(!(flags & YGUI_FLAG_PRESSED), "pressed flag cleared");

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * View Change Tests via PTY
 *===========================================================================*/

static int test_pty_view_change(void) {
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    /* Send view change event */
    pty_send_view(&ctx, "test", 2.0f, 100.0f, 50.0f);
    pty_run_iterations(&ctx, 10);

    /* Check engine state (access via internal struct) */
    ygui_engine_t* e = ctx.engine;
    TEST_ASSERT_EQ_FLOAT(e->view_zoom, 2.0f, 0.01f, "zoom level");
    TEST_ASSERT_EQ_FLOAT(e->view_scroll_x, 100.0f, 0.01f, "scroll x");
    TEST_ASSERT_EQ_FLOAT(e->view_scroll_y, 50.0f, 0.01f, "scroll y");

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_cell_size(void) {
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    /* Send cell size response */
    pty_send_cell_size(&ctx, 20, 10);  /* 20px height, 10px width */
    pty_run_iterations(&ctx, 10);

    ygui_engine_t* e = ctx.engine;
    TEST_ASSERT_EQ_FLOAT(e->cell_height, 20.0f, 0.01f, "cell height");
    TEST_ASSERT_EQ_FLOAT(e->cell_width, 10.0f, 0.01f, "cell width");

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * Disabled Widget Tests via PTY
 *===========================================================================*/

static int test_pty_disabled_button(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Test");
    ygui_button_on_click(btn, track_click, NULL);
    ygui_widget_set_enabled(btn, 0);
    pty_rebuild_grid(&ctx);

    /* Try to click disabled button */
    pty_send_click(&ctx, "test", 1, 1, 75, 70);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 75, 70);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 0, "disabled button not clicked");

    pty_test_cleanup(&ctx);
    return 0;
}

static int test_pty_invisible_button(void) {
    reset_tracking();
    pty_test_ctx_t ctx;

    if (pty_test_init(&ctx, "test", 400, 300) < 0) {
        return 1;
    }

    ygui_widget_t* btn = ygui_button(ctx.engine, "btn", 50, 50, 100, 40, "Test");
    ygui_button_on_click(btn, track_click, NULL);
    ygui_widget_set_visible(btn, 0);
    pty_rebuild_grid(&ctx);

    /* Try to click invisible button */
    pty_send_click(&ctx, "test", 1, 1, 75, 70);
    pty_run_iterations(&ctx, 10);
    pty_send_click(&ctx, "test", 1, 0, 75, 70);
    pty_run_iterations(&ctx, 10);

    TEST_ASSERT_EQ(g_click_count, 0, "invisible button not clicked");

    pty_test_cleanup(&ctx);
    return 0;
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_case_t tests[] = {
        {"pty_button_click", test_pty_button_click},
        {"pty_button_miss", test_pty_button_miss},
        {"pty_multiple_buttons", test_pty_multiple_buttons},
        {"pty_slider_drag", test_pty_slider_drag},
        {"pty_slider_clamp", test_pty_slider_clamp},
        {"pty_checkbox_toggle", test_pty_checkbox_toggle},
        {"pty_hover_state", test_pty_hover_state},
        {"pty_pressed_state", test_pty_pressed_state},
        {"pty_view_change", test_pty_view_change},
        {"pty_cell_size", test_pty_cell_size},
        {"pty_disabled_button", test_pty_disabled_button},
        {"pty_invisible_button", test_pty_invisible_button},
    };

    int count = sizeof(tests) / sizeof(tests[0]);
    return run_tests("PTY Integration Tests (libuv)", tests, count);
}
