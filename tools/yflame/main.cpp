// yflame: Interactive flamegraph visualizer for yetty
//
// Usage:
//   yflame -- ./my_program args...     Run program under perf, live flamegraph
//   yflame -p PID                      Attach to running process
//   yflame -f stacks.txt [-l]          Read collapsed stacks from file

#include "flame-renderer.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"
#include "../../src/yetty/ycat/osc.h"

#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>

using namespace yetty;

//=============================================================================
// Terminal state
//=============================================================================

static struct termios g_origTermios;
static bool g_termRawMode = false;
static bool g_running = true;
static const char* g_cardName = "yflame-app";

static void restoreTerminal() {
    if (g_termRawMode) {
        write(STDOUT_FILENO, "\033[?1003l\033[?1006l\033[?1000l", 24);
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
    write(STDOUT_FILENO, "\033[?1049h", 8);
    write(STDOUT_FILENO, "\033[?1000h\033[?1003h\033[?1006h", 24);
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
// File helpers
//=============================================================================

static std::string readFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return {};
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static timespec getFileMtime(const std::string& path) {
    struct stat st = {};
    stat(path.c_str(), &st);
    return st.st_mtim;
}

//=============================================================================
// Input handling
//=============================================================================

struct MouseEvent {
    int button;     // 0=left, 1=middle, 2=right, 64=scrollup, 65=scrolldown
    int col;        // 1-based
    int row;        // 1-based
    bool press;     // true=press, false=release
    bool motion;    // true=mouse move event
};

// Parse SGR mouse: ESC [ < Cb ; Cx ; Cy M/m
// Adapted from ybrowser.
static bool parseSgrMouse(const char* buf, int len, MouseEvent& ev, int& consumed) {
    if (len < 3 || buf[0] != '\033' || buf[1] != '[' || buf[2] != '<')
        return false;

    int cb = 0, cx = 0, cy = 0;
    int i = 3;
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cb = cb * 10 + (buf[i++] - '0');
    if (i >= len || buf[i] != ';') return false;
    i++;
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cx = cx * 10 + (buf[i++] - '0');
    if (i >= len || buf[i] != ';') return false;
    i++;
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cy = cy * 10 + (buf[i++] - '0');
    if (i >= len) return false;

    if (buf[i] != 'M' && buf[i] != 'm') return false;
    ev.press = (buf[i] == 'M');
    consumed = i + 1;

    ev.motion = (cb & 0x20) != 0;
    // Strip modifier/motion bits, keep button + scroll extension
    ev.button = (cb & 0x03) | (cb & 0xC0);
    ev.col = cx;
    ev.row = cy;
    return true;
}

//=============================================================================
// Usage
//=============================================================================

static void printUsage(const char* prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s -- <command> [args...]   Profile a command\n"
        "  %s -p <pid>                 Attach to running process\n"
        "  %s -f <file> [-l]           Read collapsed stacks (optionally live-watch)\n"
        "  %s -d <perf.data>           Read perf.data file directly\n",
        prog, prog, prog, prog);
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char** argv) {
    char logPath[256];
    std::snprintf(logPath, sizeof(logPath), "/tmp/yflame-%d.log", getpid());
    auto fileLogger = spdlog::basic_logger_mt("yflame", logPath, true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

    // Parse CLI args
    enum class Mode { None, Command, Pid, File, PerfData } mode = Mode::None;
    std::string filePath;
    std::string perfDataPath;
    bool liveFileWatch = false;
    pid_t attachPid = 0;
    std::vector<std::string> targetCmd;
    int snapshotIntervalFrames = 60;  // ~2s at 30fps

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--") {
            // Everything after -- is the command
            mode = Mode::Command;
            for (int j = i + 1; j < argc; j++) {
                targetCmd.push_back(argv[j]);
            }
            break;
        } else if ((arg == "-p" || arg == "--pid") && i + 1 < argc) {
            mode = Mode::Pid;
            attachPid = static_cast<pid_t>(std::stoi(argv[++i]));
        } else if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            mode = Mode::File;
            filePath = argv[++i];
        } else if ((arg == "-d" || arg == "--data") && i + 1 < argc) {
            mode = Mode::PerfData;
            perfDataPath = argv[++i];
        } else if (arg == "-l" || arg == "--live") {
            liveFileWatch = true;
        }
    }

    if (mode == Mode::None) {
        printUsage(argv[0]);
        return 1;
    }

    flame::FlameConfig cfg;
    flame::FlameRenderer renderer(cfg);
    flame::ViewState view;
    flame::PerfProfiler profiler;

    // Initialize based on mode
    bool usePerfProfiler = false;

    switch (mode) {
    case Mode::Command:
        if (targetCmd.empty()) {
            fprintf(stderr, "Error: no command specified after --\n");
            return 1;
        }
        if (!profiler.startCommand(targetCmd)) {
            fprintf(stderr, "Error: failed to start profiling '%s'\n",
                    targetCmd[0].c_str());
            return 1;
        }
        usePerfProfiler = true;
        // Wait for perf to finish (short commands) or timeout for long ones
        if (profiler.waitForPerf(3000)) {
            // Command finished — take final snapshot
            auto collapsed = profiler.snapshot();
            yinfo("command finished, snapshot: {} bytes collapsed", collapsed.size());
            if (!collapsed.empty()) {
                renderer.loadCollapsedStacks(collapsed);
            }
            usePerfProfiler = false;  // no more snapshots needed
        } else {
            // Long-running command — take first snapshot, continue live
            auto collapsed = profiler.snapshot();
            yinfo("long-running, first snapshot: {} bytes collapsed", collapsed.size());
            if (!collapsed.empty()) {
                renderer.loadCollapsedStacks(collapsed);
            }
        }
        break;

    case Mode::Pid:
        if (!profiler.attachPid(attachPid)) {
            fprintf(stderr, "Error: failed to attach to pid %d\n", attachPid);
            return 1;
        }
        usePerfProfiler = true;
        // Wait briefly then take initial snapshot
        usleep(1000000);
        {
            auto collapsed = profiler.snapshot();
            if (!collapsed.empty()) {
                renderer.loadCollapsedStacks(collapsed);
            }
        }
        break;

    case Mode::File: {
        auto data = readFile(filePath);
        if (data.empty()) {
            fprintf(stderr, "Error: cannot read '%s' or file is empty\n",
                    filePath.c_str());
            return 1;
        }
        renderer.loadCollapsedStacks(data);
        break;
    }
    case Mode::PerfData: {
        auto collapsed = profiler.loadPerfData(perfDataPath);
        if (collapsed.empty()) {
            fprintf(stderr, "Error: no stacks from '%s' (run perf script -i to check)\n",
                    perfDataPath.c_str());
            return 1;
        }
        renderer.loadCollapsedStacks(collapsed);
        break;
    }
    default:
        return 1;
    }

    auto lastMtime = filePath.empty() ? timespec{} : getFileMtime(filePath);

    yinfo("yflame started: mode={} samples={} maxDepth={}",
          static_cast<int>(mode), renderer.totalSamples(), renderer.maxDepth());
    yinfo("scene: {}x{} frameH={:.1f} fontSize={:.1f}",
          cfg.sceneWidth, cfg.sceneHeight,
          renderer.dynamicFrameHeight(),
          renderer.dynamicFrameHeight() * 0.55f);

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    auto buffer = *YDrawBuffer::create();

    int cols, rows;
    getTerminalSize(cols, rows);

    enterRawMode();
    atexit(restoreTerminal);

    yinfo("terminal: {}x{} cols x rows", cols, rows);

    // Initial render
    renderer.render(buffer, view);
    auto binary = buffer->serialize();
    sendOscCreate(cols, rows, binary);
    yinfo("initial render: {} bytes serialized", binary.size());

    // Event loop at ~30fps
    char inputBuf[64];
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    bool needsRedraw = false;
    int frameCount = 0;
    flame::HitFrame hoverFrame;
    float hoverSceneX = 0, hoverSceneY = 0;

    while (g_running) {
        int ret = poll(&pfd, 1, 33);
        if (ret < 0) {
            if (errno == EINTR) continue;
            break;
        }

        if (ret > 0 && (pfd.revents & POLLIN)) {
            int n = read(STDIN_FILENO, inputBuf, sizeof(inputBuf));
            if (n <= 0) break;
            yinfo("input: {} bytes, first=0x{:02x}", n,
                  static_cast<unsigned char>(inputBuf[0]));

            float panStep = 30.0f;
            int i = 0;
            while (i < n) {
                // Try SGR mouse first
                MouseEvent mev;
                int consumed;
                if (parseSgrMouse(inputBuf + i, n - i, mev, consumed)) {
                    i += consumed;
                    yinfo("mouse: btn={} col={} row={} press={}",
                          mev.button, mev.col, mev.row, mev.press);

                    if (mev.button == 64 || mev.button == 65) {
                        // Scroll: 64=up(zoom in), 65=down(zoom out)
                        if (mev.button == 64) view.zoomIn();
                        else                  view.zoomOut();
                        yinfo("scroll: zoom={:.2f} offset=({:.1f},{:.1f})",
                              view.zoom, view.offsetX, view.offsetY);
                        needsRedraw = true;
                    } else if (mev.motion) {
                        // Mouse move — update hover
                        float sceneX = static_cast<float>(mev.col - 1) * cfg.sceneWidth / static_cast<float>(cols);
                        float sceneY = static_cast<float>(mev.row - 1) * cfg.sceneHeight / static_cast<float>(rows);
                        auto hit = renderer.hitTest(sceneX, sceneY);
                        if (hit.node != hoverFrame.node) {
                            hoverFrame = hit;
                            hoverSceneX = sceneX;
                            hoverSceneY = sceneY;
                            needsRedraw = true;
                        }
                    } else if (mev.button == 0 && mev.press) {
                        // Left click — hit test and zoom-to-frame
                        float sceneX = static_cast<float>(mev.col - 1) * cfg.sceneWidth / static_cast<float>(cols);
                        float sceneY = static_cast<float>(mev.row - 1) * cfg.sceneHeight / static_cast<float>(rows);
                        auto hit = renderer.hitTest(sceneX, sceneY);
                        if (hit.node) {
                            if (renderer.zoomRoot() == hit.node ||
                                hit.depth == 0) {
                                renderer.setZoomRoot(nullptr);
                            } else {
                                renderer.setZoomRoot(hit.node);
                            }
                            hoverFrame = {};
                            needsRedraw = true;
                        }
                    }
                    continue;
                }

                // Keyboard
                char ch = inputBuf[i];
                i++;

                if (ch == 3) { g_running = false; break; } // Ctrl+C

                if (ch == '\033' && (i >= n || inputBuf[i] != '[')) {
                    // Bare Escape — reset zoom
                    renderer.setZoomRoot(nullptr);
                    needsRedraw = true;
                    continue;
                }
                if (ch == '\033' && i + 1 < n && inputBuf[i] == '[') {
                    i++; // skip '['
                    if (i < n) {
                        char arrow = inputBuf[i];
                        i++;
                        switch (arrow) {
                        case 'A': view.panUp(panStep); needsRedraw = true; break;
                        case 'B': view.panDown(panStep); needsRedraw = true; break;
                        case 'C': view.panRight(panStep); needsRedraw = true; break;
                        case 'D': view.panLeft(panStep); needsRedraw = true; break;
                        }
                    }
                    continue;
                }

                switch (ch) {
                case 'q': case 'Q': g_running = false; break;
                case 'h': view.panLeft(panStep); needsRedraw = true; break;
                case 'l': view.panRight(panStep); needsRedraw = true; break;
                case 'k': view.panUp(panStep); needsRedraw = true; break;
                case 'j': view.panDown(panStep); needsRedraw = true; break;
                case '+': case '=': view.zoomIn(); needsRedraw = true; break;
                case '-': view.zoomOut(); needsRedraw = true; break;
                case '0': view.reset(); renderer.setZoomRoot(nullptr); needsRedraw = true; break;
                }
            }
        }

        frameCount++;

        // Periodic snapshot for perf profiler mode (attach mode only —
        // command mode is handled by waitForPerf above)
        if (usePerfProfiler && frameCount >= snapshotIntervalFrames) {
            frameCount = 0;
            bool alive = (mode == Mode::Command) ? profiler.perfAlive()
                                                  : profiler.targetAlive();
            if (alive) {
                auto collapsed = profiler.snapshot();
                if (!collapsed.empty()) {
                    renderer.clearTree();
                    renderer.loadCollapsedStacks(collapsed);
                    needsRedraw = true;
                    yinfo("perf snapshot: {} samples", renderer.totalSamples());
                }
            } else {
                // Target/perf exited — take final snapshot and stop
                auto collapsed = profiler.snapshot();
                if (!collapsed.empty()) {
                    renderer.clearTree();
                    renderer.loadCollapsedStacks(collapsed);
                    needsRedraw = true;
                }
                usePerfProfiler = false;
                yinfo("target exited, final snapshot: {} samples",
                      renderer.totalSamples());
            }
        }

        // File live-watch mode
        if (mode == Mode::File && liveFileWatch &&
            frameCount >= snapshotIntervalFrames) {
            frameCount = 0;
            auto mtime = getFileMtime(filePath);
            if (mtime.tv_sec != lastMtime.tv_sec ||
                mtime.tv_nsec != lastMtime.tv_nsec) {
                lastMtime = mtime;
                auto newData = readFile(filePath);
                if (!newData.empty()) {
                    renderer.clearTree();
                    renderer.loadCollapsedStacks(newData);
                    needsRedraw = true;
                    yinfo("file reloaded: {} samples", renderer.totalSamples());
                }
            }
        }

        if (needsRedraw) {
            renderer.render(buffer, view);

            // Tooltip overlay for hovered frame
            if (hoverFrame.node) {
                const auto* node = hoverFrame.node;
                uint64_t rootTotal = renderer.zoomRoot()
                    ? renderer.zoomRoot()->total : renderer.totalSamples();
                float pct = rootTotal > 0
                    ? 100.0f * static_cast<float>(node->total) / static_cast<float>(rootTotal)
                    : 0.0f;
                float selfPct = rootTotal > 0
                    ? 100.0f * static_cast<float>(node->self) / static_cast<float>(rootTotal)
                    : 0.0f;

                char line1[512], line2[256];
                std::snprintf(line1, sizeof(line1), "%s", node->name.c_str());
                std::snprintf(line2, sizeof(line2),
                    "total: %lu (%.1f%%)  self: %lu (%.1f%%)",
                    static_cast<unsigned long>(node->total), pct,
                    static_cast<unsigned long>(node->self), selfPct);

                float tipFontSize = 10.0f;
                float charW = tipFontSize * 0.55f;
                float len1 = static_cast<float>(strlen(line1)) * charW;
                float len2 = static_cast<float>(strlen(line2)) * charW;
                float tipW = std::max(len1, len2) + 12.0f;
                float tipH = tipFontSize * 2.8f + 8.0f;

                // Position tooltip near hover, keep in scene
                float tipX = hoverSceneX + 10.0f;
                float tipY = hoverSceneY - tipH - 5.0f;
                if (tipX + tipW > cfg.sceneWidth) tipX = cfg.sceneWidth - tipW - 2.0f;
                if (tipY < 0) tipY = hoverSceneY + 20.0f;

                uint32_t tipLayer = 10000;
                buffer->addBox(tipLayer++,
                    tipX + tipW / 2.0f, tipY + tipH / 2.0f,
                    tipW / 2.0f, tipH / 2.0f,
                    0xF0222233u, 0, 0.0f, 1.0f);
                buffer->addText(tipX + 6.0f, tipY + tipFontSize + 4.0f,
                    line1, tipFontSize, 0xFFFFFFFFu, tipLayer++, -1);
                buffer->addText(tipX + 6.0f, tipY + tipFontSize * 2.4f + 6.0f,
                    line2, tipFontSize, 0xFFCCCCCCu, tipLayer++, -1);
            }

            binary = buffer->serialize();
            sendOscUpdate(binary);
            needsRedraw = false;
        }
    }

    profiler.stop();
    sendOscKill();
    restoreTerminal();
    return 0;
}
