#include "android-app-singleton.h"

#if defined(__ANDROID__)
#include <dlfcn.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

std::string AndroidAppSingleton::getDataDir() const {
    auto app = getApp();
    if (app && app->activity) {
        return app->activity->internalDataPath;
    }
    return "";
}

std::string AndroidAppSingleton::getNativeLibDir() const {
    // Get native lib directory from dladdr on our own symbol
    Dl_info info;
    if (dladdr((void*)&AndroidAppSingleton::createImpl, &info) && info.dli_fname) {
        std::string path = info.dli_fname;
        size_t lastSlash = path.rfind('/');
        if (lastSlash != std::string::npos) {
            return path.substr(0, lastSlash);
        }
    }
    return "";
}

Result<AndroidAppSingleton::Ptr> AndroidAppSingleton::createImpl() {
    return Ok(Ptr(new AndroidAppSingleton()));
}

} // namespace yetty

#endif // __ANDROID__
