#include <yetty/platform/input-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include "../shared/glfw-window-singleton.h"
#include <yetty/base/event-loop.h>
#include <yetty/base/event-queue.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>

namespace yetty {

//=============================================================================
// GlfwInputManager
//
// Subscribes to raw input events from EventQueue and transforms them into
// internal events. Runs on RENDER THREAD.
//
// RAW -> INTERNAL TRANSFORMATIONS:
//
// 1. Ctrl+letter -> control character
//    GLFW does NOT fire charCallback for Ctrl+A, Ctrl+C, etc.
//    We detect keyDown with GLFW_MOD_CONTROL + letter and generate charInput:
//      Ctrl+A = 0x01, Ctrl+B = 0x02, ..., Ctrl+Z = 0x1A
//
//=============================================================================

class GlfwInputManager : public InputManager, public base::EventListener {
public:
    Result<void> init() {
        // Ensure window singleton exists
        auto singletonResult = GlfwWindowSingleton::instance();
        if (!singletonResult) {
            return Err<void>("GlfwWindowSingleton not available", singletonResult);
        }

        // Register as event listener for KeyDown to transform raw input events
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("EventLoop not available", loopResult);
        }
        auto res = (*loopResult)->registerListener(
            base::Event::Type::KeyDown,
            sharedAs<base::EventListener>(),
            100  // High priority - process before other listeners
        );
        if (!res) {
            return Err<void>("Failed to register InputManager as listener", res);
        }

        ydebug("GlfwInputManager initialized");
        return Ok();
    }

    /// @brief Handle raw input events and generate internal events
    ///
    /// Transforms:
    /// - KeyDown + Ctrl + letter -> charInput with control character
    ///
    /// @param event Raw event from EventQueue
    /// @return Ok(false) - don't consume, let other listeners process
    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::KeyDown) {
            int mods = event.key.mods;
            int key = event.key.key;

            // Ctrl+letter -> control character (0x01 - 0x1A)
            if ((mods & GLFW_MOD_CONTROL) && key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                unsigned int controlChar = key - GLFW_KEY_A + 1;
                auto queueResult = base::EventQueue::instance();
                if (queueResult) {
                    (*queueResult)->push(base::Event::charInput(controlChar));
                }
            }
        }
        return Ok(false);
    }
};

// Factory
Result<InputManager::Ptr> InputManager::createImpl() {
    auto impl = std::make_shared<GlfwInputManager>();
    auto initResult = impl->init();
    if (!initResult) {
        return Err<InputManager::Ptr>("GlfwInputManager init failed", initResult);
    }
    return Ok(impl);
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
