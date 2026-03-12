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
