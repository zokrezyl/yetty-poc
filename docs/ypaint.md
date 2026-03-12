# YPaint Design Document

## Overview

YPaint is a 2D vector graphics overlay system using Signed Distance Fields (SDF) for GPU-accelerated rendering. It supports both static overlays and scrolling terminal-style graphics.

## Architecture

### Component Responsibilities

```
┌─────────────────────────────────────────────────────────────────┐
│                         Painter                                  │
│  - Owns Canvas (shared_ptr)                                     │
│  - Font management (MsdfAtlas, FontManager)                     │
│  - AABB computation for primitives                              │
│  - GPU buffer lifecycle (declare/allocate/write)                │
│  - Text measurement and glyph rendering                         │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ owns
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                         Canvas                                   │
│  - Spatial grid representation (deque of GridLines)             │
│  - Primitive storage with pre-computed AABBs                    │
│  - Grid cell management (PrimRef for cross-line references)     │
│  - Scrolling support (line removal, cursor tracking)            │
│  - Packed GPU format generation (gridStaging, primStaging)      │
└─────────────────────────────────────────────────────────────────┘
                              │
                              │ generates
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                      GPU Buffers                                 │
│  - Primitive data buffer                                         │
│  - Grid lookup buffer                                            │
│  - Glyph buffer                                                  │
│  - Metadata (64 bytes)                                           │
└─────────────────────────────────────────────────────────────────┘
```

### Canvas Internal Structure

```cpp
// Reference to a primitive in another line
struct PrimRef {
  uint16_t linesAhead; // relative offset to base line (0 = same line)
  uint16_t primIndex;  // index within base line's prims vector
};

// A single grid cell - contains references to overlapping primitives
struct GridCell {
  std::vector<PrimRef> refs;
};

// A single row/line in the grid
struct GridLine {
  // Primitives whose BASE (bottom-most line they touch) is this line
  std::vector<std::vector<float>> prims;
  // Grid cells for this line's Y range (indexed by X column)
  std::vector<GridCell> cells;
};

// Lines stored in deque for efficient scrolling
std::deque<GridLine> _lines;
```

### Data Flow: Demo Script to Shader

```
circles.sh
    │
    ├── YAML content base64-encoded
    │
    └── OSC command: \033]666666;run -c ypaint ...;--yaml;{base64}\033\\
           │
           ▼
    Terminal OSC Parser
           │
           ▼
    ypaint card creation (CardManager)
           │
           ▼
    yaml2ypaint → YPaintBuffer
           │
           ▼
    Painter.addYpaintBuffer()
           │
           ├── Compute AABBs for each primitive
           ├── Call Canvas.addPrimitive(data, aabb)
           └── Canvas positions primitive in grid
           │
           ▼
    GPU Upload
           │
           ├── Primitive buffer: [gridOffset][type][layer][geometry...]
           ├── Grid buffer: cell → [count][prim_offsets...]
           └── Metadata: 64 bytes
           │
           ▼
    Shader: 0x0004-ypaint.wgsl
           │
           ├── Lookup cell from pixel position
           ├── Iterate primitives in cell
           ├── Call evaluateSDF(primOffset, p)
           └── Blend colors based on distance
```

## GPUScreen Integration

### Render Layer Structure

GPUScreen has THREE render layers:

```
┌─────────────────────────────────────────────────────────────────┐
│  Layer 2: Overlay Painter (OSC 666668)                          │
│  - Fixed position overlays (HUD, tooltips, status)              │
│  - Does NOT scroll with terminal content                        │
│  - Rendered LAST (on top of everything)                         │
│  - Absolute screen coordinates                                  │
└─────────────────────────────────────────────────────────────────┘
                              ↑
┌─────────────────────────────────────────────────────────────────┐
│  Layer 1: Scrolling Painter (OSC 666667)                        │
│  - Graphics that scroll WITH terminal content                   │
│  - Cursor-relative positioning (like terminal text)             │
│  - SYNCHRONIZED with vterm scrolling                            │
└─────────────────────────────────────────────────────────────────┘
                              ↑
┌─────────────────────────────────────────────────────────────────┐
│  Layer 0: Terminal Glyph Grid                                    │
│  - Standard terminal text rendering                             │
│  - Character cells with attributes                              │
│  - Rendered FIRST (bottom layer)                                │
└─────────────────────────────────────────────────────────────────┘
```

### OSC Command Codes

Each layer has its own OSC code:

| Layer | OSC Code | Purpose |
|-------|----------|---------|
| Scrolling | `666674` | Graphics that scroll with terminal content |
| Overlay | `666675` | Fixed position overlays (HUD, tooltips) |

Note: 666667-669 are already used for shader effects, 666670 for GPU stats,
666671 for FPS control, 666673 for ScreenDrawLayer (deprecated ydraw overlay).

```bash
# Draw to scrolling layer (scrolls with terminal)
printf '\033]666674;--yaml;%s\033\\' "$base64_data"

# Draw to overlay layer (fixed position)
printf '\033]666675;--yaml;%s\033\\' "$base64_data"

# Clear scrolling layer
printf '\033]666674;--clear\033\\'

# Clear overlay layer
printf '\033]666675;--clear\033\\'
```

### CRITICAL: Bidirectional Scrolling Synchronization

The scrolling painter and vterm must stay **perfectly synchronized**. This is
bidirectional - either side can be the source of truth depending on the event:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    BIDIRECTIONAL SYNC MODEL                             │
│                                                                         │
│   ┌─────────────┐                           ┌─────────────┐             │
│   │   Canvas    │ ←───── sync ─────────────→│   VTerm     │             │
│   │  (Painter)  │                           │             │             │
│   └─────────────┘                           └─────────────┘             │
│         │                                         │                     │
│         │ Graphics added (OSC 666667)             │ Escape sequences    │
│         │ Canvas is SOURCE OF TRUTH               │ VTerm is SOURCE     │
│         │ VTerm syncs TO canvas                   │ Canvas syncs TO it  │
│         ▼                                         ▼                     │
└─────────────────────────────────────────────────────────────────────────┘
```

#### 1. Adding Graphics: Canvas is Source of Truth

When `addYpaintBuffer()` is called, the **Canvas scrolls first** (if needed),
then returns the number of lines consumed. GPUScreen uses this result to
scroll vterm to match:

```
┌──────────────────────────────────────────────────────────────────┐
│  YPaintBuffer arrives via OSC 666667                             │
│                     │                                            │
│                     ▼                                            │
│  GPUScreen calls: scrollingPainter->addYpaintBuffer(buffer)      │
│                     │                                            │
│                     ▼                                            │
│  Canvas processes buffer:                                        │
│    - Computes AABB, determines lines needed                      │
│    - If doesn't fit, Canvas SCROLLS INTERNALLY                   │
│    - Stores primitives at cursor position                        │
│    - Returns: uint32_t linesConsumed                             │
│                     │                                            │
│  *** Canvas is SOURCE OF TRUTH at this moment ***                │
│                     │                                            │
│                     ▼                                            │
│  GPUScreen scrolls vterm to MATCH canvas state:                  │
│    - Inject N newlines, OR                                       │
│    - Use vterm API to force scroll                               │
│                     │                                            │
│  VTerm now matches Canvas                                        │
└──────────────────────────────────────────────────────────────────┘
```

**Why?** The graphic occupies terminal lines. VTerm must know about this
space so subsequent text output doesn't OVERLAP the graphic.

```cpp
void GPUScreenImpl::handleScrollingPaint(YPaintBuffer::Ptr buffer) {
    // 1. Add to painter - Canvas is SOURCE OF TRUTH
    //    Canvas may scroll internally, returns lines consumed
    auto result = _scrollingPainter->addYpaintBuffer(buffer);
    if (!result) return;

    uint32_t linesConsumed = *result;

    // 2. Scroll vterm to MATCH canvas state
    //    Option A: Inject newlines
    for (uint32_t i = 0; i < linesConsumed; i++) {
        vterm_keyboard_unichar(_vterm, '\n', VTERM_MOD_NONE);
    }

    //    Option B: Force scroll via vterm API (preferred if available)
    // vterm_screen_scroll(_vtermScreen, linesConsumed);
}
```

#### 2. Escape Sequences: VTerm is Source of Truth

When escape sequences cause vterm to scroll (cursor movement, newlines, etc.),
vterm is the source of truth and the Canvas must sync TO it:

```
┌──────────────────────────────────────────────────────────────────┐
│  VTerm receives escape sequence that causes scrolling            │
│  (e.g., cursor down past bottom, newline at bottom row)          │
│                     │                                            │
│  *** VTerm is SOURCE OF TRUTH ***                                │
│                     │                                            │
│                     ▼                                            │
│  VTerm callback notifies GPUScreen of scroll event               │
│                     │                                            │
│                     ▼                                            │
│  GPUScreen calls: scrollingPainter->scrollLines(numLines)        │
│                     │                                            │
│  Canvas scrolls to MATCH vterm state                             │
│  Graphics scroll UP in sync with terminal text                   │
└──────────────────────────────────────────────────────────────────┘
```

```cpp
// VTerm scroll callback - vterm is source of truth here
int GPUScreenImpl::onVTermScroll(int numLines) {
    // Scroll the painter to MATCH vterm
    if (_scrollingPainter && numLines > 0) {
        _scrollingPainter->scrollLines(static_cast<uint16_t>(numLines));
    }
    return 0;  // allow scroll
}
```

#### 3. Overflow Handling

If the new YPaintBuffer doesn't fit at the current cursor position, the
Canvas handles the scroll internally and returns the total lines consumed.
VTerm then syncs to match:

```
┌──────────────────────────────────────────────────────────────────┐
│  Before: Cursor at row 23 (near bottom), graphic needs 10 lines  │
│                                                                  │
│  Terminal viewport (25 lines):                                   │
│  ┌────────────────────────┐                                      │
│  │ line 0: text           │                                      │
│  │ line 1: text           │                                      │
│  │ ...                    │                                      │
│  │ line 22: text          │                                      │
│  │ line 23: CURSOR ←      │  Only 2 lines available!             │
│  │ line 24: text          │                                      │
│  └────────────────────────┘                                      │
│                                                                  │
│  Canvas.addYpaintBuffer():                                       │
│    - Needs 10 lines, only 2 available                            │
│    - Canvas scrolls 8 lines internally                           │
│    - Stores graphic at new position                              │
│    - Returns 10 (total lines consumed)                           │
│                                                                  │
│  GPUScreen scrolls vterm by 10 lines to match                    │
│  (Some text and top of graphic may scroll off)                   │
└──────────────────────────────────────────────────────────────────┘
```

```cpp
void GPUScreenImpl::handleScrollingPaint(YPaintBuffer::Ptr buffer) {
    // Canvas handles everything - scrolling if needed, positioning
    // Returns total lines consumed (including any internal scrolling)
    auto result = _scrollingPainter->addYpaintBuffer(buffer);
    if (!result) return;

    uint32_t linesConsumed = *result;

    // Scroll vterm to match canvas state
    scrollVTerm(linesConsumed);
}

### GPUScreenImpl Changes

Replace `ScreenDrawLayer` with two `Painter` instances:

```cpp
class GPUScreenImpl : public GPUScreen {
    // ... existing members ...

    // Replace: ScreenDrawLayer::Ptr _screenDrawLayer;
    // With:
    Painter::Ptr _scrollingPainter;    // Layer 1: synced with vterm
    Painter::Ptr _overlayPainter;      // Layer 2: fixed position
};
```

### Render Order

```cpp
void GPUScreenImpl::render(WGPURenderPassEncoder pass) {
    // 1. Render terminal text (layer 0)
    renderGlyphGrid(pass);

    // 2. Render scrolling graphics (layer 1)
    if (_scrollingPainter && _scrollingPainter->hasContent()) {
        _scrollingPainter->render(pass);
    }

    // 3. Render overlay graphics (layer 2)
    if (_overlayPainter && _overlayPainter->hasContent()) {
        _overlayPainter->render(pass);
    }
}

## Primitive Buffer Layout

### GPU Layout (per primitive)

```
Word 0: gridOffset (u16 x | u16 y << 16) - grid cell offset for scrolling
Word 1: type (u32) - primitive type ID
Word 2: layer (u32) - render layer
Word 3+: geometry parameters (type-specific)
Word N-4: fillColor (u32)
Word N-3: strokeColor (u32)
Word N-2: strokeWidth (f32)
Word N-1: round (f32)
```

### Primitive Size Lookup Map

For O(1) jumping to the next primitive during iteration:

| Type | ID | Size (words) | Notes |
|------|-----|--------------|-------|
| Circle | 1 | 9 | Fixed |
| Box | 2 | 10 | Fixed |
| RoundedBox | 3 | 11 | Fixed |
| OrientedBox | 4 | 13 | Fixed |
| Segment | 5 | 11 | Fixed |
| Rhombus | 6 | 11 | Fixed |
| Trapezoid | 7 | 12 | Fixed |
| Parallelogram | 8 | 12 | Fixed |
| EquilateralTriangle | 9 | 10 | Fixed |
| IsoscelesTriangle | 10 | 11 | Fixed |
| Triangle | 11 | 13 | Fixed |
| UnevenCapsule | 12 | 12 | Fixed |
| Pentagon | 13 | 10 | Fixed |
| Hexagon | 14 | 11 | Fixed |
| Octagon | 15 | 10 | Fixed |
| Hexagram | 16 | 10 | Fixed |
| Star5 | 17 | 11 | Fixed |
| Star | 18 | 12 | Fixed |
| Pie | 19 | 12 | Fixed |
| CutDisk | 20 | 11 | Fixed |
| Arc | 21 | 13 | Fixed |
| Ring | 22 | 12 | Fixed |
| Horseshoe | 23 | 14 | Fixed |
| Vesica | 24 | 11 | Fixed |
| Moon | 25 | 12 | Fixed |
| CircleCross | 26 | 10 | Fixed |
| SimpleEgg | 27 | 11 | Fixed |
| Heart | 28 | 10 | Fixed |
| Cross | 29 | 12 | Fixed |
| RoundedX | 30 | 11 | Fixed |
| Ellipse | 31 | 11 | Fixed |
| Parabola | 32 | 10 | Fixed |
| ParabolaSegment | 33 | 11 | Fixed |
| Bezier | 34 | 15 | Fixed |
| BlobCross | 35 | 10 | Fixed |
| Tunnel | 36 | 11 | Fixed |
| Stairs | 37 | 12 | Fixed |
| QuadraticCircle | 38 | 10 | Fixed |
| Hyperbola | 39 | 12 | Fixed |
| CoolS | 40 | 10 | Fixed |
| CircleWave | 41 | 12 | Fixed |
| Plot | 128 | 7 + N*2 | Variable: N = numPoints |
| Polygon | 130 | 5 + N*2 | Variable: N = numVertices |
| PolygonGroup | 131 | varies | Variable: depends on groups |

## Scrolling Mode

### Concept

In scrolling mode, primitives are positioned relative to a cursor (like terminal text):
- Cursor position: (col, row) in grid cells
- gridOffset stored in word 0 of each primitive
- When content overflows, top lines scroll off

### Grid Offset Usage in Shader

```wgsl
// Read grid offset from primitive
let gridOffsetPacked = bitcast<u32>(storage[primOffset + 0u]);
let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
let gridOffsetY = f32(gridOffsetPacked >> 16u);

// Convert grid offset to pixel offset
let pixelOffset = vec2<f32>(gridOffsetX * cellSizeX, gridOffsetY * cellSizeY);

// Adjust p for primitive's position
let pAdj = p - pixelOffset;

// Now evaluate SDF with adjusted p
return sdCircle(pAdj, center, radius);
```

### Scrolling Operation

When scrolling N lines:
1. Remove top N lines from Canvas (deque pop_front)
2. Decrement gridOffset.y by N for remaining primitives
3. Mark grid as dirty for rebuild

## Grid Structure

### Cell Layout

```
Grid buffer layout:
[offset_0][offset_1]...[offset_N-1][packed_cells]

Where offset_i points to the packed data for cell i.

Packed cell data:
[count][entry_0][entry_1]...[entry_count-1]

Entry format:
- If high bit (0x80000000) is set: glyph index (entry & 0x7FFFFFFF)
- Otherwise: primitive word offset in primitive buffer
```

### Grid Lookup in Shader

```wgsl
fn getCellPrimitives(cellX: u32, cellY: u32) -> array<u32, MAX_PRIMS> {
    let cellIndex = cellY * gridWidth + cellX;
    let packedOffset = gridData[cellIndex];
    let count = gridData[packedOffset];

    var prims: array<u32, MAX_PRIMS>;
    for (var i = 0u; i < count && i < MAX_PRIMS; i++) {
        prims[i] = gridData[packedOffset + 1 + i];
    }
    return prims;
}
```

## SDF Functions

### First Argument `p`

All SDF functions take `p` as the first argument - the pixel position in scene coordinates:

```wgsl
fn sdCircle(p: vec2<f32>, center: vec2<f32>, radius: f32) -> f32 {
    return length(p - center) - radius;
}
```

### Position Adjustment for Scrolling

The gridOffset allows primitives to be positioned relative to where they were added:

```wgsl
// Without scrolling: p is raw pixel position
let d = sdCircle(p, center, radius);

// With scrolling: p is adjusted by gridOffset
let pAdj = p - pixelOffset;  // pixelOffset = gridOffset * cellSize
let d = sdCircle(pAdj, center, radius);
```

## Metadata Layout (64 bytes)

```cpp
struct YPaintMetadata {
    uint32_t primitiveOffset;  // 0: offset to primitive buffer
    uint32_t primitiveCount;   // 4: number of primitives
    uint32_t gridOffset;       // 8: offset to grid buffer
    uint32_t gridWidth;        // 12: grid width in cells
    uint32_t gridHeight;       // 16: grid height in cells
    uint32_t cellSizeXY;       // 20: cellSizeX (f16) | cellSizeY (f16)
    uint32_t glyphOffset;      // 24: offset to glyph buffer
    uint32_t glyphCount;       // 28: number of glyphs
    uint32_t sceneMinX;        // 32: scene bounds (f32 as bits)
    uint32_t sceneMinY;        // 36
    uint32_t sceneMaxX;        // 40
    uint32_t sceneMaxY;        // 44
    uint32_t widthCells;       // 48: viewport width | panX (i16)
    uint32_t heightCells;      // 52: viewport height | panY (i16)
    uint32_t flags;            // 56: flags (u16) | zoom (f16)
    uint32_t bgColor;          // 60: background color (RGBA)
};
```

## Implementation Status

### Completed

- [x] Canvas class implementation (grid management, scrolling)
- [x] Painter refactoring to use Canvas
- [x] Primitive storage with gridOffset prepended
- [x] Grid staging generation
- [x] Primitive staging generation
- [x] Scene bounds-based grid dimensions
- [x] All 155 unit tests passing

### In Progress

- [ ] Replace ScreenDrawLayer with two Painter instances in GPUScreen
- [ ] Add layer targeting to OSC protocol

### TODO

- [ ] Fix gen-ypaint-types.py to read ypaint-primitives.yaml
- [ ] Generate evaluateSDF from YAML
- [ ] Generate primitive size lookup map
- [ ] Delete hand-written evaluateSDF from distfunctions.wgsl
- [ ] Debug why demos don't work (trace OSC path)
- [ ] Unify or clearly separate ydraw/ypaint primitive IDs

## Known Issues

### 1. Broken Code Generator

`gen-ypaint-types.py` is a non-functional copy of `gen-ydraw-types.py`:
- Line 26 reads `ydraw-primitives.yaml` which doesn't exist in ypaint directory
- No generated files for ypaint primitives

### 2. Hand-Written evaluateSDF Misalignment

`distfunctions.wgsl` contains **hand-written** `evaluateSDF()` (line 886+) with hardcoded offsets:
```wgsl
fn evaluateSDF(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let radius = cardStorage[primOffset + 4u];
            return sdCircle(p, center, radius);
        }
        // ... more hardcoded offsets
    }
}
```

Meanwhile, `gen-ydraw-types.py` generates `evalSDF()` (different function!) with gridOffset support.

**Result:** Adding/changing fields in YAML causes buffer misalignment since `evaluateSDF` isn't updated.

### 3. Primitive ID Conflicts

| Primitive | ydraw ID | ypaint ID |
|-----------|----------|-----------|
| Circle    | 0        | 1         |
| Box       | 1        | 2         |
| Segment   | 2        | 5         |

Two incompatible ID schemes cause confusion.
