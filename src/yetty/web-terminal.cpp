// Web Terminal - Terminal emulation for browser via Emscripten
// Exposes functions callable from JavaScript to display terminal output

#if YETTY_WEB

#include <emscripten.h>
#include <emscripten/html5.h>
#include <vterm.h>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

// Forward declarations
class Yetty;
extern Yetty* getYettyInstance();

// Terminal state
static VTerm* s_vterm = nullptr;
static VTermScreen* s_screen = nullptr;
static int s_rows = 24;
static int s_cols = 80;

// Cell buffer for rendering
struct WebCell {
    uint32_t codepoint;
    uint32_t fg;
    uint32_t bg;
    uint8_t attrs;
};
static std::vector<WebCell> s_cells;

// Cursor position
static int s_cursorRow = 0;
static int s_cursorCol = 0;
static bool s_cursorVisible = true;

// Command line buffer
static std::string s_inputBuffer;
static std::string s_prompt = "$ ";

// Callbacks for screen updates
static int screen_damage(VTermRect rect, void* user) {
    // Mark cells as dirty - will be read on next render
    return 1;
}

static int screen_moverect(VTermRect dest, VTermRect src, void* user) {
    return 1;
}

static int screen_movecursor(VTermPos pos, VTermPos oldpos, int visible, void* user) {
    s_cursorRow = pos.row;
    s_cursorCol = pos.col;
    s_cursorVisible = visible;
    return 1;
}

static VTermScreenCallbacks screen_callbacks = {
    .damage = screen_damage,
    .moverect = screen_moverect,
    .movecursor = screen_movecursor,
    .settermprop = nullptr,
    .bell = nullptr,
    .resize = nullptr,
    .sb_pushline = nullptr,
    .sb_popline = nullptr,
    .sb_clear = nullptr,
};

// Initialize web terminal
extern "C" EMSCRIPTEN_KEEPALIVE
void web_terminal_init(int rows, int cols) {
    s_rows = rows;
    s_cols = cols;

    // Create vterm
    s_vterm = vterm_new(rows, cols);
    vterm_set_utf8(s_vterm, 1);

    // Get screen and set callbacks
    s_screen = vterm_obtain_screen(s_vterm);
    vterm_screen_set_callbacks(s_screen, &screen_callbacks, nullptr);
    vterm_screen_reset(s_screen, 1);

    // Allocate cell buffer
    s_cells.resize(rows * cols);

    std::cout << "Web terminal initialized: " << cols << "x" << rows << std::endl;
}

// Write text to terminal (handles escape sequences)
extern "C" EMSCRIPTEN_KEEPALIVE
void web_terminal_write(const char* text, int len) {
    if (!s_vterm || !text) return;

    if (len < 0) len = strlen(text);
    vterm_input_write(s_vterm, text, len);
}

// Write plain text without escape sequence processing
extern "C" EMSCRIPTEN_KEEPALIVE
void web_terminal_print(const char* text) {
    if (!s_vterm || !text) return;
    vterm_input_write(s_vterm, text, strlen(text));
}

// Get cell at position (for JavaScript rendering)
extern "C" EMSCRIPTEN_KEEPALIVE
uint32_t web_terminal_get_cell_char(int row, int col) {
    if (!s_screen || row < 0 || row >= s_rows || col < 0 || col >= s_cols) {
        return ' ';
    }

    VTermPos pos = {row, col};
    VTermScreenCell cell;
    vterm_screen_get_cell(s_screen, pos, &cell);

    if (cell.chars[0] == 0) return ' ';
    return cell.chars[0];
}

// Get cursor position
extern "C" EMSCRIPTEN_KEEPALIVE
int web_terminal_get_cursor_row() {
    return s_cursorRow;
}

extern "C" EMSCRIPTEN_KEEPALIVE
int web_terminal_get_cursor_col() {
    return s_cursorCol;
}

// Get terminal dimensions
extern "C" EMSCRIPTEN_KEEPALIVE
int web_terminal_get_rows() {
    return s_rows;
}

extern "C" EMSCRIPTEN_KEEPALIVE
int web_terminal_get_cols() {
    return s_cols;
}

// Process keyboard input
extern "C" EMSCRIPTEN_KEEPALIVE
void web_terminal_key_input(int key, int mods) {
    if (!s_vterm) return;

    // Convert key to vterm key
    VTermKey vtkey = VTERM_KEY_NONE;
    VTermModifier vtmods = (VTermModifier)0;

    if (mods & 1) vtmods = (VTermModifier)(vtmods | VTERM_MOD_SHIFT);
    if (mods & 2) vtmods = (VTermModifier)(vtmods | VTERM_MOD_CTRL);
    if (mods & 4) vtmods = (VTermModifier)(vtmods | VTERM_MOD_ALT);

    switch (key) {
        case 13: // Enter
            vtkey = VTERM_KEY_ENTER;
            break;
        case 8: // Backspace
            vtkey = VTERM_KEY_BACKSPACE;
            break;
        case 9: // Tab
            vtkey = VTERM_KEY_TAB;
            break;
        case 27: // Escape
            vtkey = VTERM_KEY_ESCAPE;
            break;
        case 38: // Up
            vtkey = VTERM_KEY_UP;
            break;
        case 40: // Down
            vtkey = VTERM_KEY_DOWN;
            break;
        case 37: // Left
            vtkey = VTERM_KEY_LEFT;
            break;
        case 39: // Right
            vtkey = VTERM_KEY_RIGHT;
            break;
        case 36: // Home
            vtkey = VTERM_KEY_HOME;
            break;
        case 35: // End
            vtkey = VTERM_KEY_END;
            break;
        case 33: // PageUp
            vtkey = VTERM_KEY_PAGEUP;
            break;
        case 34: // PageDown
            vtkey = VTERM_KEY_PAGEDOWN;
            break;
        case 46: // Delete
            vtkey = VTERM_KEY_DEL;
            break;
        default:
            // For regular characters, use vterm_keyboard_unichar
            if (key >= 32 && key < 127) {
                vterm_keyboard_unichar(s_vterm, key, vtmods);
                return;
            }
            break;
    }

    if (vtkey != VTERM_KEY_NONE) {
        vterm_keyboard_key(s_vterm, vtkey, vtmods);
    }
}

// Read output from vterm (what the terminal wants to send to the shell)
extern "C" EMSCRIPTEN_KEEPALIVE
int web_terminal_read_output(char* buffer, int maxlen) {
    if (!s_vterm || !buffer || maxlen <= 0) return 0;

    size_t len = vterm_output_read(s_vterm, buffer, maxlen - 1);
    buffer[len] = '\0';
    return len;
}

// Cleanup
extern "C" EMSCRIPTEN_KEEPALIVE
void web_terminal_cleanup() {
    if (s_vterm) {
        vterm_free(s_vterm);
        s_vterm = nullptr;
        s_screen = nullptr;
    }
    s_cells.clear();
}

#endif // YETTY_WEB
