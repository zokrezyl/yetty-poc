// VNC Test Server - Simplified terminal rendering with vterm + FreeType
// Uses vterm for terminal state, FreeType for font rendering, VncServer for streaming
// Usage: vnc-test-server [--port PORT] [--fps FPS] [--pattern PATTERN]

#include "../vnc-server.h"
#include <ytrace/ytrace.hpp>
#include <args.hxx>
#include <webgpu/webgpu.h>

#include <ft2build.h>
#include FT_FREETYPE_H

extern "C" {
#include <vterm.h>
}

#include <random>
#include <chrono>
#include <thread>
#include <csignal>
#include <cstring>
#include <unordered_map>

namespace {

volatile bool g_running = true;

void signalHandler(int) {
    yinfo("Received signal, stopping...");
    g_running = false;
}

//=============================================================================
// Simple CPU Font Renderer using FreeType
//=============================================================================

class CpuFont {
public:
    CpuFont() = default;
    ~CpuFont() {
        if (_face) FT_Done_Face(_face);
        if (_library) FT_Done_FreeType(_library);
    }

    bool init(const std::string& ttfPath, int cellWidth, int cellHeight) {
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;

        if (FT_Init_FreeType(&_library)) {
            yerror("Failed to init FreeType");
            return false;
        }

        if (FT_New_Face(_library, ttfPath.c_str(), 0, &_face)) {
            yerror("Failed to load font: {}", ttfPath);
            return false;
        }

        // Set font size to fit cell
        FT_Set_Pixel_Sizes(_face, 0, cellHeight);

        // Get metrics for baseline calculation
        int ascender = _face->size->metrics.ascender >> 6;
        int descender = std::abs(_face->size->metrics.descender >> 6);
        int lineHeight = ascender + descender;

        // Scale to fit cell
        int targetHeight = cellHeight - 2;
        if (lineHeight > 0 && targetHeight > 0) {
            int fontSize = cellHeight * targetHeight / lineHeight;
            FT_Set_Pixel_Sizes(_face, 0, fontSize);
        }

        // Recalculate baseline
        ascender = _face->size->metrics.ascender >> 6;
        descender = std::abs(_face->size->metrics.descender >> 6);
        int actualLineHeight = ascender + descender;
        int topMargin = (cellHeight - actualLineHeight) / 2;
        _baseline = topMargin + ascender;

        yinfo("CpuFont: loaded {} (cell={}x{}, baseline={})", ttfPath, cellWidth, cellHeight, _baseline);
        return true;
    }

    // Render a glyph to the framebuffer at (x, y) with color (r, g, b)
    void renderGlyph(uint32_t codepoint, uint8_t* pixels, int fbWidth, int fbHeight,
                     int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        FT_UInt glyphIndex = FT_Get_Char_Index(_face, codepoint);
        if (glyphIndex == 0) return;

        if (FT_Load_Glyph(_face, glyphIndex, FT_LOAD_RENDER)) return;

        FT_Bitmap& bitmap = _face->glyph->bitmap;
        if (bitmap.width == 0 || bitmap.rows == 0) return;

        int bearingX = _face->glyph->bitmap_left;
        int bearingY = _face->glyph->bitmap_top;
        int offsetX = bearingX;
        int offsetY = _baseline - bearingY;

        // Clamp to cell
        offsetX = std::max(0, std::min(offsetX, _cellWidth - (int)bitmap.width));
        offsetY = std::max(0, std::min(offsetY, _cellHeight - (int)bitmap.rows));

        // Blend glyph bitmap to framebuffer
        for (int gy = 0; gy < (int)bitmap.rows && (y + offsetY + gy) < fbHeight; ++gy) {
            for (int gx = 0; gx < (int)bitmap.width && (x + offsetX + gx) < fbWidth; ++gx) {
                int px = x + offsetX + gx;
                int py = y + offsetY + gy;
                if (px < 0 || py < 0) continue;

                uint8_t alpha = bitmap.buffer[gy * bitmap.pitch + gx];
                if (alpha == 0) continue;

                int idx = (py * fbWidth + px) * 4;
                // Simple alpha blend
                uint8_t invAlpha = 255 - alpha;
                pixels[idx + 0] = (pixels[idx + 0] * invAlpha + b * alpha) / 255;
                pixels[idx + 1] = (pixels[idx + 1] * invAlpha + g * alpha) / 255;
                pixels[idx + 2] = (pixels[idx + 2] * invAlpha + r * alpha) / 255;
                pixels[idx + 3] = 255;
            }
        }
    }

    int cellWidth() const { return _cellWidth; }
    int cellHeight() const { return _cellHeight; }

private:
    FT_Library _library = nullptr;
    FT_Face _face = nullptr;
    int _cellWidth = 10;
    int _cellHeight = 20;
    int _baseline = 16;
};

//=============================================================================
// Simple VTerm wrapper
//=============================================================================

class Terminal {
public:
    Terminal(int cols, int rows) : _cols(cols), _rows(rows) {
        _vterm = vterm_new(rows, cols);
        vterm_set_utf8(_vterm, 1);
        _screen = vterm_obtain_screen(_vterm);
        vterm_screen_reset(_screen, 1);
    }

    ~Terminal() {
        if (_vterm) vterm_free(_vterm);
    }

    // Write data to terminal (like PTY output)
    void write(const char* data, size_t len) {
        vterm_input_write(_vterm, data, len);
    }

    // Write string
    void write(const std::string& str) {
        write(str.c_str(), str.size());
    }

    // Get cell at position
    bool getCell(int row, int col, uint32_t& codepoint, uint8_t& r, uint8_t& g, uint8_t& b,
                 uint8_t& bgR, uint8_t& bgG, uint8_t& bgB) {
        VTermPos pos = {row, col};
        VTermScreenCell cell;
        if (!vterm_screen_get_cell(_screen, pos, &cell)) {
            return false;
        }

        codepoint = cell.chars[0];
        if (codepoint == 0) codepoint = ' ';

        // Foreground color
        VTermColor fg = cell.fg;
        vterm_screen_convert_color_to_rgb(_screen, &fg);
        r = fg.rgb.red;
        g = fg.rgb.green;
        b = fg.rgb.blue;

        // Background color
        VTermColor bg = cell.bg;
        vterm_screen_convert_color_to_rgb(_screen, &bg);
        bgR = bg.rgb.red;
        bgG = bg.rgb.green;
        bgB = bg.rgb.blue;

        return true;
    }

    int cols() const { return _cols; }
    int rows() const { return _rows; }

private:
    VTerm* _vterm = nullptr;
    VTermScreen* _screen = nullptr;
    int _cols, _rows;
};

//=============================================================================
// Test pattern generators
//=============================================================================

void generateTextPattern(Terminal& term, int frame) {
    term.write("\033[2J\033[H");  // Clear screen, home cursor

    char buf[256];

    // Row 0: Frame number
    snprintf(buf, sizeof(buf), "%08d\r\n", frame);
    term.write(buf);

    // Row 1: Counter
    snprintf(buf, sizeof(buf), "%d\r\n", frame);
    term.write(buf);

    // Row 2: Static
    term.write("ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n");

    // Row 3: Rotating digits
    for (int i = 0; i < 26; i++) {
        char c = '0' + ((frame + i) % 10);
        term.write(&c, 1);
    }
    term.write("\r\n");

    // Row 4-10: Empty
    for (int r = 4; r < 10; r++) {
        term.write("\r\n");
    }

    // Row 10: Progress bar
    int pct = frame % 50;
    for (int i = 0; i < 50; i++) {
        term.write(i < pct ? "#" : ".");
    }
    term.write("\r\n");

    // Row 11: Time
    int s = frame % 60;
    int m = (frame / 60) % 60;
    int h = (frame / 3600) % 24;
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d\r\n", h, m, s);
    term.write(buf);

    // Row 12-19: Empty
    for (int r = 12; r < 20; r++) {
        term.write("\r\n");
    }

    // Row 20: Another frame counter
    snprintf(buf, sizeof(buf), "BOTTOM:%08d\r\n", frame);
    term.write(buf);

    // Row 21: Rotating alphabet
    for (int i = 0; i < 26; i++) {
        char c = 'A' + ((frame + i) % 26);
        term.write(&c, 1);
    }
    term.write("\r\n");

    // Row 22: Alternating pattern
    for (int i = 0; i < 40; i++) {
        term.write((frame + i) % 2 == 0 ? "X" : "O");
    }
    term.write("\r\n");
}

void generateScrollPattern(Terminal& term, int frame) {
    char buf[256];
    snprintf(buf, sizeof(buf), "\033[%dm[%05d] Scrolling line with text content\033[0m\n",
             31 + (frame % 6), frame);
    term.write(buf);
}

void generateStressPattern(Terminal& term, int frame, std::mt19937& rng) {
    std::uniform_int_distribution<int> charDist(33, 126);
    std::uniform_int_distribution<int> colorDist(31, 36);

    // Fill screen with random colored chars
    term.write("\033[H");  // Home cursor
    for (int row = 0; row < term.rows(); ++row) {
        char buf[16];
        snprintf(buf, sizeof(buf), "\033[%dm", colorDist(rng));
        term.write(buf);
        for (int col = 0; col < term.cols(); ++col) {
            char c = static_cast<char>(charDist(rng));
            term.write(&c, 1);
        }
        if (row < term.rows() - 1) term.write("\n");
    }
    (void)frame;
}

} // anonymous namespace

//=============================================================================
// Main
//=============================================================================

int main(int argc, char* argv[]) {
    // Parse arguments
    args::ArgumentParser parser("vnc-test-server", "VNC test server with terminal rendering");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::ValueFlag<uint16_t> portFlag(parser, "port", "VNC port (default 5900)", {'p', "port"}, 5900);
    args::ValueFlag<int> fpsFlag(parser, "fps", "Target FPS (default 30)", {'f', "fps"}, 30);
    args::ValueFlag<std::string> patternFlag(parser, "pattern", "Test pattern: text, scroll, stress", {"pattern"}, "text");
    args::ValueFlag<std::string> fontFlag(parser, "font", "Path to TTF font", {"font"}, "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
    args::ValueFlag<int> widthFlag(parser, "width", "Frame width (default 1280)", {'W', "width"}, 1280);
    args::ValueFlag<int> heightFlag(parser, "height", "Frame height (default 720)", {'H', "height"}, 720);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    uint16_t port = args::get(portFlag);
    int fps = args::get(fpsFlag);
    std::string pattern = args::get(patternFlag);
    std::string fontPath = args::get(fontFlag);
    int width = args::get(widthFlag);
    int height = args::get(heightFlag);

    yinfo("=== VNC Test Server ===");
    yinfo("Port: {}, FPS: {}, Pattern: {}", port, fps, pattern);
    yinfo("Resolution: {}x{}, Font: {}", width, height, fontPath);

    // Signal handling
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // Dynamic sizing state (can be changed by client)
    struct SizingState {
        int width;
        int height;
        int cellHeight;
        int cellWidth;
        bool needsRecreate = true;

        int cols() const { return width / cellWidth; }
        int rows() const { return height / cellHeight; }
    };
    SizingState sizing;
    sizing.width = width;
    sizing.height = height;
    sizing.cellHeight = 20;
    sizing.cellWidth = 10;  // Will be derived from cellHeight * 0.5

    // Font and terminal (recreated on size change)
    std::unique_ptr<CpuFont> font;
    std::unique_ptr<Terminal> term;

    // Create WebGPU instance
    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);
    if (!instance) {
        yerror("Failed to create WebGPU instance");
        return 1;
    }

    // Request adapter
    WGPUAdapter adapter = nullptr;
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter result,
                            WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestAdapterStatus_Success) {
            *static_cast<WGPUAdapter*>(userdata1) = result;
        }
    };
    adapterCb.userdata1 = &adapter;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCb);

    if (!adapter) {
        yerror("Failed to get adapter");
        return 1;
    }

    // Request device
    WGPUDevice device = nullptr;
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = {"vnc-test-device", 15};

    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice result,
                           WGPUStringView, void* userdata1, void*) {
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(userdata1) = result;
        }
    };
    deviceCb.userdata1 = &device;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb);

    if (!device) {
        yerror("Failed to get device");
        return 1;
    }

    WGPUQueue queue = wgpuDeviceGetQueue(device);
    yinfo("WebGPU initialized");

    // Texture and framebuffer (recreated on size change)
    WGPUTexture texture = nullptr;
    std::vector<uint8_t> framebuffer;

    // Start VNC server
    yetty::vnc::VncServer server(device, queue);
    auto startRes = server.start(port);
    if (!startRes) {
        yerror("Failed to start VNC server: {}", startRes.error().message());
        return 1;
    }
    yinfo("VNC server started on port {}", port);

    // Store client text to display at a dedicated line (not overwritten by patterns)
    std::string clientText;

    // Set up input callbacks
    server.onTextInput = [&clientText](const std::string& text) {
        yinfo("SERVER RECEIVED TEXT: {} bytes: {}", text.size(), text);
        clientText = text;
    };
    server.onKeyDown = [&term](uint32_t keycode, uint32_t scancode, uint8_t mods) {
        if (!term) return;
        if (keycode >= 32 && keycode < 127) {
            char c = static_cast<char>(keycode);
            term->write(&c, 1);
        } else if (keycode == '\n' || keycode == '\r') {
            term->write("\n", 1);
        }
    };
    server.onResize = [&sizing](uint16_t w, uint16_t h) {
        yinfo("SERVER RESIZE: {}x{}", w, h);
        if (w > 0 && h > 0 && (sizing.width != w || sizing.height != h)) {
            sizing.width = w;
            sizing.height = h;
            sizing.needsRecreate = true;
        }
    };
    server.onCellSize = [&sizing](uint8_t cellH) {
        yinfo("SERVER CELL SIZE: {}", cellH);
        if (cellH >= 8 && cellH <= 64 && sizing.cellHeight != cellH) {
            sizing.cellHeight = cellH;
            sizing.cellWidth = cellH / 2;  // Maintain 1:2 aspect ratio
            sizing.needsRecreate = true;
        }
    };

    // Main loop
    std::mt19937 rng(std::random_device{}());
    int frameCount = 0;
    auto frameInterval = std::chrono::microseconds(1000000 / fps);
    auto lastFrame = std::chrono::steady_clock::now();

    while (g_running) {
        auto now = std::chrono::steady_clock::now();
        if (now - lastFrame < frameInterval) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        lastFrame = now;

        // Recreate resources if size changed
        if (sizing.needsRecreate) {
            yinfo("Recreating resources: {}x{} cellH={}", sizing.width, sizing.height, sizing.cellHeight);

            // Recreate font
            font = std::make_unique<CpuFont>();
            if (!font->init(fontPath, sizing.cellWidth, sizing.cellHeight)) {
                yerror("Failed to reinit font");
                break;
            }

            // Recreate terminal
            term = std::make_unique<Terminal>(sizing.cols(), sizing.rows());
            yinfo("Terminal: {}x{} cells", sizing.cols(), sizing.rows());

            // Recreate texture
            if (texture) wgpuTextureRelease(texture);
            WGPUTextureDescriptor texDesc = {};
            texDesc.size = {static_cast<uint32_t>(sizing.width), static_cast<uint32_t>(sizing.height), 1};
            texDesc.format = WGPUTextureFormat_BGRA8Unorm;
            texDesc.usage = WGPUTextureUsage_CopySrc | WGPUTextureUsage_CopyDst;
            texDesc.mipLevelCount = 1;
            texDesc.sampleCount = 1;
            texDesc.dimension = WGPUTextureDimension_2D;
            texture = wgpuDeviceCreateTexture(device, &texDesc);

            // Recreate framebuffer
            framebuffer.resize(sizing.width * sizing.height * 4);

            // Force full frame update to clients
            server.forceFullFrame();

            sizing.needsRecreate = false;
        }

        if (!term || !font) continue;

        // Generate pattern
        if (pattern == "text") {
            generateTextPattern(*term, frameCount);
        } else if (pattern == "scroll") {
            generateScrollPattern(*term, frameCount);
        } else if (pattern == "stress") {
            generateStressPattern(*term, frameCount, rng);
        }

        // Write client text at a dedicated line (row 30) - won't be overwritten by patterns
        if (!clientText.empty()) {
            char buf[256];
            snprintf(buf, sizeof(buf), "\033[30;1H");
            term->write(buf);
            term->write(clientText);
        }

        // Write VNC stats at bottom line (row 35)
        {
            auto stats = server.getStats();
            char buf[256];
            snprintf(buf, sizeof(buf), "\033[35;1H\033[36mtiles/s=%u (j=%u r=%u) avg=%uB full/s=%u fps=%u %luKB/s\033[0m",
                     stats.tilesSent, stats.tilesJpeg, stats.tilesRaw,
                     stats.avgTileSize, stats.fullUpdates, stats.frames,
                     (unsigned long)(stats.bytesPerSec / 1024));
            term->write(buf);
        }

        // Clear framebuffer to dark background
        std::memset(framebuffer.data(), 0x1E, framebuffer.size());
        for (size_t i = 3; i < framebuffer.size(); i += 4) {
            framebuffer[i] = 0xFF;  // Alpha
        }

        // Render terminal cells to framebuffer
        for (int row = 0; row < sizing.rows(); ++row) {
            for (int col = 0; col < sizing.cols(); ++col) {
                uint32_t cp;
                uint8_t r, g, b, bgR, bgG, bgB;
                if (!term->getCell(row, col, cp, r, g, b, bgR, bgG, bgB)) continue;

                int x = col * sizing.cellWidth;
                int y = row * sizing.cellHeight;

                // Fill background if not default
                if (bgR != 0 || bgG != 0 || bgB != 0) {
                    for (int py = 0; py < sizing.cellHeight && y + py < sizing.height; ++py) {
                        for (int px = 0; px < sizing.cellWidth && x + px < sizing.width; ++px) {
                            int idx = ((y + py) * sizing.width + (x + px)) * 4;
                            framebuffer[idx + 0] = bgB;
                            framebuffer[idx + 1] = bgG;
                            framebuffer[idx + 2] = bgR;
                            framebuffer[idx + 3] = 0xFF;
                        }
                    }
                }

                // Render glyph
                if (cp > 32) {
                    font->renderGlyph(cp, framebuffer.data(), sizing.width, sizing.height, x, y, r, g, b);
                }
            }
        }

        // Upload to GPU texture
        WGPUTexelCopyTextureInfo dst = {};
        dst.texture = texture;
        WGPUTexelCopyBufferLayout layout = {};
        layout.bytesPerRow = sizing.width * 4;
        layout.rowsPerImage = sizing.height;
        WGPUExtent3D extent = {static_cast<uint32_t>(sizing.width), static_cast<uint32_t>(sizing.height), 1};
        wgpuQueueWriteTexture(queue, &dst, framebuffer.data(), framebuffer.size(), &layout, &extent);

        // Send frame
        if (server.hasClients()) {
            auto res = server.sendFrame(texture, sizing.width, sizing.height);
            if (!res && frameCount % 60 == 0) {
                ywarn("Frame send failed: {}", res.error().message());
            }
        }

        server.processInput();
        frameCount++;

        if (frameCount % (fps * 5) == 0) {
            yinfo("Frame {} sent, pattern={}", frameCount, pattern);
        }
    }

    yinfo("Stopping...");
    server.stop();
    if (texture) wgpuTextureRelease(texture);
    wgpuQueueRelease(queue);
    wgpuDeviceRelease(device);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);

    yinfo("Done");
    return 0;
}
