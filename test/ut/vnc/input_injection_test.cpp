#include <boost/ut.hpp>
#include "yetty/vnc/protocol.h"
#include <cstring>
#include <vector>
#include <functional>

using namespace boost::ut;
using namespace yetty::vnc;

// Mock input sender that captures sent data for verification
class MockInputSender {
public:
    std::vector<std::vector<uint8_t>> sentPackets;

    void send(const void* data, size_t size) {
        std::vector<uint8_t> packet(size);
        std::memcpy(packet.data(), data, size);
        sentPackets.push_back(std::move(packet));
    }

    void clear() {
        sentPackets.clear();
    }

    // Helper to build mouse move packet (matches VncClient::sendMouseMove)
    void sendMouseMove(int16_t x, int16_t y) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);

        MouseMoveEvent evt = {};
        evt.x = x;
        evt.y = y;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        send(buf, sizeof(buf));
    }

    // Helper to build mouse button packet
    void sendMouseButton(int16_t x, int16_t y, MouseButton button, bool pressed) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_BUTTON);
        hdr.data_size = sizeof(MouseButtonEvent);

        MouseButtonEvent evt = {};
        evt.x = x;
        evt.y = y;
        evt.button = static_cast<uint8_t>(button);
        evt.pressed = pressed ? 1 : 0;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        send(buf, sizeof(buf));
    }

    // Helper to build mouse scroll packet
    void sendMouseScroll(int16_t x, int16_t y, int16_t deltaX, int16_t deltaY) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_SCROLL);
        hdr.data_size = sizeof(MouseScrollEvent);

        MouseScrollEvent evt = {};
        evt.x = x;
        evt.y = y;
        evt.delta_x = deltaX;
        evt.delta_y = deltaY;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        send(buf, sizeof(buf));
    }

    // Helper to build key down packet
    void sendKeyDown(uint32_t keycode, uint32_t scancode, uint8_t mods) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::KEY_DOWN);
        hdr.data_size = sizeof(KeyEvent);

        KeyEvent evt = {};
        evt.keycode = keycode;
        evt.scancode = scancode;
        evt.mods = mods;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        send(buf, sizeof(buf));
    }

    // Helper to build key up packet
    void sendKeyUp(uint32_t keycode, uint32_t scancode, uint8_t mods) {
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::KEY_UP);
        hdr.data_size = sizeof(KeyEvent);

        KeyEvent evt = {};
        evt.keycode = keycode;
        evt.scancode = scancode;
        evt.mods = mods;

        uint8_t buf[sizeof(hdr) + sizeof(evt)];
        std::memcpy(buf, &hdr, sizeof(hdr));
        std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
        send(buf, sizeof(buf));
    }

    // Helper to build text input packet
    void sendTextInput(const char* text, size_t len) {
        if (len == 0 || len > 1024) return;

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
        hdr.data_size = static_cast<uint16_t>(len);

        std::vector<uint8_t> buf(sizeof(hdr) + len);
        std::memcpy(buf.data(), &hdr, sizeof(hdr));
        std::memcpy(buf.data() + sizeof(hdr), text, len);
        send(buf.data(), buf.size());
    }
};

// Helper to parse header from packet
InputHeader parseHeader(const std::vector<uint8_t>& packet) {
    InputHeader hdr;
    std::memcpy(&hdr, packet.data(), sizeof(hdr));
    return hdr;
}

suite input_injection_tests = [] {
    "mouse move injection"_test = [] {
        MockInputSender sender;

        sender.sendMouseMove(100, 200);

        expect(sender.sentPackets.size() == 1_i);

        auto& packet = sender.sentPackets[0];
        expect(packet.size() == sizeof(InputHeader) + sizeof(MouseMoveEvent));

        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::MOUSE_MOVE));
        expect(hdr.data_size == sizeof(MouseMoveEvent));

        MouseMoveEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.x == 100_i);
        expect(evt.y == 200_i);
    };

    "mouse button press injection"_test = [] {
        MockInputSender sender;

        sender.sendMouseButton(50, 75, MouseButton::LEFT, true);

        expect(sender.sentPackets.size() == 1_i);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::MOUSE_BUTTON));

        MouseButtonEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.x == 50_i);
        expect(evt.y == 75_i);
        expect(evt.button == static_cast<uint8_t>(MouseButton::LEFT));
        expect(evt.pressed == 1_i);
    };

    "mouse button release injection"_test = [] {
        MockInputSender sender;

        sender.sendMouseButton(150, 175, MouseButton::RIGHT, false);

        auto& packet = sender.sentPackets[0];
        MouseButtonEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.button == static_cast<uint8_t>(MouseButton::RIGHT));
        expect(evt.pressed == 0_i);
    };

    "mouse scroll injection"_test = [] {
        MockInputSender sender;

        sender.sendMouseScroll(300, 400, 0, -120);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::MOUSE_SCROLL));

        MouseScrollEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.x == 300_i);
        expect(evt.y == 400_i);
        expect(evt.delta_x == 0_i);
        expect(evt.delta_y == -120_i);
    };

    "key down injection"_test = [] {
        MockInputSender sender;

        sender.sendKeyDown(65, 30, MOD_SHIFT);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::KEY_DOWN));

        KeyEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.keycode == 65_i);
        expect(evt.scancode == 30_i);
        expect(evt.mods == MOD_SHIFT);
    };

    "key up injection"_test = [] {
        MockInputSender sender;

        sender.sendKeyUp(65, 30, 0);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::KEY_UP));

        KeyEvent evt;
        std::memcpy(&evt, packet.data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.keycode == 65_i);
        expect(evt.mods == 0_i);
    };

    "text input injection"_test = [] {
        MockInputSender sender;

        const char* text = "Hello";
        sender.sendTextInput(text, 5);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.type == static_cast<uint8_t>(InputType::TEXT_INPUT));
        expect(hdr.data_size == 5_i);

        std::string received(reinterpret_cast<char*>(packet.data() + sizeof(InputHeader)), 5);
        expect(received == "Hello");
    };

    "unicode text input injection"_test = [] {
        MockInputSender sender;

        const char* text = "\xC3\xA9";  // UTF-8 'e' with acute
        sender.sendTextInput(text, 2);

        auto& packet = sender.sentPackets[0];
        auto hdr = parseHeader(packet);
        expect(hdr.data_size == 2_i);
    };

    "multiple events sequence"_test = [] {
        MockInputSender sender;

        // Simulate clicking: move -> press -> release
        sender.sendMouseMove(100, 100);
        sender.sendMouseButton(100, 100, MouseButton::LEFT, true);
        sender.sendMouseButton(100, 100, MouseButton::LEFT, false);

        expect(sender.sentPackets.size() == 3_i);

        auto hdr0 = parseHeader(sender.sentPackets[0]);
        auto hdr1 = parseHeader(sender.sentPackets[1]);
        auto hdr2 = parseHeader(sender.sentPackets[2]);

        expect(hdr0.type == static_cast<uint8_t>(InputType::MOUSE_MOVE));
        expect(hdr1.type == static_cast<uint8_t>(InputType::MOUSE_BUTTON));
        expect(hdr2.type == static_cast<uint8_t>(InputType::MOUSE_BUTTON));
    };

    "drag sequence"_test = [] {
        MockInputSender sender;

        // Simulate drag: press -> move -> move -> release
        sender.sendMouseButton(0, 0, MouseButton::LEFT, true);
        sender.sendMouseMove(50, 50);
        sender.sendMouseMove(100, 100);
        sender.sendMouseButton(100, 100, MouseButton::LEFT, false);

        expect(sender.sentPackets.size() == 4_i);
    };

    "key combo injection"_test = [] {
        MockInputSender sender;

        // Ctrl+C sequence
        sender.sendKeyDown(0x11, 29, MOD_CTRL);  // Ctrl down
        sender.sendKeyDown(67, 46, MOD_CTRL);    // C down with Ctrl
        sender.sendKeyUp(67, 46, MOD_CTRL);      // C up
        sender.sendKeyUp(0x11, 29, 0);           // Ctrl up

        expect(sender.sentPackets.size() == 4_i);
    };

    "negative coordinates"_test = [] {
        MockInputSender sender;

        // Negative coords can happen in some coordinate systems
        sender.sendMouseMove(-10, -20);

        MouseMoveEvent evt;
        std::memcpy(&evt, sender.sentPackets[0].data() + sizeof(InputHeader), sizeof(evt));
        expect(evt.x == -10_i);
        expect(evt.y == -20_i);
    };

    "all modifier combinations"_test = [] {
        MockInputSender sender;

        uint8_t allMods = MOD_SHIFT | MOD_CTRL | MOD_ALT | MOD_SUPER;
        sender.sendKeyDown(65, 30, allMods);

        KeyEvent evt;
        std::memcpy(&evt, sender.sentPackets[0].data() + sizeof(InputHeader), sizeof(evt));
        expect((evt.mods & MOD_SHIFT) != 0_i);
        expect((evt.mods & MOD_CTRL) != 0_i);
        expect((evt.mods & MOD_ALT) != 0_i);
        expect((evt.mods & MOD_SUPER) != 0_i);
    };
};
