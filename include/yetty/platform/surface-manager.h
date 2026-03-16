#pragma once

#include <yetty/base/factory.h>
#include <webgpu/webgpu.h>
#include <string>

namespace yetty {

// SurfaceManager - window lifecycle and WebGPU surface
//
// Platform implementations:
// - glfw.cpp: GLFW window (Linux, macOS, Windows)
// - android.cpp: ANativeWindow
// - webasm.cpp: Emscripten canvas
//
class SurfaceManager : public base::ThreadSingleton<SurfaceManager> {
public:
    using Ptr = std::shared_ptr<SurfaceManager>;

    static Result<Ptr> createImpl();

    virtual ~SurfaceManager() = default;

    // Window properties
    virtual void getWindowSize(int& width, int& height) const = 0;
    virtual void getFramebufferSize(int& width, int& height) const = 0;
    virtual void getContentScale(float& xscale, float& yscale) const = 0;
    virtual bool shouldClose() const = 0;

    // Window decoration
    virtual void setTitle(const std::string& title) = 0;
    virtual void setIcon(const unsigned char* data, size_t size) = 0;

    // WebGPU surface
    virtual WGPUSurface createWGPUSurface(WGPUInstance instance) = 0;

protected:
    SurfaceManager() = default;
};

} // namespace yetty
