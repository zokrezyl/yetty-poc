//=========================================================================
// Buffer management - addYpaintBuffer
//=========================================================================

Result<uint32_t>
addYpaintBuffer(std::shared_ptr<YPaintBuffer> buffer) override {
  if (!buffer) {
    return Err<uint32_t>("addYpaintBuffer: null buffer");
  }
  if (!_canvas) {
    return Err<uint32_t>("addYpaintBuffer: no canvas");
  }

  uint32_t baseGlyphIdx = static_cast<uint32_t>(_glyphs.size());

  // 1. Process fonts from input buffer (register with atlas)
  ydebug("addYpaintBuffer: processing fonts");
  buffer->forEachFont([this](int bufFontId, const uint8_t *data, size_t size,
                             const std::string &name) {
    ydebug("forEachFont: bufFontId={} name='{}' size={}", bufFontId, name,
           size);
    if (_bufferFontIdMap.count(bufFontId))
      return;
    auto res = addFontData(data, size, name);
    if (res) {
      _bufferFontIdMap[bufFontId] = *res;
      ydebug("forEachFont: SUCCESS, atlasFontId={}", *res);
    } else {
      yerror("forEachFont: FAILED: {}", res.error().message());
    }
  });

  // 2. Process text spans -> glyphs
  buffer->forEachTextSpan([this](const TextSpanData &span) {
    int atlasFontId = -1;
    auto it = _bufferFontIdMap.find(span.fontId);
    if (it != _bufferFontIdMap.end())
      atlasFontId = it->second;

    if (std::abs(span.rotation) > 0.001f) {
      ydebug("text span: rotation={} atlasFontId={} text='{}'", span.rotation,
             atlasFontId, span.text);
      auto res = addRotatedText(span.x, span.y, span.text, span.fontSize,
                                span.color, span.rotation, atlasFontId);
      if (!res) {
        yerror("addRotatedText FAILED: {}", res.error().message());
      }
    } else {
      addText(span.x, span.y, span.text, span.fontSize, span.color,
              span.layer, atlasFontId);
    }
  });

  // 2b. Extend scene bounds for new glyphs (MUST happen before cell size
  // computation!)
  for (uint32_t gi = baseGlyphIdx; gi < _glyphs.size(); gi++) {
    const auto &g = _glyphs[gi];
    if (!_hasExplicitBounds) {
      _sceneMinX = std::min(_sceneMinX, g.x);
      _sceneMinY = std::min(_sceneMinY, g.y);
      _sceneMaxX = std::max(_sceneMaxX, g.x + g.width());
      _sceneMaxY = std::max(_sceneMaxY, g.y + g.height());
    }
  }

  // 3. Process images
  buffer->forEachImage([this](const ImageData &img) {
    PendingImage pi;
    pi.x = img.x;
    pi.y = img.y;
    pi.w = img.w;
    pi.h = img.h;
    pi.pixelWidth = img.pixelWidth;
    pi.pixelHeight = img.pixelHeight;
    pi.layer = img.layer;
    pi.pixels = img.pixels;
    pi.texHandle = TextureHandle::invalid();
    pi.primId = 0;
    _pendingImages.push_back(std::move(pi));
  });

  // 4. Copy scene metadata from input buffer
  if (buffer->hasSceneBounds() && !_hasExplicitBounds) {
    _sceneMinX = std::min(_sceneMinX, buffer->sceneMinX());
    _sceneMinY = std::min(_sceneMinY, buffer->sceneMinY());
    _sceneMaxX = std::max(_sceneMaxX, buffer->sceneMaxX());
    _sceneMaxY = std::max(_sceneMaxY, buffer->sceneMaxY());
  }
  if (buffer->bgColor() != 0 && _bgColor == 0xFF2E1A1A) {
    _bgColor = buffer->bgColor();
  }
  _flags |= buffer->flags();

  // 5. Ensure grid dimensions are set
  // Auto-compute cell size if not set (DEFAULT_CELL_SIZE = 0)
  if (_cellSizeX <= 0.0f || _cellSizeY <= 0.0f) {
    float sceneW = _sceneMaxX - _sceneMinX;
    float sceneH = _sceneMaxY - _sceneMinY;
    if (sceneW <= 0)
      sceneW = 100;
    if (sceneH <= 0)
      sceneH = 100;
    // Default to ~10x10 grid cells
    _cellSizeX = std::max(1.0f, sceneW / 10.0f);
    _cellSizeY = std::max(1.0f, sceneH / 10.0f);
  }
  if (_gridWidth == 0 || _gridHeight == 0) {
    float sceneW = _sceneMaxX - _sceneMinX;
    float sceneH = _sceneMaxY - _sceneMinY;
    if (sceneW <= 0)
      sceneW = 100;
    if (sceneH <= 0)
      sceneH = 100;
    _gridWidth =
        std::max(1u, static_cast<uint32_t>(std::ceil(sceneW / _cellSizeX)));
    _gridHeight =
        std::max(1u, static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY)));
  }

  // Update canvas with scene bounds and cell size
  _canvas->setSceneBounds(_sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);
  _canvas->setCellSize(_cellSizeX, _cellSizeY);

  // 6. Compute content bounds
  float contentMinY = 1e10f;
  float contentMaxY = -1e10f;
  uint32_t primCount = 0;
  buffer->forEachPrim([&](uint32_t id, const float *data, uint32_t wc) {
    uint32_t type = sdf::detail::read_u32(data, 0);
    if (type >= 100 && type < 128)
      return; // Skip 3D
    float minX, minY, maxX, maxY;
    computeAABB(data, wc, minX, minY, maxX, maxY);
    contentMinY = std::min(contentMinY, minY);
    contentMaxY = std::max(contentMaxY, maxY);
    if (!_hasExplicitBounds) {
      _sceneMinX = std::min(_sceneMinX, minX);
      _sceneMinY = std::min(_sceneMinY, minY);
      _sceneMaxX = std::max(_sceneMaxX, maxX);
      _sceneMaxY = std::max(_sceneMaxY, maxY);
    }
    primCount++;
  });

  float contentHeight =
      (contentMaxY > contentMinY) ? (contentMaxY - contentMinY) : 0.0f;
  uint32_t contentLines = std::max(
      1u, static_cast<uint32_t>(std::ceil(contentHeight / _cellSizeY)));

  // Apply padding to scene bounds (if not explicit bounds)
  if (!_hasExplicitBounds) {
    float padX = (_sceneMaxX - _sceneMinX) * _paddingX;
    float padY = (_sceneMaxY - _sceneMinY) * _paddingY;
    _sceneMinX -= padX;
    _sceneMinY -= padY;
    _sceneMaxX += padX;
    _sceneMaxY += padY;
  }

  // Update canvas scene bounds again if they were extended
  _canvas->setSceneBounds(_sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);

  // 7. Process each primitive - add to canvas
  uint32_t addedPrims = 0;
  buffer->forEachPrim([this, &addedPrims](uint32_t id, const float *data, uint32_t wc) {
    uint32_t type = sdf::detail::read_u32(data, 0);
    bool is3D = (type >= 100 && type < 128);
    if (is3D) {
      _flags |= FLAG_HAS_3D;
      return; // Skip 3D for now
    }

    // Compute AABB
    float minX, minY, maxX, maxY;
    computeAABB(data, wc, minX, minY, maxX, maxY);

    // Build primitive data vector (without gridOffset - canvas adds it)
    std::vector<float> primData(wc);
    for (uint32_t i = 0; i < wc; i++) {
      primData[i] = data[i];
    }

    // Add to canvas - it handles grid placement and offset
    _canvas->addPrimitive(std::move(primData), minX, minY, maxX, maxY);
    addedPrims++;
  });

  _bufferDirty = true;

  ydebug("addYpaintBuffer: added {} prims (forEachPrim counted {}), canvas has {} prims, {} glyphs, "
         "scene=[{:.0f},{:.0f}]-[{:.0f},{:.0f}]",
         addedPrims, primCount, _canvas ? _canvas->primitiveCount() : 0,
         _glyphs.size() - baseGlyphIdx, _sceneMinX, _sceneMinY,
         _sceneMaxX, _sceneMaxY);

  return Ok(contentLines);
}

void clear() override {
  // Clear canvas
  if (_canvas) {
    _canvas->clear();
  }

  // Clear glyph data
  _glyphs.clear();
  _glyphSortedOrder.clear();

  // Clear grid dimensions
  _gridWidth = 0;
  _gridHeight = 0;

  // Reset cursor
  _cursorCol = 0;
  _cursorRow = 0;

  // Reset scene bounds to extreme values (auto-extend mode)
  if (!_hasExplicitBounds) {
    _sceneMinX = 1e10f;
    _sceneMinY = 1e10f;
    _sceneMaxX = -1e10f;
    _sceneMaxY = -1e10f;
  }

  // Reset dirty flags
  _bufferDirty = true;
  _metadataDirty = true;

  ydebug("Painter::clear: reset all primitive and grid data");
}
