#pragma once

#include "factory.h"
#include "event.h"
#include "event-listener.h"

namespace yetty {
namespace base {

// Thread-local singleton event loop
class EventLoop : public ThreadSingleton<EventLoop> {
public:
    ~EventLoop();

    void registerListener(Event::Type type, EventListener::Ptr listener);
    void deregisterListener(Event::Type type, EventListener::Ptr listener);
    void deregisterListener(EventListener::Ptr listener);

    // Dispatch event to registered listeners for that type
    // Returns Ok(true) if consumed, Ok(false) if not, Err on failure
    Result<bool> dispatch(const Event& event);

    // Broadcast event to ALL listeners regardless of type registration
    Result<void> broadcast(const Event& event);

private:
    friend class ThreadSingleton<EventLoop>;
    EventLoop();

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace base
} // namespace yetty
