#pragma once

#include <yetty/result.hpp>
#include <yetty/yetty-context.h>
#include <yetty/card.h>
#include <yetty/base/event-loop.h>
#include <memory>
#include <functional>
#include <cstdint>
#include <optional>

namespace yetty {

struct Cell {
    uint32_t glyph;
    uint8_t fgR, fgG, fgB, alpha;
    uint8_t bgR, bgG, bgB, style;
};
static_assert(sizeof(Cell) == 12, "Cell must be 12 bytes");

class GPUScreen : public base::EventListener {
public:
    using Ptr = std::shared_ptr<GPUScreen>;

    static Result<Ptr> create(const YettyContext& ctx) noexcept;

    // Test-only factory - creates GPUScreen without GPU resources
    static Result<Ptr> createForTest(uint32_t cols, uint32_t rows) noexcept;

    virtual ~GPUScreen() = default;

    // Write data from PTY to vterm
    virtual void write(const char* data, size_t len) = 0;

    // Resize terminal
    virtual void resize(uint32_t cols, uint32_t rows) = 0;
    virtual uint32_t getCols() const = 0;
    virtual uint32_t getRows() const = 0;

    // Damage tracking
    virtual bool hasDamage() const = 0;
    virtual void clearDamage() = 0;

    // Callback for vterm output (to write to PTY)
    using OutputCallback = std::function<void(const char*, size_t)>;
    virtual void setOutputCallback(OutputCallback cb) = 0;

    // Callback when terminal grid dimensions change (for PTY TIOCSWINSZ)
    using ResizeCallback = std::function<void(uint32_t cols, uint32_t rows)>;
    virtual void setResizeCallback(ResizeCallback cb) = 0;

    // Rendering
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Viewport (pixel coordinates within window)
    virtual void setViewport(float x, float y, float width, float height) = 0;

    // Cell size (from font metrics)
    virtual uint32_t getCellWidth() const = 0;
    virtual uint32_t getCellHeight() const = 0;

    // Test interface - access cell data
    virtual const Cell* getCellData() const = 0;
    virtual Cell getCell(int row, int col) const = 0;

    // Card lookup by name (for RPC streaming)
    virtual Card* getCardByName(const std::string& name) const = 0;
    virtual Card* getCardBySlotIndex(uint32_t slotIndex) const = 0;

    // Get all active cards (for RPC listing)
    virtual std::vector<Card*> getAllCards() const = 0;

    // CardManager access (for RPC streaming)
    virtual CardManager::Ptr cardManager() const = 0;

    // Named card registry (for RPC streaming)
    virtual void registerNamedCard(const std::string& name, uint32_t slotIndex) = 0;
    virtual void unregisterNamedCard(const std::string& name) = 0;
    virtual std::optional<uint32_t> getSlotIndexByName(const std::string& name) const = 0;
    virtual std::string getNameBySlotIndex(uint32_t slotIndex) const = 0;

protected:
    GPUScreen() = default;
};

} // namespace yetty
