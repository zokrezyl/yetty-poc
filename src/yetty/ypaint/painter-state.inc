//=========================================================================
// State management
//=========================================================================

uint32_t primitiveCount() const override {
  return _canvas ? _canvas->primitiveCount() : 0;
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
  if (_canvas) {
    _canvas->setSceneBounds(minX, minY, maxX, maxY);
  }
}
bool hasExplicitBounds() const override { return _hasExplicitBounds; }

void setGridCellSize(float sizeX, float sizeY) override {
  _cellSizeX = sizeX;
  _cellSizeY = sizeY;
  if (_canvas) {
    _canvas->setCellSize(sizeX, sizeY);
  }
}
float gridCellSizeX() const override { return _cellSizeX; }
float gridCellSizeY() const override { return _cellSizeY; }

void setPadding(float padX, float padY) override {
  _paddingX = padX;
  _paddingY = padY;
}
float paddingX() const override { return _paddingX; }
float paddingY() const override { return _paddingY; }

void setFlags(uint32_t flags) override { _flags = flags; }
void addFlags(uint32_t flags) override { _flags |= flags; }
uint32_t flags() const override { return _flags; }

void setMaxPrimsPerCell(uint32_t max) override {
  _maxPrimsPerCell = max;
  if (_canvas) {
    _canvas->setMaxPrimsPerCell(max);
  }
}
uint32_t maxPrimsPerCell() const override { return _maxPrimsPerCell; }

//=========================================================================
// Staging data access
//=========================================================================

const std::vector<uint32_t> &gridStaging() const override {
  if (_canvas && _canvas->isDirty()) {
    const_cast<PainterImpl *>(this)->rebuildPackedGrid();
  }
  return _canvas ? _canvas->gridStaging() : _emptyGridStaging;
}
void clearGridStaging() override {
  if (_canvas) {
    _canvas->clearStaging();
  }
}
const std::vector<YPaintGlyph> &glyphs() const override { return _glyphs; }
std::vector<YPaintGlyph> &glyphsMut() override { return _glyphs; }

void buildPrimStaging(std::vector<uint32_t> &out) const override {
  // Use cached version if buffer not dirty
  if (!_bufferDirty && !_primStagingCache.empty()) {
    out = _primStagingCache;
    return;
  }

  if (_canvas) {
    _canvas->buildPrimStaging(out);
    const_cast<PainterImpl *>(this)->_primStagingCache = out;
  } else {
    out.clear();
    const_cast<PainterImpl *>(this)->_primStagingCache.clear();
  }
}

bool hasContent() const override {
  if (_canvas && !_canvas->empty()) {
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
  if (_canvas) {
    _canvas->setCursorPosition(col, row);
  }
}

uint16_t cursorCol() const override { return _cursorCol; }
uint16_t cursorRow() const override { return _cursorRow; }

uint32_t sceneHeightInLines() const override {
  return _canvas ? _canvas->heightInLines() : 0;
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

// GPU buffer offsets (in float words)
uint32_t gpuPrimitiveOffset() const override {
  return _primHandle.isValid() ? _primHandle.offset / sizeof(float) : 0;
}
uint32_t gpuGridOffset() const override { return _gpuGridOffset; }
uint32_t gpuGlyphOffset() const override { return _gpuGlyphOffset; }
