#include <yetty/platform/pty-manager.h>
#include "../pty-reader/webasm.h"

#include <emscripten/emscripten.h>
#include <unordered_map>

namespace yetty {

// Global registry for JS callbacks
static std::unordered_map<uint32_t, std::weak_ptr<PtyReaderWebasm>> g_ptyInstances;
static uint32_t g_nextPtyId = 1;

class WebasmPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<PtyReader::Ptr> createPtyReader(const PtyConfig& config) override {
        auto reader = std::make_shared<PtyReaderWebasm>();
        if (auto res = reader->init(config); !res) {
            return Err<PtyReader::Ptr>("Failed to create PtyReaderWebasm", res);
        }

        uint32_t ptyId = g_nextPtyId++;
        g_ptyInstances[ptyId] = reader;

        // Create iframe for JSLinux
        EM_ASM({
            var ptyId = $0;
            var vmConfig = UTF8ToString($1);
            var cols = $2;
            var rows = $3;

            var iframe = document.createElement('iframe');
            iframe.id = 'jslinux-pty-' + ptyId;
            iframe.style.cssText = 'display:none;';
            iframe.src = 'jslinux/vm-bridge.html?ptyId=' + ptyId +
                         '&url=' + encodeURIComponent(vmConfig) +
                         '&cols=' + cols + '&rows=' + rows +
                         '&cpu=x86_64&mem=256';
            document.body.appendChild(iframe);
        }, ptyId, config.shell.c_str(), config.cols, config.rows);

        return Ok<PtyReader::Ptr>(reader);
    }
};

Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new WebasmPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebasmPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

// C API for JavaScript
extern "C" {

EMSCRIPTEN_KEEPALIVE void yetty_pty_push_data(uint32_t ptyId, const char* data, int len) {
    auto it = yetty::g_ptyInstances.find(ptyId);
    if (it != yetty::g_ptyInstances.end()) {
        if (auto reader = it->second.lock()) {
            reader->pushData(data, static_cast<size_t>(len));
        }
    }
}

}
