#include "telnet-client.h"
#include <ytrace/ytrace.hpp>

#include <cstring>

#ifdef __EMSCRIPTEN__
#include <emscripten/websocket.h>
#include <emscripten/emscripten.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#endif

namespace yetty::telnet {

using yetty::Ok;
using yetty::Err;

#ifdef __EMSCRIPTEN__
// WebSocket callbacks for Emscripten
static EM_BOOL onWsOpen(int eventType, const EmscriptenWebSocketOpenEvent* wsEvent, void* userData) {
    TelnetClient* client = static_cast<TelnetClient*>(userData);
    client->onWebSocketConnected();
    return EM_TRUE;
}

static EM_BOOL onWsError(int eventType, const EmscriptenWebSocketErrorEvent* wsEvent, void* userData) {
    TelnetClient* client = static_cast<TelnetClient*>(userData);
    yerror("Telnet WebSocket error");
    client->onWebSocketDisconnected();
    client->_wantsReconnect = true;
    return EM_TRUE;
}

static EM_BOOL onWsClose(int eventType, const EmscriptenWebSocketCloseEvent* wsEvent, void* userData) {
    TelnetClient* client = static_cast<TelnetClient*>(userData);
    ydebug("Telnet WebSocket closed: code={} reason={}", wsEvent->code, wsEvent->reason);
    client->onWebSocketDisconnected();
    client->_wantsReconnect = true;
    return EM_TRUE;
}

static EM_BOOL onWsMessage(int eventType, const EmscriptenWebSocketMessageEvent* wsEvent, void* userData) {
    TelnetClient* client = static_cast<TelnetClient*>(userData);
    if (wsEvent->isText) {
        ywarn("Telnet WebSocket received text message, expected binary");
        return EM_TRUE;
    }
    // Process binary data through telnet protocol
    client->onWebSocketData(wsEvent->data, wsEvent->numBytes);
    return EM_TRUE;
}
#endif

TelnetClient::TelnetClient() {
    _outputBuffer.reserve(4096);
    _subnegBuffer.reserve(256);
}

TelnetClient::~TelnetClient() {
    disconnect();
}

Result<void> TelnetClient::connect(const std::string& host, uint16_t port) {
    if (_connected || _connecting) {
        return Err<void>("Already connected or connecting");
    }

#ifdef __EMSCRIPTEN__
    // WebSocket connection for Emscripten
    // host may be either:
    // - a hostname (legacy): build ws://host:port
    // - a full WebSocket URL (from web UI): use as-is
    std::string wsUrl;
    if (host.find("ws://") == 0 || host.find("wss://") == 0) {
        // Full URL provided (from YETTY_TELNET env var)
        wsUrl = host;
    } else {
        // Legacy: build URL from host:port
        wsUrl = "ws://" + host + ":" + std::to_string(port);
    }
    _wsUrl = wsUrl;  // Store for reconnection
    ydebug("Telnet client connecting via WebSocket to {}", wsUrl);

    EmscriptenWebSocketCreateAttributes attrs;
    emscripten_websocket_init_create_attributes(&attrs);
    attrs.url = wsUrl.c_str();
    attrs.protocols = nullptr;
    attrs.createOnMainThread = EM_TRUE;

    _wsSocket = emscripten_websocket_new(&attrs);
    if (_wsSocket <= 0) {
        return Err<void>("Failed to create WebSocket");
    }

    // Set up callbacks
    emscripten_websocket_set_onopen_callback(_wsSocket, this, onWsOpen);
    emscripten_websocket_set_onerror_callback(_wsSocket, this, onWsError);
    emscripten_websocket_set_onclose_callback(_wsSocket, this, onWsClose);
    emscripten_websocket_set_onmessage_callback(_wsSocket, this, onWsMessage);

    _connecting = true;
    _connected = true;  // Consider connected for send operations
    ydebug("Telnet client: WebSocket created, waiting for connection...");
    return Ok();

#else
    // POSIX socket connection
    // Resolve host
    struct addrinfo hints = {}, *result;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &result) != 0) {
        return Err<void>("Failed to resolve host: " + host);
    }

    // Create socket
    _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (_socket < 0) {
        freeaddrinfo(result);
        return Err<void>("Failed to create socket");
    }

    // Set non-blocking before connect for async connection
    int flags = fcntl(_socket, F_GETFL, 0);
    fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

    // Disable Nagle's algorithm for lower latency
    int flag = 1;
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    // Non-blocking connect
    int connectResult = ::connect(_socket, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    if (connectResult < 0 && errno != EINPROGRESS) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to connect: " + std::string(strerror(errno)));
    }

    // If EINPROGRESS, wait for connection to complete with a short timeout
    // This ensures we know immediately if the connection fails (no server)
    if (errno == EINPROGRESS) {
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(_socket, &writefds);

        struct timeval tv;
        tv.tv_sec = 1;  // 1 second timeout
        tv.tv_usec = 0;

        int ret = select(_socket + 1, nullptr, &writefds, nullptr, &tv);
        if (ret <= 0) {
            close(_socket);
            _socket = -1;
            return Err<void>("Connection timed out");
        }

        // Check if connection succeeded or failed
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
            close(_socket);
            _socket = -1;
            return Err<void>("Connection failed: " + std::string(strerror(error ? error : errno)));
        }
    }

    _connecting = false;
    _connected = true;
    ydebug("Telnet connected to {}:{} socket={}", host, port, _socket);

    // Register with event loop
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        close(_socket);
        _socket = -1;
        return Err<void>("No event loop available");
    }
    auto loop = *loopResult;

    auto pollRes = loop->createPoll();
    if (!pollRes) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to create poll", pollRes);
    }
    _pollId = *pollRes;

    if (auto res = loop->configPoll(_pollId, _socket); !res) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to configure poll", res);
    }

    auto listener = sharedAs<base::EventListener>();
    if (!listener) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to get shared_ptr<EventListener>");
    }

    if (auto res = loop->registerPollListener(_pollId, listener); !res) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to register poll listener", res);
    }

    // Start polling - if connecting async, need writable to detect connect completion
    int pollEvents = base::EventLoop::POLL_READABLE;
    if (_connecting) {
        pollEvents |= base::EventLoop::POLL_WRITABLE;
    }

    if (auto res = loop->startPoll(_pollId, pollEvents); !res) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to start poll", res);
    }

    ydebug("Telnet: registered poll id={} socket={}", _pollId, _socket);
    return Ok();
#endif
}

void TelnetClient::disconnect() {
    _connected = false;
    _connecting = false;

#ifdef __EMSCRIPTEN__
    if (_wsSocket > 0) {
        emscripten_websocket_close(_wsSocket, 1000, "disconnect");
        emscripten_websocket_delete(_wsSocket);
        _wsSocket = 0;
    }
    _wsConnected = false;
#else
    if (_pollId >= 0) {
        if (auto loopResult = base::EventLoop::instance(); loopResult) {
            auto loop = *loopResult;
            loop->stopPoll(_pollId);
            loop->destroyPoll(_pollId);
        }
        _pollId = -1;
    }

    if (_socket >= 0) {
        ::shutdown(_socket, SHUT_RDWR);
        ::close(_socket);
        _socket = -1;
    }
#endif

    _state = State::DATA;
    _subnegBuffer.clear();
    _sendQueue.clear();
    _sendOffset = 0;
    _outputBuffer.clear();

    // Reset option state
    _nawsEnabled = false;
    _ttypeEnabled = false;
    _binaryEnabled = false;
    _echoEnabled = false;
    _sgaEnabled = false;
}

void TelnetClient::send(const char* data, size_t len) {
#ifdef __EMSCRIPTEN__
    if (!_connected || !_wsConnected || _wsSocket <= 0) return;
#else
    if (!_connected || _socket < 0) return;
#endif

    // Escape IAC bytes in data (IAC -> IAC IAC)
    for (size_t i = 0; i < len; i++) {
        uint8_t byte = static_cast<uint8_t>(data[i]);
        _sendQueue.push_back(byte);
        if (byte == IAC) {
            _sendQueue.push_back(IAC);  // Escape
        }
    }

    drainSendQueue();
}

void TelnetClient::sendWindowSize(uint16_t cols, uint16_t rows) {
    _cols = cols;
    _rows = rows;

    if (!_nawsEnabled) {
        ydebug("Telnet: sendWindowSize skipped, NAWS not enabled");
        return;  // Server hasn't agreed to NAWS yet
    }

    ydebug("Telnet: sendWindowSize {}x{}, queuing subnegotiation", cols, rows);
    // NAWS: IAC SB NAWS <width-hi> <width-lo> <height-hi> <height-lo> IAC SE
    uint8_t naws[4] = {
        static_cast<uint8_t>((cols >> 8) & 0xFF),
        static_cast<uint8_t>(cols & 0xFF),
        static_cast<uint8_t>((rows >> 8) & 0xFF),
        static_cast<uint8_t>(rows & 0xFF),
    };
    sendSubnegotiation(OPT_NAWS, naws, 4);
    ydebug("Telnet: sent NAWS {}x{}", cols, rows);
}

void TelnetClient::sendCommand(uint8_t cmd, uint8_t option) {
    uint8_t buf[3] = { IAC, cmd, option };
    _sendQueue.insert(_sendQueue.end(), buf, buf + 3);
    drainSendQueue();
}

void TelnetClient::sendSubnegotiation(uint8_t option, const uint8_t* data, size_t len) {
    _sendQueue.push_back(IAC);
    _sendQueue.push_back(SB);
    _sendQueue.push_back(option);

    // Escape IAC in subnegotiation data
    for (size_t i = 0; i < len; i++) {
        _sendQueue.push_back(data[i]);
        if (data[i] == IAC) {
            _sendQueue.push_back(IAC);
        }
    }

    _sendQueue.push_back(IAC);
    _sendQueue.push_back(SE);
    drainSendQueue();
}

void TelnetClient::sendTerminalType() {
    // TTYPE IS: IAC SB TTYPE IS <terminal-type> IAC SE
    std::vector<uint8_t> data;
    data.push_back(TTYPE_IS);
    for (char c : _terminalType) {
        data.push_back(static_cast<uint8_t>(c));
    }
    sendSubnegotiation(OPT_TTYPE, data.data(), data.size());
    ydebug("Telnet: sent TTYPE IS {}", _terminalType);
}

void TelnetClient::drainSendQueue() {
    if (_sendQueue.empty()) return;

#ifdef __EMSCRIPTEN__
    if (_wsSocket <= 0 || !_wsConnected) {
        ytrace("Telnet drainSendQueue: not ready (socket={} connected={})", _wsSocket, _wsConnected);
        return;
    }

    size_t toSend = _sendQueue.size() - _sendOffset;
    ytrace("Telnet drainSendQueue: sending {} bytes via WebSocket", toSend);

    // Send all data via WebSocket
    EMSCRIPTEN_RESULT result = emscripten_websocket_send_binary(
        _wsSocket,
        _sendQueue.data() + _sendOffset,
        toSend
    );

    if (result != EMSCRIPTEN_RESULT_SUCCESS) {
        yerror("Telnet: WebSocket send error: {}", result);
        return;
    }

    ytrace("Telnet drainSendQueue: sent successfully");
    // All sent
    _sendQueue.clear();
    _sendOffset = 0;
#else
    if (_socket < 0) return;

    while (_sendOffset < _sendQueue.size()) {
        ssize_t sent = ::send(_socket,
                              _sendQueue.data() + _sendOffset,
                              _sendQueue.size() - _sendOffset,
                              MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                updatePollEvents();
                return;  // Wait for writable
            }
            yerror("Telnet: send error: {}", strerror(errno));
            disconnect();
            if (_disconnectCallback) _disconnectCallback();
            return;
        }
        _sendOffset += static_cast<size_t>(sent);
    }

    // All sent
    _sendQueue.clear();
    _sendOffset = 0;
    updatePollEvents();
#endif
}

#ifndef __EMSCRIPTEN__
void TelnetClient::updatePollEvents() {
    if (_pollId < 0) return;

    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return;
    auto loop = *loopResult;

    int events = base::EventLoop::POLL_READABLE;
    if (!_sendQueue.empty() || _connecting) {
        events |= base::EventLoop::POLL_WRITABLE;
    }
    loop->setPollEvents(_pollId, events);
}

Result<bool> TelnetClient::onEvent(const base::Event& event) {
    ydebug("TelnetClient::onEvent: type={} fd={} socket={}", (int)event.type, event.poll.fd, _socket);

    if (event.type == base::Event::Type::PollWritable && event.poll.fd == _socket) {
        ydebug("TelnetClient::onEvent: PollWritable");
        if (_connecting) {
            // Check if connect completed
            int error = 0;
            socklen_t len = sizeof(error);
            if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
                yerror("Telnet: async connect failed: {}", strerror(error ? error : errno));
                disconnect();
                if (_disconnectCallback) _disconnectCallback();
                return Ok(true);
            }

            _connecting = false;
            _connected = true;
            ydebug("Telnet: async connect completed");
            updatePollEvents();
        } else {
            // Socket writable - drain send queue
            drainSendQueue();
        }
        return Ok(true);
    }

    if (event.type == base::Event::Type::PollReadable && event.poll.fd == _socket) {
        ydebug("TelnetClient::onEvent: PollReadable - calling processReceived");
        processReceived();
        return Ok(true);
    }

    return Ok(false);
}

void TelnetClient::processReceived() {
    uint8_t buf[4096];
    ssize_t n = ::recv(_socket, buf, sizeof(buf), 0);
    ydebug("TelnetClient::processReceived: recv returned {}", n);

    if (n <= 0) {
        if (n == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
            ydebug("TelnetClient: connection closed n={} errno={}", n, errno);
            disconnect();
            if (_disconnectCallback) _disconnectCallback();
        }
        return;
    }

    _outputBuffer.clear();

    for (ssize_t i = 0; i < n; i++) {
        processByte(buf[i]);
    }

    // Deliver decoded data to terminal
    if (!_outputBuffer.empty() && _dataCallback) {
        // Log first bytes for debugging
        std::string preview;
        for (size_t i = 0; i < std::min(_outputBuffer.size(), size_t(80)); i++) {
            char c = _outputBuffer[i];
            if (c >= 32 && c < 127) preview += c;
            else preview += '.';
        }
        ydebug("TelnetClient: delivering {} bytes: [{}]", _outputBuffer.size(), preview);
        _dataCallback(_outputBuffer.data(), _outputBuffer.size());
    }
}
#else
// Emscripten stubs - socket-based methods not used
void TelnetClient::updatePollEvents() {}
Result<bool> TelnetClient::onEvent(const base::Event&) { return Ok(false); }
void TelnetClient::processReceived() {}
#endif

void TelnetClient::processByte(uint8_t byte) {
    switch (_state) {
        case State::DATA:
            if (byte == IAC) {
                _state = State::IAC;
            } else {
                _outputBuffer.push_back(static_cast<char>(byte));
            }
            break;

        case State::IAC:
            switch (byte) {
                case IAC:
                    // Escaped IAC -> literal 0xFF
                    _outputBuffer.push_back(static_cast<char>(0xFF));
                    _state = State::DATA;
                    break;
                case WILL:
                    _state = State::WILL;
                    break;
                case WONT:
                    _state = State::WONT;
                    break;
                case DO:
                    _state = State::DO;
                    break;
                case DONT:
                    _state = State::DONT;
                    break;
                case SB:
                    _subnegBuffer.clear();
                    _state = State::SB;
                    break;
                case SE:
                case NOP:
                case GA:
                    _state = State::DATA;
                    break;
                default:
                    // Unknown command, ignore
                    _state = State::DATA;
                    break;
            }
            break;

        case State::WILL:
            handleWill(byte);
            _state = State::DATA;
            break;

        case State::WONT:
            handleWont(byte);
            _state = State::DATA;
            break;

        case State::DO:
            handleDo(byte);
            _state = State::DATA;
            break;

        case State::DONT:
            handleDont(byte);
            _state = State::DATA;
            break;

        case State::SB:
            if (byte == IAC) {
                _state = State::SB_IAC;
            } else {
                _subnegBuffer.push_back(byte);
            }
            break;

        case State::SB_IAC:
            if (byte == SE) {
                handleSubnegotiation();
                _state = State::DATA;
            } else if (byte == IAC) {
                // Escaped IAC in subnegotiation
                _subnegBuffer.push_back(IAC);
                _state = State::SB;
            } else {
                // Protocol error, recover
                _state = State::DATA;
            }
            break;
    }
}

void TelnetClient::handleWill(uint8_t option) {
    switch (option) {
        case OPT_ECHO:
            // Server will echo - we accept
            sendCommand(DO, OPT_ECHO);
            _echoEnabled = true;
            ydebug("Telnet: server WILL ECHO, sent DO ECHO");
            break;

        case OPT_SGA:
            // Server will suppress go-ahead - we accept
            sendCommand(DO, OPT_SGA);
            _sgaEnabled = true;
            ydebug("Telnet: server WILL SGA, sent DO SGA");
            break;

        default:
            // Refuse unknown options
            sendCommand(DONT, option);
            break;
    }
}

void TelnetClient::handleWont(uint8_t option) {
    // Server refuses option - acknowledge
    switch (option) {
        case OPT_ECHO:
            _echoEnabled = false;
            break;
        case OPT_SGA:
            _sgaEnabled = false;
            break;
    }
}

void TelnetClient::handleDo(uint8_t option) {
    switch (option) {
        case OPT_NAWS:
            // Server requests window size
            sendCommand(WILL, OPT_NAWS);
            _nawsEnabled = true;
            ydebug("Telnet: server DO NAWS, sent WILL NAWS");
            // Send current window size
            sendWindowSize(_cols, _rows);
            break;

        case OPT_TTYPE:
            // Server requests terminal type
            sendCommand(WILL, OPT_TTYPE);
            _ttypeEnabled = true;
            ydebug("Telnet: server DO TTYPE, sent WILL TTYPE");
            break;

        case OPT_BINARY:
            // Binary transmission - accept for UTF-8
            sendCommand(WILL, OPT_BINARY);
            _binaryEnabled = true;
            ydebug("Telnet: server DO BINARY, sent WILL BINARY");
            break;

        case OPT_SGA:
            // Suppress go-ahead - accept
            sendCommand(WILL, OPT_SGA);
            _sgaEnabled = true;
            ydebug("Telnet: server DO SGA, sent WILL SGA");
            break;

        case OPT_NEW_ENVIRON:
        case OPT_OLD_ENVIRON:
            // We could send environment variables, but decline for now
            sendCommand(WONT, option);
            break;

        default:
            // Refuse unknown options
            sendCommand(WONT, option);
            break;
    }
}

void TelnetClient::handleDont(uint8_t option) {
    // Server refuses option - acknowledge
    sendCommand(WONT, option);

    switch (option) {
        case OPT_NAWS:
            _nawsEnabled = false;
            break;
        case OPT_TTYPE:
            _ttypeEnabled = false;
            break;
        case OPT_BINARY:
            _binaryEnabled = false;
            break;
    }
}

void TelnetClient::handleSubnegotiation() {
    if (_subnegBuffer.empty()) return;

    uint8_t option = _subnegBuffer[0];

    switch (option) {
        case OPT_TTYPE:
            // Server requests terminal type: SB TTYPE SEND SE
            if (_subnegBuffer.size() >= 2 && _subnegBuffer[1] == TTYPE_SEND) {
                sendTerminalType();
            }
            break;

        default:
            // Unknown subnegotiation, ignore
            break;
    }
}

#ifdef __EMSCRIPTEN__
void TelnetClient::onWebSocketData(const uint8_t* data, size_t len) {
    ytrace("Telnet: received {} bytes via WebSocket", len);

    // If we're receiving data, we're connected (handles race where message arrives before onopen)
    if (!_wsConnected) {
        ydebug("Telnet: received data before onopen, setting connected");
        _wsConnected = true;
        _connected = true;
        _connecting = false;
    }

    _outputBuffer.clear();

    // Process through telnet state machine
    for (size_t i = 0; i < len; i++) {
        processByte(data[i]);
    }

    // After processing commands, drain any queued responses
    drainSendQueue();

    // Deliver decoded data to terminal
    if (!_outputBuffer.empty() && _dataCallback) {
        ytrace("Telnet: delivering {} bytes", _outputBuffer.size());
        _dataCallback(_outputBuffer.data(), _outputBuffer.size());
    }
}

void TelnetClient::onWebSocketConnected() {
    ydebug("Telnet WebSocket connected");
    _wsConnected = true;
    _connected = true;
    _connecting = false;
    _wantsReconnect = false;
    // Drain any queued data (e.g., NAWS, responses)
    drainSendQueue();
    if (_connectCallback) {
        _connectCallback();
    }
}

void TelnetClient::onWebSocketDisconnected() {
    _wsConnected = false;
    _connected = false;
    if (_disconnectCallback) {
        _disconnectCallback();
    }
}

Result<void> TelnetClient::reconnect() {
    if (_wsUrl.empty()) {
        return Err<void>("No WebSocket URL set for reconnection");
    }

    // Close existing socket if any
    if (_wsSocket > 0) {
        emscripten_websocket_delete(_wsSocket);
        _wsSocket = 0;
    }
    _wsConnected = false;
    _wantsReconnect = false;

    ydebug("Telnet: reconnecting to {}", _wsUrl);

    EmscriptenWebSocketCreateAttributes attrs;
    emscripten_websocket_init_create_attributes(&attrs);
    attrs.url = _wsUrl.c_str();
    attrs.protocols = nullptr;
    attrs.createOnMainThread = EM_TRUE;

    _wsSocket = emscripten_websocket_new(&attrs);
    if (_wsSocket <= 0) {
        _wantsReconnect = true;
        return Err<void>("Failed to create WebSocket for reconnection");
    }

    emscripten_websocket_set_onopen_callback(_wsSocket, this, onWsOpen);
    emscripten_websocket_set_onerror_callback(_wsSocket, this, onWsError);
    emscripten_websocket_set_onclose_callback(_wsSocket, this, onWsClose);
    emscripten_websocket_set_onmessage_callback(_wsSocket, this, onWsMessage);

    _connecting = true;
    _connected = true;
    return Ok();
}
#endif

} // namespace yetty::telnet
