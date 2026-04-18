//=========================================================================
// Font API
//=========================================================================

int registerFont(const std::string &cdbPath, const std::string &ttfPath,
                 MsdfCdbProvider::Ptr provider) {
  if (!_customAtlas) {
    auto atlasRes = MsdfAtlas::create(_gpuAllocator);
    if (!atlasRes) {
      yerror("registerFont: failed to create custom atlas");
      return -1;
    }
    _customAtlas = *atlasRes;
    _flags |= FLAG_CUSTOM_ATLAS;
  }
  return _customAtlas->registerCdb(cdbPath, ttfPath, std::move(provider));
}

Result<int> addFont(const std::string &ttfPath) {
  ydebug("addFont: ttfPath='{}'", ttfPath);
  if (!_fontManager) {
    yerror("addFont: _fontManager is NULL!");
    return Err<int>("addFont: no font manager");
  }

  auto stem = std::filesystem::path(ttfPath).stem().string();
  auto cacheDir = _fontManager->getCacheDir();
  auto cdbPath = cacheDir + "/" + stem + ".cdb";

  auto it = _fontIdCache.find(cdbPath);
  if (it != _fontIdCache.end())
    return Ok(it->second);

  if (!std::filesystem::exists(cdbPath)) {
    auto provider = _fontManager->getCdbProvider();
    if (!provider) {
      return Err<int>("addFont: no CDB provider available");
    }
    MsdfCdbConfig cfg;
    cfg.ttfPath = ttfPath;
    cfg.cdbPath = cdbPath;
    if (auto res = provider->generate(cfg); !res) {
      return Err<int>("addFont: CDB generation failed for " + ttfPath);
    }
  }

  if (!_customAtlas) {
    auto atlasRes = MsdfAtlas::create(_gpuAllocator);
    if (!atlasRes) {
      return Err<int>("addFont: failed to create custom atlas");
    }
    _customAtlas = *atlasRes;
    _flags |= FLAG_CUSTOM_ATLAS;
  }

  int fontId = _customAtlas->openCdb(cdbPath);
  if (fontId < 0) {
    return Err<int>("addFont: failed to open CDB: " + cdbPath);
  }
  _fontIdCache[cdbPath] = fontId;

  float atlasSize = _customAtlas->getFontSize();
  _ttfMetricsCache[fontId] = readTtfMetrics(ttfPath, atlasSize);

  ydebug("addFont: registered '{}' on custom atlas (fontId={})", stem,
         fontId);
  return Ok(fontId);
}

Result<void> addFont(int userFontId, const std::string &ttfPath) {
  auto result = addFont(ttfPath);
  if (!result) {
    return Err<void>("addFont(id=" + std::to_string(userFontId) +
                     "): " + result.error().message());
  }
  int atlasId = *result;
  _userFontIdMap[userFontId] = atlasId;
  ydebug("addFont: mapped user fontId {} -> atlas fontId {}", userFontId,
         atlasId);
  return Ok();
}

Result<int> addFontData(const uint8_t *data, size_t size,
                        const std::string &name) {
  ydebug("addFontData: name='{}' size={}", name, size);
  if (!_fontManager) {
    yerror("addFontData: _fontManager is NULL!");
    return Err<int>("addFontData: no font manager");
  }
  uint64_t h = 14695981039346656037ULL;
  for (size_t i = 0; i < size; i++) {
    h ^= data[i];
    h *= 1099511628211ULL;
  }
  char hex[17];
  std::snprintf(hex, sizeof(hex), "%016llx",
                static_cast<unsigned long long>(h));

  auto cacheDir = _fontManager->getCacheDir();
  auto ttfPath = cacheDir + "/pdf_font_" + hex + ".ttf";

  if (!std::filesystem::exists(ttfPath)) {
    std::ofstream out(ttfPath, std::ios::binary);
    if (!out)
      return Err<int>("addFontData: failed to write " + ttfPath);
    out.write(reinterpret_cast<const char *>(data),
              static_cast<std::streamsize>(size));
  }

  return addFont(ttfPath);
}

void mapFontId(int userId, int atlasId) { _userFontIdMap[userId] = atlasId; }

//=========================================================================
// Atlas access
//=========================================================================

MsdfAtlas::Ptr atlas() const override { return _atlas; }
MsdfAtlas::Ptr customAtlas() const override { return _customAtlas; }
bool hasCustomAtlas() const override { return _customAtlas != nullptr; }
MsMsdfFont::Ptr font() const override { return _font; }
FontManager::Ptr fontManager() const override { return _fontManager; }
