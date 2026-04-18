# Yetty Font System

Yetty uses a multi-layered font rendering system that supports crisp text at any zoom level, color emojis, animated glyphs, and multi-cell widgets - all rendered efficiently on the GPU.

## Font Types Overview

| Type | Description | Atlas/Buffer | Codepoint Range | Use Case |
|------|-------------|--------------|-----------------|----------|
| **MSDF** | Multi-channel Signed Distance Field | 2048x512 texture | Standard Unicode | Primary terminal text |
| **Vector SDF** | Real-time SDF from Bezier curves | ~40KB buffer | U+F0000-U+F0FFF | Infinite zoom, presentations |
| **Vector Coverage** | Supersampled curve coverage | ~40KB buffer | U+F1000-U+F1FFF | Maximum quality, thin strokes |
| **Bitmap** | RGBA color textures | 2048x2048 texture | Emoji ranges | Color emojis |
| **Shader Glyph** | Procedurally rendered in WGSL | None | U+101000-U+10FFFD | Animated spinners, effects |
| **Card Glyph** | Multi-cell shader regions | None | U+100000-U+100FFF | Images, plots, widgets |

## Architecture

```
┌───────────────────────────────────────────────────────────────────────────┐
│                             FontManager                                    │
│  - Caches loaded fonts (~/.cache/yetty/msdf-font-cache/)                  │
│  - Factory methods for each font type                                      │
│  - Generates missing CDB files via MsdfCdbProvider                         │
│  - Manages GPU storage buffers for vector fonts                            │
└───────────────────────────────────────────────────────────────────────────┘
       │          │          │            │            │            │
       ▼          ▼          ▼            ▼            ▼            ▼
  ┌─────────┐ ┌─────────┐ ┌───────────┐ ┌───────────┐ ┌─────────┐ ┌─────────┐
  │MsMsdfFont│ │VectorSdf│ │VectorCov. │ │  BmFont   │ │Shader   │ │Card     │
  │ (atlas) │ │ (curves)│ │ (samples) │ │  (emoji)  │ │Font     │ │Font     │
  └─────────┘ └─────────┘ └───────────┘ └───────────┘ └─────────┘ └─────────┘
       │          │          │            │            │            │
       │     [GPU Storage Buffers]        │       [GPU Textures]    │
       │          │          │            │            │            │
       ▼          ▼          ▼            ▼            ▼            ▼
  ┌───────────────────────────────────────────────────────────────────────────┐
  │                          GPU Screen Shader                                 │
  │  - Routes glyphs by codepoint range                                       │
  │  - MSDF: texture sample + median distance                                 │
  │  - Vector SDF: curve iteration + winding number (U+F0xxx)                 │
  │  - Vector Coverage: 16-sample supersampling (U+F1xxx)                     │
  │  - Bitmap: color texture sample                                           │
  │  - Shader/Card: procedural WGSL functions                                 │
  └───────────────────────────────────────────────────────────────────────────┘
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
FONT_TYPE_MSDF     = 0;   // Standard text (default)
FONT_TYPE_BITMAP   = 1;   // Color emoji
FONT_TYPE_SHADER   = 2;   // Animated single-cell
FONT_TYPE_CARD     = 3;   // Multi-cell widget
FONT_TYPE_VECTOR   = 4;   // Vector SDF font (U+F0xxx)
FONT_TYPE_COVERAGE = 5;   // Vector Coverage font (U+F1xxx)
```

**Note:** Vector fonts are also detected by codepoint range in the shader, so font type bits serve as a hint for optimization.

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
2. Determines font type from codepoint range and style bits
3. Routes to appropriate renderer:
   - **MSDF**: Sample atlas, compute median distance, apply anti-aliasing
   - **Vector SDF**: Iterate curves, compute minimum distance, winding number for inside/outside
   - **Vector Coverage**: 4x4 supersample grid, winding number per sample, accumulate coverage
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

## Vector Font Approaches Comparison

Yetty provides three distinct approaches for rendering scalable vector text. Each has different trade-offs between storage, GPU compute, and quality at extreme magnification.

### Overview

| Approach | Storage | GPU Compute | Magnification Quality | Anti-aliasing |
|----------|---------|-------------|----------------------|---------------|
| **MSDF (Atlas)** | High (texture) | Low | Good (some artifacts at 10x+) | Implicit in SDF |
| **Vector SDF** | Low (curves) | Medium | Perfect (infinite) | Distance-based smoothing |
| **Vector Coverage** | Low (curves) | High | Perfect (infinite) | 16-sample supersampling |

### 1. MSDF (Multi-channel Signed Distance Field)

The traditional approach using pre-rendered atlas textures.

**Storage:**
- Pre-computed 2048x512 RGBA texture atlas
- Each glyph: ~32x40 pixels average = ~5KB per glyph
- Full ASCII set: ~500KB texture memory
- CDB cache files on disk (~2-5MB per font style)

**GPU Rendering:**
- Single texture sample per pixel
- Median of 3 channels for distance
- Simple alpha calculation: `O(1)` per pixel

**Quality:**
- Excellent at normal zoom (1x-4x)
- Slight blurring/artifacts at extreme magnification (>10x)
- Fixed resolution - quality degrades beyond atlas resolution

```wgsl
// MSDF rendering - simple and fast
let msdf = textureSample(fontTexture, fontSampler, uv);
let sd = median(msdf.r, msdf.g, msdf.b);
let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);
```

**Best for:** Primary terminal text, normal viewing conditions

---

### 2. Vector SDF Font (Real-time SDF Evaluation)

Stores raw quadratic Bezier curves and computes signed distance on GPU.

**Codepoint Range:** `U+F0000` - `U+F0FFF` (Plane 15 PUA-A)

**Storage:**
- Curves stored as packed uint32 triplets (12 bytes per curve)
- Header: 4 bytes (curve count + flags)
- Typical glyph: 20-60 curves = 244-724 bytes per glyph
- Full ASCII set: ~30-50KB GPU buffer (vs ~500KB for MSDF atlas)
- **~10x smaller than MSDF atlas**

```cpp
// GPU buffer format
struct PackedCurve {
    uint32_t p0;  // x[16] | y[16]  (normalized to 0-65535)
    uint32_t p1;  // x[16] | y[16]
    uint32_t p2;  // x[16] | y[16]
};
// Per-glyph: [header: u32][curve0: 3xu32][curve1: 3xu32]...
```

**GPU Rendering:**
- Iterate all curves, compute minimum distance
- Use winding number for inside/outside determination
- `O(n)` where n = number of curves per glyph

```wgsl
// Vector SDF rendering - compute SDF in real-time
for (var i = 0u; i < curveCount; i++) {
    let dist = sdQuadraticBezier(localUV, p0, p1, p2);
    minDist = min(minDist, dist);
}
let winding = windingNumber(glyphIndex, localUV);
return select(minDist, -minDist, winding != 0);
```

**Anti-aliasing:**
- Distance-based smoothing (1.5 pixel transition zone)
- Smooth edges at any magnification

```wgsl
let pixelWidth = 1.0 / min(grid.cellSize.x, grid.cellSize.y);
let alpha = clamp(0.5 - sd / (pixelWidth * 1.5), 0.0, 1.0);
```

**Quality:**
- Perfect at any magnification (mathematically exact)
- Smooth edges via distance-based anti-aliasing
- No texture resolution limits

**Best for:** Zoomed displays, high-DPI, presentations

---

### 3. Vector Coverage Font (Multi-sample Supersampling)

Stores same curve data but uses coverage-based rendering with supersampling.

**Codepoint Range:** `U+F1000` - `U+F1FFF` (Plane 15 PUA-A)

**Storage:**
- Identical to Vector SDF (12 bytes per curve)
- Separate GPU buffers for independent font instances
- **~10x smaller than MSDF atlas**

**GPU Rendering:**
- 4x4 supersampling grid (16 samples per pixel)
- Each sample: compute winding number via ray-curve intersections
- `O(16 * n)` where n = number of curves per glyph

```wgsl
// Coverage rendering - brute-force supersampling
let samples = 4;
let sampleWeight = 1.0 / f32(samples * samples);  // 1/16

for (var sy = 0; sy < samples; sy++) {
    for (var sx = 0; sx < samples; sx++) {
        let samplePos = localUV + sampleOffset * pixelSize;
        let winding = coverageWindingNumber(glyphIndex, samplePos);
        if (winding != 0) {
            coverage += sampleWeight;
        }
    }
}
return coverage;
```

**Winding Number Calculation:**
- Ray casting: count curve crossings to the right of sample point
- Solve quadratic for y-intersection, check x position
- Accumulate signed crossings based on curve direction

**Quality:**
- Perfect at any magnification (mathematically exact)
- More accurate anti-aliasing than distance-based
- Better handling of complex self-intersecting glyphs
- Handles thin strokes without artifacts

**Best for:** Maximum quality, complex glyphs, thin strokes

---

### Performance Comparison

| Metric | MSDF | Vector SDF | Vector Coverage |
|--------|------|------------|-----------------|
| **Texture Memory** | ~500KB | 0 | 0 |
| **Buffer Memory** | 0 | ~40KB | ~40KB |
| **Total GPU Memory** | ~500KB | ~40KB | ~40KB |
| **Ops per Pixel** | 4 | ~100-300 | ~1600-4800 |
| **Shader Complexity** | Trivial | Medium | High |
| **Cache Friendly** | Yes (texture) | Moderate | Low |
| **Infinite Zoom** | No | Yes | Yes |

### When to Use Each

1. **MSDF (default):** Standard terminal rendering. Best performance, excellent quality at normal zoom levels. Use for 99% of text rendering.

2. **Vector SDF:** When you need infinite zoom capability with reasonable performance. Good for presentations, accessibility zoom, or high-DPI displays where MSDF artifacts become visible.

3. **Vector Coverage:** Maximum quality requirement. Use for rendering complex glyphs, thin strokes, or when anti-aliasing accuracy is critical. Performance cost is significant - use sparingly.

### GPU Shader Bindings

Vector fonts use storage buffers in group 1:

```wgsl
// Vector SDF font buffers
@group(1) @binding(8) var<storage, read> vectorGlyphBuffer: array<u32>;
@group(1) @binding(9) var<storage, read> vectorOffsetTable: array<u32>;

// Vector Coverage font buffers
@group(1) @binding(10) var<storage, read> coverageGlyphBuffer: array<u32>;
@group(1) @binding(11) var<storage, read> coverageOffsetTable: array<u32>;
```

**Note:** Requires `maxStorageBuffersPerShaderStage >= 10` (WebGPU device limit).

### Font Type Dispatch

The fragment shader routes glyphs by codepoint range:

```wgsl
if (isVectorGlyph(glyphIndex)) {           // U+F0000 - U+F0FFF
    // Vector SDF rendering
    let sd = sampleVectorGlyph(glyphIndex, localUV);
    let alpha = clamp(0.5 - sd / (pixelWidth * 1.5), 0.0, 1.0);
} else if (isCoverageGlyph(glyphIndex)) {  // U+F1000 - U+F1FFF
    // Vector Coverage rendering
    let coverage = sampleCoverageGlyph(glyphIndex, localUV, pixelSize);
}
```

## Key Source Files

| File | Purpose |
|------|---------|
| `include/yetty/font-manager.h` | Central font management |
| `src/yetty/ms-msdf-font.cpp` | MSDF font implementation |
| `src/yetty/vector-sdf-font.cpp` | Vector SDF font (runtime curves) |
| `src/yetty/vector-coverage-font.cpp` | Vector Coverage font (supersampled) |
| `src/yetty/bm-font.cpp` | Bitmap/emoji font |
| `src/yetty/shader-font.cpp` | Procedural shader glyphs |
| `src/yetty/gpu-screen.cpp` | GPU rendering integration |
| `src/yetty/shaders/gpu-screen.wgsl` | Main rendering shader |
| `src/yetty/shaders/lib/text.wgsl` | Text rendering utilities |
| `src/yetty/shaders/glyphs/` | Procedural glyph shaders |
