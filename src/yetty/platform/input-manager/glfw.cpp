#include <yetty/platform/input-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include "../shared/glfw-window-singleton.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

// GlfwInputManager - minimal implementation
//
// The actual input handling is done by GlfwWindowSingleton:
// - Singleton runs glfwWaitEvents() on main thread
// - GLFW callbacks push events to EventQueue
// - Render thread processes events from EventQueue
//
// This manager just ensures the singleton exists.
//
class GlfwInputManager : public InputManager {
public:
    Result<void> init() {
        // Ensure window singleton exists (creates window if needed)
        auto singletonResult = GlfwWindowSingleton::instance();
        if (!singletonResult) {
            return Err<void>("GlfwWindowSingleton not available", singletonResult);
        }
        ydebug("GlfwInputManager initialized");
        return Ok();
    }
};

// Factory
Result<InputManager::Ptr> InputManager::createImpl() {
    auto impl = Ptr(new GlfwInputManager());
    auto initResult = static_cast<GlfwInputManager*>(impl.get())->init();
    if (!initResult) {
        return Err<Ptr>("GlfwInputManager init failed", initResult);
    }
    return Ok(std::move(impl));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
