#pragma once

#include <yetty/rpc/rpc-server.h>
#include <memory>

namespace yetty {
class Workspace;

namespace rpc {

// Registers all Channel::EventLoop handlers on the given server.
// Decodes RPC messages into base::Event and dispatches them via the EventLoop.
Result<void> registerEventLoopHandlers(RpcServer& server);

// Registers workspace query handlers (ui_tree, etc.)
Result<void> registerWorkspaceHandlers(RpcServer& server, std::shared_ptr<Workspace> workspace);

} // namespace rpc
} // namespace yetty
