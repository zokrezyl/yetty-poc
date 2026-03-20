// ydoc: Interactive document editor using YRich
//
// Creates a ydraw card and handles mouse/keyboard events for
// editing rich text documents inside a terminal.
//
// Usage:
//   ydoc -h 25                    # Interactive mode with sample content
//   ydoc -f doc.ydoc.yaml -h 25   # Load and edit file
//   ydoc -f doc.ydoc.yaml --view  # View mode (display and exit on 'q')

#include "../../src/yetty/yrich/ydoc.h"
#include "../../src/yetty/yrich/yrich-persist.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"

#include <args.hxx>
#include <uv.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

using namespace yetty;
using namespace yetty::yrich;

//=============================================================================
// Terminal mode helpers
//=============================================================================

static struct termios g_orig_termios;
static bool g_raw_mode = false;

static void disableRawMode() {
    if (g_raw_mode) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_orig_termios);
        g_raw_mode = false;
    }
}

static void enableRawMode() {
    tcgetattr(STDIN_FILENO, &g_orig_termios);
    atexit(disableRawMode);

    struct termios raw = g_orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    g_raw_mode = true;
}

//=============================================================================
// OSC output helpers
//=============================================================================

static void writeOsc(const char* data, size_t len) {
    write(STDOUT_FILENO, data, len);
}

static void subscribeClicks(bool enable) {
    writeOsc(enable ? "\033[?1500h" : "\033[?1500l", 8);
}

static void subscribeMoves(bool enable) {
    writeOsc(enable ? "\033[?1501h" : "\033[?1501l", 8);
}

static void subscribeViewChanges(bool enable) {
    writeOsc(enable ? "\033[?1502h" : "\033[?1502l", 8);
}

// Base64 encoding
static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64Encode(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(((size + 2) / 3) * 4);

    for (size_t i = 0; i < size; i += 3) {
        uint32_t n = ((uint32_t)data[i]) << 16;
        if (i + 1 < size) n |= ((uint32_t)data[i + 1]) << 8;
        if (i + 2 < size) n |= (uint32_t)data[i + 2];

        result += b64_table[(n >> 18) & 0x3F];
        result += b64_table[(n >> 12) & 0x3F];
        result += (i + 1 < size) ? b64_table[(n >> 6) & 0x3F] : '=';
        result += (i + 2 < size) ? b64_table[n & 0x3F] : '=';
    }
    return result;
}

//=============================================================================
// YDoc Editor - connects YRich to terminal
//=============================================================================

class YDocEditor {
public:
    YDocEditor() : _loop(uv_default_loop()) {}

    ~YDocEditor() {
        cleanup();
    }

    bool init(int x, int y, int w, int h, const std::string& filePath = "", bool dumpMode = false) {
        _cardX = x;
        _cardY = y;
        _cardW = w;
        _cardH = h;
        _dumpMode = dumpMode;

        // Create YDraw buffer
        auto bufferResult = YDrawBuffer::create();
        if (!bufferResult) {
            std::cerr << "Failed to create YDrawBuffer\n";
            return false;
        }
        _buffer = *bufferResult;

        // Load or create document
        if (!filePath.empty()) {
            auto docResult = DocumentPersist::loadYDoc(filePath);
            if (!docResult) {
                std::cerr << "Failed to load document: " << filePath << "\n";
                return false;
            }
            _doc = *docResult;
        } else {
            auto docResult = YDoc::create();
            if (!docResult) {
                std::cerr << "Failed to create YDoc\n";
                return false;
            }
            _doc = *docResult;

            // Initialize with sample text
            _doc->setPageWidth(600);
            _doc->addParagraph("Welcome to YDoc - a rich text editor in your terminal!");
            _doc->addParagraph("");
            _doc->addParagraph("This is a demonstration of the YRich document editing framework. "
                               "You can type text, navigate with arrow keys, select with shift, "
                               "and use standard editing shortcuts.");
            _doc->addParagraph("");
            _doc->addParagraph("Features:");
            _doc->addParagraph("- Real-time collaborative editing support");
            _doc->addParagraph("- Undo/redo via operation-based commands");
            _doc->addParagraph("- Mouse selection and cursor positioning");
            _doc->addParagraph("- Word wrap and multi-paragraph documents");
            _doc->addParagraph("");
            _doc->addParagraph("Press 'q' to quit.");
        }

        _doc->setBuffer(_buffer.get());

        // Set up dirty callback to trigger re-render
        _doc->setDirtyCallback([this]() {
            _needsRender = true;
        });

        _doc->setCursor(0, 0);

        return true;
    }

    void run() {
        // Dump mode: send card and exit immediately
        if (_dumpMode) {
            sendCreateCard();
            return;
        }

        enableRawMode();

        // Create card
        sendCreateCard();

        // Subscribe to events
        subscribeClicks(true);
        subscribeMoves(true);
        subscribeViewChanges(true);

        // Set up stdin polling
        uv_poll_init(_loop, &_stdinPoll, STDIN_FILENO);
        _stdinPoll.data = this;
        uv_poll_start(&_stdinPoll, UV_READABLE, onStdinReadable);

        // Set up render timer (60fps max)
        uv_timer_init(_loop, &_renderTimer);
        _renderTimer.data = this;
        uv_timer_start(&_renderTimer, onRenderTimer, 16, 16);

        // Run event loop
        _running = true;
        while (_running) {
            uv_run(_loop, UV_RUN_NOWAIT);
        }

        // Cleanup
        uv_poll_stop(&_stdinPoll);
        uv_timer_stop(&_renderTimer);

        subscribeClicks(false);
        subscribeMoves(false);
        subscribeViewChanges(false);

        sendKillCard();
    }

private:
    void cleanup() {
        disableRawMode();
    }

    void sendCreateCard() {
        _doc->render();
        auto bytes = _buffer->serialize();

        std::string seq = "\033]666666;run -c ydraw -x " + std::to_string(_cardX) +
                          " -y " + std::to_string(_cardY) +
                          " -w " + std::to_string(_cardW) +
                          " -h " + std::to_string(_cardH) +
                          " -r --name " + _cardName + ";;";
        seq += base64Encode(bytes.data(), bytes.size());
        seq += "\033\\";

        writeOsc(seq.c_str(), seq.size());
    }

    void sendUpdateCard() {
        _doc->render();
        auto bytes = _buffer->serialize();

        std::string seq = "\033]666666;update --name " + _cardName + ";;";
        seq += base64Encode(bytes.data(), bytes.size());
        seq += "\033\\";

        writeOsc(seq.c_str(), seq.size());
    }

    void sendKillCard() {
        std::string seq = "\033]666666;kill --name " + _cardName + "\033\\";
        writeOsc(seq.c_str(), seq.size());
    }

    // Process incoming data from stdin
    void processInput() {
        char buf[4096];
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n <= 0) return;

        for (ssize_t i = 0; i < n; ++i) {
            processChar(buf[i]);
        }
    }

    void processChar(char c) {
        // Accumulate into buffer for OSC parsing
        if (_escState == EscState::None) {
            if (c == '\033') {
                _escState = EscState::Esc;
                _escBuffer.clear();
                _escBuffer += c;
            } else {
                // Regular character - keyboard input
                handleKeyChar(c);
            }
        } else {
            _escBuffer += c;

            if (_escState == EscState::Esc) {
                if (c == ']') {
                    _escState = EscState::Osc;
                } else if (c == '[') {
                    _escState = EscState::Csi;
                } else {
                    // Unknown escape, reset
                    _escState = EscState::None;
                }
            } else if (_escState == EscState::Osc) {
                // OSC ends with ST (\033\\) or BEL (\007)
                if (c == '\\' && _escBuffer.size() >= 2 &&
                    _escBuffer[_escBuffer.size() - 2] == '\033') {
                    handleOsc(_escBuffer);
                    _escState = EscState::None;
                } else if (c == '\007') {
                    handleOsc(_escBuffer);
                    _escState = EscState::None;
                }
            } else if (_escState == EscState::Csi) {
                // CSI ends with a letter
                if (c >= 0x40 && c <= 0x7E) {
                    handleCsi(_escBuffer);
                    _escState = EscState::None;
                }
            }
        }
    }

    void handleOsc(const std::string& seq) {
        if (seq.size() < 10) return;

        size_t start = 2;
        size_t semicolon = seq.find(';', start);
        if (semicolon == std::string::npos) return;

        std::string code = seq.substr(start, semicolon - start);
        std::string rest = seq.substr(semicolon + 1);

        if (rest.size() >= 2 && rest[rest.size() - 2] == '\033') {
            rest = rest.substr(0, rest.size() - 2);
        } else if (!rest.empty() && rest.back() == '\007') {
            rest.pop_back();
        }

        if (code == "777777") {
            handleMouseClick(rest);
        } else if (code == "777778") {
            handleMouseMove(rest);
        } else if (code == "777779") {
            handleViewChange(rest);
        } else if (code == "777780") {
            handlePixelSize(rest);
        }
    }

    void handleCsi(const std::string& seq) {
        if (seq.size() < 3) return;

        char cmd = seq.back();
        InputModifiers mods = getModifiers();

        // Check for modifier in CSI sequence (e.g., \033[1;5C for Ctrl+Right)
        if (seq.size() >= 5 && seq[seq.size() - 2] == '5') {
            mods.ctrl = true;
        }
        if (seq.size() >= 5 && seq[seq.size() - 2] == '2') {
            mods.shift = true;
        }

        if (cmd == 'A') {
            _doc->onKeyDown(Key::Up, mods);
        } else if (cmd == 'B') {
            _doc->onKeyDown(Key::Down, mods);
        } else if (cmd == 'C') {
            _doc->onKeyDown(Key::Right, mods);
        } else if (cmd == 'D') {
            _doc->onKeyDown(Key::Left, mods);
        } else if (cmd == 'H') {
            _doc->onKeyDown(Key::Home, mods);
        } else if (cmd == 'F') {
            _doc->onKeyDown(Key::End, mods);
        } else if (cmd == '~') {
            // Handle special keys like Delete
            if (seq.find("3") != std::string::npos) {
                _doc->onKeyDown(Key::Delete, mods);
            }
        }
    }

    void handleKeyChar(char c) {
        if (c == 'q' || c == 'Q') {
            _running = false;
            return;
        }

        InputModifiers mods = getModifiers();

        // Ctrl+key handling (Ctrl+A = 1, Ctrl+B = 2, etc.)
        if (c >= 1 && c <= 26) {
            mods.ctrl = true;
            char letter = 'a' + c - 1;

            if (letter == 'b') {
                // Ctrl+B - toggle bold
                _doc->toggleBold();
                return;
            } else if (letter == 'i') {
                // Ctrl+I - toggle italic
                _doc->toggleItalic();
                return;
            } else if (letter == 'u') {
                // Ctrl+U - toggle underline
                _doc->toggleUnderline();
                return;
            } else if (letter == 'c') {
                // Ctrl+C - copy
                _clipboard = _doc->copySelection();
                return;
            } else if (letter == 'v') {
                // Ctrl+V - paste
                if (!_clipboard.empty()) {
                    _doc->paste(_clipboard);
                }
                return;
            } else if (letter == 'x') {
                // Ctrl+X - cut
                _clipboard = _doc->copySelection();
                _doc->deleteSelection();
                return;
            } else if (letter == 'z') {
                // Ctrl+Z - undo
                _doc->undo();
                return;
            } else if (letter == 'y') {
                // Ctrl+Y - redo
                _doc->redo();
                return;
            } else if (letter == 'a') {
                // Ctrl+A - select all (handled by document)
                // TODO: implement select all
                return;
            }
        }

        if (c == '\r' || c == '\n') {
            _doc->onKeyDown(Key::Enter, mods);
        } else if (c == '\t') {
            _doc->onKeyDown(Key::Tab, mods);
        } else if (c == 127 || c == '\b') {
            _doc->onKeyDown(Key::Backspace, mods);
        } else if (c >= 32 && c < 127) {
            char str[2] = {c, 0};
            _doc->onTextInput(str);
        }
    }

    void handleMouseClick(const std::string& data) {
        std::vector<std::string> parts;
        splitString(data, ';', parts);
        if (parts.size() < 5) return;

        if (parts[0] != _cardName) return;

        int buttons = std::stoi(parts[1]);
        int press = std::stoi(parts[2]);
        float x = std::stof(parts[3]);
        float y = std::stof(parts[4]);

        float sceneX = transformX(x);
        float sceneY = transformY(y);

        InputModifiers mods = getModifiers();
        MouseButton button = MouseButton::Left;
        if (buttons & 2) button = MouseButton::Right;
        if (buttons & 4) button = MouseButton::Middle;

        if (press) {
            if (_lastClickTime > 0 &&
                (uv_now(_loop) - _lastClickTime) < 300 &&
                std::abs(_lastClickX - sceneX) < 5 &&
                std::abs(_lastClickY - sceneY) < 5) {
                _doc->onMouseDoubleClick(sceneX, sceneY, button, mods);
                _lastClickTime = 0;
            } else {
                _doc->onMouseDown(sceneX, sceneY, button, mods);
                _lastClickTime = uv_now(_loop);
                _lastClickX = sceneX;
                _lastClickY = sceneY;
            }
            _mouseDown = true;
            _mouseButton = button;
        } else {
            _doc->onMouseUp(sceneX, sceneY, button, mods);
            _mouseDown = false;
        }
    }

    void handleMouseMove(const std::string& data) {
        std::vector<std::string> parts;
        splitString(data, ';', parts);
        if (parts.size() < 4) return;

        if (parts[0] != _cardName) return;

        float x = std::stof(parts[2]);
        float y = std::stof(parts[3]);

        float sceneX = transformX(x);
        float sceneY = transformY(y);

        InputModifiers mods = getModifiers();

        if (_mouseDown) {
            _doc->onMouseDrag(sceneX, sceneY, _mouseButton, mods);
        } else {
            _doc->onMouseMove(sceneX, sceneY, mods);
        }
    }

    void handleViewChange(const std::string& data) {
        std::vector<std::string> parts;
        splitString(data, ';', parts);
        if (parts.size() < 4) return;

        if (parts[0] != _cardName) return;

        _zoom = std::stof(parts[1]);
        _scrollX = std::stof(parts[2]);
        _scrollY = std::stof(parts[3]);
    }

    void handlePixelSize(const std::string& data) {
        std::vector<std::string> parts;
        splitString(data, ';', parts);
        if (parts.size() < 3) return;

        if (parts[0] != _cardName) return;

        _displayWidth = std::stof(parts[1]);
        _displayHeight = std::stof(parts[2]);
        _havePixelSize = true;
    }

    float transformX(float displayX) const {
        if (!_havePixelSize || _displayWidth <= 0) return displayX;

        float contentW = _doc->contentWidth();
        float visibleW = contentW / _zoom;
        return _scrollX + (displayX / _displayWidth) * visibleW;
    }

    float transformY(float displayY) const {
        if (!_havePixelSize || _displayHeight <= 0) return displayY;

        float contentH = _doc->contentHeight();
        float visibleH = contentH / _zoom;
        return _scrollY + (displayY / _displayHeight) * visibleH;
    }

    InputModifiers getModifiers() const {
        return {};
    }

    static void splitString(const std::string& str, char delim,
                           std::vector<std::string>& out) {
        size_t start = 0;
        size_t end;
        while ((end = str.find(delim, start)) != std::string::npos) {
            out.push_back(str.substr(start, end - start));
            start = end + 1;
        }
        out.push_back(str.substr(start));
    }

    static void onStdinReadable(uv_poll_t* handle, int status, int events) {
        if (status < 0 || !(events & UV_READABLE)) return;
        auto* self = static_cast<YDocEditor*>(handle->data);
        self->processInput();
    }

    static void onRenderTimer(uv_timer_t* handle) {
        auto* self = static_cast<YDocEditor*>(handle->data);
        if (self->_needsRender) {
            self->sendUpdateCard();
            self->_needsRender = false;
        }
    }

    // State
    uv_loop_t* _loop;
    uv_poll_t _stdinPoll;
    uv_timer_t _renderTimer;
    bool _running = false;
    bool _needsRender = true;
    bool _dumpMode = false;

    // Card
    std::string _cardName = "ydoc0";
    int _cardX = 0, _cardY = 0, _cardW = 0, _cardH = 0;

    // Display
    float _displayWidth = 0;
    float _displayHeight = 0;
    bool _havePixelSize = false;
    float _zoom = 1.0f;
    float _scrollX = 0, _scrollY = 0;

    // Mouse
    bool _mouseDown = false;
    MouseButton _mouseButton = MouseButton::Left;
    uint64_t _lastClickTime = 0;
    float _lastClickX = 0, _lastClickY = 0;

    // Input parsing
    enum class EscState { None, Esc, Osc, Csi };
    EscState _escState = EscState::None;
    std::string _escBuffer;

    // Document
    YDrawBuffer::Ptr _buffer;
    YDoc::Ptr _doc;

    // Clipboard
    std::string _clipboard;
};

//=============================================================================
// Main
//=============================================================================

int main(int argc, char** argv) {
    args::ArgumentParser parser("ydoc - Interactive document editor");
    args::HelpFlag help(parser, "help", "Show help", {"help"});
    args::ValueFlag<int> xFlag(parser, "x", "Card X position (cells)", {'x'});
    args::ValueFlag<int> yFlag(parser, "y", "Card Y position (cells)", {'y'});
    args::ValueFlag<int> wFlag(parser, "w", "Card width (cells, 0=stretch)", {'w'});
    args::ValueFlag<int> hFlag(parser, "h", "Card height (cells)", {'h'});
    args::ValueFlag<std::string> fileFlag(parser, "file", "Load document from file", {'f', "file"});
    args::Flag dumpFlag(parser, "dump", "Dump mode (display and exit immediately)", {"dump"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << "\n";
        std::cerr << parser;
        return 1;
    }

    int x = xFlag ? args::get(xFlag) : 0;
    int y = yFlag ? args::get(yFlag) : 0;
    int w = wFlag ? args::get(wFlag) : 0;
    int h = hFlag ? args::get(hFlag) : 25;
    std::string filePath = fileFlag ? args::get(fileFlag) : "";
    bool dumpMode = dumpFlag;

    YDocEditor editor;
    if (!editor.init(x, y, w, h, filePath, dumpMode)) {
        return 1;
    }

    editor.run();
    return 0;
}
