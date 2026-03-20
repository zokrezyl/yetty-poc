#pragma once

#include <yetty/result.hpp>
#include <functional>
#include <memory>
#include <string>
#include <cstdint>

namespace yetty::ssh {

struct SshConfig {
    std::string host = "127.0.0.1";
    uint16_t port = 22;
    std::string username;
    std::string password;           // Password auth (if set)
    std::string privateKeyPath;     // Key-based auth (if set)
    std::string privateKeyPassphrase;
    std::string termType = "xterm-256color";
    uint16_t cols = 80;
    uint16_t rows = 24;
};

class SshClient {
public:
    using Ptr = std::shared_ptr<SshClient>;
    using DataCallback = std::function<void(const char*, size_t)>;
    using DisconnectCallback = std::function<void()>;

    SshClient();
    ~SshClient();

    // Non-copyable
    SshClient(const SshClient&) = delete;
    SshClient& operator=(const SshClient&) = delete;

    // Full connect (handshake + auth + shell) - use if password already known
    Result<void> connect(const SshConfig& config);

    // Step-by-step connection for interactive password entry:
    // 1. connectNoAuth() - handshake only
    // 2. Check needsPassword()
    // 3. If true, prompt user then call authenticatePassword()
    // 4. Finally call openShell()
    Result<void> connectNoAuth(const SshConfig& config);
    bool needsPassword() const;
    Result<void> authenticatePassword(const std::string& password);
    Result<void> openShell();

    void disconnect();
    bool isConnected() const;

    void send(const char* data, size_t len);
    void sendWindowSize(uint16_t cols, uint16_t rows);

    void setDataCallback(DataCallback cb);
    void setDisconnectCallback(DisconnectCallback cb);

    // Process pending I/O - call this regularly
    void poll();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace yetty::ssh
