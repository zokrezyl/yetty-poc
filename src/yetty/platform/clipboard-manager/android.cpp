#include <yetty/platform/clipboard-manager.h>

#if defined(__ANDROID__)
#include "../shared/android-app-singleton.h"
#include <jni.h>

namespace yetty {

class AndroidClipboardManager : public ClipboardManager {
public:
    Result<void> init() {
        auto appResult = AndroidAppSingleton::instance();
        if (!appResult) {
            return Err<void>("AndroidAppSingleton not available", appResult);
        }
        _appSingleton = *appResult;
        return Ok();
    }

    std::string getText() const override {
        // TODO: Implement via JNI to ClipboardManager
        // android_app* app = _appSingleton->getApp();
        // JNIEnv* env = ...
        return "";
    }

    void setText(const std::string& text) override {
        // TODO: Implement via JNI to ClipboardManager
        (void)text;
    }

private:
    AndroidAppSingleton::Ptr _appSingleton;
};

// Factory
Result<ClipboardManager::Ptr> ClipboardManager::createImpl() {
    auto impl = Ptr(new AndroidClipboardManager());
    auto initResult = static_cast<AndroidClipboardManager*>(impl.get())->init();
    if (!initResult) {
        return Err<Ptr>("AndroidClipboardManager init failed", initResult);
    }
    return Ok(std::move(impl));
}

} // namespace yetty

#endif // __ANDROID__
