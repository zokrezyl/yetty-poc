#include "ygui-overlay.h"
#include <yetty/yetty-context.h>
#include <yetty/wgpu-compat.h>
#include <yetty/base/event-loop.h>
#include <yetty/ydraw-builder.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/msdf-atlas.h>
#include "ygui-engine.h"
#include "ygui-io.h"
#include "ygui-widgets.h"
#include "ygui-theme.h"
#include "widgets/hbox.h"
#include "widgets/vbox.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "widgets/panel.h"
#include "widgets/scroll-area.h"
#include "../ydraw/ydraw-buffer.h"
#include <ytrace/ytrace.hpp>

#include <algorithm>
#include <atomic>
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

// GPU uniform buffer layout — must match OverlayUniforms in shader
struct OverlayUniforms {
    float screenWidth;
    float screenHeight;
    float sceneMinX;
    float sceneMinY;
    float sceneMaxX;
    float sceneMaxY;
    uint32_t gridWidth;
    uint32_t gridHeight;
    float cellSize;
    uint32_t primCount;
    uint32_t glyphCount;
    float pixelRange;
};

class YGuiOverlayImpl;  // forward decl

//=============================================================================
// OverlayIO — in-process IO that handles events from ygui widgets
//=============================================================================
class OverlayIO : public ygui::YGuiIO {
public:
    YGuiOverlayImpl* impl = nullptr;
    void emitEvent(const ygui::WidgetEvent& event) override;
};

//=============================================================================
// YGuiOverlayImpl
//=============================================================================
class YGuiOverlayImpl : public YGuiOverlay {
public:
    explicit YGuiOverlayImpl(const YettyContext& ctx);
    ~YGuiOverlayImpl() override;

    Result<void> init() noexcept;
    void registerForEvents();

    // EventListener
    Result<bool> onEvent(const base::Event& event) override;

    // YGuiOverlay interface
    void updateDisplaySize(uint32_t width, uint32_t height) override;
    void setStatusText(const std::string& text) override;
    void setFps(uint32_t fps) override;
    float getStatusbarHeight() const override { return STATUSBAR_HEIGHT; }
    void beginContextMenu(float x, float y) override;
    void addContextMenuItem(const ContextMenuItem& item) override;
    void clearContextMenu() override;
    bool isContextMenuOpen() const override { return _menuOpen; }
    void showGpuStatsDialog(std::function<std::string()> provider) override;
    Result<void> render(WGPURenderPassEncoder pass) override;
    ygui::YGuiEngine& engine() override { return *_engine; }
    float measureText(const std::string& text, float fontSize) override;

    static constexpr float STATUSBAR_HEIGHT = 22.0f;

    // Called by OverlayIO when widget events occur
    void handleWidgetEvent(const ygui::WidgetEvent& event);

private:
    Result<void> createPipeline();
    Result<void> buildShaderSource(std::string& source);
    void rebuildBuffers();
    void buildStatusbar();
    void buildContextMenu();
    void buildGpuStatsDialog();
    void closeGpuStatsDialog();
    bool isContextMenuWidget(const ygui::WidgetPtr& w) const;

    const YettyContext& _ctx;
    base::ObjectId _id;

    // Display
    uint32_t _displayWidth = 0;
    uint32_t _displayHeight = 0;

    // YGui engine
    YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    std::unique_ptr<OverlayIO> _io;
    std::unique_ptr<ygui::YGuiEngine> _engine;

    // Statusbar widgets
    ygui::WidgetPtr _statusbarRoot;
    std::shared_ptr<ygui::Label> _statusLabel;
    std::shared_ptr<ygui::Label> _fpsLabel;

    // Context menu
    std::vector<ContextMenuItem> _menuItems;
    bool _menuOpen = false;
    float _menuX = 0, _menuY = 0;
    ygui::WidgetPtr _contextMenuRoot;

    // GPU stats
    bool _showGpuStats = false;
    std::function<std::string()> _gpuStatsProvider;
    ygui::WidgetPtr _gpuStatsRoot;
    std::shared_ptr<ygui::Label> _gpuStatsLabel;

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

// ESC key constant (same as GLFW_KEY_ESCAPE)
constexpr int KEY_ESCAPE = 256;

static base::ObjectId s_nextOverlayId = 0xFFFE0000;

void OverlayIO::emitEvent(const ygui::WidgetEvent& event) {
    if (impl) impl->handleWidgetEvent(event);
}

YGuiOverlayImpl::YGuiOverlayImpl(const YettyContext& ctx)
    : _ctx(ctx), _id(s_nextOverlayId++) {}

YGuiOverlayImpl::~YGuiOverlayImpl() {
    if (_bindGroup) wgpuBindGroupRelease(_bindGroup);
    if (_uniformBuffer) wgpuBufferRelease(_uniformBuffer);
    if (_primBuffer) wgpuBufferRelease(_primBuffer);
    if (_gridBuffer) wgpuBufferRelease(_gridBuffer);
    if (_glyphBuffer) wgpuBufferRelease(_glyphBuffer);
    if (_pipeline) wgpuRenderPipelineRelease(_pipeline);
    if (_pipelineLayout) wgpuPipelineLayoutRelease(_pipelineLayout);
    if (_bindGroupLayout) wgpuBindGroupLayoutRelease(_bindGroupLayout);
}

Result<void> YGuiOverlayImpl::init() noexcept {
    if (!_ctx.gpu.device || !_ctx.gpu.queue)
        return Err<void>("YGuiOverlay: GPUContext not initialized");

    // Create YDrawBuffer and builder (no CardManager - standalone)
    _buffer = *YDrawBuffer::create();
    auto builderRes = YDrawBuilder::create(_ctx.fontManager, _ctx.gpuAllocator, _buffer);
    if (!builderRes) return Err<void>("YGuiOverlay: failed to create builder", builderRes);
    _builder = *builderRes;

    // Create IO and engine
    _io = std::make_unique<OverlayIO>();
    _io->impl = this;
    _engine = std::make_unique<ygui::YGuiEngine>(_buffer.get(), _io.get());

    // Wire text measurement
    auto builderPtr = _builder;
    _engine->setMeasureTextFn([builderPtr](const std::string& text, float fontSize) {
        return builderPtr->measureTextWidth(text, fontSize);
    });

    // Create uniform buffer
    WGPUBufferDescriptor ubDesc{};
    ubDesc.label = WGPU_STR("ygui-overlay-uniforms");
    ubDesc.size = sizeof(OverlayUniforms);
    ubDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _uniformBuffer = wgpuDeviceCreateBuffer(_ctx.gpu.device, &ubDesc);
    if (!_uniformBuffer) return Err<void>("YGuiOverlay: failed to create uniform buffer");

    // Create pipeline
    if (auto res = createPipeline(); !res)
        return Err<void>("YGuiOverlay: pipeline creation failed", res);

    yinfo("YGuiOverlay initialized");
    return Ok();
}

Result<void> YGuiOverlayImpl::buildShaderSource(std::string& source) {
    std::string shaderDir = std::string(YETTY_SHADERS_DIR);

    // Read overlay shader
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
            ywarn("YGuiOverlay: missing lib {}", path);
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

Result<void> YGuiOverlayImpl::createPipeline() {
    auto device = _ctx.gpu.device;

    // Build shader source
    std::string shaderSource;
    if (auto res = buildShaderSource(shaderSource); !res)
        return Err<void>("Shader source build failed", res);

    // Write composed shader to temp file for debugging
    {
        std::ofstream dbg("/tmp/ygui-overlay-composed.wgsl");
        if (dbg.is_open()) {
            dbg << shaderSource;
            yinfo("YGuiOverlay: wrote composed shader ({} lines) to /tmp/ygui-overlay-composed.wgsl",
                  std::count(shaderSource.begin(), shaderSource.end(), '\n') + 1);
        }
    }

    // Create shader module with error scoping
    WGPUShaderSourceWGSL wgslDesc{};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPU_SHADER_CODE(wgslDesc, shaderSource);

    WGPUShaderModuleDescriptor smDesc{};
    smDesc.label = WGPU_STR("ygui-overlay-shader");
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
        WGPU_DEVICE_TICK(device);  // flush callback so error is detected

        if (hadShaderError) {
            yerror("YGuiOverlay: SHADER COMPILATION ERROR: {}", shaderErrorMsg);
            if (shaderModule) wgpuShaderModuleRelease(shaderModule);
            return Err<void>("Shader compilation failed: " + shaderErrorMsg);
        }
    }

    if (!shaderModule)
        return Err<void>("Failed to create overlay shader module");

    // Bind group layout
    WGPUBindGroupLayoutEntry entries[7] = {};

    // @binding(0): uniform buffer
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Fragment;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    entries[0].buffer.minBindingSize = sizeof(OverlayUniforms);

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
    bglDesc.label = WGPU_STR("ygui-overlay-bind-group-layout");
    bglDesc.entryCount = 7;
    bglDesc.entries = entries;
    _bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc{};
    plDesc.label = WGPU_STR("ygui-overlay-pipeline-layout");
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
    rpDesc.label = WGPU_STR("ygui-overlay-pipeline");
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
        WGPU_DEVICE_TICK(device);  // flush callback so error is detected

        if (hadPipelineError) {
            yerror("YGuiOverlay: PIPELINE CREATION ERROR: {}", pipelineErrorMsg);
        }
    }

    wgpuShaderModuleRelease(shaderModule);

    if (!_pipeline)
        return Err<void>("Failed to create overlay render pipeline");

    yinfo("YGuiOverlay: pipeline created successfully");
    return Ok();
}

void YGuiOverlayImpl::rebuildBuffers() {
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

    // Translate grid prim indices to word offsets (like writeDerived does)
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
        desc.label = WGPU_STR("ygui-overlay-prims");
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
        desc.label = WGPU_STR("ygui-overlay-grid");
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
        desc.label = WGPU_STR("ygui-overlay-glyphs");
        desc.size = glyphBufSize;
        desc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        _glyphBuffer = wgpuDeviceCreateBuffer(device, &desc);
        _glyphBufferSize = glyphBufSize;
        _bindGroupDirty = true;
    }
    if (!glyphs.empty())
        wgpuQueueWriteBuffer(queue, _glyphBuffer, 0, glyphs.data(), glyphs.size() * sizeof(YDrawGlyph));

    // Upload uniforms
    OverlayUniforms uniforms{};
    uniforms.screenWidth = static_cast<float>(_displayWidth);
    uniforms.screenHeight = static_cast<float>(_displayHeight);
    uniforms.sceneMinX = _builder->sceneMinX();
    uniforms.sceneMinY = _builder->sceneMinY();
    uniforms.sceneMaxX = _builder->sceneMaxX();
    uniforms.sceneMaxY = _builder->sceneMaxY();
    uniforms.gridWidth = _builder->gridWidth();
    uniforms.gridHeight = _builder->gridHeight();
    uniforms.cellSize = _builder->cellSize();
    uniforms.primCount = primCount;
    uniforms.glyphCount = static_cast<uint32_t>(glyphs.size());

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
            ywarn("YGuiOverlay: no font atlas available");
            _bindGroup = nullptr;
            return;
        }

        WGPUBindGroupEntry bgEntries[7] = {};

        bgEntries[0].binding = 0;
        bgEntries[0].buffer = _uniformBuffer;
        bgEntries[0].size = sizeof(OverlayUniforms);

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
        bgDesc.label = WGPU_STR("ygui-overlay-bind-group");
        bgDesc.layout = _bindGroupLayout;
        bgDesc.entryCount = 7;
        bgDesc.entries = bgEntries;
        _bindGroup = wgpuDeviceCreateBindGroup(_ctx.gpu.device, &bgDesc);
        _bindGroupDirty = false;
    }
}

void YGuiOverlayImpl::buildStatusbar() {
    if (_statusbarRoot) return;  // already built

    using namespace ygui;

    // Create statusbar: HBox at bottom of screen
    auto hbox = std::make_shared<HBox>();
    hbox->id = "statusbar";
    hbox->h = STATUSBAR_HEIGHT;
    hbox->bgColor = 0xFF334455;  // visible dark blue-gray
    hbox->spacing = 8;

    // Status text label (stretches to fill)
    _statusLabel = std::make_shared<Label>();
    _statusLabel->id = "status-text";
    _statusLabel->label = "Ready";
    _statusLabel->stretch = 1;
    hbox->children.push_back(_statusLabel);

    // FPS label (fixed width)
    _fpsLabel = std::make_shared<Label>();
    _fpsLabel->id = "fps-text";
    _fpsLabel->label = "";
    _fpsLabel->w = 80;
    hbox->children.push_back(_fpsLabel);

    _statusbarRoot = hbox;
    _engine->addWidget(_statusbarRoot);
}

void YGuiOverlayImpl::updateDisplaySize(uint32_t width, uint32_t height) {
    _displayWidth = width;
    _displayHeight = height;

    // Clear drag/press state — coordinate system changes on resize
    _engine->clearInteractionState();

    _engine->setPixelSize(static_cast<float>(width), static_cast<float>(height));

    // Position statusbar at bottom
    if (_statusbarRoot) {
        _statusbarRoot->x = 0;
        _statusbarRoot->y = static_cast<float>(height) - STATUSBAR_HEIGHT;
        _statusbarRoot->w = static_cast<float>(width);
        _engine->markDirty();
    }

    // Clamp GPU stats dialog position to new screen bounds
    if (_gpuStatsRoot) {
        float fw = static_cast<float>(width);
        float fh = static_cast<float>(height);
        float margin = 20.0f;
        // Ensure at least 'margin' pixels of dialog visible
        if (_gpuStatsRoot->x + _gpuStatsRoot->w < margin)
            _gpuStatsRoot->x = margin - _gpuStatsRoot->w;
        if (_gpuStatsRoot->y < 0)
            _gpuStatsRoot->y = 0;
        if (_gpuStatsRoot->x > fw - margin)
            _gpuStatsRoot->x = fw - margin;
        if (_gpuStatsRoot->y > fh - margin)
            _gpuStatsRoot->y = fh - margin;
        _engine->markDirty();
    }
}

void YGuiOverlayImpl::setStatusText(const std::string& text) {
    if (_statusLabel) {
        _statusLabel->label = text;
        _engine->markDirty();
    }
}

void YGuiOverlayImpl::setFps(uint32_t fps) {
    if (_fpsLabel) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%u FPS", fps);
        _fpsLabel->label = buf;
        _engine->markDirty();
    }
}

void YGuiOverlayImpl::beginContextMenu(float x, float y) {
    clearContextMenu();
    _menuX = x;
    _menuY = y;
    _menuOpen = true;
    // Steal focus from GPUScreen
    auto loopRes = base::EventLoop::instance();
    if (loopRes) (*loopRes)->dispatch(base::Event::focusEvent(_id));
}

void YGuiOverlayImpl::addContextMenuItem(const ContextMenuItem& item) {
    _menuItems.push_back(item);
}

void YGuiOverlayImpl::clearContextMenu() {
    if (_contextMenuRoot) {
        _engine->removeWidget(_contextMenuRoot);
        _contextMenuRoot = nullptr;
    }
    _menuItems.clear();
    _menuOpen = false;
    _engine->markDirty();
}

void YGuiOverlayImpl::buildContextMenu() {
    if (_menuItems.empty()) return;

    using namespace ygui;
    auto& t = defaultTheme();

    // Compute width from longest label
    float maxLabelW = 120.0f;
    for (const auto& item : _menuItems) {
        float lw = measureText(item.label, t.fontSize) + t.padLarge * 2;
        if (lw > maxLabelW) maxLabelW = lw;
    }
    float menuW = maxLabelW + t.padLarge * 2;
    float menuH = static_cast<float>(_menuItems.size()) * t.rowHeight + t.padMedium * 2;

    // Clamp position to screen bounds
    float mx = _menuX;
    float my = _menuY;
    if (mx + menuW > static_cast<float>(_displayWidth))
        mx = static_cast<float>(_displayWidth) - menuW;
    if (my + menuH > static_cast<float>(_displayHeight))
        my = static_cast<float>(_displayHeight) - menuH;
    if (mx < 0) mx = 0;
    if (my < 0) my = 0;

    // Build VBox with buttons
    auto vbox = std::make_shared<VBox>();
    vbox->id = "context-menu";
    vbox->x = mx;
    vbox->y = my;
    vbox->w = menuW;
    vbox->h = menuH;
    vbox->bgColor = 0xF0222233;

    for (size_t i = 0; i < _menuItems.size(); i++) {
        auto btn = std::make_shared<Button>();
        btn->id = "menu-item-" + std::to_string(i);
        btn->label = _menuItems[i].label;
        btn->w = menuW;
        btn->h = t.rowHeight;
        btn->bgColor = 0xF0222233;
        btn->onClick = btn->id;  // enables onRelease to emit event
        vbox->children.push_back(btn);
    }

    _contextMenuRoot = vbox;
    _engine->addWidget(_contextMenuRoot);
    _engine->markDirty();
}

bool YGuiOverlayImpl::isContextMenuWidget(const ygui::WidgetPtr& w) const {
    if (!w || !_contextMenuRoot) return false;
    // Check if widget id starts with "menu-item-" or is "context-menu"
    return w->id == "context-menu" || w->id.substr(0, 10) == "menu-item-";
}

void YGuiOverlayImpl::showGpuStatsDialog(std::function<std::string()> provider) {
    closeGpuStatsDialog();
    _gpuStatsProvider = std::move(provider);
    _showGpuStats = true;
    buildGpuStatsDialog();
}

// Count newlines in text → total content height (scrollable area only)
static float computeTextContentH(const std::string& text, float fontSize,
                                  float topPad) {
    float lineH = fontSize * 1.4f;
    size_t lines = 1;
    for (char c : text) {
        if (c == '\n') lines++;
    }
    return topPad + lines * lineH + lineH;  // top padding + text + bottom padding
}

void YGuiOverlayImpl::buildGpuStatsDialog() {
    using namespace ygui;
    auto& t = defaultTheme();

    float dlgW = 600, dlgH = 500;
    float dlgX = 50, dlgY = 50;
    float headerH = t.rowHeight + t.padMedium;

    // Root panel (has built-in scrolling)
    auto panel = std::make_shared<Panel>();
    panel->id = "gpu-stats-dialog";
    panel->x = dlgX;
    panel->y = dlgY;
    panel->w = dlgW;
    panel->h = dlgH;
    panel->bgColor = 0xF0222233;
    panel->headerH = headerH;

    // Header bar
    auto header = std::make_shared<HBox>();
    header->id = "gpu-stats-header";
    header->x = 0;
    header->y = 0;
    header->w = dlgW;
    header->h = headerH;
    header->bgColor = t.bgHeader;

    auto title = std::make_shared<Label>();
    title->id = "gpu-stats-title";
    title->label = "GPU Memory Stats";
    title->stretch = 1;
    header->children.push_back(title);

    struct CloseIcon : public ygui::Widget {
        void render(ygui::RenderContext& ctx) override {
            auto& t = ctx.theme();
            uint32_t bg = isHover() ? 0xFF663333 : (isPressed() ? 0xFF882222 : 0xFF442222);
            ctx.box(x, y, w, h, bg, t.radiusMedium);
            float iconSz = std::min(w, h) * 0.25f;
            ctx.roundedX(x + w / 2, y + h / 2, iconSz, iconSz * 0.35f, fgColor, 1.0f);
        }
        std::optional<ygui::WidgetEvent> onPress(float lx, float ly) override {
            if (!onClick.empty()) return ygui::WidgetEvent{id, "press", "", ""};
            return {};
        }
        std::optional<ygui::WidgetEvent> onRelease(float lx, float ly) override {
            if (lx < 0 || lx >= w || ly < 0 || ly >= h || onClick.empty()) return {};
            return ygui::WidgetEvent{id, "click", "", ""};
        }
    };
    auto closeBtn = std::make_shared<CloseIcon>();
    closeBtn->id = "gpu-stats-close";
    closeBtn->w = t.rowHeight;
    closeBtn->onClick = "gpu-stats-close";
    closeBtn->bgColor = 0xFF442222;
    header->children.push_back(closeBtn);

    panel->children.push_back(header);

    // Content label
    _gpuStatsLabel = std::make_shared<Label>();
    _gpuStatsLabel->id = "gpu-stats-text";
    _gpuStatsLabel->x = t.padLarge;
    _gpuStatsLabel->y = headerH + t.padMedium;
    _gpuStatsLabel->w = dlgW - t.padLarge * 2;
    _gpuStatsLabel->selectable = true;
    if (_gpuStatsProvider)
        _gpuStatsLabel->label = _gpuStatsProvider();
    _gpuStatsLabel->h = dlgH - headerH;
    panel->children.push_back(_gpuStatsLabel);

    // Set contentH so Panel knows when to scroll
    panel->contentH = computeTextContentH(
        _gpuStatsLabel->label, t.fontSize, t.padMedium);

    _gpuStatsRoot = panel;
    _engine->addWidget(_gpuStatsRoot);
    _engine->markDirty();
}

void YGuiOverlayImpl::closeGpuStatsDialog() {
    if (_gpuStatsRoot) {
        _engine->removeWidget(_gpuStatsRoot);
        _gpuStatsRoot = nullptr;
        _gpuStatsLabel = nullptr;
    }
    _showGpuStats = false;
    _gpuStatsProvider = nullptr;
    _engine->markDirty();
}

void YGuiOverlayImpl::handleWidgetEvent(const ygui::WidgetEvent& event) {
    // Context menu item clicked
    if (event.eventType == "click" && event.widgetId.substr(0, 10) == "menu-item-") {
        size_t idx = 0;
        try { idx = std::stoul(event.widgetId.substr(10)); } catch (...) { return; }
        if (idx < _menuItems.size()) {
            auto item = _menuItems[idx];  // copy before clearing
            clearContextMenu();
            if (item.callback) {
                item.callback();
            } else {
                auto loopRes = base::EventLoop::instance();
                if (loopRes) (*loopRes)->dispatch(item.event);
            }
        }
        return;
    }
    // GPU stats close button
    if (event.eventType == "click" && event.widgetId == "gpu-stats-close") {
        closeGpuStatsDialog();
        return;
    }
}

float YGuiOverlayImpl::measureText(const std::string& text, float fontSize) {
    if (_builder)
        return _builder->measureTextWidth(text, fontSize);
    return text.size() * fontSize * 0.6f;
}

Result<void> YGuiOverlayImpl::render(WGPURenderPassEncoder pass) {
    if (!_pipeline || !_engine) return Ok();

    // Build statusbar on first render (needs display size)
    if (!_statusbarRoot && _displayWidth > 0) {
        buildStatusbar();
        updateDisplaySize(_displayWidth, _displayHeight);
    }

    // Lazy-build context menu widget tree
    if (_menuOpen && !_contextMenuRoot && !_menuItems.empty()) {
        buildContextMenu();
    }

    // Update GPU stats text every frame; request re-render so updates
    // are visible even at low FPS (e.g. 1 FPS idle mode)
    if (_showGpuStats && _gpuStatsProvider && _gpuStatsLabel) {
        std::string newText = _gpuStatsProvider();
        if (newText != _gpuStatsLabel->label) {
            _gpuStatsLabel->label = newText;
            // Update panel contentH for scrolling
            if (auto* panel = dynamic_cast<ygui::Panel*>(_gpuStatsRoot.get())) {
                auto& t = ygui::defaultTheme();
                panel->contentH = computeTextContentH(
                    newText, t.fontSize, t.padMedium);
            }
            _engine->markDirty();
            if (_ctx.platform) _ctx.platform->requestRender();
        }
    }

    // Rebuild if dirty
    if (_engine->isDirty()) {
        _engine->rebuild();
        // Keep builder scene bounds in sync (handles resize)
        _builder->setSceneBounds(0, 0,
            static_cast<float>(_displayWidth),
            static_cast<float>(_displayHeight));
        _builder->calculate();
        rebuildBuffers();
    }

    if (!_bindGroup) return Ok();

    // Reset viewport to full screen — GPUScreen may have set a smaller viewport
    wgpuRenderPassEncoderSetViewport(pass, 0, 0,
        static_cast<float>(_displayWidth), static_cast<float>(_displayHeight),
        0.0f, 1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass, 0, 0, _displayWidth, _displayHeight);

    wgpuRenderPassEncoderSetPipeline(pass, _pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 3, 1, 0, 0);

    return Ok();
}

void YGuiOverlayImpl::registerForEvents() {
    auto loopRes = base::EventLoop::instance();
    if (!loopRes) return;
    auto loop = *loopRes;
    auto self = sharedAs<base::EventListener>();
    // Priority 100: must see events before GPUScreen (like imgui)
    loop->registerListener(base::Event::Type::MouseDown, self, 100);
    loop->registerListener(base::Event::Type::MouseUp, self, 100);
    loop->registerListener(base::Event::Type::MouseMove, self, 100);
    loop->registerListener(base::Event::Type::Scroll, self, 100);
    loop->registerListener(base::Event::Type::KeyDown, self, 100);
}

Result<bool> YGuiOverlayImpl::onEvent(const base::Event& event) {
    if (!_engine) return Ok(false);

    auto consume = [&]() -> Result<bool> {
        if (_ctx.platform) _ctx.platform->requestRender();
        return Ok(true);
    };

    // ESC closes context menu and/or GPU stats
    if (event.type == base::Event::Type::KeyDown) {
        if (event.key.key == KEY_ESCAPE && (_menuOpen || _showGpuStats)) {
            if (_menuOpen) clearContextMenu();
            if (_showGpuStats) closeGpuStatsDialog();
            return consume();
        }
    }

    if (event.type == base::Event::Type::MouseMove) {
        _engine->handleMouseMove(event.mouse.x, event.mouse.y);
        // Consume mouse events when popup/dialog is open to prevent GPUScreen processing
        if (_menuOpen || _showGpuStats)
            return consume();
    } else if (event.type == base::Event::Type::MouseDown) {
        float wx = event.mouse.x;
        float wy = event.mouse.y;

        // Check if click is on overlay widgets
        auto hit = _engine->widgetAt(wx, wy);
        if (hit) {
            _engine->handleMouseDown(wx, wy, event.mouse.button);
            return consume();
        }

        // Click outside context menu → dismiss
        if (_menuOpen) {
            clearContextMenu();
            return consume();
        }
    } else if (event.type == base::Event::Type::MouseUp) {
        _engine->handleMouseUp(event.mouse.x, event.mouse.y, event.mouse.button);
        if (_menuOpen || _showGpuStats)
            return consume();
    } else if (event.type == base::Event::Type::Scroll) {
        auto hit = _engine->widgetAt(event.scroll.x, event.scroll.y);
        if (hit) {
            _engine->handleMouseScroll(event.scroll.x, event.scroll.y,
                                       event.scroll.dx, event.scroll.dy);
            return consume();
        }
    }

    return Ok(false);
}

//=============================================================================
// Factory
//=============================================================================

Result<YGuiOverlay::Ptr>
YGuiOverlay::createImpl(ContextType& /*ctx*/, const YettyContext& yettyCtx) noexcept {
    auto impl = Ptr(new YGuiOverlayImpl(yettyCtx));
    if (auto res = static_cast<YGuiOverlayImpl*>(impl.get())->init(); !res)
        return Err<Ptr>("YGuiOverlay init failed", res);
    static_cast<YGuiOverlayImpl*>(impl.get())->registerForEvents();
    return Ok(std::move(impl));
}

} // namespace yetty
