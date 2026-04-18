#include <yetty/ypaint/canvas.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <deque>
#include <ytrace/ytrace.hpp>

namespace yetty::ypaint {

//=============================================================================
// Internal structures
//=============================================================================

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

//=============================================================================
// CanvasImpl
//=============================================================================

class CanvasImpl : public Canvas {
public:
  explicit CanvasImpl(bool scrollingMode) : _scrollingMode(scrollingMode) {}

  ~CanvasImpl() override = default;

  //===========================================================================
  // Mode
  //===========================================================================

  bool scrollingMode() const override { return _scrollingMode; }

  //===========================================================================
  // Configuration
  //===========================================================================

  void setSceneBounds(float minX, float minY, float maxX, float maxY) override {
    _sceneMinX = minX;
    _sceneMinY = minY;
    _sceneMaxX = maxX;
    _sceneMaxY = maxY;
    updateGridDimensions();
    _dirty = true;
  }

  void setCellSize(float sizeX, float sizeY) override {
    _cellSizeX = sizeX;
    _cellSizeY = sizeY;
    updateGridDimensions();
    _dirty = true;
  }

  void setMaxPrimsPerCell(uint32_t max) override { _maxPrimsPerCell = max; }

  //===========================================================================
  // Scene bounds accessors
  //===========================================================================

  float sceneMinX() const override { return _sceneMinX; }
  float sceneMinY() const override { return _sceneMinY; }
  float sceneMaxX() const override { return _sceneMaxX; }
  float sceneMaxY() const override { return _sceneMaxY; }

  //===========================================================================
  // Grid dimension accessors
  //===========================================================================

  float cellSizeX() const override { return _cellSizeX; }
  float cellSizeY() const override { return _cellSizeY; }
  uint32_t gridWidth() const override { return _gridWidth; }
  uint32_t gridHeight() const override { return _gridHeight; }
  uint32_t maxPrimsPerCell() const override { return _maxPrimsPerCell; }

  size_t lineCount() const override { return _lines.size(); }

  uint32_t heightInLines() const override {
    if (_cellSizeY <= 0.0f)
      return 0;
    float sceneH = _sceneMaxY - _sceneMinY;
    return static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY));
  }

  //===========================================================================
  // Cursor (scrolling mode)
  //===========================================================================

  void setCursorPosition(uint16_t col, uint16_t row) override {
    _cursorCol = col;
    _cursorRow = row;
  }

  uint16_t cursorCol() const override { return _cursorCol; }
  uint16_t cursorRow() const override { return _cursorRow; }

  //===========================================================================
  // Primitive management
  //===========================================================================

  void addPrimitive(std::vector<float> primData, float aabbMinX, float aabbMinY,
                    float aabbMaxX, float aabbMaxY) override {
    float baseY = (_sceneMinY > 1e9f) ? 0.0f : _sceneMinY;

    // Local row range within primitive's AABB (not offset by cursor)
    uint16_t localMinRow = static_cast<uint16_t>(
        std::max(0, static_cast<int32_t>(std::floor((aabbMinY - baseY) / _cellSizeY))));
    uint16_t localMaxRow = static_cast<uint16_t>(
        std::max(0, static_cast<int32_t>(std::floor((aabbMaxY - baseY) / _cellSizeY))));

    // In scrolling mode, grid rows are offset by cursor position
    uint16_t primMinRow = _scrollingMode ? (_cursorRow + localMinRow) : localMinRow;
    uint16_t primMaxRow = _scrollingMode ? (_cursorRow + localMaxRow) : localMaxRow;

    ensureLines(primMaxRow + 1);
    ydebug("addPrimitive: cursor=({},{}) localRows=[{},{}] primRows=[{},{}] _lines.size={}",
           _cursorCol, _cursorRow, localMinRow, localMaxRow, primMinRow, primMaxRow, _lines.size());

    // Pack gridOffset: (cursorCol, cursorRow) - shader coordinate offset
    // This is where drawing STARTS, not where primitive is STORED
    std::vector<float> fullPrimData(primData.size() + 1);
    uint16_t gridOffsetCol = _scrollingMode ? _cursorCol : 0;
    uint16_t gridOffsetRow = _scrollingMode ? _cursorRow : 0;
    uint32_t packedOffset = static_cast<uint32_t>(gridOffsetCol) |
                            (static_cast<uint32_t>(gridOffsetRow) << 16);
    std::memcpy(&fullPrimData[0], &packedOffset, sizeof(uint32_t));
    for (size_t i = 0; i < primData.size(); i++) {
      fullPrimData[i + 1] = primData[i];
    }

    // Store primitive at primMaxRow (bottom of AABB - for scroll deletion)
    uint16_t primIndex = static_cast<uint16_t>(_lines[primMaxRow].prims.size());
    _lines[primMaxRow].prims.push_back(std::move(fullPrimData));

    // Add grid cell references
    uint16_t cellMinX = static_cast<uint16_t>(cellXFromWorld(aabbMinX));
    uint16_t cellMaxX = static_cast<uint16_t>(cellXFromWorld(aabbMaxX));

    for (uint16_t row = primMinRow; row <= primMaxRow; row++) {
      while (_lines[row].cells.size() <= cellMaxX) {
        _lines[row].cells.push_back(GridCell{});
      }
      uint16_t linesAhead = primMaxRow - row;
      for (uint16_t cx = cellMinX; cx <= cellMaxX; cx++) {
        _lines[row].cells[cx].refs.push_back({linesAhead, primIndex});
      }
    }

    _dirty = true;
  }

  //===========================================================================
  // Scrolling support
  //===========================================================================

  void scrollLines(uint16_t numLines) override {
    if (numLines == 0 || _lines.empty())
      return;

    // Pop lines from front - primitives in those lines are deleted
    for (uint16_t i = 0; i < numLines && !_lines.empty(); i++) {
      _lines.pop_front();
    }

    // Update gridOffset row in remaining primitives (decrement by numLines)
    for (auto &line : _lines) {
      for (auto &prim : line.prims) {
        if (prim.size() >= 1) {
          uint32_t packed;
          std::memcpy(&packed, &prim[0], sizeof(uint32_t));
          uint16_t col = packed & 0xFFFF;
          uint16_t row = (packed >> 16) & 0xFFFF;
          row -= numLines;
          packed = static_cast<uint32_t>(col) | (static_cast<uint32_t>(row) << 16);
          std::memcpy(&prim[0], &packed, sizeof(uint32_t));
        }
      }
    }

    // Update cursor
    if (_cursorRow >= numLines) {
      _cursorRow -= numLines;
    } else {
      _cursorRow = 0;
    }

    _dirty = true;
    ydebug("scrollLines: popped {} lines, _lines.size={} cursorRow={}",
           numLines, _lines.size(), _cursorRow);
  }

  //===========================================================================
  // Packed GPU format
  //===========================================================================

  void markDirty() override { _dirty = true; }
  bool isDirty() const override { return _dirty; }

  void rebuildPackedGrid() override {
    rebuildPackedGrid(0, [](uint32_t, float &, float &, float &, float &) {});
  }

  void rebuildPackedGrid(
      uint32_t glyphCount,
      std::function<void(uint32_t, float &, float &, float &, float &)>
          glyphBoundsFunc) override {
    if (!_dirty && !_gridStaging.empty())
      return;

    // Build line base prim index mapping (reuse member buffer)
    _lineBasePrimIdx.clear();
    uint32_t totalPrims = 0;
    for (const auto &line : _lines) {
      _lineBasePrimIdx.push_back(totalPrims);
      totalPrims += static_cast<uint32_t>(line.prims.size());
    }

    // Compute grid dimensions from scene bounds as baseline
    uint32_t gridH = heightInLines();
    uint32_t gridW = 0;
    if (_cellSizeX > 0.0f && _sceneMaxX > _sceneMinX) {
      gridW = static_cast<uint32_t>(
          std::ceil((_sceneMaxX - _sceneMinX) / _cellSizeX));
    }
    // Extend for actual lines if they go beyond scene bounds
    uint32_t linesCount = static_cast<uint32_t>(_lines.size());
    ydebug("rebuildPackedGrid: heightInLines={} _lines.size={} scene=[{},{}]-[{},{}] cellSizeY={}",
           gridH, linesCount, _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY, _cellSizeY);
    gridH = std::max(gridH, linesCount);
    for (const auto &line : _lines) {
      gridW = std::max(gridW, static_cast<uint32_t>(line.cells.size()));
    }

    // Extend grid for glyphs
    if (glyphCount > 0 && _cellSizeX > 0 && _cellSizeY > 0) {
      for (uint32_t gi = 0; gi < glyphCount; gi++) {
        float gMinX, gMinY, gMaxX, gMaxY;
        glyphBoundsFunc(gi, gMinX, gMinY, gMaxX, gMaxY);
        uint32_t maxCellX =
            static_cast<uint32_t>(std::max(
                0.0f, std::floor((gMaxX - _sceneMinX) / _cellSizeX))) +
            1;
        uint32_t maxCellY =
            static_cast<uint32_t>(std::max(
                0.0f, std::floor((gMaxY - _sceneMinY) / _cellSizeY))) +
            1;
        gridW = std::max(gridW, maxCellX);
        gridH = std::max(gridH, maxCellY);
      }
    }

    if (gridW == 0 || gridH == 0) {
      _gridStaging.clear();
      _dirty = false;
      _gridWidth = gridW;
      _gridHeight = gridH;
      return;
    }

    _gridWidth = gridW;
    _gridHeight = gridH;
    uint32_t numCells = gridW * gridH;

    // Build glyph->cell mapping (reuse member buffer)
    _cellGlyphs.resize(numCells);
    for (auto &cg : _cellGlyphs) {
      cg.clear();
    }
    if (glyphCount > 0 && _cellSizeX > 0 && _cellSizeY > 0) {
      for (uint32_t gi = 0; gi < glyphCount; gi++) {
        float gMinX, gMinY, gMaxX, gMaxY;
        glyphBoundsFunc(gi, gMinX, gMinY, gMaxX, gMaxY);

        int32_t cMinX =
            static_cast<int32_t>(std::floor((gMinX - _sceneMinX) / _cellSizeX));
        int32_t cMinY =
            static_cast<int32_t>(std::floor((gMinY - _sceneMinY) / _cellSizeY));
        int32_t cMaxX =
            static_cast<int32_t>(std::floor((gMaxX - _sceneMinX) / _cellSizeX));
        int32_t cMaxY =
            static_cast<int32_t>(std::floor((gMaxY - _sceneMinY) / _cellSizeY));

        cMinX = std::max(0, cMinX);
        cMinY = std::max(0, cMinY);
        cMaxX = std::min(static_cast<int32_t>(gridW) - 1, cMaxX);
        cMaxY = std::min(static_cast<int32_t>(gridH) - 1, cMaxY);

        for (int32_t cy = cMinY; cy <= cMaxY; cy++) {
          for (int32_t cx = cMinX; cx <= cMaxX; cx++) {
            uint32_t cellIdx =
                static_cast<uint32_t>(cy) * gridW + static_cast<uint32_t>(cx);
            _cellGlyphs[cellIdx].push_back(gi);
          }
        }
      }
    }

    // Single pass: offset table then appended entries
    _gridStaging.clear();
    _gridStaging.resize(numCells);

    for (uint32_t y = 0; y < gridH; y++) {
      bool hasLine = y < _lines.size();
      uint32_t lineCellCount = hasLine ? static_cast<uint32_t>(_lines[y].cells.size()) : 0;

      for (uint32_t x = 0; x < gridW; x++) {
        uint32_t cellIdx = y * gridW + x;
        _gridStaging[cellIdx] = static_cast<uint32_t>(_gridStaging.size());

        uint32_t countPos = static_cast<uint32_t>(_gridStaging.size());
        _gridStaging.push_back(0);
        uint32_t count = 0;

        // Add prim entries
        if (hasLine && x < lineCellCount) {
          const auto &cell = _lines[y].cells[x];
          for (const auto &ref : cell.refs) {
            uint32_t bl = y + ref.linesAhead;
            if (bl < _lineBasePrimIdx.size()) {
              _gridStaging.push_back(_lineBasePrimIdx[bl] + ref.primIndex);
              count++;
            }
          }
        }

        // Add glyph entries with GLYPH_BIT
        for (uint32_t gi : _cellGlyphs[cellIdx]) {
          _gridStaging.push_back(gi | GLYPH_BIT);
          count++;
        }

        _gridStaging[countPos] = count;
      }
    }

    _dirty = false;
  }

  const std::vector<uint32_t> &gridStaging() const override {
    return _gridStaging;
  }

  void clearStaging() override { _gridStaging.clear(); }

  //===========================================================================
  // Primitive staging for GPU
  //===========================================================================

  void buildPrimStaging(std::vector<uint32_t> &out) const override {
    uint32_t primCount = 0;
    uint32_t totalWords = 0;
    for (const auto &line : _lines) {
      for (const auto &prim : line.prims) {
        primCount++;
        totalWords += static_cast<uint32_t>(prim.size());
      }
    }

    if (primCount == 0) {
      out.clear();
      return;
    }

    out.resize(primCount + totalWords);
    uint32_t dataOffset = 0;
    uint32_t primIdx = 0;
    for (const auto &line : _lines) {
      for (const auto &prim : line.prims) {
        out[primIdx] = dataOffset;
        for (size_t i = 0; i < prim.size(); i++) {
          uint32_t val;
          std::memcpy(&val, &prim[i], sizeof(uint32_t));
          out[primCount + dataOffset + i] = val;
        }
        dataOffset += static_cast<uint32_t>(prim.size());
        primIdx++;
      }
    }
  }

  uint32_t primGpuSize() const override {
    uint32_t totalWords = 0;
    for (const auto &line : _lines) {
      for (const auto &prim : line.prims) {
        totalWords += 1 + static_cast<uint32_t>(prim.size());
      }
    }
    return totalWords * sizeof(float);
  }

  //===========================================================================
  // State management
  //===========================================================================

  void clear() override {
    _lines.clear();
    _gridStaging.clear();
    _cursorCol = 0;
    _cursorRow = 0;
    _dirty = true;
  }

  bool empty() const override {
    for (const auto &line : _lines) {
      if (!line.prims.empty())
        return false;
    }
    return true;
  }

  uint32_t primitiveCount() const override {
    uint32_t count = 0;
    for (const auto &line : _lines) {
      count += static_cast<uint32_t>(line.prims.size());
    }
    return count;
  }

  //===========================================================================
  // Debug
  //===========================================================================

  void dumpGrid() const override {
    ydebug("=== Canvas Grid Dump ===");
    ydebug("  scrollingMode={}, cursor=({}, {})", _scrollingMode, _cursorCol,
           _cursorRow);
    ydebug("  sceneBounds=({}, {}) -> ({}, {})", _sceneMinX, _sceneMinY,
           _sceneMaxX, _sceneMaxY);
    ydebug("  cellSize=({}, {}), gridDim={}x{}", _cellSizeX, _cellSizeY,
           _gridWidth, _gridHeight);
    ydebug("  lineCount={}, dirty={}", _lines.size(), _dirty);

    // Outer loop: rows
    for (size_t rowIdx = 0; rowIdx < _lines.size(); rowIdx++) {
      const auto &line = _lines[rowIdx];

      // Show primitives stored in this row
      ydebug("  Row {}: {} prims stored, {} cells", rowIdx, line.prims.size(),
             line.cells.size());

      for (size_t primIdx = 0; primIdx < line.prims.size(); primIdx++) {
        const auto &prim = line.prims[primIdx];
        if (prim.size() >= 2) {
          uint32_t gridOffset;
          std::memcpy(&gridOffset, &prim[0], sizeof(uint32_t));
          uint16_t col = gridOffset & 0xFFFF;
          int16_t row = static_cast<int16_t>((gridOffset >> 16) & 0xFFFF);
          uint32_t primType;
          std::memcpy(&primType, &prim[1], sizeof(uint32_t));
          ydebug("    prim[{}]: gridOffset=({}, {}), type={}, words={}",
                 primIdx, col, row, primType, prim.size());
        }
      }

      // Inner loop: cells with references
      for (size_t cellIdx = 0; cellIdx < line.cells.size(); cellIdx++) {
        const auto &cell = line.cells[cellIdx];
        if (cell.refs.empty())
          continue;

        ydebug("    cell[{}]: {} refs", cellIdx, cell.refs.size());

        // Third loop: references
        for (size_t refIdx = 0; refIdx < cell.refs.size(); refIdx++) {
          const auto &ref = cell.refs[refIdx];
          uint32_t baseLine = static_cast<uint32_t>(rowIdx) + ref.linesAhead;
          ydebug("      ref[{}]: linesAhead={}, primIndex={} -> baseLine={}",
                 refIdx, ref.linesAhead, ref.primIndex, baseLine);
        }
      }
    }

    ydebug("=== End Canvas Grid Dump ===");
  }

private:
  void updateGridDimensions() {
    if (_cellSizeX <= 0.0f || _cellSizeY <= 0.0f) {
      _gridWidth = 0;
      _gridHeight = 0;
      return;
    }

    float sceneW = _sceneMaxX - _sceneMinX;
    float sceneH = _sceneMaxY - _sceneMinY;

    if (sceneW <= 0.0f || sceneH <= 0.0f) {
      _gridWidth = 0;
      _gridHeight = 0;
      return;
    }

    _gridWidth =
        std::max(1u, static_cast<uint32_t>(std::ceil(sceneW / _cellSizeX)));
    _gridHeight =
        std::max(1u, static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY)));
  }

  void ensureLines(uint32_t numLines) {
    while (_lines.size() < numLines) {
      GridLine line;
      if (_gridWidth > 0) {
        line.cells.resize(_gridWidth);
      }
      _lines.push_back(std::move(line));
    }
  }

  uint32_t cellXFromWorld(float worldX) const {
    if (_gridWidth == 0 || _cellSizeX <= 0.0f)
      return 0;
    float normalized = (worldX - _sceneMinX) / _cellSizeX;
    return static_cast<uint32_t>(
        std::clamp(normalized, 0.0f, static_cast<float>(_gridWidth - 1)));
  }

  // Mode
  bool _scrollingMode = false;

  // Lines (front = top)
  std::deque<GridLine> _lines;

  // Scene bounds
  float _sceneMinX = 0.0f;
  float _sceneMinY = 0.0f;
  float _sceneMaxX = 0.0f;
  float _sceneMaxY = 0.0f;

  // Grid cell size
  float _cellSizeX = 0.0f;
  float _cellSizeY = 0.0f;

  // Grid dimensions
  uint32_t _gridWidth = 0;
  uint32_t _gridHeight = 0;
  uint32_t _maxPrimsPerCell = DEFAULT_MAX_PRIMS_PER_CELL;

  // Cursor (scrolling mode)
  uint16_t _cursorCol = 0;
  uint16_t _cursorRow = 0;

  // Packed grid for GPU
  std::vector<uint32_t> _gridStaging;
  bool _dirty = true;

  // Reusable scratch buffers (avoid per-rebuild allocation)
  std::vector<uint32_t> _lineBasePrimIdx;
  std::vector<std::vector<uint32_t>> _cellGlyphs;
};

//=============================================================================
// Factory
//=============================================================================

Result<Canvas::Ptr> Canvas::create(bool scrollingMode) {
  return Ok<Ptr>(std::make_shared<CanvasImpl>(scrollingMode));
}

} // namespace yetty::ypaint
