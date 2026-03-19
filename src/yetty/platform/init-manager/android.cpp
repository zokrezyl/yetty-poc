#include <yetty/platform/init-manager.h>

#if defined(__ANDROID__)

#include "../shared/android-app-singleton.h"
#include <yetty/base/event-queue.h>
#include <yetty/base/event.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <android/looper.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android_native_app_glue.h>
#include <thread>
#include <atomic>
#include <cmath>

namespace yetty {

class AndroidInitManager : public InitManager {
public:
    Result<void> init(android_app* app) {
        _app = app;

        // Initialize AndroidAppSingleton with the app pointer
        auto singletonResult = AndroidAppSingleton::instance();
        if (!singletonResult) {
            return Err<void>("Failed to create AndroidAppSingleton", singletonResult);
        }
        (*singletonResult)->setApp(app);

        // Set up callbacks
        _app->onAppCmd = handleAppCmd;
        _app->onInputEvent = handleInputEvent;

        ydebug("AndroidInitManager: initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("AndroidInitManager::run - waiting for window");

        // Wait for window to be ready (blocking poll)
        while (!_app->destroyRequested && !_app->window) {
            int events;
            android_poll_source* source;
            if (ALooper_pollAll(-1, nullptr, &events, (void**)&source) >= 0) {
                if (source) {
                    source->process(_app, source);
                }
            }
        }

        if (_app->destroyRequested) {
            ydebug("AndroidInitManager::run - destroy requested before window ready");
            return;
        }

        // Set window in singleton
        if (auto singleton = AndroidAppSingleton::instance(); singleton) {
            (*singleton)->setWindow(_app->window);
        }
        ydebug("AndroidInitManager::run - window ready");

        // Spawn render thread
        ydebug("AndroidInitManager::run - spawning render thread");
        std::thread renderThread([this, renderThreadFunc]() {
            ydebug("Render thread started");
            renderThreadFunc();
            ydebug("Render thread finished, stopping main loop");
            _running = false;
            ALooper_wake(ALooper_forThread());
        });

        // Run blocking ALooper event loop on main thread
        ydebug("AndroidInitManager::run - starting ALooper event loop");
        _running = true;
        while (_running && !_app->destroyRequested) {
            int events;
            android_poll_source* source;
            if (ALooper_pollAll(-1, nullptr, &events, (void**)&source) >= 0) {
                if (source) {
                    source->process(_app, source);
                }
            }
        }
        ydebug("AndroidInitManager::run - ALooper event loop ended");

        renderThread.join();
        ydebug("AndroidInitManager::run - render thread joined");
    }

private:
    // App command callback - handles window lifecycle
    static void handleAppCmd(android_app* app, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                ydebug("APP_CMD_INIT_WINDOW: window={}", static_cast<void*>(app->window));
                if (auto singleton = AndroidAppSingleton::instance(); singleton) {
                    (*singleton)->setWindow(app->window);
                }
                break;
            case APP_CMD_TERM_WINDOW:
                ydebug("APP_CMD_TERM_WINDOW");
                if (auto singleton = AndroidAppSingleton::instance(); singleton) {
                    (*singleton)->setWindow(nullptr);
                }
                break;
            case APP_CMD_GAINED_FOCUS:
                ydebug("APP_CMD_GAINED_FOCUS");
                break;
            case APP_CMD_LOST_FOCUS:
                ydebug("APP_CMD_LOST_FOCUS");
                break;
        }
    }

    // Input callback - translates Android events to Event structs and pushes to EventQueue
    static int32_t handleInputEvent(android_app* app, AInputEvent* event) {
        (void)app;

        auto queueResult = base::EventQueue::instance();
        if (!queueResult) {
            ydebug("handleInputEvent: EventQueue not available yet");
            return 0;
        }
        auto queue = *queueResult;

        int32_t eventType = AInputEvent_getType(event);

        if (eventType == AINPUT_EVENT_TYPE_MOTION) {
            handleMotionEvent(queue, event);
        } else if (eventType == AINPUT_EVENT_TYPE_KEY) {
            handleKeyEvent(queue, event);
        }

        return 1;
    }

    // Translate Android motion events to mouse/scroll events
    static void handleMotionEvent(base::EventQueue::Ptr queue, AInputEvent* event) {
        int32_t action = AMotionEvent_getAction(event);
        int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
        size_t pointerCount = AMotionEvent_getPointerCount(event);

        // Two-finger pinch gesture
        if (pointerCount == 2) {
            float x0 = AMotionEvent_getX(event, 0);
            float y0 = AMotionEvent_getY(event, 0);
            float x1 = AMotionEvent_getX(event, 1);
            float y1 = AMotionEvent_getY(event, 1);

            float dx = x1 - x0;
            float dy = y1 - y0;
            float distance = std::sqrt(dx * dx + dy * dy);
            float centerX = (x0 + x1) * 0.5f;
            float centerY = (y0 + y1) * 0.5f;

            if (!_pinchActive) {
                _pinchActive = true;
                _lastPinchDistance = distance;
                _lastPinchCenterX = centerX;
                _lastPinchCenterY = centerY;
            } else if (actionMasked == AMOTION_EVENT_ACTION_MOVE) {
                // Pinch zoom: distance change -> scroll with Ctrl mod (mods=2)
                float distanceDelta = distance - _lastPinchDistance;
                if (std::abs(distanceDelta) > 2.0f) {
                    float scrollDelta = distanceDelta * 0.02f;
                    queue->push(base::Event::scrollEvent(centerX, centerY, 0.0f, scrollDelta, 2));
                    _lastPinchDistance = distance;
                }

                // Pan while pinched (2-finger drag)
                float panDx = centerX - _lastPinchCenterX;
                float panDy = centerY - _lastPinchCenterY;
                if (std::abs(panDx) > 2.0f || std::abs(panDy) > 2.0f) {
                    queue->push(base::Event::scrollEvent(centerX, centerY, -panDx * 0.1f, -panDy * 0.1f, 0));
                    _lastPinchCenterX = centerX;
                    _lastPinchCenterY = centerY;
                }
            }
            return;
        }

        // End pinch when pointer lifted
        if (_pinchActive && (actionMasked == AMOTION_EVENT_ACTION_UP ||
                            actionMasked == AMOTION_EVENT_ACTION_POINTER_UP ||
                            actionMasked == AMOTION_EVENT_ACTION_CANCEL)) {
            _pinchActive = false;
        }

        // Single touch -> mouse events
        if (pointerCount == 1 && !_pinchActive) {
            float x = AMotionEvent_getX(event, 0);
            float y = AMotionEvent_getY(event, 0);

            switch (actionMasked) {
                case AMOTION_EVENT_ACTION_DOWN:
                    queue->push(base::Event::mouseDown(x, y, 0, 0));
                    queue->push(base::Event::mouseMove(x, y, 0));
                    break;
                case AMOTION_EVENT_ACTION_UP:
                    queue->push(base::Event::mouseUp(x, y, 0, 0));
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                    queue->push(base::Event::mouseMove(x, y, 0));
                    break;
            }
        }
    }

    // Translate Android key events to key/char events
    static void handleKeyEvent(base::EventQueue::Ptr queue, AInputEvent* event) {
        int32_t action = AKeyEvent_getAction(event);
        int32_t keyCode = AKeyEvent_getKeyCode(event);
        int32_t metaState = AKeyEvent_getMetaState(event);

        int platformKey = translateKeyCode(keyCode);
        bool pressed = (action == AKEY_EVENT_ACTION_DOWN);

        // Build modifier flags (GLFW-compatible: shift=1, ctrl=2, alt=4)
        int mods = 0;
        if (metaState & AMETA_SHIFT_ON) mods |= 1;
        if (metaState & AMETA_CTRL_ON) mods |= 2;
        if (metaState & AMETA_ALT_ON) mods |= 4;

        if (pressed) {
            // For Ctrl/Alt + printable char, send charInputWithMods ONLY (not keyDown)
            if (mods & (2 | 4)) {  // Ctrl or Alt
                uint32_t codepoint = translateToCodepoint(keyCode, metaState);
                if (codepoint) {
                    queue->push(base::Event::charInputWithMods(codepoint, mods));
                    return;
                }
            }
            // For special keys or no Ctrl/Alt, send keyDown
            queue->push(base::Event::keyDown(platformKey, mods, 0));
            // Normal character input (no Ctrl/Alt)
            uint32_t codepoint = translateToCodepoint(keyCode, metaState);
            if (codepoint) {
                queue->push(base::Event::charInputWithMods(codepoint, mods));
            }
        } else {
            queue->push(base::Event::keyUp(platformKey, mods, 0));
        }
    }

    // Translate Android keycode to GLFW-compatible key code
    static int translateKeyCode(int keyCode) {
        switch (keyCode) {
            case AKEYCODE_ENTER: return 257;       // GLFW_KEY_ENTER
            case AKEYCODE_TAB: return 258;         // GLFW_KEY_TAB
            case AKEYCODE_DEL: return 259;         // GLFW_KEY_BACKSPACE
            case AKEYCODE_FORWARD_DEL: return 261; // GLFW_KEY_DELETE
            case AKEYCODE_DPAD_RIGHT: return 262;  // GLFW_KEY_RIGHT
            case AKEYCODE_DPAD_LEFT: return 263;   // GLFW_KEY_LEFT
            case AKEYCODE_DPAD_DOWN: return 264;   // GLFW_KEY_DOWN
            case AKEYCODE_DPAD_UP: return 265;     // GLFW_KEY_UP
            case AKEYCODE_PAGE_UP: return 266;     // GLFW_KEY_PAGE_UP
            case AKEYCODE_PAGE_DOWN: return 267;   // GLFW_KEY_PAGE_DOWN
            case AKEYCODE_MOVE_HOME: return 268;   // GLFW_KEY_HOME
            case AKEYCODE_MOVE_END: return 269;    // GLFW_KEY_END
            case AKEYCODE_ESCAPE: return 256;      // GLFW_KEY_ESCAPE
            case AKEYCODE_SPACE: return 32;
            default:
                if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
                    return 'A' + (keyCode - AKEYCODE_A);
                }
                if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
                    return '0' + (keyCode - AKEYCODE_0);
                }
                return keyCode;
        }
    }

    // Translate Android keycode + meta state to Unicode codepoint
    // Returns the actual character (not control character) - mods are passed separately
    static uint32_t translateToCodepoint(int keyCode, int metaState) {
        bool shift = (metaState & AMETA_SHIFT_ON);

        // Letters - return actual letter, not control character
        // Ctrl modifier is passed separately via mods parameter
        if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
            return shift ? ('A' + (keyCode - AKEYCODE_A)) : ('a' + (keyCode - AKEYCODE_A));
        }

        // Numbers
        if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
            if (shift) {
                const char* symbols = ")!@#$%^&*(";
                return static_cast<uint32_t>(symbols[keyCode - AKEYCODE_0]);
            }
            return '0' + (keyCode - AKEYCODE_0);
        }

        // Special characters
        switch (keyCode) {
            case AKEYCODE_SPACE: return ' ';
            case AKEYCODE_ENTER: return '\r';
            case AKEYCODE_TAB: return '\t';
            case AKEYCODE_MINUS: return shift ? '_' : '-';
            case AKEYCODE_EQUALS: return shift ? '+' : '=';
            case AKEYCODE_LEFT_BRACKET: return shift ? '{' : '[';
            case AKEYCODE_RIGHT_BRACKET: return shift ? '}' : ']';
            case AKEYCODE_BACKSLASH: return shift ? '|' : '\\';
            case AKEYCODE_SEMICOLON: return shift ? ':' : ';';
            case AKEYCODE_APOSTROPHE: return shift ? '"' : '\'';
            case AKEYCODE_GRAVE: return shift ? '~' : '`';
            case AKEYCODE_COMMA: return shift ? '<' : ',';
            case AKEYCODE_PERIOD: return shift ? '>' : '.';
            case AKEYCODE_SLASH: return shift ? '?' : '/';
        }
        return 0;
    }

    android_app* _app = nullptr;
    std::atomic<bool> _running{false};

    // Pinch gesture state (static for use in static callbacks)
    static inline bool _pinchActive = false;
    static inline float _lastPinchDistance = 0.0f;
    static inline float _lastPinchCenterX = 0.0f;
    static inline float _lastPinchCenterY = 0.0f;
};

// Factory
Result<InitManager::Ptr> InitManager::createImpl() {
    return Err<Ptr>("Use createForAndroid(android_app*) on Android");
}

// Android-specific factory
Result<InitManager::Ptr> createForAndroid(android_app* app) {
    auto mgr = new AndroidInitManager();
    if (auto res = mgr->init(app); !res) {
        delete mgr;
        return Err<InitManager::Ptr>("AndroidInitManager init failed", res);
    }
    return Ok(InitManager::Ptr(mgr));
}

} // namespace yetty

#endif // __ANDROID__
