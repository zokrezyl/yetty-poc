#include <yetty/platform/init-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)

#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
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

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("GlfwInitManager: spawning render thread");

        // Spawn render thread - window creation happens there via GlfwWindowSingleton
        std::thread renderThread([this, renderThreadFunc]() {
            ydebug("Render thread started");
            renderThreadFunc();
            ydebug("Render thread finished, stopping main loop");
            stop();
        });

        // Block on event loop (main thread)
        // glfwWaitEvents is GLOBAL - processes events for all windows
        ydebug("GlfwInitManager: starting event loop");
        _running = true;
        while (_running) {
            glfwWaitEvents();
        }
        ydebug("GlfwInitManager: event loop ended");

        // Wait for render thread
        renderThread.join();
        ydebug("GlfwInitManager: render thread joined");
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
    auto impl = std::make_shared<GlfwInitManager>();
    auto initResult = impl->init();
    if (!initResult) {
        return Err<InitManager::Ptr>("GlfwInitManager init failed", initResult);
    }
    return Ok(impl);
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__
