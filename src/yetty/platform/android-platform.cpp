#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <ytrace/ytrace.hpp>

#if defined(__ANDROID__)

#include <android/native_window.h>
#include <android/input.h>
#include <android/keycodes.h>

namespace yetty {

class AndroidPlatform : public Platform {
public:
    AndroidPlatform() = default;
    ~AndroidPlatform() override = default;

    void setNativeWindow(ANativeWindow* window) {
        _window = window;
        if (_window) {
            _width = ANativeWindow_getWidth(_window);
            _height = ANativeWindow_getHeight(_window);
        }
    }

    Result<void> createWindow(int width, int height, const std::string& title) override {
        // On Android, window is provided by the system, not created
        // This should be called after setNativeWindow()
        (void)width;
        (void)height;
        (void)title;
        if (!_window) {
            return Err<void>("No native window set - call setNativeWindow first");
        }
        yinfo("Android platform window set: {}x{}", _width, _height);
        return Ok();
    }

    void destroyWindow() override {
        _window = nullptr;
    }

    void* getNativeHandle() const override {
        return _window;
    }

    void getWindowSize(int& width, int& height) const override {
        width = _width;
        height = _height;
    }

    void getFramebufferSize(int& width, int& height) const override {
        // On Android, framebuffer size equals window size
        width = _width;
        height = _height;
    }

    bool shouldClose() const override {
        return _shouldClose;
    }

    void setShouldClose(bool close) {
        _shouldClose = close;
    }

    void setTitle(const std::string& title) override {
        // No-op on Android
        (void)title;
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        if (!_window) return nullptr;

        WGPUSurfaceSourceAndroidNativeWindow androidSource = {};
        androidSource.chain.sType = WGPUSType_SurfaceSourceAndroidNativeWindow;
        androidSource.window = _window;

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = &androidSource.chain;

        return wgpuInstanceCreateSurface(instance, &surfaceDesc);
    }

    void pollEvents() override {
        // On Android, events are delivered via callbacks from the main loop
        // This is a no-op - events are processed in android_main
    }

    double getTime() const override {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec) / 1e9;
    }

    void setKeyCallback(KeyCallback cb) override { _keyCallback = std::move(cb); }
    void setCharCallback(CharCallback cb) override { _charCallback = std::move(cb); }
    void setMouseButtonCallback(MouseButtonCallback cb) override { _mouseButtonCallback = std::move(cb); }
    void setMouseMoveCallback(MouseMoveCallback cb) override { _mouseMoveCallback = std::move(cb); }
    void setScrollCallback(ScrollCallback cb) override { _scrollCallback = std::move(cb); }
    void setResizeCallback(ResizeCallback cb) override { _resizeCallback = std::move(cb); }
    void setFocusCallback(FocusCallback cb) override { _focusCallback = std::move(cb); }

    std::string getClipboardText() const override {
        // TODO: Implement via JNI to ClipboardManager
        return "";
    }

    void setClipboardText(const std::string& text) override {
        // TODO: Implement via JNI to ClipboardManager
        (void)text;
    }

    void setCursor(CursorType type) override {
        // No cursor on touch devices
        (void)type;
    }

    void getContentScale(float& xscale, float& yscale) const override {
        // TODO: Get actual density from Android
        xscale = yscale = 1.0f;
    }

    std::string getKeyName(int key, int scancode) const override {
        (void)key;
        (void)scancode;
        return "";
    }

    void runMainLoop(MainLoopCallback callback) override {
        // Android: Not used - android_main handles the loop
        (void)callback;
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        // TODO: Implement Android PTY using toybox or other shell
        return Err<std::shared_ptr<PTYProvider>>("Android PTY not yet implemented");
    }

    // Called from android_main to dispatch touch events
    void dispatchTouchEvent(int action, float x, float y) {
        // Map touch to mouse events
        switch (action) {
            case AMOTION_EVENT_ACTION_DOWN:
                if (_mouseButtonCallback) {
                    _mouseButtonCallback(MouseButton::Left, true, 0);
                }
                if (_mouseMoveCallback) {
                    _mouseMoveCallback(x, y);
                }
                break;
            case AMOTION_EVENT_ACTION_UP:
                if (_mouseButtonCallback) {
                    _mouseButtonCallback(MouseButton::Left, false, 0);
                }
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                if (_mouseMoveCallback) {
                    _mouseMoveCallback(x, y);
                }
                break;
        }
    }

    // Called when window size changes
    void dispatchResize(int width, int height) {
        _width = width;
        _height = height;
        if (_resizeCallback) {
            _resizeCallback(width, height);
        }
    }

    // Called when app gains/loses focus
    void dispatchFocus(bool focused) {
        if (_focusCallback) {
            _focusCallback(focused);
        }
    }

private:
    ANativeWindow* _window = nullptr;
    int _width = 0;
    int _height = 0;
    bool _shouldClose = false;

    // Callbacks
    KeyCallback _keyCallback;
    CharCallback _charCallback;
    MouseButtonCallback _mouseButtonCallback;
    MouseMoveCallback _mouseMoveCallback;
    ScrollCallback _scrollCallback;
    ResizeCallback _resizeCallback;
    FocusCallback _focusCallback;
};

// Factory implementation for Android
Result<Platform::Ptr> Platform::create() {
    auto platform = std::make_shared<AndroidPlatform>();
    yinfo("Android platform created");
    return Ok<Ptr>(std::move(platform));
}

} // namespace yetty

#endif // __ANDROID__
