#include <yetty/platform/webgpu-manager.h>

#if defined(__ANDROID__)

#include <yetty/result.hpp>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

class AndroidWebGpuManager : public WebGpuManager {
public:
    ~AndroidWebGpuManager() override { shutdown(); }

    Result<void> init(SurfaceCreator surfaceCreator, bool headless) override {
        _headless = headless;

        WGPUInstanceDescriptor instanceDesc = {};
        _instance = wgpuCreateInstance(&instanceDesc);
        if (!_instance) return Err<void>("Failed to create WebGPU instance");
        ydebug("AndroidWebGpuManager: Instance created");

        if (!headless && surfaceCreator) {
            _surface = surfaceCreator(_instance);
            if (_surface) {
                ydebug("AndroidWebGpuManager: Surface created");
            } else {
                ywarn("AndroidWebGpuManager: Surface creation failed");
            }
        }

        // Request adapter - Android requires Vulkan backend
        WGPURequestAdapterOptions adapterOpts = {};
        adapterOpts.compatibleSurface = _surface;
        adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;
        adapterOpts.backendType = WGPUBackendType_Vulkan;

        WGPURequestAdapterCallbackInfo adapterCb = {};
        adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
        adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                WGPUStringView, void* userdata1, void*) {
            if (status == WGPURequestAdapterStatus_Success)
                *static_cast<WGPUAdapter*>(userdata1) = adapter;
        };
        adapterCb.userdata1 = &_adapter;

        ydebug("AndroidWebGpuManager: Requesting Vulkan adapter...");
        wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);

        if (!_adapter) return Err<void>("Failed to get Vulkan adapter on Android");

        // Reject Null backend - Android must use real GPU
        WGPUAdapterInfo info = {};
        if (wgpuAdapterGetInfo(_adapter, &info) == WGPUStatus_Success) {
            if (info.backendType == WGPUBackendType_Null) {
                wgpuAdapterInfoFreeMembers(info);
                wgpuAdapterRelease(_adapter);
                _adapter = nullptr;
                return Err<void>("Null backend not supported on Android - Vulkan required");
            }
            logAdapterInfo(info);
            wgpuAdapterInfoFreeMembers(info);
        }

        if (auto res = requestDevice(); !res) return res;

        _queue = wgpuDeviceGetQueue(_device);
        ydebug("AndroidWebGpuManager: Initialization complete");
        return Ok();
    }

    WGPUInstance getInstance() const override { return _instance; }
    WGPUAdapter getAdapter() const override { return _adapter; }
    WGPUDevice getDevice() const override { return _device; }
    WGPUQueue getQueue() const override { return _queue; }
    WGPUSurface getSurface() const override { return _surface; }
    WGPUTextureFormat getSurfaceFormat() const override { return _surfaceFormat; }

    Result<void> configureSurface(uint32_t width, uint32_t height) override {
        if (!_surface) return Err<void>("No surface");
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
        ydebug("AndroidWebGpuManager: Surface configured {}x{}", width, height);
        return Ok();
    }

    Result<WGPUTextureView> getCurrentTextureView() override {
        if (!_surface) return Err<WGPUTextureView>("No surface");
        if (_currentTextureView) return Ok(_currentTextureView);

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

    void setErrorCallback(ErrorCallback cb) override { _errorCallback = std::move(cb); }

    void shutdown() override {
        releaseCurrentTexture();
        if (_queue) { wgpuQueueRelease(_queue); _queue = nullptr; }
        if (_device) { wgpuDeviceRelease(_device); _device = nullptr; }
        if (_surface) { wgpuSurfaceRelease(_surface); _surface = nullptr; }
        if (_adapter) { wgpuAdapterRelease(_adapter); _adapter = nullptr; }
        if (_instance) { wgpuInstanceRelease(_instance); _instance = nullptr; }
    }

private:
    void logAdapterInfo(const WGPUAdapterInfo& info) {
        auto sv = [](WGPUStringView s) { return (s.data && s.length > 0) ? std::string(s.data, s.length) : "(unknown)"; };
        ydebug("GPU: {} ({}) Backend: Vulkan", sv(info.device), sv(info.vendor));
    }

    Result<void> requestDevice() {
        WGPULimits adapterLimits = {};
        wgpuAdapterGetLimits(_adapter, &adapterLimits);

        // Android-specific limits - more conservative for mobile GPUs
        WGPULimits limits = {};
        limits.maxTextureDimension2D = std::min(8192u, adapterLimits.maxTextureDimension2D);
        limits.maxStorageBuffersPerShaderStage = 8;
        limits.maxStorageBufferBindingSize = std::min(static_cast<uint64_t>(128 * 1024 * 1024), adapterLimits.maxStorageBufferBindingSize);
        limits.maxBufferSize = std::min(static_cast<uint64_t>(256 * 1024 * 1024), adapterLimits.maxBufferSize);

        WGPUDeviceDescriptor deviceDesc = {};
        deviceDesc.label = WGPU_STR("yetty device");
        deviceDesc.requiredLimits = &limits;
        deviceDesc.defaultQueue.label = WGPU_STR("default queue");
        deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void* userdata1, void*) {
            auto* self = static_cast<AndroidWebGpuManager*>(userdata1);
            auto msg = message.data ? std::string(message.data, message.length) : "unknown";
            yerror("WebGPU error: {}", msg);
            if (self->_errorCallback) self->_errorCallback(type, msg);
        };
        deviceDesc.uncapturedErrorCallbackInfo.userdata1 = this;

        WGPURequestDeviceCallbackInfo deviceCb = {};
        deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
        deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView, void* userdata1, void*) {
            if (status == WGPURequestDeviceStatus_Success) *static_cast<WGPUDevice*>(userdata1) = device;
        };
        deviceCb.userdata1 = &_device;

        wgpuAdapterRequestDevice(_adapter, &deviceDesc, deviceCb);
        if (!_device) return Err<void>("Failed to create device");

        ydebug("AndroidWebGpuManager: Device created");
        return Ok();
    }

    void releaseCurrentTexture() {
        if (_currentTextureView) { wgpuTextureViewRelease(_currentTextureView); _currentTextureView = nullptr; }
        if (_currentTexture) { wgpuTextureRelease(_currentTexture); _currentTexture = nullptr; }
    }

    bool _headless = false;
    WGPUInstance _instance = nullptr;
    WGPUAdapter _adapter = nullptr;
    WGPUDevice _device = nullptr;
    WGPUQueue _queue = nullptr;
    WGPUSurface _surface = nullptr;
    WGPUTextureFormat _surfaceFormat = WGPUTextureFormat_RGBA8Unorm; // Android prefers RGBA
    WGPUTexture _currentTexture = nullptr;
    WGPUTextureView _currentTextureView = nullptr;
    uint32_t _surfaceWidth = 0;
    uint32_t _surfaceHeight = 0;
    ErrorCallback _errorCallback;
};

Result<WebGpuManager::Ptr> WebGpuManager::createImpl() {
    return Ok(Ptr(new AndroidWebGpuManager()));
}

} // namespace yetty

#endif // __ANDROID__
