#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>

namespace yetty {
namespace base {

using ObjectId = uint64_t;
using TypeId = uint64_t;

static constexpr ObjectId NoObjectId = std::numeric_limits<ObjectId>::max();
static constexpr TypeId NoTypeId = std::numeric_limits<TypeId>::max();

} // namespace base
} // namespace yetty
