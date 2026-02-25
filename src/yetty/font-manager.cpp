#include <yetty/font-manager.h>
#include <yetty/vector-sdf-font.h>
#include <yetty/vector-coverage-font.h>
#include <yetty/raster-font.h>
#include <yetty/shader-manager.h>
#include <ytrace/ytrace.hpp>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <unordered_map>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

namespace yetty {

class FontManagerImpl : public FontManager {
public:
  FontManagerImpl() = default;
  ~FontManagerImpl() override = default;

  Result<void> init(const GPUContext &gpu, GpuAllocator::Ptr allocator,
                    ShaderManager::Ptr shaderMgr,
                    MsdfCdbProvider::Ptr cdbProvider) noexcept {
    if (_initialized)
      return Ok();

    _gpu = gpu;
    _allocator = std::move(allocator);
    _shaderMgr = shaderMgr;
    _cdbProvider = cdbProvider;

    if (auto res = initMsMsdfFonts(); !res) {
      return Err<void>("Failed to initialize MSDF fonts", res);
    }

    if (auto res = initBmFont(); !res) {
      ywarn("Failed to initialize BmFont: {} - emoji rendering disabled",
            res.error().message());
    }

    if (auto res = initShaderFonts(); !res) {
      ywarn("Failed to initialize ShaderFonts: {} - shader glyphs disabled",
            res.error().message());
    }

    if (auto res = initVectorSdfFont(); !res) {
      return Err<void>("Failed to initialize VectorSdfFont", res);
    }

    if (auto res = initRasterFont(); !res) {
      ywarn("Failed to initialize RasterFont: {} - raster font disabled",
            res.error().message());
    }

    _initialized = true;
    yinfo("FontManager initialized");
    return Ok();
  }

  Result<MsMsdfFont::Ptr>
  getMsMsdfFont(const std::string &fontName) noexcept override {
    yinfo("getMsMsdfFont: entered with fontName={}", fontName);
    yinfo("getMsMsdfFont: checking cache, size={}", _msdfFontCache.size());
    auto it = _msdfFontCache.find(fontName);
    yinfo("getMsMsdfFont: find completed");
    if (it != _msdfFontCache.end()) {
      yinfo("getMsMsdfFont: found in cache");
      return Ok(it->second);
    }
    yinfo("getMsMsdfFont: not in cache, loading");

    yinfo("getMsMsdfFont: _cacheDir={}", _cacheDir);
    std::string cdbBasePath = _cacheDir + "/" + fontName;
    yinfo("getMsMsdfFont: cdbBasePath={}", cdbBasePath);

    // Check if all required CDB files exist; generate any missing ones
    yinfo("getMsMsdfFont: checking _cdbProvider={}", (void*)_cdbProvider.get());
    if (_cdbProvider) {
      static const std::array<std::string, 4> styleSuffixes = {
          "Regular", "Bold", "Oblique", "BoldOblique"};

      auto ttfPaths = findTtfPaths(fontName);
      bool anyMissing = false;
      for (size_t i = 0; i < styleSuffixes.size(); ++i) {
        if (i < ttfPaths.size() && !ttfPaths[i].empty()) {
          std::string cdbPath = cdbBasePath + "-" + styleSuffixes[i] + ".cdb";
          if (!std::filesystem::exists(cdbPath)) {
            anyMissing = true;
            break;
          }
        }
      }

      if (anyMissing && !ttfPaths.empty() && !ttfPaths[0].empty()) {
        yinfo("CDB missing for {}, generating via {} provider...", fontName,
              _cdbProvider->name());

        for (size_t i = 0; i < styleSuffixes.size(); ++i) {
          if (i >= ttfPaths.size() || ttfPaths[i].empty()) continue;
          std::string cdbPath = cdbBasePath + "-" + styleSuffixes[i] + ".cdb";
          if (std::filesystem::exists(cdbPath)) continue;

          MsdfCdbConfig cfg;
          cfg.ttfPath = ttfPaths[i];
          cfg.cdbPath = cdbPath;

          if (auto res = _cdbProvider->generate(cfg); !res) {
            return Err<MsMsdfFont::Ptr>("CDB generation failed for " + fontName,
                                        res);
          }
        }

        yinfo("CDB generation complete for {}", fontName);
      }
    }

    yinfo("getMsMsdfFont: calling MsMsdfFont::create");
    auto result = MsMsdfFont::create(cdbBasePath, _allocator);
    yinfo("getMsMsdfFont: MsMsdfFont::create returned");
    if (!result) {
      return Err<MsMsdfFont::Ptr>("Failed to load MsMsdfFont: " + fontName,
                                  result);
    }

    auto font = std::move(*result);

    // Set CJK fallback CDB if it exists
    std::string cjkFallbackPath = _cacheDir + "/NotoSansCJK-Regular.cdb";
    if (std::filesystem::exists(cjkFallbackPath)) {
      font->setFallbackCdb(cjkFallbackPath);
      yinfo("Set CJK fallback CDB: {}", cjkFallbackPath);
    }

    // Create GPU resources (texture, sampler, metadata buffer) on the atlas
    if (auto res = font->atlas()->createTexture(_gpu.device, _gpu.queue); !res) {
      return Err<MsMsdfFont::Ptr>("Failed to create MsdfAtlas texture", res);
    }
    if (auto res = font->atlas()->createGlyphMetadataBuffer(_gpu.device); !res) {
      return Err<MsMsdfFont::Ptr>("Failed to create MsdfAtlas metadata buffer",
                                  res);
    }

    _msdfFontCache[fontName] = font;

    if (_defaultFontName.empty()) {
      _defaultFontName = fontName;
    }

    yinfo("Loaded MsMsdfFont: {}", fontName);
    return Ok(font);
  }

  MsMsdfFont::Ptr getDefaultMsMsdfFont() noexcept override {
    if (!_defaultFontName.empty()) {
      auto it = _msdfFontCache.find(_defaultFontName);
      if (it != _msdfFontCache.end()) {
        return it->second;
      }
    }

    auto result = getMsMsdfFont("DejaVuSansMNerdFontMono");
    if (result) {
      return *result;
    }

    if (!_msdfFontCache.empty()) {
      return _msdfFontCache.begin()->second;
    }

    yerror("No MSDF fonts available");
    return nullptr;
  }

  BmFont::Ptr getDefaultBmFont() noexcept override { return _bitmapFont; }

  ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept override {
    return _shaderGlyphFont;
  }

  ShaderFont::Ptr getDefaultCardFont() noexcept override { return _cardFont; }

  Result<VectorSdfFont::Ptr> getVectorSdfFont(const std::string &ttfPath) noexcept override {
    auto it = _vectorFontCache.find(ttfPath);
    if (it != _vectorFontCache.end()) {
      return Ok(it->second);
    }

    auto result = VectorSdfFont::create(_gpu, _allocator, ttfPath);
    if (!result) {
      return Err<VectorSdfFont::Ptr>("Failed to create VectorSdfFont: " + ttfPath, result);
    }

    auto font = std::move(*result);

    // Load basic Latin glyphs
    if (auto res = font->loadBasicLatin(); !res) {
      ywarn("Failed to load basic Latin for VectorSdfFont: {}", res.error().message());
    }

    _vectorFontCache[ttfPath] = font;

    if (!_defaultVectorSdfFont) {
      _defaultVectorSdfFont = font;
    }

    yinfo("Created VectorSdfFont: {} ({} glyphs, {} curves, {} bytes)",
          ttfPath, font->glyphCount(), font->totalCurves(), font->bufferSize());

    return Ok(font);
  }

  VectorSdfFont::Ptr getDefaultVectorSdfFont() noexcept override {
    return _defaultVectorSdfFont;
  }

  Result<VectorCoverageFont::Ptr> getVectorCoverageFont(const std::string &ttfPath) noexcept override {
    auto it = _vectorCoverageFontCache.find(ttfPath);
    if (it != _vectorCoverageFontCache.end()) {
      return Ok(it->second);
    }

    auto result = VectorCoverageFont::create(_gpu, _allocator, ttfPath);
    if (!result) {
      return Err<VectorCoverageFont::Ptr>("Failed to create VectorCoverageFont: " + ttfPath, result);
    }

    auto font = std::move(*result);

    // Load basic Latin glyphs
    if (auto res = font->loadBasicLatin(); !res) {
      ywarn("Failed to load basic Latin for VectorCoverageFont: {}", res.error().message());
    }

    _vectorCoverageFontCache[ttfPath] = font;

    if (!_defaultVectorCoverageFont) {
      _defaultVectorCoverageFont = font;
    }

    yinfo("Created VectorCoverageFont: {} ({} glyphs, {} curves, {} bytes)",
          ttfPath, font->glyphCount(), font->totalCurves(), font->bufferSize());

    return Ok(font);
  }

  VectorCoverageFont::Ptr getDefaultVectorCoverageFont() noexcept override {
    return _defaultVectorCoverageFont;
  }

  Result<RasterFont::Ptr> getRasterFont(const std::string &ttfPath,
                                        uint32_t cellWidth,
                                        uint32_t cellHeight) noexcept override {
    // Cache by path only - cell size changes are handled via setCellSize()
    auto it = _rasterFontCache.find(ttfPath);
    if (it != _rasterFontCache.end()) {
      return Ok(it->second);
    }

    auto result = RasterFont::create(_gpu, _allocator, ttfPath, cellWidth, cellHeight);
    if (!result) {
      return Err<RasterFont::Ptr>("Failed to create RasterFont: " + ttfPath, result);
    }

    auto font = std::move(*result);

    // Load basic Latin glyphs
    if (auto res = font->loadBasicLatin(); !res) {
      ywarn("Failed to load basic Latin for RasterFont: {}", res.error().message());
    }

    // Upload to GPU
    font->uploadToGpu();

    _rasterFontCache[ttfPath] = font;

    if (!_defaultRasterFont) {
      _defaultRasterFont = font;
    }

    yinfo("Created RasterFont: {} (cell={}x{}, {} glyphs)",
          ttfPath, cellWidth, cellHeight, font->glyphCount());

    return Ok(font);
  }

  RasterFont::Ptr getDefaultRasterFont() noexcept override {
    return _defaultRasterFont;
  }

  void setDefaultFont(const std::string &fontName) noexcept override {
    _defaultFontName = fontName;
  }

  bool hasFont(const std::string &fontName) const noexcept override {
    return _msdfFontCache.find(fontName) != _msdfFontCache.end();
  }

  const std::string &getCacheDir() const noexcept override { return _cacheDir; }

  MsdfCdbProvider::Ptr getCdbProvider() const noexcept override {
    return _cdbProvider;
  }

private:
  Result<void> initMsMsdfFonts() noexcept {
#ifdef YETTY_ASSETS_DIR
    // Use prebuilt CDB files from build assets directory
    _cacheDir = std::string(YETTY_ASSETS_DIR) + "/fonts-cdb";
    yinfo("MSDF font cache dir (prebuilt): {}", _cacheDir);
#else
    const char *home = std::getenv("HOME");
    if (!home) {
      return Err<void>("HOME environment variable not set");
    }

    _cacheDir = std::string(home) + "/.cache/yetty/msdf-font-cache";

    if (!std::filesystem::exists(_cacheDir)) {
      yinfo("Creating MSDF font cache directory: {}", _cacheDir);
      std::filesystem::create_directories(_cacheDir);
    }

    yinfo("MSDF font cache dir: {}", _cacheDir);
#endif
    return Ok();
  }

  Result<void> initBmFont() noexcept {
    auto bmFontResult = BmFont::create(_gpu.device, _allocator);
    if (!bmFontResult) {
      return Err<void>("Failed to create BmFont", bmFontResult);
    }

    _bitmapFont = *bmFontResult;

    if (auto res = _bitmapFont->loadCommonGlyphs(); !res) {
      ywarn("Failed to load common glyphs: {}", res.error().message());
    }

    yinfo("BmFont created successfully");
    return Ok();
  }

  Result<void> initShaderFonts() noexcept {
    std::string shaderDir = std::string(YETTY_SHADERS_DIR) + "/";

    auto shaderGlyphResult =
        ShaderFont::create(_shaderMgr, ShaderFont::Category::Glyph, shaderDir);
    if (!shaderGlyphResult) {
      ywarn("Failed to create ShaderFont for glyphs: {}",
            shaderGlyphResult.error().message());
    } else {
      _shaderGlyphFont = *shaderGlyphResult;
      yinfo("ShaderFont for glyphs created with {} shaders",
            _shaderGlyphFont->getFunctionCount());
    }

    auto cardFontResult =
        ShaderFont::create(_shaderMgr, ShaderFont::Category::Card, shaderDir);
    if (!cardFontResult) {
      ywarn("Failed to create ShaderFont for cards: {}",
            cardFontResult.error().message());
    } else {
      _cardFont = *cardFontResult;
      yinfo("ShaderFont for cards created with {} shaders",
            _cardFont->getFunctionCount());
    }

    return Ok();
  }

  Result<void> initVectorSdfFont() noexcept {
    // Use the default monospace font TTF
#ifdef YETTY_ASSETS_DIR
    std::string ttfPath = std::string(YETTY_ASSETS_DIR) +
                          "/DejaVuSansMNerdFontMono-Regular.ttf";
#else
    std::string ttfPath = std::string(CMAKE_SOURCE_DIR) +
                          "/assets/DejaVuSansMNerdFontMono-Regular.ttf";
#endif

    if (!std::filesystem::exists(ttfPath)) {
      return Err<void>("Default TTF not found: " + ttfPath);
    }

    auto result = VectorSdfFont::create(_gpu, _allocator, ttfPath);
    if (!result) {
      return Err<void>("Failed to create VectorSdfFont", result);
    }

    _defaultVectorSdfFont = std::move(*result);

    // Load basic Latin glyphs for testing
    if (auto res = _defaultVectorSdfFont->loadBasicLatin(); !res) {
      return Err<void>("Failed to load basic Latin glyphs", res);
    }

    yinfo("VectorSdfFont initialized: {} glyphs, {} curves, {} bytes",
          _defaultVectorSdfFont->glyphCount(),
          _defaultVectorSdfFont->totalCurves(),
          _defaultVectorSdfFont->bufferSize());

    return Ok();
  }

  Result<void> initRasterFont() noexcept {
    // Use the default monospace font TTF
#ifdef YETTY_ASSETS_DIR
    std::string ttfPath = std::string(YETTY_ASSETS_DIR) +
                          "/DejaVuSansMNerdFontMono-Regular.ttf";
#else
    std::string ttfPath = std::string(CMAKE_SOURCE_DIR) +
                          "/assets/DejaVuSansMNerdFontMono-Regular.ttf";
#endif

    if (!std::filesystem::exists(ttfPath)) {
      return Err<void>("Default TTF not found: " + ttfPath);
    }

    // Create with placeholder cell size - gpu-screen will call setCellSize() with actual size
    auto result = RasterFont::create(_gpu, _allocator, ttfPath, 16, 32);
    if (!result) {
      return Err<void>("Failed to create RasterFont", result);
    }

    _defaultRasterFont = std::move(*result);

    // Load basic Latin glyphs for testing
    if (auto res = _defaultRasterFont->loadBasicLatin(); !res) {
      return Err<void>("Failed to load basic Latin glyphs", res);
    }

    // Upload to GPU
    _defaultRasterFont->uploadToGpu();

    yinfo("RasterFont initialized: {} glyphs, cell={}x{}",
          _defaultRasterFont->glyphCount(),
          _defaultRasterFont->getCellWidth(),
          _defaultRasterFont->getCellHeight());

    return Ok();
  }

  // Discover TTF paths for a font name in assets/ directory
  // Returns [Regular, Bold, Oblique, BoldOblique] paths (empty string if not
  // found)
  std::vector<std::string> findTtfPaths(const std::string &fontName) {
    static const std::array<std::string, 4> suffixes = {
        "Regular", "Bold", "Oblique", "BoldOblique"};

    std::string assetsDir = std::string(CMAKE_SOURCE_DIR) + "/assets";
    std::vector<std::string> paths(4);

    for (size_t i = 0; i < suffixes.size(); ++i) {
      std::string candidate =
          assetsDir + "/" + fontName + "-" + suffixes[i] + ".ttf";
      if (std::filesystem::exists(candidate)) {
        paths[i] = candidate;
        yinfo("Found TTF: {}", candidate);
      }
    }

    return paths;
  }

  GPUContext _gpu = {};
  GpuAllocator::Ptr _allocator;
  ShaderManager::Ptr _shaderMgr;
  MsdfCdbProvider::Ptr _cdbProvider;
  std::string _cacheDir;
  std::unordered_map<std::string, MsMsdfFont::Ptr> _msdfFontCache;
  std::string _defaultFontName;
  BmFont::Ptr _bitmapFont;
  ShaderFont::Ptr _shaderGlyphFont;
  ShaderFont::Ptr _cardFont;
  std::unordered_map<std::string, VectorSdfFont::Ptr> _vectorFontCache;
  VectorSdfFont::Ptr _defaultVectorSdfFont;
  std::unordered_map<std::string, VectorCoverageFont::Ptr> _vectorCoverageFontCache;
  VectorCoverageFont::Ptr _defaultVectorCoverageFont;
  std::unordered_map<std::string, RasterFont::Ptr> _rasterFontCache;
  RasterFont::Ptr _defaultRasterFont;
  bool _initialized = false;
};

// Factory implementation
Result<FontManager::Ptr>
FontManager::createImpl(ContextType &, const GPUContext &gpu,
                        GpuAllocator::Ptr allocator,
                        ShaderManager::Ptr shaderMgr,
                        MsdfCdbProvider::Ptr cdbProvider) noexcept {
  auto impl = Ptr(new FontManagerImpl());
  if (auto res = static_cast<FontManagerImpl *>(impl.get())
                     ->init(gpu, std::move(allocator), shaderMgr, cdbProvider);
      !res) {
    return Err<Ptr>("FontManager init failed", res);
  }
  return Ok(std::move(impl));
}

} // namespace yetty
