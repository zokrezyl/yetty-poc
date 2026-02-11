#include "diagram-renderer.h"
#include <yetty/ydraw/ydraw-builder.h>
#include "../cards/hdraw/hdraw.h"  // For SDFPrimitive, SDFType
#include <cmath>

namespace yetty::diagram {

Result<DiagramRenderer::Ptr> DiagramRenderer::create(std::shared_ptr<YDrawBuilder> builder) {
    if (!builder) {
        return Err<Ptr>("DiagramRenderer: null builder");
    }
    return Ok(Ptr(new DiagramRenderer(std::move(builder))));
}

DiagramRenderer::DiagramRenderer(std::shared_ptr<YDrawBuilder> builder)
    : _builder(std::move(builder))
{}

Result<void> DiagramRenderer::render(const Graph& graph) {
    _layer = 0;

    // Render clusters first (background)
    for (const auto& [id, cluster] : graph.clusters) {
        if (cluster.width > 0 && cluster.height > 0) {
            _builder->addBox(
                cluster.x + cluster.width / 2,
                cluster.y + cluster.height / 2,
                cluster.width / 2,
                cluster.height / 2,
                cluster.fillColor,
                cluster.strokeColor,
                1.0f,
                5.0f,
                _layer++
            );
        }
    }

    // Render edges (behind nodes)
    for (const auto& edge : graph.edges) {
        renderEdge(edge, graph);
    }

    // Render nodes
    for (const auto& [id, node] : graph.nodes) {
        if (!node.isDummy) {
            renderNode(node);
        }
    }

    return Ok();
}

//=============================================================================
// Node Rendering
//=============================================================================

void DiagramRenderer::renderNode(const GraphNode& node) {
    switch (node.shape) {
        case NodeShape::Rectangle:
            renderRectangle(node);
            break;
        case NodeShape::RoundedRect:
            renderRoundedRect(node);
            break;
        case NodeShape::Circle:
            renderCircle(node);
            break;
        case NodeShape::Diamond:
            renderDiamond(node);
            break;
        case NodeShape::Ellipse:
            renderEllipse(node);
            break;
        case NodeShape::Hexagon:
            renderHexagon(node);
            break;
        case NodeShape::Parallelogram:
            renderParallelogram(node);
            break;
        case NodeShape::Cylinder:
            renderCylinder(node);
            break;
        case NodeShape::Stadium:
            renderStadium(node);
            break;
        default:
            renderRectangle(node);
            break;
    }

    // Render label
    renderNodeLabel(node);
}

void DiagramRenderer::renderRectangle(const GraphNode& node) {
    _builder->addBox(
        node.x, node.y,
        node.width / 2, node.height / 2,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        0.0f,  // No rounding
        _layer++
    );
}

void DiagramRenderer::renderRoundedRect(const GraphNode& node) {
    _builder->addBox(
        node.x, node.y,
        node.width / 2, node.height / 2,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        node.style.cornerRadius,
        _layer++
    );
}

void DiagramRenderer::renderCircle(const GraphNode& node) {
    float radius = std::min(node.width, node.height) / 2;
    _builder->addCircle(
        node.x, node.y,
        radius,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        _layer++
    );
}

void DiagramRenderer::renderDiamond(const GraphNode& node) {
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Rhombus);
    prim.layer = _layer++;
    prim.params[0] = node.x;
    prim.params[1] = node.y;
    prim.params[2] = node.width / 2;
    prim.params[3] = node.height / 2;
    prim.fillColor = node.style.fillColor;
    prim.strokeColor = node.style.strokeColor;
    prim.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(prim);
}

void DiagramRenderer::renderEllipse(const GraphNode& node) {
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Ellipse);
    prim.layer = _layer++;
    prim.params[0] = node.x;
    prim.params[1] = node.y;
    prim.params[2] = node.width / 2;
    prim.params[3] = node.height / 2;
    prim.fillColor = node.style.fillColor;
    prim.strokeColor = node.style.strokeColor;
    prim.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(prim);
}

void DiagramRenderer::renderHexagon(const GraphNode& node) {
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Hexagon);
    prim.layer = _layer++;
    prim.params[0] = node.x;
    prim.params[1] = node.y;
    prim.params[2] = std::min(node.width, node.height) / 2;
    prim.fillColor = node.style.fillColor;
    prim.strokeColor = node.style.strokeColor;
    prim.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(prim);
}

void DiagramRenderer::renderParallelogram(const GraphNode& node) {
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Parallelogram);
    prim.layer = _layer++;
    prim.params[0] = node.x;
    prim.params[1] = node.y;
    prim.params[2] = node.width / 2;
    prim.params[3] = node.height / 2;
    prim.params[4] = node.width * 0.2f;  // Skew
    prim.fillColor = node.style.fillColor;
    prim.strokeColor = node.style.strokeColor;
    prim.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(prim);
}

void DiagramRenderer::renderCylinder(const GraphNode& node) {
    // Cylinder is rendered as: top ellipse + rectangle + bottom ellipse
    float ellipseHeight = node.height * 0.15f;
    float bodyHeight = node.height - ellipseHeight;

    // Body (rectangle)
    _builder->addBox(
        node.x, node.y,
        node.width / 2, bodyHeight / 2,
        node.style.fillColor,
        0,  // No stroke on body
        0,
        0,
        _layer++
    );

    // Top ellipse
    card::SDFPrimitive topEllipse = {};
    topEllipse.type = static_cast<uint32_t>(card::SDFType::Ellipse);
    topEllipse.layer = _layer++;
    topEllipse.params[0] = node.x;
    topEllipse.params[1] = node.y - bodyHeight / 2;
    topEllipse.params[2] = node.width / 2;
    topEllipse.params[3] = ellipseHeight;
    topEllipse.fillColor = node.style.fillColor;
    topEllipse.strokeColor = node.style.strokeColor;
    topEllipse.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(topEllipse);

    // Bottom ellipse (partial, just the visible part)
    card::SDFPrimitive bottomEllipse = {};
    bottomEllipse.type = static_cast<uint32_t>(card::SDFType::Ellipse);
    bottomEllipse.layer = _layer++;
    bottomEllipse.params[0] = node.x;
    bottomEllipse.params[1] = node.y + bodyHeight / 2;
    bottomEllipse.params[2] = node.width / 2;
    bottomEllipse.params[3] = ellipseHeight;
    bottomEllipse.fillColor = node.style.fillColor;
    bottomEllipse.strokeColor = node.style.strokeColor;
    bottomEllipse.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(bottomEllipse);

    // Side lines
    _builder->addSegment(
        node.x - node.width / 2, node.y - bodyHeight / 2,
        node.x - node.width / 2, node.y + bodyHeight / 2,
        node.style.strokeColor,
        node.style.strokeWidth,
        _layer++
    );
    _builder->addSegment(
        node.x + node.width / 2, node.y - bodyHeight / 2,
        node.x + node.width / 2, node.y + bodyHeight / 2,
        node.style.strokeColor,
        node.style.strokeWidth,
        _layer++
    );
}

void DiagramRenderer::renderStadium(const GraphNode& node) {
    // Stadium (pill shape) - using capsule
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Capsule);
    prim.layer = _layer++;
    float radius = node.height / 2;
    prim.params[0] = node.x - node.width / 2 + radius;  // Left center x
    prim.params[1] = node.y;                             // y
    prim.params[2] = node.x + node.width / 2 - radius;  // Right center x
    prim.params[3] = node.y;                             // y
    prim.params[4] = radius;                             // Radius
    prim.fillColor = node.style.fillColor;
    prim.strokeColor = node.style.strokeColor;
    prim.strokeWidth = node.style.strokeWidth;
    _builder->addPrimitive(prim);
}

//=============================================================================
// Edge Rendering
//=============================================================================

void DiagramRenderer::renderEdge(const GraphEdge& edge, const Graph& graph) {
    auto* source = graph.getNode(edge.sourceId);
    auto* target = graph.getNode(edge.targetId);

    if (!source || !target) return;

    // Get attachment points on the node boundaries
    auto [startX, startY] = getEdgeAttachPoint(*source, target->x, target->y);
    auto [endX, endY] = getEdgeAttachPoint(*target, source->x, source->y);

    // Render line based on style
    if (edge.style.lineStyle == LineStyle::Dashed) {
        renderDashedLine(startX, startY, endX, endY,
                         edge.style.strokeColor, edge.style.strokeWidth);
    } else {
        _builder->addSegment(
            startX, startY, endX, endY,
            edge.style.strokeColor,
            edge.style.strokeWidth,
            _layer++
        );
    }

    // Render arrowheads
    if (edge.style.targetArrow != ArrowStyle::None) {
        float angle = computeAngle(endX - startX, endY - startY);
        renderArrowhead(endX, endY, angle, edge.style.targetArrow,
                        edge.style.strokeColor, _options.arrowSize);
    }

    if (edge.style.sourceArrow != ArrowStyle::None) {
        float angle = computeAngle(startX - endX, startY - endY);
        renderArrowhead(startX, startY, angle, edge.style.sourceArrow,
                        edge.style.strokeColor, _options.arrowSize);
    }

    // Render edge label
    if (!edge.label.empty()) {
        renderEdgeLabel(edge);
    }
}

void DiagramRenderer::renderArrowhead(float x, float y, float angle,
                                       ArrowStyle style, uint32_t color, float size) {
    if (style == ArrowStyle::None) return;

    // Create a triangle arrowhead
    float cos_a = std::cos(angle);
    float sin_a = std::sin(angle);

    // Arrow points
    float tipX = x;
    float tipY = y;

    // Back points (spread at 30 degrees from the line)
    float spread = 0.4f;  // ~25 degrees
    float backDist = size;

    float backCenterX = x - cos_a * backDist;
    float backCenterY = y - sin_a * backDist;

    float perpX = -sin_a * size * spread;
    float perpY = cos_a * size * spread;

    float leftX = backCenterX + perpX;
    float leftY = backCenterY + perpY;
    float rightX = backCenterX - perpX;
    float rightY = backCenterY - perpY;

    // Render as filled triangle
    card::SDFPrimitive prim = {};
    prim.type = static_cast<uint32_t>(card::SDFType::Triangle);
    prim.layer = _layer++;
    prim.params[0] = tipX;
    prim.params[1] = tipY;
    prim.params[2] = leftX;
    prim.params[3] = leftY;
    prim.params[4] = rightX;
    prim.params[5] = rightY;
    prim.fillColor = color;
    prim.strokeColor = 0;
    prim.strokeWidth = 0;
    _builder->addPrimitive(prim);
}

void DiagramRenderer::renderDashedLine(float x0, float y0, float x1, float y1,
                                        uint32_t color, float width) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length < 0.01f) return;

    float unitX = dx / length;
    float unitY = dy / length;

    float dashLen = _options.dashLength;
    float gapLen = _options.dashGap;
    float segmentLen = dashLen + gapLen;

    float pos = 0;
    while (pos < length) {
        float dashEnd = std::min(pos + dashLen, length);
        _builder->addSegment(
            x0 + unitX * pos, y0 + unitY * pos,
            x0 + unitX * dashEnd, y0 + unitY * dashEnd,
            color, width, _layer
        );
        pos += segmentLen;
    }
    _layer++;
}

//=============================================================================
// Text Rendering
//=============================================================================

void DiagramRenderer::renderNodeLabel(const GraphNode& node) {
    if (node.label.empty()) return;

    // Center the text in the node
    float textWidth = _builder->measureTextWidth(node.label, node.style.fontSize);
    float textX = node.x - textWidth / 2;
    float textY = node.y + node.style.fontSize / 3;  // Approximate vertical centering

    _builder->addText(textX, textY, node.label, node.style.fontSize,
                      node.style.textColor, _layer++);
}

void DiagramRenderer::renderEdgeLabel(const GraphEdge& edge) {
    if (edge.label.empty()) return;

    // Render label at the midpoint
    float textWidth = _builder->measureTextWidth(edge.label, edge.style.labelFontSize);
    float textX = edge.labelPosition.x - textWidth / 2;
    float textY = edge.labelPosition.y + edge.style.labelFontSize / 3;

    // Add a small background box
    float padding = 3.0f;
    _builder->addBox(
        edge.labelPosition.x, edge.labelPosition.y,
        textWidth / 2 + padding, edge.style.labelFontSize / 2 + padding,
        0xFF1A1A2E,  // Dark background
        0, 0, 2.0f,
        _layer++
    );

    _builder->addText(textX, textY, edge.label, edge.style.labelFontSize,
                      edge.style.labelColor, _layer++);
}

//=============================================================================
// Geometry Utilities
//=============================================================================

std::pair<float, float> DiagramRenderer::getEdgeAttachPoint(const GraphNode& node,
                                                             float targetX, float targetY) {
    float dx = targetX - node.x;
    float dy = targetY - node.y;
    float angle = std::atan2(dy, dx);

    float hw = node.width / 2;
    float hh = node.height / 2;

    switch (node.shape) {
        case NodeShape::Circle: {
            float radius = std::min(hw, hh);
            return {node.x + radius * std::cos(angle), node.y + radius * std::sin(angle)};
        }
        case NodeShape::Diamond: {
            // Diamond intersection
            float ax = std::abs(std::cos(angle));
            float ay = std::abs(std::sin(angle));
            float t = 1.0f / (ax / hw + ay / hh);
            return {node.x + t * std::cos(angle), node.y + t * std::sin(angle)};
        }
        default: {
            // Rectangle/rounded rect intersection
            float tx = hw / std::abs(std::cos(angle) + 0.001f);
            float ty = hh / std::abs(std::sin(angle) + 0.001f);
            float t = std::min(tx, ty);
            return {node.x + t * std::cos(angle), node.y + t * std::sin(angle)};
        }
    }
}

float DiagramRenderer::computeAngle(float dx, float dy) {
    return std::atan2(dy, dx);
}

} // namespace yetty::diagram
