#pragma once

// yplot - High-level plot library composing YDrawBuffer and yfsvm
//
// Provides:
//   - Expression compilation via yfsvm
//   - Axis/grid/label rendering via YDrawBuffer
//   - Clean composition of both systems

#include <yetty/result.hpp>
#include <yetty/yfsvm/yfsvm.h>
#include <yetty/yfsvm/yfsvm-compiler.h>
#include <yetty/yast/yast.h>
#include "yetty/ydraw/ydraw-buffer.h"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace yetty::yplot {

//=============================================================================
// Constants
//=============================================================================

constexpr uint32_t MAX_FUNCTIONS = 8;
constexpr float DEFAULT_X_MIN = -3.14159f;
constexpr float DEFAULT_X_MAX = 3.14159f;
constexpr float DEFAULT_Y_MIN = -1.5f;
constexpr float DEFAULT_Y_MAX = 1.5f;

//=============================================================================
// Configuration structs
//=============================================================================

struct FunctionConfig {
    std::string name;
    std::string expression;
    uint32_t color = 0xFFFF0000;  // ARGB
    bool visible = true;
};

struct RangeConfig {
    float xMin = DEFAULT_X_MIN;
    float xMax = DEFAULT_X_MAX;
    float yMin = DEFAULT_Y_MIN;
    float yMax = DEFAULT_Y_MAX;
    bool autoRange = false;
};

struct AxisConfig {
    bool showGrid = true;
    bool showAxes = true;
    bool showLabels = true;
    uint32_t axisColor = 0xFFAAAAAA;
    uint32_t gridColor = 0xFF444444;
    uint32_t labelColor = 0xFFFFFFFF;
    float labelFontSize = 10.0f;
    int gridDivisionsX = 10;
    int gridDivisionsY = 10;
};

struct DisplayConfig {
    uint32_t bgColor = 0xFF1A1A2E;
    float zoom = 1.0f;
    float centerX = 0.5f;
    float centerY = 0.5f;
};

//=============================================================================
// YPlotState - High-level plot configuration and builder
//=============================================================================

class YPlotState {
public:
    using Ptr = std::shared_ptr<YPlotState>;

    static Result<Ptr> create();

    //=========================================================================
    // Expression management (up to MAX_FUNCTIONS)
    //=========================================================================

    Result<void> addFunction(const std::string& name, const std::string& expression,
                             uint32_t color = 0);
    Result<void> setFunction(uint32_t index, const std::string& expression);
    Result<void> setFunctionColor(uint32_t index, uint32_t color);
    Result<void> removeFunction(uint32_t index);
    void clearFunctions();

    const std::vector<FunctionConfig>& functions() const { return _functions; }
    uint32_t functionCount() const { return static_cast<uint32_t>(_functions.size()); }

    // Parse multi-expression string: "f=sin(x); g=cos(x)"
    Result<void> parseExpressions(std::string_view source);

    // Compile all functions to yfsvm bytecode
    Result<yfsvm::Program> compile() const;

    //=========================================================================
    // Configuration accessors
    //=========================================================================

    RangeConfig& range() { return _range; }
    const RangeConfig& range() const { return _range; }

    AxisConfig& axis() { return _axis; }
    const AxisConfig& axis() const { return _axis; }

    DisplayConfig& display() { return _display; }
    const DisplayConfig& display() const { return _display; }

    void setRange(float xMin, float xMax, float yMin, float yMax);

    //=========================================================================
    // YDrawBuffer generation for axes/grid/labels
    //=========================================================================

    // Build axes, grid lines, tick marks, and labels into a YDrawBuffer
    // widthCells/heightCells are the card dimensions for proper scaling
    Result<void> buildDecoration(YDrawBuffer& buffer,
                                  uint32_t widthCells, uint32_t heightCells) const;

private:
    YPlotState() = default;

    // Helper to format axis labels
    std::string formatNumber(float value) const;

    std::vector<FunctionConfig> _functions;
    std::shared_ptr<yast::PlotExprNode> _parsedExpr;  // Parsed AST for compilation
    RangeConfig _range;
    AxisConfig _axis;
    DisplayConfig _display;
};

//=============================================================================
// Utility functions
//=============================================================================

// Parse color from hex string: "#FF0000" or "FF0000"
Result<uint32_t> parseColor(std::string_view colorStr);

// Default color palette for functions
uint32_t defaultColorForIndex(uint32_t index);

} // namespace yetty::yplot
