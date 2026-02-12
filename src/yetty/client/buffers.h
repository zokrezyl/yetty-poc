#pragma once

#include "commands.h"

namespace yetty::client {

Result<void> cmdBuffers(const CmdContext& ctx, ArgIt begin, ArgIt end);

} // namespace yetty::client
