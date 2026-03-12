//=========================================================================
// Grid helper methods
//=========================================================================

uint32_t cellX(float worldX) const {
  if (_gridWidth == 0 || _cellSizeX <= 0)
    return 0;
  return static_cast<uint32_t>(
      std::clamp((worldX - _sceneMinX) / _cellSizeX, 0.0f,
                 static_cast<float>(_gridWidth - 1)));
}

uint32_t cellY(float worldY) const {
  if (_gridHeight == 0 || _cellSizeY <= 0)
    return 0;
  return static_cast<uint32_t>(
      std::clamp((worldY - _sceneMinY) / _cellSizeY, 0.0f,
                 static_cast<float>(_gridHeight - 1)));
}

// Scroll N lines in scrolling mode - delegated to canvas
void scrollLines(uint16_t numLines) override {
  if (!_canvas || numLines == 0)
    return;

  ydebug("scrollLines: removing {} lines, cursor was ({},{})",
         numLines, _cursorCol, _cursorRow);

  _canvas->scrollLines(numLines);

  // Update local cursor position to match canvas
  _cursorCol = _canvas->cursorCol();
  _cursorRow = _canvas->cursorRow();

  _bufferDirty = true;
}

void rebuildPackedGrid() {
  if (!_canvas)
    return;

  // Use canvas to rebuild with glyph bounds callback
  uint32_t glyphCount = static_cast<uint32_t>(_glyphs.size());
  _canvas->rebuildPackedGrid(glyphCount,
      [this](uint32_t gi, float &minX, float &minY, float &maxX, float &maxY) {
        if (gi < _glyphs.size()) {
          const auto &g = _glyphs[gi];
          minX = g.x;
          minY = g.y;
          maxX = g.x + g.width();
          maxY = g.y + g.height();
        }
      });

  // Update local grid dimensions from canvas
  _gridWidth = _canvas->gridWidth();
  _gridHeight = _canvas->gridHeight();

  ydebug("rebuildPackedGrid: grid={}x{} prims={} glyphs={} staging={} u32s",
         _gridWidth, _gridHeight, _canvas->primitiveCount(), _glyphs.size(),
         _canvas->gridStaging().size());
}
