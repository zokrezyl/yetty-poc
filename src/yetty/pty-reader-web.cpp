#include <yetty/pty-reader.h>

#if YETTY_WEB

#include <ytrace/ytrace.hpp>
#include <cstring>
#include <vector>
#include <mutex>

namespace yetty {

class PtyReaderWeb : public PtyReader {
public:
    PtyReaderWeb() = default;
    ~PtyReaderWeb() override = default;

    Result<void> init(const PtyConfig& config) {
        _vmConfig = config.shell;
        _cols = config.cols;
        _rows = config.rows;
        _running = true;

        yinfo("PtyReaderWeb: Initialized with VM config={}, {}x{}",
              _vmConfig, _cols, _rows);

        // TODO: Initialize JSLinux VM connection here
        // The VM will call pushData() when it has output

        return Ok();
    }

    // Called from JavaScript when VM produces output
    void pushData(const char* data, size_t len) {
        if (len == 0) return;

        {
            std::lock_guard<std::mutex> lock(_bufferMutex);
            _buffer.insert(_buffer.end(), data, data + len);
        }

        // Notify terminal that data is available
        if (_dataAvailableCallback) {
            _dataAvailableCallback();
        }
    }

    // PtyReader interface

    size_t read(char* buf, size_t maxLen) override {
        std::lock_guard<std::mutex> lock(_bufferMutex);

        size_t toRead = std::min(maxLen, _buffer.size());
        if (toRead == 0) return 0;

        std::memcpy(buf, _buffer.data(), toRead);
        _buffer.erase(_buffer.begin(), _buffer.begin() + toRead);

        return toRead;
    }

    void write(const char* data, size_t len) override {
        if (!_running || len == 0) return;

        // TODO: Send data to JSLinux VM
        // This would call into JavaScript to send keyboard input to VM
        (void)data;
        (void)len;
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;

        // TODO: Notify VM of resize
    }

    bool isRunning() const override {
        return _running;
    }

    void stop() override {
        _running = false;
        // TODO: Stop VM
    }

    void setDataAvailableCallback(DataAvailableCallback cb) override {
        _dataAvailableCallback = std::move(cb);
    }

    void setExitCallback(ExitCallback cb) override {
        _exitCallback = std::move(cb);
    }

private:
    std::string _vmConfig;
    uint32_t _cols = 80;
    uint32_t _rows = 24;
    bool _running = false;

    std::mutex _bufferMutex;
    std::vector<char> _buffer;

    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

// Global instance for JS access
static std::shared_ptr<PtyReaderWeb> g_webPtyReader;

// Factory implementation for webasm
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config) {
    auto reader = std::make_shared<PtyReaderWeb>();
    if (auto res = reader->init(config); !res) {
        return Err<Ptr>("Failed to initialize PtyReaderWeb", res);
    }
    g_webPtyReader = reader;
    return Ok<Ptr>(reader);
}

} // namespace yetty

// C API for JavaScript to call
extern "C" {

void yetty_pty_push_data(const char* data, int len) {
    if (yetty::g_webPtyReader && len > 0) {
        yetty::g_webPtyReader->pushData(data, static_cast<size_t>(len));
    }
}

}

#endif // YETTY_WEB
