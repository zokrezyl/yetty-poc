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

// Scroll N lines in scrolling mode - O(lines deleted), not O(all primitives)
void scrollLines(uint16_t numLines) override {
  if (numLines == 0 || _scrollLines.empty())
    return;
  ydebug("scrollLines: removing {} lines, had {} lines, cursor was ({},{})",
         numLines, _scrollLines.size(), _cursorCol, _cursorRow);

  // Pop lines from front - primitives and grid cells are deleted together
  for (uint16_t i = 0; i < numLines && !_scrollLines.empty(); i++) {
    _scrollLines.pop_front();
  }

  // DO NOT modify refs in remaining lines!
  // linesAhead is a RELATIVE offset (baseLine - lineIdx) that remains correct
  // after scrolling because both the ref's line and the target line shift
  // by the same amount. All refs in remaining lines point to other remaining
  // lines (by construction: refs only point forward or to themselves).

  // Update grid offset row in ALL remaining primitives' data
  // Primitive data format: [0]=packed(gridOffsetCol|gridOffsetRow<<16),
  // [1]=type, [2]=layer, [3..]=geometry Row is stored as SIGNED int16_t to
  // allow negative values (scrolled partially off-screen) After scrolling N
  // lines, each primitive's gridOffsetRow needs to decrease by N
  for (auto &line : _scrollLines) {
    for (auto &prim : line.prims) {
      if (prim.size() >= 1) {
        uint32_t packed;
        std::memcpy(&packed, &prim[0], sizeof(uint32_t));
        uint16_t col = packed & 0xFFFF;
        int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
        row -= static_cast<int16_t>(numLines); // Can go negative!
        packed = static_cast<uint32_t>(col) |
                 (static_cast<uint32_t>(static_cast<uint16_t>(row)) << 16);
        std::memcpy(&prim[0], &packed, sizeof(uint32_t));
      }
    }
  }

  // Adjust cursor position
  if (_cursorRow >= numLines) {
    _cursorRow -= numLines;
  } else {
    _cursorRow = 0;
  }

  _bufferDirty = true;
  _gridStagingDirty = true;
}

void rebuildPackedGrid() {
  if (!_gridStagingDirty)
    return;

  // Build packed grid from line-based storage (_scrollLines)
  // First, build a mapping from (lineIdx, primIdx) to global prim index
  // by iterating all lines and counting primitives
  std::vector<uint32_t> lineBasePrimIdx; // lineBasePrimIdx[line] = first prim
                                         // index for that line
  uint32_t totalPrims = 0;
  for (const auto &line : _scrollLines) {
    lineBasePrimIdx.push_back(totalPrims);
    totalPrims += static_cast<uint32_t>(line.prims.size());
  }

  // Compute grid dimensions from scroll lines
  uint32_t gridH = static_cast<uint32_t>(_scrollLines.size());
  uint32_t gridW = 0;
  for (const auto &line : _scrollLines) {
    gridW = std::max(gridW, static_cast<uint32_t>(line.cells.size()));
  }

  // Also consider grid dimensions needed for glyphs
  if (!_glyphs.empty() && _cellSizeX > 0 && _cellSizeY > 0) {
    for (const auto &g : _glyphs) {
      uint32_t maxCellX =
          static_cast<uint32_t>(std::max(
              0.0f,
              std::floor((g.x + g.width() - _sceneMinX) / _cellSizeX))) +
          1;
      uint32_t maxCellY =
          static_cast<uint32_t>(std::max(
              0.0f,
              std::floor((g.y + g.height() - _sceneMinY) / _cellSizeY))) +
          1;
      gridW = std::max(gridW, maxCellX);
      gridH = std::max(gridH, maxCellY);
    }
  }

  if (gridW == 0 || gridH == 0) {
    _gridStaging.clear();
    _gridStagingDirty = false;
    return;
  }

  _gridWidth = gridW;
  _gridHeight = gridH;
  uint32_t numCells = gridW * gridH;

  // Build glyph->cell mapping: for each cell, list of glyph indices
  std::vector<std::vector<uint32_t>> cellGlyphs(numCells);
  if (!_glyphs.empty() && _cellSizeX > 0 && _cellSizeY > 0) {
    for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
      const auto &g = _glyphs[gi];
      float gMinX = g.x;
      float gMinY = g.y;
      float gMaxX = g.x + g.width();
      float gMaxY = g.y + g.height();

      int32_t cellMinX =
          static_cast<int32_t>(std::floor((gMinX - _sceneMinX) / _cellSizeX));
      int32_t cellMinY =
          static_cast<int32_t>(std::floor((gMinY - _sceneMinY) / _cellSizeY));
      int32_t cellMaxX =
          static_cast<int32_t>(std::floor((gMaxX - _sceneMinX) / _cellSizeX));
      int32_t cellMaxY =
          static_cast<int32_t>(std::floor((gMaxY - _sceneMinY) / _cellSizeY));

      cellMinX = std::max(0, cellMinX);
      cellMinY = std::max(0, cellMinY);
      cellMaxX = std::min(static_cast<int32_t>(gridW) - 1, cellMaxX);
      cellMaxY = std::min(static_cast<int32_t>(gridH) - 1, cellMaxY);

      for (int32_t cy = cellMinY; cy <= cellMaxY; cy++) {
        for (int32_t cx = cellMinX; cx <= cellMaxX; cx++) {
          uint32_t cellIdx =
              static_cast<uint32_t>(cy) * gridW + static_cast<uint32_t>(cx);
          cellGlyphs[cellIdx].push_back(gi);
        }
      }
    }
  }

  // Pass 1: compute total entries needed (prims + glyphs per cell)
  uint32_t totalEntries = 0;
  for (uint32_t y = 0; y < gridH; y++) {
    for (uint32_t x = 0; x < gridW; x++) {
      uint32_t cellIdx = y * gridW + x;
      uint32_t primCount = 0;
      if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
        primCount =
            static_cast<uint32_t>(_scrollLines[y].cells[x].refs.size());
      }
      uint32_t glyphCount = static_cast<uint32_t>(cellGlyphs[cellIdx].size());
      totalEntries += 1 + primCount + glyphCount;
    }
  }

  _gridStaging.resize(numCells + totalEntries);

  // Pass 2: fill offsets and entries
  uint32_t pos = numCells;
  for (uint32_t y = 0; y < gridH; y++) {
    for (uint32_t x = 0; x < gridW; x++) {
      uint32_t cellIdx = y * gridW + x;
      _gridStaging[cellIdx] = pos;

      uint32_t primCount = 0;
      if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
        primCount =
            static_cast<uint32_t>(_scrollLines[y].cells[x].refs.size());
      }
      uint32_t glyphCount = static_cast<uint32_t>(cellGlyphs[cellIdx].size());
      uint32_t totalCount = primCount + glyphCount;

      _gridStaging[pos] = totalCount;
      uint32_t entryIdx = 0;

      // Add prim entries
      if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
        const auto &cell = _scrollLines[y].cells[x];
        for (size_t j = 0; j < cell.refs.size(); j++) {
          const auto &ref = cell.refs[j];
          uint32_t baseLine = y + ref.linesAhead;
          uint32_t globalPrimIdx = lineBasePrimIdx[baseLine] + ref.primIndex;
          _gridStaging[pos + 1 + entryIdx] = globalPrimIdx;
          entryIdx++;
        }
      }

      // Add glyph entries with GLYPH_BIT (0x80000000) set
      for (uint32_t gi : cellGlyphs[cellIdx]) {
        _gridStaging[pos + 1 + entryIdx] = gi | 0x80000000u;
        entryIdx++;
      }

      pos += 1 + totalCount;
    }
  }

  _gridStagingDirty = false;

  ydebug("rebuildPackedGrid: grid={}x{} prims={} glyphs={} staging={} u32s",
         gridW, gridH, totalPrims, _glyphs.size(), _gridStaging.size());
}
