#pragma once

#include "object.h"
#include "event.h"
#include <yetty/result.hpp>

namespace yetty {
namespace base {

class EventListener : public virtual Object {
public:
    using Ptr = std::shared_ptr<EventListener>;

    virtual ~EventListener() = default;

    // Handle event. Returns Ok(true) if consumed, Ok(false) if not, Err on failure.
    virtual Result<bool> onEvent(const Event& event) = 0;
};

} // namespace base
} // namespace yetty
