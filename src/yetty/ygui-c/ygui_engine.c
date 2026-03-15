/*
 * ygui_engine.c - YGui engine implementation with libuv event loop
 */

#include "ygui_internal.h"
#include "ydraw-capi.gen.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <string.h>

/* Debug logging - set YGUI_C_LOG env var to enable */
static FILE* _ygui_log_file = NULL;
static int _ygui_log_checked = 0;

static void _ygui_log_init(void) {
    if (_ygui_log_checked) return;
    _ygui_log_checked = 1;
    const char* log_path = getenv("YGUI_C_LOG");
    if (log_path) {
        _ygui_log_file = fopen(log_path, "w");
        if (_ygui_log_file) {
            setvbuf(_ygui_log_file, NULL, _IONBF, 0);
            fprintf(_ygui_log_file, "[YGUI-C] Logging initialized\n");
        }
    }
}

#define YGUI_LOG(...) do { \
    _ygui_log_init(); \
    if (_ygui_log_file) { fprintf(_ygui_log_file, "[YGUI-C] " __VA_ARGS__); fprintf(_ygui_log_file, "\n"); } \
} while(0)

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
    return "0.2.0";
}

/*=============================================================================
 * Engine Lifecycle
 *===========================================================================*/

ygui_engine_t* ygui_engine_create(const char* card_name, float width, float height) {
    ygui_engine_t* engine = (ygui_engine_t*)calloc(1, sizeof(ygui_engine_t));
    if (!engine) {
        ygui_set_error("Failed to allocate engine");
        return NULL;
    }

    /* Create YDraw buffer */
    engine->buffer = ydraw_buffer_create();
    if (!engine->buffer) {
        ygui_set_error("Failed to create YDraw buffer");
        free(engine);
        return NULL;
    }

    /* Store card name */
    engine->card_name = ygui_strdup(card_name);

    /* Default theme */
    engine->theme = ygui_theme_create_default();
    engine->owns_theme = 1;

    /* Initial state */
    engine->dirty = 1;
    engine->width = width;
    engine->height = height;
    engine->cell_width = 0.0f;   /* Will be set by CSI 16 t query */
    engine->cell_height = 0.0f;

    ygui_grid_init(&engine->grid, width, height, 32.0f);

    return engine;
}

void ygui_engine_destroy(ygui_engine_t* engine) {
    if (!engine) return;

    /* Stop running if needed */
    engine->running = 0;

    /* Unsubscribe from events */
    if (engine->clicks_subscribed) {
        ygui_osc_subscribe_clicks(0);
    }
    if (engine->moves_subscribed) {
        ygui_osc_subscribe_moves(0);
    }

    /* Kill card if shown */
    if (engine->card_shown && engine->card_name) {
        ygui_osc_kill_card(engine->card_name);
    }

    /* Clean up libuv handles if we own the loop */
    if (engine->owns_loop && engine->loop) {
        uv_loop_close(engine->loop);
        free(engine->loop);
    }

    /* Destroy all widgets */
    ygui_widget_t* w = engine->first_widget;
    while (w) {
        ygui_widget_t* next = w->next_sibling;
        ygui_widget_free(w);
        w = next;
    }

    /* Destroy grid */
    ygui_grid_destroy(&engine->grid);

    /* Destroy theme if owned */
    if (engine->owns_theme && engine->theme) {
        ygui_theme_destroy(engine->theme);
    }

    /* Destroy buffer */
    if (engine->buffer) {
        ydraw_buffer_destroy(engine->buffer);
    }

    /* Free card name */
    free(engine->card_name);

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

void ygui_engine_on_key(ygui_engine_t* engine, ygui_key_callback_t callback, void* userdata) {
    if (!engine) return;
    engine->key_callback = callback;
    engine->key_userdata = userdata;
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

/*=============================================================================
 * Rendering
 *===========================================================================*/

static void engine_rebuild(ygui_engine_t* engine) {
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

void ygui_engine_render(ygui_engine_t* engine) {
    if (!engine || !engine->buffer) return;

    /* 1. Clear buffer */
    ydraw_buffer_clear(engine->buffer);

    /* 2. Set explicit scene bounds to match full canvas */
    ydraw_buffer_set_scene_bounds(engine->buffer, 0, 0, engine->width, engine->height);

    /* 3. Rebuild UI */
    engine_rebuild(engine);

    /* 4. Serialize */
    const uint8_t* data = NULL;
    uint32_t size = ydraw_buffer_serialize(engine->buffer, &data);
    if (size == 0 || !data) return;

    /* 5. Send OSC */
    if (!engine->card_shown) {
        ygui_osc_create_card(engine->card_name,
                             engine->card_x, engine->card_y,
                             engine->card_w, engine->card_h,
                             data, size);
        engine->card_shown = 1;
    } else {
        ygui_osc_update_card(engine->card_name, data, size);
    }
}

void ygui_engine_show(ygui_engine_t* engine, int x, int y, int w, int h) {
    if (!engine) return;

    engine->card_x = x;
    engine->card_y = y;
    engine->card_w = w;
    engine->card_h = h;

    /* Query cell size for coordinate scaling */
    ygui_osc_query_cell_size();

    /* Subscribe to click events */
    ygui_osc_subscribe_clicks(1);
    engine->clicks_subscribed = 1;

    /* Render first frame */
    ygui_engine_render(engine);
}

/*=============================================================================
 * Input Handling
 *===========================================================================*/

static void emit_event(ygui_engine_t* engine, const ygui_event_t* event) {
    if (engine->event_callback && event->type != YGUI_EVENT_NONE) {
        engine->event_callback(event, engine->event_userdata);
    }
}

static float to_canvas_x(ygui_engine_t* engine, float px) {
    /* Scale from display pixels to internal canvas pixels */
    float cell_w = engine->cell_width > 0 ? engine->cell_width : 8.0f;
    float display_w = engine->card_w * cell_w;
    if (display_w > 0) {
        return px * (engine->width / display_w);
    }
    return px;
}

static float to_canvas_y(ygui_engine_t* engine, float py) {
    /* Scale from display pixels to internal canvas pixels */
    float cell_h = engine->cell_height > 0 ? engine->cell_height : 16.0f;
    float display_h = engine->card_h * cell_h;
    if (display_h > 0) {
        return py * (engine->height / display_h);
    }
    return py;
}

void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y) {
    if (!engine) return;

    ygui_widget_t* hit = ygui_grid_query(&engine->grid, x, y);

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
        float lx = x - engine->pressed->effective_x;
        float ly = y - engine->pressed->effective_y;
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

    YGUI_LOG("mouse_down at (%.1f, %.1f)", x, y);
    ygui_widget_t* hit = ygui_grid_query(&engine->grid, x, y);
    YGUI_LOG("  grid_query returned: %s (ptr=%p)", hit ? hit->id : "NULL", (void*)hit);

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
            float lx = x - hit->effective_x;
            float ly = y - hit->effective_y;
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

    YGUI_LOG("mouse_up at (%.1f, %.1f) pressed=%s", x, y, engine->pressed ? engine->pressed->id : "NULL");

    if (engine->pressed) {
        ygui_widget_t* widget = engine->pressed;
        widget->flags &= ~YGUI_FLAG_PRESSED;
        engine->dirty = 1;

        /* Check if release is on same widget (click) */
        ygui_widget_t* hit = ygui_grid_query(&engine->grid, x, y);
        YGUI_LOG("  release hit=%s, pressed=%s, match=%d", hit ? hit->id : "NULL", widget->id, hit == widget);
        if (hit == widget) {
            /* Call widget's click callback */
            if (widget->click_callback) {
                widget->click_callback(widget, widget->click_userdata);
            }

            /* Legacy event */
            ygui_event_t event = {
                .widget_id = widget->id,
                .type = YGUI_EVENT_CLICK
            };
            emit_event(engine, &event);
        }

        if (widget->on_release) {
            float lx = x - widget->effective_x;
            float ly = y - widget->effective_y;
            ygui_event_t event = {0};
            if (widget->on_release(widget, lx, ly, &event)) {
                emit_event(engine, &event);
            }
        }

        engine->pressed = NULL;
    }
}

void ygui_engine_mouse_scroll(ygui_engine_t* engine, float x, float y,
                               float dx, float dy) {
    if (!engine) return;

    ygui_widget_t* hit = ygui_grid_query(&engine->grid, x, y);

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

    /* Call global key callback */
    if (engine->key_callback) {
        engine->key_callback(engine, key, mods, engine->key_userdata);
    }

    /* Also try focused widget */
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

            /* Call widget's text callback */
            if (engine->focused->text_callback) {
                engine->focused->text_callback(engine->focused, new_text,
                                               engine->focused->text_userdata);
            }

            /* Legacy event */
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
    return ygui_grid_query(&engine->grid, x, y);
}

/*=============================================================================
 * Input Parsing (OSC 777777/777778 and keyboard)
 *===========================================================================*/

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

/* Parse CSI 6 ; h ; w t (cell size report)
 * Format: ESC [ 6 ; height ; width t
 * Returns 1 on success, 0 if not a matching sequence
 */
static int parse_cell_size_csi(const char* buf, int len,
                                int* cell_height, int* cell_width, int* consumed) {
    if (len < 8) return 0;  /* Minimum: ESC [ 6 ; h ; w t */
    if (buf[0] != '\033' || buf[1] != '[') return 0;

    int i = 2;
    /* Parse first number (should be 6) */
    int n1 = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        n1 = n1 * 10 + (buf[i] - '0');
        i++;
    }
    if (n1 != 6) return 0;  /* Not a cell size report */
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse height */
    int h = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        h = h * 10 + (buf[i] - '0');
        i++;
    }
    if (i >= len || buf[i] != ';') return 0;
    i++;

    /* Parse width */
    int w = 0;
    while (i < len && buf[i] >= '0' && buf[i] <= '9') {
        w = w * 10 + (buf[i] - '0');
        i++;
    }
    if (i >= len || buf[i] != 't') return 0;
    i++;

    *cell_height = h;
    *cell_width = w;
    *consumed = i;
    return 1;
}

/* Scale coordinates from display space to internal canvas space */
static void scale_coords(ygui_engine_t* engine, float* x, float* y) {
    if (engine->cell_width > 0 && engine->cell_height > 0 &&
        engine->card_w > 0 && engine->card_h > 0) {
        float display_w = engine->card_w * engine->cell_width;
        float display_h = engine->card_h * engine->cell_height;
        *x = *x * (engine->width / display_w);
        *y = *y * (engine->height / display_h);
    }
}

static void process_input(ygui_engine_t* engine, const char* data, int len) {
    /* Append to input buffer */
    if (engine->input_len + len > (int)sizeof(engine->input_buffer) - 1) {
        engine->input_len = 0;  /* Reset on overflow */
    }
    memcpy(engine->input_buffer + engine->input_len, data, len);
    engine->input_len += len;

    /* Process input buffer */
    int i = 0;
    while (i < engine->input_len) {
        int osc_code;
        char card_name[128];
        int buttons, press;
        float x, y;
        int consumed;
        int cell_h, cell_w;

        /* Try to parse CSI cell size response first */
        if (parse_cell_size_csi(engine->input_buffer + i,
                                 engine->input_len - i,
                                 &cell_h, &cell_w, &consumed)) {
            YGUI_LOG("Got cell size: %dx%d", cell_w, cell_h);
            engine->cell_width = (float)cell_w;
            engine->cell_height = (float)cell_h;
            i += consumed;
        } else if (parse_card_mouse_osc(engine->input_buffer + i,
                                  engine->input_len - i,
                                  &osc_code, card_name, sizeof(card_name),
                                  &buttons, &press, &x, &y, &consumed)) {
            /* Scale coordinates from display to internal space */
            scale_coords(engine, &x, &y);

            /* Dispatch mouse event */
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
        } else if (engine->input_buffer[i] == '\033') {
            /* Incomplete escape sequence - wait for more data */
            break;
        } else {
            /* Regular character - keyboard input */
            char ch = engine->input_buffer[i];

            if (ch == 'q' || ch == 'Q') {
                /* Quit on 'q' */
                engine->running = 0;
            } else if (engine->key_callback) {
                engine->key_callback(engine, (uint32_t)ch, 0, engine->key_userdata);
            }
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
}

/*=============================================================================
 * libuv Event Loop
 *===========================================================================*/

static void stdin_poll_cb(uv_poll_t* handle, int status, int events) {
    ygui_engine_t* engine = (ygui_engine_t*)handle->data;
    if (status < 0) return;

    if (events & UV_READABLE) {
        char buf[1024];
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n > 0) {
            process_input(engine, buf, (int)n);
        } else if (n == 0) {
            /* EOF */
            engine->running = 0;
        }
    }
}

static void prepare_cb(uv_prepare_t* handle) {
    ygui_engine_t* engine = (ygui_engine_t*)handle->data;

    /* Auto-render if dirty */
    if (engine->dirty) {
        ygui_engine_render(engine);
    }

    /* Check if we should stop */
    if (!engine->running) {
        uv_stop(engine->loop);
    }
}

void ygui_engine_attach(ygui_engine_t* engine, uv_loop_t* loop) {
    if (!engine || !loop) return;

    engine->loop = loop;
    engine->owns_loop = 0;

    /* Set up stdin poll */
    uv_poll_init(loop, &engine->stdin_poll, STDIN_FILENO);
    engine->stdin_poll.data = engine;
    uv_poll_start(&engine->stdin_poll, UV_READABLE, stdin_poll_cb);

    /* Set up prepare handle for auto-render */
    uv_prepare_init(loop, &engine->prepare_handle);
    engine->prepare_handle.data = engine;
    uv_prepare_start(&engine->prepare_handle, prepare_cb);
}

void ygui_engine_run(ygui_engine_t* engine) {
    if (!engine) return;

    /* Create loop if needed */
    if (!engine->loop) {
        engine->loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
        if (!engine->loop) return;
        uv_loop_init(engine->loop);
        engine->owns_loop = 1;

        /* Attach to the loop */
        ygui_engine_attach(engine, engine->loop);
    }

    engine->running = 1;

    /* Run the loop */
    uv_run(engine->loop, UV_RUN_DEFAULT);

    /* Cleanup handles */
    uv_poll_stop(&engine->stdin_poll);
    uv_prepare_stop(&engine->prepare_handle);
}

void ygui_engine_stop(ygui_engine_t* engine) {
    if (engine) engine->running = 0;
}

/*=============================================================================
 * Legacy API compatibility (ygui_engine_create with buffer)
 *===========================================================================*/

/* Keep old function signature working via macro or wrapper if needed */

/*=============================================================================
 * Widget Callbacks
 *===========================================================================*/

void ygui_button_on_click(ygui_widget_t* button, ygui_click_callback_t callback, void* userdata) {
    if (!button || button->type != YGUI_WIDGET_BUTTON) return;
    button->click_callback = callback;
    button->click_userdata = userdata;
}

void ygui_slider_on_change(ygui_widget_t* slider, ygui_change_callback_t callback, void* userdata) {
    if (!slider || slider->type != YGUI_WIDGET_SLIDER) return;
    slider->change_callback = callback;
    slider->change_userdata = userdata;
}

void ygui_checkbox_on_change(ygui_widget_t* checkbox, ygui_check_callback_t callback, void* userdata) {
    if (!checkbox || checkbox->type != YGUI_WIDGET_CHECKBOX) return;
    checkbox->check_callback = callback;
    checkbox->check_userdata = userdata;
}

void ygui_textinput_on_change(ygui_widget_t* input, ygui_text_callback_t callback, void* userdata) {
    if (!input || input->type != YGUI_WIDGET_TEXTINPUT) return;
    input->text_callback = callback;
    input->text_userdata = userdata;
}

/*=============================================================================
 * Engine Clear (removes all widgets)
 *===========================================================================*/

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
 * Deprecated/Legacy Functions
 *===========================================================================*/

void ygui_engine_subscribe_clicks(ygui_engine_t* engine, int enable) {
    if (!engine) return;
    if (enable && !engine->clicks_subscribed) {
        ygui_osc_subscribe_clicks(1);
        engine->clicks_subscribed = 1;
    } else if (!enable && engine->clicks_subscribed) {
        ygui_osc_subscribe_clicks(0);
        engine->clicks_subscribed = 0;
    }
}

void ygui_engine_subscribe_moves(ygui_engine_t* engine, int enable) {
    if (!engine) return;
    if (enable && !engine->moves_subscribed) {
        ygui_osc_subscribe_moves(1);
        engine->moves_subscribed = 1;
    } else if (!enable && engine->moves_subscribed) {
        ygui_osc_subscribe_moves(0);
        engine->moves_subscribed = 0;
    }
}

/* Rebuild without render (for internal use) */
void ygui_engine_rebuild(ygui_engine_t* engine) {
    engine_rebuild(engine);
}
