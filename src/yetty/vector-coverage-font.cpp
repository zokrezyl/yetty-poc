#include <yetty/vector-coverage-font.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

namespace yetty {

//=============================================================================
// Internal types (not exposed in header)
//=============================================================================

namespace {

// Packed quadratic Bezier curve (12 bytes = 3 x u32)
struct PackedCurve {
    uint32_t p0;  // x[16] | y[16]
    uint32_t p1;  // x[16] | y[16]
    uint32_t p2;  // x[16] | y[16]
};

// Glyph flags
constexpr uint16_t GLYPH_FLAG_EMPTY = 0x0001;

// Pack normalized point (0-1) into uint32
inline uint32_t packPoint(float x, float y) {
    uint16_t px = static_cast<uint16_t>(std::clamp(x, 0.0f, 1.0f) * 65535.0f);
    uint16_t py = static_cast<uint16_t>(std::clamp(y, 0.0f, 1.0f) * 65535.0f);
    return (static_cast<uint32_t>(px) << 16) | py;
}

// FreeType scale factor
constexpr float FT_SCALE = 64.0f;

//=============================================================================
// FreeType outline extraction context
//=============================================================================

struct Vec2 { float x, y; };

struct OutlineCtx {
    std::vector<PackedCurve> curves;
    Vec2 contourStart{0, 0};
    Vec2 lastPoint{0, 0};
    float minX, minY, maxX, maxY;  // Bounding box
    bool hasPoints = false;

    void updateBounds(float x, float y) {
        if (!hasPoints) {
            minX = maxX = x;
            minY = maxY = y;
            hasPoints = true;
        } else {
            minX = std::min(minX, x);
            minY = std::min(minY, y);
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
        }
    }
};

// Normalize point to [0,1] based on bounding box
inline Vec2 normalize(Vec2 p, float minX, float minY, float maxX, float maxY) {
    float w = maxX - minX;
    float h = maxY - minY;
    if (w < 1e-6f) w = 1.0f;
    if (h < 1e-6f) h = 1.0f;
    return {(p.x - minX) / w, (p.y - minY) / h};
}

//=============================================================================
// FreeType outline callbacks (first pass: collect bounds)
//=============================================================================

int boundsMoveTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<OutlineCtx*>(user);
    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;
    ctx->updateBounds(x, y);
    ctx->contourStart = {x, y};
    ctx->lastPoint = {x, y};
    return 0;
}

int boundsLineTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<OutlineCtx*>(user);
    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;
    ctx->updateBounds(x, y);
    ctx->lastPoint = {x, y};
    return 0;
}

int boundsConicTo(const FT_Vector* ctrl, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<OutlineCtx*>(user);
    ctx->updateBounds(ctrl->x / FT_SCALE, ctrl->y / FT_SCALE);
    ctx->updateBounds(to->x / FT_SCALE, to->y / FT_SCALE);
    ctx->lastPoint = {to->x / FT_SCALE, to->y / FT_SCALE};
    return 0;
}

int boundsCubicTo(const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<OutlineCtx*>(user);
    ctx->updateBounds(c1->x / FT_SCALE, c1->y / FT_SCALE);
    ctx->updateBounds(c2->x / FT_SCALE, c2->y / FT_SCALE);
    ctx->updateBounds(to->x / FT_SCALE, to->y / FT_SCALE);
    ctx->lastPoint = {to->x / FT_SCALE, to->y / FT_SCALE};
    return 0;
}

//=============================================================================
// FreeType outline callbacks (second pass: extract normalized curves)
//=============================================================================

struct CurveCtx {
    std::vector<PackedCurve>* curves;
    Vec2 contourStart{0, 0};
    Vec2 lastPoint{0, 0};
    float minX, minY, maxX, maxY;
};

int curveMoveTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<CurveCtx*>(user);
    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;
    ctx->contourStart = {x, y};
    ctx->lastPoint = {x, y};
    return 0;
}

int curveLineTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<CurveCtx*>(user);
    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;

    // Line as degenerate quadratic (control point at midpoint)
    Vec2 p0 = normalize(ctx->lastPoint, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 p2 = normalize({x, y}, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 p1 = {(p0.x + p2.x) * 0.5f, (p0.y + p2.y) * 0.5f};

    ctx->curves->push_back({
        packPoint(p0.x, p0.y),
        packPoint(p1.x, p1.y),
        packPoint(p2.x, p2.y)
    });

    ctx->lastPoint = {x, y};
    return 0;
}

int curveConicTo(const FT_Vector* ctrl, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<CurveCtx*>(user);

    Vec2 p0 = normalize(ctx->lastPoint, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 p1 = normalize({ctrl->x / FT_SCALE, ctrl->y / FT_SCALE},
                        ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 p2 = normalize({to->x / FT_SCALE, to->y / FT_SCALE},
                        ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);

    ctx->curves->push_back({
        packPoint(p0.x, p0.y),
        packPoint(p1.x, p1.y),
        packPoint(p2.x, p2.y)
    });

    ctx->lastPoint = {to->x / FT_SCALE, to->y / FT_SCALE};
    return 0;
}

int curveCubicTo(const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<CurveCtx*>(user);

    // Approximate cubic with 2 quadratics using midpoint subdivision
    Vec2 p0 = ctx->lastPoint;
    Vec2 cp1 = {c1->x / FT_SCALE, c1->y / FT_SCALE};
    Vec2 cp2 = {c2->x / FT_SCALE, c2->y / FT_SCALE};
    Vec2 p3 = {to->x / FT_SCALE, to->y / FT_SCALE};

    // De Casteljau midpoint
    Vec2 q0 = {(p0.x + cp1.x) * 0.5f, (p0.y + cp1.y) * 0.5f};
    Vec2 q1 = {(cp1.x + cp2.x) * 0.5f, (cp1.y + cp2.y) * 0.5f};
    Vec2 q2 = {(cp2.x + p3.x) * 0.5f, (cp2.y + p3.y) * 0.5f};
    Vec2 r0 = {(q0.x + q1.x) * 0.5f, (q0.y + q1.y) * 0.5f};
    Vec2 r1 = {(q1.x + q2.x) * 0.5f, (q1.y + q2.y) * 0.5f};
    Vec2 mid = {(r0.x + r1.x) * 0.5f, (r0.y + r1.y) * 0.5f};

    // First quadratic: p0 -> q0 -> mid (approximate)
    Vec2 np0 = normalize(p0, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 nq0 = normalize(q0, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 nmid = normalize(mid, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);

    ctx->curves->push_back({
        packPoint(np0.x, np0.y),
        packPoint(nq0.x, nq0.y),
        packPoint(nmid.x, nmid.y)
    });

    // Second quadratic: mid -> r1 -> p3
    Vec2 nr1 = normalize(r1, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);
    Vec2 np3 = normalize(p3, ctx->minX, ctx->minY, ctx->maxX, ctx->maxY);

    ctx->curves->push_back({
        packPoint(nmid.x, nmid.y),
        packPoint(nr1.x, nr1.y),
        packPoint(np3.x, np3.y)
    });

    ctx->lastPoint = p3;
    return 0;
}

} // anonymous namespace

//=============================================================================
// VectorCoverageFontImpl
//=============================================================================

class VectorCoverageFontImpl : public VectorCoverageFont {
public:
    VectorCoverageFontImpl(const GPUContext& gpu, const std::string& ttfPath)
        : _gpu(gpu), _ttfPath(ttfPath) {}

    ~VectorCoverageFontImpl() override {
        cleanup();
    }

    //=========================================================================
    // Initialization
    //=========================================================================

    Result<void> init() noexcept {
        // Initialize FreeType
        if (FT_Init_FreeType(&_ftLibrary)) {
            return Err<void>("Failed to initialize FreeType");
        }

        // Load font face
        if (FT_New_Face(_ftLibrary, _ttfPath.c_str(), 0, &_ftFace)) {
            FT_Done_FreeType(_ftLibrary);
            _ftLibrary = nullptr;
            return Err<void>("Failed to load font: " + _ttfPath);
        }

        _unitsPerEM = _ftFace->units_per_EM;

        // Capture font-wide metrics for consistent glyph normalization
        // These are in font units, we'll normalize to [0,1] based on these
        _ascender = static_cast<float>(_ftFace->ascender) / FT_SCALE;
        _descender = static_cast<float>(_ftFace->descender) / FT_SCALE;

        // Get advance width from a representative glyph ('M' or '0')
        if (FT_Load_Char(_ftFace, 'M', FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP) == 0) {
            _advanceWidth = static_cast<float>(_ftFace->glyph->advance.x) / FT_SCALE;
        } else {
            _advanceWidth = static_cast<float>(_unitsPerEM) / FT_SCALE;
        }

        yinfo("VectorCoverageFont loaded: {} (units_per_EM={} ascender={:.1f} descender={:.1f} advance={:.1f})",
              _ttfPath, _unitsPerEM, _ascender, _descender, _advanceWidth);

        return Ok();
    }

    //=========================================================================
    // Glyph loading
    //=========================================================================

    Result<void> loadGlyphs(const std::vector<uint32_t>& codepoints) override {
        for (uint32_t cp : codepoints) {
            if (_glyphOffsets.count(cp)) continue;  // Already loaded

            auto result = extractGlyph(cp);
            if (!result) {
                ywarn("Failed to extract glyph for U+{:04X}", cp);
                continue;
            }
        }

        // Rebuild GPU buffers
        return uploadToGPU();
    }

    Result<void> loadBasicLatin() override {
        std::vector<uint32_t> codepoints;
        for (uint32_t cp = 0x20; cp <= 0x7E; ++cp) {
            codepoints.push_back(cp);
        }
        return loadGlyphs(codepoints);
    }

    //=========================================================================
    // GPU buffer access
    //=========================================================================

    WGPUBuffer getGlyphBuffer() const override { return _glyphBuffer; }
    WGPUBuffer getOffsetBuffer() const override { return _offsetBuffer; }

    //=========================================================================
    // Statistics
    //=========================================================================

    size_t glyphCount() const override { return _glyphOffsets.size(); }

    size_t totalCurves() const override {
        return _totalCurves;
    }

    size_t bufferSize() const override {
        return _glyphData.size() * sizeof(uint32_t);
    }

    size_t offsetTableSize() const override {
        return 256 * sizeof(uint32_t);  // Fixed size offset table
    }

    //=========================================================================
    // Font interface
    //=========================================================================

    uint32_t getGlyphIndex(uint32_t codepoint) override {
        auto it = _glyphOffsets.find(codepoint);
        if (it != _glyphOffsets.end()) {
            return it->second;  // Return offset as "index"
        }
        return 0;
    }

    uint32_t getGlyphIndex(uint32_t codepoint, Style) override {
        return getGlyphIndex(codepoint);  // Styles not supported yet
    }

    uint32_t getGlyphIndex(uint32_t codepoint, bool, bool) override {
        return getGlyphIndex(codepoint);
    }

    void uploadToGpu() override {
        // Already uploaded in loadGlyphs
        _dirty = false;
    }

    bool isDirty() const override { return _dirty; }
    void clearDirty() override { _dirty = false; }

    uint32_t getAtlasWidth() const override { return 0; }   // No atlas
    uint32_t getAtlasHeight() const override { return 0; }

    const std::vector<uint8_t>& getAtlasData() const override {
        static std::vector<uint8_t> empty;
        return empty;
    }

private:
    //=========================================================================
    // Extract glyph curves from FreeType
    //=========================================================================

    Result<void> extractGlyph(uint32_t codepoint) {
        if (FT_Load_Char(_ftFace, codepoint, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP)) {
            return Err<void>("FT_Load_Char failed");
        }

        FT_Outline& outline = _ftFace->glyph->outline;

        // Empty glyph (e.g., space)
        if (outline.n_points == 0) {
            uint32_t offset = static_cast<uint32_t>(_glyphData.size());
            _glyphOffsets[codepoint] = offset;
            _glyphData.push_back((0 << 16) | GLYPH_FLAG_EMPTY);  // 0 curves, empty flag
            return Ok();
        }

        // First pass: get bounding box
        OutlineCtx boundsCtx;
        FT_Outline_Funcs boundsFuncs = {};
        boundsFuncs.move_to = boundsMoveTo;
        boundsFuncs.line_to = boundsLineTo;
        boundsFuncs.conic_to = boundsConicTo;
        boundsFuncs.cubic_to = boundsCubicTo;

        if (FT_Outline_Decompose(&outline, &boundsFuncs, &boundsCtx)) {
            return Err<void>("FT_Outline_Decompose (bounds) failed");
        }

        if (!boundsCtx.hasPoints) {
            // No actual points
            uint32_t offset = static_cast<uint32_t>(_glyphData.size());
            _glyphOffsets[codepoint] = offset;
            _glyphData.push_back((0 << 16) | GLYPH_FLAG_EMPTY);
            return Ok();
        }

        // Second pass: extract normalized curves using FONT-WIDE metrics
        // This ensures 'a' is smaller than 'H', proper baseline, etc.
        std::vector<PackedCurve> curves;
        CurveCtx curveCtx;
        curveCtx.curves = &curves;
        // Use font metrics for normalization, not per-glyph bounds
        curveCtx.minX = 0;
        curveCtx.minY = _descender;
        curveCtx.maxX = _advanceWidth;
        curveCtx.maxY = _ascender;

        FT_Outline_Funcs curveFuncs = {};
        curveFuncs.move_to = curveMoveTo;
        curveFuncs.line_to = curveLineTo;
        curveFuncs.conic_to = curveConicTo;
        curveFuncs.cubic_to = curveCubicTo;

        if (FT_Outline_Decompose(&outline, &curveFuncs, &curveCtx)) {
            return Err<void>("FT_Outline_Decompose (curves) failed");
        }

        // Store in glyph data buffer
        uint32_t offset = static_cast<uint32_t>(_glyphData.size());
        _glyphOffsets[codepoint] = offset;

        // Header: curveCount[16] | flags[16]
        uint16_t curveCount = static_cast<uint16_t>(std::min(curves.size(), size_t(65535)));
        uint16_t flags = 0;
        _glyphData.push_back((static_cast<uint32_t>(curveCount) << 16) | flags);

        // Curves
        for (const auto& curve : curves) {
            _glyphData.push_back(curve.p0);
            _glyphData.push_back(curve.p1);
            _glyphData.push_back(curve.p2);
        }

        _totalCurves += curves.size();
        _dirty = true;

        return Ok();
    }

    //=========================================================================
    // Upload to GPU
    //=========================================================================

    Result<void> uploadToGPU() {
        if (_glyphData.empty()) {
            return Ok();  // Nothing to upload
        }

        WGPUDevice device = _gpu.device;
        WGPUQueue queue = _gpu.queue;

        // Destroy old buffers
        if (_glyphBuffer) {
            wgpuBufferDestroy(_glyphBuffer);
            wgpuBufferRelease(_glyphBuffer);
            _glyphBuffer = nullptr;
        }
        if (_offsetBuffer) {
            wgpuBufferDestroy(_offsetBuffer);
            wgpuBufferRelease(_offsetBuffer);
            _offsetBuffer = nullptr;
        }

        // Create glyph data buffer
        WGPUBufferDescriptor bufDesc = {};
        bufDesc.label = WGPU_STR("VectorCoverageFont Glyph Buffer");
        bufDesc.size = _glyphData.size() * sizeof(uint32_t);
        bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

        _glyphBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);
        if (!_glyphBuffer) {
            return Err<void>("Failed to create glyph buffer");
        }

        wgpuQueueWriteBuffer(queue, _glyphBuffer, 0,
                             _glyphData.data(), _glyphData.size() * sizeof(uint32_t));

        // Create offset table buffer (sparse: 256 entries for now)
        constexpr size_t OFFSET_TABLE_SIZE = 256;
        std::vector<uint32_t> offsetTable(OFFSET_TABLE_SIZE, 0xFFFFFFFF);  // Invalid marker

        for (const auto& [cp, off] : _glyphOffsets) {
            if (cp < OFFSET_TABLE_SIZE) {
                offsetTable[cp] = off;
            }
        }

        bufDesc.label = WGPU_STR("VectorCoverageFont Offset Buffer");
        bufDesc.size = offsetTable.size() * sizeof(uint32_t);

        _offsetBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);
        if (!_offsetBuffer) {
            return Err<void>("Failed to create offset buffer");
        }

        wgpuQueueWriteBuffer(queue, _offsetBuffer, 0,
                             offsetTable.data(), offsetTable.size() * sizeof(uint32_t));

        yinfo("VectorCoverageFont uploaded: {} glyphs, {} curves, {} bytes",
              _glyphOffsets.size(), _totalCurves, bufferSize());

        _dirty = false;
        return Ok();
    }

    //=========================================================================
    // Cleanup
    //=========================================================================

    void cleanup() {
        if (_glyphBuffer) {
            wgpuBufferDestroy(_glyphBuffer);
            wgpuBufferRelease(_glyphBuffer);
            _glyphBuffer = nullptr;
        }
        if (_offsetBuffer) {
            wgpuBufferDestroy(_offsetBuffer);
            wgpuBufferRelease(_offsetBuffer);
            _offsetBuffer = nullptr;
        }
        if (_ftFace) {
            FT_Done_Face(_ftFace);
            _ftFace = nullptr;
        }
        if (_ftLibrary) {
            FT_Done_FreeType(_ftLibrary);
            _ftLibrary = nullptr;
        }
    }

    //=========================================================================
    // Private data
    //=========================================================================

    const GPUContext& _gpu;
    std::string _ttfPath;

    // FreeType
    FT_Library _ftLibrary = nullptr;
    FT_Face _ftFace = nullptr;
    int _unitsPerEM = 0;

    // Font-wide metrics for consistent glyph sizing
    float _ascender = 0;    // Top of em-square (positive)
    float _descender = 0;   // Bottom of em-square (negative typically)
    float _advanceWidth = 0; // Monospace advance width

    // Glyph data
    std::vector<uint32_t> _glyphData;                    // Packed glyph buffer
    std::unordered_map<uint32_t, uint32_t> _glyphOffsets;  // codepoint -> offset
    size_t _totalCurves = 0;

    // GPU resources
    WGPUBuffer _glyphBuffer = nullptr;
    WGPUBuffer _offsetBuffer = nullptr;

    bool _dirty = false;
};

//=============================================================================
// VectorCoverageFont::createImpl - ObjectFactory entry point
//=============================================================================

Result<VectorCoverageFont::Ptr> VectorCoverageFont::createImpl(ContextType&,
                                                const GPUContext& gpu,
                                                const std::string& ttfPath) {
    auto font = Ptr(new VectorCoverageFontImpl(gpu, ttfPath));
    if (auto res = static_cast<VectorCoverageFontImpl*>(font.get())->init(); !res) {
        return Err<Ptr>("Failed to initialize VectorCoverageFont", res);
    }
    return Ok(std::move(font));
}

} // namespace yetty
