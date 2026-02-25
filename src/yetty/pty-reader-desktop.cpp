#include <yetty/pty-reader.h>

#if !YETTY_WEB && !defined(__ANDROID__) && !defined(_WIN32)

#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

namespace yetty {

class PtyReaderDesktop : public PtyReader,
                          public base::EventListener {
public:
    PtyReaderDesktop() = default;

    ~PtyReaderDesktop() override {
        stop();
    }

    Result<void> init(const PtyConfig& config) {
        _shell = config.shell;
        _cols = config.cols;
        _rows = config.rows;

        struct winsize ws = {
            static_cast<unsigned short>(_rows),
            static_cast<unsigned short>(_cols),
            0, 0
        };

        _childPid = forkpty(&_ptyMaster, nullptr, nullptr, &ws);

        if (_childPid < 0) {
            return Err<void>(std::string("forkpty failed: ") + strerror(errno));
        }

        if (_childPid == 0) {
            // Child process
            for (int fd = 3; fd < 1024; fd++) {
                close(fd);
            }
            execl(_shell.c_str(), _shell.c_str(), nullptr);
            _exit(1);
        }

        // Parent process - set non-blocking
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
        yinfo("PtyReaderDesktop: Started PTY fd={}, PID={}, shell={}",
              _ptyMaster, _childPid, _shell);
        return Ok();
    }

    // PtyReader interface

    size_t read(char* buf, size_t maxLen) override {
        if (_ptyMaster < 0) return 0;

        ssize_t n = ::read(_ptyMaster, buf, maxLen);
        if (n > 0) {
            return static_cast<size_t>(n);
        }
        return 0;
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

    bool isRunning() const override {
        return _running;
    }

    void stop() override {
        if (!_running) return;
        _running = false;

        yinfo("PtyReaderDesktop: Stopping");

        if (_pollId >= 0) {
            if (auto loopResult = base::EventLoop::instance(); loopResult) {
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

    void setDataAvailableCallback(DataAvailableCallback cb) override {
        _dataAvailableCallback = std::move(cb);
    }

    void setExitCallback(ExitCallback cb) override {
        _exitCallback = std::move(cb);
    }

    // EventListener interface - triggered by poll when PTY has data

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::PollReadable &&
            event.poll.fd == _ptyMaster) {

            // Check if child exited
            int status;
            if (waitpid(_childPid, &status, WNOHANG) > 0) {
                _running = false;
                if (_exitCallback) {
                    _exitCallback(WEXITSTATUS(status));
                }
                return Ok(true);
            }

            // Notify terminal that data is available
            if (_dataAvailableCallback) {
                _dataAvailableCallback();
            }
            return Ok(true);
        }
        return Ok(false);
    }

private:
    int _ptyMaster = -1;
    pid_t _childPid = -1;
    base::PollId _pollId = -1;
    uint32_t _cols = 80;
    uint32_t _rows = 24;
    std::string _shell;
    bool _running = false;
    DataAvailableCallback _dataAvailableCallback;
    ExitCallback _exitCallback;
};

// Factory implementation for desktop
Result<PtyReader::Ptr> PtyReader::create(const PtyConfig& config) {
    auto reader = std::make_shared<PtyReaderDesktop>();
    if (auto res = reader->init(config); !res) {
        return Err<Ptr>("Failed to initialize PtyReaderDesktop", res);
    }
    return Ok<Ptr>(reader);
}

} // namespace yetty

#endif // !YETTY_WEB && !__ANDROID__ && !_WIN32
