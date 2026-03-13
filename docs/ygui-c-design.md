# YGui-C Design Document

## Overview

YGui-C is a pure C implementation of the YGui widget system, designed for direct FFI bindings to multiple programming languages without wrapper layers.

## Why Pure C

1. **No wrapper boilerplate** - Every language can call C directly (Python ctypes, Rust bindgen, Go cgo, Lua FFI)
2. **Stable ABI** - C ABI is universal and stable across compilers
3. **Simpler mental model** - No vtables, no templates, no name mangling
4. **Smaller binary** - No C++ runtime

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  Language Bindings (thin, idiomatic)                        │
│  Python / Rust / Go / Lua / ...                             │
├─────────────────────────────────────────────────────────────┤
│  ygui.h - Public C API                                      │
├─────────────────────────────────────────────────────────────┤
│  ygui-c implementation                                      │
│    ├── ygui_engine.c      Engine + spatial grid             │
│    ├── ygui_widgets.c     Widget implementations            │
│    ├── ygui_render.c      Render context                    │
│    └── ygui_events.c      Event dispatch                    │
├─────────────────────────────────────────────────────────────┤
│  ydraw-c (or existing YDrawBuffer via C shim)               │
├─────────────────────────────────────────────────────────────┤
│  GPU (WebGPU)                                               │
└─────────────────────────────────────────────────────────────┘
```

## Core Types

### Opaque Handles

All internal structures are opaque to users:

```c
typedef struct ygui_engine ygui_engine_t;
typedef struct ygui_widget ygui_widget_t;
typedef struct ygui_theme ygui_theme_t;
```

### Widget Types

```c
typedef enum {
    YGUI_WIDGET_BUTTON,
    YGUI_WIDGET_LABEL,
    YGUI_WIDGET_SLIDER,
    YGUI_WIDGET_CHECKBOX,
    YGUI_WIDGET_PANEL,
    YGUI_WIDGET_HBOX,
    YGUI_WIDGET_VBOX,
    YGUI_WIDGET_TEXTINPUT,
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
```

### Events

```c
typedef enum {
    YGUI_EVENT_CLICK,
    YGUI_EVENT_CHANGE,
    YGUI_EVENT_SCROLL,
    YGUI_EVENT_FOCUS,
    YGUI_EVENT_BLUR,
    YGUI_EVENT_KEY,
} ygui_event_type_t;

typedef struct {
    const char* widget_id;
    ygui_event_type_t type;
    union {
        float float_value;
        int int_value;
        int bool_value;
        const char* string_value;
        struct { float r, g, b, a; } color_value;
    } data;
} ygui_event_t;
```

### Callbacks

```c
typedef void (*ygui_event_callback_t)(const ygui_event_t* event, void* userdata);
```

## Public API

### Engine Lifecycle

```c
// Create engine with a YDraw buffer
ygui_engine_t* ygui_engine_create(ydraw_buffer_t* buffer);
void ygui_engine_destroy(ygui_engine_t* engine);

// Configuration
void ygui_engine_set_size(ygui_engine_t* engine, float width, float height);
void ygui_engine_set_theme(ygui_engine_t* engine, const ygui_theme_t* theme);
void ygui_engine_set_event_callback(ygui_engine_t* engine,
                                     ygui_event_callback_t callback,
                                     void* userdata);

// Render
void ygui_engine_rebuild(ygui_engine_t* engine);
int ygui_engine_is_dirty(const ygui_engine_t* engine);
void ygui_engine_mark_dirty(ygui_engine_t* engine);
```

### Input Handling

```c
void ygui_engine_mouse_move(ygui_engine_t* engine, float x, float y);
void ygui_engine_mouse_down(ygui_engine_t* engine, float x, float y, int button);
void ygui_engine_mouse_up(ygui_engine_t* engine, float x, float y, int button);
void ygui_engine_mouse_scroll(ygui_engine_t* engine, float x, float y, float dx, float dy);
void ygui_engine_key_down(ygui_engine_t* engine, uint32_t key, int mods);
void ygui_engine_key_up(ygui_engine_t* engine, uint32_t key, int mods);
void ygui_engine_text_input(ygui_engine_t* engine, const char* text);
```

### Widget Creation

```c
// All widgets return an opaque handle
ygui_widget_t* ygui_button(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           const char* label);

ygui_widget_t* ygui_label(ygui_engine_t* engine, const char* id,
                          float x, float y, const char* text);

ygui_widget_t* ygui_slider(ygui_engine_t* engine, const char* id,
                           float x, float y, float w, float h,
                           float min, float max, float value);

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

ygui_widget_t* ygui_colorpicker(ygui_engine_t* engine, const char* id,
                                float x, float y, float w, float h);

ygui_widget_t* ygui_progress(ygui_engine_t* engine, const char* id,
                             float x, float y, float w, float h,
                             float value);
```

### Widget Manipulation

```c
// Hierarchy
void ygui_widget_add_child(ygui_widget_t* parent, ygui_widget_t* child);
void ygui_widget_remove_child(ygui_widget_t* parent, ygui_widget_t* child);
void ygui_widget_remove(ygui_widget_t* widget);

// Properties
void ygui_widget_set_position(ygui_widget_t* w, float x, float y);
void ygui_widget_set_size(ygui_widget_t* w, float width, float height);
void ygui_widget_set_visible(ygui_widget_t* w, int visible);
void ygui_widget_set_enabled(ygui_widget_t* w, int enabled);

// Styling
void ygui_widget_set_bg_color(ygui_widget_t* w, uint32_t color);
void ygui_widget_set_fg_color(ygui_widget_t* w, uint32_t color);
void ygui_widget_set_accent_color(ygui_widget_t* w, uint32_t color);

// Widget-specific setters
void ygui_button_set_label(ygui_widget_t* w, const char* label);
void ygui_slider_set_value(ygui_widget_t* w, float value);
void ygui_slider_set_range(ygui_widget_t* w, float min, float max);
void ygui_checkbox_set_checked(ygui_widget_t* w, int checked);
void ygui_textinput_set_text(ygui_widget_t* w, const char* text);
void ygui_progress_set_value(ygui_widget_t* w, float value);
void ygui_label_set_text(ygui_widget_t* w, const char* text);
void ygui_panel_set_scroll(ygui_widget_t* w, float x, float y);
void ygui_panel_set_content_size(ygui_widget_t* w, float w, float h);

// Widget-specific getters
float ygui_slider_get_value(const ygui_widget_t* w);
int ygui_checkbox_get_checked(const ygui_widget_t* w);
const char* ygui_textinput_get_text(const ygui_widget_t* w);
```

### Lookup

```c
ygui_widget_t* ygui_engine_find(ygui_engine_t* engine, const char* id);
ygui_widget_t* ygui_engine_widget_at(ygui_engine_t* engine, float x, float y);
```

### Theme

```c
ygui_theme_t* ygui_theme_create(void);
ygui_theme_t* ygui_theme_create_default(void);
void ygui_theme_destroy(ygui_theme_t* theme);

void ygui_theme_set_padding(ygui_theme_t* t, float small, float medium, float large);
void ygui_theme_set_radius(ygui_theme_t* t, float small, float medium, float large);
void ygui_theme_set_colors(ygui_theme_t* t,
                           uint32_t bg_primary,
                           uint32_t bg_surface,
                           uint32_t text_primary,
                           uint32_t accent);
```

## Internal Implementation

### Widget Structure

```c
struct ygui_widget {
    char* id;
    ygui_widget_type_t type;
    float x, y, w, h;
    float effective_x, effective_y;
    uint32_t flags;  // HOVER, PRESSED, FOCUSED, DISABLED, etc.
    uint32_t bg_color, fg_color, accent_color;

    ygui_widget_t* parent;
    ygui_widget_t* first_child;
    ygui_widget_t* next_sibling;

    // Function pointers for polymorphism
    void (*render)(ygui_widget_t* self, ygui_render_ctx_t* ctx);
    void (*on_event)(ygui_widget_t* self, ygui_input_event_t* event, ygui_event_t* out);
    void (*destroy)(ygui_widget_t* self);

    // Widget-specific data (union or flexible array member)
    union {
        struct { char* label; } button;
        struct { char* text; } label;
        struct { float value, min, max; } slider;
        struct { char* label; int checked; } checkbox;
        struct { char* text; char* placeholder; int cursor; } textinput;
        struct { float scroll_x, scroll_y, content_w, content_h, header_h; } panel;
        struct { char** options; int count; int selected; int open; } dropdown;
        struct { float value; } progress;
        struct { float hue, sat, val; } colorpicker;
    } data;
};
```

### Spatial Grid

```c
struct ygui_grid_cell {
    ygui_widget_t** widgets;
    int count;
    int capacity;
};

struct ygui_spatial_grid {
    ygui_grid_cell_t* cells;
    int cols, rows;
    float cell_size;
    float width, height;
};
```

### Engine Structure

```c
struct ygui_engine {
    ydraw_buffer_t* buffer;
    ygui_spatial_grid_t grid;
    ygui_theme_t* theme;

    ygui_widget_t* root_widgets;  // Linked list of top-level widgets
    int widget_count;

    ygui_widget_t* hovered;
    ygui_widget_t* pressed;
    ygui_widget_t* focused;

    ygui_event_callback_t event_callback;
    void* event_userdata;

    float width, height;
    float cell_width, cell_height;
    int dirty;
};
```

## Memory Management

1. **Engine owns widgets** - Widgets are destroyed when engine is destroyed
2. **Explicit removal** - `ygui_widget_remove()` removes and frees a widget
3. **No reference counting** - Simple ownership model
4. **String copies** - API copies strings, caller can free after call

## Error Handling

```c
// Functions that can fail return NULL or error code
ygui_engine_t* ygui_engine_create(ydraw_buffer_t* buffer);  // NULL on failure

// Error info
const char* ygui_get_error(void);  // Thread-local error message
```

## Thread Safety

- Engine is NOT thread-safe
- All calls to a single engine must be from one thread
- Different engines can be used from different threads

## File Structure

```
src/yetty/ygui-c/
├── ygui.h                 # Public API header
├── ygui_internal.h        # Internal structures
├── ygui_engine.c          # Engine implementation
├── ygui_widgets.c         # Widget implementations
├── ygui_render.c          # Render context (calls YDraw)
├── ygui_grid.c            # Spatial grid
├── ygui_theme.c           # Theme handling
└── CMakeLists.txt

src/ygui-bindings/
├── python/
│   ├── ygui.py            # ctypes wrapper
│   └── setup.py
├── rust/
│   ├── src/lib.rs         # Rust bindings
│   ├── build.rs           # bindgen setup
│   └── Cargo.toml
├── go/
│   ├── ygui.go            # cgo bindings
│   └── go.mod
└── lua/
    └── ygui.lua           # LuaJIT FFI bindings
```

## Usage Examples

### C

```c
#include "ygui.h"

void on_event(const ygui_event_t* e, void* userdata) {
    if (e->type == YGUI_EVENT_CLICK) {
        printf("Clicked: %s\n", e->widget_id);
    }
    if (e->type == YGUI_EVENT_CHANGE) {
        printf("Changed: %s = %f\n", e->widget_id, e->data.float_value);
    }
}

int main() {
    ydraw_buffer_t* buffer = ydraw_buffer_create();
    ygui_engine_t* engine = ygui_engine_create(buffer);
    ygui_engine_set_size(engine, 800, 600);
    ygui_engine_set_event_callback(engine, on_event, NULL);

    ygui_widget_t* panel = ygui_panel(engine, "main", 50, 50, 300, 400);
    ygui_widget_t* btn = ygui_button(engine, "ok", 10, 10, 80, 30, "OK");
    ygui_widget_t* slider = ygui_slider(engine, "vol", 10, 50, 200, 24, 0, 100, 50);

    ygui_widget_add_child(panel, btn);
    ygui_widget_add_child(panel, slider);

    // Main loop
    while (running) {
        ygui_engine_mouse_move(engine, mouse_x, mouse_y);
        // ... handle other input

        if (ygui_engine_is_dirty(engine)) {
            ygui_engine_rebuild(engine);
            // render buffer to screen
        }
    }

    ygui_engine_destroy(engine);
    ydraw_buffer_destroy(buffer);
}
```

### Python

```python
from ygui import Engine, Button, Slider, Panel

def on_event(event):
    if event.type == "click":
        print(f"Clicked: {event.widget_id}")
    elif event.type == "change":
        print(f"Changed: {event.widget_id} = {event.value}")

engine = Engine(width=800, height=600)
engine.on_event(on_event)

panel = Panel("main", x=50, y=50, w=300, h=400)
panel.add(Button("ok", x=10, y=10, w=80, h=30, label="OK"))
panel.add(Slider("vol", x=10, y=50, w=200, h=24, min=0, max=100, value=50))

engine.add(panel)
```

### Rust

```rust
use ygui::{Engine, Button, Slider, Panel, Event};

fn main() {
    let engine = Engine::new(800, 600);

    engine.on_event(|event| {
        match event {
            Event::Click { widget_id } => println!("Clicked: {}", widget_id),
            Event::Change { widget_id, value } => println!("Changed: {} = {}", widget_id, value),
            _ => {}
        }
    });

    let panel = Panel::new("main", 50.0, 50.0, 300.0, 400.0);
    panel.add(Button::new("ok", 10.0, 10.0, 80.0, 30.0, "OK"));
    panel.add(Slider::new("vol", 10.0, 50.0, 200.0, 24.0, 0.0, 100.0, 50.0));

    engine.add(panel);
}
```

## Migration Path

1. Implement ygui-c core with minimal widget set (button, label, slider, panel)
2. Create Python bindings and validate API usability
3. Add remaining widgets incrementally
4. Add Rust, Go, Lua bindings
5. Deprecate C++ YGui once ygui-c is feature-complete
