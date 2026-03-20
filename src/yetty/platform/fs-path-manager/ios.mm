#include <yetty/platform/fs-path-manager.h>

#if YETTY_IOS

#include "../shared/ios-app-singleton.h"
#include <ytrace/ytrace.hpp>

#import <Foundation/Foundation.h>

namespace yetty {

class IosFsPathManager : public FsPathManager {
public:
    Result<void> init() {
        auto singletonResult = IosAppSingleton::instance();
        if (!singletonResult) {
            return Err<void>("IosAppSingleton not available", singletonResult);
        }

        _cachesDir = (*singletonResult)->getCachesDir();
        _bundleDir = (*singletonResult)->getBundleResourcePath();

        if (_cachesDir.empty()) {
            return Err<void>("Failed to get iOS caches directory");
        }

        ydebug("IosFsPathManager: caches={}, bundle={}", _cachesDir, _bundleDir);
        return Ok();
    }

    std::string getShadersDir() const override {
        // Shaders are bundled with the app
        return _bundleDir + "/shaders";
    }

    std::string getMsdfFontsDir() const override {
        // MSDF fonts are bundled with the app
        return _bundleDir + "/msdf-fonts";
    }

    std::string getFontsDir() const override {
        // Fonts are bundled with the app
        return _bundleDir + "/fonts";
    }

    std::string getRuntimeDir() const override {
        // Runtime data goes to caches (can be cleared by system)
        return _cachesDir;
    }

    std::string getBinDir() const override {
        // iOS doesn't have external binaries
        return _bundleDir;
    }

private:
    std::string _cachesDir;
    std::string _bundleDir;
};

// Factory
Result<FsPathManager::Ptr> FsPathManager::createImpl() {
    auto mgr = new IosFsPathManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("IosFsPathManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // YETTY_IOS
