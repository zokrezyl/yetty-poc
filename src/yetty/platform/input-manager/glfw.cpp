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
// Subscribes to RAW input events from EventQueue (pushed by GlfwWindowSingleton)
// and transforms them into INTERNAL events that core components understand.
//
// This abstraction layer ensures uniform input handling across platforms:
// - GLFW/Desktop, Android, WebASM all generate different raw events
// - InputManager transforms them into unified internal events
//
// RAW -> INTERNAL TRANSFORMATIONS:
//
// Keyboard:
// - Char (raw) -> Char (internal) - forwarded with platform normalization
// - KeyDown + Ctrl/Alt + char -> Char with mods (GLFW doesn't fire charCallback for Ctrl+A)
// - KeyDown (special keys) -> KeyDown (forwarded)
//
// Mouse:
// - Scroll + Ctrl -> Zoom (change view scale)
// - Scroll + Ctrl+Shift -> HardZoom (change cell size)
// - Scroll (plain) -> VScroll/HScroll
// - MouseDown/Up/Move -> forwarded with normalization
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

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("EventLoop not available", loopResult);
        }
        auto loop = *loopResult;

        // Register for ALL raw input events with high priority
        // We process them first and dispatch internal events
        const int priority = 100;

        // Keyboard events
        loop->registerListener(base::Event::Type::Char, sharedAs<base::EventListener>(), priority);
        loop->registerListener(base::Event::Type::KeyDown, sharedAs<base::EventListener>(), priority);
        loop->registerListener(base::Event::Type::KeyUp, sharedAs<base::EventListener>(), priority);

        // Mouse events
        loop->registerListener(base::Event::Type::MouseDown, sharedAs<base::EventListener>(), priority);
        loop->registerListener(base::Event::Type::MouseUp, sharedAs<base::EventListener>(), priority);
        loop->registerListener(base::Event::Type::MouseMove, sharedAs<base::EventListener>(), priority);
        loop->registerListener(base::Event::Type::Scroll, sharedAs<base::EventListener>(), priority);

        ydebug("GlfwInputManager: registered for all raw input events");
        return Ok();
    }

    Result<bool> onEvent(const base::Event& event) override {
        switch (event.type) {
            case base::Event::Type::Char:
                // Char events pass through - already in correct format
                ydebug("InputManager: Char codepoint={} mods={}", event.chr.codepoint, event.chr.mods);
                return Ok(false);

            case base::Event::Type::KeyDown:
                return handleKeyDown(event);

            case base::Event::Type::KeyUp:
                // KeyUp passes through
                return Ok(false);

            case base::Event::Type::Scroll:
                return handleScroll(event);

            case base::Event::Type::MouseDown:
            case base::Event::Type::MouseUp:
            case base::Event::Type::MouseMove:
                // Mouse events pass through
                return Ok(false);

            default:
                return Ok(false);
        }
    }

private:
    //=========================================================================
    // Keyboard handling
    //=========================================================================

    Result<bool> handleKeyDown(const base::Event& event) {
        int mods = event.key.mods;
        int key = event.key.key;
        int scancode = event.key.scancode;

        ydebug("InputManager: KeyDown key={} mods={} scancode={}", key, mods, scancode);

        // Handle Ctrl/Alt + character combinations
        // GLFW does NOT fire charCallback for Ctrl+A, Ctrl+C, etc.
        // We need to generate the Char event ourselves
        if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
            // Special case for space - let it pass through as KeyDown
            if (key == GLFW_KEY_SPACE) {
                return Ok(false);
            }

            // Get the actual character from keyboard layout
            const char* keyName = glfwGetKeyName(key, scancode);
            if (keyName && keyName[0] != '\0' && keyName[1] == '\0') {
                // Single character key - dispatch as Char with mods
                uint32_t ch = static_cast<uint32_t>(static_cast<uint8_t>(keyName[0]));
                ydebug("InputManager: Ctrl/Alt+{} -> Char codepoint={} mods={}",
                       keyName[0], ch, mods);

                auto loopResult = base::EventLoop::instance();
                if (loopResult) {
                    (*loopResult)->dispatch(base::Event::charInputWithMods(ch, mods));
                }
                return Ok(true);  // Consumed - we generated a Char event instead
            }
        }

        // Let KeyDown pass through for other listeners
        return Ok(false);
    }

    //=========================================================================
    // Mouse/Scroll handling
    //=========================================================================

    Result<bool> handleScroll(const base::Event& event) {
        int mods = event.scroll.mods;
        float dx = event.scroll.dx;
        float dy = event.scroll.dy;

        ydebug("InputManager: Scroll dx={} dy={} mods={}", dx, dy, mods);

        // Ctrl+Scroll = Zoom (change view scale)
        if (mods & GLFW_MOD_CONTROL) {
            if (mods & GLFW_MOD_SHIFT) {
                ydebug("InputManager: HardZoom dy={}", dy);
                // TODO: Dispatch HardZoom event when event type is added
            } else {
                ydebug("InputManager: Zoom dy={}", dy);
                // TODO: Dispatch Zoom event when event type is added
            }
        } else if (mods & GLFW_MOD_SHIFT) {
            ydebug("InputManager: HScroll (Shift+Scroll) dx={}", dy);
        } else {
            ydebug("InputManager: VScroll dy={}", dy);
        }

        // Let scroll pass through - transformation to Zoom/HScroll can be added later
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
