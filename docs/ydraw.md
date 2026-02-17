# YDraw Architecture

YDraw is the GPU-accelerated SDF (Signed Distance Field) rendering system.
Cards add primitives to a `YDrawBuffer`. The `YDrawBuilder` computes AABBs, builds
a spatial hash grid, and writes everything to GPU memory. The shader does O(1)
grid lookup per pixel to find which primitives to evaluate.

## Overview

```
Card (ygui, ydraw, ypdf, ...)
  │
  │  adds prims + text spans
  ▼
YDrawBuffer              ← pure data container (prims, text, fonts, scene metadata)
  │
  │  shared ptr
  ▼
YDrawBuilder             ← computes AABBs, grid, glyphs; manages GPU lifecycle
  │
  │  writes to via CardManager
  ▼
cardStorage[]            ← GPU storage buffer (binding 2) — prims + grid + glyphs
cardMetadata[]           ← GPU storage buffer (binding 1) — 64-byte metadata per card
  │
  │  shader reads from
  ▼
0x0003-ydraw.wgsl        ← O(1) grid lookup + SDF eval + MSDF text
```

## The Two GPU Buffers

The shader reads from **two** storage buffers:

| Binding | Name | Contents |
|---------|------|----------|
| 1 | `cardMetadata[]` | Array of `u32`. Each card gets a 64-byte (16 u32) slot. |
| 2 | `cardStorage[]` | Array of `f32`. Contains prim data, grid data, glyphs. Shared by all cards. |

Both are backed by CPU-side arrays in `CardManager` (metadata) and
`CardBufferManager` (storage). Cards write to CPU pointers; the managers flush
dirty ranges to GPU each frame.

### How cards get addresses into cardStorage

`CardBufferManager` is a free-list allocator. A card calls:

```cpp
auto handle = bufMgr->allocateBuffer(slotIndex, "prims", sizeBytes);
// handle.data   → uint8_t* into CPU buffer (write here)
// handle.offset → byte offset in cardStorage (for metadata references)
// handle.size   → allocated bytes
```

The `handle.data` pointer lets the card write directly. The `handle.offset`
byte offset divided by 4 gives the **word offset** the shader uses to index
into `cardStorage[]`.

A single card makes **two** separate allocations:
- `"prims"` — offset table + variable-length prim data
- `"derived"` — grid + glyphs + optional atlas header

These are at **independent** offsets in `cardStorage`. They are NOT contiguous.

## YDrawBuffer

Pure data container. No GPU, no fonts, no computation. Stores:

- **Primitives**: `unordered_map<uint32_t, PrimData>` where `PrimData.words` is
  a `vector<float>` containing the raw word data for that primitive.
- **Text spans**: `vector<TextSpanData>` — high-level text (x, y, text, fontSize,
  color, layer, fontId, rotation). NOT rendered directly; the builder converts
  these to glyphs during `calculate()`.
- **Font blobs**: `vector<FontBlob>` — raw TTF data for serialization. The builder
  registers these with its MSDF atlas during `calculate()`.
- **Scene metadata**: `sceneMinX/Y/MaxX/Y`, `bgColor`, `flags`.

### Key methods

| Method | Description |
|--------|-------------|
| `addCircle(id, ...)` | Add a circle primitive |
| `addBox(id, ...)` | Add a box primitive |
| `addRoundedBox(id, ...)` | Add a rounded box primitive |
| `addTriangle(id, ...)` | Add a triangle primitive |
| `addText(x, y, text, ...)` | Add a text span (NOT a prim) |
| `addFontBlob(data, size, name)` | Store a TTF font blob |
| `clear()` | Clear prims + text spans. Keeps fonts + scene metadata |
| `primCount()` | Number of primitives (NOT text spans) |
| `empty()` | True only if BOTH prims AND text spans are empty |
| `gpuBufferSize()` | Bytes needed for prim buffer: `(primCount + totalDataWords) * 4` |
| `writeGPU(buf, bufBytes, wordOffsets)` | Write prim offset table + data to `buf`. Fills `wordOffsets`. |
| `forEachPrim(fn)` | Iterate all prims: `fn(id, data, wordCount)` |
| `forEachTextSpan(fn)` | Iterate text spans: `fn(TextSpanData&)` |
| `forEachFont(fn)` | Iterate font blobs: `fn(fontId, data, size, name)` |

### Auto-ID

`addXxx(AUTO_ID, ...)` auto-assigns an ID starting from `AUTO_ID_BASE = 0x80000000`.
Cards like ygui use `primCount()` as the ID (0, 1, 2, ...) for sequential assignment
after `clear()`.

### Delta mode

After `serialize()`, the buffer enters delta mode. Subsequent add/update/remove
operations are recorded as deltas. `serializeDelta()` returns the delta and applies
it to the main map. This is used for incremental network updates.

## Primitive Word Layout

Each primitive is a variable-length array of words (`u32` or `f32`).
The layout is defined in `ydraw-primitives.yaml` and code-generated into
`ydraw-prim-writer.gen.h` (C++) and `sdf-types.gen.wgsl` (shader).

All 2D SDF primitives share this structure — the tail fields are always at
fixed offsets from the **end** of the word array:

```
Word 0:     type         (u32)  ← SDFType enum value
Word 1:     layer        (u32)
Words 2..N-5: geometry   (varies by type)
Word N-4:   fillColor    (u32)  ← 0 = transparent, non-zero = RGBA packed
Word N-3:   strokeColor  (u32)  ← 0 = no stroke
Word N-2:   strokeWidth  (f32)
Word N-1:   round        (f32)  ← corner rounding (used by Box only)
```

### Common types — complete word maps

The shader reads fields by **absolute offset from primOffset**. These are the
exact offsets used by `primColors()`, `primStrokeWidth()`, and `evalSDF()`:

**Circle** (type=0, 9 words):
```
+0: type=0   +1: layer   +2: cx   +3: cy   +4: r
+5: fillColor   +6: strokeColor   +7: strokeWidth   +8: round
```

**Box** (type=1, 10 words):
```
+0: type=1   +1: layer   +2: cx   +3: cy   +4: hw   +5: hh
+6: fillColor   +7: strokeColor   +8: strokeWidth   +9: round
```

**Segment** (type=2, 10 words):
```
+0: type=2   +1: layer   +2: x0   +3: y0   +4: x1   +5: y1
+6: fillColor   +7: strokeColor   +8: strokeWidth   +9: round
```

**Triangle** (type=3, 12 words):
```
+0: type=3   +1: layer   +2: ax   +3: ay   +4: bx   +5: by   +6: vx   +7: vy
+8: fillColor   +9: strokeColor   +10: strokeWidth   +11: round
```

**Ellipse** (type=6, 10 words):
```
+0: type=6   +1: layer   +2: cx   +3: cy   +4: rx   +5: ry
+6: fillColor   +7: strokeColor   +8: strokeWidth   +9: round
```

**RoundedBox** (type=8, 14 words):
```
+0: type=8   +1: layer   +2: cx   +3: cy   +4: hw   +5: hh
+6: r0   +7: r1   +8: r2   +9: r3
+10: fillColor   +11: strokeColor   +12: strokeWidth   +13: round
```

**ColorWheel** (type=44, 14 words):
```
+0: type=44   +1: layer   +2: cx   +3: cy   +4: outerR   +5: innerR
+6: hue   +7: sat   +8: val   +9: indicatorSize
+10: fillColor   +11: strokeColor   +12: strokeWidth   +13: round
```

### Shader reads for fill/stroke

`primColors(primOffset)` returns `vec4<u32>(fillColor, strokeColor, layer, 0)`.
The fill/stroke offsets are type-dependent (generated from yaml):

| Type | fillColor offset | strokeColor offset | strokeWidth offset |
|------|-----------------|-------------------|-------------------|
| Circle (0) | +5 | +6 | +7 |
| Box (1) | +6 | +7 | +8 |
| Segment (2) | +6 | +7 | +8 |
| Triangle (3) | +8 | +9 | +10 |
| Ellipse (6) | +6 | +7 | +8 |
| RoundedBox (8) | +10 | +11 | +12 |

## GPU Buffer Layout — Prim Buffer

Written by `YDrawBuffer::writeGPU()` into the `"prims"` allocation.

```
                      cardStorage[] indices (word offsets)
                      ▼
primitiveOffset ──►┌──────────────┐
                   │ wordOff[0]   │  u32: prim 0 data offset relative to primDataBase
                   │ wordOff[1]   │  u32: prim 1 data offset relative to primDataBase
                   │ ...          │
                   │ wordOff[N-1] │  u32: prim N-1 data offset
primDataBase ─────►├──────────────┤  primDataBase = primitiveOffset + primitiveCount
                   │ prim 0 data  │  variable-length (e.g. 9 words for Circle)
                   │ prim 1 data  │  variable-length (e.g. 14 words for RoundedBox)
                   │ ...          │
                   │ prim N-1     │
                   └──────────────┘
```

### Address computation

C++ side (`allocateBuffers`):
```cpp
_primHandle = bufMgr->allocateBuffer(slotIndex, "prims", buf->gpuBufferSize());
float* buf = reinterpret_cast<float*>(_primHandle.data);
_buffer->writeGPU(buf, _primHandle.size, _primWordOffsets);
// _primWordOffsets[i] = word offset of prim i relative to data section start
```

Metadata (`flushMetadata`):
```cpp
meta.primitiveOffset = _primHandle.offset / sizeof(float);  // byte→word
meta.primitiveCount = _buffer->primCount();
```

Shader reads a prim via offset table:
```wgsl
let primitiveOffset = cardMetadata[metaOffset + 0];
let primitiveCount  = cardMetadata[metaOffset + 1];
let primDataBase    = primitiveOffset + primitiveCount;

// Read prim i via offset table:
let wordOff = bitcast<u32>(cardStorage[primitiveOffset + i]);
let primOff = primDataBase + wordOff;
let primType = bitcast<u32>(cardStorage[primOff + 0]);  // type
let cx       = cardStorage[primOff + 2];                // geometry
```

Shader reads a prim via grid entry:
```wgsl
let rawIdx  = bitcast<u32>(cardStorage[gridOffset + packedStart + 1 + i]);
let primOff = primDataBase + rawIdx;  // rawIdx IS a word offset (post-translation)
```

### Numeric example

Suppose `_primHandle.offset = 128` bytes and we have 3 prims: Circle(9w), Box(10w), RoundedBox(14w).

```
primitiveOffset = 128/4 = 32       (metadata word 0)
primitiveCount  = 3                 (metadata word 1)
primDataBase    = 32 + 3 = 35

cardStorage[32] = 0     ← wordOff[0] (Circle starts at primDataBase+0 = word 35)
cardStorage[33] = 9     ← wordOff[1] (Box starts at primDataBase+9 = word 44)
cardStorage[34] = 19    ← wordOff[2] (RoundedBox starts at primDataBase+19 = word 54)
cardStorage[35] = type=0 (Circle)
cardStorage[36] = layer
cardStorage[37] = cx
...
cardStorage[44] = type=1 (Box)
...
cardStorage[54] = type=8 (RoundedBox)
...
```

The shader resolves Circle: `primOff = 35 + 0 = 35`, reads `cardStorage[35]` = type 0.
The shader resolves Box: `primOff = 35 + 9 = 44`, reads `cardStorage[44]` = type 1.
The shader resolves RoundedBox: `primOff = 35 + 19 = 54`, reads `cardStorage[54]` = type 8.

## GPU Buffer Layout — Derived Buffer (Grid + Glyphs)

Written by `YDrawBuilder::writeDerived()` into the `"derived"` allocation.
This is a **separate** allocation from the prim buffer — at a different offset in
`cardStorage`.

```
derivedHandle.offset ──►┌──────────────────┐
                        │ GRID DATA        │  _gridStaging[] copied + translated
                        │ [cell offsets]   │  (numCells u32s: one per grid cell)
                        │ [packed entries] │  ([count, entry, entry, ...] per cell)
                        ├──────────────────┤
                        │ GLYPH DATA       │  _glyphs[] copied (5 u32s each)
                        ├──────────────────┤  (only if FLAG_CUSTOM_ATLAS)
                        │ ATLAS HEADER     │  4 u32s
                        │ GLYPH METADATA   │  10 floats per atlas glyph
                        └──────────────────┘
```

Address computation:
```cpp
_gpuGridOffset  = (_derivedHandle.offset + 0)         / sizeof(float);
_gpuGlyphOffset = (_derivedHandle.offset + gridBytes) / sizeof(float);
```

These word offsets go directly into metadata words 2 and 6.

### Grid format (variable-length)

The grid has two sections packed contiguously:
1. **Cell offset table** — `gridWidth * gridHeight` u32s
2. **Packed entry lists** — `[count, entry0, entry1, ...]` per cell

```
gridOffset ──►┌───────────┐  cardStorage[gridOffset + 0]
              │ off[0]    │  → packed offset of cell 0's entry list (relative to gridOffset)
              │ off[1]    │  → packed offset of cell 1's entry list
              │ ...       │
              │ off[N-1]  │  → packed offset of cell N-1's entry list  (N = gridW * gridH)
              ├───────────┤
 off[0]──────►│ count     │  number of entries in cell 0
              │ entry_0   │  prim word offset OR glyph index | 0x80000000
              │ entry_1   │
              │ ...       │
 off[1]──────►│ count     │  number of entries in cell 1
              │ entry_0   │
              │ ...       │
              └───────────┘
```

Grid cell index from scene position:
```wgsl
let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSize, 0.0, f32(gridWidth - 1)));
let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSize, 0.0, f32(gridHeight - 1)));
let cellIndex = cellY * gridWidth + cellX;
```

Reading entries:
```wgsl
let packedStart    = bitcast<u32>(cardStorage[gridOffset + cellIndex]);
let cellEntryCount = bitcast<u32>(cardStorage[gridOffset + packedStart]);
// entries at: cardStorage[gridOffset + packedStart + 1 + i]
```

### Grid entry types

Each entry is a u32:
- **Bit 31 clear** → SDF primitive. Value is the **word offset** relative to
  `primDataBase`. Shader: `primOff = primDataBase + rawIdx`
- **Bit 31 set** → Text glyph. Lower 31 bits = glyph index.
  Shader: `glyphOffset + (rawIdx & 0x7FFFFFFF) * 5`

### Grid translation (critical!)

During `calculate()`, grid entries are **prim indices** (0, 1, 2, ...) matching
the iteration order of `forEachPrim()`.

During `writeDerived()`, these are **translated** to word offsets:
```cpp
gridPtr[idx] = _primWordOffsets[gridPtr[idx]];
// e.g. prim index 2 → wordOffset 19 (from the numeric example above)
```

This translation is critical. Without it, the shader would compute
`primDataBase + 2` instead of `primDataBase + 19` and read garbage.

The `_primWordOffsets` vector is populated by `writeGPU()`. Its iteration
order matches `forEachPrim()` — both iterate the same `unordered_map`.

### Numeric example (grid)

Suppose `_derivedHandle.offset = 512` bytes, `gridWidth = 4`, `gridHeight = 3`
(12 cells), and cell (1,1) contains prim 0 (Circle, wordOff=0) and prim 2
(RoundedBox, wordOff=19).

```
gridOffset = 512/4 = 128           (metadata word 2)

cardStorage[128] = off[0] = 12     ← cell 0 packed list at gridOffset+12
cardStorage[129] = off[1] = 13     ← cell 1 (empty, count=0)
...
cardStorage[133] = off[5] = 14     ← cell (1,1) packed list at gridOffset+14
...
cardStorage[140] = 0               ← cell 0: count=0
cardStorage[141] = 0               ← cell 1: count=0
...
cardStorage[142] = 2               ← cell (1,1): count=2
cardStorage[143] = 0               ← entry: wordOff=0 (Circle, translated from index 0)
cardStorage[144] = 19              ← entry: wordOff=19 (RoundedBox, translated from index 2)
```

Shader lookup at position in cell (1,1):
```
cellIndex = 1*4 + 1 = 5
packedStart = cardStorage[128 + 5] = 14
count = cardStorage[128 + 14] = 2
entry0 = cardStorage[128 + 14 + 1] = 0   → primOff = primDataBase + 0
entry1 = cardStorage[128 + 14 + 2] = 19  → primOff = primDataBase + 19
```

## YDrawGlyph (20 bytes = 5 u32s)

```
Word 0: x (f32)           position in scene coords
Word 1: y (f32)
Word 2: width (f16 low) | height (f16 high)    ← packed via pack2x16float
Word 3: glyphIndex (u16 low) | layer (u8) | flags (u8 high)
Word 4: color (u32)       packed RGBA
```

Glyph flags (bits 24-31 of word 3):
- `1`: GLYPH_FLAG_CUSTOM_ATLAS — use custom atlas, not shared
- `2`: GLYPH_FLAG_SELECTED — draw selection highlight behind glyph

## Metadata (64 bytes = 16 u32s)

Written by `YDrawBuilder::flushMetadata()` to `cardMetadata[]` via
`cardMgr->writeMetadata()`. The slot offset is `metaSlotIndex * 64` bytes =
`metaSlotIndex * 16` u32 words.

```
Word  Field            Type   Description
────  ─────            ────   ───────────
  0   primitiveOffset  u32    word offset into cardStorage[] for prim buffer start
  1   primitiveCount   u32    number of primitives
  2   gridOffset       u32    word offset into cardStorage[] for grid start
  3   gridWidth        u32    grid columns
  4   gridHeight       u32    grid rows
  5   cellSize         f32    grid cell size in scene units (stored as bitcast u32)
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

Shader reads: `cardMetadata[slotIndex * 16 + N]` for word N.

### Key metadata address relationships

```
primitiveOffset = _primHandle.offset / 4          (byte offset → word offset)
primDataBase    = primitiveOffset + primitiveCount (shader computes this)
gridOffset      = _derivedHandle.offset / 4
glyphOffset     = (_derivedHandle.offset + gridBytes) / 4
```

### Flags

| Bit | Name | Description |
|-----|------|-------------|
| 1 | SHOW_BOUNDS | Debug: draw bounding boxes |
| 2 | SHOW_GRID | Debug: draw grid lines |
| 4 | SHOW_EVAL_COUNT | Debug: heatmap of SDF evals per pixel |
| 8 | HAS_3D | Enable 3D raymarching pass |
| 16 | UNIFORM_SCALE | Preserve aspect ratio (circles stay circular) |
| 32 | CUSTOM_ATLAS | Custom MSDF atlas data present after glyph data |

### Pan/Zoom encoding

Pan fixedpoint: `panScene = i16value / 16384.0 * sceneExtent`

Zoom is IEEE 754 half-float (f16) in upper 16 bits of word 14.
Shader converts f16→f32 manually. Default zoom = 1.0.

## YDrawBuilder

Computes AABBs, builds spatial hash grid, manages GPU buffer lifecycle.
Takes a `YDrawBuffer` at creation time (shared pointer).

### Factory

```cpp
// Full — with CardManager for GPU lifecycle
YDrawBuilder::create(fontManager, gpuAllocator, buffer, cardMgr, metaSlotIndex);

// Lightweight — no GPU (for tests, offline tools)
YDrawBuilder::create(fontManager, gpuAllocator, buffer);
```

### calculate()

Called after all primitives/text have been added to the buffer. Does:

1. **Clear** builder-owned staging: `_primBounds`, `_gridStaging`, `_glyphs`
2. **Ingest buffer metadata**: register font blobs with MSDF atlas, convert text
   spans to glyphs via `addText()`/`addRotatedText()`
3. **Read buffer scene metadata**: bounds, bgColor, flags (buffer values used
   only if builder has no explicit bounds set)
4. **Compute AABBs**: iterate `forEachPrim()`, compute bounding box per prim,
   store in `_primBounds`
5. **Compute scene bounds** (if no explicit bounds): min/max of all AABBs + glyphs
   with 5% padding
6. **Compute grid dimensions**: auto cell size based on average prim area, capped
   at 4M total cells
7. **Build grid**: two-pass — count entries per cell, prefix-sum for offsets, fill
   entries. Prim entries are **indices** (0, 1, 2, ...) into `_primBounds`. Glyph
   entries have `GLYPH_BIT` set.
8. Set `_bufferDirty = true`

### AABB computation

Per-type bounding box:

| Type | AABB |
|------|------|
| Circle | center +/- (r + strokeWidth/2) |
| Box | center +/- (hw + round + strokeWidth/2, hh + round + strokeWidth/2) |
| RoundedBox | center +/- (hw + maxRadius + strokeWidth/2, hh + maxRadius + strokeWidth/2) |
| Triangle | min/max of 3 vertices +/- strokeWidth/2 |
| Segment | min/max of 2 endpoints +/- strokeWidth/2 |
| Ellipse | center +/- (rx + strokeWidth/2, ry + strokeWidth/2) |

## Card Lifecycle

Cards forward their `Card::` lifecycle overrides to the builder. The card manager
calls these in order each frame:

```
1. renderToStaging(time)
   └─ Card adds prims to buffer, calls builder->calculate()

2. needsBufferRealloc()
   └─ Builder checks: _primHandle.size != buffer.gpuBufferSize()
      OR computeDerivedSize() > _derivedHandle.size
      OR no handles allocated yet

3. declareBufferNeeds()         ← only if needsBufferRealloc()
   └─ bufMgr->reserve(primSize)
      bufMgr->reserve(derivedSize)
      Resets _primHandle, _derivedHandle, _primWordOffsets to invalid/empty

4. [gpu-screen calls bufMgr->commitReservations()]
   └─ Resets allocator with total needed size, recreates GPU buffer if needed

5. allocateBuffers()            ← only if needsBufferRealloc()
   └─ _primHandle = bufMgr->allocateBuffer(slot, "prims", primBytes)
      writeGPU(_primHandle.data, primBytes, _primWordOffsets)
      bufMgr->markBufferDirty(_primHandle)
      _derivedHandle = bufMgr->allocateBuffer(slot, "derived", derivedBytes)
      writeDerived()  ← copies grid (with translation) + glyphs to _derivedHandle.data
      bufMgr->markBufferDirty(_derivedHandle)
      _bufferDirty = false, _metadataDirty = true

6. needsTextureRealloc()
7. allocateTextures()           ← if custom atlas
8. writeTextures()              ← if custom atlas

9. finalize()
   └─ builder->writeBuffers():
      If _bufferDirty: re-write prim data + derived data
      If _metadataDirty: flushMetadata() → cardMgr->writeMetadata()
```

### writeGPU()

Called on the buffer with the prim handle's data pointer:

```cpp
float* buf = reinterpret_cast<float*>(_primHandle.data);
_buffer->writeGPU(buf, _primHandle.size, _primWordOffsets);
```

Implementation (ydraw-buffer.cpp):
```cpp
void YDrawBuffer::writeGPU(float* buf, uint32_t bufBytes,
                           vector<uint32_t>& wordOffsets) const {
    uint32_t count = _prims.size();
    wordOffsets.resize(count);
    float* dataBase = buf + count;  // data starts after offset table
    uint32_t dataOffset = 0;
    uint32_t i = 0;
    for (const auto& [id, pd] : _prims) {
        wordOffsets[i] = dataOffset;
        memcpy(&buf[i], &dataOffset, sizeof(uint32_t));  // offset table entry
        memcpy(dataBase + dataOffset, pd.words.data(), pd.words.size() * 4);
        dataOffset += pd.words.size();
        i++;
    }
}
```

Key: `wordOffsets[i]` and the offset table entry `buf[i]` contain the **same value**
— the word offset of prim i relative to the data section.

### writeDerived()

1. Copy `_gridStaging` to `_derivedHandle.data`
2. **Translate** grid prim entries from indices to word offsets:
   ```cpp
   if (rawVal < _primWordOffsets.size())
       gridPtr[idx] = _primWordOffsets[rawVal];
   ```
   (skips entries with `GLYPH_BIT` set)
3. Copy `_glyphs` to derived handle after grid
4. If custom atlas: write atlas header + glyph metadata

### flushMetadata()

Packs all metadata fields into a 64-byte `YDrawMetadata` struct and writes
it to the metadata buffer via `cardMgr->writeMetadata()`.

Key field computations:
```cpp
meta.primitiveOffset = _primHandle.offset / sizeof(float);  // byte→word
meta.primitiveCount  = _buffer->primCount();
meta.gridOffset      = _gpuGridOffset;   // = _derivedHandle.offset / 4
meta.glyphOffset     = _gpuGlyphOffset;  // = (_derivedHandle.offset + gridBytes) / 4
meta.cellSize        = bitcast(_cellSize);
meta.gridWidth       = _gridWidth;
meta.gridHeight      = _gridHeight;
```

## Shader (0x0003-ydraw.wgsl) — Full Read Path

### 1. Read metadata

```wgsl
let slotIndex = fg & 0xFFFFFF;
let metaOffset = slotIndex * 16;
let primitiveOffset = cardMetadata[metaOffset + 0];
let primitiveCount  = cardMetadata[metaOffset + 1];
let gridOffset      = cardMetadata[metaOffset + 2];
let gridWidth       = cardMetadata[metaOffset + 3];
let gridHeight      = cardMetadata[metaOffset + 4];
let cellSize        = bitcast<f32>(cardMetadata[metaOffset + 5]);
let glyphOffset     = cardMetadata[metaOffset + 6];
let glyphCount      = cardMetadata[metaOffset + 7];
// ... scene bounds from words 8-11, view from 12-14, bg from 15
```

### 2. Compute primDataBase

```wgsl
let primDataBase = primitiveOffset + primitiveCount;
```

### 3. Compute scene position from UV

Widget UV → scene position via view transform (zoom/pan/aspect).

### 4. O(1) grid lookup

```wgsl
let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSize, 0.0, f32(gridWidth-1)));
let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSize, 0.0, f32(gridHeight-1)));
let cellIndex = cellY * gridWidth + cellX;
let packedStart = bitcast<u32>(cardStorage[gridOffset + cellIndex]);
let cellEntryCount = bitcast<u32>(cardStorage[gridOffset + packedStart]);
```

### 5. Evaluate each entry

```wgsl
for (var i = 0u; i < cellEntryCount; i++) {
    let rawIdx = bitcast<u32>(cardStorage[gridOffset + packedStart + 1 + i]);

    if ((rawIdx & 0x80000000) != 0) {
        // GLYPH: gi = rawIdx & 0x7FFFFFFF
        // read from cardStorage[glyphOffset + gi * 5 + ...]
    } else {
        // SDF PRIMITIVE:
        let primOff = primDataBase + rawIdx;
        let d = evalSDF(primOff, scenePos);
        let colors = primColors(primOff);   // returns (fillColor, strokeColor, layer, 0)
        // fill:   d < 0 && fillColor != 0 → blend
        // stroke: |d| < strokeWidth/2 && strokeColor != 0 → blend
    }
}
```

### What makes a prim visible

A prim renders if **all** of these are true:
1. Its AABB overlaps at least one grid cell (computed during `calculate()`)
2. The pixel's scene position maps to a cell containing that prim
3. `evalSDF(primOff, scenePos)` returns `d < 0` (fill) or `|d| < strokeWidth/2` (stroke)
4. The corresponding color (fillColor or strokeColor) is **non-zero**

If fillColor == 0 and strokeColor == 0, the prim is invisible even if present in the grid.

## Example: ygui button rendering

```cpp
// In rebuildPrimitives():
_buffer->clear();
_builder->setSceneBounds(0, 0, pixelWidth, pixelHeight);

// Button background — RoundedBox (id=0)
_buffer->addRoundedBox(_buffer->primCount(),   // id = 0
    cx, cy, hw, hh, r, r, r, r,
    bgColor, 0, 0.0f, 0.0f);

// Button text — text span (NOT a prim, converted to glyphs by calculate())
_buffer->addText(textX, textY, "Click", 14.0f, textColor, 0, -1);

// Hover outline — RoundedBox with stroke only (id=1)
if (hovered) {
    _buffer->addRoundedBox(_buffer->primCount(), // id = 1
        cx, cy, hw, hh, r, r, r, r,
        0, accentColor, 2.0f, 0.0f);            // fill=0, stroke=accent
}

_builder->calculate();
// → _primBounds has 1-2 entries, _glyphs has N glyphs
// → grid built with prim indices (0, 1), _bufferDirty = true
```

Then the card lifecycle:
1. `needsBufferRealloc()` → true (new buffer content)
2. `declareBufferNeeds()` → reserve prim + derived space
3. `commitReservations()` → resize GPU buffer
4. `allocateBuffers()` → write prim data to handle, translate grid, write derived
5. `writeBuffers()` → flush metadata to cardMetadata[]
6. `flush()` → upload dirty ranges to GPU
7. Shader renders using O(1) grid lookup per pixel

## Testing: Mock CardManager

To test the real builder write pipeline without GPU, create mock implementations
of `CardBufferManager` and `CardManager` that use plain CPU byte arrays:

```cpp
class MockCardBufferManager : public CardBufferManager {
    std::vector<uint8_t> _storage;      // CPU-side cardStorage
    uint32_t _nextOffset = 0;
    uint32_t _pendingReservation = 0;
    std::map<std::pair<uint32_t,std::string>, BufferHandle> _allocs;
public:
    void reserve(uint32_t size) override { _pendingReservation += (size+15)&~15u; }
    Result<void> commitReservations() override {
        _storage.resize(_pendingReservation);
        _nextOffset = 0;
        _allocs.clear();
        _pendingReservation = 0;
        return Ok();
    }
    Result<BufferHandle> allocateBuffer(uint32_t slot, const std::string& scope, uint32_t size) override {
        size = (size+15)&~15u;
        BufferHandle h{_storage.data() + _nextOffset, _nextOffset, size};
        _allocs[{slot, scope}] = h;
        _nextOffset += size;
        return Ok(h);
    }
    void markBufferDirty(BufferHandle) override {}
    // ... other methods return defaults
    const uint8_t* data() const { return _storage.data(); }
    const float* storageAsFloat() const { return reinterpret_cast<const float*>(_storage.data()); }
    BufferHandle getBufferHandle(uint32_t s, const std::string& sc) const override {
        auto it = _allocs.find({s, sc});
        return it != _allocs.end() ? it->second : BufferHandle::invalid();
    }
};

class MockCardManager : public CardManager {
    std::shared_ptr<MockCardBufferManager> _bufMgr;
    std::vector<uint8_t> _metadata;
public:
    MockCardManager() : _bufMgr(std::make_shared<MockCardBufferManager>()),
                        _metadata(16 * 64, 0) {}
    CardBufferManager::Ptr bufferManager() const override { return _bufMgr; }
    Result<void> writeMetadata(MetadataHandle h, const void* data, uint32_t size) override {
        memcpy(_metadata.data() + h.offset, data, size);
        return Ok();
    }
    // ... other methods return defaults/nullptr
    const uint32_t* metadataAsU32() const { return reinterpret_cast<const uint32_t*>(_metadata.data()); }
    auto mockBufMgr() { return _bufMgr; }
};
```

Test pattern:
```cpp
auto buf = *YDrawBuffer::create();
buf->addRoundedBox(0, 100, 100, 40, 30, 5,5,5,5, 0xFFAABBCC, 0, 0, 0);

auto mockCM = std::make_shared<MockCardManager>();
auto builder = *YDrawBuilder::create(nullptr, nullptr, buf, mockCM, /*slot=*/0);
builder->setSceneBounds(0, 0, 200, 200);
builder->calculate();
builder->declareBufferNeeds();
mockCM->mockBufMgr()->commitReservations();
builder->allocateBuffers();
builder->writeBuffers();

// Now inspect what the builder actually wrote:
auto primH = mockCM->mockBufMgr()->getBufferHandle(0, "prims");
auto derivedH = mockCM->mockBufMgr()->getBufferHandle(0, "derived");
const float* storage = mockCM->mockBufMgr()->storageAsFloat();
const uint32_t* meta = mockCM->metadataAsU32();

uint32_t primOff = meta[0];  // primitiveOffset
uint32_t primCnt = meta[1];  // primitiveCount
uint32_t gridOff = meta[2];  // gridOffset
// ... verify types, colors, grid entries
```
