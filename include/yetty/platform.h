#pragma once

#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <functional>
#include <memory>
#include <string>

namespace yetty {

// Cursor types (subset that makes sense across platforms)
enum class CursorType {
    Arrow,
    IBeam,
    Hand,
    ResizeH,
    ResizeV,
    Hidden
};

// Key action
enum class KeyAction {
    Press,
    Release,
    Repeat
};

// Mouse button
enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2
};

// Platform abstraction layer
// Handles window creation, input, and platform-specific functionality
class Platform {
public:
    using Ptr = std::shared_ptr<Platform>;

    // Callback types
    using KeyCallback = std::function<void(int key, int scancode, KeyAction action, int mods)>;
    using CharCallback = std::function<void(unsigned int codepoint)>;
    using MouseButtonCallback = std::function<void(MouseButton button, bool pressed, int mods)>;
    using MouseMoveCallback = std::function<void(double x, double y)>;
    using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
    using ResizeCallback = std::function<void(int width, int height)>;
    using FocusCallback = std::function<void(bool focused)>;

    virtual ~Platform() = default;

    // Factory - creates platform-appropriate implementation
    static Result<Ptr> create();

    // Window management
    virtual Result<void> createWindow(int width, int height, const std::string& title) = 0;
    virtual void destroyWindow() = 0;
    virtual void* getNativeHandle() const = 0;  // GLFWwindow*, ANativeWindow*, etc.
    virtual void getWindowSize(int& width, int& height) const = 0;
    virtual void getFramebufferSize(int& width, int& height) const = 0;
    virtual bool shouldClose() const = 0;
    virtual void setTitle(const std::string& title) = 0;

    // WebGPU surface creation (platform-specific)
    virtual WGPUSurface createWGPUSurface(WGPUInstance instance) = 0;

    // Event loop
    virtual void pollEvents() = 0;
    virtual double getTime() const = 0;

    // Main loop callback type - returns false to stop loop
    using MainLoopCallback = std::function<bool()>;

    // Run the main loop (platform-specific implementation)
    // - Desktop: Just calls the callback in a loop until it returns false
    // - Web: Uses emscripten_request_animation_frame_loop
    virtual void runMainLoop(MainLoopCallback callback) = 0;

    // Input callbacks
    virtual void setKeyCallback(KeyCallback cb) = 0;
    virtual void setCharCallback(CharCallback cb) = 0;
    virtual void setMouseButtonCallback(MouseButtonCallback cb) = 0;
    virtual void setMouseMoveCallback(MouseMoveCallback cb) = 0;
    virtual void setScrollCallback(ScrollCallback cb) = 0;
    virtual void setResizeCallback(ResizeCallback cb) = 0;
    virtual void setFocusCallback(FocusCallback cb) = 0;

    // Clipboard (optional - may return empty/do nothing on some platforms)
    virtual std::string getClipboardText() const = 0;
    virtual void setClipboardText(const std::string& text) = 0;

    // Cursor (no-op on touch platforms)
    virtual void setCursor(CursorType type) = 0;

    // Content scale (for HiDPI)
    virtual void getContentScale(float& xscale, float& yscale) const = 0;

    // Key name lookup (for GLFW key codes)
    virtual std::string getKeyName(int key, int scancode) const = 0;

protected:
    Platform() = default;
};

} // namespace yetty
