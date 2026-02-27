#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace yetty {

// =============================================================================
// ConPTY - PTY Provider using Windows ConPTY
// =============================================================================

static std::atomic<uint32_t> g_nextPtyId{1};

class ConPTY : public PTYProvider {
public:
    ConPTY() : _id(g_nextPtyId++) {}

    ~ConPTY() override {
        stop();
    }

    Result<void> start(const std::string& shell, uint32_t cols, uint32_t rows) override {
        _shell = shell;
        _cols = cols;
        _rows = rows;

        yinfo("ConPTY[{}]: Starting shell '{}' ({}x{})", _id, shell, cols, rows);

        // Create pipes for PTY input/output
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };

        if (!CreatePipe(&_pipeInRead, &_pipeInWrite, &sa, 0)) {
            return Err<void>("Failed to create input pipe");
        }
        if (!CreatePipe(&_pipeOutRead, &_pipeOutWrite, &sa, 0)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            return Err<void>("Failed to create output pipe");
        }

        // Create pseudo console (ConPTY)
        COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
        HRESULT hr = CreatePseudoConsole(size, _pipeInRead, _pipeOutWrite, 0, &_hPC);
        if (FAILED(hr)) {
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to create pseudo console");
        }

        // Set up startup info with pseudo console
        STARTUPINFOEXW siEx = {};
        siEx.StartupInfo.cb = sizeof(STARTUPINFOEXW);

        SIZE_T attrListSize = 0;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
        siEx.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
            HeapAlloc(GetProcessHeap(), 0, attrListSize));

        if (!siEx.lpAttributeList) {
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to allocate attribute list");
        }

        if (!InitializeProcThreadAttributeList(siEx.lpAttributeList, 1, 0, &attrListSize)) {
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to init attribute list");
        }

        if (!UpdateProcThreadAttribute(siEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
                                        _hPC, sizeof(_hPC), nullptr, nullptr)) {
            DeleteProcThreadAttributeList(siEx.lpAttributeList);
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to update attribute list");
        }

        // Convert shell path to wide string
        std::wstring wShell(shell.begin(), shell.end());

        // Create process
        PROCESS_INFORMATION pi = {};
        if (!CreateProcessW(nullptr, wShell.data(), nullptr, nullptr, FALSE,
                           EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                           &siEx.StartupInfo, &pi)) {
            DeleteProcThreadAttributeList(siEx.lpAttributeList);
            HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);
            ClosePseudoConsole(_hPC);
            CloseHandle(_pipeInRead);
            CloseHandle(_pipeInWrite);
            CloseHandle(_pipeOutRead);
            CloseHandle(_pipeOutWrite);
            return Err<void>("Failed to create process");
        }

        _hProcess = pi.hProcess;
        CloseHandle(pi.hThread);

        DeleteProcThreadAttributeList(siEx.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, siEx.lpAttributeList);

        // Close pipe ends we don't need
        CloseHandle(_pipeInRead);
        _pipeInRead = INVALID_HANDLE_VALUE;
        CloseHandle(_pipeOutWrite);
        _pipeOutWrite = INVALID_HANDLE_VALUE;

        _running = true;

        // Start reader thread
        _readerThread = std::thread([this]() { readerThreadFunc(); });

        yinfo("ConPTY[{}]: Started, PID={}", _id, GetProcessId(_hProcess));
        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        yinfo("ConPTY[{}]: Stopping", _id);

        if (_hPC) {
            ClosePseudoConsole(_hPC);
            _hPC = nullptr;
        }

        if (_pipeInWrite != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeInWrite);
            _pipeInWrite = INVALID_HANDLE_VALUE;
        }
        if (_pipeOutRead != INVALID_HANDLE_VALUE) {
            CloseHandle(_pipeOutRead);
            _pipeOutRead = INVALID_HANDLE_VALUE;
        }

        if (_readerThread.joinable()) {
            _readerThread.join();
        }

        if (_hProcess != INVALID_HANDLE_VALUE) {
            DWORD exitCode = 0;
            GetExitCodeProcess(_hProcess, &exitCode);
            CloseHandle(_hProcess);
            _hProcess = INVALID_HANDLE_VALUE;
            if (_exitCallback) {
                _exitCallback(static_cast<int>(exitCode));
            }
        }
    }

    void write(const char* data, size_t len) override {
        if (_pipeInWrite != INVALID_HANDLE_VALUE && len > 0) {
            DWORD written;
            WriteFile(_pipeInWrite, data, static_cast<DWORD>(len), &written, nullptr);
        }
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;
        if (_hPC) {
            COORD size = { static_cast<SHORT>(cols), static_cast<SHORT>(rows) };
            ResizePseudoConsole(_hPC, size);
        }
    }

    bool isRunning() const override { return _running; }

    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

private:
    void readerThreadFunc() {
        char buf[65536];
        while (_running) {
            DWORD bytesRead = 0;
            if (ReadFile(_pipeOutRead, buf, sizeof(buf), &bytesRead, nullptr) && bytesRead > 0) {
                if (_dataCallback) {
                    _dataCallback(buf, static_cast<size_t>(bytesRead));
                }
            } else {
                // Check if process exited
                DWORD exitCode;
                if (GetExitCodeProcess(_hProcess, &exitCode) && exitCode != STILL_ACTIVE) {
                    _running = false;
                    if (_exitCallback) {
                        _exitCallback(static_cast<int>(exitCode));
                    }
                    break;
                }
                Sleep(10);
            }
        }
    }

    uint32_t _id;
    HPCON _hPC = nullptr;
    HANDLE _hProcess = INVALID_HANDLE_VALUE;
    HANDLE _pipeInRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeInWrite = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutWrite = INVALID_HANDLE_VALUE;

    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _shell;
    std::atomic<bool> _running{false};

    std::thread _readerThread;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;
};

// =============================================================================
// GlfwPlatform (Windows version)
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
        (void)callback;
    }

    void requestRender() override {
        // Desktop: dispatch ScreenUpdate event directly
        if (auto loop = base::EventLoop::instance(); loop) {
            (*loop)->dispatch(base::Event::screenUpdateEvent());
        }
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<ConPTY>()));
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

// Factory implementation for Windows
Result<Platform::Ptr> Platform::create() {
    auto platform = std::make_shared<GlfwPlatform>();
    if (auto res = platform->init(); !res) {
        return Err<Ptr>("Failed to initialize GLFW platform", res);
    }
    return Ok<Ptr>(std::move(platform));
}

} // namespace yetty

#endif // _WIN32
