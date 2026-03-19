#include <yetty/platform/init-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/base/event-queue.h>
#include <yetty/base/event.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace yetty {

// Map browser keyCode to GLFW key code
static int browserToGLFWKey(int keyCode) {
    switch (keyCode) {
        case 13: return 257;   // Enter -> GLFW_KEY_ENTER
        case 8:  return 259;   // Backspace -> GLFW_KEY_BACKSPACE
        case 9:  return 258;   // Tab -> GLFW_KEY_TAB
        case 27: return 256;   // Escape -> GLFW_KEY_ESCAPE
        case 38: return 265;   // ArrowUp -> GLFW_KEY_UP
        case 40: return 264;   // ArrowDown -> GLFW_KEY_DOWN
        case 37: return 263;   // ArrowLeft -> GLFW_KEY_LEFT
        case 39: return 262;   // ArrowRight -> GLFW_KEY_RIGHT
        case 36: return 268;   // Home -> GLFW_KEY_HOME
        case 35: return 269;   // End -> GLFW_KEY_END
        case 33: return 266;   // PageUp -> GLFW_KEY_PAGE_UP
        case 34: return 267;   // PageDown -> GLFW_KEY_PAGE_DOWN
        case 45: return 260;   // Insert -> GLFW_KEY_INSERT
        case 46: return 261;   // Delete -> GLFW_KEY_DELETE
        case 112: return 290;  // F1 -> GLFW_KEY_F1
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
        case 16: return 340;   // Shift -> GLFW_KEY_LEFT_SHIFT
        case 17: return 341;   // Control -> GLFW_KEY_LEFT_CONTROL
        case 18: return 342;   // Alt -> GLFW_KEY_LEFT_ALT
        default: return keyCode; // Pass through for letters/numbers
    }
}

class WebInitManager : public InitManager {
public:
    Result<void> init() {
        ydebug("WebInitManager: initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("WebInitManager: starting");

        _renderFunc = std::move(renderThreadFunc);
        _running = true;

        // Register input callbacks - push events to EventQueue
        emscripten_set_keydown_callback("#canvas", nullptr, true, keyCallback);
        emscripten_set_keyup_callback("#canvas", nullptr, true, keyCallback);
        emscripten_set_mousedown_callback("#canvas", nullptr, true, mouseCallback);
        emscripten_set_mouseup_callback("#canvas", nullptr, true, mouseCallback);
        emscripten_set_mousemove_callback("#canvas", nullptr, true, mouseMoveCallback);
        emscripten_set_wheel_callback("#canvas", nullptr, true, wheelCallback);
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, resizeCallback);
        ydebug("WebInitManager: input callbacks registered");

        // WebASM is single-threaded - call render func directly
        // It will set up requestAnimationFrame internally
        if (_renderFunc) {
            _renderFunc();
        }

        // Note: On web, run() returns immediately
        // The browser's event loop drives everything via RAF
        ydebug("WebInitManager: run() returning (browser takes over)");
    }

private:
    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void*) {
        ydebug("WebInitManager::keyCallback: eventType={} keyCode={} key={}", eventType, e->keyCode, e->key);
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) {
            ydebug("WebInitManager::keyCallback: EventQueue not available");
            return EM_TRUE;
        }
        auto queue = *queueResult;

        int mods = 0;
        if (e->ctrlKey) mods |= 2;   // GLFW_MOD_CONTROL
        if (e->shiftKey) mods |= 1;  // GLFW_MOD_SHIFT
        if (e->altKey) mods |= 4;    // GLFW_MOD_ALT

        int glfwKey = browserToGLFWKey(e->keyCode);
        bool pressed = (eventType == EMSCRIPTEN_EVENT_KEYDOWN);

        if (pressed) {
            // For Ctrl/Alt + printable char, send charInputWithMods ONLY
            if ((mods & (2 | 4)) && e->key[0] && !e->key[1]) {
                queue->push(base::Event::charInputWithMods(static_cast<uint32_t>(e->key[0]), mods));
                return EM_TRUE;
            }
            queue->push(base::Event::keyDown(glfwKey, mods, 0));
            // Normal character input (no Ctrl/Alt)
            if (e->key[0] && !e->key[1] && !(mods & (2 | 4))) {
                queue->push(base::Event::charInputWithMods(static_cast<uint32_t>(e->key[0]), mods));
            }
        } else {
            queue->push(base::Event::keyUp(glfwKey, mods, 0));
        }
        return EM_TRUE;
    }

    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void*) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return EM_TRUE;
        auto queue = *queueResult;

        int button = e->button;  // 0=left, 1=middle, 2=right
        float x = static_cast<float>(e->targetX);
        float y = static_cast<float>(e->targetY);

        if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN) {
            queue->push(base::Event::mouseDown(x, y, button, 0));
        } else {
            queue->push(base::Event::mouseUp(x, y, button, 0));
        }
        return EM_TRUE;
    }

    static EM_BOOL mouseMoveCallback(int, const EmscriptenMouseEvent* e, void*) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return EM_TRUE;
        auto queue = *queueResult;

        float x = static_cast<float>(e->targetX);
        float y = static_cast<float>(e->targetY);
        queue->push(base::Event::mouseMove(x, y, 0));
        return EM_TRUE;
    }

    static EM_BOOL wheelCallback(int, const EmscriptenWheelEvent* e, void*) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return EM_TRUE;
        auto queue = *queueResult;

        float x = static_cast<float>(e->mouse.targetX);
        float y = static_cast<float>(e->mouse.targetY);
        float dx = static_cast<float>(-e->deltaX * 0.01);
        float dy = static_cast<float>(-e->deltaY * 0.01);

        int mods = 0;
        if (e->mouse.ctrlKey) mods |= 2;
        if (e->mouse.shiftKey) mods |= 1;

        queue->push(base::Event::scrollEvent(x, y, dx, dy, mods));
        return EM_TRUE;
    }

    static EM_BOOL resizeCallback(int, const EmscriptenUiEvent*, void*) {
        // Get actual canvas container size (not window inner size)
        int width = EM_ASM_INT({
            var container = document.getElementById('canvas-container');
            return container ? Math.floor(container.getBoundingClientRect().width) : 0;
        });
        int height = EM_ASM_INT({
            var container = document.getElementById('canvas-container');
            return container ? Math.floor(container.getBoundingClientRect().height) : 0;
        });

        if (width <= 0 || height <= 0) return EM_TRUE;

        // Update canvas element size
        emscripten_set_canvas_element_size("#canvas", width, height);

        // Push resize event
        auto queueResult = base::EventQueue::instance();
        if (queueResult) {
            (*queueResult)->push(base::Event::resizeEvent(width, height));
        }
        return EM_TRUE;
    }

    RenderThreadFunc _renderFunc;
    bool _running = false;
};

// Factory
Result<InitManager::Ptr> InitManager::createImpl() {
    auto mgr = new WebInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
