#include <yetty/platform/pty-manager.h>

#if YETTY_IOS

#include "../../telnet/telnet-pty-reader.h"
#include <ytrace/ytrace.hpp>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <deque>
#include <atomic>

namespace yetty {

// iOS Demo PTY Reader - provides simulated terminal when no SSH connection
class IosDemoPtyReader : public PtyReader {
public:
    Result<void> init(const PtyConfig& config) {
        _cols = config.cols;
        _rows = config.rows;
        _running = true;
        _needsWelcome = true;
        return Ok();
    }

    size_t read(char* buf, size_t maxLen) override {
        sendWelcomeIfNeeded();

        std::lock_guard<std::mutex> lock(_mutex);
        size_t toRead = std::min(maxLen, _recvBuffer.size());
        for (size_t i = 0; i < toRead; i++) {
            buf[i] = _recvBuffer.front();
            _recvBuffer.pop_front();
        }
        return toRead;
    }

    void write(const char* data, size_t len) override {
        std::string input(data, len);
        std::string response;

        for (char c : input) {
            if (c == '\r' || c == '\n') {
                response += processCommand(_inputLine);
                _inputLine.clear();
            } else if (c == 127 || c == 8) {
                if (!_inputLine.empty()) {
                    _inputLine.pop_back();
                    response += "\b \b";
                }
            } else {
                _inputLine += c;
                response += c;
            }
        }

        if (!response.empty()) {
            std::lock_guard<std::mutex> lock(_mutex);
            for (char c : response) {
                _recvBuffer.push_back(c);
            }
            if (_dataCallback) {
                _dataCallback();
            }
        }
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;
    }

    bool isRunning() const override { return _running; }

    void stop() override {
        _running = false;
    }

    void setDataAvailableCallback(DataAvailableCallback cb) override {
        _dataCallback = std::move(cb);
        sendWelcomeIfNeeded();
        if (_dataCallback && !_recvBuffer.empty()) {
            _dataCallback();
        }
    }

    void setExitCallback(ExitCallback cb) override {
        _exitCallback = std::move(cb);
    }

private:
    void sendWelcomeIfNeeded() {
        if (!_needsWelcome) return;
        _needsWelcome = false;

        std::string welcome = generateWelcome();
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (char c : welcome) {
                _recvBuffer.push_back(c);
            }
        }
    }

    std::string generateWelcome() {
        std::string msg;
        msg += "\033[2J\033[H";
        msg += "\033[1;36m";
        msg += "Yetty Terminal - iOS Demo Mode\r\n";
        msg += "\033[0m\r\n";
        msg += "iOS does not allow spawning shell processes.\r\n";
        msg += "Configure SSH connection in settings for remote shell access.\r\n\r\n";
        msg += "Type '\033[1;33mhelp\033[0m' for available demo commands.\r\n\r\n";
        msg += "\033[1;32myetty\033[0m$ ";
        return msg;
    }

    std::string processCommand(const std::string& cmd) {
        std::string response = "\r\n";
        std::string trimmed = cmd;

        while (!trimmed.empty() && (trimmed.back() == ' ' || trimmed.back() == '\t'))
            trimmed.pop_back();
        while (!trimmed.empty() && (trimmed.front() == ' ' || trimmed.front() == '\t'))
            trimmed.erase(0, 1);

        if (trimmed.empty()) {
            // Just prompt
        } else if (trimmed == "help") {
            response += "Demo commands: help, clear, uname, date, whoami, pwd, ls, echo, exit\r\n";
        } else if (trimmed == "clear") {
            response = "\033[2J\033[H";
        } else if (trimmed == "uname" || trimmed == "uname -a") {
            response += "Darwin iPhone iOS Yetty-Demo arm64\r\n";
        } else if (trimmed == "date") {
            time_t now = time(nullptr);
            char buf[64];
            strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Z %Y", localtime(&now));
            response += buf;
            response += "\r\n";
        } else if (trimmed == "whoami") {
            response += "mobile\r\n";
        } else if (trimmed == "pwd") {
            response += "/var/mobile/yetty\r\n";
        } else if (trimmed == "ls" || trimmed == "ls -la") {
            response += "total 0\r\n";
            response += "drwxr-xr-x  2 mobile mobile 64 Jan  1 00:00 .\r\n";
            response += "drwxr-xr-x  3 mobile mobile 96 Jan  1 00:00 ..\r\n";
        } else if (trimmed.substr(0, 4) == "echo") {
            if (trimmed.length() > 5) {
                response += trimmed.substr(5) + "\r\n";
            } else {
                response += "\r\n";
            }
        } else if (trimmed == "exit") {
            response += "Goodbye!\r\n";
            _running = false;
            if (_exitCallback) {
                _exitCallback(0);
            }
            return response;
        } else {
            response += "\033[1;31m" + trimmed + ": command not found\033[0m\r\n";
        }

        response += "\033[1;32myetty\033[0m$ ";
        return response;
    }

    std::mutex _mutex;
    std::deque<char> _recvBuffer;
    std::string _inputLine;
    uint32_t _cols = 80;
    uint32_t _rows = 24;
    std::atomic<bool> _running{false};
    std::atomic<bool> _needsWelcome{false};
    DataAvailableCallback _dataCallback;
    ExitCallback _exitCallback;
};


class IosPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<PtyReader::Ptr> createPtyReader(const PtyConfig& config) override {
        ydebug("IosPtyManager::createPtyReader called");

        // Check environment for SSH configuration
        const char* sshHost = std::getenv("YETTY_SSH_HOST");
        const char* sshPort = std::getenv("YETTY_SSH_PORT");

        // If shell config looks like host:port, try telnet/SSH connection
        if (!config.shell.empty() && config.shell[0] != '/') {
            yinfo("iOS: Attempting connection to {}", config.shell);

            auto reader = std::make_shared<telnet::TelnetPtyReader>();
            if (auto res = reader->init(config); res) {
                yinfo("iOS: Connection successful!");
                return Ok<PtyReader::Ptr>(reader);
            } else {
                ywarn("iOS: Connection failed: {}", res.error().message());
            }
        }

        // Try SSH from environment
        if (sshHost && sshHost[0] != '\0') {
            std::string addr = std::string(sshHost);
            if (sshPort && sshPort[0] != '\0') {
                addr += ":" + std::string(sshPort);
            } else {
                addr += ":22";
            }

            yinfo("iOS: Attempting SSH connection to {}", addr);

            PtyConfig sshConfig = config;
            sshConfig.shell = addr;

            auto reader = std::make_shared<telnet::TelnetPtyReader>();
            if (auto res = reader->init(sshConfig); res) {
                yinfo("iOS: SSH connection successful!");
                return Ok<PtyReader::Ptr>(reader);
            } else {
                ywarn("iOS: SSH connection failed: {}", res.error().message());
            }
        }

        // Fall back to demo mode
        yinfo("iOS: Using demo mode (no SSH connection configured)");
        auto reader = std::make_shared<IosDemoPtyReader>();
        if (auto res = reader->init(config); !res) {
            return Err<PtyReader::Ptr>("Failed to initialize demo PTY", res);
        }
        return Ok<PtyReader::Ptr>(reader);
    }
};

// Factory
Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto mgr = new IosPtyManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("IosPtyManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // YETTY_IOS
