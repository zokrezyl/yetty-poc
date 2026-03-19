#include <yetty/platform/init-manager.h>

#if defined(__linux__) && !defined(__ANDROID__)

#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>

namespace yetty {

class LinuxInitManager : public InitManager {
public:
    ~LinuxInitManager() override {
        if (_glfwInitialized) {
            glfwTerminate();
        }
    }

    Result<void> init() {
        // Linux-specific: could detect X11 vs Wayland here if needed
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }
        _glfwInitialized = true;
        ydebug("LinuxInitManager: GLFW initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("LinuxInitManager: spawning render thread");

        std::thread renderThread([this, renderThreadFunc]() {
            ydebug("Render thread started");
            renderThreadFunc();
            ydebug("Render thread finished, stopping main loop");
            stop();
        });

        ydebug("LinuxInitManager: starting event loop");
        _running = true;
        while (_running) {
            glfwWaitEvents();
        }
        ydebug("LinuxInitManager: event loop ended");

        renderThread.join();
        ydebug("LinuxInitManager: render thread joined");
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
    auto mgr = new LinuxInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("LinuxInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __linux__ && !__ANDROID__
