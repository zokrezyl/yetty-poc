#include <yetty/yetty-font-manager.h>
#include <ytrace/ytrace.hpp>

#include <cstdlib>
#include <filesystem>

namespace yetty {

YettyFontManager::YettyFontManager() noexcept = default;

YettyFontManager::~YettyFontManager() = default;

Result<YettyFontManager::Ptr> YettyFontManager::create() noexcept {
    auto mgr = Ptr(new YettyFontManager());
    auto initResult = mgr->init();
    if (!initResult) {
        return Err<Ptr>("Failed to initialize YettyFontManager", initResult);
    }
    return Ok(std::move(mgr));
}

Result<void> YettyFontManager::init() noexcept {
    // Determine cache directory: ~/.cache/yetty/msdf-font-cache
    const char* home = std::getenv("HOME");
    if (!home) {
        return Err<void>("HOME environment variable not set");
    }

    _cacheDir = std::string(home) + "/.cache/yetty/msdf-font-cache";

    // Check if cache directory exists
    if (!std::filesystem::exists(_cacheDir)) {
        ywarn("MSDF font cache directory does not exist: {}", _cacheDir);
        // Don't fail - fonts can be loaded later if directory is created
    }

    yinfo("YettyFontManager initialized, cache dir: {}", _cacheDir);
    return Ok();
}

Result<MsMsdfFont::Ptr> YettyFontManager::getMsMsdfFont(const std::string& fontName) noexcept {
    // Check if already cached
    auto it = _fontCache.find(fontName);
    if (it != _fontCache.end()) {
        return Ok(it->second);
    }

    // Build CDB base path
    std::string cdbBasePath = _cacheDir + "/" + fontName;

    // Try to create the font
    auto result = MsMsdfFont::create(cdbBasePath);
    if (!result) {
        return Err<MsMsdfFont::Ptr>("Failed to load MsMsdfFont: " + fontName, result);
    }

    // Cache and return
    auto font = std::move(*result);
    _fontCache[fontName] = font;

    // Set as default if this is the first font
    if (_defaultFontName.empty()) {
        _defaultFontName = fontName;
    }

    yinfo("Loaded MsMsdfFont: {}", fontName);
    return Ok(font);
}

MsMsdfFont::Ptr YettyFontManager::getDefaultFont() noexcept {
    // Return cached default if available
    if (!_defaultFontName.empty()) {
        auto it = _fontCache.find(_defaultFontName);
        if (it != _fontCache.end()) {
            return it->second;
        }
    }

    // Try to load default font (base family name, not with -Regular suffix)
    auto result = getMsMsdfFont("DejaVuSansMNerdFontMono");
    if (result) {
        return *result;
    }

    // Return first cached font if available
    if (!_fontCache.empty()) {
        return _fontCache.begin()->second;
    }

    yerror("No fonts available");
    return nullptr;
}

void YettyFontManager::setDefaultFont(const std::string& fontName) noexcept {
    _defaultFontName = fontName;
}

bool YettyFontManager::hasFont(const std::string& fontName) const noexcept {
    return _fontCache.find(fontName) != _fontCache.end();
}

} // namespace yetty
