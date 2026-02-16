#include "util.h"
#include <yetty/font/freetype.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace yetty::font {

FontUtil::~FontUtil() {
    for (auto& fd : _fonts) {
        if (fd.face) FT_Done_Face(fd.face);
    }
}

FT_Face FontUtil::findFace(int fontId) const {
    for (const auto& fd : _fonts) {
        if (fd.fontId == fontId) return fd.face;
    }
    return nullptr;
}

Result<int> FontUtil::addFontData(const uint8_t* data, size_t size,
                                   const std::string& name) {
    int id = _nextFontId++;

    FontData fd;
    fd.fontId = id;
    fd.name = name;
    fd.ttfData.assign(data, data + size);

    FT_Error err = FT_New_Memory_Face(ftLibrary(),
                                       fd.ttfData.data(),
                                       static_cast<FT_Long>(fd.ttfData.size()),
                                       0, &fd.face);
    if (err) {
        return Err<int>("FontUtil::addFontData: FreeType error " + std::to_string(err));
    }

    // Nominal size for metric queries (scaled at query time)
    FT_Set_Char_Size(fd.face, 0, 32 * 64, 72, 72);

    _fonts.push_back(std::move(fd));
    return Ok(id);
}

float FontUtil::measureTextWidth(const std::string& text,
                                  float fontSize, int fontId) {
    FT_Face face = findFace(fontId);
    if (!face || text.empty()) return 0.0f;

    float scale = fontSize / 32.0f;
    float width = 0.0f;

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
    const uint8_t* end = ptr + text.size();

    while (ptr < end) {
        uint32_t cp = 0;
        if ((*ptr & 0x80) == 0) {
            cp = *ptr++;
        } else if ((*ptr & 0xE0) == 0xC0) {
            cp = (*ptr++ & 0x1F) << 6;
            if (ptr < end) cp |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF0) == 0xE0) {
            cp = (*ptr++ & 0x0F) << 12;
            if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) cp |= (*ptr++ & 0x3F);
        } else if ((*ptr & 0xF8) == 0xF0) {
            cp = (*ptr++ & 0x07) << 18;
            if (ptr < end) cp |= (*ptr++ & 0x3F) << 12;
            if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
            if (ptr < end) cp |= (*ptr++ & 0x3F);
        } else {
            ptr++;
            continue;
        }

        FT_UInt glyphIndex = FT_Get_Char_Index(face, cp);
        if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_BITMAP) == 0) {
            width += (face->glyph->advance.x / 64.0f) * scale;
        } else {
            width += fontSize * 0.5f;
        }
    }

    return width;
}

float FontUtil::fontAscent(float fontSize, int fontId) {
    FT_Face face = findFace(fontId);
    if (!face) return fontSize * 0.8f;
    float scale = fontSize / 32.0f;
    return (face->size->metrics.ascender / 64.0f) * scale;
}

float FontUtil::fontDescent(float fontSize, int fontId) {
    FT_Face face = findFace(fontId);
    if (!face) return fontSize * 0.2f;
    float scale = fontSize / 32.0f;
    return (-face->size->metrics.descender / 64.0f) * scale;
}

} // namespace yetty::font
