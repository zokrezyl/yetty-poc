#pragma once

#include "ypaint-buffer.h"
#include "animation.h"
#include <string>

namespace yetty {

// Returns the animation (null if YAML has no animation blocks)
Result<animation::Animation::Ptr> parseYPaintYAML(YPaintBuffer::Ptr buffer,
                                                  const std::string& yaml);

} // namespace yetty
