#pragma once

#include "ygui-widgets.h"
#include <string>
#include <vector>

namespace yetty::ygui {

/// Parse YAML widget definition and return root widgets.
std::vector<WidgetPtr> parseWidgets(const std::string& yaml);

/// Parse a single widget from YAML node (used internally).
WidgetPtr parseWidget(const void* node);

} // namespace yetty::ygui
