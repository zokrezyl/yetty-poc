/*
 * ygui_engine.c - YGui engine implementation
 */

#include "ygui_internal.h"
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <termios.h>
#include <signal.h>

/*=============================================================================
 * Terminal State
 *===========================================================================*/

static struct termios ygui_orig_termios;
static int ygui_raw_mode = 0;
static int ygui_initialized = 0;

static void ygui_restore_terminal(void) {
    if (ygui_raw_mode) {
        tcsetattr(STDIN_FILENO, TCSANOW, &ygui_orig_termios);
        ygui_raw_mode = 0;
    }
}

static void ygui_signal_handler(int sig) {
    ygui_restore_terminal();
    /* Re-raise signal with default handler */
    signal(sig, SIG_DFL);
    raise(sig);
}

int ygui_init(void) {
    if (ygui_initialized) return 0;

    /* Save original terminal settings */
    if (tcgetattr(STDIN_FILENO, &ygui_orig_termios) < 0) {
        ygui_set_error("Failed to get terminal attributes");
        return -1;
    }

    /* Set up raw mode */
    struct termios raw = ygui_orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag |= OPOST;  /* Keep output processing */
    raw.c_cflag |= CS8;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) < 0) {
        ygui_set_error("Failed to set raw terminal mode");
        return -1;
    }
    ygui_raw_mode = 1;

    /* Set up signal handlers for clean exit */
    signal(SIGINT, ygui_signal_handler);
    signal(SIGTERM, ygui_signal_handler);
    signal(SIGQUIT, ygui_signal_handler);

    /* Register atexit handler */
    atexit(ygui_restore_terminal);

    ygui_initialized = 1;
    return 0;
}

void ygui_shutdown(void) {
    ygui_restore_terminal();
    ygui_initialized = 0;
}

/*=============================================================================
 * Thread-local error message
 *===========================================================================*/

static _Thread_local char ygui_error_msg[256] = {0};

void ygui_set_error(const char* msg) {
    if (msg) {
        snprintf(ygui_error_msg, sizeof(ygui_error_msg), "%s", msg);
    } else {
        ygui_error_msg[0] = '\0';
    }
}

const char* ygui_get_error(void) {
    return ygui_error_msg;
}

/*=============================================================================
 * Version
 *===========================================================================*/

const char* ygui_version(void) {
    return "0.1.0";
}

/*=============================================================================
 * Engine Lifecycle
 *===========================================================================*/

ygui_engine_t* ygui_engine_create(ydraw_buffer_t* buffer) {
    ygui_engine_t* engine = (ygui_engine_t*)calloc(1, sizeof(ygui_engine_t));
    if (!engine) {
        ygui_set_error("Failed to allocate engine");
        return NULL;
    }

    engine->buffer = buffer;
    engine->theme = ygui_theme_create_default();
    engine->owns_theme = 1;
    engine->dirty = 1;
    engine->width = 800;
    engine->height = 600;
    engine->cell_width = 10.0f;
    engine->cell_height = 20.0f;

    ygui_grid_init(&engine->grid, engine->width, engine->height, 32.0f);

    return engine;
}

void ygui_engine_destroy(ygui_engine_t* engine) {
    if (!engine) return;

    /* Destroy all widgets */
    ygui_engine_clear(engine);

    /* Destroy grid */
    ygui_grid_destroy(&engine->grid);

    /* Destroy theme if owned */
    if (engine->owns_theme && engine->theme) {
        ygui_theme_destroy(engine->theme);
    }

    free(engine);
}

/*=============================================================================
 * Engine Configuration
 *===========================================================================*/

void ygui_engine_set_size(ygui_engine_t* engine, float width, float height) {
    if (!engine) return;
    engine->width = width;
    engine->height = height;
    ygui_grid_destroy(&engine->grid);
    ygui_grid_init(&engine->grid, width, height, 32.0f);
    engine->dirty = 1;
}

void ygui_engine_set_cell_size(ygui_engine_t* engine, float cell_w, float cell_h) {
    if (!engine) return;
    engine->cell_width = cell_w;
    engine->cell_height = cell_h;
}

void ygui_engine_set_theme(ygui_engine_t* engine, ygui_theme_t* theme) {
    if (!engine) return;
    if (engine->owns_theme && engine->theme) {
        ygui_theme_destroy(engine->theme);
    }
    engine->theme = theme;
    engine->owns_theme = 0;
    engine->dirty = 1;
}

void ygui_engine_set_event_callback(ygui_engine_t* engine,
                                     ygui_event_callback_t callback,
                                     void* userdata) {
    if (!engine) return;
    engine->event_callback = callback;
    engine->event_userdata = userdata;
}

/*=============================================================================
 * Engine State
 *===========================================================================*/

int ygui_engine_is_dirty(const ygui_engine_t* engine) {
    return engine ? engine->dirty : 0;
}

void ygui_engine_mark_dirty(ygui_engine_t* engine) {
    if (engine) engine->dirty = 1;
}

void ygui_engine_clear(ygui_engine_t* engine) {
    if (!engine) return;

    /* Free all widgets */
    ygui_widget_t* w = engine->first_widget;
    while (w) {
        ygui_widget_t* next = w->next_sibling;
        ygui_widget_free(w);
        w = next;
    }

    engine->first_widget = NULL;
    engine->last_widget = NULL;
    engine->widget_count = 0;
    engine->hovered = NULL;
    engine->pressed = NULL;
    engine->focused = NULL;

    ygui_grid_clear(&engine->grid);
    engine->dirty = 1;
}

/*=============================================================================
 * Rendering
 *===========================================================================*/

void ygui_engine_rebuild(ygui_engine_t* engine) {
    if (!engine || !engine->buffer) return;

    /* Clear the grid */
    ygui_grid_clear(&engine->grid);

    /* Create render context */
    ygui_render_ctx_t ctx;
    ygui_render_ctx_init(&ctx, engine->buffer, engine->theme);

    /* Render all top-level widgets */
    for (ygui_widget_t* w = engine->first_widget; w; w = w->next_sibling) {
        w->was_rendered = 0;
        if (w->render_all) {
            w->render_all(w, &ctx);
        } else {
            ygui_widget_render_all_default(w, &ctx);
        }
    }

    /* Rebuild spatial grid with rendered widgets */
    for (ygui_widget_t* w = engine->first_widget; w; w = w->next_sibling) {
        if (w->was_rendered) {
            ygui_grid_insert(&engine->grid, w);
        }
    }

    engine->dirty = 0;
}

/*=============================================================================
 * Input Handling
 *===========================================================================*/

static void emit_event(ygui_engine_t* engine, const ygui_event_t* event) {
    if (engine->event_callback && event->type != YGUI_EVENT_NONE) {
        engine->event_callback(event, engine->event_userdata);
    }
}

static float to_widget_x(ygui_engine_t* engine, float px) {
    return px * 10.0f / engine->cell_width;
}

static float to_widget_y(ygui_engine_t* engine, float py) {
    return py * 20.0f / engine->cell_height;
}

void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y) {
    if (!engine) return;

    float wx = to_widget_x(engine, x);
    float wy = to_widget_y(engine, y);

    ygui_widget_t* hit = ygui_grid_query(&engine->grid, wx, wy);

    /* Handle hover changes */
    if (hit != engine->hovered) {
        if (engine->hovered) {
            engine->hovered->flags &= ~YGUI_FLAG_HOVER;
            engine->dirty = 1;
        }
        if (hit) {
            hit->flags |= YGUI_FLAG_HOVER;
            engine->dirty = 1;
        }
        engine->hovered = hit;
    }

    /* Handle drag */
    if (engine->pressed && engine->pressed->on_drag) {
        float lx = wx - engine->pressed->effective_x;
        float ly = wy - engine->pressed->effective_y;
        ygui_event_t event = {0};
        if (engine->pressed->on_drag(engine->pressed, lx, ly, &event)) {
            emit_event(engine, &event);
            engine->dirty = 1;
        }
    }
}

void ygui_engine_mouse_down(ygui_engine_t* engine, float x, float y, int button) {
    if (!engine) return;
    (void)button;

    float wx = to_widget_x(engine, x);
    float wy = to_widget_y(engine, y);

    ygui_widget_t* hit = ygui_grid_query(&engine->grid, wx, wy);

    if (hit) {
        hit->flags |= YGUI_FLAG_PRESSED;
        engine->pressed = hit;
        engine->dirty = 1;

        /* Focus change */
        if (engine->focused != hit) {
            if (engine->focused) {
                engine->focused->flags &= ~YGUI_FLAG_FOCUSED;
            }
            hit->flags |= YGUI_FLAG_FOCUSED;
            engine->focused = hit;
        }

        if (hit->on_press) {
            float lx = wx - hit->effective_x;
            float ly = wy - hit->effective_y;
            ygui_event_t event = {0};
            if (hit->on_press(hit, lx, ly, &event)) {
                emit_event(engine, &event);
            }
        }
    }
}

void ygui_engine_mouse_up(ygui_engine_t* engine, float x, float y, int button) {
    if (!engine) return;
    (void)button;

    float wx = to_widget_x(engine, x);
    float wy = to_widget_y(engine, y);

    if (engine->pressed) {
        engine->pressed->flags &= ~YGUI_FLAG_PRESSED;
        engine->dirty = 1;

        if (engine->pressed->on_release) {
            float lx = wx - engine->pressed->effective_x;
            float ly = wy - engine->pressed->effective_y;
            ygui_event_t event = {0};
            if (engine->pressed->on_release(engine->pressed, lx, ly, &event)) {
                emit_event(engine, &event);
            }
        }

        engine->pressed = NULL;
    }
}

void ygui_engine_mouse_scroll(ygui_engine_t* engine, float x, float y,
                               float dx, float dy) {
    if (!engine) return;

    float wx = to_widget_x(engine, x);
    float wy = to_widget_y(engine, y);

    ygui_widget_t* hit = ygui_grid_query(&engine->grid, wx, wy);

    if (hit && hit->on_scroll) {
        ygui_event_t event = {0};
        if (hit->on_scroll(hit, dx, dy, &event)) {
            emit_event(engine, &event);
            engine->dirty = 1;
        }
    }
}

void ygui_engine_key_down(ygui_engine_t* engine, uint32_t key, int mods) {
    if (!engine) return;

    if (engine->focused && engine->focused->on_key) {
        ygui_event_t event = {0};
        if (engine->focused->on_key(engine->focused, key, mods, &event)) {
            emit_event(engine, &event);
            engine->dirty = 1;
        }
    }
}

void ygui_engine_key_up(ygui_engine_t* engine, uint32_t key, int mods) {
    (void)engine;
    (void)key;
    (void)mods;
    /* Currently unused */
}

void ygui_engine_text_input(ygui_engine_t* engine, const char* text) {
    if (!engine || !engine->focused) return;

    /* Only textinput handles text input */
    if (engine->focused->type == YGUI_WIDGET_TEXTINPUT) {
        /* Append text to input */
        char* old_text = engine->focused->data.textinput.text;
        size_t old_len = old_text ? strlen(old_text) : 0;
        size_t add_len = strlen(text);
        char* new_text = (char*)malloc(old_len + add_len + 1);
        if (new_text) {
            if (old_text) memcpy(new_text, old_text, old_len);
            memcpy(new_text + old_len, text, add_len + 1);
            free(old_text);
            engine->focused->data.textinput.text = new_text;
            engine->focused->data.textinput.cursor_pos = (int)(old_len + add_len);
            engine->dirty = 1;

            ygui_event_t event = {
                .widget_id = engine->focused->id,
                .type = YGUI_EVENT_CHANGE,
                .data.string_value = new_text
            };
            emit_event(engine, &event);
        }
    }
}

/*=============================================================================
 * Widget Lookup
 *===========================================================================*/

static ygui_widget_t* find_recursive(ygui_widget_t* w, const char* id) {
    if (!w || !id) return NULL;
    if (w->id && strcmp(w->id, id) == 0) return w;

    for (ygui_widget_t* child = w->first_child; child; child = child->next_sibling) {
        ygui_widget_t* found = find_recursive(child, id);
        if (found) return found;
    }
    return NULL;
}

ygui_widget_t* ygui_engine_find(ygui_engine_t* engine, const char* id) {
    if (!engine || !id) return NULL;

    for (ygui_widget_t* w = engine->first_widget; w; w = w->next_sibling) {
        ygui_widget_t* found = find_recursive(w, id);
        if (found) return found;
    }
    return NULL;
}

ygui_widget_t* ygui_engine_widget_at(ygui_engine_t* engine, float x, float y) {
    if (!engine) return NULL;
    float wx = to_widget_x(engine, x);
    float wy = to_widget_y(engine, y);
    return ygui_grid_query(&engine->grid, wx, wy);
}

/*=============================================================================
 * Event Loop
 *===========================================================================*/

void ygui_engine_subscribe_clicks(ygui_engine_t* engine, int enable) {
    if (!engine) return;
    if (enable && !engine->clicks_subscribed) {
        write(STDOUT_FILENO, "\033[?1500h", 8);
        engine->clicks_subscribed = 1;
    } else if (!enable && engine->clicks_subscribed) {
        write(STDOUT_FILENO, "\033[?1500l", 8);
        engine->clicks_subscribed = 0;
    }
}

void ygui_engine_subscribe_moves(ygui_engine_t* engine, int enable) {
    if (!engine) return;
    if (enable && !engine->moves_subscribed) {
        write(STDOUT_FILENO, "\033[?1501h", 8);
        engine->moves_subscribed = 1;
    } else if (!enable && engine->moves_subscribed) {
        write(STDOUT_FILENO, "\033[?1501l", 8);
        engine->moves_subscribed = 0;
    }
}

/* Parse OSC 777777 (click) or 777778 (move) sequence
 * Format: ESC ] CODE ; card-name ; buttons ; [press ;] x ; y ESC \
 * Returns 1 on success, 0 if not a matching sequence
 */
static int parse_card_mouse_osc(const char* buf, int len,
                                 int* osc_code, char* card_name, int name_max,
                                 int* buttons, int* press,
                                 float* x, float* y, int* consumed) {
    if (len < 10) return 0;
    if (buf[0] != '\033' || buf[1] != ']') return 0;

    int i = 2;
    int code = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        code = code * 10 + (buf[i] - '0');
        i++;
    }
    if (code != 777777 && code != 777778) return 0;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse card name */
    int name_start = i;
    while (i < len && buf[i] != ';') i++;
    if (i >= len) return 0;
    int name_len = i - name_start;
    if (name_len >= name_max) name_len = name_max - 1;
    memcpy(card_name, buf + name_start, name_len);
    card_name[name_len] = '\0';
    i++;

    /* Parse buttons */
    int btn = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        btn = btn * 10 + (buf[i] - '0');
        i++;
    }
    *buttons = btn;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* For OSC 777777, parse press */
    if (code == 777777) {
        int p = 0;
        while (i < len && buf[i] >= '0' && buf[i] <= '9') {
            p = p * 10 + (buf[i] - '0');
            i++;
        }
        *press = p;
        if (i >= len || buf[i] != ';') return 0;
        i++;
    } else {
        *press = -1;  /* N/A for move */
    }

    /* Parse x */
    int ix = 0;
    int neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        ix = ix * 10 + (buf[i] - '0');
        i++;
    }
    *x = neg ? -(float)ix : (float)ix;
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse y */
    int iy = 0;
    neg = 0;
    if (i < len && buf[i] == '-') { neg = 1; i++; }
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        iy = iy * 10 + (buf[i] - '0');
        i++;
    }
    *y = neg ? -(float)iy : (float)iy;

    /* Expect ST: ESC \ */
    if (i + 1 >= len || buf[i] != '\033' || buf[i+1] != '\\') return 0;

    *osc_code = code;
    *consumed = i + 2;
    return 1;
}

int ygui_engine_poll(ygui_engine_t* engine, int timeout_ms) {
    if (!engine) return -1;

    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    int ret = poll(&pfd, 1, timeout_ms);
    if (ret < 0) return -1;
    if (ret == 0) return 0;

    if (!(pfd.revents & POLLIN)) return 0;

    char buf[256];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) return -1;

    /* Append to input buffer */
    if (engine->input_len + (int)n > (int)sizeof(engine->input_buffer) - 1) {
        engine->input_len = 0;  /* Reset on overflow */
    }
    memcpy(engine->input_buffer + engine->input_len, buf, n);
    engine->input_len += (int)n;

    /* Try to parse OSC sequences */
    int processed = 0;
    int i = 0;
    while (i < engine->input_len) {
        int osc_code;
        char card_name[128];
        int buttons, press;
        float x, y;
        int consumed;

        if (parse_card_mouse_osc(engine->input_buffer + i,
                                  engine->input_len - i,
                                  &osc_code, card_name, sizeof(card_name),
                                  &buttons, &press, &x, &y, &consumed)) {
            /* Dispatch to widget engine */
            if (osc_code == 777777) {
                /* Click event */
                if (press == 1) {
                    ygui_engine_mouse_down(engine, x, y, buttons & 0x7);
                } else {
                    ygui_engine_mouse_up(engine, x, y, buttons & 0x7);
                }
            } else {
                /* Move event */
                ygui_engine_mouse_move(engine, x, y);
            }

            i += consumed;
            processed = 1;
        } else if (engine->input_buffer[i] == 'q' || engine->input_buffer[i] == 'Q') {
            /* Quit on 'q' key */
            engine->running = 0;
            i++;
            processed = 1;
        } else {
            i++;
        }
    }

    /* Compact buffer */
    if (i > 0 && i < engine->input_len) {
        memmove(engine->input_buffer, engine->input_buffer + i,
                engine->input_len - i);
        engine->input_len -= i;
    } else if (i >= engine->input_len) {
        engine->input_len = 0;
    }

    return processed ? 1 : 0;
}

void ygui_engine_run(ygui_engine_t* engine) {
    if (!engine) return;
    engine->running = 1;
    while (engine->running) {
        ygui_engine_poll(engine, 50);  /* 50ms timeout */
    }
}

void ygui_engine_stop(ygui_engine_t* engine) {
    if (engine) engine->running = 0;
}
