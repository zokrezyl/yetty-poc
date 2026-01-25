#include <yetty/new-yetty.h>
#include <yetty/gpu-context.h>
#include <yetty/terminal-view.h>
#include <yetty/workspace.h>
#include <yetty/yetty-font-manager.h>
#include <yetty/shader-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/base/base.h>
#include <glfw3webgpu.h>
#include <array>
#include <iostream>
#include <ytrace/ytrace.hpp>

namespace yetty {

class NewYettyImpl : public NewYetty, public base::EventListener {
public:
    NewYettyImpl() = default;
    ~NewYettyImpl() override { shutdown(); }

    Result<void> init(int argc, char* argv[]) noexcept;
    int run() noexcept override;
    Result<bool> onEvent(const base::Event& event) override;

private:
    Result<void> parseArgs(int argc, char* argv[]) noexcept;
    Result<void> initWindow() noexcept;
    Result<void> initWebGPU() noexcept;
    Result<void> initSharedResources() noexcept;
    Result<void> initFonts() noexcept;
    Result<void> initWorkspace() noexcept;
    Result<void> initCallbacks() noexcept;
    void shutdown() noexcept;

    void mainLoopIteration() noexcept;
    void handleResize(int width, int height) noexcept;
    void configureSurface(uint32_t width, uint32_t height) noexcept;
    Result<WGPUTextureView> getCurrentTextureView() noexcept;
    void present() noexcept;

#if !YETTY_WEB && !defined(__ANDROID__)
    void initEventLoop() noexcept;
    void shutdownEventLoop() noexcept;
#endif

    Result<Workspace::Ptr> createWorkspace() noexcept;

    // Window
    GLFWwindow* _window = nullptr;
    uint32_t _initialWidth = 1024;
    uint32_t _initialHeight = 768;

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
        float _pad1;
        float _pad2;
    };
    WGPUBuffer _sharedUniformBuffer = nullptr;
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
    SharedUniforms _sharedUniforms = {};
    GPUContext _gpuContext = {};

#if !YETTY_WEB && !defined(__ANDROID__)
    std::unique_ptr<CardBufferManager> _cardBufferManager;
#endif

    // Workspaces
    std::vector<Workspace::Ptr> _workspaces;
    Workspace::Ptr _activeWorkspace;

    // Grid dimensions
    uint32_t _cols = 80;
    uint32_t _rows = 24;
    float _baseCellWidth = 10.0f;
    float _baseCellHeight = 20.0f;
    float _zoomLevel = 0.5f;

#if !YETTY_WEB && !defined(__ANDROID__)
    base::TimerId _frameTimerId = -1;
#endif

    // FPS tracking
    double _lastFpsTime = 0.0;
    uint32_t _frameCount = 0;

    static NewYettyImpl* s_instance;
};

NewYettyImpl* NewYettyImpl::s_instance = nullptr;

//=============================================================================
// Factory
//=============================================================================

Result<NewYetty::Ptr> NewYetty::create(int argc, char* argv[]) noexcept {
    auto impl = std::make_shared<NewYettyImpl>();
    if (auto res = impl->init(argc, argv); !res) {
        return Err<Ptr>("Failed to init NewYetty", res);
    }
    return Ok<Ptr>(impl);
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> NewYettyImpl::init(int argc, char* argv[]) noexcept {
    yinfo("NewYetty starting...");

    if (auto res = parseArgs(argc, argv); !res) return res;
    if (auto res = initWindow(); !res) return res;
    if (auto res = initWebGPU(); !res) return res;
    if (auto res = initSharedResources(); !res) return res;

    // Initialize ShaderManager before fonts (so fonts can register as providers)
    if (auto res = ShaderManager::instance()->init(_gpuContext); !res) {
        return Err<void>("Failed to init ShaderManager", res);
    }

    // Initialize fonts - ShaderFont will self-register with ShaderManager
    if (auto res = initFonts(); !res) return res;

    // Compile shaders after all providers (fonts) are registered
    if (auto res = ShaderManager::instance()->compile(); !res) {
        return Err<void>("Failed to compile shaders", res);
    }

#if !YETTY_WEB && !defined(__ANDROID__)
    initEventLoop();
#endif

    if (auto res = initWorkspace(); !res) return res;
    if (auto res = initCallbacks(); !res) return res;

    s_instance = this;
    _lastFpsTime = glfwGetTime();

    return Ok();
}

Result<void> NewYettyImpl::parseArgs(int argc, char* argv[]) noexcept {
    (void)argc;
    (void)argv;
    return Ok();
}

Result<void> NewYettyImpl::initWindow() noexcept {
    if (!glfwInit()) {
        return Err<void>("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(_initialWidth, _initialHeight, "yetty", nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        return Err<void>("Failed to create window");
    }

    return Ok();
}

Result<void> NewYettyImpl::initWebGPU() noexcept {
    // Create instance
    WGPUInstanceDescriptor instanceDesc = {};
    _instance = wgpuCreateInstance(&instanceDesc);
    if (!_instance) {
        return Err<void>("Failed to create WebGPU instance");
    }

    // Create surface
    _surface = glfwCreateWindowWGPUSurface(_instance, _window);
    if (!_surface) {
        return Err<void>("Failed to create WebGPU surface");
    }

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
    wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCallbackInfo);

    if (!_adapter) {
        return Err<void>("Failed to get WebGPU adapter");
    }

    // Request device with higher storage buffer limits
    WGPULimits limits = {};
    limits.maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension2D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureArrayLayers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroups = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxStorageBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED;
    limits.maxStorageBufferBindingSize = 512 * 1024 * 1024;  // 512MB
    limits.minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxBufferSize = WGPU_LIMIT_U64_UNDEFINED;
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
        yerror("WebGPU error ({}): {}", static_cast<int>(type), message.data ? std::string(message.data, message.length) : "unknown");
    };

    WGPURequestDeviceCallbackInfo deviceCallbackInfo = {};
    deviceCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCallbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                                     WGPUStringView message, void* userdata1, void* userdata2) {
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(userdata1) = device;
        } else {
            yerror("WebGPU device request failed ({}): {}", static_cast<int>(status),
                   message.data ? std::string(message.data, message.length) : "unknown");
        }
    };
    deviceCallbackInfo.userdata1 = &_device;
    wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCallbackInfo);

    if (!_device) {
        return Err<void>("Failed to get WebGPU device");
    }

    _queue = wgpuDeviceGetQueue(_device);

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

void NewYettyImpl::configureSurface(uint32_t width, uint32_t height) noexcept {
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

Result<WGPUTextureView> NewYettyImpl::getCurrentTextureView() noexcept {
    if (_currentTextureView) {
        return Ok(_currentTextureView);
    }

    WGPUSurfaceTexture surfaceTexture;
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

void NewYettyImpl::present() noexcept {
    if (_currentTextureView) {
        wgpuTextureViewRelease(_currentTextureView);
        _currentTextureView = nullptr;
    }
    if (_currentTexture) {
        wgpuTextureRelease(_currentTexture);
        _currentTexture = nullptr;
    }
    wgpuSurfacePresent(_surface);
}

Result<void> NewYettyImpl::initSharedResources() noexcept {
#if !YETTY_WEB && !defined(__ANDROID__)
    // Create CardBufferManager
    auto cbmResult = CardBufferManager::create(_device);
    if (!cbmResult) {
        return Err<void>("Failed to create CardBufferManager", cbmResult);
    }
    _cardBufferManager = std::move(*cbmResult);

    if (auto res = _cardBufferManager->initAtlas(); !res) {
        return Err<void>("Failed to initialize card atlas", res);
    }
#endif

    // Create shared uniform buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.label = WGPU_STR("Shared Uniforms");
    bufDesc.size = sizeof(SharedUniforms);
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _sharedUniformBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);

#if !YETTY_WEB && !defined(__ANDROID__)
    // Create bind group layout with card buffers
    std::array<WGPUBindGroupLayoutEntry, 6> layoutEntries = {};

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

    layoutEntries[5].binding = 5;
    layoutEntries[5].visibility = WGPUShaderStage_Fragment;
    layoutEntries[5].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.label = WGPU_STR("Shared Bind Group Layout");
    layoutDesc.entryCount = layoutEntries.size();
    layoutDesc.entries = layoutEntries.data();
    _sharedBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &layoutDesc);

    // Create bind group
    std::array<WGPUBindGroupEntry, 6> bindEntries = {};

    bindEntries[0].binding = 0;
    bindEntries[0].buffer = _sharedUniformBuffer;
    bindEntries[0].size = sizeof(SharedUniforms);

    bindEntries[1].binding = 1;
    bindEntries[1].buffer = _cardBufferManager->metadataBuffer();
    bindEntries[1].size = wgpuBufferGetSize(_cardBufferManager->metadataBuffer());

    bindEntries[2].binding = 2;
    bindEntries[2].buffer = _cardBufferManager->storageBuffer();
    bindEntries[2].size = wgpuBufferGetSize(_cardBufferManager->storageBuffer());

    bindEntries[3].binding = 3;
    bindEntries[3].textureView = _cardBufferManager->atlasTextureView();

    bindEntries[4].binding = 4;
    bindEntries[4].sampler = _cardBufferManager->atlasSampler();

    bindEntries[5].binding = 5;
    bindEntries[5].buffer = _cardBufferManager->imageDataBuffer();
    bindEntries[5].size = wgpuBufferGetSize(_cardBufferManager->imageDataBuffer());

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Shared Bind Group");
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();
    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);
#else
    // Web/Android: only shared uniforms
    WGPUBindGroupLayoutEntry layoutEntry = {};
    layoutEntry.binding = 0;
    layoutEntry.visibility = WGPUShaderStage_Fragment;
    layoutEntry.buffer.type = WGPUBufferBindingType_Uniform;
    layoutEntry.buffer.minBindingSize = sizeof(SharedUniforms);

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 1;
    layoutDesc.entries = &layoutEntry;
    _sharedBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &layoutDesc);

    WGPUBindGroupEntry bindEntry = {};
    bindEntry.binding = 0;
    bindEntry.buffer = _sharedUniformBuffer;
    bindEntry.size = sizeof(SharedUniforms);

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = 1;
    bindDesc.entries = &bindEntry;
    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);
#endif

    // Initialize GPUContext
    _gpuContext.device = _device;
    _gpuContext.queue = _queue;
    _gpuContext.surfaceFormat = _surfaceFormat;
    _gpuContext.sharedBindGroupLayout = _sharedBindGroupLayout;
    _gpuContext.sharedBindGroup = _sharedBindGroup;

    return Ok();
}

Result<void> NewYettyImpl::initFonts() noexcept {
    auto fontManager = YettyFontManager::instance();
    if (auto res = fontManager->init(_gpuContext); !res) {
        return Err<void>("Failed to init YettyFontManager", res);
    }

    _cols = static_cast<uint32_t>(_initialWidth / (_baseCellWidth * _zoomLevel));
    _rows = static_cast<uint32_t>(_initialHeight / (_baseCellHeight * _zoomLevel));

    return Ok();
}

Result<void> NewYettyImpl::initWorkspace() noexcept {
    auto wsResult = createWorkspace();
    if (!wsResult) {
        return Err<void>("Failed to create default workspace", wsResult);
    }
    yinfo("Created default workspace: {} cols x {} rows", _cols, _rows);
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

Result<Workspace::Ptr> NewYettyImpl::createWorkspace() noexcept {
    auto wsResult = Workspace::create(_gpuContext);
    if (!wsResult) {
        return Err<Workspace::Ptr>("Failed to create Workspace", wsResult);
    }
    auto workspace = *wsResult;
    workspace->resize(static_cast<float>(_initialWidth), static_cast<float>(_initialHeight));

#if !YETTY_WEB && !defined(__ANDROID__)
    // Create 2x2 matrix of terminal panes
    auto matrixResult = createMatrix2x2(*workspace);
    if (!matrixResult) {
        return Err<Workspace::Ptr>("Failed to create 2x2 matrix", matrixResult);
    }
    workspace->setRoot(*matrixResult);
    yinfo("Created 2x2 terminal matrix (4 terminals)");
#endif

    _workspaces.push_back(workspace);
    if (_workspaces.size() == 1) {
        _activeWorkspace = workspace;
    }

    return Ok(workspace);
}

Result<void> NewYettyImpl::initCallbacks() noexcept {
    glfwSetWindowUserPointer(_window, this);

    glfwSetKeyCallback(_window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        ydebug("glfwKeyCallback: key={} scancode={} action={} mods={}", key, scancode, action, mods);
        if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

        auto loop = base::EventLoop::instance();

        // Handle Ctrl/Alt + character combinations using glfwGetKeyName
        // This is how the old InputHandler::onKey did it
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
            // Special case for space key - glfwGetKeyName may return NULL for it
            if (key == GLFW_KEY_SPACE) {
                ydebug("Sending Ctrl/Alt+Space");
                loop->dispatch(base::Event::keyDown(key, mods, scancode));
                return;
            }

            const char* keyName = glfwGetKeyName(key, scancode);
            if (keyName && keyName[0] != '\0' && keyName[1] == '\0') {
                // Single character key - dispatch as char with mods
                uint32_t ch = static_cast<uint32_t>(keyName[0]);
                ydebug("Ctrl/Alt+char: keyName='{}' -> dispatching charInput with mods", keyName);
                loop->dispatch(base::Event::charInputWithMods(ch, mods));
                return;
            }
        }

        // For special keys (Enter, Backspace, arrows, etc.), dispatch keyDown
        loop->dispatch(base::Event::keyDown(key, mods, scancode));
    });

    glfwSetCharCallback(_window, [](GLFWwindow* w, unsigned int codepoint) {
        ydebug("glfwCharCallback: codepoint={} ('{}')", codepoint, codepoint < 32 ? '?' : (char)codepoint);
        auto loop = base::EventLoop::instance();
        loop->dispatch(base::Event::charInput(codepoint));
    });

    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* w, int newWidth, int newHeight) {
        auto* impl = static_cast<NewYettyImpl*>(glfwGetWindowUserPointer(w));
        if (impl) impl->handleResize(newWidth, newHeight);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* w) {
        ydebug("glfwWindowCloseCallback triggered!");
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* w, int button, int action, int mods) {
        (void)mods;
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(w, &xpos, &ypos);
            ydebug("glfwMouseButtonCallback: button={} at ({}, {})", button, xpos, ypos);
            auto loop = base::EventLoop::instance();
            loop->dispatch(base::Event::mouseDown(static_cast<float>(xpos), static_cast<float>(ypos), button));
        }
    });

    return Ok();
}

void NewYettyImpl::shutdown() noexcept {
#if !YETTY_WEB && !defined(__ANDROID__)
    shutdownEventLoop();
#endif

    _activeWorkspace.reset();
    _workspaces.clear();

    if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);
    if (_sharedBindGroupLayout) wgpuBindGroupLayoutRelease(_sharedBindGroupLayout);
    if (_sharedUniformBuffer) wgpuBufferRelease(_sharedUniformBuffer);
    if (_device) wgpuDeviceRelease(_device);
    if (_adapter) wgpuAdapterRelease(_adapter);
    if (_surface) wgpuSurfaceRelease(_surface);
    if (_instance) wgpuInstanceRelease(_instance);

    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    glfwTerminate();

    s_instance = nullptr;
}

//=============================================================================
// Event Loop
//=============================================================================

#if !YETTY_WEB && !defined(__ANDROID__)
void NewYettyImpl::initEventLoop() noexcept {
    auto loop = base::EventLoop::instance();
    _frameTimerId = loop->createTimer();
    loop->configTimer(_frameTimerId, 16);
    loop->registerTimerListener(_frameTimerId, sharedAs<base::EventListener>());
}

void NewYettyImpl::shutdownEventLoop() noexcept {
    if (_frameTimerId >= 0) {
        base::EventLoop::instance()->destroyTimer(_frameTimerId);
        _frameTimerId = -1;
    }
}
#endif

Result<bool> NewYettyImpl::onEvent(const base::Event& event) {
#if !YETTY_WEB && !defined(__ANDROID__)
    if (event.type == base::Event::Type::Timer && event.timer.timerId == _frameTimerId) {
        glfwPollEvents();
        if (glfwWindowShouldClose(_window)) {
            base::EventLoop::instance()->stop();
            return Ok(true);
        }
        mainLoopIteration();
        return Ok(true);
    }
#endif
    return Ok(false);
}

int NewYettyImpl::run() noexcept {
    yinfo("Starting render loop...");

#if !YETTY_WEB && !defined(__ANDROID__)
    auto loop = base::EventLoop::instance();
    loop->startTimer(_frameTimerId);
    loop->start();
#endif

    yinfo("Shutting down...");
    return 0;
}

void NewYettyImpl::mainLoopIteration() noexcept {
    auto viewResult = getCurrentTextureView();
    if (!viewResult) return;
    WGPUTextureView targetView = *viewResult;

    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encoderDesc);
    if (!encoder) return;

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
            _activeWorkspace->render(pass);
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

    _frameCount++;
    double now = glfwGetTime();
    if (now - _lastFpsTime >= 1.0) {
        yinfo("FPS: {}", _frameCount);
        _frameCount = 0;
        _lastFpsTime = now;
    }
}

void NewYettyImpl::handleResize(int newWidth, int newHeight) noexcept {
    if (newWidth == 0 || newHeight == 0) return;

    configureSurface(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight));

    _cols = static_cast<uint32_t>(newWidth / (_baseCellWidth * _zoomLevel));
    _rows = static_cast<uint32_t>(newHeight / (_baseCellHeight * _zoomLevel));

    if (_activeWorkspace) {
        _activeWorkspace->resize(static_cast<float>(newWidth), static_cast<float>(newHeight));
    }
}

} // namespace yetty
