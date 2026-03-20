#pragma once

#include <yetty/pty-reader.h>
#include "ssh-client.h"
#include <deque>
#include <mutex>
#include <string>

namespace yetty::ssh {

enum class SshState {
    Disconnected,
    WaitingForPassword,  // Displayed "Password:" prompt, waiting for user input
    Authenticating,
    Connected
};

class SshPtyReader : public PtyReader {
public:
    SshPtyReader() = default;
    ~SshPtyReader() override;

    Result<void> init(const PtyConfig& config, const SshConfig& sshConfig);

    size_t read(char* buf, size_t maxLen) override;
    void write(const char* data, size_t len) override;
    void resize(uint32_t cols, uint32_t rows) override;
    bool isRunning() const override;
    void stop() override;
    void setDataAvailableCallback(DataAvailableCallback cb) override;
    void setExitCallback(ExitCallback cb) override;

private:
    void onSshData(const char* data, size_t len);
    void onSshDisconnect();
    void sendToTerminal(const std::string& text);
    void handlePasswordInput(char c);
    void attemptAuthentication();

    std::shared_ptr<SshClient> _client;
    SshConfig _sshConfig;
    std::mutex _mutex;
    std::deque<char> _recvBuffer;
    std::string _passwordBuffer;
    SshState _state = SshState::Disconnected;
    std::atomic<bool> _running{false};
    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

// Factory function
Result<PtyReader::Ptr> createSshPtyReader(const PtyConfig& config, const SshConfig& sshConfig);

} // namespace yetty::ssh
