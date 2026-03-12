//=========================================================================
// Text API
//=========================================================================

Result<void> addText(float x, float y, const std::string &text,
                     float fontSize, uint32_t color, uint32_t layer) {
  return addText(x, y, text, fontSize, color, layer, -1);
}

Result<void> addText(float x, float y, const std::string &text,
                     float fontSize, uint32_t color, uint32_t layer,
                     int fontId) {
  int resolvedFontId = fontId;
  if (fontId >= 0) {
    auto it = _userFontIdMap.find(fontId);
    if (it != _userFontIdMap.end()) {
      resolvedFontId = it->second;
    }
  }
  bool useCustom = (resolvedFontId >= 0 && _customAtlas);
  // Custom fonts are NOT fully supported in scrolling mode - the scrolling
  // shader only samples from the shared fontTexture, not the custom atlas.
  // Fall back to shared atlas with a warning.
  if (useCustom && _scrollingMode) {
    ywarn("addText: custom fonts not supported in scrolling mode, using "
          "default font");
    useCustom = false;
  }
  MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
  if (!activeAtlas) {
    return Err<void>("addText: no atlas available");
  }
  if (text.empty()) {
    return Ok();
  }

  float fontBaseSize = activeAtlas->getFontSize();
  float scale = fontSize / fontBaseSize;

  float cursorX = x;
  uint32_t glyphsAdded = 0;

  const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
  const uint8_t *end = ptr + text.size();

  while (ptr < end) {
    uint32_t codepoint = 0;
    if ((*ptr & 0x80) == 0) {
      codepoint = *ptr++;
    } else if ((*ptr & 0xE0) == 0xC0) {
      codepoint = (*ptr++ & 0x1F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF0) == 0xE0) {
      codepoint = (*ptr++ & 0x0F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF8) == 0xF0) {
      codepoint = (*ptr++ & 0x07) << 18;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else {
      ptr++;
      continue;
    }

    uint32_t glyphIndex;
    if (useCustom) {
      glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
    } else if (_font) {
      glyphIndex = _font->getGlyphIndex(codepoint);
    } else {
      continue;
    }

    const auto &metadata = activeAtlas->getGlyphMetadata();
    if (glyphIndex >= metadata.size()) {
      cursorX += fontSize * 0.5f;
      continue;
    }

    const auto &glyph = metadata[glyphIndex];

    YPaintGlyph posGlyph = {};
    posGlyph.x = cursorX + glyph._bearingX * scale;
    posGlyph.y = y - glyph._bearingY * scale;
    posGlyph.setSize(glyph._sizeX * scale, glyph._sizeY * scale);
    posGlyph.setGlyphLayerFlags(static_cast<uint16_t>(glyphIndex),
                                static_cast<uint8_t>(layer),
                                useCustom ? 1u : 0u);
    posGlyph.color = color;

    _glyphs.push_back(posGlyph);
    glyphsAdded++;

    cursorX += glyph._advance * scale;
  }

  if (glyphsAdded > 0) {
    _gridStaging.clear(); // invalidate grid
  }

  return Ok();
}

Result<void> addRotatedText(float x, float y, const std::string &text,
                            float fontSize, uint32_t color,
                            float angleRadians, int fontId) {
  // Rotated text is NOT supported in scrolling mode - the scrolling shader
  // doesn't have SDF_ROTATED_GLYPH handling or custom atlas texture binding.
  // Log a warning and skip rendering (return Ok to not break the flow).
  if (_scrollingMode) {
    ywarn("addRotatedText: rotated text not supported in scrolling mode, "
          "text='{}' ignored",
          text);
    return Ok();
  }
  int resolvedFontId = fontId;
  if (fontId >= 0) {
    auto it = _userFontIdMap.find(fontId);
    if (it != _userFontIdMap.end())
      resolvedFontId = it->second;
  }
  if (resolvedFontId < 0 || !_customAtlas) {
    return Err<void>("addRotatedText: requires a custom font (rotated "
                     "glyphs only work with custom atlas)");
  }
  if (text.empty())
    return Ok();
  MsdfAtlas *activeAtlas = _customAtlas.get();

  float fontBaseSize = activeAtlas->getFontSize();
  float scale = fontSize / fontBaseSize;
  float cosA = std::cos(angleRadians);
  float sinA = std::sin(angleRadians);
  float cursorAdvance = 0.0f;

  const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
  const uint8_t *end = ptr + text.size();

  while (ptr < end) {
    uint32_t codepoint = 0;
    if ((*ptr & 0x80) == 0) {
      codepoint = *ptr++;
    } else if ((*ptr & 0xE0) == 0xC0) {
      codepoint = (*ptr++ & 0x1F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF0) == 0xE0) {
      codepoint = (*ptr++ & 0x0F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF8) == 0xF0) {
      codepoint = (*ptr++ & 0x07) << 18;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else {
      ptr++;
      continue;
    }

    uint32_t glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
    const auto &metadata = activeAtlas->getGlyphMetadata();
    if (glyphIndex >= metadata.size()) {
      cursorAdvance += fontSize * 0.5f;
      continue;
    }

    const auto &glyph = metadata[glyphIndex];
    float bx = glyph._bearingX * scale;
    float by = -glyph._bearingY * scale;
    float gw = glyph._sizeX * scale;
    float gh = glyph._sizeY * scale;

    float gx = x + (cursorAdvance + bx) * cosA - by * sinA;
    float gy = y + (cursorAdvance + bx) * sinA + by * cosA;

    // Build RotatedGlyph primitive (15 words with gridOffset prefix)
    // Layout: [gridOffset][type=65][layer][x][y][scaleX][scaleY][angle]
    //         [glyphIndex][cosAngle][sinAngle][fillColor][strokeColor][strokeWidth][round]
    std::vector<float> primData(15);
    uint16_t gridOffsetCol = _scrollingMode ? _cursorCol : 0;
    uint16_t gridOffsetRow = _scrollingMode ? _cursorRow : 0;
    uint32_t packedOffset = static_cast<uint32_t>(gridOffsetCol) |
                            (static_cast<uint32_t>(gridOffsetRow) << 16);
    std::memcpy(&primData[0], &packedOffset, sizeof(uint32_t));
    sdf::detail::write_u32(primData.data(), 1, 65u); // RotatedGlyph type
    sdf::detail::write_u32(primData.data(), 2, 0u);  // layer 0
    primData[3] = gx;
    primData[4] = gy;
    primData[5] = gw; // width in pixels (shader expects gw, not scale)
    primData[6] = gh; // height in pixels (shader expects gh, not scale)
    primData[7] = angleRadians;
    sdf::detail::write_u32(primData.data(), 8, glyphIndex);
    primData[9] = cosA;
    primData[10] = sinA;
    sdf::detail::write_u32(primData.data(), 11, color); // fillColor
    sdf::detail::write_u32(primData.data(), 12, 0u);    // strokeColor
    primData[13] = 0.0f;                                // strokeWidth
    primData[14] = 0.0f;                                // round

    // Compute AABB for grid placement
    // Rotated glyph bounding box: corners rotated by angle
    float halfW = gw * 0.5f;
    float halfH = gh * 0.5f;
    float corners[4][2] = {
        {-halfW, -halfH}, {halfW, -halfH}, {halfW, halfH}, {-halfW, halfH}};
    float minX = gx, maxX = gx, minY = gy, maxY = gy;
    for (int c = 0; c < 4; c++) {
      float rx = gx + corners[c][0] * cosA - corners[c][1] * sinA;
      float ry = gy + corners[c][0] * sinA + corners[c][1] * cosA;
      minX = std::min(minX, rx);
      maxX = std::max(maxX, rx);
      minY = std::min(minY, ry);
      maxY = std::max(maxY, ry);
    }

    // Compute which lines this primitive spans
    // Use sceneMinY as base for grid line calculation
    // Guard against division by zero if cell size not yet computed
    float baseY = (_sceneMinY < 1e9f) ? _sceneMinY : 0.0f;
    float effectiveCellSizeY =
        (_cellSizeY > 0.0f) ? _cellSizeY : 20.0f; // default fallback
    int32_t localMinLine =
        static_cast<int32_t>(std::floor((minY - baseY) / effectiveCellSizeY));
    int32_t localMaxLine =
        static_cast<int32_t>(std::floor((maxY - baseY) / effectiveCellSizeY));
    int32_t primMinLineSigned =
        static_cast<int32_t>(gridOffsetRow) + localMinLine;
    int32_t primMaxLineSigned =
        static_cast<int32_t>(gridOffsetRow) + localMaxLine;
    primMinLineSigned = std::max(primMinLineSigned, 0);
    primMaxLineSigned = std::max(primMaxLineSigned, 0);
    uint32_t primMinLine = static_cast<uint32_t>(primMinLineSigned);
    uint32_t primMaxLine = static_cast<uint32_t>(primMaxLineSigned);

    // Ensure enough lines exist
    while (_scrollLines.size() <= primMaxLine) {
      ScrollLine line;
      line.cells.resize(_gridWidth > 0 ? _gridWidth : 1);
      _scrollLines.push_back(std::move(line));
    }
    primMinLine =
        std::min(primMinLine, static_cast<uint32_t>(_scrollLines.size() - 1));
    primMaxLine =
        std::min(primMaxLine, static_cast<uint32_t>(_scrollLines.size() - 1));

    // Store primitive in base line
    uint32_t baseLine = primMaxLine;
    uint16_t primIndex =
        static_cast<uint16_t>(_scrollLines[baseLine].prims.size());
    _scrollLines[baseLine].prims.push_back(std::move(primData));

    // Add grid cell references
    uint32_t cellMinX = cellX(minX);
    uint32_t cellMaxX = cellX(maxX);
    for (uint32_t lineIdx = primMinLine; lineIdx <= primMaxLine; lineIdx++) {
      while (_scrollLines[lineIdx].cells.size() <= cellMaxX) {
        _scrollLines[lineIdx].cells.push_back(ScrollGridCell{});
      }
      uint16_t linesAhead = static_cast<uint16_t>(baseLine - lineIdx);
      for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
        _scrollLines[lineIdx].cells[cx].refs.push_back(
            {linesAhead, primIndex});
      }
    }

    // Update scene bounds
    if (!_hasExplicitBounds) {
      _sceneMinX = std::min(_sceneMinX, minX);
      _sceneMinY = std::min(_sceneMinY, minY);
      _sceneMaxX = std::max(_sceneMaxX, maxX);
      _sceneMaxY = std::max(_sceneMaxY, maxY);
    }

    _gridStagingDirty = true;
    _bufferDirty = true;
    cursorAdvance += glyph._advance * scale;
  }

  return Ok();
}

//=========================================================================
// Text measurement & font metrics
//=========================================================================

float measureTextWidth(const std::string &text, float fontSize,
                       int fontId) override {
  int resolvedFontId = fontId;
  if (fontId >= 0) {
    auto it = _userFontIdMap.find(fontId);
    if (it != _userFontIdMap.end())
      resolvedFontId = it->second;
  }
  bool useCustom = (resolvedFontId >= 0 && _customAtlas);
  MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
  if (!activeAtlas || text.empty())
    return 0.0f;

  float fontBaseSize = activeAtlas->getFontSize();
  float scale = fontSize / fontBaseSize;

  float width = 0.0f;
  const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
  const uint8_t *end = ptr + text.size();

  while (ptr < end) {
    uint32_t codepoint = 0;
    if ((*ptr & 0x80) == 0) {
      codepoint = *ptr++;
    } else if ((*ptr & 0xE0) == 0xC0) {
      codepoint = (*ptr++ & 0x1F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF0) == 0xE0) {
      codepoint = (*ptr++ & 0x0F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF8) == 0xF0) {
      codepoint = (*ptr++ & 0x07) << 18;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 12;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F) << 6;
      if (ptr < end)
        codepoint |= (*ptr++ & 0x3F);
    } else {
      ptr++;
      continue;
    }

    uint32_t glyphIndex;
    if (useCustom) {
      glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
    } else if (_font) {
      glyphIndex = _font->getGlyphIndex(codepoint);
    } else {
      continue;
    }

    const auto &metadata = activeAtlas->getGlyphMetadata();
    if (glyphIndex < metadata.size()) {
      width += metadata[glyphIndex]._advance * scale;
    } else {
      width += fontSize * 0.5f;
    }
  }

  return width;
}

float fontAscent(float fontSize, int fontId) override {
  auto it = _ttfMetricsCache.find(fontId);
  if (it != _ttfMetricsCache.end()) {
    bool useCustom = (fontId >= 0 && _customAtlas);
    MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
    float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
    return it->second.ascender * (fontSize / atlasSize);
  }
  return fontSize * 0.8f;
}

float fontDescent(float fontSize, int fontId) override {
  auto it = _ttfMetricsCache.find(fontId);
  if (it != _ttfMetricsCache.end()) {
    bool useCustom = (fontId >= 0 && _customAtlas);
    MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
    float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
    return it->second.descender * (fontSize / atlasSize);
  }
  return fontSize * 0.2f;
}
