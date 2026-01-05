#pragma once

#include <cstdint>

namespace yetty {

// Rectangle representing damaged (changed) cells
// Separate header to avoid libuv dependency for grid-renderer on web builds
struct DamageRect {
    uint32_t _startCol, _startRow;
    uint32_t _endCol, _endRow;  // exclusive
};

} // namespace yetty
