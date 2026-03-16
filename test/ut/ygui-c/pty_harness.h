/*
 * pty_harness.h - PTY-based test framework for ygui-c
 *
 * Creates a PTY pair to simulate terminal interactions:
 * - Master end: Test code writes OSC sequences, reads output
 * - Slave end: Engine reads input via libuv
 *
 * This tests the full libuv integration path.
 */

#ifndef YGUI_PTY_HARNESS_H
#define YGUI_PTY_HARNESS_H

#include "test_harness.h"
#include "../../../src/yetty/ygui-c/ygui.h"
#include "../../../src/yetty/ygui-c/ygui_internal.h"
#include <pty.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <sys/select.h>
#include <errno.h>

/*=============================================================================
 * PTY Test Context
 *===========================================================================*/

typedef struct {
    int master_fd;           /* Master end - test controls this */
    int slave_fd;            /* Slave end - engine reads from this */
    ygui_engine_t* engine;   /* Engine under test */
    uv_loop_t* loop;         /* libuv loop */
} pty_test_ctx_t;

/*=============================================================================
 * PTY Setup/Teardown
 *===========================================================================*/

/* Create PTY pair and engine connected to it */
static inline int pty_test_init(pty_test_ctx_t* ctx, const char* name,
                                 float width, float height) {
    memset(ctx, 0, sizeof(*ctx));

    /* Create PTY pair */
    if (openpty(&ctx->master_fd, &ctx->slave_fd, NULL, NULL, NULL) < 0) {
        printf("  FAIL: openpty failed: %s\n", strerror(errno));
        return -1;
    }

    /* Set slave to raw mode so data flows through immediately */
    struct termios raw;
    if (tcgetattr(ctx->slave_fd, &raw) == 0) {
        cfmakeraw(&raw);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(ctx->slave_fd, TCSANOW, &raw);
    }

    /* Make master non-blocking for reads */
    int flags = fcntl(ctx->master_fd, F_GETFL, 0);
    fcntl(ctx->master_fd, F_SETFL, flags | O_NONBLOCK);

    /* Create engine (without terminal init - we have our own PTY) */
    int cols = (int)(width / 10);   /* Approximate cell size 10x16 */
    int rows = (int)(height / 16);
    ctx->engine = ygui_engine_create(name, 0, 0, cols > 0 ? cols : 1, rows > 0 ? rows : 1);
    if (!ctx->engine) {
        close(ctx->master_fd);
        close(ctx->slave_fd);
        printf("  FAIL: ygui_engine_create failed\n");
        return -1;
    }
    /* Set display pixel size directly for testing */
    ygui_engine_set_display_pixel_size(ctx->engine, width, height);

    /* Set engine to use our PTY slave for input */
    ygui_engine_set_input_fd(ctx->engine, ctx->slave_fd);

    /* Create and attach libuv loop */
    ctx->loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_loop_init(ctx->loop);
    ygui_engine_attach(ctx->engine, ctx->loop);

    return 0;
}

/* Cleanup PTY and engine */
static inline void pty_test_cleanup(pty_test_ctx_t* ctx) {
    if (ctx->engine) {
        ygui_engine_destroy(ctx->engine);
        ctx->engine = NULL;
    }
    if (ctx->loop) {
        uv_loop_close(ctx->loop);
        free(ctx->loop);
        ctx->loop = NULL;
    }
    if (ctx->master_fd > 0) {
        close(ctx->master_fd);
        ctx->master_fd = 0;
    }
    if (ctx->slave_fd > 0) {
        close(ctx->slave_fd);
        ctx->slave_fd = 0;
    }
}

/*=============================================================================
 * PTY Write Helpers
 *===========================================================================*/

/* Write raw data to master (simulates terminal sending data to app) */
static inline int pty_write(pty_test_ctx_t* ctx, const char* data, size_t len) {
    ssize_t written = write(ctx->master_fd, data, len);
    if (written < 0) {
        printf("  FAIL: pty_write failed: %s\n", strerror(errno));
        return -1;
    }
    return (int)written;
}

/* Write null-terminated string */
static inline int pty_write_str(pty_test_ctx_t* ctx, const char* str) {
    return pty_write(ctx, str, strlen(str));
}

/* Send OSC 777777 click event */
static inline int pty_send_click(pty_test_ctx_t* ctx, const char* card_name,
                                  int buttons, int press, int x, int y) {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "\033]777777;%s;%d;%d;%d;%d\033\\",
                       card_name, buttons, press, x, y);
    return pty_write(ctx, buf, len);
}

/* Send OSC 777778 move event */
static inline int pty_send_move(pty_test_ctx_t* ctx, const char* card_name,
                                 int buttons, int x, int y) {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "\033]777778;%s;%d;%d;%d\033\\",
                       card_name, buttons, x, y);
    return pty_write(ctx, buf, len);
}

/* Send OSC 777779 view change event */
static inline int pty_send_view(pty_test_ctx_t* ctx, const char* card_name,
                                 float zoom, float scroll_x, float scroll_y) {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "\033]777779;%s;%.2f;%.0f;%.0f\033\\",
                       card_name, zoom, scroll_x, scroll_y);
    return pty_write(ctx, buf, len);
}

/* Send CSI cell size response */
static inline int pty_send_cell_size(pty_test_ctx_t* ctx, int height, int width) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "\033[6;%d;%dt", height, width);
    return pty_write(ctx, buf, len);
}

/*=============================================================================
 * Event Loop Helpers
 *===========================================================================*/

/* Small delay to let PTY data propagate */
static inline void pty_sync(void) {
    struct timespec ts = {0, 1000000};  /* 1ms */
    nanosleep(&ts, NULL);
}

/* Wait for slave fd to be readable (data available from master) */
static inline int pty_wait_slave_readable(pty_test_ctx_t* ctx, int timeout_ms) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(ctx->slave_fd, &fds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    return select(ctx->slave_fd + 1, &fds, NULL, NULL, &tv);
}

/* Run loop once - waits for and processes one event */
static inline int pty_run_once(pty_test_ctx_t* ctx) {
    return uv_run(ctx->loop, UV_RUN_ONCE);
}

/* Run loop for N iterations (processing available events) */
static inline void pty_run_iterations(pty_test_ctx_t* ctx, int iterations) {
    /* Small delay to let PTY data propagate through kernel */
    pty_sync();

    for (int i = 0; i < iterations; i++) {
        /* UV_RUN_ONCE will wait for and process one event */
        int result = uv_run(ctx->loop, UV_RUN_ONCE);
        if (result == 0) break;  /* No more pending handles/requests */
        pty_sync();  /* Small delay between iterations */
    }
}

/* Run loop with timeout until events are processed */
static inline void pty_run_with_timeout(pty_test_ctx_t* ctx, int timeout_ms) {
    int elapsed = 0;
    int step_ms = 5;

    while (elapsed < timeout_ms) {
        int result = uv_run(ctx->loop, UV_RUN_NOWAIT);
        if (result == 0) break;  /* No more events */

        struct timespec ts = {0, step_ms * 1000000};
        nanosleep(&ts, NULL);
        elapsed += step_ms;
    }
}

/* Run loop until no more pending events or max iterations reached */
static inline int pty_run_until_idle(pty_test_ctx_t* ctx, int max_iterations) {
    pty_sync();
    int i = 0;
    while (i < max_iterations && uv_run(ctx->loop, UV_RUN_NOWAIT)) {
        i++;
        pty_sync();
    }
    return i;
}

/* Wait for data to arrive with timeout (milliseconds) */
static inline int pty_wait_readable(pty_test_ctx_t* ctx, int timeout_ms) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(ctx->master_fd, &fds);

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    return select(ctx->master_fd + 1, &fds, NULL, NULL, &tv);
}

/*=============================================================================
 * Grid Rebuild Helper
 *===========================================================================*/

/* Helper to rebuild spatial grid after adding widgets */
static inline void pty_rebuild_grid(pty_test_ctx_t* ctx) {
    ygui_engine_t* e = ctx->engine;
    if (!e) return;

    /* Clear grid */
    ygui_grid_clear(&e->grid);

    /* Insert all top-level widgets */
    for (ygui_widget_t* w = e->first_widget; w; w = w->next_sibling) {
        w->effective_x = w->x;
        w->effective_y = w->y;
        w->was_rendered = 1;
        ygui_grid_insert(&e->grid, w);
    }
}

#endif /* YGUI_PTY_HARNESS_H */
