/*
 * dual_process_test.c - Dual-process integration tests for ygui-c
 *
 * Tests run with two processes communicating via PTY (like real terminal),
 * with a control pipe for test synchronization and assertions.
 * Both sides run libuv continuously - no artificial timing.
 */

#include "dual_process_harness.h"

/*=============================================================================
 * Test State Machine
 *
 * Each test is a state machine driven by:
 * - on_pty_data: client rendered something
 * - on_ctrl_response: client responded to query/assert
 *
 * States advance through the test steps asynchronously.
 *===========================================================================*/

typedef enum {
    STATE_INIT = 0,
    STATE_WAIT_RENDER,      /* Waiting for initial render */
    STATE_SEND_PIXEL_SIZE,  /* Send OSC 777780 pixel size */
    STATE_SEND_EVENT,       /* Send mouse/key event */
    STATE_WAIT_RESPONSE,    /* Wait for render after event */
    STATE_QUERY,            /* Query state */
    STATE_ASSERT,           /* Assert state */
    STATE_CLEANUP,          /* Shutdown and finish */
    STATE_DONE
} test_state_t;

typedef struct {
    test_state_t state;
    int step;               /* Sub-step within state */
    int render_count;       /* Number of renders received */
    int expected_renders;   /* Renders to wait for before proceeding */
} test_sm_t;

/*=============================================================================
 * Button Click Test
 *===========================================================================*/

static void button_client_setup(client_ctx_t* ctx) {
    ygui_widget_t* btn = ygui_button(ctx->engine, "btn", 50, 50, 100, 40, "Click");
    ygui_button_on_click(btn, client_track_click, ctx);
}

/*=============================================================================
 * Simple synchronous test approach using blocking reads
 *
 * For simplicity, let's use a different approach:
 * Run the test steps in a timer callback with explicit waits
 *===========================================================================*/

/*=============================================================================
 * Timer-driven State Machine Test Framework
 *===========================================================================*/

/* Global test context for timer callback access */
static terminal_ctx_t* g_test_ctx = NULL;
static test_sm_t g_test_sm;
static int g_assertion_pending = 0;

/* Generic PTY data handler */
static void generic_on_pty(terminal_ctx_t* ctx, const char* data, int len) {
    (void)ctx;
    (void)len;
    if (strstr(data, "]666666;") != NULL) {
        g_test_sm.render_count++;
    }
}

/* Generic control response handler */
static void generic_on_ctrl(terminal_ctx_t* ctx, const char* response) {
    if (g_assertion_pending) {
        if (strncmp(response, "TEST:PASS", 9) == 0) {
            /* Assertion passed, continue */
            g_assertion_pending = 0;
        } else if (strncmp(response, "TEST:FAIL", 9) == 0) {
            ctx->test_passed = 0;
            fprintf(stderr, "  Assertion failed: %s\n", response);
            g_assertion_pending = 0;
        } else if (strncmp(response, "TEST:ACK", 8) == 0) {
            g_assertion_pending = 0;
        }
    }
}

/* Timer callback for button click test */
static void button_test_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    /* Don't proceed if waiting for assertion response */
    if (g_assertion_pending) {
        return;
    }

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        /* Don't reset render_count - render may have already happened */
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        /* Send pixel size matching canvas (400x300) for 1:1 coordinate mapping */
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Mouse down */
            terminal_send_click(ctx, "test", 1, 1, 100.0f, 70.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            /* Mouse up */
            terminal_send_click(ctx, "test", 1, 0, 100.0f, 70.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            /* Ready to assert */
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void reset_test_state(void) {
    memset(&g_test_sm, 0, sizeof(g_test_sm));
    g_assertion_pending = 0;
    g_test_ctx = NULL;
}

static void button_click_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, button_test_timer_cb, 10, 10);
}

/*=============================================================================
 * Slider Drag Test
 *===========================================================================*/

static void slider_client_setup(client_ctx_t* ctx) {
    ygui_widget_t* sld = ygui_slider(ctx->engine, "slider", 50, 50, 200, 30, 0, 100, 50);
    ygui_slider_on_change(sld, client_track_change, ctx);
}

static void slider_test_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Press at center (50%) */
            terminal_send_click(ctx, "test", 1, 1, 150.0f, 65.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            /* Drag to 75% */
            terminal_send_move(ctx, "test", 1, 200.0f, 65.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            /* Release */
            terminal_send_click(ctx, "test", 1, 0, 200.0f, 65.0f);
            g_test_sm.step = 3;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 3 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
            g_test_sm.step = 0;
        }
        break;

    case STATE_ASSERT:
        if (g_test_sm.step == 0) {
            /* Assert change_count >= 2 (press + drag) */
            terminal_assert(ctx, "change_count", "2");
            g_assertion_pending = 1;
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            /* Assert value around 75 */
            terminal_assert(ctx, "last_value", "75");
            g_assertion_pending = 1;
            g_test_sm.state = STATE_CLEANUP;
        }
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void slider_drag_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, slider_test_timer_cb, 10, 10);
}

/*=============================================================================
 * Multiple Clicks Test
 *===========================================================================*/

static int g_expected_clicks = 0;

static void multi_click_client_setup(client_ctx_t* ctx) {
    ygui_widget_t* btn1 = ygui_button(ctx->engine, "btn1", 10, 10, 80, 30, "One");
    ygui_widget_t* btn2 = ygui_button(ctx->engine, "btn2", 100, 10, 80, 30, "Two");
    ygui_button_on_click(btn1, client_track_click, ctx);
    ygui_button_on_click(btn2, client_track_click, ctx);
}

static void multi_click_test_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        /* Click btn1 */
        if (g_test_sm.step == 0) {
            terminal_send_click(ctx, "test", 1, 1, 50.0f, 25.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 50.0f, 25.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        }
        /* Click btn2 */
        else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 1, 140.0f, 25.0f);
            g_test_sm.step = 3;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 3 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 140.0f, 25.0f);
            g_test_sm.step = 4;
            g_test_sm.render_count = 0;
        }
        /* Click btn1 again */
        else if (g_test_sm.step == 4 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 1, 50.0f, 25.0f);
            g_test_sm.step = 5;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 5 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 50.0f, 25.0f);
            g_test_sm.step = 6;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 6 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "3");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void multi_click_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, multi_click_test_timer_cb, 10, 10);
}

/*=============================================================================
 * Miss Click Test (click outside button)
 *===========================================================================*/

static void miss_click_test_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click outside button */
            terminal_send_click(ctx, "test", 1, 1, 10.0f, 10.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 10.0f, 10.0f);
            g_test_sm.step = 2;
            /* No render expected for miss, use a small delay */
        } else if (g_test_sm.step >= 2) {
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void miss_click_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, miss_click_test_timer_cb, 10, 10);
}

/*=============================================================================
 * Boundary Click Tests - Test exact hit detection boundaries
 *
 * Button at (50, 50) with size (100, 40)
 * Valid click area: x in [50, 150), y in [50, 90)
 *===========================================================================*/

/* Track which edge was hit */
static const char* g_last_clicked_widget = NULL;

static void boundary_track_click(ygui_widget_t* w, void* userdata) {
    client_ctx_t* ctx = (client_ctx_t*)userdata;
    ctx->click_count++;
    g_last_clicked_widget = ygui_widget_id(w);
}

static void boundary_client_setup(client_ctx_t* ctx) {
    /* Button at exact position (50, 50) with size (100, 40) */
    ygui_widget_t* btn = ygui_button(ctx->engine, "boundary_btn", 50, 50, 100, 40, "Test");
    ygui_button_on_click(btn, boundary_track_click, ctx);
}

/* Test: Click at left edge (x=50) - SHOULD HIT */
static void left_edge_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=50 (exact left edge), y=70 (middle) */
            terminal_send_click(ctx, "test", 1, 1, 50.0f, 70.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 50.0f, 70.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        /* Should have 1 click - left edge is inclusive */
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void left_edge_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, left_edge_hit_timer_cb, 10, 10);
}

/* Test: Click just left of button (x=49) - SHOULD MISS */
static void left_edge_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=49 (just outside left edge), y=70 (middle) */
            terminal_send_click(ctx, "test", 1, 1, 49.0f, 70.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 49.0f, 70.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            /* No render expected for miss - wait a few ticks */
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        /* Should have 0 clicks - x=49 is outside [50, 150) */
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void left_edge_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, left_edge_miss_timer_cb, 10, 10);
}

/* Test: Click at right edge (x=149, last valid pixel) - SHOULD HIT */
static void right_edge_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=149 (last valid pixel), y=70 (middle)
             * Button bounds: x in [50, 150), so x=149 is the last valid */
            terminal_send_click(ctx, "test", 1, 1, 149.0f, 70.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 149.0f, 70.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        /* Should have 1 click - x=149 is within [50, 150) */
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void right_edge_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, right_edge_hit_timer_cb, 10, 10);
}

/* Test: Click just right of button (x=150) - SHOULD MISS */
static void right_edge_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=150 (just outside right edge), y=70 (middle)
             * Button bounds: x in [50, 150), so x=150 is outside */
            terminal_send_click(ctx, "test", 1, 1, 150.0f, 70.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 150.0f, 70.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            /* No render expected for miss - wait a few ticks */
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        /* Should have 0 clicks - x=150 is outside [50, 150) */
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void right_edge_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, right_edge_miss_timer_cb, 10, 10);
}

/* Test: Click at top edge (y=50) - SHOULD HIT */
static void top_edge_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=100 (middle), y=50 (exact top edge) */
            terminal_send_click(ctx, "test", 1, 1, 100.0f, 50.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 100.0f, 50.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        /* Should have 1 click - y=50 is within [50, 90) */
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void top_edge_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, top_edge_hit_timer_cb, 10, 10);
}

/* Test: Click at bottom edge (y=89, last valid pixel) - SHOULD HIT */
static void bottom_edge_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=100 (middle), y=89 (last valid pixel)
             * Button bounds: y in [50, 90), so y=89 is the last valid */
            terminal_send_click(ctx, "test", 1, 1, 100.0f, 89.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 100.0f, 89.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        /* Should have 1 click - y=89 is within [50, 90) */
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void bottom_edge_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, bottom_edge_hit_timer_cb, 10, 10);
}

/* Test: Click just below button (y=90) - SHOULD MISS */
static void bottom_edge_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 400.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at x=100 (middle), y=90 (just outside bottom edge)
             * Button bounds: y in [50, 90), so y=90 is outside */
            terminal_send_click(ctx, "test", 1, 1, 100.0f, 90.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 100.0f, 90.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            /* No render expected for miss - wait a few ticks */
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        /* Should have 0 clicks - y=90 is outside [50, 90) */
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void bottom_edge_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, bottom_edge_miss_timer_cb, 10, 10);
}

/*=============================================================================
 * Scaled Coordinate Tests
 *
 * These tests simulate the real terminal scenario where:
 * - Canvas size != display size (coordinates need scaling)
 * - Terminal sends cell size via CSI response
 * - Clicks come in display coordinates and get transformed to canvas
 *
 * Setup:
 * - Canvas: 400 x 300 (widget coordinates)
 * - Card: 40 x 20 cells
 * - Cell size: 8 x 15 pixels
 * - Display: 40*8=320 x 20*15=300 (pixel dimensions)
 *
 * Button at canvas (50, 50) with size (100, 40)
 * Canvas bounds: x in [50, 150), y in [50, 90)
 *
 * Display to canvas transform: canvas_x = (display_x / 320) * 400
 *
 * Button in display space:
 * - Left edge: 50 * 320/400 = 40
 * - Right edge: 150 * 320/400 = 120
 * - Top edge: 50 * 300/300 = 50
 * - Bottom edge: 90 * 300/300 = 90
 *===========================================================================*/

static void scaled_client_setup(client_ctx_t* ctx) {
    /* Set card dimensions for scaling (normally set by show()) */
    ygui_engine_set_card_size(ctx->engine, 40, 20);

    /* Button at canvas (50, 50) with size (100, 40) */
    ygui_widget_t* btn = ygui_button(ctx->engine, "scaled_btn", 50, 50, 100, 40, "Test");
    ygui_button_on_click(btn, boundary_track_click, ctx);
}

/* Scaled test: Click at display left edge (x=40) - maps to canvas x=50 - SHOULD HIT */
static void scaled_left_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        /* Display is 320x300 (40*8 x 20*15), canvas is 400x300 */
        terminal_send_pixel_size(ctx, "test", 320.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=40, which maps to canvas x=50 (left edge)
             * canvas_x = (40/320)*400 = 50 */
            terminal_send_click(ctx, "test", 1, 1, 40.0f, 70.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 40.0f, 70.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void scaled_left_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, scaled_left_hit_timer_cb, 10, 10);
}

/* Scaled test: Click at display x=39 - maps to canvas x=48.75 - SHOULD MISS */
static void scaled_left_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 320.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=39, which maps to canvas x=48.75 (outside)
             * canvas_x = (39/320)*400 = 48.75 < 50 */
            terminal_send_click(ctx, "test", 1, 1, 39.0f, 70.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 39.0f, 70.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void scaled_left_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, scaled_left_miss_timer_cb, 10, 10);
}

/* Scaled test: Click at display x=119 - maps to canvas x=148.75 - SHOULD HIT */
static void scaled_right_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 320.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=119, which maps to canvas x=148.75 (inside)
             * canvas_x = (119/320)*400 = 148.75 < 150 */
            terminal_send_click(ctx, "test", 1, 1, 119.0f, 70.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 119.0f, 70.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void scaled_right_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, scaled_right_hit_timer_cb, 10, 10);
}

/* Scaled test: Click at display x=120 - maps to canvas x=150 - SHOULD MISS */
static void scaled_right_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 320.0f, 300.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=120, which maps to canvas x=150 (outside)
             * canvas_x = (120/320)*400 = 150 >= 150 */
            terminal_send_click(ctx, "test", 1, 1, 120.0f, 70.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 120.0f, 70.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void scaled_right_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, scaled_right_miss_timer_cb, 10, 10);
}

/*=============================================================================
 * Real Demo Scenario Test (CANVAS_FIT + SCALE_ON)
 *
 * Simulates the exact scenario from 00-debug-events.py:
 * - Canvas: 500 x 300
 * - Card: 62 x 18 cells
 * - Cell size: 8 x 16 pixels
 * - Display: 62*8=496 x 18*16=288 pixels
 *
 * With CANVAS_FIT + SCALE_ON:
 * - Canvas resizes to 496 x 288
 * - Widgets scale by (496/500, 288/300) = (0.992, 0.96)
 * - Button at (20, 50) scales to (19.84, 48) with size (198.4, 48)
 * - Button right edge: 19.84 + 198.4 = 218.24
 *
 * This test catches the bug where handle_resize() wasn't called on
 * first cell size response, causing coordinate mismatch.
 *===========================================================================*/

static void demo_client_setup(client_ctx_t* ctx) {
    /* Set canvas to 500x300 to match the real demo (00-debug-events.py) */
    ygui_engine_set_size(ctx->engine, 500, 300);

    /* Match the real demo: card 62x18 cells */
    ygui_engine_set_card_size(ctx->engine, 62, 18);

    /* Enable CANVAS_FIT + SCALE_ON for 1:1 coordinate mapping */
    ygui_engine_set_canvas_mode(ctx->engine, YGUI_CANVAS_FIT);
    ygui_engine_set_scale_mode(ctx->engine, YGUI_SCALE_ON);

    /* Button at same position as 00-debug-events.py */
    ygui_widget_t* btn = ygui_button(ctx->engine, "demo_btn", 20, 50, 200, 50, "Click Me!");
    ygui_button_on_click(btn, boundary_track_click, ctx);
}

/* Test: Click at last pixel inside right edge (display x=217) - SHOULD HIT
 * After scaling with floorf(): button bounds = [19, 218)
 * Display x=217 -> canvas x=217 (CANVAS_FIT is 1:1)
 * 217 is inside [19, 218), so HIT */
static void demo_right_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        /* 62*8=496, 18*16=288 - matches demo scenario */
        terminal_send_pixel_size(ctx, "test", 496.0f, 288.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=217, y=72 (last pixel inside button)
             * Button after scaling with floorf(): x=[19, 218), y=[48, 96)
             * Display 217 is inside [19, 218), so should HIT */
            terminal_send_click(ctx, "test", 1, 1, 217.0f, 72.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 217.0f, 72.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void demo_right_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, demo_right_hit_timer_cb, 10, 10);
}

/* Test: Click outside right edge (display x=219) - SHOULD MISS
 * After scaling: button at (19.84, 48) with size (198.4, 48), right edge = 218.24
 * Display x=219 -> canvas x=219 (CANVAS_FIT is 1:1)
 * 219 >= 218.24, so MISS */
static void demo_right_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 496.0f, 288.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=219 - outside button right edge (218.24) */
            terminal_send_click(ctx, "test", 1, 1, 219.0f, 72.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 219.0f, 72.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void demo_right_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, demo_right_miss_timer_cb, 10, 10);
}

/* Test: Click inside left edge (display x=20) - SHOULD HIT
 * After scaling: button at (19.84, 48), left edge = 19.84
 * Display x=20 -> canvas x=20 (CANVAS_FIT is 1:1)
 * 20 >= 19.84, so HIT */
static void demo_left_hit_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 496.0f, 288.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=20 (inside left edge 19.84) */
            terminal_send_click(ctx, "test", 1, 1, 20.0f, 72.0f);
            g_test_sm.step = 1;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 1 && g_test_sm.render_count >= 1) {
            terminal_send_click(ctx, "test", 1, 0, 20.0f, 72.0f);
            g_test_sm.step = 2;
            g_test_sm.render_count = 0;
        } else if (g_test_sm.step == 2 && g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_ASSERT;
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "1");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void demo_left_hit_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, demo_left_hit_timer_cb, 10, 10);
}

/* Test: Click outside left edge (display x=19) - SHOULD MISS
 * After scaling: button at (19.84, 48), left edge = 19.84
 * Display x=19 -> canvas x=19 (CANVAS_FIT is 1:1)
 * 19 < 19.84, so MISS */
static void demo_left_miss_timer_cb(uv_timer_t* timer) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)timer->data;

    if (g_assertion_pending) return;

    switch (g_test_sm.state) {
    case STATE_INIT:
        g_test_sm.state = STATE_WAIT_RENDER;
        break;

    case STATE_WAIT_RENDER:
        if (g_test_sm.render_count >= 1) {
            g_test_sm.state = STATE_SEND_PIXEL_SIZE;
        }
        break;

    case STATE_SEND_PIXEL_SIZE:
        terminal_send_pixel_size(ctx, "test", 496.0f, 288.0f);
        g_test_sm.state = STATE_SEND_EVENT;
        g_test_sm.step = 0;
        break;

    case STATE_SEND_EVENT:
        if (g_test_sm.step == 0) {
            /* Click at display x=19 (outside left edge 19.84) */
            terminal_send_click(ctx, "test", 1, 1, 19.0f, 72.0f);
            g_test_sm.step = 1;
        } else if (g_test_sm.step == 1) {
            terminal_send_click(ctx, "test", 1, 0, 19.0f, 72.0f);
            g_test_sm.step = 2;
        } else if (g_test_sm.step >= 2) {
            g_test_sm.step++;
            if (g_test_sm.step > 5) {
                g_test_sm.state = STATE_ASSERT;
            }
        }
        break;

    case STATE_ASSERT:
        terminal_assert(ctx, "click_count", "0");
        g_assertion_pending = 1;
        g_test_sm.state = STATE_CLEANUP;
        break;

    case STATE_CLEANUP:
        terminal_shutdown(ctx);
        g_assertion_pending = 1;
        g_test_sm.state = STATE_DONE;
        break;

    case STATE_DONE:
        uv_timer_stop(timer);
        ctx->test_done = 1;
        break;

    default:
        break;
    }
}

static void demo_left_miss_terminal_test(terminal_ctx_t* ctx) {
    ctx->on_pty_data = generic_on_pty;
    ctx->on_ctrl_response = generic_on_ctrl;

    reset_test_state();
    g_test_ctx = ctx;

    uv_timer_t* timer = (uv_timer_t*)malloc(sizeof(uv_timer_t));
    uv_timer_init(ctx->loop, timer);
    timer->data = ctx;
    uv_timer_start(timer, demo_left_miss_timer_cb, 10, 10);
}

/*=============================================================================
 * Main
 *===========================================================================*/

int main(void) {
    test_scenario_t scenarios[] = {
        {"button_click", button_client_setup, button_click_terminal_test},
        {"button_miss", button_client_setup, miss_click_terminal_test},
        {"multi_click", multi_click_client_setup, multi_click_terminal_test},
        {"slider_drag", slider_client_setup, slider_drag_terminal_test},
        /* Boundary click tests (direct canvas coordinates) */
        {"left_edge_hit", boundary_client_setup, left_edge_hit_terminal_test},
        {"left_edge_miss", boundary_client_setup, left_edge_miss_terminal_test},
        {"right_edge_hit", boundary_client_setup, right_edge_hit_terminal_test},
        {"right_edge_miss", boundary_client_setup, right_edge_miss_terminal_test},
        {"top_edge_hit", boundary_client_setup, top_edge_hit_terminal_test},
        {"bottom_edge_hit", boundary_client_setup, bottom_edge_hit_terminal_test},
        {"bottom_edge_miss", boundary_client_setup, bottom_edge_miss_terminal_test},
        /* Scaled coordinate tests (display -> canvas transformation) */
        {"scaled_left_hit", scaled_client_setup, scaled_left_hit_terminal_test},
        {"scaled_left_miss", scaled_client_setup, scaled_left_miss_terminal_test},
        {"scaled_right_hit", scaled_client_setup, scaled_right_hit_terminal_test},
        {"scaled_right_miss", scaled_client_setup, scaled_right_miss_terminal_test},
        /* Demo scenario tests (CANVAS_FIT + SCALE_ON, 1:1 coordinates) */
        {"demo_right_hit", demo_client_setup, demo_right_hit_terminal_test},
        {"demo_right_miss", demo_client_setup, demo_right_miss_terminal_test},
        {"demo_left_hit", demo_client_setup, demo_left_hit_terminal_test},
        {"demo_left_miss", demo_client_setup, demo_left_miss_terminal_test},
    };

    int count = sizeof(scenarios) / sizeof(scenarios[0]);
    return run_all_scenarios(scenarios, count);
}
