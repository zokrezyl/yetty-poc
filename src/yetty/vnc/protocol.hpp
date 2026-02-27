#pragma once

#include <cstdint>

namespace yetty::vnc {

constexpr uint16_t DEFAULT_PORT = 5900;
constexpr uint16_t TILE_SIZE = 64;

enum class Encoding : uint8_t {
    RAW = 0,
    RLE = 1,
    JPEG = 2,
};

#pragma pack(push, 1)

struct FrameHeader {
    uint16_t width;
    uint16_t height;
    uint16_t tile_size;
    uint16_t num_tiles;
};

struct TileHeader {
    uint16_t tile_x;
    uint16_t tile_y;
    uint8_t encoding;
    uint32_t data_size;
};

#pragma pack(pop)

// Calculate tile grid dimensions
inline uint16_t tiles_x(uint16_t width) {
    return (width + TILE_SIZE - 1) / TILE_SIZE;
}

inline uint16_t tiles_y(uint16_t height) {
    return (height + TILE_SIZE - 1) / TILE_SIZE;
}

} // namespace yetty::vnc
