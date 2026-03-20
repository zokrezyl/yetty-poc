#include <yetty/platform/pty-manager.h>
#include "../pty-reader/webasm.h"

#include <emscripten/emscripten.h>
#include <unordered_map>

namespace yetty {

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

        uint32_t ptyId = _nextPtyId++;
        reader->setPtyId(ptyId);
        _ptyInstances[ptyId] = reader;

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

    std::shared_ptr<PtyReaderWebasm> getPtyReader(uint32_t ptyId) {
        auto it = _ptyInstances.find(ptyId);
        if (it != _ptyInstances.end()) {
            return it->second.lock();
        }
        return nullptr;
    }

private:
    std::unordered_map<uint32_t, std::weak_ptr<PtyReaderWebasm>> _ptyInstances;
    uint32_t _nextPtyId = 1;
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
    auto mgrResult = yetty::PtyManager::instance();
    if (!mgrResult) return;
    auto mgr = std::dynamic_pointer_cast<yetty::WebasmPtyManager>(*mgrResult);
    if (!mgr) return;
    auto reader = mgr->getPtyReader(ptyId);
    if (reader) {
        reader->pushData(data, static_cast<size_t>(len));
    }
}

// Called from JavaScript when data arrives from JSLinux iframe
EMSCRIPTEN_KEEPALIVE void webpty_on_data(uint32_t ptyId, const char* data, int len) {
    yetty_pty_push_data(ptyId, data, len);
}

// Legacy JS interop functions - stubs for compatibility
EMSCRIPTEN_KEEPALIVE void yetty_write(const char* data, int len) {
    (void)data; (void)len;
}

EMSCRIPTEN_KEEPALIVE void yetty_key(int key, int mods) {
    (void)key; (void)mods;
}

EMSCRIPTEN_KEEPALIVE void yetty_special_key(int key, int mods) {
    (void)key; (void)mods;
}

EMSCRIPTEN_KEEPALIVE int yetty_read_input(char* buffer, int maxLen) {
    (void)buffer; (void)maxLen;
    return 0;
}

EMSCRIPTEN_KEEPALIVE void yetty_sync() {
}

EMSCRIPTEN_KEEPALIVE void yetty_set_scale(float scaleX, float scaleY) {
    (void)scaleX; (void)scaleY;
}

EMSCRIPTEN_KEEPALIVE void yetty_resize(int cols, int rows) {
    (void)cols; (void)rows;
}

EMSCRIPTEN_KEEPALIVE int yetty_get_cols() {
    return 80;
}

EMSCRIPTEN_KEEPALIVE int yetty_get_rows() {
    return 24;
}

} // extern "C"

// Initialize message listener at startup
static struct WebPTYInit {
    WebPTYInit() {
        EM_ASM({
            window.addEventListener('message', function(e) {
                if (e.data && e.data.type === 'term-output' && e.data.ptyId) {
                    var data = e.data.data;
                    if (!data || data.length === 0) return;
                    var ptyId = parseInt(e.data.ptyId, 10);
                    if (isNaN(ptyId)) return;
                    var encoder = new TextEncoder();
                    var bytes = encoder.encode(data);
                    if (bytes.length === 0) return;
                    var ptr = Module._malloc(bytes.length);
                    if (ptr === 0) return;
                    Module.HEAPU8.set(bytes, ptr);
                    Module._webpty_on_data(ptyId, ptr, bytes.length);
                    Module._free(ptr);
                }
            });
        });
    }
} g_webPtyInit;
