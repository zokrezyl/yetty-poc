#include <yetty/yetty-font-manager.h>
#include <ytrace/ytrace.hpp>

#include <cstdlib>
#include <filesystem>
#include <unordered_map>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

namespace yetty {

class YettyFontManagerImpl : public YettyFontManager {
public:
    YettyFontManagerImpl() = default;
    ~YettyFontManagerImpl() override = default;

    Result<void> init(const GPUContext& gpu) noexcept override {
        if (_initialized) return Ok();

        _gpu = gpu;

        if (auto res = initMsMsdfFonts(); !res) {
            return Err<void>("Failed to initialize MSDF fonts", res);
        }

        if (auto res = initBmFont(gpu); !res) {
            ywarn("Failed to initialize BmFont: {} - emoji rendering disabled",
                  res.error().message());
        }

        if (auto res = initShaderFonts(); !res) {
            ywarn("Failed to initialize ShaderFonts: {} - shader glyphs disabled",
                  res.error().message());
        }

        _initialized = true;
        yinfo("YettyFontManager initialized");
        return Ok();
    }

    Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string& fontName) noexcept override {
        auto it = _msdfFontCache.find(fontName);
        if (it != _msdfFontCache.end()) {
            return Ok(it->second);
        }

        std::string cdbBasePath = _cacheDir + "/" + fontName;
        auto result = MsMsdfFont::create(cdbBasePath);
        if (!result) {
            return Err<MsMsdfFont::Ptr>("Failed to load MsMsdfFont: " + fontName, result);
        }

        auto font = std::move(*result);

        // Create GPU resources (texture, sampler, metadata buffer)
        if (auto res = font->createTexture(_gpu.device, _gpu.queue); !res) {
            return Err<MsMsdfFont::Ptr>("Failed to create MsMsdfFont texture", res);
        }
        if (auto res = font->createGlyphMetadataBuffer(_gpu.device); !res) {
            return Err<MsMsdfFont::Ptr>("Failed to create MsMsdfFont metadata buffer", res);
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

    BmFont::Ptr getDefaultBmFont() noexcept override {
        return _bitmapFont;
    }

    ShaderFont::Ptr getDefaultShaderGlyphFont() noexcept override {
        return _shaderGlyphFont;
    }

    ShaderFont::Ptr getDefaultCardFont() noexcept override {
        return _cardFont;
    }

    void setDefaultFont(const std::string& fontName) noexcept override {
        _defaultFontName = fontName;
    }

    bool hasFont(const std::string& fontName) const noexcept override {
        return _msdfFontCache.find(fontName) != _msdfFontCache.end();
    }

    const std::string& getCacheDir() const noexcept override {
        return _cacheDir;
    }

private:
    Result<void> initMsMsdfFonts() noexcept {
        const char* home = std::getenv("HOME");
        if (!home) {
            return Err<void>("HOME environment variable not set");
        }

        _cacheDir = std::string(home) + "/.cache/yetty/msdf-font-cache";

        if (!std::filesystem::exists(_cacheDir)) {
            ywarn("MSDF font cache directory does not exist: {}", _cacheDir);
        }

        yinfo("MSDF font cache dir: {}", _cacheDir);
        return Ok();
    }

    Result<void> initBmFont(const GPUContext& gpu) noexcept {
        auto bmFontResult = BmFont::create(gpu.device);
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
        std::string shaderDir = std::string(CMAKE_SOURCE_DIR) + "/src/yetty/shaders/";

        auto shaderGlyphResult = ShaderFont::create(ShaderFont::Category::Glyph, shaderDir);
        if (!shaderGlyphResult) {
            ywarn("Failed to create ShaderFont for glyphs: {}", shaderGlyphResult.error().message());
        } else {
            _shaderGlyphFont = *shaderGlyphResult;
            yinfo("ShaderFont for glyphs created with {} shaders",
                  _shaderGlyphFont->getFunctionCount());
        }

        auto cardFontResult = ShaderFont::create(ShaderFont::Category::Card, shaderDir);
        if (!cardFontResult) {
            ywarn("Failed to create ShaderFont for cards: {}", cardFontResult.error().message());
        } else {
            _cardFont = *cardFontResult;
            yinfo("ShaderFont for cards created with {} shaders",
                  _cardFont->getFunctionCount());
        }

        return Ok();
    }

    GPUContext _gpu = {};
    std::string _cacheDir;
    std::unordered_map<std::string, MsMsdfFont::Ptr> _msdfFontCache;
    std::string _defaultFontName;
    BmFont::Ptr _bitmapFont;
    ShaderFont::Ptr _shaderGlyphFont;
    ShaderFont::Ptr _cardFont;
    bool _initialized = false;
};

// Factory for ThreadSingleton
YettyFontManager::Ptr YettyFontManager::createImpl() noexcept {
    return std::make_shared<YettyFontManagerImpl>();
}

} // namespace yetty
