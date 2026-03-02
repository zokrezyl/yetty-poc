#include "telnet-client.h"

#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)

#include <ytrace/ytrace.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

namespace yetty::telnet {

using yetty::Ok;
using yetty::Err;

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

    _connecting = (errno == EINPROGRESS);
    _connected = !_connecting;

    if (_connected) {
        yinfo("Telnet connected to {}:{} (immediate)", host, port);
    } else {
        yinfo("Telnet connecting to {}:{} (async)...", host, port);
    }

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

    yinfo("Telnet: registered poll id={} socket={}", _pollId, _socket);
    return Ok();
}

void TelnetClient::disconnect() {
    _connected = false;
    _connecting = false;

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
    if (!_connected || _socket < 0) return;

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
        return;  // Server hasn't agreed to NAWS yet
    }

    // NAWS: IAC SB NAWS <width-hi> <width-lo> <height-hi> <height-lo> IAC SE
    uint8_t naws[4] = {
        static_cast<uint8_t>((cols >> 8) & 0xFF),
        static_cast<uint8_t>(cols & 0xFF),
        static_cast<uint8_t>((rows >> 8) & 0xFF),
        static_cast<uint8_t>(rows & 0xFF),
    };
    sendSubnegotiation(OPT_NAWS, naws, 4);
    yinfo("Telnet: sent NAWS {}x{}", cols, rows);
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
    yinfo("Telnet: sent TTYPE IS {}", _terminalType);
}

void TelnetClient::drainSendQueue() {
    if (_sendQueue.empty() || _socket < 0) return;

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
}

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
    if (event.type == base::Event::Type::PollWritable && event.poll.fd == _socket) {
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
            yinfo("Telnet: async connect completed");
            updatePollEvents();
        } else {
            // Socket writable - drain send queue
            drainSendQueue();
        }
        return Ok(true);
    }

    if (event.type == base::Event::Type::PollReadable && event.poll.fd == _socket) {
        processReceived();
        return Ok(true);
    }

    return Ok(false);
}

void TelnetClient::processReceived() {
    uint8_t buf[4096];
    ssize_t n = ::recv(_socket, buf, sizeof(buf), 0);

    if (n <= 0) {
        if (n == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
            yinfo("Telnet: connection closed");
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
        _dataCallback(_outputBuffer.data(), _outputBuffer.size());
    }
}

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
            yinfo("Telnet: server WILL ECHO, sent DO ECHO");
            break;

        case OPT_SGA:
            // Server will suppress go-ahead - we accept
            sendCommand(DO, OPT_SGA);
            _sgaEnabled = true;
            yinfo("Telnet: server WILL SGA, sent DO SGA");
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
            yinfo("Telnet: server DO NAWS, sent WILL NAWS");
            // Send current window size
            sendWindowSize(_cols, _rows);
            break;

        case OPT_TTYPE:
            // Server requests terminal type
            sendCommand(WILL, OPT_TTYPE);
            _ttypeEnabled = true;
            yinfo("Telnet: server DO TTYPE, sent WILL TTYPE");
            break;

        case OPT_BINARY:
            // Binary transmission - accept for UTF-8
            sendCommand(WILL, OPT_BINARY);
            _binaryEnabled = true;
            yinfo("Telnet: server DO BINARY, sent WILL BINARY");
            break;

        case OPT_SGA:
            // Suppress go-ahead - accept
            sendCommand(WILL, OPT_SGA);
            _sgaEnabled = true;
            yinfo("Telnet: server DO SGA, sent WILL SGA");
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

} // namespace yetty::telnet

#endif // !defined(_WIN32) && !defined(__EMSCRIPTEN__)
