# YDraw Improvements Design Plan

## Problem Statement

Current issues with ydraw screen overlay:
1. Full grid rebuild on every content addition (inefficient for many primitives)
2. AABB calculated for ALL content, not just new content (wrong scroll amount)
3. No distinction between inline (scrolling) and absolute (fixed) content
4. Cursor position not synchronized between terminal and ydraw
5. YDrawBuilder tied to single YDrawBuffer at construction (inflexible)

## Architecture Overview

### Two Synchronized Grids

```
Terminal Grid                    YDraw Overlay Grid
┌───┬───┬───┬───┐               ┌───┬───┬───┬───┐
│   │   │   │   │  row 0        │   │   │   │   │
├───┼───┼───┼───┤               ├───┼───┼───┼───┤
│   │   │   │   │  row 1        │   │   │   │   │
├───┼───┼───┼───┤               ├───┼───┼───┼───┤
│   │ * │   │   │  row 2        │   │ ● │   │   │  (cursor + inline content)
└───┴───┴───┴───┘               └───┴───┴───┴───┘
      ↑                               ↑
   cursor                     cursor-relative content
```

**Key principle**: Both grids use the same cell size and must scroll together for inline content.

### Content Types

| Type | `isInline` | Scrolls | Cursor-relative | Use Case |
|------|------------|---------|-----------------|----------|
| Inline | `true` | Yes | Yes | Charts, diagrams in terminal flow |
| Absolute | `false` | No | No | HUD, floating UI, overlays |
| Card | N/A | With cell | N/A | Embedded widgets |

## Proposed API Changes

### YDrawBuilder

```cpp
class YDrawBuilder {
public:
    // === NEW: Builder owns its internal buffer ===
    static Result<Ptr> create(FontManager*, GpuAllocator*);  // No buffer param

    // === NEW: Cursor position (synced from gpu-screen) ===
    void setCursorPosition(float x, float y);
    float cursorX() const;
    float cursorY() const;

    // === NEW: Add content from external buffer ===
    // - isInline=true: positions at cursor, content will scroll with terminal
    // - isInline=false: absolute position, content stays fixed
    // - Returns AABB of NEW content only (for scroll calculation)
    // - Updates grid incrementally (no full rebuild)
    Result<YDrawAABB> addYDrawBuffer(YDrawBuffer::Ptr content, bool isInline);

    // === MODIFIED: Scroll only affects inline content ===
    // Shifts grid references for inline primitives only
    // Absolute primitives remain in their original grid cells
    void scroll(int32_t numLines);

    // === NEW: Clear by type ===
    void clearInline();   // Clear only inline content
    void clearAbsolute(); // Clear only absolute content
    void clearAll();      // Clear everything

    // === Existing ===
    void setGridCellSize(float w, float h);
    void calculate();  // Still needed for initial setup
    // ... other existing methods

private:
    YDrawBuffer::Ptr _internalBuffer;
    float _cursorX = 0, _cursorY = 0;

    // Track which primitives are inline vs absolute
    std::unordered_set<uint32_t> _inlinePrimIds;
    std::unordered_set<uint32_t> _absolutePrimIds;
};
```

### YDrawAABB (new struct)

```cpp
struct YDrawAABB {
    float minX = 1e10f, minY = 1e10f;
    float maxX = -1e10f, maxY = -1e10f;
    bool valid = false;

    void expand(float x, float y);
    void expand(float x, float y, float w, float h);
    float width() const { return maxX - minX; }
    float height() const { return maxY - minY; }
    uint32_t heightInCells(float cellHeight) const;
};
```

### ScreenDrawLayer

```cpp
class ScreenDrawLayer {
public:
    // === MODIFIED: Uses new builder API ===

    // Called when terminal cursor moves
    void setCursorPosition(float x, float y);

    // Add new content, returns height in cells for scroll
    Result<uint32_t> addContent(const std::string& payload, bool isInline);

    // Scroll (delegated to builder, only affects inline)
    void scroll(int32_t numLines);

    // === Existing ===
    void prepareForRender();
    bool hasContent() const;
    // ... buffer accessors
};
```

## Implementation Details

### 1. Primitive Tracking

Each primitive added via `addYDrawBuffer()` gets tagged as inline or absolute:

```cpp
Result<YDrawAABB> YDrawBuilder::addYDrawBuffer(YDrawBuffer::Ptr content, bool isInline) {
    YDrawAABB aabb;
    float offsetX = isInline ? _cursorX : 0;
    float offsetY = isInline ? _cursorY : 0;

    content->forEachPrim([&](uint32_t id, const float* data, uint32_t wordCount) {
        // Copy primitive to internal buffer with offset applied
        uint32_t newId = copyPrimWithOffset(data, wordCount, offsetX, offsetY);

        // Track inline vs absolute
        if (isInline) {
            _inlinePrimIds.insert(newId);
        } else {
            _absolutePrimIds.insert(newId);
        }

        // Expand AABB with primitive bounds
        expandAABB(aabb, newId);
    });

    // Incremental grid update for new primitives only
    updateGridIncremental(newPrimIds);

    return Ok(aabb);
}
```

### 2. Incremental Grid Update

Instead of rebuilding entire grid:

```cpp
void YDrawBuilder::updateGridIncremental(const std::vector<uint32_t>& newPrimIds) {
    for (uint32_t primId : newPrimIds) {
        auto& bounds = _primBounds[primId];

        // Find affected grid cells
        uint32_t minCellX = worldToCell(bounds.minX);
        uint32_t maxCellX = worldToCell(bounds.maxX);
        uint32_t minCellY = worldToCell(bounds.minY);
        uint32_t maxCellY = worldToCell(bounds.maxY);

        // Add primitive to each affected cell
        for (uint32_t cy = minCellY; cy <= maxCellY; cy++) {
            for (uint32_t cx = minCellX; cx <= maxCellX; cx++) {
                _gridCells[cy][cx].push_back(primId);
            }
        }
    }

    // Rebuild flat GPU buffer from per-row structure
    rebuildGpuGrid();
}
```

### 3. Scroll with Inline/Absolute Separation

```cpp
void YDrawBuilder::scroll(int32_t numLines) {
    if (numLines == 0) return;

    float offsetY = numLines * _cellSizeY;

    // Only update inline primitive positions in grid
    for (uint32_t primId : _inlinePrimIds) {
        // Remove from old cells
        removeFromGrid(primId);

        // Update bounds
        _primBounds[primId].minY -= offsetY;
        _primBounds[primId].maxY -= offsetY;

        // Skip if scrolled completely out
        if (_primBounds[primId].maxY < 0) {
            _inlinePrimIds.erase(primId);
            _primBounds.erase(primId);
            continue;
        }

        // Add to new cells
        addToGrid(primId);
    }

    // Absolute primitives unchanged

    rebuildGpuGrid();
}
```

### 4. Cursor Synchronization

In `gpu-screen.cpp`:

```cpp
// When terminal cursor moves
void GPUScreenImpl::onCursorMove(int row, int col) {
    _cursorRow = row;
    _cursorCol = col;

    // Sync to ydraw builder
    if (_screenDrawLayer) {
        float x = col * getCellWidthF();
        float y = row * getCellHeightF();
        _screenDrawLayer->setCursorPosition(x, y);
    }
}

// When terminal scrolls
int GPUScreenImpl::onScrollRect(VTermRect rect, int downward, ...) {
    // ... existing scrollback logic ...

    // Sync scroll to ydraw (inline content only)
    if (_screenDrawLayer && _screenDrawLayer->hasContent()) {
        _screenDrawLayer->scroll(downward);
        _needsBindGroupRecreation = true;
    }

    return 0;
}
```

### 5. Content Addition Flow

```cpp
// In gpu-screen.cpp OSC handler
void handleOSC666673(const std::string& args, const std::string& payload) {
    bool isInline = !hasExplicitPosition(args);

    // Parse into temporary buffer
    auto tempBuffer = YDrawBuffer::create();
    parseYDrawYAML(tempBuffer, payload);

    // Add to screen layer
    auto result = _screenDrawLayer->addContent(tempBuffer, isInline);
    if (!result) return;

    uint32_t heightCells = *result;

    // For inline: scroll terminal by content height
    if (isInline && heightCells > 0) {
        injectNewlines(heightCells);
    }
}
```

## Per-Row Grid Storage

For efficient scroll, store grid as vector of rows:

```cpp
// Instead of flat array
std::vector<uint32_t> _gridStaging;  // OLD

// Use per-row storage
struct GridRow {
    std::vector<std::vector<uint32_t>> cells;  // cells[col] = list of prim IDs
};
std::vector<GridRow> _gridRows;  // NEW

// Scroll = shift rows
void scroll(int numLines) {
    if (numLines > 0) {
        // Drop top rows, add empty at bottom
        _gridRows.erase(_gridRows.begin(), _gridRows.begin() + numLines);
        _gridRows.resize(_gridHeight);
    }
    rebuildFlatGrid();  // For GPU upload
}
```

## Migration Path

1. **Phase 1**: Add `YDrawAABB` struct and `addYDrawBuffer()` method
2. **Phase 2**: Implement primitive inline/absolute tracking
3. **Phase 3**: Implement incremental grid update
4. **Phase 4**: Modify scroll to only affect inline primitives
5. **Phase 5**: Add cursor synchronization
6. **Phase 6**: Update ScreenDrawLayer to use new API
7. **Phase 7**: Remove old full-rebuild code paths

## Testing

1. **Inline scroll test**: Add inline content, verify it scrolls with terminal
2. **Absolute fixed test**: Add absolute content, verify it stays fixed during scroll
3. **Mixed content test**: Add both inline and absolute, verify correct behavior
4. **AABB test**: Verify returned AABB matches actual content bounds
5. **Incremental update test**: Add content progressively, verify no full rebuild
6. **Primitive cleanup test**: Verify inline primitives removed when scrolled out

## Open Questions

1. Should we support converting inline to absolute (pin content)?
2. Maximum inline primitive count before cleanup?
3. Should absolute content have z-ordering relative to inline?
4. Memory management for long-running sessions with lots of inline content?
