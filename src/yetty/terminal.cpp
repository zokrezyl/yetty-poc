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
        ydebug("Terminal::readPty: _ptyMaster={} _childPid={}", _ptyMaster, _childPid);
        int status;
        if (waitpid(_childPid, &status, WNOHANG) > 0) {
            _running = false;
            yinfo("Shell exited");
            return;
        }

        static constexpr size_t PTY_READ_MAX = 40960; // 40KB
        char buf[PTY_READ_MAX];
        size_t totalRead = 0;
        ssize_t n;

        while ((n = read(_ptyMaster, buf + totalRead, PTY_READ_MAX - totalRead)) > 0) {
            totalRead += n;
            if (totalRead >= PTY_READ_MAX) break;
        }

        if (totalRead > 0 && _gpuScreen) {
            _gpuScreen->write(buf, totalRead);
        }
    }

    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
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
