# YGui-C Design Document

## Overview

YGui-C is a self-contained C widget library for yetty terminal. It handles everything internally:
- Widget management and rendering
- Event loop (via libuv)
- Terminal input parsing (stdin)
- OSC output to yetty

**Any language that can call C gets the full functionality.** No reimplementation of OSC, event loops, or input parsing in each language.

## Why Pure C API

1. **Universal FFI** - Every language calls C directly (Python ctypes, Rust bindgen, Go cgo, Lua FFI, Java JNI, etc.)
2. **Self-contained** - OSC output, event loop, input parsing all in C
3. **Zero boilerplate** - Language wrappers are thin and idiomatic
4. **Stable ABI** - C ABI is universal across compilers

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  User Application (any language)                            │
│  - Creates engine                                           │
│  - Adds widgets                                             │
│  - Registers callbacks                                      │
│  - Optionally adds timers/sockets to libuv loop             │
├─────────────────────────────────────────────────────────────┤
│  ygui-c (C library)                                         │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Event Loop (libuv)                                  │    │
│  │  - stdin watcher (OSC + keyboard input)             │    │
│  │  - User can add: timers, sockets, file watchers     │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Input Parser                                        │    │
│  │  - OSC 777777 → mouse click → widget callback       │    │
│  │  - OSC 777778 → mouse move  → widget callback       │    │
│  │  - Keypresses → keyboard    → user callback         │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Widget Engine                                       │    │
│  │  - Hit testing, state management                    │    │
│  │  - Renders to YDraw buffer                          │    │
│  │  - Auto-renders when dirty                          │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ OSC Output (to stdout)                              │    │
│  │  - First frame: run command with position           │    │
│  │  - Updates: serialized buffer data                  │    │
│  │  - Cleanup: kill command                            │    │
│  └─────────────────────────────────────────────────────┘    │
├─────────────────────────────────────────────────────────────┤
│  YDraw C API (wraps C++ YDrawBuffer)                        │
├─────────────────────────────────────────────────────────────┤
│  Yetty Terminal                                             │
│  - Receives OSC, renders card                               │
│  - Sends mouse events back via OSC 777777/777778            │
└─────────────────────────────────────────────────────────────┘
```

## YDraw Buffer: C API Wrapping C++ Backend

The YDraw C API is a **thin wrapper** around the C++ `YDrawBuffer` class, not a reimplementation:

```cpp
// ydraw-capi-wrapper.gen.cpp
struct ydraw_buffer {
    YDrawBuffer::Ptr impl;           // C++ implementation
    std::vector<uint8_t> serialized; // Cache for serialize()
};

extern "C" int32_t ydraw_add_box(ydraw_buffer_t* buf, ...) {
    return buf->impl->addBox(...);   // Delegates to C++
}
```

This gives us:
- Full C++ YDrawBuffer functionality
- C ABI for FFI from any language
- No code duplication

## Event Loop: libuv Integration

The event loop is powered by **libuv**, allowing users to integrate their own async events.

### Why libuv?

- Cross-platform async I/O
- File descriptor watchers (stdin)
- Timers, TCP/UDP, file watchers
- Used by Node.js - battle-tested
- User can add their own handlers to the same loop

### Usage Patterns

**Simple (ygui owns loop):**
```c
ygui_engine_t* e = ygui_engine_create("myapp", 400, 300);
ygui_button(e, "btn", 10, 10, 100, 40, "Click");
ygui_engine_show(e, 2, 2, 50, 18);
ygui_engine_run(e);  // Creates libuv loop internally
```

**Advanced (user owns loop):**
```c
uv_loop_t* loop = uv_default_loop();

ygui_engine_t* e = ygui_engine_create("myapp", 400, 300);
ygui_button(e, "btn", 10, 10, 100, 40, "Click");
ygui_engine_show(e, 2, 2, 50, 18);
ygui_engine_attach(e, loop);  // Adds stdin watcher to user's loop

// User adds their own handlers
uv_timer_t timer;
uv_timer_init(loop, &timer);
uv_timer_start(&timer, my_timer_cb, 1000, 1000);

uv_tcp_t socket;
uv_tcp_init(loop, &socket);
// ... networking code

uv_run(loop, UV_RUN_DEFAULT);  // User runs the loop
```

## stdin: ALL Terminal Input

When terminal is in raw mode, **stdin receives everything**:
- OSC sequences from yetty (`\033]777777;card;buttons;press;x;y\033\\`)
- User keypresses (`a`, `b`, `Enter`, etc.)
- Escape sequences (arrows, function keys)

The library parses all of this:

```
stdin bytes → parser → ┬→ OSC 777777 → mouse click → hit test → widget callback
                       ├→ OSC 777778 → mouse move  → hit test → widget callback
                       ├→ 'q'        → key event   → user key callback
                       ├→ ESC [ A    → arrow up    → user key callback
                       └→ etc.
```

## Widget Callbacks: High-Level API

**Users write elegant, high-level code.** The library handles all low-level details.

### C
```c
void on_click(ygui_widget_t* btn, void* data) {
    printf("Button clicked!\n");
}

void on_volume_change(ygui_widget_t* slider, float value, void* data) {
    printf("Volume: %.0f%%\n", value);
}

int main() {
    ygui_init();

    ygui_engine_t* e = ygui_engine_create("myapp", 400, 300);

    ygui_widget_t* btn = ygui_button(e, "btn", 10, 10, 100, 40, "Click Me");
    ygui_button_on_click(btn, on_click, NULL);

    ygui_widget_t* slider = ygui_slider(e, "vol", 10, 60, 200, 30, 0, 100, 50);
    ygui_slider_on_change(slider, on_volume_change, NULL);

    ygui_engine_show(e, 2, 2, 50, 18);
    ygui_engine_run(e);

    ygui_engine_destroy(e);
    return 0;
}
```

### Python
```python
import ygui

ygui.init()
engine = ygui.Engine("myapp", 400, 300)

btn = engine.button("btn", 10, 10, 100, 40, "Click Me")
btn.on_click(lambda: print("Button clicked!"))

slider = engine.slider("vol", 10, 60, 200, 30, 0, 100, 50)
slider.on_change(lambda v: print(f"Volume: {v:.0f}%"))

engine.show(2, 2, 50, 18)
engine.run()
```

### Go
```go
package main

import "ygui"

func main() {
    ygui.Init()
    engine := ygui.NewEngine("myapp", 400, 300)

    btn := engine.Button("btn", 10, 10, 100, 40, "Click Me")
    btn.OnClick(func() {
        fmt.Println("Button clicked!")
    })

    slider := engine.Slider("vol", 10, 60, 200, 30, 0, 100, 50)
    slider.OnChange(func(v float32) {
        fmt.Printf("Volume: %.0f%%\n", v)
    })

    engine.Show(2, 2, 50, 18)
    engine.Run()
}
```

### Rust
```rust
use ygui::{Engine, init};

fn main() {
    init();
    let engine = Engine::new("myapp", 400, 300);

    let btn = engine.button("btn", 10.0, 10.0, 100.0, 40.0, "Click Me");
    btn.on_click(|| println!("Button clicked!"));

    let slider = engine.slider("vol", 10.0, 60.0, 200.0, 30.0, 0.0, 100.0, 50.0);
    slider.on_change(|v| println!("Volume: {:.0}%", v));

    engine.show(2, 2, 50, 18);
    engine.run();
}
```

**Same pattern in every language.** User never touches:
- Raw coordinates
- Hit testing
- OSC parsing
- Buffer management
- Frame rendering

## Public API

### Initialization
```c
int ygui_init(void);      // Set up raw terminal mode, signal handlers
void ygui_shutdown(void); // Restore terminal
```

### Engine Lifecycle
```c
// Create engine with card name and pixel dimensions
ygui_engine_t* ygui_engine_create(const char* name, float width, float height);
void ygui_engine_destroy(ygui_engine_t* engine);

// Display card at terminal cell position
void ygui_engine_show(ygui_engine_t* engine, int x, int y, int w, int h);

// Event loop
void ygui_engine_attach(ygui_engine_t* engine, uv_loop_t* loop); // User's loop
void ygui_engine_run(ygui_engine_t* engine);  // Simple: creates loop internally
void ygui_engine_stop(ygui_engine_t* engine);
```

### Widget Creation
```c
ygui_widget_t* ygui_button(ygui_engine_t* e, const char* id,
                           float x, float y, float w, float h,
                           const char* label);

ygui_widget_t* ygui_label(ygui_engine_t* e, const char* id,
                          float x, float y, const char* text);

ygui_widget_t* ygui_slider(ygui_engine_t* e, const char* id,
                           float x, float y, float w, float h,
                           float min, float max, float value);

ygui_widget_t* ygui_checkbox(ygui_engine_t* e, const char* id,
                             float x, float y, float w, float h,
                             const char* label, int checked);

ygui_widget_t* ygui_textinput(ygui_engine_t* e, const char* id,
                              float x, float y, float w, float h,
                              const char* placeholder);

ygui_widget_t* ygui_progress(ygui_engine_t* e, const char* id,
                             float x, float y, float w, float h,
                             float value);

ygui_widget_t* ygui_panel(ygui_engine_t* e, const char* id,
                          float x, float y, float w, float h);
```

### Widget Callbacks
```c
// Callback types
typedef void (*ygui_click_callback_t)(ygui_widget_t* widget, void* userdata);
typedef void (*ygui_change_callback_t)(ygui_widget_t* widget, float value, void* userdata);
typedef void (*ygui_text_callback_t)(ygui_widget_t* widget, const char* text, void* userdata);

// Register callbacks
void ygui_button_on_click(ygui_widget_t* btn, ygui_click_callback_t cb, void* userdata);
void ygui_slider_on_change(ygui_widget_t* slider, ygui_change_callback_t cb, void* userdata);
void ygui_checkbox_on_change(ygui_widget_t* checkbox, ygui_change_callback_t cb, void* userdata);
void ygui_textinput_on_change(ygui_widget_t* input, ygui_text_callback_t cb, void* userdata);
```

### Global Keyboard Callback
```c
typedef void (*ygui_key_callback_t)(ygui_engine_t* engine, int key, int mods, void* userdata);
void ygui_engine_on_key(ygui_engine_t* engine, ygui_key_callback_t cb, void* userdata);
```

### Widget Properties
```c
void ygui_widget_set_visible(ygui_widget_t* w, int visible);
void ygui_widget_set_enabled(ygui_widget_t* w, int enabled);

void ygui_button_set_label(ygui_widget_t* w, const char* label);
void ygui_label_set_text(ygui_widget_t* w, const char* text);
void ygui_slider_set_value(ygui_widget_t* w, float value);
float ygui_slider_get_value(const ygui_widget_t* w);
void ygui_checkbox_set_checked(ygui_widget_t* w, int checked);
int ygui_checkbox_get_checked(const ygui_widget_t* w);
void ygui_progress_set_value(ygui_widget_t* w, float value);
```

## Internal Flow

### Frame Rendering (automatic)

When widget state changes:
1. Widget marks engine dirty
2. On next event loop iteration:
   - Clear YDraw buffer
   - Rebuild all widgets to buffer
   - Serialize buffer
   - Write OSC update to stdout

User never calls render manually. It's automatic.

### Mouse Event Flow

1. User clicks on card in yetty
2. Yetty sends: `\033]777777;cardname;1;1;150;80\033\\`
3. libuv stdin watcher fires
4. ygui parses OSC 777777: buttons=1, press=1, x=150, y=80
5. ygui hit tests: which widget at (150, 80)?
6. Found button "btn"
7. Update button state (pressed)
8. Call user's on_click callback
9. Mark dirty
10. Auto-render new frame

### Keyboard Event Flow

1. User presses 'q' in terminal
2. libuv stdin watcher fires
3. ygui sees regular character 'q' (not OSC)
4. Call user's on_key callback with key='q'

## Dependencies

- **libuv** - Event loop, stdin watcher
- **YDraw C++ backend** - Drawing primitives (via C wrapper)

## File Structure

```
src/yetty/ygui-c/
├── ygui.h                 # Public API header
├── ygui_internal.h        # Internal structures
├── ygui_engine.c          # Engine, event loop, OSC output
├── ygui_widgets.c         # Widget implementations
├── ygui_render.c          # Render to YDraw buffer
├── ygui_input.c           # stdin parsing (OSC + keyboard)
├── ygui_grid.c            # Spatial grid for hit testing
├── ygui_theme.c           # Theming
└── CMakeLists.txt

src/yetty/ydraw/
├── ydraw-capi.gen.h       # C API header (generated)
├── ydraw-capi-wrapper.gen.cpp  # C++ wrapper (generated)
└── ...

src/ygui-bindings/
├── python/ygui.py         # Thin Python wrapper
├── go/ygui.go             # Thin Go wrapper
├── rust/src/lib.rs        # Thin Rust wrapper
└── lua/ygui.lua           # Thin Lua wrapper
```

## OSC Protocol

### Terminal Modes (app → yetty)
```
\033[?1500h   - Subscribe to card click events
\033[?1500l   - Unsubscribe from card click events
\033[?1501h   - Subscribe to card move events
\033[?1501l   - Unsubscribe from card move events
```

### Card Commands (app → yetty via stdout)
```
\033]666666;run -c ydraw -x X -y Y -w W -h H --name NAME;;BASE64\033\\
\033]666666;update --name NAME;;BASE64\033\\
\033]666666;kill --name NAME\033\\
```

### Mouse Events (yetty → app via stdin)
```
\033]777777;card-name;buttons;press;x;y\033\\   (click: press=0 release, press=1 press)
\033]777778;card-name;buttons;x;y\033\\         (move)
```

Button bitmask: bit0=left(1), bit1=right(2), bit2=middle(4), bit3=scroll-up(8), bit4=scroll-down(16)

## Summary

The key design principle: **Everything in C, wrappers are thin.**

- libuv event loop in C
- stdin parsing in C
- OSC output in C
- Widget callbacks in C
- Auto-rendering in C

Language wrappers just expose the C API idiomatically. No logic duplication.

User writes clean, high-level code. Library handles everything else.
