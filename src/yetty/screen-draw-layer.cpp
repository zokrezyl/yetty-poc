#include "screen-draw-layer.h"
#include <yetty/yetty-context.h>
#include <yetty/wgpu-compat.h>
#include <yetty/ydraw-builder.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include "ydraw/ydraw-buffer.h"
#include "ydraw/yaml2ydraw.h"
#include <ytrace/ytrace.hpp>
#include <lz4frame.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif
#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

namespace yetty {

// GPU uniform buffer layout — must match OverlayUniforms in gpu-screen.wgsl
struct ScreenDrawUniforms {
    float sceneMinX;
    float sceneMinY;
    float sceneMaxX;
    float sceneMaxY;
    uint32_t gridWidth;
    uint32_t gridHeight;
    float cellSizeX;
    float cellSizeY;
    uint32_t primCount;
    uint32_t glyphCount;
    float pixelRange;
    uint32_t hasOverlay;  // 1 if overlay present, 0 otherwise
    float offsetX;        // Pixel offset for inline positioning
    float offsetY;
    float _pad0;          // Padding to 16-byte alignment
    float _pad1;
};

//=============================================================================
// ScreenDrawLayerImpl
//=============================================================================
class ScreenDrawLayerImpl : public ScreenDrawLayer {
public:
    explicit ScreenDrawLayerImpl(const YettyContext& ctx);
    ~ScreenDrawLayerImpl() override;

    Result<void> init() noexcept;

    // ScreenDrawLayer interface
    void updateDisplaySize(uint32_t width, uint32_t height) override;
    void setCellSize(float cellWidth, float cellHeight) override;
    void clear() override;
    Result<void> update(const std::string& args, const std::string& payload) override;
    void setOriginOffset(float x, float y) override;
    void scroll(int32_t num_lines) override;
    void getContentBounds(float& minX, float& minY, float& maxX, float& maxY) const override;
    uint32_t getContentHeightCells() const override;
    Result<void> render(WGPURenderPassEncoder pass) override;
    bool hasContent() const override;

    // Buffer accessors for integrated rendering
    WGPUBuffer getGridBuffer() const override { return _gridBuffer; }
    WGPUBuffer getGlyphBuffer() const override { return _glyphBuffer; }
    WGPUBuffer getPrimBuffer() const override { return _primBuffer; }
    WGPUBuffer getUniformBuffer() const override { return _uniformBuffer; }
    uint32_t getGridBufferSize() const override { return _gridBufferSize; }
    uint32_t getGlyphBufferSize() const override { return _glyphBufferSize; }
    uint32_t getPrimBufferSize() const override { return _primBufferSize; }
    void prepareForRender() override;

private:
    Result<void> createPipeline();
    Result<void> buildShaderSource(std::string& source);
    void rebuildBuffers();
    Result<void> parseBinary(const std::string& payload);
    Result<void> decompressLZ4(const uint8_t* src, size_t srcSize, std::vector<uint8_t>& dst);

    // Per-row grid management for scroll support
    void extractRowsFromGrid();   // Extract builder's grid into per-row format
    void buildGridFromRows();     // Build flat grid from per-row data
    void shiftRows(int32_t lines); // Shift rows for scrolling

    const YettyContext& _ctx;

    // Display
    uint32_t _displayWidth = 0;
    uint32_t _displayHeight = 0;
    float _cellWidth = 10.0f;
    float _cellHeight = 20.0f;

    // Origin offset for inline positioning
    float _originOffsetX = 0.0f;
    float _originOffsetY = 0.0f;

    // YDraw
    YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    bool _dirty = false;

    // Per-row grid storage for scroll support
    // _rowEntries[row][col] = vector of primitive/glyph indices for that cell
    std::vector<std::vector<std::vector<uint32_t>>> _rowEntries;
    std::vector<uint32_t> _scrollableGrid;  // Flat grid built from _rowEntries
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    bool _rowsInitialized = false;  // True after extractRowsFromGrid() called

    // GPU resources
    WGPURenderPipeline _pipeline = nullptr;
    WGPUPipelineLayout _pipelineLayout = nullptr;
    WGPUBindGroupLayout _bindGroupLayout = nullptr;
    WGPUBindGroup _bindGroup = nullptr;
    WGPUBuffer _uniformBuffer = nullptr;
    WGPUBuffer _primBuffer = nullptr;
    WGPUBuffer _gridBuffer = nullptr;
    WGPUBuffer _glyphBuffer = nullptr;
    uint32_t _primBufferSize = 0;
    uint32_t _gridBufferSize = 0;
    uint32_t _glyphBufferSize = 0;
    bool _bindGroupDirty = true;
};

ScreenDrawLayerImpl::ScreenDrawLayerImpl(const YettyContext& ctx)
    : _ctx(ctx) {}

ScreenDrawLayerImpl::~ScreenDrawLayerImpl() {
    if (_bindGroup) wgpuBindGroupRelease(_bindGroup);
    if (_uniformBuffer) wgpuBufferRelease(_uniformBuffer);
    if (_primBuffer) wgpuBufferRelease(_primBuffer);
    if (_gridBuffer) wgpuBufferRelease(_gridBuffer);
    if (_glyphBuffer) wgpuBufferRelease(_glyphBuffer);
    if (_pipeline) wgpuRenderPipelineRelease(_pipeline);
    if (_pipelineLayout) wgpuPipelineLayoutRelease(_pipelineLayout);
    if (_bindGroupLayout) wgpuBindGroupLayoutRelease(_bindGroupLayout);
}

Result<void> ScreenDrawLayerImpl::init() noexcept {
    if (!_ctx.gpu.device || !_ctx.gpu.queue)
        return Err<void>("ScreenDrawLayer: GPUContext not initialized");

    // Create YDrawBuffer and builder (no CardManager - standalone)
    _buffer = *YDrawBuffer::create();
    auto builderRes = YDrawBuilder::create(_ctx.fontManager, _ctx.gpuAllocator, _buffer);
    if (!builderRes) return Err<void>("ScreenDrawLayer: failed to create builder", builderRes);
    _builder = *builderRes;

    // Create uniform buffer
    WGPUBufferDescriptor ubDesc{};
    ubDesc.label = WGPU_STR("screen-draw-uniforms");
    ubDesc.size = sizeof(ScreenDrawUniforms);
    ubDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _uniformBuffer = wgpuDeviceCreateBuffer(_ctx.gpu.device, &ubDesc);
    if (!_uniformBuffer) return Err<void>("ScreenDrawLayer: failed to create uniform buffer");

    // Create pipeline
    if (auto res = createPipeline(); !res)
        return Err<void>("ScreenDrawLayer: pipeline creation failed", res);

    yinfo("ScreenDrawLayer initialized");
    return Ok();
}

Result<void> ScreenDrawLayerImpl::buildShaderSource(std::string& source) {
    std::string shaderDir = std::string(YETTY_SHADERS_DIR);

    // Read overlay shader (reuse ygui-overlay shader)
    std::string overlayPath = shaderDir + "/ygui-overlay.wgsl";
    std::ifstream overlayFile(overlayPath);
    if (!overlayFile.is_open())
        return Err<void>("Cannot open overlay shader: " + overlayPath);
    std::stringstream overlayBuf;
    overlayBuf << overlayFile.rdbuf();
    std::string overlayCode = overlayBuf.str();

    // Read lib files we need
    auto readLib = [&](const std::string& name) -> std::string {
        std::string path = shaderDir + "/lib/" + name;
        std::ifstream f(path);
        if (!f.is_open()) {
            ywarn("ScreenDrawLayer: missing lib {}", path);
            return "";
        }
        std::stringstream buf;
        buf << f.rdbuf();
        return buf.str();
    };

    std::string sdfTypes = readLib("sdf-types.gen.wgsl");
    std::string distFunctions = readLib("distfunctions.wgsl");

    // Inject lib code into overlay shader at marker positions
    auto replace = [](std::string& str, const std::string& marker, const std::string& replacement) {
        auto pos = str.find(marker);
        if (pos != std::string::npos)
            str.replace(pos, marker.size(), replacement);
    };

    replace(overlayCode, "// INJECT_SDF_TYPES", sdfTypes);
    replace(overlayCode, "// INJECT_DISTFUNCTIONS", distFunctions);

    source = std::move(overlayCode);
    return Ok();
}

Result<void> ScreenDrawLayerImpl::createPipeline() {
    auto device = _ctx.gpu.device;

    // Build shader source
    std::string shaderSource;
    if (auto res = buildShaderSource(shaderSource); !res)
        return Err<void>("Shader source build failed", res);

    // Create shader module with error scoping
    WGPUShaderSourceWGSL wgslDesc{};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, shaderSource);

    WGPUShaderModuleDescriptor smDesc{};
    smDesc.label = WGPU_STR("screen-draw-shader");
    smDesc.nextInChain = &wgslDesc.chain;

    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);

    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &smDesc);

    {
        bool hadShaderError = false;
        std::string shaderErrorMsg;
        WGPUPopErrorScopeCallbackInfo popInfo = {};
        popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        popInfo.callback = [](WGPUPopErrorScopeStatus, WGPUErrorType type,
                              WGPUStringView message, void* ud1, void* ud2) {
            if (type != WGPUErrorType_NoError) {
                *static_cast<bool*>(ud1) = true;
                auto* msg = static_cast<std::string*>(ud2);
                if (message.data && message.length > 0)
                    *msg = std::string(message.data, message.length);
                else
                    *msg = "Unknown shader error";
            }
        };
        popInfo.userdata1 = &hadShaderError;
        popInfo.userdata2 = &shaderErrorMsg;
        wgpuDevicePopErrorScope(device, popInfo);
        WGPU_DEVICE_TICK(device);

        if (hadShaderError) {
            yerror("ScreenDrawLayer: SHADER COMPILATION ERROR: {}", shaderErrorMsg);
            if (shaderModule) wgpuShaderModuleRelease(shaderModule);
            return Err<void>("Shader compilation failed: " + shaderErrorMsg);
        }
    }

    if (!shaderModule)
        return Err<void>("Failed to create screen draw shader module");

    // Bind group layout
    WGPUBindGroupLayoutEntry entries[7] = {};

    // @binding(0): uniform buffer
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Fragment;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    entries[0].buffer.minBindingSize = sizeof(ScreenDrawUniforms);

    // @binding(1): cardStorage (prim data)
    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Fragment;
    entries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // @binding(2): gridData
    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Fragment;
    entries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // @binding(3): glyphBuffer
    entries[3].binding = 3;
    entries[3].visibility = WGPUShaderStage_Fragment;
    entries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // @binding(4): fontTexture
    entries[4].binding = 4;
    entries[4].visibility = WGPUShaderStage_Fragment;
    entries[4].texture.sampleType = WGPUTextureSampleType_Float;
    entries[4].texture.viewDimension = WGPUTextureViewDimension_2D;

    // @binding(5): fontSampler
    entries[5].binding = 5;
    entries[5].visibility = WGPUShaderStage_Fragment;
    entries[5].sampler.type = WGPUSamplerBindingType_Filtering;

    // @binding(6): glyphMetadata
    entries[6].binding = 6;
    entries[6].visibility = WGPUShaderStage_Fragment;
    entries[6].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor bglDesc{};
    bglDesc.label = WGPU_STR("screen-draw-bind-group-layout");
    bglDesc.entryCount = 7;
    bglDesc.entries = entries;
    _bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc{};
    plDesc.label = WGPU_STR("screen-draw-pipeline-layout");
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &_bindGroupLayout;
    _pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &plDesc);

    // Render pipeline with alpha blending
    WGPUBlendState blendState{};
    blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blendState.color.operation = WGPUBlendOperation_Add;
    blendState.alpha.srcFactor = WGPUBlendFactor_One;
    blendState.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blendState.alpha.operation = WGPUBlendOperation_Add;

    WGPUColorTargetState colorTarget{};
    colorTarget.format = _ctx.gpu.surfaceFormat;
    colorTarget.blend = &blendState;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragmentState{};
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = WGPU_STR("fs_main");
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    WGPURenderPipelineDescriptor rpDesc{};
    rpDesc.label = WGPU_STR("screen-draw-pipeline");
    rpDesc.layout = _pipelineLayout;
    rpDesc.vertex.module = shaderModule;
    rpDesc.vertex.entryPoint = WGPU_STR("vs_main");
    rpDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    rpDesc.primitive.frontFace = WGPUFrontFace_CCW;
    rpDesc.primitive.cullMode = WGPUCullMode_None;
    rpDesc.fragment = &fragmentState;
    rpDesc.multisample.count = 1;
    rpDesc.multisample.mask = ~0u;

    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);

    _pipeline = wgpuDeviceCreateRenderPipeline(device, &rpDesc);

    {
        bool hadPipelineError = false;
        std::string pipelineErrorMsg;
        WGPUPopErrorScopeCallbackInfo popInfo = {};
        popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        popInfo.callback = [](WGPUPopErrorScopeStatus, WGPUErrorType type,
                              WGPUStringView message, void* ud1, void* ud2) {
            if (type != WGPUErrorType_NoError) {
                *static_cast<bool*>(ud1) = true;
                auto* msg = static_cast<std::string*>(ud2);
                if (message.data && message.length > 0)
                    *msg = std::string(message.data, message.length);
                else
                    *msg = "Unknown pipeline error";
            }
        };
        popInfo.userdata1 = &hadPipelineError;
        popInfo.userdata2 = &pipelineErrorMsg;
        wgpuDevicePopErrorScope(device, popInfo);
        WGPU_DEVICE_TICK(device);

        if (hadPipelineError) {
            yerror("ScreenDrawLayer: PIPELINE CREATION ERROR: {}", pipelineErrorMsg);
        }
    }

    wgpuShaderModuleRelease(shaderModule);

    if (!_pipeline)
        return Err<void>("Failed to create screen draw render pipeline");

    yinfo("ScreenDrawLayer: pipeline created successfully");
    return Ok();
}

void ScreenDrawLayerImpl::rebuildBuffers() {
    auto device = _ctx.gpu.device;
    auto queue = _ctx.gpu.queue;

    // Get computed data from builder
    const auto& glyphs = _builder->glyphs();
    uint32_t primCount = _builder->primitiveCount();

    // Write prim data to staging
    std::vector<uint32_t> primWordOffsets;
    uint32_t primBytes = _buffer->gpuBufferSize();
    std::vector<float> primData;
    if (primBytes > 0) {
        primData.resize(primBytes / sizeof(float));
        _buffer->writeGPU(primData.data(), primBytes, primWordOffsets);
    }

    // Use scrollable grid if initialized, otherwise fall back to builder's grid
    std::vector<uint32_t> gridData;
    uint32_t gridWidth, gridHeight;
    if (_rowsInitialized && !_scrollableGrid.empty()) {
        gridData = _scrollableGrid;
        gridWidth = _gridWidth;
        gridHeight = static_cast<uint32_t>(_rowEntries.size());
    } else {
        gridData = _builder->gridStaging();
        gridWidth = _builder->gridWidth();
        gridHeight = _builder->gridHeight();
    }
    uint32_t numCells = gridWidth * gridHeight;
    if (!primWordOffsets.empty() && numCells <= gridData.size()) {
        for (uint32_t ci = 0; ci < numCells; ci++) {
            uint32_t packedOff = gridData[ci];
            if (packedOff >= gridData.size()) continue;
            uint32_t cnt = gridData[packedOff];
            for (uint32_t j = 0; j < cnt; j++) {
                uint32_t idx = packedOff + 1 + j;
                if (idx >= gridData.size()) break;
                uint32_t rawVal = gridData[idx];
                if ((rawVal & 0x80000000u) != 0) continue;
                if (rawVal < primWordOffsets.size())
                    gridData[idx] = primWordOffsets[rawVal];
            }
        }
    }

    // Upload prim data
    uint32_t primBufSize = std::max((uint32_t)(primData.size() * sizeof(float)), 4u);
    if (!_primBuffer || _primBufferSize < primBufSize) {
        if (_primBuffer) wgpuBufferRelease(_primBuffer);
        WGPUBufferDescriptor desc{};
        desc.label = WGPU_STR("screen-draw-prims");
        desc.size = primBufSize;
        desc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        _primBuffer = wgpuDeviceCreateBuffer(device, &desc);
        _primBufferSize = primBufSize;
        _bindGroupDirty = true;
    }
    if (!primData.empty())
        wgpuQueueWriteBuffer(queue, _primBuffer, 0, primData.data(), primData.size() * sizeof(float));

    // Upload grid data
    uint32_t gridBufSize = std::max((uint32_t)(gridData.size() * sizeof(uint32_t)), 4u);
    if (!_gridBuffer || _gridBufferSize < gridBufSize) {
        if (_gridBuffer) wgpuBufferRelease(_gridBuffer);
        WGPUBufferDescriptor desc{};
        desc.label = WGPU_STR("screen-draw-grid");
        desc.size = gridBufSize;
        desc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        _gridBuffer = wgpuDeviceCreateBuffer(device, &desc);
        _gridBufferSize = gridBufSize;
        _bindGroupDirty = true;
    }
    if (!gridData.empty())
        wgpuQueueWriteBuffer(queue, _gridBuffer, 0, gridData.data(), gridData.size() * sizeof(uint32_t));

    // Upload glyph data
    uint32_t glyphBufSize = std::max((uint32_t)(glyphs.size() * sizeof(YDrawGlyph)), 4u);
    if (!_glyphBuffer || _glyphBufferSize < glyphBufSize) {
        if (_glyphBuffer) wgpuBufferRelease(_glyphBuffer);
        WGPUBufferDescriptor desc{};
        desc.label = WGPU_STR("screen-draw-glyphs");
        desc.size = glyphBufSize;
        desc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        _glyphBuffer = wgpuDeviceCreateBuffer(device, &desc);
        _glyphBufferSize = glyphBufSize;
        _bindGroupDirty = true;
    }
    if (!glyphs.empty())
        wgpuQueueWriteBuffer(queue, _glyphBuffer, 0, glyphs.data(), glyphs.size() * sizeof(YDrawGlyph));

    // Upload uniforms
    ScreenDrawUniforms uniforms{};
    uniforms.sceneMinX = _builder->sceneMinX();
    uniforms.sceneMinY = _builder->sceneMinY();
    uniforms.sceneMaxX = _builder->sceneMaxX();
    uniforms.sceneMaxY = _builder->sceneMaxY();
    uniforms.gridWidth = gridWidth;   // Use local computed value
    uniforms.gridHeight = gridHeight; // Use local computed value
    uniforms.cellSizeX = _builder->cellSizeX();
    uniforms.cellSizeY = _builder->cellSizeY();
    uniforms.primCount = primCount;
    uniforms.glyphCount = static_cast<uint32_t>(glyphs.size());
    uniforms.hasOverlay = 1;  // We have overlay content
    uniforms.offsetX = _originOffsetX;
    uniforms.offsetY = _originOffsetY;

    // Get pixel range from font atlas
    auto font = _ctx.fontManager->getDefaultMsMsdfFont();
    if (font) {
        auto atlas = font->atlas();
        if (atlas)
            uniforms.pixelRange = static_cast<float>(atlas->getPixelRange());
    }
    if (uniforms.pixelRange <= 0) uniforms.pixelRange = 4.0f;

    wgpuQueueWriteBuffer(queue, _uniformBuffer, 0, &uniforms, sizeof(uniforms));

    // Rebuild bind group if buffers changed
    if (_bindGroupDirty) {
        if (_bindGroup) wgpuBindGroupRelease(_bindGroup);

        auto font = _ctx.fontManager->getDefaultMsMsdfFont();
        auto atlas = font ? font->atlas() : nullptr;
        if (!atlas) {
            ywarn("ScreenDrawLayer: no font atlas available");
            _bindGroup = nullptr;
            return;
        }

        WGPUBindGroupEntry bgEntries[7] = {};

        bgEntries[0].binding = 0;
        bgEntries[0].buffer = _uniformBuffer;
        bgEntries[0].size = sizeof(ScreenDrawUniforms);

        bgEntries[1].binding = 1;
        bgEntries[1].buffer = _primBuffer;
        bgEntries[1].size = _primBufferSize;

        bgEntries[2].binding = 2;
        bgEntries[2].buffer = _gridBuffer;
        bgEntries[2].size = _gridBufferSize;

        bgEntries[3].binding = 3;
        bgEntries[3].buffer = _glyphBuffer;
        bgEntries[3].size = _glyphBufferSize;

        bgEntries[4].binding = 4;
        bgEntries[4].textureView = atlas->getTextureView();

        bgEntries[5].binding = 5;
        bgEntries[5].sampler = atlas->getSampler();

        bgEntries[6].binding = 6;
        bgEntries[6].buffer = atlas->getGlyphMetadataBuffer();
        bgEntries[6].size = wgpuBufferGetSize(atlas->getGlyphMetadataBuffer());

        WGPUBindGroupDescriptor bgDesc{};
        bgDesc.label = WGPU_STR("screen-draw-bind-group");
        bgDesc.layout = _bindGroupLayout;
        bgDesc.entryCount = 7;
        bgDesc.entries = bgEntries;
        _bindGroup = wgpuDeviceCreateBindGroup(_ctx.gpu.device, &bgDesc);
        _bindGroupDirty = false;
    }
}

void ScreenDrawLayerImpl::updateDisplaySize(uint32_t width, uint32_t height) {
    _displayWidth = width;
    _displayHeight = height;
    // Note: Don't set scene bounds here - let builder compute from content AABBs
    // This ensures getContentHeightCells() returns actual content height, not display height
}

void ScreenDrawLayerImpl::setCellSize(float cellWidth, float cellHeight) {
    _cellWidth = cellWidth;
    _cellHeight = cellHeight;

    // Update builder grid cell size to match terminal cells (1:1 mapping)
    if (_builder) {
        _builder->setGridCellSize(cellWidth, cellHeight);
        _dirty = true;
    }
}

void ScreenDrawLayerImpl::clear() {
    if (_buffer) {
        _buffer->clear();
        _dirty = true;
    }
    // Reset scroll state
    _rowEntries.clear();
    _scrollableGrid.clear();
    _rowsInitialized = false;
}

Result<void> ScreenDrawLayerImpl::update(const std::string& args, const std::string& payload) {
    if (payload.empty()) return Ok();

    ydebug("ScreenDrawLayer::update: args='{}' payload={} bytes", args, payload.size());

    // Parse args
    bool append = args.find("--append") != std::string::npos;
    bool yamlMode = args.find("--yaml") != std::string::npos;

    if (!append) {
        _buffer->clear();
    }

    // Parse payload (binary or YAML)
    if (yamlMode) {
        auto res = parseYDrawYAML(_buffer, payload);
        if (!res) {
            yerror("ScreenDrawLayer::update: parseYDrawYAML failed: {}", error_msg(res));
            return Err<void>(res.error().message());
        }
    } else {
        if (auto res = parseBinary(payload); !res) {
            yerror("ScreenDrawLayer::update: parseBinary failed: {}", error_msg(res));
            return res;
        }
    }

    // Calculate immediately so scene bounds are available for getContentHeightCells()
    if (_builder) {
        _builder->calculate();
        // Extract grid into per-row format for scroll support
        extractRowsFromGrid();
        buildGridFromRows();
    }

    _dirty = true;  // Still mark dirty for buffer rebuild in prepareForRender
    ydebug("ScreenDrawLayer::update: {} prims, {} text spans",
           _buffer->primCount(), _buffer->textSpanCount());
    return Ok();
}

void ScreenDrawLayerImpl::setOriginOffset(float x, float y) {
    _originOffsetX = x;
    _originOffsetY = y;
    // Note: Don't set _dirty here - offset is just a uniform, not a grid change
}

void ScreenDrawLayerImpl::scroll(int32_t num_lines) {
    if (num_lines == 0) return;
    if (!_builder) return;

    // Two things must happen for scroll:
    // 1. Builder shifts grid row references (row N becomes row N-1)
    //    This ensures correct primitives are found for each screen position
    // 2. Offset adjusts where primitives are drawn on screen
    //    This shifts the visual rendering position
    _builder->scroll(num_lines);
    _originOffsetY -= static_cast<float>(num_lines) * _cellHeight;
    _dirty = true;
}

void ScreenDrawLayerImpl::extractRowsFromGrid() {
    if (!_builder) return;

    const auto& gridData = _builder->gridStaging();
    uint32_t gridW = _builder->gridWidth();
    uint32_t gridH = _builder->gridHeight();

    if (gridData.empty() || gridW == 0 || gridH == 0) {
        _rowEntries.clear();
        _rowsInitialized = false;
        return;
    }

    _gridWidth = gridW;
    _gridHeight = gridH;

    // Resize to [gridH][gridW]
    _rowEntries.resize(gridH);
    for (uint32_t row = 0; row < gridH; row++) {
        _rowEntries[row].resize(gridW);
        for (uint32_t col = 0; col < gridW; col++) {
            _rowEntries[row][col].clear();
        }
    }

    // Extract entries from builder's grid
    uint32_t numCells = gridW * gridH;
    for (uint32_t row = 0; row < gridH; row++) {
        for (uint32_t col = 0; col < gridW; col++) {
            uint32_t cellIdx = row * gridW + col;
            if (cellIdx >= numCells) continue;

            uint32_t offset = gridData[cellIdx];
            if (offset >= gridData.size()) continue;

            uint32_t count = gridData[offset];
            for (uint32_t i = 0; i < count; i++) {
                uint32_t entryIdx = offset + 1 + i;
                if (entryIdx >= gridData.size()) break;
                _rowEntries[row][col].push_back(gridData[entryIdx]);
            }
        }
    }

    _rowsInitialized = true;
    yinfo("extractRowsFromGrid: {}x{} grid extracted", gridW, gridH);
}

void ScreenDrawLayerImpl::buildGridFromRows() {
    if (!_rowsInitialized || _rowEntries.empty()) {
        _scrollableGrid.clear();
        return;
    }

    uint32_t gridW = _gridWidth;
    uint32_t gridH = static_cast<uint32_t>(_rowEntries.size());
    uint32_t numCells = gridW * gridH;

    // Pass 1: count entries per cell
    std::vector<uint32_t> cellCounts(numCells, 0);
    for (uint32_t row = 0; row < gridH; row++) {
        for (uint32_t col = 0; col < gridW && col < _rowEntries[row].size(); col++) {
            cellCounts[row * gridW + col] = static_cast<uint32_t>(_rowEntries[row][col].size());
        }
    }

    // Compute offsets (prefix sum)
    uint32_t pos = numCells;
    _scrollableGrid.resize(numCells);
    for (uint32_t i = 0; i < numCells; i++) {
        _scrollableGrid[i] = pos;
        pos += 1 + cellCounts[i];  // +1 for count field
    }
    _scrollableGrid.resize(pos, 0);

    // Initialize count fields to 0
    for (uint32_t i = 0; i < numCells; i++) {
        _scrollableGrid[_scrollableGrid[i]] = 0;
    }

    // Pass 2: fill entries
    for (uint32_t row = 0; row < gridH; row++) {
        for (uint32_t col = 0; col < gridW && col < _rowEntries[row].size(); col++) {
            uint32_t cellIdx = row * gridW + col;
            uint32_t off = _scrollableGrid[cellIdx];
            for (uint32_t primIdx : _rowEntries[row][col]) {
                uint32_t cnt = _scrollableGrid[off];
                _scrollableGrid[off + 1 + cnt] = primIdx;
                _scrollableGrid[off] = cnt + 1;
            }
        }
    }
}

void ScreenDrawLayerImpl::shiftRows(int32_t lines) {
    if (lines == 0 || _rowEntries.empty()) return;

    uint32_t gridW = _gridWidth;

    if (lines > 0) {
        // Scroll up: drop top rows, add empty rows at bottom
        uint32_t drop = std::min(static_cast<uint32_t>(lines), static_cast<uint32_t>(_rowEntries.size()));
        _rowEntries.erase(_rowEntries.begin(), _rowEntries.begin() + drop);

        // Add empty rows at bottom
        for (uint32_t i = 0; i < drop; i++) {
            _rowEntries.emplace_back(gridW);  // Empty row with gridW empty cells
        }
    } else {
        // Scroll down: drop bottom rows, add empty rows at top
        uint32_t drop = std::min(static_cast<uint32_t>(-lines), static_cast<uint32_t>(_rowEntries.size()));
        _rowEntries.erase(_rowEntries.end() - drop, _rowEntries.end());

        // Add empty rows at top
        for (uint32_t i = 0; i < drop; i++) {
            _rowEntries.insert(_rowEntries.begin(), std::vector<std::vector<uint32_t>>(gridW));
        }
    }

    yinfo("shiftRows: shifted by {} lines, now {} rows", lines, _rowEntries.size());
}

void ScreenDrawLayerImpl::getContentBounds(float& minX, float& minY, float& maxX, float& maxY) const {
    if (_buffer) {
        minX = _buffer->sceneMinX();
        minY = _buffer->sceneMinY();
        maxX = _buffer->sceneMaxX();
        maxY = _buffer->sceneMaxY();
    } else {
        minX = minY = maxX = maxY = 0.0f;
    }
}

uint32_t ScreenDrawLayerImpl::getContentHeightCells() const {
    if (!_builder || _cellHeight <= 0.0f) return 0;
    // Use builder's calculated scene bounds (set by calculate())
    float height = _builder->sceneMaxY() - _builder->sceneMinY();
    return static_cast<uint32_t>(std::ceil(height / _cellHeight));
}

Result<void> ScreenDrawLayerImpl::parseBinary(const std::string& payload) {
    if (payload.empty()) return Ok();

    const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());
    size_t size = payload.size();

    // Check for LZ4 frame magic (0x184D2204 little-endian)
    std::vector<uint8_t> decompressed;
    if (size >= 4) {
        uint32_t magic = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        if (magic == LZ4F_MAGICNUMBER) {
            ydebug("parseBinary: detected LZ4 compressed data");
            auto res = decompressLZ4(data, size, decompressed);
            if (!res) return res;
            data = decompressed.data();
            size = decompressed.size();
            yinfo("parseBinary: decompressed {} -> {} bytes", payload.size(), size);
        }
    }

    auto res = _buffer->deserialize(data, size);
    if (!res) {
        yerror("parseBinary: deserialize failed: {}", error_msg(res));
    }
    return res;
}

Result<void> ScreenDrawLayerImpl::decompressLZ4(const uint8_t* src, size_t srcSize, std::vector<uint8_t>& dst) {
    LZ4F_dctx* dctx = nullptr;
    LZ4F_errorCode_t err = LZ4F_createDecompressionContext(&dctx, LZ4F_VERSION);
    if (LZ4F_isError(err)) {
        return Err<void>(std::string("LZ4F_createDecompressionContext: ") + LZ4F_getErrorName(err));
    }

    // Get frame info to know uncompressed size (if available)
    LZ4F_frameInfo_t info;
    size_t srcConsumed = srcSize;
    err = LZ4F_getFrameInfo(dctx, &info, src, &srcConsumed);
    if (LZ4F_isError(err)) {
        LZ4F_freeDecompressionContext(dctx);
        return Err<void>(std::string("LZ4F_getFrameInfo: ") + LZ4F_getErrorName(err));
    }

    // Allocate output buffer
    size_t dstCapacity = info.contentSize ? info.contentSize : srcSize * 4;
    dst.resize(dstCapacity);

    size_t dstPos = 0;
    size_t srcPos = srcConsumed;

    while (srcPos < srcSize) {
        size_t dstRemaining = dst.size() - dstPos;
        size_t srcRemaining = srcSize - srcPos;
        size_t ret = LZ4F_decompress(dctx, dst.data() + dstPos, &dstRemaining,
                                      src + srcPos, &srcRemaining, nullptr);
        if (LZ4F_isError(ret)) {
            LZ4F_freeDecompressionContext(dctx);
            return Err<void>(std::string("LZ4F_decompress: ") + LZ4F_getErrorName(ret));
        }
        dstPos += dstRemaining;
        srcPos += srcRemaining;

        // Grow buffer if needed
        if (dstPos >= dst.size() && srcPos < srcSize) {
            dst.resize(dst.size() * 2);
        }
    }

    dst.resize(dstPos);
    LZ4F_freeDecompressionContext(dctx);
    return Ok();
}

bool ScreenDrawLayerImpl::hasContent() const {
    return _buffer && !_buffer->empty();
}

void ScreenDrawLayerImpl::prepareForRender() {
    if (!_buffer || _buffer->empty()) return;

    // Recalculate spatial hash grid if dirty
    if (_dirty) {
        _builder->setSceneBounds(0, 0, static_cast<float>(_displayWidth),
                                  static_cast<float>(_displayHeight));
        _builder->calculate();
        rebuildBuffers();
        _dirty = false;
    }
}

Result<void> ScreenDrawLayerImpl::render(WGPURenderPassEncoder pass) {
    if (!_pipeline) return Ok();
    if (!hasContent()) return Ok();

    // Recalculate if dirty
    if (_dirty) {
        _builder->setSceneBounds(0, 0,
            static_cast<float>(_displayWidth),
            static_cast<float>(_displayHeight));
        _builder->calculate();
        rebuildBuffers();
        _dirty = false;
    }

    if (!_bindGroup) return Ok();

    // Set viewport to full screen
    wgpuRenderPassEncoderSetViewport(pass, 0, 0,
        static_cast<float>(_displayWidth), static_cast<float>(_displayHeight),
        0.0f, 1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass, 0, 0, _displayWidth, _displayHeight);

    wgpuRenderPassEncoderSetPipeline(pass, _pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 3, 1, 0, 0);

    return Ok();
}

//=============================================================================
// Factory
//=============================================================================

Result<ScreenDrawLayer::Ptr> ScreenDrawLayer::createImpl(
    ContextType&, const YettyContext& yettyCtx) noexcept {

    auto impl = std::make_shared<ScreenDrawLayerImpl>(yettyCtx);
    if (auto res = impl->init(); !res) {
        return Err<ScreenDrawLayer::Ptr>("ScreenDrawLayer init failed", res);
    }
    return Ok(std::static_pointer_cast<ScreenDrawLayer>(impl));
}

} // namespace yetty
