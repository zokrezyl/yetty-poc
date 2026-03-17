#include "glfw-window-singleton.h"

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <yetty/base/event-queue.h>
#include <yetty/base/event.h>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>

namespace yetty {

//=============================================================================
// GlfwWindowSingletonImpl
//
// THREADING MODEL:
//
// This singleton is created on the RENDER THREAD, but the callbacks it sets
// up are CALLED on the MAIN THREAD during glfwWaitEvents().
//
// - glfwInit()         -> main thread (in InitManager)
// - glfwCreateWindow() -> render thread (here)
// - glfwSetKeyCallback() etc. -> render thread (here), but callbacks FIRE
//                                on main thread during glfwWaitEvents()
// - glfwWaitEvents()   -> main thread (in InitManager), blocks waiting for
//                         events, then dispatches to callbacks
//
// CALLBACK EXECUTION:
//
// All GLFW callbacks (keyCallback, charCallback, mouseCallback, etc.) execute
// on the MAIN THREAD. They push events to EventQueue (thread-safe), which
// wakes the render thread via libuv async.
//
// CTRL+KEY HANDLING:
//
// GLFW does NOT fire charCallback for control characters (Ctrl+A, Ctrl+C, etc).
// The keyCallback must detect Ctrl+letter combinations and generate charInput
// events for control characters (Ctrl+A = 0x01, Ctrl+B = 0x02, ..., Ctrl+Z = 0x1A).
// This happens HERE in the callback (main thread) before pushing to EventQueue.
//
//=============================================================================

class GlfwWindowSingletonImpl : public GlfwWindowSingleton {
public:
    ~GlfwWindowSingletonImpl() override {
        if (_window) {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    Result<void> init() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(1280, 720, "yetty", nullptr, nullptr);
        if (!_window) {
            return Err<void>("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(_window, this);
        setupCallbacks();

        ydebug("GlfwWindowSingleton: window created");
        return Ok();
    }

    GLFWwindow* getWindow() const override {
        return _window;
    }

    void getWindowSize(int& width, int& height) const override {
        if (_window) {
            glfwGetWindowSize(_window, &width, &height);
        } else {
            width = height = 0;
        }
    }

    void getFramebufferSize(int& width, int& height) const override {
        if (_window) {
            glfwGetFramebufferSize(_window, &width, &height);
        } else {
            width = height = 0;
        }
    }

    void getContentScale(float& xscale, float& yscale) const override {
        if (_window) {
            glfwGetWindowContentScale(_window, &xscale, &yscale);
        } else {
            xscale = yscale = 1.0f;
        }
    }

    bool shouldClose() const override {
        return _window ? glfwWindowShouldClose(_window) : true;
    }

    void setTitle(const std::string& title) override {
        if (_window) {
            glfwSetWindowTitle(_window, title.c_str());
        }
    }

    void setIcon(const unsigned char* data, size_t size) override {
        // TODO: Decode PNG and set icon
        (void)data; (void)size;
    }

private:
    void setupCallbacks() {
        glfwSetKeyCallback(_window, keyCallback);
        glfwSetCharCallback(_window, charCallback);
        glfwSetMouseButtonCallback(_window, mouseButtonCallback);
        glfwSetCursorPosCallback(_window, cursorPosCallback);
        glfwSetScrollCallback(_window, scrollCallback);
        glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
    }

    /// @brief GLFW key callback - executes on MAIN THREAD during glfwWaitEvents()
    ///
    /// Pushes RAW key events to EventQueue. No transformation here.
    /// InputManager (on render thread) subscribes to these and generates
    /// internal events (e.g., Ctrl+letter -> control character).
    ///
    /// @param window   GLFW window handle (unused)
    /// @param key      GLFW key code (GLFW_KEY_A, GLFW_KEY_ESCAPE, etc.)
    /// @param scancode Platform-specific scancode
    /// @param action   GLFW_PRESS, GLFW_RELEASE, or GLFW_REPEAT
    /// @param mods     Modifier flags (GLFW_MOD_SHIFT, GLFW_MOD_CONTROL, etc.)
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            (*queueResult)->push(base::Event::keyDown(key, mods, scancode));
        } else if (action == GLFW_RELEASE) {
            (*queueResult)->push(base::Event::keyUp(key, mods, scancode));
        }
    }

    /// @brief GLFW character callback - executes on MAIN THREAD during glfwWaitEvents()
    ///
    /// Pushes RAW character input to EventQueue.
    /// NOTE: GLFW does NOT fire this for control characters (Ctrl+A, etc.).
    /// InputManager handles Ctrl+letter -> control character conversion.
    ///
    /// @param window    GLFW window handle (unused)
    /// @param codepoint Unicode codepoint of the character
    static void charCallback(GLFWwindow* window, unsigned int codepoint) {
        (void)window;
        ydebug("GlfwWindowSingleton::charCallback: codepoint={}", codepoint);
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) {
            yerror("GlfwWindowSingleton::charCallback: EventQueue not available!");
            return;
        }
        (*queueResult)->push(base::Event::charInput(codepoint));
        ydebug("GlfwWindowSingleton::charCallback: pushed to EventQueue");
    }

    /// @brief GLFW mouse button callback - executes on MAIN THREAD
    ///
    /// @param window GLFW window handle (used to get cursor position)
    /// @param button GLFW_MOUSE_BUTTON_LEFT, _RIGHT, _MIDDLE, etc.
    /// @param action GLFW_PRESS or GLFW_RELEASE
    /// @param mods   Modifier flags
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        if (action == GLFW_PRESS) {
            (*queueResult)->push(base::Event::mouseDown(
                static_cast<float>(x), static_cast<float>(y), button, mods));
        } else {
            (*queueResult)->push(base::Event::mouseUp(
                static_cast<float>(x), static_cast<float>(y), button, mods));
        }
    }

    /// @brief GLFW cursor position callback - executes on MAIN THREAD
    ///
    /// @param window GLFW window handle (unused)
    /// @param x      Cursor X position in window coordinates
    /// @param y      Cursor Y position in window coordinates
    static void cursorPosCallback(GLFWwindow* window, double x, double y) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;
        (*queueResult)->push(base::Event::mouseMove(
            static_cast<float>(x), static_cast<float>(y)));
    }

    /// @brief GLFW scroll callback - executes on MAIN THREAD
    ///
    /// @param window  GLFW window handle (used to get cursor pos and modifier state)
    /// @param xoffset Horizontal scroll offset
    /// @param yoffset Vertical scroll offset
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        int mods = 0;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            mods |= 0x0001;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            mods |= 0x0002;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
            mods |= 0x0004;
        }

        (*queueResult)->push(base::Event::scrollEvent(
            static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(xoffset), static_cast<float>(yoffset), mods));
    }

    /// @brief GLFW framebuffer size callback - executes on MAIN THREAD
    ///
    /// @param window GLFW window handle (unused)
    /// @param width  New framebuffer width in pixels
    /// @param height New framebuffer height in pixels
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;
        (*queueResult)->push(base::Event::resizeEvent(
            static_cast<float>(width), static_cast<float>(height)));
    }

    GLFWwindow* _window = nullptr;
};

Result<GlfwWindowSingleton::Ptr> GlfwWindowSingleton::createImpl() {
    auto singleton = new GlfwWindowSingletonImpl();
    if (auto res = singleton->init(); !res) {
        delete singleton;
        return Err<Ptr>("GlfwWindowSingleton init failed", res);
    }
    return Ok(Ptr(singleton));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
