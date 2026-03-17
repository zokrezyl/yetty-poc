#pragma once

#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <functional>

namespace yetty {

// WebGpuManager - platform-specific WebGPU initialization
//
// Handles the platform differences in WebGPU setup:
// - Linux: Dawn/Vulkan
// - macOS: Dawn/Metal
// - Windows: Dawn/D3D12 with D3D11 fallback for older GPUs
// - Android: Vulkan backend required, reject Null
// - WebASM: Async operations with emscripten_sleep
//
class WebGpuManager : public base::ThreadSingleton<WebGpuManager> {
public:
    using Ptr = std::shared_ptr<WebGpuManager>;
    using ErrorCallback = std::function<void(WGPUErrorType, const std::string&)>;

    static Result<Ptr> createImpl();

    virtual ~WebGpuManager() = default;

    // Initialize WebGPU: instance, adapter, device, queue
    // surfaceCreator is called to create the surface (from SurfaceManager)
    using SurfaceCreator = std::function<WGPUSurface(WGPUInstance)>;
    virtual Result<void> init(SurfaceCreator surfaceCreator, bool headless = false) = 0;

    // Accessors
    virtual WGPUInstance getInstance() const = 0;
    virtual WGPUAdapter getAdapter() const = 0;
    virtual WGPUDevice getDevice() const = 0;
    virtual WGPUQueue getQueue() const = 0;
    virtual WGPUSurface getSurface() const = 0;
    virtual WGPUTextureFormat getSurfaceFormat() const = 0;

    // Surface management
    virtual Result<void> configureSurface(uint32_t width, uint32_t height) = 0;
    virtual Result<WGPUTextureView> getCurrentTextureView() = 0;
    virtual void presentSurface() = 0;

    // Error handling
    virtual void setErrorCallback(ErrorCallback cb) = 0;

    // Cleanup
    virtual void shutdown() = 0;

protected:
    WebGpuManager() = default;
};

} // namespace yetty
