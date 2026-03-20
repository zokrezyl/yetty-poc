#include <yetty/platform/webgpu-manager.h>

#if defined(__APPLE__)

#include <yetty/result.hpp>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

class MacosWebGpuManager : public WebGpuManager {
public:
    ~MacosWebGpuManager() override {
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
        ydebug("MacosWebGpuManager: Instance created");

        // Create surface
        if (!headless && surfaceCreator) {
            _surface = surfaceCreator(_instance);
            if (_surface) {
                ydebug("MacosWebGpuManager: Surface created");
            } else {
                ywarn("MacosWebGpuManager: Surface creation failed");
            }
        }

        // Request adapter - macOS uses Metal via Dawn
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

        ydebug("MacosWebGpuManager: Requesting adapter...");
        wgpuInstanceRequestAdapter(_instance, &adapterOpts, adapterCb);

        if (!_adapter) {
            return Err<void>("Failed to get WebGPU adapter");
        }

        logAdapterInfo();

        if (auto res = requestDevice(); !res) {
            return res;
        }

        _queue = wgpuDeviceGetQueue(_device);
        ydebug("MacosWebGpuManager: Initialization complete");

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
    void logAdapterInfo() {
        WGPUAdapterInfo info = {};
        if (wgpuAdapterGetInfo(_adapter, &info) == WGPUStatus_Success) {
            auto sv = [](WGPUStringView s) { return (s.data && s.length > 0) ? std::string(s.data, s.length) : "(unknown)"; };
            ydebug("GPU: {} ({}) Backend: {}", sv(info.device), sv(info.vendor), static_cast<int>(info.backendType));
            wgpuAdapterInfoFreeMembers(info);
        }
    }

    Result<void> requestDevice() {
        WGPULimits adapterLimits = {};
        wgpuAdapterGetLimits(_adapter, &adapterLimits);

        WGPULimits limits = {};
        limits.maxTextureDimension2D = std::min(16384u, adapterLimits.maxTextureDimension2D);
        limits.maxStorageBuffersPerShaderStage = 10;
        limits.maxStorageBufferBindingSize = std::min(static_cast<uint64_t>(512 * 1024 * 1024), adapterLimits.maxStorageBufferBindingSize);
        limits.maxBufferSize = std::min(static_cast<uint64_t>(1024 * 1024 * 1024), adapterLimits.maxBufferSize);

        WGPUDeviceDescriptor deviceDesc = {};
        deviceDesc.label = WGPU_STR("yetty device");
        deviceDesc.requiredLimits = &limits;
        deviceDesc.defaultQueue.label = WGPU_STR("default queue");
        deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void* userdata1, void*) {
            auto* self = static_cast<MacosWebGpuManager*>(userdata1);
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

        ydebug("MacosWebGpuManager: Device created");
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
    WGPUTextureFormat _surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
    WGPUTexture _currentTexture = nullptr;
    WGPUTextureView _currentTextureView = nullptr;
    uint32_t _surfaceWidth = 0;
    uint32_t _surfaceHeight = 0;
    ErrorCallback _errorCallback;
};

Result<WebGpuManager::Ptr> WebGpuManager::createImpl() {
    return Ok(Ptr(new MacosWebGpuManager()));
}

} // namespace yetty

#endif // __APPLE__
