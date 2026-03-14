// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

#ifndef YDRAW_CAPI_GEN_H
#define YDRAW_CAPI_GEN_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * YDraw C API - Generated from ydraw-primitives.yaml
 *
 * Simple append-only buffer for YDraw primitives.
 * Auto-grows when full (doubles capacity like std::vector).
 * Usage: create -> add primitives -> get data/size -> upload to GPU -> clear -> repeat
 *===========================================================================*/

/* Opaque buffer handle */
typedef struct ydraw_buffer ydraw_buffer_t;

/*=============================================================================
 * Buffer lifecycle
 *===========================================================================*/

/* Create a new YDraw buffer. Auto-grows as needed. */
ydraw_buffer_t* ydraw_buffer_create(void);

/* Destroy buffer and free all memory */
void ydraw_buffer_destroy(ydraw_buffer_t* buf);

/* Clear all primitives (resets to empty, keeps allocated memory) */
void ydraw_buffer_clear(ydraw_buffer_t* buf);

/* Get primitive count */
uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf);

/* Get word count (floats used) */
uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf);

/* Get byte size for GPU upload */
uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf);

/* Get raw data pointer for GPU upload (read-only) */
const float* ydraw_buffer_data(const ydraw_buffer_t* buf);

/*=============================================================================
 * Text spans - high-level text storage (converted to glyphs by builder)
 *===========================================================================*/

/* Text span data */
typedef struct {
    float x, y;
    const char* text;
    float fontSize;
    uint32_t color;
    uint32_t layer;
    int fontId;
    float rotation;
} ydraw_text_span_t;

/* Add text span (fontId -1 = default font) */
int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                       float fontSize, uint32_t color, uint32_t layer, int fontId);

/* Add rotated text span */
int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                               float fontSize, uint32_t color, float rotation, int fontId);

/* Get text span count */
uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf);

/* Get text span by index (returns NULL if out of bounds) */
const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index);

/*=============================================================================
 * Primitive type enum
 *===========================================================================*/

typedef enum {

    YDRAW_CIRCLE = 0,
    YDRAW_BOX = 1,
    YDRAW_SEGMENT = 2,
    YDRAW_TRIANGLE = 3,
    YDRAW_BEZIER2 = 4,
    YDRAW_BEZIER3 = 5,
    YDRAW_ELLIPSE = 6,
    YDRAW_ARC = 7,
    YDRAW_ROUNDED_BOX = 8,
    YDRAW_RHOMBUS = 9,
    YDRAW_PENTAGON = 10,
    YDRAW_HEXAGON = 11,
    YDRAW_STAR = 12,
    YDRAW_PIE = 13,
    YDRAW_RING = 14,
    YDRAW_HEART = 15,
    YDRAW_CROSS = 16,
    YDRAW_ROUNDED_X = 17,
    YDRAW_CAPSULE = 18,
    YDRAW_MOON = 19,
    YDRAW_EGG = 20,
    YDRAW_CHAMFER_BOX = 21,
    YDRAW_ORIENTED_BOX = 22,
    YDRAW_TRAPEZOID = 23,
    YDRAW_PARALLELOGRAM = 24,
    YDRAW_EQUILATERAL_TRIANGLE = 25,
    YDRAW_ISOSCELES_TRIANGLE = 26,
    YDRAW_UNEVEN_CAPSULE = 27,
    YDRAW_OCTOGON = 28,
    YDRAW_HEXAGRAM = 29,
    YDRAW_PENTAGRAM = 30,
    YDRAW_CUT_DISK = 31,
    YDRAW_HORSESHOE = 32,
    YDRAW_VESICA = 33,
    YDRAW_ORIENTED_VESICA = 34,
    YDRAW_ROUNDED_CROSS = 35,
    YDRAW_PARABOLA = 36,
    YDRAW_BLOBBY_CROSS = 37,
    YDRAW_TUNNEL = 38,
    YDRAW_STAIRS = 39,
    YDRAW_QUADRATIC_CIRCLE = 40,
    YDRAW_HYPERBOLA = 41,
    YDRAW_COOL_S = 42,
    YDRAW_CIRCLE_WAVE = 43,
    YDRAW_COLOR_WHEEL = 44,
    YDRAW_TEXT_GLYPH = 64,
    YDRAW_ROTATED_GLYPH = 65,
    YDRAW_SPHERE_3D = 100,
    YDRAW_BOX_3D = 101,
    YDRAW_BOX_FRAME_3D = 102,
    YDRAW_TORUS_3D = 103,
    YDRAW_CAPPED_TORUS_3D = 104,
    YDRAW_CYLINDER_3D = 105,
    YDRAW_CAPPED_CYLINDER_3D = 106,
    YDRAW_ROUNDED_CYLINDER_3D = 107,
    YDRAW_VERTICAL_CAPSULE_3D = 108,
    YDRAW_CONE_3D = 109,
    YDRAW_CAPPED_CONE_3D = 110,
    YDRAW_ROUND_CONE_3D = 111,
    YDRAW_PLANE_3D = 112,
    YDRAW_HEX_PRISM_3D = 113,
    YDRAW_TRI_PRISM_3D = 114,
    YDRAW_OCTAHEDRON_3D = 115,
    YDRAW_PYRAMID_3D = 116,
    YDRAW_ELLIPSOID_3D = 117,
    YDRAW_RHOMBUS_3D = 118,
    YDRAW_LINK_3D = 119,
    YDRAW_PLOT = 128,
    YDRAW_IMAGE = 129,
    YDRAW_POLYGON = 130,
    YDRAW_POLYGON_GROUP = 131,
    YDRAW_LINEAR_GRADIENT_BOX = 132,
    YDRAW_LINEAR_GRADIENT_CIRCLE = 133,
    YDRAW_RADIAL_GRADIENT_CIRCLE = 134,
} ydraw_prim_type_t;

/*=============================================================================
 * Add primitive functions
 *
 * All functions append to buffer (auto-grows if needed).
 * Returns primitive index on success, -1 on failure (OOM).
 *===========================================================================*/

int32_t ydraw_add_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_segment(ydraw_buffer_t* buf, uint32_t layer, float x0, float y0, float x1, float y1, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_triangle(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float vx, float vy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_bezier2(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_bezier3(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_ellipse(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float rx, float ry, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_arc(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_rounded_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float r0, float r1, float r2, float r3, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_rhombus(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_pentagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_hexagon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_star(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float n, float m, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_pie(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_ring(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float nx, float ny, float r, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_heart(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_rounded_x(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_capsule(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_moon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float d, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_egg(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float ra, float rb, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_chamfer_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float chamfer, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_oriented_box(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float th, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_trapezoid(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_parallelogram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wi, float he, float sk, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_equilateral_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_isosceles_triangle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float qx, float qy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_uneven_capsule(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r1, float r2, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_octogon(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_hexagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_pentagram(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_cut_disk(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_horseshoe(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float sc_x, float sc_y, float r, float wx, float wy, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_vesica(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float w, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_oriented_vesica(ydraw_buffer_t* buf, uint32_t layer, float ax, float ay, float bx, float by, float w, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_rounded_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_parabola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_blobby_cross(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_tunnel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_stairs(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float wh_x, float wh_y, float n, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_quadratic_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_hyperbola(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float k, float he, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_cool_s(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float scale, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_circle_wave(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float tb, float ra, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_color_wheel(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float outerR, float innerR, float hue, float sat, float val, float indicatorSize, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_text_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, uint32_t glyphIndex, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_rotated_glyph(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float scaleX, float scaleY, float angle, uint32_t glyphIndex, float cosAngle, float sinAngle, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_sphere_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_box_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float bx, float by, float bz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_torus_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float majorR, float minorR, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_cylinder_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float r, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_vertical_capsule_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_capped_cone_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, float r1, float r2, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_octahedron_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float s, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_pyramid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float h, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_ellipsoid_3d(ydraw_buffer_t* buf, uint32_t layer, float px, float py, float pz, float rx, float ry, float rz, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_plot(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t dataCount, float minVal, float maxVal, uint32_t flags, uint32_t lineColor, uint32_t bgColor);
int32_t ydraw_add_image(ydraw_buffer_t* buf, uint32_t layer, float x, float y, float w, float h, uint32_t atlasX, uint32_t atlasY, uint32_t texW, uint32_t texH);
int32_t ydraw_add_polygon(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_polygon_group(ydraw_buffer_t* buf, uint32_t layer, uint32_t vertexCount, uint32_t contourCount, uint32_t fillColor, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_linear_gradient_box(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float hw, float hh, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_linear_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gx1, float gy1, float gx2, float gy2, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_);
int32_t ydraw_add_radial_gradient_circle(ydraw_buffer_t* buf, uint32_t layer, float cx, float cy, float r, float gcx, float gcy, float gr, uint32_t color1, uint32_t color2, uint32_t strokeColor, float strokeWidth, float round_);

#ifdef __cplusplus
}
#endif

#endif /* YDRAW_CAPI_GEN_H */
