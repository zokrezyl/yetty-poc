/**
 * WebGPU implementation for multi-channel signed distance field generator
 * Main implementation file
 */

#include "msdfgl.h"

// Use yetty's WebGPU compatibility macros for cross-platform support
#include <yetty/wgpu-compat.h>

#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <climits>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace msdfgl {

// Serializer scale factor (matches FreeType units)
constexpr float SERIALIZER_SCALE = 64.0f;

//------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------

void ortho(float left, float right, float bottom, float top,
           float nearVal, float farVal, float dest[16]) {
    std::memset(dest, 0, 16 * sizeof(float));

    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = -1.0f / (farVal - nearVal);

    dest[0] = 2.0f * rl;
    dest[5] = 2.0f * tb;
    dest[10] = 2.0f * fn;
    dest[12] = -(right + left) * rl;
    dest[13] = -(top + bottom) * tb;
    dest[14] = (farVal + nearVal) * fn;
    dest[15] = 1.0f;
}

uint32_t parseUtf8(const uint8_t* buf, size_t& len) {
    len++;

    uint8_t c = buf[0];
    uint8_t x = buf[0];

    // Calculate the number of leading ones
    int leadingOnes = 0;
    while (!(~x & (1 << (CHAR_BIT - 1)))) {
        x = (x << 1);
        leadingOnes++;
    }

    // Clear the leading ones
    c <<= leadingOnes;
    c >>= leadingOnes;

    // Process the remaining bytes
    uint32_t C = c;
    for (int i = 0; i < leadingOnes - 1; i++) {
        C <<= 6;
        C |= buf[i + 1] & 0x3f;
        len++;
    }
    return C;
}

// Check if code is a unicode control character
static inline bool isControl(int32_t code) {
    return (code <= 31) || (code >= 128 && code <= 159);
}

//------------------------------------------------------------------------------
// Serializer - converts FreeType glyph outlines to point/contour data
//------------------------------------------------------------------------------

namespace serializer {

enum Color {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7
};

struct Vec2 {
    float x, y;
};

inline Vec2 mix(Vec2 a, Vec2 b, float weight) {
    return {a.x * (1.0f - weight) + b.x * weight,
            a.y * (1.0f - weight) + b.y * weight};
}

inline Vec2 sub(Vec2 p1, Vec2 p2) { return {p1.x - p2.x, p1.y - p2.y}; }
inline float len(Vec2 v) { return std::sqrt(v.x * v.x + v.y * v.y); }
inline Vec2 divide(Vec2 v, float f) { return {v.x / f, v.y / f}; }
inline float cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
inline float dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

static bool isCorner(Vec2 a, Vec2 b, float crossThreshold) {
    return dot(a, b) <= 0 || std::fabs(cross(a, b)) > crossThreshold;
}

inline Vec2 normalize(Vec2 v) { return divide(v, len(v)); }

inline Vec2 segmentDirection(const Vec2* points, int npoints, float param) {
    return mix(sub(points[1], points[0]),
               sub(points[npoints - 1], points[npoints - 2]), param);
}

inline Vec2 segmentPoint(const Vec2* points, int npoints, float param) {
    return mix(mix(points[0], points[1], param),
               mix(points[npoints - 2], points[npoints - 1], param), param);
}

inline float shoelace(Vec2 a, Vec2 b) {
    return (b.x - a.x) * (a.y + b.y);
}

struct GlyphLenCtx {
    int metaSize;
    int dataSize;
};

static int addContourSize(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphLenCtx*>(user);
    ctx->dataSize += 1;
    ctx->metaSize += 2;
    return 0;
}

static int addLinearSize(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphLenCtx*>(user);
    ctx->dataSize += 1;
    ctx->metaSize += 2;
    return 0;
}

static int addQuadSize(const FT_Vector* control, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphLenCtx*>(user);
    ctx->dataSize += 2;
    ctx->metaSize += 2;
    return 0;
}

static int addCubicSize(const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* s) {
    fprintf(stderr, "msdfgl: Cubic segments not supported\n");
    return -1;
}

int glyphBufferSize(FT_Face face, int code, size_t& metaSize, size_t& pointSize) {
    if (FT_Load_Char(face, code, FT_LOAD_NO_SCALE))
        return -1;

    FT_Outline_Funcs fns = {};
    fns.shift = 0;
    fns.delta = 0;
    fns.move_to = addContourSize;
    fns.line_to = addLinearSize;
    fns.conic_to = addQuadSize;
    fns.cubic_to = addCubicSize;

    GlyphLenCtx ctx = {1, 0};
    if (FT_Outline_Decompose(&face->glyph->outline, &fns, &ctx))
        return -1;

    metaSize = ctx.metaSize;
    pointSize = ctx.dataSize * 2 * sizeof(float);
    return 0;
}

// Bounds calculation - computes actual outline bounds like msdf-gen does
struct GlyphBounds {
    float minX, minY, maxX, maxY;
    bool empty;
};

struct BoundsCtx {
    float minX, minY, maxX, maxY;
    float curX, curY;
    bool first;
};

static void updateBounds(BoundsCtx* ctx, float x, float y) {
    if (ctx->first) {
        ctx->minX = ctx->maxX = x;
        ctx->minY = ctx->maxY = y;
        ctx->first = false;
    } else {
        if (x < ctx->minX) ctx->minX = x;
        if (x > ctx->maxX) ctx->maxX = x;
        if (y < ctx->minY) ctx->minY = y;
        if (y > ctx->maxY) ctx->maxY = y;
    }
}

static int boundsMoveTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<BoundsCtx*>(user);
    float x = to->x / SERIALIZER_SCALE;
    float y = to->y / SERIALIZER_SCALE;
    updateBounds(ctx, x, y);
    ctx->curX = x;
    ctx->curY = y;
    return 0;
}

static int boundsLineTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<BoundsCtx*>(user);
    float x = to->x / SERIALIZER_SCALE;
    float y = to->y / SERIALIZER_SCALE;
    updateBounds(ctx, x, y);
    ctx->curX = x;
    ctx->curY = y;
    return 0;
}

// Evaluate quadratic bezier at parameter t
static float quadBezier(float p0, float p1, float p2, float t) {
    float mt = 1.0f - t;
    return mt * mt * p0 + 2.0f * mt * t * p1 + t * t * p2;
}

static int boundsConicTo(const FT_Vector* control, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<BoundsCtx*>(user);
    float p0x = ctx->curX, p0y = ctx->curY;  // start point
    float p1x = control->x / SERIALIZER_SCALE;  // control point
    float p1y = control->y / SERIALIZER_SCALE;
    float p2x = to->x / SERIALIZER_SCALE;  // end point
    float p2y = to->y / SERIALIZER_SCALE;

    // Always include endpoints
    updateBounds(ctx, p2x, p2y);

    // Find extrema using derivative: t = (p1-p0) / (2*p1 - p0 - p2)
    // For X axis
    float botx = 2.0f * p1x - p0x - p2x;
    if (std::abs(botx) > 1e-6f) {
        float tx = (p1x - p0x) / botx;
        if (tx > 0.0f && tx < 1.0f) {
            float ex = quadBezier(p0x, p1x, p2x, tx);
            float ey = quadBezier(p0y, p1y, p2y, tx);
            updateBounds(ctx, ex, ey);
        }
    }
    // For Y axis
    float boty = 2.0f * p1y - p0y - p2y;
    if (std::abs(boty) > 1e-6f) {
        float ty = (p1y - p0y) / boty;
        if (ty > 0.0f && ty < 1.0f) {
            float ex = quadBezier(p0x, p1x, p2x, ty);
            float ey = quadBezier(p0y, p1y, p2y, ty);
            updateBounds(ctx, ex, ey);
        }
    }

    ctx->curX = p2x;
    ctx->curY = p2y;
    return 0;
}

// Evaluate cubic bezier at parameter t
static float cubicBezier(float p0, float p1, float p2, float p3, float t) {
    float mt = 1.0f - t;
    float mt2 = mt * mt;
    float t2 = t * t;
    return mt2 * mt * p0 + 3.0f * mt2 * t * p1 + 3.0f * mt * t2 * p2 + t2 * t * p3;
}

// Solve quadratic ax² + bx + c = 0, return number of solutions in [0,1]
static int solveQuadraticIn01(float a, float b, float c, float* solutions) {
    int count = 0;
    if (std::abs(a) < 1e-12f) {
        // Linear: bx + c = 0
        if (std::abs(b) > 1e-12f) {
            float t = -c / b;
            if (t > 0.0f && t < 1.0f)
                solutions[count++] = t;
        }
    } else {
        float disc = b * b - 4.0f * a * c;
        if (disc >= 0.0f) {
            float sd = std::sqrt(disc);
            float t1 = (-b - sd) / (2.0f * a);
            float t2 = (-b + sd) / (2.0f * a);
            if (t1 > 0.0f && t1 < 1.0f)
                solutions[count++] = t1;
            if (t2 > 0.0f && t2 < 1.0f && std::abs(t2 - t1) > 1e-12f)
                solutions[count++] = t2;
        }
    }
    return count;
}

static int boundsCubicTo(const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<BoundsCtx*>(user);
    float p0x = ctx->curX, p0y = ctx->curY;
    float p1x = c1->x / SERIALIZER_SCALE, p1y = c1->y / SERIALIZER_SCALE;
    float p2x = c2->x / SERIALIZER_SCALE, p2y = c2->y / SERIALIZER_SCALE;
    float p3x = to->x / SERIALIZER_SCALE, p3y = to->y / SERIALIZER_SCALE;

    // Always include endpoints
    updateBounds(ctx, p3x, p3y);

    // Derivative coefficients: B'(t) = a2*t² + a1*t + a0
    // a0 = p1 - p0
    // a1 = 2*(p2 - p1 - a0) = 2*(p2 - 2*p1 + p0)
    // a2 = p3 - 3*p2 + 3*p1 - p0
    float solutions[2];
    int n;

    // X axis extrema
    float a0x = p1x - p0x;
    float a1x = 2.0f * (p2x - 2.0f * p1x + p0x);
    float a2x = p3x - 3.0f * p2x + 3.0f * p1x - p0x;
    n = solveQuadraticIn01(a2x, a1x, a0x, solutions);
    for (int i = 0; i < n; i++) {
        float ex = cubicBezier(p0x, p1x, p2x, p3x, solutions[i]);
        float ey = cubicBezier(p0y, p1y, p2y, p3y, solutions[i]);
        updateBounds(ctx, ex, ey);
    }

    // Y axis extrema
    float a0y = p1y - p0y;
    float a1y = 2.0f * (p2y - 2.0f * p1y + p0y);
    float a2y = p3y - 3.0f * p2y + 3.0f * p1y - p0y;
    n = solveQuadraticIn01(a2y, a1y, a0y, solutions);
    for (int i = 0; i < n; i++) {
        float ex = cubicBezier(p0x, p1x, p2x, p3x, solutions[i]);
        float ey = cubicBezier(p0y, p1y, p2y, p3y, solutions[i]);
        updateBounds(ctx, ex, ey);
    }

    ctx->curX = p3x;
    ctx->curY = p3y;
    return 0;
}

GlyphBounds getGlyphBounds(FT_Face face, int code) {
    GlyphBounds result = {0, 0, 0, 0, true};

    if (FT_Load_Char(face, code, FT_LOAD_NO_SCALE))
        return result;

    if (face->glyph->outline.n_points == 0)
        return result;  // Empty glyph

    FT_Outline_Funcs fns = {};
    fns.shift = 0;
    fns.delta = 0;
    fns.move_to = boundsMoveTo;
    fns.line_to = boundsLineTo;
    fns.conic_to = boundsConicTo;
    fns.cubic_to = boundsCubicTo;

    BoundsCtx ctx;
    ctx.first = true;
    ctx.curX = ctx.curY = 0;
    ctx.minX = ctx.minY = ctx.maxX = ctx.maxY = 0;

    if (FT_Outline_Decompose(&face->glyph->outline, &fns, &ctx))
        return result;

    if (ctx.first)
        return result;  // No points processed

    result.minX = ctx.minX;
    result.minY = ctx.minY;
    result.maxX = ctx.maxX;
    result.maxY = ctx.maxY;
    result.empty = false;
    return result;
}

struct GlyphDataCtx {
    int metaIndex;
    char* metaBuffer;
    Vec2* segment;
    int nsegmentsIndex;
};

static int addContour(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphDataCtx*>(user);

    ctx->segment += 1;
    ctx->segment[0].x = to->x / SERIALIZER_SCALE;
    ctx->segment[0].y = to->y / SERIALIZER_SCALE;

    ctx->metaBuffer[0] += 1;
    ctx->metaBuffer[ctx->metaIndex++] = 0;
    ctx->nsegmentsIndex = ctx->metaIndex++;
    ctx->metaBuffer[ctx->nsegmentsIndex] = 0;

    return 0;
}

static int addLinear(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphDataCtx*>(user);
    ctx->segment[1].x = to->x / SERIALIZER_SCALE;
    ctx->segment[1].y = to->y / SERIALIZER_SCALE;

    // Skip zero-dimensional segments
    if (ctx->segment[1].x == ctx->segment[0].x && ctx->segment[1].y == ctx->segment[0].y)
        return 0;

    ctx->segment += 1;
    ctx->metaBuffer[ctx->metaIndex++] = 0;
    ctx->metaBuffer[ctx->metaIndex++] = 2;
    ctx->metaBuffer[ctx->nsegmentsIndex]++;
    return 0;
}

static int addQuad(const FT_Vector* control, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphDataCtx*>(user);

    ctx->segment[1].x = control->x / SERIALIZER_SCALE;
    ctx->segment[1].y = control->y / SERIALIZER_SCALE;
    ctx->segment[2].x = to->x / SERIALIZER_SCALE;
    ctx->segment[2].y = to->y / SERIALIZER_SCALE;

    // Handle degenerate quad segments as linear
    if ((ctx->segment[1].x == ctx->segment[0].x && ctx->segment[1].y == ctx->segment[0].y) ||
        (ctx->segment[2].x == ctx->segment[1].x && ctx->segment[2].y == ctx->segment[1].y)) {
        return addLinear(to, user);
    }

    ctx->segment += 2;
    ctx->metaBuffer[ctx->metaIndex++] = 0;
    ctx->metaBuffer[ctx->metaIndex++] = 3;
    ctx->metaBuffer[ctx->nsegmentsIndex]++;
    return 0;
}

void switchColor(Color& color, unsigned long long& seed, Color* banned) {
    Color ban = banned ? *banned : BLACK;
    Color combined = static_cast<Color>(color & ban);

    if (combined == RED || combined == GREEN || combined == BLUE) {
        color = static_cast<Color>(combined ^ WHITE);
        return;
    }
    if (color == BLACK || color == WHITE) {
        static const Color start[3] = {CYAN, MAGENTA, YELLOW};
        color = start[seed % 3];
        seed /= 3;
        return;
    }
    int shifted = color << (1 + (seed & 1));
    color = static_cast<Color>((shifted | (shifted >> 3)) & WHITE);
    seed >>= 1;
}

int serializeGlyph(FT_Face face, int code, char* metaBuffer, float* pointBuffer) {
    if (FT_Load_Char(face, code, FT_LOAD_NO_SCALE))
        return -1;

    FT_Outline_Funcs fns = {};
    fns.shift = 0;
    fns.delta = 0;
    fns.move_to = addContour;
    fns.line_to = addLinear;
    fns.conic_to = addQuad;
    fns.cubic_to = nullptr;

    GlyphDataCtx ctx;
    ctx.metaBuffer = metaBuffer;
    ctx.metaIndex = 1;
    ctx.metaBuffer[0] = 0;
    ctx.segment = reinterpret_cast<Vec2*>(pointBuffer) - 1;

    if (FT_Outline_Decompose(&face->glyph->outline, &fns, &ctx))
        return -1;

    // Calculate windings
    int metaIndex = 0;
    Vec2* pointPtr = reinterpret_cast<Vec2*>(pointBuffer);

    int ncontours = metaBuffer[metaIndex++];
    for (int i = 0; i < ncontours; ++i) {
        int windingIndex = metaIndex++;
        int nsegments = metaBuffer[metaIndex++];

        float total = 0;
        if (nsegments == 1) {
            int npoints = metaBuffer[metaIndex + 1];
            Vec2 a = segmentPoint(pointPtr, npoints, 0);
            Vec2 b = segmentPoint(pointPtr, npoints, 1 / 3.0f);
            Vec2 c = segmentPoint(pointPtr, npoints, 2 / 3.0f);
            total += shoelace(a, b);
            total += shoelace(b, c);
            total += shoelace(c, a);
            pointPtr += npoints - 1;
            metaIndex += 2;
        } else if (nsegments == 2) {
            int npoints = metaBuffer[metaIndex + 1];
            Vec2 a = segmentPoint(pointPtr, npoints, 0);
            Vec2 b = segmentPoint(pointPtr, npoints, 0.5f);
            pointPtr += npoints - 1;
            metaIndex += 2;
            npoints = metaBuffer[metaIndex + 1];
            Vec2 c = segmentPoint(pointPtr, npoints, 0);
            Vec2 d = segmentPoint(pointPtr, npoints, 0.5f);
            total += shoelace(a, b);
            total += shoelace(b, c);
            total += shoelace(c, d);
            total += shoelace(d, a);
            pointPtr += npoints - 1;
            metaIndex += 2;
        } else {
            int prevNpoints = metaBuffer[metaIndex + 2 * (nsegments - 2) + 1];
            Vec2* prevPtr = pointPtr;
            for (int j = 0; j < nsegments - 1; ++j) {
                int _npoints = metaBuffer[metaIndex + 2 * j + 1];
                prevPtr += (_npoints - 1);
            }
            Vec2 prev = segmentPoint(prevPtr, prevNpoints, 0);

            for (int j = 0; j < nsegments; ++j) {
                metaIndex++;
                int npoints = metaBuffer[metaIndex++];
                Vec2 cur = segmentPoint(pointPtr, npoints, 0);
                total += shoelace(prev, cur);
                pointPtr += (npoints - 1);
                prev = cur;
            }
        }
        pointPtr += 1;
        metaBuffer[windingIndex] = total > 0 ? 2 : 0;
    }

    // Calculate coloring
    float crossThreshold = std::sin(3.0f);
    unsigned long long seed = 0;

    metaIndex = 0;
    pointPtr = reinterpret_cast<Vec2*>(pointBuffer);

    std::vector<int> corners;
    corners.reserve(64);  // Reserve reasonable space, will grow if needed

    ncontours = metaBuffer[metaIndex++];
    for (int i = 0; i < ncontours; ++i) {
        metaIndex++;  // Winding
        int nsegments = metaBuffer[metaIndex++];
        int _meta = metaIndex;
        Vec2* _point = pointPtr;

        corners.clear();

        if (nsegments) {
            int prevNpoints = metaBuffer[metaIndex + 2 * (nsegments - 2) + 1];
            Vec2* prevPtr = pointPtr;
            for (int j = 0; j < nsegments - 1; ++j)
                prevPtr += (metaBuffer[metaIndex + 2 * j + 1] - 1);
            Vec2 prevDirection = segmentDirection(prevPtr, prevNpoints, 1);
            int index = 0;
            Vec2* curPoints = pointPtr;
            for (int j = 0; j < nsegments; ++j, ++index) {
                metaIndex++;
                int npoints = metaBuffer[metaIndex++];
                Vec2 curDirection = segmentDirection(curPoints, npoints, 0.0f);
                Vec2 newPrevDirection = segmentDirection(curPoints, npoints, 1.0f);

                if (isCorner(normalize(prevDirection), normalize(curDirection), crossThreshold))
                    corners.push_back(index);
                curPoints += (npoints - 1);
                prevDirection = newPrevDirection;
            }
        }

        metaIndex = _meta;
        pointPtr = _point;

        if (corners.empty()) {
            // Smooth contour
            for (int j = 0; j < nsegments; ++j) {
                metaBuffer[metaIndex++] = WHITE;
                metaIndex++;
            }
        } else if (corners.size() == 1) {
            // Teardrop
            Color colors[3] = {WHITE, WHITE, WHITE};
            switchColor(colors[0], seed, nullptr);
            colors[2] = colors[0];
            switchColor(colors[2], seed, nullptr);

            int corner = corners[0];
            if (nsegments >= 3) {
                int m = nsegments;
                for (int k = 0; k < m; ++k) {
                    Color c = colors[1 + static_cast<int>(3 + 2.875 * k / (m - 1) - 1.4375 + .5) - 3];
                    metaBuffer[metaIndex + 2 * ((corner + k) % m)] = static_cast<char>(c);
                }
            } else if (nsegments >= 1) {
                fprintf(stderr, "msdfgl: Non-supported shape\n");
            }
        } else {
            // Multiple corners
            size_t cornerCount = corners.size();
            int spline = 0;
            int start = corners[0];
            int m = nsegments;
            Color color = WHITE;
            switchColor(color, seed, nullptr);
            Color initialColor = color;
            for (int k = 0; k < m; ++k) {
                int index = (start + k) % m;

                if (static_cast<size_t>(spline + 1) < cornerCount && corners[spline + 1] == index) {
                    ++spline;
                    Color banned = static_cast<Color>((static_cast<size_t>(spline) == cornerCount - 1) * initialColor);
                    switchColor(color, seed, &banned);
                }
                metaBuffer[metaIndex + 2 * index] = static_cast<char>(color);
            }
        }

        metaIndex = _meta;
        pointPtr = _point;

        for (int j = 0; j < nsegments; ++j) {
            metaIndex++;
            pointPtr += (metaBuffer[metaIndex++] - 1);
        }
        pointPtr += 1;
    }

    return 0;
}

}  // namespace serializer

//------------------------------------------------------------------------------
// Atlas implementation
//------------------------------------------------------------------------------

Atlas::Atlas(WGPUDevice device, int textureWidth, int padding)
    : _device(device), _textureWidth(textureWidth), _padding(padding) {

    // Create index buffer (start with small allocation)
    WGPUBufferDescriptor indexBufDesc = {};
    indexBufDesc.label = WGPU_STR("MSDFGL Index Buffer");
    indexBufDesc.size = sizeof(IndexEntry) * 256;
    indexBufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst | WGPUBufferUsage_CopySrc;
    _indexBuffer = wgpuDeviceCreateBuffer(_device, &indexBufDesc);
    _nallocated = 256;
}

Atlas::~Atlas() {
    cleanup();
}

Atlas::Atlas(Atlas&& other) noexcept {
    *this = std::move(other);
}

Atlas& Atlas::operator=(Atlas&& other) noexcept {
    if (this != &other) {
        cleanup();
        _device = other._device;
        _atlasTexture = other._atlasTexture;
        _atlasTextureView = other._atlasTextureView;
        _indexBuffer = other._indexBuffer;
        _textureWidth = other._textureWidth;
        _textureHeight = other._textureHeight;
        _nglyphs = other._nglyphs;
        _nallocated = other._nallocated;
        _offsetX = other._offsetX;
        _offsetY = other._offsetY;
        _yIncrement = other._yIncrement;
        _padding = other._padding;
        _refcount = other._refcount;
        _implicit = other._implicit;

        other._device = nullptr;
        other._atlasTexture = nullptr;
        other._atlasTextureView = nullptr;
        other._indexBuffer = nullptr;
    }
    return *this;
}

void Atlas::cleanup() {
    if (_atlasTextureView) {
        wgpuTextureViewRelease(_atlasTextureView);
        _atlasTextureView = nullptr;
    }
    if (_atlasTexture) {
        wgpuTextureDestroy(_atlasTexture);
        wgpuTextureRelease(_atlasTexture);
        _atlasTexture = nullptr;
    }
    if (_indexBuffer) {
        wgpuBufferDestroy(_indexBuffer);
        wgpuBufferRelease(_indexBuffer);
        _indexBuffer = nullptr;
    }
}

bool Atlas::allocateGlyph(float width, float height, float& outX, float& outY) {
    if (_offsetX + width > _textureWidth) {
        _offsetY += (_yIncrement + _padding);
        _offsetX = 1;
        _yIncrement = 0;
    }

    _yIncrement = std::max(_yIncrement, static_cast<size_t>(height));

    outX = static_cast<float>(_offsetX);
    outY = static_cast<float>(_offsetY);

    _offsetX += static_cast<size_t>(width) + _padding;

    return true;
}

bool Atlas::resizeTexture(int newHeight) {
    if (newHeight <= _textureHeight) return true;

    // Create new texture
    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("MSDFGL Atlas Texture");
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {static_cast<uint32_t>(_textureWidth), static_cast<uint32_t>(newHeight), 1};
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.format = WGPUTextureFormat_RGBA32Float;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_RenderAttachment |
                    WGPUTextureUsage_CopySrc | WGPUTextureUsage_CopyDst;

    WGPUTexture newTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!newTexture) return false;

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.label = WGPU_STR("MSDFGL Atlas Texture View");
    viewDesc.format = WGPUTextureFormat_RGBA32Float;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.baseMipLevel = 0;
    viewDesc.mipLevelCount = 1;
    viewDesc.baseArrayLayer = 0;
    viewDesc.arrayLayerCount = 1;

    WGPUTextureView newView = wgpuTextureCreateView(newTexture, &viewDesc);

    // Copy old texture data if it exists
    if (_atlasTexture && _textureHeight > 0) {
        // In WebGPU, we'd need to use a command encoder to copy
        // For simplicity, we'll skip the copy for now and let the atlas be regenerated
        // In production, you'd use wgpuCommandEncoderCopyTextureToTexture
    }

    // Release old resources
    if (_atlasTextureView) wgpuTextureViewRelease(_atlasTextureView);
    if (_atlasTexture) {
        wgpuTextureDestroy(_atlasTexture);
        wgpuTextureRelease(_atlasTexture);
    }

    _atlasTexture = newTexture;
    _atlasTextureView = newView;
    _textureHeight = newHeight;

    return true;
}

void Atlas::updateIndexBuffer(const std::vector<IndexEntry>& entries, size_t startIndex) {
    // Resize buffer if needed
    size_t requiredSize = (startIndex + entries.size()) * sizeof(IndexEntry);
    if (requiredSize > _nallocated * sizeof(IndexEntry)) {
        size_t newAlloc = _nallocated;
        while (newAlloc * sizeof(IndexEntry) < requiredSize) {
            newAlloc *= 2;
        }

        WGPUBufferDescriptor bufDesc = {};
        bufDesc.label = WGPU_STR("MSDFGL Index Buffer");
        bufDesc.size = newAlloc * sizeof(IndexEntry);
        bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst | WGPUBufferUsage_CopySrc;

        WGPUBuffer newBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);

        // Copy old data to new buffer
        if (_indexBuffer && _nglyphs > 0) {
            WGPUCommandEncoderDescriptor encDesc = {};
            WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);
            wgpuCommandEncoderCopyBufferToBuffer(encoder, _indexBuffer, 0, newBuffer, 0,
                                                  _nglyphs * sizeof(IndexEntry));
            WGPUCommandBufferDescriptor cmdBufDesc = {};
            WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
            wgpuQueueSubmit(wgpuDeviceGetQueue(_device), 1, &cmdBuffer);
            wgpuCommandBufferRelease(cmdBuffer);
            wgpuCommandEncoderRelease(encoder);
        }

        if (_indexBuffer) {
            wgpuBufferDestroy(_indexBuffer);
            wgpuBufferRelease(_indexBuffer);
        }
        _indexBuffer = newBuffer;
        _nallocated = newAlloc;
    }

    // Write new entries
    wgpuQueueWriteBuffer(wgpuDeviceGetQueue(_device), _indexBuffer,
                         startIndex * sizeof(IndexEntry),
                         entries.data(), entries.size() * sizeof(IndexEntry));
}

//------------------------------------------------------------------------------
// Font implementation
//------------------------------------------------------------------------------

Font::Font(Context& ctx, FT_Face face, const FontConfig& config, std::shared_ptr<Atlas> atlas)
    : _context(ctx), _face(face), _scale(config.scale), _range(config.range), _atlas(atlas) {

    _verticalAdvance = static_cast<float>(_face->ascender - _face->descender);
    initMap();

    // Create GPU buffers for glyph generation
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

    bufDesc.label = WGPU_STR("MSDFGL Meta Input Buffer");
    bufDesc.size = 4096;  // Initial size
    _metaInputBuffer = wgpuDeviceCreateBuffer(_context.getDevice(), &bufDesc);

    bufDesc.label = WGPU_STR("MSDFGL Point Input Buffer");
    bufDesc.size = 16384;  // Initial size
    _pointInputBuffer = wgpuDeviceCreateBuffer(_context.getDevice(), &bufDesc);
}

Font::~Font() {
    cleanup();
}

void Font::cleanup() {
    if (_metaInputBuffer) {
        wgpuBufferDestroy(_metaInputBuffer);
        wgpuBufferRelease(_metaInputBuffer);
        _metaInputBuffer = nullptr;
    }
    if (_pointInputBuffer) {
        wgpuBufferDestroy(_pointInputBuffer);
        wgpuBufferRelease(_pointInputBuffer);
        _pointInputBuffer = nullptr;
    }
    if (_face) {
        FT_Done_Face(_face);
        _face = nullptr;
    }
}

void Font::initMap() {
    for (auto& item : _linearMap) {
        item.index = -1;
    }
}

MapItem* Font::mapGet(uint32_t code) {
    if (code < 256) {
        return _linearMap[code].index != -1 ? &_linearMap[code] : nullptr;
    }

    // Binary search in dynamic map
    auto it = std::lower_bound(_dynamicMap.begin(), _dynamicMap.end(), code,
        [](const MapItem& item, uint32_t c) { return item.code < c; });

    if (it != _dynamicMap.end() && it->code == code) {
        return &(*it);
    }
    return nullptr;
}

MapItem* Font::mapInsert(uint32_t code) {
    if (code < 256) {
        _linearMap[code].code = code;
        return &_linearMap[code];
    }

    // Find insertion point
    auto it = std::lower_bound(_dynamicMap.begin(), _dynamicMap.end(), code,
        [](const MapItem& item, uint32_t c) { return item.code < c; });

    if (it != _dynamicMap.end() && it->code == code) {
        return &(*it);
    }

    // Insert new item
    MapItem newItem;
    newItem.code = code;
    newItem.index = -1;
    newItem.advance[0] = 0;
    newItem.advance[1] = 0;
    it = _dynamicMap.insert(it, newItem);
    return &(*it);
}

MapItem* Font::getGlyph(int32_t codepoint) {
    return mapGet(static_cast<uint32_t>(codepoint));
}

MapItem* Font::getOrAddGlyph(int32_t codepoint) {
    MapItem* item = mapGet(static_cast<uint32_t>(codepoint));
    if (!item) {
        if (_context._missingGlyphCallback) {
            if (_context._missingGlyphCallback(*this, codepoint)) {
                item = mapGet(static_cast<uint32_t>(codepoint));
            }
        }
    }
    return item;
}

int Font::generateGlyph(int32_t codepoint) {
    return _context.generateGlyphsInternal(*this, codepoint, codepoint + 1, true, nullptr, 0);
}

int Font::generateGlyphs(int32_t start, int32_t end) {
    return _context.generateGlyphsInternal(*this, start, end + 1, true, nullptr, 0);
}

int Font::generateGlyphList(const std::vector<int32_t>& codepoints) {
    return _context.generateGlyphsInternal(*this, 0, 0, false, codepoints.data(),
                                            static_cast<int>(codepoints.size()));
}

float Font::getVerticalAdvance(float size) const {
    return _verticalAdvance * (size * _context.getDpiY() / 72.0f) / _face->units_per_EM;
}

//------------------------------------------------------------------------------
// Context implementation
//------------------------------------------------------------------------------

// Shader file paths - these need to be set before Context creation
static std::string sGenShaderPath = "shaders/msdf_gen.wgsl";
static std::string sRenderShaderPath = "shaders/font_render.wgsl";

// Helper to load shader from file
static std::string loadShaderFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::fprintf(stderr, "[MSDFGL] Failed to open shader file: %s\n", path.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Set shader paths before creating Context
void setShaderPaths(const std::string& genPath, const std::string& renderPath) {
    sGenShaderPath = genPath;
    sRenderShaderPath = renderPath;
}

Context::Context(WGPUDevice device, const ContextConfig& config)
    : _device(device), _dpi{config.dpiX, config.dpiY} {

    // Initialize FreeType
    if (FT_Init_FreeType(&_ftLibrary)) {
        throw std::runtime_error("Failed to initialize FreeType");
    }

    // Use a reasonable default max texture size (WebGPU typically supports at least 8192)
    // Most modern GPUs support 16384, but 8192 is a safe default
    _maxTextureSize = 8192;

    if (!initShaders()) {
        throw std::runtime_error("Failed to initialize shaders");
    }

    if (!initPipelines()) {
        throw std::runtime_error("Failed to initialize pipelines");
    }

    // Create atlas sampler
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.label = WGPU_STR("MSDFGL Atlas Sampler");
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Nearest;
    samplerDesc.maxAnisotropy = 1;
    _atlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);

    // Create uniform buffers
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;

    bufDesc.label = WGPU_STR("MSDFGL Gen Uniform Buffer");
    bufDesc.size = 256;  // Padded for alignment
    _genUniformBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);

    bufDesc.label = WGPU_STR("MSDFGL Render Uniform Buffer");
    bufDesc.size = 256;
    _renderUniformBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
}

Context::~Context() {
    cleanup();
}

void Context::cleanup() {
    if (_atlasSampler) {
        wgpuSamplerRelease(_atlasSampler);
        _atlasSampler = nullptr;
    }
    if (_genUniformBuffer) {
        wgpuBufferDestroy(_genUniformBuffer);
        wgpuBufferRelease(_genUniformBuffer);
        _genUniformBuffer = nullptr;
    }
    if (_renderUniformBuffer) {
        wgpuBufferDestroy(_renderUniformBuffer);
        wgpuBufferRelease(_renderUniformBuffer);
        _renderUniformBuffer = nullptr;
    }
    if (_genPipeline) {
        wgpuRenderPipelineRelease(_genPipeline);
        _genPipeline = nullptr;
    }
    if (_renderPipeline) {
        wgpuRenderPipelineRelease(_renderPipeline);
        _renderPipeline = nullptr;
    }
    if (_genShaderModule) {
        wgpuShaderModuleRelease(_genShaderModule);
        _genShaderModule = nullptr;
    }
    if (_renderShaderModule) {
        wgpuShaderModuleRelease(_renderShaderModule);
        _renderShaderModule = nullptr;
    }
    if (_genBindGroupLayout) {
        wgpuBindGroupLayoutRelease(_genBindGroupLayout);
        _genBindGroupLayout = nullptr;
    }
    if (_renderBindGroupLayout) {
        wgpuBindGroupLayoutRelease(_renderBindGroupLayout);
        _renderBindGroupLayout = nullptr;
    }
    if (_ftLibrary) {
        FT_Done_FreeType(_ftLibrary);
        _ftLibrary = nullptr;
    }
}

bool Context::initShaders() {
    std::fprintf(stderr, "[MSDFGL] initShaders() starting...\n");

    // Load generation shader from file
    std::string genShaderSource = loadShaderFile(sGenShaderPath);
    if (genShaderSource.empty()) {
        std::fprintf(stderr, "[MSDFGL] Failed to load gen shader from: %s\n", sGenShaderPath.c_str());
        return false;
    }
    std::fprintf(stderr, "[MSDFGL] Gen shader loaded from %s, length: %zu bytes\n",
                 sGenShaderPath.c_str(), genShaderSource.size());

    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, genShaderSource);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.label = WGPU_STR("MSDFGL Gen Shader");
    shaderDesc.nextInChain = &wgslDesc.chain;

    _genShaderModule = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    std::fprintf(stderr, "[MSDFGL] Gen shader module: %p\n", (void*)_genShaderModule);
    if (!_genShaderModule) {
        std::fprintf(stderr, "[MSDFGL] FAILED to create gen shader module!\n");
        return false;
    }

    // Load render shader from file
    std::string renderShaderSource = loadShaderFile(sRenderShaderPath);
    if (renderShaderSource.empty()) {
        std::fprintf(stderr, "[MSDFGL] Failed to load render shader from: %s\n", sRenderShaderPath.c_str());
        return false;
    }
    std::fprintf(stderr, "[MSDFGL] Render shader loaded from %s, length: %zu bytes\n",
                 sRenderShaderPath.c_str(), renderShaderSource.size());

    WGPU_SHADER_CODE(wgslDesc, renderShaderSource);
    shaderDesc.label = WGPU_STR("MSDFGL Render Shader");

    _renderShaderModule = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    std::fprintf(stderr, "[MSDFGL] Render shader module: %p\n", (void*)_renderShaderModule);
    if (!_renderShaderModule) {
        std::fprintf(stderr, "[MSDFGL] FAILED to create render shader module!\n");
        return false;
    }

    std::fprintf(stderr, "[MSDFGL] initShaders() SUCCESS\n");
    return true;
}

bool Context::initPipelines() {
    std::fprintf(stderr, "[MSDFGL] initPipelines() starting...\n");

    // Create bind group layouts

    // Generation pipeline bind group layout
    WGPUBindGroupLayoutEntry genBindGroupEntries[3] = {};

    // Uniform buffer
    genBindGroupEntries[0].binding = 0;
    genBindGroupEntries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    genBindGroupEntries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // Metadata storage buffer
    genBindGroupEntries[1].binding = 1;
    genBindGroupEntries[1].visibility = WGPUShaderStage_Fragment;
    genBindGroupEntries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // Point data storage buffer
    genBindGroupEntries[2].binding = 2;
    genBindGroupEntries[2].visibility = WGPUShaderStage_Fragment;
    genBindGroupEntries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor genBindGroupLayoutDesc = {};
    genBindGroupLayoutDesc.label = WGPU_STR("MSDFGL Gen Bind Group Layout");
    genBindGroupLayoutDesc.entryCount = 3;
    genBindGroupLayoutDesc.entries = genBindGroupEntries;

    _genBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &genBindGroupLayoutDesc);

    // Render pipeline bind group layout
    WGPUBindGroupLayoutEntry renderBindGroupEntries[4] = {};

    // Uniform buffer
    renderBindGroupEntries[0].binding = 0;
    renderBindGroupEntries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    renderBindGroupEntries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // Atlas texture
    renderBindGroupEntries[1].binding = 1;
    renderBindGroupEntries[1].visibility = WGPUShaderStage_Fragment;
    renderBindGroupEntries[1].texture.sampleType = WGPUTextureSampleType_Float;
    renderBindGroupEntries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

    // Sampler
    renderBindGroupEntries[2].binding = 2;
    renderBindGroupEntries[2].visibility = WGPUShaderStage_Fragment;
    renderBindGroupEntries[2].sampler.type = WGPUSamplerBindingType_Filtering;

    // Index storage buffer
    renderBindGroupEntries[3].binding = 3;
    renderBindGroupEntries[3].visibility = WGPUShaderStage_Vertex;
    renderBindGroupEntries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor renderBindGroupLayoutDesc = {};
    renderBindGroupLayoutDesc.label = WGPU_STR("MSDFGL Render Bind Group Layout");
    renderBindGroupLayoutDesc.entryCount = 4;
    renderBindGroupLayoutDesc.entries = renderBindGroupEntries;

    _renderBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &renderBindGroupLayoutDesc);

    // Create pipeline layouts
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &_genBindGroupLayout;

    WGPUPipelineLayout genPipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &pipelineLayoutDesc);

    pipelineLayoutDesc.bindGroupLayouts = &_renderBindGroupLayout;
    WGPUPipelineLayout renderPipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &pipelineLayoutDesc);

    // Create generation pipeline
    WGPUVertexAttribute genVertexAttribs[1] = {};
    genVertexAttribs[0].format = WGPUVertexFormat_Float32x2;
    genVertexAttribs[0].offset = 0;
    genVertexAttribs[0].shaderLocation = 0;

    WGPUVertexBufferLayout genVertexBufferLayout = {};
    genVertexBufferLayout.arrayStride = sizeof(float) * 2;
    genVertexBufferLayout.stepMode = WGPUVertexStepMode_Vertex;
    genVertexBufferLayout.attributeCount = 1;
    genVertexBufferLayout.attributes = genVertexAttribs;

    WGPUColorTargetState genColorTarget = {};
    genColorTarget.format = WGPUTextureFormat_RGBA32Float;
    genColorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState genFragmentState = {};
    genFragmentState.module = _genShaderModule;
    genFragmentState.entryPoint = WGPU_STR("fs_main");
    genFragmentState.targetCount = 1;
    genFragmentState.targets = &genColorTarget;

    WGPURenderPipelineDescriptor genPipelineDesc = {};
    genPipelineDesc.label = WGPU_STR("MSDFGL Gen Pipeline");
    genPipelineDesc.layout = genPipelineLayout;
    genPipelineDesc.vertex.module = _genShaderModule;
    genPipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    genPipelineDesc.vertex.bufferCount = 1;
    genPipelineDesc.vertex.buffers = &genVertexBufferLayout;
    genPipelineDesc.fragment = &genFragmentState;
    genPipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    genPipelineDesc.multisample.count = 1;
    genPipelineDesc.multisample.mask = ~0u;
    genPipelineDesc.multisample.alphaToCoverageEnabled = false;

    _genPipeline = wgpuDeviceCreateRenderPipeline(_device, &genPipelineDesc);

    // Create render pipeline
    WGPUVertexAttribute renderVertexAttribs[7] = {};
    renderVertexAttribs[0].format = WGPUVertexFormat_Float32x2;  // position
    renderVertexAttribs[0].offset = offsetof(Glyph, x);
    renderVertexAttribs[0].shaderLocation = 0;
    renderVertexAttribs[1].format = WGPUVertexFormat_Unorm8x4;   // color
    renderVertexAttribs[1].offset = offsetof(Glyph, color);
    renderVertexAttribs[1].shaderLocation = 1;
    renderVertexAttribs[2].format = WGPUVertexFormat_Sint32;     // key
    renderVertexAttribs[2].offset = offsetof(Glyph, key);
    renderVertexAttribs[2].shaderLocation = 2;
    renderVertexAttribs[3].format = WGPUVertexFormat_Float32;    // size
    renderVertexAttribs[3].offset = offsetof(Glyph, size);
    renderVertexAttribs[3].shaderLocation = 3;
    renderVertexAttribs[4].format = WGPUVertexFormat_Float32;    // offset
    renderVertexAttribs[4].offset = offsetof(Glyph, offset);
    renderVertexAttribs[4].shaderLocation = 4;
    renderVertexAttribs[5].format = WGPUVertexFormat_Float32;    // skew
    renderVertexAttribs[5].offset = offsetof(Glyph, skew);
    renderVertexAttribs[5].shaderLocation = 5;
    renderVertexAttribs[6].format = WGPUVertexFormat_Float32;    // strength
    renderVertexAttribs[6].offset = offsetof(Glyph, strength);
    renderVertexAttribs[6].shaderLocation = 6;

    WGPUVertexBufferLayout renderVertexBufferLayout = {};
    renderVertexBufferLayout.arrayStride = sizeof(Glyph);
    renderVertexBufferLayout.stepMode = WGPUVertexStepMode_Instance;
    renderVertexBufferLayout.attributeCount = 7;
    renderVertexBufferLayout.attributes = renderVertexAttribs;

    WGPUColorTargetState renderColorTarget = {};
    renderColorTarget.format = WGPUTextureFormat_BGRA8Unorm;  // Typical swapchain format
    renderColorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUBlendState blendState = {};
    blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blendState.color.operation = WGPUBlendOperation_Add;
    blendState.alpha.srcFactor = WGPUBlendFactor_One;
    blendState.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blendState.alpha.operation = WGPUBlendOperation_Add;
    renderColorTarget.blend = &blendState;

    WGPUFragmentState renderFragmentState = {};
    renderFragmentState.module = _renderShaderModule;
    renderFragmentState.entryPoint = WGPU_STR("fs_main");
    renderFragmentState.targetCount = 1;
    renderFragmentState.targets = &renderColorTarget;

    WGPURenderPipelineDescriptor renderPipelineDesc = {};
    renderPipelineDesc.label = WGPU_STR("MSDFGL Render Pipeline");
    renderPipelineDesc.layout = renderPipelineLayout;
    renderPipelineDesc.vertex.module = _renderShaderModule;
    renderPipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    renderPipelineDesc.vertex.bufferCount = 1;
    renderPipelineDesc.vertex.buffers = &renderVertexBufferLayout;
    renderPipelineDesc.fragment = &renderFragmentState;
    renderPipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    renderPipelineDesc.multisample.count = 1;
    renderPipelineDesc.multisample.mask = ~0u;
    renderPipelineDesc.multisample.alphaToCoverageEnabled = false;

    _renderPipeline = wgpuDeviceCreateRenderPipeline(_device, &renderPipelineDesc);

    std::fprintf(stderr, "[MSDFGL] initPipelines() - genPipeline=%p, renderPipeline=%p\n",
                 (void*)_genPipeline, (void*)_renderPipeline);

    wgpuPipelineLayoutRelease(genPipelineLayout);
    wgpuPipelineLayoutRelease(renderPipelineLayout);

    bool success = _genPipeline && _renderPipeline;
    std::fprintf(stderr, "[MSDFGL] initPipelines() %s\n", success ? "SUCCESS" : "FAILED");
    return success;
}

std::unique_ptr<Font> Context::loadFont(const std::string& fontPath, const FontConfig& config,
                                         std::shared_ptr<Atlas> atlas) {
    FT_Face face;
    if (FT_New_Face(_ftLibrary, fontPath.c_str(), 0, &face)) {
        return nullptr;
    }

    if (!atlas) {
        atlas = createAtlas();
    }

    return std::make_unique<Font>(*this, face, config, atlas);
}

std::unique_ptr<Font> Context::loadFontMem(const void* data, size_t size, const FontConfig& config,
                                            std::shared_ptr<Atlas> atlas) {
    FT_Face face;
    if (FT_New_Memory_Face(_ftLibrary, static_cast<const FT_Byte*>(data), size, 0, &face)) {
        return nullptr;
    }

    if (!atlas) {
        atlas = createAtlas();
    }

    return std::make_unique<Font>(*this, face, config, atlas);
}

std::shared_ptr<Atlas> Context::createAtlas(const AtlasConfig& config) {
    int width = config.textureWidth > 0 ? config.textureWidth : _maxTextureSize;
    return std::make_shared<Atlas>(_device, width, config.padding);
}

void Context::setDpi(float horizontal, float vertical) {
    _dpi[0] = horizontal;
    _dpi[1] = vertical;
}

void Context::setMissingGlyphCallback(MissingGlyphCallback callback) {
    _missingGlyphCallback = std::move(callback);
}

int Context::generateGlyphsInternal(Font& font, int32_t start, int32_t end,
                                     bool useRange, const int32_t* keys, int nkeys) {
    int nrender = useRange ? (end - start) : nkeys;
    if (nrender <= 0) return -1;

    auto atlas = font.getAtlas();

    // Calculate buffer sizes and serialize glyphs
    std::vector<size_t> metaSizes(nrender);
    std::vector<size_t> pointSizes(nrender);
    std::vector<IndexEntry> atlasIndex(nrender);

    size_t metaSizeSum = 0, pointSizeSum = 0;

    for (int i = 0; i < nrender; ++i) {
        int code = useRange ? start + i : keys[i];
        if (serializer::glyphBufferSize(font.getFace(), code, metaSizes[i], pointSizes[i]) < 0) {
            metaSizes[i] = 0;
            pointSizes[i] = 0;
        }
        metaSizeSum += metaSizes[i];
        pointSizeSum += pointSizes[i];
    }

    // Allocate and fill buffers
    std::vector<char> metadata(metaSizeSum);
    std::vector<float> pointData(pointSizeSum / sizeof(float));

    char* metaPtr = metadata.data();
    float* pointPtr = pointData.data();

    int newTextureHeight = atlas->getTextureHeight() ? atlas->getTextureHeight() : 1;

    for (int i = 0; i < nrender; ++i) {
        int code = useRange ? start + i : keys[i];

        serializer::serializeGlyph(font.getFace(), code, metaPtr, pointPtr);

        MapItem* m = font.mapInsert(static_cast<uint32_t>(code));
        m->index = static_cast<int32_t>(atlas->_nglyphs + i);
        m->advance[0] = static_cast<float>(font.getFace()->glyph->metrics.horiAdvance);
        m->advance[1] = static_cast<float>(font.getFace()->glyph->metrics.vertAdvance);

        // Handle control characters
        if (useRange && start == 0 && code != 0 && isControl(code)) {
            atlasIndex[i] = atlasIndex[0];
            metaPtr += metaSizes[i];
            pointPtr += pointSizes[i] / sizeof(float);
            continue;
        }

        // Get actual outline bounds (like msdf-gen does) for precise sizing
        auto bounds = serializer::getGlyphBounds(font.getFace(), code);

        float bufferWidth, bufferHeight;
        float glyphWidth, glyphHeight;

        if (bounds.empty) {
            // Fallback to FreeType metrics for empty/unusual glyphs
            // Use same formula as bounds case: ceil(size * scale) + padding * 2
            float metricsWidth = font.getFace()->glyph->metrics.width / SERIALIZER_SCALE;
            float metricsHeight = font.getFace()->glyph->metrics.height / SERIALIZER_SCALE;
            int padding = static_cast<int>(std::ceil(font.getRange()));
            float logicalSizeX = metricsWidth * font.getScale();
            float logicalSizeY = metricsHeight * font.getScale();
            bufferWidth = std::ceil(logicalSizeX) + padding * 2;
            bufferHeight = std::ceil(logicalSizeY) + padding * 2;
            glyphWidth = static_cast<float>(font.getFace()->glyph->metrics.width);
            glyphHeight = static_cast<float>(font.getFace()->glyph->metrics.height);
        } else {
            // Use actual outline bounds (matching CPU generator formula exactly)
            // CPU: bitmapW = ceil(boundsWidth * scale) + padding * 2
            float boundsWidth = bounds.maxX - bounds.minX;
            float boundsHeight = bounds.maxY - bounds.minY;
            int padding = static_cast<int>(std::ceil(font.getRange()));
            float logicalSizeX = boundsWidth * font.getScale();
            float logicalSizeY = boundsHeight * font.getScale();
            bufferWidth = std::ceil(logicalSizeX) + padding * 2;
            bufferHeight = std::ceil(logicalSizeY) + padding * 2;
            // Store actual glyph dimensions in font units (for consistency with FreeType format)
            glyphWidth = boundsWidth * SERIALIZER_SCALE;
            glyphHeight = boundsHeight * SERIALIZER_SCALE;
        }

        float offsetX, offsetY;
        atlas->allocateGlyph(bufferWidth, bufferHeight, offsetX, offsetY);

        atlasIndex[i].offsetX = offsetX;
        atlasIndex[i].offsetY = offsetY;
        atlasIndex[i].sizeX = bufferWidth;
        atlasIndex[i].sizeY = bufferHeight;
        atlasIndex[i].bearingX = static_cast<float>(font.getFace()->glyph->metrics.horiBearingX);
        atlasIndex[i].bearingY = static_cast<float>(font.getFace()->glyph->metrics.horiBearingY);
        atlasIndex[i].glyphWidth = glyphWidth;
        atlasIndex[i].glyphHeight = glyphHeight;

        // Check if we need to expand texture height
        while ((atlas->_offsetY + bufferHeight) > newTextureHeight) {
            newTextureHeight *= 2;
        }
        if (newTextureHeight > _maxTextureSize) {
            return -2;  // Texture too large
        }

        metaPtr += metaSizes[i];
        pointPtr += pointSizes[i] / sizeof(float);
    }

    // Resize atlas texture if needed
    if (newTextureHeight > atlas->getTextureHeight()) {
        atlas->resizeTexture(newTextureHeight);
    }

    // Update index buffer
    atlas->updateIndexBuffer(atlasIndex, atlas->_nglyphs);

    // Upload glyph data to GPU buffers
    WGPUQueue queue = wgpuDeviceGetQueue(_device);

    if (!metadata.empty()) {
        // Convert metadata from bytes to uint32s (shader expects array<u32>)
        std::vector<uint32_t> metadataU32(metadata.size());
        for (size_t i = 0; i < metadata.size(); ++i) {
            metadataU32[i] = static_cast<uint32_t>(static_cast<unsigned char>(metadata[i]));
        }
        size_t requiredSize = metadataU32.size() * sizeof(uint32_t);
        // Recreate buffer if too small
        if (wgpuBufferGetSize(font._metaInputBuffer) < requiredSize) {
            wgpuBufferDestroy(font._metaInputBuffer);
            wgpuBufferRelease(font._metaInputBuffer);
            WGPUBufferDescriptor bufDesc = {};
            bufDesc.label = WGPU_STR("MSDFGL Meta Input Buffer");
            bufDesc.size = requiredSize;
            bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
            font._metaInputBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
        }
        wgpuQueueWriteBuffer(queue, font._metaInputBuffer, 0, metadataU32.data(), requiredSize);
    }
    if (!pointData.empty()) {
        size_t requiredSize = pointData.size() * sizeof(float);
        // Recreate buffer if too small
        if (wgpuBufferGetSize(font._pointInputBuffer) < requiredSize) {
            wgpuBufferDestroy(font._pointInputBuffer);
            wgpuBufferRelease(font._pointInputBuffer);
            WGPUBufferDescriptor bufDesc = {};
            bufDesc.label = WGPU_STR("MSDFGL Point Input Buffer");
            bufDesc.size = requiredSize;
            bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
            font._pointInputBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
        }
        wgpuQueueWriteBuffer(queue, font._pointInputBuffer, 0, pointData.data(), requiredSize);
    }

    // Generate MSDF for each glyph using the GPU
    // Create orthographic projection for atlas rendering
    float atlasW = static_cast<float>(atlas->getTextureWidth());
    float atlasH = static_cast<float>(atlas->getTextureHeight());
    float projection[16] = {
        2.0f / atlasW, 0, 0, 0,
        0, -2.0f / atlasH, 0, 0,
        0, 0, 1, 0,
        -1, 1, 0, 1
    };

    // Create command encoder for all glyph generation
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encoderDesc);

    // Collect resources to release after submit
    std::vector<WGPUBuffer> vertexBuffers;
    std::vector<WGPUBuffer> uniformBuffers;
    std::vector<WGPUBindGroup> bindGroups;

    // Accumulate offsets into metadata and point buffers
    size_t metaOffset = 0;
    size_t pointOffset = 0;

    for (int i = 0; i < nrender; ++i) {
        // Skip glyphs with no data
        if (metaSizes[i] == 0 || atlasIndex[i].sizeX <= 0 || atlasIndex[i].sizeY <= 0) {
            metaOffset += metaSizes[i];
            pointOffset += pointSizes[i] / sizeof(float);
            continue;
        }

        float glyphW = atlasIndex[i].sizeX;
        float glyphH = atlasIndex[i].sizeY;
        float offsetX = atlasIndex[i].offsetX;
        float offsetY = atlasIndex[i].offsetY;

        // Create quad vertices for this glyph (2 triangles)
        float vertices[] = {
            0, 0,
            glyphW, 0,
            0, glyphH,
            glyphW, 0,
            glyphW, glyphH,
            0, glyphH
        };

        // Create vertex buffer for this glyph's quad
        WGPUBufferDescriptor vertBufDesc = {};
        vertBufDesc.label = WGPU_STR("MSDFGL Glyph Vertex Buffer");
        vertBufDesc.size = sizeof(vertices);
        vertBufDesc.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
        WGPUBuffer vertexBuffer = wgpuDeviceCreateBuffer(_device, &vertBufDesc);
        wgpuQueueWriteBuffer(queue, vertexBuffer, 0, vertices, sizeof(vertices));
        vertexBuffers.push_back(vertexBuffer);

        // Set up uniforms for this glyph
        struct GenUniforms {
            float projection[16];
            float offset[2];
            float translate[2];
            float scale[2];
            float range;
            int32_t meta_offset;
            int32_t point_offset;
            float glyph_height;
            float padding[2];  // Pad to 16-byte alignment
        } uniforms;

        memcpy(uniforms.projection, projection, sizeof(projection));
        uniforms.offset[0] = offsetX;
        uniforms.offset[1] = offsetY;
        uniforms.translate[0] = font.getRange();  // Padding offset
        uniforms.translate[1] = font.getRange();
        uniforms.scale[0] = font.getScale();
        uniforms.scale[1] = font.getScale();
        uniforms.range = font.getRange();
        // metaOffset is in bytes, and each byte is now a uint32 element
        uniforms.meta_offset = static_cast<int32_t>(metaOffset);
        // pointOffset is in floats, but shader expects point index (each point = 2 floats)
        uniforms.point_offset = static_cast<int32_t>(pointOffset / 2);
        uniforms.glyph_height = glyphH;
        uniforms.padding[0] = 0;
        uniforms.padding[1] = 0;

        // Create per-glyph uniform buffer (needed because GPU reads happen after all writes)
        WGPUBufferDescriptor uniformBufDesc = {};
        uniformBufDesc.label = WGPU_STR("MSDFGL Glyph Uniform Buffer");
        uniformBufDesc.size = sizeof(uniforms);
        uniformBufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        WGPUBuffer uniformBuffer = wgpuDeviceCreateBuffer(_device, &uniformBufDesc);
        wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &uniforms, sizeof(uniforms));
        uniformBuffers.push_back(uniformBuffer);

        // Create bind group for this glyph
        WGPUBindGroupEntry entries[3] = {};
        entries[0].binding = 0;
        entries[0].buffer = uniformBuffer;
        entries[0].size = sizeof(uniforms);

        entries[1].binding = 1;
        entries[1].buffer = font._metaInputBuffer;
        entries[1].size = wgpuBufferGetSize(font._metaInputBuffer);

        entries[2].binding = 2;
        entries[2].buffer = font._pointInputBuffer;
        entries[2].size = wgpuBufferGetSize(font._pointInputBuffer);

        WGPUBindGroupDescriptor bindGroupDesc = {};
        bindGroupDesc.layout = _genBindGroupLayout;
        bindGroupDesc.entryCount = 3;
        bindGroupDesc.entries = entries;
        WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(_device, &bindGroupDesc);
        bindGroups.push_back(bindGroup);

        // Create render pass targeting the atlas texture
        WGPURenderPassColorAttachment colorAttachment = {};
        colorAttachment.view = atlas->getAtlasTextureView();
        colorAttachment.loadOp = WGPULoadOp_Load;
        colorAttachment.storeOp = WGPUStoreOp_Store;
        colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;  // Required for 2D textures

        WGPURenderPassDescriptor passDesc = {};
        passDesc.colorAttachmentCount = 1;
        passDesc.colorAttachments = &colorAttachment;

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
        wgpuRenderPassEncoderSetPipeline(pass, _genPipeline);
        wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
        wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertexBuffer, 0, sizeof(vertices));
        wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);

        metaOffset += metaSizes[i];
        pointOffset += pointSizes[i] / sizeof(float);
    }

    // Submit all glyph generation commands
    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);
    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);

    // Now release resources after submit
    for (auto bg : bindGroups) {
        wgpuBindGroupRelease(bg);
    }
    for (auto vb : vertexBuffers) {
        wgpuBufferDestroy(vb);
        wgpuBufferRelease(vb);
    }
    for (auto ub : uniformBuffers) {
        wgpuBufferDestroy(ub);
        wgpuBufferRelease(ub);
    }

    atlas->_nglyphs += nrender;
    return nrender;
}

static int sRenderCallCount = 0;

void Context::render(Font& font, const std::vector<Glyph>& glyphs, const float projection[16],
                     WGPUTextureView targetView) {
    sRenderCallCount++;

    if (sRenderCallCount <= 5) {
        std::fprintf(stderr, "[MSDFGL::render] call #%d, %zu glyphs\n", sRenderCallCount, glyphs.size());
    }

    if (glyphs.empty()) {
        if (sRenderCallCount <= 5) {
            std::fprintf(stderr, "[MSDFGL::render] No glyphs to render!\n");
        }
        return;
    }

    auto atlas = font.getAtlas();
    if (!atlas || !atlas->getAtlasTextureView()) {
        if (sRenderCallCount <= 5) {
            std::fprintf(stderr, "[MSDFGL::render] Atlas not available! atlas=%p, view=%p\n",
                        (void*)atlas.get(), atlas ? (void*)atlas->getAtlasTextureView() : nullptr);
        }
        return;
    }

    if (sRenderCallCount <= 5) {
        std::fprintf(stderr, "[MSDFGL::render] Atlas: %dx%d, pipeline=%p, bindGroupLayout=%p\n",
                    atlas->getTextureWidth(), atlas->getTextureHeight(),
                    (void*)_renderPipeline, (void*)_renderBindGroupLayout);
    }

    // Prepare glyphs - convert keys to indices
    std::vector<Glyph> preparedGlyphs = glyphs;
    for (auto& glyph : preparedGlyphs) {
        if (glyph.key >= font._directLookupUpperLimit) {
            MapItem* item = font.getGlyph(glyph.key);
            glyph.key = item ? item->index : 0;
        }
    }

    // Create glyph buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("MSDFGL Glyph Buffer");
    bufDesc.size = preparedGlyphs.size() * sizeof(Glyph);
    bufDesc.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;

    WGPUBuffer glyphBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    WGPUQueue queue = wgpuDeviceGetQueue(_device);
    wgpuQueueWriteBuffer(queue, glyphBuffer, 0, preparedGlyphs.data(), bufDesc.size);

    // Update uniform buffer
    struct RenderUniforms {
        float projection[16];
        float fontProjection[16];
        float padding;
        float unitsPerEm;
        float dpi[2];
    } uniforms;

    std::memcpy(uniforms.projection, projection, 16 * sizeof(float));

    // Font projection for atlas texture coordinates
    ortho(-static_cast<float>(atlas->getTextureWidth()),
          static_cast<float>(atlas->getTextureWidth()),
          -static_cast<float>(atlas->getTextureHeight()),
          static_cast<float>(atlas->getTextureHeight()),
          -1.0f, 1.0f, uniforms.fontProjection);

    uniforms.padding = font.getRange() / 2.0f * SERIALIZER_SCALE;
    uniforms.unitsPerEm = static_cast<float>(font.getFace()->units_per_EM);
    uniforms.dpi[0] = _dpi[0];
    uniforms.dpi[1] = _dpi[1];

    wgpuQueueWriteBuffer(queue, _renderUniformBuffer, 0, &uniforms, sizeof(uniforms));

    // Create bind group
    WGPUBindGroupEntry bindGroupEntries[4] = {};
    bindGroupEntries[0].binding = 0;
    bindGroupEntries[0].buffer = _renderUniformBuffer;
    bindGroupEntries[0].size = sizeof(uniforms);
    bindGroupEntries[1].binding = 1;
    bindGroupEntries[1].textureView = atlas->getAtlasTextureView();
    bindGroupEntries[2].binding = 2;
    bindGroupEntries[2].sampler = _atlasSampler;
    bindGroupEntries[3].binding = 3;
    bindGroupEntries[3].buffer = atlas->getIndexBuffer();
    bindGroupEntries[3].size = atlas->_nallocated * sizeof(IndexEntry);

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.label = WGPU_STR("MSDFGL Render Bind Group");
    bindGroupDesc.layout = _renderBindGroupLayout;
    bindGroupDesc.entryCount = 4;
    bindGroupDesc.entries = bindGroupEntries;

    WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(_device, &bindGroupDesc);

    // Create command encoder
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encoderDesc);

    // Create render pass
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Load;
    colorAttachment.storeOp = WGPUStoreOp_Store;

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);

    wgpuRenderPassEncoderSetPipeline(renderPass, _renderPipeline);
    wgpuRenderPassEncoderSetBindGroup(renderPass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, glyphBuffer, 0, bufDesc.size);

    // Draw 6 vertices (2 triangles) per glyph instance
    wgpuRenderPassEncoderDraw(renderPass, 6, static_cast<uint32_t>(preparedGlyphs.size()), 0, 0);

    wgpuRenderPassEncoderEnd(renderPass);

    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    // Cleanup
    wgpuCommandBufferRelease(cmdBuffer);
    wgpuRenderPassEncoderRelease(renderPass);
    wgpuCommandEncoderRelease(encoder);
    wgpuBindGroupRelease(bindGroup);
    wgpuBufferDestroy(glyphBuffer);
    wgpuBufferRelease(glyphBuffer);
}

static int sPrintfCallCount = 0;

float Context::printf(float x, float y, Font& font, float size, uint32_t color,
                      const float projection[16], WGPUTextureView targetView,
                      PrintfFlags flags, const char* fmt, ...) {
    sPrintfCallCount++;

    va_list args;
    va_start(args, fmt);
    int len = std::vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    if (sPrintfCallCount <= 5) {
        std::fprintf(stderr, "[MSDFGL::printf] call #%d, len=%d, size=%.1f, color=0x%08X\n",
                     sPrintfCallCount, len, size, color);
    }

    if (len <= 0) return (flags & PrintfFlags::Vertical) ? y : x;

    std::vector<char> buffer(len + 1);
    va_start(args, fmt);
    std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);

    std::vector<Glyph> glyphs;
    glyphs.reserve(len);

    size_t bufIdx = 0;
    size_t bufSize = static_cast<size_t>(len);

    while (bufIdx < bufSize) {
        Glyph glyph;
        glyph.x = x;
        glyph.y = y;
        glyph.color = color;
        glyph.size = size;
        glyph.offset = 0;
        glyph.skew = 0;
        glyph.strength = 0.5f;

        if (flags & PrintfFlags::Utf8) {
            glyph.key = parseUtf8(reinterpret_cast<const uint8_t*>(buffer.data() + bufIdx), bufIdx);
        } else {
            glyph.key = static_cast<uint8_t>(buffer[bufIdx++]);
        }

        MapItem* item = font.getOrAddGlyph(glyph.key);
        if (!item) continue;

        glyphs.push_back(glyph);

        // Apply kerning if requested
        if ((flags & PrintfFlags::Kerning) && glyphs.size() > 1 && FT_HAS_KERNING(font.getFace())) {
            FT_Vector kerning;
            FT_Get_Kerning(font.getFace(),
                          FT_Get_Char_Index(font.getFace(), glyphs[glyphs.size() - 2].key),
                          FT_Get_Char_Index(font.getFace(), glyph.key),
                          FT_KERNING_UNSCALED, &kerning);

            if (flags & PrintfFlags::Vertical) {
                glyphs[glyphs.size() - 2].y += kerning.y * (size * _dpi[1] / 72.0f) /
                                                font.getFace()->units_per_EM;
            } else {
                glyphs[glyphs.size() - 2].x += kerning.x * (size * _dpi[0] / 72.0f) /
                                                font.getFace()->units_per_EM;
            }
        }

        // Advance position
        if (flags & PrintfFlags::Vertical) {
            y += item->advance[1] * (size * _dpi[1] / 72.0f) / font.getFace()->units_per_EM;
        } else {
            x += item->advance[0] * (size * _dpi[0] / 72.0f) / font.getFace()->units_per_EM;
        }
    }

    render(font, glyphs, projection, targetView);

    return (flags & PrintfFlags::Vertical) ? y : x;
}

void Context::geometry(float& x, float& y, Font& font, float size, PrintfFlags flags,
                       const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = std::vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    if (len <= 0) return;

    std::vector<char> buffer(len + 1);
    va_start(args, fmt);
    std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);

    size_t bufIdx = 0;
    size_t bufSize = static_cast<size_t>(len);
    int32_t prevKey = -1;

    while (bufIdx < bufSize) {
        int32_t key;
        if (flags & PrintfFlags::Utf8) {
            key = parseUtf8(reinterpret_cast<const uint8_t*>(buffer.data() + bufIdx), bufIdx);
        } else {
            key = static_cast<uint8_t>(buffer[bufIdx++]);
        }

        MapItem* item = font.getOrAddGlyph(key);
        if (!item) continue;

        FT_Vector kerning = {0, 0};
        if ((flags & PrintfFlags::Kerning) && prevKey >= 0 && FT_HAS_KERNING(font.getFace())) {
            FT_Get_Kerning(font.getFace(),
                          FT_Get_Char_Index(font.getFace(), prevKey),
                          FT_Get_Char_Index(font.getFace(), key),
                          FT_KERNING_UNSCALED, &kerning);
        }

        if (flags & PrintfFlags::Vertical) {
            y += (item->advance[1] + kerning.y) * (size * _dpi[1] / 72.0f) / font.getFace()->units_per_EM;
        } else {
            x += (item->advance[0] + kerning.x) * (size * _dpi[0] / 72.0f) / font.getFace()->units_per_EM;
        }

        prevKey = key;
    }
}

}  // namespace msdfgl
