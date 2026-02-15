#include <yetty/platform.h>
#include <ytrace/ytrace.hpp>

#if defined(__EMSCRIPTEN__)

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace yetty {

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
        WGPUSurfaceSourceCanvasHTMLSelector_Emscripten canvasSource = {};
        canvasSource.chain.sType = WGPUSType_SurfaceSourceCanvasHTMLSelector_Emscripten;
        canvasSource.selector = "#canvas";

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

private:
    static EM_BOOL keyCallback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
        auto* self = static_cast<WebPlatform*>(userData);
        if (self->_keyCallback) {
            KeyAction action = (eventType == EMSCRIPTEN_EVENT_KEYDOWN) ? KeyAction::Press : KeyAction::Release;
            int mods = 0;
            if (e->ctrlKey) mods |= 0x0002;  // GLFW_MOD_CONTROL
            if (e->shiftKey) mods |= 0x0001; // GLFW_MOD_SHIFT
            if (e->altKey) mods |= 0x0004;   // GLFW_MOD_ALT
            self->_keyCallback(e->keyCode, 0, action, mods);
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

#endif // __EMSCRIPTEN__
