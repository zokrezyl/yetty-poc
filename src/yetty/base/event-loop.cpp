#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <vector>
#include <unordered_map>
#include <algorithm>

#if !YETTY_WEB && !defined(__ANDROID__)
#include <uv.h>
#endif

namespace yetty {
namespace base {

struct EventTypeHash {
    std::size_t operator()(Event::Type t) const noexcept {
        return static_cast<std::size_t>(t);
    }
};

#if !YETTY_WEB && !defined(__ANDROID__)
struct PollHandle {
    uv_poll_t poll;
    int fd = -1;
    std::vector<std::weak_ptr<EventListener>> listeners;
};

struct TimerHandle {
    uv_timer_t timer;
    int id = -1;
    Timeout timeout = 0;
    std::vector<std::weak_ptr<EventListener>> listeners;
};
#endif

class EventLoopImpl : public EventLoop {
public:
    EventLoopImpl() {
#if !YETTY_WEB && !defined(__ANDROID__)
        _loop = uv_default_loop();
#endif
    }

    ~EventLoopImpl() override = default;

    int start() override {
#if !YETTY_WEB && !defined(__ANDROID__)
        yinfo("EventLoop::start: running uv_default_loop");
        return uv_run(_loop, UV_RUN_DEFAULT);
#else
        return 0;
#endif
    }

    Result<void> stop() override {
#if !YETTY_WEB && !defined(__ANDROID__)
        yinfo("EventLoop::stop");
        uv_stop(_loop);
#endif
        return Ok();
    }

    Result<void> registerListener(Event::Type type, EventListener::Ptr listener, int priority = 0) override {
        auto& vec = _listeners[type];
        // Insert sorted by priority (descending - higher priority first)
        PrioritizedListener entry{listener, priority};
        auto insertPos = std::lower_bound(vec.begin(), vec.end(), entry,
            [](const PrioritizedListener& a, const PrioritizedListener& b) {
                return a.priority > b.priority;  // descending
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
        if (it == _listeners.end()) return Ok(false);

        auto listeners = it->second;  // copy for safe iteration
        for (const auto& pl : listeners) {
            if (auto sp = pl.listener.lock()) {
                auto result = sp->onEvent(event);
                if (!result) {
                    return Err<bool>("Event handler failed", result);
                }
                if (*result) {
                    return Ok(true);  // consumed by higher priority listener
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

    Result<PollId> createPoll() override {
#if !YETTY_WEB && !defined(__ANDROID__)
        PollId id = _nextPollId++;
        _polls[id] = std::make_unique<PollHandle>();
        return Ok(id);
#else
        return Err<PollId>("Poll not supported on this platform");
#endif
    }

    Result<void> configPoll(PollId id, int fd) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        auto& ph = it->second;
        ph->fd = fd;
        uv_poll_init(_loop, &ph->poll, fd);
        ph->poll.data = ph.get();
        return Ok();
#else
        (void)id;
        (void)fd;
        return Err<void>("Poll not supported on this platform");
#endif
    }

    Result<void> startPoll(PollId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        ydebug("EventLoop::startPoll: id={} fd={}", id, it->second->fd);
        uv_poll_start(&it->second->poll, UV_READABLE, onPollCallback);
        return Ok();
#else
        (void)id;
        return Err<void>("Poll not supported on this platform");
#endif
    }

    Result<void> stopPoll(PollId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        uv_poll_stop(&it->second->poll);
        return Ok();
#else
        (void)id;
        return Err<void>("Poll not supported on this platform");
#endif
    }

    Result<void> destroyPoll(PollId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        uv_poll_stop(&it->second->poll);
        uv_close(reinterpret_cast<uv_handle_t*>(&it->second->poll), nullptr);
        _polls.erase(it);
        return Ok();
#else
        (void)id;
        return Err<void>("Poll not supported on this platform");
#endif
    }

    Result<void> registerPollListener(PollId id, EventListener::Ptr listener) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        ydebug("EventLoop::registerPollListener: id={} fd={} listener={}", id, it->second->fd, (void*)listener.get());
        it->second->listeners.push_back(listener);
        return Ok();
#else
        (void)id;
        (void)listener;
        return Err<void>("Poll not supported on this platform");
#endif
    }

    Result<TimerId> createTimer() override {
#if !YETTY_WEB && !defined(__ANDROID__)
        TimerId id = _nextTimerId++;
        auto th = std::make_unique<TimerHandle>();
        th->id = id;
        uv_timer_init(_loop, &th->timer);
        th->timer.data = th.get();
        _timers[id] = std::move(th);
        ydebug("EventLoop::createTimer: id={}", id);
        return Ok(id);
#else
        return Err<TimerId>("Timer not supported on this platform");
#endif
    }

    Result<void> configTimer(TimerId id, Timeout timeoutMs) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& th = it->second;
        th->timeout = timeoutMs;
        // Restart timer if it's already running
        if (uv_is_active(reinterpret_cast<uv_handle_t*>(&th->timer))) {
            uv_timer_start(&th->timer, onTimerCallback, timeoutMs, timeoutMs);
        }
        ydebug("EventLoop::configTimer: id={} timeout={}", id, timeoutMs);
        return Ok();
#else
        (void)id;
        (void)timeoutMs;
        return Err<void>("Timer not supported on this platform");
#endif
    }

    Result<void> startTimer(TimerId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& th = it->second;
        ydebug("EventLoop::startTimer: id={} timeout={}", id, th->timeout);
        uv_timer_start(&th->timer, onTimerCallback, th->timeout, th->timeout);
        return Ok();
#else
        (void)id;
        return Err<void>("Timer not supported on this platform");
#endif
    }

    Result<void> stopTimer(TimerId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        uv_timer_stop(&it->second->timer);
        return Ok();
#else
        (void)id;
        return Err<void>("Timer not supported on this platform");
#endif
    }

    Result<void> destroyTimer(TimerId id) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        uv_timer_stop(&it->second->timer);
        uv_close(reinterpret_cast<uv_handle_t*>(&it->second->timer), nullptr);
        _timers.erase(it);
        return Ok();
#else
        (void)id;
        return Err<void>("Timer not supported on this platform");
#endif
    }

    Result<void> registerTimerListener(TimerId id, EventListener::Ptr listener) override {
#if !YETTY_WEB && !defined(__ANDROID__)
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        ydebug("EventLoop::registerTimerListener: id={} listener={}", id, (void*)listener.get());
        it->second->listeners.push_back(listener);
        return Ok();
#else
        (void)id;
        (void)listener;
        return Err<void>("Timer not supported on this platform");
#endif
    }

private:
#if !YETTY_WEB && !defined(__ANDROID__)
    static void onPollCallback(uv_poll_t* handle, int status, int events) {
        auto* ph = static_cast<PollHandle*>(handle->data);
        ydebug("EventLoop::onPollCallback: fd={} status={} events={}", ph->fd, status, events);
        if (status < 0 || !(events & UV_READABLE)) return;

        Event event;
        event.type = Event::Type::PollReadable;
        event.poll.fd = ph->fd;

        ydebug("EventLoop::onPollCallback: dispatching to {} listeners", ph->listeners.size());
        for (const auto& wp : ph->listeners) {
            if (auto sp = wp.lock()) {
                ydebug("EventLoop::onPollCallback: calling listener onEvent");
                sp->onEvent(event);
            }
        }
    }

    static void onTimerCallback(uv_timer_t* handle) {
        auto* th = static_cast<TimerHandle*>(handle->data);
        ydebug("EventLoop::onTimerCallback: timerId={}", th->id);

        Event event = Event::timerEvent(th->id);

        for (const auto& wp : th->listeners) {
            if (auto sp = wp.lock()) {
                sp->onEvent(event);
            }
        }
    }
#endif

    struct PrioritizedListener {
        std::weak_ptr<EventListener> listener;
        int priority;
    };

    std::unordered_map<Event::Type, std::vector<PrioritizedListener>, EventTypeHash> _listeners;

#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* _loop = nullptr;
    std::unordered_map<PollId, std::unique_ptr<PollHandle>> _polls;
    std::unordered_map<TimerId, std::unique_ptr<TimerHandle>> _timers;
    PollId _nextPollId = 1;
    TimerId _nextTimerId = 1;
#endif
};

// Factory implementation
Result<EventLoop::Ptr> EventLoop::createImpl() noexcept {
    return Ok(Ptr(new EventLoopImpl()));
}

} // namespace base
} // namespace yetty
