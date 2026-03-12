/**
 * Card Runner - Standalone interactive card renderer
 *
 * Renders a single card without the terminal overhead.
 * Useful for fast iteration when developing card shaders.
 *
 * Usage: card-runner <card-name> "<args>"
 * Example: card-runner yplot "{f=sin(x) @range=-pi..pi,-1..1 @grid=on}"
 */

#include <args.hxx>
#include <yetty/wgpu-compat.h>
#include <yetty/yetty-context.h>
#include <yetty/card-factory.h>
#include <yetty/gpu-memory-manager.h>
#include <yetty/shader-manager.h>
#include <yetty/font-manager.h>
#include <yetty/gpu-allocator.h>
#include <yetty/config.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <chrono>
#include <cstring>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

namespace yetty {

// Shared uniforms struct (matches gpu-screen.wgsl)
struct SharedUniforms {
    float time;
    float deltaTime;
    float screenWidth;
    float screenHeight;
    float mouseX;
    float mouseY;
    uint32_t lastChar;
    float lastCharTime;
};

class CardRunner {
public:
    struct Options {
        std::string cardType;
        std::string cardArgs;
        int windowWidth = 800;
        int windowHeight = 600;
        uint32_t cellsW = 1;  // Use 1x1 cell so widgetUV = localUV (fullscreen rendering)
        uint32_t cellsH = 1;
    };

    static Result<std::unique_ptr<CardRunner>> create(const Options& opts) {
        auto runner = std::make_unique<CardRunner>(opts);
        if (auto res = runner->init(); !res) {
            return Err<std::unique_ptr<CardRunner>>("CardRunner init failed", res);
        }
        return Ok(std::move(runner));
    }

    explicit CardRunner(const Options& opts) : _opts(opts) {}

    ~CardRunner() {
        cleanup();
    }

    Result<void> run() {
        yinfo("Card Runner: entering render loop");

        auto startTime = std::chrono::high_resolution_clock::now();
        float lastTime = 0.0f;

        while (!glfwWindowShouldClose(_window)) {
            glfwPollEvents();

            auto now = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float>(now - startTime).count();
            float deltaTime = time - lastTime;
            lastTime = time;

            if (auto res = renderFrame(time, deltaTime); !res) {
                return res;
            }
        }

        return Ok();
    }

private:
    Result<void> init() {
        ydebug("initGLFW...");
        if (auto res = initGLFW(); !res) { yerror("initGLFW FAILED: {}", error_msg(res)); return res; }
        ydebug("initWebGPU...");
        if (auto res = initWebGPU(); !res) { yerror("initWebGPU FAILED: {}", error_msg(res)); return res; }
        ydebug("initYettyContext...");
        if (auto res = initYettyContext(); !res) { yerror("initYettyContext FAILED: {}", error_msg(res)); return res; }
        ydebug("initCard...");
        if (auto res = initCard(); !res) { yerror("initCard FAILED: {}", error_msg(res)); return res; }
        ydebug("initPipeline...");
        if (auto res = initPipeline(); !res) { yerror("initPipeline FAILED: {}", error_msg(res)); return res; }
        ydebug("init complete");
        return Ok();
    }

    Result<void> initGLFW() {
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        _window = glfwCreateWindow(_opts.windowWidth, _opts.windowHeight,
                                   ("Card Runner: " + _opts.cardType).c_str(),
                                   nullptr, nullptr);
        if (!_window) {
            glfwTerminate();
            return Err<void>("Failed to create GLFW window");
        }

        return Ok();
    }

    Result<void> initWebGPU() {
        WGPUInstanceDescriptor instanceDesc = {};
        _instance = wgpuCreateInstance(&instanceDesc);
        if (!_instance) {
            return Err<void>("Failed to create WebGPU instance");
        }

        _surface = glfwCreateWindowWGPUSurface(_instance, _window);
        if (!_surface) {
            return Err<void>("Failed to create WebGPU surface");
        }

        // Request adapter
        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = _surface;
        adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

        WGPURequestAdapterCallbackInfo adapterCb = {};
        adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
        adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                WGPUStringView, void* userdata1, void*) {
            if (status == WGPURequestAdapterStatus_Success) {
                *static_cast<WGPUAdapter*>(userdata1) = adapter;
            }
        };
        adapterCb.userdata1 = &_adapter;
        wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);

        while (!_adapter) {
            wgpuInstanceProcessEvents(_instance);
        }
        yinfo("Card Runner: adapter obtained");

        // Request device
        WGPUDeviceDescriptor deviceDesc = {};
        deviceDesc.label = WGPU_STR("card-runner device");
        deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const*, WGPUErrorType type,
                                                              WGPUStringView message, void*, void*) {
            std::cerr << "WebGPU error (" << static_cast<int>(type) << "): "
                      << (message.data ? std::string(message.data, message.length) : "unknown") << "\n";
        };

        WGPURequestDeviceCallbackInfo deviceCb = {};
        deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
        deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                               WGPUStringView, void* userdata1, void*) {
            if (status == WGPURequestDeviceStatus_Success) {
                *static_cast<WGPUDevice*>(userdata1) = device;
            }
        };
        deviceCb.userdata1 = &_device;
        wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCb);

        while (!_device) {
            wgpuInstanceProcessEvents(_instance);
        }
        yinfo("Card Runner: device obtained");

        _queue = wgpuDeviceGetQueue(_device);

        // Configure surface
        WGPUSurfaceCapabilities caps = {};
        wgpuSurfaceGetCapabilities(_surface, _adapter, &caps);
        if (caps.formatCount > 0) {
            _surfaceFormat = caps.formats[0];
        }

        _surfaceConfig.device = _device;
        _surfaceConfig.format = _surfaceFormat;
        _surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
        _surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
        _surfaceConfig.width = static_cast<uint32_t>(_opts.windowWidth);
        _surfaceConfig.height = static_cast<uint32_t>(_opts.windowHeight);
        _surfaceConfig.presentMode = WGPUPresentMode_Fifo;
        wgpuSurfaceConfigure(_surface, &_surfaceConfig);

        // Create uniform buffer
        WGPUBufferDescriptor uniformBufDesc = {};
        uniformBufDesc.size = sizeof(SharedUniforms);
        uniformBufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        _uniformBuffer = wgpuDeviceCreateBuffer(_device, &uniformBufDesc);

        return Ok();
    }

    Result<void> initYettyContext() {
        _ctx.gpu.device = _device;
        _ctx.gpu.queue = _queue;
        _ctx.gpu.surfaceFormat = _surfaceFormat;
        _ctx.gpu.sharedUniformBuffer = _uniformBuffer;
        _ctx.gpu.sharedUniformSize = sizeof(SharedUniforms);

        _ctx.gpuAllocator = std::make_shared<GpuAllocator>(_device);

        // Create shared bind group layout (required by GpuMemoryManager)
        if (auto res = createSharedBindGroupLayout(); !res) {
            return Err<void>("Failed to create shared bind group layout", res);
        }

        auto shaderMgrResult = ShaderManager::create(_ctx.gpu, _ctx.gpuAllocator);
        if (!shaderMgrResult) {
            return Err<void>("Failed to create ShaderManager", shaderMgrResult);
        }
        _ctx.shaderManager = *shaderMgrResult;

        auto fontMgrResult = FontManager::create(_ctx.gpu, _ctx.gpuAllocator, _ctx.shaderManager, nullptr);
        if (!fontMgrResult) {
            return Err<void>("Failed to create FontManager", fontMgrResult);
        }
        _ctx.fontManager = *fontMgrResult;

        auto configResult = Config::create();
        if (configResult) {
            _ctx.config = *configResult;
        }

        auto cardMgrResult = GpuMemoryManager::create(&_ctx.gpu, _ctx.gpuAllocator,
                                                  _uniformBuffer, sizeof(SharedUniforms));
        if (!cardMgrResult) {
            return Err<void>("Failed to create GpuMemoryManager", cardMgrResult);
        }
        _ctx.cardManager = *cardMgrResult;

        auto factoryResult = CardFactory::create(_ctx.gpu);
        if (!factoryResult) {
            return Err<void>("Failed to create CardFactory", factoryResult);
        }
        _ctx.cardFactory = *factoryResult;

        return Ok();
    }

    Result<void> createSharedBindGroupLayout() {
        // 5 entries: uniform, metadata storage, data storage, texture, sampler
        // This matches the GpuMemoryManager's shared bind group layout
        std::array<WGPUBindGroupLayoutEntry, 5> layoutEntries = {};

        layoutEntries[0].binding = 0;
        layoutEntries[0].visibility = WGPUShaderStage_Fragment;
        layoutEntries[0].buffer.type = WGPUBufferBindingType_Uniform;
        layoutEntries[0].buffer.minBindingSize = sizeof(SharedUniforms);

        layoutEntries[1].binding = 1;
        layoutEntries[1].visibility = WGPUShaderStage_Fragment;
        layoutEntries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

        layoutEntries[2].binding = 2;
        layoutEntries[2].visibility = WGPUShaderStage_Fragment;
        layoutEntries[2].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

        layoutEntries[3].binding = 3;
        layoutEntries[3].visibility = WGPUShaderStage_Fragment;
        layoutEntries[3].texture.sampleType = WGPUTextureSampleType_Float;
        layoutEntries[3].texture.viewDimension = WGPUTextureViewDimension_2D;

        layoutEntries[4].binding = 4;
        layoutEntries[4].visibility = WGPUShaderStage_Fragment;
        layoutEntries[4].sampler.type = WGPUSamplerBindingType_Filtering;

        WGPUBindGroupLayoutDescriptor layoutDesc = {};
        layoutDesc.label = WGPU_STR("Shared Bind Group Layout");
        layoutDesc.entryCount = layoutEntries.size();
        layoutDesc.entries = layoutEntries.data();
        _sharedBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &layoutDesc);

        // Create dummy resources for fallback bind group
        WGPUBufferDescriptor dummyBufDesc = {};
        dummyBufDesc.label = WGPU_STR("dummy storage");
        dummyBufDesc.size = 4;
        dummyBufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
        WGPUBuffer dummyBuffer = _ctx.gpuAllocator->createBuffer(dummyBufDesc);

        WGPUTextureDescriptor dummyTexDesc = {};
        dummyTexDesc.label = WGPU_STR("dummy texture");
        dummyTexDesc.size = {1, 1, 1};
        dummyTexDesc.format = WGPUTextureFormat_RGBA8Unorm;
        dummyTexDesc.usage = WGPUTextureUsage_TextureBinding;
        dummyTexDesc.dimension = WGPUTextureDimension_2D;
        dummyTexDesc.mipLevelCount = 1;
        dummyTexDesc.sampleCount = 1;
        WGPUTexture dummyTexture = _ctx.gpuAllocator->createTexture(dummyTexDesc);

        WGPUTextureViewDescriptor dummyViewDesc = {};
        dummyViewDesc.format = WGPUTextureFormat_RGBA8Unorm;
        dummyViewDesc.dimension = WGPUTextureViewDimension_2D;
        dummyViewDesc.mipLevelCount = 1;
        dummyViewDesc.arrayLayerCount = 1;
        WGPUTextureView dummyView = wgpuTextureCreateView(dummyTexture, &dummyViewDesc);

        WGPUSamplerDescriptor dummySamplerDesc = {};
        dummySamplerDesc.magFilter = WGPUFilterMode_Linear;
        dummySamplerDesc.minFilter = WGPUFilterMode_Linear;
        dummySamplerDesc.maxAnisotropy = 1;
        _dummySampler = wgpuDeviceCreateSampler(_device, &dummySamplerDesc);

        std::array<WGPUBindGroupEntry, 5> bindEntries = {};
        bindEntries[0].binding = 0;
        bindEntries[0].buffer = _uniformBuffer;
        bindEntries[0].size = sizeof(SharedUniforms);
        bindEntries[1].binding = 1;
        bindEntries[1].buffer = dummyBuffer;
        bindEntries[1].size = 4;
        bindEntries[2].binding = 2;
        bindEntries[2].buffer = dummyBuffer;
        bindEntries[2].size = 4;
        bindEntries[3].binding = 3;
        bindEntries[3].textureView = dummyView;
        bindEntries[4].binding = 4;
        bindEntries[4].sampler = _dummySampler;

        WGPUBindGroupDescriptor bindDesc = {};
        bindDesc.label = WGPU_STR("Fallback Shared Bind Group");
        bindDesc.layout = _sharedBindGroupLayout;
        bindDesc.entryCount = bindEntries.size();
        bindDesc.entries = bindEntries.data();
        _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);

        // Release dummy resources (bind group retains references)
        wgpuTextureViewRelease(dummyView);
        wgpuTextureRelease(dummyTexture);
        wgpuBufferRelease(dummyBuffer);

        _ctx.gpu.sharedBindGroupLayout = _sharedBindGroupLayout;
        _ctx.gpu.sharedBindGroup = _sharedBindGroup;

        return Ok();
    }

    Result<void> initCard() {
        auto cardResult = _ctx.cardFactory->createCard(
            _ctx, _opts.cardType, 0, 0, _opts.cellsW, _opts.cellsH, _opts.cardArgs, "");

        if (!cardResult) {
            return Err<void>("Failed to create card", cardResult);
        }
        _card = *cardResult;
        yinfo("Card Runner: created card '{}' with glyph 0x{:X}", _opts.cardType, _card->shaderGlyph());

        // Initialize card buffers
        _card->renderToStaging(0.0f);
        if (_card->needsBuffer()) {
            _card->declareBufferNeeds();
            if (auto res = _ctx.cardManager->bufferManager()->commitReservations(); !res) {
                return Err<void>("Failed to commit buffer reservations", res);
            }
            if (auto res = _card->allocateBuffers(); !res) {
                return Err<void>("Failed to allocate buffers", res);
            }
        }
        _card->finalize();
        _ctx.cardManager->flush(_queue);

        // Debug: print buffer info
        WGPUBuffer metaBuf = _ctx.cardManager->metadataBuffer();
        WGPUBuffer storageBuf = _ctx.cardManager->bufferManager()->bufferGpu();
        ydebug("initCard: metaBuf={} size={} storageBuf={} size={}",
               (void*)metaBuf, wgpuBufferGetSize(metaBuf),
               (void*)storageBuf, wgpuBufferGetSize(storageBuf));

        return Ok();
    }

    Result<void> initPipeline() {
        uint32_t slotIndex = _card->metadataSlotIndex();
        yinfo("Card Runner: card at slotIndex={}", slotIndex);

        std::string shaderCode = buildCardShader(_card->shaderGlyph(), slotIndex);
        if (shaderCode.empty()) {
            return Err<void>("Failed to build card shader");
        }

        // Debug output
        {
            std::ofstream debug("/tmp/card-runner-shader.wgsl");
            debug << shaderCode;
            yinfo("Card Runner: shader saved to /tmp/card-runner-shader.wgsl");
        }

        // Use GpuMemoryManager's shared bind group layout (like gpu-screen.cpp does)
        WGPUBindGroupLayout sharedLayout = _ctx.cardManager->sharedBindGroupLayout();
        if (!sharedLayout) {
            return Err<void>("No shared bind group layout from GpuMemoryManager");
        }

        // Create pipeline using the shared layout
        WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
        pipelineLayoutDesc.bindGroupLayoutCount = 1;
        pipelineLayoutDesc.bindGroupLayouts = &sharedLayout;
        WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &pipelineLayoutDesc);

        WGPUShaderSourceWGSL wgslSource = {};
        wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
        wgslSource.code = {shaderCode.c_str(), shaderCode.size()};

        WGPUShaderModuleDescriptor shaderDesc = {};
        shaderDesc.nextInChain = &wgslSource.chain;
        WGPUShaderModule shader = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
        if (!shader) {
            return Err<void>("Failed to create shader module");
        }

        WGPUColorTargetState colorTarget = {};
        colorTarget.format = _surfaceFormat;
        colorTarget.writeMask = WGPUColorWriteMask_All;

        WGPUFragmentState fragState = {};
        fragState.module = shader;
        fragState.entryPoint = {"fs_main", 7};
        fragState.targetCount = 1;
        fragState.targets = &colorTarget;

        WGPURenderPipelineDescriptor pipelineDesc = {};
        pipelineDesc.layout = pipelineLayout;
        pipelineDesc.vertex.module = shader;
        pipelineDesc.vertex.entryPoint = {"vs_main", 7};
        pipelineDesc.fragment = &fragState;
        pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
        pipelineDesc.multisample.count = 1;
        pipelineDesc.multisample.mask = 0xFFFFFFFF;

        _pipeline = wgpuDeviceCreateRenderPipeline(_device, &pipelineDesc);
        wgpuShaderModuleRelease(shader);
        wgpuPipelineLayoutRelease(pipelineLayout);

        if (!_pipeline) {
            return Err<void>("Failed to create render pipeline");
        }

        yinfo("Card Runner: pipeline created");
        return Ok();
    }

    Result<void> renderFrame(float time, float deltaTime) {
        // Handle resize
        int w, h;
        glfwGetFramebufferSize(_window, &w, &h);
        if (w != _opts.windowWidth || h != _opts.windowHeight) {
            _opts.windowWidth = w;
            _opts.windowHeight = h;
            _surfaceConfig.width = static_cast<uint32_t>(w);
            _surfaceConfig.height = static_cast<uint32_t>(h);
            wgpuSurfaceConfigure(_surface, &_surfaceConfig);
        }

        // Update card - GpuMemoryManager's flush() handles bind group updates
        _card->renderToStaging(time);
        if (_card->needsBuffer() && _card->needsBufferRealloc()) {
            _card->declareBufferNeeds();
            _ctx.cardManager->bufferManager()->commitReservations();
            _card->allocateBuffers();
        }
        _card->finalize();
        _ctx.cardManager->flush(_queue);

        // Update uniforms
        double mx, my;
        glfwGetCursorPos(_window, &mx, &my);

        SharedUniforms uniforms = {};
        uniforms.time = time;
        uniforms.deltaTime = deltaTime;
        uniforms.screenWidth = static_cast<float>(_opts.windowWidth);
        uniforms.screenHeight = static_cast<float>(_opts.windowHeight);
        uniforms.mouseX = static_cast<float>(mx);
        uniforms.mouseY = static_cast<float>(my);
        wgpuQueueWriteBuffer(_queue, _uniformBuffer, 0, &uniforms, sizeof(uniforms));

        // Get surface texture
        WGPUSurfaceTexture surfaceTexture;
        wgpuSurfaceGetCurrentTexture(_surface, &surfaceTexture);
        if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
            surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
            return Ok();
        }

        WGPUTextureView textureView = wgpuTextureCreateView(surfaceTexture.texture, nullptr);

        WGPUCommandEncoderDescriptor encDesc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

        WGPURenderPassColorAttachment colorAttachment = {};
        colorAttachment.view = textureView;
        colorAttachment.loadOp = WGPULoadOp_Clear;
        colorAttachment.storeOp = WGPUStoreOp_Store;
        colorAttachment.clearValue = {1.0, 0.0, 1.0, 1.0};  // Magenta - obvious if no rendering
        colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

        WGPURenderPassDescriptor passDesc = {};
        passDesc.colorAttachmentCount = 1;
        passDesc.colorAttachments = &colorAttachment;

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
        wgpuRenderPassEncoderSetPipeline(pass, _pipeline);
        // Use GpuMemoryManager's shared bind group (like gpu-screen.cpp does)
        WGPUBindGroup bindGroup = _ctx.cardManager->sharedBindGroup();
        wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
        wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);

        WGPUCommandBufferDescriptor cmdBufDesc = {};
        WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
        wgpuQueueSubmit(_queue, 1, &cmdBuf);
        wgpuSurfacePresent(_surface);

        wgpuCommandBufferRelease(cmdBuf);
        wgpuCommandEncoderRelease(encoder);
        wgpuTextureViewRelease(textureView);
        wgpuTextureRelease(surfaceTexture.texture);

        return Ok();
    }

    std::string buildCardShader(uint32_t shaderGlyph, uint32_t slotIndex) {
        std::string shaderDir = YETTY_SHADERS_DIR;

        // Load all shader libraries
        std::string utilLib = loadShaderFile(shaderDir + "/lib/util.wgsl");
        std::string distfunctionsLib = loadShaderFile(shaderDir + "/lib/distfunctions.wgsl");
        std::string sdfTypesLib = loadShaderFile(shaderDir + "/lib/sdf-types.gen.wgsl");
        std::string yfsvmLib = loadShaderFile(shaderDir + "/lib/yfsvm.wgsl");

        // Find the card shader based on glyph
        uint16_t glyphLow = shaderGlyph & 0xFFFF;
        std::string cardShader;

        for (const auto& entry : std::filesystem::directory_iterator(shaderDir + "/cards")) {
            std::string filename = entry.path().filename().string();
            if (filename.size() > 6 && filename[0] == '0' && filename[1] == 'x' &&
                filename.find(".wgsl") != std::string::npos) {
                try {
                    uint32_t fileGlyph = std::stoul(filename.substr(2, 4), nullptr, 16);
                    if (fileGlyph == glyphLow) {
                        cardShader = loadShaderFile(entry.path().string());
                        yinfo("Card Runner: loaded shader {}", filename);
                        break;
                    }
                } catch (...) { continue; }
            }
        }

        if (cardShader.empty()) {
            yerror("Card Runner: no shader found for glyph 0x{:X}", shaderGlyph);
            return "";
        }

        // Build the complete shader with all bindings cards might need
        std::stringstream combined;
        combined << R"(
// === SHARED UNIFORMS (group 0) ===
struct SharedUniforms {
    time: f32,
    deltaTime: f32,
    screenWidth: f32,
    screenHeight: f32,
    mouseX: f32,
    mouseY: f32,
    lastChar: u32,
    lastCharTime: f32,
}

// Grid uniforms - cards like ydraw use grid.cellSize for aspect ratio calculations
struct GridUniforms {
    projection: mat4x4<f32>,
    screenSize: vec2<f32>,
    cellSize: vec2<f32>,
    gridSize: vec2<f32>,
    pixelRange: f32,
    scale: f32,
    cursorPos: vec2<f32>,
    cursorVisible: f32,
    cursorShape: f32,
    viewportOrigin: vec2<f32>,
    cursorBlink: f32,
    hasSelection: f32,
    selStart: vec2<f32>,
    selEnd: vec2<f32>,
    preEffectIndex: u32,
    postEffectIndex: u32,
    preEffectP0: f32,
    preEffectP1: f32,
    preEffectP2: f32,
    preEffectP3: f32,
    preEffectP4: f32,
    preEffectP5: f32,
    postEffectP0: f32,
    postEffectP1: f32,
    postEffectP2: f32,
    postEffectP3: f32,
    postEffectP4: f32,
    postEffectP5: f32,
    defaultFg: u32,
    defaultBg: u32,
    spaceGlyph: u32,
    effectIndex: u32,
    effectP0: f32,
    effectP1: f32,
    effectP2: f32,
    effectP3: f32,
    effectP4: f32,
    effectP5: f32,
    visualZoomScale: f32,
    visualZoomOffsetX: f32,
    visualZoomOffsetY: f32,
    _pad0: f32,
}

// Glyph metadata for MSDF text rendering
struct GlyphMetadata {
    uvMin: vec2<f32>,
    uvMax: vec2<f32>,
    size: vec2<f32>,
    bearing: vec2<f32>,
    advance: f32,
    _pad: f32,
}

// Emoji glyph metadata
struct EmojiGlyphMetadata {
    uvMin: vec2<f32>,
    uvMax: vec2<f32>,
    size: vec2<f32>,
    _pad: vec2<f32>,
}

// Shared bindings (group 0) - matches GpuMemoryManager's layout
@group(0) @binding(0) var<uniform> globals: SharedUniforms;
@group(0) @binding(1) var<storage, read> cardMetadata: array<u32>;
@group(0) @binding(2) var<storage, read> cardStorage: array<f32>;
@group(0) @binding(3) var cardImageAtlas: texture_2d<f32>;
@group(0) @binding(4) var cardImageSampler: sampler;

// Grid uniforms for card-runner (computed from screen size)
var<private> grid: GridUniforms;

// === GLYPH RANGE CONSTANTS ===
const CARD_GLYPH_BASE: u32 = 0x100000u;
const CARD_GLYPH_END: u32 = 0x100FFFu;
const SHADER_GLYPH_BASE: u32 = 0x101000u;
const SHADER_GLYPH_END: u32 = 0x10FFFDu;

// === GLYPH TYPE DETECTION ===
fn isCardGlyph(codepoint: u32) -> bool {
    return codepoint >= CARD_GLYPH_BASE && codepoint <= CARD_GLYPH_END;
}

fn isShaderGlyph(codepoint: u32) -> bool {
    return codepoint >= SHADER_GLYPH_BASE && codepoint <= SHADER_GLYPH_END;
}

// === MSDF TEXT RENDERING STUBS (for cards that need text) ===
// Note: card-runner doesn't have full font support yet, these are stubs
fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

struct GlyphRenderResult {
    color: vec3<f32>,
    alpha: f32,
}

// Stub: renders nothing (cards using MSDF text need full font binding support)
fn renderMsdfGlyph(
    localUV: vec2<f32>,
    glyphIndex: u32,
    fgColor: vec3<f32>,
    bgColor: vec3<f32>,
    pixelRange: f32,
    screenScale: f32
) -> GlyphRenderResult {
    var result: GlyphRenderResult;
    result.color = bgColor;
    result.alpha = 0.0;
    // TODO: implement full MSDF glyph rendering with font texture binding
    return result;
}

// === UTILITY FUNCTIONS ===
)" << utilLib << R"(

// === SDF TYPES ===
)" << sdfTypesLib << R"(

// === SDF DISTANCE FUNCTIONS ===
)" << distfunctionsLib << R"(

// === YFSVM BYTECODE INTERPRETER ===
)" << yfsvmLib << R"(

// === COLOR HELPERS (used by cards that don't define their own) ===
)";
        // Only include unpackColor if the card shader doesn't define its own
        if (cardShader.find("fn unpackColor") == std::string::npos) {
            combined << R"(
fn unpackColor(packed: u32) -> vec3<f32> {
    return vec3<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0
    );
}

fn unpackColorAlpha(packed: u32) -> vec4<f32> {
    return vec4<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0,
        f32((packed >> 24u) & 0xFFu) / 255.0
    );
}
)";
        }

        combined << R"(
// === CARD SHADER ===
)" << cardShader << R"(

// === VERTEX SHADER ===
@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> @builtin(position) vec4f {
    var pos = array<vec2f, 6>(vec2f(-1,-1), vec2f(1,-1), vec2f(1,1), vec2f(-1,-1), vec2f(1,1), vec2f(-1,1));
    return vec4f(pos[idx], 0.0, 1.0);
}

// === FRAGMENT SHADER ===
@fragment
fn fs_main(@builtin(position) fragCoord: vec4f) -> @location(0) vec4f {
    // Initialize grid uniforms for cards that use them
    grid.screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    grid.cellSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    grid.gridSize = vec2<f32>(1.0, 1.0);
    grid.pixelRange = 4.0;
    grid.scale = 1.0;

    // Compute localUV (0-1 range, Y flipped)
    let localUV = vec2<f32>(
        fragCoord.x / globals.screenWidth,
        1.0 - fragCoord.y / globals.screenHeight
    );

    let mousePos = vec2<f32>(globals.mouseX, globals.mouseY);
    let fg = )" << slotIndex << R"(u;
    let bg = 0u;

    let color = shaderGlyph_)" << shaderGlyph << R"((
        localUV,
        globals.time,
        fg,
        bg,
        fragCoord.xy,
        mousePos,
        globals.lastChar,
        globals.lastCharTime
    );

    return vec4f(color, 1.0);
}
)";
        return combined.str();
    }

    static std::string loadShaderFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            yerror("Failed to open shader file: {}", path);
            return "";
        }
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    void cleanup() {
        if (_card) { _card->dispose(); _card.reset(); }
        if (_pipeline) wgpuRenderPipelineRelease(_pipeline);

        _ctx.cardManager.reset();
        _ctx.cardFactory.reset();
        _ctx.fontManager.reset();
        _ctx.shaderManager.reset();
        _ctx.gpuAllocator.reset();

        if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);
        if (_sharedBindGroupLayout) wgpuBindGroupLayoutRelease(_sharedBindGroupLayout);
        if (_dummySampler) wgpuSamplerRelease(_dummySampler);
        if (_uniformBuffer) wgpuBufferRelease(_uniformBuffer);
        if (_queue) wgpuQueueRelease(_queue);
        if (_device) wgpuDeviceRelease(_device);
        if (_adapter) wgpuAdapterRelease(_adapter);
        if (_surface) wgpuSurfaceRelease(_surface);
        if (_instance) wgpuInstanceRelease(_instance);
        if (_window) { glfwDestroyWindow(_window); glfwTerminate(); }

        yinfo("Card Runner: cleanup complete");
    }

    Options _opts;
    GLFWwindow* _window = nullptr;
    WGPUInstance _instance = nullptr;
    WGPUSurface _surface = nullptr;
    WGPUAdapter _adapter = nullptr;
    WGPUDevice _device = nullptr;
    WGPUQueue _queue = nullptr;
    WGPUTextureFormat _surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
    WGPUSurfaceConfiguration _surfaceConfig = {};
    WGPUBuffer _uniformBuffer = nullptr;
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
    WGPUSampler _dummySampler = nullptr;
    WGPURenderPipeline _pipeline = nullptr;
    YettyContext _ctx;
    CardPtr _card;
};

} // namespace yetty

int main(int argc, char** argv) {
    args::ArgumentParser parser("Card Runner - standalone card renderer");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Positional<std::string> cardName(parser, "card", "Card type (e.g. yplot, ydraw)");
    args::Positional<std::string> cardArgs(parser, "args", "Card arguments");
    args::ValueFlag<int> widthFlag(parser, "width", "Window width", {'W', "width"}, 800);
    args::ValueFlag<int> heightFlag(parser, "height", "Window height", {'H', "height"}, 600);
    args::ValueFlag<int> cellsW(parser, "cells-w", "Card width in cells", {'w'}, 1);
    args::ValueFlag<int> cellsH(parser, "cells-h", "Card height in cells", {'h'}, 1);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << "\n" << parser;
        return 1;
    }

    if (!cardName) {
        std::cerr << "Error: card type required\n" << parser;
        return 1;
    }

    // Enable all ytrace output for debugging and set spdlog to trace level
    spdlog::set_level(spdlog::level::trace);
    yenable_all();

    yetty::CardRunner::Options opts;
    opts.cardType = args::get(cardName);
    opts.cardArgs = cardArgs ? args::get(cardArgs) : "";
    opts.windowWidth = args::get(widthFlag);
    opts.windowHeight = args::get(heightFlag);
    opts.cellsW = static_cast<uint32_t>(args::get(cellsW));
    opts.cellsH = static_cast<uint32_t>(args::get(cellsH));

    yinfo("Card Runner: card='{}' args='{}' size={}x{} cells",
          opts.cardType, opts.cardArgs, opts.cellsW, opts.cellsH);

    auto runnerResult = yetty::CardRunner::create(opts);
    if (!runnerResult) {
        std::cerr << "Failed to create CardRunner: " << yetty::error_msg(runnerResult) << "\n";
        return 1;
    }

    if (auto res = (*runnerResult)->run(); !res) {
        std::cerr << "CardRunner error: " << yetty::error_msg(res) << "\n";
        return 1;
    }

    return 0;
}
