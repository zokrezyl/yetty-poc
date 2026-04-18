// ybrowser: Standalone terminal browser using litehtml + ydraw
//
// Renders HTML pages via litehtml into YDrawBuffer, sends ydraw primitives
// to yetty via OSC 666666, receives mouse/keyboard input via standard
// terminal protocols (SGR mouse mode).

#include "html-renderer.h"
#include "html-container.h"
#include "http-fetcher.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"
#include "../../src/yetty/ycat/osc.h"

#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <litehtml.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <optional>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <signal.h>

using namespace yetty;
using namespace yetty::card;

//=============================================================================
// Terminal state
//=============================================================================

static struct termios g_origTermios;
static bool g_termRawMode = false;
static bool g_running = true;
static std::string g_cardName = "ybrowser-card";

static void restoreTerminal() {
    if (g_termRawMode) {
        // Disable mouse tracking
        write(STDOUT_FILENO, "\033[?1003l", 8);  // disable all-motion
        write(STDOUT_FILENO, "\033[?1006l", 8);  // disable SGR mode
        write(STDOUT_FILENO, "\033[?1000l", 8);  // disable click tracking
        // Restore alternate screen
        write(STDOUT_FILENO, "\033[?1049l", 8);
        // Restore terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &g_origTermios);
        g_termRawMode = false;
    }
}

static void signalHandler(int sig) {
    (void)sig;
    g_running = false;
}

static void enterRawMode() {
    tcgetattr(STDIN_FILENO, &g_origTermios);
    struct termios raw = g_origTermios;
    cfmakeraw(&raw);
    // Keep output processing for \n → \r\n
    raw.c_oflag |= OPOST;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    g_termRawMode = true;

    // Alternate screen
    write(STDOUT_FILENO, "\033[?1049h", 8);
    // Enable mouse tracking
    write(STDOUT_FILENO, "\033[?1000h", 8);  // click tracking
    write(STDOUT_FILENO, "\033[?1006h", 8);  // SGR mode
    write(STDOUT_FILENO, "\033[?1003h", 8);  // all-motion tracking (for hover)
}

static void getTerminalSize(int& cols, int& rows,
                            float& cellWidth, float& cellHeight) {
    struct winsize ws = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        cols = ws.ws_col;
        rows = ws.ws_row;
        // ws_xpixel/ws_ypixel give total pixel dimensions if supported
        if (ws.ws_xpixel > 0 && ws.ws_ypixel > 0) {
            cellWidth = static_cast<float>(ws.ws_xpixel) / static_cast<float>(cols);
            cellHeight = static_cast<float>(ws.ws_ypixel) / static_cast<float>(rows);
        } else {
            cellWidth = 9.0f;
            cellHeight = 20.0f;
        }
    } else {
        cols = 80;
        rows = 24;
        cellWidth = 9.0f;
        cellHeight = 20.0f;
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
        cols, rows, g_cardName.c_str());

    std::string seq;
    seq.reserve(strlen(header) + b64.size() + 4);
    seq += header;
    seq += b64;
    seq += "\033\\";
    seq = ycat::maybeWrapForTmux(seq);

    yinfo("ybrowser: sendOscCreate: header='{}' b64={} bytes, total={} bytes",
          header, b64.size(), seq.size());
    write(STDOUT_FILENO, seq.data(), seq.size());
}

static void sendOscUpdate(const std::vector<uint8_t>& binary) {
    auto b64 = ycat::base64Encode(binary);
    char header[512];
    std::snprintf(header, sizeof(header),
        "\033]666666;update --name %s;;",
        g_cardName.c_str());

    std::string seq;
    seq.reserve(strlen(header) + b64.size() + 4);
    seq += header;
    seq += b64;
    seq += "\033\\";
    seq = ycat::maybeWrapForTmux(seq);

    yinfo("ybrowser: sendOscUpdate: b64={} bytes", b64.size());
    write(STDOUT_FILENO, seq.data(), seq.size());
}

static void sendOscKill() {
    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "\033]666666;kill --name %s\033\\", g_cardName.c_str());
    std::string seq = ycat::maybeWrapForTmux(buf);
    write(STDOUT_FILENO, seq.data(), seq.size());
}

//=============================================================================
// SGR mouse parser
//=============================================================================

struct MouseEvent {
    int button;     // 0=left, 1=middle, 2=right, 64=scrollup, 65=scrolldown
    int col;        // 1-based
    int row;        // 1-based
    bool press;     // true=press, false=release
    bool motion;    // true if motion event
    bool shift;     // shift modifier
    bool alt;       // alt/meta modifier
    bool ctrl;      // control modifier
};

// Parse SGR mouse: ESC [ < Cb ; Cx ; Cy M/m
static bool parseSgrMouse(const char* buf, int len, MouseEvent& ev, int& consumed) {
    // Look for ESC [ <
    if (len < 3 || buf[0] != '\033' || buf[1] != '[' || buf[2] != '<')
        return false;

    int cb = 0, cx = 0, cy = 0;
    int i = 3;
    // Parse button
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cb = cb * 10 + (buf[i++] - '0');
    if (i >= len || buf[i] != ';') return false;
    i++;
    // Parse column
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cx = cx * 10 + (buf[i++] - '0');
    if (i >= len || buf[i] != ';') return false;
    i++;
    // Parse row
    while (i < len && buf[i] >= '0' && buf[i] <= '9')
        cy = cy * 10 + (buf[i++] - '0');
    if (i >= len) return false;

    if (buf[i] != 'M' && buf[i] != 'm') return false;
    ev.press = (buf[i] == 'M');
    consumed = i + 1;

    ev.motion = (cb & 32) != 0;
    ev.shift   = (cb & 4) != 0;
    ev.alt     = (cb & 8) != 0;
    ev.ctrl    = (cb & 16) != 0;
    // Button is in bits 0-1 and 6-7. Bits 2-5 are modifiers + motion flag.
    ev.button = (cb & 0x03) | (cb & 0xC0);  // keep button + scroll extension
    ev.col = cx;
    ev.row = cy;
    return true;
}

//=============================================================================
// Browser state
//=============================================================================

// User CSS: make links visible and hoverable
static const char* g_userStyles =
    "a { color: #0000EE !important; text-decoration: underline !important; cursor: pointer !important; }"
    "a:hover { background-color: #FFFF00 !important; color: #0000FF !important; }";

struct BrowserState {
    std::shared_ptr<YDrawBuffer> buffer;
    std::shared_ptr<HttpFetcher> fetcher;
    std::shared_ptr<HtmlContainer> container;
    std::shared_ptr<litehtml::document> document;

    std::string currentUrl;
    float fontSize = 16.0f;
    float viewWidth = 600.0f;
    int docWidth = 0;
    int docHeight = 0;
    int termCols = 80;
    int termRows = 24;
    float cellWidth = 9.0f;
    float cellHeight = 20.0f;

    float scrollX = 0.0f;    // horizontal scroll offset in document pixels
    float scrollY = 0.0f;    // vertical scroll offset in document pixels
    float scrollStep = 40.0f; // pixels per scroll step
    float zoom = 1.0f;       // zoom factor (>1 = zoomed in, <1 = zoomed out)
    float baseViewWidth = 600.0f; // view width at zoom=1

    bool needsRedraw = false;

    // Deferred navigation URL (avoid re-entrancy in litehtml callbacks)
    std::string pendingNavigateUrl;

    // Deferred form submission (avoid re-entrancy in litehtml callbacks)
    struct PendingFormSubmit {
        std::string action;
        std::string method;
        std::string formData;
    };
    std::optional<PendingFormSubmit> pendingFormSubmit;

    // Status bar
    std::string cursorType = "default";
    std::string statusText;

    void setupCallbacks() {
        if (!container) return;

        // Defer navigation to avoid re-entrancy in litehtml
        container->setNavigateCallback([this](const std::string& url) {
            yinfo("ybrowser: navigate callback -> {}", url);
            pendingNavigateUrl = url;
        });

        // Cursor change feedback (e.g. "pointer" when hovering links)
        container->setCursorCallback([this](const std::string& cursor) {
            if (cursor != cursorType) {
                yinfo("ybrowser: cursor changed: {} -> {}", cursorType, cursor);
                cursorType = cursor;
                updateStatusBar();
            }
        });

        // Form submission (e.g. <input type="submit"> clicked)
        container->setFormSubmitCallback(
            [this](const std::string& action, const std::string& method,
                   const std::string& formData) {
                yinfo("ybrowser: form submit callback action={} method={}", action, method);
                pendingFormSubmit = PendingFormSubmit{action, method, formData};
            });
    }

    void updateStatusBar() {
        // Show status at bottom of terminal (below the ydraw card)
        // Card occupies rows 0..termRows-1, status bar at termRows+1 (1-based)
        char buf[512];
        std::string indicator;
        if (cursorType == "pointer") {
            indicator = "[LINK] Click to navigate";
        } else {
            indicator = currentUrl.empty() ? "Press 'g' to enter URL" : currentUrl;
        }
        if (std::abs(zoom - 1.0f) > 0.01f) {
            char zoomStr[32];
            std::snprintf(zoomStr, sizeof(zoomStr), " [%.0f%%]", zoom * 100.0f);
            indicator += zoomStr;
        }

        int statusRow = termRows + 1;  // 1-based row below card
        int len = std::snprintf(buf, sizeof(buf),
            "\033[%d;1H\033[2K\033[7m %-*.*s\033[0m",
            statusRow, termCols - 1, termCols - 1, indicator.c_str());
        if (len > 0) {
            write(STDOUT_FILENO, buf, len);
        }
    }

    void navigateTo(const std::string& url) {
        if (!fetcher) return;

        std::string resolvedUrl = fetcher->resolveUrl(url);
        yinfo("ybrowser: navigating to {}", resolvedUrl);

        fetcher->setBaseUrl(resolvedUrl);
        auto body = fetcher->fetch(resolvedUrl);
        if (!body) {
            ywarn("ybrowser: failed to fetch {}", resolvedUrl);
            return;
        }

        renderPage(*body, resolvedUrl);
    }

    void renderPage(const std::string& html, const std::string& url) {
        currentUrl = url;
        scrollX = 0.0f;
        scrollY = 0.0f;
        cursorType = "default";

        // Destroy old document before container
        document.reset();
        container.reset();

        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        auto result = renderHtmlToBuffer(html, buffer, fontSize, viewWidth,
                                          fetcher.get(), g_userStyles);
        if (result.document) {
            container = result.container;
            document = result.document;
            docWidth = result.documentWidth;
            docHeight = result.documentHeight;
            setupCallbacks();

            // Re-draw with viewport clipping (renderHtmlToBuffer drew full doc)
            redraw();

            yinfo("ybrowser: rendered {}x{} vpH={}", docWidth, docHeight,
                  static_cast<int>(viewportHeight()));
        } else {
            needsRedraw = true;
        }
        updateStatusBar();
    }

    // Viewport dimensions in document pixels (matches terminal pixel dimensions)
    float viewportWidth() const {
        return static_cast<float>(termCols) * cellWidth;
    }

    float viewportHeight() const {
        return static_cast<float>(termRows) * cellHeight;
    }

    float maxScrollY() const {
        return std::max(0.0f, static_cast<float>(docHeight) - viewportHeight());
    }

    float maxScrollX() const {
        return std::max(0.0f, static_cast<float>(docWidth) - viewWidth);
    }

    void clampScroll() {
        scrollX = std::clamp(scrollX, 0.0f, maxScrollX());
        scrollY = std::clamp(scrollY, 0.0f, maxScrollY());
    }

    // Set scene bounds to the visible viewport (not the full document).
    // This gives 1:1 pixel mapping between document coordinates and screen.
    void setSceneBounds() {
        float vpW = viewportWidth();
        float vpH = viewportHeight();
        if (vpH > 0) {
            buffer->setSceneBounds(0, 0, vpW, vpH);
        }
    }

    void redraw() {
        if (!document) return;
        clampScroll();
        buffer->clear();
        buffer->setBgColor(0xFFFFFFFF);
        int vpW = static_cast<int>(viewportWidth());
        int vpH = static_cast<int>(viewportHeight());
        int iScrollX = static_cast<int>(scrollX);
        int iScrollY = static_cast<int>(scrollY);
        // Clip in screen coordinates (after offset applied)
        litehtml::position clip(0, 0, vpW, vpH);
        // hdc=0, x=-scrollX, y=-scrollY shifts document into view
        document->draw(0, -iScrollX, -iScrollY, &clip);
        setSceneBounds();
        needsRedraw = true;
    }

    void sendToTerminal(bool isUpdate) {
        auto binary = buffer->serialize();
        if (isUpdate) {
            sendOscUpdate(binary);
        } else {
            sendOscCreate(termCols, termRows, binary);
        }
        needsRedraw = false;
    }

    // Convert terminal cell coordinates to document coordinates.
    // With viewport-based rendering (scene bounds = viewport size),
    // the mapping is 1:1 between card pixels and document pixels,
    // plus the scroll offset.
    void cellToDocCoords(int col, int row, int& docX, int& docY) {
        // Cell center in pixels (1-based cell coords)
        float pixelX = (static_cast<float>(col) - 0.5f) * cellWidth;
        float pixelY = (static_cast<float>(row) - 0.5f) * cellHeight;

        // Add scroll offsets to get absolute document coordinates
        docX = static_cast<int>(pixelX + scrollX);
        docY = static_cast<int>(pixelY + scrollY);
    }

    void applyZoom(float newZoom) {
        newZoom = std::clamp(newZoom, 0.25f, 4.0f);
        if (std::abs(newZoom - zoom) < 0.01f) return;
        zoom = newZoom;
        viewWidth = baseViewWidth / zoom;
        scrollX = 0.0f;
        scrollY = 0.0f;
        // Re-render at new width (reflows text like browser zoom)
        if (document && container) {
            document->render(static_cast<int>(viewWidth));
            docWidth = static_cast<int>(viewWidth);
            docHeight = document->height();
        }
        redraw();
    }

    void handleMouseEvent(const MouseEvent& ev) {
        if (!document) return;

        // Scroll events (button 64=up, 65=down)
        if (ev.button == 64 || ev.button == 65) {
            float dir = (ev.button == 64) ? -1.0f : 1.0f;
            if (ev.ctrl) {
                // Ctrl+scroll = zoom
                float step = 0.1f * -dir;  // scroll up = zoom in
                applyZoom(zoom + step);
            } else if (ev.shift) {
                // Shift+scroll = horizontal scroll
                scrollX = std::clamp(scrollX + dir * scrollStep, 0.0f, maxScrollX());
                redraw();
            } else {
                // Plain scroll = vertical scroll
                scrollY = std::clamp(scrollY + dir * scrollStep, 0.0f, maxScrollY());
                redraw();
            }
            return;
        }

        int docX, docY;
        cellToDocCoords(ev.col, ev.row, docX, docY);
        yinfo("ybrowser: mouse btn={} col={} row={} press={} motion={} -> doc({},{})",
              ev.button, ev.col, ev.row, ev.press, ev.motion, docX, docY);

        litehtml::position::vector redrawBoxes;

        if (ev.motion) {
            document->on_mouse_over(docX, docY, docX, docY, redrawBoxes);
        } else if (ev.button == 0 && ev.press) {
            // Ensure hover state is set before click (litehtml uses m_over_element)
            document->on_mouse_over(docX, docY, docX, docY, redrawBoxes);
            document->on_lbutton_down(docX, docY, docX, docY, redrawBoxes);
            yinfo("ybrowser: lbutton_down at doc({},{}) cursor={}", docX, docY, cursorType);
        } else if (ev.button == 0 && !ev.press) {
            document->on_mouse_over(docX, docY, docX, docY, redrawBoxes);
            document->on_lbutton_up(docX, docY, docX, docY, redrawBoxes);
            yinfo("ybrowser: lbutton_up at doc({},{}) cursor={}", docX, docY, cursorType);
        }

        if (!redrawBoxes.empty()) {
            yinfo("ybrowser: mouse at doc({},{}) triggered redraw ({} boxes)",
                  docX, docY, redrawBoxes.size());
            redraw();
        }
    }

    // Process deferred actions (call from event loop, outside litehtml callbacks)
    void processPending() {
        if (!pendingNavigateUrl.empty()) {
            std::string url = std::move(pendingNavigateUrl);
            pendingNavigateUrl.clear();
            navigateTo(url);
        }
        if (pendingFormSubmit) {
            auto fs = std::move(*pendingFormSubmit);
            pendingFormSubmit.reset();
            handleFormSubmit(fs.action, fs.method, fs.formData);
        }
    }

    void handleFormSubmit(const std::string& action,
                          const std::string& method,
                          const std::string& formData) {
        if (!fetcher) return;

        std::string resolvedAction = fetcher->resolveUrl(action);
        yinfo("ybrowser: form submit {} {} data={}bytes",
              method, resolvedAction, formData.size());

        if (method == "GET") {
            // Append form data as query string
            std::string url = resolvedAction;
            if (!formData.empty()) {
                url += (url.find('?') != std::string::npos ? "&" : "?");
                url += formData;
            }
            navigateTo(url);
        } else {
            // POST form data
            auto body = fetcher->post(resolvedAction, formData);
            if (!body) {
                ywarn("ybrowser: form POST failed");
                return;
            }
            yinfo("ybrowser: form POST response {} bytes", body->size());
            renderPage(*body, resolvedAction);
        }
    }
};

//=============================================================================
// URL input mode
//=============================================================================

static std::string readUrlFromUser() {
    // Temporarily show a prompt on the status line
    // Move cursor to last line, clear it, show prompt
    write(STDOUT_FILENO, "\033[999;1H", 8);   // move to bottom
    write(STDOUT_FILENO, "\033[2K", 4);        // clear line
    const char* prompt = "URL: ";
    write(STDOUT_FILENO, prompt, strlen(prompt));

    std::string url;
    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == '\r' || ch == '\n') break;
        if (ch == 27) { url.clear(); break; }  // ESC cancels
        if (ch == 127 || ch == 8) {  // backspace
            if (!url.empty()) {
                url.pop_back();
                write(STDOUT_FILENO, "\b \b", 3);
            }
            continue;
        }
        if (ch >= 32) {
            url += ch;
            write(STDOUT_FILENO, &ch, 1);
        }
    }

    // Clear the prompt line
    write(STDOUT_FILENO, "\033[999;1H", 8);
    write(STDOUT_FILENO, "\033[2K", 4);

    return url;
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char** argv) {
    // Setup logging - use PID in filename to avoid conflicts
    char logPath[256];
    std::snprintf(logPath, sizeof(logPath), "/tmp/ybrowser-%d.log", getpid());
    auto fileLogger = spdlog::basic_logger_mt("ybrowser", logPath, true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::debug);

    yinfo("ybrowser: starting, pid={}, log={}", getpid(), logPath);

    // Parse URL argument
    std::string initialUrl;
    if (argc > 1) {
        initialUrl = argv[1];
    }
    yinfo("ybrowser: initialUrl='{}'", initialUrl);

    // Setup signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create fetcher
    auto fetcherResult = HttpFetcher::create();
    if (!fetcherResult) {
        fprintf(stderr, "Error: failed to create HTTP fetcher\n");
        return 1;
    }
    auto fetcher = std::move(*fetcherResult);

    // Create buffer
    auto buffer = *YDrawBuffer::create();

    // Get terminal size with pixel dimensions for accurate cell size
    int cols, rows;
    float cellW, cellH;
    getTerminalSize(cols, rows, cellW, cellH);
    yinfo("ybrowser: terminal {}x{} cells, cell={:.1f}x{:.1f}px", cols, rows, cellW, cellH);

    // Enter raw mode + alternate screen + mouse tracking
    enterRawMode();
    atexit(restoreTerminal);

    // Initialize browser state (reserve last row for status bar)
    BrowserState browser;
    browser.buffer = buffer;
    browser.fetcher = fetcher;
    browser.termCols = cols;
    browser.termRows = rows - 1;
    browser.cellWidth = cellW;
    browser.cellHeight = cellH;
    browser.viewWidth = static_cast<float>(cols) * cellW;
    browser.baseViewWidth = browser.viewWidth;

    // Navigate to initial URL or show welcome
    if (!initialUrl.empty()) {
        browser.navigateTo(initialUrl);
    } else {
        std::string welcomeHtml =
            "<html><body style='font-family: sans-serif; padding: 20px;'>"
            "<h1>ybrowser</h1>"
            "<p>A terminal-based browser powered by litehtml + ydraw</p>"
            "<p>Press <b>g</b> to enter a URL, <b>q</b> to quit</p>"
            "<p>Try: <a href='https://example.com'>CLICK HERE - example.com</a></p>"
            "</body></html>";
        browser.renderPage(welcomeHtml, "");
    }

    // Send initial render
    yinfo("ybrowser: initial render doc={}x{} vpH={}",
          browser.docWidth, browser.docHeight,
          static_cast<int>(browser.viewportHeight()));
    browser.sendToTerminal(false);
    browser.updateStatusBar();

    // Event loop
    char inputBuf[256];
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    while (g_running) {
        // Process deferred navigation (outside litehtml callback context)
        browser.processPending();

        int ret = poll(&pfd, 1, 50);  // 50ms timeout for responsive updates
        if (ret < 0) {
            if (errno == EINTR) continue;
            break;
        }

        if (ret > 0 && (pfd.revents & POLLIN)) {
            int n = read(STDIN_FILENO, inputBuf, sizeof(inputBuf));
            if (n <= 0) break;

            int i = 0;
            while (i < n) {
                // Try to parse SGR mouse sequence
                MouseEvent mev;
                int consumed;
                if (parseSgrMouse(inputBuf + i, n - i, mev, consumed)) {
                    browser.handleMouseEvent(mev);
                    i += consumed;
                    continue;
                }

                // Handle keyboard
                char ch = inputBuf[i];
                i++;

                // Ctrl+C (raw mode disables ISIG, so handle manually)
                if (ch == 3) {
                    g_running = false;
                    break;
                }

                if (ch == 'q' || ch == 'Q') {
                    g_running = false;
                    break;
                }

                if (ch == 'g' || ch == 'G') {
                    std::string url = readUrlFromUser();
                    if (!url.empty()) {
                        if (url.find("://") == std::string::npos) {
                            url = "https://" + url;
                        }
                        browser.navigateTo(url);
                    }
                    continue;
                }

                if (ch == 'r' || ch == 'R') {
                    if (!browser.currentUrl.empty()) {
                        browser.navigateTo(browser.currentUrl);
                    }
                    continue;
                }

                // Arrow keys (ESC [ A/B/C/D)
                if (ch == '\033' && i + 1 < n && inputBuf[i] == '[') {
                    i++;  // skip [
                    if (i < n) {
                        char arrow = inputBuf[i];
                        i++;
                        if (arrow == 'A') {  // Up
                            browser.scrollY = std::max(0.0f,
                                browser.scrollY - browser.scrollStep);
                            browser.redraw();
                        } else if (arrow == 'B') {  // Down
                            browser.scrollY = std::min(browser.maxScrollY(),
                                browser.scrollY + browser.scrollStep);
                            browser.redraw();
                        }
                    }
                    continue;
                }
            }
        }

        // Send update if needed
        if (browser.needsRedraw) {
            browser.sendToTerminal(true);
        }
    }

    // Cleanup
    sendOscKill();
    restoreTerminal();

    return 0;
}
