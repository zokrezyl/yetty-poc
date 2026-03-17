#include <yetty/platform/webgpu-manager.h>

#if defined(__linux__) && !defined(__ANDROID__)

#include <yetty/result.hpp>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

class LinuxWebGpuManager : public WebGpuManager {
public:
    ~LinuxWebGpuManager() override {
        shutdown();
    }

    Result<void> init(SurfaceCreator surfaceCreator, bool headless) override {
        _headless = headless;

        // Create instance
        WGPUInstanceDescriptor instanceDesc = {};
        _instance = wgpuCreateInstance(&instanceDesc);
        if (!_instance) {
            return Err<void>("Failed to create WebGPU instance");
        }
        ydebug("LinuxWebGpuManager: Instance created");

        // Create surface
        if (!headless && surfaceCreator) {
            _surface = surfaceCreator(_instance);
            if (_surface) {
                ydebug("LinuxWebGpuManager: Surface created");
            } else {
                ywarn("LinuxWebGpuManager: Surface creation failed");
            }
        }

        // Request adapter - Linux uses Vulkan via Dawn
        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = _surface;
        adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

        WGPURequestAdapterCallbackInfo adapterCb = {};
        adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
        adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                WGPUStringView message, void* userdata1, void* userdata2) {
            if (status == WGPURequestAdapterStatus_Success) {
                *static_cast<WGPUAdapter*>(userdata1) = adapter;
            }
        };
        adapterCb.userdata1 = &_adapter;

        ydebug("LinuxWebGpuManager: Requesting adapter...");
        wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);

        if (!_adapter) {
            return Err<void>("Failed to get WebGPU adapter");
        }

        logAdapterInfo();

        // Request device
        if (auto res = requestDevice(); !res) {
            return res;
        }

        _queue = wgpuDeviceGetQueue(_device);
        ydebug("LinuxWebGpuManager: Initialization complete");

        return Ok();
    }

    WGPUInstance getInstance() const override { return _instance; }
    WGPUAdapter getAdapter() const override { return _adapter; }
    WGPUDevice getDevice() const override { return _device; }
    WGPUQueue getQueue() const override { return _queue; }
    WGPUSurface getSurface() const override { return _surface; }
    WGPUTextureFormat getSurfaceFormat() const override { return _surfaceFormat; }

    Result<void> configureSurface(uint32_t width, uint32_t height) override {
        if (!_surface) {
            return Err<void>("No surface to configure");
        }

        // Release old texture views
        releaseCurrentTexture();

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
        ydebug("LinuxWebGpuManager: Surface configured {}x{}", width, height);

        return Ok();
    }

    Result<WGPUTextureView> getCurrentTextureView() override {
        if (!_surface) {
            return Err<WGPUTextureView>("No surface");
        }

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

    void presentSurface() override {
        if (!_surface) return;

        releaseCurrentTexture();
        wgpuSurfacePresent(_surface);
    }

    void setErrorCallback(ErrorCallback cb) override {
        _errorCallback = std::move(cb);
    }

    void shutdown() override {
        releaseCurrentTexture();

        if (_queue) { wgpuQueueRelease(_queue); _queue = nullptr; }
        if (_device) { wgpuDeviceRelease(_device); _device = nullptr; }
        if (_surface) { wgpuSurfaceRelease(_surface); _surface = nullptr; }
        if (_adapter) { wgpuAdapterRelease(_adapter); _adapter = nullptr; }
        if (_instance) { wgpuInstanceRelease(_instance); _instance = nullptr; }
    }

private:
    void logAdapterInfo() {
        WGPUAdapterInfo info = {};
        if (wgpuAdapterGetInfo(_adapter, &info) == WGPUStatus_Success) {
            auto sv = [](WGPUStringView s) -> std::string {
                return (s.data && s.length > 0) ? std::string(s.data, s.length) : "(unknown)";
            };
            ydebug("GPU: {} ({})", sv(info.device), sv(info.vendor));
            ydebug("Backend: {} | Type: {}", static_cast<int>(info.backendType), static_cast<int>(info.adapterType));
            wgpuAdapterInfoFreeMembers(info);
        }
    }

    Result<void> requestDevice() {
        WGPULimits adapterLimits = {};
        wgpuAdapterGetLimits(_adapter, &adapterLimits);

        WGPULimits limits = {};
        limits.maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED;
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
        limits.maxStorageBuffersPerShaderStage = 10;
        limits.maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
        limits.maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED;
        limits.maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED;
        limits.maxStorageBufferBindingSize = std::min(static_cast<uint64_t>(512 * 1024 * 1024),
                                                       adapterLimits.maxStorageBufferBindingSize);
        limits.minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
        limits.minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED;
        limits.maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED;
        limits.maxBufferSize = std::min(static_cast<uint64_t>(1024 * 1024 * 1024), adapterLimits.maxBufferSize);
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
            auto* self = static_cast<LinuxWebGpuManager*>(userdata1);
            auto msg = message.data ? std::string(message.data, message.length) : "unknown";
            yerror("WebGPU error ({}): {}", static_cast<int>(type), msg);
            if (self->_errorCallback) {
                self->_errorCallback(type, msg);
            }
        };
        deviceDesc.uncapturedErrorCallbackInfo.userdata1 = this;

        std::string deviceError;
        WGPURequestDeviceCallbackInfo deviceCb = {};
        deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
        deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                               WGPUStringView message, void* userdata1, void* userdata2) {
            if (status == WGPURequestDeviceStatus_Success) {
                *static_cast<WGPUDevice*>(userdata1) = device;
            } else {
                auto msg = message.data ? std::string(message.data, message.length) : "unknown";
                *static_cast<std::string*>(userdata2) = msg;
            }
        };
        deviceCb.userdata1 = &_device;
        deviceCb.userdata2 = &deviceError;

        ydebug("LinuxWebGpuManager: Requesting device...");
        wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCb);

        if (!_device) {
            return Err<void>("Failed to create device: " + deviceError);
        }

        ydebug("LinuxWebGpuManager: Device created");
        return Ok();
    }

    void releaseCurrentTexture() {
        if (_currentTextureView) {
            wgpuTextureViewRelease(_currentTextureView);
            _currentTextureView = nullptr;
        }
        if (_currentTexture) {
            wgpuTextureRelease(_currentTexture);
            _currentTexture = nullptr;
        }
    }

    bool _headless = false;
    WGPUInstance _instance = nullptr;
    WGPUAdapter _adapter = nullptr;
    WGPUDevice _device = nullptr;
    WGPUQueue _queue = nullptr;
    WGPUSurface _surface = nullptr;
    WGPUTextureFormat _surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
    WGPUTexture _currentTexture = nullptr;
    WGPUTextureView _currentTextureView = nullptr;
    uint32_t _surfaceWidth = 0;
    uint32_t _surfaceHeight = 0;
    ErrorCallback _errorCallback;
};

Result<WebGpuManager::Ptr> WebGpuManager::createImpl() {
    return Ok(Ptr(new LinuxWebGpuManager()));
}

} // namespace yetty

#endif // __linux__ && !__ANDROID__
