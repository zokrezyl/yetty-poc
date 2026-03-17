#include <yetty/platform/pty-manager.h>

#if defined(__ANDROID__)
#include "../shared/android-app-singleton.h"
#include <yetty/pty-provider.h>

namespace yetty {

class AndroidPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        // TODO: Implement toybox-based PTY or Termux connection
        return Err<std::shared_ptr<PTYProvider>>("Android PTY not yet implemented");
    }
};

// Factory
Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new AndroidPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("AndroidPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __ANDROID__
