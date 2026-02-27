#include <yetty/osc-scanner.h>

namespace yetty {

void OscScanner::scan(const char* data, size_t len) noexcept {
    if (data == nullptr || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        const char c = data[i];
        switch (_state) {
        case State::Normal:
            if (c == '\033') {
                _state = State::Esc;
            }
            break;

        case State::Esc:
            if (c == ']') {
                _state = State::InOsc;
            } else {
                _state = State::Normal;
            }
            break;

        case State::InOsc:
            if (c == '\007') {
                // BEL terminator
                _state = State::Normal;
                _completedCount++;
            } else if (c == '\033') {
                // Possible ST terminator start
                _state = State::OscEscEnd;
            }
            break;

        case State::OscEscEnd:
            if (c == '\\') {
                // ST terminator complete
                _state = State::Normal;
                _completedCount++;
            } else if (c == '\033') {
                // Another ESC - first one was body data, this new one might start ST
                // Stay in OscEscEnd
            } else {
                // ESC was part of OSC body, continue in OSC
                _state = State::InOsc;
            }
            break;
        }
    }
}

} // namespace yetty
