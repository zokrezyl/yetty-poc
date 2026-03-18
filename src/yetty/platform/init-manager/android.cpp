#include <yetty/platform/init-manager.h>

#if defined(__ANDROID__)

#include "../shared/android-app-singleton.h"
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <android/looper.h>
#include <android/log.h>
#include <android_native_app_glue.h>

// Forward declaration of extern "C" function defined in input-manager/android.cpp
extern "C" void yetty_android_dispatch_input(AInputEvent* event);

// Forward declaration of event loop tick function
extern "C" void yetty_android_event_loop_tick();

namespace yetty {

class AndroidInitManager : public InitManager {
public:
    Result<void> init(android_app* app) {
        _app = app;

        // Initialize AndroidAppSingleton with the app pointer
        auto singletonResult = AndroidAppSingleton::instance();
        if (!singletonResult) {
            return Err<void>("Failed to create AndroidAppSingleton", singletonResult);
        }
        (*singletonResult)->setApp(app);

        ydebug("AndroidInitManager: initialized");
        return Ok();
    }

    void run(RenderThreadFunc renderThreadFunc) override {
        __android_log_print(ANDROID_LOG_INFO, "yetty", "AndroidInitManager: starting main loop");

        _renderFunc = std::move(renderThreadFunc);
        _running = true;

        // Android runs single-threaded - process events and render in same loop
        while (_running && !_app->destroyRequested) {
            int events;
            android_poll_source* source;

            // Process all pending events (non-blocking when animating)
            while (ALooper_pollAll(_animating ? 0 : -1, nullptr, &events, (void**)&source) >= 0) {
                if (source) {
                    source->process(_app, source);
                }

                if (_app->destroyRequested) {
                    _running = false;
                    break;
                }
            }

            // When window is ready and animating
            if (_app->window && _animating && _running) {
                // First time: call setup function
                if (_renderFunc && !_renderStarted) {
                    __android_log_print(ANDROID_LOG_INFO, "yetty", "AndroidInitManager: calling renderFunc");
                    _renderStarted = true;
                    _renderFunc();
                    __android_log_print(ANDROID_LOG_INFO, "yetty", "AndroidInitManager: renderFunc returned");
                }

                // After setup: tick the event loop to process timers and render frames
                if (_renderStarted) {
                    yetty_android_event_loop_tick();
                }
            }
        }

        __android_log_print(ANDROID_LOG_INFO, "yetty", "AndroidInitManager: main loop ended");
    }

    void stop() override {
        _running = false;
        _animating = false;
    }

    void setAnimating(bool animating) {
        _animating = animating;
    }

    android_app* getApp() const { return _app; }

private:
    android_app* _app = nullptr;
    RenderThreadFunc _renderFunc;
    bool _running = false;
    bool _animating = false;
    bool _renderStarted = false;
};

// Global instance for android_main callbacks
static AndroidInitManager* g_initManager = nullptr;

// App command callback
static void handleAppCmd(android_app* app, int32_t cmd) {
    (void)app;
    if (!g_initManager) return;

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            ydebug("APP_CMD_INIT_WINDOW");
            g_initManager->setAnimating(true);
            break;
        case APP_CMD_TERM_WINDOW:
            ydebug("APP_CMD_TERM_WINDOW");
            g_initManager->setAnimating(false);
            break;
        case APP_CMD_GAINED_FOCUS:
            ydebug("APP_CMD_GAINED_FOCUS");
            g_initManager->setAnimating(true);
            break;
        case APP_CMD_LOST_FOCUS:
            ydebug("APP_CMD_LOST_FOCUS");
            g_initManager->setAnimating(false);
            break;
    }
}

// Input callback
static int32_t handleInputEvent(android_app* app, AInputEvent* event) {
    (void)app;
    // Forward to InputManager (declared at top of file)
    ::yetty_android_dispatch_input(event);
    return 1;
}

// Factory - takes android_app* via context
Result<InitManager::Ptr> InitManager::createImpl() {
    // Note: For Android, caller must use createForAndroid() instead
    return Err<Ptr>("Use createForAndroid(android_app*) on Android");
}

// Android-specific factory
Result<InitManager::Ptr> createForAndroid(android_app* app) {
    auto mgr = new AndroidInitManager();

    // Set up callbacks before init
    app->onAppCmd = handleAppCmd;
    app->onInputEvent = handleInputEvent;

    if (auto res = mgr->init(app); !res) {
        delete mgr;
        return Err<InitManager::Ptr>("AndroidInitManager init failed", res);
    }

    g_initManager = mgr;
    return Ok(InitManager::Ptr(mgr));
}

} // namespace yetty

#endif // __ANDROID__
