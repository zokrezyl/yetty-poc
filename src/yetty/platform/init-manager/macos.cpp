#include <yetty/platform/init-manager.h>

#if defined(__APPLE__)

#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>

namespace yetty {

class MacosInitManager : public InitManager {
public:
    ~MacosInitManager() override {
        if (_glfwInitialized) {
            glfwTerminate();
        }
    }

    Result<void> init() {
        // macOS-specific: Cocoa initialization handled by GLFW
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }
        _glfwInitialized = true;
        ydebug("MacosInitManager: GLFW initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("MacosInitManager: spawning render thread");

        std::thread renderThread([this, renderThreadFunc]() {
            ydebug("Render thread started");
            renderThreadFunc();
            ydebug("Render thread finished, stopping main loop");
            stop();
        });

        ydebug("MacosInitManager: starting event loop");
        _running = true;
        while (_running) {
            glfwWaitEvents();
        }
        ydebug("MacosInitManager: event loop ended");

        renderThread.join();
        ydebug("MacosInitManager: render thread joined");
    }

    void stop() override {
        _running = false;
        glfwPostEmptyEvent();
    }

private:
    bool _glfwInitialized = false;
    std::atomic<bool> _running{false};
};

Result<InitManager::Ptr> InitManager::createImpl() {
    auto mgr = new MacosInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("MacosInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __APPLE__
