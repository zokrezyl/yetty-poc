#pragma once

#include "types.h"
#include <cstdint>

namespace yetty {
namespace base {

struct Event {
    enum class Type {
        None,
        // Input events
        KeyDown,
        KeyUp,
        Char,
        MouseDown,
        MouseUp,
        MouseMove,
        MouseDrag,
        Scroll,
        // Focus events
        FocusPane,
        // Resize
        Resize
    };

    struct KeyEvent {
        int key;
        int mods;
    };

    struct CharEvent {
        uint32_t codepoint;
    };

    struct MouseEvent {
        float x;
        float y;
        int button;
    };

    struct ScrollEvent {
        float x;
        float y;
        float dx;
        float dy;
    };

    struct FocusPaneEvent {
        ObjectId objectId;
    };

    struct ResizeEvent {
        float width;
        float height;
    };

    Type type = Type::None;

    union {
        KeyEvent key;
        CharEvent chr;
        MouseEvent mouse;
        ScrollEvent scroll;
        FocusPaneEvent focus;
        ResizeEvent resize;
    };

    // Factory methods
    static Event keyDown(int key, int mods) {
        Event e;
        e.type = Type::KeyDown;
        e.key = {key, mods};
        return e;
    }

    static Event keyUp(int key, int mods) {
        Event e;
        e.type = Type::KeyUp;
        e.key = {key, mods};
        return e;
    }

    static Event charInput(uint32_t codepoint) {
        Event e;
        e.type = Type::Char;
        e.chr = {codepoint};
        return e;
    }

    static Event mouseDown(float x, float y, int button) {
        Event e;
        e.type = Type::MouseDown;
        e.mouse = {x, y, button};
        return e;
    }

    static Event mouseUp(float x, float y, int button) {
        Event e;
        e.type = Type::MouseUp;
        e.mouse = {x, y, button};
        return e;
    }

    static Event mouseMove(float x, float y) {
        Event e;
        e.type = Type::MouseMove;
        e.mouse = {x, y, 0};
        return e;
    }

    static Event mouseDrag(float x, float y, int button) {
        Event e;
        e.type = Type::MouseDrag;
        e.mouse = {x, y, button};
        return e;
    }

    static Event scrollEvent(float x, float y, float dx, float dy) {
        Event e;
        e.type = Type::Scroll;
        e.scroll = {x, y, dx, dy};
        return e;
    }

    static Event focusPane(ObjectId objectId) {
        Event e;
        e.type = Type::FocusPane;
        e.focus = {objectId};
        return e;
    }

    static Event resizeEvent(float width, float height) {
        Event e;
        e.type = Type::Resize;
        e.resize = {width, height};
        return e;
    }
};

} // namespace base
} // namespace yetty
