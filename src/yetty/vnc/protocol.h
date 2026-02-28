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

constexpr uint32_t FRAME_MAGIC = 0x594E4346;  // "YNCF" - Yetty VNC Frame

struct FrameHeader {
    uint32_t magic;      // FRAME_MAGIC for sync detection
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

// Input event types (client -> server)
enum class InputType : uint8_t {
    MOUSE_MOVE = 0,
    MOUSE_BUTTON = 1,
    MOUSE_SCROLL = 2,
    KEY_DOWN = 3,
    KEY_UP = 4,
    TEXT_INPUT = 5,
    RESIZE = 6,      // Client window resized
    CELL_SIZE = 7,   // Client sets cell height (ctrl+wheel zoom)
};

enum class MouseButton : uint8_t {
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2,
};

struct InputHeader {
    uint8_t type;       // InputType
    uint8_t reserved;
    uint16_t data_size; // Size of following data
};

struct MouseMoveEvent {
    int16_t x;
    int16_t y;
};

struct MouseButtonEvent {
    int16_t x;
    int16_t y;
    uint8_t button;     // MouseButton
    uint8_t pressed;    // 1 = pressed, 0 = released
};

struct MouseScrollEvent {
    int16_t x;
    int16_t y;
    int16_t delta_x;
    int16_t delta_y;
};

struct KeyEvent {
    uint32_t keycode;   // Platform keycode
    uint32_t scancode;  // Hardware scancode
    uint8_t mods;       // Modifier flags (shift, ctrl, alt, etc.)
};

// Modifier flags for KeyEvent.mods
constexpr uint8_t MOD_SHIFT = 0x01;
constexpr uint8_t MOD_CTRL  = 0x02;
constexpr uint8_t MOD_ALT   = 0x04;
constexpr uint8_t MOD_SUPER = 0x08;

// Text input event - for unicode text (follows InputHeader, variable length UTF-8)

struct ResizeEvent {
    uint16_t width;    // New client window width
    uint16_t height;   // New client window height
};

struct CellSizeEvent {
    uint8_t cellHeight;  // Absolute cell height in pixels (client commands the size)
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
