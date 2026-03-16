/*
 * dual_process_harness.h - Dual-process test framework for ygui-c
 *
 * Emulates real terminal↔client architecture:
 * - Terminal emulator process (test controller) with libuv loop
 * - YGui-C client process with libuv loop
 * - PTY for normal OSC/CSI communication
 * - Control pipe for test synchronization and assertions
 *
 * Both processes run libuv continuously - no artificial timing.
 * Synchronization happens through explicit protocol messages.
 */

#ifndef YGUI_DUAL_PROCESS_HARNESS_H
#define YGUI_DUAL_PROCESS_HARNESS_H

#include "../../../src/yetty/ygui-c/ygui.h"
#include "../../../src/yetty/ygui-c/ygui_internal.h"
#include <uv.h>
#include <pty.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

/*=============================================================================
 * Control Protocol
 *===========================================================================*/

#define CTRL_SYNC          "TEST:SYNC\n"
#define CTRL_ACK           "TEST:ACK\n"
#define CTRL_QUERY_PREFIX  "TEST:QUERY:"
#define CTRL_REPLY_PREFIX  "TEST:REPLY:"
#define CTRL_ASSERT_PREFIX "TEST:ASSERT:"
#define CTRL_PASS          "TEST:PASS\n"
#define CTRL_FAIL_PREFIX   "TEST:FAIL:"
#define CTRL_SHUTDOWN      "TEST:SHUTDOWN\n"

/*=============================================================================
 * Client Side - Runs in child process
 *===========================================================================*/

typedef struct {
    /* Engine */
    ygui_engine_t* engine;

    /* Control pipe (read commands, write responses) */
    int ctrl_read_fd;
    int ctrl_write_fd;
    uv_poll_t ctrl_poll;

    /* Control input buffer */
    char ctrl_buf[1024];
    int ctrl_len;

    /* Tracked state for test queries */
    int click_count;
    int change_count;
    int check_count;
    float last_value;
    int last_checked;
    const char* last_widget_id;

    /* Running flag */
    int running;
} client_ctx_t;

/* Forward declarations */
static void client_ctrl_cb(uv_poll_t* handle, int status, int events);
static void client_track_click(ygui_widget_t* w, void* userdata);
static void client_track_change(ygui_widget_t* w, float value, void* userdata);
static void client_track_check(ygui_widget_t* w, int checked, void* userdata);

/* Process a control command and send response */
static void client_process_ctrl(client_ctx_t* ctx, const char* cmd) {
    char response[256];

    if (strcmp(cmd, "SYNC") == 0) {
        write(ctx->ctrl_write_fd, CTRL_ACK, strlen(CTRL_ACK));
    }
    else if (strcmp(cmd, "SHUTDOWN") == 0) {
        ctx->running = 0;
        write(ctx->ctrl_write_fd, CTRL_ACK, strlen(CTRL_ACK));
    }
    else if (strncmp(cmd, "QUERY:", 6) == 0) {
        const char* query = cmd + 6;

        if (strcmp(query, "click_count") == 0) {
            snprintf(response, sizeof(response), "%s%s=%d\n",
                     CTRL_REPLY_PREFIX, query, ctx->click_count);
        }
        else if (strcmp(query, "change_count") == 0) {
            snprintf(response, sizeof(response), "%s%s=%d\n",
                     CTRL_REPLY_PREFIX, query, ctx->change_count);
        }
        else if (strcmp(query, "check_count") == 0) {
            snprintf(response, sizeof(response), "%s%s=%d\n",
                     CTRL_REPLY_PREFIX, query, ctx->check_count);
        }
        else if (strcmp(query, "last_value") == 0) {
            snprintf(response, sizeof(response), "%s%s=%.2f\n",
                     CTRL_REPLY_PREFIX, query, ctx->last_value);
        }
        else if (strcmp(query, "last_checked") == 0) {
            snprintf(response, sizeof(response), "%s%s=%d\n",
                     CTRL_REPLY_PREFIX, query, ctx->last_checked);
        }
        else if (strcmp(query, "last_widget_id") == 0) {
            snprintf(response, sizeof(response), "%s%s=%s\n",
                     CTRL_REPLY_PREFIX, query,
                     ctx->last_widget_id ? ctx->last_widget_id : "null");
        }
        else {
            snprintf(response, sizeof(response), "%sunknown_query\n", CTRL_REPLY_PREFIX);
        }
        write(ctx->ctrl_write_fd, response, strlen(response));
    }
    else if (strncmp(cmd, "ASSERT:", 7) == 0) {
        const char* assertion = cmd + 7;
        char key[64], expected[64];
        int actual_int;
        float actual_float;
        int pass = 0;

        /* Parse "key=value" */
        if (sscanf(assertion, "%63[^=]=%63s", key, expected) == 2) {
            if (strcmp(key, "click_count") == 0) {
                actual_int = ctx->click_count;
                pass = (actual_int == atoi(expected));
                if (!pass) {
                    snprintf(response, sizeof(response), "%sgot=%d,expected=%s\n",
                             CTRL_FAIL_PREFIX, actual_int, expected);
                }
            }
            else if (strcmp(key, "change_count") == 0) {
                actual_int = ctx->change_count;
                pass = (actual_int == atoi(expected));
                if (!pass) {
                    snprintf(response, sizeof(response), "%sgot=%d,expected=%s\n",
                             CTRL_FAIL_PREFIX, actual_int, expected);
                }
            }
            else if (strcmp(key, "last_value") == 0) {
                actual_float = ctx->last_value;
                float exp_float = atof(expected);
                pass = (actual_float >= exp_float - 1.0f && actual_float <= exp_float + 1.0f);
                if (!pass) {
                    snprintf(response, sizeof(response), "%sgot=%.2f,expected=%s\n",
                             CTRL_FAIL_PREFIX, actual_float, expected);
                }
            }
            /* Add more assertions as needed */
        }

        if (pass) {
            write(ctx->ctrl_write_fd, CTRL_PASS, strlen(CTRL_PASS));
        } else {
            write(ctx->ctrl_write_fd, response, strlen(response));
        }
    }
}

/* Control pipe read callback */
static void client_ctrl_cb(uv_poll_t* handle, int status, int events) {
    client_ctx_t* ctx = (client_ctx_t*)handle->data;
    if (status < 0 || !(events & UV_READABLE)) return;

    char buf[256];
    ssize_t n = read(ctx->ctrl_read_fd, buf, sizeof(buf) - 1);
    if (n <= 0) {
        ctx->running = 0;
        return;
    }
    buf[n] = '\0';

    /* Append to buffer and process complete lines */
    if (ctx->ctrl_len + n < (int)sizeof(ctx->ctrl_buf)) {
        memcpy(ctx->ctrl_buf + ctx->ctrl_len, buf, n);
        ctx->ctrl_len += n;
    }

    /* Process complete lines */
    char* line_start = ctx->ctrl_buf;
    char* newline;
    while ((newline = strchr(line_start, '\n')) != NULL) {
        *newline = '\0';

        /* Skip "TEST:" prefix if present */
        char* cmd = line_start;
        if (strncmp(cmd, "TEST:", 5) == 0) {
            cmd += 5;
        }

        client_process_ctrl(ctx, cmd);
        line_start = newline + 1;
    }

    /* Move remaining partial line to start of buffer */
    int remaining = ctx->ctrl_len - (line_start - ctx->ctrl_buf);
    if (remaining > 0) {
        memmove(ctx->ctrl_buf, line_start, remaining);
    }
    ctx->ctrl_len = remaining;
}

/* Callback trackers */
static void client_track_click(ygui_widget_t* w, void* userdata) {
    client_ctx_t* ctx = (client_ctx_t*)userdata;
    ctx->click_count++;
    ctx->last_widget_id = ygui_widget_id(w);
}

static void client_track_change(ygui_widget_t* w, float value, void* userdata) {
    client_ctx_t* ctx = (client_ctx_t*)userdata;
    ctx->change_count++;
    ctx->last_value = value;
    ctx->last_widget_id = ygui_widget_id(w);
}

static void client_track_check(ygui_widget_t* w, int checked, void* userdata) {
    client_ctx_t* ctx = (client_ctx_t*)userdata;
    ctx->check_count++;
    ctx->last_checked = checked;
    ctx->last_widget_id = ygui_widget_id(w);
}

/*=============================================================================
 * Terminal Side - Runs in parent process
 *===========================================================================*/

/* Forward declaration for callback types */
typedef struct terminal_ctx terminal_ctx_t;

struct terminal_ctx {
    /* PTY master fd */
    int pty_master;
    uv_poll_t pty_poll;

    /* Control pipe (write commands, read responses) */
    int ctrl_write_fd;
    int ctrl_read_fd;
    uv_poll_t ctrl_poll;

    /* Response buffer */
    char response_buf[1024];
    int response_len;

    /* Callback for PTY data received (render output from client) */
    void (*on_pty_data)(terminal_ctx_t* ctx, const char* data, int len);

    /* Callback for control response received */
    void (*on_ctrl_response)(terminal_ctx_t* ctx, const char* response);

    /* libuv loop */
    uv_loop_t* loop;

    /* Test state */
    int test_passed;
    int test_done;
    void* userdata;

    /* Child PID */
    pid_t child_pid;
};

/* Forward declarations */
static void terminal_pty_cb(uv_poll_t* handle, int status, int events);
static void terminal_ctrl_cb(uv_poll_t* handle, int status, int events);

/*=============================================================================
 * Test Scenario Definition
 *===========================================================================*/

/* Function pointer types for test scenarios */
typedef void (*client_setup_fn)(client_ctx_t* ctx);
typedef void (*terminal_test_fn)(terminal_ctx_t* ctx);

typedef struct {
    const char* name;
    client_setup_fn client_setup;    /* Called in client to set up widgets */
    terminal_test_fn terminal_test;  /* Called in terminal to run test */
} test_scenario_t;

/*=============================================================================
 * Helper Functions for Terminal Side
 *===========================================================================*/

/* Send OSC click event via PTY */
static void terminal_send_click(terminal_ctx_t* ctx, const char* card_name,
                                 int buttons, int press, int x, int y) {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "\033]777777;%s;%d;%d;%d;%d\033\\",
                       card_name, buttons, press, x, y);
    write(ctx->pty_master, buf, len);
}

/* Send OSC move event via PTY */
static void terminal_send_move(terminal_ctx_t* ctx, const char* card_name,
                                int buttons, int x, int y) {
    char buf[256];
    int len = snprintf(buf, sizeof(buf), "\033]777778;%s;%d;%d;%d\033\\",
                       card_name, buttons, x, y);
    write(ctx->pty_master, buf, len);
}

/* Send CSI cell size response via PTY
 * Format: ESC [ 6 ; height ; width t
 * This enables coordinate scaling in the client */
static void terminal_send_cell_size(terminal_ctx_t* ctx, int cell_height, int cell_width) {
    char buf[64];
    int len = snprintf(buf, sizeof(buf), "\033[6;%d;%dt", cell_height, cell_width);
    write(ctx->pty_master, buf, len);
}

/* Send OSC 777780 card pixel size response via PTY
 * Format: ESC ] 777780 ; cardname ; width ; height ST
 * This is the PRIMARY source for coordinate transformation */
static void terminal_send_pixel_size(terminal_ctx_t* ctx, const char* card_name, float width, float height) {
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "\033]777780;%s;%.6f;%.6f\033\\", card_name, width, height);
    write(ctx->pty_master, buf, len);
}

/* Send control command and wait for response synchronously
 * NOTE: This blocks until response is received - call from timer/idle callback */
static void terminal_send_ctrl(terminal_ctx_t* ctx, const char* cmd) {
    write(ctx->ctrl_write_fd, cmd, strlen(cmd));
}

/* Synchronize with client - blocks until ACK received */
static void terminal_sync(terminal_ctx_t* ctx) {
    terminal_send_ctrl(ctx, CTRL_SYNC);
}

/* Request shutdown of client */
static void terminal_shutdown(terminal_ctx_t* ctx) {
    terminal_send_ctrl(ctx, CTRL_SHUTDOWN);
}

/* Query a value from client */
static void terminal_query(terminal_ctx_t* ctx, const char* key) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%s%s\n", CTRL_QUERY_PREFIX, key);
    terminal_send_ctrl(ctx, buf);
}

/* Assert a value on client side */
static void terminal_assert(terminal_ctx_t* ctx, const char* key, const char* expected) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%s%s=%s\n", CTRL_ASSERT_PREFIX, key, expected);
    terminal_send_ctrl(ctx, buf);
}

/* PTY data callback - client rendered something */
static void terminal_pty_cb(uv_poll_t* handle, int status, int events) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)handle->data;
    if (status < 0 || !(events & UV_READABLE)) return;

    char buf[4096];
    ssize_t n = read(ctx->pty_master, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        if (ctx->on_pty_data) {
            ctx->on_pty_data(ctx, buf, n);
        }
    }
}

/* Control response callback */
static void terminal_ctrl_cb(uv_poll_t* handle, int status, int events) {
    terminal_ctx_t* ctx = (terminal_ctx_t*)handle->data;
    if (status < 0 || !(events & UV_READABLE)) return;

    char buf[256];
    ssize_t n = read(ctx->ctrl_read_fd, buf, sizeof(buf) - 1);
    if (n <= 0) return;
    buf[n] = '\0';

    /* Append to buffer */
    if (ctx->response_len + n < (int)sizeof(ctx->response_buf)) {
        memcpy(ctx->response_buf + ctx->response_len, buf, n);
        ctx->response_len += n;
    }

    /* Process complete lines */
    char* line_start = ctx->response_buf;
    char* newline;
    while ((newline = strchr(line_start, '\n')) != NULL) {
        *newline = '\0';

        if (ctx->on_ctrl_response) {
            ctx->on_ctrl_response(ctx, line_start);
        }
        line_start = newline + 1;
    }

    /* Move remaining to start */
    int remaining = ctx->response_len - (line_start - ctx->response_buf);
    if (remaining > 0) {
        memmove(ctx->response_buf, line_start, remaining);
    }
    ctx->response_len = remaining;
}

/*=============================================================================
 * Test Runner
 *===========================================================================*/

/* Run a single test scenario - returns 0 on pass, 1 on fail */
static int run_test_scenario(test_scenario_t* scenario) {
    int pty_master, pty_slave;
    int ctrl_to_client[2];   /* terminal writes, client reads */
    int ctrl_to_terminal[2]; /* client writes, terminal reads */

    /* Create PTY */
    if (openpty(&pty_master, &pty_slave, NULL, NULL, NULL) < 0) {
        fprintf(stderr, "FAIL: %s - openpty failed\n", scenario->name);
        return 1;
    }

    /* Set PTY to raw mode */
    struct termios raw;
    tcgetattr(pty_slave, &raw);
    cfmakeraw(&raw);
    tcsetattr(pty_slave, TCSANOW, &raw);

    /* Create control pipes */
    if (pipe(ctrl_to_client) < 0 || pipe(ctrl_to_terminal) < 0) {
        fprintf(stderr, "FAIL: %s - pipe failed\n", scenario->name);
        close(pty_master);
        close(pty_slave);
        return 1;
    }

    /* Fork */
    pid_t pid = fork();

    if (pid == 0) {
        /*=== CHILD PROCESS (Client) ===*/
        close(pty_master);
        close(ctrl_to_client[1]);  /* Close write end */
        close(ctrl_to_terminal[0]); /* Close read end */

        /* Redirect stdin/stdout to PTY slave
         * This makes the child behave like a real terminal application */
        dup2(pty_slave, STDIN_FILENO);
        dup2(pty_slave, STDOUT_FILENO);
        if (pty_slave > STDERR_FILENO) {
            close(pty_slave);  /* Close original if not needed */
        }

        /* Initialize client context */
        client_ctx_t ctx = {0};
        ctx.ctrl_read_fd = ctrl_to_client[0];
        ctx.ctrl_write_fd = ctrl_to_terminal[1];
        ctx.running = 1;

        /* Create engine - will use stdin/stdout (now PTY) */
        ctx.engine = ygui_engine_create("test", 400, 300);
        /* input_fd/output_fd default to STDIN/STDOUT which are now the PTY */

        /* Run client setup (creates widgets, sets callbacks) */
        if (scenario->client_setup) {
            scenario->client_setup(&ctx);
        }

        /* Create libuv loop */
        uv_loop_t loop;
        uv_loop_init(&loop);

        /* Attach engine to loop */
        ygui_engine_attach(ctx.engine, &loop);

        /* Add control pipe to loop */
        uv_poll_init(&loop, &ctx.ctrl_poll, ctx.ctrl_read_fd);
        ctx.ctrl_poll.data = &ctx;
        uv_poll_start(&ctx.ctrl_poll, UV_READABLE, client_ctrl_cb);

        /* Run until shutdown */
        while (ctx.running) {
            uv_run(&loop, UV_RUN_ONCE);
        }

        /* Cleanup */
        uv_poll_stop(&ctx.ctrl_poll);
        ygui_engine_destroy(ctx.engine);
        uv_loop_close(&loop);
        close(pty_slave);
        close(ctx.ctrl_read_fd);
        close(ctx.ctrl_write_fd);

        _exit(0);
    }

    /*=== PARENT PROCESS (Terminal) ===*/
    close(pty_slave);
    close(ctrl_to_client[0]);   /* Close read end */
    close(ctrl_to_terminal[1]); /* Close write end */

    /* Make PTY master non-blocking */
    int flags = fcntl(pty_master, F_GETFL, 0);
    fcntl(pty_master, F_SETFL, flags | O_NONBLOCK);

    /* Initialize terminal context */
    terminal_ctx_t ctx = {0};
    ctx.pty_master = pty_master;
    ctx.ctrl_write_fd = ctrl_to_client[1];
    ctx.ctrl_read_fd = ctrl_to_terminal[0];
    ctx.child_pid = pid;
    ctx.test_passed = 1;  /* Assume pass until failure */
    ctx.test_done = 0;

    /* Create libuv loop */
    uv_loop_t loop;
    uv_loop_init(&loop);
    ctx.loop = &loop;

    /* Add PTY to loop */
    uv_poll_init(&loop, &ctx.pty_poll, pty_master);
    ctx.pty_poll.data = &ctx;
    uv_poll_start(&ctx.pty_poll, UV_READABLE, terminal_pty_cb);

    /* Add control pipe to loop */
    uv_poll_init(&loop, &ctx.ctrl_poll, ctx.ctrl_read_fd);
    ctx.ctrl_poll.data = &ctx;
    uv_poll_start(&ctx.ctrl_poll, UV_READABLE, terminal_ctrl_cb);

    /* Run terminal test */
    if (scenario->terminal_test) {
        scenario->terminal_test(&ctx);
    }

    /* Run event loop until test is done */
    while (!ctx.test_done) {
        uv_run(&loop, UV_RUN_ONCE);
    }

    /* Cleanup */
    uv_poll_stop(&ctx.pty_poll);
    uv_poll_stop(&ctx.ctrl_poll);
    uv_loop_close(&loop);
    close(pty_master);
    close(ctx.ctrl_write_fd);
    close(ctx.ctrl_read_fd);

    /* Wait for child */
    int status;
    waitpid(pid, &status, 0);

    /* Report result */
    if (ctx.test_passed) {
        printf("PASS: %s\n", scenario->name);
        return 0;
    } else {
        printf("FAIL: %s\n", scenario->name);
        return 1;
    }
}

/* Run all test scenarios */
static int run_all_scenarios(test_scenario_t* scenarios, int count) {
    int passed = 0, failed = 0;

    printf("\n=== Dual-Process Integration Tests ===\n");

    for (int i = 0; i < count; i++) {
        if (run_test_scenario(&scenarios[i]) == 0) {
            passed++;
        } else {
            failed++;
        }
    }

    printf("\nResults: %d passed, %d failed\n", passed, failed);
    return failed;
}

#endif /* YGUI_DUAL_PROCESS_HARNESS_H */
