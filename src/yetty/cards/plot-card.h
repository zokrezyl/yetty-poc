#pragma once

#include "../card.h"
#include "../card-factory.h"
#include <vector>

namespace yetty {

//=============================================================================
// PlotCard - Card for rendering plots via shader glyph
//
// Uses shader glyph 1048592 (U+100010)
//
// Metadata layout (64 bytes - uses SLOT_64):
//   offset 0:  plotType (u32)   - 0=line, 1=bar, 2=scatter, 3=area
//   offset 4:  dataOffset (u32) - float index into cardStorage (NOT byte offset!)
//   offset 8:  dataCount (u32)  - number of data points
//   offset 12: minValue (f32)   - data minimum for Y axis
//   offset 16: maxValue (f32)   - data maximum for Y axis
//   offset 20: lineColor (u32)  - packed RGBA for line/points
//   offset 24: fillColor (u32)  - packed RGBA for fill/bars
//   offset 28: flags (u32)      - bit 0: show grid, bit 1: show axes
//   offset 32: widgetCol (u32)  - widget's column position in grid
//   offset 36: widgetRow (u32)  - widget's row position in grid
//   offset 40: widthCells (u32) - widget width in cells
//   offset 44: heightCells (u32)- widget height in cells
//   offset 48-63: reserved (16 bytes padding)
//=============================================================================

class PlotCard : public Card {
public:
    // Shader glyph codepoint for plots
    static constexpr uint32_t SHADER_GLYPH = 1048592;  // U+100010

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
        CardBufferManager* mgr,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload
    );

    ~PlotCard() override;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> init() override;
    void dispose() override;
    void update(float time) override;
    const char* typeName() const override { return "plot"; }

    //=========================================================================
    // Plot-specific API
    //=========================================================================

    // Set plot type
    void setPlotType(PlotType type);
    PlotType plotType() const { return plotType_; }

    // Set data points (copies and uploads to GPU)
    Result<void> setData(const std::vector<float>& data);
    Result<void> setData(const float* data, uint32_t count);

    // Set value range (auto-calculated if not set)
    void setRange(float minVal, float maxVal);
    void setAutoRange(bool enabled) { autoRange_ = enabled; }

    // Set colors (packed RGBA: 0xAABBGGRR)
    void setLineColor(uint32_t color);
    void setFillColor(uint32_t color);

    // Set flags
    void setShowGrid(bool show);
    void setShowAxes(bool show);

private:
    PlotCard(CardBufferManager* mgr, int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload);

    // Parse args string (e.g., "--type line --grid --axes")
    void parseArgs(const std::string& args);

    // Parse payload as comma-separated values
    Result<void> parsePayload(const std::string& payload);

    // Upload metadata to GPU
    Result<void> uploadMetadata();

    // Calculate min/max from data
    void calculateRange();

    // Metadata structure (matches shader layout)
    struct Metadata {
        uint32_t plotType;
        uint32_t dataOffset;
        uint32_t dataCount;
        float minValue;
        float maxValue;
        uint32_t lineColor;
        uint32_t fillColor;
        uint32_t flags;
        uint32_t widgetCol;    // Widget's column position
        uint32_t widgetRow;    // Widget's row position
        uint32_t widthCells;   // Widget width in cells
        uint32_t heightCells;  // Widget height in cells
        uint32_t _reserved[4]; // Padding to 64 bytes
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    PlotType plotType_ = PlotType::Line;
    std::vector<float> data_;
    float minValue_ = 0.0f;
    float maxValue_ = 1.0f;
    bool autoRange_ = true;
    uint32_t lineColor_ = 0xFF00FF00;  // Green
    uint32_t fillColor_ = 0x8000FF00;  // Semi-transparent green
    uint32_t flags_ = FLAG_GRID | FLAG_AXES;

    StorageHandle storageHandle_ = StorageHandle::invalid();
    bool metadataDirty_ = true;

    std::string argsStr_;
    std::string payloadStr_;
};

// Register PlotCard with CardFactory
void registerPlotCard(CardFactory& factory);

} // namespace yetty
