#include <yetty/font-manager.h>
#include <yetty/webgpu-context.h>
#include <spdlog/spdlog.h>

#if !YETTY_USE_PREBUILT_ATLAS
#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fontconfig.h>
#endif

namespace yetty {

Result<FontManager::Ptr> FontManager::create(WebGPUContext::Ptr ctx) noexcept {
    if (!ctx) {
        return Err<Ptr>("FontManager::create: null WebGPUContext");
    }
    auto mgr = Ptr(new FontManager(std::move(ctx)));
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("Failed to initialize FontManager", res);
    }
    return Ok(std::move(mgr));
}

FontManager::FontManager(WebGPUContext::Ptr ctx) noexcept
    : ctx_(std::move(ctx)) {}

FontManager::~FontManager() {
    unloadAll();
}

Result<void> FontManager::init() noexcept {
    // Nothing to initialize currently
    return Ok();
}

FontCacheKey FontManager::createCacheKey(FT_Face face) noexcept {
    FontCacheKey key;
#if !YETTY_USE_PREBUILT_ATLAS
    key.familyName = face->family_name ? face->family_name : "";
    key.styleName = face->style_name ? face->style_name : "";
    // Use actual font properties to distinguish embedded fonts with empty/same names
    key.numGlyphs = face->num_glyphs;
    key.unitsPerEM = face->units_per_EM;
#endif
    return key;
}

FontCacheKey FontManager::createCacheKey(const std::string& family, Font::Style style) noexcept {
    FontCacheKey key;
    key.familyName = family;

    switch (style) {
        case Font::Bold:
            key.styleName = "Bold";
            break;
        case Font::Italic:
            key.styleName = "Italic";
            break;
        case Font::BoldItalic:
            key.styleName = "Bold Italic";
            break;
        case Font::Regular:
        default:
            key.styleName = "Regular";
            break;
    }

    key.numGlyphs = 0;
    key.unitsPerEM = 0;

    return key;
}

std::string FontManager::findFontPath(const std::string& family, Font::Style style) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    FcConfig* config = FcInitLoadConfigAndFonts();
    if (!config) {
        spdlog::error("FontManager: Failed to initialize fontconfig");
        return "";
    }

    FcPattern* pattern = FcPatternCreate();
    if (!pattern) {
        FcConfigDestroy(config);
        return "";
    }

    FcPatternAddString(pattern, FC_FAMILY, reinterpret_cast<const FcChar8*>(family.c_str()));

    switch (style) {
        case Font::Bold:
            FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ROMAN);
            break;
        case Font::Italic:
            FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_REGULAR);
            FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
            break;
        case Font::BoldItalic:
            FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);
            FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
            break;
        case Font::Regular:
        default:
            FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_REGULAR);
            FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ROMAN);
            break;
    }

    FcPatternAddBool(pattern, FC_OUTLINE, FcTrue);
    FcPatternAddBool(pattern, FC_SCALABLE, FcTrue);

    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult result;
    FcPattern* match = FcFontMatch(config, pattern, &result);

    std::string fontPath;
    if (match) {
        FcChar8* file = nullptr;
        if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch && file) {
            fontPath = reinterpret_cast<const char*>(file);
        }
        FcPatternDestroy(match);
    }

    FcPatternDestroy(pattern);
    FcConfigDestroy(config);

    if (!fontPath.empty()) {
        spdlog::debug("FontManager: Found font '{}' {} at: {}",
                      family,
                      style == Font::Bold ? "Bold" :
                      style == Font::Italic ? "Italic" :
                      style == Font::BoldItalic ? "BoldItalic" : "Regular",
                      fontPath);
    } else {
        spdlog::warn("FontManager: Could not find font '{}' with style {}", family, static_cast<int>(style));
    }

    return fontPath;
#else
    spdlog::warn("FontManager: findFontPath not available on this platform");
    return "";
#endif
}

Result<std::unique_ptr<Font>> FontManager::generateFont(const std::string& path, float fontSize) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    auto font = std::make_unique<Font>();

    if (!font->generate(path, fontSize)) {
        return Err<std::unique_ptr<Font>>("Failed to generate font atlas from: " + path);
    }

    if (!font->createTexture(ctx_->getDevice(), ctx_->getQueue())) {
        return Err<std::unique_ptr<Font>>("Failed to create font texture");
    }

    if (!font->createGlyphMetadataBuffer(ctx_->getDevice())) {
        return Err<std::unique_ptr<Font>>("Failed to create glyph metadata buffer");
    }

    return Ok(std::move(font));
#else
    return Err<std::unique_ptr<Font>>("Font generation not available on this platform");
#endif
}

Result<std::unique_ptr<Font>> FontManager::generateFont(FT_Face face, const std::string& fontName, float fontSize) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    auto font = std::make_unique<Font>();

    if (!font->generate(face, fontName, fontSize)) {
        return Err<std::unique_ptr<Font>>("Failed to generate font atlas from FT_Face");
    }

    if (!font->createTexture(ctx_->getDevice(), ctx_->getQueue())) {
        return Err<std::unique_ptr<Font>>("Failed to create font texture for embedded font");
    }

    if (!font->createGlyphMetadataBuffer(ctx_->getDevice())) {
        return Err<std::unique_ptr<Font>>("Failed to create glyph metadata buffer for embedded font");
    }

    return Ok(std::move(font));
#else
    return Err<std::unique_ptr<Font>>("Font generation not available on this platform");
#endif
}

Result<Font*> FontManager::getFont(const std::string& family, Font::Style style, float fontSize) noexcept {
    FontCacheKey key = createCacheKey(family, style);

    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        spdlog::debug("FontManager: cache hit for '{}' {}", family, static_cast<int>(style));
        return Ok(it->second.get());
    }

    std::string fontPath = findFontPath(family, style);
    if (fontPath.empty()) {
        return Err<Font*>("Could not find font: " + family);
    }

    auto result = generateFont(fontPath, fontSize);
    if (!result) {
        return Err<Font*>(result.error().message());
    }

    Font* ptr = result.value().get();
    fontCache_[key] = std::move(result.value());

    if (!hasDefaultFont_) {
        defaultFontKey_ = key;
        hasDefaultFont_ = true;
    }

    spdlog::info("FontManager: loaded font '{}' {} from {}", family, static_cast<int>(style), fontPath);
    return Ok(ptr);
}

Result<Font*> FontManager::getFont(FT_Face face, const std::string& fontName, float fontSize) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    if (!face) {
        return Err<Font*>("Invalid FreeType face");
    }

    // Use the provided fontName for caching (MuPDF font names are unique)
    FontCacheKey key;
    key.familyName = fontName;
    key.styleName = "";
    key.numGlyphs = 0;
    key.unitsPerEM = 0;

    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        spdlog::debug("FontManager: cache hit for FT_Face '{}'", fontName);
        return Ok(it->second.get());
    }

    spdlog::info("FontManager: cache miss for FT_Face '{}', generating font", fontName);

    auto result = generateFont(face, fontName, fontSize);
    if (!result) {
        return Err<Font*>(result.error().message());
    }

    Font* ptr = result.value().get();
    fontCache_[key] = std::move(result.value());

    if (!hasDefaultFont_) {
        defaultFontKey_ = key;
        hasDefaultFont_ = true;
    }

    return Ok(ptr);
#else
    return Err<Font*>("FreeType font loading not available on this platform");
#endif
}

Result<std::unique_ptr<Font>> FontManager::generateFont(const unsigned char* data, size_t dataLen,
                                                         const std::string& fontName, float fontSize) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    auto font = std::make_unique<Font>();

    if (!font->generate(data, dataLen, fontName, fontSize)) {
        return Err<std::unique_ptr<Font>>("Failed to generate font atlas from font data");
    }

    if (!font->createTexture(ctx_->getDevice(), ctx_->getQueue())) {
        return Err<std::unique_ptr<Font>>("Failed to create font texture for embedded font");
    }

    if (!font->createGlyphMetadataBuffer(ctx_->getDevice())) {
        return Err<std::unique_ptr<Font>>("Failed to create glyph metadata buffer for embedded font");
    }

    return Ok(std::move(font));
#else
    return Err<std::unique_ptr<Font>>("Font generation not available on this platform");
#endif
}

Result<Font*> FontManager::getFont(const unsigned char* data, size_t dataLen,
                                    const std::string& fontName, float fontSize) noexcept {
#if !YETTY_USE_PREBUILT_ATLAS
    if (!data || dataLen == 0) {
        return Err<Font*>("Invalid font data");
    }

    // Use the provided fontName for caching
    FontCacheKey key;
    key.familyName = fontName;
    key.styleName = "";
    key.numGlyphs = 0;
    key.unitsPerEM = 0;

    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        spdlog::debug("FontManager: cache hit for font data '{}'", fontName);
        return Ok(it->second.get());
    }

    spdlog::info("FontManager: cache miss for font data '{}', generating font", fontName);

    auto result = generateFont(data, dataLen, fontName, fontSize);
    if (!result) {
        return Err<Font*>(result.error().message());
    }

    Font* ptr = result.value().get();
    fontCache_[key] = std::move(result.value());

    if (!hasDefaultFont_) {
        defaultFontKey_ = key;
        hasDefaultFont_ = true;
    }

    return Ok(ptr);
#else
    return Err<Font*>("Font loading from data not available on this platform");
#endif
}

Font* FontManager::getDefaultFont() noexcept {
    if (!hasDefaultFont_) {
        return nullptr;
    }

    auto it = fontCache_.find(defaultFontKey_);
    if (it != fontCache_.end()) {
        return it->second.get();
    }

    return nullptr;
}

void FontManager::setDefaultFont(const std::string& family, Font::Style style) noexcept {
    FontCacheKey key = createCacheKey(family, style);

    if (fontCache_.find(key) != fontCache_.end()) {
        defaultFontKey_ = key;
        hasDefaultFont_ = true;
    }
}

bool FontManager::hasFont(const std::string& family, Font::Style style) const noexcept {
    FontCacheKey key = createCacheKey(family, style);
    return fontCache_.find(key) != fontCache_.end();
}


void FontManager::unloadFont(const std::string& family, Font::Style style) noexcept {
    FontCacheKey key = createCacheKey(family, style);

    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        if (hasDefaultFont_ && key == defaultFontKey_) {
            hasDefaultFont_ = false;
            for (const auto& [k, v] : fontCache_) {
                if (!(k == key)) {
                    defaultFontKey_ = k;
                    hasDefaultFont_ = true;
                    break;
                }
            }
        }

        fontCache_.erase(it);
        spdlog::info("FontManager: unloaded font '{}' {}", family, static_cast<int>(style));
    }
}

void FontManager::unloadAll() noexcept {
    fontCache_.clear();
    hasDefaultFont_ = false;
    spdlog::debug("FontManager: unloaded all fonts");
}

} // namespace yetty
