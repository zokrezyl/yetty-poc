#pragma once

#include <yetty/base/factory.h>
#include <yetty/base/event-queue.h>
#include <string>
#include <thread>
#include <atomic>
#include <functional>

// Only for GLFW platforms (Linux, macOS, Windows)
#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

struct GLFWwindow;

namespace yetty {

// GlfwWindowSingleton - owns GLFWwindow* and runs input loop on main thread
//
// Lifecycle:
// 1. Main thread: create() → creates GLFW window
// 2. Main thread: startInputLoop() → blocks on glfwWaitEvents(), pushes to EventQueue
// 3. Render thread: getWindow() → use for surface creation
// 4. Main thread: stopInputLoop() → breaks out of wait loop
//
class GlfwWindowSingleton : public base::ThreadSingleton<GlfwWindowSingleton> {
public:
    using Ptr = std::shared_ptr<GlfwWindowSingleton>;

    virtual ~GlfwWindowSingleton() = default;

    static Result<Ptr> createImpl();

    // Window access (thread-safe, window created in init)
    virtual GLFWwindow* getWindow() const = 0;

    // Window properties
    virtual void getWindowSize(int& width, int& height) const = 0;
    virtual void getFramebufferSize(int& width, int& height) const = 0;
    virtual void getContentScale(float& xscale, float& yscale) const = 0;
    virtual bool shouldClose() const = 0;

    // Window decoration (must call from main thread)
    virtual void setTitle(const std::string& title) = 0;
    virtual void setIcon(const unsigned char* data, size_t size) = 0;

    // Input loop control
    // startInputLoop() blocks the calling thread (must be main thread)
    // Events are pushed to EventQueue
    virtual void startInputLoop() = 0;
    virtual void stopInputLoop() = 0;

    // Called from render thread to wake input loop for safe shutdown
    virtual void postEmptyEvent() = 0;

protected:
    GlfwWindowSingleton() = default;
};

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
