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
    ydebug("SshPtyReader::init BEGIN {}@{}:{}", sshConfig.username, sshConfig.host, sshConfig.port);
    _sshConfig = sshConfig;
    _sshConfig.cols = static_cast<uint16_t>(config.cols);
    _sshConfig.rows = static_cast<uint16_t>(config.rows);

    _client = std::make_shared<SshClient>();
    _running = true;
    ydebug("SshPtyReader::init: created SshClient");

    // Set up callbacks
    _client->setDataCallback([this](const char* data, size_t len) {
        onSshData(data, len);
    });

    _client->setDisconnectCallback([this]() {
        onSshDisconnect();
    });

    // Set up status callback to display connection progress in terminal
    _client->setStatusCallback([this](const std::string& msg) {
        sendToTerminal(msg);
    });

    ydebug("SshPtyReader::init DONE - connection will start in run()");
    return Ok();
}

Result<void> SshPtyReader::run() {
    ydebug("SshPtyReader::run BEGIN {}@{}:{}", _sshConfig.username, _sshConfig.host, _sshConfig.port);

    // Show connection attempt message
    std::string connectMsg = "\033[1;36mConnecting to ";
    if (!_sshConfig.username.empty()) {
        connectMsg += _sshConfig.username + "@";
    }
    connectMsg += _sshConfig.host + ":" + std::to_string(_sshConfig.port) + "...\033[0m\r\n";
    sendToTerminal(connectMsg);

    // Start async connection - returns immediately
    ydebug("SshPtyReader::run: calling connectNoAuth");
    auto res = _client->connectNoAuth(_sshConfig);
    ydebug("SshPtyReader::run: connectNoAuth returned");
    if (!res) {
        // Connection setup failed immediately (e.g., DNS resolution)
        std::string errMsg = "\033[1;31mConnection failed: " + res.error().message() + "\033[0m\r\n";
        sendToTerminal(errMsg);
        _state = SshState::Disconnected;
        ydebug("SshPtyReader::run: connection failed immediately: {}", res.error().message());
        // Still return Ok - we displayed the error in the terminal
        return Ok();
    }

    // Connection is in progress (async)
    // State machine will advance via poll events
    _state = SshState::Authenticating;

    ydebug("SshPtyReader::run DONE - async connect started to {}@{}:{}",
           _sshConfig.username, _sshConfig.host, _sshConfig.port);
    return Ok();
}

size_t SshPtyReader::read(char* buf, size_t maxLen) {
    // Poll for new data first (only needed if not using event loop)
    if (_client) {
        _client->poll();
    }

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
        // Auth failed - SshClient already sent status message via callback
        _state = SshState::WaitingForPassword;
        return;
    }

    // Auth succeeded - state will advance via poll events
    // (openShell is now handled by SshClient state machine)
    _state = SshState::Connected;
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

    // If there's already data in the buffer (e.g., "Connecting..." message from init),
    // notify immediately so the terminal reads it
    if (!_recvBuffer.empty() && _dataAvailableCallback) {
        _dataAvailableCallback();
    }
}

void SshPtyReader::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

void SshPtyReader::onSshData(const char* data, size_t len) {
    ytrace("SshPtyReader::onSshData received {} bytes", len);
    for (size_t i = 0; i < len; i++) {
        _recvBuffer.push_back(data[i]);
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
