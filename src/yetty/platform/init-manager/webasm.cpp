#include <yetty/platform/init-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace yetty {

class WebInitManager : public InitManager {
public:
    Result<void> init() {
        ydebug("WebInitManager: initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        ydebug("WebInitManager: starting");

        _renderFunc = std::move(renderThreadFunc);
        _running = true;

        // WebASM is single-threaded - call render func directly
        // It will set up requestAnimationFrame internally
        if (_renderFunc) {
            _renderFunc();
        }

        // Note: On web, run() returns immediately
        // The browser's event loop drives everything via RAF
        ydebug("WebInitManager: run() returning (browser takes over)");
    }

private:
    RenderThreadFunc _renderFunc;
    bool _running = false;
};

// Factory
Result<InitManager::Ptr> InitManager::createImpl() {
    auto mgr = new WebInitManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebInitManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
