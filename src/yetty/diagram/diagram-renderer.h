#pragma once

#include "graph-ir.h"
#include <memory>

namespace yetty {
class YDrawBuilder;
}

namespace yetty::diagram {

//=============================================================================
// DiagramRenderer - Converts Graph IR to YDrawBuilder primitives
//=============================================================================
class DiagramRenderer {
public:
    using Ptr = std::shared_ptr<DiagramRenderer>;

    static Result<Ptr> create(std::shared_ptr<YDrawBuilder> builder);

    // Render the graph to YDrawBuilder primitives
    Result<void> render(const Graph& graph);

    // Rendering options
    struct Options {
        bool renderBackground = false;
        uint32_t backgroundColor = 0xFF1A1A2E;
        float arrowSize = 8.0f;
        float minNodeWidth = 60.0f;
        float minNodeHeight = 40.0f;
        float dashLength = 6.0f;
        float dashGap = 4.0f;
    };

    void setOptions(const Options& opts) { _options = opts; }
    Options options() const { return _options; }

private:
    DiagramRenderer(std::shared_ptr<YDrawBuilder> builder);

    std::shared_ptr<YDrawBuilder> _builder;
    Options _options;
    uint32_t _layer = 0;  // Current layer counter

    // Node rendering
    void renderNode(const GraphNode& node);
    void renderRectangle(const GraphNode& node);
    void renderRoundedRect(const GraphNode& node);
    void renderCircle(const GraphNode& node);
    void renderDiamond(const GraphNode& node);
    void renderEllipse(const GraphNode& node);
    void renderHexagon(const GraphNode& node);
    void renderParallelogram(const GraphNode& node);
    void renderCylinder(const GraphNode& node);
    void renderStadium(const GraphNode& node);

    // Edge rendering
    void renderEdge(const GraphEdge& edge, const Graph& graph);
    void renderArrowhead(float x, float y, float angle, ArrowStyle style,
                         uint32_t color, float size);
    void renderDashedLine(float x0, float y0, float x1, float y1,
                          uint32_t color, float width);

    // Text rendering
    void renderNodeLabel(const GraphNode& node);
    void renderEdgeLabel(const GraphEdge& edge);

    // Geometry utilities
    std::pair<float, float> getEdgeAttachPoint(const GraphNode& node,
                                                float targetX, float targetY);
    float computeAngle(float dx, float dy);
};

} // namespace yetty::diagram
