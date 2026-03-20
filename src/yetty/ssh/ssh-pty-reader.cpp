#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "ssh-pty-reader.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>

namespace yetty::ssh {

SshPtyReader::~SshPtyReader() {
    stop();
}

Result<void> SshPtyReader::init(const PtyConfig& config, const SshConfig& sshConfig) {
    _sshConfig = sshConfig;
    _sshConfig.cols = static_cast<uint16_t>(config.cols);
    _sshConfig.rows = static_cast<uint16_t>(config.rows);

    _client = std::make_shared<SshClient>();

    // Set up callbacks
    _client->setDataCallback([this](const char* data, size_t len) {
        onSshData(data, len);
    });

    _client->setDisconnectCallback([this]() {
        onSshDisconnect();
    });

    // Connect and handshake (no auth yet)
    auto res = _client->connectNoAuth(_sshConfig);
    if (!res) {
        return Err<void>("Failed to connect via SSH", res);
    }

    _running = true;

    // Check if password is needed
    if (_client->needsPassword()) {
        // If password is already in config, try it first
        if (!_sshConfig.password.empty()) {
            ydebug("SshPtyReader: trying password from config");
            auto authRes = _client->authenticatePassword(_sshConfig.password);
            if (authRes) {
                // Password worked - open shell
                auto shellRes = _client->openShell();
                if (!shellRes) {
                    return Err<void>("Failed to open shell", shellRes);
                }
                _state = SshState::Connected;
                ydebug("SshPtyReader: authenticated with config password");
            } else {
                // Password failed - show error and prompt for password
                sendToTerminal("\033[1;31mAuthentication failed\033[0m\r\n");
                sendToTerminal("Password: ");
                _state = SshState::WaitingForPassword;
                ydebug("SshPtyReader: config password failed, waiting for user input");
            }
        } else {
            // Show password prompt in terminal
            _state = SshState::WaitingForPassword;
            sendToTerminal("Password: ");
            ydebug("SshPtyReader: waiting for password input");
        }
    } else {
        // Already authenticated (e.g., public key worked)
        auto shellRes = _client->openShell();
        if (!shellRes) {
            return Err<void>("Failed to open shell", shellRes);
        }
        _state = SshState::Connected;
        ydebug("SshPtyReader: connected (no password needed)");
    }

    ydebug("SshPtyReader: init complete to {}@{}:{}", _sshConfig.username, _sshConfig.host, _sshConfig.port);
    return Ok();
}

size_t SshPtyReader::read(char* buf, size_t maxLen) {
    // Poll for new data first
    if (_client) {
        _client->poll();
    }

    std::lock_guard<std::mutex> lock(_mutex);
    size_t toRead = std::min(maxLen, _recvBuffer.size());
    for (size_t i = 0; i < toRead; i++) {
        buf[i] = _recvBuffer.front();
        _recvBuffer.pop_front();
    }
    return toRead;
}

void SshPtyReader::write(const char* data, size_t len) {
    if (_state == SshState::WaitingForPassword) {
        // Handle password input
        for (size_t i = 0; i < len; i++) {
            handlePasswordInput(data[i]);
        }
        return;
    }

    if (_client && _client->isConnected()) {
        _client->send(data, len);
    }
}

void SshPtyReader::sendToTerminal(const std::string& text) {
    std::lock_guard<std::mutex> lock(_mutex);
    for (char c : text) {
        _recvBuffer.push_back(c);
    }
    if (_dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

void SshPtyReader::handlePasswordInput(char c) {
    if (c == '\r' || c == '\n') {
        // User pressed Enter - attempt authentication
        sendToTerminal("\r\n");
        attemptAuthentication();
    } else if (c == 127 || c == 8) {
        // Backspace
        if (!_passwordBuffer.empty()) {
            _passwordBuffer.pop_back();
            // Don't echo anything for password
        }
    } else if (c >= 32) {
        // Regular character - add to password buffer, don't echo
        _passwordBuffer += c;
    }
}

void SshPtyReader::attemptAuthentication() {
    _state = SshState::Authenticating;

    auto res = _client->authenticatePassword(_passwordBuffer);
    _passwordBuffer.clear();  // Clear password from memory

    if (!res) {
        // Auth failed
        sendToTerminal("\033[1;31mAuthentication failed\033[0m\r\n");
        sendToTerminal("Password: ");
        _state = SshState::WaitingForPassword;
        return;
    }

    // Auth succeeded - open shell
    auto shellRes = _client->openShell();
    if (!shellRes) {
        sendToTerminal("\033[1;31mFailed to open shell\033[0m\r\n");
        _running = false;
        return;
    }

    _state = SshState::Connected;
    ydebug("SshPtyReader: authenticated and shell opened");
}

void SshPtyReader::resize(uint32_t cols, uint32_t rows) {
    if (_client && _client->isConnected()) {
        _client->sendWindowSize(static_cast<uint16_t>(cols),
                                 static_cast<uint16_t>(rows));
    }
}

bool SshPtyReader::isRunning() const {
    if (!_running) return false;
    if (_state == SshState::WaitingForPassword || _state == SshState::Authenticating) {
        return true;  // Still running, waiting for password
    }
    return _client && _client->isConnected();
}

void SshPtyReader::stop() {
    if (!_running) return;
    _running = false;

    if (_client) {
        _client->disconnect();
        _client.reset();
    }

    ydebug("SshPtyReader: stopped");
}

void SshPtyReader::setDataAvailableCallback(DataAvailableCallback cb) {
    _dataAvailableCallback = std::move(cb);

    // If there's already data in the buffer (e.g., "Password: " prompt from init),
    // notify immediately so the terminal reads it
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_recvBuffer.empty() && _dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

void SshPtyReader::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

void SshPtyReader::onSshData(const char* data, size_t len) {
    ytrace("SshPtyReader::onSshData received {} bytes", len);
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (size_t i = 0; i < len; i++) {
            _recvBuffer.push_back(data[i]);
        }
    }

    if (_dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

void SshPtyReader::onSshDisconnect() {
    _running = false;

    if (_exitCallback) {
        _exitCallback(0);
    }
}

Result<PtyReader::Ptr> createSshPtyReader(const PtyConfig& config, const SshConfig& sshConfig) {
    auto reader = std::make_shared<SshPtyReader>();
    if (auto res = reader->init(config, sshConfig); !res) {
        return Err<PtyReader::Ptr>("Failed to initialize SshPtyReader", res);
    }
    return Ok<PtyReader::Ptr>(reader);
}

} // namespace yetty::ssh
