#include <yetty/platform/surface-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !YETTY_IOS

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

        // Create cursors
        _cursors[static_cast<int>(CursorType::Arrow)] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _cursors[static_cast<int>(CursorType::IBeam)] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        _cursors[static_cast<int>(CursorType::Hand)] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        _cursors[static_cast<int>(CursorType::ResizeH)] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        _cursors[static_cast<int>(CursorType::ResizeV)] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        _cursors[static_cast<int>(CursorType::Hidden)] = nullptr;

        ydebug("GlfwSurfaceManager initialized");
        return Ok();
    }

    ~GlfwSurfaceManager() {
        for (int i = 0; i < 6; i++) {
            if (_cursors[i]) glfwDestroyCursor(_cursors[i]);
        }
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

    void setCursor(CursorType type) override {
        GLFWwindow* window = _windowSingleton->getWindow();
        if (!window) return;

        if (type == CursorType::Hidden) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GLFWcursor* cursor = _cursors[static_cast<int>(type)];
            if (cursor) glfwSetCursor(window, cursor);
        }
    }

    double getTime() const override {
        return glfwGetTime();
    }

    void requestRender() override {
        glfwPostEmptyEvent();
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        GLFWwindow* window = _windowSingleton->getWindow();
        if (!window) return nullptr;
        return glfwCreateWindowWGPUSurface(instance, window);
    }

private:
    GlfwWindowSingleton::Ptr _windowSingleton;
    GLFWcursor* _cursors[6] = {};
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
