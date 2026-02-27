#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <atomic>
#include <vector>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace yetty {

// =============================================================================
// ForkPTY - PTY Provider using forkpty() for Unix systems
// =============================================================================

static std::atomic<uint32_t> g_nextPtyId{1};

class ForkPTY : public PTYProvider, public base::EventListener {
public:
    ForkPTY() : _id(g_nextPtyId++) {}

    ~ForkPTY() override {
        stop();
    }

    Result<void> start(const std::string& shell, uint32_t cols, uint32_t rows) override {
        _shell = shell;
        _cols = cols;
        _rows = rows;

        yinfo("ForkPTY[{}]: Starting shell '{}' ({}x{})", _id, shell, cols, rows);

        struct winsize ws = {
            static_cast<unsigned short>(rows),
            static_cast<unsigned short>(cols),
            0, 0
        };

        _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);

        if (_childPid < 0) {
            return Err<void>(std::string("forkpty failed: ") + strerror(errno));
        }

        if (_childPid == 0) {
            // Child process
            for (int fd = 3; fd < 1024; fd++)
                close(fd);

            execl(shell.c_str(), shell.c_str(), nullptr);
            _exit(1);
        }

        // Parent process
        int flags = fcntl(_ptyMaster, F_GETFL, 0);
        fcntl(_ptyMaster, F_SETFL, flags | O_NONBLOCK);

        // Set up event loop polling
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("No event loop available");
        }
        auto loop = *loopResult;

        auto pollResult = loop->createPoll();
        if (!pollResult) {
            return Err<void>("Failed to create poll", pollResult);
        }
        _pollId = *pollResult;

        if (auto res = loop->configPoll(_pollId, _ptyMaster); !res) {
            return Err<void>("Failed to configure poll", res);
        }

        if (auto res = loop->registerPollListener(_pollId, sharedAs<base::EventListener>()); !res) {
            return Err<void>("Failed to register poll listener", res);
        }

        if (auto res = loop->startPoll(_pollId); !res) {
            return Err<void>("Failed to start poll", res);
        }

        _running = true;
        yinfo("ForkPTY[{}]: Started PTY fd={}, PID={}", _id, _ptyMaster, _childPid);
        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        yinfo("ForkPTY[{}]: Stopping", _id);

        if (_pollId >= 0) {
            auto loopResult = base::EventLoop::instance();
            if (loopResult) {
                auto loop = *loopResult;
                loop->deregisterListener(sharedAs<base::EventListener>());
                loop->destroyPoll(_pollId);
            }
            _pollId = -1;
        }

        if (_ptyMaster >= 0) {
            close(_ptyMaster);
            _ptyMaster = -1;
        }

        if (_childPid > 0) {
            kill(_childPid, SIGTERM);
            int status;
            waitpid(_childPid, &status, 0);
            if (_exitCallback) {
                _exitCallback(WEXITSTATUS(status));
            }
            _childPid = -1;
        }
    }

    void write(const char* data, size_t len) override {
        if (_ptyMaster >= 0 && len > 0) {
            ssize_t written = ::write(_ptyMaster, data, len);
            (void)written;
        }
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;

        if (_ptyMaster >= 0) {
            struct winsize ws = {
                static_cast<unsigned short>(rows),
                static_cast<unsigned short>(cols),
                0, 0
            };
            ioctl(_ptyMaster, TIOCSWINSZ, &ws);
        }
    }

    bool isRunning() const override { return _running; }

    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::PollReadable && event.poll.fd == _ptyMaster) {
            readPty();
            return Ok(true);
        }
        return Ok(false);
    }

private:
    void readPty() {
        // Check if child exited
        int status;
        if (waitpid(_childPid, &status, WNOHANG) > 0) {
            _running = false;
            if (_exitCallback) {
                _exitCallback(WEXITSTATUS(status));
            }
            return;
        }

        static constexpr size_t READ_CHUNK = 65536;
        std::vector<char> buffer(READ_CHUNK);

        ssize_t n = read(_ptyMaster, buffer.data(), READ_CHUNK);
        if (n > 0 && _dataCallback) {
            _dataCallback(buffer.data(), static_cast<size_t>(n));
        }
    }

    uint32_t _id;
    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _shell;
    bool _running = false;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;
};

// =============================================================================
// GlfwPlatform
// =============================================================================

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

    void runMainLoop(MainLoopCallback callback) override {
        // Desktop: Not used - yetty uses EventLoop for desktop builds
        // This is here for interface completeness
        (void)callback;
    }

    void requestRender() override {
        // Desktop: dispatch ScreenUpdate event directly
        if (auto loop = base::EventLoop::instance(); loop) {
            (*loop)->dispatch(base::Event::screenUpdateEvent());
        }
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<ForkPTY>()));
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
