#pragma once

#include <yetty/base/factory.h>
#include <atomic>

#if defined(__ANDROID__)
#include <android/native_window.h>
#include <android_native_app_glue.h>

namespace yetty {

// AndroidAppSingleton - owns android_app* and ANativeWindow* lifetime
//
// Internal glue for Android platform. All Android managers access
// platform state through this singleton.
//
// Thread-safe: uses atomics for cross-thread access (main thread sets,
// render thread reads).
//
class AndroidAppSingleton : public base::Singleton<AndroidAppSingleton> {
public:
    virtual ~AndroidAppSingleton() = default;

    // Called from main thread
    void setApp(android_app* app) { _app.store(app, std::memory_order_release); }
    void setWindow(ANativeWindow* window) { _window.store(window, std::memory_order_release); }

    // Called from any thread
    android_app* getApp() const { return _app.load(std::memory_order_acquire); }
    ANativeWindow* getWindow() const { return _window.load(std::memory_order_acquire); }

    // Paths derived from app
    std::string getDataDir() const;
    std::string getNativeLibDir() const;

    static Result<Ptr> createImpl();

protected:
    AndroidAppSingleton() = default;

private:
    std::atomic<android_app*> _app{nullptr};
    std::atomic<ANativeWindow*> _window{nullptr};
};

} // namespace yetty

#endif // __ANDROID__
