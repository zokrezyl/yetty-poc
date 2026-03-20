#include "webasm.h"

#include <ytrace/ytrace.hpp>
#include <emscripten/emscripten.h>
#include <cstring>

namespace yetty {

Result<void> PtyReaderWebasm::init(const PtyConfig& config) {
    _vmConfig = config.shell;
    _cols = config.cols;
    _rows = config.rows;
    _running = true;

    ydebug("PtyReaderWebasm: Initialized with VM config={}, {}x{}", _vmConfig, _cols, _rows);
    return Ok();
}

void PtyReaderWebasm::pushData(const char* data, size_t len) {
    if (len == 0) return;

    {
        std::lock_guard<std::mutex> lock(_bufferMutex);
        _buffer.insert(_buffer.end(), data, data + len);
    }

    if (_dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

size_t PtyReaderWebasm::read(char* buf, size_t maxLen) {
    std::lock_guard<std::mutex> lock(_bufferMutex);

    size_t toRead = std::min(maxLen, _buffer.size());
    if (toRead == 0) return 0;

    std::memcpy(buf, _buffer.data(), toRead);
    _buffer.erase(_buffer.begin(), _buffer.begin() + toRead);

    return toRead;
}

void PtyReaderWebasm::write(const char* data, size_t len) {
    if (!_running || len == 0 || _ptyId == 0) return;

    ydebug("PtyReaderWebasm::write: ptyId={} len={}", _ptyId, len);

    // Send input to JSLinux iframe via postMessage
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
    }, _ptyId, data, len);
}

void PtyReaderWebasm::resize(uint32_t cols, uint32_t rows) {
    _cols = cols;
    _rows = rows;
    if (_ptyId == 0) return;

    ydebug("PtyReaderWebasm::resize: ptyId={} {}x{}", _ptyId, cols, rows);

    // Send resize to JSLinux iframe via postMessage
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
    }, _ptyId, cols, rows);
}

bool PtyReaderWebasm::isRunning() const {
    return _running;
}

void PtyReaderWebasm::stop() {
    if (!_running) return;
    _running = false;

    ydebug("PtyReaderWebasm::stop: ptyId={}", _ptyId);

    if (_ptyId != 0) {
        // Remove JSLinux iframe
        EM_ASM({
            var ptyId = $0;
            var iframe = document.getElementById('jslinux-pty-' + ptyId);
            if (iframe) {
                iframe.remove();
            }
        }, _ptyId);
    }

    if (_exitCallback) {
        _exitCallback(0);
    }
}

void PtyReaderWebasm::setDataAvailableCallback(DataAvailableCallback cb) {
    _dataAvailableCallback = std::move(cb);
}

void PtyReaderWebasm::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

} // namespace yetty
