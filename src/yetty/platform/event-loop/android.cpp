#include <yetty/platform/event-loop.h>

#if defined(__ANDROID__)

#include <ytrace/ytrace.hpp>
#include <android/looper.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <uv.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace yetty {
namespace base {

struct EventTypeHash {
    std::size_t operator()(Event::Type t) const noexcept {
        return static_cast<std::size_t>(t);
    }
};

struct TimerData {
    int fd = -1;
    int id = -1;
    Timeout timeout = 0;
    bool running = false;
    std::vector<std::weak_ptr<EventListener>> listeners;
};

struct PollData {
    int fd = -1;
    int id = -1;
    int events = 0;
    std::vector<std::weak_ptr<EventListener>> listeners;
};

class AndroidEventLoop : public EventLoop {
public:
    AndroidEventLoop() = default;

    Result<void> init() {
        _looper = ALooper_forThread();
        if (!_looper) {
            return Err<void>("ALooper_forThread returned NULL - looper not prepared");
        }
        ALooper_acquire(_looper);
        ydebug("AndroidEventLoop: acquired looper {:p}", static_cast<void*>(_looper));
        return Ok();
    }

    ~AndroidEventLoop() override {
        for (auto& [id, td] : _timers) {
            if (td.fd >= 0) {
                ALooper_removeFd(_looper, td.fd);
                close(td.fd);
            }
        }
        for (auto& [id, pd] : _polls) {
            if (pd.fd >= 0) {
                ALooper_removeFd(_looper, pd.fd);
            }
        }
        if (_looper) {
            ALooper_release(_looper);
        }
    }

    // On Android, start() doesn't run a loop - InitManager runs ALooper_pollAll
    // Our timers/polls are registered with ALooper callbacks
    int start() override {
        ydebug("AndroidEventLoop::start (no-op, InitManager drives loop)");
        _running = true;
        return 0;
    }

    Result<void> stop() override {
        ydebug("AndroidEventLoop::stop");
        _running = false;
        ALooper_wake(_looper);
        return Ok();
    }

    Result<void> registerListener(Event::Type type, EventListener::Ptr listener, int priority = 0) override {
        auto& vec = _listeners[type];
        PrioritizedListener entry{listener, priority};
        auto insertPos = std::lower_bound(vec.begin(), vec.end(), entry,
            [](const PrioritizedListener& a, const PrioritizedListener& b) {
                return a.priority > b.priority;
            });
        vec.insert(insertPos, entry);
        return Ok();
    }

    Result<void> deregisterListener(Event::Type type, EventListener::Ptr listener) override {
        auto it = _listeners.find(type);
        if (it == _listeners.end()) return Ok();

        auto& vec = it->second;
        vec.erase(
            std::remove_if(vec.begin(), vec.end(),
                [&](const PrioritizedListener& pl) {
                    auto sp = pl.listener.lock();
                    return !sp || sp == listener;
                }),
            vec.end());
        return Ok();
    }

    Result<void> deregisterListener(EventListener::Ptr listener) override {
        for (auto& [type, vec] : _listeners) {
            vec.erase(
                std::remove_if(vec.begin(), vec.end(),
                    [&](const PrioritizedListener& pl) {
                        auto sp = pl.listener.lock();
                        return !sp || sp == listener;
                    }),
                vec.end());
        }
        return Ok();
    }

    Result<bool> dispatch(const Event& event) override {
        auto it = _listeners.find(event.type);
        if (it == _listeners.end()) {
            return Ok(false);
        }

        auto listeners = it->second;
        for (const auto& pl : listeners) {
            if (auto sp = pl.listener.lock()) {
                auto result = sp->onEvent(event);
                if (!result) {
                    return Err<bool>("Event handler failed", result);
                }
                if (*result) {
                    return Ok(true);
                }
            }
        }
        return Ok(false);
    }

    Result<void> broadcast(const Event& event) override {
        auto listenersCopy = _listeners;
        for (auto& [type, vec] : listenersCopy) {
            for (const auto& pl : vec) {
                if (auto sp = pl.listener.lock()) {
                    auto result = sp->onEvent(event);
                    if (!result) {
                        return Err<void>("Broadcast handler failed", result);
                    }
                }
            }
        }
        return Ok();
    }

    // Poll
    Result<PollId> createPoll() override {
        PollId id = _nextPollId++;
        _polls[id] = PollData{};
        _polls[id].id = id;
        return Ok(id);
    }

    Result<void> configPoll(PollId id, int fd) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }
        it->second.fd = fd;
        return Ok();
    }

    Result<void> startPoll(PollId id, int events) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        auto& pd = it->second;
        int looperEvents = 0;
        if (events & POLL_READABLE) looperEvents |= ALOOPER_EVENT_INPUT;
        if (events & POLL_WRITABLE) looperEvents |= ALOOPER_EVENT_OUTPUT;
        pd.events = looperEvents;

        int result = ALooper_addFd(_looper, pd.fd, 0, looperEvents, onPollCallback, &pd);
        if (result < 0) {
            return Err<void>("ALooper_addFd failed");
        }
        return Ok();
    }

    Result<void> setPollEvents(PollId id, int events) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        auto& pd = it->second;
        int looperEvents = 0;
        if (events & POLL_READABLE) looperEvents |= ALOOPER_EVENT_INPUT;
        if (events & POLL_WRITABLE) looperEvents |= ALOOPER_EVENT_OUTPUT;

        if (pd.events == looperEvents) {
            return Ok();
        }

        pd.events = looperEvents;
        ALooper_addFd(_looper, pd.fd, 0, looperEvents, onPollCallback, &pd);
        return Ok();
    }

    Result<void> stopPoll(PollId id) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }
        ALooper_removeFd(_looper, it->second.fd);
        return Ok();
    }

    Result<void> destroyPoll(PollId id) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }
        ALooper_removeFd(_looper, it->second.fd);
        _polls.erase(it);
        return Ok();
    }

    Result<void> registerPollListener(PollId id, EventListener::Ptr listener) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }
        it->second.listeners.push_back(listener);
        return Ok();
    }

    // Timer via timerfd
    Result<TimerId> createTimer() override {
        TimerId id = _nextTimerId++;

        int fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        if (fd < 0) {
            return Err<TimerId>("timerfd_create failed");
        }

        TimerData td;
        td.fd = fd;
        td.id = id;
        _timers[id] = td;

        ydebug("createTimer id={} fd={}", id, fd);
        return Ok(id);
    }

    Result<void> configTimer(TimerId id, Timeout timeoutMs) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.timeout = timeoutMs;

        if (it->second.running) {
            setTimerfd(it->second.fd, timeoutMs);
        }

        ydebug("configTimer id={} timeout={}", id, timeoutMs);
        return Ok();
    }

    Result<void> startTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& td = it->second;
        setTimerfd(td.fd, td.timeout);

        // Register with ALooper using callback
        int result = ALooper_addFd(_looper, td.fd, 0, ALOOPER_EVENT_INPUT, onTimerCallback, &td);
        if (result < 0) {
            return Err<void>("ALooper_addFd failed for timer");
        }

        td.running = true;
        ydebug("startTimer id={} timeout={}", id, td.timeout);
        return Ok();
    }

    Result<void> stopTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& td = it->second;

        // Disarm timerfd
        struct itimerspec ts = {};
        timerfd_settime(td.fd, 0, &ts, nullptr);

        ALooper_removeFd(_looper, td.fd);
        td.running = false;
        return Ok();
    }

    Result<void> destroyTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        if (it->second.running) {
            ALooper_removeFd(_looper, it->second.fd);
        }
        close(it->second.fd);
        _timers.erase(it);
        return Ok();
    }

    Result<void> registerTimerListener(TimerId id, EventListener::Ptr listener) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.listeners.push_back(listener);
        return Ok();
    }

private:
    static void setTimerfd(int fd, Timeout timeoutMs) {
        struct itimerspec ts;
        ts.it_value.tv_sec = timeoutMs / 1000;
        ts.it_value.tv_nsec = (timeoutMs % 1000) * 1000000;
        ts.it_interval = ts.it_value;  // Repeating
        timerfd_settime(fd, 0, &ts, nullptr);
    }

    // ALooper callback for timer - called when timerfd is readable
    static int onTimerCallback(int fd, int events, void* data) {
        (void)events;
        auto* td = static_cast<TimerData*>(data);
        ydebug("onTimerCallback: fd={} id={} listeners={}", fd, td->id, td->listeners.size());

        // Read to clear the timerfd
        uint64_t expirations;
        read(fd, &expirations, sizeof(expirations));

        // Dispatch timer event to listeners, remove expired ones
        Event event = Event::timerEvent(td->id);
        td->listeners.erase(
            std::remove_if(td->listeners.begin(), td->listeners.end(),
                [&event](const std::weak_ptr<EventListener>& wp) {
                    auto sp = wp.lock();
                    if (!sp) return true;  // Remove expired

                    auto result = sp->onEvent(event);
                    if (!result) {
                        yerror("Timer callback failed: {}", result.error().message());
                    }
                    return false;
                }),
            td->listeners.end());

        // Process EventQueue (cross-thread events)
        uv_run(uv_default_loop(), UV_RUN_NOWAIT);

        return 1;  // Continue receiving events
    }

    // ALooper callback for poll - called when fd is readable/writable
    static int onPollCallback(int fd, int events, void* data) {
        auto* pd = static_cast<PollData*>(data);

        // Helper to dispatch and cleanup expired listeners
        auto dispatchAndCleanup = [pd](Event::Type type, int fd) {
            Event event;
            event.type = type;
            event.poll.fd = fd;
            pd->listeners.erase(
                std::remove_if(pd->listeners.begin(), pd->listeners.end(),
                    [&event](const std::weak_ptr<EventListener>& wp) {
                        auto sp = wp.lock();
                        if (!sp) return true;  // Remove expired

                        auto result = sp->onEvent(event);
                        if (!result) {
                            yerror("Poll callback failed: {}", result.error().message());
                        }
                        return false;
                    }),
                pd->listeners.end());
        };

        if (events & ALOOPER_EVENT_INPUT) {
            dispatchAndCleanup(Event::Type::PollReadable, fd);
        }

        if (events & ALOOPER_EVENT_OUTPUT) {
            dispatchAndCleanup(Event::Type::PollWritable, fd);
        }

        // Process EventQueue (cross-thread events)
        uv_run(uv_default_loop(), UV_RUN_NOWAIT);

        return 1;  // Continue receiving events
    }

    struct PrioritizedListener {
        std::weak_ptr<EventListener> listener;
        int priority;
    };

    ALooper* _looper = nullptr;
    bool _running = false;

    std::unordered_map<Event::Type, std::vector<PrioritizedListener>, EventTypeHash> _listeners;
    std::unordered_map<TimerId, TimerData> _timers;
    std::unordered_map<PollId, PollData> _polls;
    TimerId _nextTimerId = 1;
    PollId _nextPollId = 1;
};

// Factory
Result<EventLoop::Ptr> EventLoop::createImpl() noexcept {
    auto impl = new AndroidEventLoop();
    if (auto res = impl->init(); !res) {
        delete impl;
        return Err<Ptr>("AndroidEventLoop init failed", res);
    }
    return Ok(Ptr(impl));
}

} // namespace base
} // namespace yetty

#endif // __ANDROID__
