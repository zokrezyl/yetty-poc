#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <yetty/gpu-screen-manager.h>
#include <yetty/gpu-screen.h>
#include <ytrace/ytrace.hpp>

#if defined(__EMSCRIPTEN__)

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <atomic>
#include <unordered_map>

namespace yetty {

// =============================================================================
// WebPTY - PTY Provider that uses JSLinux in an iframe
// =============================================================================

// Global registry of WebPTY instances for postMessage callbacks
static std::unordered_map<uint32_t, class WebPTY*> g_ptyInstances;
static std::atomic<uint32_t> g_nextPtyId{1};

class WebPTY : public PTYProvider {
public:
    WebPTY() : _id(g_nextPtyId++) {
        g_ptyInstances[_id] = this;
    }

    ~WebPTY() override {
        stop();
        g_ptyInstances.erase(_id);
    }

    Result<void> start(const std::string& vmConfig, uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;
        _vmConfig = vmConfig;
        _running = true;

        yinfo("WebPTY[{}]: Starting with config '{}' ({}x{})", _id, vmConfig, cols, rows);

        // Create iframe and start JSLinux emulator via JavaScript
        EM_ASM({
            var ptyId = $0;
            var vmConfig = UTF8ToString($1);
            var cols = $2;
            var rows = $3;

            // Create hidden iframe for the emulator
            var iframe = document.createElement('iframe');
            iframe.id = 'jslinux-pty-' + ptyId;
            iframe.style.cssText = 'position:absolute;width:1px;height:1px;opacity:0;pointer-events:none;';
            iframe.src = '/jslinux/vm-bridge.html?ptyId=' + ptyId +
                         '&url=' + encodeURIComponent(vmConfig) +
                         '&cols=' + cols + '&rows=' + rows +
                         '&cpu=x86_64&mem=256';
            document.body.appendChild(iframe);

            console.log('WebPTY: Created iframe for pty ' + ptyId);
        }, _id, vmConfig.c_str(), cols, rows);

        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        yinfo("WebPTY[{}]: Stopping", _id);

        // Remove iframe
        EM_ASM({
            var ptyId = $0;
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe) {
                iframe.remove();
                console.log('WebPTY: Removed iframe for pty ' + ptyId);
            }
        }, _id);

        if (_exitCallback) {
            _exitCallback(0);
        }
    }

    void write(const char* data, size_t len) override {
        if (!_running || len == 0) return;

        // Send input to iframe via postMessage
        EM_ASM({
            var ptyId = $0;
            var data = UTF8ToString($1, $2);
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe && iframe.contentWindow) {
                iframe.contentWindow.postMessage({
                    type: 'term-input',
                    ptyId: ptyId,
                    data: data
                }, '*');
            }
        }, _id, data, len);
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;

        // Send resize to iframe
        EM_ASM({
            var ptyId = $0;
            var cols = $1;
            var rows = $2;
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe && iframe.contentWindow) {
                iframe.contentWindow.postMessage({
                    type: 'term-resize',
                    ptyId: ptyId,
                    cols: cols,
                    rows: rows
                }, '*');
            }
        }, _id, cols, rows);
    }

    bool isRunning() const override { return _running; }

    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

    // Called from JavaScript when data arrives from iframe
    void onDataFromEmulator(const char* data, size_t len) {
        if (_dataCallback) {
            _dataCallback(data, len);
        }
    }

private:
    uint32_t _id;
    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _vmConfig;
    bool _running = false;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;
};

// =============================================================================
// WebPlatform - Main platform implementation for Emscripten
// =============================================================================

class WebPlatform : public Platform {
public:
    WebPlatform() = default;
    ~WebPlatform() override = default;

    Result<void> createWindow(int width, int height, const std::string& title) override {
        _width = width;
        _height = height;
        _title = title;

        // Set canvas size
        emscripten_set_canvas_element_size("#canvas", width, height);

        // Register callbacks
        emscripten_set_keydown_callback("#canvas", this, true, keyCallback);
        emscripten_set_keyup_callback("#canvas", this, true, keyCallback);
        emscripten_set_mousedown_callback("#canvas", this, true, mouseCallback);
        emscripten_set_mouseup_callback("#canvas", this, true, mouseCallback);
        emscripten_set_mousemove_callback("#canvas", this, true, mouseMoveCallback);
        emscripten_set_wheel_callback("#canvas", this, true, wheelCallback);
        emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, resizeCallback);

        yinfo("Web platform window created: {}x{}", width, height);
        return Ok();
    }

    void destroyWindow() override {
        // Nothing to destroy on web
    }

    void* getNativeHandle() const override {
        // Return canvas selector - WebGPU uses this
        return const_cast<char*>("#canvas");
    }

    void getWindowSize(int& width, int& height) const override {
        width = _width;
        height = _height;
    }

    void getFramebufferSize(int& width, int& height) const override {
        width = _width;
        height = _height;
    }

    bool shouldClose() const override {
        return false;  // Web apps don't "close"
    }

    void setTitle(const std::string& title) override {
        _title = title;
        EM_ASM({ document.title = UTF8ToString($0); }, title.c_str());
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource = {};
        canvasSource.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
        canvasSource.selector = { .data = "#canvas", .length = 7 };

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = &canvasSource.chain;

        return wgpuInstanceCreateSurface(instance, &surfaceDesc);
    }

    void pollEvents() override {
        // Events are delivered via callbacks, no polling needed
    }

    double getTime() const override {
        return emscripten_get_now() / 1000.0;
    }

    void runMainLoop(MainLoopCallback callback) override {
        _mainLoopCallback = std::move(callback);
        // 1 = 1 FPS default to reduce log output; use OSC 666671 to increase
        emscripten_set_main_loop_arg(mainLoopTrampoline, this, 1, true);
    }

    void setKeyCallback(KeyCallback cb) override { _keyCallback = std::move(cb); }
    void setCharCallback(CharCallback cb) override { _charCallback = std::move(cb); }
    void setMouseButtonCallback(MouseButtonCallback cb) override { _mouseButtonCallback = std::move(cb); }
    void setMouseMoveCallback(MouseMoveCallback cb) override { _mouseMoveCallback = std::move(cb); }
    void setScrollCallback(ScrollCallback cb) override { _scrollCallback = std::move(cb); }
    void setResizeCallback(ResizeCallback cb) override { _resizeCallback = std::move(cb); }
    void setFocusCallback(FocusCallback cb) override { _focusCallback = std::move(cb); }

    std::string getClipboardText() const override {
        // TODO: Implement via async clipboard API
        return "";
    }

    void setClipboardText(const std::string& text) override {
        EM_ASM({ navigator.clipboard.writeText(UTF8ToString($0)); }, text.c_str());
    }

    void setCursor(CursorType type) override {
        const char* cursor = "default";
        switch (type) {
            case CursorType::Arrow: cursor = "default"; break;
            case CursorType::IBeam: cursor = "text"; break;
            case CursorType::Hand: cursor = "pointer"; break;
            case CursorType::ResizeH: cursor = "ew-resize"; break;
            case CursorType::ResizeV: cursor = "ns-resize"; break;
            case CursorType::Hidden: cursor = "none"; break;
        }
        EM_ASM({ document.body.style.cursor = UTF8ToString($0); }, cursor);
    }

    void getContentScale(float& xscale, float& yscale) const override {
        double ratio = emscripten_get_device_pixel_ratio();
        xscale = yscale = static_cast<float>(ratio);
    }

    std::string getKeyName(int key, int scancode) const override {
        (void)key;
        (void)scancode;
        return "";
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<WebPTY>()));
    }

private:
    static void mainLoopTrampoline(void* userData) {
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_mainLoopCallback) {
            bool continueLoop = self->_mainLoopCallback();
            if (!continueLoop) {
                emscripten_cancel_main_loop();
            }
        }
    }

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

    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_keyCallback) {
            KeyAction action = (eventType == EMSCRIPTEN_EVENT_KEYDOWN) ? KeyAction::Press : KeyAction::Release;
            int mods = 0;
            if (e->ctrlKey) mods |= 0x0002;  // GLFW_MOD_CONTROL
            if (e->shiftKey) mods |= 0x0001; // GLFW_MOD_SHIFT
            if (e->altKey) mods |= 0x0004;   // GLFW_MOD_ALT
            int glfwKey = browserToGLFWKey(e->keyCode);
            self->_keyCallback(glfwKey, 0, action, mods);
        }
        if (self->_charCallback && eventType == EMSCRIPTEN_EVENT_KEYDOWN && e->key[0] && !e->key[1]) {
            // Single character key
            self->_charCallback(static_cast<unsigned int>(e->key[0]));
        }
        return EM_TRUE;
    }

    static EM_BOOL mouseCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_mouseButtonCallback) {
            MouseButton button = MouseButton::Left;
            if (e->button == 1) button = MouseButton::Middle;
            else if (e->button == 2) button = MouseButton::Right;
            bool pressed = (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN);
            self->_mouseButtonCallback(button, pressed, 0);
        }
        return EM_TRUE;
    }

    static EM_BOOL mouseMoveCallback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
        (void)eventType;
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_mouseMoveCallback) {
            self->_mouseMoveCallback(e->targetX, e->targetY);
        }
        return EM_TRUE;
    }

    static EM_BOOL wheelCallback(int eventType, const EmscriptenWheelEvent* e, void* userData) {
        (void)eventType;
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_scrollCallback) {
            self->_scrollCallback(e->deltaX, e->deltaY);
        }
        return EM_TRUE;
    }

    static EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent* e, void* userData) {
        (void)eventType;
        auto* self = static_cast<WebPlatform*>(userData);
        self->_width = e->windowInnerWidth;
        self->_height = e->windowInnerHeight;
        emscripten_set_canvas_element_size("#canvas", self->_width, self->_height);
        if (self->_resizeCallback) {
            self->_resizeCallback(self->_width, self->_height);
        }
        return EM_TRUE;
    }

    int _width = 0;
    int _height = 0;
    std::string _title;
    MainLoopCallback _mainLoopCallback;

    // Callbacks
    KeyCallback _keyCallback;
    CharCallback _charCallback;
    MouseButtonCallback _mouseButtonCallback;
    MouseMoveCallback _mouseMoveCallback;
    ScrollCallback _scrollCallback;
    ResizeCallback _resizeCallback;
    FocusCallback _focusCallback;
};

// Factory implementation for Web
Result<Platform::Ptr> Platform::create() {
    auto platform = std::make_shared<WebPlatform>();
    yinfo("Web platform created");
    return Ok<Ptr>(std::move(platform));
}

} // namespace yetty

// =============================================================================
// Exported C functions for JavaScript interop
// =============================================================================

extern "C" {

// Called from JavaScript when data arrives from JSLinux iframe
EMSCRIPTEN_KEEPALIVE
void webpty_on_data(uint32_t ptyId, const char* data, int len) {
    auto it = yetty::g_ptyInstances.find(ptyId);
    if (it != yetty::g_ptyInstances.end()) {
        it->second->onDataFromEmulator(data, static_cast<size_t>(len));
    }
}

// Write data to terminal (from JavaScript) - legacy, kept for compatibility
EMSCRIPTEN_KEEPALIVE
void yetty_write(const char* data, int len) {
    if (len <= 0) return;

    auto mgrResult = yetty::GPUScreenManager::instance();
    if (!mgrResult) return;

    auto screens = (*mgrResult)->screens();
    if (screens.empty()) return;

    screens[0]->write(data, static_cast<size_t>(len));
}

EMSCRIPTEN_KEEPALIVE
void yetty_key(int key, int mods) {
    (void)key; (void)mods;
}

EMSCRIPTEN_KEEPALIVE
void yetty_special_key(int key, int mods) {
    (void)key; (void)mods;
}

EMSCRIPTEN_KEEPALIVE
int yetty_read_input(char* buffer, int maxLen) {
    (void)buffer; (void)maxLen;
    return 0;
}

EMSCRIPTEN_KEEPALIVE
void yetty_sync() {
}

EMSCRIPTEN_KEEPALIVE
void yetty_set_scale(float scaleX, float scaleY) {
    (void)scaleX; (void)scaleY;
}

EMSCRIPTEN_KEEPALIVE
void yetty_resize(int cols, int rows) {
    auto mgr = yetty::GPUScreenManager::instance();
    if (!mgr) return;

    auto screens = (*mgr)->screens();
    if (!screens.empty()) {
        screens[0]->resize(static_cast<uint32_t>(cols), static_cast<uint32_t>(rows));
    }
}

EMSCRIPTEN_KEEPALIVE
int yetty_get_cols() {
    auto mgr = yetty::GPUScreenManager::instance();
    if (!mgr) return 80;

    auto screens = (*mgr)->screens();
    if (screens.empty()) return 80;

    return static_cast<int>(screens[0]->getCols());
}

EMSCRIPTEN_KEEPALIVE
int yetty_get_rows() {
    auto mgr = yetty::GPUScreenManager::instance();
    if (!mgr) return 24;

    auto screens = (*mgr)->screens();
    if (screens.empty()) return 24;

    return static_cast<int>(screens[0]->getRows());
}

} // extern "C"

// Initialize message listener at startup
static struct WebPTYInit {
    WebPTYInit() {
        EM_ASM({
            window.addEventListener('message', function(e) {
                if (e.data && e.data.type === 'term-output' && e.data.ptyId) {
                    var data = e.data.data;
                    var ptyId = parseInt(e.data.ptyId, 10);  // Convert string to int
                    if (isNaN(ptyId)) {
                        console.error('WebPTY: Invalid ptyId:', e.data.ptyId);
                        return;
                    }
                    console.log('WebPTY: Received term-output for pty ' + ptyId + ', len=' + data.length);
                    var encoder = new TextEncoder();
                    var bytes = encoder.encode(data);
                    var ptr = Module._malloc(bytes.length);
                    Module.HEAPU8.set(bytes, ptr);
                    Module._webpty_on_data(ptyId, ptr, bytes.length);
                    Module._free(ptr);
                }
            });
            console.log('WebPTY: Message listener initialized');
        });
    }
} g_webPtyInit;

#endif // __EMSCRIPTEN__
