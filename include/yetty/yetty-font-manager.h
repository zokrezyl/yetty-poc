#pragma once

#include <yetty/ms-msdf-font.h>
#include <yetty/result.hpp>
#include <string>
#include <memory>
#include <unordered_map>

namespace yetty {

/**
 * @brief Font manager for MsMsdfFont instances.
 *
 * Manages loading and caching of MSDF fonts from CDB files stored in
 * ~/.cache/yetty/msdf-font-cache/
 */
class YettyFontManager {
public:
    using Ptr = std::shared_ptr<YettyFontManager>;

    /**
     * @brief Create a new YettyFontManager instance.
     */
    static Result<Ptr> create() noexcept;

    ~YettyFontManager();

    // Non-copyable
    YettyFontManager(const YettyFontManager&) = delete;
    YettyFontManager& operator=(const YettyFontManager&) = delete;

    /**
     * @brief Get an MsMsdfFont by font name.
     *
     * Looks for CDB files at: ~/.cache/yetty/msdf-font-cache/<fontName>-regular.cdb
     *
     * @param fontName Base name of the font (e.g., "DejaVuSansMNerdFontMono-Regular")
     * @return Shared pointer to MsMsdfFont, or error if not found
     */
    Result<MsMsdfFont::Ptr> getMsMsdfFont(const std::string& fontName) noexcept;

    /**
     * @brief Get the default MSDF font.
     *
     * Returns the first successfully loaded font, or attempts to load
     * "DejaVuSansMNerdFontMono-Regular" if no fonts are loaded.
     */
    std::shared_ptr<MsMsdfFont> getDefaultFont() noexcept;

    /**
     * @brief Set which font should be the default.
     */
    void setDefaultFont(const std::string& fontName) noexcept;

    /**
     * @brief Check if a font is already loaded.
     */
    bool hasFont(const std::string& fontName) const noexcept;

    /**
     * @brief Get the cache directory path.
     */
    const std::string& getCacheDir() const noexcept { return _cacheDir; }

    /**
     * @brief Get number of cached fonts.
     */
    size_t getCacheSize() const noexcept { return _fontCache.size(); }

private:
    YettyFontManager() noexcept;
    Result<void> init() noexcept;

    std::string _cacheDir;
    std::unordered_map<std::string, std::shared_ptr<MsMsdfFont>> _fontCache;
    std::string _defaultFontName;
};

} // namespace yetty
