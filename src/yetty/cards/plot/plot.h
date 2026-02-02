#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
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
// Metadata layout (64 bytes = 16 u32 slots):
//   [0]  plotType(8) | flags(8) | pad(16)
//   [1]  widthCells(16) | heightCells(16)
//   [2]  dataOffset (u32) - float index into cardStorage
//   [3]  dataCount (u32)
//   [4]  minValue (f32)
//   [5]  maxValue (f32)
//   [6]  lineColor (u32 RGBA)
//   [7]  fillColor (u32 RGBA)
//   [8]  bgColor (u32 RGBA)
//   [9]  zoom (f32)
//   [10] centerX (f32)
//   [11] centerY (f32)
//   [12] glyphBase0(8) | glyphDot(8) | glyphMinus(8) | pad(8)
//   [13-15] reserved
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
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    // ObjectFactory createImpl
    static Result<Ptr> createImpl(
        ContextType& factoryCtx,
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    virtual ~Plot() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> dispose() override = 0;
    Result<void> render(float time) override = 0;
    const char* typeName() const override { return "plot"; }
    bool needsBuffer() const override { return true; }

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
    Plot(CardManager::Ptr mgr, const GPUContext& gpu,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
