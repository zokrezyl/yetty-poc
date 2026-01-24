#pragma once

#include <cstdint>
#include <variant>

namespace yetty {

using PaneId = uint32_t;

struct SplitVertically { PaneId pane_id; };
struct SplitHorizontally { PaneId pane_id; };
struct ClosePane { PaneId pane_id; };
struct FocusPane { PaneId pane_id; };

using Command = std::variant<
    SplitVertically,
    SplitHorizontally,
    ClosePane,
    FocusPane
>;

} // namespace yetty
