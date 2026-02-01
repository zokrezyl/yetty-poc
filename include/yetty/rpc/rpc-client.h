#pragma once

#include <yetty/rpc/rpc-message.h>
#include <yetty/base/factory.h>
#include <string>
#include <cstdint>

struct uv_loop_s;
typedef struct uv_loop_s uv_loop_t;

namespace yetty {
namespace rpc {

// RPC client for connecting to the yetty RPC server over Unix domain sockets.
//
// Two modes:
//   - Sync (default): blocking POSIX connect/send/recv. No libuv dependency at runtime.
//     Good for CLI tools, scripts, one-shot commands.
//   - Async: integrates with caller's uv_loop_t* for non-blocking I/O.
//     Good for long-running TUI apps that run their own event loop.
//
// Usage (sync):
//   auto client = RpcClient::create("/tmp/yetty.sock");
//   client->connect();
//   client->keyDown(65, 0);
//
// Usage (async):
//   auto client = RpcClient::create("/tmp/yetty.sock", uv_default_loop());
//   client->connect();  // non-blocking
//   client->keyDown(65, 0);  // queues write
//   // caller runs uv_run()
//
class RpcClient : public base::ObjectFactory<RpcClient> {
public:
    using Ptr = std::shared_ptr<RpcClient>;

    // Sync mode factory
    static Result<Ptr> createImpl(const std::string& socketPath) noexcept;

    // Async mode factory
    static Result<Ptr> createImpl(const std::string& socketPath, uv_loop_t* loop) noexcept;

    virtual ~RpcClient() = default;

    virtual Result<void> connect() = 0;
    virtual void disconnect() = 0;

    // Channel 0: EventLoop methods (typed convenience wrappers)
    virtual Result<void> keyDown(int key, int mods, int scancode = 0) = 0;
    virtual Result<void> keyUp(int key, int mods, int scancode = 0) = 0;
    virtual Result<void> charInput(uint32_t codepoint, int mods = 0) = 0;
    virtual Result<void> mouseDown(float x, float y, int button) = 0;
    virtual Result<void> mouseUp(float x, float y, int button) = 0;
    virtual Result<void> mouseMove(float x, float y) = 0;
    virtual Result<void> mouseDrag(float x, float y, int button) = 0;
    virtual Result<void> scroll(float x, float y, float dx, float dy, int mods = 0) = 0;
    virtual Result<void> setFocus(uint64_t objectId) = 0;
    virtual Result<void> resize(float width, float height) = 0;
    virtual Result<void> contextMenuAction(uint64_t objectId, const std::string& action, int row, int col) = 0;
    virtual Result<void> cardMouseDown(uint64_t targetId, float x, float y, int button) = 0;
    virtual Result<void> cardMouseUp(uint64_t targetId, float x, float y, int button) = 0;
    virtual Result<void> cardMouseMove(uint64_t targetId, float x, float y) = 0;
    virtual Result<void> cardScroll(uint64_t targetId, float x, float y, float dx, float dy) = 0;

    // Tree manipulation
    virtual Result<void> close(uint64_t objectId) = 0;
    virtual Result<void> split(uint64_t objectId, int orientation) = 0;

    // Queries
    virtual Result<std::string> uiTree() = 0;

    // Generic: send notification on any channel
    virtual Result<void> notify(Channel channel, const std::string& method, const msgpack::sbuffer& params) = 0;

    // Generic: send request on any channel and get response
    virtual Result<msgpack::object_handle> request(Channel channel, const std::string& method, const msgpack::sbuffer& params) = 0;

protected:
    RpcClient() = default;
};

} // namespace rpc
} // namespace yetty
