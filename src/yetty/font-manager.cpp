#include <yetty/font-manager.h>
#include <yetty/shader-manager.h>
#include <ytrace/ytrace.hpp>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <unordered_map>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

namespace yetty {

class FontManagerImpl : public FontManager {
public:
  FontManagerImpl() = default;
  ~FontManagerImpl() override = default;

  Result<void> init(const GPUContext &gpu, ShaderManager::Ptr shaderMgr,
                    MsdfCdbProvider::Ptr cdbProvider) noexcept {
    if (_initialized)
      return Ok();

    _gpu = gpu;
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

    _initialized = true;
    yinfo("FontManager initialized");
    return Ok();
  }

  Result<MsMsdfFont::Ptr>
  getMsMsdfFont(const std::string &fontName) noexcept override {
    auto it = _msdfFontCache.find(fontName);
    if (it != _msdfFontCache.end()) {
      return Ok(it->second);
    }

    std::string cdbBasePath = _cacheDir + "/" + fontName;

    // Check if all required CDB files exist; generate any missing ones
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

    auto result = MsMsdfFont::create(cdbBasePath);
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
    return Ok();
  }

  Result<void> initBmFont() noexcept {
    auto bmFontResult = BmFont::create(_gpu.device);
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
    std::string shaderDir =
        std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/";

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
  ShaderManager::Ptr _shaderMgr;
  MsdfCdbProvider::Ptr _cdbProvider;
  std::string _cacheDir;
  std::unordered_map<std::string, MsMsdfFont::Ptr> _msdfFontCache;
  std::string _defaultFontName;
  BmFont::Ptr _bitmapFont;
  ShaderFont::Ptr _shaderGlyphFont;
  ShaderFont::Ptr _cardFont;
  bool _initialized = false;
};

// Factory implementation
Result<FontManager::Ptr>
FontManager::createImpl(ContextType &, const GPUContext &gpu,
                        ShaderManager::Ptr shaderMgr,
                        MsdfCdbProvider::Ptr cdbProvider) noexcept {
  auto impl = Ptr(new FontManagerImpl());
  if (auto res = static_cast<FontManagerImpl *>(impl.get())
                     ->init(gpu, shaderMgr, cdbProvider);
      !res) {
    return Err<Ptr>("FontManager init failed", res);
  }
  return Ok(std::move(impl));
}

} // namespace yetty
