# Scrolling YPaintBuilder Data Structure

## Overview

The scrolling YPaintBuilder implements terminal-style scrolling for SDF primitives. Primitives are stored at the LAST (lowest) line they overlap, enabling O(1) scroll removal via `pop_front()`.

## State Variables

```cpp
bool _scrollingMode = false;      // true for scrolling overlay
uint16_t _cursorCol = 0;          // current cursor column
uint16_t _cursorRow = 0;          // current cursor row (decremented on scroll)
```

## Core Data Structures

### ScrollPrimRef
Reference to a primitive from a grid cell:
```cpp
struct ScrollPrimRef {
    uint16_t linesAhead;  // offset to base line (0 = same line, N = N lines down)
    uint16_t primIndex;   // index within base line's prims vector
};
```

### ScrollGridCell
A single grid cell (one X column within a line):
```cpp
struct ScrollGridCell {
    std::vector<ScrollPrimRef> refs;  // primitives overlapping this cell
};
```

### ScrollLine
A single line in the scroll buffer:
```cpp
struct ScrollLine {
    std::vector<std::vector<float>> prims;  // primitives with BASE at this line
    std::vector<ScrollGridCell> cells;       // grid cells indexed by X column
};
```

### Line Storage
```cpp
std::deque<ScrollLine> _scrollLines;  // front=line 0 (top), pop_front on scroll
```

## Primitive Data Layout (GPU Format)

Each primitive in `ScrollLine::prims` is a `std::vector<float>` with **gridOffset prepended**:

| Word | Content | Description |
|------|---------|-------------|
| 0 | **gridOffset** | Packed: `(gridOffsetCol \| gridOffsetRow << 16)` as u32 bits |
| 1 | type | Primitive type (u32) - from original word[0] |
| 2 | layer | Layer/z-order (u32) - from original word[1] |
| 3+ | geometry | Original primitive data starting from original word[2] |

**CRITICAL FORMAT CHANGE**:
- OLD format: `[type][layer][gridOffset][geometry...]` (gridOffset at +2)
- NEW format: `[gridOffset][type][layer][geometry...]` (gridOffset at +0)

The gridOffset is:
- Set from cursor position at primitive addition time
- Used by shader to translate LOCAL coordinates to WORLD coordinates
- **DECREMENTED** when scrolling occurs (can go negative!)

**Layout Rule**: gridOffset FIRST, then original primitive layout shifted by 1.

**Shader must read**: type from +1, layer from +2, geometry from +3+.

## Adding Primitives (addYpaintBuffer)

When a primitive with local Y coordinates `[minY, maxY]` is added at cursor position `(cursorCol, cursorRow)`:

1. Compute local line range:
   ```cpp
   localMinLine = floor(minY / cellSizeY)
   localMaxLine = floor(maxY / cellSizeY)
   ```

2. Compute absolute grid lines:
   ```cpp
   primMinLine = cursorRow + localMinLine
   primMaxLine = cursorRow + localMaxLine
   ```

3. Store primitive at BASE line (last/lowest overlapping line):
   ```cpp
   baseLine = primMaxLine
   _scrollLines[baseLine].prims.push_back(primData)
   ```

4. Add refs to ALL overlapping lines:
   ```cpp
   for (lineIdx = primMinLine; lineIdx <= primMaxLine; lineIdx++) {
       linesAhead = baseLine - lineIdx
       _scrollLines[lineIdx].cells[cx].refs.push_back({linesAhead, primIndex})
   }
   ```

5. Store cursor offset in word[0] (NEW FORMAT):
   ```cpp
   primData[0] = pack(cursorCol, cursorRow)  // gridOffset at word[0]!
   primData[1] = originalData[0]  // type (shifted from original position)
   primData[2] = originalData[1]  // layer (shifted)
   // ... rest of data shifted by 1
   ```

## Scrolling (scrollLines)

When `scrollLines(N)` is called:

1. Remove N lines from front:
   ```cpp
   for (i = 0; i < N; i++) _scrollLines.pop_front()
   ```

2. **DECREMENT offset in ALL remaining primitives** (word[0], not word[2]!):
   ```cpp
   for (line : _scrollLines) {
       for (prim : line.prims) {
           // gridOffset is at word[0] in NEW FORMAT
           row = unpack_row(prim[0])
           row -= N  // CAN GO NEGATIVE! (signed int16)
           prim[0] = pack(col, row)
       }
   }
   ```

3. Update cursor:
   ```cpp
   _cursorRow -= N
   ```

4. Mark dirty:
   ```cpp
   _bufferDirty = true
   _gridStagingDirty = true
   ```

**NOTE**: `linesAhead` refs remain valid because they are RELATIVE offsets.

## Grid Lookup

To find primitive data from a grid cell reference:
```cpp
ScrollPrimRef ref = _scrollLines[lineIdx].cells[colIdx].refs[i];
std::vector<float>& primData = _scrollLines[lineIdx + ref.linesAhead].prims[ref.primIndex];
```

## Shader Coordinate Translation

The shader reads gridOffset from **word[0]** (not word[2]!) and translates coordinates:

```wgsl
// NEW FORMAT: gridOffset at +0, type at +1, layer at +2, geometry at +3+
let gridOffsetPacked = bitcast<u32>(scrollingStorage[primOffset + 0u]);
let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
let gridOffsetY = f32(gridOffsetPacked >> 16u);
let pixelOffset = vec2<f32>(gridOffsetX * scrolling.cellSizeX, gridOffsetY * scrolling.cellSizeY);
let pAdj = p - pixelOffset;  // Adjusted position for SDF evaluation

let primType = bitcast<u32>(scrollingStorage[primOffset + 1u]);  // type at +1
```

**Formula**: `localPos = worldPos - (gridOffset * cellSize)`

## GPU Grid Format (_gridStaging)

The packed grid format for GPU:
```
[offset_cell_0][offset_cell_1]...[offset_cell_N-1][cell_0_data][cell_1_data]...

cell_data = [count][primIdx_0][primIdx_1]...
```

Where `primIdx` is the global primitive index (linearized across all lines).

## Shader Grid Lookup

```wgsl
let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSizeX, 0.0, f32(gridWidth - 1u)));
let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSizeY, 0.0, f32(gridHeight - 1u)));
let cellIndex = cellY * gridWidth + cellX;
```

**CRITICAL**: The shader looks up using WORLD pixel coordinates. The grid must be organized such that world coordinate Y maps to the correct grid row.

## Signed Grid Offset (Critical Fix)

After scrolling, `gridOffsetRow` can become **negative** (e.g., -3 for a primitive that started at row 3 and scrolled up 6 lines). The shader MUST interpret this as a signed int16, not unsigned.

**Bug (fixed)**: Shader was reading `gridOffsetY = f32(gridOffsetPacked >> 16u)`, which interprets -3 as 65533, causing massive coordinate offset and invisible primitives.

**Fix**: Use arithmetic right shift to preserve sign:
```wgsl
let gridOffsetYRaw = i32(gridOffsetPacked) >> 16;  // arithmetic shift preserves sign
let gridOffsetY = f32(gridOffsetYRaw);
```

## Current Limitations

1. **Rotated text not supported** - The scrolling shader doesn't handle SDF_ROTATED_GLYPH (type 65). Rotated text in scrolling mode is skipped with a warning.

2. **Custom fonts not supported** - The scrolling shader only samples from the shared `fontTexture`, not custom atlas textures. Custom fonts fall back to the default font with a warning.

## Grid Offset Explained

### What is the Grid Offset?

The **grid offset** is a per-primitive value stored in **word[0]** that tells the shader how to translate the primitive's LOCAL coordinates to WORLD coordinates.

```
GPU Format: [gridOffset][type][layer][geometry...]
word[0] = packed(gridOffsetCol | gridOffsetRow << 16)  // u32 stored as float bits
word[1] = type
word[2] = layer
word[3+] = geometry
```

### Why Do Scrolling Primitives Need a Grid Offset?

In scrolling mode, primitives are stored with **local coordinates** (relative to position 0,0). The grid offset tells the shader where in the world these local coordinates should be rendered.

**Without grid offset**: A circle at local (100, 120) would always render at pixel (100, 120) on screen, regardless of where in the terminal it was added.

**With grid offset**: A circle at local (100, 120) with gridOffsetRow=3 renders at pixel (100, 120 + 3*cellSizeY) = (100, 180) when cellSizeY=20.

### How the Grid Offset Works

1. **At primitive addition time**:
   - Primitive has local coordinates (e.g., circle center at local Y=120)
   - Current grid position is recorded as gridOffsetRow (e.g., 3)
   - Primitive is stored in grid at row = gridOffsetRow + localRow

2. **At render time (shader)**:
   - Shader samples at world pixel position (e.g., Y=180)
   - Reads gridOffsetRow from primitive word[0]
   - Computes: `localPos = worldPos - gridOffset * cellSize`
   - Evaluates SDF at localPos (e.g., 180 - 60 = 120)

3. **At scroll time**:
   - Grid lines are removed from front (`pop_front`)
   - gridOffsetRow in ALL remaining primitives is **DECREMENTED**
   - This keeps the world-to-local translation correct

### Example: Scrolling a Circle

Initial state (cursor at row 3):
```
Circle: local center (100, 120), gridOffsetRow = 3
World center: (100, 120 + 3*20) = (100, 180)
Stored in grid row: 3 + floor(120/20) = 3 + 6 = 9
```

After scrolling 3 lines:
```
Circle: local center (100, 120), gridOffsetRow = 0 (decremented by 3)
World center: (100, 120 + 0*20) = (100, 120)
Now in grid row: 9 - 3 = 6 (line indices shifted)
```

The circle appears to have scrolled up by 3 lines (60 pixels).

### Grid Offset vs Cursor Position

The grid offset is SET from the cursor position at addition time, but they are NOT the same thing:

- **Cursor position**: Where the terminal cursor is NOW (changes as user types)
- **Grid offset**: Where this primitive was placed IN THE GRID (fixed at addition, decremented on scroll)

After scrolling, cursor position and grid offset diverge. The grid offset tracks where the primitive should render relative to the current scroll position.
