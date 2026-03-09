# ladybird-ydraw: LibWeb to YDraw Renderer

Renders web pages using Ladybird's LibWeb engine, outputting to yetty's YDraw buffer via OSC 666666 sequences.

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    ladybird-ydraw                           │
├─────────────────────────────────────────────────────────────┤
│  LibWeb (HTML/CSS parser, layout, JS engine)                │
│     │                                                       │
│     ▼                                                       │
│  YDrawPainter : Gfx::Painter                                │
│     │ fill_rect() → buffer.addBox()                         │
│     │ stroke_path() → buffer.addSegment()                   │
│     │ fill_path() → buffer.addBox() (tessellated)           │
│     ▼                                                       │
│  ydraw::Buffer (standalone serialization)                   │
│     │                                                       │
│     ▼                                                       │
│  OSC 666666 output → yetty terminal                         │
└─────────────────────────────────────────────────────────────┘
```

## Why LibWeb?

LibWeb is Ladybird's browser engine with:
- **Full HTML5/CSS3 support** (ongoing development)
- **JavaScript engine** (LibJS) - ES2023+ compliant
- **Modern layout algorithms** including flexbox, grid
- **MIT-compatible BSD-2-Clause license**

The key insight: LibWeb uses a `Gfx::Painter` abstraction with only 10 virtual methods:
```cpp
class Painter {
    virtual void clear_rect(...) = 0;
    virtual void fill_rect(...) = 0;
    virtual void draw_bitmap(...) = 0;
    virtual void stroke_path(...) = 0;  // 4 overloads
    virtual void fill_path(...) = 0;    // 3 overloads
    virtual void set_transform(...) = 0;
    virtual void save() = 0;
    virtual void restore() = 0;
    virtual void clip(...) = 0;
    virtual void reset() = 0;
};
```

We implement `YDrawPainter` that converts these calls to YDraw primitives.

## Build Requirements

Ladybird has significant dependencies:
- CMake 3.30+
- C++23 compiler (clang-21 or gcc-14)
- Rust toolchain
- vcpkg dependencies: Skia, SDL3, OpenSSL, ICU, etc.

### Option 1: Use Pre-built Ladybird

```bash
# Build Ladybird separately
cd /path/to/ladybird
./Meta/ladybird.py build

# Then build this tool linking to Ladybird libs
cd /path/to/yetty/tools/ladybird
cmake -B build \
    -DLADYBIRD_BUILD_DIR=/path/to/ladybird/Build/release
cmake --build build
```

### Option 2: Standalone Test Mode (No Ladybird)

```bash
# Build test mode only (no LibWeb dependency)
cd /path/to/yetty/tools/ladybird
g++ -std=c++23 -o ladybird-ydraw \
    main.cpp \
    ydraw-buffer-standalone.cpp \
    -DTEST_MODE_ONLY
./ladybird-ydraw --test | yetty
```

## Integration Path

Full LibWeb integration requires:

### 1. Initialize LibCore Event Loop
```cpp
Core::EventLoop event_loop;
```

### 2. Initialize LibJS VM
```cpp
auto& vm = Web::Bindings::main_thread_vm();
```

### 3. Set Up Resource Loader
LibWeb needs RequestServer for HTTP and ImageDecoder for images.
These are separate IPC processes in Ladybird's architecture.

```cpp
// Connect to RequestServer
auto request_client = TRY(Requests::RequestClient::try_create());
Web::ResourceLoader::initialize(request_client);
```

### 4. Create Web::Page with YDrawPainter

```cpp
// Create a custom PageClient that uses YDrawPainter
class YDrawPageClient : public Web::PageClient {
    void paint(Gfx::Bitmap& bitmap, ...) override {
        // Instead of painting to bitmap, paint to YDraw buffer
        ydraw::Buffer buffer;
        ydraw::YDrawPainter painter(buffer);
        // ... render via painter
    }
};
```

### 5. Load URL and Render

```cpp
page.load(URL::URL { url_string });
// Wait for load, then paint
```

## Simplification: Modify Ladybird Directly

The cleanest integration is to add a new backend to Ladybird itself:

1. Add `YDrawPaintingSurface` alongside `SkiaPaintingSurface`
2. Add `--ydraw-output` CLI flag to WebContent
3. Output OSC sequences instead of SharedBitmap

This would make `ladybird-ydraw` a thin wrapper around the existing Ladybird infrastructure.

## Current Status

- [x] YDraw buffer standalone serialization
- [x] YDrawPainter interface implementation
- [x] Test pattern output
- [ ] Full LibWeb integration
- [ ] RequestServer/ImageDecoder setup
- [ ] Font rendering via LibWeb's font system

## Usage

```bash
# Test mode (no dependencies)
./ladybird-ydraw --test

# Full mode (requires Ladybird)
./ladybird-ydraw https://example.com --width 800 --height 600
```

Output is OSC 666666 sequences - pipe to yetty or save to file.
