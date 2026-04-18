#include <yetty/platform/clipboard-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !YETTY_IOS

#include "../shared/glfw-window-singleton.h"
#include <yetty/result.hpp>
#include <GLFW/glfw3.h>

namespace yetty {

class GlfwClipboardManager : public ClipboardManager {
public:
    Result<void> init() {
        // Ensure window singleton exists (needed for GLFW clipboard functions)
        auto windowResult = GlfwWindowSingleton::instance();
        if (!windowResult) {
            return Err<void>("GlfwWindowSingleton not available", windowResult);
        }
        _windowSingleton = *windowResult;
        return Ok();
    }

    std::string getText() const override {
        GLFWwindow* window = _windowSingleton->getWindow();
        if (!window) return "";

        const char* text = glfwGetClipboardString(window);
        return text ? std::string(text) : "";
    }

    void setText(const std::string& text) override {
        GLFWwindow* window = _windowSingleton->getWindow();
        if (window) {
            glfwSetClipboardString(window, text.c_str());
        }
    }

private:
    GlfwWindowSingleton::Ptr _windowSingleton;
};

// Factory
Result<ClipboardManager::Ptr> ClipboardManager::createImpl() {
    auto mgr = new GlfwClipboardManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("GlfwClipboardManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
