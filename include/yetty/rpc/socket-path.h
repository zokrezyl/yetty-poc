#pragma once

#include <yetty/result.hpp>
#include <string>

namespace yetty {
namespace rpc {

// Compute the default socket path for this yetty instance:
//   $XDG_RUNTIME_DIR/yetty/yetty-<pid>.sock
// Creates the directory if it doesn't exist.
// Returns the full path.
Result<std::string> createSocketPath();

// Export the socket path as $YETTY_SOCKET so child processes (shells,
// commands spawned inside the terminal) can discover it.
Result<void> exportSocketPath(const std::string& path);

} // namespace rpc
} // namespace yetty
