# Ladybird LibWeb to YDraw Integration

## 1. Overview

### 1.1 Goal

Create a standalone executable `libweb-to-ydraw` that:
1. Takes an HTTP/HTTPS URL as input
2. Downloads HTML and all referenced assets (CSS, images, fonts)
3. Parses and renders the page using Ladybird's LibWeb engine
4. Outputs YDraw primitives via OSC 666666 sequences for display in yetty terminal

### 1.2 Why LibWeb?

LibWeb is Ladybird's web rendering engine with:
- Full HTML5 parser (spec-compliant)
- Complete CSS3 support (Flexbox, Grid, etc.)
- Modern layout engine
- JavaScript support via LibJS (optional for static pages)

Unlike simpler alternatives (litehtml), LibWeb handles real-world websites correctly.

## 2. LibWeb Architecture

### 2.1 Rendering Pipeline

```
HTML Source
    │
    ▼
┌─────────────────┐
│   HTMLParser    │  Parse HTML into DOM tree
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│    DOM Tree     │  Document Object Model
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   CSS Parser    │  Parse stylesheets
│   + Cascade     │  Compute styles for each element
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Layout Engine  │  Compute box positions/sizes
│  (BFC, IFC,     │  Block/Inline/Flex/Grid formatting
│   Flex, Grid)   │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Paintable Tree  │  Visual representation of layout
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ DisplayList     │  List of drawing commands
│ Recorder        │  (FillRect, DrawGlyphRun, etc.)
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ DisplayList     │  Execute commands to backend
│ Player          │  (Skia, or our YDraw)
└────────┬────────┘
         │
         ▼
     Rendered Output
```

### 2.2 Key Components

| Component | Location | Purpose |
|-----------|----------|---------|
| AK | `AK/` | Standard library (String, Vector, RefPtr) |
| LibGfx | `Libraries/LibGfx/` | Graphics primitives (Color, Rect, Path) |
| LibCore | `Libraries/LibCore/` | Event loop, I/O |
| LibURL | `Libraries/LibURL/` | URL parsing |
| LibTextCodec | `Libraries/LibTextCodec/` | Character encoding |
| LibWeb/HTML | `Libraries/LibWeb/HTML/` | HTML parser, DOM |
| LibWeb/CSS | `Libraries/LibWeb/CSS/` | CSS parser, cascade |
| LibWeb/Layout | `Libraries/LibWeb/Layout/` | Layout algorithms |
| LibWeb/Painting | `Libraries/LibWeb/Painting/` | DisplayList, Paintables |

### 2.3 DisplayList System

LibWeb records all drawing operations into a `DisplayList`:

```cpp
// Libraries/LibWeb/Painting/DisplayListCommand.h
using DisplayListCommand = Variant<
    DrawGlyphRun,           // Text rendering
    FillRect,               // Solid rectangle
    FillRectWithRoundedCorners,
    DrawLine,
    FillEllipse,
    DrawEllipse,
    FillPath,               // SVG paths
    StrokePath,
    PaintLinearGradient,
    PaintRadialGradient,
    PaintConicGradient,
    PaintOuterBoxShadow,
    PaintInnerBoxShadow,
    DrawScaledImmutableBitmap,  // Images
    Save, Restore,          // State management
    Translate,
    AddClipRect,
    AddRoundedRectClip,
    // ... more
>;
```

The `DisplayListPlayer` abstract class executes these commands:

```cpp
// Libraries/LibWeb/Painting/DisplayList.h
class DisplayListPlayer {
    virtual void fill_rect(FillRect const&) = 0;
    virtual void draw_glyph_run(DrawGlyphRun const&) = 0;
    virtual void draw_line(DrawLine const&) = 0;
    virtual void fill_ellipse(FillEllipse const&) = 0;
    virtual void fill_rect_with_rounded_corners(FillRectWithRoundedCorners const&) = 0;
    virtual void paint_linear_gradient(PaintLinearGradient const&) = 0;
    // ... ~25 virtual methods total
};
```

Ladybird implements `DisplayListPlayerSkia` which renders to Skia surfaces.

## 3. Integration Design

### 3.1 Our Implementation: DisplayListPlayerYDraw

We implement `DisplayListPlayerYDraw` that converts LibWeb commands to YDraw primitives:

```cpp
class DisplayListPlayerYDraw : public Web::Painting::DisplayListPlayer {
public:
    DisplayListPlayerYDraw(ydraw::Buffer& buffer);

private:
    // Convert LibWeb commands to YDraw primitives
    void fill_rect(FillRect const& cmd) override {
        _buffer.addBox(cmd.rect.x(), cmd.rect.y(),
                       cmd.rect.width(), cmd.rect.height(),
                       toYDrawColor(cmd.color));
    }

    void draw_glyph_run(DrawGlyphRun const& cmd) override {
        // Extract text from glyph run
        // _buffer.addText(...)
    }

    void draw_line(DrawLine const& cmd) override {
        _buffer.addSegment(cmd.from.x(), cmd.from.y(),
                          cmd.to.x(), cmd.to.y(),
                          cmd.thickness, toYDrawColor(cmd.color));
    }

    // ... implement all virtual methods

    ydraw::Buffer& _buffer;
    std::vector<ClipRect> _clipStack;
    Gfx::IntPoint _translation;
};
```

### 3.2 Command Mapping

| LibWeb Command | YDraw Primitive | Notes |
|----------------|-----------------|-------|
| `FillRect` | `addBox` | Direct mapping |
| `FillRectWithRoundedCorners` | `addRoundedBox` | Direct mapping |
| `DrawLine` | `addSegment` | Direct mapping |
| `FillEllipse` | `addCircle` | Use larger dimension for radius |
| `DrawEllipse` | `addArc` | Stroke only |
| `DrawGlyphRun` | `addText` | Extract text string from glyphs |
| `PaintLinearGradient` | `addBox` | Simplified: use average color |
| `PaintRadialGradient` | `addCircle` | Simplified: use center color |
| `FillPath` | Multiple `addSegment` | Decompose path to segments |
| `StrokePath` | Multiple `addSegment` | Decompose path to segments |
| `DrawScaledImmutableBitmap` | `addBox` | Placeholder rectangle |
| `PaintOuterBoxShadow` | `addRoundedBox` | Larger, semi-transparent |
| `Save`/`Restore` | State stack | Track transforms, clips |
| `Translate` | Offset accumulator | Apply to all coordinates |
| `AddClipRect` | Clip stack | Filter primitives by clip |

### 3.3 Limitations

YDraw is an SDF-based 2D renderer optimized for terminal UI, not full web rendering:

1. **No images**: Bitmaps shown as colored placeholders
2. **Simplified gradients**: Use single/average color
3. **Limited paths**: Complex SVG decomposed to line segments
4. **No filters**: Blur, drop-shadow approximated
5. **No blend modes**: Standard alpha compositing only

These limitations are acceptable for basic web page visualization in terminal.

## 4. Build Architecture

### 4.1 Directory Structure

```
tools/ladybird/
├── CMakeLists.txt              # Main build configuration
├── AK/                         # Ladybird's standard library
│   ├── *.h                     # Headers (copied from Ladybird)
│   └── *.cpp                   # Sources
├── LibGfx/                     # Graphics primitives
│   ├── Color.h, Rect.h, etc.
│   └── *.cpp
├── LibCore/                    # Core utilities (minimal)
├── LibURL/                     # URL parsing
├── LibTextCodec/               # Character encoding
├── LibWeb/                     # Web engine
│   ├── HTML/                   # HTML parser
│   ├── CSS/                    # CSS parser + cascade
│   ├── DOM/                    # Document Object Model
│   ├── Layout/                 # Layout engine
│   └── Painting/               # DisplayList, Paintables
├── DisplayListPlayerYDraw.h    # Our YDraw backend
├── DisplayListPlayerYDraw.cpp
├── ydraw-buffer-standalone.h   # Standalone YDraw buffer
├── ydraw-buffer-standalone.cpp
├── http-fetcher.h              # HTTP client (libcurl)
└── libweb-to-ydraw.cpp         # Main executable
```

### 4.2 Build Dependencies

**From Ladybird (copied sources):**
- AK (all)
- LibGfx (core types, no Skia)
- LibCore (minimal: EventLoop, File)
- LibURL
- LibTextCodec
- LibUnicode (minimal)
- LibWeb (HTML, CSS, DOM, Layout, Painting)

**External dependencies:**
- libcurl (HTTP fetching)
- ICU (Unicode, required by LibWeb)

**NOT needed:**
- LibJS (JavaScript - skip for static rendering)
- LibWasm (WebAssembly)
- LibMedia (Audio/Video)
- Skia (we replace with YDraw)

### 4.3 CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.20)
project(libweb-to-ydraw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Find dependencies
find_package(CURL REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n data)

# AK library
file(GLOB AK_SOURCES AK/*.cpp)
add_library(ak STATIC ${AK_SOURCES})
target_include_directories(ak PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# LibGfx (minimal, no Skia)
file(GLOB LIBGFX_SOURCES LibGfx/*.cpp)
list(FILTER LIBGFX_SOURCES EXCLUDE REGEX ".*Skia.*")
add_library(gfx STATIC ${LIBGFX_SOURCES})
target_link_libraries(gfx PUBLIC ak)

# LibCore (minimal)
add_library(core STATIC
    LibCore/EventLoop.cpp
    LibCore/File.cpp
    # ... minimal set
)
target_link_libraries(core PUBLIC ak)

# LibURL
file(GLOB LIBURL_SOURCES LibURL/*.cpp)
add_library(url STATIC ${LIBURL_SOURCES})
target_link_libraries(url PUBLIC ak)

# LibWeb (HTML, CSS, Layout, Painting)
file(GLOB_RECURSE LIBWEB_SOURCES
    LibWeb/HTML/*.cpp
    LibWeb/CSS/*.cpp
    LibWeb/DOM/*.cpp
    LibWeb/Layout/*.cpp
    LibWeb/Painting/*.cpp
)
# Exclude Skia player
list(FILTER LIBWEB_SOURCES EXCLUDE REGEX ".*Skia.*")
add_library(web STATIC ${LIBWEB_SOURCES})
target_link_libraries(web PUBLIC gfx core url ICU::uc ICU::i18n)

# YDraw buffer standalone
add_library(ydraw-buffer STATIC
    ydraw-buffer-standalone.cpp
)

# DisplayListPlayerYDraw
add_library(display-list-player-ydraw STATIC
    DisplayListPlayerYDraw.cpp
)
target_link_libraries(display-list-player-ydraw PUBLIC web ydraw-buffer)

# Main executable
add_executable(libweb-to-ydraw
    libweb-to-ydraw.cpp
)
target_link_libraries(libweb-to-ydraw PRIVATE
    display-list-player-ydraw
    web
    CURL::libcurl
)
```

## 5. Implementation Plan

### Phase 1: Build Foundation
1. Copy AK library sources
2. Create CMakeLists.txt, verify AK compiles standalone
3. Copy LibGfx core types (Color, Rect, Point, Path)
4. Stub out Skia dependencies

### Phase 2: Core Libraries
1. Copy LibCore minimal subset
2. Copy LibURL
3. Copy LibTextCodec
4. Verify libraries link together

### Phase 3: LibWeb Painting
1. Copy LibWeb/Painting sources
2. Implement DisplayListPlayerYDraw
3. Create test with hardcoded DisplayList commands
4. Verify YDraw output works

### Phase 4: HTML/CSS Parser
1. Copy LibWeb/HTML parser
2. Copy LibWeb/CSS parser and cascade
3. Copy LibWeb/DOM
4. Test parsing simple HTML

### Phase 5: Layout Engine
1. Copy LibWeb/Layout
2. Integrate layout with Painting
3. Test full pipeline: HTML → DOM → Layout → DisplayList → YDraw

### Phase 6: HTTP Integration
1. Add HTTP fetching with libcurl
2. Handle relative URLs for assets
3. Test with real websites

### Phase 7: Polish
1. Add command-line options
2. Handle errors gracefully
3. Optimize performance
4. Add ytrace logging

## 6. Usage

```bash
# Build
cd tools/ladybird
mkdir build && cd build
cmake .. && make

# Run
./libweb-to-ydraw https://example.com --width 800 --height 600

# Output goes to stdout as OSC 666666 sequence
# Pipe to yetty or use in ydraw card
```

## 7. Testing Strategy

### Unit Tests
- DisplayListPlayerYDraw command mapping
- Coordinate transforms
- Clip region handling

### Integration Tests
- Simple HTML pages (headings, paragraphs, links)
- CSS layouts (flexbox, grid)
- Complex pages (news sites, documentation)

### Visual Tests
- Compare YDraw output with browser screenshots
- Verify text positioning
- Check color accuracy

## 8. Current State and Cleanup

### 8.1 Files to DELETE (wrong approach using litehtml)

These files use litehtml which is NOT LibWeb:

```
tools/ladybird/
├── litehtml-container.h      # DELETE - litehtml container
├── html-to-ydraw.cpp         # DELETE - uses litehtml
├── http-fetch.h              # KEEP - useful for HTTP fetching
```

### 8.2 Files to KEEP (standalone YDraw buffer)

```
tools/ladybird/
├── ydraw-buffer-standalone.h     # KEEP - standalone YDraw buffer
├── ydraw-buffer-standalone.cpp   # KEEP - standalone YDraw buffer
├── display-list-standalone.h     # DELETE - replaced by real LibWeb
├── display-list-player-ydraw-standalone.h  # DELETE - replaced by real impl
├── display-list-io.h             # DELETE - not needed
├── main.cpp                      # REPLACE - test harness, will rewrite
├── web-to-ydraw.cpp              # DELETE - wrong approach
```

### 8.3 Source Location

Ladybird sources are in: `/home/misi/work/my/yetty/tmp/ladybird/`

Key directories:
- `AK/` - Standard library
- `Libraries/LibGfx/` - Graphics
- `Libraries/LibWeb/Painting/` - DisplayList system

## 9. Future Enhancements

1. **JavaScript support**: Enable LibJS for dynamic pages
2. **Image rendering**: Integrate with yetty's image card system
3. **Incremental rendering**: Stream DisplayList commands
4. **Caching**: Cache parsed documents and assets
5. **Better gradients**: Approximate gradients with multiple rectangles

## 10. Current Implementation Status

### What Works (as of 2025-03)

**Working Tools** (in `tools/ladybird/`):

1. **`displaylist-to-ydraw`** - Converts JSON DisplayList to YDraw OSC 666666
   - Handles all core DisplayList commands
   - Supports transform/clip state management
   - Outputs OSC 666666 for yetty terminal

2. **`html-to-displaylist`** - Simple HTML to DisplayList JSON converter
   - Fetches URLs via libcurl
   - Handles basic HTML (headings, paragraphs, links)
   - Outputs JSON DisplayList format

3. **`libweb-to-ydraw`** - Full pipeline wrapper script
   - Combines html-to-displaylist | displaylist-to-ydraw
   - Supports URL fetching or stdin input

4. **`ladybird-ydraw`** - Test harness (generates test pattern)

**Usage:**
```bash
# Build standalone tools
cd tools/ladybird
mkdir build && cd build
cmake .. -DBUILD_LIBWEB=OFF
make

# Render a URL to yetty
./libweb-to-ydraw https://example.com

# Or use the pipeline directly
./html-to-displaylist https://example.com | ./displaylist-to-ydraw

# From HTML string
echo '<h1>Hello</h1><p>World</p>' | ./libweb-to-ydraw --stdin
```

### JSON DisplayList Format

The tools communicate via a JSON format matching LibWeb's DisplayListCommand:

```json
{
  "width": 800,
  "height": 600,
  "commands": [
    {"type": "FillRect", "rect": {"x":0,"y":0,"width":800,"height":600}, "color": {"r":255,"g":255,"b":255,"a":255}},
    {"type": "DrawGlyphRun", "rect": {"x":10,"y":30,"width":200,"height":24}, "color": {"r":0,"g":0,"b":0,"a":255}, "text": "Hello World", "font_size": 24},
    {"type": "DrawLine", "from": {"x":0,"y":50}, "to": {"x":800,"y":50}, "color": {"r":200,"g":200,"b":200,"a":255}, "thickness": 1}
  ]
}
```

### For Full LibWeb Integration

The full Ladybird build is blocked by:
1. **Python venv issue** - ANGLE port fails to create Python venv
2. **vcpkg complexity** - Many dependencies need system packages

To enable full LibWeb:
1. Install system dependencies:
   ```bash
   # Ubuntu/Debian - install python3-venv for your Python version
   sudo apt install python3.12-venv ninja-build
   ```
2. Build Ladybird:
   ```bash
   cd tmp/ladybird
   cmake --preset Release
   cmake --build Build/release
   ```
3. Create a LibWeb tool that outputs JSON DisplayList

The current simple HTML renderer provides basic functionality while LibWeb integration is completed.

## 11. Architecture Diagram

```
                    ┌─────────────────────┐
                    │      URL/HTML       │
                    └──────────┬──────────┘
                               │
          ┌────────────────────┴────────────────────┐
          │                                         │
          ▼                                         ▼
┌─────────────────────┐                 ┌─────────────────────┐
│  html-to-displaylist│                 │    LibWeb (TODO)    │
│  (simple fallback)  │                 │  (full web engine)  │
└──────────┬──────────┘                 └──────────┬──────────┘
           │                                       │
           │              JSON DisplayList         │
           └───────────────────┬───────────────────┘
                               │
                               ▼
                   ┌─────────────────────┐
                   │ displaylist-to-ydraw│
                   │                     │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │    OSC 666666       │
                   │   (YDraw buffer)    │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │   yetty terminal    │
                   │   (GPU rendering)   │
                   └─────────────────────┘
```

## 12. Risk Assessment

### Completed
- IPC JSON format for DisplayList commands
- YDraw buffer serialization and OSC output
- Basic HTML rendering fallback

### Remaining Work
- Full LibWeb integration (blocked by build issues)
- Path decomposition for complex SVG
- Image handling (currently shows placeholder)
- Better gradient approximation

### Mitigation
- Simple HTML renderer provides immediate functionality
- JSON format allows easy swap between renderers
- LibWeb integration can be added incrementally
