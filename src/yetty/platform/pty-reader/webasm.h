#pragma once

#include <yetty/platform/pty-reader.h>
#include <mutex>
#include <vector>
#include <string>

namespace yetty {

class PtyReaderWebasm : public PtyReader {
public:
    PtyReaderWebasm() = default;
    ~PtyReaderWebasm() override = default;

    Result<void> init(const PtyConfig& config);

    // Called from JavaScript when VM produces output
    void pushData(const char* data, size_t len);

    // PtyReader interface
    size_t read(char* buf, size_t maxLen) override;
    void write(const char* data, size_t len) override;
    void resize(uint32_t cols, uint32_t rows) override;
    bool isRunning() const override;
    void stop() override;
    void setDataAvailableCallback(DataAvailableCallback cb) override;
    void setExitCallback(ExitCallback cb) override;

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

} // namespace yetty
