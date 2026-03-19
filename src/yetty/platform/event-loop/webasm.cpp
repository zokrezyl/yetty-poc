#include <yetty/platform/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <emscripten/emscripten.h>
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

class EventLoopImpl : public EventLoop {
public:
    EventLoopImpl() = default;
    ~EventLoopImpl() override = default;

    int start() override {
        ydebug("EventLoop::start (webasm) - setting up emscripten main loop");
        _running = true;
        // Set up browser main loop - runs at ~60fps via requestAnimationFrame
        // The callback fires timer events for registered timers
        emscripten_set_main_loop_arg(mainLoopCallback, this, 0, true);
        // Note: emscripten_set_main_loop with simulateInfiniteLoop=true doesn't return
        return 0;
    }

    Result<void> stop() override {
        ydebug("EventLoop::stop (webasm)");
        _running = false;
        emscripten_cancel_main_loop();
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

    // Poll - not used on webasm (JS handles I/O)
    Result<PollId> createPoll() override {
        return Err<PollId>("Poll not used on webasm");
    }
    Result<void> configPoll(PollId, int) override {
        return Err<void>("Poll not used on webasm");
    }
    Result<void> startPoll(PollId, int) override {
        return Err<void>("Poll not used on webasm");
    }
    Result<void> setPollEvents(PollId, int) override {
        return Err<void>("Poll not used on webasm");
    }
    Result<void> stopPoll(PollId) override {
        return Err<void>("Poll not used on webasm");
    }
    Result<void> destroyPoll(PollId) override {
        return Err<void>("Poll not used on webasm");
    }
    Result<void> registerPollListener(PollId, EventListener::Ptr) override {
        return Err<void>("Poll not used on webasm");
    }

    // Timer
    Result<TimerId> createTimer() override {
        TimerId id = _nextTimerId++;
        _timers[id] = TimerData{};
        ydebug("EventLoop::createTimer (webasm): id={}", id);
        return Ok(id);
    }

    Result<void> configTimer(TimerId id, Timeout timeoutMs) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.timeout = timeoutMs;
        ydebug("EventLoop::configTimer (webasm): id={} timeout={}", id, timeoutMs);
        return Ok();
    }

    Result<void> startTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.running = true;
        it->second.lastFire = emscripten_get_now();
        ydebug("EventLoop::startTimer (webasm): id={}", id);
        return Ok();
    }

    Result<void> stopTimer(TimerId id) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.running = false;
        ydebug("EventLoop::stopTimer (webasm): id={}", id);
        return Ok();
    }

    Result<void> destroyTimer(TimerId id) override {
        _timers.erase(id);
        ydebug("EventLoop::destroyTimer (webasm): id={}", id);
        return Ok();
    }

    Result<void> registerTimerListener(TimerId id, EventListener::Ptr listener) override {
        auto it = _timers.find(id);
        if (it == _timers.end()) {
            return Err<void>("Timer not found");
        }
        it->second.listeners.push_back(listener);
        ydebug("EventLoop::registerTimerListener (webasm): id={}", id);
        return Ok();
    }

    void requestScreenUpdate() override {
        // No-op for WebASM - already running at ~60fps via requestAnimationFrame
        // Screen updates happen every frame anyway
    }

private:
    static void mainLoopCallback(void* arg) {
        auto* self = static_cast<EventLoopImpl*>(arg);
        self->tick();
    }

    void tick() {
        if (!_running) return;

        double now = emscripten_get_now();

        // Fire timers that are due
        for (auto& [id, td] : _timers) {
            if (!td.running) continue;

            double elapsed = now - td.lastFire;
            if (elapsed >= td.timeout) {
                td.lastFire = now;

                Event event = Event::timerEvent(id);
                for (const auto& wp : td.listeners) {
                    if (auto sp = wp.lock()) {
                        sp->onEvent(event);
                    }
                }
            }
        }
    }

    struct PrioritizedListener {
        std::weak_ptr<EventListener> listener;
        int priority;
    };

    struct TimerData {
        Timeout timeout = 0;
        bool running = false;
        double lastFire = 0;
        std::vector<std::weak_ptr<EventListener>> listeners;
    };

    std::unordered_map<Event::Type, std::vector<PrioritizedListener>, EventTypeHash> _listeners;
    std::unordered_map<TimerId, TimerData> _timers;
    TimerId _nextTimerId = 1;
    bool _running = false;
};

// Factory
Result<EventLoop::Ptr> EventLoop::createImpl() noexcept {
    return Ok(Ptr(new EventLoopImpl()));
}

} // namespace base
} // namespace yetty
