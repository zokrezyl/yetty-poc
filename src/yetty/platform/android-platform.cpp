#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#if defined(__ANDROID__)
#include <android/log.h>
#include <android/native_window.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android_native_app_glue.h>
#include <jni.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <dlfcn.h>

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

    const AndroidPaths& getAndroidPaths() const override {
        return _paths;
    }

    void setAndroidPaths(const std::string& dataDir,
                         const std::string& nativeLibDir) override {
        _paths.dataDir = dataDir;
        _paths.nativeLibDir = nativeLibDir;
        _paths.binDir = dataDir + "/bin";
        ydebug("Android paths set: dataDir={}, nativeLib={}, bin={}",
              _paths.dataDir, _paths.nativeLibDir, _paths.binDir);
        setupBinDirectory();
    }

private:
    // Toybox commands to create symlinks for
    static constexpr const char* TOYBOX_COMMANDS[] = {
        "sh", "cat", "ls", "pwd", "cd", "echo", "env", "export", "mkdir", "rm", "rmdir",
        "cp", "mv", "ln", "chmod", "chown", "touch", "head", "tail", "grep", "sed", "awk",
        "sort", "uniq", "wc", "tr", "cut", "paste", "diff", "find", "xargs", "which",
        "ps", "kill", "sleep", "date", "uname", "hostname", "id", "whoami", "basename",
        "dirname", "realpath", "readlink", "test", "[", "true", "false", "yes", "seq",
        "expr", "bc", "clear", "reset", "stty", "tty", "hexdump", "od", "xxd", "base64",
        "md5sum", "sha1sum", "sha256sum", "tar", "gzip", "gunzip", "zcat", "bzip2",
        "bunzip2", "bzcat", "xz", "unxz", "xzcat", "vi", "less", "more", "file", "stat",
        "df", "du", "free", "uptime", "top", "dmesg", "mount", "umount", "login", "su",
        "ping", "netstat", "ifconfig", "ip", "nc", "wget", "curl", "telnet", "telnetd",
        nullptr
    };

    bool createSymlink(const std::string& target, const std::string& link) {
        unlink(link.c_str());
        if (symlink(target.c_str(), link.c_str()) == 0) {
            return true;
        }
        yerror("Failed to create symlink {} -> {}: {}", link, target, strerror(errno));
        return false;
    }

    void setupBinDirectory() {
        mkdir(_paths.binDir.c_str(), 0755);

        std::string toyboxLib = _paths.nativeLibDir + "/libtoybox.so";
        std::string ycatLib = _paths.nativeLibDir + "/libycat.so";
        std::string yechoLib = _paths.nativeLibDir + "/libyecho.so";
        std::string loginLib = _paths.nativeLibDir + "/liblogin.so";

        // Create toybox symlinks
        if (access(toyboxLib.c_str(), X_OK) == 0) {
            for (int i = 0; TOYBOX_COMMANDS[i]; i++) {
                createSymlink(toyboxLib, _paths.binDir + "/" + TOYBOX_COMMANDS[i]);
            }
            ydebug("Created toybox symlinks in {}", _paths.binDir);
        }

        // Create ycat symlink
        if (access(ycatLib.c_str(), X_OK) == 0) {
            createSymlink(ycatLib, _paths.binDir + "/ycat");
            ydebug("Created ycat symlink");
        }

        // Create yecho symlink
        if (access(yechoLib.c_str(), X_OK) == 0) {
            createSymlink(yechoLib, _paths.binDir + "/yecho");
            ydebug("Created yecho symlink");
        }

        // Create login symlink
        if (access(loginLib.c_str(), X_OK) == 0) {
            createSymlink(loginLib, _paths.binDir + "/login");
            ydebug("Created login symlink");
        }
    }

public:

    Result<void> createWindow(int width, int height, const std::string& title) override {
        // On Android, window is provided by the system, not created
        // This should be called after setNativeWindow()
        (void)width;
        (void)height;
        (void)title;
        __android_log_print(ANDROID_LOG_INFO, "yetty", "createWindow called, _window=%p", _window);
        if (!_window) {
            __android_log_print(ANDROID_LOG_ERROR, "yetty", "No native window set!");
            return Err<void>("No native window set - call setNativeWindow first");
        }
        __android_log_print(ANDROID_LOG_INFO, "yetty", "Android platform window set: %dx%d", _width, _height);
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

    void setIcon(const unsigned char*, size_t) override {
        // Not supported on Android - app icon is set via manifest
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

    void requestRender() override {
        if (auto loop = base::EventLoop::instance(); loop) {
            (*loop)->dispatch(base::Event::screenUpdateEvent());
        }
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

    // Called from android_main for keyboard events
    void dispatchKeyEvent(int key, int scancode, KeyAction action, int mods) {
        if (_keyCallback) {
            _keyCallback(key, scancode, action, mods);
        }
    }

    // Called from android_main for character input
    void dispatchCharEvent(unsigned int codepoint) {
        if (_charCallback) {
            _charCallback(codepoint);
        }
    }

private:
    ANativeWindow* _window = nullptr;
    int _width = 0;
    int _height = 0;
    bool _shouldClose = false;
    AndroidPaths _paths;

    // Callbacks
    KeyCallback _keyCallback;
    CharCallback _charCallback;
    MouseButtonCallback _mouseButtonCallback;
    MouseMoveCallback _mouseMoveCallback;
    ScrollCallback _scrollCallback;
    ResizeCallback _resizeCallback;
    FocusCallback _focusCallback;
};

// Global state (set from android_main before Platform::create)
static ANativeWindow* g_nativeWindow = nullptr;
static android_app* g_androidApp = nullptr;
static AndroidPlatform* g_androidPlatform = nullptr;

// Called from android_main when window becomes available/unavailable
void android_set_native_window(void* window) {
    g_nativeWindow = static_cast<ANativeWindow*>(window);
    __android_log_print(ANDROID_LOG_INFO, "yetty", "android_set_native_window: %s",
                        window ? "window set" : "window cleared");
}

// Called from android_main to store app reference
void android_set_app(android_app* app) {
    g_androidApp = app;
    __android_log_print(ANDROID_LOG_INFO, "yetty", "android_set_app: %p", app);
}

// Extract paths from android_app and set up platform
static void initAndroidPaths(AndroidPlatform* platform) {
    if (!g_androidApp || !g_androidApp->activity) {
        yerror("Cannot init Android paths: no app/activity");
        return;
    }

    // Data directory from activity
    std::string dataDir = g_androidApp->activity->internalDataPath;

    // Native lib directory from dladdr on our own symbol
    std::string nativeLibDir;
    Dl_info info;
    if (dladdr((void*)android_set_app, &info) && info.dli_fname) {
        nativeLibDir = info.dli_fname;
        size_t lastSlash = nativeLibDir.rfind('/');
        if (lastSlash != std::string::npos) {
            nativeLibDir = nativeLibDir.substr(0, lastSlash);
        }
    }

    __android_log_print(ANDROID_LOG_INFO, "yetty",
        "Android paths: dataDir=%s, nativeLibDir=%s",
        dataDir.c_str(), nativeLibDir.c_str());

    platform->setAndroidPaths(dataDir, nativeLibDir);
}

// Factory implementation for Android
Result<Platform::Ptr> Platform::create(bool headless) {
    (void)headless;
    auto platform = std::make_shared<AndroidPlatform>();

    if (g_nativeWindow) {
        platform->setNativeWindow(g_nativeWindow);
    }

    // Extract and set Android paths
    initAndroidPaths(platform.get());

    g_androidPlatform = platform.get();
    ydebug("Android platform created");
    return Ok<Ptr>(std::move(platform));
}

// Dispatch touch events from android_main
void android_dispatch_touch(int action, float x, float y) {
    if (g_androidPlatform) {
        g_androidPlatform->dispatchTouchEvent(action, x, y);
    }
}

// Map Android keycode to platform key
static int androidKeyToPlatformKey(int keyCode) {
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
        case AKEYCODE_SPACE: return 32;        // GLFW_KEY_SPACE
        default:
            // For letter keys (A-Z)
            if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
                return 'A' + (keyCode - AKEYCODE_A);
            }
            // For number keys (0-9)
            if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
                return '0' + (keyCode - AKEYCODE_0);
            }
            return keyCode;  // Pass through unknown
    }
}

// Dispatch key events from android_main
void android_dispatch_key(int action, int keyCode, int metaState) {
    if (!g_androidPlatform) return;

    __android_log_print(ANDROID_LOG_INFO, "yetty", "KEY: action=%d keyCode=%d metaState=0x%x", action, keyCode, metaState);

    int platformKey = androidKeyToPlatformKey(keyCode);
    bool pressed = (action == AKEY_EVENT_ACTION_DOWN);

    // Build modifier flags
    int mods = 0;
    if (metaState & AMETA_SHIFT_ON) mods |= 1;  // GLFW_MOD_SHIFT
    if (metaState & AMETA_CTRL_ON) mods |= 2;   // GLFW_MOD_CONTROL
    if (metaState & AMETA_ALT_ON) mods |= 4;    // GLFW_MOD_ALT

    __android_log_print(ANDROID_LOG_INFO, "yetty", "KEY: platformKey=%d pressed=%d mods=%d", platformKey, pressed, mods);

    // Dispatch key event
    KeyAction keyAction = pressed ? KeyAction::Press : KeyAction::Release;
    g_androidPlatform->dispatchKeyEvent(platformKey, 0, keyAction, mods);

    // For printable characters, also dispatch char event on key down
    if (!pressed) return;

    bool shift = (metaState & AMETA_SHIFT_ON);
    bool ctrl = (metaState & AMETA_CTRL_ON);
    char c = 0;

    if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
        if (ctrl) {
            // Ctrl+A = 1, Ctrl+B = 2, ..., Ctrl+Z = 26
            c = 1 + (keyCode - AKEYCODE_A);
        } else {
            c = shift ? ('A' + (keyCode - AKEYCODE_A)) : ('a' + (keyCode - AKEYCODE_A));
        }
    } else if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
        // Handle shift for number row symbols
        if (shift) {
            const char* symbols = ")!@#$%^&*(";
            c = symbols[keyCode - AKEYCODE_0];
        } else {
            c = '0' + (keyCode - AKEYCODE_0);
        }
    } else {
        // Handle special characters
        switch (keyCode) {
            case AKEYCODE_SPACE: c = ' '; break;
            case AKEYCODE_ENTER: c = '\r'; break;
            case AKEYCODE_TAB: c = '\t'; break;
            case AKEYCODE_MINUS: c = shift ? '_' : '-'; break;
            case AKEYCODE_EQUALS: c = shift ? '+' : '='; break;
            case AKEYCODE_LEFT_BRACKET: c = shift ? '{' : '['; break;
            case AKEYCODE_RIGHT_BRACKET: c = shift ? '}' : ']'; break;
            case AKEYCODE_BACKSLASH: c = shift ? '|' : '\\'; break;
            case AKEYCODE_SEMICOLON: c = shift ? ':' : ';'; break;
            case AKEYCODE_APOSTROPHE: c = shift ? '"' : '\''; break;
            case AKEYCODE_GRAVE: c = shift ? '~' : '`'; break;
            case AKEYCODE_COMMA: c = shift ? '<' : ','; break;
            case AKEYCODE_PERIOD: c = shift ? '>' : '.'; break;
            case AKEYCODE_SLASH: c = shift ? '?' : '/'; break;
        }
    }

    if (c) {
        __android_log_print(ANDROID_LOG_INFO, "yetty", "CHAR: c=%d (0x%02x) '%c' ctrl=%d", c, c, c > 31 ? c : '?', ctrl);
        g_androidPlatform->dispatchCharEvent(c);
    }
}

// Show soft keyboard via JNI
void android_show_soft_keyboard(android_app* app) {
    if (!app || !app->activity) return;

    JNIEnv* env = nullptr;
    app->activity->vm->AttachCurrentThread(&env, nullptr);
    if (!env) return;

    jclass activityClass = env->GetObjectClass(app->activity->clazz);
    jmethodID getSystemService = env->GetMethodID(activityClass, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");

    jstring serviceName = env->NewStringUTF("input_method");
    jobject inputMethodManager = env->CallObjectMethod(app->activity->clazz, getSystemService, serviceName);
    env->DeleteLocalRef(serviceName);

    if (inputMethodManager) {
        jclass immClass = env->GetObjectClass(inputMethodManager);
        jmethodID toggleSoftInput = env->GetMethodID(immClass, "toggleSoftInput", "(II)V");
        env->CallVoidMethod(inputMethodManager, toggleSoftInput, 2, 0);  // SHOW_FORCED
        env->DeleteLocalRef(immClass);
        env->DeleteLocalRef(inputMethodManager);
    }

    env->DeleteLocalRef(activityClass);
    app->activity->vm->DetachCurrentThread();
}

} // namespace yetty

#endif // __ANDROID__
