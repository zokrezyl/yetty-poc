#include "terminal.h"
#include "gpu-screen.h"
#include <yetty/base/event-loop.h>
#include <yetty/ymery/types.h>
#include <ytrace/ytrace.hpp>

#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32) && !defined(_WIN32)
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif
#endif

namespace yetty {

class TerminalImpl : public Terminal {
public:
    explicit TerminalImpl(const YettyContext& ctx)
        : _ctx(ctx) {}

    ~TerminalImpl() override = default;

    Result<void> init() {
        auto screenResult = GPUScreen::create(_ctx);
        if (!screenResult) {
            return Err<void>("Failed to create GPUScreen", screenResult);
        }
        _gpuScreen = *screenResult;

        _gpuScreen->setOutputCallback([this](const char* data, size_t len) {
            writeToPty(data, len);
        });
        _gpuScreen->setResizeCallback([this](uint32_t cols, uint32_t rows) {
            resizePty(cols, rows);
        });

#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
        if (auto res = startShell(); !res) {
            return Err<void>("Failed to start shell", res);
        }
#endif

        yinfo("Terminal created with GPUScreen");
        return Ok();
    }

    Result<void> onShutdown() override {
        Result<void> result = Ok();

#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
        _running = false;

        if (_pollId >= 0) {
            auto loop = *base::EventLoop::instance();

            // Deregister event listeners — safe here, shared_ptr is still alive
            loop->deregisterListener(sharedAs<base::EventListener>());

            loop->destroyPoll(_pollId);
            _pollId = -1;
        }

        if (_ptyMaster >= 0) {
            close(_ptyMaster);
            _ptyMaster = -1;
        }

        if (_childPid > 0) {
            kill(_childPid, SIGTERM);
            waitpid(_childPid, nullptr, 0);
            _childPid = -1;
        }
#endif

        if (_gpuScreen) {
            if (auto res = _gpuScreen->shutdown(); !res) {
                result = Err<void>("GPUScreen shutdown failed", res);
            }
        }

        return result;
    }

    bool isRunning() const override { return _running; }

    Result<void> render(WGPURenderPassEncoder pass) override {
        if (!_gpuScreen) {
            return Err<void>("No GPUScreen");
        }
        auto result = _gpuScreen->render(pass);
        if (result) {
            _gpuScreen->clearDamage();
        }
        return result;
    }

    void resize(uint32_t cols, uint32_t rows) override {
        if (_gpuScreen) {
            _gpuScreen->resize(cols, rows);
        }
        resizePty(cols, rows);
    }

    void setViewport(float x, float y, float width, float height) override {
        if (_gpuScreen) {
            _gpuScreen->setViewport(x, y, width, height);
            // GPUScreen calculated and resized - now resize PTY to match
            resizePty(_gpuScreen->getCols(), _gpuScreen->getRows());
        }
    }

    uint32_t getCellWidth() const override {
        return _gpuScreen ? _gpuScreen->getCellWidth() : 10;
    }

    uint32_t getCellHeight() const override {
        return _gpuScreen ? _gpuScreen->getCellHeight() : 20;
    }

    Result<bool> onEvent(const base::Event& event) override {
#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
        if (event.type == base::Event::Type::PollReadable && event.poll.fd == _ptyMaster) {
            ydebug("Terminal::onEvent: PollReadable on PTY");
            readPty();
            return Ok(true);
        }
#endif
        // Keyboard events are handled by GPUScreen directly
        return Ok(false);
    }

private:
    void writeToPty(const char* data, size_t len) {
#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
        if (_ptyMaster >= 0 && len > 0) {
            ssize_t written = ::write(_ptyMaster, data, len);
            (void)written;
        }
#else
        (void)data;
        (void)len;
#endif
    }

    void resizePty(uint32_t cols, uint32_t rows) {
#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
        if (_ptyMaster >= 0) {
            struct winsize ws = {
                static_cast<unsigned short>(rows),
                static_cast<unsigned short>(cols), 0, 0
            };
            ioctl(_ptyMaster, TIOCSWINSZ, &ws);
        }
#else
        (void)cols;
        (void)rows;
#endif
    }

#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)
    Result<void> startShell() {
        const char* shellEnv = getenv("SHELL");
        std::string shellPath = shellEnv ? shellEnv : "/bin/sh";

        yinfo("Terminal starting shell: {}", shellPath);

        // Validate config and resolve shell/env before forking
        if (!_ctx.config) {
            yerror("Terminal: no config available, cannot start shell");
            return Err<void>("Terminal: config is required to start shell");
        }
        auto envKeysResult = _ctx.config->getMetadataKeys(ymery::DataPath("shell/env"));
        if (!envKeysResult) {
            yerror("Terminal: failed to read shell/env from config: {}", error_msg(envKeysResult));
            return Err<void>("Terminal: failed to read shell/env from config", envKeysResult);
        }
        if (envKeysResult->empty()) {
            yerror("Terminal: shell/env is empty in config");
            return Err<void>("Terminal: shell/env must not be empty");
        }

        // Resolve all env var values before forking
        std::vector<std::pair<std::string, std::string>> envVars;
        for (const auto& name : *envKeysResult) {
            auto val = _ctx.config->Config::get<std::string>("shell/env/" + name);
            if (!val) {
                yerror("Terminal: shell/env/{} is missing or not a string", name);
                return Err<void>("Terminal: shell/env/" + name + " is missing or not a string");
            }
            envVars.emplace_back(name, *val);
        }

        struct winsize ws = {24, 80, 0, 0};

        _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);

        if (_childPid < 0) {
            return Err<void>(std::string("forkpty failed: ") + strerror(errno));
        }

        if (_childPid == 0) {
            // Export shell env from config (TERM, COLORTERM, YETTY_SOCKET, etc.)
            for (const auto& [name, value] : envVars) {
                setenv(name.c_str(), value.c_str(), 1);
            }

            for (int fd = 3; fd < 1024; fd++)
                close(fd);

            execl(shellPath.c_str(), shellPath.c_str(), nullptr);
            _exit(1);
        }

        int flags = fcntl(_ptyMaster, F_GETFL, 0);
        fcntl(_ptyMaster, F_SETFL, flags | O_NONBLOCK);

        auto loop = *base::EventLoop::instance();
        auto pollResult = loop->createPoll();
        if (!pollResult) {
            return Err<void>("Failed to create poll", pollResult);
        }
        _pollId = *pollResult;
        if (auto res = loop->configPoll(_pollId, _ptyMaster); !res) {
            return Err<void>("Failed to configure poll", res);
        }
        if (auto res = loop->registerPollListener(_pollId, sharedAs<base::EventListener>()); !res) {
            return Err<void>("Failed to register poll listener", res);
        }
        if (auto res = loop->startPoll(_pollId); !res) {
            return Err<void>("Failed to start poll", res);
        }

        _running = true;
        yinfo("Terminal started: PTY fd={}, PID={}", _ptyMaster, _childPid);
        return Ok();
    }

    void readPty() {
        ydebug("Terminal::readPty: _ptyMaster={} _childPid={}, pendingBuf={}",
               _ptyMaster, _childPid, _ptyBuffer.size());
        int status;
        if (waitpid(_childPid, &status, WNOHANG) > 0) {
            _running = false;
            yinfo("Shell exited");
            return;
        }

        static constexpr size_t PTY_READ_CHUNK = 65536; // 64KB
        ssize_t n;

        // Read and scan for OSC in a loop
        // - Normal text: stop at PTY_READ_CHUNK to allow rendering
        // - Inside OSC: keep reading until terminator found
        int loopCount = 0;
        while (true) {
            size_t oldSize = _ptyBuffer.size();
            _ptyBuffer.resize(oldSize + PTY_READ_CHUNK);

            n = read(_ptyMaster, _ptyBuffer.data() + oldSize, PTY_READ_CHUNK);
            if (n <= 0) {
                _ptyBuffer.resize(oldSize); // Remove unused space
                ydebug("readPty: read returned {}, isInOsc={}", n, isInOsc());
                if (isInOsc()) {
                    // Still inside OSC but no more data - wait for next poll
                    ydebug("readPty: incomplete OSC, keeping {} bytes for next call", _ptyBuffer.size());
                    return;
                }
                break;
            }
            _ptyBuffer.resize(oldSize + n);

            // Scan this chunk to update OSC state
            scanForOsc(_ptyBuffer.data() + oldSize, n);
            ydebug("readPty loop {}: read {} bytes, totalBuf={}, isInOsc={}",
                   loopCount, n, _ptyBuffer.size(), isInOsc());

            // If not inside an OSC, stop after first chunk to allow rendering
            if (!isInOsc() && _ptyBuffer.size() >= PTY_READ_CHUNK) {
                break;
            }
            // If inside OSC, keep reading until terminator found
            loopCount++;
        }

        ydebug("readPty: processing {} bytes in {} loops", _ptyBuffer.size(), loopCount);
        if (_ptyBuffer.empty() || !_gpuScreen) return;

        // Reset scan state before processing (processPtyData has its own state)
        resetOscScan();

        // Process the complete buffer and clear it
        processPtyData(_ptyBuffer.data(), _ptyBuffer.size());
        _ptyBuffer.clear();
    }

    // Lightweight scan: just detect if we're inside ANY OSC (ESC ] ... until BEL or ST)
    void scanForOsc(const char* data, size_t len) {
        for (size_t i = 0; i < len; i++) {
            char c = data[i];
            int prev = _oscScanState;
            switch (_oscScanState) {
            case 0: // Normal
                if (c == '\033') _oscScanState = 1; // Esc
                break;
            case 1: // Esc
                _oscScanState = (c == ']') ? 2 : 0; // InOsc or Normal
                break;
            case 2: // InOsc
                if (c == '\007') {
                    _oscScanState = 0; // BEL terminator
                } else if (c == '\033') {
                    _oscScanState = 3; // OscEscEnd
                }
                break;
            case 3: // OscEscEnd (checking for \)
                _oscScanState = (c == '\\') ? 0 : 2; // Normal or back to InOsc
                break;
            }
            if (prev != _oscScanState && (_oscScanState == 2 || prev == 2 || _oscScanState == 0 && prev >= 2)) {
                ydebug("scanForOsc: state {} -> {} at pos {} (char 0x{:02x})",
                       prev, _oscScanState, i, (unsigned char)c);
            }
        }
    }

    bool isInOsc() const {
        return _oscScanState >= 2; // InOsc or OscEscEnd
    }

    void resetOscScan() {
        _oscScanState = 0;
    }

    // OSC parsing states
    enum class OscState {
        Normal,      // Not in OSC
        Esc,         // Saw ESC
        OscStart,    // Saw ESC ]
        OscCmd,      // Parsing command number
        OscBody,     // In OSC body (for yetty OSC)
        OscEscEnd    // Saw ESC in body (looking for \)
    };

    void processPtyData(const char* data, size_t len) {
        size_t i = 0;
        size_t normalStart = 0;  // Start of normal (non-OSC) data

        auto flushNormal = [&](size_t end) {
            if (end > normalStart) {
                _gpuScreen->write(data + normalStart, end - normalStart);
            }
        };

        auto flushYettyOsc = [&]() {
            if (!_oscBuffer.empty()) {
                // Build full sequence: "cmd;body"
                std::string fullSeq = std::to_string(_oscCmd) + ";" + _oscBuffer;
                std::string response;
                uint32_t linesToAdvance = 0;
                bool handled = _gpuScreen->handleOSCSequence(fullSeq, &response, &linesToAdvance);
                if (handled) {
                    if (!response.empty()) {
                        writeToPty(response.c_str(), response.size());
                    }
                    if (linesToAdvance > 0) {
                        std::string nl(linesToAdvance, '\n');
                        _gpuScreen->write(nl.c_str(), nl.size());
                    }
                }
                _oscBuffer.clear();
            }
            _oscState = OscState::Normal;
            _oscCmd = 0;
        };

        while (i < len) {
            char c = data[i];

            switch (_oscState) {
            case OscState::Normal:
                if (c == '\033') {
                    _oscState = OscState::Esc;
                    _oscEscPos = i;
                }
                i++;
                break;

            case OscState::Esc:
                if (c == ']') {
                    _oscState = OscState::OscStart;
                    _oscCmd = 0;
                    _oscCmdStr.clear();
                } else {
                    // Not OSC, stay normal
                    _oscState = OscState::Normal;
                }
                i++;
                break;

            case OscState::OscStart:
            case OscState::OscCmd:
                if (c >= '0' && c <= '9') {
                    _oscCmdStr += c;
                    _oscState = OscState::OscCmd;
                    i++;
                } else if (c == ';') {
                    // End of command number
                    if (!_oscCmdStr.empty()) {
                        _oscCmd = std::stoi(_oscCmdStr);
                    }
                    if (_oscCmd >= 666666) {
                        // Yetty OSC - bypass vterm
                        flushNormal(_oscEscPos);
                        _oscBuffer.clear();
                        _oscState = OscState::OscBody;
                    } else {
                        // Standard OSC - let vterm handle
                        _oscState = OscState::Normal;
                    }
                    i++;
                } else if (c == '\007' || c == '\033') {
                    // Terminator without body
                    _oscState = OscState::Normal;
                    if (c == '\033') {
                        // Stay at this position to check for backslash
                    } else {
                        i++;
                    }
                } else {
                    // Invalid OSC
                    _oscState = OscState::Normal;
                    i++;
                }
                break;

            case OscState::OscBody:
                if (c == '\007') {
                    // BEL terminator
                    flushYettyOsc();
                    normalStart = i + 1;
                    i++;
                } else if (c == '\033') {
                    _oscState = OscState::OscEscEnd;
                    i++;
                } else {
                    _oscBuffer += c;
                    i++;
                }
                break;

            case OscState::OscEscEnd:
                if (c == '\\') {
                    // ST terminator (ESC \)
                    flushYettyOsc();
                    normalStart = i + 1;
                } else {
                    // Not a terminator, ESC is part of body
                    _oscBuffer += '\033';
                    _oscBuffer += c;
                    _oscState = OscState::OscBody;
                }
                i++;
                break;
            }
        }

        // Flush remaining normal data
        if (_oscState == OscState::Normal) {
            flushNormal(len);
        } else if (_oscState == OscState::Esc || _oscState == OscState::OscStart || _oscState == OscState::OscCmd) {
            // Incomplete standard OSC start - flush everything, vterm will handle
            flushNormal(len);
            _oscState = OscState::Normal;
        }
        // If in OscBody or OscEscEnd, keep accumulating (waiting for more data)

        // Trigger screen refresh
        if (auto loop = base::EventLoop::instance(); loop) {
            (*loop)->dispatch(base::Event::screenUpdateEvent());
        }
    }

    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;

    // OSC parsing state (for processPtyData)
    OscState _oscState = OscState::Normal;
    int _oscCmd = 0;
    std::string _oscCmdStr;
    std::string _oscBuffer;
    size_t _oscEscPos = 0;

    // OSC scan state (for readPty to decide read length)
    // 0=Normal, 1=Esc, 2=InOsc, 3=OscEscEnd
    int _oscScanState = 0;

    // Persistent buffer for incomplete OSC sequences
    std::vector<char> _ptyBuffer;
#endif

    bool _running = false;
    YettyContext _ctx;
    GPUScreen::Ptr _gpuScreen;
};

Result<Terminal::Ptr> Terminal::create(const YettyContext& ctx) noexcept {
    auto term = std::make_shared<TerminalImpl>(ctx);
    if (auto res = term->init(); !res) {
        return Err<Ptr>("Failed to initialize Terminal", res);
    }
    return Ok<Ptr>(term);
}

} // namespace yetty
