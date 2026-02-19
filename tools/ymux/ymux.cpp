#include "ymux.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

namespace ymux {

static constexpr int VENDOR_ID = 666666;

static constexpr char BASE64_CHARS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Multiplexer::Multiplexer(uint16_t cols, uint16_t rows)
    : _cols(cols), _rows(rows) {}

Multiplexer::~Multiplexer() = default;

std::string Multiplexer::base64Encode(const std::vector<uint8_t>& data) {
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);
    size_t i = 0;
    const size_t len = data.size();

    while (i + 2 < len) {
        uint32_t triple = (static_cast<uint32_t>(data[i]) << 16) |
                          (static_cast<uint32_t>(data[i + 1]) << 8) |
                          static_cast<uint32_t>(data[i + 2]);
        result.push_back(BASE64_CHARS[(triple >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(triple >> 6) & 0x3F]);
        result.push_back(BASE64_CHARS[triple & 0x3F]);
        i += 3;
    }
    if (i + 1 == len) {
        uint32_t val = static_cast<uint32_t>(data[i]) << 16;
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back('=');
        result.push_back('=');
    } else if (i + 2 == len) {
        uint32_t val = (static_cast<uint32_t>(data[i]) << 16) |
                       (static_cast<uint32_t>(data[i + 1]) << 8);
        result.push_back(BASE64_CHARS[(val >> 18) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 12) & 0x3F]);
        result.push_back(BASE64_CHARS[(val >> 6) & 0x3F]);
        result.push_back('=');
    }
    return result;
}

std::string Multiplexer::buildCreateOsc(const Pane& pane, int x, int y, int w, int h) {
    auto payload = pane.buildFullPayload();
    auto encoded = base64Encode(payload);

    std::string seq;
    seq += "\033]";
    seq += std::to_string(VENDOR_ID);
    seq += ";run -c ygrid";
    seq += " -x " + std::to_string(x);
    seq += " -y " + std::to_string(y);
    seq += " -w " + std::to_string(w);
    seq += " -h " + std::to_string(h);
    seq += " -r";
    seq += " --name " + pane.cardName();
    seq += ";;";
    seq += encoded;
    seq += "\033\\";
    return seq;
}

std::string Multiplexer::buildUpdateOsc(const Pane& pane, const std::vector<uint8_t>& payload) {
    if (payload.empty()) return {};

    auto encoded = base64Encode(payload);

    std::string seq;
    seq += "\033]";
    seq += std::to_string(VENDOR_ID);
    seq += ";update --name " + pane.cardName();
    seq += ";;";
    seq += encoded;
    seq += "\033\\";
    return seq;
}

void Multiplexer::sendCreateCard(Pane& pane, int x, int y, int w, int h) {
    auto seq = buildCreateOsc(pane, x, y, w, h);
    ssize_t wr = write(STDOUT_FILENO, seq.data(), seq.size());
    (void)wr;
}

void Multiplexer::sendUpdate(Pane& pane, const std::vector<uint8_t>& payload) {
    auto seq = buildUpdateOsc(pane, payload);
    if (seq.empty()) return;
    ssize_t wr = write(STDOUT_FILENO, seq.data(), seq.size());
    (void)wr;
}

bool Multiplexer::addPane(const std::string& shell) {
    auto pane = Pane::create(_cols, _rows, shell);
    if (!pane) return false;

    size_t idx = _panes.size();
    pane->setCardName("ymux-pane-" + std::to_string(idx));

    // Create ygrid card at position (0, 0) spanning full terminal
    sendCreateCard(*pane, 0, 0, _cols, _rows);

    _panes.push_back(std::move(pane));
    _activePane = idx;
    return true;
}

void Multiplexer::processStdinInput(const char* data, size_t len) {
    if (_escapeMode) {
        _escapeMode = false;
        if (len > 0) {
            char ch = data[0];
            switch (ch) {
            case 'c': // Create new pane
                addPane();
                return;
            case 'n': // Next pane
                if (!_panes.empty()) {
                    _activePane = (_activePane + 1) % _panes.size();
                }
                return;
            case 'p': // Previous pane
                if (!_panes.empty()) {
                    _activePane = (_activePane + _panes.size() - 1) % _panes.size();
                }
                return;
            case 'q': // Quit
                _running = false;
                return;
            case 0x02: // Ctrl-B again: send literal Ctrl-B
                break;
            default:
                break;
            }
        }
    }

    // Check for prefix key (Ctrl-B, like tmux)
    for (size_t i = 0; i < len; i++) {
        if (data[i] == 0x02) { // Ctrl-B
            // Send everything before the prefix to the active pane
            if (i > 0 && _activePane < _panes.size()) {
                _panes[_activePane]->sendKey(data, i);
            }
            _escapeMode = true;
            // Process rest recursively if there's more
            if (i + 1 < len) {
                processStdinInput(data + i + 1, len - i - 1);
            }
            return;
        }
    }

    // No prefix key found — send all to active pane
    if (_activePane < _panes.size()) {
        _panes[_activePane]->sendKey(data, len);
    }
}

int Multiplexer::run() {
    _running = true;

    // Put stdin in raw mode
    struct termios oldTerm, newTerm;
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    cfmakeraw(&newTerm);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);

    char buf[4096];

    while (_running) {
        // Build poll set: stdin + all pane PTY fds
        std::vector<struct pollfd> fds;

        // stdin
        fds.push_back({STDIN_FILENO, POLLIN, 0});

        // Pane PTYs
        for (auto& pane : _panes) {
            fds.push_back({pane->fd(), POLLIN, 0});
        }

        int ret = poll(fds.data(), fds.size(), 16); // 16ms timeout (~60fps)
        if (ret < 0) {
            if (errno == EINTR) continue;
            break;
        }

        // Process stdin
        if (fds[0].revents & POLLIN) {
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n > 0) {
                processStdinInput(buf, n);
            } else if (n == 0) {
                break; // EOF
            }
        }

        // Process pane PTY output
        for (size_t i = 0; i < _panes.size(); i++) {
            if (fds[i + 1].revents & POLLIN) {
                ssize_t n = read(_panes[i]->fd(), buf, sizeof(buf));
                if (n > 0) {
                    _panes[i]->feedInput(buf, n);
                } else if (n <= 0) {
                    // Pane closed — could remove it, for now just skip
                    continue;
                }
            }
        }

        // Send damage updates for all panes
        for (auto& pane : _panes) {
            if (pane->hasDamage()) {
                auto payload = pane->buildDamagePayload();
                sendUpdate(*pane, payload);
            }
        }

        // Clean up dead panes
        for (auto it = _panes.begin(); it != _panes.end();) {
            if (!(*it)->isAlive()) {
                if (_activePane >= static_cast<size_t>(it - _panes.begin()) && _activePane > 0) {
                    _activePane--;
                }
                it = _panes.erase(it);
            } else {
                ++it;
            }
        }

        if (_panes.empty()) {
            break; // All panes closed
        }
    }

    // Restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);

    return 0;
}

} // namespace ymux
