// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "triangulate.h"
#include "ypaint-buffer.h"
#include "ypaint-types.gen.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <unordered_map>
#include <yetty/card-texture-manager.h>
#include <yetty/msdf-glyph-data.h> // For GlyphMetadataGPU
#include <yetty/ypaint/painter.h>
#include <ytrace/ytrace.hpp>

namespace yetty::ypaint {

//=============================================================================
// Read font ascender/descender from TTF hhea + head tables (no FreeType)
//=============================================================================
struct TtfMetrics {
  float ascender;
  float descender;
};

static TtfMetrics readTtfMetrics(const std::string &path, float fontSize) {
  TtfMetrics fallback{fontSize * 0.8f, fontSize * 0.2f};
  std::ifstream f(path, std::ios::binary);
  if (!f)
    return fallback;

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
    uint32_t off =
        (static_cast<uint32_t>(r16(entry + 8)) << 16) | r16(entry + 10);
    if (std::memcmp(tag, "hhea", 4) == 0)
      hheaOff = off;
    if (std::memcmp(tag, "head", 4) == 0)
      headOff = off;
  }
  if (!hheaOff || !headOff)
    return fallback;

  uint16_t upm = r16(headOff + 18);
  int16_t asc = ri16(hheaOff + 4);
  int16_t desc = ri16(hheaOff + 6);
  if (upm == 0)
    return fallback;

  float scale = fontSize / static_cast<float>(upm);
  return {asc * scale, std::abs(desc) * scale};
}

using card::SDFType;

//=============================================================================
// AABB utilities
//=============================================================================

// Compute AABB directly from variable-length buffer words.
// Layout: word[0]=type, word[1]=layer, word[2..wc-5]=geometry params,
//         word[wc-4]=fillColor, word[wc-3]=strokeColor, word[wc-2]=strokeWidth,
//         word[wc-1]=round
// p[i] aliases data[2+i] (geometry params).
static void computeAABB(const float *data, uint32_t wc, float &aabbMinX,
                        float &aabbMinY, float &aabbMaxX, float &aabbMaxY) {
  uint32_t type = sdf::detail::read_u32(data, 0);
  const float *p = data + 2; // geometry params
  float strokeWidth = data[wc - 2];
  float round_ = data[wc - 1];
  float expand = strokeWidth * 0.5f;

  switch (static_cast<SDFType>(type)) {
  case SDFType::Circle: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Box: {
    float hw = p[2] + round_ + expand;
    float hh = p[3] + round_ + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
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
    aabbMinX = p[0] - p[2] - expand;
    aabbMinY = p[1] - p[3] - expand;
    aabbMaxX = p[0] + p[2] + expand;
    aabbMaxY = p[1] + p[3] + expand;
    break;
  }
  case SDFType::Arc: {
    float r = std::max(p[4], p[5]) + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::RoundedBox: {
    float maxR = std::max({p[4], p[5], p[6], p[7]});
    float hw = p[2] + maxR + expand;
    float hh = p[3] + maxR + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::Rhombus: {
    float hw = p[2] + expand;
    float hh = p[3] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::Pentagon:
  case SDFType::Hexagon: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Star: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Pie: {
    float r = p[4] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Ring: {
    float r = p[4] + p[5] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Heart: {
    float s = p[2] * 1.5f + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::Cross: {
    float hw = std::max(p[2], p[3]) + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hw;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hw;
    break;
  }
  case SDFType::RoundedX: {
    float s = p[2] + p[3] + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::Capsule: {
    float r = p[4] + expand;
    aabbMinX = std::min(p[0], p[2]) - r;
    aabbMinY = std::min(p[1], p[3]) - r;
    aabbMaxX = std::max(p[0], p[2]) + r;
    aabbMaxY = std::max(p[1], p[3]) + r;
    break;
  }
  case SDFType::Moon: {
    float r = std::max(p[3], p[4]) + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r + p[2];
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Egg: {
    float r = std::max(p[2], p[3]) + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r + p[2];
    break;
  }
  case SDFType::ChamferBox: {
    float hw = p[2] + p[4] + expand;
    float hh = p[3] + p[4] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::OrientedBox: {
    float th = p[4] * 0.5f + expand;
    aabbMinX = std::min(p[0], p[2]) - th;
    aabbMinY = std::min(p[1], p[3]) - th;
    aabbMaxX = std::max(p[0], p[2]) + th;
    aabbMaxY = std::max(p[1], p[3]) + th;
    break;
  }
  case SDFType::Trapezoid: {
    float hw = std::max(p[2], p[3]) + expand;
    float hh = p[4] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::Parallelogram: {
    float hw = p[2] + std::fabs(p[4]) + expand;
    float hh = p[3] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::EquilateralTriangle:
  case SDFType::Octogon:
  case SDFType::Hexagram:
  case SDFType::Pentagram: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::IsoscelesTriangle: {
    float hw = p[2] + expand;
    float hh = p[3] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - expand;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::UnevenCapsule: {
    float rMax = std::max(p[2], p[3]) + expand;
    aabbMinX = p[0] - rMax;
    aabbMinY = p[1] - p[2] - expand;
    aabbMaxX = p[0] + rMax;
    aabbMaxY = p[1] + p[4] + p[3] + expand;
    break;
  }
  case SDFType::CutDisk: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Horseshoe: {
    float r = p[4] + p[5] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::Vesica: {
    float hw = p[2] + expand;
    float hh = p[3] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::OrientedVesica: {
    float w = p[4] + expand;
    aabbMinX = std::min(p[0], p[2]) - w;
    aabbMinY = std::min(p[1], p[3]) - w;
    aabbMaxX = std::max(p[0], p[2]) + w;
    aabbMaxY = std::max(p[1], p[3]) + w;
    break;
  }
  case SDFType::RoundedCross: {
    float s = (p[2] + 1.0f) + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::Parabola: {
    float s = 2.0f / p[2] + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::BlobbyCross: {
    float s = p[2] * 1.5f + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::Tunnel: {
    float hw = p[2] + expand;
    float hh = p[3] + expand;
    aabbMinX = p[0] - hw;
    aabbMinY = p[1] - hh;
    aabbMaxX = p[0] + hw;
    aabbMaxY = p[1] + hh;
    break;
  }
  case SDFType::Stairs: {
    float tw = p[2] * p[4] + expand;
    float th = p[3] * p[4] + expand;
    aabbMinX = p[0] - expand;
    aabbMinY = p[1] - expand;
    aabbMaxX = p[0] + tw;
    aabbMaxY = p[1] + th;
    break;
  }
  case SDFType::QuadraticCircle: {
    float s = p[2] + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::Hyperbola: {
    float s = p[3] + expand + 1.0f;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::CoolS: {
    float s = p[2] * 1.2f + expand;
    aabbMinX = p[0] - s;
    aabbMinY = p[1] - s;
    aabbMaxX = p[0] + s;
    aabbMaxY = p[1] + s;
    break;
  }
  case SDFType::CircleWave: {
    float r = p[3] * 2.0f + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
    break;
  }
  case SDFType::RotatedGlyph: {
    float gx = p[0], gy = p[1], gw = p[2], gh = p[3], angle = p[4];
    float ca = std::cos(angle), sa = std::sin(angle);
    float cx[4] = {0, gw, gw, 0}, cy[4] = {0, 0, gh, gh};
    aabbMinX = gx;
    aabbMaxX = gx;
    aabbMinY = gy;
    aabbMaxY = gy;
    for (int i = 0; i < 4; i++) {
      float rx = gx + cx[i] * ca - cy[i] * sa;
      float ry = gy + cx[i] * sa + cy[i] * ca;
      aabbMinX = std::min(aabbMinX, rx);
      aabbMaxX = std::max(aabbMaxX, rx);
      aabbMinY = std::min(aabbMinY, ry);
      aabbMaxY = std::max(aabbMaxY, ry);
    }
    break;
  }
  case SDFType::ColorWheel: {
    float r = p[2] + expand;
    aabbMinX = p[0] - r;
    aabbMinY = p[1] - r;
    aabbMaxX = p[0] + r;
    aabbMaxY = p[1] + r;
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
    aabbMinX = 0;
    aabbMinY = 0;
    aabbMaxX = 0;
    aabbMaxY = 0;
    break;
  }
  case SDFType::Polygon: {
    // Polygon: header(7) + vertices(vertexCount*2)
    uint32_t vertexCount = sdf::detail::read_u32(data, 2);
    strokeWidth = data[5];
    float expand = strokeWidth * 0.5f;
    const float *verts = data + 7;
    if (vertexCount > 0) {
      aabbMinX = verts[0];
      aabbMaxX = verts[0];
      aabbMinY = verts[1];
      aabbMaxY = verts[1];
      for (uint32_t i = 1; i < vertexCount; ++i) {
        float vx = verts[i * 2];
        float vy = verts[i * 2 + 1];
        aabbMinX = std::min(aabbMinX, vx);
        aabbMinY = std::min(aabbMinY, vy);
        aabbMaxX = std::max(aabbMaxX, vx);
        aabbMaxY = std::max(aabbMaxY, vy);
      }
      aabbMinX -= expand;
      aabbMinY -= expand;
      aabbMaxX += expand;
      aabbMaxY += expand;
    } else {
      aabbMinX = 0;
      aabbMinY = 0;
      aabbMaxX = 0;
      aabbMaxY = 0;
    }
    break;
  }
  case SDFType::PolygonGroup: {
    // PolygonGroup: header(8) + contourStarts(contourCount) +
    // vertices(vertexCount*2)
    uint32_t vertexCount = sdf::detail::read_u32(data, 2);
    uint32_t contourCount = sdf::detail::read_u32(data, 3);
    strokeWidth = data[6];
    float expand = strokeWidth * 0.5f;
    const float *verts = data + 8 + contourCount;
    if (vertexCount > 0) {
      aabbMinX = verts[0];
      aabbMaxX = verts[0];
      aabbMinY = verts[1];
      aabbMaxY = verts[1];
      for (uint32_t i = 1; i < vertexCount; ++i) {
        float vx = verts[i * 2];
        float vy = verts[i * 2 + 1];
        aabbMinX = std::min(aabbMinX, vx);
        aabbMinY = std::min(aabbMinY, vy);
        aabbMaxX = std::max(aabbMaxX, vx);
        aabbMaxY = std::max(aabbMaxY, vy);
      }
      aabbMinX -= expand;
      aabbMinY -= expand;
      aabbMaxX += expand;
      aabbMaxY += expand;
    } else {
      aabbMinX = 0;
      aabbMinY = 0;
      aabbMaxX = 0;
      aabbMaxY = 0;
    }
    break;
  }
  case SDFType::LinearGradientBox: {
    // Layout: type(0), layer(1), cx(2), cy(3), hw(4), hh(5), gx1(6), gy1(7),
    // gx2(8), gy2(9),
    //         color1(10), color2(11), strokeColor(12), strokeWidth(13),
    //         round(14)
    float cx = p[0], cy = p[1], hw = p[2], hh = p[3];
    strokeWidth = data[13];
    float r = data[14]; // round
    float expand = strokeWidth * 0.5f;
    aabbMinX = cx - hw - r - expand;
    aabbMinY = cy - hh - r - expand;
    aabbMaxX = cx + hw + r + expand;
    aabbMaxY = cy + hh + r + expand;
    break;
  }
  case SDFType::LinearGradientCircle: {
    // Layout: type(0), layer(1), cx(2), cy(3), r(4), ...
    float cx = p[0], cy = p[1], r = p[2];
    strokeWidth = data[12];
    float expand = strokeWidth * 0.5f;
    aabbMinX = cx - r - expand;
    aabbMinY = cy - r - expand;
    aabbMaxX = cx + r + expand;
    aabbMaxY = cy + r + expand;
    break;
  }
  case SDFType::RadialGradientCircle: {
    // Layout: type(0), layer(1), cx(2), cy(3), r(4), ...
    float cx = p[0], cy = p[1], r = p[2];
    strokeWidth = data[11];
    float expand = strokeWidth * 0.5f;
    aabbMinX = cx - r - expand;
    aabbMinY = cy - r - expand;
    aabbMaxX = cx + r + expand;
    aabbMaxY = cy + r + expand;
    break;
  }
  default:
    aabbMinX = -1e10f;
    aabbMinY = -1e10f;
    aabbMaxX = 1e10f;
    aabbMaxY = 1e10f;
    break;
  }
}

//=============================================================================
// PainterImpl
//=============================================================================

class PainterImpl : public Painter {
public:
  PainterImpl(FontManager::Ptr fontManager, GpuAllocator::Ptr allocator,
              CardManager::Ptr cardMgr, uint32_t metaSlotIndex,
              bool scrollingMode = false)
      : _fontManager(std::move(fontManager)),
        _gpuAllocator(std::move(allocator)), _cardMgr(std::move(cardMgr)),
        _metaSlotIndex(metaSlotIndex), _scrollingMode(scrollingMode) {
    if (_fontManager) {
      _font = _fontManager->getDefaultMsMsdfFont();
      if (_font) {
        _atlas = _font->atlas();
      }
    }
  }

  ~PainterImpl() override = default;

  //=========================================================================
  // Text API
  //=========================================================================

  Result<void> addText(float x, float y, const std::string &text,
                       float fontSize, uint32_t color, uint32_t layer) {
    return addText(x, y, text, fontSize, color, layer, -1);
  }

  Result<void> addText(float x, float y, const std::string &text,
                       float fontSize, uint32_t color, uint32_t layer,
                       int fontId) {
    int resolvedFontId = fontId;
    if (fontId >= 0) {
      auto it = _userFontIdMap.find(fontId);
      if (it != _userFontIdMap.end()) {
        resolvedFontId = it->second;
      }
    }
    bool useCustom = (resolvedFontId >= 0 && _customAtlas);
    // Custom fonts are NOT fully supported in scrolling mode - the scrolling
    // shader only samples from the shared fontTexture, not the custom atlas.
    // Fall back to shared atlas with a warning.
    if (useCustom && _scrollingMode) {
      ywarn("addText: custom fonts not supported in scrolling mode, using "
            "default font");
      useCustom = false;
    }
    MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
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

    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
    const uint8_t *end = ptr + text.size();

    while (ptr < end) {
      uint32_t codepoint = 0;
      if ((*ptr & 0x80) == 0) {
        codepoint = *ptr++;
      } else if ((*ptr & 0xE0) == 0xC0) {
        codepoint = (*ptr++ & 0x1F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF0) == 0xE0) {
        codepoint = (*ptr++ & 0x0F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF8) == 0xF0) {
        codepoint = (*ptr++ & 0x07) << 18;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
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

      const auto &metadata = activeAtlas->getGlyphMetadata();
      if (glyphIndex >= metadata.size()) {
        cursorX += fontSize * 0.5f;
        continue;
      }

      const auto &glyph = metadata[glyphIndex];

      YPaintGlyph posGlyph = {};
      posGlyph.x = cursorX + glyph._bearingX * scale;
      posGlyph.y = y - glyph._bearingY * scale;
      posGlyph.setSize(glyph._sizeX * scale, glyph._sizeY * scale);
      posGlyph.setGlyphLayerFlags(static_cast<uint16_t>(glyphIndex),
                                  static_cast<uint8_t>(layer),
                                  useCustom ? 1u : 0u);
      posGlyph.color = color;

      _glyphs.push_back(posGlyph);
      glyphsAdded++;

      cursorX += glyph._advance * scale;
    }

    if (glyphsAdded > 0) {
      _gridStaging.clear(); // invalidate grid
    }

    return Ok();
  }

  Result<void> addRotatedText(float x, float y, const std::string &text,
                              float fontSize, uint32_t color,
                              float angleRadians, int fontId) {
    // Rotated text is NOT supported in scrolling mode - the scrolling shader
    // doesn't have SDF_ROTATED_GLYPH handling or custom atlas texture binding.
    // Log a warning and skip rendering (return Ok to not break the flow).
    if (_scrollingMode) {
      ywarn("addRotatedText: rotated text not supported in scrolling mode, "
            "text='{}' ignored",
            text);
      return Ok();
    }
    int resolvedFontId = fontId;
    if (fontId >= 0) {
      auto it = _userFontIdMap.find(fontId);
      if (it != _userFontIdMap.end())
        resolvedFontId = it->second;
    }
    if (resolvedFontId < 0 || !_customAtlas) {
      return Err<void>("addRotatedText: requires a custom font (rotated "
                       "glyphs only work with custom atlas)");
    }
    if (text.empty())
      return Ok();
    MsdfAtlas *activeAtlas = _customAtlas.get();

    float fontBaseSize = activeAtlas->getFontSize();
    float scale = fontSize / fontBaseSize;
    float cosA = std::cos(angleRadians);
    float sinA = std::sin(angleRadians);
    float cursorAdvance = 0.0f;

    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
    const uint8_t *end = ptr + text.size();

    while (ptr < end) {
      uint32_t codepoint = 0;
      if ((*ptr & 0x80) == 0) {
        codepoint = *ptr++;
      } else if ((*ptr & 0xE0) == 0xC0) {
        codepoint = (*ptr++ & 0x1F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF0) == 0xE0) {
        codepoint = (*ptr++ & 0x0F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF8) == 0xF0) {
        codepoint = (*ptr++ & 0x07) << 18;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else {
        ptr++;
        continue;
      }

      uint32_t glyphIndex = _customAtlas->loadGlyph(resolvedFontId, codepoint);
      const auto &metadata = activeAtlas->getGlyphMetadata();
      if (glyphIndex >= metadata.size()) {
        cursorAdvance += fontSize * 0.5f;
        continue;
      }

      const auto &glyph = metadata[glyphIndex];
      float bx = glyph._bearingX * scale;
      float by = -glyph._bearingY * scale;
      float gw = glyph._sizeX * scale;
      float gh = glyph._sizeY * scale;

      float gx = x + (cursorAdvance + bx) * cosA - by * sinA;
      float gy = y + (cursorAdvance + bx) * sinA + by * cosA;

      // Build RotatedGlyph primitive (15 words with gridOffset prefix)
      // Layout: [gridOffset][type=65][layer][x][y][scaleX][scaleY][angle]
      //         [glyphIndex][cosAngle][sinAngle][fillColor][strokeColor][strokeWidth][round]
      std::vector<float> primData(15);
      uint16_t gridOffsetCol = _scrollingMode ? _cursorCol : 0;
      uint16_t gridOffsetRow = _scrollingMode ? _cursorRow : 0;
      uint32_t packedOffset = static_cast<uint32_t>(gridOffsetCol) |
                              (static_cast<uint32_t>(gridOffsetRow) << 16);
      std::memcpy(&primData[0], &packedOffset, sizeof(uint32_t));
      sdf::detail::write_u32(primData.data(), 1, 65u); // RotatedGlyph type
      sdf::detail::write_u32(primData.data(), 2, 0u);  // layer 0
      primData[3] = gx;
      primData[4] = gy;
      primData[5] = gw; // width in pixels (shader expects gw, not scale)
      primData[6] = gh; // height in pixels (shader expects gh, not scale)
      primData[7] = angleRadians;
      sdf::detail::write_u32(primData.data(), 8, glyphIndex);
      primData[9] = cosA;
      primData[10] = sinA;
      sdf::detail::write_u32(primData.data(), 11, color); // fillColor
      sdf::detail::write_u32(primData.data(), 12, 0u);    // strokeColor
      primData[13] = 0.0f;                                // strokeWidth
      primData[14] = 0.0f;                                // round

      // Compute AABB for grid placement
      // Rotated glyph bounding box: corners rotated by angle
      float halfW = gw * 0.5f;
      float halfH = gh * 0.5f;
      float corners[4][2] = {
          {-halfW, -halfH}, {halfW, -halfH}, {halfW, halfH}, {-halfW, halfH}};
      float minX = gx, maxX = gx, minY = gy, maxY = gy;
      for (int c = 0; c < 4; c++) {
        float rx = gx + corners[c][0] * cosA - corners[c][1] * sinA;
        float ry = gy + corners[c][0] * sinA + corners[c][1] * cosA;
        minX = std::min(minX, rx);
        maxX = std::max(maxX, rx);
        minY = std::min(minY, ry);
        maxY = std::max(maxY, ry);
      }

      // Compute which lines this primitive spans
      // Use sceneMinY as base for grid line calculation
      // Guard against division by zero if cell size not yet computed
      float baseY = (_sceneMinY < 1e9f) ? _sceneMinY : 0.0f;
      float effectiveCellSizeY =
          (_cellSizeY > 0.0f) ? _cellSizeY : 20.0f; // default fallback
      int32_t localMinLine =
          static_cast<int32_t>(std::floor((minY - baseY) / effectiveCellSizeY));
      int32_t localMaxLine =
          static_cast<int32_t>(std::floor((maxY - baseY) / effectiveCellSizeY));
      int32_t primMinLineSigned =
          static_cast<int32_t>(gridOffsetRow) + localMinLine;
      int32_t primMaxLineSigned =
          static_cast<int32_t>(gridOffsetRow) + localMaxLine;
      primMinLineSigned = std::max(primMinLineSigned, 0);
      primMaxLineSigned = std::max(primMaxLineSigned, 0);
      uint32_t primMinLine = static_cast<uint32_t>(primMinLineSigned);
      uint32_t primMaxLine = static_cast<uint32_t>(primMaxLineSigned);

      // Ensure enough lines exist
      while (_scrollLines.size() <= primMaxLine) {
        ScrollLine line;
        line.cells.resize(_gridWidth > 0 ? _gridWidth : 1);
        _scrollLines.push_back(std::move(line));
      }
      primMinLine =
          std::min(primMinLine, static_cast<uint32_t>(_scrollLines.size() - 1));
      primMaxLine =
          std::min(primMaxLine, static_cast<uint32_t>(_scrollLines.size() - 1));

      // Store primitive in base line
      uint32_t baseLine = primMaxLine;
      uint16_t primIndex =
          static_cast<uint16_t>(_scrollLines[baseLine].prims.size());
      _scrollLines[baseLine].prims.push_back(std::move(primData));

      // Add grid cell references
      uint32_t cellMinX = cellX(minX);
      uint32_t cellMaxX = cellX(maxX);
      for (uint32_t lineIdx = primMinLine; lineIdx <= primMaxLine; lineIdx++) {
        while (_scrollLines[lineIdx].cells.size() <= cellMaxX) {
          _scrollLines[lineIdx].cells.push_back(ScrollGridCell{});
        }
        uint16_t linesAhead = static_cast<uint16_t>(baseLine - lineIdx);
        for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
          _scrollLines[lineIdx].cells[cx].refs.push_back(
              {linesAhead, primIndex});
        }
      }

      // Update scene bounds
      if (!_hasExplicitBounds) {
        _sceneMinX = std::min(_sceneMinX, minX);
        _sceneMinY = std::min(_sceneMinY, minY);
        _sceneMaxX = std::max(_sceneMaxX, maxX);
        _sceneMaxY = std::max(_sceneMaxY, maxY);
      }

      _gridStagingDirty = true;
      _bufferDirty = true;
      cursorAdvance += glyph._advance * scale;
    }

    return Ok();
  }

  //=========================================================================
  // Text measurement & font metrics
  //=========================================================================

  float measureTextWidth(const std::string &text, float fontSize,
                         int fontId) override {
    int resolvedFontId = fontId;
    if (fontId >= 0) {
      auto it = _userFontIdMap.find(fontId);
      if (it != _userFontIdMap.end())
        resolvedFontId = it->second;
    }
    bool useCustom = (resolvedFontId >= 0 && _customAtlas);
    MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
    if (!activeAtlas || text.empty())
      return 0.0f;

    float fontBaseSize = activeAtlas->getFontSize();
    float scale = fontSize / fontBaseSize;

    float width = 0.0f;
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(text.data());
    const uint8_t *end = ptr + text.size();

    while (ptr < end) {
      uint32_t codepoint = 0;
      if ((*ptr & 0x80) == 0) {
        codepoint = *ptr++;
      } else if ((*ptr & 0xE0) == 0xC0) {
        codepoint = (*ptr++ & 0x1F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF0) == 0xE0) {
        codepoint = (*ptr++ & 0x0F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
      } else if ((*ptr & 0xF8) == 0xF0) {
        codepoint = (*ptr++ & 0x07) << 18;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 12;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F) << 6;
        if (ptr < end)
          codepoint |= (*ptr++ & 0x3F);
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

      const auto &metadata = activeAtlas->getGlyphMetadata();
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
      MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
      float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
      return it->second.ascender * (fontSize / atlasSize);
    }
    return fontSize * 0.8f;
  }

  float fontDescent(float fontSize, int fontId) override {
    auto it = _ttfMetricsCache.find(fontId);
    if (it != _ttfMetricsCache.end()) {
      bool useCustom = (fontId >= 0 && _customAtlas);
      MsdfAtlas *activeAtlas = useCustom ? _customAtlas.get() : _atlas.get();
      float atlasSize = activeAtlas ? activeAtlas->getFontSize() : 32.0f;
      return it->second.descender * (fontSize / atlasSize);
    }
    return fontSize * 0.2f;
  }

  //=========================================================================
  // Font API
  //=========================================================================

  int registerFont(const std::string &cdbPath, const std::string &ttfPath,
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

  Result<int> addFont(const std::string &ttfPath) {
    ydebug("addFont: ttfPath='{}'", ttfPath);
    if (!_fontManager) {
      yerror("addFont: _fontManager is NULL!");
      return Err<int>("addFont: no font manager");
    }

    auto stem = std::filesystem::path(ttfPath).stem().string();
    auto cacheDir = _fontManager->getCacheDir();
    auto cdbPath = cacheDir + "/" + stem + ".cdb";

    auto it = _fontIdCache.find(cdbPath);
    if (it != _fontIdCache.end())
      return Ok(it->second);

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

    ydebug("addFont: registered '{}' on custom atlas (fontId={})", stem,
           fontId);
    return Ok(fontId);
  }

  Result<void> addFont(int userFontId, const std::string &ttfPath) {
    auto result = addFont(ttfPath);
    if (!result) {
      return Err<void>("addFont(id=" + std::to_string(userFontId) +
                       "): " + result.error().message());
    }
    int atlasId = *result;
    _userFontIdMap[userFontId] = atlasId;
    ydebug("addFont: mapped user fontId {} -> atlas fontId {}", userFontId,
           atlasId);
    return Ok();
  }

  Result<int> addFontData(const uint8_t *data, size_t size,
                          const std::string &name) {
    ydebug("addFontData: name='{}' size={}", name, size);
    if (!_fontManager) {
      yerror("addFontData: _fontManager is NULL!");
      return Err<int>("addFontData: no font manager");
    }
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
      out.write(reinterpret_cast<const char *>(data),
                static_cast<std::streamsize>(size));
    }

    return addFont(ttfPath);
  }

  void mapFontId(int userId, int atlasId) { _userFontIdMap[userId] = atlasId; }

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
    // Unified: count primitives from _scrollLines (used for both modes)
    uint32_t count = 0;
    for (const auto &line : _scrollLines) {
      count += static_cast<uint32_t>(line.prims.size());
    }
    return count;
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

  void setGridCellSize(float sizeX, float sizeY) override {
    _cellSizeX = sizeX;
    _cellSizeY = sizeY;
  }
  float gridCellSizeX() const override { return _cellSizeX; }
  float gridCellSizeY() const override { return _cellSizeY; }

  void setFlags(uint32_t flags) override { _flags = flags; }
  void addFlags(uint32_t flags) override { _flags |= flags; }
  uint32_t flags() const override { return _flags; }

  void setMaxPrimsPerCell(uint32_t max) override { _maxPrimsPerCell = max; }
  uint32_t maxPrimsPerCell() const override { return _maxPrimsPerCell; }

  //=========================================================================
  // Staging data access
  //=========================================================================

  const std::vector<uint32_t> &gridStaging() const override {
    if (_gridStagingDirty) {
      const_cast<PainterImpl *>(this)->rebuildPackedGrid();
    }
    return _gridStaging;
  }
  void clearGridStaging() override { _gridStaging.clear(); }
  const std::vector<YPaintGlyph> &glyphs() const override { return _glyphs; }
  std::vector<YPaintGlyph> &glyphsMut() override { return _glyphs; }

  void buildPrimStaging(std::vector<uint32_t> &out) const override {
    // Use cached version if buffer not dirty
    if (!_bufferDirty && !_primStagingCache.empty()) {
      out = _primStagingCache;
      return;
    }

    // Primitives are stored in _scrollLines (line-based storage)
    // Count total primitives and words
    uint32_t primCount = 0;
    uint32_t totalWords = 0;
    for (const auto &line : _scrollLines) {
      for (const auto &prim : line.prims) {
        primCount++;
        totalWords += static_cast<uint32_t>(prim.size());
      }
    }

    if (primCount == 0) {
      out.clear();
      const_cast<PainterImpl *>(this)->_primStagingCache.clear();
      return;
    }

    out.resize(primCount + totalWords);
    uint32_t dataOffset = 0;
    uint32_t primIdx = 0;
    for (const auto &line : _scrollLines) {
      for (const auto &prim : line.prims) {
        out[primIdx] = dataOffset;
        for (size_t i = 0; i < prim.size(); i++) {
          uint32_t val;
          std::memcpy(&val, &prim[i], sizeof(uint32_t));
          out[primCount + dataOffset + i] = val;
        }
        dataOffset += static_cast<uint32_t>(prim.size());
        primIdx++;
      }
    }

    // Cache the result
    const_cast<PainterImpl *>(this)->_primStagingCache = out;
  }

  bool hasContent() const override {
    for (const auto &line : _scrollLines) {
      if (!line.prims.empty())
        return true;
    }
    return !_glyphs.empty();
  }

  //=========================================================================
  // Scrolling feature
  //=========================================================================

  bool scrollingMode() const override { return _scrollingMode; }

  void setCursorPosition(uint16_t col, uint16_t row) override {
    _cursorCol = col;
    _cursorRow = row;
  }

  uint16_t cursorCol() const override { return _cursorCol; }
  uint16_t cursorRow() const override { return _cursorRow; }

  uint32_t sceneHeightInLines() const override {
    if (_cellSizeY <= 0.0f)
      return 0;
    float sceneH = _sceneMaxY - _sceneMinY;
    return static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY));
  }

  //=========================================================================
  // Buffer management - addYpaintBuffer
  //=========================================================================

  Result<uint32_t>
  addYpaintBuffer(std::shared_ptr<YPaintBuffer> buffer) override {
    if (!buffer) {
      return Err<uint32_t>("addYpaintBuffer: null buffer");
    }

    uint32_t baseGlyphIdx = static_cast<uint32_t>(_glyphs.size());

    // 1. Process fonts from input buffer (register with atlas)
    ydebug("addYpaintBuffer: processing fonts");
    buffer->forEachFont([this](int bufFontId, const uint8_t *data, size_t size,
                               const std::string &name) {
      ydebug("forEachFont: bufFontId={} name='{}' size={}", bufFontId, name,
             size);
      if (_bufferFontIdMap.count(bufFontId))
        return;
      auto res = addFontData(data, size, name);
      if (res) {
        _bufferFontIdMap[bufFontId] = *res;
        ydebug("forEachFont: SUCCESS, atlasFontId={}", *res);
      } else {
        yerror("forEachFont: FAILED: {}", res.error().message());
      }
    });

    // 2. Process text spans -> glyphs
    buffer->forEachTextSpan([this](const TextSpanData &span) {
      int atlasFontId = -1;
      auto it = _bufferFontIdMap.find(span.fontId);
      if (it != _bufferFontIdMap.end())
        atlasFontId = it->second;

      if (std::abs(span.rotation) > 0.001f) {
        ydebug("text span: rotation={} atlasFontId={} text='{}'", span.rotation,
               atlasFontId, span.text);
        auto res = addRotatedText(span.x, span.y, span.text, span.fontSize,
                                  span.color, span.rotation, atlasFontId);
        if (!res) {
          yerror("addRotatedText FAILED: {}", res.error().message());
        }
      } else {
        addText(span.x, span.y, span.text, span.fontSize, span.color,
                span.layer, atlasFontId);
      }
    });

    // 2b. Extend scene bounds for new glyphs (MUST happen before cell size
    // computation!)
    for (uint32_t gi = baseGlyphIdx; gi < _glyphs.size(); gi++) {
      const auto &g = _glyphs[gi];
      if (!_hasExplicitBounds) {
        _sceneMinX = std::min(_sceneMinX, g.x);
        _sceneMinY = std::min(_sceneMinY, g.y);
        _sceneMaxX = std::max(_sceneMaxX, g.x + g.width());
        _sceneMaxY = std::max(_sceneMaxY, g.y + g.height());
      }
    }

    // 3. Process images (TODO: store in _scrollLines as well)
    buffer->forEachImage([this](const ImageData &img) {
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
      pi.primId = 0;
      _pendingImages.push_back(std::move(pi));
    });

    // 4. Copy scene metadata from input buffer
    if (buffer->hasSceneBounds() && !_hasExplicitBounds) {
      _sceneMinX = std::min(_sceneMinX, buffer->sceneMinX());
      _sceneMinY = std::min(_sceneMinY, buffer->sceneMinY());
      _sceneMaxX = std::max(_sceneMaxX, buffer->sceneMaxX());
      _sceneMaxY = std::max(_sceneMaxY, buffer->sceneMaxY());
    }
    if (buffer->bgColor() != 0 && _bgColor == 0xFF2E1A1A) {
      _bgColor = buffer->bgColor();
    }
    _flags |= buffer->flags();

    // 5. Ensure grid dimensions are set
    // Auto-compute cell size if not set (DEFAULT_CELL_SIZE = 0)
    if (_cellSizeX <= 0.0f || _cellSizeY <= 0.0f) {
      float sceneW = _sceneMaxX - _sceneMinX;
      float sceneH = _sceneMaxY - _sceneMinY;
      if (sceneW <= 0)
        sceneW = 100;
      if (sceneH <= 0)
        sceneH = 100;
      // Default to ~10x10 grid cells
      _cellSizeX = std::max(1.0f, sceneW / 10.0f);
      _cellSizeY = std::max(1.0f, sceneH / 10.0f);
    }
    if (_gridWidth == 0 || _gridHeight == 0) {
      float sceneW = _sceneMaxX - _sceneMinX;
      float sceneH = _sceneMaxY - _sceneMinY;
      if (sceneW <= 0)
        sceneW = 100;
      if (sceneH <= 0)
        sceneH = 100;
      _gridWidth =
          std::max(1u, static_cast<uint32_t>(std::ceil(sceneW / _cellSizeX)));
      _gridHeight =
          std::max(1u, static_cast<uint32_t>(std::ceil(sceneH / _cellSizeY)));
    }

    // 6. Compute content bounds
    float contentMinY = 1e10f;
    float contentMaxY = -1e10f;
    uint32_t primCount = 0;
    buffer->forEachPrim([&](uint32_t id, const float *data, uint32_t wc) {
      uint32_t type = sdf::detail::read_u32(data, 0);
      if (type >= 100 && type < 128)
        return; // Skip 3D
      float minX, minY, maxX, maxY;
      computeAABB(data, wc, minX, minY, maxX, maxY);
      contentMinY = std::min(contentMinY, minY);
      contentMaxY = std::max(contentMaxY, maxY);
      if (!_hasExplicitBounds) {
        _sceneMinX = std::min(_sceneMinX, minX);
        _sceneMinY = std::min(_sceneMinY, minY);
        _sceneMaxX = std::max(_sceneMaxX, maxX);
        _sceneMaxY = std::max(_sceneMaxY, maxY);
      }
      primCount++;
    });

    float contentHeight =
        (contentMaxY > contentMinY) ? (contentMaxY - contentMinY) : 0.0f;
    uint32_t contentLines = std::max(
        1u, static_cast<uint32_t>(std::ceil(contentHeight / _cellSizeY)));

    // 7. Ensure we have enough lines
    uint32_t totalLines = sceneHeightInLines();
    if (totalLines == 0)
      totalLines = 24;
    while (_scrollLines.size() < totalLines) {
      ScrollLine line;
      line.cells.resize(_gridWidth > 0 ? _gridWidth : 1);
      _scrollLines.push_back(std::move(line));
    }

    // 8. Capture grid offset for this batch
    // Scrolling mode: offset = cursor position
    // Non-scrolling mode: offset = 0
    uint16_t gridOffsetCol = _scrollingMode ? _cursorCol : 0;
    uint16_t gridOffsetRow = _scrollingMode ? _cursorRow : 0;

    // 9. Process each primitive - store in line-based storage
    // Grid line = gridOffsetRow + floor((Y - sceneMinY) / cellSizeY)
    // Always use sceneMinY for world-position grid mapping
    float baseY = _sceneMinY;
    if (baseY > 1e9f)
      baseY = 0.0f; // Fallback if not set
    buffer->forEachPrim([this, gridOffsetCol, gridOffsetRow,
                         baseY](uint32_t id, const float *data, uint32_t wc) {
      uint32_t type = sdf::detail::read_u32(data, 0);
      bool is3D = (type >= 100 && type < 128);
      if (is3D) {
        _flags |= FLAG_HAS_3D;
        return; // Skip 3D for now
      }

      // Compute AABB
      float minX, minY, maxX, maxY;
      computeAABB(data, wc, minX, minY, maxX, maxY);

      // Compute which grid lines this primitive spans
      // Grid line = gridOffsetRow + floor((Y - sceneMinY) / cellSizeY)
      int32_t localMinLine =
          static_cast<int32_t>(std::floor((minY - baseY) / _cellSizeY));
      int32_t localMaxLine =
          static_cast<int32_t>(std::floor((maxY - baseY) / _cellSizeY));
      int32_t primMinLineSigned =
          static_cast<int32_t>(gridOffsetRow) + localMinLine;
      int32_t primMaxLineSigned =
          static_cast<int32_t>(gridOffsetRow) + localMaxLine;

      // Clamp to valid range
      primMinLineSigned = std::max(primMinLineSigned, 0);
      primMaxLineSigned = std::max(primMaxLineSigned, 0);
      uint32_t primMinLine = static_cast<uint32_t>(primMinLineSigned);
      uint32_t primMaxLine = static_cast<uint32_t>(primMaxLineSigned);
      if (!_scrollLines.empty()) {
        primMinLine = std::min(primMinLine,
                               static_cast<uint32_t>(_scrollLines.size() - 1));
        primMaxLine = std::min(primMaxLine,
                               static_cast<uint32_t>(_scrollLines.size() - 1));
      }

      // Base line determines where the primitive is stored in _scrollLines
      // Use max line (bottom of primitive) - removed when bottom scrolls off
      // screen
      uint32_t baseLine = primMaxLine;

      // Ensure base line exists
      while (_scrollLines.size() <= baseLine) {
        ScrollLine line;
        line.cells.resize(_gridWidth > 0 ? _gridWidth : 1);
        _scrollLines.push_back(std::move(line));
      }

      // Build primitive data: [gridOffset][type][layer][geometry...]
      std::vector<float> primData(wc + 1);
      uint32_t packedOffset = static_cast<uint32_t>(gridOffsetCol) |
                              (static_cast<uint32_t>(gridOffsetRow) << 16);
      std::memcpy(&primData[0], &packedOffset, sizeof(uint32_t));
      for (uint32_t i = 0; i < wc; i++) {
        primData[i + 1] = data[i];
      }

      // Store primitive in base line
      uint16_t primIndex =
          static_cast<uint16_t>(_scrollLines[baseLine].prims.size());
      _scrollLines[baseLine].prims.push_back(std::move(primData));

      // Add grid cell references
      uint32_t cellMinX = cellX(minX);
      uint32_t cellMaxX = cellX(maxX);

      for (uint32_t lineIdx = primMinLine; lineIdx <= primMaxLine; lineIdx++) {
        while (_scrollLines[lineIdx].cells.size() <= cellMaxX) {
          _scrollLines[lineIdx].cells.push_back(ScrollGridCell{});
        }
        uint16_t linesAhead = static_cast<uint16_t>(baseLine - lineIdx);
        for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
          _scrollLines[lineIdx].cells[cx].refs.push_back(
              {linesAhead, primIndex});
        }
      }
    });

    _gridStagingDirty = true;
    _bufferDirty = true;

    ydebug("addYpaintBuffer: added {} prims, {} glyphs, "
           "scene=[{:.0f},{:.0f}]-[{:.0f},{:.0f}]",
           primCount, _glyphs.size() - baseGlyphIdx, _sceneMinX, _sceneMinY,
           _sceneMaxX, _sceneMaxY);

    return Ok(contentLines);
  }

  void clear() override {
    // Clear line-based primitive storage
    _scrollLines.clear();

    // Clear glyph data
    _glyphs.clear();
    _glyphSortedOrder.clear();

    // Clear grid
    _gridWidth = 0;
    _gridHeight = 0;
    _gridStaging.clear();
    _gridStagingDirty = true;

    // Reset cursor
    _cursorCol = 0;
    _cursorRow = 0;

    // Reset scene bounds to extreme values (auto-extend mode)
    if (!_hasExplicitBounds) {
      _sceneMinX = 1e10f;
      _sceneMinY = 1e10f;
      _sceneMaxX = -1e10f;
      _sceneMaxY = -1e10f;
    }

    // Reset dirty flags
    _bufferDirty = true;
    _metadataDirty = true;

    ydebug("Painter::clear: reset all primitive and grid data");
  }

  //=========================================================================
  // Grid helper methods
  //=========================================================================

  uint32_t cellX(float worldX) const {
    if (_gridWidth == 0 || _cellSizeX <= 0)
      return 0;
    return static_cast<uint32_t>(
        std::clamp((worldX - _sceneMinX) / _cellSizeX, 0.0f,
                   static_cast<float>(_gridWidth - 1)));
  }

  uint32_t cellY(float worldY) const {
    if (_gridHeight == 0 || _cellSizeY <= 0)
      return 0;
    return static_cast<uint32_t>(
        std::clamp((worldY - _sceneMinY) / _cellSizeY, 0.0f,
                   static_cast<float>(_gridHeight - 1)));
  }

  // Scroll N lines in scrolling mode - O(lines deleted), not O(all primitives)
  void scrollLines(uint16_t numLines) override {
    if (numLines == 0 || _scrollLines.empty())
      return;
    ydebug("scrollLines: removing {} lines, had {} lines, cursor was ({},{})",
           numLines, _scrollLines.size(), _cursorCol, _cursorRow);

    // Pop lines from front - primitives and grid cells are deleted together
    for (uint16_t i = 0; i < numLines && !_scrollLines.empty(); i++) {
      _scrollLines.pop_front();
    }

    // DO NOT modify refs in remaining lines!
    // linesAhead is a RELATIVE offset (baseLine - lineIdx) that remains correct
    // after scrolling because both the ref's line and the target line shift
    // by the same amount. All refs in remaining lines point to other remaining
    // lines (by construction: refs only point forward or to themselves).

    // Update grid offset row in ALL remaining primitives' data
    // Primitive data format: [0]=packed(gridOffsetCol|gridOffsetRow<<16),
    // [1]=type, [2]=layer, [3..]=geometry Row is stored as SIGNED int16_t to
    // allow negative values (scrolled partially off-screen) After scrolling N
    // lines, each primitive's gridOffsetRow needs to decrease by N
    for (auto &line : _scrollLines) {
      for (auto &prim : line.prims) {
        if (prim.size() >= 1) {
          uint32_t packed;
          std::memcpy(&packed, &prim[0], sizeof(uint32_t));
          uint16_t col = packed & 0xFFFF;
          int16_t row = static_cast<int16_t>((packed >> 16) & 0xFFFF);
          row -= static_cast<int16_t>(numLines); // Can go negative!
          packed = static_cast<uint32_t>(col) |
                   (static_cast<uint32_t>(static_cast<uint16_t>(row)) << 16);
          std::memcpy(&prim[0], &packed, sizeof(uint32_t));
        }
      }
    }

    // Adjust cursor position
    if (_cursorRow >= numLines) {
      _cursorRow -= numLines;
    } else {
      _cursorRow = 0;
    }

    _bufferDirty = true;
    _gridStagingDirty = true;
  }

  void rebuildPackedGrid() {
    if (!_gridStagingDirty)
      return;

    // Build packed grid from line-based storage (_scrollLines)
    // First, build a mapping from (lineIdx, primIdx) to global prim index
    // by iterating all lines and counting primitives
    std::vector<uint32_t> lineBasePrimIdx; // lineBasePrimIdx[line] = first prim
                                           // index for that line
    uint32_t totalPrims = 0;
    for (const auto &line : _scrollLines) {
      lineBasePrimIdx.push_back(totalPrims);
      totalPrims += static_cast<uint32_t>(line.prims.size());
    }

    // Compute grid dimensions from scroll lines
    uint32_t gridH = static_cast<uint32_t>(_scrollLines.size());
    uint32_t gridW = 0;
    for (const auto &line : _scrollLines) {
      gridW = std::max(gridW, static_cast<uint32_t>(line.cells.size()));
    }

    // Also consider grid dimensions needed for glyphs
    if (!_glyphs.empty() && _cellSizeX > 0 && _cellSizeY > 0) {
      for (const auto &g : _glyphs) {
        uint32_t maxCellX =
            static_cast<uint32_t>(std::max(
                0.0f,
                std::floor((g.x + g.width() - _sceneMinX) / _cellSizeX))) +
            1;
        uint32_t maxCellY =
            static_cast<uint32_t>(std::max(
                0.0f,
                std::floor((g.y + g.height() - _sceneMinY) / _cellSizeY))) +
            1;
        gridW = std::max(gridW, maxCellX);
        gridH = std::max(gridH, maxCellY);
      }
    }

    if (gridW == 0 || gridH == 0) {
      _gridStaging.clear();
      _gridStagingDirty = false;
      return;
    }

    _gridWidth = gridW;
    _gridHeight = gridH;
    uint32_t numCells = gridW * gridH;

    // Build glyph->cell mapping: for each cell, list of glyph indices
    std::vector<std::vector<uint32_t>> cellGlyphs(numCells);
    if (!_glyphs.empty() && _cellSizeX > 0 && _cellSizeY > 0) {
      for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
        const auto &g = _glyphs[gi];
        float gMinX = g.x;
        float gMinY = g.y;
        float gMaxX = g.x + g.width();
        float gMaxY = g.y + g.height();

        int32_t cellMinX =
            static_cast<int32_t>(std::floor((gMinX - _sceneMinX) / _cellSizeX));
        int32_t cellMinY =
            static_cast<int32_t>(std::floor((gMinY - _sceneMinY) / _cellSizeY));
        int32_t cellMaxX =
            static_cast<int32_t>(std::floor((gMaxX - _sceneMinX) / _cellSizeX));
        int32_t cellMaxY =
            static_cast<int32_t>(std::floor((gMaxY - _sceneMinY) / _cellSizeY));

        cellMinX = std::max(0, cellMinX);
        cellMinY = std::max(0, cellMinY);
        cellMaxX = std::min(static_cast<int32_t>(gridW) - 1, cellMaxX);
        cellMaxY = std::min(static_cast<int32_t>(gridH) - 1, cellMaxY);

        for (int32_t cy = cellMinY; cy <= cellMaxY; cy++) {
          for (int32_t cx = cellMinX; cx <= cellMaxX; cx++) {
            uint32_t cellIdx =
                static_cast<uint32_t>(cy) * gridW + static_cast<uint32_t>(cx);
            cellGlyphs[cellIdx].push_back(gi);
          }
        }
      }
    }

    // Pass 1: compute total entries needed (prims + glyphs per cell)
    uint32_t totalEntries = 0;
    for (uint32_t y = 0; y < gridH; y++) {
      for (uint32_t x = 0; x < gridW; x++) {
        uint32_t cellIdx = y * gridW + x;
        uint32_t primCount = 0;
        if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
          primCount =
              static_cast<uint32_t>(_scrollLines[y].cells[x].refs.size());
        }
        uint32_t glyphCount = static_cast<uint32_t>(cellGlyphs[cellIdx].size());
        totalEntries += 1 + primCount + glyphCount;
      }
    }

    _gridStaging.resize(numCells + totalEntries);

    // Pass 2: fill offsets and entries
    uint32_t pos = numCells;
    for (uint32_t y = 0; y < gridH; y++) {
      for (uint32_t x = 0; x < gridW; x++) {
        uint32_t cellIdx = y * gridW + x;
        _gridStaging[cellIdx] = pos;

        uint32_t primCount = 0;
        if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
          primCount =
              static_cast<uint32_t>(_scrollLines[y].cells[x].refs.size());
        }
        uint32_t glyphCount = static_cast<uint32_t>(cellGlyphs[cellIdx].size());
        uint32_t totalCount = primCount + glyphCount;

        _gridStaging[pos] = totalCount;
        uint32_t entryIdx = 0;

        // Add prim entries
        if (y < _scrollLines.size() && x < _scrollLines[y].cells.size()) {
          const auto &cell = _scrollLines[y].cells[x];
          for (size_t j = 0; j < cell.refs.size(); j++) {
            const auto &ref = cell.refs[j];
            uint32_t baseLine = y + ref.linesAhead;
            uint32_t globalPrimIdx = lineBasePrimIdx[baseLine] + ref.primIndex;
            _gridStaging[pos + 1 + entryIdx] = globalPrimIdx;
            entryIdx++;
          }
        }

        // Add glyph entries with GLYPH_BIT (0x80000000) set
        for (uint32_t gi : cellGlyphs[cellIdx]) {
          _gridStaging[pos + 1 + entryIdx] = gi | 0x80000000u;
          entryIdx++;
        }

        pos += 1 + totalCount;
      }
    }

    _gridStagingDirty = false;

    ydebug("rebuildPackedGrid: grid={}x{} prims={} glyphs={} staging={} u32s",
           gridW, gridH, totalPrims, _glyphs.size(), _gridStaging.size());
  }

  //=========================================================================
  // GPU buffer lifecycle
  //=========================================================================

  // Compute GPU buffer size for scrolling mode primitives
  uint32_t primGpuSize() const {
    uint32_t totalWords = 0;
    for (const auto &line : _scrollLines) {
      for (const auto &prim : line.prims) {
        // Each prim: offset table entry (1 word) + prim data
        totalWords += 1 + static_cast<uint32_t>(prim.size());
      }
    }
    return totalWords * sizeof(float);
  }

  // Write scrolling mode primitives to GPU buffer
  void writePrimsGPU(float *buf, uint32_t bufBytes) {
    uint32_t totalPrims = 0;
    for (const auto &line : _scrollLines) {
      totalPrims += static_cast<uint32_t>(line.prims.size());
    }

    if (totalPrims == 0)
      return;

    // Layout: [offset0][offset1]...[offsetN-1][prim0 data][prim1 data]...
    // offsets[i] is the word offset RELATIVE to the data section start (after
    // offset table)
    float *dataBase = buf + totalPrims; // Data starts after offset table
    uint32_t dataOffset = 0;            // Relative offset within data section

    _primWordOffsets.clear();
    _primWordOffsets.reserve(totalPrims);

    uint32_t primIdx = 0;
    for (const auto &line : _scrollLines) {
      for (const auto &prim : line.prims) {
        // Write offset (relative to data section start)
        uint32_t *offsets = reinterpret_cast<uint32_t *>(buf);
        offsets[primIdx] = dataOffset;
        _primWordOffsets.push_back(dataOffset);

        // Write prim data to data section
        std::memcpy(dataBase + dataOffset, prim.data(),
                    prim.size() * sizeof(float));
        dataOffset += static_cast<uint32_t>(prim.size());
        primIdx++;
      }
    }
  }

  Result<void> declareBufferNeeds() override {
    if (!_cardMgr)
      return Err<void>("declareBufferNeeds: no CardManager");
    auto bufMgr = _cardMgr->bufferManager();

    // Reset handles — they will be re-allocated
    _primHandle = BufferHandle::invalid();
    _derivedHandle = BufferHandle::invalid();
    _primWordOffsets.clear();

    bool hasContent = false;
    for (const auto &line : _scrollLines) {
      if (!line.prims.empty()) {
        hasContent = true;
        break;
      }
    }
    if (!hasContent && _glyphs.empty()) {
      clearGridStaging();
      return Ok();
    }

    // Reserve compact prim storage
    uint32_t primBytes = primGpuSize();
    if (primBytes > 0) {
      bufMgr->reserve(primBytes);
    }

    // Build packed grid if dirty
    if (_gridStagingDirty) {
      rebuildPackedGrid();
    }

    // Reserve derived size (grid + glyphs + optional atlas metadata)
    uint32_t derivedSize = computeDerivedSize();
    if (derivedSize > 0) {
      bufMgr->reserve(derivedSize);
    }

    return Ok();
  }

  Result<void> allocateBuffers() override {
    if (!_cardMgr)
      return Err<void>("allocateBuffers: no CardManager");
    auto bufMgr = _cardMgr->bufferManager();

    // Allocate and write compact prim data
    {
      uint32_t allocBytes = primGpuSize();
      if (allocBytes > 0) {
        auto primResult =
            bufMgr->allocateBuffer(_metaSlotIndex, "prims", allocBytes);
        if (!primResult)
          return Err<void>("allocateBuffers: prim alloc failed", primResult);
        _primHandle = *primResult;

        ydebug("allocateBuffers: primHandle.offset={} size={} words={}",
               _primHandle.offset, _primHandle.size,
               _primHandle.offset / sizeof(float));

        float *buf = reinterpret_cast<float *>(_primHandle.data);
        writePrimsGPU(buf, _primHandle.size);
        bufMgr->markBufferDirty(_primHandle);
      }
    }

    // Allocate derived storage (grid + glyphs + atlas header)
    uint32_t derivedSize = computeDerivedSize();
    if (derivedSize > 0) {
      auto derivedResult =
          bufMgr->allocateBuffer(_metaSlotIndex, "derived", derivedSize);
      if (!derivedResult)
        return Err<void>("allocateBuffers: derived alloc failed",
                         derivedResult);
      _derivedHandle = *derivedResult;

      ydebug("allocateBuffers: derivedHandle.offset={} size={} words={}",
             _derivedHandle.offset, _derivedHandle.size,
             _derivedHandle.offset / sizeof(float));

      if (auto res = writeDerived(); !res)
        return res;
      bufMgr->markBufferDirty(_derivedHandle);
    }

    _bufferDirty = false;
    _metadataDirty = true;
    return Ok();
  }

  Result<void> allocateTextures() override {
    if (!_cardMgr)
      return Ok();

    auto texMgr = _cardMgr->textureManager();

    // Allocate custom MSDF atlas if present
    if (_customAtlas) {
      uint32_t atlasW = _customAtlas->getAtlasWidth();
      uint32_t atlasH = _customAtlas->getAtlasHeight();
      const auto &atlasData = _customAtlas->getAtlasData();
      if (!atlasData.empty() && atlasW > 0 && atlasH > 0) {
        auto allocResult = texMgr->allocate(atlasW, atlasH);
        if (!allocResult)
          return Err<void>("allocateTextures: msdf atlas failed", allocResult);
        _atlasTextureHandle = *allocResult;
        _allocatedAtlasW = atlasW;
        _allocatedAtlasH = atlasH;
      }
    }

    // Allocate texture space for each pending image
    for (auto &pi : _pendingImages) {
      if (pi.pixelWidth == 0 || pi.pixelHeight == 0 || pi.pixels.empty())
        continue;
      auto allocResult = texMgr->allocate(pi.pixelWidth, pi.pixelHeight);
      if (!allocResult) {
        ywarn("allocateTextures: image {}x{} failed: {}", pi.pixelWidth,
              pi.pixelHeight, allocResult.error().message());
        continue;
      }
      pi.texHandle = *allocResult;
    }

    return Ok();
  }

  Result<void> writeTextures() override {
    if (!_cardMgr)
      return Ok();

    auto texMgr = _cardMgr->textureManager();

    // Write custom MSDF atlas
    if (_atlasTextureHandle.isValid() && _customAtlas) {
      const auto &atlasData = _customAtlas->getAtlasData();
      if (!atlasData.empty()) {
        if (auto res = texMgr->write(_atlasTextureHandle, atlasData.data());
            !res) {
          return Err<void>("writeTextures: msdf atlas write failed", res);
        }
      }

      // Now that the atlas is packed, write the atlas header into derived
      // buffer
      if (_derivedHandle.isValid() && _gpuAtlasHeaderOffset > 0) {
        auto atlasPos = texMgr->getAtlasPosition(_atlasTextureHandle);
        uint32_t msdfAtlasW = _customAtlas->getAtlasWidth();
        uint32_t msdfAtlasH = _customAtlas->getAtlasHeight();
        const auto &glyphMeta = _customAtlas->getGlyphMetadata();

        uint32_t headerLocalOff =
            _gpuAtlasHeaderOffset * sizeof(float) - _derivedHandle.offset;
        uint8_t *base = _derivedHandle.data + headerLocalOff;

        uint32_t atlasHeader[4] = {
            (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
            (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
            static_cast<uint32_t>(glyphMeta.size()), 0};
        std::memcpy(base, atlasHeader, sizeof(atlasHeader));

        _cardMgr->bufferManager()->markBufferDirty(_derivedHandle);
      }
    }

    // Write image pixels and update Image primitives with atlas coords
    for (auto &pi : _pendingImages) {
      if (!pi.texHandle.isValid() || pi.primId == 0)
        continue;

      // Write pixel data to atlas
      if (auto res = texMgr->write(pi.texHandle, pi.pixels.data()); !res) {
        ywarn("writeTextures: image write failed: {}", res.error().message());
        continue;
      }

      // Update Image primitive with real atlas coordinates
      // TODO: images need to be stored in _scrollLines and updated here
      auto atlasPos = texMgr->getAtlasPosition(pi.texHandle);
      (void)atlasPos; // Suppress unused warning for now
    }

    // Mark buffer dirty so writeBuffers re-uploads with updated atlas coords
    if (!_pendingImages.empty()) {
      _bufferDirty = true;
    }

    return Ok();
  }

  Result<void> writeBuffers() override {
    if (!_cardMgr)
      return Err<void>("writeBuffers: no CardManager");

    ydebug("writeBuffers: bufferDirty={} metaDirty={} primHandle={} "
           "derivedHandle={} scrollingMode={}",
           _bufferDirty, _metadataDirty, _primHandle.isValid(),
           _derivedHandle.isValid(), _scrollingMode);

    if (_bufferDirty) {
      auto bufMgr = _cardMgr->bufferManager();

      // Check if we have primitives
      bool hasPrims = false;
      for (const auto &line : _scrollLines) {
        if (!line.prims.empty()) {
          hasPrims = true;
          break;
        }
      }

      if (_primHandle.isValid() && hasPrims) {
        ydebug("writeBuffers: re-writing prims");
        float *buf = reinterpret_cast<float *>(_primHandle.data);
        writePrimsGPU(buf, _primHandle.size);
        bufMgr->markBufferDirty(_primHandle);
      }

      // Rebuild packed grid if dirty (important after scrolling!)
      if (_gridStagingDirty) {
        ydebug("writeBuffers: rebuilding packed grid");
        rebuildPackedGrid();
      }

      // Re-write derived (grid + glyphs + atlas header)
      if (_derivedHandle.isValid()) {
        ydebug("writeBuffers: re-writing derived");
        if (auto res = writeDerived(); !res)
          return res;
        bufMgr->markBufferDirty(_derivedHandle);
      }

      _bufferDirty = false;
      _metadataDirty = true;
    }

    if (_metadataDirty) {
      if (auto res = flushMetadata(); !res)
        return res;
      _metadataDirty = false;
    }

    return Ok();
  }

  bool needsBufferRealloc() const override {
    if (!_cardMgr)
      return false;
    uint32_t primNeeded = primGpuSize();
    if (_primHandle.isValid()) {
      if (primNeeded != _primHandle.size)
        return true;
    } else if (primNeeded > 0) {
      return true;
    }
    uint32_t derivedNeeded = computeDerivedSize();
    if (_derivedHandle.isValid()) {
      if (derivedNeeded > _derivedHandle.size)
        return true;
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
      const auto &atlasData = _customAtlas->getAtlasData();
      if (!_atlasTextureHandle.isValid()) {
        if (!atlasData.empty())
          return true;
      } else if (atlasW != _allocatedAtlasW || atlasH != _allocatedAtlasH) {
        return true;
      }
    }

    // Check pending images
    for (const auto &pi : _pendingImages) {
      if (!pi.pixels.empty() && !pi.texHandle.isValid())
        return true;
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
                const auto &ga = _glyphs[a];
                const auto &gb = _glyphs[b];
                if (ga.y != gb.y)
                  return ga.y < gb.y;
                return ga.x < gb.x;
              });
  }

  int32_t findNearestGlyphSorted(float sceneX, float sceneY) override {
    if (_glyphSortedOrder.empty())
      return -1;

    float bestDist = std::numeric_limits<float>::max();
    int32_t bestSorted = -1;

    for (size_t si = 0; si < _glyphSortedOrder.size(); si++) {
      const auto &g = _glyphs[_glyphSortedOrder[si]];
      float gw = g.width();
      float gh = g.height();

      if (sceneX >= g.x && sceneX < g.x + gw && sceneY >= g.y &&
          sceneY < g.y + gh) {
        return static_cast<int32_t>(si);
      }

      float cx = g.x + gw * 0.5f;
      float cy = g.y + gh * 0.5f;
      float dist =
          (cx - sceneX) * (cx - sceneX) + (cy - sceneY) * (cy - sceneY);
      if (dist < bestDist) {
        bestDist = dist;
        bestSorted = static_cast<int32_t>(si);
      }
    }
    return bestSorted;
  }

  void setSelectionRange(int32_t startSorted, int32_t endSorted) override {
    for (auto &g : _glyphs) {
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
    if (_glyphSortedOrder.empty())
      return {};

    std::string result;
    float prevY = -1e30f;
    float prevEndX = -1e30f;
    bool first = true;

    for (size_t i = 0; i < _glyphSortedOrder.size(); i++) {
      const auto &g = _glyphs[_glyphSortedOrder[i]];
      if (((g.glyphLayerFlags >> 24) & GLYPH_FLAG_SELECTED) == 0)
        continue;

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
      bool isCustom =
          ((g.glyphLayerFlags >> 24) & GLYPH_FLAG_CUSTOM_ATLAS) != 0;

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
  float cellSizeX() const override { return _cellSizeX; }
  float cellSizeY() const override { return _cellSizeY; }
  uint32_t gridWidth() const override { return _gridWidth; }
  uint32_t gridHeight() const override { return _gridHeight; }

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
      int32_t exp = ((f32bits >> 23) & 0xFF) - 127 + 15;
      uint32_t mant = (f32bits >> 13) & 0x3FF;
      if (exp <= 0) {
        exp = 0;
        mant = 0;
      } else if (exp >= 31) {
        exp = 31;
        mant = 0;
      }
      zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
    }

    float contentW = _sceneMaxX - _sceneMinX;
    float contentH = _sceneMaxY - _sceneMinY;
    int16_t panXi16 = static_cast<int16_t>(std::clamp(
        _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
    int16_t panYi16 = static_cast<int16_t>(std::clamp(
        _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

    uint32_t primOffset =
        _primHandle.isValid() ? _primHandle.offset / sizeof(float) : 0;

    YPaintMetadata meta = {};
    meta.primitiveOffset = primOffset;
    meta.primitiveCount = primitiveCount();
    meta.gridOffset = _gpuGridOffset;
    meta.gridWidth = _gridWidth;
    meta.gridHeight = _gridHeight;
    meta.cellSizeXY = YPaintMetadata::packCellSize(_cellSizeX, _cellSizeY);
    meta.glyphOffset = _gpuGlyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
    std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
    meta.widthCells =
        (_viewWidthCells & 0xFFFF) |
        (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
    meta.heightCells =
        (_viewHeightCells & 0xFFFF) |
        (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
    meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
    meta.bgColor = _bgColor;

    ydebug("flushMetadata: primOff={} primCnt={} gridOff={} grid={}x{} "
           "scene=[{},{}]-[{},{}]",
           meta.primitiveOffset, meta.primitiveCount, meta.gridOffset,
           meta.gridWidth, meta.gridHeight, _sceneMinX, _sceneMinY, _sceneMaxX,
           _sceneMaxY);

    return _cardMgr->writeMetadata(MetadataHandle{_metaSlotIndex * 64, 64},
                                   &meta, sizeof(meta));
  }

  uint32_t computeDerivedSize() const {
    uint32_t gridBytes =
        static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
    uint32_t glyphBytes =
        static_cast<uint32_t>(_glyphs.size() * sizeof(YPaintGlyph));
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

    uint8_t *base = _derivedHandle.data;
    uint32_t offset = 0;

    // Copy grid and translate prim indices to word offsets
    uint32_t gridBytes =
        static_cast<uint32_t>(_gridStaging.size()) * sizeof(uint32_t);
    _gpuGridOffset = (_derivedHandle.offset + offset) / sizeof(float);
    ydebug("writeDerived: gridSize={} gridOffset={} primWordOffsets.size={}",
           _gridStaging.size(), _gpuGridOffset, _primWordOffsets.size());
    if (!_gridStaging.empty()) {
      std::memcpy(base + offset, _gridStaging.data(), gridBytes);
      // Translate prim indices in grid entries to word offsets
      uint32_t *gridPtr = reinterpret_cast<uint32_t *>(base + offset);
      uint32_t gridSize = static_cast<uint32_t>(_gridStaging.size());
      uint32_t numCells = _gridWidth * _gridHeight;
      if (!_primWordOffsets.empty() && numCells <= gridSize) {
        for (uint32_t ci = 0; ci < numCells; ci++) {
          uint32_t packedOff = gridPtr[ci];
          if (packedOff >= gridSize)
            continue;
          uint32_t cnt = gridPtr[packedOff];
          for (uint32_t j = 0; j < cnt; j++) {
            uint32_t idx = packedOff + 1 + j;
            if (idx >= gridSize)
              break;
            uint32_t rawVal = gridPtr[idx];
            if ((rawVal & 0x80000000u) != 0) {
              continue;
            }
            if (rawVal < static_cast<uint32_t>(_primWordOffsets.size())) {
              gridPtr[idx] = _primWordOffsets[rawVal];
            }
          }
        }
      }
    }
    offset += gridBytes;

    // Copy glyphs
    uint32_t glyphBytes =
        static_cast<uint32_t>(_glyphs.size() * sizeof(YPaintGlyph));
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
        const auto &glyphMeta = _customAtlas->getGlyphMetadata();
        uint32_t atlasHeader[4] = {
            (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
            (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
            static_cast<uint32_t>(glyphMeta.size()), 0};
        std::memcpy(base + offset, atlasHeader, sizeof(atlasHeader));
      } else {
        // Texture not yet allocated — zero-fill, writeTextures() will fill
        // later
        std::memset(base + offset, 0, atlasHeaderBytes);
      }
      offset += atlasHeaderBytes;

      const auto &glyphMeta = _customAtlas->getGlyphMetadata();
      if (!glyphMeta.empty()) {
        uint32_t metaBytes =
            static_cast<uint32_t>(glyphMeta.size() * sizeof(GlyphMetadataGPU));
        std::memcpy(base + offset, glyphMeta.data(), metaBytes);
      }
    }

    return Ok();
  }

  //=========================================================================
  // Data
  //=========================================================================

  // Packed GPU format (cache, rebuilt when _gridStagingDirty)
  std::vector<uint32_t> _gridStaging;
  bool _gridStagingDirty = true;

  // Prim staging cache (rebuilt when _bufferDirty)
  std::vector<uint32_t> _primStagingCache;

  std::vector<YPaintGlyph> _glyphs;
  std::vector<uint32_t> _glyphSortedOrder;

  // Grid dimensions
  uint32_t _gridWidth = 0;
  uint32_t _gridHeight = 0;

  // Scene bounds (initialized to extreme values, extended by addYpaintBuffer)
  float _sceneMinX = 1e10f, _sceneMinY = 1e10f;
  float _sceneMaxX = -1e10f, _sceneMaxY = -1e10f;
  bool _hasExplicitBounds = false;

  // Grid parameters
  float _cellSizeX = DEFAULT_CELL_SIZE;
  float _cellSizeY = DEFAULT_CELL_SIZE;
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
    uint32_t primId = 0; // ID of Image primitive in buffer
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

  // Scrolling mode state (terminal-style scrolling)
  bool _scrollingMode = false;
  uint16_t _cursorCol = 0;
  uint16_t _cursorRow = 0;

  //=========================================================================
  // Scrolling mode line-based storage
  //=========================================================================

  // Reference to a primitive: linesAhead = offset to line containing the data,
  // primIndex = index within that line's prims vector
  struct ScrollPrimRef {
    uint16_t linesAhead; // relative offset to base line (0 = same line)
    uint16_t primIndex;  // index within base line's prims
  };

  // Grid cell in scrolling mode: stores references to primitives
  struct ScrollGridCell {
    std::vector<ScrollPrimRef> refs;
  };

  // A single line in scrolling mode
  struct ScrollLine {
    // Primitives whose BASE (last line they touch) is this line
    std::vector<std::vector<float>> prims;
    // Grid cells for this line's Y range (indexed by X)
    std::vector<ScrollGridCell> cells;
  };

  // Lines in scrolling mode (front = top, pop_front on scroll)
  std::deque<ScrollLine> _scrollLines;
};

//=============================================================================
// Factory
//=============================================================================

Result<Painter::Ptr> Painter::createImpl(FontManager::Ptr fontManager,
                                         GpuAllocator::Ptr allocator,
                                         CardManager::Ptr cardMgr,
                                         uint32_t metaSlotIndex,
                                         bool scrollingMode) {
  return Ok(
      Ptr(new PainterImpl(std::move(fontManager), std::move(allocator),
                          std::move(cardMgr), metaSlotIndex, scrollingMode)));
}

Result<Painter::Ptr> Painter::createImpl(FontManager::Ptr fontManager,
                                         GpuAllocator::Ptr allocator,
                                         bool scrollingMode) {
  return Ok(Ptr(new PainterImpl(std::move(fontManager), std::move(allocator),
                                CardManager::Ptr{}, 0, scrollingMode)));
}

} // namespace yetty::ypaint
