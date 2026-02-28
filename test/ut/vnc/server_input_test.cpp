#include <boost/ut.hpp>
#include "yetty/vnc/protocol.h"
#include <cstring>
#include <vector>
#include <functional>
#include <optional>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>

using namespace boost::ut;
using namespace yetty::vnc;

// Mock server input parser - simulates server-side input parsing
class MockServerInputParser {
public:
    // Recorded events
    struct MouseMoveRecord { int16_t x, y; };
    struct MouseButtonRecord { int16_t x, y; MouseButton button; bool pressed; };
    struct MouseScrollRecord { int16_t x, y; int16_t dx, dy; };
    struct KeyRecord { uint32_t keycode; uint32_t scancode; uint8_t mods; bool down; };
    struct TextRecord { std::string text; };

    std::vector<MouseMoveRecord> mouseMoves;
    std::vector<MouseButtonRecord> mouseButtons;
    std::vector<MouseScrollRecord> mouseScrolls;
    std::vector<KeyRecord> keys;
    std::vector<TextRecord> texts;

    // Parse state (simulates partial read handling)
    std::vector<uint8_t> buffer;
    size_t needed = sizeof(InputHeader);
    bool readingHeader = true;
    InputHeader currentHeader;

    void reset() {
        mouseMoves.clear();
        mouseButtons.clear();
        mouseScrolls.clear();
        keys.clear();
        texts.clear();
        buffer.clear();
        needed = sizeof(InputHeader);
        readingHeader = true;
    }

    // Feed data to parser (simulates recv())
    void feedData(const uint8_t* data, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            buffer.push_back(data[i]);

            if (buffer.size() >= needed) {
                if (readingHeader) {
                    std::memcpy(&currentHeader, buffer.data(), sizeof(InputHeader));
                    buffer.erase(buffer.begin(), buffer.begin() + sizeof(InputHeader));
                    needed = currentHeader.data_size;
                    readingHeader = false;

                    if (needed == 0) {
                        dispatchEvent(nullptr, 0);
                        readingHeader = true;
                        needed = sizeof(InputHeader);
                    }
                } else {
                    dispatchEvent(buffer.data(), needed);
                    buffer.erase(buffer.begin(), buffer.begin() + needed);
                    readingHeader = true;
                    needed = sizeof(InputHeader);
                }
            }
        }
    }

    void dispatchEvent(const uint8_t* data, size_t len) {
        auto type = static_cast<InputType>(currentHeader.type);

        switch (type) {
            case InputType::MOUSE_MOVE: {
                if (len >= sizeof(MouseMoveEvent)) {
                    MouseMoveEvent evt;
                    std::memcpy(&evt, data, sizeof(evt));
                    mouseMoves.push_back({evt.x, evt.y});
                }
                break;
            }
            case InputType::MOUSE_BUTTON: {
                if (len >= sizeof(MouseButtonEvent)) {
                    MouseButtonEvent evt;
                    std::memcpy(&evt, data, sizeof(evt));
                    mouseButtons.push_back({
                        evt.x, evt.y,
                        static_cast<MouseButton>(evt.button),
                        evt.pressed != 0
                    });
                }
                break;
            }
            case InputType::MOUSE_SCROLL: {
                if (len >= sizeof(MouseScrollEvent)) {
                    MouseScrollEvent evt;
                    std::memcpy(&evt, data, sizeof(evt));
                    mouseScrolls.push_back({evt.x, evt.y, evt.delta_x, evt.delta_y});
                }
                break;
            }
            case InputType::KEY_DOWN: {
                if (len >= sizeof(KeyEvent)) {
                    KeyEvent evt;
                    std::memcpy(&evt, data, sizeof(evt));
                    keys.push_back({evt.keycode, evt.scancode, evt.mods, true});
                }
                break;
            }
            case InputType::KEY_UP: {
                if (len >= sizeof(KeyEvent)) {
                    KeyEvent evt;
                    std::memcpy(&evt, data, sizeof(evt));
                    keys.push_back({evt.keycode, evt.scancode, evt.mods, false});
                }
                break;
            }
            case InputType::TEXT_INPUT: {
                if (len > 0) {
                    texts.push_back({std::string(reinterpret_cast<const char*>(data), len)});
                }
                break;
            }
        }
    }
};

// Helper to build a complete packet
std::vector<uint8_t> buildMouseMovePacket(int16_t x, int16_t y) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
    hdr.data_size = sizeof(MouseMoveEvent);

    MouseMoveEvent evt = {};
    evt.x = x;
    evt.y = y;

    std::vector<uint8_t> packet(sizeof(hdr) + sizeof(evt));
    std::memcpy(packet.data(), &hdr, sizeof(hdr));
    std::memcpy(packet.data() + sizeof(hdr), &evt, sizeof(evt));
    return packet;
}

std::vector<uint8_t> buildMouseButtonPacket(int16_t x, int16_t y, MouseButton btn, bool pressed) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::MOUSE_BUTTON);
    hdr.data_size = sizeof(MouseButtonEvent);

    MouseButtonEvent evt = {};
    evt.x = x;
    evt.y = y;
    evt.button = static_cast<uint8_t>(btn);
    evt.pressed = pressed ? 1 : 0;

    std::vector<uint8_t> packet(sizeof(hdr) + sizeof(evt));
    std::memcpy(packet.data(), &hdr, sizeof(hdr));
    std::memcpy(packet.data() + sizeof(hdr), &evt, sizeof(evt));
    return packet;
}

std::vector<uint8_t> buildKeyPacket(InputType type, uint32_t keycode, uint32_t scancode, uint8_t mods) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(type);
    hdr.data_size = sizeof(KeyEvent);

    KeyEvent evt = {};
    evt.keycode = keycode;
    evt.scancode = scancode;
    evt.mods = mods;

    std::vector<uint8_t> packet(sizeof(hdr) + sizeof(evt));
    std::memcpy(packet.data(), &hdr, sizeof(hdr));
    std::memcpy(packet.data() + sizeof(hdr), &evt, sizeof(evt));
    return packet;
}

std::vector<uint8_t> buildTextPacket(const std::string& text) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
    hdr.data_size = static_cast<uint16_t>(text.size());

    std::vector<uint8_t> packet(sizeof(hdr) + text.size());
    std::memcpy(packet.data(), &hdr, sizeof(hdr));
    std::memcpy(packet.data() + sizeof(hdr), text.data(), text.size());
    return packet;
}

suite server_input_tests = [] {
    "parse single mouse move"_test = [] {
        MockServerInputParser parser;

        auto packet = buildMouseMovePacket(100, 200);
        parser.feedData(packet.data(), packet.size());

        expect(parser.mouseMoves.size() == 1_i);
        expect(parser.mouseMoves[0].x == 100_i);
        expect(parser.mouseMoves[0].y == 200_i);
    };

    "parse single mouse button"_test = [] {
        MockServerInputParser parser;

        auto packet = buildMouseButtonPacket(50, 75, MouseButton::LEFT, true);
        parser.feedData(packet.data(), packet.size());

        expect(parser.mouseButtons.size() == 1_i);
        expect(parser.mouseButtons[0].x == 50_i);
        expect(parser.mouseButtons[0].y == 75_i);
        expect(parser.mouseButtons[0].button == MouseButton::LEFT);
        expect(parser.mouseButtons[0].pressed == true);
    };

    "parse key down"_test = [] {
        MockServerInputParser parser;

        auto packet = buildKeyPacket(InputType::KEY_DOWN, 65, 30, MOD_SHIFT);
        parser.feedData(packet.data(), packet.size());

        expect(parser.keys.size() == 1_i);
        expect(parser.keys[0].keycode == 65_i);
        expect(parser.keys[0].scancode == 30_i);
        expect(parser.keys[0].mods == MOD_SHIFT);
        expect(parser.keys[0].down == true);
    };

    "parse key up"_test = [] {
        MockServerInputParser parser;

        auto packet = buildKeyPacket(InputType::KEY_UP, 65, 30, 0);
        parser.feedData(packet.data(), packet.size());

        expect(parser.keys.size() == 1_i);
        expect(parser.keys[0].down == false);
    };

    "parse text input"_test = [] {
        MockServerInputParser parser;

        auto packet = buildTextPacket("Hello");
        parser.feedData(packet.data(), packet.size());

        expect(parser.texts.size() == 1_i);
        expect(parser.texts[0].text == "Hello");
    };

    "parse multiple events in single feed"_test = [] {
        MockServerInputParser parser;

        auto p1 = buildMouseMovePacket(10, 20);
        auto p2 = buildMouseMovePacket(30, 40);
        auto p3 = buildMouseMovePacket(50, 60);

        // Combine all packets
        std::vector<uint8_t> combined;
        combined.insert(combined.end(), p1.begin(), p1.end());
        combined.insert(combined.end(), p2.begin(), p2.end());
        combined.insert(combined.end(), p3.begin(), p3.end());

        parser.feedData(combined.data(), combined.size());

        expect(parser.mouseMoves.size() == 3_i);
        expect(parser.mouseMoves[0].x == 10_i);
        expect(parser.mouseMoves[1].x == 30_i);
        expect(parser.mouseMoves[2].x == 50_i);
    };

    "parse partial header then body"_test = [] {
        MockServerInputParser parser;

        auto packet = buildMouseMovePacket(100, 200);

        // Feed header only (first 4 bytes)
        parser.feedData(packet.data(), 2);
        expect(parser.mouseMoves.size() == 0_i);

        // Feed rest of header
        parser.feedData(packet.data() + 2, 2);
        expect(parser.mouseMoves.size() == 0_i);

        // Feed body
        parser.feedData(packet.data() + 4, packet.size() - 4);
        expect(parser.mouseMoves.size() == 1_i);
        expect(parser.mouseMoves[0].x == 100_i);
    };

    "parse byte by byte"_test = [] {
        MockServerInputParser parser;

        auto packet = buildMouseMovePacket(42, 84);

        for (size_t i = 0; i < packet.size(); ++i) {
            parser.feedData(&packet[i], 1);
        }

        expect(parser.mouseMoves.size() == 1_i);
        expect(parser.mouseMoves[0].x == 42_i);
        expect(parser.mouseMoves[0].y == 84_i);
    };

    "parse mixed event types"_test = [] {
        MockServerInputParser parser;

        auto p1 = buildMouseMovePacket(100, 100);
        auto p2 = buildMouseButtonPacket(100, 100, MouseButton::LEFT, true);
        auto p3 = buildKeyPacket(InputType::KEY_DOWN, 65, 30, 0);
        auto p4 = buildTextPacket("A");
        auto p5 = buildMouseButtonPacket(100, 100, MouseButton::LEFT, false);

        std::vector<uint8_t> combined;
        combined.insert(combined.end(), p1.begin(), p1.end());
        combined.insert(combined.end(), p2.begin(), p2.end());
        combined.insert(combined.end(), p3.begin(), p3.end());
        combined.insert(combined.end(), p4.begin(), p4.end());
        combined.insert(combined.end(), p5.begin(), p5.end());

        parser.feedData(combined.data(), combined.size());

        expect(parser.mouseMoves.size() == 1_i);
        expect(parser.mouseButtons.size() == 2_i);
        expect(parser.keys.size() == 1_i);
        expect(parser.texts.size() == 1_i);
    };

    "parse click sequence"_test = [] {
        MockServerInputParser parser;

        // Full click: move -> press -> release
        auto p1 = buildMouseMovePacket(200, 300);
        auto p2 = buildMouseButtonPacket(200, 300, MouseButton::LEFT, true);
        auto p3 = buildMouseButtonPacket(200, 300, MouseButton::LEFT, false);

        std::vector<uint8_t> combined;
        combined.insert(combined.end(), p1.begin(), p1.end());
        combined.insert(combined.end(), p2.begin(), p2.end());
        combined.insert(combined.end(), p3.begin(), p3.end());

        parser.feedData(combined.data(), combined.size());

        expect(parser.mouseMoves.size() == 1_i);
        expect(parser.mouseButtons.size() == 2_i);
        expect(parser.mouseButtons[0].pressed == true);
        expect(parser.mouseButtons[1].pressed == false);
    };

    "parse key combo"_test = [] {
        MockServerInputParser parser;

        // Ctrl+C: ctrl down, c down, c up, ctrl up
        auto p1 = buildKeyPacket(InputType::KEY_DOWN, 0x11, 29, MOD_CTRL);
        auto p2 = buildKeyPacket(InputType::KEY_DOWN, 67, 46, MOD_CTRL);
        auto p3 = buildKeyPacket(InputType::KEY_UP, 67, 46, MOD_CTRL);
        auto p4 = buildKeyPacket(InputType::KEY_UP, 0x11, 29, 0);

        std::vector<uint8_t> combined;
        combined.insert(combined.end(), p1.begin(), p1.end());
        combined.insert(combined.end(), p2.begin(), p2.end());
        combined.insert(combined.end(), p3.begin(), p3.end());
        combined.insert(combined.end(), p4.begin(), p4.end());

        parser.feedData(combined.data(), combined.size());

        expect(parser.keys.size() == 4_i);
        expect(parser.keys[0].down == true);
        expect(parser.keys[1].down == true);
        expect(parser.keys[2].down == false);
        expect(parser.keys[3].down == false);
    };

    "parse unicode text"_test = [] {
        MockServerInputParser parser;

        std::string utf8 = "\xC3\xA9\xC3\xA8";  // e-acute, e-grave
        auto packet = buildTextPacket(utf8);
        parser.feedData(packet.data(), packet.size());

        expect(parser.texts.size() == 1_i);
        expect(parser.texts[0].text == utf8);
    };

    "state persists across feeds"_test = [] {
        MockServerInputParser parser;

        auto packet = buildMouseMovePacket(99, 88);

        // Split in the middle of header
        parser.feedData(packet.data(), 3);
        expect(parser.mouseMoves.size() == 0_i);

        // Reset should clear partial state
        parser.reset();
        expect(parser.mouseMoves.size() == 0_i);

        // Now feed complete packet
        parser.feedData(packet.data(), packet.size());
        expect(parser.mouseMoves.size() == 1_i);
    };

    "large text input"_test = [] {
        MockServerInputParser parser;

        std::string longText(500, 'A');
        auto packet = buildTextPacket(longText);
        parser.feedData(packet.data(), packet.size());

        expect(parser.texts.size() == 1_i);
        expect(parser.texts[0].text.size() == 500_i);
    };
};

// Tests for callback dispatch behavior (simulates VncServer callback handling)
class CallbackDispatcher {
public:
    std::function<void(int16_t, int16_t)> onMouseMove;
    std::function<void(int16_t, int16_t, MouseButton, bool)> onMouseButton;
    std::function<void(uint32_t, uint32_t, uint8_t)> onKeyDown;
    std::function<void(const std::string&)> onTextInput;

    void dispatchInput(const InputHeader& hdr, const uint8_t* data) {
        switch (static_cast<InputType>(hdr.type)) {
            case InputType::MOUSE_MOVE:
                if (data && hdr.data_size >= sizeof(MouseMoveEvent) && onMouseMove) {
                    const MouseMoveEvent* m = reinterpret_cast<const MouseMoveEvent*>(data);
                    onMouseMove(m->x, m->y);
                }
                break;
            case InputType::MOUSE_BUTTON:
                if (data && hdr.data_size >= sizeof(MouseButtonEvent) && onMouseButton) {
                    const MouseButtonEvent* m = reinterpret_cast<const MouseButtonEvent*>(data);
                    onMouseButton(m->x, m->y, static_cast<MouseButton>(m->button), m->pressed != 0);
                }
                break;
            case InputType::KEY_DOWN:
                if (data && hdr.data_size >= sizeof(KeyEvent) && onKeyDown) {
                    const KeyEvent* k = reinterpret_cast<const KeyEvent*>(data);
                    onKeyDown(k->keycode, k->scancode, k->mods);
                }
                break;
            case InputType::TEXT_INPUT:
                if (data && hdr.data_size > 0 && onTextInput) {
                    std::string text(reinterpret_cast<const char*>(data), hdr.data_size);
                    onTextInput(text);
                }
                break;
            default:
                break;
        }
    }
};

suite callback_dispatch_tests = [] {
    "dispatch with null callback does not crash"_test = [] {
        CallbackDispatcher dispatcher;
        // All callbacks are null

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);

        MouseMoveEvent evt = {100, 200};

        // Should not crash even with null callback
        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));
        expect(true);  // Just checking we didn't crash
    };

    "dispatch mouse move invokes callback"_test = [] {
        CallbackDispatcher dispatcher;
        int16_t receivedX = 0, receivedY = 0;

        dispatcher.onMouseMove = [&](int16_t x, int16_t y) {
            receivedX = x;
            receivedY = y;
        };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);

        MouseMoveEvent evt = {150, 250};
        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));

        expect(receivedX == 150_i);
        expect(receivedY == 250_i);
    };

    "dispatch mouse button invokes callback"_test = [] {
        CallbackDispatcher dispatcher;
        int16_t rx = 0, ry = 0;
        MouseButton rBtn = MouseButton::LEFT;
        bool rPressed = false;

        dispatcher.onMouseButton = [&](int16_t x, int16_t y, MouseButton btn, bool pressed) {
            rx = x;
            ry = y;
            rBtn = btn;
            rPressed = pressed;
        };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_BUTTON);
        hdr.data_size = sizeof(MouseButtonEvent);

        MouseButtonEvent evt = {};
        evt.x = 100;
        evt.y = 200;
        evt.button = static_cast<uint8_t>(MouseButton::RIGHT);
        evt.pressed = 1;

        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));

        expect(rx == 100_i);
        expect(ry == 200_i);
        expect(rBtn == MouseButton::RIGHT);
        expect(rPressed == true);
    };

    "dispatch key down invokes callback"_test = [] {
        CallbackDispatcher dispatcher;
        uint32_t rKeycode = 0, rScancode = 0;
        uint8_t rMods = 0;

        dispatcher.onKeyDown = [&](uint32_t keycode, uint32_t scancode, uint8_t mods) {
            rKeycode = keycode;
            rScancode = scancode;
            rMods = mods;
        };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::KEY_DOWN);
        hdr.data_size = sizeof(KeyEvent);

        KeyEvent evt = {};
        evt.keycode = 65;
        evt.scancode = 30;
        evt.mods = MOD_SHIFT | MOD_CTRL;

        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));

        expect(rKeycode == 65_i);
        expect(rScancode == 30_i);
        expect(rMods == (MOD_SHIFT | MOD_CTRL));
    };

    "dispatch text input invokes callback"_test = [] {
        CallbackDispatcher dispatcher;
        std::string receivedText;

        dispatcher.onTextInput = [&](const std::string& text) {
            receivedText = text;
        };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
        hdr.data_size = 5;

        const char* text = "Hello";
        dispatcher.dispatchInput(hdr, reinterpret_cast<const uint8_t*>(text));

        expect(receivedText == "Hello");
    };

    "dispatch with null data does not crash"_test = [] {
        CallbackDispatcher dispatcher;
        bool called = false;
        dispatcher.onMouseMove = [&](int16_t, int16_t) { called = true; };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);

        // Null data - should not invoke callback
        dispatcher.dispatchInput(hdr, nullptr);
        expect(called == false);
    };

    "dispatch with undersized data does not crash"_test = [] {
        CallbackDispatcher dispatcher;
        bool called = false;
        dispatcher.onMouseMove = [&](int16_t, int16_t) { called = true; };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = 2;  // Too small for MouseMoveEvent

        uint8_t smallData[2] = {1, 2};
        dispatcher.dispatchInput(hdr, smallData);
        expect(called == false);
    };

    "callback set after dispatch setup"_test = [] {
        CallbackDispatcher dispatcher;

        // First dispatch with no callback
        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);
        MouseMoveEvent evt = {10, 20};

        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));
        // Should not crash

        // Now set callback
        int16_t rx = 0, ry = 0;
        dispatcher.onMouseMove = [&](int16_t x, int16_t y) {
            rx = x;
            ry = y;
        };

        // Second dispatch should now work
        MouseMoveEvent evt2 = {30, 40};
        dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt2));

        expect(rx == 30_i);
        expect(ry == 40_i);
    };

    "concurrent callback modification simulation"_test = [] {
        CallbackDispatcher dispatcher;
        std::atomic<int> callCount{0};

        // Simulate race: callback is set while dispatch might be happening
        dispatcher.onMouseMove = [&](int16_t, int16_t) {
            callCount++;
        };

        InputHeader hdr = {};
        hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
        hdr.data_size = sizeof(MouseMoveEvent);
        MouseMoveEvent evt = {100, 200};

        // Multiple dispatches
        for (int i = 0; i < 100; i++) {
            dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));
        }

        expect(callCount.load() == 100_i);

        // Clear callback
        dispatcher.onMouseMove = nullptr;

        // More dispatches should not crash
        for (int i = 0; i < 100; i++) {
            dispatcher.dispatchInput(hdr, reinterpret_cast<uint8_t*>(&evt));
        }

        expect(callCount.load() == 100_i);  // No more calls after clearing
    };
};
