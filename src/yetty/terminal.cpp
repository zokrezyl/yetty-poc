#include "terminal.h"
#include "gpu-screen.h"
#include <yetty/base/event-loop.h>
#include <yetty/result.hpp>
#include <ytrace/ytrace.hpp>
#include <cstring>


#if YETTY_WEB
#include <yetty/platform.h>
#include <yetty/pty-provider.h>
#else
#include <yetty/osc-scanner.h>
#include <yetty/pty-reader.h>
#include "telnet/telnet-pty-reader.h"
#endif

namespace yetty {

class TerminalImpl : public Terminal {
public:
    explicit TerminalImpl(const YettyContext& ctx)
        : _ctx(ctx) {}

    ~TerminalImpl() override = default;

    Result<void> init() {
        yinfo("Terminal::init() start");
        auto screenResult = GPUScreen::create(_ctx);
        if (!screenResult) {
            yerror("Terminal::init() GPUScreen::create FAILED");
            return Err<void>("Failed to create GPUScreen", screenResult);
        }
        yinfo("Terminal::init() GPUScreen created");
        _gpuScreen = *screenResult;

#if YETTY_WEB
        // Webasm: GPUScreen callbacks write to PTYProvider
        _gpuScreen->setOutputCallback([this](const char* data, size_t len) {
            if (_pty) {
                _pty->write(data, len);
            }
        });
        _gpuScreen->setResizeCallback([this](uint32_t cols, uint32_t rows) {
            if (_pty) {
                _pty->resize(cols, rows);
            }
        });
#else
        // Desktop: GPUScreen callbacks write to PtyReader
        _gpuScreen->setOutputCallback([this](const char* data, size_t len) {
            if (_ptyReader) {
                _ptyReader->write(data, len);
            }
        });
        _gpuScreen->setResizeCallback([this](uint32_t cols, uint32_t rows) {
            if (_ptyReader) {
                _ptyReader->resize(cols, rows);
            }
        });
#endif

        // PTY start is deferred to first setViewport() call so we have correct size
        // (GPUScreen cols/rows are 0 until first resize)

        yinfo("Terminal created with GPUScreen");
        return Ok();
    }

    Result<void> onShutdown() override {
        Result<void> result = Ok();

#if YETTY_WEB
        if (_pty) {
            _pty->stop();
            _pty.reset();
        }
#else
        if (_ptyReader) {
            _ptyReader->stop();
            _ptyReader.reset();
        }
#endif
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
#if YETTY_WEB
        if (_pty) {
            _pty->resize(cols, rows);
        }
#else
        if (_ptyReader) {
            _ptyReader->resize(cols, rows);
        }
#endif
    }

    void setViewport(float x, float y, float width, float height) override {
        if (_gpuScreen) {
            _gpuScreen->setViewport(x, y, width, height);

            // Start PTY on first setViewport with valid size (deferred from init)
            // This ensures PTY gets correct initial size instead of hardcoded fallback
            if (!_ptyStarted && _gpuScreen->getCols() > 0 && _gpuScreen->getRows() > 0) {
                _ptyStarted = true;
                if (auto res = startPty(); !res) {
                    ywarn("Terminal: PTY not available: {}", error_msg(res));
                }
            }

#if YETTY_WEB
            if (_pty) {
                _pty->resize(_gpuScreen->getCols(), _gpuScreen->getRows());
            }
#else
            if (_ptyReader) {
                _ptyReader->resize(_gpuScreen->getCols(), _gpuScreen->getRows());
            }
#endif
        }
    }

    void setRenderTargetSize(uint32_t width, uint32_t height) override {
        if (_gpuScreen) {
            _gpuScreen->setRenderTargetSize(width, height);
        }
    }

    float getCellWidth() const override {
        return _gpuScreen ? _gpuScreen->getCellWidth() : 10.0f;
    }

    float getCellHeight() const override {
        return _gpuScreen ? _gpuScreen->getCellHeight() : 20.0f;
    }

    Result<bool> onEvent(const base::Event& event) override {
        (void)event;
        return Ok(false);
    }

private:
    Result<void> startPty() {
        uint32_t cols = (_gpuScreen && _gpuScreen->getCols() > 0) ? _gpuScreen->getCols() : 80;
        uint32_t rows = (_gpuScreen && _gpuScreen->getRows() > 0) ? _gpuScreen->getRows() : 24;

#if YETTY_WEB
        // Webasm: Use Platform::createPTY() which creates WebPTY with JSLinux iframe
        const char* vmConfigEnv = getenv("YETTY_VM_CONFIG");
        std::string vmConfig = vmConfigEnv ? vmConfigEnv : "alpine-x86_64.cfg";
        yinfo("Terminal: using VM config: {}", vmConfig);

        // Get platform to create PTY
        auto platformResult = Platform::create();
        if (!platformResult) {
            return Err<void>("Failed to get platform", platformResult);
        }
        auto platform = *platformResult;

        auto ptyResult = platform->createPTY();
        if (!ptyResult) {
            return Err<void>("Failed to create PTY", ptyResult);
        }
        _pty = *ptyResult;

        // Data callback - write directly to GPUScreen
        // OSC handling is done by GPUScreen::write() which calls handleOSCSequence
        _pty->setDataCallback([this](const char* data, size_t len) {
            if (_gpuScreen) {
                processPtyData(data, len);
            }
        });

        _pty->setExitCallback([this](int exitCode) {
            yinfo("PTY exited with code {}", exitCode);
            _running = false;
        });

        // Start the VM
        if (auto res = _pty->start(vmConfig, cols, rows); !res) {
            return Err<void>("Failed to start PTY", res);
        }

        _running = true;
        yinfo("Terminal started WebPTY: {} ({}x{})", vmConfig, cols, rows);
        return Ok();
#else
        // Check for telnet mode (--telnet flag)
        std::string telnetAddress;
        if (_ctx.config) {
            auto telnetOpt = _ctx.config->get<std::string>("shell/telnet");
            if (telnetOpt && !telnetOpt->empty()) {
                telnetAddress = *telnetOpt;
            }
        }

        if (!telnetAddress.empty()) {
            // Telnet mode: connect to remote/local telnet server
            yinfo("Terminal: using telnet connection to {}", telnetAddress);

            PtyConfig config;
            config.shell = telnetAddress;  // host:port format
            config.cols = cols;
            config.rows = rows;

            auto reader = std::make_shared<telnet::TelnetPtyReader>();
            if (auto res = reader->init(config); !res) {
                return Err<void>("Failed to initialize TelnetPtyReader", res);
            }
            _ptyReader = reader;
            yinfo("Terminal started telnet: {} ({}x{})", telnetAddress, cols, rows);
        } else {
            // Desktop: Use PtyReader with OSC-aware reading
            // Get shell path from SHELL env (or COMSPEC on Windows)
#ifdef _WIN32
            // Windows: prefer PowerShell, then COMSPEC (cmd.exe)
            // Avoid MSYS/Git bash under ConPTY (interactive I/O issues)
            const char* shellEnv = nullptr;
            std::string shellPath;
            if (auto* ps = getenv("YETTY_SHELL")) {
                shellPath = ps;  // Explicit override
            } else if (std::filesystem::exists("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe")) {
                shellPath = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
            } else {
                shellEnv = getenv("COMSPEC");
                shellPath = shellEnv ? shellEnv : "cmd.exe";
            }
#else
            const char* shellEnv = getenv("SHELL");
            std::string shellPath = shellEnv ? shellEnv : "/bin/sh";
#endif

            // Get command from config (if specified via -c flag)
            std::string command;
            if (_ctx.config) {
                auto cmdOpt = _ctx.config->get<std::string>("shell/command");
                if (cmdOpt && !cmdOpt->empty()) {
                    command = *cmdOpt;
                    yinfo("Terminal: using command from config: {}", command);
                }
            }

            PtyConfig config;
            config.shell = shellPath;
            config.command = command;
            config.cols = cols;
            config.rows = rows;

            yinfo("Creating PtyReader: shell={} {}x{}", shellPath, cols, rows);
            auto readerResult = PtyReader::create(config, _ctx.platform);
            if (!readerResult) {
                yerror("PtyReader::create FAILED: {}", readerResult.error().message());
                return Err<void>("Failed to create PtyReader", readerResult);
            }
            yinfo("PtyReader::create SUCCESS");
            _ptyReader = *readerResult;
            yinfo("Terminal started PTY: {} ({}x{})", shellPath, cols, rows);
        }

        // Set up callbacks
        _ptyReader->setDataAvailableCallback([this]() {
            readFromPty();
        });

        _ptyReader->setExitCallback([this](int exitCode) {
            yinfo("PTY exited with code {}", exitCode);
            _running = false;
        });

        _running = true;
        return Ok();
#endif
    }

#if !YETTY_WEB
    /**
     * Read from PTY with OSC-aware logic (Desktop only):
     * - Normal data: read chunk, process, allow rendering
     * - OSC data: keep reading until terminator (could be huge)
     */
    void readFromPty() {
        static constexpr size_t CHUNK_SIZE = 65536;  // 64KB
        char chunk[CHUNK_SIZE];

        int loopCount = 0;
        while (true) {
            size_t n = _ptyReader->read(chunk, CHUNK_SIZE);
            if (n == 0) {
                // No more data available
                if (_oscScanner.isInOsc()) {
                    // Inside OSC, wait for more data (don't process yet)
                    ydebug("readFromPty: incomplete OSC, buffered {} bytes", _ptyBuffer.size());
                    return;
                }
                break;  // Not in OSC, process what we have
            }

            // Accumulate in buffer
            size_t oldSize = _ptyBuffer.size();
            _ptyBuffer.resize(oldSize + n);
            std::memcpy(_ptyBuffer.data() + oldSize, chunk, n);

            // Scan for OSC state
            _oscScanner.scan(chunk, n);

            ydebug("readFromPty loop {}: read {} bytes, total={}, isInOsc={}",
                   loopCount, n, _ptyBuffer.size(), _oscScanner.isInOsc());

            // If NOT in OSC and we have enough data, stop to allow rendering
            if (!_oscScanner.isInOsc() && _ptyBuffer.size() >= CHUNK_SIZE) {
                break;
            }
            // If IN OSC, keep reading until terminator found (could be 2000MB)
            loopCount++;
        }

        if (_ptyBuffer.empty() || !_gpuScreen) return;

        ydebug("readFromPty: processing {} bytes after {} loops", _ptyBuffer.size(), loopCount);

        // DEBUG: hex dump first 64 bytes of PTY data
        {
            std::string hex, ascii;
            size_t dumpLen = std::min(_ptyBuffer.size(), size_t(64));
            for (size_t di = 0; di < dumpLen; di++) {
                uint8_t b = static_cast<uint8_t>(_ptyBuffer[di]);
                char h[4]; snprintf(h, sizeof(h), "%02x ", b); hex += h;
                ascii += (b >= 0x20 && b < 0x7f) ? static_cast<char>(b) : '.';
            }
            yinfo("PTY DATA[{}]: {}", dumpLen, hex);
            yinfo("PTY ASCII: {}", ascii);
        }

        // Reset scanner before processing (processPtyData has its own state)
        _oscScanner.reset();

        // Process: normal data -> vterm, OSC -> handle
        processPtyData(_ptyBuffer.data(), _ptyBuffer.size());
        _ptyBuffer.clear();
    }
#endif

    // OSC parsing states for processPtyData
    enum class OscState {
        Normal,      // Not in OSC
        Esc,         // Saw ESC
        InOsc,       // Inside OSC (accumulating body)
        OscEscEnd    // Saw ESC in body (looking for \)
    };

    void processPtyData(const char* data, size_t len) {
        size_t i = 0;
        size_t normalStart = 0;

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
#if YETTY_WEB
                if (!response.empty() && _pty) {
                    _pty->write(response.c_str(), response.size());
                }
#else
                if (!response.empty() && _ptyReader) {
                    _ptyReader->write(response.c_str(), response.size());
                }
#endif
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
                } else if (c == '\033') {
                    // Another ESC - first one was body data, this one might start ST
                    _oscBuffer += '\033';
                    // Stay in OscEscEnd
                    i++;
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
            flushNormal(len);
            _oscState = OscState::Normal;
        }

        // Note: screen update is already triggered by _gpuScreen->write() -> requestScreenUpdate()
        // via EventQueue (thread-safe cross-thread dispatch to main thread)
    }

#if YETTY_WEB
    // Webasm: PTYProvider (WebPTY)
    std::shared_ptr<PTYProvider> _pty;
#else
    // Desktop: PtyReader with OSC-aware buffering
    PtyReader::Ptr _ptyReader;

    // OSC scanner for readFromPty (detects OSC boundaries)
    OscScanner _oscScanner;

    // Buffer for accumulating data (especially for large OSC sequences)
    std::vector<char> _ptyBuffer;
#endif

    // OSC parsing state for processPtyData (used by both platforms)
    OscState _oscState = OscState::Normal;
    std::string _oscBuffer;
    size_t _oscEscPos = 0;

    bool _running = false;
    bool _ptyStarted = false;  // Track if PTY has been started (deferred until first resize)
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
