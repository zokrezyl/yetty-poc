/**
 * MSDF-WGSL: Compute shader-based MSDF generation for WebGPU
 */

#include "msdf-wgsl.h"
#include <yetty/wgpu-compat.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <atomic>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace msdf {

// Scale factor for FreeType units
constexpr float FT_SCALE = 64.0f;

// Shader path
static std::string sShaderPath = "";

void setShaderPath(const std::string& path) {
    sShaderPath = path;
}

// Get directory of current executable
static std::string getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string p(path);
    auto pos = p.find_last_of("\\/");
    return pos != std::string::npos ? p.substr(0, pos) : ".";
#else
    char path[4096];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len > 0) {
        path[len] = '\0';
        std::string p(path);
        auto pos = p.find_last_of('/');
        return pos != std::string::npos ? p.substr(0, pos) : ".";
    }
    return ".";
#endif
}

// Find shader file, trying multiple locations
static std::string findShaderPath() {
    // 1. Explicit path set by user
    if (!sShaderPath.empty()) {
        std::ifstream f(sShaderPath);
        if (f.good()) return sShaderPath;
    }

    // 2. Next to executable
    std::string exeDir = getExecutableDir();
    std::string path = exeDir + "/msdf_gen.wgsl";
    {
        std::ifstream f(path);
        if (f.good()) return path;
    }

    // 3. In shaders/ subdir next to executable
    path = exeDir + "/shaders/msdf_gen.wgsl";
    {
        std::ifstream f(path);
        if (f.good()) return path;
    }

    // 4. Legacy relative path
    return "shaders/msdf_gen.wgsl";
}

//------------------------------------------------------------------------------
// FreeType Glyph Serializer
//------------------------------------------------------------------------------

namespace serializer {

enum Color : uint8_t {
    BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3,
    BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7
};

struct Vec2 { float x, y; };

inline Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }
inline Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
inline Vec2 operator*(Vec2 v, float s) { return {v.x * s, v.y * s}; }
inline float dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
inline float cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
inline float length(Vec2 v) { return std::sqrt(dot(v, v)); }
inline Vec2 normalize(Vec2 v) { float l = length(v); return l > 0 ? Vec2{v.x/l, v.y/l} : Vec2{0,0}; }

// Glyph data structures
struct GlyphCtx {
    std::vector<uint32_t> metadata;  // [ncontours, (winding, nsegments, (color, npoints)*)*]
    std::vector<float> points;       // [x, y, x, y, ...]
    int currentContourIndex = -1;
    Vec2 lastPoint = {0, 0};
};

// Helper to add closing segment for a contour
static void closeContour(GlyphCtx* ctx) {
    if (ctx->currentContourIndex < 0) return;

    // Find start of current contour
    size_t contourStart = 0;
    int metaIdx = 1;
    for (int i = 0; i < ctx->currentContourIndex; i++) {
        int nseg = ctx->metadata[metaIdx + 1];
        metaIdx += 2;
        for (int j = 0; j < nseg; j++) {
            int np = ctx->metadata[metaIdx + 1];
            contourStart += (np - 1) * 2;
            metaIdx += 2;
        }
        contourStart += 2;
    }

    // Get first point of this contour
    float startX = ctx->points[contourStart];
    float startY = ctx->points[contourStart + 1];

    // Only add closing segment if we're not already at the start
    if (std::abs(ctx->lastPoint.x - startX) > 1e-6f ||
        std::abs(ctx->lastPoint.y - startY) > 1e-6f) {
        // Add closing line segment
        int segMetaIdx = 1;
        for (int i = 0; i <= ctx->currentContourIndex; i++) {
            if (i < ctx->currentContourIndex) {
                int nseg = ctx->metadata[segMetaIdx + 1];
                segMetaIdx += 2 + nseg * 2;
            } else {
                ctx->metadata[segMetaIdx + 1]++;  // Increment segment count
            }
        }
        ctx->metadata.push_back(WHITE);  // Color
        ctx->metadata.push_back(2);      // 2 points for line
        ctx->points.push_back(startX);
        ctx->points.push_back(startY);
        ctx->lastPoint = {startX, startY};
    }
}

// Callbacks for FT_Outline_Decompose
static int moveTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphCtx*>(user);

    // Close previous contour with a segment
    closeContour(ctx);

    ctx->currentContourIndex++;
    ctx->metadata[0]++;  // Increment contour count
    ctx->metadata.push_back(1);  // Winding placeholder
    ctx->metadata.push_back(0);  // Segment count

    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;
    ctx->points.push_back(x);
    ctx->points.push_back(y);
    ctx->lastPoint = {x, y};

    return 0;
}

static int lineTo(const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphCtx*>(user);

    // Find current contour's segment count position
    int metaIdx = 1;
    for (int i = 0; i < ctx->currentContourIndex; i++) {
        int nseg = ctx->metadata[metaIdx + 1];
        metaIdx += 2 + nseg * 2;
    }
    ctx->metadata[metaIdx + 1]++;  // Increment segment count

    ctx->metadata.push_back(WHITE);  // Color (will be recomputed)
    ctx->metadata.push_back(2);      // 2 points for line

    float x = to->x / FT_SCALE;
    float y = to->y / FT_SCALE;
    ctx->points.push_back(x);
    ctx->points.push_back(y);
    ctx->lastPoint = {x, y};

    return 0;
}

static int conicTo(const FT_Vector* ctrl, const FT_Vector* to, void* user) {
    auto* ctx = static_cast<GlyphCtx*>(user);

    int metaIdx = 1;
    for (int i = 0; i < ctx->currentContourIndex; i++) {
        int nseg = ctx->metadata[metaIdx + 1];
        metaIdx += 2 + nseg * 2;
    }
    ctx->metadata[metaIdx + 1]++;

    ctx->metadata.push_back(WHITE);
    ctx->metadata.push_back(3);  // 3 points for quadratic

    ctx->points.push_back(ctrl->x / FT_SCALE);
    ctx->points.push_back(ctrl->y / FT_SCALE);
    ctx->points.push_back(to->x / FT_SCALE);
    ctx->points.push_back(to->y / FT_SCALE);
    ctx->lastPoint = {to->x / FT_SCALE, to->y / FT_SCALE};

    return 0;
}

static int cubicTo(const FT_Vector* ctrl1, const FT_Vector* ctrl2, const FT_Vector* to, void* user) {
    // Approximate cubic with quadratics (simple midpoint approximation)
    // For now, we just skip cubics - they need proper subdivision
    fprintf(stderr, "msdf: Cubic curves not fully supported\n");
    return conicTo(ctrl1, to, user);
}

// Compute winding for a contour
static float computeWinding(const std::vector<float>& points, size_t start, size_t count) {
    float total = 0;
    for (size_t i = 0; i < count; i += 2) {
        size_t j = (i + 2) % count;
        float x0 = points[start + i];
        float y0 = points[start + i + 1];
        float x1 = points[start + j];
        float y1 = points[start + j + 1];
        total += (x1 - x0) * (y1 + y0);
    }
    return total;
}

// Assign edge colors - all WHITE (pure SDF mode)
// MSDF edge coloring would improve corner sharpness but requires
// pseudo-distance correction to avoid artifacts
static void assignColors(GlyphCtx& ctx) {
    int metaIdx = 1;
    for (uint32_t i = 0; i < ctx.metadata[0]; i++) {
        metaIdx++;  // Skip winding
        int nsegments = ctx.metadata[metaIdx++];

        for (int j = 0; j < nsegments; j++) {
            ctx.metadata[metaIdx] = WHITE;
            metaIdx += 2;
        }
    }
}

// Compute windings for all contours
static void computeWindings(GlyphCtx& ctx) {
    size_t pointIdx = 0;
    int metaIdx = 1;

    for (uint32_t i = 0; i < ctx.metadata[0]; i++) {
        size_t contourStart = pointIdx;
        int windingIdx = metaIdx++;
        int nsegments = ctx.metadata[metaIdx++];

        size_t contourPoints = 0;
        for (int j = 0; j < nsegments; j++) {
            metaIdx++;  // Skip color
            int npoints = ctx.metadata[metaIdx++];
            contourPoints += (npoints - 1);
        }
        contourPoints++;  // Closing point

        float winding = computeWinding(ctx.points, contourStart * 2, contourPoints * 2);
        ctx.metadata[windingIdx] = winding > 0 ? 2 : 0;  // 2 = CCW, 0 = CW

        pointIdx += contourPoints;
    }
}

bool serializeGlyph(FT_Face face, uint32_t codepoint,
                    std::vector<uint32_t>& metadata, std::vector<float>& points) {
    if (FT_Load_Char(face, codepoint, FT_LOAD_NO_SCALE)) {
        return false;
    }

    GlyphCtx ctx;
    ctx.metadata.push_back(0);  // Contour count

    FT_Outline_Funcs funcs = {};
    funcs.move_to = moveTo;
    funcs.line_to = lineTo;
    funcs.conic_to = conicTo;
    funcs.cubic_to = cubicTo;

    if (FT_Outline_Decompose(&face->glyph->outline, &funcs, &ctx)) {
        return false;
    }

    // Close last contour with a proper segment
    closeContour(&ctx);

    if (ctx.metadata[0] > 0) {
        computeWindings(ctx);
        assignColors(ctx);
    }

    metadata = std::move(ctx.metadata);
    points = std::move(ctx.points);
    return true;
}

// Get glyph bounds from outline
struct Bounds { float minX, minY, maxX, maxY; bool empty; };

Bounds getGlyphBounds(FT_Face face, uint32_t codepoint) {
    Bounds b = {0, 0, 0, 0, true};

    if (FT_Load_Char(face, codepoint, FT_LOAD_NO_SCALE)) return b;

    FT_BBox bbox;
    FT_Outline_Get_CBox(&face->glyph->outline, &bbox);

    if (bbox.xMin == bbox.xMax || bbox.yMin == bbox.yMax) return b;

    b.minX = bbox.xMin / FT_SCALE;
    b.minY = bbox.yMin / FT_SCALE;
    b.maxX = bbox.xMax / FT_SCALE;
    b.maxY = bbox.yMax / FT_SCALE;
    b.empty = false;
    return b;
}

} // namespace serializer

//------------------------------------------------------------------------------
// Atlas Implementation
//------------------------------------------------------------------------------

Atlas::Atlas(WGPUDevice device, int width)
    : _device(device), _width(width), _height(0) {}

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
        _texture = other._texture;
        _textureView = other._textureView;
        _width = other._width;
        _height = other._height;
        _rowHeight = other._rowHeight;
        _cursorX = other._cursorX;
        _cursorY = other._cursorY;
        _glyphCount = other._glyphCount;

        other._texture = nullptr;
        other._textureView = nullptr;
    }
    return *this;
}

void Atlas::cleanup() {
    if (_textureView) {
        wgpuTextureViewRelease(_textureView);
        _textureView = nullptr;
    }
    if (_texture) {
        wgpuTextureDestroy(_texture);
        wgpuTextureRelease(_texture);
        _texture = nullptr;
    }
}

bool Atlas::allocate(int w, int h, int& outX, int& outY) {
    // Simple row-based packing
    if (_cursorX + w + 1 > _width) {
        _cursorX = 1;
        _cursorY += _rowHeight + 1;
        _rowHeight = 0;
    }

    if (_cursorY + h + 1 > _height) {
        int newHeight = std::max(_height * 2, _cursorY + h + 64);
        if (!resize(newHeight)) return false;
    }

    outX = _cursorX;
    outY = _cursorY;
    _cursorX += w + 1;
    _rowHeight = std::max(_rowHeight, h);
    _glyphCount++;

    return true;
}

bool Atlas::resize(int newHeight) {
    if (newHeight <= _height) return true;

    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("MSDF Atlas");
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {static_cast<uint32_t>(_width), static_cast<uint32_t>(newHeight), 1};
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.format = WGPUTextureFormat_RGBA32Float;
    texDesc.usage = WGPUTextureUsage_StorageBinding | WGPUTextureUsage_TextureBinding |
                    WGPUTextureUsage_CopySrc | WGPUTextureUsage_CopyDst;

    WGPUTexture newTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!newTexture) return false;

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA32Float;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    WGPUTextureView newView = wgpuTextureCreateView(newTexture, &viewDesc);

    // Copy old data if exists
    if (_texture && _height > 0) {
        WGPUCommandEncoderDescriptor encDesc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

        WGPUTexelCopyTextureInfo src = {};
        src.texture = _texture;

        WGPUTexelCopyTextureInfo dst = {};
        dst.texture = newTexture;

        WGPUExtent3D size = {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height), 1};
        wgpuCommandEncoderCopyTextureToTexture(encoder, &src, &dst, &size);

        WGPUCommandBufferDescriptor cmdDesc = {};
        WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdDesc);
        WGPUQueue queue = wgpuDeviceGetQueue(_device);
        wgpuQueueSubmit(queue, 1, &cmdBuf);

        wgpuCommandBufferRelease(cmdBuf);
        wgpuCommandEncoderRelease(encoder);
    }

    cleanup();
    _texture = newTexture;
    _textureView = newView;
    _height = newHeight;

    return true;
}

//------------------------------------------------------------------------------
// Font Implementation
//------------------------------------------------------------------------------

Font::Font(Context& ctx, FT_Face face, const FontConfig& config)
    : _context(ctx), _face(face), _scale(config.scale), _range(config.range) {
    _atlas = std::make_unique<Atlas>(ctx.getDevice(), 8192);
}

Font::~Font() {
    cleanup();
}

Font::Font(Font&& other) noexcept : _context(other._context) {
    *this = std::move(other);
}

Font& Font::operator=(Font&& other) noexcept {
    if (this != &other) {
        cleanup();
        _face = other._face;
        _scale = other._scale;
        _range = other._range;
        _atlas = std::move(other._atlas);
        _glyphs = std::move(other._glyphs);
        _metadataBuffer = other._metadataBuffer;
        _pointsBuffer = other._pointsBuffer;

        other._face = nullptr;
        other._metadataBuffer = nullptr;
        other._pointsBuffer = nullptr;
    }
    return *this;
}

void Font::cleanup() {
    if (_metadataBuffer) {
        wgpuBufferDestroy(_metadataBuffer);
        wgpuBufferRelease(_metadataBuffer);
        _metadataBuffer = nullptr;
    }
    if (_pointsBuffer) {
        wgpuBufferDestroy(_pointsBuffer);
        wgpuBufferRelease(_pointsBuffer);
        _pointsBuffer = nullptr;
    }
    if (_face) {
        FT_Done_Face(_face);
        _face = nullptr;
    }
}

//------------------------------------------------------------------------------
// Context Implementation
//------------------------------------------------------------------------------

Context::Context(WGPUDevice device, WGPUInstance instance) : _device(device), _instance(instance) {
    if (FT_Init_FreeType(&_ftLibrary)) {
        throw std::runtime_error("Failed to initialize FreeType");
    }

    if (!initCompute()) {
        throw std::runtime_error("Failed to initialize compute pipeline");
    }
}

Context::~Context() {
    cleanup();
}

void Context::cleanup() {
    if (_computePipeline) {
        wgpuComputePipelineRelease(_computePipeline);
        _computePipeline = nullptr;
    }
    if (_bindGroupLayout) {
        wgpuBindGroupLayoutRelease(_bindGroupLayout);
        _bindGroupLayout = nullptr;
    }
    if (_computeShader) {
        wgpuShaderModuleRelease(_computeShader);
        _computeShader = nullptr;
    }
    if (_ftLibrary) {
        FT_Done_FreeType(_ftLibrary);
        _ftLibrary = nullptr;
    }
}

bool Context::initCompute() {
    // Load shader from file
    std::string shaderPath = findShaderPath();
    std::ifstream file(shaderPath);
    if (!file.is_open()) {
        fprintf(stderr, "[MSDF] Failed to open shader: %s\n", shaderPath.c_str());
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSource = buffer.str();

    fprintf(stderr, "[MSDF] Loaded shader from %s (%zu bytes)\n",
            shaderPath.c_str(), shaderSource.size());

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, shaderSource);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.label = WGPU_STR("MSDF Compute Shader");
    shaderDesc.nextInChain = &wgslDesc.chain;

    _computeShader = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!_computeShader) {
        fprintf(stderr, "[MSDF] Failed to create shader module\n");
        return false;
    }

    // Create bind group layout
    WGPUBindGroupLayoutEntry entries[4] = {};

    // Uniform buffer
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Compute;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // Metadata storage buffer
    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Compute;
    entries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // Points storage buffer
    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Compute;
    entries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // Output texture (storage)
    entries[3].binding = 3;
    entries[3].visibility = WGPUShaderStage_Compute;
    entries[3].storageTexture.access = WGPUStorageTextureAccess_WriteOnly;
    entries[3].storageTexture.format = WGPUTextureFormat_RGBA32Float;
    entries[3].storageTexture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 4;
    layoutDesc.entries = entries;

    _bindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &layoutDesc);
    if (!_bindGroupLayout) {
        fprintf(stderr, "[MSDF] Failed to create bind group layout\n");
        return false;
    }

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &_bindGroupLayout;

    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &pipelineLayoutDesc);

    // Create compute pipeline
    WGPUComputePipelineDescriptor pipelineDesc = {};
    pipelineDesc.label = WGPU_STR("MSDF Compute Pipeline");
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.compute.module = _computeShader;
    pipelineDesc.compute.entryPoint = WGPU_STR("main");

    _computePipeline = wgpuDeviceCreateComputePipeline(_device, &pipelineDesc);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!_computePipeline) {
        fprintf(stderr, "[MSDF] Failed to create compute pipeline\n");
        return false;
    }

    fprintf(stderr, "[MSDF] Compute pipeline initialized\n");
    return true;
}

std::unique_ptr<Font> Context::loadFont(const std::string& path, const FontConfig& config) {
    FT_Face face;
    if (FT_New_Face(_ftLibrary, path.c_str(), 0, &face)) {
        fprintf(stderr, "[MSDF] Failed to load font: %s\n", path.c_str());
        return nullptr;
    }

    return std::unique_ptr<Font>(new Font(*this, face, config));
}

int Context::generateGlyphs(Font& font, const std::vector<uint32_t>& codepoints) {
    fprintf(stderr, "[MSDF] generateGlyphs called with %zu codepoints\n", codepoints.size());
    if (codepoints.empty()) return 0;

    WGPUQueue queue = wgpuDeviceGetQueue(_device);
    int generated = 0;

    // Pre-allocate and serialize all glyphs
    std::vector<std::vector<uint32_t>> allMetadata(codepoints.size());
    std::vector<std::vector<float>> allPoints(codepoints.size());
    std::vector<GlyphData> glyphData(codepoints.size());
    std::vector<serializer::Bounds> allBounds(codepoints.size());

    size_t totalMetaSize = 0;
    size_t totalPointSize = 0;

    for (size_t i = 0; i < codepoints.size(); i++) {
        uint32_t cp = codepoints[i];

        if (!serializer::serializeGlyph(font.getFace(), cp, allMetadata[i], allPoints[i])) {
            continue;
        }

        auto bounds = serializer::getGlyphBounds(font.getFace(), cp);
        if (bounds.empty) continue;

        allBounds[i] = bounds;  // Store bounds for later use
        float boundsW = bounds.maxX - bounds.minX;
        float boundsH = bounds.maxY - bounds.minY;
        int padding = static_cast<int>(std::ceil(font.getRange()));

        int atlasW = static_cast<int>(std::ceil(boundsW * font.getScale())) + padding * 2;
        int atlasH = static_cast<int>(std::ceil(boundsH * font.getScale())) + padding * 2;

        int atlasX, atlasY;
        if (!font.getAtlas()->allocate(atlasW, atlasH, atlasX, atlasY)) {
            fprintf(stderr, "[MSDF] Failed to allocate atlas space for U+%04X\n", cp);
            continue;
        }

        glyphData[i].codepoint = cp;
        glyphData[i].atlasX = atlasX;
        glyphData[i].atlasY = atlasY;
        glyphData[i].atlasW = atlasW;
        glyphData[i].atlasH = atlasH;
        glyphData[i].bearingX = static_cast<int16_t>(font.getFace()->glyph->metrics.horiBearingX);
        glyphData[i].bearingY = static_cast<int16_t>(font.getFace()->glyph->metrics.horiBearingY);
        glyphData[i].glyphWidth = static_cast<uint16_t>(boundsW * FT_SCALE);
        glyphData[i].glyphHeight = static_cast<uint16_t>(boundsH * FT_SCALE);
        glyphData[i].advance = static_cast<float>(font.getFace()->glyph->metrics.horiAdvance);

        totalMetaSize += allMetadata[i].size();
        totalPointSize += allPoints[i].size();
        generated++;
    }

    if (generated == 0) return 0;

    fprintf(stderr, "[MSDF] Serialized %d glyphs, creating buffers...\n", generated);

    // Create combined buffers
    std::vector<uint32_t> combinedMeta;
    std::vector<float> combinedPoints;
    combinedMeta.reserve(totalMetaSize);
    combinedPoints.reserve(totalPointSize);

    std::vector<size_t> metaOffsets(codepoints.size());
    std::vector<size_t> pointOffsets(codepoints.size());

    for (size_t i = 0; i < codepoints.size(); i++) {
        if (allMetadata[i].empty()) continue;

        metaOffsets[i] = combinedMeta.size();
        pointOffsets[i] = combinedPoints.size() / 2;  // Point index, not float index

        combinedMeta.insert(combinedMeta.end(), allMetadata[i].begin(), allMetadata[i].end());
        combinedPoints.insert(combinedPoints.end(), allPoints[i].begin(), allPoints[i].end());
    }

    // Create GPU buffers
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;

    bufDesc.label = WGPU_STR("MSDF Metadata Buffer");
    bufDesc.size = combinedMeta.size() * sizeof(uint32_t);
    WGPUBuffer metaBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    wgpuQueueWriteBuffer(queue, metaBuffer, 0, combinedMeta.data(), bufDesc.size);

    bufDesc.label = WGPU_STR("MSDF Points Buffer");
    bufDesc.size = combinedPoints.size() * sizeof(float);
    WGPUBuffer pointsBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    wgpuQueueWriteBuffer(queue, pointsBuffer, 0, combinedPoints.data(), bufDesc.size);

    fprintf(stderr, "[MSDF] Buffers created, dispatching compute...\n");

    // Create command encoder
    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

    // Keep track of uniform buffers and bind groups - they must stay alive until submit completes
    std::vector<WGPUBuffer> uniformBuffers;
    std::vector<WGPUBindGroup> bindGroups;

    // Dispatch compute for each glyph
    for (size_t i = 0; i < codepoints.size(); i++) {
        if (allMetadata[i].empty()) continue;

        auto& glyph = glyphData[i];

        // Create uniform buffer for this glyph
        struct GlyphUniforms {
            uint32_t atlas_offset[2];
            uint32_t glyph_size[2];
            float translate[2];
            float scale;
            float range;
            uint32_t meta_offset;
            uint32_t point_offset;
            float glyph_height;
            uint32_t _padding;
        } uniforms;

        auto& bounds = allBounds[i];
        uniforms.atlas_offset[0] = glyph.atlasX;
        uniforms.atlas_offset[1] = glyph.atlasY;
        uniforms.glyph_size[0] = glyph.atlasW;
        uniforms.glyph_size[1] = glyph.atlasH;
        // Translate from pixel coords to glyph outline coords (in glyph space units)
        // X: pixel_pos.x/scale - translate.x = glyph_x
        //    At pixel_x = padding, we want glyph_x = bounds.minX
        //    So translate.x = padding/scale - bounds.minX
        // Y: The shader flips Y: p.y = glyph_height/scale - (pixel_pos.y/scale - translate.y)
        //    Working out the math: translate.y = bounds.minY - padding/scale
        float paddingInGlyphSpace = font.getRange() / font.getScale();
        uniforms.translate[0] = paddingInGlyphSpace - bounds.minX;
        uniforms.translate[1] = bounds.minY - paddingInGlyphSpace;
        uniforms.scale = font.getScale();
        uniforms.range = font.getRange();
        uniforms.meta_offset = metaOffsets[i];
        uniforms.point_offset = pointOffsets[i];
        uniforms.glyph_height = static_cast<float>(glyph.atlasH);
        uniforms._padding = 0;

        WGPUBufferDescriptor uniformDesc = {};
        uniformDesc.label = WGPU_STR("MSDF Glyph Uniforms");
        uniformDesc.size = sizeof(uniforms);
        uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        WGPUBuffer uniformBuffer = wgpuDeviceCreateBuffer(_device, &uniformDesc);
        wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &uniforms, sizeof(uniforms));

        // Create bind group
        WGPUBindGroupEntry entries[4] = {};
        entries[0].binding = 0;
        entries[0].buffer = uniformBuffer;
        entries[0].size = sizeof(uniforms);

        entries[1].binding = 1;
        entries[1].buffer = metaBuffer;
        entries[1].size = combinedMeta.size() * sizeof(uint32_t);

        entries[2].binding = 2;
        entries[2].buffer = pointsBuffer;
        entries[2].size = combinedPoints.size() * sizeof(float);

        entries[3].binding = 3;
        entries[3].textureView = font.getAtlas()->getTextureView();

        WGPUBindGroupDescriptor bgDesc = {};
        bgDesc.layout = _bindGroupLayout;
        bgDesc.entryCount = 4;
        bgDesc.entries = entries;
        WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(_device, &bgDesc);

        // Store for cleanup after submit
        uniformBuffers.push_back(uniformBuffer);
        bindGroups.push_back(bindGroup);

        // Dispatch compute
        WGPUComputePassDescriptor passDesc = {};
        WGPUComputePassEncoder pass = wgpuCommandEncoderBeginComputePass(encoder, &passDesc);
        wgpuComputePassEncoderSetPipeline(pass, _computePipeline);
        wgpuComputePassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);

        uint32_t workgroupsX = (glyph.atlasW + 7) / 8;
        uint32_t workgroupsY = (glyph.atlasH + 7) / 8;
        wgpuComputePassEncoderDispatchWorkgroups(pass, workgroupsX, workgroupsY, 1);
        wgpuComputePassEncoderEnd(pass);

        wgpuComputePassEncoderRelease(pass);
    }

    fprintf(stderr, "[MSDF] Submitting GPU work...\n");

    // Submit and wait
    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuf);

    fprintf(stderr, "[MSDF] GPU work submitted, waiting for completion...\n");

    // Wait for compute work to complete
    std::atomic<bool> computeDone{false};
    WGPUQueueWorkDoneCallbackInfo computeCallback = {};
    computeCallback.mode = WGPUCallbackMode_AllowSpontaneous;
    computeCallback.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* u1, void*) {
        *static_cast<std::atomic<bool>*>(u1) = true;
    };
    computeCallback.userdata1 = &computeDone;
    wgpuQueueOnSubmittedWorkDone(queue, computeCallback);

    while (!computeDone) {
        if (_instance) {
            wgpuInstanceProcessEvents(_instance);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    fprintf(stderr, "[MSDF] Compute work complete\n");

    // Cleanup - now safe to destroy resources used by GPU
    wgpuCommandBufferRelease(cmdBuf);
    wgpuCommandEncoderRelease(encoder);

    // Clean up per-glyph resources
    for (auto& bg : bindGroups) {
        wgpuBindGroupRelease(bg);
    }
    for (auto& ub : uniformBuffers) {
        wgpuBufferDestroy(ub);
        wgpuBufferRelease(ub);
    }

    wgpuBufferDestroy(metaBuffer);
    wgpuBufferRelease(metaBuffer);
    wgpuBufferDestroy(pointsBuffer);
    wgpuBufferRelease(pointsBuffer);

    // Store glyph data
    for (const auto& glyph : glyphData) {
        if (glyph.atlasW > 0) {
            font._glyphs.push_back(glyph);
        }
    }

    fprintf(stderr, "[MSDF] generateGlyphs returning %d\n", generated);
    return generated;
}

std::vector<uint8_t> Context::readAtlasToRGBA8(const Atlas& atlas) {
    fprintf(stderr, "[MSDF] readAtlasToRGBA8 called, atlas %dx%d\n", atlas.getWidth(), atlas.getHeight());
    if (!atlas.getTexture() || atlas.getWidth() == 0 || atlas.getHeight() == 0) {
        fprintf(stderr, "[MSDF] No atlas texture!\n");
        return {};
    }

    size_t bytesPerPixel = 16;  // RGBA32Float
    size_t bytesPerRow = atlas.getWidth() * bytesPerPixel;
    size_t alignedBytesPerRow = ((bytesPerRow + 255) / 256) * 256;
    size_t bufferSize = alignedBytesPerRow * atlas.getHeight();

    // Create staging buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("MSDF Readback Buffer");
    bufDesc.size = bufferSize;
    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;

    WGPUBuffer stagingBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);

    // Copy texture to buffer
    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

    WGPUTexelCopyTextureInfo src = {};
    src.texture = atlas.getTexture();

    WGPUTexelCopyBufferInfo dst = {};
    dst.buffer = stagingBuffer;
    dst.layout.bytesPerRow = alignedBytesPerRow;
    dst.layout.rowsPerImage = atlas.getHeight();

    WGPUExtent3D size = {static_cast<uint32_t>(atlas.getWidth()),
                         static_cast<uint32_t>(atlas.getHeight()), 1};
    wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &size);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    WGPUQueue queue = wgpuDeviceGetQueue(_device);
    wgpuQueueSubmit(queue, 1, &cmdBuf);

    wgpuCommandBufferRelease(cmdBuf);
    wgpuCommandEncoderRelease(encoder);

    // Wait for GPU - use AllowSpontaneous which works with Dawn
    std::atomic<bool> done{false};
    WGPUQueueWorkDoneCallbackInfo callback = {};
    callback.mode = WGPUCallbackMode_AllowSpontaneous;
    callback.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* u1, void*) {
        *static_cast<std::atomic<bool>*>(u1) = true;
    };
    callback.userdata1 = &done;

    fprintf(stderr, "[MSDF] Waiting for queue work done...\n");
    wgpuQueueOnSubmittedWorkDone(queue, callback);

    // Process events until callback fires
    while (!done) {
        if (_instance) {
            wgpuInstanceProcessEvents(_instance);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    fprintf(stderr, "[MSDF] Queue work done\n");

    // Map buffer
    std::atomic<bool> mapDone{false};
    std::atomic<WGPUMapAsyncStatus> mapStatus{WGPUMapAsyncStatus_Error};

    WGPUBufferMapCallbackInfo mapCallback = {};
    mapCallback.mode = WGPUCallbackMode_AllowSpontaneous;
    mapCallback.callback = [](WGPUMapAsyncStatus status, WGPUStringView, void* u1, void* u2) {
        *static_cast<std::atomic<WGPUMapAsyncStatus>*>(u2) = status;
        *static_cast<std::atomic<bool>*>(u1) = true;
    };
    mapCallback.userdata1 = &mapDone;
    mapCallback.userdata2 = &mapStatus;

    fprintf(stderr, "[MSDF] Mapping buffer...\n");
    wgpuBufferMapAsync(stagingBuffer, WGPUMapMode_Read, 0, bufferSize, mapCallback);

    while (!mapDone) {
        if (_instance) {
            wgpuInstanceProcessEvents(_instance);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    fprintf(stderr, "[MSDF] Buffer mapped\n");

    if (mapStatus != WGPUMapAsyncStatus_Success) {
        wgpuBufferRelease(stagingBuffer);
        return {};
    }

    // Convert RGBA32Float to RGBA8
    const float* src_data = static_cast<const float*>(
        wgpuBufferGetConstMappedRange(stagingBuffer, 0, bufferSize));

    std::vector<uint8_t> result(atlas.getWidth() * atlas.getHeight() * 4);

    for (int y = 0; y < atlas.getHeight(); y++) {
        const float* srcRow = src_data + (y * alignedBytesPerRow / sizeof(float));
        uint8_t* dstRow = result.data() + (y * atlas.getWidth() * 4);

        for (int x = 0; x < atlas.getWidth(); x++) {
            const float* px = srcRow + x * 4;
            uint8_t* dst = dstRow + x * 4;

            dst[0] = static_cast<uint8_t>(std::clamp(px[0] * 255.0f, 0.0f, 255.0f));
            dst[1] = static_cast<uint8_t>(std::clamp(px[1] * 255.0f, 0.0f, 255.0f));
            dst[2] = static_cast<uint8_t>(std::clamp(px[2] * 255.0f, 0.0f, 255.0f));
            dst[3] = static_cast<uint8_t>(std::clamp(px[3] * 255.0f, 0.0f, 255.0f));
        }
    }

    wgpuBufferUnmap(stagingBuffer);
    wgpuBufferDestroy(stagingBuffer);
    wgpuBufferRelease(stagingBuffer);

    return result;
}

//------------------------------------------------------------------------------
// Default Charset
//------------------------------------------------------------------------------

std::vector<uint32_t> getDefaultCharset(bool) {
    // This function is deprecated - use Font::getAllCodepoints() instead
    std::vector<uint32_t> charset;
    // Basic Latin only as fallback
    for (uint32_t c = 0x20; c <= 0x7E; c++) charset.push_back(c);
    return charset;
}

std::vector<uint32_t> Font::getAllCodepoints() const {
    std::vector<uint32_t> codepoints;

    FT_ULong charcode;
    FT_UInt glyphIndex;

    // Iterate through all codepoints in the font
    charcode = FT_Get_First_Char(_face, &glyphIndex);
    while (glyphIndex != 0) {
        codepoints.push_back(static_cast<uint32_t>(charcode));
        charcode = FT_Get_Next_Char(_face, charcode, &glyphIndex);
    }

    return codepoints;
}

} // namespace msdf
