#include <yetty/font/raw-font.h>
#include <yetty/font/freetype.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <vector>

namespace yetty::font {

class RawFontImpl : public RawFont {
public:
    RawFontImpl(std::vector<uint8_t> data, std::string name)
        : _data(std::move(data)), _name(std::move(name)) {}

    ~RawFontImpl() override {
        if (_face) FT_Done_Face(_face);
    }

    Result<void> init() {
        FT_Error err = FT_New_Memory_Face(ftLibrary(),
                                          _data.data(),
                                          static_cast<FT_Long>(_data.size()),
                                          0, &_face);
        if (err) {
            return Err("RawFont: FreeType error " + std::to_string(err));
        }

        // Nominal size for metric queries (scaled at query time)
        FT_Set_Char_Size(_face, 0, 32 * 64, 72, 72);
        return Ok();
    }

    const std::string& name() const override { return _name; }

    float measureTextWidth(const std::string& text, float fontSize) override {
        if (!_face || text.empty()) return 0.0f;

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

            // Check cache first
            auto it = _advanceCache.find(cp);
            if (it != _advanceCache.end()) {
                width += it->second * scale;
                continue;
            }

            // Load glyph with NO_HINTING to skip expensive TT bytecode interpreter
            FT_UInt glyphIndex = FT_Get_Char_Index(_face, cp);
            if (FT_Load_Glyph(_face, glyphIndex, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING) == 0) {
                float advance = _face->glyph->advance.x / 64.0f;
                _advanceCache[cp] = advance;
                width += advance * scale;
            } else {
                _advanceCache[cp] = 16.0f;  // fallback
                width += fontSize * 0.5f;
            }
        }

        return width;
    }

    float fontAscent(float fontSize) override {
        if (!_face) return fontSize * 0.8f;
        float scale = fontSize / 32.0f;
        return (_face->size->metrics.ascender / 64.0f) * scale;
    }

    float fontDescent(float fontSize) override {
        if (!_face) return fontSize * 0.2f;
        float scale = fontSize / 32.0f;
        return (-_face->size->metrics.descender / 64.0f) * scale;
    }

private:
    std::vector<uint8_t> _data;
    std::string _name;
    FT_Face _face = nullptr;
    mutable std::unordered_map<uint32_t, float> _advanceCache;
};

Result<RawFont::Ptr> RawFont::createImpl(const uint8_t* data, size_t size,
                                          const std::string& name) {
    std::vector<uint8_t> fontData(data, data + size);
    auto impl = Ptr(new RawFontImpl(std::move(fontData), name));
    auto res = static_cast<RawFontImpl*>(impl.get())->init();
    if (!res) return Err<Ptr>("RawFont creation failed", res);
    return Ok(std::move(impl));
}

} // namespace yetty::font
