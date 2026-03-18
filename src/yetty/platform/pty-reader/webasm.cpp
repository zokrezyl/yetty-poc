#include "webasm.h"

#include <ytrace/ytrace.hpp>
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
    if (!_running || len == 0) return;
    // TODO: Send data to JSLinux VM via JavaScript
    (void)data;
    (void)len;
}

void PtyReaderWebasm::resize(uint32_t cols, uint32_t rows) {
    _cols = cols;
    _rows = rows;
    // TODO: Notify VM of resize
}

bool PtyReaderWebasm::isRunning() const {
    return _running;
}

void PtyReaderWebasm::stop() {
    _running = false;
    // TODO: Stop VM
}

void PtyReaderWebasm::setDataAvailableCallback(DataAvailableCallback cb) {
    _dataAvailableCallback = std::move(cb);
}

void PtyReaderWebasm::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

} // namespace yetty
