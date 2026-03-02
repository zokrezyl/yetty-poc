#pragma once

// yplot card - Plot card composing YDrawBuffer and yfsvm
//
// Uses shader glyph U+100009 (card base + 0x0009)
//
// Composes:
//   - YDrawBuffer + YDrawBuilder for axes, grid, labels
//   - yfsvm bytecode for expression evaluation
//   - A shader that renders both ydraw primitives and yfsvm plot lines
//
// Metadata layout (64 bytes = 16 u32):
//   [0]  flags(8) | funcCount(8) | glyphBase0(8) | glyphDot(8)
//   [1]  widthCells(16) | heightCells(16)
//   [2]  bytecodeOffset (u32) - index into cardStorage (expression mode)
//   [3]  bytecodeSize (u32)
//   [4]  xMin (f32)
//   [5]  xMax (f32)
//   [6]  yMin (f32)
//   [7]  yMax (f32)
//   [8]  dataOffset (u32) - index into cardStorage (buffer mode)
//   [9]  dataCount (u32) - number of floats
//   [10] marginLeft (f32)  - plot area margins in normalized coords
//   [11] marginBottom (f32)
//   [12] zoom (f32)
//   [13] centerX (f32)
//   [14] centerY (f32)
//   [15] colorTableOffset(24) | glyphMinus(8)

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <yetty/ydraw-builder.h>
#include "yetty/yplot/yplot.h"
#include "yetty/ydraw/ydraw-buffer.h"

namespace yetty::card {

class YPlotCard : public Card,
                  public base::ObjectFactory<YPlotCard> {
public:
    using Ptr = std::shared_ptr<YPlotCard>;

    // Shader glyph codepoint
    static constexpr uint32_t SHADER_GLYPH = 0x100009;

    // Flags
    static constexpr uint32_t FLAG_GRID = 1;
    static constexpr uint32_t FLAG_AXES = 2;
    static constexpr uint32_t FLAG_LABELS = 4;
    static constexpr uint32_t FLAG_BUFFER = 8;  // Buffer mode (vs expression mode)

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

    virtual ~YPlotCard() = default;

    //=========================================================================
    // Card interface
    //=========================================================================
    Result<void> dispose() override = 0;
    void suspend() override = 0;
    void declareBufferNeeds() override = 0;
    Result<void> allocateBuffers() override = 0;
    Result<void> finalize() override = 0;
    Result<void> update(const std::string& args, const std::string& payload) override = 0;

    const char* typeName() const override { return "yplot"; }
    bool needsBuffer() const override { return true; }

    // Card accessors
    uint32_t metadataOffset() const override { return _metaHandle.offset; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    uint32_t shaderGlyph() const override { return SHADER_GLYPH; }
    int32_t x() const override { return _x; }
    int32_t y() const override { return _y; }
    void setPosition(int32_t x, int32_t y) override { _x = x; _y = y; }
    uint32_t widthCells() const override { return _widthCells; }
    uint32_t heightCells() const override { return _heightCells; }
    const std::string& name() const override { return _name; }
    void setName(const std::string& n) override { _name = n; }
    bool hasName() const override { return !_name.empty(); }
    void setScreenOrigin(float sx, float sy) override { _screenOriginX = sx; _screenOriginY = sy; }

    //=========================================================================
    // YPlot-specific API
    //=========================================================================

    // Set expressions from multi-expression string
    virtual Result<void> setExpressions(const std::string& multiExpr) = 0;

    // Get state
    virtual const yplot::YPlotState& state() const = 0;

protected:
    YPlotCard(CardManager::Ptr mgr, const GPUContext& gpu,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr)), _gpu(gpu)
        , _x(x), _y(y), _widthCells(widthCells), _heightCells(heightCells)
    {}

    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    int32_t _x, _y;
    uint32_t _widthCells, _heightCells;
    float _screenOriginX = 0.0f, _screenOriginY = 0.0f;
    std::string _name;
};

} // namespace yetty::card
