#pragma once

#include <yetty/result.hpp>
#include <string>

namespace yetty {
namespace rpc {

// Compute the socket path for this yetty instance.
// Uses the provided runtime directory from Platform::getRuntimeDir().
// Creates the directory if it doesn't exist.
// Returns the full path.
//   Unix: <runtimeDir>/yetty/yetty-<pid>.sock
//   Windows: \\.\pipe\yetty-<pid>
Result<std::string> createSocketPath(const std::string& runtimeDir);

// Export the socket path as $YETTY_SOCKET so child processes (shells,
// commands spawned inside the terminal) can discover it.
Result<void> exportSocketPath(const std::string& path);

} // namespace rpc
} // namespace yetty
