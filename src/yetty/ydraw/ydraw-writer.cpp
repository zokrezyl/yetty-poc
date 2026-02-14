#include <yetty/ydraw-writer.h>
#include <yetty/ydraw-builder.h>
#include <yetty/font-manager.h>
#include <yetty/gpu-allocator.h>
#include "../cards/hdraw/hdraw.h" // For SDFPrimitive, SDFType
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace yetty {

//=============================================================================
// YDrawWriterInternalImpl — wraps YDrawBuilder
//=============================================================================

class YDrawWriterInternalImpl : public YDrawWriterInternal {
public:
    YDrawWriterInternalImpl(YDrawBuilder::Ptr builder)
        : _builder(std::move(builder))
    {}

    const char* typeName() const override { return "YDrawWriterInternal"; }

    YDrawBuilder::Ptr builder() const override { return _builder; }

    //=========================================================================
    // Font management
    //=========================================================================

    Result<int> addFont(const std::string& ttfPath) override {
        return _builder->addFont(ttfPath);
    }

    Result<int> addFontData(const uint8_t* data, size_t size,
                             const std::string& name) override {
        return _builder->addFontData(data, size, name);
    }

    //=========================================================================
    // Text — glyph placement with CDB atlas metrics
    //=========================================================================

    float addText(float x, float y, const std::string& text,
                  float fontSize, uint32_t color, int fontId) override {
        return addText(x, y, text, fontSize, color, 0u, fontId);
    }

    float addText(float x, float y, const std::string& text,
                  float fontSize, uint32_t color,
                  uint32_t layer, int fontId) override {
        if (text.empty()) return 0.0f;

        // If we have a custom atlas and fontId, use per-glyph placement
        if (_builder->hasCustomAtlas()) {
            return placeGlyphsCustom(x, y, text, fontSize, color, fontId, layer);
        }

        // Fallback: use builder's default addText (no advance returned)
        _builder->addText(x, y, text, fontSize, color, layer, fontId);
        return _builder->measureTextWidth(text, fontSize, fontId);
    }

    float measureText(const std::string& text,
                      float fontSize, int fontId) override {
        if (text.empty()) return 0.0f;

        if (_builder->hasCustomAtlas()) {
            return measureGlyphsCustom(text, fontSize, fontId);
        }

        // Fallback to builder's measure
        return _builder->measureTextWidth(text, fontSize, fontId);
    }

    //=========================================================================
    // Font metrics — delegate to builder
    //=========================================================================

    float fontAscent(float fontSize, int fontId) override {
        return _builder->fontAscent(fontSize, fontId);
    }

    float fontDescent(float fontSize, int fontId) override {
        return _builder->fontDescent(fontSize, fontId);
    }

    //=========================================================================
    // Primitives — delegate to builder
    //=========================================================================

    void addBox(float cx, float cy, float halfW, float halfH,
                uint32_t fillColor, uint32_t strokeColor,
                float strokeWidth, float round, uint32_t layer) override {
        _builder->addBox(cx, cy, halfW, halfH,
                         fillColor, strokeColor, strokeWidth, round, layer);
    }

    void addRoundedBox(float cx, float cy, float halfW, float halfH,
                       float radius, uint32_t fillColor,
                       uint32_t strokeColor, float strokeWidth) override {
        // YDrawBuilder::addBox has 'round' parameter for corner radius
        _builder->addBox(cx, cy, halfW, halfH, fillColor,
                         strokeColor, strokeWidth, radius);
    }

    void addTriangle(float x0, float y0, float x1, float y1,
                     float x2, float y2, uint32_t fillColor,
                     uint32_t strokeColor, float strokeWidth) override {
        // Create triangle primitive directly
        card::SDFPrimitive p = {};
        p.type = static_cast<uint32_t>(card::SDFType::Triangle);
        p.layer = _builder->primitiveCount();
        p.params[0] = x0;
        p.params[1] = y0;
        p.params[2] = x1;
        p.params[3] = y1;
        p.params[4] = x2;
        p.params[5] = y2;
        p.fillColor = fillColor;
        p.strokeColor = strokeColor;
        p.strokeWidth = strokeWidth;
        p.round = 0;
        p.aabbMinX = std::min({x0, x1, x2});
        p.aabbMinY = std::min({y0, y1, y2});
        p.aabbMaxX = std::max({x0, x1, x2});
        p.aabbMaxY = std::max({y0, y1, y2});
        _builder->addPrimitive(p);
    }

    void addCircle(float cx, float cy, float radius,
                   uint32_t fillColor, uint32_t strokeColor,
                   float strokeWidth) override {
        _builder->addCircle(cx, cy, radius, fillColor, strokeColor, strokeWidth);
    }

    void addSegment(float x0, float y0, float x1, float y1,
                    uint32_t strokeColor, float strokeWidth,
                    uint32_t layer) override {
        _builder->addSegment(x0, y0, x1, y1, strokeColor, strokeWidth, layer);
    }

    void addCircle(float cx, float cy, float radius,
                   uint32_t fillColor, uint32_t strokeColor,
                   float strokeWidth, uint32_t layer) override {
        _builder->addCircle(cx, cy, radius,
                            fillColor, strokeColor, strokeWidth, layer);
    }

    void addColorWheel(float cx, float cy, float outerR, float innerR,
                       float hue, float sat, float val,
                       float selectorRadius) override {
        _builder->addColorWheel(cx, cy, outerR, innerR, hue, sat, val,
                                selectorRadius, _builder->primitiveCount());
    }

    //=========================================================================
    // Scene — delegate to builder
    //=========================================================================

    void clear() override {
        _builder->clear();
    }

    void setSceneBounds(float minX, float minY,
                        float maxX, float maxY) override {
        _builder->setSceneBounds(minX, minY, maxX, maxY);
    }

    void setBgColor(uint32_t color) override {
        _builder->setBgColor(color);
    }

    void setFlags(uint32_t flags) override {
        _builder->setFlags(flags);
    }

    void addFlags(uint32_t flags) override {
        _builder->addFlags(flags);
    }

    //=========================================================================
    // Rotated text — each glyph becomes an SDF primitive
    //=========================================================================

    float addRotatedText(float x, float y, const std::string& text,
                          float fontSize, uint32_t color,
                          float rotationRadians, int fontId) override {
        if (text.empty()) return 0.0f;

        auto atlas = _builder->customAtlas();
        if (!atlas) return 0.0f;

        float fontBaseSize = atlas->getFontSize();
        if (fontBaseSize <= 0.0f) fontBaseSize = 1.0f;
        float scale = fontSize / fontBaseSize;

        float cosA = std::cos(rotationRadians);
        float sinA = std::sin(rotationRadians);
        float cursorAdvance = 0.0f;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = decodeUtf8(ptr, end);
            if (codepoint == 0 && ptr >= end) break;

            uint32_t glyphIndex = atlas->loadGlyph(fontId, codepoint);
            const auto& metadata = atlas->getGlyphMetadata();
            if (glyphIndex >= metadata.size()) {
                cursorAdvance += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];
            float bx = glyph._bearingX * scale;
            float by = -glyph._bearingY * scale;
            float gw = glyph._sizeX * scale;
            float gh = glyph._sizeY * scale;

            // Glyph origin: advance along rotated direction + bearing
            float gx = x + (cursorAdvance + bx) * cosA - by * sinA;
            float gy = y + (cursorAdvance + bx) * sinA + by * cosA;

            _builder->addRotatedGlyph(gx, gy, gw, gh,
                                       rotationRadians, glyphIndex, color);

            cursorAdvance += glyph._advance * scale;
        }

        return cursorAdvance;
    }

private:
    //=========================================================================
    // Per-glyph placement using custom atlas CDB metrics
    // (Ported from YPdf::placeGlyphs)
    //=========================================================================

    float placeGlyphsCustom(float posX, float posY,
                             const std::string& text,
                             float fontSize, uint32_t color,
                             int fontId, uint32_t layer = 0) {
        auto atlas = _builder->customAtlas();
        if (!atlas) return 0.0f;

        float fontBaseSize = atlas->getFontSize();
        if (fontBaseSize <= 0.0f) fontBaseSize = 1.0f;

        float scale = fontSize / fontBaseSize;
        float cursorX = posX;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = decodeUtf8(ptr, end);
            if (codepoint == 0 && ptr >= end) break;

            uint32_t glyphIndex = atlas->loadGlyph(fontId, codepoint);
            const auto& metadata = atlas->getGlyphMetadata();

            if (glyphIndex >= metadata.size()) {
                // Fallback advance for missing glyphs
                cursorX += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];

            YDrawGlyph g = {};
            g.x = cursorX + glyph._bearingX * scale;
            g.y = posY - glyph._bearingY * scale;
            g.setSize(glyph._sizeX * scale, glyph._sizeY * scale);
            g.setGlyphLayerFlags(static_cast<uint16_t>(glyphIndex),
                                 static_cast<uint8_t>(layer),
                                 YDrawBuilder::GLYPH_FLAG_CUSTOM_ATLAS);
            g.color = color;
            _builder->glyphsMut().push_back(g);

            cursorX += glyph._advance * scale;
        }

        return cursorX - posX;
    }

    float measureGlyphsCustom(const std::string& text,
                               float fontSize, int fontId) {
        auto atlas = _builder->customAtlas();
        if (!atlas) return 0.0f;

        float fontBaseSize = atlas->getFontSize();
        if (fontBaseSize <= 0.0f) fontBaseSize = 1.0f;

        float scale = fontSize / fontBaseSize;
        float width = 0.0f;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = decodeUtf8(ptr, end);
            if (codepoint == 0 && ptr >= end) break;

            uint32_t glyphIndex = atlas->loadGlyph(fontId, codepoint);
            const auto& metadata = atlas->getGlyphMetadata();

            if (glyphIndex >= metadata.size()) {
                width += fontSize * 0.5f;
                continue;
            }

            width += metadata[glyphIndex]._advance * scale;
        }

        return width;
    }

    static uint32_t decodeUtf8(const uint8_t*& ptr, const uint8_t* end) {
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
        }
        return cp;
    }

    YDrawBuilder::Ptr _builder;
};

//=============================================================================
// Factory
//=============================================================================

Result<YDrawWriterInternal::Ptr> YDrawWriterInternal::createImpl(
    FontManager::Ptr fontManager,
    GpuAllocator::Ptr allocator)
{
    auto builderRes = YDrawBuilder::create(std::move(fontManager),
                                           std::move(allocator));
    if (!builderRes) {
        return Err<Ptr>("YDrawWriterInternal::create: "
                        "failed to create builder", builderRes);
    }

    auto builder = *builderRes;
    builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE |
                      YDrawBuilder::FLAG_CUSTOM_ATLAS);

    return Ok<Ptr>(Ptr(new YDrawWriterInternalImpl(std::move(builder))));
}

} // namespace yetty
