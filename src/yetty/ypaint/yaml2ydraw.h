#pragma once

#include "ydraw-buffer.h"
#include "animation.h"
#include <string>

namespace yetty {

// Returns the animation (null if YAML has no animation blocks)
Result<animation::Animation::Ptr> parseYDrawYAML(YDrawBuffer::Ptr buffer,
                                                  const std::string& yaml);

} // namespace yetty
