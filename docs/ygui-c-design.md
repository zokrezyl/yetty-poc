# YGui-C Design Document

## Overview

YGui-C is a self-contained C widget library for yetty terminal. It handles everything internally:
- Widget management and rendering
- Event loop (via libuv)
- Terminal input parsing (stdin)
- OSC output to yetty

**Any language that can call C gets the full functionality.** No reimplementation of OSC, event loops, or input parsing in each language.

## Canvas vs Card (Viewport)

A key architectural concept is the separation between **canvas** and **card**:

- **Canvas**: The full UI surface where widgets are positioned. Can be arbitrarily large (e.g., 100000×100000 pixels for a 200-page scrollable document).
- **Card**: The visible viewport displayed in the terminal. Shows a portion of the canvas based on current scroll position and zoom level.

```
┌─────────────────────────────────────────────┐
│  Canvas (e.g., 500×2000 - scrollable doc)   │
│  ┌───────────────────────────────────────┐  │
│  │  Widget at (10, 10)                   │  │
│  │  Widget at (10, 100)                  │  │
│  ├───────────────────────────────────────┤◄─┼── Visible viewport (card)
│  │  Widget at (10, 300)  ← visible       │  │   e.g., 500×300 pixels
│  │  Widget at (10, 400)  ← visible       │  │
│  ├───────────────────────────────────────┤  │
│  │  Widget at (10, 600)                  │  │
│  │  ...more content below...             │  │
│  └───────────────────────────────────────┘  │
└─────────────────────────────────────────────┘
```

The entire canvas is serialized to the YDraw buffer and sent to yetty. The ydraw card handles:
- Displaying only the visible portion
- GPU-side zoom (no buffer regeneration needed)
- Scroll position management

This enables:
- Large scrollable UIs (e.g., document viewers, long forms)
- Zoom in/out without regenerating the buffer
- Efficient rendering (GPU handles viewport clipping)

## Coordinate Systems and Transformation

### Display vs Canvas Coordinates

Mouse events from yetty arrive in **display coordinates** (pixels within the card's visible area). These must be transformed to **canvas coordinates** for widget hit testing.

```
Display coords (0,0) to (display_w, display_h)
    │
    │  Transform considering:
    │  - Scroll offset (pan_x, pan_y)
    │  - Zoom level
    │  - Display size = card_cells × cell_pixels
    ▼
Canvas coords (visible region of canvas)
```

The transformation:
```c
// Display size = card dimensions in pixels
float display_w = card_w_cells * cell_width;
float display_h = card_h_cells * cell_height;

// Visible canvas region (depends on zoom/scroll)
float visible_w = canvas_w / zoom;
float visible_h = canvas_h / zoom;

// Transform display coords to canvas coords
canvas_x = scroll_x + (display_x / display_w) * visible_w;
canvas_y = scroll_y + (display_y / display_h) * visible_h;
```

### Cell Size Query (CSI 16 t)

Before creating the card, ygui-c must query the terminal's cell size in pixels:

```
App → Terminal:  \033[16t        (query cell size)
Terminal → App:  \033[6;H;Wt     (response: height H, width W in pixels)
```

This is required:
1. **Before creating the card** - to calculate display dimensions
2. **After terminal resize (SIGWINCH)** - cell pixel size may change

### Resize Handling (SIGWINCH)

**One engine per app**: A terminal app reads stdin exclusively, so only one ygui-c engine runs at a time. A single static SIGWINCH flag is sufficient.

When the terminal resizes (SIGWINCH), ygui-c re-queries cell size (CSI 16 t) and adjusts based on **two orthogonal flags**:

#### Canvas Mode
- **`YGUI_CANVAS_FIXED`**: Canvas size stays constant. Zoom/scroll changes viewport only (ydraw card handles it).
- **`YGUI_CANVAS_FIT`**: Canvas size always matches card pixel size (card_cells × cell_pixels).

#### Widget Mode
- **`YGUI_SCALE_OFF`**: Widgets keep their canvas positions/sizes. May clip if canvas shrinks.
- **`YGUI_SCALE_ON`**: Widgets scale/re-layout proportionally when canvas changes.

#### Combinations

| Canvas | Widgets | Behavior |
|--------|---------|----------|
| FIXED | OFF | Canvas constant, viewport changes on zoom. Widgets unchanged. |
| FIXED | ON | N/A (canvas doesn't change, nothing to scale) |
| FIT | OFF | Canvas = card pixels. Widgets keep sizes, may clip. |
| FIT | ON | Canvas = card pixels. Widgets re-layout to fit. |

```c
typedef enum {
    YGUI_CANVAS_FIXED,  // Canvas size stays constant
    YGUI_CANVAS_FIT     // Canvas size = card pixel size
} ygui_canvas_mode_t;

typedef enum {
    YGUI_SCALE_OFF,     // Widgets keep positions/sizes
    YGUI_SCALE_ON       // Widgets scale with canvas
} ygui_scale_mode_t;

void ygui_engine_set_canvas_mode(ygui_engine_t* e, ygui_canvas_mode_t mode);
void ygui_engine_set_scale_mode(ygui_engine_t* e, ygui_scale_mode_t mode);
```

On resize with `YGUI_CANVAS_FIT` + `YGUI_SCALE_ON`:
```c
// reference_w, reference_h = initial display size (stored at startup)
float new_display_w = card_w_cells * cell_width;
float new_display_h = card_h_cells * cell_height;
float scale_x = new_display_w / reference_w;
float scale_y = new_display_h / reference_h;

// Scale all widget positions/sizes
for (widget = first_widget; widget; widget = widget->next) {
    widget->x *= scale_x;
    widget->y *= scale_y;
    widget->w *= scale_x;
    widget->h *= scale_y;
}
```

## Why Pure C API

1. **Universal FFI** - Every language calls C directly (Python ctypes, Rust bindgen, Go cgo, Lua FFI, Java JNI, etc.)
2. **Self-contained** - OSC output, event loop, input parsing all in C
3. **Zero boilerplate** - Language wrappers are thin and idiomatic
4. **Stable ABI** - C ABI is universal across compilers

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  User Application (any language)                            │
│  - Creates engine with canvas size                          │
│  - Adds widgets (positioned in canvas coordinates)          │
│  - Registers callbacks                                      │
│  - Optionally adds timers/sockets to libuv loop             │
├─────────────────────────────────────────────────────────────┤
│  ygui-c (C library)                                         │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Event Loop (libuv)                                  │    │
│  │  - stdin watcher (OSC + keyboard input)             │    │
│  │  - SIGWINCH handler (terminal resize)               │    │
│  │  - User can add: timers, sockets, file watchers     │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Input Parser                                        │    │
│  │  - CSI 6;H;Wt  → cell size response                 │    │
│  │  - OSC 777777  → mouse click (display coords)       │    │
│  │  - OSC 777778  → mouse move  (display coords)       │    │
│  │  - OSC 777779  → view change (zoom/scroll)           │    │
│  │  - Keypresses  → keyboard → user callback           │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Coordinate Transform                                │    │
│  │  - Display coords → Canvas coords                   │    │
│  │  - Uses: cell size, card cells, zoom, scroll        │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ Widget Engine                                       │    │
│  │  - Hit testing in canvas coordinates                │    │
│  │  - State management                                 │    │
│  │  - Renders full canvas to YDraw buffer              │    │
│  │  - Auto-renders when dirty                          │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌─────────────────────────────────────────────────────┐    │
│  │ OSC Output (to stdout)                              │    │
│  │  - Query: CSI 16 t (cell size)                      │    │
│  │  - First frame: run command with card position      │    │
│  │  - Updates: serialized buffer (full canvas)         │    │
│  │  - Cleanup: kill command                            │    │
│  └─────────────────────────────────────────────────────┘    │
├─────────────────────────────────────────────────────────────┤
│  YDraw C API (wraps C++ YDrawBuffer)                        │
│  - Serializes full canvas with scene bounds                 │
├─────────────────────────────────────────────────────────────┤
│  Yetty Terminal (ydraw card)                                │
│  - Receives serialized buffer                               │
│  - Renders visible portion (viewport) of canvas             │
│  - GPU-side zoom/scroll (no buffer regeneration)            │
│  - Sends mouse events with display coordinates              │
│  - Sends view change events on user zoom/scroll             │
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
ygui_init();  // Sets up terminal, queries cell size

// Canvas size 400x300 - widgets positioned in this coordinate space
ygui_engine_t* e = ygui_engine_create("myapp", 400, 300);
ygui_button(e, "btn", 10, 10, 100, 40, "Click");

// Card at cell (2,2), size 50x18 cells
// Display size = 50*cell_w x 18*cell_h pixels (viewport into canvas)
ygui_engine_show(e, 2, 2, 50, 18);
ygui_engine_run(e);  // Creates libuv loop internally
```

**Scrollable content:**
```c
ygui_init();

// Large canvas for scrollable content (e.g., 400 wide x 2000 tall)
ygui_engine_t* e = ygui_engine_create("myapp", 400, 2000);

// Add many widgets down the canvas
ygui_button(e, "btn1", 10, 10, 100, 40, "Top");
ygui_button(e, "btn2", 10, 500, 100, 40, "Middle");
ygui_button(e, "btn3", 10, 1900, 100, 40, "Bottom");

// Card shows viewport - user can scroll within yetty
ygui_engine_show(e, 2, 2, 50, 18);
ygui_engine_run(e);
```

**Advanced (user owns loop):**
```c
uv_loop_t* loop = uv_default_loop();
ygui_init();

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
- CSI responses from yetty (`\033[6;H;Wt` cell size)
- OSC sequences from yetty (`\033]777777;card;buttons;press;x;y\033\\`)
- User keypresses (`a`, `b`, `Enter`, etc.)
- Escape sequences (arrows, function keys)

The library parses all of this:

```
stdin bytes → parser → ┬→ CSI 6;H;Wt → cell size   → store for coord transform
                       ├→ OSC 777777 → mouse click → transform → hit test → callback
                       ├→ OSC 777778 → mouse move  → transform → hit test → callback
                       ├→ OSC 777779 → view change → update zoom/scroll state
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
// Create engine with card name and canvas dimensions
// Canvas can be larger than visible area for scrollable content
ygui_engine_t* ygui_engine_create(const char* name, float canvas_w, float canvas_h);
void ygui_engine_destroy(ygui_engine_t* engine);

// Display card at terminal cell position
// Queries cell size (CSI 16 t) before creating the card
// w, h are in terminal cells (not pixels)
void ygui_engine_show(ygui_engine_t* engine, int x, int y, int w, int h);

// Event loop
void ygui_engine_attach(ygui_engine_t* engine, uv_loop_t* loop); // User's loop
void ygui_engine_run(ygui_engine_t* engine);  // Simple: creates loop internally
void ygui_engine_stop(ygui_engine_t* engine);

// Resize handling
typedef enum {
    YGUI_RESIZE_FIXED,   // Keep widget pixel sizes, may clip
    YGUI_RESIZE_SCALED   // Scale widgets proportionally
} ygui_resize_mode_t;

void ygui_engine_set_resize_mode(ygui_engine_t* engine, ygui_resize_mode_t mode);
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
4. ygui parses OSC 777777: buttons=1, press=1, x=150, y=80 (display coords)
5. ygui transforms to canvas coords considering zoom/scroll
6. ygui hit tests: which widget at canvas position?
7. Found button "btn"
8. Update button state (pressed)
9. Call user's on_click callback
10. Mark dirty
11. Auto-render new frame

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
\033[?1502h   - Subscribe to card view change events (zoom/scroll)
\033[?1502l   - Unsubscribe from card view change events
```

### Terminal Queries (app → yetty)
```
\033[16t     - Query cell size in pixels
             Response: \033[6;HEIGHT;WIDTHt
```

### Card Commands (app → yetty via stdout)
```
\033]666666;run -c ydraw -x X -y Y -w W -h H --name NAME;;BASE64\033\\
\033]666666;update --name NAME;;BASE64\033\\
\033]666666;kill --name NAME\033\\

# Zoom command
\033]666666;zoom --name NAME --level 1.5\033\\
  --level  Zoom factor (1.0 = 100%, 2.0 = 200%)

# Scroll command
\033]666666;scroll --name NAME -x 100 -y 200\033\\       (absolute)
\033]666666;scroll --name NAME --dx 50 --dy -30\033\\    (relative)
\033]666666;scroll --name NAME -x 100 --dy -30\033\\     (combined)
  -x/-y    Absolute position in canvas pixels
  --dx/--dy  Relative delta from current position
```

### Mouse Events (yetty → app via stdin)
```
\033]777777;card-name;buttons;press;x;y\033\\   (click: press=0 release, press=1 press)
\033]777778;card-name;buttons;x;y\033\\         (move)
```

Button bitmask: bit0=left(1), bit1=right(2), bit2=middle(4), bit3=scroll-up(8), bit4=scroll-down(16)

Coordinates x,y are in display pixels (card surface). Transform to canvas coords using current zoom/scroll.

### View Change Events (yetty → app via stdin)
```
\033]777779;card-name;zoom;scroll-x;scroll-y\033\\   (view changed)
```

Sent when the **user** interacts with the card (Ctrl+scroll to zoom, scroll to pan). Not sent as acknowledgment for app-initiated zoom/scroll commands.

Requires subscription: `\033[?1502h` (DEC mode 1502)

The app uses this to:
- Update coordinate transformation for hit testing
- Adjust widget behavior based on visible area
- Implement lazy loading for large canvases

## Summary

The key design principles:

1. **Everything in C, wrappers are thin**
   - libuv event loop in C
   - stdin parsing in C
   - OSC output in C
   - Widget callbacks in C
   - Auto-rendering in C

2. **Canvas/viewport separation**
   - Canvas: full UI, can be arbitrarily large
   - Card: visible viewport with zoom/scroll
   - GPU handles viewport clipping efficiently

3. **Coordinate transformation**
   - Mouse events arrive in display coords
   - Transform to canvas coords using zoom/scroll state
   - Query cell size before creating card (CSI 16 t)
   - Re-query on terminal resize (SIGWINCH)

4. **Resize modes**
   - Fixed: widgets keep pixel sizes
   - Scaled: widgets scale with viewport

Language wrappers just expose the C API idiomatically. No logic duplication.

User writes clean, high-level code. Library handles everything else.
