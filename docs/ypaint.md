# YPaint Architecture

YPaint is the GPU-accelerated SDF (Signed Distance Field) rendering system.
Cards add primitives to a `YPaintBuffer`. The `ypaintBuilder` computes AABBs, builds
a spatial hash grid, and writes everything to GPU memory. The shader does O(1)
grid lookup per pixel to find which primitives to evaluate.

## Overview

```
Card (ygui, ypaint, ypdf, ...)
  │
  │  adds prims + text spans
  ▼
YPaintBuffer              ← pure data container (prims, text, fonts, scene metadata)
  │
  │  shared ptr
  ▼
YPaintBuilder             ← computes AABBs, grid, glyphs; manages GPU lifecycle
  │
  │  writes to via CardManager
  ▼
cardStorage[]            ← GPU storage buffer (binding 2) — prims + grid + glyphs
cardMetadata[]           ← GPU storage buffer (binding 1) — 64-byte metadata per card
  │
  │  shader reads from
  ▼
0x0003-ypaint.wgsl        ← O(1) grid lookup + SDF eval + MSDF text
```

## CRITICAL: Current Format Change (Scrolling Support)

**The primitive word layout has CHANGED to support scrolling mode.**

### NEW Primitive Word Layout (GPU Format)

All primitives written to GPU now have **gridOffset prepended**:

```
Word 0:     gridOffset   (u32)  ← packed: (gridOffsetCol | gridOffsetRow << 16)
Word 1:     type         (u32)  ← SDFType enum value (MOVED from word 0)
Word 2:     layer        (u32)  ← (MOVED from word 1)
Words 3..N: geometry + colors   ← (MOVED from word 2+)
```

### OLD vs NEW Format Comparison

| Field | OLD Offset | NEW Offset |
|-------|------------|------------|
| type | +0 | +1 |
| layer | +1 | +2 |
| gridOffset | +2 | +0 |
| geometry start | +2 | +3 |
| fillColor | N-4 | N-3 (relative to new end) |
| strokeColor | N-3 | N-2 |
| strokeWidth | N-2 | N-1 |
| round | N-1 | N |

**All shader code must read from the NEW offsets.**

### Affected Files

1. **sdf-types.gen.wgsl** - Auto-generated, reads type from +1, gridOffset from +0
2. **sdf-overlay.gen.wgsl** - Auto-generated, same format
3. **0x0003-ypaint.wgsl** - Hand-written, MUST manually update special case handling

## The Two GPU Buffers

The shader reads from **two** storage buffers:

| Binding | Name | Contents |
|---------|------|----------|
| 1 | `cardMetadata[]` | Array of `u32`. Each card gets a 64-byte (16 u32) slot. |
| 2 | `cardStorage[]` | Array of `f32`. Contains prim data, grid data, glyphs. Shared by all cards. |

Both are backed by CPU-side arrays in `CardManager` (metadata) and
`CardBufferManager` (storage). Cards write to CPU pointers; the managers flush
dirty ranges to GPU each frame.

## YPaintBuilder Internal Data Structures

### Scrolling Mode Storage

The builder uses line-based storage for efficient scrolling:

```cpp
std::deque<ScrollLine> _scrollLines;  // front=line 0 (top), pop_front on scroll

struct ScrollLine {
    std::vector<std::vector<float>> prims;  // primitives with BASE at this line
    std::vector<ScrollGridCell> cells;       // grid cells indexed by X column
};

struct ScrollGridCell {
    std::vector<ScrollPrimRef> refs;  // primitives overlapping this cell
};

struct ScrollPrimRef {
    uint16_t linesAhead;  // offset to base line (0 = same line, N = N lines down)
    uint16_t primIndex;   // index within base line's prims vector
};
```

### Primitive Storage in ScrollLine

Each primitive in `ScrollLine::prims` is a `std::vector<float>` with NEW format:

| Word | Content | Description |
|------|---------|-------------|
| 0 | **gridOffset** | Packed: `(gridOffsetCol \| gridOffsetRow << 16)` |
| 1 | type | Primitive type (original word[0]) |
| 2 | layer | Layer/z-order (original word[1]) |
| 3+ | data | Original primitive geometry data (from original word[2]+) |

### Grid Offset Explained

The **gridOffset** tells the shader how to translate primitive LOCAL coordinates to WORLD coordinates:

- **Non-scrolling mode**: gridOffset = 0 (no translation needed)
- **Scrolling mode**: gridOffset = cursor position at primitive addition time

**Shader formula**: `localPos = worldPos - gridOffset * cellSize`

When scrolling occurs, gridOffsetRow is **DECREMENTED** in all remaining primitives.

### State Variables

```cpp
bool _scrollingMode = false;      // true for scrolling overlay
uint16_t _cursorCol = 0;          // current cursor column
uint16_t _cursorRow = 0;          // current cursor row
float _cellSizeX, _cellSizeY;     // grid cell dimensions
uint32_t _gridWidth, _gridHeight; // grid dimensions

// Dirty flags
bool _bufferDirty = false;        // primitives changed, need GPU write
bool _gridStagingDirty = false;   // grid needs rebuild
```

## Primitive Word Layout (NEW Format with gridOffset)

All 2D SDF primitives now have gridOffset at word 0. Example layouts:

**Circle** (type=0, 10 words total):
```
+0: gridOffset  +1: type=0  +2: layer  +3: cx  +4: cy  +5: r
+6: fillColor  +7: strokeColor  +8: strokeWidth  +9: round
```

**Box** (type=1, 11 words total):
```
+0: gridOffset  +1: type=1  +2: layer  +3: cx  +4: cy  +5: hw  +6: hh
+7: fillColor  +8: strokeColor  +9: strokeWidth  +10: round
```

**Triangle** (type=3, 13 words total):
```
+0: gridOffset  +1: type=3  +2: layer  +3: ax  +4: ay  +5: bx  +6: by  +7: cx  +8: cy
+9: fillColor  +10: strokeColor  +11: strokeWidth  +12: round
```

**RoundedBox** (type=8, 15 words total):
```
+0: gridOffset  +1: type=8  +2: layer  +3: cx  +4: cy  +5: hw  +6: hh
+7: r0  +8: r1  +9: r2  +10: r3
+11: fillColor  +12: strokeColor  +13: strokeWidth  +14: round
```

**Star** (type=12, 12 words total):
```
+0: gridOffset  +1: type=12  +2: layer  +3: cx  +4: cy  +5: r  +6: n  +7: m
+8: fillColor  +9: strokeColor  +10: strokeWidth  +11: round
```

**ColorWheel** (type=44, 15 words total):
```
+0: gridOffset  +1: type=44  +2: layer  +3: cx  +4: cy  +5: outerR  +6: innerR
+7: hue  +8: sat  +9: val  +10: indicatorSize
+11: fillColor  +12: strokeColor  +13: strokeWidth  +14: round
```

**RotatedGlyph** (type=65, 15 words total):
```
+0: gridOffset  +1: type=65  +2: layer  +3: x  +4: y  +5: scaleX  +6: scaleY
+7: angle  +8: glyphIndex  +9: cosAngle  +10: sinAngle
+11: fillColor  +12: strokeColor  +13: strokeWidth  +14: round
```

**Image** (type=129, 11 words total):
```
+0: gridOffset  +1: type=129  +2: layer  +3: x  +4: y  +5: w  +6: h
+7: atlasX  +8: atlasY  +9: texW  +10: texH
```

### Shader Read Offsets

The shader MUST read using these offsets:

| Field | Offset |
|-------|--------|
| gridOffset | +0 |
| type | +1 |
| layer | +2 |
| geometry start | +3 |

For type-specific color offsets, see `primColors()` in `sdf-types.gen.wgsl`.

## KNOWN BUGS AND ISSUES

### 1. Text Rendering in Grid is BROKEN

**Status**: FAILING

Text glyphs added via `addText()` are not appearing in the grid correctly. The unit tests for glyph grid placement are failing:

- `glyphs appear in grid with GLYPH_BIT` - FAILED
- `mixed prims and text both reachable from grid` - FAILED
- `ygui full widget scenario — prims + text all reachable` - FAILED

**Root cause**: Not yet determined. The glyph path uses GLYPH_BIT entries in the grid (bit 31 set), separate from SDF primitive entries.

### 2. Shader Special Cases Not Updated

**Status**: PARTIALLY FIXED

The hand-written `0x0003-ypaint.wgsl` has special handling for:
- ColorWheel (type 44)
- Image (type 129)
- RotatedGlyph (type 65)

These read geometry directly from cardStorage and MUST use the new offsets (+3 for geometry start instead of +2).

**Fix applied**: Updated offsets in 0x0003-ypaint.wgsl for these special cases.

### 3. Scrolling Grid/Shader Mismatch

**Status**: OPEN BUG

When primitives are added with cursor at non-zero row, there's a mismatch between:
- Grid storage: primitives stored at cursor-adjusted rows (cursorRow + localLine)
- Shader lookup: uses world pixel position / cellSize

**Example**:
- Circle at world Y=120, cellSize=20 → shader cellY = 6
- Added at cursorRow=3 → stored in grid rows 7-11
- Shader looks at row 6, finds nothing → **BUG**

See `docs/scrolling-ypaint-builder.md` for detailed analysis.

## Unit Test Limitations

### The Problem with Current Tests

The unit tests have significant limitations that prevent them from catching real bugs:

#### 1. Tests Don't Verify Shader Behavior

The tests verify:
- Primitive data is written correctly to GPU buffer
- Grid contains expected primitive indices
- Metadata has correct offsets

The tests DO NOT verify:
- Shader reads data from correct offsets
- Shader grid lookup matches CPU grid storage
- Rendered output is correct

**Result**: A bug where shader reads from wrong offset (like primType at +0 vs +1) passes all tests but breaks rendering.

#### 2. Tests Use Mock Objects That Skip Critical Code Paths

The mock `CardManager` and `CardBufferManager`:
- Don't involve actual GPU
- Don't run shader code
- Don't validate coordinate translation

**Result**: Grid storage bugs that cause shader lookup mismatches are not caught.

#### 3. Grid Tests Only Check Presence, Not Position

Tests verify that primitives appear in the grid, but don't verify:
- Grid cell coordinates match world coordinates
- Shader lookup at world position finds the primitive

**Example of undetected bug**:
```cpp
// Test passes: "grid contains prim 0"
// But prim 0 is stored at grid row 7, shader looks at row 3
// Test doesn't check this → BUG goes undetected
```

#### 4. Format Change Tests Are Incomplete

When the primitive format changed from `[type][layer][gridOffset]` to `[gridOffset][type][layer]`:

- Tests were updated to read type from +1 instead of +0 ✓
- Tests don't run shader code that also needs updating ✗
- Hand-written shader special cases were not updated ✗

### What Tests SHOULD Do

To catch real bugs, tests should:

1. **Verify shader expectations explicitly**:
   ```cpp
   // GPU format: [0]=gridOffset, [1]=type, [2]=layer
   uint32_t type = readU32(storage, primOff + 1);  // Document why +1
   ASSERT_EQ(type, SDF_CIRCLE);
   ```

2. **Test grid lookup matches world coordinates**:
   ```cpp
   float worldY = 120.0f;
   uint32_t expectedGridRow = floor(worldY / cellSizeY);
   uint32_t actualGridRow = findPrimInGrid(primId);
   ASSERT_EQ(expectedGridRow, actualGridRow);
   ```

3. **Test shader-side coordinate translation**:
   ```cpp
   // Simulate shader grid lookup
   uint32_t cellY = floor((worldPos.y - sceneMinY) / cellSizeY);
   uint32_t cellIdx = cellY * gridWidth + cellX;
   // Verify primitive is in this cell
   ```

4. **Include visual regression tests**:
   - Render to offscreen buffer
   - Compare against golden images
   - Detect rendering differences automatically

## YPaintBuffer

Pure data container. No GPU, no fonts, no computation. Stores:

- **Primitives**: `unordered_map<uint32_t, PrimData>` where `PrimData.words` is
  a `vector<float>` containing the raw word data (WITHOUT gridOffset - that's added by builder).
- **Text spans**: `vector<TextSpanData>` — high-level text (x, y, text, fontSize,
  color, layer, fontId, rotation). NOT rendered directly; the builder converts
  these to glyphs during `addYpaintBuffer()`.
- **Font blobs**: `vector<FontBlob>` — raw TTF data for serialization.
- **Scene metadata**: `sceneMinX/Y/MaxX/Y`, `bgColor`, `flags`.

### Primitive Format in YPaintBuffer (WITHOUT gridOffset)

YPaintBuffer stores primitives in the ORIGINAL format from `ypaint-prim-writer.gen.h`:

```
Word 0:     type         (u32)  ← SDFType enum value
Word 1:     layer        (u32)
Words 2..N: geometry + colors
```

The YPaintBuilder PREPENDS gridOffset when writing to GPU:

```cpp
// In addYpaintBuffer():
std::vector<float> primData(wc + 1);  // +1 for gridOffset
primData[0] = packedGridOffset;        // NEW: gridOffset first
for (uint32_t i = 0; i < wc; i++) {
    primData[i + 1] = data[i];         // Original data shifted by 1
}
```

## GPU Buffer Layout — Prim Buffer

Written by `YPaintBuilder::writePrimsGPU()` into the `"prims"` allocation.

```
                      cardStorage[] indices (word offsets)
                      ▼
primitiveOffset ──►┌──────────────┐
                   │ wordOff[0]   │  u32: prim 0 data offset relative to primDataBase
                   │ wordOff[1]   │  u32: prim 1 data offset relative to primDataBase
                   │ ...          │
                   │ wordOff[N-1] │  u32: prim N-1 data offset
primDataBase ─────►├──────────────┤  primDataBase = primitiveOffset + primitiveCount
                   │ prim 0 data  │  [gridOffset][type][layer][geometry...]
                   │ prim 1 data  │  [gridOffset][type][layer][geometry...]
                   │ ...          │
                   │ prim N-1     │
                   └──────────────┘
```

### Shader Prim Read Example

```wgsl
let primitiveOffset = cardMetadata[metaOffset + 0];
let primitiveCount  = cardMetadata[metaOffset + 1];
let primDataBase    = primitiveOffset + primitiveCount;

// Read prim via grid entry (rawIdx is word offset):
let primOff = primDataBase + rawIdx;

// NEW FORMAT: gridOffset at +0, type at +1
let gridOffsetPacked = bitcast<u32>(cardStorage[primOff + 0u]);
let primType = bitcast<u32>(cardStorage[primOff + 1u]);
let layer = bitcast<u32>(cardStorage[primOff + 2u]);
let cx = cardStorage[primOff + 3u];  // geometry starts at +3
```

## YPaintGlyph (20 bytes = 5 u32s)

**Note**: Glyphs do NOT have gridOffset prepended. They use their own format:

```
Word 0: x (f32)           position in scene coords
Word 1: y (f32)
Word 2: width (f16 low) | height (f16 high)    ← packed via pack2x16float
Word 3: glyphIndex (u16 low) | layer (u8) | flags (u8 high)
Word 4: color (u32)       packed RGBA
```

Glyphs are stored in a separate section of the derived buffer, not mixed with SDF primitives.

## Metadata (64 bytes = 16 u32s)

```
Word  Field            Type   Description
────  ─────            ────   ───────────
  0   primitiveOffset  u32    word offset into cardStorage[] for prim buffer start
  1   primitiveCount   u32    number of primitives
  2   gridOffset       u32    word offset into cardStorage[] for grid start
  3   gridWidth        u32    grid columns
  4   gridHeight       u32    grid rows
  5   cellSizeXY       u32    cellSizeX (f16 low) | cellSizeY (f16 high)
  6   glyphOffset      u32    word offset into cardStorage[] for glyph array
  7   glyphCount       u32    number of text glyphs
  8   sceneMinX        f32    content bounds min X (= grid origin X)
  9   sceneMinY        f32    content bounds min Y (= grid origin Y)
 10   sceneMaxX        f32    content bounds max X
 11   sceneMaxY        f32    content bounds max Y
 12   widthCells        —     [15:0] = widthCells (terminal), [31:16] = panX (i16 fixedpoint)
 13   heightCells       —     [15:0] = heightCells (terminal), [31:16] = panY (i16 fixedpoint)
 14   flags             —     [15:0] = flags, [31:16] = zoom (f16 IEEE 754 half)
 15   bgColor          u32    background color (packed RGBA)
```

## Scrolling Implementation

See `docs/scrolling-ypaint-builder.md` for detailed documentation of:

- ScrollLine, ScrollGridCell, ScrollPrimRef structures
- How primitives are stored at base lines
- How grid references work with linesAhead offsets
- How scrolling updates gridOffset in primitives
- Known grid/shader mismatch bug

## File Reference

| File | Purpose | Format Awareness |
|------|---------|------------------|
| `ypaint-prim-writer.gen.h` | Write primitives to buffer | OLD format (no gridOffset) |
| `ypaint-builder.cpp` | Build GPU data, prepend gridOffset | NEW format (gridOffset at +0) |
| `sdf-types.gen.wgsl` | Card shader SDF functions | NEW format (auto-generated) |
| `sdf-overlay.gen.wgsl` | Overlay shader SDF functions | NEW format (auto-generated) |
| `0x0003-ypaint.wgsl` | Card shader main + special cases | NEW format (MANUAL updates needed) |
| `gpu-screen.wgsl` | Overlay shader main | Uses sdf-overlay.gen.wgsl |

## Debugging Tips

### Verify Primitive Format

```cpp
// After writeBuffers(), check GPU data:
const float* storage = bufMgr->storageAsFloat();
uint32_t primOff = primDataBase + wordOffset;

// NEW FORMAT verification:
uint32_t gridOffset = readU32(storage, primOff + 0);  // Should be cursor-based
uint32_t type = readU32(storage, primOff + 1);        // Should be SDF type enum
uint32_t layer = readU32(storage, primOff + 2);       // Should be layer value
float cx = storage[primOff + 3];                       // Should be geometry
```

### Verify Grid Lookup Matches Shader

```cpp
// Simulate shader lookup:
float worldY = primitiveWorldY;
uint32_t shaderCellY = floor((worldY - sceneMinY) / cellSizeY);

// Check grid storage:
uint32_t storageCellY = lineWherePermIsStored;

// These MUST match for primitive to render:
ASSERT_EQ(shaderCellY, storageCellY);
```

### Common Bugs

1. **Primitive invisible**: Check gridOffset is not causing coordinate mismatch
2. **Wrong colors**: Check fillColor/strokeColor offsets (+8/+9 for Star, varies by type)
3. **Text not rendering**: Check GLYPH_BIT entries in grid, glyph array population
4. **Scrolling breaks rendering**: Check gridOffset is decremented correctly on scroll
