#pragma once

#include "graph-ir.h"
#include <memory>
#include <functional>

namespace yetty::diagram {

// Text measurement callback: (text, fontSize) -> width
using MeasureTextFn = std::function<float(const std::string& text, float fontSize)>;

//=============================================================================
// Layout Engine Interface
//=============================================================================
class LayoutEngine {
public:
    using Ptr = std::shared_ptr<LayoutEngine>;

    virtual ~LayoutEngine() = default;

    // Layout the graph, filling node positions and edge control points
    virtual Result<void> layout(Graph& graph, MeasureTextFn measureText = nullptr) = 0;

    // Layout parameters
    struct Params {
        float nodeSpacingX = 60.0f;       // Min horizontal spacing
        float nodeSpacingY = 100.0f;      // Min vertical spacing (between layers)
        float nodePaddingX = 15.0f;       // Text padding inside nodes
        float nodePaddingY = 10.0f;
        float edgeSpacing = 20.0f;        // Spacing between parallel edges
        uint32_t maxIterations = 25;      // For crossing reduction
    };

    virtual void setParams(const Params& params) = 0;
    virtual Params params() const = 0;
};

} // namespace yetty::diagram
