// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT


#include "ydraw-capi.gen.h"
#include "ydraw-buffer.h"

#include <cstring>
#include <cstdlib>

using namespace yetty;

//=============================================================================
// The opaque ydraw_buffer_t wraps YDrawBuffer::Ptr
//=============================================================================

struct ydraw_buffer {
    YDrawBuffer::Ptr impl;
    std::vector<uint8_t> serialized;  // Cached for C API access
};

//=============================================================================
// Buffer lifecycle
//=============================================================================

extern "C" ydraw_buffer_t* ydraw_buffer_create(void) {
    auto result = YDrawBuffer::create();
    if (!result) return nullptr;
    auto* buf = new ydraw_buffer();
    buf->impl = *result;
    return buf;
}

extern "C" void ydraw_buffer_destroy(ydraw_buffer_t* buf) {
    delete buf;
}

extern "C" void ydraw_buffer_clear(ydraw_buffer_t* buf) {
    if (buf && buf->impl) {
        buf->impl->clear();
        buf->serialized.clear();
    }
}

extern "C" uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf) {
    return (buf && buf->impl) ? buf->impl->primCount() : 0;
}

extern "C" uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf) {
    return 0;  // Not directly available
}

extern "C" uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf) {
    return buf ? static_cast<uint32_t>(buf->serialized.size()) : 0;
}

extern "C" const float* ydraw_buffer_data(const ydraw_buffer_t* buf) {
    return nullptr;  // Not directly exposed
}

extern "C" uint32_t ydraw_buffer_serialize(ydraw_buffer_t* buf, const uint8_t** out_data) {
    if (!buf || !buf->impl || !out_data) {
        if (out_data) *out_data = nullptr;
        return 0;
    }
    buf->serialized = buf->impl->serialize();
    *out_data = buf->serialized.data();
    return static_cast<uint32_t>(buf->serialized.size());
}

//=============================================================================
// Text spans
//=============================================================================

extern "C" int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                                   float fontSize, uint32_t color, uint32_t layer, int fontId) {
    if (!buf || !buf->impl || !text) return -1;
    buf->impl->addTextSpan(x, y, text, fontSize, color, layer, fontId);
    return 0;
}

extern "C" int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                                           float fontSize, uint32_t color, float rotation, int fontId) {
    if (!buf || !buf->impl || !text) return -1;
    buf->impl->addRotatedTextSpan(x, y, text, fontSize, color, rotation, fontId);
    return 0;
}

extern "C" uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf) {
    return (buf && buf->impl) ? static_cast<uint32_t>(buf->impl->textSpans().size()) : 0;
}

extern "C" const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index) {
    return nullptr;  // Would need caching
}

//=============================================================================
// Primitive add functions - delegate to YDrawBuffer
//=============================================================================

extern "C" int32_t ydraw_add_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCircle(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addBox(layer, cx, cy, hw, hh, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_segment(ydraw_buffer_t* buf, uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addSegment(layer, x0, y0, x1, y1, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_triangle(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addTriangle(layer, ax, ay, bx, by, vx, vy, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_bezier2(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addBezier2(layer, ax, ay, bx, by, cx, cy, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_bezier3(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addBezier3(layer, ax, ay, bx, by, cx, cy, dx, dy, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_ellipse(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addEllipse(layer, cx, cy, rx, ry, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_arc(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addArc(layer, cx, cy, sc_x, sc_y, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_rounded_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRoundedBox(layer, cx, cy, hw, hh, r0, r1, r2, r3, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_rhombus(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRhombus(layer, cx, cy, bx, by, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_pentagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPentagon(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_hexagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addHexagon(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_star(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addStar(layer, cx, cy, r, n, m, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_pie(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPie(layer, cx, cy, sc_x, sc_y, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_ring(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRing(layer, cx, cy, nx, ny, r, th, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_heart(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addHeart(layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCross(layer, cx, cy, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_rounded_x(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRoundedX(layer, cx, cy, w, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_capsule(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCapsule(layer, ax, ay, bx, by, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_moon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addMoon(layer, cx, cy, d, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_egg(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addEgg(layer, cx, cy, ra, rb, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_chamfer_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addChamferBox(layer, cx, cy, hw, hh, chamfer, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_oriented_box(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addOrientedBox(layer, ax, ay, bx, by, th, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_trapezoid(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addTrapezoid(layer, cx, cy, r1, r2, he, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_parallelogram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addParallelogram(layer, cx, cy, wi, he, sk, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_equilateral_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addEquilateralTriangle(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_isosceles_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addIsoscelesTriangle(layer, cx, cy, qx, qy, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_uneven_capsule(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addUnevenCapsule(layer, cx, cy, r1, r2, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_octogon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addOctogon(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_hexagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addHexagram(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_pentagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPentagram(layer, cx, cy, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_cut_disk(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCutDisk(layer, cx, cy, r, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_horseshoe(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addHorseshoe(layer, cx, cy, sc_x, sc_y, r, wx, wy, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_vesica(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addVesica(layer, cx, cy, w, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_oriented_vesica(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addOrientedVesica(layer, ax, ay, bx, by, w, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_rounded_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRoundedCross(layer, cx, cy, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_parabola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addParabola(layer, cx, cy, k, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_blobby_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addBlobbyCross(layer, cx, cy, he, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_tunnel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addTunnel(layer, cx, cy, wh_x, wh_y, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_stairs(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addStairs(layer, cx, cy, wh_x, wh_y, n, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_quadratic_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addQuadraticCircle(layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_hyperbola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addHyperbola(layer, cx, cy, k, he, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_cool_s(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCoolS(layer, cx, cy, scale, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_circle_wave(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCircleWave(layer, cx, cy, tb, ra, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_color_wheel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addColorWheel(layer, cx, cy, outerR, innerR, hue, sat, val, indicatorSize, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_text_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addTextGlyph(layer, x, y, scaleX, scaleY, glyphIndex, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_rotated_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRotatedGlyph(layer, x, y, scaleX, scaleY, angle, glyphIndex, cosAngle, sinAngle, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_sphere_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addSphere3D(layer, px, py, pz, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_box_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addBox3D(layer, px, py, pz, bx, by, bz, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_torus_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float majorR, float minorR, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addTorus3D(layer, px, py, pz, majorR, minorR, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_cylinder_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCylinder3D(layer, px, py, pz, r, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_vertical_capsule_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addVerticalCapsule3D(layer, px, py, pz, h, r, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_capped_cone_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r1, float r2, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addCappedCone3D(layer, px, py, pz, h, r1, r2, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_octahedron_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float s, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addOctahedron3D(layer, px, py, pz, s, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_pyramid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPyramid3D(layer, px, py, pz, h, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_ellipsoid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float rx, float ry, float rz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addEllipsoid3D(layer, px, py, pz, rx, ry, rz, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_plot(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPlot(layer, x, y, w, h, dataCount, minVal, maxVal, flags, lineColor, bgColor);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_image(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addImage(layer, x, y, w, h, atlasX, atlasY, texW, texH);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_polygon(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPolygon(layer, vertexCount, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_polygon_group(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t contourCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addPolygonGroup(layer, vertexCount, contourCount, fillColor, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_linear_gradient_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addLinearGradientBox(layer, cx, cy, hw, hh, gx1, gy1, gx2, gy2, color1, color2, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_linear_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addLinearGradientCircle(layer, cx, cy, r, gx1, gy1, gx2, gy2, color1, color2, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}

extern "C" int32_t ydraw_add_radial_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gcx, float gcy, float gr, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_) {
    if (!buf || !buf->impl) return -1;
    auto result = buf->impl->addRadialGradientCircle(layer, cx, cy, r, gcx, gcy, gr, color1, color2, strokeColor, strokeWidth, round_);
    return result ? static_cast<int32_t>(*result) : -1;
}
