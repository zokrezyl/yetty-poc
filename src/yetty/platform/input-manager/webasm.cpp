#include <yetty/platform/input-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/platform/event-loop.h>
#include <yetty/base/event.h>
#include <yetty/result.hpp>
#include <emscripten/html5.h>

namespace yetty {

class WebInputManager : public InputManager {
public:
    std::string getKeyName(int key, int scancode) const override {
        (void)scancode;
        if (key >= 'A' && key <= 'Z') return std::string(1, static_cast<char>(key));
        if (key >= '0' && key <= '9') return std::string(1, static_cast<char>(key));
        return "";
    }

    Result<void> init() {
        // Register Emscripten callbacks
        emscripten_set_keydown_callback("#canvas", this, true, keyCallback);
        emscripten_set_keyup_callback("#canvas", this, true, keyCallback);
        emscripten_set_mousedown_callback("#canvas", this, true, mouseCallback);
        emscripten_set_mouseup_callback("#canvas", this, true, mouseCallback);
        emscripten_set_mousemove_callback("#canvas", this, true, mouseMoveCallback);
        emscripten_set_wheel_callback("#canvas", this, true, wheelCallback);

        return Ok();
    }

private:
    // Track modifier state for scroll events
    bool _ctrlPressed = false;
    bool _shiftPressed = false;
    float _lastMouseX = 0.0f;
    float _lastMouseY = 0.0f;

    static int browserToGLFWKey(int keyCode) {
        switch (keyCode) {
            case 13: return 257;   // Enter
            case 8:  return 259;   // Backspace
            case 9:  return 258;   // Tab
            case 27: return 256;   // Escape
            case 38: return 265;   // ArrowUp
            case 40: return 264;   // ArrowDown
            case 37: return 263;   // ArrowLeft
            case 39: return 262;   // ArrowRight
            case 36: return 268;   // Home
            case 35: return 269;   // End
            case 33: return 266;   // PageUp
            case 34: return 267;   // PageDown
            case 45: return 260;   // Insert
            case 46: return 261;   // Delete
            case 112: return 290;  // F1
            case 113: return 291;  // F2
            case 114: return 292;  // F3
            case 115: return 293;  // F4
            case 116: return 294;  // F5
            case 117: return 295;  // F6
            case 118: return 296;  // F7
            case 119: return 297;  // F8
            case 120: return 298;  // F9
            case 121: return 299;  // F10
            case 122: return 300;  // F11
            case 123: return 301;  // F12
            case 16: return 340;   // Shift
            case 17: return 341;   // Control
            case 18: return 342;   // Alt
            default: return keyCode;
        }
    }

    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
        auto* self = static_cast<WebInputManager*>(userData);
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        // Track modifier state
        if (e->keyCode == 17) {
            self->_ctrlPressed = (eventType == EMSCRIPTEN_EVENT_KEYDOWN);
        }
        if (e->keyCode == 16) {
            self->_shiftPressed = (eventType == EMSCRIPTEN_EVENT_KEYDOWN);
        }

        int mods = 0;
        if (e->ctrlKey) mods |= 0x0002;
        if (e->shiftKey) mods |= 0x0001;
        if (e->altKey) mods |= 0x0004;

        int glfwKey = browserToGLFWKey(e->keyCode);

        if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
            loop->dispatch(base::Event::keyDown(glfwKey, mods, 0));

            // Generate Char event for printable keys
            // Ctrl+letter handled via charInputWithMods
            if (e->key[0] && !e->key[1]) {
                if (e->ctrlKey || e->altKey) {
                    // Ctrl/Alt + letter -> charInputWithMods
                    uint32_t ch = static_cast<uint32_t>(static_cast<uint8_t>(e->key[0]));
                    loop->dispatch(base::Event::charInputWithMods(ch, mods));
                } else {
                    loop->dispatch(base::Event::charInput(static_cast<uint32_t>(e->key[0])));
                }
            }
        } else {
            loop->dispatch(base::Event::keyUp(glfwKey, mods, 0));
        }

        return EM_TRUE;
    }

    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
        (void)userData;
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        int button = e->button;  // 0=left, 1=middle, 2=right
        float x = static_cast<float>(e->targetX);
        float y = static_cast<float>(e->targetY);

        if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
            loop->dispatch(base::Event::mouseDown(x, y, button, 0));
        } else {
            loop->dispatch(base::Event::mouseUp(x, y, button, 0));
        }

        return EM_TRUE;
    }

    static EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
        (void)eventType;
        auto* self = static_cast<WebInputManager*>(userData);
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        self->_lastMouseX = static_cast<float>(e->targetX);
        self->_lastMouseY = static_cast<float>(e->targetY);

        loop->dispatch(base::Event::mouseMove(self->_lastMouseX, self->_lastMouseY));
        return EM_TRUE;
    }

    static EM_BOOL wheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData) {
        (void)eventType;
        auto* self = static_cast<WebInputManager*>(userData);
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        int mods = 0;
        if (self->_ctrlPressed) mods |= 0x0002;
        if (self->_shiftPressed) mods |= 0x0001;

        // Normalize scroll values
        float dx = static_cast<float>(-e->deltaX / 100.0);
        float dy = static_cast<float>(-e->deltaY / 100.0);

        loop->dispatch(base::Event::scrollEvent(
            self->_lastMouseX, self->_lastMouseY, dx, dy, mods));

        return EM_TRUE;
    }
};

// Factory
Result<InputManager::Ptr> InputManager::createImpl() {
    auto mgr = new WebInputManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebInputManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
