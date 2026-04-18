#include <boost/ut.hpp>
#include "yetty/vnc/protocol.h"
#include <cstring>
#include <vector>

using namespace boost::ut;
using namespace yetty::vnc;

suite protocol_tests = [] {
    "protocol struct sizes are packed"_test = [] {
        // Verify packed struct sizes for wire protocol
        expect(sizeof(FrameHeader) == 12_i);   // magic(4) + width(2) + height(2) + tile_size(2) + num_tiles(2)
        expect(sizeof(TileHeader) == 9_i);     // tile_x(2) + tile_y(2) + encoding(1) + data_size(4)
        expect(sizeof(RectHeader) == 14_i);    // px_x(2) + px_y(2) + width(2) + height(2) + encoding(1) + reserved(1) + data_size(4)
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
        evt.mods = VNC_MOD_SHIFT | VNC_MOD_CTRL;

        std::vector<uint8_t> buf(sizeof(evt));
        std::memcpy(buf.data(), &evt, sizeof(evt));

        KeyEvent parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.keycode == 65_i);
        expect(parsed.scancode == 30_i);
        expect(parsed.mods == (VNC_MOD_SHIFT | VNC_MOD_CTRL));
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

    "rect header serialization"_test = [] {
        RectHeader rect;
        rect.px_x = 128;
        rect.px_y = 256;
        rect.width = 192;
        rect.height = 128;
        rect.encoding = static_cast<uint8_t>(Encoding::RECT_JPEG);
        rect.reserved = 0;
        rect.data_size = 8192;

        std::vector<uint8_t> buf(sizeof(rect));
        std::memcpy(buf.data(), &rect, sizeof(rect));

        RectHeader parsed;
        std::memcpy(&parsed, buf.data(), sizeof(parsed));

        expect(parsed.px_x == 128_i);
        expect(parsed.px_y == 256_i);
        expect(parsed.width == 192_i);
        expect(parsed.height == 128_i);
        expect(parsed.encoding == static_cast<uint8_t>(Encoding::RECT_JPEG));
        expect(parsed.data_size == 8192_i);
    };

    "rectangle mode detection"_test = [] {
        // tile_size == 0 indicates rectangle mode
        FrameHeader rectMode;
        rectMode.magic = FRAME_MAGIC;
        rectMode.width = 1024;
        rectMode.height = 768;
        rectMode.tile_size = 0;  // Rectangle mode
        rectMode.num_tiles = 3;  // 3 rectangles

        expect(rectMode.tile_size == 0_i);

        FrameHeader tileMode;
        tileMode.tile_size = TILE_SIZE;  // Tile mode
        expect(tileMode.tile_size == TILE_SIZE);
    };

    "encoding enum values"_test = [] {
        expect(static_cast<uint8_t>(Encoding::RAW) == 0_i);
        expect(static_cast<uint8_t>(Encoding::RLE) == 1_i);
        expect(static_cast<uint8_t>(Encoding::JPEG) == 2_i);
        expect(static_cast<uint8_t>(Encoding::FULL_FRAME) == 3_i);
        expect(static_cast<uint8_t>(Encoding::RECT_RAW) == 4_i);
        expect(static_cast<uint8_t>(Encoding::RECT_JPEG) == 5_i);
    };

    "input type enum values"_test = [] {
        expect(static_cast<uint8_t>(InputType::MOUSE_MOVE) == 0_i);
        expect(static_cast<uint8_t>(InputType::MOUSE_BUTTON) == 1_i);
        expect(static_cast<uint8_t>(InputType::MOUSE_SCROLL) == 2_i);
        expect(static_cast<uint8_t>(InputType::KEY_DOWN) == 3_i);
        expect(static_cast<uint8_t>(InputType::KEY_UP) == 4_i);
        expect(static_cast<uint8_t>(InputType::TEXT_INPUT) == 5_i);
        expect(static_cast<uint8_t>(InputType::RESIZE) == 6_i);
        expect(static_cast<uint8_t>(InputType::CELL_SIZE) == 7_i);
        expect(static_cast<uint8_t>(InputType::CHAR_WITH_MODS) == 8_i);
        expect(static_cast<uint8_t>(InputType::FRAME_ACK) == 9_i);
    };

    "frame ack message format"_test = [] {
        // FRAME_ACK has no payload, just InputHeader
        InputHeader ack;
        ack.type = static_cast<uint8_t>(InputType::FRAME_ACK);
        ack.reserved = 0;
        ack.data_size = 0;

        expect(ack.type == 9_i);
        expect(ack.data_size == 0_i);
    };

    "modifier flags"_test = [] {
        expect(VNC_MOD_SHIFT == 1_i);
        expect(VNC_MOD_CTRL == 2_i);
        expect(VNC_MOD_ALT == 4_i);
        expect(VNC_MOD_SUPER == 8_i);

        // Test combinations
        uint8_t mods = VNC_MOD_SHIFT | VNC_MOD_CTRL;
        expect((mods & VNC_MOD_SHIFT) != 0_i);
        expect((mods & VNC_MOD_CTRL) != 0_i);
        expect((mods & VNC_MOD_ALT) == 0_i);
    };
};
