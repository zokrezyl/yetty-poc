#include <yetty/base/event-queue.h>
#include <yetty/base/event-loop.h>
#include <queue>
#include <mutex>

#if !YETTY_WEB
#include <uv.h>
#endif

namespace yetty {
namespace base {

#if YETTY_WEB

// Web/WASM implementation - single-threaded, no libuv needed
// Events are queued and dispatched on next frame via requestAnimationFrame
class EventQueueImpl : public EventQueue {
public:
    EventQueueImpl() = default;
    ~EventQueueImpl() override = default;

    Result<void> init() {
        return Ok();
    }

    void push(const Event& event) override {
        _queue.push(event);
        // In web builds, events will be drained by the main loop
        // No async wakeup needed - the browser's requestAnimationFrame drives the loop
    }

    // Called by EventLoop to drain queued events (web-specific)
    void drain() {
        auto loopResult = EventLoop::instance();
        if (!loopResult) {
            return;
        }
        auto loop = *loopResult;

        while (!_queue.empty()) {
            (void)loop->dispatch(_queue.front());
            _queue.pop();
        }
    }

private:
    std::queue<Event> _queue;
};

#else

// Native implementation with libuv for thread-safe cross-thread wakeup
class EventQueueImpl : public EventQueue {
public:
    EventQueueImpl() = default;

    ~EventQueueImpl() override {
        if (_asyncHandle) {
            uv_close(reinterpret_cast<uv_handle_t*>(_asyncHandle), [](uv_handle_t* h) {
                delete reinterpret_cast<uv_async_t*>(h);
            });
            _asyncHandle = nullptr;
        }
    }

    Result<void> init() {
        _asyncHandle = new uv_async_t;
        _asyncHandle->data = this;

        // uv_default_loop() returns the same loop used by EventLoop
        int r = uv_async_init(uv_default_loop(), _asyncHandle, onAsync);
        if (r != 0) {
            delete _asyncHandle;
            _asyncHandle = nullptr;
            return Err<void>("uv_async_init failed: " + std::string(uv_strerror(r)));
        }

        return Ok();
    }

    void push(const Event& event) override {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(event);
        }

        // Wake up main thread - uv_async_send is thread-safe
        if (_asyncHandle) {
            uv_async_send(_asyncHandle);
        }
    }

private:
    static void onAsync(uv_async_t* handle) {
        auto* self = static_cast<EventQueueImpl*>(handle->data);
        self->drain();
    }

    void drain() {
        // Get EventLoop on main thread - correct because drain() runs on main thread
        auto loopResult = EventLoop::instance();
        if (!loopResult) {
            return;
        }
        auto loop = *loopResult;

        // Drain queue
        std::queue<Event> events;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            std::swap(events, _queue);
        }

        // Dispatch all events
        while (!events.empty()) {
            (void)loop->dispatch(events.front());
            events.pop();
        }
    }

    std::mutex _mutex;
    std::queue<Event> _queue;
    uv_async_t* _asyncHandle = nullptr;
};

#endif // YETTY_WEB

Result<EventQueue::Ptr> EventQueue::createImpl() noexcept {
    auto impl = std::make_shared<EventQueueImpl>();
    if (auto res = impl->init(); !res) {
        return Err<Ptr>("EventQueue init failed", res);
    }
    return Ok(Ptr(std::move(impl)));
}

} // namespace base
} // namespace yetty
