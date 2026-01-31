#pragma once

#include "factory.h"
#include "event.h"
#include "event-listener.h"

namespace yetty {
namespace base {

using PollId = int;
using TimerId = int;
using Timeout = int;

class EventLoop : public ThreadSingleton<EventLoop> {
public:
    using Ptr = std::shared_ptr<EventLoop>;

    // Factory for ThreadSingleton
    static Result<Ptr> createImpl() noexcept;

    virtual ~EventLoop() = default;

    // Start the event loop (blocking)
    virtual int start() = 0;

    // Stop the event loop
    virtual Result<void> stop() = 0;

    // Event listener registration by type
    // priority: higher value = called first (default 0)
    virtual Result<void> registerListener(Event::Type type, EventListener::Ptr listener, int priority = 0) = 0;
    virtual Result<void> deregisterListener(Event::Type type, EventListener::Ptr listener) = 0;
    virtual Result<void> deregisterListener(EventListener::Ptr listener) = 0;

    virtual Result<bool> dispatch(const Event& event) = 0;
    virtual Result<void> broadcast(const Event& event) = 0;

    // Poll (file descriptor) management
    virtual Result<PollId> createPoll() = 0;
    virtual Result<void> configPoll(PollId id, int fd) = 0;
    virtual Result<void> startPoll(PollId id) = 0;
    virtual Result<void> stopPoll(PollId id) = 0;
    virtual Result<void> destroyPoll(PollId id) = 0;
    virtual Result<void> registerPollListener(PollId id, EventListener::Ptr listener) = 0;

    // Timer management
    virtual Result<TimerId> createTimer() = 0;
    virtual Result<void> configTimer(TimerId id, Timeout timeoutMs) = 0;
    virtual Result<void> startTimer(TimerId id) = 0;
    virtual Result<void> stopTimer(TimerId id) = 0;
    virtual Result<void> destroyTimer(TimerId id) = 0;
    virtual Result<void> registerTimerListener(TimerId id, EventListener::Ptr listener) = 0;

protected:
    EventLoop() = default;
};

} // namespace base
} // namespace yetty
