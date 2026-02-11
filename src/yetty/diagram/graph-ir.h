#pragma once

#include <yetty/result.hpp>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstdint>

namespace yetty::diagram {

//=============================================================================
// Node Shapes (maps to SDFType where possible)
//=============================================================================
enum class NodeShape {
    Rectangle,        // SDFType::Box (round=0)
    RoundedRect,      // SDFType::Box (round>0)
    Circle,           // SDFType::Circle
    Diamond,          // SDFType::Rhombus
    Ellipse,          // SDFType::Ellipse
    Hexagon,          // SDFType::Hexagon
    Parallelogram,    // SDFType::Parallelogram
    Cylinder,         // Custom (stacked ellipses + box)
    Stadium,          // SDFType::Capsule
    Trapezoid,        // SDFType::Trapezoid
    DoubleCircle      // Two concentric circles (for accept states)
};

//=============================================================================
// Edge Arrow Styles
//=============================================================================
enum class ArrowStyle {
    None,             // No arrowhead
    Normal,           // Standard filled triangle
    Open,             // Open triangle
    Diamond,          // Diamond shape
    Circle,           // Circle marker
    Dot               // Small dot
};

enum class LineStyle {
    Solid,
    Dashed,
    Dotted,
    Thick
};

//=============================================================================
// Colors and Styling
//=============================================================================
struct NodeStyle {
    uint32_t fillColor = 0xFFAA5533;      // Default blue (ABGR)
    uint32_t strokeColor = 0xFFFFFFFF;    // White border
    float strokeWidth = 2.0f;
    uint32_t textColor = 0xFFFFFFFF;      // White text
    float fontSize = 14.0f;
    float cornerRadius = 8.0f;            // For rounded rectangles
};

struct EdgeStyle {
    uint32_t strokeColor = 0xFFFFFFFF;
    float strokeWidth = 1.5f;
    LineStyle lineStyle = LineStyle::Solid;
    ArrowStyle sourceArrow = ArrowStyle::None;
    ArrowStyle targetArrow = ArrowStyle::Normal;
    uint32_t labelColor = 0xFFCCCCCC;
    float labelFontSize = 12.0f;
};

//=============================================================================
// Graph Node
//=============================================================================
struct GraphNode {
    std::string id;                       // Unique identifier
    std::string label;                    // Display text (may contain newlines)
    NodeShape shape = NodeShape::Rectangle;
    NodeStyle style;

    // Layout results (filled by layout engine)
    float x = 0, y = 0;                   // Center position
    float width = 0, height = 0;          // Computed from label + padding

    // For subgraphs/clusters
    std::string clusterId;                // Empty if not in a cluster

    // Metadata for layout algorithm
    int layer = -1;                       // Sugiyama layer assignment
    int position = -1;                    // Position within layer

    // Is this a dummy node (for edge routing)?
    bool isDummy = false;
};

//=============================================================================
// Graph Edge
//=============================================================================
struct GraphEdge {
    std::string id;                       // Unique identifier (auto-generated)
    std::string sourceId;
    std::string targetId;
    std::string label;                    // Optional edge label
    EdgeStyle style;

    // Layout results (filled by layout engine)
    struct Point { float x, y; };
    std::vector<Point> controlPoints;     // For Bezier curves
    Point labelPosition = {0, 0};         // Where to place edge label

    // For cycle removal
    bool reversed = false;                // True if edge was reversed for DAG
};

//=============================================================================
// Graph Cluster (subgraph)
//=============================================================================
struct GraphCluster {
    std::string id;
    std::string label;
    std::vector<std::string> nodeIds;     // Nodes in this cluster
    uint32_t fillColor = 0x40666666;      // Semi-transparent
    uint32_t strokeColor = 0xFF888888;

    // Layout results
    float x = 0, y = 0;
    float width = 0, height = 0;
};

//=============================================================================
// Graph Direction
//=============================================================================
enum class GraphDirection {
    TopToBottom,      // TB (default)
    BottomToTop,      // BT
    LeftToRight,      // LR
    RightToLeft       // RL
};

//=============================================================================
// Graph (complete IR)
//=============================================================================
struct Graph {
    std::string title;
    GraphDirection direction = GraphDirection::TopToBottom;

    std::unordered_map<std::string, GraphNode> nodes;
    std::vector<GraphEdge> edges;
    std::unordered_map<std::string, GraphCluster> clusters;

    // Global style defaults
    NodeStyle defaultNodeStyle;
    EdgeStyle defaultEdgeStyle;

    // Layout parameters
    float nodeSpacingX = 60.0f;           // Horizontal spacing between nodes
    float nodeSpacingY = 100.0f;          // Vertical spacing between layers
    float clusterPadding = 20.0f;         // Padding inside clusters

    // Computed bounds (after layout)
    float minX = 0, minY = 0;
    float maxX = 0, maxY = 0;

    // Helper methods
    GraphNode* getNode(const std::string& id) {
        auto it = nodes.find(id);
        return it != nodes.end() ? &it->second : nullptr;
    }

    const GraphNode* getNode(const std::string& id) const {
        auto it = nodes.find(id);
        return it != nodes.end() ? &it->second : nullptr;
    }

    void addNode(GraphNode node) {
        nodes[node.id] = std::move(node);
    }

    void addEdge(GraphEdge edge) {
        if (edge.id.empty()) {
            edge.id = "e" + std::to_string(edges.size());
        }
        edges.push_back(std::move(edge));
    }

    void addCluster(GraphCluster cluster) {
        clusters[cluster.id] = std::move(cluster);
    }

    // Get all edges from a node
    std::vector<const GraphEdge*> outEdges(const std::string& nodeId) const {
        std::vector<const GraphEdge*> result;
        for (const auto& edge : edges) {
            if (edge.sourceId == nodeId) {
                result.push_back(&edge);
            }
        }
        return result;
    }

    // Get all edges to a node
    std::vector<const GraphEdge*> inEdges(const std::string& nodeId) const {
        std::vector<const GraphEdge*> result;
        for (const auto& edge : edges) {
            if (edge.targetId == nodeId) {
                result.push_back(&edge);
            }
        }
        return result;
    }
};

//=============================================================================
// Parser Interface
//=============================================================================
class GraphParser {
public:
    virtual ~GraphParser() = default;
    virtual Result<Graph> parse(const std::string& input) = 0;
    virtual bool canParse(const std::string& input) const = 0;
};

} // namespace yetty::diagram
