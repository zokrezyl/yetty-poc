#include <yetty/platform/init-manager.h>

#if defined(_WIN32)

#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>

namespace yetty {

class WindowsInitManager : public InitManager {
public:
    ~WindowsInitManager() override {
        if (_glfwInitialized) {
            glfwTerminate();
        }
    }

    Result<void> init() {
        // Windows-specific: Win32 initialization handled by GLFW
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }
        _glfwInitialized = true;
        ydebug("WindowsInitManager: GLFW initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("WindowsInitManager: spawning render thread");

        std::thread renderThread([this, renderThreadFunc]() {
            ydebug("Render thread started");
            renderThreadFunc();
            ydebug("Render thread finished, stopping main loop");
            stop();
        });

        ydebug("WindowsInitManager: starting event loop");
        _running = true;
        while (_running) {
            glfwWaitEvents();
        }
        ydebug("WindowsInitManager: event loop ended");

        renderThread.join();
        ydebug("WindowsInitManager: render thread joined");
    }

private:
    void stop() {
        _running = false;
        glfwPostEmptyEvent();
    }

    bool _glfwInitialized = false;
    std::atomic<bool> _running{false};
};

Result<InitManager::Ptr> InitManager::createImpl() {
    auto mgr = new WindowsInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WindowsInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // _WIN32
