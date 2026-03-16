#include <yetty/platform/pty-manager.h>

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__) && !defined(_WIN32)

#include <yetty/pty-provider.h>
#include <yetty/base/event-loop.h>
#include <yetty/base/event-queue.h>
#include <ytrace/ytrace.hpp>

#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <atomic>
#include <vector>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

namespace yetty {

static std::atomic<uint32_t> g_nextPtyId{1};

class ForkPTY : public PTYProvider, public base::EventListener {
public:
    ForkPTY() : _id(g_nextPtyId++) {}

    ~ForkPTY() override {
        stop();
    }

    Result<void> start(const std::string& shell, uint32_t cols, uint32_t rows) override {
        _shell = shell;
        _cols = cols;
        _rows = rows;

        ydebug("ForkPTY[{}]: Starting shell '{}' ({}x{})", _id, shell, cols, rows);

        struct winsize ws = {
            static_cast<unsigned short>(rows),
            static_cast<unsigned short>(cols),
            0, 0
        };

        _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);

        if (_childPid < 0) {
            return Err<void>(std::string("forkpty failed: ") + strerror(errno));
        }

        if (_childPid == 0) {
            // Child process
            for (int fd = 3; fd < 1024; fd++)
                close(fd);

            execl(shell.c_str(), shell.c_str(), nullptr);
            _exit(1);
        }

        // Parent process
        int flags = fcntl(_ptyMaster, F_GETFL, 0);
        fcntl(_ptyMaster, F_SETFL, flags | O_NONBLOCK);

        // Set up event loop polling
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("No event loop available");
        }
        auto loop = *loopResult;

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
        ydebug("ForkPTY[{}]: Started PTY fd={}, PID={}", _id, _ptyMaster, _childPid);
        return Ok();
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        ydebug("ForkPTY[{}]: Stopping", _id);

        if (_pollId >= 0) {
            auto loopResult = base::EventLoop::instance();
            if (loopResult) {
                auto loop = *loopResult;
                loop->deregisterListener(sharedAs<base::EventListener>());
                loop->destroyPoll(_pollId);
            }
            _pollId = -1;
        }

        if (_ptyMaster >= 0) {
            close(_ptyMaster);
            _ptyMaster = -1;
        }

        if (_childPid > 0) {
            kill(_childPid, SIGTERM);
            int status;
            waitpid(_childPid, &status, 0);
            if (_exitCallback) {
                _exitCallback(WEXITSTATUS(status));
            }
            _childPid = -1;
        }
    }

    void write(const char* data, size_t len) override {
        if (_ptyMaster >= 0 && len > 0) {
            ssize_t written = ::write(_ptyMaster, data, len);
            (void)written;
        }
    }

    void resize(uint32_t cols, uint32_t rows) override {
        _cols = cols;
        _rows = rows;

        if (_ptyMaster >= 0) {
            struct winsize ws = {
                static_cast<unsigned short>(rows),
                static_cast<unsigned short>(cols),
                0, 0
            };
            ioctl(_ptyMaster, TIOCSWINSZ, &ws);
        }
    }

    bool isRunning() const override { return _running; }

    void setDataCallback(DataCallback cb) override { _dataCallback = std::move(cb); }
    void setExitCallback(ExitCallback cb) override { _exitCallback = std::move(cb); }
    uint32_t getId() const override { return _id; }

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::PollReadable && event.poll.fd == _ptyMaster) {
            readPty();
            return Ok(true);
        }
        return Ok(false);
    }

private:
    void readPty() {
        // Check if child exited
        int status;
        if (waitpid(_childPid, &status, WNOHANG) > 0) {
            _running = false;
            if (_exitCallback) {
                _exitCallback(WEXITSTATUS(status));
            }
            return;
        }

        static constexpr size_t READ_CHUNK = 65536;
        std::vector<char> buffer(READ_CHUNK);

        ssize_t n = read(_ptyMaster, buffer.data(), READ_CHUNK);
        if (n > 0 && _dataCallback) {
            _dataCallback(buffer.data(), static_cast<size_t>(n));
        }
    }

    uint32_t _id;
    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
    uint32_t _cols = 80;
    uint32_t _rows = 25;
    std::string _shell;
    bool _running = false;
    DataCallback _dataCallback;
    ExitCallback _exitCallback;
};

class UnixPtyManager : public PtyManager {
public:
    Result<void> init() {
        return Ok();
    }

    Result<std::shared_ptr<PTYProvider>> createPTY() override {
        return Ok(std::static_pointer_cast<PTYProvider>(std::make_shared<ForkPTY>()));
    }
};

// Factory
Result<PtyManager::Ptr> PtyManager::createImpl() {
    auto impl = Ptr(new UnixPtyManager());
    auto initResult = static_cast<UnixPtyManager*>(impl.get())->init();
    if (!initResult) {
        return Err<Ptr>("UnixPtyManager init failed", initResult);
    }
    return Ok(std::move(impl));
}

} // namespace yetty

#endif // !__ANDROID__ && !__EMSCRIPTEN__ && !_WIN32
