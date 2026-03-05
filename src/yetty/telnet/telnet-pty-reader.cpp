#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "telnet-pty-reader.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>

namespace yetty::telnet {

TelnetPtyReader::~TelnetPtyReader() {
    stop();
}

Result<void> TelnetPtyReader::init(const PtyConfig& config) {
    // Parse shell as WebSocket URL, "host:port", or just "port"
    std::string host = "127.0.0.1";
    uint16_t port = 8023;  // Default Termux telnetd port

    const std::string& spec = config.shell;
    if (!spec.empty()) {
        // Check if it's a WebSocket URL (for Emscripten)
        if (spec.find("ws://") == 0 || spec.find("wss://") == 0) {
            // Pass the full URL as host - TelnetClient::connect will handle it
            host = spec;
            port = 0;  // Ignored for WebSocket URLs
        } else {
            size_t colonPos = spec.find(':');
            if (colonPos != std::string::npos) {
                host = spec.substr(0, colonPos);
                port = static_cast<uint16_t>(std::stoi(spec.substr(colonPos + 1)));
            } else {
                // Just a port number
                port = static_cast<uint16_t>(std::stoi(spec));
            }
        }
    }

    _client = std::make_shared<TelnetClient>();
    _client->setTerminalType("xterm-256color");

    // Set up callbacks
    _client->setDataCallback([this](const char* data, size_t len) {
        onTelnetData(data, len);
    });

    _client->setDisconnectCallback([this]() {
        onTelnetDisconnect();
    });

    // Connect
    auto res = _client->connect(host, port);
    if (!res) {
        return Err<void>("Failed to connect to telnet server", res);
    }

    // Send initial window size after connection
    _client->sendWindowSize(config.cols, config.rows);

    _running = true;
    yinfo("TelnetPtyReader: connected to {}:{}", host, port);
    return Ok();
}

size_t TelnetPtyReader::read(char* buf, size_t maxLen) {
    std::lock_guard<std::mutex> lock(_mutex);

    size_t toRead = std::min(maxLen, _recvBuffer.size());
    for (size_t i = 0; i < toRead; i++) {
        buf[i] = _recvBuffer.front();
        _recvBuffer.pop_front();
    }
    return toRead;
}

void TelnetPtyReader::write(const char* data, size_t len) {
    if (_client && _client->isConnected()) {
        _client->send(data, len);
    }
}

void TelnetPtyReader::resize(uint32_t cols, uint32_t rows) {
    if (_client && _client->isConnected()) {
        _client->sendWindowSize(static_cast<uint16_t>(cols),
                                 static_cast<uint16_t>(rows));
    }
}

bool TelnetPtyReader::isRunning() const {
    return _running && _client && _client->isConnected();
}

void TelnetPtyReader::stop() {
    if (!_running) return;
    _running = false;

    if (_client) {
        _client->disconnect();
        _client.reset();
    }

    yinfo("TelnetPtyReader: stopped");
}

void TelnetPtyReader::setDataAvailableCallback(DataAvailableCallback cb) {
    _dataAvailableCallback = std::move(cb);
}

void TelnetPtyReader::setExitCallback(ExitCallback cb) {
    _exitCallback = std::move(cb);
}

void TelnetPtyReader::onTelnetData(const char* data, size_t len) {
    ytrace("TelnetPtyReader::onTelnetData received {} bytes", len);
    {
        std::lock_guard<std::mutex> lock(_mutex);
        for (size_t i = 0; i < len; i++) {
            _recvBuffer.push_back(data[i]);
        }
    }

    // Notify terminal that data is available
    if (_dataAvailableCallback) {
        _dataAvailableCallback();
    } else {
        ywarn("TelnetPtyReader: NO dataAvailableCallback set!");
    }
}

void TelnetPtyReader::onTelnetDisconnect() {
    _running = false;

    if (_exitCallback) {
        _exitCallback(0);  // Exit code 0 for clean disconnect
    }
}

// Factory function for creating TelnetPtyReader
Result<PtyReader::Ptr> createTelnetPtyReader(const PtyConfig& config) {
    auto reader = std::make_shared<TelnetPtyReader>();
    if (auto res = reader->init(config); !res) {
        return Err<PtyReader::Ptr>("Failed to initialize TelnetPtyReader", res);
    }
    return Ok<PtyReader::Ptr>(reader);
}

} // namespace yetty::telnet
