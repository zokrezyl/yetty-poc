#pragma once

#include <yetty/result.hpp>
#include <yetty/gpu-context.h>
#include <memory>
#include <functional>
#include <cstdint>

namespace yetty {

struct Cell {
    uint32_t glyph;
    uint8_t fgR, fgG, fgB, alpha;
    uint8_t bgR, bgG, bgB, style;
};
static_assert(sizeof(Cell) == 12, "Cell must be 12 bytes");

class GPUScreen {
public:
    using Ptr = std::shared_ptr<GPUScreen>;

    static Result<Ptr> create(const GPUContext& gpu) noexcept;

    virtual ~GPUScreen() = default;

    // Write data from PTY to vterm
    virtual void write(const char* data, size_t len) = 0;

    // Resize (creates vterm on first call)
    virtual void resize(uint32_t cols, uint32_t rows) = 0;

    // Cell buffer for rendering
    virtual const Cell* getCellData() const = 0;
    virtual uint32_t getCols() const = 0;
    virtual uint32_t getRows() const = 0;

    // Cursor
    virtual int getCursorCol() const = 0;
    virtual int getCursorRow() const = 0;
    virtual bool isCursorVisible() const = 0;

    // Scrollback
    virtual void scrollUp(int lines) = 0;
    virtual void scrollDown(int lines) = 0;
    virtual void scrollToBottom() = 0;
    virtual bool isScrolledBack() const = 0;

    // Damage tracking
    virtual bool hasDamage() const = 0;
    virtual void clearDamage() = 0;

    // Callback for vterm output (to write to PTY)
    using OutputCallback = std::function<void(const char*, size_t)>;
    virtual void setOutputCallback(OutputCallback cb) = 0;

    // Input (keyboard/mouse -> vterm)
    virtual void sendKey(uint32_t codepoint, int mods) = 0;
    virtual void sendSpecialKey(int key, int mods) = 0;

    // Rendering
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Viewport (pixel coordinates within window)
    virtual void setViewport(float x, float y, float width, float height) = 0;

    // Cell size (from font metrics * zoom)
    virtual float getCellWidth() const = 0;
    virtual float getCellHeight() const = 0;

protected:
    GPUScreen() = default;
};

} // namespace yetty
