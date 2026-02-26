// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <yetty/ydraw-builder.h>
#include <yetty/card-texture-manager.h>
#include "ydraw-types.gen.h"
#include "ydraw-buffer.h"
#include <yetty/msdf-glyph-data.h>  // For GlyphMetadataGPU
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <numeric>
#include <unordered_map>

namespace yetty {

//=============================================================================
// Read font ascender/descender from TTF hhea + head tables (no FreeType)
//=============================================================================
struct TtfMetrics { float ascender; float descender; };

static TtfMetrics readTtfMetrics(const std::string& path, float fontSize) {
    TtfMetrics fallback{fontSize * 0.8f, fontSize * 0.2f};
    std::ifstream f(path, std::ios::binary);
    if (!f) return fallback;

    auto r16 = [&](uint32_t off) -> uint16_t {
        char b[2];
        f.seekg(off);
        f.read(b, 2);
        return (uint8_t(b[0]) << 8) | uint8_t(b[1]);
    };
    auto ri16 = [&](uint32_t off) -> int16_t {
        return static_cast<int16_t>(r16(off));
    };

    uint16_t numTables = r16(4);
    uint32_t hheaOff = 0, headOff = 0;
    for (uint16_t i = 0; i < numTables; i++) {
        uint32_t entry = 12 + i * 16;
        char tag[4];
        f.seekg(entry);
        f.read(tag, 4);
        uint32_t off = (static_cast<uint32_t>(r16(entry + 8)) << 16) | r16(entry + 10);
        if (std::memcmp(tag, "hhea", 4) == 0) hheaOff = off;
        if (std::memcmp(tag, "head", 4) == 0) headOff = off;
    }
    if (!hheaOff || !headOff) return fallback;

    uint16_t upm = r16(headOff + 18);
    int16_t asc = ri16(hheaOff + 4);
    int16_t desc = ri16(hheaOff + 6);
    if (upm == 0) return fallback;

    float scale = fontSize / static_cast<float>(upm);
    return {asc * scale, std::abs(desc) * scale};
}

using card::SDFType;

//=============================================================================
// AABB utilities
//=============================================================================

// Compute AABB directly from variable-length buffer words.
// Layout: word[0]=type, word[1]=layer, word[2..wc-5]=geometry params,
//         word[wc-4]=fillColor, word[wc-3]=strokeColor, word[wc-2]=strokeWidth, word[wc-1]=round
// p[i] aliases data[2+i] (geometry params).
static void computeAABB(const float* data, uint32_t wc,
                        float& aabbMinX, float& aabbMinY,
                        float& aabbMaxX, float& aabbMaxY) {
    uint32_t type = sdf::detail::read_u32(data, 0);
    const float* p = data + 2;  // geometry params
    float strokeWidth = data[wc - 2];
    float round_ = data[wc - 1];
    float expand = strokeWidth * 0.5f;

    switch (static_cast<SDFType>(type)) {
        case SDFType::Circle: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = p[2] + round_ + expand;
            float hh = p[3] + round_ + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::Segment: {
            aabbMinX = std::min(p[0], p[2]) - expand;
            aabbMinY = std::min(p[1], p[3]) - expand;
            aabbMaxX = std::max(p[0], p[2]) + expand;
            aabbMaxY = std::max(p[1], p[3]) + expand;
            break;
        }
        case SDFType::Triangle: {
            aabbMinX = std::min({p[0], p[2], p[4]}) - expand;
            aabbMinY = std::min({p[1], p[3], p[5]}) - expand;
            aabbMaxX = std::max({p[0], p[2], p[4]}) + expand;
            aabbMaxY = std::max({p[1], p[3], p[5]}) + expand;
            break;
        }
        case SDFType::Bezier2: {
            aabbMinX = std::min({p[0], p[2], p[4]}) - expand;
            aabbMinY = std::min({p[1], p[3], p[5]}) - expand;
            aabbMaxX = std::max({p[0], p[2], p[4]}) + expand;
            aabbMaxY = std::max({p[1], p[3], p[5]}) + expand;
            break;
        }
        case SDFType::Bezier3: {
            aabbMinX = std::min({p[0], p[2], p[4], p[6]}) - expand;
            aabbMinY = std::min({p[1], p[3], p[5], p[7]}) - expand;
            aabbMaxX = std::max({p[0], p[2], p[4], p[6]}) + expand;
            aabbMaxY = std::max({p[1], p[3], p[5], p[7]}) + expand;
            break;
        }
        case SDFType::Ellipse: {
            aabbMinX = p[0] - p[2] - expand; aabbMinY = p[1] - p[3] - expand;
            aabbMaxX = p[0] + p[2] + expand; aabbMaxY = p[1] + p[3] + expand;
            break;
        }
        case SDFType::Arc: {
            float r = std::max(p[4], p[5]) + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({p[4], p[5], p[6], p[7]});
            float hw = p[2] + maxR + expand;
            float hh = p[3] + maxR + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::Rhombus: {
            float hw = p[2] + expand; float hh = p[3] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::Pentagon:
        case SDFType::Hexagon: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Star: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Pie: {
            float r = p[4] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Ring: {
            float r = p[4] + p[5] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Heart: {
            float s = p[2] * 1.5f + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::Cross: {
            float hw = std::max(p[2], p[3]) + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hw;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hw;
            break;
        }
        case SDFType::RoundedX: {
            float s = p[2] + p[3] + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::Capsule: {
            float r = p[4] + expand;
            aabbMinX = std::min(p[0], p[2]) - r; aabbMinY = std::min(p[1], p[3]) - r;
            aabbMaxX = std::max(p[0], p[2]) + r; aabbMaxY = std::max(p[1], p[3]) + r;
            break;
        }
        case SDFType::Moon: {
            float r = std::max(p[3], p[4]) + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r + p[2]; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Egg: {
            float r = std::max(p[2], p[3]) + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r + p[2];
            break;
        }
        case SDFType::ChamferBox: {
            float hw = p[2] + p[4] + expand; float hh = p[3] + p[4] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::OrientedBox: {
            float th = p[4] * 0.5f + expand;
            aabbMinX = std::min(p[0], p[2]) - th; aabbMinY = std::min(p[1], p[3]) - th;
            aabbMaxX = std::max(p[0], p[2]) + th; aabbMaxY = std::max(p[1], p[3]) + th;
            break;
        }
        case SDFType::Trapezoid: {
            float hw = std::max(p[2], p[3]) + expand; float hh = p[4] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::Parallelogram: {
            float hw = p[2] + std::fabs(p[4]) + expand; float hh = p[3] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::EquilateralTriangle:
        case SDFType::Octogon:
        case SDFType::Hexagram:
        case SDFType::Pentagram: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::IsoscelesTriangle: {
            float hw = p[2] + expand; float hh = p[3] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - expand;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::UnevenCapsule: {
            float rMax = std::max(p[2], p[3]) + expand;
            aabbMinX = p[0] - rMax; aabbMinY = p[1] - p[2] - expand;
            aabbMaxX = p[0] + rMax; aabbMaxY = p[1] + p[4] + p[3] + expand;
            break;
        }
        case SDFType::CutDisk: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Horseshoe: {
            float r = p[4] + p[5] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::Vesica: {
            float hw = p[2] + expand; float hh = p[3] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::OrientedVesica: {
            float w = p[4] + expand;
            aabbMinX = std::min(p[0], p[2]) - w; aabbMinY = std::min(p[1], p[3]) - w;
            aabbMaxX = std::max(p[0], p[2]) + w; aabbMaxY = std::max(p[1], p[3]) + w;
            break;
        }
        case SDFType::RoundedCross: {
            float s = (p[2] + 1.0f) + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::Parabola: {
            float s = 2.0f / p[2] + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::BlobbyCross: {
            float s = p[2] * 1.5f + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::Tunnel: {
            float hw = p[2] + expand; float hh = p[3] + expand;
            aabbMinX = p[0] - hw; aabbMinY = p[1] - hh;
            aabbMaxX = p[0] + hw; aabbMaxY = p[1] + hh;
            break;
        }
        case SDFType::Stairs: {
            float tw = p[2] * p[4] + expand; float th = p[3] * p[4] + expand;
            aabbMinX = p[0] - expand; aabbMinY = p[1] - expand;
            aabbMaxX = p[0] + tw; aabbMaxY = p[1] + th;
            break;
        }
        case SDFType::QuadraticCircle: {
            float s = p[2] + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::Hyperbola: {
            float s = p[3] + expand + 1.0f;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::CoolS: {
            float s = p[2] * 1.2f + expand;
            aabbMinX = p[0] - s; aabbMinY = p[1] - s;
            aabbMaxX = p[0] + s; aabbMaxY = p[1] + s;
            break;
        }
        case SDFType::CircleWave: {
            float r = p[3] * 2.0f + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        case SDFType::RotatedGlyph: {
            float gx = p[0], gy = p[1], gw = p[2], gh = p[3], angle = p[4];
            float ca = std::cos(angle), sa = std::sin(angle);
            float cx[4] = {0, gw, gw, 0}, cy[4] = {0, 0, gh, gh};
            aabbMinX = gx; aabbMaxX = gx; aabbMinY = gy; aabbMaxY = gy;
            for (int i = 0; i < 4; i++) {
                float rx = gx + cx[i] * ca - cy[i] * sa;
                float ry = gy + cx[i] * sa + cy[i] * ca;
                aabbMinX = std::min(aabbMinX, rx); aabbMaxX = std::max(aabbMaxX, rx);
                aabbMinY = std::min(aabbMinY, ry); aabbMaxY = std::max(aabbMaxY, ry);
            }
            break;
        }
        case SDFType::ColorWheel: {
            float r = p[2] + expand;
            aabbMinX = p[0] - r; aabbMinY = p[1] - r;
            aabbMaxX = p[0] + r; aabbMaxY = p[1] + r;
            break;
        }
        // 3D primitives - not used for grid, set dummy AABB
        case SDFType::Sphere3D: case SDFType::Box3D: case SDFType::BoxFrame3D:
        case SDFType::Torus3D: case SDFType::CappedTorus3D: case SDFType::Cylinder3D:
        case SDFType::CappedCylinder3D: case SDFType::RoundedCylinder3D:
        case SDFType::VerticalCapsule3D: case SDFType::Cone3D: case SDFType::CappedCone3D:
        case SDFType::RoundCone3D: case SDFType::Plane3D: case SDFType::HexPrism3D:
        case SDFType::TriPrism3D: case SDFType::Octahedron3D: case SDFType::Pyramid3D:
        case SDFType::Ellipsoid3D: case SDFType::Rhombus3D: case SDFType::Link3D: {
            aabbMinX = 0; aabbMinY = 0; aabbMaxX = 0; aabbMaxY = 0;
            break;
        }
        default:
            aabbMinX = -1e10f; aabbMinY = -1e10f;
            aabbMaxX = 1e10f; aabbMaxY = 1e10f;
            break;
    }
}


//=============================================================================
// YDrawBuilderImpl
//=============================================================================

class YDrawBuilderImpl : public YDrawBuilder {
public:
    YDrawBuilderImpl(FontManager::Ptr fontManager, GpuAllocator::Ptr allocator,
                     YDrawBuffer::Ptr buffer, CardManager::Ptr cardMgr,
                     uint32_t metaSlotIndex)
        : _fontManager(std::move(fontManager))
        , _gpuAllocator(std::move(allocator))
        , _buffer(std::move(buffer))
        , _cardMgr(std::move(cardMgr))
        , _metaSlotIndex(metaSlotIndex)
    {
        if (_fontManager) {
            _font = _fontManager->getDefaultMsMsdfFont();
            if (_font) {
                _atlas = _font->atlas();
            }
        }
    }

    ~YDrawBuilderImpl() override = default;

    //=========================================================================
    // Text API
    //=========================================================================

    Result<void> addText(float x, float y, const std::string& text,
                         float fontSize, uint32_t color,
                         uint32_t layer) {
        return addText(x, y, text, fontSize, color, layer, -1);
    }

    Result<void> addText(float x, float y, const std::string& text,
                         float fontSize, uint32_t color,
                         uint32_t layer, int fontId) {
        int resolvedFontId = fontId;
        if (fontId >= 0) {
            auto it = _userFontIdMap.find(fontId);
            if (it != _userFontIdMap.end()) {
                resolvedFontId = it->second;
            }
        }
        bool useCustom = (resolvedFontId >= 0 && _customAtlas);
        MsdfAtlas* activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
        if (!activeAtlas) {
            return Err<void>("addText: no atlas available");
        }
        if (text.empty()) {
            return Ok();
        }

        float fontBaseSize = activeAtlas->getFontSize();
        float scale = fontSize / fontBaseSize;

        float cursorX = x;
        uint32_t glyphsAdded = 0;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = 0;
            if ((*ptr & 0x80) == 0) {
                codepoint = *ptr++;
            } else if ((*ptr & 0xE0) == 0xC0) {
                codepoint = (*ptr++ & 0x1F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF0) == 0xE0) {
                codepoint = (*ptr++ & 0x0F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF8) == 0xF0) {
                codepoint = (*ptr++ & 0x07) << 18;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else {
                ptr++;
                continue;
            }

            uint32_t glyphIndex;
            if (useCustom) {
                glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
            } else if (_font) {
                glyphIndex = _font->getGlyphIndex(codepoint);
            } else {
                continue;
            }

            const auto& metadata = activeAtlas->getGlyphMetadata();
            if (glyphIndex >= metadata.size()) {
                cursorX += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];

            YDrawGlyph posGlyph = {};
            posGlyph.x = cursorX + glyph._bearingX * scale;
            posGlyph.y = y - glyph._bearingY * scale;
            posGlyph.setSize(glyph._sizeX * scale, glyph._sizeY * scale);
            posGlyph.setGlyphLayerFlags(
                static_cast<uint16_t>(glyphIndex),
                static_cast<uint8_t>(layer),
                useCustom ? 1u : 0u);
            posGlyph.color = color;

            _glyphs.push_back(posGlyph);
            glyphsAdded++;

            cursorX += glyph._advance * scale;
        }

        if (glyphsAdded > 0) {
            _gridStaging.clear();  // invalidate grid
        }

        return Ok();
    }

    Result<void> addRotatedText(float x, float y, const std::string& text,
                                 float fontSize, uint32_t color,
                                 float angleRadians, int fontId) {
        int resolvedFontId = fontId;
        if (fontId >= 0) {
            auto it = _userFontIdMap.find(fontId);
            if (it != _userFontIdMap.end()) resolvedFontId = it->second;
        }
        if (resolvedFontId < 0 || !_customAtlas) {
            return Err<void>("addRotatedText: requires a custom font (rotated "
                             "glyphs only work with custom atlas)");
        }
        if (text.empty()) return Ok();
        MsdfAtlas* activeAtlas = _customAtlas.get();

        float fontBaseSize = activeAtlas->getFontSize();
        float scale = fontSize / fontBaseSize;
        float cosA = std::cos(angleRadians);
        float sinA = std::sin(angleRadians);
        float cursorAdvance = 0.0f;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = 0;
            if ((*ptr & 0x80) == 0) {
                codepoint = *ptr++;
            } else if ((*ptr & 0xE0) == 0xC0) {
                codepoint = (*ptr++ & 0x1F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF0) == 0xE0) {
                codepoint = (*ptr++ & 0x0F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF8) == 0xF0) {
                codepoint = (*ptr++ & 0x07) << 18;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else {
                ptr++;
                continue;
            }

            uint32_t glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
            const auto& metadata = activeAtlas->getGlyphMetadata();
            if (glyphIndex >= metadata.size()) {
                cursorAdvance += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];
            float bx = glyph._bearingX * scale;
            float by = -glyph._bearingY * scale;
            float gw = glyph._sizeX * scale;
            float gh = glyph._sizeY * scale;

            float gx = x + (cursorAdvance + bx) * cosA - by * sinA;
            float gy = y + (cursorAdvance + bx) * sinA + by * cosA;

            // Write RotatedGlyph prim to buffer
            _buffer->addRotatedGlyph(0, gx, gy, gw, gh, angleRadians,
                                    glyphIndex, cosA, sinA, color, 0, 0, 0);
            cursorAdvance += glyph._advance * scale;
        }

        return Ok();
    }

    //=========================================================================
    // Text measurement & font metrics
    //=========================================================================

    float measureTextWidth(const std::string& text,
                           float fontSize, int fontId) {
        int resolvedFontId = fontId;
        if (fontId >= 0) {
            auto it = _userFontIdMap.find(fontId);
            if (it != _userFontIdMap.end()) resolvedFontId = it->second;
        }
        bool useCustom = (resolvedFontId >= 0 && _customAtlas);
        MsdfAtlas* activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
        if (!activeAtlas || text.empty()) return 0.0f;

        float fontBaseSize = activeAtlas->getFontSize();
        float scale = fontSize / fontBaseSize;

        float width = 0.0f;
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = 0;
            if ((*ptr & 0x80) == 0) {
                codepoint = *ptr++;
            } else if ((*ptr & 0xE0) == 0xC0) {
                codepoint = (*ptr++ & 0x1F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF0) == 0xE0) {
                codepoint = (*ptr++ & 0x0F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF8) == 0xF0) {
                codepoint = (*ptr++ & 0x07) << 18;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else {
                ptr++;
                continue;
            }

            uint32_t glyphIndex;
            if (useCustom) {
                glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
            } else if (_font) {
                glyphIndex = _font->getGlyphIndex(codepoint);
            } else {
                continue;
            }

            const auto& metadata = activeAtlas->getGlyphMetadata();
            if (glyphIndex < metadata.size()) {
                width += metadata[glyphIndex]._advance * scale;
            } else {
                width += fontSize * 0.5f;
            }
        }

        return width;
    }

    float fontAscent(float fontSize, int fontId) {
        auto it = _ttfMetricsCache.find(fontId);
        if (it != _ttfMetricsCache.end()) {
            bool useCustom = (fontId >= 0 && _customAtlas);
            MsdfAtlas* activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
            float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
            return it->second.ascender * (fontSize / atlasSize);
        }
        return fontSize * 0.8f;
    }

    float fontDescent(float fontSize, int fontId) {
        auto it = _ttfMetricsCache.find(fontId);
        if (it != _ttfMetricsCache.end()) {
            bool useCustom = (fontId >= 0 && _customAtlas);
            MsdfAtlas* activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
            float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
            return it->second.descender * (fontSize / atlasSize);
        }
        return fontSize * 0.2f;
    }

    //=========================================================================
    // Font API
    //=========================================================================

    int registerFont(const std::string& cdbPath,
                     const std::string& ttfPath,
                     MsdfCdbProvider::Ptr provider) {
        if (!_customAtlas) {
            auto atlasRes = MsdfAtlas::create(_gpuAllocator);
            if (!atlasRes) {
                yerror("registerFont: failed to create custom atlas");
                return -1;
            }
            _customAtlas = *atlasRes;
            _flags |= FLAG_CUSTOM_ATLAS;
        }
        return _customAtlas->registerCdb(cdbPath, ttfPath, std::move(provider));
    }

    Result<int> addFont(const std::string& ttfPath) {
        if (!_fontManager) return Err<int>("addFont: no font manager");

        auto stem = std::filesystem::path(ttfPath).stem().string();
        auto cacheDir = _fontManager->getCacheDir();
        auto cdbPath = cacheDir + "/" + stem + ".cdb";

        auto it = _fontIdCache.find(cdbPath);
        if (it != _fontIdCache.end()) return Ok(it->second);

        if (!std::filesystem::exists(cdbPath)) {
            auto provider = _fontManager->getCdbProvider();
            if (!provider) {
                return Err<int>("addFont: no CDB provider available");
            }
            MsdfCdbConfig cfg;
            cfg.ttfPath = ttfPath;
            cfg.cdbPath = cdbPath;
            if (auto res = provider->generate(cfg); !res) {
                return Err<int>("addFont: CDB generation failed for " + ttfPath);
            }
        }

        if (!_customAtlas) {
            auto atlasRes = MsdfAtlas::create(_gpuAllocator);
            if (!atlasRes) {
                return Err<int>("addFont: failed to create custom atlas");
            }
            _customAtlas = *atlasRes;
            _flags |= FLAG_CUSTOM_ATLAS;
        }

        int fontId = _customAtlas->openCdb(cdbPath);
        if (fontId < 0) {
            return Err<int>("addFont: failed to open CDB: " + cdbPath);
        }
        _fontIdCache[cdbPath] = fontId;

        float atlasSize = _customAtlas->getFontSize();
        _ttfMetricsCache[fontId] = readTtfMetrics(ttfPath, atlasSize);

        yinfo("addFont: registered '{}' on custom atlas (fontId={})", stem, fontId);
        return Ok(fontId);
    }

    Result<void> addFont(int userFontId, const std::string& ttfPath) {
        auto result = addFont(ttfPath);
        if (!result) {
            return Err<void>("addFont(id=" + std::to_string(userFontId) + "): " +
                             result.error().message());
        }
        int atlasId = *result;
        _userFontIdMap[userFontId] = atlasId;
        yinfo("addFont: mapped user fontId {} -> atlas fontId {}", userFontId, atlasId);
        return Ok();
    }

    Result<int> addFontData(const uint8_t* data, size_t size,
                             const std::string& /*name*/) {
        uint64_t h = 14695981039346656037ULL;
        for (size_t i = 0; i < size; i++) {
            h ^= data[i];
            h *= 1099511628211ULL;
        }
        char hex[17];
        std::snprintf(hex, sizeof(hex), "%016llx",
                      static_cast<unsigned long long>(h));

        auto cacheDir = _fontManager->getCacheDir();
        auto ttfPath = cacheDir + "/pdf_font_" + hex + ".ttf";

        if (!std::filesystem::exists(ttfPath)) {
            std::ofstream out(ttfPath, std::ios::binary);
            if (!out)
                return Err<int>("addFontData: failed to write " + ttfPath);
            out.write(reinterpret_cast<const char*>(data),
                      static_cast<std::streamsize>(size));
        }

        return addFont(ttfPath);
    }

    void mapFontId(int userId, int atlasId) {
        _userFontIdMap[userId] = atlasId;
    }

    //=========================================================================
    // Atlas access
    //=========================================================================

    MsdfAtlas::Ptr atlas() const override { return _atlas; }
    MsdfAtlas::Ptr customAtlas() const override { return _customAtlas; }
    bool hasCustomAtlas() const override { return _customAtlas != nullptr; }
    MsMsdfFont::Ptr font() const override { return _font; }
    FontManager::Ptr fontManager() const override { return _fontManager; }

    //=========================================================================
    // State management
    //=========================================================================

    uint32_t primitiveCount() const override {
        return _buffer->primCount();
    }

    uint32_t glyphCount() const override {
        return static_cast<uint32_t>(_glyphs.size());
    }

    void setBgColor(uint32_t color) override { _bgColor = color; }
    uint32_t bgColor() const override { return _bgColor; }

    void setSceneBounds(float minX, float minY, float maxX, float maxY) override {
        _sceneMinX = minX;
        _sceneMinY = minY;
        _sceneMaxX = maxX;
        _sceneMaxY = maxY;
        _hasExplicitBounds = true;
    }
    bool hasExplicitBounds() const override { return _hasExplicitBounds; }

    void setGridCellSize(float size) override { _cellSize = size; }
    float gridCellSize() const override { return _cellSize; }

    void setFlags(uint32_t flags) override { _flags = flags; }
    void addFlags(uint32_t flags) override { _flags |= flags; }
    uint32_t flags() const override { return _flags; }

    void setMaxPrimsPerCell(uint32_t max) override { _maxPrimsPerCell = max; }
    uint32_t maxPrimsPerCell() const override { return _maxPrimsPerCell; }

    //=========================================================================
    // Staging data access
    //=========================================================================

    const std::vector<uint32_t>& gridStaging() const override { return _gridStaging; }
    void clearGridStaging() override { _gridStaging.clear(); }
    const std::vector<YDrawGlyph>& glyphs() const override { return _glyphs; }
    std::vector<YDrawGlyph>& glyphsMut() override { return _glyphs; }

    //=========================================================================
    // Buffer access
    //=========================================================================

    //=========================================================================
    // GPU buffer lifecycle
    //=========================================================================

    Result<void> declareBufferNeeds() override {
        if (!_cardMgr) return Err<void>("declareBufferNeeds: no CardManager");
        auto bufMgr = _cardMgr->bufferManager();

        // Reset handles — they will be re-allocated
        _primHandle = BufferHandle::invalid();
        _derivedHandle = BufferHandle::invalid();
        _primWordOffsets.clear();

        if (_buffer->empty() && _glyphs.empty()) {
            clearGridStaging();
            return Ok();
        }

        // Reserve compact prim storage (only if there are actual prims)
        uint32_t primBytes = _buffer->gpuBufferSize();
        if (primBytes > 0) {
            bufMgr->reserve(primBytes);
        }

        // Build grid if not already computed
        if (_gridStaging.empty()) {
            calculate();
        }

        // Reserve derived size (grid + glyphs + optional atlas metadata)
        uint32_t derivedSize = computeDerivedSize();
        if (derivedSize > 0) {
            bufMgr->reserve(derivedSize);
        }

        return Ok();
    }

    Result<void> allocateBuffers() override {
        if (!_cardMgr) return Err<void>("allocateBuffers: no CardManager");
        auto bufMgr = _cardMgr->bufferManager();

        // Allocate and write compact prim data (skip if no prims — text-only buffers
        // have gpuBufferSize()==0 even though empty()==false due to text spans)
        {
            uint32_t allocBytes = _buffer->gpuBufferSize();
            if (allocBytes > 0) {
                auto primResult = bufMgr->allocateBuffer(_metaSlotIndex, "prims", allocBytes);
                if (!primResult) return Err<void>("allocateBuffers: prim alloc failed", primResult);
                _primHandle = *primResult;

                float* buf = reinterpret_cast<float*>(_primHandle.data);
                _buffer->writeGPU(buf, _primHandle.size, _primWordOffsets);
                bufMgr->markBufferDirty(_primHandle);
            }
        }

        // Allocate derived storage (grid + glyphs + atlas header)
        uint32_t derivedSize = computeDerivedSize();
        if (derivedSize > 0) {
            auto derivedResult = bufMgr->allocateBuffer(_metaSlotIndex, "derived", derivedSize);
            if (!derivedResult) return Err<void>("allocateBuffers: derived alloc failed", derivedResult);
            _derivedHandle = *derivedResult;

            if (auto res = writeDerived(); !res) return res;
            bufMgr->markBufferDirty(_derivedHandle);
        }

        _bufferDirty = false;
        _metadataDirty = true;
        return Ok();
    }

    Result<void> allocateTextures() override {
        if (!_cardMgr) return Ok();

        auto texMgr = _cardMgr->textureManager();

        // Allocate custom MSDF atlas if present
        if (_customAtlas) {
            uint32_t atlasW = _customAtlas->getAtlasWidth();
            uint32_t atlasH = _customAtlas->getAtlasHeight();
            const auto& atlasData = _customAtlas->getAtlasData();
            if (!atlasData.empty() && atlasW > 0 && atlasH > 0) {
                auto allocResult = texMgr->allocate(atlasW, atlasH);
                if (!allocResult) return Err<void>("allocateTextures: msdf atlas failed", allocResult);
                _atlasTextureHandle = *allocResult;
                _allocatedAtlasW = atlasW;
                _allocatedAtlasH = atlasH;
            }
        }

        // Allocate texture space for each pending image
        for (auto& pi : _pendingImages) {
            if (pi.pixelWidth == 0 || pi.pixelHeight == 0 || pi.pixels.empty()) continue;
            auto allocResult = texMgr->allocate(pi.pixelWidth, pi.pixelHeight);
            if (!allocResult) {
                ywarn("allocateTextures: image {}x{} failed: {}", pi.pixelWidth, pi.pixelHeight, allocResult.error().message());
                continue;
            }
            pi.texHandle = *allocResult;
        }

        return Ok();
    }

    Result<void> writeTextures() override {
        if (!_cardMgr) return Ok();

        auto texMgr = _cardMgr->textureManager();

        // Write custom MSDF atlas
        if (_atlasTextureHandle.isValid() && _customAtlas) {
            const auto& atlasData = _customAtlas->getAtlasData();
            if (!atlasData.empty()) {
                if (auto res = texMgr->write(_atlasTextureHandle, atlasData.data()); !res) {
                    return Err<void>("writeTextures: msdf atlas write failed", res);
                }
            }

            // Now that the atlas is packed, write the atlas header into derived buffer
            if (_derivedHandle.isValid() && _gpuAtlasHeaderOffset > 0) {
                auto atlasPos = texMgr->getAtlasPosition(_atlasTextureHandle);
                uint32_t msdfAtlasW = _customAtlas->getAtlasWidth();
                uint32_t msdfAtlasH = _customAtlas->getAtlasHeight();
                const auto& glyphMeta = _customAtlas->getGlyphMetadata();

                uint32_t headerLocalOff = _gpuAtlasHeaderOffset * sizeof(float) - _derivedHandle.offset;
                uint8_t* base = _derivedHandle.data + headerLocalOff;

                uint32_t atlasHeader[4] = {
                    (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
                    (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
                    static_cast<uint32_t>(glyphMeta.size()),
                    0
                };
                std::memcpy(base, atlasHeader, sizeof(atlasHeader));

                _cardMgr->bufferManager()->markBufferDirty(_derivedHandle);
            }
        }

        // Write image pixels and update Image primitives with atlas coords
        for (auto& pi : _pendingImages) {
            if (!pi.texHandle.isValid() || pi.primId == 0) continue;

            // Write pixel data to atlas
            if (auto res = texMgr->write(pi.texHandle, pi.pixels.data()); !res) {
                ywarn("writeTextures: image write failed: {}", res.error().message());
                continue;
            }

            // Update Image primitive with real atlas coordinates
            auto atlasPos = texMgr->getAtlasPosition(pi.texHandle);
            _buffer->updateImage(pi.primId, pi.layer, pi.x, pi.y, pi.w, pi.h,
                                 atlasPos.x, atlasPos.y, pi.pixelWidth, pi.pixelHeight);
        }

        // Mark buffer dirty so writeBuffers re-uploads with updated atlas coords
        if (!_pendingImages.empty()) {
            _bufferDirty = true;
        }

        return Ok();
    }

    Result<void> writeBuffers() override {
        if (!_cardMgr) return Err<void>("writeBuffers: no CardManager");

        if (_bufferDirty) {
            auto bufMgr = _cardMgr->bufferManager();

            // Re-write prims
            if (_primHandle.isValid() && !_buffer->empty()) {
                float* buf = reinterpret_cast<float*>(_primHandle.data);
                _buffer->writeGPU(buf, _primHandle.size, _primWordOffsets);
                bufMgr->markBufferDirty(_primHandle);
            }

            // Re-write derived (grid + glyphs + atlas header)
            if (_derivedHandle.isValid()) {
                if (auto res = writeDerived(); !res) return res;
                bufMgr->markBufferDirty(_derivedHandle);
            }

            _bufferDirty = false;
            _metadataDirty = true;
        }

        if (_metadataDirty) {
            if (auto res = flushMetadata(); !res) return res;
            _metadataDirty = false;
        }

        return Ok();
    }

    bool needsBufferRealloc() const override {
        if (!_cardMgr) return false;
        uint32_t primNeeded = _buffer->gpuBufferSize();
        if (_primHandle.isValid()) {
            if (primNeeded != _primHandle.size) return true;
        } else if (primNeeded > 0) {
            return true;
        }
        uint32_t derivedNeeded = computeDerivedSize();
        if (_derivedHandle.isValid()) {
            if (derivedNeeded > _derivedHandle.size) return true;
        } else if (derivedNeeded > 0) {
            return true;
        }
        return false;
    }

    bool needsTextureRealloc() const override {
        // Check custom atlas
        if (_customAtlas) {
            uint32_t atlasW = _customAtlas->getAtlasWidth();
            uint32_t atlasH = _customAtlas->getAtlasHeight();
            const auto& atlasData = _customAtlas->getAtlasData();
            if (!_atlasTextureHandle.isValid()) {
                if (!atlasData.empty()) return true;
            } else if (atlasW != _allocatedAtlasW || atlasH != _allocatedAtlasH) {
                return true;
            }
        }

        // Check pending images
        for (const auto& pi : _pendingImages) {
            if (!pi.pixels.empty() && !pi.texHandle.isValid()) return true;
        }

        return false;
    }

    void setViewport(uint32_t widthCells, uint32_t heightCells) override {
        if (_viewWidthCells != widthCells || _viewHeightCells != heightCells) {
            _viewWidthCells = widthCells;
            _viewHeightCells = heightCells;
            _metadataDirty = true;
        }
    }

    void setView(float zoom, float panX, float panY) override {
        if (_viewZoom != zoom || _viewPanX != panX || _viewPanY != panY) {
            _viewZoom = zoom;
            _viewPanX = panX;
            _viewPanY = panY;
            _metadataDirty = true;
        }
    }

    //=========================================================================
    // Text selection
    //=========================================================================

    void buildGlyphSortedOrder() override {
        _glyphSortedOrder.resize(_glyphs.size());
        std::iota(_glyphSortedOrder.begin(), _glyphSortedOrder.end(), 0u);
        std::sort(_glyphSortedOrder.begin(), _glyphSortedOrder.end(),
            [this](uint32_t a, uint32_t b) {
                const auto& ga = _glyphs[a];
                const auto& gb = _glyphs[b];
                if (ga.y != gb.y) return ga.y < gb.y;
                return ga.x < gb.x;
            });
    }

    int32_t findNearestGlyphSorted(float sceneX, float sceneY) override {
        if (_glyphSortedOrder.empty()) return -1;

        float bestDist = std::numeric_limits<float>::max();
        int32_t bestSorted = -1;

        for (size_t si = 0; si < _glyphSortedOrder.size(); si++) {
            const auto& g = _glyphs[_glyphSortedOrder[si]];
            float gw = g.width();
            float gh = g.height();

            if (sceneX >= g.x && sceneX < g.x + gw &&
                sceneY >= g.y && sceneY < g.y + gh) {
                return static_cast<int32_t>(si);
            }

            float cx = g.x + gw * 0.5f;
            float cy = g.y + gh * 0.5f;
            float dist = (cx - sceneX) * (cx - sceneX) + (cy - sceneY) * (cy - sceneY);
            if (dist < bestDist) {
                bestDist = dist;
                bestSorted = static_cast<int32_t>(si);
            }
        }
        return bestSorted;
    }

    void setSelectionRange(int32_t startSorted, int32_t endSorted) override {
        for (auto& g : _glyphs) {
            g.glyphLayerFlags &= ~(static_cast<uint32_t>(GLYPH_FLAG_SELECTED) << 24);
        }

        if (startSorted < 0 || endSorted < 0 || _glyphSortedOrder.empty()) {
            _bufferDirty = true;
            return;
        }

        int32_t lo = std::min(startSorted, endSorted);
        int32_t hi = std::min(std::max(startSorted, endSorted),
                              static_cast<int32_t>(_glyphSortedOrder.size()) - 1);

        for (int32_t i = lo; i <= hi; i++) {
            _glyphs[_glyphSortedOrder[i]].glyphLayerFlags |=
                (static_cast<uint32_t>(GLYPH_FLAG_SELECTED) << 24);
        }
        _bufferDirty = true;
    }

    std::string getSelectedText() override {
        if (_glyphSortedOrder.empty()) return {};

        std::string result;
        float prevY = -1e30f;
        float prevEndX = -1e30f;
        bool first = true;

        for (size_t i = 0; i < _glyphSortedOrder.size(); i++) {
            const auto& g = _glyphs[_glyphSortedOrder[i]];
            if (((g.glyphLayerFlags >> 24) & GLYPH_FLAG_SELECTED) == 0) continue;

            float gh = g.height();
            float gw = g.width();

            if (!first) {
                if ((g.y - prevY) > gh * 0.5f) {
                    result += '\n';
                    prevEndX = -1e30f;
                } else if (prevEndX > -1e29f && (g.x - prevEndX) > gw * 0.3f) {
                    result += ' ';
                }
            }
            first = false;
            prevY = g.y;
            prevEndX = g.x + gw;

            uint16_t glyphIdx = static_cast<uint16_t>(g.glyphLayerFlags & 0xFFFF);
            bool isCustom = ((g.glyphLayerFlags >> 24) & GLYPH_FLAG_CUSTOM_ATLAS) != 0;

            uint32_t cp = 0;
            if (isCustom && _customAtlas) {
                cp = _customAtlas->getCodepoint(glyphIdx);
            } else if (_atlas) {
                cp = _atlas->getCodepoint(glyphIdx);
            }

            if (cp > 0) {
                if (cp < 0x80) {
                    result += static_cast<char>(cp);
                } else if (cp < 0x800) {
                    result += static_cast<char>(0xC0 | (cp >> 6));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                } else if (cp < 0x10000) {
                    result += static_cast<char>(0xE0 | (cp >> 12));
                    result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                } else {
                    result += static_cast<char>(0xF0 | (cp >> 18));
                    result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
                    result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (cp & 0x3F));
                }
            }
        }
        return result;
    }

    //=========================================================================
    // Grid computation
    //=========================================================================

    float sceneMinX() const override { return _sceneMinX; }
    float sceneMinY() const override { return _sceneMinY; }
    float sceneMaxX() const override { return _sceneMaxX; }
    float sceneMaxY() const override { return _sceneMaxY; }
    float cellSize() const override { return _cellSize; }
    uint32_t gridWidth() const override { return _gridWidth; }
    uint32_t gridHeight() const override { return _gridHeight; }

    void calculate() override {
        // Clear builder-owned staging data — will be rebuilt from buffer state
        _primBounds.clear();
        _gridStaging.clear();
        _glyphs.clear();

        // Step 0: Ingest buffer metadata — fonts, text spans, scene bounds
        // The buffer may carry font blobs and text spans (e.g. from PDF renderer).
        // Register fonts with the builder's MSDF atlas, then convert text spans
        // into glyphs via the builder's existing addText/addRotatedText.
        _buffer->forEachFont([this](int bufFontId, const uint8_t* data,
                                     size_t size, const std::string& name) {
            if (_bufferFontIdMap.count(bufFontId)) return;
            auto res = addFontData(data, size, name);
            if (res) {
                _bufferFontIdMap[bufFontId] = *res;
            } else {
                ywarn("calculate: addFontData failed for '{}': {}", name, res.error().message());
            }
        });

        _buffer->forEachTextSpan([this](const TextSpanData& span) {
            int atlasFontId = -1;
            auto it = _bufferFontIdMap.find(span.fontId);
            if (it != _bufferFontIdMap.end()) atlasFontId = it->second;

            if (std::abs(span.rotation) > 0.001f) {
                addRotatedText(span.x, span.y, span.text,
                               span.fontSize, span.color,
                               span.rotation, atlasFontId);
            } else {
                addText(span.x, span.y, span.text,
                        span.fontSize, span.color, span.layer, atlasFontId);
            }
        });

        if (_buffer->hasSceneBounds() && !_hasExplicitBounds) {
            _sceneMinX = _buffer->sceneMinX();
            _sceneMinY = _buffer->sceneMinY();
            _sceneMaxX = _buffer->sceneMaxX();
            _sceneMaxY = _buffer->sceneMaxY();
            _hasExplicitBounds = true;
        }

        if (_buffer->bgColor() != 0) {
            _bgColor = _buffer->bgColor();
        }

        if (_buffer->flags() != 0) {
            _flags |= _buffer->flags();
        }

        // Ingest images from buffer into pending list and add Image primitives
        _pendingImages.clear();
        _buffer->forEachImage([this](const ImageData& img) {
            PendingImage pi;
            pi.x = img.x;
            pi.y = img.y;
            pi.w = img.w;
            pi.h = img.h;
            pi.pixelWidth = img.pixelWidth;
            pi.pixelHeight = img.pixelHeight;
            pi.layer = img.layer;
            pi.pixels = img.pixels;
            pi.texHandle = TextureHandle::invalid();
            // Add Image primitive with placeholder atlas coords (will be updated in writeTextures)
            auto res = _buffer->addImage(img.layer, img.x, img.y, img.w, img.h,
                                         0, 0, img.pixelWidth, img.pixelHeight);
            if (res) {
                pi.primId = *res;
            }
            _pendingImages.push_back(std::move(pi));
        });

        // Step 1: Read all prims from buffer, compute AABB for each
        _buffer->forEachPrim([this](uint32_t /*id*/, const float* data, uint32_t wc) {
            uint32_t type = sdf::detail::read_u32(data, 0);
            float minX, minY, maxX, maxY;
            computeAABB(data, wc, minX, minY, maxX, maxY);
            if (type >= 100) {
                _flags |= FLAG_HAS_3D;
            }
            _primBounds.push_back({minX, minY, maxX, maxY, type});
        });

        // Step 2: Compute scene bounds from prim AABBs
        if (!_hasExplicitBounds) {
            _sceneMinX = 1e10f; _sceneMinY = 1e10f;
            _sceneMaxX = -1e10f; _sceneMaxY = -1e10f;
            for (const auto& pb : _primBounds) {
                if (pb.type >= 100) continue;
                _sceneMinX = std::min(_sceneMinX, pb.minX);
                _sceneMinY = std::min(_sceneMinY, pb.minY);
                _sceneMaxX = std::max(_sceneMaxX, pb.maxX);
                _sceneMaxY = std::max(_sceneMaxY, pb.maxY);
            }
            for (const auto& g : _glyphs) {
                _sceneMinX = std::min(_sceneMinX, g.x);
                _sceneMinY = std::min(_sceneMinY, g.y);
                _sceneMaxX = std::max(_sceneMaxX, g.x + g.width());
                _sceneMaxY = std::max(_sceneMaxY, g.y + g.height());
            }
            float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
            float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
            _sceneMinX -= padX; _sceneMinY -= padY;
            _sceneMaxX += padX; _sceneMaxY += padY;
            if (_sceneMinX >= _sceneMaxX) { _sceneMinX = 0; _sceneMaxX = 100; }
            if (_sceneMinY >= _sceneMaxY) { _sceneMinY = 0; _sceneMaxY = 100; }
        }

        // Step 3: Compute grid dimensions
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;
        uint32_t gridW = 0, gridH = 0;
        float cs = _cellSize;

        uint32_t num2DPrims = 0;
        for (const auto& pb : _primBounds) {
            if (pb.type < 100) num2DPrims++;
        }

        if (num2DPrims > 0 || !_glyphs.empty()) {
            if (cs <= 0.0f) {
                float primCs = 0.0f, glyphCs = 0.0f;
                if (num2DPrims > 0) {
                    float avgPrimArea = 0.0f;
                    for (const auto& pb : _primBounds) {
                        if (pb.type >= 100) continue;
                        float w = pb.maxX - pb.minX;
                        float h = pb.maxY - pb.minY;
                        avgPrimArea += w * h;
                    }
                    avgPrimArea /= num2DPrims;
                    primCs = std::sqrt(avgPrimArea) * 1.5f;
                }
                uint32_t glyphCount = static_cast<uint32_t>(_glyphs.size());
                if (glyphCount > 0) {
                    float avgGlyphH = 0.0f;
                    for (const auto& g : _glyphs) avgGlyphH += g.height();
                    avgGlyphH /= glyphCount;
                    glyphCs = avgGlyphH * 2.0f;
                }
                if (num2DPrims > 0 && glyphCount > 0) {
                    cs = (num2DPrims * primCs + glyphCount * glyphCs) / (num2DPrims + glyphCount);
                } else if (num2DPrims > 0) {
                    cs = primCs;
                } else {
                    cs = glyphCs;
                }
                if (cs <= 0.0f) cs = 1.0f;
            }
            gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
            gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
            constexpr uint32_t MAX_TOTAL_CELLS = 4u * 1024u * 1024u;
            uint64_t totalCells = static_cast<uint64_t>(gridW) * gridH;
            if (totalCells > MAX_TOTAL_CELLS) {
                float scale = std::sqrt(static_cast<float>(totalCells) / MAX_TOTAL_CELLS);
                cs *= scale;
                gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
                gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
            }
        }

        _gridWidth = gridW;
        _gridHeight = gridH;
        _cellSize = cs;

        yinfo("YDrawBuilder::calculate: grid={}x{} cellSize={:.1f} prims={} glyphs={} scene=[{:.0f},{:.0f}]-[{:.0f},{:.0f}]",
              gridW, gridH, cs, _primBounds.size(), _glyphs.size(),
              _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);

        // Step 4: Build variable-length grid into staging
        uint32_t numCells = gridW * gridH;
        std::vector<uint32_t> cellCounts(numCells, 0);

        for (const auto& pb : _primBounds) {
            if (pb.type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((pb.minX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((pb.maxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((pb.minY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((pb.maxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            for (uint32_t cy = cMinY; cy <= cMaxY; cy++)
                for (uint32_t cx = cMinX; cx <= cMaxX; cx++)
                    cellCounts[cy * gridW + cx]++;
        }
        for (const auto& g : _glyphs) {
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((g.x - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((g.x + g.width() - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((g.y - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((g.y + g.height() - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            for (uint32_t cy = cMinY; cy <= cMaxY; cy++)
                for (uint32_t cx = cMinX; cx <= cMaxX; cx++)
                    cellCounts[cy * gridW + cx]++;
        }

        // Compute offsets (prefix sum)
        uint32_t pos = numCells;
        _gridStaging.resize(numCells);
        for (uint32_t i = 0; i < numCells; i++) {
            _gridStaging[i] = pos;
            pos += 1 + cellCounts[i];
        }
        _gridStaging.resize(pos, 0);

        for (uint32_t i = 0; i < numCells; i++) {
            _gridStaging[_gridStaging[i]] = 0;
        }

        // Pass 2: fill entries
        for (uint32_t primIdx = 0; primIdx < static_cast<uint32_t>(_primBounds.size()); primIdx++) {
            const auto& pb = _primBounds[primIdx];
            if (pb.type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((pb.minX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((pb.maxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((pb.minY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((pb.maxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            for (uint32_t cy = cMinY; cy <= cMaxY; cy++) {
                for (uint32_t cx = cMinX; cx <= cMaxX; cx++) {
                    uint32_t off = _gridStaging[cy * gridW + cx];
                    uint32_t cnt = _gridStaging[off];
                    _gridStaging[off + 1 + cnt] = primIdx;
                    _gridStaging[off] = cnt + 1;
                }
            }
        }
        for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
            const auto& g = _glyphs[gi];
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((g.x - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((g.x + g.width() - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((g.y - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((g.y + g.height() - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            for (uint32_t cy = cMinY; cy <= cMaxY; cy++) {
                for (uint32_t cx = cMinX; cx <= cMaxX; cx++) {
                    uint32_t off = _gridStaging[cy * gridW + cx];
                    uint32_t cnt = _gridStaging[off];
                    _gridStaging[off + 1 + cnt] = gi | GLYPH_BIT;
                    _gridStaging[off] = cnt + 1;
                }
            }
        }

        // Grid stats
        uint32_t maxEntries = 0, totalEntries = 0, nonEmptyCells = 0;
        for (uint32_t i = 0; i < numCells; i++) {
            uint32_t cnt = _gridStaging[_gridStaging[i]];
            totalEntries += cnt;
            if (cnt > 0) nonEmptyCells++;
            if (cnt > maxEntries) maxEntries = cnt;
        }
        yinfo("YDrawBuilder::calculate: gridSize={} u32s ({} KB) cells={} nonEmpty={} totalEntries={} maxPerCell={} avgPerNonEmpty={:.1f}",
              _gridStaging.size(), _gridStaging.size() * 4 / 1024,
              numCells, nonEmptyCells, totalEntries, maxEntries,
              nonEmptyCells > 0 ? float(totalEntries) / nonEmptyCells : 0.0f);

        _bufferDirty = true;
    }

private:
    //=========================================================================
    // GPU helpers
    //=========================================================================

    Result<void> flushMetadata() {
        // Pack zoom as f16 in upper 16 bits of flags
        uint32_t zoomBits;
        {
            uint32_t f32bits;
            std::memcpy(&f32bits, &_viewZoom, sizeof(float));
            uint32_t sign = (f32bits >> 16) & 0x8000;
            int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
            uint32_t mant = (f32bits >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
        }

        float contentW = _sceneMaxX - _sceneMinX;
        float contentH = _sceneMaxY - _sceneMinY;
        int16_t panXi16 = static_cast<int16_t>(std::clamp(
            _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
        int16_t panYi16 = static_cast<int16_t>(std::clamp(
            _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

        uint32_t primOffset = _primHandle.isValid()
            ? _primHandle.offset / sizeof(float) : 0;

        YDrawMetadata meta = {};
        meta.primitiveOffset = primOffset;
        meta.primitiveCount = _buffer->primCount();
        meta.gridOffset = _gpuGridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));
        meta.glyphOffset = _gpuGlyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
        std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
        meta.widthCells  = (_viewWidthCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
        meta.heightCells = (_viewHeightCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
        meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
        meta.bgColor = _bgColor;

        return _cardMgr->writeMetadata(
            MetadataHandle{_metaSlotIndex * 64, 64}, &meta, sizeof(meta));
    }

    uint32_t computeDerivedSize() const {
        uint32_t gridBytes = static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(YDrawGlyph));
        uint32_t total = gridBytes + glyphBytes;
        if (_customAtlas) {
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
            uint32_t glyphMetaBytes = static_cast<uint32_t>(
                _customAtlas->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
            total += atlasHeaderBytes + glyphMetaBytes;
        }
        return total;
    }

    Result<void> writeDerived() {
        if (!_derivedHandle.isValid()) {
            return Err<void>("writeDerived: derived handle not allocated");
        }

        uint8_t* base = _derivedHandle.data;
        uint32_t offset = 0;

        // Copy grid and translate prim indices to word offsets
        uint32_t gridBytes = static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
        _gpuGridOffset = (_derivedHandle.offset + offset) / sizeof(float);
        if (!_gridStaging.empty()) {
            std::memcpy(base + offset, _gridStaging.data(), gridBytes);
            // Translate prim indices in grid entries to word offsets
            uint32_t* gridPtr = reinterpret_cast<uint32_t*>(base + offset);
            uint32_t gridSize = static_cast<uint32_t>(_gridStaging.size());
            uint32_t numCells = _gridWidth * _gridHeight;
            if (!_primWordOffsets.empty() && numCells <= gridSize) {
                for (uint32_t ci = 0; ci < numCells; ci++) {
                    uint32_t packedOff = gridPtr[ci];
                    if (packedOff >= gridSize) continue;
                    uint32_t cnt = gridPtr[packedOff];
                    for (uint32_t j = 0; j < cnt; j++) {
                        uint32_t idx = packedOff + 1 + j;
                        if (idx >= gridSize) break;
                        uint32_t rawVal = gridPtr[idx];
                        if ((rawVal & 0x80000000u) != 0) continue;
                        if (rawVal < static_cast<uint32_t>(_primWordOffsets.size())) {
                            gridPtr[idx] = _primWordOffsets[rawVal];
                        }
                    }
                }
            }
        }
        offset += gridBytes;

        // Copy glyphs
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(YDrawGlyph));
        _gpuGlyphOffset = (_derivedHandle.offset + offset) / sizeof(float);
        if (!_glyphs.empty()) {
            std::memcpy(base + offset, _glyphs.data(), glyphBytes);
        }
        offset += glyphBytes;

        // Custom atlas header + glyph metadata
        if (_customAtlas) {
            _gpuAtlasHeaderOffset = (_derivedHandle.offset + offset) / sizeof(float);
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);

            if (_atlasTextureHandle.isValid()) {
                // Texture already allocated — write actual atlas position
                auto texMgr = _cardMgr->textureManager();
                auto atlasPos = texMgr->getAtlasPosition(_atlasTextureHandle);
                uint32_t msdfAtlasW = _customAtlas->getAtlasWidth();
                uint32_t msdfAtlasH = _customAtlas->getAtlasHeight();
                const auto& glyphMeta = _customAtlas->getGlyphMetadata();
                uint32_t atlasHeader[4] = {
                    (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
                    (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
                    static_cast<uint32_t>(glyphMeta.size()),
                    0
                };
                std::memcpy(base + offset, atlasHeader, sizeof(atlasHeader));
            } else {
                // Texture not yet allocated — zero-fill, writeTextures() will fill later
                std::memset(base + offset, 0, atlasHeaderBytes);
            }
            offset += atlasHeaderBytes;

            const auto& glyphMeta = _customAtlas->getGlyphMetadata();
            if (!glyphMeta.empty()) {
                uint32_t metaBytes = static_cast<uint32_t>(
                    glyphMeta.size() * sizeof(GlyphMetadataGPU));
                std::memcpy(base + offset, glyphMeta.data(), metaBytes);
            }
        }

        return Ok();
    }

    //=========================================================================
    // Data
    //=========================================================================

    // Lightweight AABB data for grid computation (populated by calculate from buffer)
    struct PrimBounds {
        float minX, minY, maxX, maxY;
        uint32_t type;
    };
    std::vector<PrimBounds> _primBounds;

    // The buffer holding all primitives (shared with card)
    YDrawBuffer::Ptr _buffer;

    std::vector<uint32_t> _gridStaging;
    std::vector<YDrawGlyph> _glyphs;
    std::vector<uint32_t> _glyphSortedOrder;

    // Grid dimensions (computed by calculate())
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // Grid parameters
    float _cellSize = DEFAULT_CELL_SIZE;
    uint32_t _maxPrimsPerCell = DEFAULT_MAX_PRIMS_PER_CELL;

    // Appearance
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;

    // Font state
    FontManager::Ptr _fontManager;
    MsMsdfFont::Ptr _font;
    MsdfAtlas::Ptr _atlas;
    std::unordered_map<std::string, int> _fontIdCache;

    // Custom atlas
    MsdfAtlas::Ptr _customAtlas;
    GpuAllocator::Ptr _gpuAllocator;

    // TTF font metrics cache (fontId -> hhea ascender/descender)
    std::unordered_map<int, TtfMetrics> _ttfMetricsCache;

    // User-specified font ID mapping (user fontId -> atlas fontId)
    std::unordered_map<int, int> _userFontIdMap;

    // Buffer font ID -> atlas font ID mapping (populated during calculate())
    std::unordered_map<int, int> _bufferFontIdMap;

    // GPU state (managed by lifecycle methods)
    CardManager::Ptr _cardMgr;
    uint32_t _metaSlotIndex = 0;
    BufferHandle _primHandle = BufferHandle::invalid();
    BufferHandle _derivedHandle = BufferHandle::invalid();
    std::vector<uint32_t> _primWordOffsets;
    uint32_t _gpuGridOffset = 0;
    uint32_t _gpuGlyphOffset = 0;
    uint32_t _gpuAtlasHeaderOffset = 0;
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();
    uint32_t _allocatedAtlasW = 0;
    uint32_t _allocatedAtlasH = 0;

    // Pending images (from buffer, to be allocated in texture atlas)
    struct PendingImage {
        float x, y, w, h;
        uint32_t pixelWidth, pixelHeight;
        uint32_t layer;
        std::vector<uint8_t> pixels;
        TextureHandle texHandle = TextureHandle::invalid();
        uint32_t primId = 0;  // ID of Image primitive in buffer
    };
    std::vector<PendingImage> _pendingImages;

    bool _bufferDirty = true;
    bool _metadataDirty = true;

    // View state (set by card via setViewport/setView)
    uint32_t _viewWidthCells = 0;
    uint32_t _viewHeightCells = 0;
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
};

//=============================================================================
// Factory
//=============================================================================

Result<YDrawBuilder::Ptr> YDrawBuilder::createImpl(
    FontManager::Ptr fontManager, GpuAllocator::Ptr allocator,
    std::shared_ptr<YDrawBuffer> buffer,
    CardManager::Ptr cardMgr, uint32_t metaSlotIndex)
{
    // cardMgr may be null for transitional cards that still manage their own GPU state.
    // Lifecycle methods (declareBufferNeeds, allocateBuffers, etc.) validate it at call time.
    return Ok(Ptr(new YDrawBuilderImpl(std::move(fontManager),
                                        std::move(allocator),
                                        std::move(buffer),
                                        std::move(cardMgr), metaSlotIndex)));
}

Result<YDrawBuilder::Ptr> YDrawBuilder::createImpl(
    FontManager::Ptr fontManager, GpuAllocator::Ptr allocator,
    std::shared_ptr<YDrawBuffer> buffer)
{
    return Ok(Ptr(new YDrawBuilderImpl(std::move(fontManager),
                                        std::move(allocator),
                                        std::move(buffer),
                                        CardManager::Ptr{}, 0)));
}

} // namespace yetty
