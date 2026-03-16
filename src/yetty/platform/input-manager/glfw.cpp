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
// 1. Ctrl/Alt + character -> charInputWithMods
//    GLFW does NOT fire charCallback for Ctrl+A, Ctrl+C, etc.
//    We detect keyDown with GLFW_MOD_CONTROL/ALT and use glfwGetKeyName()
//    to get the actual character (keyboard layout aware), then dispatch
//    charInputWithMods(ch, mods).
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
    /// - KeyDown + Ctrl/Alt + character -> charInputWithMods
    ///
    /// Uses glfwGetKeyName() to get the actual character (keyboard layout aware).
    /// This matches how the old Platform handled Ctrl+key combinations.
    ///
    /// @param event Raw event from EventQueue
    /// @return Ok(false) - don't consume, let other listeners process
    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::KeyDown) {
            int mods = event.key.mods;
            int key = event.key.key;
            int scancode = event.key.scancode;
            ydebug("InputManager: KeyDown key={} mods={} scancode={}", key, mods, scancode);

            // Handle Ctrl/Alt + character combinations
            // GLFW_MOD_CONTROL = 0x0002, GLFW_MOD_ALT = 0x0004
            if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
                ydebug("InputManager: Ctrl/Alt detected, key={} mods={}", key, mods);

                // Special case for space (GLFW_KEY_SPACE = 32)
                if (key == GLFW_KEY_SPACE) {
                    // Space with modifier - let keyDown pass through
                    return Ok(false);
                }

                // Get the actual character from keyboard layout
                const char* keyName = glfwGetKeyName(key, scancode);
                ydebug("InputManager: glfwGetKeyName returned '{}'", keyName ? keyName : "(null)");
                if (keyName && keyName[0] != '\0' && keyName[1] == '\0') {
                    // Single character key - dispatch as charInputWithMods
                    uint32_t ch = static_cast<uint32_t>(static_cast<uint8_t>(keyName[0]));
                    ydebug("InputManager: Ctrl/Alt+{} -> charInputWithMods ch={} mods={}",
                           keyName[0], ch, mods);
                    auto queueResult = base::EventQueue::instance();
                    if (queueResult) {
                        (*queueResult)->push(base::Event::charInputWithMods(ch, mods));
                    }
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
