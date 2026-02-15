#include <yetty/platform.h>
#include <ytrace/ytrace.hpp>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace yetty {

class GlfwPlatform : public Platform {
public:
    GlfwPlatform() = default;
    ~GlfwPlatform() override {
        destroyWindow();
        if (_initialized) {
            glfwTerminate();
        }
    }

    Result<void> init() {
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }
        _initialized = true;
        return Ok();
    }

    Result<void> createWindow(int width, int height, const std::string& title) override {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!_window) {
            return Err<void>("Failed to create GLFW window");
        }

        // Store this pointer for callbacks
        glfwSetWindowUserPointer(_window, this);

        // Set up callbacks
        glfwSetKeyCallback(_window, keyCallbackStatic);
        glfwSetCharCallback(_window, charCallbackStatic);
        glfwSetMouseButtonCallback(_window, mouseButtonCallbackStatic);
        glfwSetCursorPosCallback(_window, cursorPosCallbackStatic);
        glfwSetScrollCallback(_window, scrollCallbackStatic);
        glfwSetFramebufferSizeCallback(_window, framebufferSizeCallbackStatic);
        glfwSetWindowFocusCallback(_window, windowFocusCallbackStatic);

        // Create cursors
        _cursorArrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        _cursorIBeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        _cursorHand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        _cursorResizeH = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        _cursorResizeV = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

        yinfo("GLFW window created: {}x{}", width, height);
        return Ok();
    }

    void destroyWindow() override {
        if (_cursorArrow) { glfwDestroyCursor(_cursorArrow); _cursorArrow = nullptr; }
        if (_cursorIBeam) { glfwDestroyCursor(_cursorIBeam); _cursorIBeam = nullptr; }
        if (_cursorHand) { glfwDestroyCursor(_cursorHand); _cursorHand = nullptr; }
        if (_cursorResizeH) { glfwDestroyCursor(_cursorResizeH); _cursorResizeH = nullptr; }
        if (_cursorResizeV) { glfwDestroyCursor(_cursorResizeV); _cursorResizeV = nullptr; }

        if (_window) {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    void* getNativeHandle() const override {
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

    bool shouldClose() const override {
        return _window ? glfwWindowShouldClose(_window) : true;
    }

    void setTitle(const std::string& title) override {
        if (_window) {
            glfwSetWindowTitle(_window, title.c_str());
        }
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        if (!_window) return nullptr;
        return glfwCreateWindowWGPUSurface(instance, _window);
    }

    void pollEvents() override {
        glfwPollEvents();
    }

    double getTime() const override {
        return glfwGetTime();
    }

    void setKeyCallback(KeyCallback cb) override { _keyCallback = std::move(cb); }
    void setCharCallback(CharCallback cb) override { _charCallback = std::move(cb); }
    void setMouseButtonCallback(MouseButtonCallback cb) override { _mouseButtonCallback = std::move(cb); }
    void setMouseMoveCallback(MouseMoveCallback cb) override { _mouseMoveCallback = std::move(cb); }
    void setScrollCallback(ScrollCallback cb) override { _scrollCallback = std::move(cb); }
    void setResizeCallback(ResizeCallback cb) override { _resizeCallback = std::move(cb); }
    void setFocusCallback(FocusCallback cb) override { _focusCallback = std::move(cb); }

    std::string getClipboardText() const override {
        if (_window) {
            const char* text = glfwGetClipboardString(_window);
            return text ? text : "";
        }
        return "";
    }

    void setClipboardText(const std::string& text) override {
        if (_window) {
            glfwSetClipboardString(_window, text.c_str());
        }
    }

    void setCursor(CursorType type) override {
        if (!_window) return;

        GLFWcursor* cursor = nullptr;
        switch (type) {
            case CursorType::Arrow: cursor = _cursorArrow; break;
            case CursorType::IBeam: cursor = _cursorIBeam; break;
            case CursorType::Hand: cursor = _cursorHand; break;
            case CursorType::ResizeH: cursor = _cursorResizeH; break;
            case CursorType::ResizeV: cursor = _cursorResizeV; break;
            case CursorType::Hidden:
                glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                return;
        }
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (cursor) {
            glfwSetCursor(_window, cursor);
        }
    }

    void getContentScale(float& xscale, float& yscale) const override {
        if (_window) {
            glfwGetWindowContentScale(_window, &xscale, &yscale);
        } else {
            xscale = yscale = 1.0f;
        }
    }

    std::string getKeyName(int key, int scancode) const override {
        const char* name = glfwGetKeyName(key, scancode);
        return name ? name : "";
    }

private:
    // Static callback trampolines
    static void keyCallbackStatic(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_keyCallback) {
            KeyAction ka = KeyAction::Press;
            if (action == GLFW_RELEASE) ka = KeyAction::Release;
            else if (action == GLFW_REPEAT) ka = KeyAction::Repeat;
            self->_keyCallback(key, scancode, ka, mods);
        }
    }

    static void charCallbackStatic(GLFWwindow* window, unsigned int codepoint) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_charCallback) {
            self->_charCallback(codepoint);
        }
    }

    static void mouseButtonCallbackStatic(GLFWwindow* window, int button, int action, int mods) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_mouseButtonCallback) {
            MouseButton mb = MouseButton::Left;
            if (button == GLFW_MOUSE_BUTTON_RIGHT) mb = MouseButton::Right;
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) mb = MouseButton::Middle;
            self->_mouseButtonCallback(mb, action == GLFW_PRESS, mods);
        }
    }

    static void cursorPosCallbackStatic(GLFWwindow* window, double xpos, double ypos) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_mouseMoveCallback) {
            self->_mouseMoveCallback(xpos, ypos);
        }
    }

    static void scrollCallbackStatic(GLFWwindow* window, double xoffset, double yoffset) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_scrollCallback) {
            self->_scrollCallback(xoffset, yoffset);
        }
    }

    static void framebufferSizeCallbackStatic(GLFWwindow* window, int width, int height) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_resizeCallback) {
            self->_resizeCallback(width, height);
        }
    }

    static void windowFocusCallbackStatic(GLFWwindow* window, int focused) {
        auto* self = static_cast<GlfwPlatform*>(glfwGetWindowUserPointer(window));
        if (self && self->_focusCallback) {
            self->_focusCallback(focused == GLFW_TRUE);
        }
    }

    GLFWwindow* _window = nullptr;
    bool _initialized = false;

    // Cursors
    GLFWcursor* _cursorArrow = nullptr;
    GLFWcursor* _cursorIBeam = nullptr;
    GLFWcursor* _cursorHand = nullptr;
    GLFWcursor* _cursorResizeH = nullptr;
    GLFWcursor* _cursorResizeV = nullptr;

    // Callbacks
    KeyCallback _keyCallback;
    CharCallback _charCallback;
    MouseButtonCallback _mouseButtonCallback;
    MouseMoveCallback _mouseMoveCallback;
    ScrollCallback _scrollCallback;
    ResizeCallback _resizeCallback;
    FocusCallback _focusCallback;
};

// Factory implementation for GLFW
Result<Platform::Ptr> Platform::create() {
    auto platform = std::make_shared<GlfwPlatform>();
    if (auto res = platform->init(); !res) {
        return Err<Ptr>("Failed to initialize GLFW platform", res);
    }
    return Ok<Ptr>(std::move(platform));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
