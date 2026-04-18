#include "yplot.h"

#include <yetty/yast/yast.h>
#include <charconv>
#include <array>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace yetty::yplot {

//=============================================================================
// Default color palette
//=============================================================================

static constexpr std::array<uint32_t, 8> COLOR_PALETTE = {
    0xFFFF6B6B,  // Red
    0xFF4ECDC4,  // Teal
    0xFFFFE66D,  // Yellow
    0xFF95E1D3,  // Mint
    0xFFF38181,  // Coral
    0xFFAA96DA,  // Purple
    0xFF72D6C9,  // Cyan
    0xFFFCBF49,  // Orange
};

uint32_t defaultColorForIndex(uint32_t index) {
    return COLOR_PALETTE[index % COLOR_PALETTE.size()];
}

//=============================================================================
// Color parsing
//=============================================================================

Result<uint32_t> parseColor(std::string_view colorStr) {
    if (colorStr.empty()) {
        return Err<uint32_t>("empty color string");
    }

    // Skip leading '#'
    if (colorStr[0] == '#') {
        colorStr = colorStr.substr(1);
    }

    if (colorStr.length() != 6 && colorStr.length() != 8) {
        return Err<uint32_t>("color must be 6 or 8 hex digits");
    }

    uint32_t value = 0;
    auto result = std::from_chars(colorStr.data(), colorStr.data() + colorStr.length(),
                                   value, 16);

    if (result.ec != std::errc{}) {
        return Err<uint32_t>("invalid hex color");
    }

    // If 6 digits, add full alpha
    if (colorStr.length() == 6) {
        value = 0xFF000000 | value;
    }

    return Ok(value);
}

//=============================================================================
// YPlotState implementation
//=============================================================================

Result<YPlotState::Ptr> YPlotState::create() {
    return Ok(Ptr(new YPlotState()));
}

Result<void> YPlotState::addFunction(const std::string& name, const std::string& expression,
                                      uint32_t color) {
    if (_functions.size() >= MAX_FUNCTIONS) {
        return Err<void>("maximum function count reached");
    }

    FunctionConfig func;
    func.name = name;
    func.expression = expression;
    func.color = color == 0 ? defaultColorForIndex(static_cast<uint32_t>(_functions.size())) : color;
    func.visible = true;

    _functions.push_back(std::move(func));
    return Ok();
}

Result<void> YPlotState::setFunction(uint32_t index, const std::string& expression) {
    if (index >= _functions.size()) {
        return Err<void>("function index out of range");
    }

    _functions[index].expression = expression;
    return Ok();
}

Result<void> YPlotState::setFunctionColor(uint32_t index, uint32_t color) {
    if (index >= _functions.size()) {
        return Err<void>("function index out of range");
    }

    _functions[index].color = color;
    return Ok();
}

Result<void> YPlotState::removeFunction(uint32_t index) {
    if (index >= _functions.size()) {
        return Err<void>("function index out of range");
    }

    _functions.erase(_functions.begin() + index);
    return Ok();
}

void YPlotState::clearFunctions() {
    _functions.clear();
}

Result<void> YPlotState::parseExpressions(std::string_view source) {
    // Parse using yast plot expression parser
    auto parseResult = yast::parsePlotExpression(source);
    if (!parseResult) {
        return Err<void>("failed to parse expressions");
    }

    const auto& definitions = parseResult->definitions;
    clearFunctions();

    // Store the parsed AST for later compilation
    _parsedExpr = parseResult;

    for (size_t i = 0; i < definitions.size() && i < MAX_FUNCTIONS; ++i) {
        const auto& def = definitions[i];

        FunctionConfig func;
        func.name = def->name;
        // We don't have the original expression string, but we have the AST
        // Store empty string - we'll use the AST for compilation
        func.expression = "";
        func.color = defaultColorForIndex(static_cast<uint32_t>(i));
        func.visible = true;

        _functions.push_back(std::move(func));
    }

    return Ok();
}

Result<yfsvm::Program> YPlotState::compile() const {
    if (_functions.empty() || !_parsedExpr) {
        return Err<yfsvm::Program>("no functions to compile");
    }

    // Use the parsed AST directly with yfsvm compiler
    yfsvm::Compiler compiler;
    return compiler.compileMulti(_parsedExpr.get());
}

void YPlotState::setRange(float xMin, float xMax, float yMin, float yMax) {
    _range.xMin = xMin;
    _range.xMax = xMax;
    _range.yMin = yMin;
    _range.yMax = yMax;
}

//=============================================================================
// YDrawBuffer generation
//=============================================================================

std::string YPlotState::formatNumber(float value) const {
    // Format number nicely for axis labels
    if (std::abs(value) < 0.0001f && value != 0.0f) {
        // Scientific notation for very small numbers
        std::ostringstream ss;
        ss << std::scientific << std::setprecision(1) << value;
        return ss.str();
    }

    // Check if it's close to a nice number
    if (std::abs(value - std::round(value)) < 0.0001f) {
        return std::to_string(static_cast<int>(std::round(value)));
    }

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string result = ss.str();

    // Remove trailing zeros
    size_t dotPos = result.find('.');
    if (dotPos != std::string::npos) {
        size_t lastNonZero = result.find_last_not_of('0');
        if (lastNonZero != std::string::npos && lastNonZero > dotPos) {
            result = result.substr(0, lastNonZero + 1);
        }
        if (result.back() == '.') {
            result.pop_back();
        }
    }

    return result;
}

Result<void> YPlotState::buildDecoration(YDrawBuffer& buffer,
                                          uint32_t widthCells, uint32_t heightCells) const {
    // Scene bounds in normalized coordinates [0,1]
    buffer.setSceneBounds(0.0f, 0.0f, 1.0f, 1.0f);
    buffer.setBgColor(_display.bgColor);

    const float xMin = _range.xMin;
    const float xMax = _range.xMax;
    const float yMin = _range.yMin;
    const float yMax = _range.yMax;

    // Margins for labels (in normalized coords)
    const float marginLeft = 0.08f;
    const float marginRight = 0.02f;
    const float marginBottom = 0.08f;
    const float marginTop = 0.02f;

    // Plot area in normalized coords
    const float plotLeft = marginLeft;
    const float plotRight = 1.0f - marginRight;
    const float plotBottom = marginBottom;
    const float plotTop = 1.0f - marginTop;
    const float plotWidth = plotRight - plotLeft;
    const float plotHeight = plotTop - plotBottom;

    // Helper to convert data coords to normalized coords
    auto dataToNorm = [&](float dataX, float dataY) -> std::pair<float, float> {
        float normX = plotLeft + (dataX - xMin) / (xMax - xMin) * plotWidth;
        float normY = plotBottom + (dataY - yMin) / (yMax - yMin) * plotHeight;
        return {normX, normY};
    };

    uint32_t layer = 0;

    // Draw grid lines
    if (_axis.showGrid) {
        float xStep = (xMax - xMin) / _axis.gridDivisionsX;
        float yStep = (yMax - yMin) / _axis.gridDivisionsY;

        // Vertical grid lines
        for (int i = 0; i <= _axis.gridDivisionsX; ++i) {
            float dataX = xMin + i * xStep;
            auto [x0, y0] = dataToNorm(dataX, yMin);
            auto [x1, y1] = dataToNorm(dataX, yMax);
            buffer.addSegment(layer, x0, y0, x1, y1, 0, _axis.gridColor, 0.002f, 0);
        }

        // Horizontal grid lines
        for (int i = 0; i <= _axis.gridDivisionsY; ++i) {
            float dataY = yMin + i * yStep;
            auto [x0, y0] = dataToNorm(xMin, dataY);
            auto [x1, y1] = dataToNorm(xMax, dataY);
            buffer.addSegment(layer, x0, y0, x1, y1, 0, _axis.gridColor, 0.002f, 0);
        }
    }

    // Draw axes
    if (_axis.showAxes) {
        // Find where zero crosses (if visible)
        bool xAxisVisible = (yMin <= 0 && yMax >= 0);
        bool yAxisVisible = (xMin <= 0 && xMax >= 0);

        if (xAxisVisible) {
            // X-axis at y=0
            auto [x0, y0] = dataToNorm(xMin, 0);
            auto [x1, y1] = dataToNorm(xMax, 0);
            buffer.addSegment(layer, x0, y0, x1, y1, 0, _axis.axisColor, 0.003f, 0);
        }

        if (yAxisVisible) {
            // Y-axis at x=0
            auto [x0, y0] = dataToNorm(0, yMin);
            auto [x1, y1] = dataToNorm(0, yMax);
            buffer.addSegment(layer, x0, y0, x1, y1, 0, _axis.axisColor, 0.003f, 0);
        }

        // Always draw border axes
        auto [bl_x, bl_y] = dataToNorm(xMin, yMin);
        auto [br_x, br_y] = dataToNorm(xMax, yMin);
        auto [tl_x, tl_y] = dataToNorm(xMin, yMax);
        auto [tr_x, tr_y] = dataToNorm(xMax, yMax);

        // Bottom border
        buffer.addSegment(layer, bl_x, bl_y, br_x, br_y, 0, _axis.axisColor, 0.002f, 0);
        // Left border
        buffer.addSegment(layer, bl_x, bl_y, tl_x, tl_y, 0, _axis.axisColor, 0.002f, 0);
    }

    // Draw labels
    if (_axis.showLabels) {
        float fontSize = _axis.labelFontSize / (heightCells * 20.0f);  // Approximate scaling

        // X-axis labels
        float xStep = (xMax - xMin) / _axis.gridDivisionsX;
        for (int i = 0; i <= _axis.gridDivisionsX; ++i) {
            float dataX = xMin + i * xStep;
            auto [x, y] = dataToNorm(dataX, yMin);
            std::string label = formatNumber(dataX);
            buffer.addText(x - 0.02f, y - 0.04f, label, fontSize, _axis.labelColor, layer, -1);
        }

        // Y-axis labels
        float yStep = (yMax - yMin) / _axis.gridDivisionsY;
        for (int i = 0; i <= _axis.gridDivisionsY; ++i) {
            float dataY = yMin + i * yStep;
            auto [x, y] = dataToNorm(xMin, dataY);
            std::string label = formatNumber(dataY);
            buffer.addText(x - 0.07f, y - 0.01f, label, fontSize, _axis.labelColor, layer, -1);
        }
    }

    return Ok();
}

} // namespace yetty::yplot
