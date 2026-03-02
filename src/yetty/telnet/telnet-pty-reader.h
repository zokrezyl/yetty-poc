#pragma once

#include <yetty/pty-reader.h>
#include "telnet-client.h"
#include <mutex>
#include <deque>

namespace yetty::telnet {

/**
 * TelnetPtyReader - PtyReader implementation using Telnet protocol
 *
 * Connects to a telnet server (typically localhost) and provides
 * PTY-like interface. Designed for Android where Termux runs telnetd.
 *
 * Setup in Termux:
 *   pkg install busybox
 *   busybox telnetd -l /bin/bash -p 8023 -F
 *
 * Or with socat for better PTY support:
 *   pkg install socat
 *   socat TCP-LISTEN:8023,reuseaddr,fork EXEC:bash,pty,stderr,setsid
 */
class TelnetPtyReader : public PtyReader {
public:
    TelnetPtyReader() = default;
    ~TelnetPtyReader() override;

    /**
     * Initialize and connect
     * @param config.shell - "host:port" format (e.g., "127.0.0.1:8023")
     *                       or just "port" for localhost
     */
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
    void onTelnetData(const char* data, size_t len);
    void onTelnetDisconnect();

    TelnetClient::Ptr _client;

    // Receive buffer (thread-safe)
    mutable std::mutex _mutex;
    std::deque<char> _recvBuffer;

    // Callbacks
    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;

    bool _running = false;
};

} // namespace yetty::telnet
