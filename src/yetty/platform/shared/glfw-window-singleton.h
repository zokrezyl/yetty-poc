#pragma once

#include <yetty/base/factory.h>
#include <string>

// Only for GLFW platforms (Linux, macOS, Windows)
#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

struct GLFWwindow;

namespace yetty {

// GlfwWindowSingleton - creates and owns GLFWwindow*
//
// ThreadSingleton - ONLY created on render thread.
// Creates the window in init().
//
class GlfwWindowSingleton : public base::ThreadSingleton<GlfwWindowSingleton> {
public:
    using Ptr = std::shared_ptr<GlfwWindowSingleton>;

    virtual ~GlfwWindowSingleton() = default;

    static Result<Ptr> createImpl();

    // Window access
    virtual GLFWwindow* getWindow() const = 0;

    // Window properties
    virtual void getWindowSize(int& width, int& height) const = 0;
    virtual void getFramebufferSize(int& width, int& height) const = 0;
    virtual void getContentScale(float& xscale, float& yscale) const = 0;
    virtual bool shouldClose() const = 0;

    // Window decoration
    virtual void setTitle(const std::string& title) = 0;
    virtual void setIcon(const unsigned char* data, size_t size) = 0;

protected:
    GlfwWindowSingleton() = default;
};

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
