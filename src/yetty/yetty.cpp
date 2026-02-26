#include <yetty/yetty.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/yetty-context.h>
#include <yetty/terminal-view.h>
#include <yetty/workspace.h>
#include <yetty/font-manager.h>
#include <yetty/msdf-cdb-provider.h>
#include <yetty/shader-manager.h>
#include <yetty/card-factory.h>
#include <yetty/imgui-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/platform.h>
#include <yetty/base/base.h>
#include <yetty/rpc/rpc-server.h>
#include <yetty/rpc/event-loop-handler.h>
#include <yetty/rpc/stream-handler.h>
#include <yetty/rpc/socket-path.h>
#include "cards/plot/plot-sampler-provider.h"
#include "cards/plot/plot-transformer-provider.h"
#include "cards/plot/plot-renderer-provider.h"
#include <array>
#include <iostream>
#include <csignal>
#include <ytrace/ytrace.hpp>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

namespace yetty {

class YettyImpl : public Yetty, public base::EventListener {
public:
    YettyImpl() = default;
    ~YettyImpl() override = default;

    Result<void> init(int argc, char* argv[]) noexcept;
    Result<void> run() noexcept override;
    Result<bool> onEvent(const base::Event& event) override;

protected:
    Result<void> onShutdown() override;

private:
    Result<void> parseArgs(int argc, char* argv[]) noexcept;
    Result<void> initWindow() noexcept;
    Result<void> initWebGPU() noexcept;
    Result<void> initSharedResources() noexcept;
    Result<void> initWorkspace() noexcept;
    Result<void> initCallbacks() noexcept;

    Result<void> mainLoopIteration() noexcept;
    void handleResize(int width, int height) noexcept;
    void configureSurface(uint32_t width, uint32_t height) noexcept;
    Result<WGPUTextureView> getCurrentTextureView() noexcept;
    void present() noexcept;

#if !YETTY_WEB && !defined(__ANDROID__)
    void initEventLoop() noexcept;
    void shutdownEventLoop() noexcept;
#endif

    Result<Workspace::Ptr> createWorkspace() noexcept;

    // Platform abstraction (window, input, etc.)
    Platform::Ptr _platform;
    uint32_t _initialWidth = 1024;
    uint32_t _initialHeight = 768;

    // Content scale for mouse coordinate conversion (framebuffer / window)
    float _contentScaleX = 1.0f;
    float _contentScaleY = 1.0f;

    // Track mouse position for button events
    float _lastMouseX = 0.0f;
    float _lastMouseY = 0.0f;

    // Track modifier key state for scroll events
    bool _ctrlPressed = false;
    bool _shiftPressed = false;

    // WebGPU handles
    WGPUInstance _instance = nullptr;
    WGPUAdapter _adapter = nullptr;
    WGPUDevice _device = nullptr;
    WGPUQueue _queue = nullptr;
    WGPUSurface _surface = nullptr;
    WGPUTextureFormat _surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
    WGPUTextureView _currentTextureView = nullptr;
    WGPUTexture _currentTexture = nullptr;
    uint32_t _surfaceWidth = 0;
    uint32_t _surfaceHeight = 0;

    // Shared resources
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
    GpuAllocator::Ptr _gpuAllocator;
    WGPUBuffer _sharedUniformBuffer = nullptr;
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
    SharedUniforms _sharedUniforms = {};
    GPUContext _gpuContext = {};
    YettyContext _yettyContext = {};


    // Workspaces
    std::vector<Workspace::Ptr> _workspaces;
    Workspace::Ptr _activeWorkspace;

    // Grid dimensions

#if !YETTY_WEB && !defined(__ANDROID__)
    base::TimerId _frameTimerId = -1;   // Render timer (configurable FPS)
    base::TimerId _inputTimerId = -1;   // Input poll timer (always fast)
#endif

    // Fatal GPU error tracking
    bool _fatalGpuError = false;
    std::string _fatalGpuErrorMsg;

    // FPS tracking
    double _lastFpsTime = 0.0;
    uint32_t _frameCount = 0;

    // RPC server
    rpc::RpcServer::Ptr _rpcServer;

    // Command line options
    std::string _executeCommand;
    std::string _msdfProviderName = "cpu";  // "cpu" or "gpu"

    static YettyImpl* s_instance;
};

YettyImpl* YettyImpl::s_instance = nullptr;

//=============================================================================
// Factory
//=============================================================================

Result<Yetty::Ptr> Yetty::createImpl(ContextType&, int argc, char* argv[]) noexcept {
    auto impl = Ptr(new YettyImpl());
    if (auto res = static_cast<YettyImpl*>(impl.get())->init(argc, argv); !res) {
        return Err<Ptr>("Failed to init Yetty", res);
    }
    return Ok(std::move(impl));
}

#if defined(__ANDROID__)
Result<Yetty::Ptr> Yetty::createImpl(ContextType&, struct android_app* app) noexcept {
    (void)app;  // TODO: Store app pointer for platform initialization
    auto impl = Ptr(new YettyImpl());
    // Android init without argc/argv - use defaults
    if (auto res = static_cast<YettyImpl*>(impl.get())->init(0, nullptr); !res) {
        return Err<Ptr>("Failed to init Yetty", res);
    }
    return Ok(std::move(impl));
}
#endif

//=============================================================================
// Initialization
//=============================================================================

Result<void> YettyImpl::init(int argc, char* argv[]) noexcept {
    yinfo("Yetty starting...");

    if (auto res = parseArgs(argc, argv); !res) return res;

    // Create Config early (before anything that reads it)
    auto configResult = Config::create();
    if (!configResult) {
        return Err<void>("Failed to create Config", configResult);
    }
    _yettyContext.config = *configResult;

    if (auto res = initWindow(); !res) return res;
    if (auto res = initWebGPU(); !res) return res;
    if (auto res = initSharedResources(); !res) return res;

    // Create ShaderManager with GPUContext and allocator
    auto shaderMgrResult = ShaderManager::create(_gpuContext, _gpuAllocator);
    if (!shaderMgrResult) {
        return Err<void>("Failed to create ShaderManager", shaderMgrResult);
    }
    auto shaderMgr = *shaderMgrResult;

    // Create MSDF CDB provider based on CLI flag
    MsdfCdbProvider::Ptr cdbProvider;
#if !YETTY_WEB
    if (_msdfProviderName == "cpu") {
        cdbProvider = std::make_shared<CpuMsdfCdbProvider>();
        yinfo("Using CPU MSDF CDB provider");
    } else {
        cdbProvider = std::make_shared<GpuMsdfCdbProvider>(_instance);
        yinfo("Using GPU MSDF CDB provider");
    }
#endif

    // Create FontManager with GPUContext, ShaderManager, and CDB provider
    auto fontMgrResult = FontManager::create(_gpuContext, _gpuAllocator, shaderMgr, cdbProvider);
    if (!fontMgrResult) {
        return Err<void>("Failed to create FontManager", fontMgrResult);
    }
    auto fontMgr = *fontMgrResult;

    // Build YettyContext
    _yettyContext.gpu = _gpuContext;
    _yettyContext.gpuAllocator = _gpuAllocator;
    _yettyContext.platform = _platform;
#if !YETTY_WEB && !defined(__ANDROID__)
    _yettyContext.gpuMonitor = gpu::GpuMonitor::create();
#endif
    _yettyContext.shaderManager = shaderMgr;
    _yettyContext.fontManager = fontMgr;

    // Create ImguiManager
    auto imguiMgrResult = ImguiManager::create(_yettyContext);
    if (!imguiMgrResult) {
        return Err<void>("Failed to create ImguiManager", imguiMgrResult);
    }
    _yettyContext.imguiManager = *imguiMgrResult;
    _yettyContext.imguiManager->updateDisplaySize(_surfaceWidth, _surfaceHeight);

#if !defined(__ANDROID__)
    // Create CardFactory (card types registry, no CardBufferManager needed)
    {
        auto cardFactoryResult = CardFactory::create(_gpuContext);
        if (!cardFactoryResult) {
            return Err<void>("Failed to create CardFactory", cardFactoryResult);
        }
        _yettyContext.cardFactory = *cardFactoryResult;
    }

    // Register plot providers with ShaderManager
    card::PlotSamplerProvider::instance()->registerWith(shaderMgr);
    card::PlotTransformerProvider::instance()->registerWith(shaderMgr);
    card::PlotRendererProvider::instance()->registerWith(shaderMgr);
#endif

    // Compile shaders after all providers (fonts, plot) are registered
    if (auto res = shaderMgr->compile(); !res) {
        return Err<void>("Failed to compile shaders", res);
    }

#if !YETTY_WEB && !defined(__ANDROID__)
    initEventLoop();

    // Create RPC server and write socket path to config BEFORE workspace/terminal
    // (Terminal reads shell/env from config when forking the shell)
    {
        auto socketResult = rpc::createSocketPath();
        if (!socketResult) {
            return Err<void>("Failed to create RPC socket path", socketResult);
        }
        auto rpcResult = rpc::RpcServer::create(*socketResult);
        if (!rpcResult) {
            return Err<void>("Failed to create RPC server", rpcResult);
        }
        _rpcServer = *rpcResult;

        _yettyContext.config->setString("rpc/socket-path", _rpcServer->socketPath());
        _yettyContext.config->setString("shell/env/YETTY_SOCKET", _rpcServer->socketPath());

        rpc::registerEventLoopHandlers(*_rpcServer);
    }
#endif

    if (auto res = initWorkspace(); !res) return res;

#if !YETTY_WEB && !defined(__ANDROID__)
    // Register workspace handlers now that workspace exists
    rpc::registerWorkspaceHandlers(*_rpcServer, _activeWorkspace);

    // Register stream handlers (uses GPUScreenManager singleton)
    rpc::registerStreamHandlers(*_rpcServer);

    if (auto res = _rpcServer->start(); !res) {
        return Err<void>("Failed to start RPC server", res);
    }
    yinfo("RPC server started on {}", _rpcServer->socketPath());
#endif

    if (auto res = initCallbacks(); !res) return res;

    s_instance = this;
    _lastFpsTime = _platform->getTime();

    return Ok();
}

Result<void> YettyImpl::parseArgs(int argc, char* argv[]) noexcept {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-e" && i + 1 < argc) {
            _executeCommand = argv[++i];
            yinfo("Execute command: {}", _executeCommand);
        } else if (arg == "--msdf-provider" && i + 1 < argc) {
            _msdfProviderName = argv[++i];
            yinfo("MSDF provider: {}", _msdfProviderName);
        }
    }
    return Ok();
}

Result<void> YettyImpl::initWindow() noexcept {
    auto platformResult = Platform::create();
    if (!platformResult) {
        return Err<void>("Failed to create platform", platformResult);
    }
    _platform = *platformResult;

    if (auto res = _platform->createWindow(_initialWidth, _initialHeight, "yetty"); !res) {
        return Err<void>("Failed to create window", res);
    }

    return Ok();
}

Result<void> YettyImpl::initWebGPU() noexcept {
    yinfo("initWebGPU: Creating instance...");

    // Create instance
    WGPUInstanceDescriptor instanceDesc = {};
    _instance = wgpuCreateInstance(&instanceDesc);
    if (!_instance) {
        return Err<void>("Failed to create WebGPU instance");
    }
    yinfo("initWebGPU: Instance created");

    // Create surface via platform
    _surface = _platform->createWGPUSurface(_instance);
    if (!_surface) {
        return Err<void>("Failed to create WebGPU surface");
    }
    yinfo("initWebGPU: Surface created");

    // Request adapter
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = _surface;
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo adapterCallbackInfo = {};
    adapterCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCallbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                      WGPUStringView message, void* userdata1, void* userdata2) {
        if (status == WGPURequestAdapterStatus_Success) {
            *static_cast<WGPUAdapter*>(userdata1) = adapter;
        }
    };
    adapterCallbackInfo.userdata1 = &_adapter;

    yinfo("initWebGPU: Requesting adapter...");
    wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCallbackInfo);

#if defined(__EMSCRIPTEN__)
    // On Emscripten, WebGPU operations are async - poll until callback fires
    while (!_adapter) {
        emscripten_sleep(1);
    }
#endif

    if (!_adapter) {
        return Err<void>("Failed to get WebGPU adapter");
    }
    yinfo("initWebGPU: Adapter obtained");

    // Log adapter info
    {
        WGPUAdapterInfo info = {};
        if (wgpuAdapterGetInfo(_adapter, &info) == WGPUStatus_Success) {
            auto sv = [](WGPUStringView s) -> std::string {
                return (s.data && s.length > 0) ? std::string(s.data, s.length) : "(unknown)";
            };
            yinfo("GPU adapter: {} ({})", sv(info.device), sv(info.vendor));
            yinfo("GPU backend: {} | architecture: {} | description: {}",
                  static_cast<int>(info.backendType), sv(info.architecture), sv(info.description));
            yinfo("GPU vendor ID: 0x{:x} | device ID: 0x{:x} | type: {}",
                  info.vendorID, info.deviceID, static_cast<int>(info.adapterType));
            wgpuAdapterInfoFreeMembers(info);
        } else {
            ywarn("Failed to query adapter info");
        }
    }

    // Query adapter limits to avoid requesting more than supported
    WGPULimits adapterLimits = {};
    wgpuAdapterGetLimits(_adapter, &adapterLimits);
    uint64_t adapterMaxStorage = adapterLimits.maxStorageBufferBindingSize;
    yinfo("GPU adapter maxStorageBufferBindingSize: {} MB", adapterMaxStorage / (1024 * 1024));

    // Request device with limits capped to adapter support
    WGPULimits limits = {};
    limits.maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED;
    // Request higher texture size for large font atlases (CJK support needs >8192)
    limits.maxTextureDimension2D = std::min(16384u, adapterLimits.maxTextureDimension2D);
    limits.maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureArrayLayers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroups = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxStorageBuffersPerShaderStage = 10;  // Need 9 for grid shader (vector + coverage fonts)
    limits.maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED;
    limits.maxStorageBufferBindingSize = std::min(static_cast<uint64_t>(512 * 1024 * 1024), adapterMaxStorage);
    limits.minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBufferSize = std::min(static_cast<uint64_t>(1024) * 1024 * 1024, adapterLimits.maxBufferSize);
    limits.maxVertexAttributes = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBufferArrayStride = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxInterStageShaderVariables = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxColorAttachments = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxColorAttachmentBytesPerSample = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupStorageSize = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeInvocationsPerWorkgroup = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeX = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeY = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupSizeZ = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxComputeWorkgroupsPerDimension = WGPU_LIMIT_U32_UNDEFINED;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = WGPU_STR("yetty device");
    deviceDesc.requiredFeatureCount = 0;
    deviceDesc.requiredLimits = &limits;
    deviceDesc.defaultQueue.label = WGPU_STR("default queue");
    deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const* device, WGPUErrorType type,
                                                         WGPUStringView message, void* userdata1, void* userdata2) {
        auto* self = static_cast<YettyImpl*>(userdata1);
        auto msg = message.data ? std::string(message.data, message.length) : "unknown";
        yerror("WebGPU error ({}): {}", static_cast<int>(type), msg);
        self->_fatalGpuError = true;
        self->_fatalGpuErrorMsg = msg;
    };
    deviceDesc.uncapturedErrorCallbackInfo.userdata1 = this;

    std::string deviceError;
    WGPURequestDeviceCallbackInfo deviceCallbackInfo = {};
    deviceCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCallbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                                     WGPUStringView message, void* userdata1, void* userdata2) {
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(userdata1) = device;
        } else {
            auto msg = message.data ? std::string(message.data, message.length) : "unknown";
            *static_cast<std::string*>(userdata2) = msg;
        }
    };
    deviceCallbackInfo.userdata1 = &_device;
    deviceCallbackInfo.userdata2 = &deviceError;

    yinfo("initWebGPU: Requesting device...");
    wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCallbackInfo);

#if defined(__EMSCRIPTEN__)
    // On Emscripten, WebGPU operations are async - poll until callback fires
    while (!_device && deviceError.empty()) {
        emscripten_sleep(1);
    }
#endif

    if (!_device) {
        return Err<void>("Failed to get WebGPU device: " + deviceError);
    }
    yinfo("initWebGPU: Device obtained");

    _queue = wgpuDeviceGetQueue(_device);
    yinfo("initWebGPU: Queue obtained");

    // Configure surface
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(_surface, _adapter, &caps);
    if (caps.formatCount > 0) {
        _surfaceFormat = caps.formats[0];
    }
    wgpuSurfaceCapabilitiesFreeMembers(caps);

    configureSurface(_initialWidth, _initialHeight);

    yinfo("WebGPU initialized: device={} queue={}", (void*)_device, (void*)_queue);
    return Ok();
}

void YettyImpl::configureSurface(uint32_t width, uint32_t height) noexcept {
    // Release any held texture/view before reconfiguring - they become invalid
    if (_currentTextureView) {
        wgpuTextureViewRelease(_currentTextureView);
        _currentTextureView = nullptr;
    }
    if (_currentTexture) {
        wgpuTextureRelease(_currentTexture);
        _currentTexture = nullptr;
    }

    _surfaceWidth = width;
    _surfaceHeight = height;

    WGPUSurfaceConfiguration config = {};
    config.device = _device;
    config.format = _surfaceFormat;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.width = width;
    config.height = height;
    config.presentMode = WGPUPresentMode_Fifo;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;

    wgpuSurfaceConfigure(_surface, &config);
}

Result<WGPUTextureView> YettyImpl::getCurrentTextureView() noexcept {
    if (_currentTextureView) {
        return Ok(_currentTextureView);
    }

    WGPUSurfaceTexture surfaceTexture = {};
    wgpuSurfaceGetCurrentTexture(_surface, &surfaceTexture);

    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        return Err<WGPUTextureView>("Failed to get surface texture");
    }

    _currentTexture = surfaceTexture.texture;

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = _surfaceFormat;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;

    _currentTextureView = wgpuTextureCreateView(_currentTexture, &viewDesc);
    return Ok(_currentTextureView);
}

void YettyImpl::present() noexcept {
    if (_currentTextureView) {
        wgpuTextureViewRelease(_currentTextureView);
        _currentTextureView = nullptr;
    }
    if (_currentTexture) {
        wgpuTextureRelease(_currentTexture);
        _currentTexture = nullptr;
    }
#if !defined(__EMSCRIPTEN__)
    // wgpuSurfacePresent is not needed on Emscripten - browser handles via requestAnimationFrame
    wgpuSurfacePresent(_surface);
#endif
}

Result<void> YettyImpl::initSharedResources() noexcept {
    // Initialize GPUContext first (needed by CardBufferManager)
    _gpuContext.device = _device;
    _gpuContext.queue = _queue;
    _gpuContext.surfaceFormat = _surfaceFormat;

    // Create global GPU allocator for shared resources (fonts, shaders, etc.)
    _gpuAllocator = std::make_shared<GpuAllocator>(_device);

    // Create shared uniform buffer FIRST (needed by CardBufferManager)
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("Shared Uniforms");
    bufDesc.size = sizeof(SharedUniforms);
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _sharedUniformBuffer = _gpuAllocator->createBuffer(bufDesc);
    _gpuContext.sharedUniformBuffer = _sharedUniformBuffer;
    _gpuContext.sharedUniformSize = sizeof(SharedUniforms);

    // Full shared bind group layout matching shader group(0):
    //   binding 0: shared uniforms (uniform buffer)
    //   binding 1: card metadata (read-only storage)
    //   binding 2: card storage (read-only storage)
    //   binding 3: atlas texture (2D float)
    //   binding 4: atlas sampler (filtering)
    // Each GPUScreen's CardBufferManager creates a bind group matching this layout.
    // We also create a fallback bind group with dummy resources for non-card rendering.
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
    WGPUBuffer dummyBuffer = _gpuAllocator->createBuffer(dummyBufDesc);

    WGPUTextureDescriptor dummyTexDesc = {};
    dummyTexDesc.label = WGPU_STR("dummy texture");
    dummyTexDesc.size = {1, 1, 1};
    dummyTexDesc.format = WGPUTextureFormat_RGBA8Unorm;
    dummyTexDesc.usage = WGPUTextureUsage_TextureBinding;
    dummyTexDesc.dimension = WGPUTextureDimension_2D;
    dummyTexDesc.mipLevelCount = 1;
    dummyTexDesc.sampleCount = 1;
    WGPUTexture dummyTexture = _gpuAllocator->createTexture(dummyTexDesc);

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
    WGPUSampler dummySampler = wgpuDeviceCreateSampler(_device, &dummySamplerDesc);

    std::array<WGPUBindGroupEntry, 5> bindEntries = {};
    bindEntries[0].binding = 0;
    bindEntries[0].buffer = _sharedUniformBuffer;
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
    bindEntries[4].sampler = dummySampler;

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Fallback Shared Bind Group");
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();
    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);

    // Release dummy resources (bind group retains references)
    wgpuTextureViewRelease(dummyView);
    wgpuTextureRelease(dummyTexture);
    wgpuSamplerRelease(dummySampler);
    wgpuBufferRelease(dummyBuffer);

    _gpuContext.sharedBindGroupLayout = _sharedBindGroupLayout;
    _gpuContext.sharedBindGroup = _sharedBindGroup;

    return Ok();
}


Result<void> YettyImpl::initWorkspace() noexcept {
    auto wsResult = createWorkspace();
    if (!wsResult) {
        return Err<void>("Failed to create default workspace", wsResult);
    }
    yinfo("Created default workspace");
    return Ok();
}

// Helper to create a 2x2 matrix of panes
static Result<Tile::Ptr> createMatrix2x2(Workspace& workspace) {
    // Create 4 panes
    std::array<Pane::Ptr, 4> panes;
    for (int i = 0; i < 4; ++i) {
        auto paneResult = workspace.createPane();
        if (!paneResult) {
            return Err<Tile::Ptr>("Failed to create pane", paneResult);
        }
        panes[i] = *paneResult;
    }

    // Top row: panes[0] | panes[1]
    auto topResult = Split::create(Orientation::Vertical);
    if (!topResult) return Err<Tile::Ptr>("Failed to create split", topResult);
    auto top = *topResult;
    top->setFirst(panes[0]);
    top->setSecond(panes[1]);

    // Bottom row: panes[2] | panes[3]
    auto bottomResult = Split::create(Orientation::Vertical);
    if (!bottomResult) return Err<Tile::Ptr>("Failed to create split", bottomResult);
    auto bottom = *bottomResult;
    bottom->setFirst(panes[2]);
    bottom->setSecond(panes[3]);

    // Stack top and bottom
    auto rootResult = Split::create(Orientation::Horizontal);
    if (!rootResult) return Err<Tile::Ptr>("Failed to create split", rootResult);
    auto root = *rootResult;
    root->setFirst(top);
    root->setSecond(bottom);

    return Ok<Tile::Ptr>(root);
}

Result<Workspace::Ptr> YettyImpl::createWorkspace() noexcept {
    auto wsResult = Workspace::create(_yettyContext);
    if (!wsResult) {
        return Err<Workspace::Ptr>("Failed to create Workspace", wsResult);
    }
    auto workspace = *wsResult;
    float statusbarHeight = _yettyContext.imguiManager ? _yettyContext.imguiManager->getStatusbarHeight() : 0.0f;
    workspace->resize(static_cast<float>(_initialWidth), static_cast<float>(_initialHeight) - statusbarHeight);

    // Create single pane for debugging
    auto paneResult = workspace->createPane();
    if (!paneResult) {
        return Err<Workspace::Ptr>("Failed to create pane", paneResult);
    }
    workspace->setRoot(*paneResult);
    yinfo("Created single terminal pane");

    _workspaces.push_back(workspace);
    if (_workspaces.size() == 1) {
        _activeWorkspace = workspace;
    }

    return Ok(workspace);
}

Result<void> YettyImpl::initCallbacks() noexcept {
    // Focus callback
    _platform->setFocusCallback([](bool focused) {
        ydebug("FocusCallback: focused={}", focused);
    });

    // Key callback
    _platform->setKeyCallback([this](int key, int scancode, KeyAction action, int mods) {
        ydebug("KeyCallback: key={} scancode={} action={} mods={}", key, scancode, static_cast<int>(action), mods);

        // Track modifier key state for scroll events
        // GLFW_KEY_LEFT_CONTROL=341, GLFW_KEY_RIGHT_CONTROL=345
        // GLFW_KEY_LEFT_SHIFT=340, GLFW_KEY_RIGHT_SHIFT=344
        if (key == 341 || key == 345) {
            _ctrlPressed = (action == KeyAction::Press || action == KeyAction::Repeat);
        }
        if (key == 340 || key == 344) {
            _shiftPressed = (action == KeyAction::Press || action == KeyAction::Repeat);
        }

        if (action != KeyAction::Press && action != KeyAction::Repeat) return;

        auto loop = *base::EventLoop::instance();

        // Handle Ctrl/Alt + character combinations using platform key name lookup
        // GLFW_MOD_CONTROL = 0x0002, GLFW_MOD_ALT = 0x0004
        if (mods & (0x0002 | 0x0004)) {
            // Special case for space key (GLFW_KEY_SPACE = 32)
            if (key == 32) {
                ydebug("Sending Ctrl/Alt+Space");
                loop->dispatch(base::Event::keyDown(key, mods, scancode));
                return;
            }

            std::string keyName = _platform->getKeyName(key, scancode);
            if (!keyName.empty() && keyName.size() == 1) {
                // Single character key - dispatch as char with mods
                uint32_t ch = static_cast<uint32_t>(keyName[0]);
                ydebug("Ctrl/Alt+char: keyName='{}' -> dispatching charInput with mods", keyName);
                loop->dispatch(base::Event::charInputWithMods(ch, mods));
                return;
            }
        }

        // For special keys (Enter, Backspace, arrows, etc.), dispatch keyDown
        ydebug("KeyCallback: dispatching keyDown key={} mods={}", key, mods);
        loop->dispatch(base::Event::keyDown(key, mods, scancode));
    });

    // Char callback
    _platform->setCharCallback([this](unsigned int codepoint) {
        ydebug("CharCallback: codepoint={} ('{}')", codepoint, codepoint < 32 ? '?' : (char)codepoint);
        uint32_t glyphIdx = codepoint;
        if (auto font = _yettyContext.fontManager->getDefaultMsMsdfFont()) {
            glyphIdx = font->getGlyphIndex(codepoint);
        }
        _sharedUniforms.lastChar = glyphIdx;
        _sharedUniforms.lastCharTime = static_cast<float>(_platform->getTime());

        auto loop = *base::EventLoop::instance();
        loop->dispatch(base::Event::charInput(codepoint));
    });

    // Resize callback
    _platform->setResizeCallback([this](int newWidth, int newHeight) {
        handleResize(newWidth, newHeight);
    });

    // Mouse button callback
    _platform->setMouseButtonCallback([this](MouseButton button, bool pressed, int mods) {
        // Get current mouse position for event dispatch
        // Note: we track mouse position via move callback, but for now get from window size
        // The actual position is tracked in mouse move events
        auto loop = *base::EventLoop::instance();

        // Middle-click paste: read clipboard and dispatch Paste event
        if (button == MouseButton::Middle && pressed) {
            std::string clipboard = _platform->getClipboardText();
            if (!clipboard.empty()) {
                auto text = std::make_shared<std::string>(std::move(clipboard));
                loop->dispatch(base::Event::pasteEvent(std::move(text)));
                ydebug("MouseButtonCallback: middle-click paste");
            }
            return;
        }

        int buttonInt = static_cast<int>(button);
        if (pressed) {
            ydebug("MouseButtonCallback: button={} PRESS at ({}, {})",
                   buttonInt, _lastMouseX, _lastMouseY);
            loop->dispatch(base::Event::mouseDown(_lastMouseX, _lastMouseY, buttonInt, mods));
        } else {
            ydebug("MouseButtonCallback: button={} RELEASE at ({}, {})",
                   buttonInt, _lastMouseX, _lastMouseY);
            loop->dispatch(base::Event::mouseUp(_lastMouseX, _lastMouseY, buttonInt, mods));
        }
    });

    // Mouse move callback
    _platform->setMouseMoveCallback([this](double xpos, double ypos) {
        // Scale mouse coordinates from window to framebuffer space
        float mx = static_cast<float>(xpos) * _contentScaleX;
        float my = static_cast<float>(ypos) * _contentScaleY;
        _lastMouseX = mx;
        _lastMouseY = my;
        auto loop = *base::EventLoop::instance();
        loop->dispatch(base::Event::mouseMove(mx, my));
    });

    // Scroll callback
    _platform->setScrollCallback([this](double xoffset, double yoffset) {
        // Build modifier state from tracked key presses
        // GLFW_MOD_SHIFT = 0x0001, GLFW_MOD_CONTROL = 0x0002
        int mods = 0;
        if (_shiftPressed) mods |= 0x0001;
        if (_ctrlPressed) mods |= 0x0002;
        auto loop = *base::EventLoop::instance();
        loop->dispatch(base::Event::scrollEvent(
            _lastMouseX, _lastMouseY,
            static_cast<float>(xoffset), static_cast<float>(yoffset), mods));
    });

    // Initialize content scale at startup
    int fbWidth, fbHeight, winWidth, winHeight;
    _platform->getFramebufferSize(fbWidth, fbHeight);
    _platform->getWindowSize(winWidth, winHeight);
    if (winWidth > 0 && winHeight > 0) {
        _contentScaleX = static_cast<float>(fbWidth) / static_cast<float>(winWidth);
        _contentScaleY = static_cast<float>(fbHeight) / static_cast<float>(winHeight);
        yinfo("Initial content scale: {}x{} (fb {}x{} / win {}x{})",
              _contentScaleX, _contentScaleY, fbWidth, fbHeight, winWidth, winHeight);
    }

    return Ok();
}

Result<void> YettyImpl::onShutdown() {
    Result<void> result = Ok();

#if !YETTY_WEB && !defined(__ANDROID__)
    if (_rpcServer) {
        _rpcServer->stop();
        _rpcServer.reset();
    }
    shutdownEventLoop();
#endif

    // Shutdown workspaces while shared_ptrs are still alive
    for (auto& ws : _workspaces) {
        if (auto res = ws->shutdown(); !res) {
            result = Err<void>("Workspace shutdown failed", res);
        }
    }
    _activeWorkspace.reset();
    _workspaces.clear();

    if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);
    if (_sharedBindGroupLayout) wgpuBindGroupLayoutRelease(_sharedBindGroupLayout);
    if (_sharedUniformBuffer && _gpuAllocator) _gpuAllocator->releaseBuffer(_sharedUniformBuffer);
    // Surface must be released before device — swapchain references the device
    if (_surface) wgpuSurfaceRelease(_surface);
    if (_device) wgpuDeviceRelease(_device);
    if (_adapter) wgpuAdapterRelease(_adapter);
    if (_instance) wgpuInstanceRelease(_instance);

    // Platform handles window destruction
    _platform.reset();

    s_instance = nullptr;
    return result;
}

//=============================================================================
// Event Loop
//=============================================================================

#if !YETTY_WEB && !defined(__ANDROID__)
void YettyImpl::initEventLoop() noexcept {
    auto loop = *base::EventLoop::instance();

    // Input poll timer - always fast (8ms = 120Hz) for responsive UI
    auto inputTimerResult = loop->createTimer();
    if (!inputTimerResult) {
        yerror("Failed to create input timer: {}", error_msg(inputTimerResult));
        return;
    }
    _inputTimerId = *inputTimerResult;
    if (auto res = loop->configTimer(_inputTimerId, 8); !res) {
        yerror("Failed to configure input timer: {}", error_msg(res));
        return;
    }
    if (auto res = loop->registerTimerListener(_inputTimerId, sharedAs<base::EventListener>()); !res) {
        yerror("Failed to register input timer listener: {}", error_msg(res));
        return;
    }

    // Frame render timer - configurable FPS (default 60)
    auto frameTimerResult = loop->createTimer();
    if (!frameTimerResult) {
        yerror("Failed to create frame timer: {}", error_msg(frameTimerResult));
        return;
    }
    _frameTimerId = *frameTimerResult;
    if (auto res = loop->configTimer(_frameTimerId, 16); !res) {
        yerror("Failed to configure frame timer: {}", error_msg(res));
        return;
    }
    if (auto res = loop->registerTimerListener(_frameTimerId, sharedAs<base::EventListener>()); !res) {
        yerror("Failed to register frame timer listener: {}", error_msg(res));
        return;
    }

    // Register for Copy events to write to system clipboard
    loop->registerListener(base::Event::Type::Copy, sharedAs<base::EventListener>());
    // Register for SetCursor events from GPUScreen
    loop->registerListener(base::Event::Type::SetCursor, sharedAs<base::EventListener>());
    // Register for SetFrameRate events (OSC 666671)
    loop->registerListener(base::Event::Type::SetFrameRate, sharedAs<base::EventListener>());
    // Register for ScreenUpdate events (PTY activity)
    loop->registerListener(base::Event::Type::ScreenUpdate, sharedAs<base::EventListener>());
}

void YettyImpl::shutdownEventLoop() noexcept {
    auto loop = *base::EventLoop::instance();
    if (_inputTimerId >= 0) {
        loop->destroyTimer(_inputTimerId);
        _inputTimerId = -1;
    }
    if (_frameTimerId >= 0) {
        loop->destroyTimer(_frameTimerId);
        _frameTimerId = -1;
    }
}
#endif

Result<bool> YettyImpl::onEvent(const base::Event& event) {
#if !YETTY_WEB && !defined(__ANDROID__)
    // Input timer: poll events only (fast, always responsive)
    if (event.type == base::Event::Type::Timer && event.timer.timerId == _inputTimerId) {
        _platform->pollEvents();
        if (_platform->shouldClose()) {
            (*base::EventLoop::instance())->stop();
        }
        return Ok(true);
    }

    // Frame timer or ScreenUpdate: render
    if ((event.type == base::Event::Type::Timer && event.timer.timerId == _frameTimerId) ||
        event.type == base::Event::Type::ScreenUpdate) {
        if (auto res = mainLoopIteration(); !res) {
            yerror("Fatal render error: {}", error_msg(res));
            (*base::EventLoop::instance())->stop();
        }
        return Ok(true);
    }

    // SetCursor event: change mouse cursor shape
    if (event.type == base::Event::Type::SetCursor && _platform) {
        int shape = event.setCursor.shape;
        // Map GLFW cursor constants to CursorType
        CursorType cursorType = CursorType::Arrow;
        switch (shape) {
            case 0: cursorType = CursorType::Arrow; break;       // Default
            case 0x00036002: cursorType = CursorType::IBeam; break;  // GLFW_IBEAM_CURSOR
            case 0x00036004: cursorType = CursorType::Hand; break;   // GLFW_POINTING_HAND_CURSOR
            case 0x00036005: cursorType = CursorType::ResizeH; break; // GLFW_RESIZE_EW_CURSOR
            case 0x00036006: cursorType = CursorType::ResizeV; break; // GLFW_RESIZE_NS_CURSOR
            default: cursorType = CursorType::Arrow; break;
        }
        _platform->setCursor(cursorType);
        return Ok(true);
    }

    // Copy event: write selected text to system clipboard
    if (event.type == base::Event::Type::Copy && event.payload && _platform) {
        auto text = std::static_pointer_cast<std::string>(event.payload);
        if (text && !text->empty()) {
            _platform->setClipboardText(*text);
            yinfo("Clipboard: copied {} bytes", text->size());
        }
        return Ok(true);
    }

    // SetFrameRate event: reconfigure frame timer (OSC 666671)
    if (event.type == base::Event::Type::SetFrameRate) {
        uint32_t fps = event.setFrameRate.fps;
        uint32_t intervalMs = (fps > 0) ? (1000 / fps) : 16;
        if (intervalMs < 1) intervalMs = 1;
        auto loop = *base::EventLoop::instance();
        if (auto res = loop->configTimer(_frameTimerId, intervalMs); !res) {
            yerror("Failed to reconfigure frame timer: {}", error_msg(res));
        } else {
            yinfo("Frame rate changed to {} FPS ({}ms interval)", fps, intervalMs);
        }
        return Ok(true);
    }
#endif
    return Ok(false);
}

static void signalHandler(int sig) {
    yinfo("Received signal {}, shutting down...", sig);
    (void)(*base::EventLoop::instance())->stop();
}

Result<void> YettyImpl::run() noexcept {
    yinfo("Starting render loop...");

    // Handle Ctrl+C from launching terminal
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

#if YETTY_WEB
    // For web builds, use platform's main loop (emscripten_set_main_loop_arg)
    _platform->runMainLoop([this]() -> bool {
        auto result = mainLoopIteration();
        if (!result) {
            yerror("Main loop iteration failed: {}", result.error().message());
            return false;
        }
        return !_fatalGpuError;
    });
    // Note: runMainLoop doesn't return on web (emscripten takes over)
    return Ok();
#elif !defined(__ANDROID__)
    auto loop = *base::EventLoop::instance();
    loop->startTimer(_inputTimerId);
    loop->startTimer(_frameTimerId);
    loop->start();

    yinfo("Run finished.");

    if (_fatalGpuError) {
        return Err<void>("Fatal GPU error: " + _fatalGpuErrorMsg);
    }
    return Ok();
#else
    // Android: main loop is handled externally
    return Ok();
#endif
}

Result<void> YettyImpl::mainLoopIteration() noexcept {
    if (_fatalGpuError) {
        return Err<void>("Fatal GPU error: " + _fatalGpuErrorMsg);
    }

    auto viewResult = getCurrentTextureView();
    if (!viewResult) return Err<void>("Failed to get texture view");
    WGPUTextureView targetView = *viewResult;

    // Update shared uniforms
    static double lastTime = _platform->getTime();
    double now = _platform->getTime();
    float deltaTime = static_cast<float>(now - lastTime);
    lastTime = now;

    int windowWidth, windowHeight;
    _platform->getWindowSize(windowWidth, windowHeight);

    _sharedUniforms.time = static_cast<float>(now);
    _sharedUniforms.deltaTime = deltaTime;
    _sharedUniforms.screenWidth = static_cast<float>(windowWidth);
    _sharedUniforms.screenHeight = static_cast<float>(windowHeight);
    _sharedUniforms.mouseX = _lastMouseX;
    _sharedUniforms.mouseY = _lastMouseY;
    wgpuQueueWriteBuffer(_queue, _sharedUniformBuffer, 0, &_sharedUniforms, sizeof(SharedUniforms));

    // Upload any pending font glyphs (e.g., bold/italic loaded on demand)
    if (_yettyContext.fontManager) {
        if (auto msdfFont = _yettyContext.fontManager->getDefaultMsMsdfFont()) {
            if (msdfFont->atlas()->hasPendingGlyphs()) {
                auto uploadResult = msdfFont->atlas()->uploadPendingGlyphs(_device, _queue);
                if (!uploadResult) {
                    ywarn("Failed to upload pending glyphs: {}", uploadResult.error().message());
                }
            }
        }
    }

    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encoderDesc);
    if (!encoder) return Err<void>("Failed to create command encoder");

    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.1f, 0.1f, 0.2f, 1.0f};
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (pass) {
        if (_activeWorkspace) {
            if (auto res = _activeWorkspace->render(pass); !res) {
                wgpuRenderPassEncoderEnd(pass);
                wgpuRenderPassEncoderRelease(pass);
                wgpuCommandEncoderRelease(encoder);
                return Err<void>("Workspace render failed", res);
            }
        }
        // Render ImGui (context menus, etc.) after main content
        if (_yettyContext.imguiManager) {
            if (auto res = _yettyContext.imguiManager->render(pass); !res) {
                wgpuRenderPassEncoderEnd(pass);
                wgpuRenderPassEncoderRelease(pass);
                wgpuCommandEncoderRelease(encoder);
                return Err<void>("ImGui render failed", res);
            }
        }
        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);
    }

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    if (cmdBuffer) {
        wgpuQueueSubmit(_queue, 1, &cmdBuffer);
        wgpuCommandBufferRelease(cmdBuffer);
    }
    wgpuCommandEncoderRelease(encoder);

    present();

    // Check if GPU error occurred during this frame
    if (_fatalGpuError) {
        return Err<void>("GPU error during frame: " + _fatalGpuErrorMsg);
    }

    _frameCount++;
    double fpsNow = _platform->getTime();
    if (fpsNow - _lastFpsTime >= 1.0) {
        yinfo("FPS: {}", _frameCount);
        if (_yettyContext.imguiManager) {
            _yettyContext.imguiManager->setFps(_frameCount);
        }
        _frameCount = 0;
        _lastFpsTime = fpsNow;
    }

    return Ok();
}

void YettyImpl::handleResize(int newWidth, int newHeight) noexcept {
    if (newWidth == 0 || newHeight == 0) return;

    // Update content scale (framebuffer / window)
    int windowWidth, windowHeight;
    _platform->getWindowSize(windowWidth, windowHeight);
    if (windowWidth > 0 && windowHeight > 0) {
        _contentScaleX = static_cast<float>(newWidth) / static_cast<float>(windowWidth);
        _contentScaleY = static_cast<float>(newHeight) / static_cast<float>(windowHeight);
        yinfo("Content scale updated: {}x{} (fb {}x{} / win {}x{})",
              _contentScaleX, _contentScaleY, newWidth, newHeight, windowWidth, windowHeight);
    }

    // Convert framebuffer pixels to logical coordinates
    // Dawn/Metal handles Retina scaling internally via contentsScale
    float logicalW = (windowWidth > 0) ? static_cast<float>(windowWidth) : static_cast<float>(newWidth);
    float logicalH = (windowHeight > 0) ? static_cast<float>(windowHeight) : static_cast<float>(newHeight);

    // Surface uses logical size — Dawn handles physical backing automatically
    configureSurface(static_cast<uint32_t>(logicalW), static_cast<uint32_t>(logicalH));

    if (_yettyContext.imguiManager) {
        _yettyContext.imguiManager->updateDisplaySize(
            static_cast<uint32_t>(logicalW), static_cast<uint32_t>(logicalH));
    }

    if (_activeWorkspace) {
        float statusbarHeight = _yettyContext.imguiManager ? _yettyContext.imguiManager->getStatusbarHeight() : 0.0f;
        _activeWorkspace->resize(logicalW, logicalH - statusbarHeight);
    }
}

} // namespace yetty
