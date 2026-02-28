#include <boost/ut.hpp>
#include "yetty/vnc/protocol.h"
#include <cstring>
#include <vector>

using namespace boost::ut;
using namespace yetty::vnc;

suite protocol_tests = [] {
    "protocol struct sizes are packed"_test = [] {
        // Verify packed struct sizes for wire protocol
        expect(sizeof(FrameHeader) == 8_i);
        expect(sizeof(TileHeader) == 9_i);
        expect(sizeof(InputHeader) == 4_i);
        expect(sizeof(MouseMoveEvent) == 4_i);
        expect(sizeof(MouseButtonEvent) == 6_i);
        expect(sizeof(MouseScrollEvent) == 8_i);
        expect(sizeof(KeyEvent) == 9_i);
    };

    "frame header serialization"_test = [] {
        FrameHeader hdr;
        hdr.width = 1920;
        hdr.height = 1080;
        hdr.tile_size = TILE_SIZE;
        hdr.num_tiles = 510;

        // Serialize to bytes
        std::vector<uint8_t> buf(sizeof(hdr));
        std::memcpy(buf.data(), &hdr, sizeof(hdr));

        // Deserialize
        FrameHeader parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.width == 1920_i);
        expect(parsed.height == 1080_i);
        expect(parsed.tile_size == TILE_SIZE);
        expect(parsed.num_tiles == 510_i);
    };

    "tile header serialization"_test = [] {
        TileHeader tile;
        tile.tile_x = 5;
        tile.tile_y = 10;
        tile.encoding = static_cast<uint8_t>(Encoding::JPEG);
        tile.data_size = 4096;

        std::vector<uint8_t> buf(sizeof(tile));
        std::memcpy(buf.data(), &tile, sizeof(tile));

        TileHeader parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.tile_x == 5_i);
        expect(parsed.tile_y == 10_i);
        expect(parsed.encoding == static_cast<uint8_t>(Encoding::JPEG));
        expect(parsed.data_size == 4096_i);
    };

    "input header serialization"_test = [] {
        InputHeader hdr;
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.reserved = 0;
        hdr.data_size = sizeof(MouseMoveEvent);

        std::vector<uint8_t> buf(sizeof(hdr));
        std::memcpy(buf.data(), &hdr, sizeof(hdr));

        InputHeader parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.type == static_cast<uint8_t>(InputType::MOUSE_MOVE));
        expect(parsed.data_size == sizeof(MouseMoveEvent));
    };

    "mouse move event serialization"_test = [] {
        MouseMoveEvent evt;
        evt.x = 500;
        evt.y = 300;

        std::vector<uint8_t> buf(sizeof(evt));
        std::memcpy(buf.data(), &evt, sizeof(evt));

        MouseMoveEvent parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.x == 500_i);
        expect(parsed.y == 300_i);
    };

    "mouse button event serialization"_test = [] {
        MouseButtonEvent evt;
        evt.x = 100;
        evt.y = 200;
        evt.button = static_cast<uint8_t>(MouseButton::LEFT);
        evt.pressed = 1;

        std::vector<uint8_t> buf(sizeof(evt));
        std::memcpy(buf.data(), &evt, sizeof(evt));

        MouseButtonEvent parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.x == 100_i);
        expect(parsed.y == 200_i);
        expect(parsed.button == static_cast<uint8_t>(MouseButton::LEFT));
        expect(parsed.pressed == 1_i);
    };

    "mouse scroll event serialization"_test = [] {
        MouseScrollEvent evt;
        evt.x = 400;
        evt.y = 300;
        evt.delta_x = 0;
        evt.delta_y = -120;

        std::vector<uint8_t> buf(sizeof(evt));
        std::memcpy(buf.data(), &evt, sizeof(evt));

        MouseScrollEvent parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.x == 400_i);
        expect(parsed.y == 300_i);
        expect(parsed.delta_x == 0_i);
        expect(parsed.delta_y == -120_i);
    };

    "key event serialization"_test = [] {
        KeyEvent evt;
        evt.keycode = 65;  // 'A'
        evt.scancode = 30;
        evt.mods = MOD_SHIFT | MOD_CTRL;

        std::vector<uint8_t> buf(sizeof(evt));
        std::memcpy(buf.data(), &evt, sizeof(evt));

        KeyEvent parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.keycode == 65_i);
        expect(parsed.scancode == 30_i);
        expect(parsed.mods == (MOD_SHIFT | MOD_CTRL));
    };

    "tiles_x calculation"_test = [] {
        expect(tiles_x(64) == 1_i);
        expect(tiles_x(65) == 2_i);
        expect(tiles_x(128) == 2_i);
        expect(tiles_x(1920) == 30_i);
        expect(tiles_x(1) == 1_i);
    };

    "tiles_y calculation"_test = [] {
        expect(tiles_y(64) == 1_i);
        expect(tiles_y(65) == 2_i);
        expect(tiles_y(128) == 2_i);
        expect(tiles_y(1080) == 17_i);
        expect(tiles_y(1) == 1_i);
    };

    "encoding enum values"_test = [] {
        expect(static_cast<uint8_t>(Encoding::RAW) == 0_i);
        expect(static_cast<uint8_t>(Encoding::RLE) == 1_i);
        expect(static_cast<uint8_t>(Encoding::JPEG) == 2_i);
    };

    "input type enum values"_test = [] {
        expect(static_cast<uint8_t>(InputType::MOUSE_MOVE) == 0_i);
        expect(static_cast<uint8_t>(InputType::MOUSE_BUTTON) == 1_i);
        expect(static_cast<uint8_t>(InputType::MOUSE_SCROLL) == 2_i);
        expect(static_cast<uint8_t>(InputType::KEY_DOWN) == 3_i);
        expect(static_cast<uint8_t>(InputType::KEY_UP) == 4_i);
        expect(static_cast<uint8_t>(InputType::TEXT_INPUT) == 5_i);
    };

    "modifier flags"_test = [] {
        expect(MOD_SHIFT == 1_i);
        expect(MOD_CTRL == 2_i);
        expect(MOD_ALT == 4_i);
        expect(MOD_SUPER == 8_i);

        // Test combinations
        uint8_t mods = MOD_SHIFT | MOD_CTRL;
        expect((mods & MOD_SHIFT) != 0_i);
        expect((mods & MOD_CTRL) != 0_i);
        expect((mods & MOD_ALT) == 0_i);
    };
};
