#pragma once

#include <yetty/platform/pty-reader.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>

namespace yetty {

class PtyReaderWindows : public PtyReader {
public:
    PtyReaderWindows() = default;
    ~PtyReaderWindows() override;

    Result<void> init(const PtyConfig& config);

    // PtyReader interface
    size_t read(char* buf, size_t maxLen) override;
    void write(const char* data, size_t len) override;
    void resize(uint32_t cols, uint32_t rows) override;
    bool isRunning() const override;
    void stop() override;
    void setDataAvailableCallback(DataAvailableCallback cb) override;
    void setExitCallback(ExitCallback cb) override;

private:
    void readerThreadFunc();

    HPCON _hPC = nullptr;
    HANDLE _hProcess = INVALID_HANDLE_VALUE;
    HANDLE _pipeInRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeInWrite = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutRead = INVALID_HANDLE_VALUE;
    HANDLE _pipeOutWrite = INVALID_HANDLE_VALUE;

    uint32_t _cols = 80;
    uint32_t _rows = 24;
    std::string _shell;
    std::string _command;
    std::atomic<bool> _running{false};

    std::thread _readerThread;
    std::mutex _bufferMutex;
    std::vector<char> _readBuffer;

    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

} // namespace yetty
