//=========================================================================
// State management
//=========================================================================

uint32_t primitiveCount() const override {
  // Unified: count primitives from _scrollLines (used for both modes)
  uint32_t count = 0;
  for (const auto &line : _scrollLines) {
    count += static_cast<uint32_t>(line.prims.size());
  }
  return count;
}

uint32_t glyphCount() const override {
  return static_cast<uint32_t>(_glyphs.size());
}

void setBgColor(uint32_t color) override { _bgColor = color; }
uint32_t bgColor() const override { return _bgColor; }

void setSceneBounds(float minX, float minY, float maxX, float maxY) override {
  _sceneMinX = minX;
  _sceneMinY = minY;
  _sceneMaxX = maxX;
  _sceneMaxY = maxY;
  _hasExplicitBounds = true;
}
bool hasExplicitBounds() const override { return _hasExplicitBounds; }

void setGridCellSize(float sizeX, float sizeY) override {
  _cellSizeX = sizeX;
  _cellSizeY = sizeY;
}
float gridCellSizeX() const override { return _cellSizeX; }
float gridCellSizeY() const override { return _cellSizeY; }

void setFlags(uint32_t flags) override { _flags = flags; }
void addFlags(uint32_t flags) override { _flags |= flags; }
uint32_t flags() const override { return _flags; }

void setMaxPrimsPerCell(uint32_t max) override { _maxPrimsPerCell = max; }
uint32_t maxPrimsPerCell() const override { return _maxPrimsPerCell; }

//=========================================================================
// Staging data access
//=========================================================================

const std::vector<uint32_t> &gridStaging() const override {
  if (_gridStagingDirty) {
    const_cast<PainterImpl *>(this)->rebuildPackedGrid();
  }
  return _gridStaging;
}
void clearGridStaging() override { _gridStaging.clear(); }
const std::vector<YPaintGlyph> &glyphs() const override { return _glyphs; }
std::vector<YPaintGlyph> &glyphsMut() override { return _glyphs; }

void buildPrimStaging(std::vector<uint32_t> &out) const override {
  // Use cached version if buffer not dirty
  if (!_bufferDirty && !_primStagingCache.empty()) {
    out = _primStagingCache;
    return;
  }

  // Primitives are stored in _scrollLines (line-based storage)
  // Count total primitives and words
  uint32_t primCount = 0;
  uint32_t totalWords = 0;
  for (const auto &line : _scrollLines) {
    for (const auto &prim : line.prims) {
      primCount++;
      totalWords += static_cast<uint32_t>(prim.size());
    }
  }

  if (primCount == 0) {
    out.clear();
    const_cast<PainterImpl *>(this)->_primStagingCache.clear();
    return;
  }

  out.resize(primCount + totalWords);
  uint32_t dataOffset = 0;
  uint32_t primIdx = 0;
  for (const auto &line : _scrollLines) {
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

  // Cache the result
  const_cast<PainterImpl *>(this)->_primStagingCache = out;
}

bool hasContent() const override {
  for (const auto &line : _scrollLines) {
    if (!line.prims.empty())
      return true;
  }
  return !_glyphs.empty();
}

//=========================================================================
// Scrolling feature
//=========================================================================

bool scrollingMode() const override { return _scrollingMode; }

void setCursorPosition(uint16_t col, uint16_t row) override {
  _cursorCol = col;
  _cursorRow = row;
}

uint16_t cursorCol() const override { return _cursorCol; }
uint16_t cursorRow() const override { return _cursorRow; }

uint32_t sceneHeightInLines() const override {
  if (_cellSizeY <= 0.0f)
    return 0;
  float sceneH = _sceneMaxY - _sceneMinY;
  return static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY));
}

//=========================================================================
// Grid computation
//=========================================================================

float sceneMinX() const override { return _sceneMinX; }
float sceneMinY() const override { return _sceneMinY; }
float sceneMaxX() const override { return _sceneMaxX; }
float sceneMaxY() const override { return _sceneMaxY; }
float cellSizeX() const override { return _cellSizeX; }
float cellSizeY() const override { return _cellSizeY; }
uint32_t gridWidth() const override { return _gridWidth; }
uint32_t gridHeight() const override { return _gridHeight; }
