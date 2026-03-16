# Yetty WASM Card

Run WebAssembly programs as terminal cards with access to yetty's rendering system.

## Objectives

Allow users to write portable programs that:
- Run on any platform (desktop, web, Android)
- Have access to yetty's rich vector-based rendering (MSDF fonts, SDF shapes)
- Can be sent to the terminal via OSC sequences
- Execute sandboxed within the wasm3 interpreter

## Architecture

```
User program (C/C++) → wasm binary → ycat/OSC → yetty → wasm3 → YDrawBuilder → GPU
```

### Components

- **Runtime**: wasm3 interpreter v0.5.0
- **Card type**: `ywasm` in `src/yetty/cards/ywasm/`
- **Rendering**: Uses ydraw system (YDrawBuffer + YDrawBuilder)
- **Shader**: Reuses ydraw shader (`0x100003`)

## OSC Protocol

```bash
PAYLOAD=$(base64 -w0 < program.wasm)
printf '\033]666666;run -c ywasm -x 0 -y 0 -w 80 -h 24 -r;<args>;%s\033\\' "$PAYLOAD"
```

## WASM API

Programs import functions from the `yetty` module.

### Drawing (SDF primitives)

```c
void yetty_draw_rect(float x, float y, float w, float h, int32_t color);
void yetty_draw_circle(float cx, float cy, float r, int32_t color);
void yetty_draw_line(float x1, float y1, float x2, float y2, int32_t color, float width);
void yetty_draw_text(const char* text, float x, float y, float size, int32_t color);
void yetty_clear();
```

### Card info

```c
int32_t yetty_get_width();   // card width in cells
int32_t yetty_get_height();  // card height in cells
```

### Lifecycle

```c
void yetty_request_frame();  // request on_frame callback
void yetty_exit(int32_t code);
```

### Logging

```c
void yetty_log(const char* msg);
```

## Exports (called by yetty)

- `_start` or `main` — entry point (required)
- `on_frame(float dt)` — animation frame callback (optional)

## Example

```c
#include "yetty.h"

void _start(void) {
    yetty_log("Hello from WebAssembly!");
    yetty_draw_rect(10, 10, 200, 100, 0xFF0000FF);  // red
    yetty_draw_circle(300, 60, 40, 0x0000FFFF);     // blue
    yetty_draw_text("WASM!", 50, 50, 24, 0xFFFFFFFF);
    yetty_exit(0);
}
```

Build:
```bash
clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export=_start \
      -Wl,--allow-undefined -o hello.wasm hello.c
```

Run:
```bash
demo/scripts/cards/ywasm.sh hello.wasm
```

## Files

| File | Description |
|------|-------------|
| `build-tools/cmake/libs/wasm3.cmake` | wasm3 library integration |
| `src/yetty/cards/ywasm/ywasm.h` | Card header |
| `src/yetty/cards/ywasm/ywasm.cpp` | Card implementation |
| `demo/wasm/yetty.h` | C API header for wasm programs |
| `demo/wasm/hello.c` | Example program |
| `demo/wasm/build.sh` | Build script |
| `demo/scripts/cards/ywasm.sh` | Demo runner |

## TODO

### Drawing API
- [ ] `yetty_draw_triangle()`
- [ ] `yetty_draw_bezier()`
- [ ] `yetty_draw_polygon()`
- [ ] `yetty_draw_arc()`
- [ ] `yetty_draw_ellipse()`
- [ ] `yetty_set_stroke_color()` / `yetty_set_fill_color()`
- [ ] Font selection for text

### Input
- [ ] `yetty_poll_key()` — keyboard input
- [ ] `yetty_poll_mouse()` — mouse position and buttons
- [ ] `on_key(int key, int mods)` export
- [ ] `on_mouse(int event, float x, float y)` export

### Card control
- [ ] `yetty_set_size(cols, rows)` — resize card
- [ ] `yetty_set_title(text)` — set card title
- [ ] Background color control

### Advanced
- [ ] Spawn other cards (image, plot) from wasm
- [ ] Access terminal output (write to PTY)
- [ ] File/network I/O via WASI subset
- [ ] Shared memory for bulk data (textures)

### Tooling
- [ ] ycat integration for `.wasm` files
- [ ] Rust SDK
- [ ] AssemblyScript bindings
