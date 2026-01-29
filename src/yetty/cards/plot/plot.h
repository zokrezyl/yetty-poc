#pragma once

#include <yetty/card.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <vector>

namespace yetty::card {

//=============================================================================
// Plot - Card for rendering plots via shader glyph
//
// Uses shader glyph U+100001 (card base + offset 0x0001)
//
// =============================================================================
// ANSI-Compatible Cell Encoding (24-bit true-color)
// =============================================================================
// Cards are rendered by outputting ANSI escape sequences to vterm, allowing
// proper scrolling and scrollback buffer management. Each cell encodes:
//
//   fg (24 bits via ANSI RGB): Metadata SLOT INDEX (not byte offset!)
//     - slotIndex = byteOffset / 32 (metadata is 32-byte aligned)
//     - Shader converts: metaOffset_u32 = slotIndex * 8
//     - Addressable: 2^24 × 32 = 512MB metadata
//
//   bg (24 bits via ANSI RGB): Relative position within widget
//     - Encoding: (relRow << 12) | relCol
//     - 12 bits each → max 4096×4096 cell widgets
//
// =============================================================================
// Metadata layout (48 bytes, uses 64-byte slot):
//   offset 0:  plotType (u32)   - 0=line, 1=bar, 2=scatter, 3=area
//   offset 4:  dataOffset (u32) - float index into cardStorage (NOT byte offset!)
//   offset 8:  dataCount (u32)  - number of data points
//   offset 12: minValue (f32)   - data minimum for Y axis
//   offset 16: maxValue (f32)   - data maximum for Y axis
//   offset 20: lineColor (u32)  - packed RGBA for line/points
//   offset 24: fillColor (u32)  - packed RGBA for fill/bars
//   offset 28: flags (u32)      - bit 0: show grid, bit 1: show axes
//   offset 32: widthCells (u32) - widget width in cells
//   offset 36: heightCells (u32)- widget height in cells
//   offset 40: bgColor (u32)    - packed RGBA for background
//   offset 44-63: reserved (20 bytes padding)
//=============================================================================

class Plot : public Card,
             public base::ObjectFactory<Plot> {
public:
    using Ptr = std::shared_ptr<Plot>;

    // Shader glyph codepoint for plots
    static constexpr uint32_t SHADER_GLYPH = 0x100001;  // Card base + 0x0001

    // Plot types
    enum class PlotType : uint32_t {
        Line = 0,
        Bar = 1,
        Scatter = 2,
        Area = 3
    };

    // Flags
    static constexpr uint32_t FLAG_GRID = 1;
    static constexpr uint32_t FLAG_AXES = 2;

    //=========================================================================
    // Factory method
    //=========================================================================
    static Result<CardPtr> create(
        CardBufferManager::Ptr mgr,
        const GPUContext& gpu,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // ObjectFactory createImpl
    static Result<Ptr> createImpl(
        ContextType& ctx,
        CardBufferManager::Ptr mgr,
        const GPUContext& gpu,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    virtual ~Plot() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> dispose() override = 0;
    Result<void> update(float time) override = 0;
    const char* typeName() const override { return "plot"; }

    // Override for 64-byte metadata slots (shader uses slotIndex * 16)
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    //=========================================================================
    // Plot-specific API
    //=========================================================================

    // Set plot type
    virtual void setPlotType(PlotType type) = 0;
    virtual PlotType plotType() const = 0;

    // Set data points (copies and uploads to GPU)
    virtual Result<void> setData(const std::vector<float>& data) = 0;
    virtual Result<void> setData(const float* data, uint32_t count) = 0;

    // Set value range (auto-calculated if not set)
    virtual void setRange(float minVal, float maxVal) = 0;
    virtual void setAutoRange(bool enabled) = 0;

    // Get current range
    virtual float minValue() const = 0;
    virtual float maxValue() const = 0;

    // Set colors (packed RGBA: 0xAABBGGRR)
    virtual void setLineColor(uint32_t color) = 0;
    virtual void setFillColor(uint32_t color) = 0;
    virtual void setBgColor(uint32_t color) = 0;

    // Get colors
    virtual uint32_t lineColor() const = 0;
    virtual uint32_t fillColor() const = 0;
    virtual uint32_t bgColor() const = 0;

    // Set flags
    virtual void setShowGrid(bool show) = 0;
    virtual void setShowAxes(bool show) = 0;

    // Get data count
    virtual uint32_t dataCount() const = 0;

protected:
    Plot(CardBufferManager::Ptr mgr, const GPUContext& gpu,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
