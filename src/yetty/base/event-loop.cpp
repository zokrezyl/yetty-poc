#include "event-loop.h"
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

struct EventLoop::Impl {
    std::unordered_map<Event::Type, std::vector<std::weak_ptr<EventListener>>, EventTypeHash> listeners;
};

EventLoop::EventLoop() : _impl(std::make_unique<Impl>()) {}
EventLoop::~EventLoop() = default;

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

    // Copy to avoid iterator invalidation if handlers modify listeners
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
    // Copy to avoid iterator invalidation if handlers modify listeners
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

} // namespace base
} // namespace yetty
