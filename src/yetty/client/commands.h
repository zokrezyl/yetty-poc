#pragma once

#include <yetty/result.hpp>
#include <functional>
#include <string>
#include <vector>

namespace yetty::client {

// Iterator type for passing remaining args to sub-parsers
using ArgIt = std::vector<std::string>::const_iterator;

// Common context passed down the command tree
struct CmdContext {
    std::string prog;
    std::string socketPath;
};

// Command handler signature — returns Result<void> (Err on failure)
using CmdFn = std::function<Result<void>(const CmdContext& ctx, ArgIt begin, ArgIt end)>;

} // namespace yetty::client
