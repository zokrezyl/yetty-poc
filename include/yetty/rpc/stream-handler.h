#pragma once

#include <yetty/result.hpp>
#include <yetty/rpc/rpc-server.h>

namespace yetty {
namespace rpc {

/**
 * Register RPC handlers for card buffer/texture streaming.
 *
 * Uses GPUScreenManager singleton to access CardManager.
 *
 * Methods on Channel::CardStream:
 *   - stream_connect: {} -> {data_shm, data_size}
 *   - stream_get_buffer: {name, scope} -> {offset, size} or error
 *   - stream_disconnect: {} -> {}
 */
Result<void> registerStreamHandlers(RpcServer& server);

} // namespace rpc
} // namespace yetty
