#include <yetty/platform/pty-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/pty-provider.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <emscripten/emscripten.h>
#include <unordered_map>
#include <atomic>

namespace yetty {

// Global PTY registry for postMessage callbacks from JSLinux iframe
static std::unordered_map<uint32_t, class WebPTY*> g_ptyInstances;
static std::atomic<uint32_t> g_nextPtyId{1};

class WebPTY : public PTYProvider {
public:
    WebPTY() : _id(g_nextPtyId++) {
        g_ptyInstances[_id] = this;
    }

    ~WebPTY() override {
        stop();
        g_ptyInstances.erase(_id);
    }

    Result<void> start(const std::string& vmConfig, uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;
        _vmConfig = vmConfig;
        _running = true;

        ydebug("WebPTY[{}]: Starting with config '{}' ({}x{})", _id, vmConfig, cols, rows);

        // Create iframe and start JSLinux emulator
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
        }, _id, vmConfig.c_str(), cols, rows);

        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        ydebug("WebPTY[{}]: Stopping", _id);

        EM_ASM({
            var ptyId = $0;
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe) iframe.remove();
        }, _id);

        if (_exitCallback) {
            _exitCallback(0);
        }
    }

    void write(const char* data, size_t len) override {
        if (!_running || len == 0) return;

        EM_ASM({
            var ptyId = $0;
            var data = UTF8ToString($1, $2);
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe && iframe.contentWindow) {
                iframe.contentWindow.postMessage({
                    type: 'term-input',
                    ptyId: ptyId,
                    data: data
                }, '*');
            }
        }, _id, data, len);
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;

        EM_ASM({
            var ptyId = $0;
            var cols = $1;
            var rows = $2;
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe && iframe.contentWindow) {
                iframe.contentWindow.postMessage({
                    type: 'term-resize',
                    ptyId: ptyId,
                    cols: cols,
                    rows: rows
                }, '*');
            }
        }, _id, cols, rows);
    }

    bool isRunning() const override { return _running; }
    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

    // Called from JS when data arrives from iframe
    void onDataFromEmulator(const char* data, size_t len) {
        if (_dataCallback) {
            _dataCallback(data, len);
        }
    }

private:
    uint32_t _id;
    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _vmConfig;
    bool _running = false;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;
};

class WebPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<WebPTY>()));
    }
};

// Factory
Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new WebPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

// C function for JS to deliver data from iframe
extern "C" EMSCRIPTEN_KEEPALIVE void yetty_web_pty_data(uint32_t ptyId, const char* data, size_t len) {
    auto it = g_ptyInstances.find(ptyId);
    if (it != g_ptyInstances.end()) {
        it->second->onDataFromEmulator(data, len);
    }
}

} // namespace yetty

#endif // __EMSCRIPTEN__
