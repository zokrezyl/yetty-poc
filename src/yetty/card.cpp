#include <yetty/card.h>

namespace yetty {

Result<bool> Card::onEvent(const base::Event& event) {
    // Default implementation - event not handled
    // Subclasses can override to handle specific events
    (void)event;
    return Ok(false);
}

} // namespace yetty
