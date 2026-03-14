/*
 * ygui_widgets.c - Widget implementations
 */

#include "ygui_internal.h"
#include <stdio.h>

/*=============================================================================
 * Widget Base Functions
 *===========================================================================*/

ygui_widget_t* ygui_widget_alloc(ygui_engine_t* engine, ygui_widget_type_t type, const char* id) {
    ygui_widget_t* w = (ygui_widget_t*)calloc(1, sizeof(ygui_widget_t));
    if (!w) {
        ygui_set_error("Failed to allocate widget");
        return NULL;
    }

    w->id = ygui_strdup(id);
    w->type = type;
    w->engine = engine;
    w->flags = YGUI_FLAG_VISIBLE;
    w->bg_color = engine->theme->bg_surface;
    w->fg_color = engine->theme->text_primary;
    w->accent_color = engine->theme->accent;

    return w;
}

void ygui_widget_init_base(ygui_widget_t* widget, float x, float y, float w, float h) {
    widget->x = x;
    widget->y = y;
    widget->w = w;
    widget->h = h;
}

void ygui_widget_free(ygui_widget_t* widget) {
    if (!widget) return;

    /* Free children recursively */
    ygui_widget_t* child = widget->first_child;
    while (child) {
        ygui_widget_t* next = child->next_sibling;
        ygui_widget_free(child);
        child = next;
    }

    /* Call type-specific destroy */
    if (widget->destroy) {
        widget->destroy(widget);
    }

    free(widget->id);
    free(widget);
}

void ygui_widget_render_all_default(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    self->effective_x = self->x + ctx->offset_x;
    self->effective_y = self->y + ctx->offset_y;
    self->was_rendered = 1;

    if (self->render) {
        self->render(self, ctx);
    }

    /* Render children */
    for (ygui_widget_t* child = self->first_child; child; child = child->next_sibling) {
        if (child->render_all) {
            child->render_all(child, ctx);
        } else {
            ygui_widget_render_all_default(child, ctx);
        }
    }
}

/*=============================================================================
 * Widget Hierarchy
 *===========================================================================*/

static void add_to_engine(ygui_engine_t* engine, ygui_widget_t* widget) {
    widget->engine = engine;
    if (!engine->first_widget) {
        engine->first_widget = widget;
        engine->last_widget = widget;
    } else {
        engine->last_widget->next_sibling = widget;
        widget->prev_sibling = engine->last_widget;
        engine->last_widget = widget;
    }
    engine->widget_count++;
    engine->dirty = 1;
}

void ygui_widget_add_child(ygui_widget_t* parent, ygui_widget_t* child) {
    if (!parent || !child) return;

    /* Remove from engine's top-level list if present */
    ygui_engine_t* engine = parent->engine;
    if (engine && !child->parent) {
        if (child->prev_sibling) {
            child->prev_sibling->next_sibling = child->next_sibling;
        } else if (engine->first_widget == child) {
            engine->first_widget = child->next_sibling;
        }
        if (child->next_sibling) {
            child->next_sibling->prev_sibling = child->prev_sibling;
        } else if (engine->last_widget == child) {
            engine->last_widget = child->prev_sibling;
        }
        engine->widget_count--;
    }

    child->parent = parent;
    child->prev_sibling = NULL;
    child->next_sibling = NULL;

    if (!parent->first_child) {
        parent->first_child = child;
        parent->last_child = child;
    } else {
        parent->last_child->next_sibling = child;
        child->prev_sibling = parent->last_child;
        parent->last_child = child;
    }

    if (engine) engine->dirty = 1;
}

void ygui_widget_remove_child(ygui_widget_t* parent, ygui_widget_t* child) {
    if (!parent || !child || child->parent != parent) return;

    if (child->prev_sibling) {
        child->prev_sibling->next_sibling = child->next_sibling;
    } else {
        parent->first_child = child->next_sibling;
    }

    if (child->next_sibling) {
        child->next_sibling->prev_sibling = child->prev_sibling;
    } else {
        parent->last_child = child->prev_sibling;
    }

    child->parent = NULL;
    child->prev_sibling = NULL;
    child->next_sibling = NULL;

    if (parent->engine) parent->engine->dirty = 1;
}

void ygui_widget_remove(ygui_widget_t* widget) {
    if (!widget) return;

    if (widget->parent) {
        ygui_widget_remove_child(widget->parent, widget);
    } else if (widget->engine) {
        ygui_engine_t* engine = widget->engine;
        if (widget->prev_sibling) {
            widget->prev_sibling->next_sibling = widget->next_sibling;
        } else {
            engine->first_widget = widget->next_sibling;
        }
        if (widget->next_sibling) {
            widget->next_sibling->prev_sibling = widget->prev_sibling;
        } else {
            engine->last_widget = widget->prev_sibling;
        }
        engine->widget_count--;
        engine->dirty = 1;
    }

    ygui_widget_free(widget);
}

ygui_widget_t* ygui_widget_parent(ygui_widget_t* widget) {
    return widget ? widget->parent : NULL;
}

ygui_widget_t* ygui_widget_first_child(ygui_widget_t* widget) {
    return widget ? widget->first_child : NULL;
}

ygui_widget_t* ygui_widget_next_sibling(ygui_widget_t* widget) {
    return widget ? widget->next_sibling : NULL;
}

/*=============================================================================
 * Widget Properties (Generic)
 *===========================================================================*/

const char* ygui_widget_id(const ygui_widget_t* widget) {
    return widget ? widget->id : NULL;
}

ygui_widget_type_t ygui_widget_type(const ygui_widget_t* widget) {
    return widget ? widget->type : YGUI_WIDGET_CUSTOM;
}

void ygui_widget_set_position(ygui_widget_t* widget, float x, float y) {
    if (!widget) return;
    widget->x = x;
    widget->y = y;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_widget_get_position(const ygui_widget_t* widget, float* x, float* y) {
    if (!widget) return;
    if (x) *x = widget->x;
    if (y) *y = widget->y;
}

void ygui_widget_set_size(ygui_widget_t* widget, float w, float h) {
    if (!widget) return;
    widget->w = w;
    widget->h = h;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_widget_get_size(const ygui_widget_t* widget, float* w, float* h) {
    if (!widget) return;
    if (w) *w = widget->w;
    if (h) *h = widget->h;
}

void ygui_widget_set_visible(ygui_widget_t* widget, int visible) {
    if (!widget) return;
    if (visible) {
        widget->flags |= YGUI_FLAG_VISIBLE;
    } else {
        widget->flags &= ~YGUI_FLAG_VISIBLE;
    }
    if (widget->engine) widget->engine->dirty = 1;
}

int ygui_widget_is_visible(const ygui_widget_t* widget) {
    return widget ? (widget->flags & YGUI_FLAG_VISIBLE) != 0 : 0;
}

void ygui_widget_set_enabled(ygui_widget_t* widget, int enabled) {
    if (!widget) return;
    if (enabled) {
        widget->flags &= ~YGUI_FLAG_DISABLED;
    } else {
        widget->flags |= YGUI_FLAG_DISABLED;
    }
    if (widget->engine) widget->engine->dirty = 1;
}

int ygui_widget_is_enabled(const ygui_widget_t* widget) {
    return widget ? (widget->flags & YGUI_FLAG_DISABLED) == 0 : 0;
}

uint32_t ygui_widget_get_flags(const ygui_widget_t* widget) {
    return widget ? widget->flags : 0;
}

void ygui_widget_set_bg_color(ygui_widget_t* widget, uint32_t color) {
    if (!widget) return;
    widget->bg_color = color;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_widget_set_fg_color(ygui_widget_t* widget, uint32_t color) {
    if (!widget) return;
    widget->fg_color = color;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_widget_set_accent_color(ygui_widget_t* widget, uint32_t color) {
    if (!widget) return;
    widget->accent_color = color;
    if (widget->engine) widget->engine->dirty = 1;
}

/*=============================================================================
 * Button Widget
 *===========================================================================*/

static void button_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    const ygui_theme_t* t = ctx->theme;
    uint32_t bg = (self->flags & YGUI_FLAG_PRESSED) ? self->accent_color : self->bg_color;

    ygui_render_box(ctx, self->x, self->y, self->w, self->h, bg, t->radius_medium);
    if (self->data.button.label) {
        ygui_render_text(ctx, self->data.button.label,
                         self->x + t->pad_large, self->y + t->pad_medium,
                         self->fg_color, t->font_size);
    }
    if (self->flags & YGUI_FLAG_HOVER) {
        ygui_render_box_outline(ctx, self->x, self->y, self->w, self->h,
                                self->accent_color, t->radius_medium, 2.0f);
    }
}

static int button_on_press(ygui_widget_t* self, float lx, float ly, ygui_event_t* out) {
    (void)lx; (void)ly;
    out->widget_id = self->id;
    out->type = YGUI_EVENT_PRESS;
    return 1;
}

static int button_on_release(ygui_widget_t* self, float lx, float ly, ygui_event_t* out) {
    if (lx >= 0 && lx < self->w && ly >= 0 && ly < self->h) {
        out->widget_id = self->id;
        out->type = YGUI_EVENT_CLICK;
        return 1;
    }
    return 0;
}

static void button_destroy(ygui_widget_t* self) {
    free(self->data.button.label);
}

ygui_widget_t* ygui_button(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           const char* label) {
    ygui_widget_t* btn = ygui_widget_alloc(engine, YGUI_WIDGET_BUTTON, id);
    if (!btn) return NULL;

    ygui_widget_init_base(btn, x, y, w, h);
    btn->data.button.label = ygui_strdup(label);
    btn->render = button_render;
    btn->on_press = button_on_press;
    btn->on_release = button_on_release;
    btn->destroy = button_destroy;

    add_to_engine(engine, btn);
    return btn;
}

void ygui_button_set_label(ygui_widget_t* widget, const char* label) {
    if (!widget || widget->type != YGUI_WIDGET_BUTTON) return;
    free(widget->data.button.label);
    widget->data.button.label = ygui_strdup(label);
    if (widget->engine) widget->engine->dirty = 1;
}

const char* ygui_button_get_label(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_BUTTON) return NULL;
    return widget->data.button.label;
}

/*=============================================================================
 * Label Widget
 *===========================================================================*/

static void label_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    if (self->data.label.text) {
        float font_size = self->data.label.font_size > 0
                          ? self->data.label.font_size
                          : ctx->theme->font_size;
        ygui_render_text(ctx, self->data.label.text,
                         self->x, self->y, self->fg_color, font_size);
    }
}

static void label_destroy(ygui_widget_t* self) {
    free(self->data.label.text);
}

ygui_widget_t* ygui_label(ygui_engine_t* engine, const char* id,
                          float x, float y, const char* text) {
    ygui_widget_t* lbl = ygui_widget_alloc(engine, YGUI_WIDGET_LABEL, id);
    if (!lbl) return NULL;

    float h = engine->theme->row_height;
    ygui_widget_init_base(lbl, x, y, 100, h);  /* Width is flexible */
    lbl->data.label.text = ygui_strdup(text);
    lbl->data.label.font_size = 0;  /* Use theme default */
    lbl->render = label_render;
    lbl->destroy = label_destroy;

    add_to_engine(engine, lbl);
    return lbl;
}

void ygui_label_set_text(ygui_widget_t* widget, const char* text) {
    if (!widget || widget->type != YGUI_WIDGET_LABEL) return;
    free(widget->data.label.text);
    widget->data.label.text = ygui_strdup(text);
    if (widget->engine) widget->engine->dirty = 1;
}

const char* ygui_label_get_text(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_LABEL) return NULL;
    return widget->data.label.text;
}

void ygui_label_set_font_size(ygui_widget_t* widget, float size) {
    if (!widget || widget->type != YGUI_WIDGET_LABEL) return;
    widget->data.label.font_size = size;
    if (widget->engine) widget->engine->dirty = 1;
}

/*=============================================================================
 * Slider Widget
 *===========================================================================*/

static void slider_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    const ygui_theme_t* t = ctx->theme;
    float track_h = t->pad_medium;
    float track_y = self->y + (self->h - track_h) / 2;

    /* Track background */
    ygui_render_box(ctx, self->x, track_y, self->w, track_h,
                    self->bg_color, t->radius_small);

    /* Filled portion */
    float range = self->data.slider.max_val - self->data.slider.min_val;
    float pct = range > 0 ? (self->data.slider.value - self->data.slider.min_val) / range : 0;
    float fill_w = pct * self->w;
    ygui_render_box(ctx, self->x, track_y, fill_w, track_h,
                    self->accent_color, t->radius_small);

    /* Handle */
    float handle_w = t->scrollbar_size;
    float handle_x = self->x + fill_w - handle_w / 2;
    ygui_render_box(ctx, handle_x, self->y, handle_w, self->h,
                    self->accent_color, handle_w / 2);
}

static void slider_update_value(ygui_widget_t* self, float local_x) {
    float pct = ygui_clamp(local_x / self->w, 0.0f, 1.0f);
    float range = self->data.slider.max_val - self->data.slider.min_val;
    self->data.slider.value = self->data.slider.min_val + pct * range;
}

static int slider_on_press(ygui_widget_t* self, float lx, float ly, ygui_event_t* out) {
    (void)ly;
    slider_update_value(self, lx);

    /* Call user callback */
    if (self->change_callback) {
        self->change_callback(self, self->data.slider.value, self->change_userdata);
    }

    out->widget_id = self->id;
    out->type = YGUI_EVENT_CHANGE;
    out->data.float_value = self->data.slider.value;
    return 1;
}

static int slider_on_drag(ygui_widget_t* self, float lx, float ly, ygui_event_t* out) {
    (void)ly;
    slider_update_value(self, lx);

    /* Call user callback */
    if (self->change_callback) {
        self->change_callback(self, self->data.slider.value, self->change_userdata);
    }

    out->widget_id = self->id;
    out->type = YGUI_EVENT_CHANGE;
    out->data.float_value = self->data.slider.value;
    return 1;
}

static int slider_on_scroll(ygui_widget_t* self, float dx, float dy, ygui_event_t* out) {
    (void)dx;
    float range = self->data.slider.max_val - self->data.slider.min_val;
    float delta = dy * range * 0.05f;
    self->data.slider.value = ygui_clamp(
        self->data.slider.value + delta,
        self->data.slider.min_val,
        self->data.slider.max_val);
    out->widget_id = self->id;
    out->type = YGUI_EVENT_CHANGE;
    out->data.float_value = self->data.slider.value;
    return 1;
}

ygui_widget_t* ygui_slider(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           float min_val, float max_val, float value) {
    ygui_widget_t* sld = ygui_widget_alloc(engine, YGUI_WIDGET_SLIDER, id);
    if (!sld) return NULL;

    ygui_widget_init_base(sld, x, y, w, h);
    sld->data.slider.min_val = min_val;
    sld->data.slider.max_val = max_val;
    sld->data.slider.value = ygui_clamp(value, min_val, max_val);
    sld->render = slider_render;
    sld->on_press = slider_on_press;
    sld->on_drag = slider_on_drag;
    sld->on_scroll = slider_on_scroll;

    add_to_engine(engine, sld);
    return sld;
}

void ygui_slider_set_value(ygui_widget_t* widget, float value) {
    if (!widget || widget->type != YGUI_WIDGET_SLIDER) return;
    widget->data.slider.value = ygui_clamp(
        value, widget->data.slider.min_val, widget->data.slider.max_val);
    if (widget->engine) widget->engine->dirty = 1;
}

float ygui_slider_get_value(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_SLIDER) return 0;
    return widget->data.slider.value;
}

void ygui_slider_set_range(ygui_widget_t* widget, float min_val, float max_val) {
    if (!widget || widget->type != YGUI_WIDGET_SLIDER) return;
    widget->data.slider.min_val = min_val;
    widget->data.slider.max_val = max_val;
    widget->data.slider.value = ygui_clamp(
        widget->data.slider.value, min_val, max_val);
    if (widget->engine) widget->engine->dirty = 1;
}

/*=============================================================================
 * Checkbox Widget
 *===========================================================================*/

static void checkbox_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    const ygui_theme_t* t = ctx->theme;
    float box_size = self->h - t->pad_small * 2;
    float box_y = self->y + t->pad_small;

    /* Box background */
    uint32_t box_color = self->data.checkbox.checked ? self->accent_color : self->bg_color;
    ygui_render_box(ctx, self->x, box_y, box_size, box_size, box_color, t->radius_small);
    ygui_render_box_outline(ctx, self->x, box_y, box_size, box_size,
                            t->border, t->radius_small, 1.5f);

    /* Checkmark (simple cross for now) */
    if (self->data.checkbox.checked) {
        float cx = self->x + box_size / 2;
        float cy = box_y + box_size / 2;
        float s = box_size * 0.3f;
        /* Draw a simple checkmark using triangles */
        ygui_render_box(ctx, cx - s, cy - 1, s * 2, 3, self->fg_color, 1);
    }

    /* Label */
    if (self->data.checkbox.label) {
        float text_x = self->x + box_size + t->pad_medium;
        ygui_render_text(ctx, self->data.checkbox.label,
                         text_x, self->y + t->pad_medium,
                         self->fg_color, t->font_size);
    }
}

static int checkbox_on_release(ygui_widget_t* self, float lx, float ly, ygui_event_t* out) {
    if (lx >= 0 && lx < self->w && ly >= 0 && ly < self->h) {
        self->data.checkbox.checked = !self->data.checkbox.checked;

        /* Call user callback */
        if (self->check_callback) {
            self->check_callback(self, self->data.checkbox.checked, self->check_userdata);
        }

        out->widget_id = self->id;
        out->type = YGUI_EVENT_CHANGE;
        out->data.bool_value = self->data.checkbox.checked;
        return 1;
    }
    return 0;
}

static void checkbox_destroy(ygui_widget_t* self) {
    free(self->data.checkbox.label);
}

ygui_widget_t* ygui_checkbox(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             const char* label, int checked) {
    ygui_widget_t* chk = ygui_widget_alloc(engine, YGUI_WIDGET_CHECKBOX, id);
    if (!chk) return NULL;

    ygui_widget_init_base(chk, x, y, w, h);
    chk->data.checkbox.label = ygui_strdup(label);
    chk->data.checkbox.checked = checked;
    chk->render = checkbox_render;
    chk->on_release = checkbox_on_release;
    chk->destroy = checkbox_destroy;

    add_to_engine(engine, chk);
    return chk;
}

void ygui_checkbox_set_checked(ygui_widget_t* widget, int checked) {
    if (!widget || widget->type != YGUI_WIDGET_CHECKBOX) return;
    widget->data.checkbox.checked = checked;
    if (widget->engine) widget->engine->dirty = 1;
}

int ygui_checkbox_get_checked(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_CHECKBOX) return 0;
    return widget->data.checkbox.checked;
}

void ygui_checkbox_set_label(ygui_widget_t* widget, const char* label) {
    if (!widget || widget->type != YGUI_WIDGET_CHECKBOX) return;
    free(widget->data.checkbox.label);
    widget->data.checkbox.label = ygui_strdup(label);
    if (widget->engine) widget->engine->dirty = 1;
}

/*=============================================================================
 * Panel Widget
 *===========================================================================*/

static void panel_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    const ygui_theme_t* t = ctx->theme;
    float radius = self->data.panel.corner_radius > 0
                   ? self->data.panel.corner_radius : t->radius_large;

    /* Background */
    ygui_render_box(ctx, self->x, self->y, self->w, self->h, self->bg_color, radius);

    /* Scrollbar if needed */
    float scrollable_h = self->h - self->data.panel.header_h;
    float content_h = self->data.panel.content_h;
    if (content_h > scrollable_h && scrollable_h > 0) {
        float sb_w = t->scrollbar_size;
        float track_x = self->x + self->w - sb_w;
        float track_y = self->y + self->data.panel.header_h;
        float track_h = scrollable_h;

        /* Track */
        ygui_render_box(ctx, track_x, track_y, sb_w, track_h,
                        t->bg_secondary, sb_w / 2);

        /* Thumb */
        float max_scroll = content_h - scrollable_h;
        float thumb_h = ygui_max(20.0f, track_h * scrollable_h / content_h);
        float thumb_range = track_h - thumb_h;
        float thumb_y = track_y + (max_scroll > 0
                        ? (self->data.panel.scroll_y / max_scroll) * thumb_range : 0);
        ygui_render_box(ctx, track_x + t->pad_small, thumb_y,
                        sb_w - t->pad_medium, thumb_h,
                        t->thumb_normal, (sb_w - t->pad_medium) / 2);
    }
}

static void panel_render_all(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    self->effective_x = self->x + ctx->offset_x;
    self->effective_y = self->y + ctx->offset_y;
    self->was_rendered = 1;

    /* Render panel background and scrollbar */
    panel_render(self, ctx);

    const ygui_theme_t* t = ctx->theme;
    float header_h = self->data.panel.header_h;
    float scrollable_h = self->h - header_h;
    float content_h = self->data.panel.content_h;
    float sb_w = (content_h > scrollable_h && scrollable_h > 0) ? t->scrollbar_size : 0;

    /* Save context state */
    float old_offset_x = ctx->offset_x;
    float old_offset_y = ctx->offset_y;

    /* Render header children (no scrolling) */
    ctx->offset_x = old_offset_x + self->x;
    ctx->offset_y = old_offset_y + self->y;
    for (ygui_widget_t* child = self->first_child; child; child = child->next_sibling) {
        if (child->y < header_h) {
            if (child->render_all) {
                child->render_all(child, ctx);
            } else {
                ygui_widget_render_all_default(child, ctx);
            }
        }
    }

    /* Render scrollable children */
    ctx->offset_x = old_offset_x + self->x - self->data.panel.scroll_x;
    ctx->offset_y = old_offset_y + self->y - self->data.panel.scroll_y;
    for (ygui_widget_t* child = self->first_child; child; child = child->next_sibling) {
        if (child->y >= header_h) {
            /* TODO: proper clipping */
            if (child->render_all) {
                child->render_all(child, ctx);
            } else {
                ygui_widget_render_all_default(child, ctx);
            }
        }
    }

    /* Restore context */
    ctx->offset_x = old_offset_x;
    ctx->offset_y = old_offset_y;
}

static int panel_on_scroll(ygui_widget_t* self, float dx, float dy, ygui_event_t* out) {
    (void)dx;
    float scrollable_h = self->h - self->data.panel.header_h;
    float max_scroll = ygui_max(0, self->data.panel.content_h - scrollable_h);
    float speed = 20.0f;  /* TODO: get from theme */

    self->data.panel.scroll_y = ygui_clamp(
        self->data.panel.scroll_y - dy * speed, 0, max_scroll);

    out->widget_id = self->id;
    out->type = YGUI_EVENT_SCROLL;
    out->data.scroll.x = self->data.panel.scroll_x;
    out->data.scroll.y = self->data.panel.scroll_y;
    return 1;
}

ygui_widget_t* ygui_panel(ygui_engine_t* engine, const char* id,
                          float x, float y, float w, float h) {
    ygui_widget_t* pnl = ygui_widget_alloc(engine, YGUI_WIDGET_PANEL, id);
    if (!pnl) return NULL;

    ygui_widget_init_base(pnl, x, y, w, h);
    pnl->data.panel.scroll_x = 0;
    pnl->data.panel.scroll_y = 0;
    pnl->data.panel.content_w = w;
    pnl->data.panel.content_h = h;
    pnl->data.panel.header_h = 0;
    pnl->data.panel.corner_radius = 0;
    pnl->render = panel_render;
    pnl->render_all = panel_render_all;
    pnl->on_scroll = panel_on_scroll;

    add_to_engine(engine, pnl);
    return pnl;
}

void ygui_panel_set_scroll(ygui_widget_t* widget, float x, float y) {
    if (!widget || widget->type != YGUI_WIDGET_PANEL) return;
    widget->data.panel.scroll_x = x;
    widget->data.panel.scroll_y = y;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_panel_get_scroll(const ygui_widget_t* widget, float* x, float* y) {
    if (!widget || widget->type != YGUI_WIDGET_PANEL) return;
    if (x) *x = widget->data.panel.scroll_x;
    if (y) *y = widget->data.panel.scroll_y;
}

void ygui_panel_set_content_size(ygui_widget_t* widget, float w, float h) {
    if (!widget || widget->type != YGUI_WIDGET_PANEL) return;
    widget->data.panel.content_w = w;
    widget->data.panel.content_h = h;
    if (widget->engine) widget->engine->dirty = 1;
}

void ygui_panel_set_header_height(ygui_widget_t* widget, float h) {
    if (!widget || widget->type != YGUI_WIDGET_PANEL) return;
    widget->data.panel.header_h = h;
    if (widget->engine) widget->engine->dirty = 1;
}

/*=============================================================================
 * Progress Widget
 *===========================================================================*/

static void progress_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    const ygui_theme_t* t = ctx->theme;

    /* Background track */
    ygui_render_box(ctx, self->x, self->y, self->w, self->h,
                    self->bg_color, t->radius_small);

    /* Filled portion */
    float pct = ygui_clamp(self->data.progress.value, 0, 1);
    float fill_w = pct * self->w;
    if (fill_w > 0) {
        ygui_render_box(ctx, self->x, self->y, fill_w, self->h,
                        self->accent_color, t->radius_small);
    }
}

ygui_widget_t* ygui_progress(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             float value) {
    ygui_widget_t* prg = ygui_widget_alloc(engine, YGUI_WIDGET_PROGRESS, id);
    if (!prg) return NULL;

    ygui_widget_init_base(prg, x, y, w, h);
    prg->data.progress.value = ygui_clamp(value, 0, 1);
    prg->render = progress_render;

    add_to_engine(engine, prg);
    return prg;
}

void ygui_progress_set_value(ygui_widget_t* widget, float value) {
    if (!widget || widget->type != YGUI_WIDGET_PROGRESS) return;
    widget->data.progress.value = ygui_clamp(value, 0, 1);
    if (widget->engine) widget->engine->dirty = 1;
}

float ygui_progress_get_value(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_PROGRESS) return 0;
    return widget->data.progress.value;
}

/*=============================================================================
 * Separator Widget
 *===========================================================================*/

static void separator_render(ygui_widget_t* self, ygui_render_ctx_t* ctx) {
    ygui_render_box(ctx, self->x, self->y, self->w, self->h,
                    ctx->theme->border, 0);
}

ygui_widget_t* ygui_separator(ygui_engine_t* engine, const char* id,
                              float x, float y, float w, float h) {
    ygui_widget_t* sep = ygui_widget_alloc(engine, YGUI_WIDGET_SEPARATOR, id);
    if (!sep) return NULL;

    ygui_widget_init_base(sep, x, y, w, h);
    sep->render = separator_render;

    add_to_engine(engine, sep);
    return sep;
}

/*=============================================================================
 * Stub implementations for remaining widgets
 * TODO: Implement fully
 *===========================================================================*/

ygui_widget_t* ygui_textinput(ygui_engine_t* engine, const char* id,
                              float x, float y, float w, float h,
                              const char* placeholder) {
    ygui_widget_t* txt = ygui_widget_alloc(engine, YGUI_WIDGET_TEXTINPUT, id);
    if (!txt) return NULL;

    ygui_widget_init_base(txt, x, y, w, h);
    txt->data.textinput.text = ygui_strdup("");
    txt->data.textinput.placeholder = ygui_strdup(placeholder);
    txt->data.textinput.cursor_pos = 0;

    add_to_engine(engine, txt);
    return txt;
}

void ygui_textinput_set_text(ygui_widget_t* widget, const char* text) {
    if (!widget || widget->type != YGUI_WIDGET_TEXTINPUT) return;
    free(widget->data.textinput.text);
    widget->data.textinput.text = ygui_strdup(text);
    if (widget->engine) widget->engine->dirty = 1;
}

const char* ygui_textinput_get_text(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_TEXTINPUT) return NULL;
    return widget->data.textinput.text;
}

void ygui_textinput_set_placeholder(ygui_widget_t* widget, const char* text) {
    if (!widget || widget->type != YGUI_WIDGET_TEXTINPUT) return;
    free(widget->data.textinput.placeholder);
    widget->data.textinput.placeholder = ygui_strdup(text);
    if (widget->engine) widget->engine->dirty = 1;
}

ygui_widget_t* ygui_hbox(ygui_engine_t* engine, const char* id,
                         float x, float y, float w, float h) {
    ygui_widget_t* hbox = ygui_widget_alloc(engine, YGUI_WIDGET_HBOX, id);
    if (!hbox) return NULL;
    ygui_widget_init_base(hbox, x, y, w, h);
    add_to_engine(engine, hbox);
    return hbox;
}

ygui_widget_t* ygui_vbox(ygui_engine_t* engine, const char* id,
                         float x, float y, float w, float h) {
    ygui_widget_t* vbox = ygui_widget_alloc(engine, YGUI_WIDGET_VBOX, id);
    if (!vbox) return NULL;
    ygui_widget_init_base(vbox, x, y, w, h);
    add_to_engine(engine, vbox);
    return vbox;
}

ygui_widget_t* ygui_dropdown(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             const char** options, int option_count) {
    ygui_widget_t* dd = ygui_widget_alloc(engine, YGUI_WIDGET_DROPDOWN, id);
    if (!dd) return NULL;
    ygui_widget_init_base(dd, x, y, w, h);
    /* TODO: copy options */
    (void)options;
    dd->data.dropdown.option_count = option_count;
    dd->data.dropdown.selected = 0;
    add_to_engine(engine, dd);
    return dd;
}

void ygui_dropdown_set_options(ygui_widget_t* widget,
                               const char** options, int count) {
    (void)widget; (void)options; (void)count;
    /* TODO */
}

void ygui_dropdown_set_selected(ygui_widget_t* widget, int index) {
    if (!widget || widget->type != YGUI_WIDGET_DROPDOWN) return;
    widget->data.dropdown.selected = index;
    if (widget->engine) widget->engine->dirty = 1;
}

int ygui_dropdown_get_selected(const ygui_widget_t* widget) {
    if (!widget || widget->type != YGUI_WIDGET_DROPDOWN) return 0;
    return widget->data.dropdown.selected;
}

ygui_widget_t* ygui_colorpicker(ygui_engine_t* engine, const char* id,
                                float x, float y, float w, float h) {
    ygui_widget_t* cp = ygui_widget_alloc(engine, YGUI_WIDGET_COLORPICKER, id);
    if (!cp) return NULL;
    ygui_widget_init_base(cp, x, y, w, h);
    cp->data.colorpicker.hue = 0;
    cp->data.colorpicker.sat = 1;
    cp->data.colorpicker.val = 1;
    cp->data.colorpicker.alpha = 1;
    add_to_engine(engine, cp);
    return cp;
}

void ygui_colorpicker_set_color(ygui_widget_t* widget,
                                float r, float g, float b, float a) {
    (void)widget; (void)r; (void)g; (void)b; (void)a;
    /* TODO: RGB to HSV conversion */
}

void ygui_colorpicker_get_color(const ygui_widget_t* widget,
                                float* r, float* g, float* b, float* a) {
    (void)widget;
    /* TODO: HSV to RGB conversion */
    if (r) *r = 1;
    if (g) *g = 1;
    if (b) *b = 1;
    if (a) *a = 1;
}
