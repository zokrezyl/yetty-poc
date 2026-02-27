#pragma once

// Compatibility header - types moved to ygui namespace
// This header provides backward compatibility for ymery module

#include <yetty/ygui/types.h>

namespace yetty::ymery {

// Import types from ygui namespace for backward compatibility
using yetty::ygui::Value;
using yetty::ygui::Dict;
using yetty::ygui::List;
using yetty::ygui::getAs;
using yetty::ygui::DataPath;
using yetty::ygui::TreeLike;
using yetty::ygui::TreeLikePtr;

} // namespace yetty::ymery
