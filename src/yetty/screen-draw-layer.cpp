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

    const YettyContext& _ctx;

    // Display
    uint32_t _displayWidth = 0;
    uint32_t _displayHeight = 0;
    float _cellWidth = 10.0f;
    float _cellHeight = 20.0f;

    // YDraw
    YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    bool _dirty = false;

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

    ydebug("ScreenDrawLayer initialized");
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

    ydebug("ScreenDrawLayer: pipeline created successfully");
    return Ok();
}

void ScreenDrawLayerImpl::rebuildBuffers() {
    auto device = _ctx.gpu.device;
    auto queue = _ctx.gpu.queue;

    // Get computed data from builder
    const auto& gridStaging = _builder->gridStaging();
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

    // Translate grid prim indices to word offsets
    std::vector<uint32_t> gridData = gridStaging;  // copy
    uint32_t gridWidth = _builder->gridWidth();
    uint32_t gridHeight = _builder->gridHeight();
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
    uniforms.gridWidth = _builder->gridWidth();
    uniforms.gridHeight = _builder->gridHeight();
    uniforms.cellSizeX = _builder->cellSizeX();
    uniforms.cellSizeY = _builder->cellSizeY();
    uniforms.primCount = primCount;
    uniforms.glyphCount = static_cast<uint32_t>(glyphs.size());
    uniforms.hasOverlay = 1;  // We have overlay content

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

    // Update scene bounds to match display
    if (_builder) {
        _builder->setSceneBounds(0, 0, static_cast<float>(width), static_cast<float>(height));
        _dirty = true;
    }
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

    _dirty = true;
    ydebug("ScreenDrawLayer::update: {} prims, {} text spans",
           _buffer->primCount(), _buffer->textSpanCount());
    return Ok();
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
            ydebug("parseBinary: decompressed {} -> {} bytes", payload.size(), size);
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
