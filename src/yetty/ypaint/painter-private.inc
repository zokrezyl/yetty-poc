//=========================================================================
// GPU helpers (private)
//=========================================================================

Result<void> flushMetadata() {
  // Pack zoom as f16 in upper 16 bits of flags
  uint32_t zoomBits;
  {
    uint32_t f32bits;
    std::memcpy(&f32bits, &_viewZoom, sizeof(float));
    uint32_t sign = (f32bits >> 16) & 0x8000;
    int32_t exp = ((f32bits >> 23) & 0xFF) - 127 + 15;
    uint32_t mant = (f32bits >> 13) & 0x3FF;
    if (exp <= 0) {
      exp = 0;
      mant = 0;
    } else if (exp >= 31) {
      exp = 31;
      mant = 0;
    }
    zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
  }

  float contentW = _sceneMaxX - _sceneMinX;
  float contentH = _sceneMaxY - _sceneMinY;
  int16_t panXi16 = static_cast<int16_t>(std::clamp(
      _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
  int16_t panYi16 = static_cast<int16_t>(std::clamp(
      _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

  uint32_t primOffset =
      _primHandle.isValid() ? _primHandle.offset / sizeof(float) : 0;

  YPaintMetadata meta = {};
  meta.primitiveOffset = primOffset;
  meta.primitiveCount = primitiveCount();
  meta.gridOffset = _gpuGridOffset;
  meta.gridWidth = _gridWidth;
  meta.gridHeight = _gridHeight;
  meta.cellSizeXY = YPaintMetadata::packCellSize(_cellSizeX, _cellSizeY);
  meta.glyphOffset = _gpuGlyphOffset;
  meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
  std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
  std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
  std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
  std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
  meta.widthCells =
      (_viewWidthCells & 0xFFFF) |
      (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
  meta.heightCells =
      (_viewHeightCells & 0xFFFF) |
      (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
  meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
  meta.bgColor = _bgColor;

  ydebug("flushMetadata: primOff={} primCnt={} gridOff={} grid={}x{} "
         "scene=[{},{}]-[{},{}]",
         meta.primitiveOffset, meta.primitiveCount, meta.gridOffset,
         meta.gridWidth, meta.gridHeight, _sceneMinX, _sceneMinY, _sceneMaxX,
         _sceneMaxY);

  return _cardMgr->writeMetadata(MetadataHandle{_metaSlotIndex * 64, 64},
                                 &meta, sizeof(meta));
}

uint32_t computeDerivedSize() const {
  uint32_t gridBytes =
      static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
  uint32_t glyphBytes =
      static_cast<uint32_t>(_glyphs.size() * sizeof(YPaintGlyph));
  uint32_t total = gridBytes + glyphBytes;
  if (_customAtlas) {
    uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
    uint32_t glyphMetaBytes = static_cast<uint32_t>(
        _customAtlas->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
    total += atlasHeaderBytes + glyphMetaBytes;
  }
  return total;
}

Result<void> writeDerived() {
  if (!_derivedHandle.isValid()) {
    return Err<void>("writeDerived: derived handle not allocated");
  }

  uint8_t *base = _derivedHandle.data;
  uint32_t offset = 0;

  // Copy grid and translate prim indices to word offsets
  uint32_t gridBytes =
      static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
  _gpuGridOffset = (_derivedHandle.offset + offset) / sizeof(float);
  ydebug("writeDerived: gridSize={} gridOffset={} primWordOffsets.size={}",
         _gridStaging.size(), _gpuGridOffset, _primWordOffsets.size());
  if (!_gridStaging.empty()) {
    std::memcpy(base + offset, _gridStaging.data(), gridBytes);
    // Translate prim indices in grid entries to word offsets
    uint32_t *gridPtr = reinterpret_cast<uint32_t *>(base + offset);
    uint32_t gridSize = static_cast<uint32_t>(_gridStaging.size());
    uint32_t numCells = _gridWidth * _gridHeight;
    if (!_primWordOffsets.empty() && numCells <= gridSize) {
      for (uint32_t ci = 0; ci < numCells; ci++) {
        uint32_t packedOff = gridPtr[ci];
        if (packedOff >= gridSize)
          continue;
        uint32_t cnt = gridPtr[packedOff];
        for (uint32_t j = 0; j < cnt; j++) {
          uint32_t idx = packedOff + 1 + j;
          if (idx >= gridSize)
            break;
          uint32_t rawVal = gridPtr[idx];
          if ((rawVal & 0x80000000u) != 0) {
            continue;
          }
          if (rawVal < static_cast<uint32_t>(_primWordOffsets.size())) {
            gridPtr[idx] = _primWordOffsets[rawVal];
          }
        }
      }
    }
  }
  offset += gridBytes;

  // Copy glyphs
  uint32_t glyphBytes =
      static_cast<uint32_t>(_glyphs.size() * sizeof(YPaintGlyph));
  _gpuGlyphOffset = (_derivedHandle.offset + offset) / sizeof(float);
  if (!_glyphs.empty()) {
    std::memcpy(base + offset, _glyphs.data(), glyphBytes);
  }
  offset += glyphBytes;

  // Custom atlas header + glyph metadata
  if (_customAtlas) {
    _gpuAtlasHeaderOffset = (_derivedHandle.offset + offset) / sizeof(float);
    uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);

    if (_atlasTextureHandle.isValid()) {
      // Texture already allocated — write actual atlas position
      auto texMgr = _cardMgr->textureManager();
      auto atlasPos = texMgr->getAtlasPosition(_atlasTextureHandle);
      uint32_t msdfAtlasW = _customAtlas->getAtlasWidth();
      uint32_t msdfAtlasH = _customAtlas->getAtlasHeight();
      const auto &glyphMeta = _customAtlas->getGlyphMetadata();
      uint32_t atlasHeader[4] = {
          (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
          (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
          static_cast<uint32_t>(glyphMeta.size()), 0};
      std::memcpy(base + offset, atlasHeader, sizeof(atlasHeader));
    } else {
      // Texture not yet allocated — zero-fill, writeTextures() will fill
      // later
      std::memset(base + offset, 0, atlasHeaderBytes);
    }
    offset += atlasHeaderBytes;

    const auto &glyphMeta = _customAtlas->getGlyphMetadata();
    if (!glyphMeta.empty()) {
      uint32_t metaBytes =
          static_cast<uint32_t>(glyphMeta.size() * sizeof(GlyphMetadataGPU));
      std::memcpy(base + offset, glyphMeta.data(), metaBytes);
    }
  }

  return Ok();
}

//=========================================================================
// Data (private members)
//=========================================================================

// Packed GPU format (cache, rebuilt when _gridStagingDirty)
std::vector<uint32_t> _gridStaging;
bool _gridStagingDirty = true;

// Prim staging cache (rebuilt when _bufferDirty)
std::vector<uint32_t> _primStagingCache;

std::vector<YPaintGlyph> _glyphs;
std::vector<uint32_t> _glyphSortedOrder;

// Grid dimensions
uint32_t _gridWidth = 0;
uint32_t _gridHeight = 0;

// Scene bounds (initialized to extreme values, extended by addYpaintBuffer)
float _sceneMinX = 1e10f, _sceneMinY = 1e10f;
float _sceneMaxX = -1e10f, _sceneMaxY = -1e10f;
bool _hasExplicitBounds = false;

// Grid parameters
float _cellSizeX = DEFAULT_CELL_SIZE;
float _cellSizeY = DEFAULT_CELL_SIZE;
uint32_t _maxPrimsPerCell = DEFAULT_MAX_PRIMS_PER_CELL;

// Appearance
uint32_t _bgColor = 0xFF2E1A1A;
uint32_t _flags = 0;

// Font state
FontManager::Ptr _fontManager;
MsMsdfFont::Ptr _font;
MsdfAtlas::Ptr _atlas;
std::unordered_map<std::string, int> _fontIdCache;

// Custom atlas
MsdfAtlas::Ptr _customAtlas;
GpuAllocator::Ptr _gpuAllocator;

// TTF font metrics cache (fontId -> hhea ascender/descender)
std::unordered_map<int, TtfMetrics> _ttfMetricsCache;

// User-specified font ID mapping (user fontId -> atlas fontId)
std::unordered_map<int, int> _userFontIdMap;

// Buffer font ID -> atlas font ID mapping (populated during calculate())
std::unordered_map<int, int> _bufferFontIdMap;

// GPU state (managed by lifecycle methods)
CardManager::Ptr _cardMgr;
uint32_t _metaSlotIndex = 0;
BufferHandle _primHandle = BufferHandle::invalid();
BufferHandle _derivedHandle = BufferHandle::invalid();
std::vector<uint32_t> _primWordOffsets;
uint32_t _gpuGridOffset = 0;
uint32_t _gpuGlyphOffset = 0;
uint32_t _gpuAtlasHeaderOffset = 0;
TextureHandle _atlasTextureHandle = TextureHandle::invalid();
uint32_t _allocatedAtlasW = 0;
uint32_t _allocatedAtlasH = 0;

// Pending images (from buffer, to be allocated in texture atlas)
struct PendingImage {
  float x, y, w, h;
  uint32_t pixelWidth, pixelHeight;
  uint32_t layer;
  std::vector<uint8_t> pixels;
  TextureHandle texHandle = TextureHandle::invalid();
  uint32_t primId = 0; // ID of Image primitive in buffer
};
std::vector<PendingImage> _pendingImages;

bool _bufferDirty = true;
bool _metadataDirty = true;

// View state (set by card via setViewport/setView)
uint32_t _viewWidthCells = 0;
uint32_t _viewHeightCells = 0;
float _viewZoom = 1.0f;
float _viewPanX = 0.0f;
float _viewPanY = 0.0f;

// Scrolling mode state (terminal-style scrolling)
bool _scrollingMode = false;
uint16_t _cursorCol = 0;
uint16_t _cursorRow = 0;

//=========================================================================
// Scrolling mode line-based storage
//=========================================================================

// Reference to a primitive: linesAhead = offset to line containing the data,
// primIndex = index within that line's prims vector
struct ScrollPrimRef {
  uint16_t linesAhead; // relative offset to base line (0 = same line)
  uint16_t primIndex;  // index within base line's prims
};

// Grid cell in scrolling mode: stores references to primitives
struct ScrollGridCell {
  std::vector<ScrollPrimRef> refs;
};

// A single line in scrolling mode
struct ScrollLine {
  // Primitives whose BASE (last line they touch) is this line
  std::vector<std::vector<float>> prims;
  // Grid cells for this line's Y range (indexed by X)
  std::vector<ScrollGridCell> cells;
};

// Lines in scrolling mode (front = top, pop_front on scroll)
std::deque<ScrollLine> _scrollLines;
