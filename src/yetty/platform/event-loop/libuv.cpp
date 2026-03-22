#include <yetty/platform/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <uv.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <atomic>

namespace yetty {
namespace base {

struct EventTypeHash {
    std::size_t operator()(Event::Type t) const noexcept {
        return static_cast<std::size_t>(t);
    }
};

struct PollHandle {
    uv_poll_t poll;
    int fd = -1;
    int events = UV_READABLE;
    std::vector<std::weak_ptr<EventListener>> listeners;
};

struct TimerHandle {
    uv_timer_t timer;
    int id = -1;
    Timeout timeout = 0;
    std::vector<std::weak_ptr<EventListener>> listeners;
};

class EventLoopImpl : public EventLoop {
public:
    EventLoopImpl() {
        _loop = uv_default_loop();

        // Initialize screen update async handle
        _screenUpdateAsync.data = this;
        uv_async_init(_loop, &_screenUpdateAsync, onScreenUpdateAsync);

        // Initialize signal handlers for graceful shutdown
        _sigintHandle.data = this;
        _sigtermHandle.data = this;
        uv_signal_init(_loop, &_sigintHandle);
        uv_signal_init(_loop, &_sigtermHandle);
        uv_signal_start(&_sigintHandle, onSignal, SIGINT);
        uv_signal_start(&_sigtermHandle, onSignal, SIGTERM);
    }

    ~EventLoopImpl() override {
        uv_signal_stop(&_sigintHandle);
        uv_signal_stop(&_sigtermHandle);
        uv_close(reinterpret_cast<uv_handle_t*>(&_sigintHandle), nullptr);
        uv_close(reinterpret_cast<uv_handle_t*>(&_sigtermHandle), nullptr);
        uv_close(reinterpret_cast<uv_handle_t*>(&_screenUpdateAsync), nullptr);
    }

    static void onSignal(uv_signal_t* handle, int signum) {
        ydebug("EventLoop: received signal {} - stopping", signum);
        auto* self = static_cast<EventLoopImpl*>(handle->data);
        uv_stop(self->_loop);
    }

    int start() override {
        ydebug("EventLoop::start: running uv_default_loop");
        return uv_run(_loop, UV_RUN_DEFAULT);
    }

    Result<void> stop() override {
        ydebug("EventLoop::stop");
        uv_stop(_loop);
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

        auto listeners = it->second;  // copy for safe iteration
        for (const auto& pl : listeners) {
            if (auto sp = pl.listener.lock()) {
                auto result = sp->onEvent(event);
                if (!result) {
                    return Err<bool>("Event handler failed", result);
                }
                if (*result) {
                    return Ok(true);  // consumed
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
        PollId id = _nextPollId++;
        _polls[id] = std::make_unique<PollHandle>();
        ydebug("EventLoop::createPoll: id={}", id);
        return Ok(id);
    }

    Result<void> configPoll(PollId id, int fd) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        auto& ph = it->second;
        ph->fd = fd;
#ifdef _WIN32
        int r = uv_poll_init_socket(_loop, &ph->poll, fd);
#else
        int r = uv_poll_init(_loop, &ph->poll, fd);
#endif
        if (r != 0) {
            yerror("EventLoop::configPoll: uv_poll_init failed for fd={}: {}", fd, uv_strerror(r));
            return Err<void>(std::string("uv_poll_init failed: ") + uv_strerror(r));
        }
        ph->poll.data = ph.get();
        ydebug("EventLoop::configPoll: id={} fd={} success", id, fd);
        return Ok();
    }

    Result<void> startPoll(PollId id, int events = POLL_READABLE) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        int uvEvents = 0;
        if (events & POLL_READABLE) uvEvents |= UV_READABLE;
        if (events & POLL_WRITABLE) uvEvents |= UV_WRITABLE;
        it->second->events = uvEvents;

        ydebug("EventLoop::startPoll: id={} fd={} events={}", id, it->second->fd, uvEvents);
        int r = uv_poll_start(&it->second->poll, uvEvents, onPollCallback);
        if (r != 0) {
            yerror("EventLoop::startPoll: uv_poll_start failed for fd={}: {}", it->second->fd, uv_strerror(r));
            return Err<void>(std::string("uv_poll_start failed: ") + uv_strerror(r));
        }
        return Ok();
    }

    Result<void> setPollEvents(PollId id, int events) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        int uvEvents = 0;
        if (events & POLL_READABLE) uvEvents |= UV_READABLE;
        if (events & POLL_WRITABLE) uvEvents |= UV_WRITABLE;

        if (it->second->events == uvEvents) {
            return Ok();
        }

        it->second->events = uvEvents;
        ydebug("EventLoop::setPollEvents: id={} fd={} events={}", id, it->second->fd, uvEvents);

        int r = uv_poll_start(&it->second->poll, uvEvents, onPollCallback);
        if (r != 0) {
            yerror("EventLoop::setPollEvents: uv_poll_start failed for fd={}: {}", it->second->fd, uv_strerror(r));
            return Err<void>(std::string("uv_poll_start failed: ") + uv_strerror(r));
        }
        return Ok();
    }

    Result<void> stopPoll(PollId id) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        uv_poll_stop(&it->second->poll);
        return Ok();
    }

    Result<void> destroyPoll(PollId id) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        uv_poll_stop(&it->second->poll);

        // Move to pending close set - libuv needs handle alive until close callback
        auto handle = std::move(it->second);
        _polls.erase(it);

        auto* rawPtr = handle.release();
        _pendingPollClose.insert(rawPtr);

        uv_close(reinterpret_cast<uv_handle_t*>(&rawPtr->poll), onPollCloseCallback);
        return Ok();
    }

    static void onPollCloseCallback(uv_handle_t* handle) {
        auto* ph = reinterpret_cast<PollHandle*>(handle);
        // Get EventLoopImpl from singleton to remove from pending set
        if (auto loopResult = EventLoop::instance(); loopResult) {
            auto* impl = static_cast<EventLoopImpl*>(loopResult->get());
            impl->_pendingPollClose.erase(ph);
        }
        delete ph;
    }

    Result<void> registerPollListener(PollId id, EventListener::Ptr listener) override {
        auto it = _polls.find(id);
        if (it == _polls.end()) {
            return Err<void>("Poll not found");
        }

        ydebug("EventLoop::registerPollListener: id={} fd={}", id, it->second->fd);
        it->second->listeners.push_back(listener);
        return Ok();
    }

    Result<TimerId> createTimer() override {
        TimerId id = _nextTimerId++;
        auto th = std::make_unique<TimerHandle>();
        th->id = id;
        uv_timer_init(_loop, &th->timer);
        th->timer.data = th.get();
        _timers[id] = std::move(th);
        ydebug("EventLoop::createTimer: id={}", id);
        return Ok(id);
    }

    Result<void> configTimer(TimerId id, Timeout timeoutMs) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& th = it->second;
        th->timeout = timeoutMs;
        if (uv_is_active(reinterpret_cast<uv_handle_t*>(&th->timer))) {
            uv_timer_start(&th->timer, onTimerCallback, timeoutMs, timeoutMs);
        }
        ydebug("EventLoop::configTimer: id={} timeout={}", id, timeoutMs);
        return Ok();
    }

    Result<void> startTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        auto& th = it->second;
        ydebug("EventLoop::startTimer: id={} timeout={}", id, th->timeout);
        uv_timer_start(&th->timer, onTimerCallback, th->timeout, th->timeout);
        return Ok();
    }

    Result<void> stopTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        uv_timer_stop(&it->second->timer);
        return Ok();
    }

    Result<void> destroyTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        uv_timer_stop(&it->second->timer);
        uv_close(reinterpret_cast<uv_handle_t*>(&it->second->timer), nullptr);
        _timers.erase(it);
        return Ok();
    }

    Result<void> registerTimerListener(TimerId id, EventListener::Ptr listener) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }

        ydebug("EventLoop::registerTimerListener: id={}", id);
        it->second->listeners.push_back(listener);
        return Ok();
    }

    // Request immediate screen update - async dispatch of ScreenUpdate event
    void requestScreenUpdate() override {
        _screenUpdatePending = true;
        uv_async_send(&_screenUpdateAsync);
    }

private:
    static void onScreenUpdateAsync(uv_async_t* handle) {
        auto* self = static_cast<EventLoopImpl*>(handle->data);
        if (self->_screenUpdatePending.exchange(false)) {
            self->dispatch(Event::screenUpdateEvent());
        }
    }

    static void onPollCallback(uv_poll_t* handle, int status, int events) {
        auto* ph = static_cast<PollHandle*>(handle->data);

        if (status < 0) {
            ywarn("EventLoop::onPollCallback: error status={} for fd={}", status, ph->fd);
            return;
        }

        if (events & UV_READABLE) {
            Event event;
            event.type = Event::Type::PollReadable;
            event.poll.fd = ph->fd;

            for (const auto& wp : ph->listeners) {
                if (auto sp = wp.lock()) {
                    sp->onEvent(event);
                }
            }
        }

        if (events & UV_WRITABLE) {
            Event event;
            event.type = Event::Type::PollWritable;
            event.poll.fd = ph->fd;

            for (const auto& wp : ph->listeners) {
                if (auto sp = wp.lock()) {
                    sp->onEvent(event);
                }
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

    struct PrioritizedListener {
        std::weak_ptr<EventListener> listener;
        int priority;
    };

    std::unordered_map<Event::Type, std::vector<PrioritizedListener>, EventTypeHash> _listeners;

    uv_loop_t* _loop = nullptr;
    std::unordered_map<PollId, std::unique_ptr<PollHandle>> _polls;
    std::unordered_map<TimerId, std::unique_ptr<TimerHandle>> _timers;
    std::unordered_set<PollHandle*> _pendingPollClose;  // Handles awaiting async close
    PollId _nextPollId = 1;
    TimerId _nextTimerId = 1;

    // Screen update async - for immediate re-render without waiting for timer
    uv_async_t _screenUpdateAsync;
    std::atomic<bool> _screenUpdatePending{false};

    // Signal handlers for graceful shutdown
    uv_signal_t _sigintHandle;
    uv_signal_t _sigtermHandle;
};

// Factory implementation
Result<EventLoop::Ptr> EventLoop::createImpl() noexcept {
    return Ok(Ptr(new EventLoopImpl()));
}

} // namespace base
} // namespace yetty
