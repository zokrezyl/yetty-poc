// ydraw-zoo: Standalone terminal zoo animation
//
// Renders an animated network of connected nodes via YDrawBuffer, sends
// ydraw primitives to yetty via OSC 666666. Runs in alternate screen.

#include "zoo-renderer.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"
#include "../../src/yetty/ycat/osc.h"

#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <signal.h>

using namespace yetty;

//=============================================================================
// Terminal state
//=============================================================================

static struct termios g_origTermios;
static bool g_termRawMode = false;
static bool g_running = true;
static const char* g_cardName = "ydraw-zoo-app";

static void restoreTerminal() {
    if (g_termRawMode) {
        write(STDOUT_FILENO, "\033[?1049l", 8);
        tcsetattr(STDIN_FILENO, TCSANOW, &g_origTermios);
        g_termRawMode = false;
    }
}

static void signalHandler(int) { g_running = false; }

static void enterRawMode() {
    tcgetattr(STDIN_FILENO, &g_origTermios);
    struct termios raw = g_origTermios;
    cfmakeraw(&raw);
    raw.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    g_termRawMode = true;
    write(STDOUT_FILENO, "\033[?1049h", 8);  // alternate screen
}

static void getTerminalSize(int& cols, int& rows) {
    struct winsize ws = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        cols = ws.ws_col;
        rows = ws.ws_row;
    } else {
        cols = 80;
        rows = 24;
    }
}

//=============================================================================
// OSC helpers
//=============================================================================

static void sendOscCreate(int cols, int rows,
                          const std::vector<uint8_t>& binary) {
    auto b64 = ycat::base64Encode(binary);
    char header[512];
    std::snprintf(header, sizeof(header),
        "\033]666666;run -c ydraw -x 0 -y 0 -w %d -h %d -r --name %s;;",
        cols, rows, g_cardName);

    std::string seq;
    seq.reserve(strlen(header) + b64.size() + 4);
    seq += header;
    seq += b64;
    seq += "\033\\";
    seq = ycat::maybeWrapForTmux(seq);
    write(STDOUT_FILENO, seq.data(), seq.size());
}

static void sendOscUpdate(const std::vector<uint8_t>& binary) {
    auto b64 = ycat::base64Encode(binary);
    char header[512];
    std::snprintf(header, sizeof(header),
        "\033]666666;update --name %s;;", g_cardName);

    std::string seq;
    seq.reserve(strlen(header) + b64.size() + 4);
    seq += header;
    seq += b64;
    seq += "\033\\";
    seq = ycat::maybeWrapForTmux(seq);
    write(STDOUT_FILENO, seq.data(), seq.size());
}

static void sendOscKill() {
    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "\033]666666;kill --name %s\033\\", g_cardName);
    std::string seq = ycat::maybeWrapForTmux(buf);
    write(STDOUT_FILENO, seq.data(), seq.size());
}

//=============================================================================
// Time
//=============================================================================

static float now() {
    static auto start = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::steady_clock::now() - start;
    return std::chrono::duration<float>(elapsed).count();
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char** argv) {
    char logPath[256];
    std::snprintf(logPath, sizeof(logPath), "/tmp/ydraw-zoo-%d.log", getpid());
    auto fileLogger = spdlog::basic_logger_mt("ydraw-zoo", logPath, true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

    // Collect all args after argv[0] as renderer args
    std::string rendererArgs;
    for (int i = 1; i < argc; i++) {
        if (i > 1) rendererArgs += ' ';
        rendererArgs += argv[i];
    }

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    auto cfg = zoo::parseArgs(rendererArgs);
    zoo::ZooRenderer renderer(cfg);

    auto buffer = *YDrawBuffer::create();

    int cols, rows;
    getTerminalSize(cols, rows);

    enterRawMode();
    atexit(restoreTerminal);

    // Initial render
    float t = now();
    renderer.render(buffer, t);
    auto binary = buffer->serialize();
    sendOscCreate(cols, rows, binary);

    // Event loop: re-render at ~30fps, check for quit key
    char inputBuf[64];
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    while (g_running) {
        int ret = poll(&pfd, 1, 33);  // ~30fps
        if (ret < 0) {
            if (errno == EINTR) continue;
            break;
        }

        if (ret > 0 && (pfd.revents & POLLIN)) {
            int n = read(STDIN_FILENO, inputBuf, sizeof(inputBuf));
            if (n <= 0) break;
            for (int i = 0; i < n; i++) {
                char ch = inputBuf[i];
                if (ch == 'q' || ch == 'Q' || ch == 3 || ch == 27) {  // q, Ctrl+C, ESC
                    g_running = false;
                    break;
                }
            }
        }

        t = now();
        renderer.render(buffer, t);
        binary = buffer->serialize();
        sendOscUpdate(binary);
    }

    sendOscKill();
    restoreTerminal();
    return 0;
}
