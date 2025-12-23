#include "Decorator.h"

namespace yetty {

Decorator::Decorator(uint32_t id, DecoratorPlugin* plugin, PositionMode mode,
                     int32_t x, int32_t y, uint32_t widthCells, uint32_t heightCells)
    : id_(id)
    , plugin_(plugin)
    , positionMode_(mode)
    , x_(x)
    , y_(y)
    , widthCells_(widthCells)
    , heightCells_(heightCells)
{
}

Decorator::~Decorator() {
    // Plugin cleanup is handled by DecoratorManager
}

} // namespace yetty
