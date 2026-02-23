#include "terminal.h"
#include "gpu-screen.h"
#include <yetty/base/event-loop.h>
#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#include <yetty/ymery/types.h>
#include <ytrace/ytrace.hpp>

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

        // Start PTY via platform
        if (auto res = startPty(); !res) {
            // On some platforms (e.g., Android stub), PTY may not be available
            // Log warning but don't fail - terminal can still be used for display
            ywarn("Terminal: PTY not available: {}", error_msg(res));
        }

        yinfo("Terminal created with GPUScreen");
        return Ok();
    }

    Result<void> onShutdown() override {
        Result<void> result = Ok();

        if (_pty) {
            _pty->stop();
            _pty.reset();
        }
        _running = false;

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
        // PTY events are now handled internally by PTYProvider
        // Keyboard events are handled by GPUScreen directly
        (void)event;
        return Ok(false);
    }

private:
    void writeToPty(const char* data, size_t len) {
        if (_pty && len > 0) {
            _pty->write(data, len);
        }
    }

    void resizePty(uint32_t cols, uint32_t rows) {
        if (_pty) {
            _pty->resize(cols, rows);
        }
    }

    Result<void> startPty() {
        if (!_ctx.platform) {
            return Err<void>("No platform available");
        }

        auto ptyResult = _ctx.platform->createPTY();
        if (!ptyResult) {
            return Err<void>("Failed to create PTY", ptyResult);
        }
        _pty = *ptyResult;

        // Set up callbacks
        _pty->setDataCallback([this](const char* data, size_t len) {
            onPtyData(data, len);
        });

        _pty->setExitCallback([this](int exitCode) {
            yinfo("PTY exited with code {}", exitCode);
            _running = false;
        });

        // Determine shell/VM config
        std::string shellOrConfig;
#if YETTY_WEB
        // Web: use JSLinux VM config
        shellOrConfig = "alpine-x86_64.cfg";
#else
        // Desktop: use shell from environment or config
        const char* shellEnv = getenv("SHELL");
        shellOrConfig = shellEnv ? shellEnv : "/bin/sh";
#endif

        uint32_t cols = (_gpuScreen && _gpuScreen->getCols() > 0) ? _gpuScreen->getCols() : 80;
        uint32_t rows = (_gpuScreen && _gpuScreen->getRows() > 0) ? _gpuScreen->getRows() : 24;

        yinfo("Terminal starting PTY: {} ({}x{})", shellOrConfig, cols, rows);

        if (auto res = _pty->start(shellOrConfig, cols, rows); !res) {
            return Err<void>("Failed to start PTY", res);
        }

        _running = true;
        return Ok();
    }

    void onPtyData(const char* data, size_t len) {
        if (!_gpuScreen || len == 0) return;

        // Accumulate data in buffer for OSC processing
        size_t oldSize = _ptyBuffer.size();
        _ptyBuffer.resize(oldSize + len);
        std::memcpy(_ptyBuffer.data() + oldSize, data, len);

        // Scan for incomplete OSC sequences
        scanForOsc(data, len);

        // If inside an OSC, wait for more data
        if (isInOsc()) {
            ydebug("onPtyData: incomplete OSC, buffering {} bytes", _ptyBuffer.size());
            return;
        }

        // Process complete buffer
        if (!_ptyBuffer.empty()) {
            resetOscScan();
            processPtyData(_ptyBuffer.data(), _ptyBuffer.size());
            _ptyBuffer.clear();
        }
    }

    // Lightweight scan: just detect if we're inside ANY OSC (ESC ] ... until BEL or ST)
    void scanForOsc(const char* data, size_t len) {
        for (size_t i = 0; i < len; i++) {
            char c = data[i];
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
        InOsc,       // Inside OSC (accumulating body)
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

        auto handleOsc = [&](size_t terminatorEnd, bool isST) {
            std::string response;
            uint32_t linesToAdvance = 0;
            bool handled = _gpuScreen->handleOSCSequence(_oscBuffer, &response, &linesToAdvance);
            if (handled) {
                if (!response.empty()) {
                    writeToPty(response.c_str(), response.size());
                }
                if (linesToAdvance > 0) {
                    std::string nl(linesToAdvance, '\n');
                    _gpuScreen->write(nl.c_str(), nl.size());
                }
            } else {
                // Not handled - reconstruct and forward to vterm
                std::string reconstructed = "\033]";
                reconstructed += _oscBuffer;
                reconstructed += isST ? "\033\\" : "\007";
                _gpuScreen->write(reconstructed.data(), reconstructed.size());
            }
            _oscBuffer.clear();
            _oscState = OscState::Normal;
            normalStart = terminatorEnd;
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
                    flushNormal(_oscEscPos);
                    _oscBuffer.clear();
                    _oscState = OscState::InOsc;
                } else {
                    _oscState = OscState::Normal;
                }
                i++;
                break;

            case OscState::InOsc:
                if (c == '\007') {
                    i++;
                    handleOsc(i, false);
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
                    i++;
                    handleOsc(i, true);
                } else {
                    // Not a terminator, ESC is part of body
                    _oscBuffer += '\033';
                    _oscBuffer += c;
                    _oscState = OscState::InOsc;
                    i++;
                }
                break;
            }
        }

        // Flush remaining normal data
        if (_oscState == OscState::Normal) {
            flushNormal(len);
        } else if (_oscState == OscState::Esc) {
            // Incomplete ESC at end of chunk - flush everything
            flushNormal(len);
            _oscState = OscState::Normal;
        }
        // If in InOsc or OscEscEnd, keep accumulating (waiting for more data)

        // Trigger screen refresh
        if (auto loop = base::EventLoop::instance(); loop) {
            (*loop)->dispatch(base::Event::screenUpdateEvent());
        }
    }

    // PTY provider (platform-specific)
    PTYProvider::Ptr _pty;

    // OSC parsing state (for processPtyData)
    OscState _oscState = OscState::Normal;
    std::string _oscBuffer;
    size_t _oscEscPos = 0;

    // OSC scan state (for onPtyData to detect incomplete sequences)
    // 0=Normal, 1=Esc, 2=InOsc, 3=OscEscEnd
    int _oscScanState = 0;

    // Persistent buffer for incomplete OSC sequences
    std::vector<char> _ptyBuffer;

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
