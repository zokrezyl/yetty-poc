#pragma once

#include <yetty/platform/pty-reader.h>
#include <yetty/platform/event-loop.h>
#include <string>

namespace yetty {

class PtyReaderUnix : public PtyReader, public base::EventListener {
public:
    PtyReaderUnix() = default;
    ~PtyReaderUnix() override;

    Result<void> init(const PtyConfig& config);

    // PtyReader interface
    size_t read(char* buf, size_t maxLen) override;
    void write(const char* data, size_t len) override;
    void resize(uint32_t cols, uint32_t rows) override;
    bool isRunning() const override;
    void stop() override;
    void setDataAvailableCallback(DataAvailableCallback cb) override;
    void setExitCallback(ExitCallback cb) override;

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override;

private:
    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
    uint32_t _cols = 80;
    uint32_t _rows = 24;
    std::string _shell;
    std::string _command;
    bool _running = false;
    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

} // namespace yetty
