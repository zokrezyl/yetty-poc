#include <yetty/ydraw-builder.h>
#include "../cards/hdraw/hdraw.h"  // For SDFPrimitive, SDFType
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

using card::SDFPrimitive;
using card::SDFType;

//=============================================================================
// AABB utilities
//=============================================================================

static void computeAABB(SDFPrimitive& prim) {
    float expand = prim.strokeWidth * 0.5f;

    switch (static_cast<SDFType>(prim.type)) {
        case SDFType::Circle: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = prim.params[2] + prim.round + expand;
            float hh = prim.params[3] + prim.round + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Segment: {
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - expand;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - expand;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + expand;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + expand;
            break;
        }
        case SDFType::Triangle: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier2: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier3: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) + expand;
            break;
        }
        case SDFType::Ellipse: {
            prim.aabbMinX = prim.params[0] - prim.params[2] - expand;
            prim.aabbMinY = prim.params[1] - prim.params[3] - expand;
            prim.aabbMaxX = prim.params[0] + prim.params[2] + expand;
            prim.aabbMaxY = prim.params[1] + prim.params[3] + expand;
            break;
        }
        case SDFType::Arc: {
            float r = std::max(prim.params[4], prim.params[5]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({prim.params[4], prim.params[5],
                                   prim.params[6], prim.params[7]});
            float hw = prim.params[2] + maxR + expand;
            float hh = prim.params[3] + maxR + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Rhombus: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Pentagon:
        case SDFType::Hexagon: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Star: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Pie: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Ring: {
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Heart: {
            float s = prim.params[2] * 1.5f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Cross: {
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = hw;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::RoundedX: {
            float s = prim.params[2] + prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Capsule: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - r;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - r;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + r;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + r;
            break;
        }
        case SDFType::Moon: {
            float r = std::max(prim.params[3], prim.params[4]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r + prim.params[2];
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Egg: {
            float r = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r + prim.params[2];
            break;
        }
        case SDFType::ChamferBox: {
            float hw = prim.params[2] + prim.params[4] + expand;
            float hh = prim.params[3] + prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::OrientedBox: {
            float maxX = std::max(prim.params[0], prim.params[2]);
            float minX = std::min(prim.params[0], prim.params[2]);
            float maxY = std::max(prim.params[1], prim.params[3]);
            float minY = std::min(prim.params[1], prim.params[3]);
            float th = prim.params[4] * 0.5f + expand;
            prim.aabbMinX = minX - th;
            prim.aabbMinY = minY - th;
            prim.aabbMaxX = maxX + th;
            prim.aabbMaxY = maxY + th;
            break;
        }
        case SDFType::Trapezoid: {
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Parallelogram: {
            float hw = prim.params[2] + std::fabs(prim.params[4]) + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::EquilateralTriangle:
        case SDFType::Octogon:
        case SDFType::Hexagram:
        case SDFType::Pentagram: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::IsoscelesTriangle: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - expand;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::UnevenCapsule: {
            float rMax = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - rMax;
            prim.aabbMinY = prim.params[1] - prim.params[2] - expand;
            prim.aabbMaxX = prim.params[0] + rMax;
            prim.aabbMaxY = prim.params[1] + prim.params[4] + prim.params[3] + expand;
            break;
        }
        case SDFType::CutDisk: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Horseshoe: {
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Vesica: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::OrientedVesica: {
            float w = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - w;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - w;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + w;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + w;
            break;
        }
        case SDFType::RoundedCross: {
            float s = (prim.params[2] + 1.0f) + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Parabola: {
            float s = 2.0f / prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::BlobbyCross: {
            float s = prim.params[2] * 1.5f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Tunnel: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Stairs: {
            float tw = prim.params[2] * prim.params[4] + expand;
            float th = prim.params[3] * prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - expand;
            prim.aabbMinY = prim.params[1] - expand;
            prim.aabbMaxX = prim.params[0] + tw;
            prim.aabbMaxY = prim.params[1] + th;
            break;
        }
        case SDFType::QuadraticCircle: {
            float s = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Hyperbola: {
            float s = prim.params[3] + expand + 1.0f;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::CoolS: {
            float s = prim.params[2] * 1.2f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::CircleWave: {
            float r = prim.params[3] * 2.0f + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::RotatedGlyph: {
            float gx = prim.params[0], gy = prim.params[1];
            float gw = prim.params[2], gh = prim.params[3];
            float angle = prim.params[4];
            float ca = std::cos(angle), sa = std::sin(angle);
            // Rotate 4 corners of the glyph quad, take AABB
            float cx[4] = {0, gw, gw, 0};
            float cy[4] = {0, 0, gh, gh};
            float minX = gx, maxX = gx, minY = gy, maxY = gy;
            for (int i = 0; i < 4; i++) {
                float rx = gx + cx[i] * ca - cy[i] * sa;
                float ry = gy + cx[i] * sa + cy[i] * ca;
                minX = std::min(minX, rx); maxX = std::max(maxX, rx);
                minY = std::min(minY, ry); maxY = std::max(maxY, ry);
            }
            prim.aabbMinX = minX;
            prim.aabbMinY = minY;
            prim.aabbMaxX = maxX;
            prim.aabbMaxY = maxY;
            break;
        }
        case SDFType::ColorWheel: {
            // params[0-1]=center, params[2]=outerR
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        // 3D primitives - not used for grid, set dummy AABB
        case SDFType::Sphere3D:
        case SDFType::Box3D:
        case SDFType::BoxFrame3D:
        case SDFType::Torus3D:
        case SDFType::CappedTorus3D:
        case SDFType::Cylinder3D:
        case SDFType::CappedCylinder3D:
        case SDFType::RoundedCylinder3D:
        case SDFType::VerticalCapsule3D:
        case SDFType::Cone3D:
        case SDFType::CappedCone3D:
        case SDFType::RoundCone3D:
        case SDFType::Plane3D:
        case SDFType::HexPrism3D:
        case SDFType::TriPrism3D:
        case SDFType::Octahedron3D:
        case SDFType::Pyramid3D:
        case SDFType::Ellipsoid3D:
        case SDFType::Rhombus3D:
        case SDFType::Link3D: {
            prim.aabbMinX = 0; prim.aabbMinY = 0;
            prim.aabbMaxX = 0; prim.aabbMaxY = 0;
            break;
        }
        default:
            prim.aabbMinX = -1e10f;
            prim.aabbMinY = -1e10f;
            prim.aabbMaxX = 1e10f;
            prim.aabbMaxY = 1e10f;
            break;
    }
}

// Static public API
/* static */ void YDrawBuilder::recomputeAABB(card::SDFPrimitive& prim) {
    computeAABB(prim);
}

//=============================================================================
// YDrawBuilderImpl
//=============================================================================

class YDrawBuilderImpl : public YDrawBuilder {
public:
    YDrawBuilderImpl(FontManager::Ptr fontManager, GpuAllocator::Ptr allocator)
        : _fontManager(std::move(fontManager))
        , _globalAllocator(std::move(allocator))
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
    // Primitive API
    //=========================================================================

    uint32_t addPrimitive(const card::SDFPrimitive& prim) override {
        if (prim.type >= 100) {
            _flags |= FLAG_HAS_3D;
        }
        SDFPrimitive p = prim;
        if (p.aabbMinX == 0 && p.aabbMaxX == 0) {
            computeAABB(p);
        }
        uint32_t idx = static_cast<uint32_t>(_primStaging.size());
        _primStaging.push_back(p);
        _gridStaging.clear();  // invalidate grid
        return idx;
    }

    uint32_t addCircle(float cx, float cy, float radius,
                       uint32_t fillColor, uint32_t strokeColor,
                       float strokeWidth, uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.layer = layer;
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = radius;
        prim.fillColor = fillColor;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    uint32_t addBox(float cx, float cy, float halfW, float halfH,
                    uint32_t fillColor, uint32_t strokeColor,
                    float strokeWidth, float round, uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Box);
        prim.layer = layer;
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = halfW;
        prim.params[3] = halfH;
        prim.fillColor = fillColor;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        prim.round = round;
        return addPrimitive(prim);
    }

    uint32_t addSegment(float x0, float y0, float x1, float y1,
                        uint32_t strokeColor, float strokeWidth,
                        uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Segment);
        prim.layer = layer;
        prim.params[0] = x0;
        prim.params[1] = y0;
        prim.params[2] = x1;
        prim.params[3] = y1;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    uint32_t addBezier2(float x0, float y0, float x1, float y1,
                        float x2, float y2,
                        uint32_t strokeColor, float strokeWidth,
                        uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Bezier2);
        prim.layer = layer;
        prim.params[0] = x0;
        prim.params[1] = y0;
        prim.params[2] = x1;
        prim.params[3] = y1;
        prim.params[4] = x2;
        prim.params[5] = y2;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    uint32_t addRotatedGlyph(float x, float y, float w, float h,
                             float angle, uint32_t glyphIndex,
                             uint32_t color) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::RotatedGlyph);
        prim.params[0] = x;
        prim.params[1] = y;
        prim.params[2] = w;
        prim.params[3] = h;
        prim.params[4] = angle;
        float glyphIdxF;
        std::memcpy(&glyphIdxF, &glyphIndex, sizeof(float));
        prim.params[5] = glyphIdxF;
        prim.params[6] = std::cos(angle);
        prim.params[7] = std::sin(angle);
        prim.fillColor = color;
        return addPrimitive(prim);
    }

    uint32_t addColorWheel(float cx, float cy, float outerR, float innerR,
                           float hue, float sat, float val,
                           float indicatorSize, uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::ColorWheel);
        prim.layer = layer;
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = outerR;
        prim.params[3] = innerR;
        prim.params[4] = hue;
        prim.params[5] = sat;
        prim.params[6] = val;
        prim.params[7] = indicatorSize;
        return addPrimitive(prim);
    }

    //=========================================================================
    // Text API
    //=========================================================================

    Result<void> addText(float x, float y, const std::string& text,
                         float fontSize, uint32_t color,
                         uint32_t layer) override {
        return addText(x, y, text, fontSize, color, layer, -1);
    }

    Result<void> addText(float x, float y, const std::string& text,
                         float fontSize, uint32_t color,
                         uint32_t layer, int fontId) override {
        // Resolve user-specified fontId to atlas-internal fontId
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
            // Decode UTF-8 codepoint
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
                                 float angleRadians, int fontId) override {
        // Rotated glyphs require the custom atlas (shader type 65 only
        // supports custom atlas sampling). Shared atlas is not supported.
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

            addRotatedGlyph(gx, gy, gw, gh, angleRadians, glyphIndex, color);
            cursorAdvance += glyph._advance * scale;
        }

        return Ok();
    }

    //=========================================================================
    // Text measurement & font metrics
    //=========================================================================

    float measureTextWidth(const std::string& text,
                           float fontSize, int fontId) override {
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

    float fontAscent(float fontSize, int fontId) override {
        auto it = _ttfMetricsCache.find(fontId);
        if (it != _ttfMetricsCache.end()) {
            bool useCustom = (fontId >= 0 && _customAtlas);
            MsdfAtlas* activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
            float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
            return it->second.ascender * (fontSize / atlasSize);
        }
        return fontSize * 0.8f;
    }

    float fontDescent(float fontSize, int fontId) override {
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
                     MsdfCdbProvider::Ptr provider) override {
        if (!_customAtlas) {
            auto atlasRes = MsdfAtlas::create(_globalAllocator);
            if (!atlasRes) {
                yerror("registerFont: failed to create custom atlas");
                return -1;
            }
            _customAtlas = *atlasRes;
            _flags |= FLAG_CUSTOM_ATLAS;
        }
        return _customAtlas->registerCdb(cdbPath, ttfPath, std::move(provider));
    }

    Result<int> addFont(const std::string& ttfPath) override {
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
            auto atlasRes = MsdfAtlas::create(_globalAllocator);
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

        // Cache TTF hhea metrics for correct ascent/descent
        float atlasSize = _customAtlas->getFontSize();
        _ttfMetricsCache[fontId] = readTtfMetrics(ttfPath, atlasSize);

        yinfo("addFont: registered '{}' on custom atlas (fontId={})", stem, fontId);
        return Ok(fontId);
    }

    Result<void> addFont(int userFontId, const std::string& ttfPath) override {
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
                             const std::string& /*name*/) override {
        // FNV-1a content hash → stable temp filename → stable CDB cache key
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

    void mapFontId(int userId, int atlasId) override {
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

    void clear() override {
        _primStaging.clear();
        _gridStaging.clear();
        _glyphs.clear();
    }

    uint32_t primitiveCount() const override {
        return static_cast<uint32_t>(_primStaging.size());
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

    const std::vector<card::SDFPrimitive>& primStaging() const override { return _primStaging; }
    std::vector<card::SDFPrimitive>& primStagingMut() override { return _primStaging; }
    const std::vector<uint32_t>& gridStaging() const override { return _gridStaging; }
    void clearGridStaging() override { _gridStaging.clear(); }
    const std::vector<YDrawGlyph>& glyphs() const override { return _glyphs; }
    std::vector<YDrawGlyph>& glyphsMut() override { return _glyphs; }

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

        if (startSorted < 0 || endSorted < 0 || _glyphSortedOrder.empty()) return;

        int32_t lo = std::min(startSorted, endSorted);
        int32_t hi = std::min(std::max(startSorted, endSorted),
                              static_cast<int32_t>(_glyphSortedOrder.size()) - 1);

        for (int32_t i = lo; i <= hi; i++) {
            _glyphs[_glyphSortedOrder[i]].glyphLayerFlags |=
                (static_cast<uint32_t>(GLYPH_FLAG_SELECTED) << 24);
        }
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
        // Compute scene bounds from staging
        if (!_hasExplicitBounds) {
            _sceneMinX = 1e10f; _sceneMinY = 1e10f;
            _sceneMaxX = -1e10f; _sceneMaxY = -1e10f;
            for (const auto& prim : _primStaging) {
                if (prim.type >= 100) continue;
                _sceneMinX = std::min(_sceneMinX, prim.aabbMinX);
                _sceneMinY = std::min(_sceneMinY, prim.aabbMinY);
                _sceneMaxX = std::max(_sceneMaxX, prim.aabbMaxX);
                _sceneMaxY = std::max(_sceneMaxY, prim.aabbMaxY);
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

        // Compute grid dimensions
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;
        uint32_t gridW = 0, gridH = 0;
        float cs = _cellSize;

        uint32_t num2DPrims = 0;
        for (const auto& prim : _primStaging) {
            if (prim.type < 100) num2DPrims++;
        }

        if (num2DPrims > 0 || !_glyphs.empty()) {
            if (cs <= 0.0f) {
                float primCs = 0.0f, glyphCs = 0.0f;
                if (num2DPrims > 0) {
                    float avgPrimArea = 0.0f;
                    for (const auto& prim : _primStaging) {
                        if (prim.type >= 100) continue;
                        float w = prim.aabbMaxX - prim.aabbMinX;
                        float h = prim.aabbMaxY - prim.aabbMinY;
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
            // Grid dimensions derived from content — no arbitrary per-axis cap.
            // Only cap total cell count to keep memory bounded (offset table = cells * 4 bytes).
            gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
            gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
            constexpr uint32_t MAX_TOTAL_CELLS = 4u * 1024u * 1024u; // 4M cells = 16MB offset table
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
              gridW, gridH, cs, _primStaging.size(), _glyphs.size(),
              _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);

        // Build variable-length grid into staging
        // Layout: [off0][off1]...[offN] [count0,e,e,...] [count1,e,...] ...
        // Pass 1: count entries per cell
        uint32_t numCells = gridW * gridH;
        std::vector<uint32_t> cellCounts(numCells, 0);

        for (const auto& prim : _primStaging) {
            if (prim.type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((prim.aabbMinX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((prim.aabbMaxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((prim.aabbMinY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((prim.aabbMaxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
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
        uint32_t pos = numCells; // packed data starts after offset table
        _gridStaging.resize(numCells); // will grow below
        for (uint32_t i = 0; i < numCells; i++) {
            _gridStaging[i] = pos;
            pos += 1 + cellCounts[i]; // count + entries
        }
        _gridStaging.resize(pos, 0);

        // Initialize packed counts to 0
        for (uint32_t i = 0; i < numCells; i++) {
            _gridStaging[_gridStaging[i]] = 0;
        }

        // Pass 2: fill entries
        for (uint32_t primIdx = 0; primIdx < static_cast<uint32_t>(_primStaging.size()); primIdx++) {
            const auto& prim = _primStaging[primIdx];
            if (prim.type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((prim.aabbMinX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((prim.aabbMaxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((prim.aabbMinY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((prim.aabbMaxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
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
    }

    //=========================================================================
    // GPU-resident primitive operations
    //=========================================================================

    void computeSceneBoundsFromPrims(
        const card::SDFPrimitive* prims, uint32_t count) override
    {
        if (_hasExplicitBounds) return;

        _sceneMinX = 1e10f; _sceneMinY = 1e10f;
        _sceneMaxX = -1e10f; _sceneMaxY = -1e10f;

        for (uint32_t i = 0; i < count; i++) {
            if (prims[i].type >= 100) continue;
            _sceneMinX = std::min(_sceneMinX, prims[i].aabbMinX);
            _sceneMinY = std::min(_sceneMinY, prims[i].aabbMinY);
            _sceneMaxX = std::max(_sceneMaxX, prims[i].aabbMaxX);
            _sceneMaxY = std::max(_sceneMaxY, prims[i].aabbMaxY);
        }

        for (const auto& glyph : _glyphs) {
            _sceneMinX = std::min(_sceneMinX, glyph.x);
            _sceneMinY = std::min(_sceneMinY, glyph.y);
            _sceneMaxX = std::max(_sceneMaxX, glyph.x + glyph.width());
            _sceneMaxY = std::max(_sceneMaxY, glyph.y + glyph.height());
        }

        float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
        float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
        _sceneMinX -= padX; _sceneMinY -= padY;
        _sceneMaxX += padX; _sceneMaxY += padY;

        if (_sceneMinX >= _sceneMaxX) { _sceneMinX = 0; _sceneMaxX = 100; }
        if (_sceneMinY >= _sceneMaxY) { _sceneMinY = 0; _sceneMaxY = 100; }
    }

    GridDims computeGridDims(
        const card::SDFPrimitive* prims, uint32_t count) override
    {
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;
        uint32_t gridW = 0, gridH = 0;
        float cs = _cellSize;

        uint32_t num2DPrims = 0;
        for (uint32_t i = 0; i < count; i++) {
            if (prims[i].type < 100) num2DPrims++;
        }

        if (num2DPrims > 0 || !_glyphs.empty()) {
            if (cs <= 0.0f) {
                float primCs = 0.0f, glyphCs = 0.0f;
                if (num2DPrims > 0) {
                    float avgPrimArea = 0.0f;
                    for (uint32_t i = 0; i < count; i++) {
                        if (prims[i].type >= 100) continue;
                        float w = prims[i].aabbMaxX - prims[i].aabbMinX;
                        float h = prims[i].aabbMaxY - prims[i].aabbMinY;
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

        _cellSize = cs;
        return {gridW, gridH, cs};
    }

    void buildGridFromPrims(
        const card::SDFPrimitive* prims, uint32_t count) override
    {
        uint32_t gridW = _gridWidth;
        uint32_t gridH = _gridHeight;
        float cs = _cellSize;
        if (gridW == 0 || gridH == 0) return;

        uint32_t numCells = gridW * gridH;
        std::vector<uint32_t> cellCounts(numCells, 0);

        // Pass 1: count entries per cell
        for (uint32_t i = 0; i < count; i++) {
            if (prims[i].type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((prims[i].aabbMinX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((prims[i].aabbMaxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((prims[i].aabbMinY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((prims[i].aabbMaxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
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

        // Compute offsets
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
        for (uint32_t primIdx = 0; primIdx < count; primIdx++) {
            if (prims[primIdx].type >= 100) continue;
            uint32_t cMinX = static_cast<uint32_t>(std::clamp((prims[primIdx].aabbMinX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMaxX = static_cast<uint32_t>(std::clamp((prims[primIdx].aabbMaxX - _sceneMinX) / cs, 0.0f, float(gridW - 1)));
            uint32_t cMinY = static_cast<uint32_t>(std::clamp((prims[primIdx].aabbMinY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
            uint32_t cMaxY = static_cast<uint32_t>(std::clamp((prims[primIdx].aabbMaxY - _sceneMinY) / cs, 0.0f, float(gridH - 1)));
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
    }

private:
    // Staging vectors
    std::vector<card::SDFPrimitive> _primStaging;
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
    GpuAllocator::Ptr _globalAllocator;

    // TTF font metrics cache (fontId -> hhea ascender/descender)
    std::unordered_map<int, TtfMetrics> _ttfMetricsCache;

    // User-specified font ID mapping (user fontId -> atlas fontId)
    std::unordered_map<int, int> _userFontIdMap;
};

//=============================================================================
// Factory
//=============================================================================

Result<YDrawBuilder::Ptr> YDrawBuilder::createImpl(
    FontManager::Ptr fontManager, GpuAllocator::Ptr allocator)
{
    return Ok(Ptr(new YDrawBuilderImpl(std::move(fontManager),
                                        std::move(allocator))));
}

} // namespace yetty
