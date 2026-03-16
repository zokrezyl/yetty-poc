#include "glfw-window-singleton.h"

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <yetty/base/event-queue.h>
#include <yetty/base/event.h>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>

namespace yetty {

class GlfwWindowSingletonImpl : public GlfwWindowSingleton {
public:
    ~GlfwWindowSingletonImpl() override {
        if (_window) {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    Result<void> init() {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        _window = glfwCreateWindow(1280, 720, "yetty", nullptr, nullptr);
        if (!_window) {
            return Err<void>("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(_window, this);
        setupCallbacks();

        ydebug("GlfwWindowSingleton: window created");
        return Ok();
    }

    GLFWwindow* getWindow() const override {
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

    void getContentScale(float& xscale, float& yscale) const override {
        if (_window) {
            glfwGetWindowContentScale(_window, &xscale, &yscale);
        } else {
            xscale = yscale = 1.0f;
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

    void setIcon(const unsigned char* data, size_t size) override {
        // TODO: Decode PNG and set icon
        (void)data; (void)size;
    }

private:
    void setupCallbacks() {
        glfwSetKeyCallback(_window, keyCallback);
        glfwSetCharCallback(_window, charCallback);
        glfwSetMouseButtonCallback(_window, mouseButtonCallback);
        glfwSetCursorPosCallback(_window, cursorPosCallback);
        glfwSetScrollCallback(_window, scrollCallback);
        glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            (*queueResult)->push(base::Event::keyDown(key, mods, scancode));
        } else if (action == GLFW_RELEASE) {
            (*queueResult)->push(base::Event::keyUp(key, mods, scancode));
        }
    }

    static void charCallback(GLFWwindow* window, unsigned int codepoint) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;
        (*queueResult)->push(base::Event::charInput(codepoint));
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        if (action == GLFW_PRESS) {
            (*queueResult)->push(base::Event::mouseDown(
                static_cast<float>(x), static_cast<float>(y), button, mods));
        } else {
            (*queueResult)->push(base::Event::mouseUp(
                static_cast<float>(x), static_cast<float>(y), button, mods));
        }
    }

    static void cursorPosCallback(GLFWwindow* window, double x, double y) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;
        (*queueResult)->push(base::Event::mouseMove(
            static_cast<float>(x), static_cast<float>(y)));
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        int mods = 0;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            mods |= 0x0001;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            mods |= 0x0002;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
            mods |= 0x0004;
        }

        (*queueResult)->push(base::Event::scrollEvent(
            static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(xoffset), static_cast<float>(yoffset), mods));
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        (void)window;
        auto queueResult = base::EventQueue::instance();
        if (!queueResult) return;
        (*queueResult)->push(base::Event::resizeEvent(
            static_cast<float>(width), static_cast<float>(height)));
    }

    GLFWwindow* _window = nullptr;
};

Result<GlfwWindowSingleton::Ptr> GlfwWindowSingleton::createImpl() {
    auto impl = Ptr(new GlfwWindowSingletonImpl());
    auto initResult = static_cast<GlfwWindowSingletonImpl*>(impl.get())->init();
    if (!initResult) {
        return Err<Ptr>("GlfwWindowSingleton init failed", initResult);
    }
    return Ok(std::move(impl));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
