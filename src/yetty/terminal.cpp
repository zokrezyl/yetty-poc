#include "terminal.h"
#include "gpu-screen.h"
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#if !YETTY_WEB && !defined(__ANDROID__)
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
    explicit TerminalImpl(const GPUContext& gpuContext)
        : _gpuContext(gpuContext) {}

    ~TerminalImpl() override {
        stop();
    }

    Result<void> init() {
        auto gpuResult = GPUScreen::create(_gpuContext);
        if (!gpuResult) {
            return Err<void>("Failed to create GPUScreen", gpuResult);
        }
        _gpuScreen = *gpuResult;

        _gpuScreen->setOutputCallback([this](const char* data, size_t len) {
            writeToPty(data, len);
        });

#if !YETTY_WEB && !defined(__ANDROID__)
        if (auto res = startShell(); !res) {
            return Err<void>("Failed to start shell", res);
        }
#endif

        yinfo("Terminal created with GPUScreen");
        return Ok();
    }

    void stop() override {
#if !YETTY_WEB && !defined(__ANDROID__)
        _running = false;

        auto loop = base::EventLoop::instance();

        // Deregister keyboard event listeners
        loop->deregisterListener(sharedAs<base::EventListener>());

        if (_pollId >= 0) {
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

    float getCellWidth() const override {
        return _gpuScreen ? _gpuScreen->getCellWidth() : 10.0f;
    }

    float getCellHeight() const override {
        return _gpuScreen ? _gpuScreen->getCellHeight() : 20.0f;
    }

    void sendKey(uint32_t codepoint, int mods) override {
        if (_gpuScreen) {
            _gpuScreen->sendKey(codepoint, mods);
        }
    }

    void sendSpecialKey(int key, int mods) override {
        if (_gpuScreen) {
            _gpuScreen->sendSpecialKey(key, mods);
        }
    }

    Result<bool> onEvent(const base::Event& event) override {
#if !YETTY_WEB && !defined(__ANDROID__)
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
#if !YETTY_WEB && !defined(__ANDROID__)
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
#if !YETTY_WEB && !defined(__ANDROID__)
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

#if !YETTY_WEB && !defined(__ANDROID__)
    Result<void> startShell() {
        const char* shellEnv = getenv("SHELL");
        std::string shellPath = shellEnv ? shellEnv : "/bin/sh";

        yinfo("Terminal starting shell: {}", shellPath);

        struct winsize ws = {24, 80, 0, 0};

        _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);

        if (_childPid < 0) {
            return Err<void>(std::string("forkpty failed: ") + strerror(errno));
        }

        if (_childPid == 0) {
            setenv("TERM", "xterm-256color", 1);
            setenv("COLORTERM", "truecolor", 1);

            for (int fd = 3; fd < 1024; fd++)
                close(fd);

            execl(shellPath.c_str(), shellPath.c_str(), nullptr);
            _exit(1);
        }

        int flags = fcntl(_ptyMaster, F_GETFL, 0);
        fcntl(_ptyMaster, F_SETFL, flags | O_NONBLOCK);

        auto loop = base::EventLoop::instance();
        _pollId = loop->createPoll();
        loop->configPoll(_pollId, _ptyMaster);
        loop->registerPollListener(_pollId, sharedAs<base::EventListener>());
        loop->startPoll(_pollId);

        // Register GPUScreen for focus events (keyboard events are handled by GPUScreen)
        if (_gpuScreen) {
            _gpuScreen->registerForFocus();
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

        char buf[4096];
        ssize_t n;

        while ((n = read(_ptyMaster, buf, sizeof(buf))) > 0) {
            ydebug("Terminal::readPty: read {} bytes", n);
            if (_gpuScreen) {
                _gpuScreen->write(buf, static_cast<size_t>(n));
            }
        }
        ydebug("Terminal::readPty: done reading");
    }

    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
#endif

    bool _running = false;
    GPUContext _gpuContext;
    GPUScreen::Ptr _gpuScreen;
};

Result<Terminal::Ptr> Terminal::create(const GPUContext& gpuContext) noexcept {
    auto term = std::make_shared<TerminalImpl>(gpuContext);
    if (auto res = term->init(); !res) {
        return Err<Ptr>("Failed to initialize Terminal", res);
    }
    return Ok<Ptr>(term);
}

} // namespace yetty
