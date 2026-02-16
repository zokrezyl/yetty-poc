#pragma once

#include <yetty/result.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

typedef struct FT_FaceRec_* FT_Face;

namespace yetty::font {

/// Lightweight FreeType-based font metrics utility.
/// Uses the thread-local FT_Library from freetype.h; manages its own FT_Face handles.
class FontUtil {
public:
    FontUtil() = default;
    ~FontUtil();

    FontUtil(const FontUtil&) = delete;
    FontUtil& operator=(const FontUtil&) = delete;

    /// Load a font from raw TTF/OTF data. Returns a font id.
    Result<int> addFontData(const uint8_t* data, size_t size, const std::string& name);

    /// Text measurement using FreeType glyph advances.
    float measureTextWidth(const std::string& text, float fontSize, int fontId);
    float fontAscent(float fontSize, int fontId);
    float fontDescent(float fontSize, int fontId);

private:
    FT_Face findFace(int fontId) const;

    struct FontData {
        int fontId;
        std::string name;
        std::vector<uint8_t> ttfData;
        FT_Face face = nullptr;
    };
    std::vector<FontData> _fonts;
    int _nextFontId = 0;
};

} // namespace yetty::font
