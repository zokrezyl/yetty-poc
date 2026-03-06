#pragma once

#include "ydraw-buffer.h"
#include "animation.h"
#include <string>

namespace yetty {

// AABB of parsed content
struct YDrawAABB {
    float minX = 1e10f, minY = 1e10f;
    float maxX = -1e10f, maxY = -1e10f;
    bool valid = false;

    void expand(float x, float y) {
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
        valid = true;
    }
    void expand(float x, float y, float w, float h) {
        expand(x, y);
        expand(x + w, y + h);
    }
    float width() const { return maxX - minX; }
    float height() const { return maxY - minY; }
};

// Returns the animation (null if YAML has no animation blocks)
// If outAABB is provided, fills it with the bounding box of parsed content
Result<animation::Animation::Ptr> parseYDrawYAML(YDrawBuffer::Ptr buffer,
                                                  const std::string& yaml,
                                                  YDrawAABB* outAABB = nullptr);

} // namespace yetty
