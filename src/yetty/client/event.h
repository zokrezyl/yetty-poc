#pragma once

#include "commands.h"

namespace yetty::client {

// Top-level "event" command — dispatches to send, list, etc.
Result<void> cmdEvent(const CmdContext& ctx, ArgIt begin, ArgIt end);

} // namespace yetty::client
