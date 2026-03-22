#include "ssh-client.h"
#include <yetty/platform/event-loop.h>
#include <yetty/base/event-listener.h>
#include <ytrace/ytrace.hpp>
#include <libssh2.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#endif

#include <cstring>

namespace yetty::ssh {

// Connection state machine
enum class ConnectState {
    Idle,
    Connecting,      // TCP connect in progress
    Handshaking,     // SSH handshake
    Authenticating,  // Auth with key or password
    OpeningChannel,  // Opening SSH channel
    RequestingPty,   // Requesting PTY
    StartingShell,   // Starting shell
    Connected,       // Fully connected
    Failed,          // Connection failed
    WaitingPassword  // Waiting for user to input password
};

class SshClient::Impl : public base::EventListener {
public:
    Impl() = default;
    ~Impl() override { disconnect(); }

    // Start async connection - returns immediately
    Result<void> startConnect(const SshConfig& config) {
        ydebug("SSH startConnect: BEGIN {}@{}:{}", config.username, config.host, config.port);
        _config = config;

        // Initialize libssh2
        ydebug("SSH startConnect: libssh2_init");
        if (libssh2_init(0) != 0) {
            return Err<void>("Failed to initialize libssh2");
        }
        _libssh2Initialized = true;

        // Resolve hostname (synchronous but fast)
        ydebug("SSH startConnect: getaddrinfo for {}", config.host);
        struct addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo* result = nullptr;
        std::string portStr = std::to_string(config.port);
        int rc = getaddrinfo(config.host.c_str(), portStr.c_str(), &hints, &result);
        ydebug("SSH startConnect: getaddrinfo returned {}", rc);
        if (rc != 0 || !result) {
            return Err<void>("Failed to resolve hostname: " + config.host);
        }

        // Create non-blocking socket
        ydebug("SSH startConnect: creating socket");
        _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (_socket < 0) {
            freeaddrinfo(result);
            return Err<void>("Failed to create socket");
        }

        // Set non-blocking
        ydebug("SSH startConnect: set non-blocking");
        int flags = fcntl(_socket, F_GETFL, 0);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

        // Disable Nagle
        int flag = 1;
        setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

        // Start async connect
        ydebug("SSH startConnect: calling connect()");
        int connectResult = ::connect(_socket, result->ai_addr, result->ai_addrlen);
        ydebug("SSH startConnect: connect() returned {}, errno={}", connectResult, errno);
        freeaddrinfo(result);

        if (connectResult < 0 && errno != EINPROGRESS) {
            closeSocket();
            return Err<void>("Failed to connect: " + std::string(strerror(errno)));
        }

        _state = ConnectState::Connecting;
        ydebug("SSH startConnect: state=Connecting, getting EventLoop");

        // Register with event loop
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            closeSocket();
            ydebug("SSH startConnect: NO EVENT LOOP!");
            return Err<void>("No event loop available");
        }
        _loop = *loopResult;
        ydebug("SSH startConnect: got EventLoop");

        auto pollRes = _loop->createPoll();
        if (!pollRes) {
            closeSocket();
            return Err<void>("Failed to create poll", pollRes);
        }
        _pollId = *pollRes;

        if (auto res = _loop->configPoll(_pollId, _socket); !res) {
            closeSocket();
            return Err<void>("Failed to configure poll", res);
        }

        // Register self as listener using sharedAs
        auto listener = sharedAs<base::EventListener>();
        if (!listener) {
            closeSocket();
            return Err<void>("Failed to get shared_ptr<EventListener>");
        }

        if (auto res = _loop->registerPollListener(_pollId, listener); !res) {
            closeSocket();
            return Err<void>("Failed to register poll listener", res);
        }

        // Start polling for writable (connect completion)
        if (auto res = _loop->startPoll(_pollId, base::EventLoop::POLL_WRITABLE); !res) {
            closeSocket();
            return Err<void>("Failed to start poll", res);
        }

        ydebug("SSH startConnect: DONE - async connect started to {}:{}", config.host, config.port);
        return Ok();
    }

    // EventListener interface - handle poll events
    Result<bool> onEvent(const base::Event& event) override {
        // Handle both readable and writable events for our socket
        if ((event.type == base::Event::Type::PollReadable ||
             event.type == base::Event::Type::PollWritable) &&
            event.poll.fd == _socket) {

            bool readable = (event.type == base::Event::Type::PollReadable);
            bool writable = (event.type == base::Event::Type::PollWritable);

            ytrace("SSH: poll event state={} readable={} writable={}",
                   static_cast<int>(_state), readable, writable);

            // Advance state machine based on current state
            switch (_state) {
                case ConnectState::Connecting:
                    handleConnecting();
                    break;
                case ConnectState::Handshaking:
                    handleHandshaking();
                    break;
                case ConnectState::Authenticating:
                    handleAuthenticating();
                    break;
                case ConnectState::OpeningChannel:
                    handleOpeningChannel();
                    break;
                case ConnectState::RequestingPty:
                    handleRequestingPty();
                    break;
                case ConnectState::StartingShell:
                    handleStartingShell();
                    break;
                case ConnectState::Connected:
                    handleConnected();
                    break;
                case ConnectState::WaitingPassword:
                    // Do nothing - waiting for user input
                    break;
                default:
                    break;
            }

            return Ok(true);
        }

        return Ok(false);
    }

    void handleConnecting() {
        // Check if connect completed
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
            reportStatus("\033[1;31mConnection failed: " + std::string(strerror(error ? error : errno)) + "\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        reportStatus("\033[1;32mConnected, starting SSH handshake...\033[0m\r\n");

        // Create SSH session
        _session = libssh2_session_init();
        if (!_session) {
            reportStatus("\033[1;31mFailed to create SSH session\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        libssh2_session_set_blocking(_session, 0);
        transitionTo(ConnectState::Handshaking);
        handleHandshaking();  // Try immediately
    }

    void handleHandshaking() {
        int rc = libssh2_session_handshake(_session, _socket);
        if (rc == LIBSSH2_ERROR_EAGAIN) {
            updatePollEvents();
            return;  // Wait for next poll
        }
        if (rc != 0) {
            char* errmsg = nullptr;
            libssh2_session_last_error(_session, &errmsg, nullptr, 0);
            reportStatus("\033[1;31mSSH handshake failed: " + std::string(errmsg ? errmsg : "unknown error") + "\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        reportStatus("\033[1;32mHandshake complete, authenticating...\033[0m\r\n");

        // Check auth methods
        char* authlist = libssh2_userauth_list(_session, _config.username.c_str(), _config.username.size());
        if (authlist) {
            _authMethods = authlist;
            ydebug("SSH auth methods: {}", _authMethods);
        }

        transitionTo(ConnectState::Authenticating);
        handleAuthenticating();  // Try immediately
    }

    void handleAuthenticating() {
        int rc;

        // Try public key auth first if we have a key
        if (!_config.privateKeyPath.empty() && !_triedKeyAuth) {
            const char* passphrase = _config.privateKeyPassphrase.empty() ?
                nullptr : _config.privateKeyPassphrase.c_str();

            rc = libssh2_userauth_publickey_fromfile(_session,
                _config.username.c_str(), nullptr,
                _config.privateKeyPath.c_str(), passphrase);

            if (rc == LIBSSH2_ERROR_EAGAIN) {
                updatePollEvents();
                return;
            }
            _triedKeyAuth = true;
            if (rc == 0) {
                reportStatus("\033[1;32mAuthenticated with public key\033[0m\r\n");
                _authenticated = true;
                transitionTo(ConnectState::OpeningChannel);
                handleOpeningChannel();
                return;
            }
            ydebug("SSH: public key auth failed with rc={}", rc);
        }

        // Try password if provided
        if (!_config.password.empty() && !_triedPasswordAuth) {
            rc = libssh2_userauth_password(_session,
                _config.username.c_str(), _config.password.c_str());

            if (rc == LIBSSH2_ERROR_EAGAIN) {
                updatePollEvents();
                return;
            }
            _triedPasswordAuth = true;
            if (rc == 0) {
                reportStatus("\033[1;32mAuthenticated with password\033[0m\r\n");
                _authenticated = true;
                transitionTo(ConnectState::OpeningChannel);
                handleOpeningChannel();
                return;
            }
            ydebug("SSH: password auth failed with rc={}", rc);
        }

        // Need password from user
        if (!_authenticated) {
            reportStatus("Password: ");
            transitionTo(ConnectState::WaitingPassword);
            _needsPassword = true;
        }
    }

    void handleOpeningChannel() {
        _channel = libssh2_channel_open_session(_session);
        if (_channel == nullptr) {
            if (libssh2_session_last_errno(_session) == LIBSSH2_ERROR_EAGAIN) {
                updatePollEvents();
                return;
            }
            reportStatus("\033[1;31mFailed to open SSH channel\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        transitionTo(ConnectState::RequestingPty);
        handleRequestingPty();
    }

    void handleRequestingPty() {
        int rc = libssh2_channel_request_pty_ex(_channel,
            _config.termType.c_str(), _config.termType.length(),
            nullptr, 0, _config.cols, _config.rows, 0, 0);

        if (rc == LIBSSH2_ERROR_EAGAIN) {
            updatePollEvents();
            return;
        }
        if (rc != 0) {
            reportStatus("\033[1;31mFailed to request PTY\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        transitionTo(ConnectState::StartingShell);
        handleStartingShell();
    }

    void handleStartingShell() {
        int rc = libssh2_channel_shell(_channel);
        if (rc == LIBSSH2_ERROR_EAGAIN) {
            updatePollEvents();
            return;
        }
        if (rc != 0) {
            reportStatus("\033[1;31mFailed to start shell\033[0m\r\n");
            transitionTo(ConnectState::Failed);
            return;
        }

        libssh2_channel_set_blocking(_channel, 0);
        _connected = true;
        reportStatus("\033[1;32mShell ready!\033[0m\r\n");
        transitionTo(ConnectState::Connected);

        // Switch to read mode
        if (_loop && _pollId >= 0) {
            _loop->setPollEvents(_pollId, base::EventLoop::POLL_READABLE);
        }
    }

    void handleConnected() {
        // Read data from channel
        char buf[4096];
        ssize_t rc;

        while ((rc = libssh2_channel_read(_channel, buf, sizeof(buf))) > 0) {
            if (_dataCallback) {
                _dataCallback(buf, rc);
            }
        }

        // Check for EOF
        if (libssh2_channel_eof(_channel)) {
            _connected = false;
            if (_disconnectCallback) {
                _disconnectCallback();
            }
        }
    }

    void transitionTo(ConnectState newState) {
        ydebug("SSH: state {} -> {}", static_cast<int>(_state), static_cast<int>(newState));
        _state = newState;

        if (newState == ConnectState::Failed) {
            cleanup();
        }
    }

    void updatePollEvents() {
        if (!_loop || _pollId < 0 || !_session) return;

        int dir = libssh2_session_block_directions(_session);
        int events = 0;
        if (dir & LIBSSH2_SESSION_BLOCK_INBOUND) {
            events |= base::EventLoop::POLL_READABLE;
        }
        if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND) {
            events |= base::EventLoop::POLL_WRITABLE;
        }
        if (events == 0) {
            events = base::EventLoop::POLL_READABLE;
        }
        _loop->setPollEvents(_pollId, events);
    }

    void reportStatus(const std::string& msg) {
        if (_statusCallback) {
            _statusCallback(msg);
        }
    }

    void cleanup() {
        if (_pollId >= 0 && _loop) {
            _loop->stopPoll(_pollId);
            _loop->destroyPoll(_pollId);
            _pollId = -1;
        }
    }

    void disconnect() {
        cleanup();

        if (_channel) {
            libssh2_channel_close(_channel);
            libssh2_channel_free(_channel);
            _channel = nullptr;
        }
        if (_session) {
            libssh2_session_disconnect(_session, "Normal shutdown");
            libssh2_session_free(_session);
            _session = nullptr;
        }
        closeSocket();
        if (_libssh2Initialized) {
            libssh2_exit();
            _libssh2Initialized = false;
        }
        _connected = false;
        _state = ConnectState::Idle;
    }

    bool isConnected() const {
        return _connected && _channel && !libssh2_channel_eof(_channel);
    }

    bool needsPassword() const { return _needsPassword; }

    Result<void> authenticatePassword(const std::string& password) {
        if (!_session || _state != ConnectState::WaitingPassword) {
            return Err<void>("Not waiting for password");
        }

        _config.password = password;
        _needsPassword = false;

        int rc = libssh2_userauth_password(_session,
            _config.username.c_str(), password.c_str());

        if (rc == LIBSSH2_ERROR_EAGAIN) {
            transitionTo(ConnectState::Authenticating);
            updatePollEvents();
            return Ok();
        }
        if (rc == 0) {
            reportStatus("\033[1;32mAuthenticated!\033[0m\r\n");
            _authenticated = true;
            transitionTo(ConnectState::OpeningChannel);
            handleOpeningChannel();
            return Ok();
        }

        reportStatus("\033[1;31mAuthentication failed\033[0m\r\n");
        reportStatus("Password: ");
        _needsPassword = true;
        transitionTo(ConnectState::WaitingPassword);
        return Err<void>("Wrong password");
    }

    Result<void> openShell() {
        // Shell opening is part of the state machine now
        return Ok();
    }

    void send(const char* data, size_t len) {
        if (!_channel || !_connected) return;

        size_t written = 0;
        while (written < len) {
            ssize_t rc = libssh2_channel_write(_channel, data + written, len - written);
            if (rc == LIBSSH2_ERROR_EAGAIN) {
                // Buffer is full - we could queue this but for now just return
                ydebug("SSH: write would block, {} of {} written", written, len);
                break;
            }
            if (rc < 0) {
                yerror("SSH write error: {}", rc);
                break;
            }
            written += rc;
        }
    }

    void sendWindowSize(uint16_t cols, uint16_t rows) {
        if (!_channel) return;
        _config.cols = cols;
        _config.rows = rows;

        int rc = libssh2_channel_request_pty_size(_channel, cols, rows);
        if (rc != 0 && rc != LIBSSH2_ERROR_EAGAIN) {
            ywarn("SSH: failed to send window size: {}", rc);
        }
    }

    void setDataCallback(DataCallback cb) { _dataCallback = std::move(cb); }
    void setDisconnectCallback(DisconnectCallback cb) { _disconnectCallback = std::move(cb); }
    void setStatusCallback(std::function<void(const std::string&)> cb) { _statusCallback = std::move(cb); }

    void poll() {
        // For manual polling when not using event loop
        if (_state == ConnectState::Connected) {
            handleConnected();
        }
    }

    void closeSocket() {
        if (_socket >= 0) {
#ifdef _WIN32
            closesocket(_socket);
#else
            close(_socket);
#endif
            _socket = -1;
        }
    }

    SshConfig _config;
    ConnectState _state = ConnectState::Idle;
    std::string _authMethods;
    int _socket = -1;
    LIBSSH2_SESSION* _session = nullptr;
    LIBSSH2_CHANNEL* _channel = nullptr;
    bool _connected = false;
    bool _authenticated = false;
    bool _libssh2Initialized = false;
    bool _needsPassword = false;
    bool _triedKeyAuth = false;
    bool _triedPasswordAuth = false;

    base::EventLoop::Ptr _loop;
    base::PollId _pollId = -1;

    DataCallback _dataCallback;
    DisconnectCallback _disconnectCallback;
    std::function<void(const std::string&)> _statusCallback;
};

// SshClient wrapper - owns shared_ptr to Impl
SshClient::SshClient() : _impl(std::make_shared<Impl>()) {}
SshClient::~SshClient() = default;

Result<void> SshClient::connect(const SshConfig& config) {
    return _impl->startConnect(config);
}

Result<void> SshClient::connectNoAuth(const SshConfig& config) {
    return _impl->startConnect(config);
}

bool SshClient::needsPassword() const { return _impl->needsPassword(); }
Result<void> SshClient::authenticatePassword(const std::string& password) { return _impl->authenticatePassword(password); }
Result<void> SshClient::openShell() { return _impl->openShell(); }
void SshClient::disconnect() { _impl->disconnect(); }
bool SshClient::isConnected() const { return _impl->isConnected(); }
void SshClient::send(const char* data, size_t len) { _impl->send(data, len); }
void SshClient::sendWindowSize(uint16_t cols, uint16_t rows) { _impl->sendWindowSize(cols, rows); }
void SshClient::setDataCallback(DataCallback cb) { _impl->setDataCallback(std::move(cb)); }
void SshClient::setDisconnectCallback(DisconnectCallback cb) { _impl->setDisconnectCallback(std::move(cb)); }
void SshClient::poll() { _impl->poll(); }

void SshClient::setStatusCallback(std::function<void(const std::string&)> cb) {
    _impl->setStatusCallback(std::move(cb));
}

} // namespace yetty::ssh
