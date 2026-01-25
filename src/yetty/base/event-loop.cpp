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

struct EventLoop::Impl {
    std::unordered_map<Event::Type, std::vector<std::weak_ptr<EventListener>>, EventTypeHash> listeners;

#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* loop = nullptr;
    std::unordered_map<PollId, std::unique_ptr<PollHandle>> polls;
    std::unordered_map<TimerId, std::unique_ptr<TimerHandle>> timers;
    PollId nextPollId = 1;
    TimerId nextTimerId = 1;
#endif
};

EventLoop::EventLoop() : _impl(std::make_unique<Impl>()) {
#if !YETTY_WEB && !defined(__ANDROID__)
    _impl->loop = uv_default_loop();
#endif
}

EventLoop::~EventLoop() = default;

int EventLoop::start() {
#if !YETTY_WEB && !defined(__ANDROID__)
    yinfo("EventLoop::start: running uv_default_loop");
    return uv_run(_impl->loop, UV_RUN_DEFAULT);
#else
    return 0;
#endif
}

void EventLoop::stop() {
#if !YETTY_WEB && !defined(__ANDROID__)
    yinfo("EventLoop::stop");
    uv_stop(_impl->loop);
#endif
}

void EventLoop::registerListener(Event::Type type, EventListener::Ptr listener) {
    _impl->listeners[type].push_back(listener);
}

void EventLoop::deregisterListener(Event::Type type, EventListener::Ptr listener) {
    auto it = _impl->listeners.find(type);
    if (it == _impl->listeners.end()) return;

    auto& vec = it->second;
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
            [&](const std::weak_ptr<EventListener>& wp) {
                auto sp = wp.lock();
                return !sp || sp == listener;
            }),
        vec.end());
}

void EventLoop::deregisterListener(EventListener::Ptr listener) {
    for (auto& [type, vec] : _impl->listeners) {
        vec.erase(
            std::remove_if(vec.begin(), vec.end(),
                [&](const std::weak_ptr<EventListener>& wp) {
                    auto sp = wp.lock();
                    return !sp || sp == listener;
                }),
            vec.end());
    }
}

Result<bool> EventLoop::dispatch(const Event& event) {
    auto it = _impl->listeners.find(event.type);
    if (it == _impl->listeners.end()) return Ok(false);

    auto listeners = it->second;
    for (const auto& wp : listeners) {
        if (auto sp = wp.lock()) {
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

Result<void> EventLoop::broadcast(const Event& event) {
    auto listenersCopy = _impl->listeners;
    for (auto& [type, vec] : listenersCopy) {
        for (const auto& wp : vec) {
            if (auto sp = wp.lock()) {
                auto result = sp->onEvent(event);
                if (!result) {
                    return Err<void>("Broadcast handler failed", result);
                }
            }
        }
    }
    return Ok();
}

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

PollId EventLoop::createPoll() {
#if !YETTY_WEB && !defined(__ANDROID__)
    PollId id = _impl->nextPollId++;
    _impl->polls[id] = std::make_unique<PollHandle>();
    return id;
#else
    return -1;
#endif
}

void EventLoop::configPoll(PollId id, int fd) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->polls.find(id);
    if (it == _impl->polls.end()) return;

    auto& ph = it->second;
    ph->fd = fd;
    uv_poll_init(_impl->loop, &ph->poll, fd);
    ph->poll.data = ph.get();
#endif
}

void EventLoop::startPoll(PollId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->polls.find(id);
    if (it == _impl->polls.end()) return;

    ydebug("EventLoop::startPoll: id={} fd={}", id, it->second->fd);
    uv_poll_start(&it->second->poll, UV_READABLE, onPollCallback);
#endif
}

void EventLoop::stopPoll(PollId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->polls.find(id);
    if (it == _impl->polls.end()) return;

    uv_poll_stop(&it->second->poll);
#endif
}

void EventLoop::destroyPoll(PollId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->polls.find(id);
    if (it == _impl->polls.end()) return;

    uv_poll_stop(&it->second->poll);
    uv_close(reinterpret_cast<uv_handle_t*>(&it->second->poll), nullptr);
    _impl->polls.erase(it);
#endif
}

void EventLoop::registerPollListener(PollId id, EventListener::Ptr listener) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->polls.find(id);
    if (it == _impl->polls.end()) return;

    ydebug("EventLoop::registerPollListener: id={} fd={} listener={}", id, it->second->fd, (void*)listener.get());
    it->second->listeners.push_back(listener);
#endif
}

// Timer management
TimerId EventLoop::createTimer() {
#if !YETTY_WEB && !defined(__ANDROID__)
    TimerId id = _impl->nextTimerId++;
    auto th = std::make_unique<TimerHandle>();
    th->id = id;
    uv_timer_init(_impl->loop, &th->timer);
    th->timer.data = th.get();
    _impl->timers[id] = std::move(th);
    ydebug("EventLoop::createTimer: id={}", id);
    return id;
#else
    return -1;
#endif
}

void EventLoop::configTimer(TimerId id, Timeout timeoutMs) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->timers.find(id);
    if (it == _impl->timers.end()) return;

    it->second->timeout = timeoutMs;
    ydebug("EventLoop::configTimer: id={} timeout={}", id, timeoutMs);
#endif
}

void EventLoop::startTimer(TimerId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->timers.find(id);
    if (it == _impl->timers.end()) return;

    auto& th = it->second;
    ydebug("EventLoop::startTimer: id={} timeout={}", id, th->timeout);
    uv_timer_start(&th->timer, onTimerCallback, th->timeout, th->timeout);
#endif
}

void EventLoop::stopTimer(TimerId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->timers.find(id);
    if (it == _impl->timers.end()) return;

    uv_timer_stop(&it->second->timer);
#endif
}

void EventLoop::destroyTimer(TimerId id) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->timers.find(id);
    if (it == _impl->timers.end()) return;

    uv_timer_stop(&it->second->timer);
    uv_close(reinterpret_cast<uv_handle_t*>(&it->second->timer), nullptr);
    _impl->timers.erase(it);
#endif
}

void EventLoop::registerTimerListener(TimerId id, EventListener::Ptr listener) {
#if !YETTY_WEB && !defined(__ANDROID__)
    auto it = _impl->timers.find(id);
    if (it == _impl->timers.end()) return;

    ydebug("EventLoop::registerTimerListener: id={} listener={}", id, (void*)listener.get());
    it->second->listeners.push_back(listener);
#endif
}

} // namespace base
} // namespace yetty
