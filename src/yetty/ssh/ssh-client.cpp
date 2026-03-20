#include "ssh-client.h"
#include <ytrace/ytrace.hpp>
#include <libssh2.h>
#include <uv.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <cstring>

namespace yetty::ssh {

class SshClient::Impl {
public:
    Impl() = default;
    ~Impl() { disconnect(); }

    Result<void> connect(const SshConfig& config) {
        _config = config;

        // Initialize libssh2
        if (libssh2_init(0) != 0) {
            return Err<void>("Failed to initialize libssh2");
        }
        _libssh2Initialized = true;

        // Resolve hostname
        struct addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo* result = nullptr;
        std::string portStr = std::to_string(config.port);
        int rc = getaddrinfo(config.host.c_str(), portStr.c_str(), &hints, &result);
        if (rc != 0 || !result) {
            return Err<void>("Failed to resolve hostname: " + config.host);
        }

        // Create socket
        _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (_socket < 0) {
            freeaddrinfo(result);
            return Err<void>("Failed to create socket");
        }

        // Connect
        if (::connect(_socket, result->ai_addr, result->ai_addrlen) != 0) {
            freeaddrinfo(result);
            closeSocket();
            return Err<void>("Failed to connect to " + config.host + ":" + portStr);
        }
        freeaddrinfo(result);

        // Create libssh2 session
        _session = libssh2_session_init();
        if (!_session) {
            closeSocket();
            return Err<void>("Failed to create SSH session");
        }

        // Set non-blocking mode
        libssh2_session_set_blocking(_session, 0);

        // Handshake
        while ((rc = libssh2_session_handshake(_session, _socket)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            char* errmsg = nullptr;
            libssh2_session_last_error(_session, &errmsg, nullptr, 0);
            std::string err = errmsg ? errmsg : "SSH handshake failed";
            disconnect();
            return Err<void>(err);
        }

        // Authenticate
        if (auto res = authenticate(); !res) {
            disconnect();
            return res;
        }

        // Open channel
        while ((_channel = libssh2_channel_open_session(_session)) == nullptr &&
               libssh2_session_last_errno(_session) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (!_channel) {
            disconnect();
            return Err<void>("Failed to open SSH channel");
        }

        // Request PTY
        while ((rc = libssh2_channel_request_pty_ex(_channel,
                                                     config.termType.c_str(), config.termType.length(),
                                                     nullptr, 0,
                                                     config.cols, config.rows, 0, 0)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            disconnect();
            return Err<void>("Failed to request PTY");
        }

        // Start shell
        while ((rc = libssh2_channel_shell(_channel)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            disconnect();
            return Err<void>("Failed to start shell");
        }

        // Set non-blocking for channel
        libssh2_channel_set_blocking(_channel, 0);

        _connected = true;
        yinfo("SSH connected to {}@{}:{}", config.username, config.host, config.port);
        return Ok();
    }

    void disconnect() {
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
    }

    bool isConnected() const { return _connected && _channel && !libssh2_channel_eof(_channel); }

    // Step 1: Connect and handshake only, no auth
    Result<void> connectNoAuth(const SshConfig& config) {
        _config = config;

        if (libssh2_init(0) != 0) {
            return Err<void>("Failed to initialize libssh2");
        }
        _libssh2Initialized = true;

        struct addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        struct addrinfo* result = nullptr;
        std::string portStr = std::to_string(config.port);
        int rc = getaddrinfo(config.host.c_str(), portStr.c_str(), &hints, &result);
        if (rc != 0 || !result) {
            return Err<void>("Failed to resolve hostname: " + config.host);
        }

        _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (_socket < 0) {
            freeaddrinfo(result);
            return Err<void>("Failed to create socket");
        }

        if (::connect(_socket, result->ai_addr, result->ai_addrlen) != 0) {
            freeaddrinfo(result);
            closeSocket();
            return Err<void>("Failed to connect to " + config.host + ":" + portStr);
        }
        freeaddrinfo(result);

        _session = libssh2_session_init();
        if (!_session) {
            closeSocket();
            return Err<void>("Failed to create SSH session");
        }

        libssh2_session_set_blocking(_session, 0);

        while ((rc = libssh2_session_handshake(_session, _socket)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            char* errmsg = nullptr;
            libssh2_session_last_error(_session, &errmsg, nullptr, 0);
            std::string err = errmsg ? errmsg : "SSH handshake failed";
            disconnect();
            return Err<void>(err);
        }

        // Check what auth methods are available
        char* authlist = libssh2_userauth_list(_session, config.username.c_str(), config.username.size());
        if (authlist) {
            _authMethods = authlist;
            ydebug("SSH auth methods: {}", _authMethods);
        }

        // Try public key auth if available
        if (!config.privateKeyPath.empty()) {
            const char* passphrase = config.privateKeyPassphrase.empty() ?
                nullptr : config.privateKeyPassphrase.c_str();

            while ((rc = libssh2_userauth_publickey_fromfile(_session,
                config.username.c_str(), nullptr,
                config.privateKeyPath.c_str(), passphrase)) == LIBSSH2_ERROR_EAGAIN) {
                waitSocket();
            }
            if (rc == 0) {
                ydebug("SSH: authenticated with public key");
                _authenticated = true;
            }
        }

        yinfo("SSH handshake complete to {}@{}:{}", config.username, config.host, config.port);
        return Ok();
    }

    // Step 2: Check if password needed
    bool needsPassword() const {
        return !_authenticated;
    }

    // Step 3: Authenticate with password
    Result<void> authenticatePassword(const std::string& password) {
        if (!_session) {
            return Err<void>("Not connected");
        }

        int rc;
        ydebug("SSH: authenticating with password");
        while ((rc = libssh2_userauth_password(_session,
                                                _config.username.c_str(),
                                                password.c_str())) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc == 0) {
            ydebug("SSH: password auth successful");
            _authenticated = true;
            return Ok();
        }

        ywarn("SSH: password auth failed with rc={}", rc);
        return Err<void>("Authentication failed - wrong password");
    }

    // Step 4: Open shell
    Result<void> openShell() {
        if (!_session || !_authenticated) {
            return Err<void>("Not authenticated");
        }

        int rc;

        while ((_channel = libssh2_channel_open_session(_session)) == nullptr &&
               libssh2_session_last_errno(_session) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (!_channel) {
            return Err<void>("Failed to open SSH channel");
        }

        while ((rc = libssh2_channel_request_pty_ex(_channel,
            _config.termType.c_str(), _config.termType.length(),
            nullptr, 0, _config.cols, _config.rows, 0, 0)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            return Err<void>("Failed to request PTY");
        }

        while ((rc = libssh2_channel_shell(_channel)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            return Err<void>("Failed to start shell");
        }

        libssh2_channel_set_blocking(_channel, 0);
        _connected = true;
        yinfo("SSH shell opened to {}@{}:{}", _config.username, _config.host, _config.port);
        return Ok();
    }

    void send(const char* data, size_t len) {
        if (!_channel) return;

        size_t written = 0;
        while (written < len) {
            ssize_t rc = libssh2_channel_write(_channel, data + written, len - written);
            if (rc == LIBSSH2_ERROR_EAGAIN) {
                waitSocket();
                continue;
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

        int rc;
        while ((rc = libssh2_channel_request_pty_size(_channel, cols, rows)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc) {
            ywarn("SSH: failed to send window size: {}", rc);
        }
    }

    void setDataCallback(DataCallback cb) { _dataCallback = std::move(cb); }
    void setDisconnectCallback(DisconnectCallback cb) { _disconnectCallback = std::move(cb); }

    void poll() {
        if (!_channel) return;

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

private:
    Result<void> authenticate() {
        int rc;

        ydebug("SSH auth: user={}, hasPassword={}, hasKey={}",
               _config.username,
               !_config.password.empty(),
               !_config.privateKeyPath.empty());

        // Try public key auth first if key path provided
        if (!_config.privateKeyPath.empty()) {
            const char* passphrase = _config.privateKeyPassphrase.empty() ?
                nullptr : _config.privateKeyPassphrase.c_str();

            while ((rc = libssh2_userauth_publickey_fromfile(_session,
                                                              _config.username.c_str(),
                                                              nullptr,  // pubkey path (auto-detect)
                                                              _config.privateKeyPath.c_str(),
                                                              passphrase)) == LIBSSH2_ERROR_EAGAIN) {
                waitSocket();
            }
            if (rc == 0) {
                ydebug("SSH: authenticated with public key");
                return Ok();
            }
            ywarn("SSH: public key auth failed, trying password");
        }

        // Try password auth
        if (!_config.password.empty()) {
            ydebug("SSH: trying password auth for user {}", _config.username);
            while ((rc = libssh2_userauth_password(_session,
                                                    _config.username.c_str(),
                                                    _config.password.c_str())) == LIBSSH2_ERROR_EAGAIN) {
                waitSocket();
            }
            if (rc == 0) {
                ydebug("SSH: authenticated with password");
                return Ok();
            }
            ywarn("SSH: password auth failed with rc={}", rc);
        }

        // Try keyboard-interactive auth
        ydebug("SSH: trying keyboard-interactive auth");
        while ((rc = libssh2_userauth_keyboard_interactive(_session,
                                                            _config.username.c_str(),
                                                            &Impl::kbdintCallback)) == LIBSSH2_ERROR_EAGAIN) {
            waitSocket();
        }
        if (rc == 0) {
            ydebug("SSH: authenticated with keyboard-interactive");
            return Ok();
        }
        ywarn("SSH: keyboard-interactive auth failed with rc={}", rc);

        return Err<void>("SSH authentication failed");
    }

    // Keyboard-interactive callback - prompts user for password via iOS alert
    static void kbdintCallback(const char* name, int name_len,
                                const char* instruction, int instruction_len,
                                int num_prompts,
                                const LIBSSH2_USERAUTH_KBDINT_PROMPT* prompts,
                                LIBSSH2_USERAUTH_KBDINT_RESPONSE* responses,
                                void** abstract) {
        (void)name; (void)name_len;
        (void)instruction; (void)instruction_len;
        (void)abstract;

        for (int i = 0; i < num_prompts; i++) {
            ydebug("SSH kbd-int prompt {}: len={}", i, prompts[i].length);

            // For now, send empty response - server will reject
            // TODO: implement iOS alert to prompt for password
            responses[i].text = strdup("");
            responses[i].length = 0;
        }
    }

    void waitSocket() {
        int dir = libssh2_session_block_directions(_session);
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(_socket, &fds);

        struct timeval tv = { 0, 100000 };  // 100ms timeout

        if (dir & LIBSSH2_SESSION_BLOCK_INBOUND) {
            select(_socket + 1, &fds, nullptr, nullptr, &tv);
        }
        if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND) {
            select(_socket + 1, nullptr, &fds, nullptr, &tv);
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
    std::string _authMethods;
    int _socket = -1;
    LIBSSH2_SESSION* _session = nullptr;
    LIBSSH2_CHANNEL* _channel = nullptr;
    bool _connected = false;
    bool _authenticated = false;
    bool _libssh2Initialized = false;

    DataCallback _dataCallback;
    DisconnectCallback _disconnectCallback;
};

// SshClient wrapper implementation
SshClient::SshClient() : _impl(std::make_unique<Impl>()) {}
SshClient::~SshClient() = default;

Result<void> SshClient::connect(const SshConfig& config) { return _impl->connect(config); }
Result<void> SshClient::connectNoAuth(const SshConfig& config) { return _impl->connectNoAuth(config); }
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

} // namespace yetty::ssh
