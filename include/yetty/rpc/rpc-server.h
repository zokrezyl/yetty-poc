#pragma once

#include <yetty/base/base.h>
#include <yetty/rpc/rpc-message.h>
#include <functional>
#include <string>

namespace yetty {
namespace rpc {

// Handler callback for incoming RPC requests/notifications.
// For requests: return a packable result (serialized via msgpack).
// For notifications: return value is ignored.
using RpcHandler = std::function<msgpack::object_handle(const RpcMessage& msg)>;

// RPC server that listens on a Unix domain socket and multiplexes
// messages to registered channel handlers via the EventLoop.
//
// Wire format (msgpack-rpc with channel extension):
//   Request:      [0, msgid, channel, method, params]
//   Response:     [1, msgid, error, result]
//   Notification: [2, channel, method, params]
//
class RpcServer : public base::EventListener, public base::ObjectFactory<RpcServer> {
public:
    using Ptr = std::shared_ptr<RpcServer>;

    static Result<Ptr> createImpl(const std::string& socketPath) noexcept;

    ~RpcServer() override = default;

    // Start accepting connections (integrates with the EventLoop)
    virtual Result<void> start() = 0;

    // Stop the server and close all connections
    virtual Result<void> stop() = 0;

    // Register a handler for a (channel, method) pair
    virtual void registerHandler(Channel channel, const std::string& method, RpcHandler handler) = 0;

    // Socket path this server is bound to
    virtual const std::string& socketPath() const = 0;

protected:
    RpcServer() = default;
};

} // namespace rpc
} // namespace yetty
