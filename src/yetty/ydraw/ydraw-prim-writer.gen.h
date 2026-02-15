// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

// Forward declaration — include hdraw-types.gen.h for full enum
namespace yetty::card { enum class SDFType : uint32_t; struct SDFPrimitive; }

namespace yetty::sdf {

namespace detail {
inline void write_u32(float* buf, uint32_t off, uint32_t val) {
    std::memcpy(&buf[off], &val, sizeof(uint32_t));
}
inline uint32_t read_u32(const float* buf, uint32_t off) {
    uint32_t v; std::memcpy(&v, &buf[off], sizeof(uint32_t)); return v;
}
} // namespace detail

/// Write Circle (9 words). Returns word count.
inline uint32_t writeCircle(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 0u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Box (10 words). Returns word count.
inline uint32_t writeBox(float* buf, uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 1u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = hw;
    buf[5] = hh;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Segment (10 words). Returns word count.
inline uint32_t writeSegment(float* buf, uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 2u);
    detail::write_u32(buf, 1, layer);
    buf[2] = x0;
    buf[3] = y0;
    buf[4] = x1;
    buf[5] = y1;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Triangle (12 words). Returns word count.
inline uint32_t writeTriangle(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 3u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = vx;
    buf[7] = vy;
    detail::write_u32(buf, 8, fillColor);
    detail::write_u32(buf, 9, strokeColor);
    buf[10] = strokeWidth;
    buf[11] = round_;
    return 12;
}

/// Write Bezier2 (12 words). Returns word count.
inline uint32_t writeBezier2(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 4u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = cx;
    buf[7] = cy;
    detail::write_u32(buf, 8, fillColor);
    detail::write_u32(buf, 9, strokeColor);
    buf[10] = strokeWidth;
    buf[11] = round_;
    return 12;
}

/// Write Bezier3 (14 words). Returns word count.
inline uint32_t writeBezier3(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 5u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = cx;
    buf[7] = cy;
    buf[8] = dx;
    buf[9] = dy;
    detail::write_u32(buf, 10, fillColor);
    detail::write_u32(buf, 11, strokeColor);
    buf[12] = strokeWidth;
    buf[13] = round_;
    return 14;
}

/// Write Ellipse (10 words). Returns word count.
inline uint32_t writeEllipse(float* buf, uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 6u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = rx;
    buf[5] = ry;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Arc (12 words). Returns word count.
inline uint32_t writeArc(float* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 7u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = sc_x;
    buf[5] = sc_y;
    buf[6] = ra;
    buf[7] = rb;
    detail::write_u32(buf, 8, fillColor);
    detail::write_u32(buf, 9, strokeColor);
    buf[10] = strokeWidth;
    buf[11] = round_;
    return 12;
}

/// Write RoundedBox (14 words). Returns word count.
inline uint32_t writeRoundedBox(float* buf, uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 8u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = hw;
    buf[5] = hh;
    buf[6] = r0;
    buf[7] = r1;
    buf[8] = r2;
    buf[9] = r3;
    detail::write_u32(buf, 10, fillColor);
    detail::write_u32(buf, 11, strokeColor);
    buf[12] = strokeWidth;
    buf[13] = round_;
    return 14;
}

/// Write Rhombus (10 words). Returns word count.
inline uint32_t writeRhombus(float* buf, uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 9u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = bx;
    buf[5] = by;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Pentagon (9 words). Returns word count.
inline uint32_t writePentagon(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 10u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Hexagon (9 words). Returns word count.
inline uint32_t writeHexagon(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 11u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Star (11 words). Returns word count.
inline uint32_t writeStar(float* buf, uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 12u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    buf[5] = n;
    buf[6] = m;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Pie (11 words). Returns word count.
inline uint32_t writePie(float* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 13u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = sc_x;
    buf[5] = sc_y;
    buf[6] = r;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Ring (12 words). Returns word count.
inline uint32_t writeRing(float* buf, uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 14u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = nx;
    buf[5] = ny;
    buf[6] = r;
    buf[7] = th;
    detail::write_u32(buf, 8, fillColor);
    detail::write_u32(buf, 9, strokeColor);
    buf[10] = strokeWidth;
    buf[11] = round_;
    return 12;
}

/// Write Heart (9 words). Returns word count.
inline uint32_t writeHeart(float* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 15u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = scale;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Cross (11 words). Returns word count.
inline uint32_t writeCross(float* buf, uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 16u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = r;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write RoundedX (10 words). Returns word count.
inline uint32_t writeRoundedX(float* buf, uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 17u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = w;
    buf[5] = r;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Capsule (11 words). Returns word count.
inline uint32_t writeCapsule(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 18u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = r;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Moon (11 words). Returns word count.
inline uint32_t writeMoon(float* buf, uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 19u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = d;
    buf[5] = ra;
    buf[6] = rb;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Egg (10 words). Returns word count.
inline uint32_t writeEgg(float* buf, uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 20u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = ra;
    buf[5] = rb;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write ChamferBox (11 words). Returns word count.
inline uint32_t writeChamferBox(float* buf, uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 21u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = hw;
    buf[5] = hh;
    buf[6] = chamfer;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write OrientedBox (11 words). Returns word count.
inline uint32_t writeOrientedBox(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 22u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = th;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Trapezoid (11 words). Returns word count.
inline uint32_t writeTrapezoid(float* buf, uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 23u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r1;
    buf[5] = r2;
    buf[6] = he;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Parallelogram (11 words). Returns word count.
inline uint32_t writeParallelogram(float* buf, uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 24u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = wi;
    buf[5] = he;
    buf[6] = sk;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write EquilateralTriangle (9 words). Returns word count.
inline uint32_t writeEquilateralTriangle(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 25u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write IsoscelesTriangle (10 words). Returns word count.
inline uint32_t writeIsoscelesTriangle(float* buf, uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 26u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = qx;
    buf[5] = qy;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write UnevenCapsule (11 words). Returns word count.
inline uint32_t writeUnevenCapsule(float* buf, uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 27u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r1;
    buf[5] = r2;
    buf[6] = h;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write Octogon (9 words). Returns word count.
inline uint32_t writeOctogon(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 28u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Hexagram (9 words). Returns word count.
inline uint32_t writeHexagram(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 29u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Pentagram (9 words). Returns word count.
inline uint32_t writePentagram(float* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 30u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write CutDisk (10 words). Returns word count.
inline uint32_t writeCutDisk(float* buf, uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 31u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = r;
    buf[5] = h;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Horseshoe (13 words). Returns word count.
inline uint32_t writeHorseshoe(float* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 32u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = sc_x;
    buf[5] = sc_y;
    buf[6] = r;
    buf[7] = wx;
    buf[8] = wy;
    detail::write_u32(buf, 9, fillColor);
    detail::write_u32(buf, 10, strokeColor);
    buf[11] = strokeWidth;
    buf[12] = round_;
    return 13;
}

/// Write Vesica (10 words). Returns word count.
inline uint32_t writeVesica(float* buf, uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 33u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = w;
    buf[5] = h;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write OrientedVesica (11 words). Returns word count.
inline uint32_t writeOrientedVesica(float* buf, uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 34u);
    detail::write_u32(buf, 1, layer);
    buf[2] = ax;
    buf[3] = ay;
    buf[4] = bx;
    buf[5] = by;
    buf[6] = w;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write RoundedCross (9 words). Returns word count.
inline uint32_t writeRoundedCross(float* buf, uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 35u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = h;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Parabola (9 words). Returns word count.
inline uint32_t writeParabola(float* buf, uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 36u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = k;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write BlobbyCross (9 words). Returns word count.
inline uint32_t writeBlobbyCross(float* buf, uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 37u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = he;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Tunnel (10 words). Returns word count.
inline uint32_t writeTunnel(float* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 38u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = wh_x;
    buf[5] = wh_y;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Stairs (11 words). Returns word count.
inline uint32_t writeStairs(float* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 39u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = wh_x;
    buf[5] = wh_y;
    buf[6] = n;
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write QuadraticCircle (9 words). Returns word count.
inline uint32_t writeQuadraticCircle(float* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 40u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = scale;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write Hyperbola (10 words). Returns word count.
inline uint32_t writeHyperbola(float* buf, uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 41u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = k;
    buf[5] = he;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write CoolS (9 words). Returns word count.
inline uint32_t writeCoolS(float* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 42u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = scale;
    detail::write_u32(buf, 5, fillColor);
    detail::write_u32(buf, 6, strokeColor);
    buf[7] = strokeWidth;
    buf[8] = round_;
    return 9;
}

/// Write CircleWave (10 words). Returns word count.
inline uint32_t writeCircleWave(float* buf, uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 43u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = tb;
    buf[5] = ra;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write ColorWheel (14 words). Returns word count.
inline uint32_t writeColorWheel(float* buf, uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 44u);
    detail::write_u32(buf, 1, layer);
    buf[2] = cx;
    buf[3] = cy;
    buf[4] = outerR;
    buf[5] = innerR;
    buf[6] = hue;
    buf[7] = sat;
    buf[8] = val;
    buf[9] = indicatorSize;
    detail::write_u32(buf, 10, fillColor);
    detail::write_u32(buf, 11, strokeColor);
    buf[12] = strokeWidth;
    buf[13] = round_;
    return 14;
}

/// Write TextGlyph (11 words). Returns word count.
inline uint32_t writeTextGlyph(float* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 64u);
    detail::write_u32(buf, 1, layer);
    buf[2] = x;
    buf[3] = y;
    buf[4] = scaleX;
    buf[5] = scaleY;
    detail::write_u32(buf, 6, glyphIndex);
    detail::write_u32(buf, 7, fillColor);
    detail::write_u32(buf, 8, strokeColor);
    buf[9] = strokeWidth;
    buf[10] = round_;
    return 11;
}

/// Write RotatedGlyph (14 words). Returns word count.
inline uint32_t writeRotatedGlyph(float* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 65u);
    detail::write_u32(buf, 1, layer);
    buf[2] = x;
    buf[3] = y;
    buf[4] = scaleX;
    buf[5] = scaleY;
    buf[6] = angle;
    detail::write_u32(buf, 7, glyphIndex);
    buf[8] = cosAngle;
    buf[9] = sinAngle;
    detail::write_u32(buf, 10, fillColor);
    detail::write_u32(buf, 11, strokeColor);
    buf[12] = strokeWidth;
    buf[13] = round_;
    return 14;
}

/// Write Sphere3D (10 words). Returns word count.
inline uint32_t writeSphere3D(float* buf, uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 100u);
    detail::write_u32(buf, 1, layer);
    buf[2] = px;
    buf[3] = py;
    buf[4] = pz;
    buf[5] = r;
    detail::write_u32(buf, 6, fillColor);
    detail::write_u32(buf, 7, strokeColor);
    buf[8] = strokeWidth;
    buf[9] = round_;
    return 10;
}

/// Write Box3D (12 words). Returns word count.
inline uint32_t writeBox3D(float* buf, uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    detail::write_u32(buf, 0, 101u);
    detail::write_u32(buf, 1, layer);
    buf[2] = px;
    buf[3] = py;
    buf[4] = pz;
    buf[5] = bx;
    buf[6] = by;
    buf[7] = bz;
    detail::write_u32(buf, 8, fillColor);
    detail::write_u32(buf, 9, strokeColor);
    buf[10] = strokeWidth;
    buf[11] = round_;
    return 12;
}

/// Write Plot (12 words). Returns word count.
inline uint32_t writePlot(float* buf, uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor) {
    detail::write_u32(buf, 0, 128u);
    detail::write_u32(buf, 1, layer);
    buf[2] = x;
    buf[3] = y;
    buf[4] = w;
    buf[5] = h;
    detail::write_u32(buf, 6, dataCount);
    buf[7] = minVal;
    buf[8] = maxVal;
    detail::write_u32(buf, 9, flags);
    detail::write_u32(buf, 10, lineColor);
    detail::write_u32(buf, 11, bgColor);
    return 12;
}

/// Write Image (10 words). Returns word count.
inline uint32_t writeImage(float* buf, uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH) {
    detail::write_u32(buf, 0, 129u);
    detail::write_u32(buf, 1, layer);
    buf[2] = x;
    buf[3] = y;
    buf[4] = w;
    buf[5] = h;
    detail::write_u32(buf, 6, atlasX);
    detail::write_u32(buf, 7, atlasY);
    detail::write_u32(buf, 8, texW);
    detail::write_u32(buf, 9, texH);
    return 10;
}

/// Return word count for a given SDF type ID. 0 = unknown.
inline uint32_t wordCountForType(uint32_t type) {
    switch (type) {
    case 0u: return 9; // Circle
    case 1u: return 10; // Box
    case 2u: return 10; // Segment
    case 3u: return 12; // Triangle
    case 4u: return 12; // Bezier2
    case 5u: return 14; // Bezier3
    case 6u: return 10; // Ellipse
    case 7u: return 12; // Arc
    case 8u: return 14; // RoundedBox
    case 9u: return 10; // Rhombus
    case 10u: return 9; // Pentagon
    case 11u: return 9; // Hexagon
    case 12u: return 11; // Star
    case 13u: return 11; // Pie
    case 14u: return 12; // Ring
    case 15u: return 9; // Heart
    case 16u: return 11; // Cross
    case 17u: return 10; // RoundedX
    case 18u: return 11; // Capsule
    case 19u: return 11; // Moon
    case 20u: return 10; // Egg
    case 21u: return 11; // ChamferBox
    case 22u: return 11; // OrientedBox
    case 23u: return 11; // Trapezoid
    case 24u: return 11; // Parallelogram
    case 25u: return 9; // EquilateralTriangle
    case 26u: return 10; // IsoscelesTriangle
    case 27u: return 11; // UnevenCapsule
    case 28u: return 9; // Octogon
    case 29u: return 9; // Hexagram
    case 30u: return 9; // Pentagram
    case 31u: return 10; // CutDisk
    case 32u: return 13; // Horseshoe
    case 33u: return 10; // Vesica
    case 34u: return 11; // OrientedVesica
    case 35u: return 9; // RoundedCross
    case 36u: return 9; // Parabola
    case 37u: return 9; // BlobbyCross
    case 38u: return 10; // Tunnel
    case 39u: return 11; // Stairs
    case 40u: return 9; // QuadraticCircle
    case 41u: return 10; // Hyperbola
    case 42u: return 9; // CoolS
    case 43u: return 10; // CircleWave
    case 44u: return 14; // ColorWheel
    case 64u: return 11; // TextGlyph
    case 65u: return 14; // RotatedGlyph
    case 100u: return 10; // Sphere3D
    case 101u: return 12; // Box3D
    case 128u: return 12; // Plot
    case 129u: return 10; // Image
    default: return 0;
    }
}

#ifdef YETTY_CARD_SDF_PRIMITIVE_DEFINED

/// Read buffer into SDFPrimitive. Returns words consumed (0 = unknown type).
inline uint32_t readPrimitive(const float* buf, card::SDFPrimitive& prim) {
    std::memset(&prim, 0, sizeof(prim));
    uint32_t primType = detail::read_u32(buf, 0);
    switch (static_cast<card::SDFType>(primType)) {
    case card::SDFType::Circle: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Box: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Segment: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Triangle: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.fillColor = detail::read_u32(buf, 8);
        prim.strokeColor = detail::read_u32(buf, 9);
        prim.strokeWidth = buf[10];
        prim.round = buf[11];
        return 12;
    }
    case card::SDFType::Bezier2: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.fillColor = detail::read_u32(buf, 8);
        prim.strokeColor = detail::read_u32(buf, 9);
        prim.strokeWidth = buf[10];
        prim.round = buf[11];
        return 12;
    }
    case card::SDFType::Bezier3: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.params[6] = buf[8];
        prim.params[7] = buf[9];
        prim.fillColor = detail::read_u32(buf, 10);
        prim.strokeColor = detail::read_u32(buf, 11);
        prim.strokeWidth = buf[12];
        prim.round = buf[13];
        return 14;
    }
    case card::SDFType::Ellipse: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Arc: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.fillColor = detail::read_u32(buf, 8);
        prim.strokeColor = detail::read_u32(buf, 9);
        prim.strokeWidth = buf[10];
        prim.round = buf[11];
        return 12;
    }
    case card::SDFType::RoundedBox: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.params[6] = buf[8];
        prim.params[7] = buf[9];
        prim.fillColor = detail::read_u32(buf, 10);
        prim.strokeColor = detail::read_u32(buf, 11);
        prim.strokeWidth = buf[12];
        prim.round = buf[13];
        return 14;
    }
    case card::SDFType::Rhombus: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Pentagon: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Hexagon: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Star: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Pie: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Ring: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.fillColor = detail::read_u32(buf, 8);
        prim.strokeColor = detail::read_u32(buf, 9);
        prim.strokeWidth = buf[10];
        prim.round = buf[11];
        return 12;
    }
    case card::SDFType::Heart: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Cross: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::RoundedX: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Capsule: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Moon: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Egg: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::ChamferBox: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::OrientedBox: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Trapezoid: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Parallelogram: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::EquilateralTriangle: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::IsoscelesTriangle: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::UnevenCapsule: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::Octogon: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Hexagram: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Pentagram: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::CutDisk: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Horseshoe: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.params[6] = buf[8];
        prim.fillColor = detail::read_u32(buf, 9);
        prim.strokeColor = detail::read_u32(buf, 10);
        prim.strokeWidth = buf[11];
        prim.round = buf[12];
        return 13;
    }
    case card::SDFType::Vesica: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::OrientedVesica: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::RoundedCross: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Parabola: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::BlobbyCross: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Tunnel: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Stairs: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::QuadraticCircle: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::Hyperbola: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::CoolS: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.fillColor = detail::read_u32(buf, 5);
        prim.strokeColor = detail::read_u32(buf, 6);
        prim.strokeWidth = buf[7];
        prim.round = buf[8];
        return 9;
    }
    case card::SDFType::CircleWave: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::ColorWheel: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.params[6] = buf[8];
        prim.params[7] = buf[9];
        prim.fillColor = detail::read_u32(buf, 10);
        prim.strokeColor = detail::read_u32(buf, 11);
        prim.strokeWidth = buf[12];
        prim.round = buf[13];
        return 14;
    }
    case card::SDFType::TextGlyph: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        std::memcpy(&prim.params[4], &buf[6], sizeof(float));
        prim.fillColor = detail::read_u32(buf, 7);
        prim.strokeColor = detail::read_u32(buf, 8);
        prim.strokeWidth = buf[9];
        prim.round = buf[10];
        return 11;
    }
    case card::SDFType::RotatedGlyph: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        std::memcpy(&prim.params[5], &buf[7], sizeof(float));
        prim.params[6] = buf[8];
        prim.params[7] = buf[9];
        prim.fillColor = detail::read_u32(buf, 10);
        prim.strokeColor = detail::read_u32(buf, 11);
        prim.strokeWidth = buf[12];
        prim.round = buf[13];
        return 14;
    }
    case card::SDFType::Sphere3D: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.fillColor = detail::read_u32(buf, 6);
        prim.strokeColor = detail::read_u32(buf, 7);
        prim.strokeWidth = buf[8];
        prim.round = buf[9];
        return 10;
    }
    case card::SDFType::Box3D: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        prim.params[4] = buf[6];
        prim.params[5] = buf[7];
        prim.fillColor = detail::read_u32(buf, 8);
        prim.strokeColor = detail::read_u32(buf, 9);
        prim.strokeWidth = buf[10];
        prim.round = buf[11];
        return 12;
    }
    case card::SDFType::Plot: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        std::memcpy(&prim.params[4], &buf[6], sizeof(float));
        prim.params[5] = buf[7];
        prim.params[6] = buf[8];
        std::memcpy(&prim.params[7], &buf[9], sizeof(float));
        std::memcpy(&prim.params[8], &buf[10], sizeof(float));
        std::memcpy(&prim.params[9], &buf[11], sizeof(float));
        return 12;
    }
    case card::SDFType::Image: {
        prim.type = detail::read_u32(buf, 0);
        prim.layer = detail::read_u32(buf, 1);
        prim.params[0] = buf[2];
        prim.params[1] = buf[3];
        prim.params[2] = buf[4];
        prim.params[3] = buf[5];
        std::memcpy(&prim.params[4], &buf[6], sizeof(float));
        std::memcpy(&prim.params[5], &buf[7], sizeof(float));
        std::memcpy(&prim.params[6], &buf[8], sizeof(float));
        std::memcpy(&prim.params[7], &buf[9], sizeof(float));
        return 10;
    }
    default:
        return 0;
    }
}

/// Write SDFPrimitive to buffer. Returns word count (0 = unknown type).
inline uint32_t writePrimitive(float* buf, const card::SDFPrimitive& prim) {
    switch (static_cast<card::SDFType>(prim.type)) {
    case card::SDFType::Circle: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Box: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Segment: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Triangle: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        detail::write_u32(buf, 8, prim.fillColor);
        detail::write_u32(buf, 9, prim.strokeColor);
        buf[10] = prim.strokeWidth;
        buf[11] = prim.round;
        return 12;
    }
    case card::SDFType::Bezier2: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        detail::write_u32(buf, 8, prim.fillColor);
        detail::write_u32(buf, 9, prim.strokeColor);
        buf[10] = prim.strokeWidth;
        buf[11] = prim.round;
        return 12;
    }
    case card::SDFType::Bezier3: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        buf[8] = prim.params[6];
        buf[9] = prim.params[7];
        detail::write_u32(buf, 10, prim.fillColor);
        detail::write_u32(buf, 11, prim.strokeColor);
        buf[12] = prim.strokeWidth;
        buf[13] = prim.round;
        return 14;
    }
    case card::SDFType::Ellipse: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Arc: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        detail::write_u32(buf, 8, prim.fillColor);
        detail::write_u32(buf, 9, prim.strokeColor);
        buf[10] = prim.strokeWidth;
        buf[11] = prim.round;
        return 12;
    }
    case card::SDFType::RoundedBox: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        buf[8] = prim.params[6];
        buf[9] = prim.params[7];
        detail::write_u32(buf, 10, prim.fillColor);
        detail::write_u32(buf, 11, prim.strokeColor);
        buf[12] = prim.strokeWidth;
        buf[13] = prim.round;
        return 14;
    }
    case card::SDFType::Rhombus: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Pentagon: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Hexagon: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Star: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Pie: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Ring: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        detail::write_u32(buf, 8, prim.fillColor);
        detail::write_u32(buf, 9, prim.strokeColor);
        buf[10] = prim.strokeWidth;
        buf[11] = prim.round;
        return 12;
    }
    case card::SDFType::Heart: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Cross: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::RoundedX: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Capsule: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Moon: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Egg: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::ChamferBox: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::OrientedBox: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Trapezoid: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Parallelogram: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::EquilateralTriangle: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::IsoscelesTriangle: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::UnevenCapsule: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::Octogon: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Hexagram: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Pentagram: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::CutDisk: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Horseshoe: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        buf[8] = prim.params[6];
        detail::write_u32(buf, 9, prim.fillColor);
        detail::write_u32(buf, 10, prim.strokeColor);
        buf[11] = prim.strokeWidth;
        buf[12] = prim.round;
        return 13;
    }
    case card::SDFType::Vesica: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::OrientedVesica: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::RoundedCross: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Parabola: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::BlobbyCross: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Tunnel: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Stairs: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::QuadraticCircle: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::Hyperbola: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::CoolS: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        detail::write_u32(buf, 5, prim.fillColor);
        detail::write_u32(buf, 6, prim.strokeColor);
        buf[7] = prim.strokeWidth;
        buf[8] = prim.round;
        return 9;
    }
    case card::SDFType::CircleWave: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::ColorWheel: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        buf[8] = prim.params[6];
        buf[9] = prim.params[7];
        detail::write_u32(buf, 10, prim.fillColor);
        detail::write_u32(buf, 11, prim.strokeColor);
        buf[12] = prim.strokeWidth;
        buf[13] = prim.round;
        return 14;
    }
    case card::SDFType::TextGlyph: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        std::memcpy(&buf[6], &prim.params[4], sizeof(float));
        detail::write_u32(buf, 7, prim.fillColor);
        detail::write_u32(buf, 8, prim.strokeColor);
        buf[9] = prim.strokeWidth;
        buf[10] = prim.round;
        return 11;
    }
    case card::SDFType::RotatedGlyph: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        std::memcpy(&buf[7], &prim.params[5], sizeof(float));
        buf[8] = prim.params[6];
        buf[9] = prim.params[7];
        detail::write_u32(buf, 10, prim.fillColor);
        detail::write_u32(buf, 11, prim.strokeColor);
        buf[12] = prim.strokeWidth;
        buf[13] = prim.round;
        return 14;
    }
    case card::SDFType::Sphere3D: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        detail::write_u32(buf, 6, prim.fillColor);
        detail::write_u32(buf, 7, prim.strokeColor);
        buf[8] = prim.strokeWidth;
        buf[9] = prim.round;
        return 10;
    }
    case card::SDFType::Box3D: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        buf[6] = prim.params[4];
        buf[7] = prim.params[5];
        detail::write_u32(buf, 8, prim.fillColor);
        detail::write_u32(buf, 9, prim.strokeColor);
        buf[10] = prim.strokeWidth;
        buf[11] = prim.round;
        return 12;
    }
    case card::SDFType::Plot: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        std::memcpy(&buf[6], &prim.params[4], sizeof(float));
        buf[7] = prim.params[5];
        buf[8] = prim.params[6];
        std::memcpy(&buf[9], &prim.params[7], sizeof(float));
        std::memcpy(&buf[10], &prim.params[8], sizeof(float));
        std::memcpy(&buf[11], &prim.params[9], sizeof(float));
        return 12;
    }
    case card::SDFType::Image: {
        detail::write_u32(buf, 0, prim.type);
        detail::write_u32(buf, 1, prim.layer);
        buf[2] = prim.params[0];
        buf[3] = prim.params[1];
        buf[4] = prim.params[2];
        buf[5] = prim.params[3];
        std::memcpy(&buf[6], &prim.params[4], sizeof(float));
        std::memcpy(&buf[7], &prim.params[5], sizeof(float));
        std::memcpy(&buf[8], &prim.params[6], sizeof(float));
        std::memcpy(&buf[9], &prim.params[7], sizeof(float));
        return 10;
    }
    default:
        return 0;
    }
}

/// Compute total bytes needed for compact prim buffer:
/// [offset_table: count words] + [compact_prim_data]
inline uint32_t computeCompactSize(
        const card::SDFPrimitive* prims, uint32_t count) {
    uint32_t dataWords = 0;
    float tmp[24];
    for (uint32_t i = 0; i < count; i++) {
        uint32_t wc = writePrimitive(tmp, prims[i]);
        dataWords += (wc > 0) ? wc : 1;
    }
    return (count + dataWords) * sizeof(float);
}

/// Write compact format to GPU buffer: [offset_table][compact_data].
/// Fills wordOffsets with per-prim word offsets (for grid translation).
inline void writeCompactToBuffer(
        float* buf, uint32_t bufBytes,
        const card::SDFPrimitive* prims, uint32_t count,
        std::vector<uint32_t>& wordOffsets) {
    wordOffsets.resize(count);
    float* dataBase = buf + count;
    uint32_t dataOffset = 0;
    for (uint32_t i = 0; i < count; i++) {
        wordOffsets[i] = dataOffset;
        uint32_t off = dataOffset;
        std::memcpy(&buf[i], &off, sizeof(uint32_t));
        uint32_t wc = writePrimitive(dataBase + dataOffset, prims[i]);
        if (wc == 0) wc = 1;
        dataOffset += wc;
    }
}

/// Translate grid entries from primitive indices to word offsets.
/// Grid layout: [off0..offN-1][packed_cells...] where cell = [count][e0][e1]...
/// Non-glyph entries (prim indices) are replaced with word offsets.
inline void translateGridEntries(
        uint32_t* grid, uint32_t gridSize,
        uint32_t gridW, uint32_t gridH,
        const std::vector<uint32_t>& wordOffsets) {
    if (wordOffsets.empty() || gridSize == 0) return;
    uint32_t numCells = gridW * gridH;
    if (numCells > gridSize) return;
    for (uint32_t ci = 0; ci < numCells; ci++) {
        uint32_t packedOff = grid[ci];
        if (packedOff >= gridSize) continue;
        uint32_t cnt = grid[packedOff];
        for (uint32_t j = 0; j < cnt; j++) {
            uint32_t idx = packedOff + 1 + j;
            if (idx >= gridSize) break;
            uint32_t rawVal = grid[idx];
            if ((rawVal & 0x80000000u) != 0) continue;
            if (rawVal < static_cast<uint32_t>(wordOffsets.size())) {
                grid[idx] = wordOffsets[rawVal];
            }
        }
    }
}

#endif // YETTY_CARD_SDF_PRIMITIVE_DEFINED

} // namespace yetty::sdf
