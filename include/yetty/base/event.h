#pragma once

#include "types.h"
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>

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
        SetFocus,
        // Resize
        Resize,
        // Poll
        PollReadable,
        // Timer
        Timer,
        // Context menu
        ContextMenuAction,
        // Card-local mouse events (dispatched by GPUScreen with local coords)
        CardMouseDown,
        CardMouseUp,
        CardMouseMove,
        CardScroll,
        // Tree manipulation
        Close,
        SplitPane,
        // Clipboard
        Copy,
        Paste
    };

    struct KeyEvent {
        int key;
        int mods;
        int scancode;
    };

    struct CharEvent {
        uint32_t codepoint;
        int mods;
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
        int mods;
    };

    struct SetFocusEvent {
        ObjectId objectId;
    };

    struct ResizeEvent {
        float width;
        float height;
    };

    struct PollEvent {
        int fd;
    };

    struct TimerEvent {
        int timerId;
    };

    struct ContextMenuActionEvent {
        ObjectId objectId;
        int row;
        int col;
        char action[32];
    };

    struct CardMouseEvent {
        ObjectId targetId;
        float x;  // card-local pixel x
        float y;  // card-local pixel y
        int button;
    };

    struct CardScrollEvent {
        ObjectId targetId;
        float x;  // card-local pixel x
        float y;  // card-local pixel y
        float dx;
        float dy;
        int mods;
    };

    struct CloseEvent {
        ObjectId objectId;
    };

    struct SplitPaneEvent {
        ObjectId objectId;   // target pane
        uint8_t orientation; // 0 = Horizontal, 1 = Vertical
    };

    Type type = Type::None;

    union {
        KeyEvent key;
        CharEvent chr;
        MouseEvent mouse;
        ScrollEvent scroll;
        SetFocusEvent setFocus;
        ResizeEvent resize;
        PollEvent poll;
        TimerEvent timer;
        ContextMenuActionEvent ctxMenu;
        CardMouseEvent cardMouse;
        CardScrollEvent cardScroll;
        CloseEvent closeEv;
        SplitPaneEvent splitPane;
    };

    // Optional heap-allocated payload, automatically freed when event goes out of scope.
    // Used by Copy/Paste events to carry strings; generic enough for any data type.
    // Handlers cast via std::static_pointer_cast<T>(event.payload).
    std::shared_ptr<void> payload;

    // Factory methods
    static Event keyDown(int key, int mods, int scancode = 0) {
        Event e;
        e.type = Type::KeyDown;
        e.key = {key, mods, scancode};
        return e;
    }

    static Event keyUp(int key, int mods, int scancode = 0) {
        Event e;
        e.type = Type::KeyUp;
        e.key = {key, mods, scancode};
        return e;
    }

    static Event charInput(uint32_t codepoint) {
        Event e;
        e.type = Type::Char;
        e.chr = {codepoint, 0};
        return e;
    }

    static Event charInputWithMods(uint32_t codepoint, int mods) {
        Event e;
        e.type = Type::Char;
        e.chr = {codepoint, mods};
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

    static Event scrollEvent(float x, float y, float dx, float dy, int mods = 0) {
        Event e;
        e.type = Type::Scroll;
        e.scroll = {x, y, dx, dy, mods};
        return e;
    }

    static Event focusEvent(ObjectId objectId) {
        Event e;
        e.type = Type::SetFocus;
        e.setFocus = {objectId};
        return e;
    }

    static Event resizeEvent(float width, float height) {
        Event e;
        e.type = Type::Resize;
        e.resize = {width, height};
        return e;
    }

    static Event timerEvent(int timerId) {
        Event e;
        e.type = Type::Timer;
        e.timer = {timerId};
        return e;
    }

    // Card-local mouse events (pre-computed local coordinates)
    static Event cardMouseDown(ObjectId targetId, float x, float y, int button) {
        Event e;
        e.type = Type::CardMouseDown;
        e.cardMouse = {targetId, x, y, button};
        return e;
    }

    static Event cardMouseUp(ObjectId targetId, float x, float y, int button) {
        Event e;
        e.type = Type::CardMouseUp;
        e.cardMouse = {targetId, x, y, button};
        return e;
    }

    static Event cardMouseMove(ObjectId targetId, float x, float y) {
        Event e;
        e.type = Type::CardMouseMove;
        e.cardMouse = {targetId, x, y, 0};
        return e;
    }

    static Event cardScrollEvent(ObjectId targetId, float x, float y, float dx, float dy, int mods = 0) {
        Event e;
        e.type = Type::CardScroll;
        e.cardScroll = {targetId, x, y, dx, dy, mods};
        return e;
    }

    static Event closeEvent(ObjectId objectId) {
        Event e;
        e.type = Type::Close;
        e.closeEv = {objectId};
        return e;
    }

    static Event splitPaneEvent(ObjectId objectId, uint8_t orientation) {
        Event e;
        e.type = Type::SplitPane;
        e.splitPane = {objectId, orientation};
        return e;
    }

    static Event copyEvent(std::shared_ptr<std::string> text) {
        Event e;
        e.type = Type::Copy;
        e.payload = std::move(text);
        return e;
    }

    static Event pasteEvent(std::shared_ptr<std::string> text) {
        Event e;
        e.type = Type::Paste;
        e.payload = std::move(text);
        return e;
    }

    // Context menu action with cell position payload
    static Event contextMenuAction(ObjectId objectId, const std::string& action, int row, int col) {
        Event e;
        e.type = Type::ContextMenuAction;
        e.ctxMenu.objectId = objectId;
        e.ctxMenu.row = row;
        e.ctxMenu.col = col;
        std::strncpy(e.ctxMenu.action, action.c_str(), sizeof(e.ctxMenu.action) - 1);
        e.ctxMenu.action[sizeof(e.ctxMenu.action) - 1] = '\0';
        return e;
    }
};

} // namespace base
} // namespace yetty
