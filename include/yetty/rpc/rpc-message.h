#pragma once

#include <cstdint>
#include <string>
#include <msgpack.hpp>

namespace yetty {
namespace rpc {

// Channel IDs for multiplexing RPC messages across subsystems
enum class Channel : uint32_t {
    EventLoop = 0,   // Post events to the event loop
    // Future channels:
    // Window   = 1, // Window/rendering control
    // Terminal = 2, // Terminal instance management
};

// Message types following msgpack-rpc spec (like nvim)
enum class MessageType : uint32_t {
    Request      = 0,  // [type, msgid, method, params]
    Response     = 1,  // [type, msgid, error, result]
    Notification = 2,  // [type, method, params]
};

// A single RPC message decoded from the wire
struct RpcMessage {
    MessageType type;
    uint32_t    msgid;    // 0 for notifications
    uint32_t    channel;
    std::string method;
    msgpack::object_handle params;  // lazy-decoded params
};

} // namespace rpc
} // namespace yetty
