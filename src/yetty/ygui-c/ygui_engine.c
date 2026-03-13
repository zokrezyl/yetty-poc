/*
 * ygui_engine.c - YGui engine implementation
 */

#include "ygui_internal.h"
#include <stdio.h>

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
