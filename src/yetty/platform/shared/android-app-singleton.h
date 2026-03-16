#pragma once

#include <yetty/base/factory.h>

#if defined(__ANDROID__)
#include <android/native_window.h>
#include <android_native_app_glue.h>

namespace yetty {

// AndroidAppSingleton - owns android_app* and ANativeWindow* lifetime
//
// Internal glue for Android platform. All Android managers access
// platform state through this singleton.
//
class AndroidAppSingleton : public base::ThreadSingleton<AndroidAppSingleton> {
public:
    virtual ~AndroidAppSingleton() = default;

    // Called from android_main before anything else
    void setApp(android_app* app) { _app = app; }
    void setWindow(ANativeWindow* window) { _window = window; }

    android_app* getApp() const { return _app; }
    ANativeWindow* getWindow() const { return _window; }

    // Paths derived from app
    std::string getDataDir() const;
    std::string getNativeLibDir() const;

    static Result<Ptr> createImpl();

protected:
    AndroidAppSingleton() = default;

private:
    android_app* _app = nullptr;
    ANativeWindow* _window = nullptr;
};

} // namespace yetty

#endif // __ANDROID__
