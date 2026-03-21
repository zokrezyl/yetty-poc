#include <yetty/platform/init-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/yetty.h>
#include <yetty/platform/event-loop.h>
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

    Result<void> run(int argc, char** argv) override {
        ydebug("WebInitManager: starting");
        _running = true;

        // Create Config first
        auto configResult = Config::create(argc, argv);
        if (!configResult) {
            return Err<void>("Failed to create Config", configResult);
        }
        auto config = *configResult;

        // Create Yetty with Config
        auto yettyResult = Yetty::create(config);
        if (!yettyResult) {
            return Err<void>("Failed to create Yetty", yettyResult);
        }
        _yettyInstance = *yettyResult;
        ydebug("Yetty created");

        // Register input callbacks BEFORE run() - yettyInstance->run() never returns on webasm
        // WebASM is single-threaded, dispatch directly to EventLoop (no EventQueue needed)
        emscripten_set_keydown_callback("#canvas", nullptr, true, keyCallback);
        emscripten_set_keyup_callback("#canvas", nullptr, true, keyCallback);
        emscripten_set_mousedown_callback("#canvas", nullptr, true, mouseCallback);
        emscripten_set_mouseup_callback("#canvas", nullptr, true, mouseCallback);
        emscripten_set_mousemove_callback("#canvas", nullptr, true, mouseMoveCallback);
        emscripten_set_wheel_callback("#canvas", nullptr, true, wheelCallback);
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, resizeCallback);
        ydebug("WebInitManager: input callbacks registered");

        // Run yetty - sets up RAF render loop (NEVER RETURNS on webasm)
        ydebug("WebInitManager: starting yetty->run() (will not return)");
        auto runResult = _yettyInstance->run();
        if (!runResult) {
            return Err<void>("Yetty run failed", runResult);
        }
        return Ok();
    }

private:
    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void*) {
        ydebug("WebInitManager::keyCallback: eventType={} keyCode={} key={}", eventType, e->keyCode, e->key);
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            ydebug("WebInitManager::keyCallback: EventLoop not available");
            return EM_TRUE;
        }
        auto loop = *loopResult;

        int mods = 0;
        if (e->ctrlKey) mods |= 2;   // GLFW_MOD_CONTROL
        if (e->shiftKey) mods |= 1;  // GLFW_MOD_SHIFT
        if (e->altKey) mods |= 4;    // GLFW_MOD_ALT

        int glfwKey = browserToGLFWKey(e->keyCode);
        bool pressed = (eventType == EMSCRIPTEN_EVENT_KEYDOWN);

        if (pressed) {
            // Ctrl/Alt + character: handle specially in keyDown handler (matches desktop)
            ydebug("WebInitManager: dispatching keyDown glfwKey={} mods={}", glfwKey, mods);
            auto keyResult = loop->dispatch(base::Event::keyDown(glfwKey, mods, 0));
            ydebug("WebInitManager: keyDown dispatch result={}", keyResult.has_value() ? (*keyResult ? "handled" : "not handled") : "error");

            // Char input for printable keys (single character, no Ctrl/Alt)
            // Matches main branch: Platform callback fires charCallback separately
            if (e->key[0] && !e->key[1] && !(e->ctrlKey) && !(e->altKey)) {
                uint32_t ch = static_cast<uint32_t>(e->key[0]);
                ydebug("WebInitManager: dispatching charInput ch={} ('{}')", ch, static_cast<char>(ch));
                auto charResult = loop->dispatch(base::Event::charInput(ch));
                ydebug("WebInitManager: charInput dispatch result={}", charResult.has_value() ? (*charResult ? "handled" : "not handled") : "error");
            }
        } else {
            loop->dispatch(base::Event::keyUp(glfwKey, mods, 0));
        }
        return EM_TRUE;
    }

    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void*) {
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

    static EM_BOOL mouseMoveCallback(int, const EmscriptenMouseEvent* e, void*) {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        float x = static_cast<float>(e->targetX);
        float y = static_cast<float>(e->targetY);
        loop->dispatch(base::Event::mouseMove(x, y, 0));
        return EM_TRUE;
    }

    static EM_BOOL wheelCallback(int, const EmscriptenWheelEvent* e, void*) {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return EM_TRUE;
        auto loop = *loopResult;

        float x = static_cast<float>(e->mouse.targetX);
        float y = static_cast<float>(e->mouse.targetY);
        // Normalize wheel delta to match GLFW's ~1.0 per scroll notch
        // Browser deltaMode: 0=pixel (~100/notch), 1=line (~3/notch), 2=page
        float dx = static_cast<float>(-e->deltaX);
        float dy = static_cast<float>(-e->deltaY);
        switch (e->deltaMode) {
            case 0: dx /= 100.0f; dy /= 100.0f; break;  // Pixel
            case 1: dx /= 3.0f; dy /= 3.0f; break;      // Line
        }

        int mods = 0;
        if (e->mouse.ctrlKey) mods |= 2;
        if (e->mouse.shiftKey) mods |= 1;

        loop->dispatch(base::Event::scrollEvent(x, y, dx, dy, mods));
        return EM_TRUE;
    }

    static EM_BOOL resizeCallback(int, const EmscriptenUiEvent*, void*) {
        // Get actual canvas container size (not window inner size)
        int width = EM_ASM_INT({
            var container = document.getElementById('canvas-container');
            return container ? Math.floor(container.getBoundingClientRect().width) : window.innerWidth;
        });
        int height = EM_ASM_INT({
            var container = document.getElementById('canvas-container');
            return container ? Math.floor(container.getBoundingClientRect().height) : window.innerHeight;
        });

        if (width <= 0 || height <= 0) return EM_TRUE;

        // Update canvas element size
        emscripten_set_canvas_element_size("#canvas", width, height);

        // Dispatch resize event directly to EventLoop
        auto loopResult = base::EventLoop::instance();
        if (loopResult) {
            (*loopResult)->dispatch(base::Event::resizeEvent(width, height));
        }
        return EM_TRUE;
    }

    Yetty::Ptr _yettyInstance;
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
