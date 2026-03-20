#include "yrich-operation.h"
#include <sstream>

namespace yetty::yrich {

//=============================================================================
// Operation serialization (placeholder - implement with msgpack or similar)
//=============================================================================

std::string Operation::serialize() const {
    // TODO: Implement proper serialization using msgpack
    // For now, return empty - real implementation would serialize:
    // - type
    // - timestamp
    // - author
    // - data variant
    return {};
}

Operation Operation::deserialize(std::string_view data) {
    // TODO: Implement proper deserialization
    // For now, return a no-op
    return Operation(OperationType::CursorMove, CursorMoveOp{}, Timestamp{}, LOCAL_SESSION);
}

} // namespace yetty::yrich
