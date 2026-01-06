#include "input-handler.h"
#include <yetty/yetty.h>
#include "grid-renderer.h"

#include <iostream>

#if !YETTY_WEB
#include "terminal.h"
#include "plugin-manager.h"
#include <vterm.h>
#endif

#include <spdlog/spdlog.h>
#include <cstring>

namespace yetty {

// Helper macro to check Result in callbacks where we can't return errors
#define CHECK_RESULT(expr) \
    do { \
        if (auto _res = (expr); !_res) { \
            spdlog::error("{}: {}", #expr, _res.error().message()); \
        } \
    } while(0)

//-----------------------------------------------------------------------------
// InputHandler
//-----------------------------------------------------------------------------

InputHandler::InputHandler(Yetty* engine) noexcept
    : _engine(engine) {}

Result<InputHandler::Ptr> InputHandler::create(Yetty* engine) noexcept {
    auto p = Ptr(new InputHandler(engine));
    if (auto res = p->init(); !res) {
        return Err<Ptr>("Failed to init InputHandler", res);
    }
    return Ok(p);
}

Result<void> InputHandler::init() noexcept {
    return Ok();
}

void InputHandler::onMouseMove(double xpos, double ypos) noexcept {
    _mouseX = xpos;
    _mouseY = ypos;

#if !YETTY_WEB
    auto terminal = _engine->terminal();
    auto pluginManager = _engine->pluginManager();

    // Handle selection extension while dragging
    if (_selecting && terminal) {
        float cellWidth = _engine->cellWidth();
        float cellHeight = _engine->cellHeight();

        int col = static_cast<int>(xpos / cellWidth);
        int row = static_cast<int>(ypos / cellHeight);

        terminal->extendSelection(row, col);
        return;  // Don't route to plugins while selecting
    }

    // Route to plugins
    if (pluginManager && terminal) {
        float cellWidth = _engine->cellWidth();
        float cellHeight = _engine->cellHeight();
        int scrollOffset = terminal->getScrollOffset();

        pluginManager->onMouseMove(
            static_cast<float>(xpos), static_cast<float>(ypos),
            &terminal->getGrid(), cellWidth, cellHeight, scrollOffset);
    }
#endif
}

void InputHandler::onMouseButton(int button, int action, int mods) noexcept {
#if !YETTY_WEB
    auto terminal = _engine->terminal();
    auto pluginManager = _engine->pluginManager();
    GLFWwindow* window = _engine->window();

    if (!terminal) return;

    float cellWidth = _engine->cellWidth();
    float cellHeight = _engine->cellHeight();
    int scrollOffset = terminal->getScrollOffset();

    // Check if Shift is held - force selection mode even if app wants mouse
    bool shiftHeld = (mods & GLFW_MOD_SHIFT) != 0;

    // If app wants mouse events and Shift is not held, pass to app
    if (!shiftHeld && terminal->wantsMouseEvents()) {
        // Convert pixel coords to cell coords for vterm
        int col = static_cast<int>(_mouseX / cellWidth);
        int row = static_cast<int>(_mouseY / cellHeight);

        // Send mouse event to vterm (which sends to PTY)
        VTermModifier vtermMod = VTERM_MOD_NONE;
        if (mods & GLFW_MOD_CONTROL) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_CTRL);
        if (mods & GLFW_MOD_ALT) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_ALT);
        if (mods & GLFW_MOD_SHIFT) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_SHIFT);

        // Note: vterm buttons are 1-indexed (1=left, 2=middle, 3=right)
        int vtermButton = button + 1;
        // For now, skip direct vterm mouse - apps will work via plugin system
        (void)vtermButton;
        (void)vtermMod;
        (void)row;
        (void)col;
    }

    // Route to plugins first (unless selecting)
    if (!_selecting && pluginManager) {
        bool consumed = pluginManager->onMouseButton(
            button, action == GLFW_PRESS,
            static_cast<float>(_mouseX), static_cast<float>(_mouseY),
            &terminal->getGrid(), cellWidth, cellHeight, scrollOffset);

        if (consumed) return;
    }

    // Handle middle mouse button for paste
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        const char* clipboard = glfwGetClipboardString(window);
        if (clipboard && *clipboard) {
            terminal->clearSelection();
            terminal->sendRaw(clipboard, strlen(clipboard));
            spdlog::debug("Pasted {} bytes from clipboard (middle-click)", strlen(clipboard));
        }
        return;
    }

    // Handle left mouse button for selection
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        int col = static_cast<int>(_mouseX / cellWidth);
        int row = static_cast<int>(_mouseY / cellHeight);

        if (action == GLFW_PRESS) {
            // Detect double/triple click
            double currentTime = glfwGetTime();
            double timeSinceLastClick = currentTime - _lastClickTime;

            if (timeSinceLastClick < 0.3) {  // 300ms threshold
                _clickCount++;
                if (_clickCount > 3) _clickCount = 1;
            } else {
                _clickCount = 1;
            }
            _lastClickTime = currentTime;

            // Clear any existing selection on new click
            terminal->clearSelection();

            // Start selection based on click count
            SelectionMode mode = SelectionMode::Character;
            if (_clickCount == 2) {
                mode = SelectionMode::Word;
            } else if (_clickCount == 3) {
                mode = SelectionMode::Line;
            }

            terminal->startSelection(row, col, mode);
            _selecting = true;

        } else if (action == GLFW_RELEASE) {
            _selecting = false;

            // Copy selection to clipboard if any
            if (terminal->hasSelection()) {
                std::string selectedText = terminal->getSelectedText();
                if (!selectedText.empty()) {
                    glfwSetClipboardString(window, selectedText.c_str());
                    spdlog::debug("Copied {} bytes to clipboard", selectedText.size());
                }
            }
        }
    }
#else
    (void)button; (void)action; (void)mods;
#endif
}

void InputHandler::onKey(int key, int scancode, int action, int mods) noexcept {
#if !YETTY_WEB
    auto terminal = _engine->terminal();
    auto pluginManager = _engine->pluginManager();
    GLFWwindow* window = _engine->window();

    if (!terminal) return;

    // Handle clipboard shortcuts (Ctrl+Shift+C/V or Shift+Insert)
    if (action == GLFW_PRESS) {
        bool ctrlShift = (mods & GLFW_MOD_CONTROL) && (mods & GLFW_MOD_SHIFT);

        // Ctrl+Shift+C: Copy selection
        if (ctrlShift && key == GLFW_KEY_C) {
            if (terminal->hasSelection()) {
                std::string selectedText = terminal->getSelectedText();
                if (!selectedText.empty()) {
                    glfwSetClipboardString(window, selectedText.c_str());
                    spdlog::debug("Copied {} bytes to clipboard (Ctrl+Shift+C)", selectedText.size());
                }
            }
            return;
        }

        // Ctrl+Shift+V or Shift+Insert: Paste
        if ((ctrlShift && key == GLFW_KEY_V) ||
            ((mods & GLFW_MOD_SHIFT) && key == GLFW_KEY_INSERT)) {
            const char* clipboard = glfwGetClipboardString(window);
            if (clipboard && *clipboard) {
                // Clear selection before pasting
                terminal->clearSelection();
                // Send clipboard content to terminal
                terminal->sendRaw(clipboard, strlen(clipboard));
                spdlog::debug("Pasted {} bytes from clipboard", strlen(clipboard));
            }
            return;
        }

        // Escape clears selection
        if (key == GLFW_KEY_ESCAPE && terminal->hasSelection()) {
            terminal->clearSelection();
            // Don't return - still send ESC to terminal
        }
    }

    // Route to focused plugin first
    if (pluginManager) {
        if (pluginManager->onKey(key, scancode, action, mods)) {
            return;  // Plugin consumed the event
        }
    }

    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    VTermModifier vtermMod = VTERM_MOD_NONE;
    if (mods & GLFW_MOD_CONTROL) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_CTRL);
    if (mods & GLFW_MOD_ALT) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_ALT);
    if (mods & GLFW_MOD_SHIFT) vtermMod = (VTermModifier)(vtermMod | VTERM_MOD_SHIFT);

    // Handle Ctrl/Alt combinations - use glfwGetKeyName for correct keyboard layout
    if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT)) {
        const char* keyName = glfwGetKeyName(key, scancode);
        if (keyName && keyName[0] != '\0') {
            // Single character key name - send it with modifier
            if (keyName[1] == '\0') {
                uint32_t ch = keyName[0];
                spdlog::debug("Sending Ctrl/Alt+'{}'", (char)ch);
                terminal->sendKey(ch, vtermMod);
                return;
            }
        }
    }

    // Map GLFW keys to VTerm keys
    switch (key) {
        case GLFW_KEY_ENTER:
            terminal->sendSpecialKey(VTERM_KEY_ENTER, vtermMod);
            break;
        case GLFW_KEY_BACKSPACE:
            terminal->sendSpecialKey(VTERM_KEY_BACKSPACE, vtermMod);
            break;
        case GLFW_KEY_TAB:
            terminal->sendSpecialKey(VTERM_KEY_TAB, vtermMod);
            break;
        case GLFW_KEY_ESCAPE:
            terminal->sendSpecialKey(VTERM_KEY_ESCAPE, vtermMod);
            break;
        case GLFW_KEY_UP:
            terminal->sendSpecialKey(VTERM_KEY_UP, vtermMod);
            break;
        case GLFW_KEY_DOWN:
            terminal->sendSpecialKey(VTERM_KEY_DOWN, vtermMod);
            break;
        case GLFW_KEY_LEFT:
            terminal->sendSpecialKey(VTERM_KEY_LEFT, vtermMod);
            break;
        case GLFW_KEY_RIGHT:
            terminal->sendSpecialKey(VTERM_KEY_RIGHT, vtermMod);
            break;
        case GLFW_KEY_HOME:
            terminal->sendSpecialKey(VTERM_KEY_HOME, vtermMod);
            break;
        case GLFW_KEY_END:
            terminal->sendSpecialKey(VTERM_KEY_END, vtermMod);
            break;
        case GLFW_KEY_PAGE_UP:
            terminal->sendSpecialKey(VTERM_KEY_PAGEUP, vtermMod);
            break;
        case GLFW_KEY_PAGE_DOWN:
            terminal->sendSpecialKey(VTERM_KEY_PAGEDOWN, vtermMod);
            break;
        case GLFW_KEY_INSERT:
            terminal->sendSpecialKey(VTERM_KEY_INS, vtermMod);
            break;
        case GLFW_KEY_DELETE:
            terminal->sendSpecialKey(VTERM_KEY_DEL, vtermMod);
            break;
        default:
            break;
    }
#else
    (void)key; (void)scancode; (void)action; (void)mods;
#endif
}

void InputHandler::onChar(unsigned int codepoint) noexcept {
#if !YETTY_WEB
    auto terminal = _engine->terminal();
    auto pluginManager = _engine->pluginManager();

    if (!terminal) return;

    // Route to focused plugin first
    if (pluginManager) {
        if (pluginManager->onChar(codepoint)) {
            return;  // Plugin consumed the event
        }
    }

    terminal->sendKey(codepoint);
#else
    (void)codepoint;
#endif
}

void InputHandler::onScroll(double xoffset, double yoffset) noexcept {
    if (!_engine->renderer()) return;

    bool ctrlPressed = glfwGetKey(_engine->window(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                       glfwGetKey(_engine->window(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
    int mods = ctrlPressed ? GLFW_MOD_CONTROL : 0;

#if !YETTY_WEB
    auto terminal = _engine->terminal();
    auto pluginManager = _engine->pluginManager();

    // Check if scroll is over a plugin first (with or without Ctrl)
    if (pluginManager && terminal) {
        float cellWidth = _engine->cellWidth();
        float cellHeight = _engine->cellHeight();
        int scrollOffset = terminal->getScrollOffset();

        spdlog::debug("onScroll: yoffset={:.2f} ctrlPressed={} mouseX={:.1f} mouseY={:.1f} scrollOffset={}", 
                      yoffset, ctrlPressed, _mouseX, _mouseY, scrollOffset);

        bool consumed = pluginManager->onMouseScroll(
            static_cast<float>(xoffset), static_cast<float>(yoffset), mods,
            static_cast<float>(_mouseX), static_cast<float>(_mouseY),
            &terminal->getGrid(), cellWidth, cellHeight, scrollOffset);

        spdlog::debug("onScroll: plugin consumed={}", consumed);

        if (consumed) {
            spdlog::info("onScroll: Plugin consumed scroll event - NOT scrolling terminal");
            return;
        }
    }

    // Scroll through scrollback (no Ctrl, not over plugin)
    if (!ctrlPressed && terminal) {
        int lines = static_cast<int>(yoffset * 3);  // 3 lines per scroll notch
        spdlog::info("onScroll: Scrolling terminal by {} lines", lines);
        if (lines > 0) {
            terminal->scrollUp(lines);
        } else if (lines < 0) {
            terminal->scrollDown(-lines);
        }
        return;
    }
#endif

    // Zoom grid (Ctrl + wheel, not over plugin)
    float newZoom = _engine->zoomLevel() + static_cast<float>(yoffset) * 0.1f;
    newZoom = glm::clamp(newZoom, 0.2f, 5.0f);
    _engine->setZoomLevel(newZoom);

    // Recalculate grid size based on new cell size
    int w, h;
    glfwGetFramebufferSize(_engine->window(), &w, &h);
    float cellWidth = _engine->baseCellWidth() * newZoom;
    float cellHeight = _engine->baseCellHeight() * newZoom;
    uint32_t newCols = static_cast<uint32_t>(w / cellWidth);
    uint32_t newRows = static_cast<uint32_t>(h / cellHeight);

    if (newCols != _engine->cols() || newRows != _engine->rows()) {
        _engine->updateGridSize(newCols, newRows);
        std::cout << "Zoom: " << (newZoom * 100.0f) << "% -> Grid "
                  << newCols << "x" << newRows << std::endl;
    }
}

//-----------------------------------------------------------------------------
// GLFW Callback Functions
//-----------------------------------------------------------------------------

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* engine = static_cast<Yetty*>(glfwGetWindowUserPointer(window));
    if (!engine) return;

#if !YETTY_WEB
    auto inputHandler = std::dynamic_pointer_cast<InputHandler>(
        std::const_pointer_cast<InputHandler>(
            std::shared_ptr<const InputHandler>(engine->pluginManager() ?
                reinterpret_cast<const InputHandler*>(glfwGetWindowUserPointer(window)) : nullptr,
                [](const InputHandler*){})));
    // Actually we need to store InputHandler in the engine and get it from there
    // For now, cast through engine
#endif

    // The InputHandler is stored in the Yetty engine, we access it indirectly
    // through a static method or by storing it externally
    // TODO: Clean this up - for now use g_inputHandler
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window; (void)button; (void)action; (void)mods;
    // Implemented in Yetty::initCallbacks via lambda
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window; (void)key; (void)scancode; (void)action; (void)mods;
    // Implemented in Yetty::initCallbacks via lambda
}

void glfwCharCallback(GLFWwindow* window, unsigned int codepoint) {
    (void)window; (void)codepoint;
    // Implemented in Yetty::initCallbacks via lambda
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window; (void)xoffset; (void)yoffset;
    // Implemented in Yetty::initCallbacks via lambda
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window; (void)width; (void)height;
    // Implemented in Yetty::initCallbacks via lambda
}

} // namespace yetty
