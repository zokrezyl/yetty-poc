# Yetty Font System

Yetty uses a multi-layered font rendering system that supports crisp text at any zoom level, color emojis, animated glyphs, and multi-cell widgets - all rendered efficiently on the GPU.

## Font Types Overview

| Type | Description | Atlas | Use Case |
|------|-------------|-------|----------|
| **MSDF** | Multi-channel Signed Distance Field | 2048x512 | Primary terminal text |
| **Bitmap** | RGBA color textures | 2048x2048 | Color emojis |
| **Shader Glyph** | Procedurally rendered in WGSL | None | Animated spinners, effects |
| **Card Glyph** | Multi-cell shader regions | None | Images, plots, widgets |

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      FontManager                             │
│  - Caches loaded fonts (~/.cache/yetty/msdf-font-cache/)    │
│  - Factory methods for each font type                        │
│  - Generates missing CDB files via MsdfCdbProvider           │
└─────────────────────────────────────────────────────────────┘
           │              │              │              │
           ▼              ▼              ▼              ▼
    ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐
    │ MsMsdfFont│   │  BmFont  │   │ShaderFont│   │ CardFont │
    │  (text)  │   │ (emoji)  │   │ (anim)   │   │ (widget) │
    └──────────┘   └──────────┘   └──────────┘   └──────────┘
           │              │              │              │
           ▼              ▼              ▼              ▼
    ┌─────────────────────────────────────────────────────────┐
    │                    GPU Screen Shader                     │
    │  - Routes glyphs by type (style bits 5-7)               │
    │  - MSDF: texture + distance field                        │
    │  - Bitmap: color texture sample                          │
    │  - Shader/Card: procedural WGSL functions               │
    └─────────────────────────────────────────────────────────┘
```

## MSDF Fonts (Multi-channel Signed Distance Field)

MSDF is the primary text rendering method. It produces crisp, scalable text at any zoom level without blur or aliasing artifacts.

### How MSDF Works

Unlike bitmap fonts that store pixel colors, MSDF fonts store **distance information** in RGB channels. Each channel encodes the signed distance to a different edge of the glyph shape. The fragment shader computes the median of these three distances to determine if a pixel is inside or outside the glyph.

```wgsl
// MSDF rendering in shader
fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

let msdf = textureSample(fontTexture, fontSampler, uv);
let sd = median(msdf.r, msdf.g, msdf.b);
let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);
```

### Storage Format

MSDF glyph data is stored in **CDB files** (Constant Database) - a fast key-value format:

- Location: `~/.cache/yetty/msdf-font-cache/<fontname>-<style>.cdb`
- Styles: Regular, Bold, Italic, BoldOblique
- Generated on-demand by `MsdfCdbProvider` using msdfgen

### Glyph Metadata Structure

```cpp
// On-disk format (in CDB)
struct MsdfGlyphData {
    uint32_t codepoint;
    uint16_t width, height;     // Bitmap dimensions
    float bearingX, bearingY;   // Offset from baseline
    float sizeX, sizeY;         // Logical size
    float advance;              // Horizontal advance
    // Followed by: width * height * 4 bytes RGBA
};

// GPU format (40 bytes, in SSBO)
struct GlyphMetadata {
    vec2<f32> uvMin;            // Atlas UV coordinates
    vec2<f32> uvMax;
    vec2<f32> size;             // Size in pixels
    vec2<f32> bearing;          // Baseline offset
    f32 advance;
    f32 _pad;
};
```

### Atlas Management

- **Size**: 2048 x 512 pixels (RGBA8Unorm)
- **Packing**: Shelf algorithm with 2-pixel padding
- **Preload**: ASCII printable characters (32-126)
- **On-demand**: Other glyphs loaded when first needed

```cpp
// Shelf packing algorithm
if (shelfX + glyphWidth + PADDING > atlasWidth) {
    shelfX = PADDING;
    shelfY += shelfHeight + PADDING;
    shelfHeight = 0;
}
shelfHeight = max(shelfHeight, glyphHeight);
```

## Bitmap Fonts (Emoji Support)

Bitmap fonts render color emojis using RGBA textures. They integrate with the system's emoji font (Noto Color Emoji, Apple Color Emoji, etc.) via FreeType and fontconfig.

### Characteristics

- **Atlas size**: 2048 x 2048 pixels
- **Cell size**: 64 x 64 pixels per glyph
- **Capacity**: 1024 glyphs maximum
- **Preloaded**: ~200 most common emojis

### Emoji Detection

The system automatically routes emoji codepoints to the bitmap renderer:

```cpp
bool isEmoji(uint32_t cp) {
    // Major emoji ranges:
    // 0x1F600-0x1F64F  Emoticons
    // 0x1F300-0x1F5FF  Symbols & Pictographs
    // 0x1F680-0x1F6FF  Transport & Map
    // 0x1F900-0x1F9FF  Supplemental Symbols
    // 0x2600-0x26FF    Miscellaneous Symbols
    // 0x2700-0x27BF    Dingbats
    // 0x1F1E0-0x1F1FF  Regional Indicators (flags)
}
```

### Platform Support

| Platform | Implementation |
|----------|----------------|
| Desktop (Linux, macOS, Windows) | Full FreeType + fontconfig |
| Android | Stub (system emoji via other means) |
| WebAssembly | Stub |

## Shader Glyphs (Procedural Animations)

Shader glyphs are rendered entirely in WGSL shader code - no texture atlas needed. They enable animated, interactive single-cell effects.

### Codepoint Range

- **Base**: `0x101000` (Unicode Plane 16, Private Use Area B)
- **Range**: `0x101000` - `0x10FFFD`

### Shader Function Signature

Each shader glyph is a WGSL function with this signature:

```wgsl
fn shaderGlyph_NNNNNN(
    localUV: vec2<f32>,       // Normalized position within cell (0-1)
    time: f32,                // Animation time in seconds
    fg: u32,                  // Foreground color (packed RGBA)
    bg: u32,                  // Background color (packed RGBA)
    pixelPos: vec2<f32>,      // Absolute pixel position
    mousePos: vec2<f32>,      // Current mouse position
    lastChar: u32,            // Last typed character codepoint
    lastCharTime: f32         // Time when last char was typed
) -> vec3<f32>;              // Returns final RGB color
```

### Loading Mechanism

Shader files are loaded from `src/yetty/shaders/glyphs/`:

- Filename format: `0xNNNN-name.wgsl` (hex glyph ID)
- Hot-reload supported via `reload()` method
- Injected into main shader at compile time

### Built-in Shader Glyphs

| ID | Name | Description |
|----|------|-------------|
| 0x0000 | spinner | Rotating arc animation |
| 0x0001 | pulse | Pulsing dot |
| 0x0002 | wave | Wave animation |
| 0x0003 | fire | Procedural fire effect |
| 0x0004 | heart | Beating heart |
| 0x0005 | smiley | Animated smiley face |
| ... | ... | 40+ more effects |

## Card Glyphs (Multi-Cell Widgets)

Card glyphs span multiple terminal cells and render complex content like images, plots, and interactive widgets.

### Codepoint Range

- **Base**: `0x100000` (Unicode Plane 16, Private Use Area B)
- **Range**: `0x100000` - `0x100FFF`

### How Cards Work

1. Card occupies a rectangular region of terminal cells
2. First cell contains card glyph codepoint with metadata
3. Continuation cells use special marker `0xFFFE`
4. Card shader reads metadata and renders content across all cells

### Built-in Card Types

| Card | Description |
|------|-------------|
| image | PNG, JPEG, WebP display |
| pdf | PDF document rendering |
| plot | GPU-accelerated data plots |
| ydraw | SDF primitives + MSDF text |
| ymery | ImGui interactive widgets |
| thorvg | SVG and Lottie animations |

## GPU Integration

### Cell Buffer Structure

Each terminal cell is 12 bytes:

```cpp
struct Cell {
    uint32_t glyph;                    // Glyph index/codepoint
    uint8_t fgR, fgG, fgB, alpha;      // Foreground color + alpha
    uint8_t bgR, bgG, bgB, style;      // Background color + style
};
```

### Font Type Encoding

Font type is encoded in bits 5-7 of the style byte:

```cpp
FONT_TYPE_MSDF   = 0;   // Standard text (default)
FONT_TYPE_BITMAP = 1;   // Color emoji
FONT_TYPE_SHADER = 2;   // Animated single-cell
FONT_TYPE_CARD   = 3;   // Multi-cell widget
```

### Shader Bindings

Font resources are bound in group 1:

```wgsl
@group(1) @binding(1) var fontTexture: texture_2d<f32>;      // MSDF atlas
@group(1) @binding(2) var fontSampler: sampler;
@group(1) @binding(3) var<storage, read> glyphMetadata: array<GlyphMetadata>;
@group(1) @binding(5) var emojiTexture: texture_2d<f32>;     // Emoji atlas
@group(1) @binding(6) var emojiSampler: sampler;
@group(1) @binding(7) var<storage, read> emojiMetadata: array<EmojiGlyphMetadata>;
```

### Rendering Pipeline

1. Fragment shader reads cell from buffer
2. Determines font type from style bits
3. Routes to appropriate renderer:
   - **MSDF**: Sample atlas, compute median distance, apply anti-aliasing
   - **Bitmap**: Sample color texture, blend by alpha
   - **Shader**: Call procedural glyph function
   - **Card**: Multi-cell compositing
4. Apply text decorations (underline, strikethrough)
5. Apply post-processing effects

## YDraw Text Rendering

The YDraw card type supports embedded text with variable MSDF font sizes within vector graphics scenes.

### YDrawGlyph Structure

```cpp
struct YDrawGlyph {
    float x, y;           // Position in scene coordinates
    float width, height;  // Rendered size
    uint32_t glyphIndex;  // Index into MSDF metadata
    uint32_t color;       // Packed RGBA
    uint32_t layer;       // Z-ordering layer
};
```

### Text in YDraw

```cpp
// Add text at arbitrary position and size
ydraw->addText(x, y, "Hello World", fontSize, color, layer);
```

Text glyphs are stored in the spatial hash grid alongside SDF primitives for efficient culling.

## Key Source Files

| File | Purpose |
|------|---------|
| `include/yetty/font-manager.h` | Central font management |
| `src/yetty/ms-msdf-font.cpp` | MSDF font implementation |
| `src/yetty/bm-font.cpp` | Bitmap/emoji font |
| `src/yetty/shader-font.cpp` | Procedural shader glyphs |
| `src/yetty/gpu-screen.cpp` | GPU rendering integration |
| `src/yetty/shaders/gpu-screen.wgsl` | Main rendering shader |
| `src/yetty/shaders/lib/text.wgsl` | Text rendering utilities |
| `src/yetty/shaders/glyphs/` | Procedural glyph shaders |
