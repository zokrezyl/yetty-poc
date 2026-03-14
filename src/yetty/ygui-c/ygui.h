/*
 * ygui.h - YGui C API
 *
 * Pure C widget library for interactive terminal UIs.
 * Renders via YDraw SDF primitives.
 */

#ifndef YGUI_H
#define YGUI_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * Library Initialization
 *===========================================================================*/

/* Initialize the library (sets up raw terminal mode, signal handlers)
 * Must be called before any other ygui function.
 * Returns 0 on success, -1 on error. */
int ygui_init(void);

/* Shutdown the library (restores terminal settings)
 * Should be called before exit. */
void ygui_shutdown(void);

/*=============================================================================
 * Opaque Types
 *===========================================================================*/

typedef struct ygui_engine ygui_engine_t;
typedef struct ygui_widget ygui_widget_t;
typedef struct ygui_theme ygui_theme_t;
typedef struct ydraw_buffer ydraw_buffer_t;

/*=============================================================================
 * Enums
 *===========================================================================*/

typedef enum {
    YGUI_WIDGET_BUTTON,
    YGUI_WIDGET_LABEL,
    YGUI_WIDGET_SLIDER,
    YGUI_WIDGET_CHECKBOX,
    YGUI_WIDGET_TEXTINPUT,
    YGUI_WIDGET_PANEL,
    YGUI_WIDGET_HBOX,
    YGUI_WIDGET_VBOX,
    YGUI_WIDGET_DROPDOWN,
    YGUI_WIDGET_LISTBOX,
    YGUI_WIDGET_TABLE,
    YGUI_WIDGET_TABBAR,
    YGUI_WIDGET_COLORPICKER,
    YGUI_WIDGET_SCROLLAREA,
    YGUI_WIDGET_PROGRESS,
    YGUI_WIDGET_SEPARATOR,
    YGUI_WIDGET_CUSTOM,
} ygui_widget_type_t;

typedef enum {
    YGUI_EVENT_NONE = 0,
    YGUI_EVENT_CLICK,
    YGUI_EVENT_PRESS,
    YGUI_EVENT_RELEASE,
    YGUI_EVENT_CHANGE,
    YGUI_EVENT_SCROLL,
    YGUI_EVENT_FOCUS,
    YGUI_EVENT_BLUR,
    YGUI_EVENT_KEY,
    YGUI_EVENT_TEXT,
} ygui_event_type_t;

typedef enum {
    YGUI_FLAG_NONE     = 0,
    YGUI_FLAG_HOVER    = 1 << 0,
    YGUI_FLAG_PRESSED  = 1 << 1,
    YGUI_FLAG_FOCUSED  = 1 << 2,
    YGUI_FLAG_DISABLED = 1 << 3,
    YGUI_FLAG_CHECKED  = 1 << 4,
    YGUI_FLAG_OPEN     = 1 << 5,
    YGUI_FLAG_VISIBLE  = 1 << 6,
} ygui_flags_t;

/*=============================================================================
 * Event Structure
 *===========================================================================*/

typedef struct {
    const char* widget_id;
    ygui_event_type_t type;
    union {
        float float_value;
        int32_t int_value;
        int bool_value;
        const char* string_value;
        struct { float r, g, b, a; } color;
        struct { float x, y; } scroll;
        struct { uint32_t key; int mods; } key;
    } data;
} ygui_event_t;

/*=============================================================================
 * Callbacks
 *===========================================================================*/

typedef void (*ygui_event_callback_t)(const ygui_event_t* event, void* userdata);

/*=============================================================================
 * Engine API
 *===========================================================================*/

/* Create/destroy */
ygui_engine_t* ygui_engine_create(ydraw_buffer_t* buffer);
void ygui_engine_destroy(ygui_engine_t* engine);

/* Configuration */
void ygui_engine_set_size(ygui_engine_t* engine, float width, float height);
void ygui_engine_set_cell_size(ygui_engine_t* engine, float cell_w, float cell_h);
void ygui_engine_set_theme(ygui_engine_t* engine, ygui_theme_t* theme);
void ygui_engine_set_event_callback(ygui_engine_t* engine,
                                     ygui_event_callback_t callback,
                                     void* userdata);

/* State */
int ygui_engine_is_dirty(const ygui_engine_t* engine);
void ygui_engine_mark_dirty(ygui_engine_t* engine);
void ygui_engine_rebuild(ygui_engine_t* engine);
void ygui_engine_clear(ygui_engine_t* engine);

/* Input handling */
void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y);
void ygui_engine_mouse_down(ygui_engine_t* engine, float x, float y, int button);
void ygui_engine_mouse_up(ygui_engine_t* engine, float x, float y, int button);
void ygui_engine_mouse_scroll(ygui_engine_t* engine, float x, float y,
                               float dx, float dy);
void ygui_engine_key_down(ygui_engine_t* engine, uint32_t key, int mods);
void ygui_engine_key_up(ygui_engine_t* engine, uint32_t key, int mods);
void ygui_engine_text_input(ygui_engine_t* engine, const char* text);

/* Widget lookup */
ygui_widget_t* ygui_engine_find(ygui_engine_t* engine, const char* id);
ygui_widget_t* ygui_engine_widget_at(ygui_engine_t* engine, float x, float y);

/*=============================================================================
 * Event Loop API (for card mouse events from yetty)
 *===========================================================================*/

/* Subscribe to card mouse events (sends DEC modes 1500/1501 to stdout) */
void ygui_engine_subscribe_clicks(ygui_engine_t* engine, int enable);
void ygui_engine_subscribe_moves(ygui_engine_t* engine, int enable);

/* Poll for events from stdin
 * timeout_ms: -1 = block forever, 0 = non-blocking, >0 = milliseconds
 * Returns: 1 if event processed, 0 if timeout, -1 on error */
int ygui_engine_poll(ygui_engine_t* engine, int timeout_ms);

/* Run event loop until ygui_engine_stop() called */
void ygui_engine_run(ygui_engine_t* engine);
void ygui_engine_stop(ygui_engine_t* engine);

/*=============================================================================
 * Widget Creation
 *===========================================================================*/

ygui_widget_t* ygui_button(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           const char* label);

ygui_widget_t* ygui_label(ygui_engine_t* engine, const char* id,
                          float x, float y, const char* text);

ygui_widget_t* ygui_slider(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           float min_val, float max_val, float value);

ygui_widget_t* ygui_checkbox(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             const char* label, int checked);

ygui_widget_t* ygui_textinput(ygui_engine_t* engine, const char* id,
                              float x, float y, float w, float h,
                              const char* placeholder);

ygui_widget_t* ygui_panel(ygui_engine_t* engine, const char* id,
                          float x, float y, float w, float h);

ygui_widget_t* ygui_hbox(ygui_engine_t* engine, const char* id,
                         float x, float y, float w, float h);

ygui_widget_t* ygui_vbox(ygui_engine_t* engine, const char* id,
                         float x, float y, float w, float h);

ygui_widget_t* ygui_dropdown(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             const char** options, int option_count);

ygui_widget_t* ygui_progress(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             float value);

ygui_widget_t* ygui_separator(ygui_engine_t* engine, const char* id,
                              float x, float y, float w, float h);

ygui_widget_t* ygui_colorpicker(ygui_engine_t* engine, const char* id,
                                float x, float y, float w, float h);

/*=============================================================================
 * Widget Hierarchy
 *===========================================================================*/

void ygui_widget_add_child(ygui_widget_t* parent, ygui_widget_t* child);
void ygui_widget_remove_child(ygui_widget_t* parent, ygui_widget_t* child);
void ygui_widget_remove(ygui_widget_t* widget);
ygui_widget_t* ygui_widget_parent(ygui_widget_t* widget);
ygui_widget_t* ygui_widget_first_child(ygui_widget_t* widget);
ygui_widget_t* ygui_widget_next_sibling(ygui_widget_t* widget);

/*=============================================================================
 * Widget Properties (Generic)
 *===========================================================================*/

const char* ygui_widget_id(const ygui_widget_t* widget);
ygui_widget_type_t ygui_widget_type(const ygui_widget_t* widget);

void ygui_widget_set_position(ygui_widget_t* widget, float x, float y);
void ygui_widget_get_position(const ygui_widget_t* widget, float* x, float* y);

void ygui_widget_set_size(ygui_widget_t* widget, float w, float h);
void ygui_widget_get_size(const ygui_widget_t* widget, float* w, float* h);

void ygui_widget_set_visible(ygui_widget_t* widget, int visible);
int ygui_widget_is_visible(const ygui_widget_t* widget);

void ygui_widget_set_enabled(ygui_widget_t* widget, int enabled);
int ygui_widget_is_enabled(const ygui_widget_t* widget);

uint32_t ygui_widget_get_flags(const ygui_widget_t* widget);

/* Styling */
void ygui_widget_set_bg_color(ygui_widget_t* widget, uint32_t color);
void ygui_widget_set_fg_color(ygui_widget_t* widget, uint32_t color);
void ygui_widget_set_accent_color(ygui_widget_t* widget, uint32_t color);

/*=============================================================================
 * Widget-Specific Properties
 *===========================================================================*/

/* Button */
void ygui_button_set_label(ygui_widget_t* widget, const char* label);
const char* ygui_button_get_label(const ygui_widget_t* widget);

/* Label */
void ygui_label_set_text(ygui_widget_t* widget, const char* text);
const char* ygui_label_get_text(const ygui_widget_t* widget);
void ygui_label_set_font_size(ygui_widget_t* widget, float size);

/* Slider */
void ygui_slider_set_value(ygui_widget_t* widget, float value);
float ygui_slider_get_value(const ygui_widget_t* widget);
void ygui_slider_set_range(ygui_widget_t* widget, float min_val, float max_val);

/* Checkbox */
void ygui_checkbox_set_checked(ygui_widget_t* widget, int checked);
int ygui_checkbox_get_checked(const ygui_widget_t* widget);
void ygui_checkbox_set_label(ygui_widget_t* widget, const char* label);

/* TextInput */
void ygui_textinput_set_text(ygui_widget_t* widget, const char* text);
const char* ygui_textinput_get_text(const ygui_widget_t* widget);
void ygui_textinput_set_placeholder(ygui_widget_t* widget, const char* text);

/* Panel */
void ygui_panel_set_scroll(ygui_widget_t* widget, float x, float y);
void ygui_panel_get_scroll(const ygui_widget_t* widget, float* x, float* y);
void ygui_panel_set_content_size(ygui_widget_t* widget, float w, float h);
void ygui_panel_set_header_height(ygui_widget_t* widget, float h);

/* Progress */
void ygui_progress_set_value(ygui_widget_t* widget, float value);
float ygui_progress_get_value(const ygui_widget_t* widget);

/* Dropdown */
void ygui_dropdown_set_options(ygui_widget_t* widget,
                               const char** options, int count);
void ygui_dropdown_set_selected(ygui_widget_t* widget, int index);
int ygui_dropdown_get_selected(const ygui_widget_t* widget);

/* ColorPicker */
void ygui_colorpicker_set_color(ygui_widget_t* widget,
                                float r, float g, float b, float a);
void ygui_colorpicker_get_color(const ygui_widget_t* widget,
                                float* r, float* g, float* b, float* a);

/*=============================================================================
 * Theme API
 *===========================================================================*/

ygui_theme_t* ygui_theme_create(void);
ygui_theme_t* ygui_theme_create_default(void);
void ygui_theme_destroy(ygui_theme_t* theme);

void ygui_theme_set_padding(ygui_theme_t* theme,
                            float small, float medium, float large);
void ygui_theme_set_radius(ygui_theme_t* theme,
                           float small, float medium, float large);
void ygui_theme_set_row_height(ygui_theme_t* theme, float height);
void ygui_theme_set_font_size(ygui_theme_t* theme, float size);
void ygui_theme_set_scrollbar_size(ygui_theme_t* theme, float size);

void ygui_theme_set_bg_primary(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_bg_surface(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_bg_hover(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_text_primary(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_text_muted(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_accent(ygui_theme_t* theme, uint32_t color);
void ygui_theme_set_border(ygui_theme_t* theme, uint32_t color);

/*=============================================================================
 * Error Handling
 *===========================================================================*/

const char* ygui_get_error(void);

/*=============================================================================
 * Version
 *===========================================================================*/

#define YGUI_VERSION_MAJOR 0
#define YGUI_VERSION_MINOR 1
#define YGUI_VERSION_PATCH 0

const char* ygui_version(void);

#ifdef __cplusplus
}
#endif

#endif /* YGUI_H */
