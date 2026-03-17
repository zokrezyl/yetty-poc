#include <yetty/platform/surface-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include "../shared/glfw-window-singleton.h"
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace yetty {

class GlfwSurfaceManager : public SurfaceManager {
public:
    Result<void> init() {
        auto singletonResult = GlfwWindowSingleton::instance();
        if (!singletonResult) {
            return Err<void>("GlfwWindowSingleton not available", singletonResult);
        }
        _windowSingleton = *singletonResult;
        ydebug("GlfwSurfaceManager initialized");
        return Ok();
    }

    void getWindowSize(int& width, int& height) const override {
        _windowSingleton->getWindowSize(width, height);
    }

    void getFramebufferSize(int& width, int& height) const override {
        _windowSingleton->getFramebufferSize(width, height);
    }

    void getContentScale(float& xscale, float& yscale) const override {
        _windowSingleton->getContentScale(xscale, yscale);
    }

    bool shouldClose() const override {
        return _windowSingleton->shouldClose();
    }

    void setTitle(const std::string& title) override {
        _windowSingleton->setTitle(title);
    }

    void setIcon(const unsigned char* data, size_t size) override {
        _windowSingleton->setIcon(data, size);
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        GLFWwindow* window = _windowSingleton->getWindow();
        if (!window) return nullptr;
        return glfwCreateWindowWGPUSurface(instance, window);
    }

private:
    GlfwWindowSingleton::Ptr _windowSingleton;
};

// Factory
Result<SurfaceManager::Ptr> SurfaceManager::createImpl() {
    auto mgr = new GlfwSurfaceManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("GlfwSurfaceManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
