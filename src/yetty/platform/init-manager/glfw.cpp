#include <yetty/platform/init-manager.h>

// Desktop platforms: Linux, macOS, Windows - all use GLFW
#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <yetty/yetty.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include "../shared/glfw-window-singleton.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>

namespace yetty {

class GlfwInitManager : public InitManager {
public:
    ~GlfwInitManager() override {
        if (_glfwInitialized) {
            glfwTerminate();
        }
    }

    Result<void> init() {
        if (!glfwInit()) {
            return Err<void>("Failed to initialize GLFW");
        }
        _glfwInitialized = true;
        ydebug("GlfwInitManager: GLFW initialized");
        return Ok();
    }

    void run(int argc, char** argv) override {
        ydebug("GlfwInitManager: spawning render thread");

        std::thread renderThread([this, argc, argv]() {
            ydebug("Render thread started");

            // Create window
            auto windowResult = GlfwWindowSingleton::instance();
            if (!windowResult) {
                yerror("Failed to create window: {}", error_msg(windowResult));
                stop();
                return;
            }

            // Create Yetty
            auto result = Yetty::create(argc, argv);
            if (!result) {
                std::string msg = result.error().message();
                if (msg != "Help requested") {
                    yerror("Failed to create Yetty: {}", error_msg(result));
                }
                stop();
                return;
            }
            auto yetty = *result;

            // Run
            auto runResult = yetty->run();
            if (!runResult) {
                yerror("Yetty run failed: {}", error_msg(runResult));
            }

            // Shutdown
            auto shutdownResult = yetty->shutdown();
            if (!shutdownResult) {
                yerror("Yetty shutdown failed: {}", error_msg(shutdownResult));
            }

            ydebug("Render thread finished");
            stop();
        });

        ydebug("GlfwInitManager: starting event loop");
        _running = true;
        while (_running) {
            glfwWaitEvents();
        }
        ydebug("GlfwInitManager: event loop ended");

        renderThread.join();
        ydebug("GlfwInitManager: render thread joined");
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
    auto mgr = new GlfwInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("GlfwInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
