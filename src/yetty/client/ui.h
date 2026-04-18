#pragma once

#include "commands.h"

namespace yetty::client {

Result<void> cmdUi(const CmdContext& ctx, ArgIt begin, ArgIt end);

} // namespace yetty::client
