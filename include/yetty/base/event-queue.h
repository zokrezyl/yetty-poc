#pragma once

#include "factory.h"
#include "event.h"

namespace yetty {
namespace base {

// EventQueue - thread-safe queue for cross-thread event delivery
//
// Use this when posting events from non-main threads (e.g., GPU callbacks).
// Main thread code should continue using EventLoop::instance()->dispatch() directly.
//
// Usage from GPU thread:
//   EventQueue::instance()->push(Event::screenUpdateEvent());
//
// The event will be delivered on the main thread via EventLoop::instance()->dispatch().
//
class EventQueue : public Singleton<EventQueue> {
public:
    using Ptr = std::shared_ptr<EventQueue>;

    static Result<Ptr> createImpl() noexcept;

    virtual ~EventQueue() = default;

    // Push event to queue (thread-safe, can be called from any thread)
    // Wakes up main thread to process the event
    virtual void push(const Event& event) = 0;

    // Drain queued events (web-specific, called from main loop)
    // On native builds with libuv, this is a no-op (events are drained via async callback)
    virtual void drain() {}

protected:
    EventQueue() = default;
};

} // namespace base
} // namespace yetty
