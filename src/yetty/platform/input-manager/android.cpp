#include <yetty/platform/input-manager.h>

#if defined(__ANDROID__)
#include "../shared/android-app-singleton.h"
#include <yetty/platform/event-loop.h>
#include <yetty/base/event.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android/log.h>
#include <cmath>

namespace yetty {

class AndroidInputManager : public InputManager {
public:
    std::string getKeyName(int key, int scancode) const override {
        (void)scancode;
        if (key >= 'A' && key <= 'Z') return std::string(1, static_cast<char>(key));
        if (key >= '0' && key <= '9') return std::string(1, static_cast<char>(key));
        return "";
    }

    Result<void> init() {
        // Register ourselves to receive input events from android_main
        // This is done via a global function pointer that android_main calls
        g_instance = this;
        return Ok();
    }

    // Called from android_main when an input event arrives
    void handleInputEvent(AInputEvent* event) {
        int32_t eventType = AInputEvent_getType(event);

        if (eventType == AINPUT_EVENT_TYPE_MOTION) {
            handleMotionEvent(event);
        } else if (eventType == AINPUT_EVENT_TYPE_KEY) {
            handleKeyEvent(event);
        }
    }

private:
    void handleMotionEvent(AInputEvent* event) {
        int32_t action = AMotionEvent_getAction(event);
        int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
        size_t pointerCount = AMotionEvent_getPointerCount(event);

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;

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
                // Pinch zoom: distance change -> scroll with Ctrl mod
                float distanceDelta = distance - _lastPinchDistance;
                if (std::abs(distanceDelta) > 2.0f) {
                    float scrollDelta = distanceDelta * 0.02f;
                    // mods=2 (Ctrl) triggers visual zoom in gpu-screen
                    loop->dispatch(base::Event::scrollEvent(
                        centerX, centerY, 0.0f, scrollDelta, 2));
                    _lastPinchDistance = distance;
                }

                // Pan while pinched (2-finger drag)
                float panDx = centerX - _lastPinchCenterX;
                float panDy = centerY - _lastPinchCenterY;
                if (std::abs(panDx) > 2.0f || std::abs(panDy) > 2.0f) {
                    // Plain scroll for pan (no mods)
                    loop->dispatch(base::Event::scrollEvent(
                        centerX, centerY, -panDx * 0.1f, -panDy * 0.1f, 0));
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
                    loop->dispatch(base::Event::mouseDown(0, x, y, 0));
                    loop->dispatch(base::Event::mouseMove(x, y));
                    break;
                case AMOTION_EVENT_ACTION_UP:
                    loop->dispatch(base::Event::mouseUp(0, x, y, 0));
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                    loop->dispatch(base::Event::mouseMove(x, y));
                    break;
            }
        }
    }

    void handleKeyEvent(AInputEvent* event) {
        int32_t action = AKeyEvent_getAction(event);
        int32_t keyCode = AKeyEvent_getKeyCode(event);
        int32_t metaState = AKeyEvent_getMetaState(event);

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;

        int platformKey = translateKeyCode(keyCode);
        bool pressed = (action == AKEY_EVENT_ACTION_DOWN);

        // Build modifier flags
        int mods = 0;
        if (metaState & AMETA_SHIFT_ON) mods |= 0x0001;
        if (metaState & AMETA_CTRL_ON) mods |= 0x0002;
        if (metaState & AMETA_ALT_ON) mods |= 0x0004;

        if (pressed) {
            loop->dispatch(base::Event::keyDown(platformKey, 0, mods));
        } else {
            loop->dispatch(base::Event::keyUp(platformKey, 0, mods));
        }

        // Character input on key down
        if (pressed) {
            char c = translateToChar(keyCode, metaState);
            if (c) {
                loop->dispatch(base::Event::charInput(static_cast<uint32_t>(c)));
            }
        }
    }

    static int translateKeyCode(int keyCode) {
        switch (keyCode) {
            case AKEYCODE_ENTER: return 257;
            case AKEYCODE_TAB: return 258;
            case AKEYCODE_DEL: return 259;
            case AKEYCODE_FORWARD_DEL: return 261;
            case AKEYCODE_DPAD_RIGHT: return 262;
            case AKEYCODE_DPAD_LEFT: return 263;
            case AKEYCODE_DPAD_DOWN: return 264;
            case AKEYCODE_DPAD_UP: return 265;
            case AKEYCODE_PAGE_UP: return 266;
            case AKEYCODE_PAGE_DOWN: return 267;
            case AKEYCODE_MOVE_HOME: return 268;
            case AKEYCODE_MOVE_END: return 269;
            case AKEYCODE_ESCAPE: return 256;
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

    static char translateToChar(int keyCode, int metaState) {
        bool shift = (metaState & AMETA_SHIFT_ON);
        bool ctrl = (metaState & AMETA_CTRL_ON);

        if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
            if (ctrl) {
                return 1 + (keyCode - AKEYCODE_A);
            }
            return shift ? ('A' + (keyCode - AKEYCODE_A)) : ('a' + (keyCode - AKEYCODE_A));
        }
        if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
            if (shift) {
                const char* symbols = ")!@#$%^&*(";
                return symbols[keyCode - AKEYCODE_0];
            }
            return '0' + (keyCode - AKEYCODE_0);
        }
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

    // Pinch gesture state
    bool _pinchActive = false;
    float _lastPinchDistance = 0.0f;
    float _lastPinchCenterX = 0.0f;
    float _lastPinchCenterY = 0.0f;

    // Global instance for android_main to call
    static AndroidInputManager* g_instance;

public:
    // Called from android_main
    static void dispatchInputEvent(AInputEvent* event) {
        if (g_instance) {
            g_instance->handleInputEvent(event);
        }
    }
};

AndroidInputManager* AndroidInputManager::g_instance = nullptr;

// Factory
Result<InputManager::Ptr> InputManager::createImpl() {
    auto mgr = new AndroidInputManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("AndroidInputManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

// C function for android_main to call
extern "C" void yetty_android_dispatch_input(AInputEvent* event) {
    yetty::AndroidInputManager::dispatchInputEvent(event);
}

#endif // __ANDROID__
