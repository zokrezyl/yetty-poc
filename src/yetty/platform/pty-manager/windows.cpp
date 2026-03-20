#include <yetty/platform/pty-manager.h>
#include "../pty-reader/windows.h"

namespace yetty {

class WindowsPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<PtyReader::Ptr> createPtyReader(const PtyConfig& config) override {
        auto reader = std::make_shared<PtyReaderWindows>();
        if (auto res = reader->init(config); !res) {
            return Err<PtyReader::Ptr>("Failed to create PtyReaderWindows", res);
        }
        return Ok<PtyReader::Ptr>(reader);
    }
};

Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new WindowsPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WindowsPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty
