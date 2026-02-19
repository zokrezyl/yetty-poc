#include "pane.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

#if __has_include(<pty.h>)
#include <pty.h>
#elif __has_include(<util.h>)
#include <util.h>
#endif

namespace ymux {

Pane::Pane(int ptyFd, pid_t childPid, uint16_t cols, uint16_t rows)
    : _ptyFd(ptyFd), _childPid(childPid), _cols(cols), _rows(rows)
    , _damagedCells(cols * rows, false)
{
    // Create vterm instance
    _vt = vterm_new(rows, cols);
    vterm_set_utf8(_vt, 1);

    // Get screen and set callbacks
    _vtScreen = vterm_obtain_screen(_vt);

    static VTermScreenCallbacks screenCbs = {};
    screenCbs.damage = Pane::onDamage;
    screenCbs.movecursor = Pane::onMoveCursor;

    vterm_screen_set_callbacks(_vtScreen, &screenCbs, this);
    vterm_screen_reset(_vtScreen, 1);
}

Pane::~Pane() {
    if (_vt) {
        vterm_free(_vt);
    }
    if (_ptyFd >= 0) {
        close(_ptyFd);
    }
    if (_childPid > 0) {
        kill(_childPid, SIGTERM);
        int status;
        waitpid(_childPid, &status, WNOHANG);
    }
}

Pane::Ptr Pane::create(uint16_t cols, uint16_t rows, const std::string& shell) {
    int masterFd;
    pid_t pid;
    struct winsize ws = {};
    ws.ws_col = cols;
    ws.ws_row = rows;

    pid = forkpty(&masterFd, nullptr, nullptr, &ws);
    if (pid < 0) {
        return nullptr;
    }

    if (pid == 0) {
        // Child process
        std::string sh = shell;
        if (sh.empty()) {
            const char* env = getenv("SHELL");
            sh = env ? env : "/bin/sh";
        }
        // Set TERM to something reasonable
        setenv("TERM", "xterm-256color", 1);
        execlp(sh.c_str(), sh.c_str(), nullptr);
        _exit(127);
    }

    // Parent
    return Ptr(new Pane(masterFd, pid, cols, rows));
}

void Pane::feedInput(const char* data, size_t len) {
    vterm_input_write(_vt, data, len);
}

void Pane::sendKey(const char* data, size_t len) {
    if (_ptyFd >= 0) {
        ssize_t wr = write(_ptyFd, data, len);
        (void)wr;
    }
}

GridCell Pane::readVTermCell(int row, int col) const {
    VTermPos pos = {row, col};
    VTermScreenCell cell;
    vterm_screen_get_cell(_vtScreen, pos, &cell);

    GridCell gc = {};
    gc.codepoint = (cell.chars[0] != 0) ? cell.chars[0] : ' ';
    gc.alpha = 255;

    // Extract colors
    if (VTERM_COLOR_IS_RGB(&cell.fg)) {
        gc.fgR = cell.fg.rgb.red;
        gc.fgG = cell.fg.rgb.green;
        gc.fgB = cell.fg.rgb.blue;
    } else if (VTERM_COLOR_IS_INDEXED(&cell.fg)) {
        // Convert indexed color - use vterm's built-in conversion
        VTermColor rgb = cell.fg;
        vterm_screen_convert_color_to_rgb(_vtScreen, &rgb);
        gc.fgR = rgb.rgb.red;
        gc.fgG = rgb.rgb.green;
        gc.fgB = rgb.rgb.blue;
    } else {
        // Default foreground
        gc.fgR = 200; gc.fgG = 200; gc.fgB = 200;
    }

    if (VTERM_COLOR_IS_RGB(&cell.bg)) {
        gc.bgR = cell.bg.rgb.red;
        gc.bgG = cell.bg.rgb.green;
        gc.bgB = cell.bg.rgb.blue;
    } else if (VTERM_COLOR_IS_INDEXED(&cell.bg)) {
        VTermColor rgb = cell.bg;
        vterm_screen_convert_color_to_rgb(_vtScreen, &rgb);
        gc.bgR = rgb.rgb.red;
        gc.bgG = rgb.rgb.green;
        gc.bgB = rgb.rgb.blue;
    } else {
        // Default background
        gc.bgR = 0; gc.bgG = 0; gc.bgB = 0;
    }

    // Pack style bits
    uint8_t style = 0;
    if (cell.attrs.bold)    style |= ATTR_BOLD;
    if (cell.attrs.italic)  style |= ATTR_ITALIC;
    if (cell.attrs.underline) style |= (cell.attrs.underline & 0x03) << ATTR_UNDERLINE_SHIFT;
    if (cell.attrs.strike)  style |= ATTR_STRIKETHROUGH;
    // Font type bits 5-7 left as 0 (MSDF) — ygrid will translate
    gc.style = style;

    return gc;
}

std::vector<uint8_t> Pane::buildFullPayload() const {
    size_t cellCount = static_cast<size_t>(_cols) * _rows;
    size_t payloadSize = 8 + cellCount * sizeof(GridCell);

    std::vector<uint8_t> payload(payloadSize);
    uint8_t* p = payload.data();

    // Header
    uint32_t magic = YGRD_MAGIC;
    std::memcpy(p, &magic, 4); p += 4;
    uint16_t flags = YGRD_FLAG_FULL;
    std::memcpy(p, &flags, 2); p += 2;
    *p++ = static_cast<uint8_t>(_cols);
    *p++ = static_cast<uint8_t>(_rows);

    // Cells
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            GridCell gc = readVTermCell(row, col);
            std::memcpy(p, &gc, sizeof(GridCell));
            p += sizeof(GridCell);
        }
    }

    return payload;
}

std::vector<uint8_t> Pane::buildDamagePayload() {
    // Count damaged cells
    uint32_t count = 0;
    for (size_t i = 0; i < _damagedCells.size(); i++) {
        if (_damagedCells[i]) count++;
    }

    if (count == 0) return {};

    // If more than half the cells are damaged, send full update
    size_t totalCells = static_cast<size_t>(_cols) * _rows;
    if (count > totalCells / 2) {
        _hasDamage = false;
        std::fill(_damagedCells.begin(), _damagedCells.end(), false);
        return buildFullPayload();
    }

    // Partial update
    size_t payloadSize = 8 + 4 + count * (2 + sizeof(GridCell));
    std::vector<uint8_t> payload(payloadSize);
    uint8_t* p = payload.data();

    // Header (partial — no FULL flag)
    uint32_t magic = YGRD_MAGIC;
    std::memcpy(p, &magic, 4); p += 4;
    uint16_t flags = 0;
    std::memcpy(p, &flags, 2); p += 2;
    *p++ = static_cast<uint8_t>(_cols);
    *p++ = static_cast<uint8_t>(_rows);

    // Count
    std::memcpy(p, &count, 4); p += 4;

    // Entries
    for (int row = 0; row < _rows; row++) {
        for (int col = 0; col < _cols; col++) {
            size_t idx = static_cast<size_t>(row) * _cols + col;
            if (_damagedCells[idx]) {
                *p++ = static_cast<uint8_t>(row);
                *p++ = static_cast<uint8_t>(col);
                GridCell gc = readVTermCell(row, col);
                std::memcpy(p, &gc, sizeof(GridCell));
                p += sizeof(GridCell);
            }
        }
    }

    _hasDamage = false;
    std::fill(_damagedCells.begin(), _damagedCells.end(), false);

    return payload;
}

void Pane::resize(uint16_t cols, uint16_t rows) {
    _cols = cols;
    _rows = rows;
    _damagedCells.resize(cols * rows, false);

    vterm_set_size(_vt, rows, cols);

    if (_ptyFd >= 0) {
        struct winsize ws = {};
        ws.ws_col = cols;
        ws.ws_row = rows;
        ioctl(_ptyFd, TIOCSWINSZ, &ws);
    }
}

bool Pane::isAlive() const {
    if (_childPid <= 0) return false;
    int status;
    pid_t result = waitpid(_childPid, &status, WNOHANG);
    return result == 0; // 0 means child still running
}

// VTerm callbacks
int Pane::onDamage(VTermRect rect, void* user) {
    auto* self = static_cast<Pane*>(user);
    for (int row = rect.start_row; row < rect.end_row; row++) {
        for (int col = rect.start_col; col < rect.end_col; col++) {
            size_t idx = static_cast<size_t>(row) * self->_cols + col;
            if (idx < self->_damagedCells.size()) {
                self->_damagedCells[idx] = true;
            }
        }
    }
    self->_hasDamage = true;
    if (self->_damageCb) {
        self->_damageCb(*self);
    }
    return 1;
}

int Pane::onMoveCursor(VTermPos pos, VTermPos oldpos, int visible, void* user) {
    auto* self = static_cast<Pane*>(user);
    self->_hasDamage = true;
    return 1;
}

} // namespace ymux
