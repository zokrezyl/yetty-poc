#include "ypaint-overlay.h"
#include <yetty/yetty-context.h>
#include <yetty/gpu-memory-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include <ytrace/ytrace.hpp>

#include <cstring>
#include <fstream>
#include <sstream>
#include <filesystem>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif
#ifndef YETTY_SHADERS_DIR
#define YETTY_SHADERS_DIR CMAKE_SOURCE_DIR "/src/yetty/shaders"
#endif

namespace yetty::ypaint {

// Uniform buffer layout — must match shader (96 bytes)
// WGSL vec3<f32> has 16-byte alignment, so total struct is 96 bytes
struct YPaintOverlayUniforms {
    float screenWidth;          // 0
    float screenHeight;         // 4
    // Metadata fields
    uint32_t primitiveOffset;   // 8
    uint32_t primitiveCount;    // 12
    uint32_t gridOffset;        // 16
    uint32_t gridWidth;         // 20
    uint32_t gridHeight;        // 24
    uint32_t cellSizeXY;        // 28 (f16 pair)
    uint32_t glyphOffset;       // 32
    uint32_t glyphCount;        // 36
    uint32_t sceneMinX;         // 40 (f32 as bits)
    uint32_t sceneMinY;         // 44
    uint32_t sceneMaxX;         // 48
    uint32_t sceneMaxY;         // 52
    uint32_t widthCells;        // 56
    uint32_t heightCells;       // 60
    uint32_t flags;             // 64
    uint32_t bgColor;           // 68
    float pixelRange;           // 72
    float _pad0;                // 76 (pad to 80 for vec3 alignment)
    float _pad1[4];             // 80-96 (vec3<f32> = 12 bytes + 4 alignment = 16)
};

class YPaintOverlayImpl : public YPaintOverlay {
public:
    YPaintOverlayImpl(const YettyContext& ctx, Painter::Ptr painter);
    ~YPaintOverlayImpl() override;

    Result<void> init() noexcept;

    void updateDisplaySize(uint32_t width, uint32_t height) override;
    Result<void> render(WGPURenderPassEncoder pass) override;
    bool hasContent() const override;
    Painter::Ptr painter() const override { return _painter; }

private:
    Result<void> createPipeline();
    Result<void> buildShaderSource(std::string& source);
    void updateBindGroup();
    void updateUniforms();

    const YettyContext& _ctx;
    Painter::Ptr _painter;

    uint32_t _displayWidth = 0;
    uint32_t _displayHeight = 0;

    // GPU resources
    WGPURenderPipeline _pipeline = nullptr;
    WGPUPipelineLayout _pipelineLayout = nullptr;
    WGPUBindGroupLayout _bindGroupLayout = nullptr;
    WGPUBindGroup _bindGroup = nullptr;
    WGPUBuffer _uniformBuffer = nullptr;
    bool _bindGroupDirty = true;
};

YPaintOverlayImpl::YPaintOverlayImpl(const YettyContext& ctx, Painter::Ptr painter)
    : _ctx(ctx), _painter(std::move(painter)) {}

YPaintOverlayImpl::~YPaintOverlayImpl() {
    if (_bindGroup) wgpuBindGroupRelease(_bindGroup);
    if (_uniformBuffer) wgpuBufferRelease(_uniformBuffer);
    if (_pipeline) wgpuRenderPipelineRelease(_pipeline);
    if (_pipelineLayout) wgpuPipelineLayoutRelease(_pipelineLayout);
    if (_bindGroupLayout) wgpuBindGroupLayoutRelease(_bindGroupLayout);
}

Result<void> YPaintOverlayImpl::init() noexcept {
    if (!_ctx.gpu.device || !_ctx.gpu.queue)
        return Err<void>("YPaintOverlay: GPUContext not initialized");

    // Create uniform buffer
    WGPUBufferDescriptor ubDesc{};
    ubDesc.label = WGPU_STR("ypaint-overlay-uniforms");
    ubDesc.size = sizeof(YPaintOverlayUniforms);
    ubDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _uniformBuffer = wgpuDeviceCreateBuffer(_ctx.gpu.device, &ubDesc);
    if (!_uniformBuffer)
        return Err<void>("YPaintOverlay: failed to create uniform buffer");

    // Create pipeline
    if (auto res = createPipeline(); !res)
        return Err<void>("YPaintOverlay: pipeline creation failed", res);

    ydebug("YPaintOverlay initialized");
    return Ok();
}

Result<void> YPaintOverlayImpl::buildShaderSource(std::string& source) {
    std::string shaderDir = std::string(YETTY_SHADERS_DIR);

    // Read ypaint overlay shader
    std::string overlayPath = shaderDir + "/ypaint-overlay.wgsl";
    std::ifstream overlayFile(overlayPath);
    if (!overlayFile.is_open())
        return Err<void>("Cannot open ypaint overlay shader: " + overlayPath);
    std::stringstream buf;
    buf << overlayFile.rdbuf();
    source = buf.str();

    // Note: SDF libraries (ypaint-sdf.gen, distfunctions) not injected yet.
    // Currently only rendering glyphs via MSDF. SDF primitives will be added later.

    return Ok();
}

Result<void> YPaintOverlayImpl::createPipeline() {
    auto device = _ctx.gpu.device;

    std::string shaderSource;
    if (auto res = buildShaderSource(shaderSource); !res)
        return Err<void>("Shader source build failed", res);

    WGPUShaderSourceWGSL wgslDesc{};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, shaderSource);

    WGPUShaderModuleDescriptor smDesc{};
    smDesc.label = WGPU_STR("ypaint-overlay-shader");
    smDesc.nextInChain = &wgslDesc.chain;

    wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &smDesc);

    {
        bool hadError = false;
        std::string errorMsg;
        WGPUPopErrorScopeCallbackInfo popInfo = {};
        popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        popInfo.callback = [](WGPUPopErrorScopeStatus, WGPUErrorType type,
                              WGPUStringView message, void* ud1, void* ud2) {
            if (type != WGPUErrorType_NoError) {
                *static_cast<bool*>(ud1) = true;
                auto* msg = static_cast<std::string*>(ud2);
                if (message.data && message.length > 0)
                    *msg = std::string(message.data, message.length);
            }
        };
        popInfo.userdata1 = &hadError;
        popInfo.userdata2 = &errorMsg;
        wgpuDevicePopErrorScope(device, popInfo);
        WGPU_DEVICE_TICK(device);

        if (hadError) {
            yerror("YPaintOverlay: SHADER ERROR: {}", errorMsg);
            if (shaderModule) wgpuShaderModuleRelease(shaderModule);
            return Err<void>("Shader compilation failed: " + errorMsg);
        }
    }

    if (!shaderModule)
        return Err<void>("Failed to create ypaint overlay shader module");

    // Bind group layout: uniform, cardStorage, fontTexture, fontSampler, glyphMetadata
    WGPUBindGroupLayoutEntry entries[5] = {};

    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Fragment;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    entries[0].buffer.minBindingSize = sizeof(YPaintOverlayUniforms);

    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Fragment;
    entries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Fragment;
    entries[2].texture.sampleType = WGPUTextureSampleType_Float;
    entries[2].texture.viewDimension = WGPUTextureViewDimension_2D;

    entries[3].binding = 3;
    entries[3].visibility = WGPUShaderStage_Fragment;
    entries[3].sampler.type = WGPUSamplerBindingType_Filtering;

    entries[4].binding = 4;
    entries[4].visibility = WGPUShaderStage_Fragment;
    entries[4].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor bglDesc{};
    bglDesc.label = WGPU_STR("ypaint-overlay-bind-group-layout");
    bglDesc.entryCount = 5;
    bglDesc.entries = entries;
    _bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc{};
    plDesc.label = WGPU_STR("ypaint-overlay-pipeline-layout");
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
    rpDesc.label = WGPU_STR("ypaint-overlay-pipeline");
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
        bool hadError = false;
        std::string errorMsg;
        WGPUPopErrorScopeCallbackInfo popInfo = {};
        popInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        popInfo.callback = [](WGPUPopErrorScopeStatus, WGPUErrorType type,
                              WGPUStringView message, void* ud1, void* ud2) {
            if (type != WGPUErrorType_NoError) {
                *static_cast<bool*>(ud1) = true;
                auto* msg = static_cast<std::string*>(ud2);
                if (message.data && message.length > 0)
                    *msg = std::string(message.data, message.length);
            }
        };
        popInfo.userdata1 = &hadError;
        popInfo.userdata2 = &errorMsg;
        wgpuDevicePopErrorScope(device, popInfo);
        WGPU_DEVICE_TICK(device);

        if (hadError) {
            yerror("YPaintOverlay: PIPELINE ERROR: {}", errorMsg);
        }
    }

    wgpuShaderModuleRelease(shaderModule);

    if (!_pipeline)
        return Err<void>("Failed to create ypaint overlay render pipeline");

    ydebug("YPaintOverlay: pipeline created successfully");
    return Ok();
}

void YPaintOverlayImpl::updateDisplaySize(uint32_t width, uint32_t height) {
    if (_displayWidth != width || _displayHeight != height) {
        _displayWidth = width;
        _displayHeight = height;
        _bindGroupDirty = true;
    }
}

void YPaintOverlayImpl::updateBindGroup() {
    if (!_bindGroupDirty) return;
    if (_bindGroup) {
        wgpuBindGroupRelease(_bindGroup);
        _bindGroup = nullptr;
    }

    auto cardMgr = _ctx.cardManager;
    if (!cardMgr) return;

    auto font = _ctx.fontManager->getDefaultMsMsdfFont();
    auto atlas = font ? font->atlas() : nullptr;
    if (!atlas) {
        ywarn("YPaintOverlay: no font atlas");
        return;
    }

    WGPUBindGroupEntry bgEntries[5] = {};

    bgEntries[0].binding = 0;
    bgEntries[0].buffer = _uniformBuffer;
    bgEntries[0].size = sizeof(YPaintOverlayUniforms);

    bgEntries[1].binding = 1;
    bgEntries[1].buffer = cardMgr->bufferManager()->bufferGpu();
    bgEntries[1].size = wgpuBufferGetSize(cardMgr->bufferManager()->bufferGpu());

    bgEntries[2].binding = 2;
    bgEntries[2].textureView = atlas->getTextureView();

    bgEntries[3].binding = 3;
    bgEntries[3].sampler = atlas->getSampler();

    bgEntries[4].binding = 4;
    bgEntries[4].buffer = atlas->getGlyphMetadataBuffer();
    bgEntries[4].size = wgpuBufferGetSize(atlas->getGlyphMetadataBuffer());

    WGPUBindGroupDescriptor bgDesc{};
    bgDesc.label = WGPU_STR("ypaint-overlay-bind-group");
    bgDesc.layout = _bindGroupLayout;
    bgDesc.entryCount = 5;
    bgDesc.entries = bgEntries;
    _bindGroup = wgpuDeviceCreateBindGroup(_ctx.gpu.device, &bgDesc);
    _bindGroupDirty = false;
}

void YPaintOverlayImpl::updateUniforms() {
    if (!_painter) return;

    YPaintOverlayUniforms u{};
    u.screenWidth = static_cast<float>(_displayWidth);
    u.screenHeight = static_cast<float>(_displayHeight);
    u.primitiveOffset = _painter->gpuPrimitiveOffset();
    u.primitiveCount = _painter->primitiveCount();
    u.gridOffset = _painter->gpuGridOffset();
    u.gridWidth = _painter->gridWidth();
    u.gridHeight = _painter->gridHeight();

    // Pack cell sizes as f16 pair
    auto toF16 = [](float f) -> uint16_t {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        uint16_t sign = (b >> 16) & 0x8000;
        int32_t exp = ((b >> 23) & 0xFF) - 127 + 15;
        uint16_t mant = (b >> 13) & 0x3FF;
        if (exp <= 0) { exp = 0; mant = 0; }
        else if (exp >= 31) { exp = 31; mant = 0; }
        return sign | (static_cast<uint16_t>(exp) << 10) | mant;
    };
    u.cellSizeXY = static_cast<uint32_t>(toF16(_painter->cellSizeX())) |
                   (static_cast<uint32_t>(toF16(_painter->cellSizeY())) << 16);

    u.glyphOffset = _painter->gpuGlyphOffset();
    u.glyphCount = _painter->glyphCount();

    // Scene bounds as u32 bits
    float minX = _painter->sceneMinX();
    float minY = _painter->sceneMinY();
    float maxX = _painter->sceneMaxX();
    float maxY = _painter->sceneMaxY();
    std::memcpy(&u.sceneMinX, &minX, 4);
    std::memcpy(&u.sceneMinY, &minY, 4);
    std::memcpy(&u.sceneMaxX, &maxX, 4);
    std::memcpy(&u.sceneMaxY, &maxY, 4);

    u.widthCells = _displayWidth / static_cast<uint32_t>(_painter->cellSizeX());
    u.heightCells = _displayHeight / static_cast<uint32_t>(_painter->cellSizeY());
    u.flags = _painter->flags();
    u.bgColor = _painter->bgColor();

    // Pixel range from atlas
    auto font = _ctx.fontManager->getDefaultMsMsdfFont();
    if (font && font->atlas())
        u.pixelRange = static_cast<float>(font->atlas()->getPixelRange());
    if (u.pixelRange <= 0) u.pixelRange = 4.0f;

    wgpuQueueWriteBuffer(_ctx.gpu.queue, _uniformBuffer, 0, &u, sizeof(u));
}

bool YPaintOverlayImpl::hasContent() const {
    return _painter && _painter->hasContent();
}

Result<void> YPaintOverlayImpl::render(WGPURenderPassEncoder pass) {
    if (!_pipeline) return Ok();
    if (!hasContent()) return Ok();

    updateBindGroup();
    if (!_bindGroup) return Ok();

    updateUniforms();

    ydebug("YPaintOverlay::render: display={}x{} primOff={} primCnt={} gridOff={} "
           "grid={}x{} glyphOff={} glyphCnt={} scene=[{},{}]-[{},{}]",
           _displayWidth, _displayHeight,
           _painter->gpuPrimitiveOffset(), _painter->primitiveCount(),
           _painter->gpuGridOffset(), _painter->gridWidth(), _painter->gridHeight(),
           _painter->gpuGlyphOffset(), _painter->glyphCount(),
           _painter->sceneMinX(), _painter->sceneMinY(),
           _painter->sceneMaxX(), _painter->sceneMaxY());

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

Result<YPaintOverlay::Ptr> YPaintOverlay::createImpl(
    const YettyContext& yettyCtx, Painter::Ptr painter) noexcept {

    auto impl = std::make_shared<YPaintOverlayImpl>(yettyCtx, std::move(painter));
    if (auto res = impl->init(); !res) {
        return Err<YPaintOverlay::Ptr>("YPaintOverlay init failed", res);
    }
    return Ok(std::static_pointer_cast<YPaintOverlay>(impl));
}

} // namespace yetty::ypaint
