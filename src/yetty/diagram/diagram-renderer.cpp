#include "diagram-renderer.h"
#include <yetty/ydraw-builder.h>
#include "../ydraw/ydraw-types.gen.h"
#include "../ydraw/ydraw-buffer.h"
#include <cmath>

namespace yetty::diagram {

Result<DiagramRenderer::Ptr> DiagramRenderer::create(std::shared_ptr<YDrawBuilder> builder,
                                                     std::shared_ptr<YDrawBuffer> buffer,
                                                     MeasureTextFn measureText) {
    if (!builder) {
        return Err<Ptr>("DiagramRenderer: null builder");
    }
    return Ok(Ptr(new DiagramRenderer(std::move(builder), std::move(buffer), std::move(measureText))));
}

DiagramRenderer::DiagramRenderer(std::shared_ptr<YDrawBuilder> builder,
                                 std::shared_ptr<YDrawBuffer> buffer,
                                 MeasureTextFn measureText)
    : _builder(std::move(builder))
    , _buffer(std::move(buffer))
    , _measureText(std::move(measureText))
{}

Result<void> DiagramRenderer::render(const Graph& graph) {
    _layer = 0;

    // Render clusters first (background)
    for (const auto& [id, cluster] : graph.clusters) {
        if (cluster.width > 0 && cluster.height > 0) {
            _buffer->addBox(
                _layer++,
                cluster.x + cluster.width / 2,
                cluster.y + cluster.height / 2,
                cluster.width / 2,
                cluster.height / 2,
                cluster.fillColor,
                cluster.strokeColor,
                1.0f,
                5.0f
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
    _buffer->addBox(
        _layer++,
        node.x, node.y,
        node.width / 2, node.height / 2,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        0.0f  // No rounding
    );
}

void DiagramRenderer::renderRoundedRect(const GraphNode& node) {
    _buffer->addBox(
        _layer++,
        node.x, node.y,
        node.width / 2, node.height / 2,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        node.style.cornerRadius
    );
}

void DiagramRenderer::renderCircle(const GraphNode& node) {
    float radius = std::min(node.width, node.height) / 2;
    _buffer->addCircle(
        _layer++,
        node.x, node.y,
        radius,
        node.style.fillColor,
        node.style.strokeColor,
        node.style.strokeWidth,
        0.0f
    );
}

void DiagramRenderer::renderDiamond(const GraphNode& node) {
    _buffer->addRhombus(
        _layer++, node.x, node.y, node.width / 2, node.height / 2,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);
}

void DiagramRenderer::renderEllipse(const GraphNode& node) {
    _buffer->addEllipse(
        _layer++, node.x, node.y, node.width / 2, node.height / 2,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);
}

void DiagramRenderer::renderHexagon(const GraphNode& node) {
    _buffer->addHexagon(
        _layer++, node.x, node.y, std::min(node.width, node.height) / 2,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);
}

void DiagramRenderer::renderParallelogram(const GraphNode& node) {
    _buffer->addParallelogram(
        _layer++, node.x, node.y, node.width / 2, node.height / 2, node.width * 0.2f,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);
}

void DiagramRenderer::renderCylinder(const GraphNode& node) {
    // Cylinder is rendered as: top ellipse + rectangle + bottom ellipse
    float ellipseHeight = node.height * 0.15f;
    float bodyHeight = node.height - ellipseHeight;

    // Body (rectangle)
    _buffer->addBox(
        _layer++,
        node.x, node.y,
        node.width / 2, bodyHeight / 2,
        node.style.fillColor,
        0,  // No stroke on body
        0,
        0
    );

    // Top ellipse
    _buffer->addEllipse(
        _layer++, node.x, node.y - bodyHeight / 2, node.width / 2, ellipseHeight,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);

    // Bottom ellipse (partial, just the visible part)
    _buffer->addEllipse(
        _layer++, node.x, node.y + bodyHeight / 2, node.width / 2, ellipseHeight,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);

    // Side lines
    _buffer->addSegment(
        _layer++,
        node.x - node.width / 2, node.y - bodyHeight / 2,
        node.x - node.width / 2, node.y + bodyHeight / 2,
        0,
        node.style.strokeColor,
        node.style.strokeWidth,
        0.0f
    );
    _buffer->addSegment(
        _layer++,
        node.x + node.width / 2, node.y - bodyHeight / 2,
        node.x + node.width / 2, node.y + bodyHeight / 2,
        0,
        node.style.strokeColor,
        node.style.strokeWidth,
        0.0f
    );
}

void DiagramRenderer::renderStadium(const GraphNode& node) {
    float radius = node.height / 2;
    _buffer->addCapsule(
        _layer++,
        node.x - node.width / 2 + radius, node.y,
        node.x + node.width / 2 - radius, node.y,
        radius,
        node.style.fillColor, node.style.strokeColor, node.style.strokeWidth, 0.0f);
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
        _buffer->addSegment(
            _layer++,
            startX, startY, endX, endY,
            0,
            edge.style.strokeColor,
            edge.style.strokeWidth,
            0.0f
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
    _buffer->addTriangle(
        _layer++, tipX, tipY, leftX, leftY, rightX, rightY,
        color, 0, 0.0f, 0.0f);
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
        _buffer->addSegment(
            _layer,
            x0 + unitX * pos, y0 + unitY * pos,
            x0 + unitX * dashEnd, y0 + unitY * dashEnd,
            0, color, width, 0.0f
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
    float textWidth = _measureText ? _measureText(node.label, node.style.fontSize)
                                   : node.style.fontSize * 0.6f * node.label.size();
    float textX = node.x - textWidth / 2;
    float textY = node.y + node.style.fontSize / 3;  // Approximate vertical centering

    _buffer->addText(textX, textY, node.label, node.style.fontSize,
                     node.style.textColor, _layer++, -1);
}

void DiagramRenderer::renderEdgeLabel(const GraphEdge& edge) {
    if (edge.label.empty()) return;

    // Render label at the midpoint
    float textWidth = _measureText ? _measureText(edge.label, edge.style.labelFontSize)
                                   : edge.style.labelFontSize * 0.6f * edge.label.size();
    float textX = edge.labelPosition.x - textWidth / 2;
    float textY = edge.labelPosition.y + edge.style.labelFontSize / 3;

    // Add a small background box
    float padding = 3.0f;
    _buffer->addBox(
        _layer++,
        edge.labelPosition.x, edge.labelPosition.y,
        textWidth / 2 + padding, edge.style.labelFontSize / 2 + padding,
        0xFF1A1A2E,  // Dark background
        0, 0, 2.0f
    );

    _buffer->addText(textX, textY, edge.label, edge.style.labelFontSize,
                     edge.style.labelColor, _layer++, -1);
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
