#include <yetty/platform/init-manager.h>

// Desktop platforms: Linux, macOS, Windows - all use GLFW
#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !YETTY_IOS

#include <yetty/yetty.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include "../shared/glfw-window-singleton.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>
#include <future>

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

    Result<void> run(int argc, char** argv) override {
        ydebug("GlfwInitManager: creating Config");

        // Create Config first (parses argc/argv)
        auto configResult = Config::create(argc, argv);
        if (!configResult) {
            return Err<void>("Failed to create Config", configResult);
        }
        auto config = *configResult;

        ydebug("GlfwInitManager: spawning render thread");

        std::promise<Result<void>> resultPromise;
        auto resultFuture = resultPromise.get_future();

        // Check for headless mode BEFORE creating window
        bool headless = config->get<bool>("vnc/headless", false);

        std::thread renderThread([this, config, headless, &resultPromise]() {
            ydebug("Render thread started");

            // Create window (skip in headless mode)
            if (!headless) {
                auto windowResult = GlfwWindowSingleton::instance();
                if (!windowResult) {
                    resultPromise.set_value(Err<void>("Failed to create window", windowResult));
                    stop();
                    return;
                }
            } else {
                ydebug("Headless mode: skipping window creation");
            }

            // Create Yetty with Config
            auto yettyResult = Yetty::create(config);
            if (!yettyResult) {
                resultPromise.set_value(Err<void>("Failed to create Yetty", yettyResult));
                stop();
                return;
            }
            auto yetty = *yettyResult;

            // Run
            auto runResult = yetty->run();
            if (!runResult) {
                resultPromise.set_value(Err<void>("Yetty run failed", runResult));
                stop();
                return;
            }

            // Shutdown
            auto shutdownResult = yetty->shutdown();
            if (!shutdownResult) {
                resultPromise.set_value(Err<void>("Yetty shutdown failed", shutdownResult));
                stop();
                return;
            }

            ydebug("Render thread finished");
            resultPromise.set_value(Ok());
            stop();
        });

        ydebug("GlfwInitManager: starting event loop (headless={})", headless);
        _running = true;
        if (headless) {
            // Headless mode: no GLFW window, just wait for render thread
            // The render thread runs its own event loop (libuv)
            renderThread.join();
        } else {
            // Normal mode: GLFW event loop
            while (_running) {
                glfwWaitEvents();
            }
            ydebug("GlfwInitManager: event loop ended");
            renderThread.join();
        }
        ydebug("GlfwInitManager: render thread joined");

        return resultFuture.get();
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
