#pragma once

#include <yetty/result.hpp>
#include <yetty/base/event-loop.h>
#include <yetty/base/event-listener.h>
#include "telnet-protocol.h"
#include <string>
#include <vector>
#include <functional>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten/websocket.h>
#endif

namespace yetty::telnet {

/**
 * TelnetClient - Telnet protocol handler for terminal connections
 *
 * Implements:
 * - RFC 854  - Telnet protocol
 * - RFC 856  - Binary transmission
 * - RFC 857  - Echo
 * - RFC 858  - Suppress Go Ahead
 * - RFC 1073 - NAWS (window size)
 * - RFC 1091 - Terminal type
 * - RFC 1572 - Environment variables
 *
 * Usage for Android terminal:
 * 1. Termux runs: busybox telnetd -l /bin/bash -p 8023
 * 2. Yetty connects to localhost:8023
 * 3. Full PTY via telnet protocol over localhost TCP
 *
 * Usage for WebAssembly:
 * 1. Run telnetd on host
 * 2. Run WebSocket proxy: ./tools/telnet-websocket.sh
 * 3. Browser connects via WebSocket
 */
class TelnetClient : public base::EventListener {
public:
    using Ptr = std::shared_ptr<TelnetClient>;

    // Callbacks
    using DataCallback = std::function<void(const char* data, size_t len)>;
    using DisconnectCallback = std::function<void()>;
    using ConnectCallback = std::function<void()>;

    TelnetClient();
    ~TelnetClient() override;

    /**
     * Connect to telnet server
     * @param host Hostname, IP, or WebSocket URL (ws://...)
     * @param port Port number (ignored if host is WebSocket URL)
     */
    Result<void> connect(const std::string& host, uint16_t port);

    /**
     * Disconnect from server
     */
    void disconnect();

    /**
     * Check if connected
     */
    bool isConnected() const { return _connected; }

    /**
     * Send data to server (user input)
     */
    void send(const char* data, size_t len);

    /**
     * Send terminal resize (NAWS)
     */
    void sendWindowSize(uint16_t cols, uint16_t rows);

    /**
     * Set terminal type (sent during negotiation)
     */
    void setTerminalType(const std::string& type) { _terminalType = type; }

    /**
     * Set callback for received data (after telnet processing)
     */
    void setDataCallback(DataCallback cb) { _dataCallback = std::move(cb); }

    /**
     * Set callback for disconnection
     */
    void setDisconnectCallback(DisconnectCallback cb) { _disconnectCallback = std::move(cb); }

    /**
     * Set callback for connection established
     */
    void setConnectCallback(ConnectCallback cb) { _connectCallback = std::move(cb); }

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override;

#ifdef __EMSCRIPTEN__
    // WebSocket callbacks need access to internals
    void onWebSocketData(const uint8_t* data, size_t len);
    void onWebSocketConnected();  // Called when WS opens
    void onWebSocketDisconnected();  // Called when WS closes

    // Public for WebSocket callbacks
    bool _wsConnected = false;
    bool _wantsReconnect = false;
    std::string _wsUrl;  // For reconnection

    // Reconnection support
    void setReconnectParams(const std::string& url) { _wsUrl = url; }
    Result<void> reconnect();
#endif

private:
    // Telnet protocol state machine
    enum class State {
        DATA,       // Normal data
        IAC,        // Received IAC
        WILL,       // Received IAC WILL
        WONT,       // Received IAC WONT
        DO,         // Received IAC DO
        DONT,       // Received IAC DONT
        SB,         // In subnegotiation
        SB_IAC,     // IAC in subnegotiation (looking for SE)
    };

    void processReceived();
    void processByte(uint8_t byte);
    void handleWill(uint8_t option);
    void handleWont(uint8_t option);
    void handleDo(uint8_t option);
    void handleDont(uint8_t option);
    void handleSubnegotiation();

    void sendCommand(uint8_t cmd, uint8_t option);
    void sendSubnegotiation(uint8_t option, const uint8_t* data, size_t len);
    void sendTerminalType();
    void flushSendBuffer();
    void drainSendQueue();
    void updatePollEvents();

    // Connection state
    int _socket = -1;
    bool _connected = false;
    bool _connecting = false;
    base::PollId _pollId = -1;

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_WEBSOCKET_T _wsSocket = 0;
#endif

    // Protocol state
    State _state = State::DATA;
    std::vector<uint8_t> _subnegBuffer;  // Subnegotiation accumulator

    // Option negotiation state
    bool _nawsEnabled = false;
    bool _ttypeEnabled = false;
    bool _binaryEnabled = false;
    bool _echoEnabled = false;
    bool _sgaEnabled = false;

    // Terminal config
    std::string _terminalType = "xterm-256color";
    uint16_t _cols = 80;
    uint16_t _rows = 24;

    // Send buffer (for non-blocking writes)
    std::vector<uint8_t> _sendQueue;
    size_t _sendOffset = 0;

    // Output buffer (decoded data for terminal)
    std::vector<char> _outputBuffer;

    // Callbacks
    DataCallback _dataCallback;
    DisconnectCallback _disconnectCallback;
    ConnectCallback _connectCallback;
};

} // namespace yetty::telnet
