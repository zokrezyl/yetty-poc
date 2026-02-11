#include "sugiyama-layout.h"
#include <yetty/ydraw/ydraw-builder.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>

namespace yetty::diagram {

Result<SugiyamaLayout::Ptr> SugiyamaLayout::create() {
    return Ok(Ptr(new SugiyamaLayout()));
}

Result<void> SugiyamaLayout::layout(Graph& graph, YDrawBuilder* builder) {
    if (graph.nodes.empty()) {
        return Ok();
    }

    // Clear any previous dummy nodes
    _dummyNodeIds.clear();
    _edgeDummies.clear();

    // Phase 1: Remove cycles
    removeCycles(graph);

    // Phase 2: Assign layers
    assignLayers(graph);

    // Phase 3: Insert dummy nodes for long edges
    insertDummyNodes(graph);

    // Phase 4: Reduce crossings
    reduceCrossings(graph);

    // Phase 5: Position nodes
    positionNodes(graph, builder);

    // Phase 6: Route edges
    routeEdges(graph);

    // Compute bounds
    computeGraphBounds(graph);

    // Apply direction transform (for LR, RL, BT)
    applyDirection(graph);

    return Ok();
}

//=============================================================================
// Phase 1: Cycle Removal using DFS
//=============================================================================
void SugiyamaLayout::removeCycles(Graph& graph) {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> inStack;

    std::function<void(const std::string&)> dfs = [&](const std::string& nodeId) {
        visited.insert(nodeId);
        inStack.insert(nodeId);

        for (auto& edge : graph.edges) {
            if (edge.sourceId == nodeId) {
                if (inStack.count(edge.targetId)) {
                    // Back edge found - reverse it
                    std::swap(edge.sourceId, edge.targetId);
                    edge.reversed = true;
                } else if (!visited.count(edge.targetId)) {
                    dfs(edge.targetId);
                }
            }
        }

        inStack.erase(nodeId);
    };

    for (const auto& [id, _] : graph.nodes) {
        if (!visited.count(id)) {
            dfs(id);
        }
    }
}

//=============================================================================
// Phase 2: Layer Assignment (longest path from sinks)
//=============================================================================
void SugiyamaLayout::assignLayers(Graph& graph) {
    std::unordered_map<std::string, int> memo;

    // Find maximum layer for each node
    for (auto& [id, node] : graph.nodes) {
        node.layer = computeMaxLayer(id, graph, memo);
    }

    // Find the maximum layer
    int maxLayer = 0;
    for (const auto& [id, node] : graph.nodes) {
        maxLayer = std::max(maxLayer, node.layer);
    }

    // Invert layers so sources are at top (layer 0)
    for (auto& [id, node] : graph.nodes) {
        node.layer = maxLayer - node.layer;
    }
}

int SugiyamaLayout::computeMaxLayer(const std::string& nodeId, Graph& graph,
                                     std::unordered_map<std::string, int>& memo) {
    auto it = memo.find(nodeId);
    if (it != memo.end()) {
        return it->second;
    }

    int maxChildLayer = -1;
    for (const auto& edge : graph.edges) {
        if (edge.sourceId == nodeId) {
            int childLayer = computeMaxLayer(edge.targetId, graph, memo);
            maxChildLayer = std::max(maxChildLayer, childLayer);
        }
    }

    int layer = maxChildLayer + 1;
    memo[nodeId] = layer;
    return layer;
}

//=============================================================================
// Phase 3: Insert Dummy Nodes
//=============================================================================
void SugiyamaLayout::insertDummyNodes(Graph& graph) {
    std::vector<GraphEdge> newEdges;
    std::vector<size_t> edgesToRemove;

    for (size_t i = 0; i < graph.edges.size(); i++) {
        auto& edge = graph.edges[i];
        auto* sourceNode = graph.getNode(edge.sourceId);
        auto* targetNode = graph.getNode(edge.targetId);

        if (!sourceNode || !targetNode) continue;

        int span = targetNode->layer - sourceNode->layer;
        if (span <= 1) continue;

        // Create dummy nodes for each intermediate layer
        std::vector<std::string> dummies;
        std::string prevId = edge.sourceId;

        for (int layer = sourceNode->layer + 1; layer < targetNode->layer; layer++) {
            std::string dummyId = "_d_" + edge.id + "_" + std::to_string(layer);

            GraphNode dummy;
            dummy.id = dummyId;
            dummy.label = "";
            dummy.layer = layer;
            dummy.isDummy = true;
            dummy.width = 0;
            dummy.height = 0;
            graph.nodes[dummyId] = dummy;
            _dummyNodeIds.push_back(dummyId);
            dummies.push_back(dummyId);

            // Create edge from prev to dummy
            GraphEdge e;
            e.id = edge.id + "_seg_" + std::to_string(layer);
            e.sourceId = prevId;
            e.targetId = dummyId;
            e.style = edge.style;
            e.style.sourceArrow = ArrowStyle::None;
            e.style.targetArrow = ArrowStyle::None;
            newEdges.push_back(e);

            prevId = dummyId;
        }

        // Final edge from last dummy to target
        GraphEdge finalEdge;
        finalEdge.id = edge.id + "_seg_final";
        finalEdge.sourceId = prevId;
        finalEdge.targetId = edge.targetId;
        finalEdge.style = edge.style;
        finalEdge.style.sourceArrow = ArrowStyle::None;
        finalEdge.label = edge.label;
        newEdges.push_back(finalEdge);

        _edgeDummies[edge.id] = dummies;
        edgesToRemove.push_back(i);
    }

    // Remove original long edges (in reverse order)
    std::sort(edgesToRemove.rbegin(), edgesToRemove.rend());
    for (size_t idx : edgesToRemove) {
        graph.edges.erase(graph.edges.begin() + static_cast<long>(idx));
    }

    // Add new edges
    for (auto& e : newEdges) {
        graph.edges.push_back(std::move(e));
    }
}

//=============================================================================
// Phase 4: Crossing Reduction (Barycenter method)
//=============================================================================
void SugiyamaLayout::reduceCrossings(Graph& graph) {
    // Group nodes by layer
    std::unordered_map<int, std::vector<std::string>> layers;
    int maxLayer = 0;

    for (const auto& [id, node] : graph.nodes) {
        layers[node.layer].push_back(id);
        maxLayer = std::max(maxLayer, node.layer);
    }

    // Initialize positions within each layer
    for (auto& [layer, nodeIds] : layers) {
        int pos = 0;
        for (const auto& id : nodeIds) {
            graph.nodes[id].position = pos++;
        }
    }

    // Iterate: sweep down then up
    for (uint32_t iter = 0; iter < _params.maxIterations; iter++) {
        // Sweep down
        for (int layer = 1; layer <= maxLayer; layer++) {
            orderLayerByBarycenter(graph, layer, true, layers);
        }
        // Sweep up
        for (int layer = maxLayer - 1; layer >= 0; layer--) {
            orderLayerByBarycenter(graph, layer, false, layers);
        }
    }
}

void SugiyamaLayout::orderLayerByBarycenter(Graph& graph, int layer, bool downward,
                                             std::unordered_map<int, std::vector<std::string>>& layers) {
    auto& nodeIds = layers[layer];

    // Compute barycenter for each node
    std::vector<std::pair<float, std::string>> barycenters;
    for (const auto& id : nodeIds) {
        float bc = barycenter(id, graph, layers, downward);
        barycenters.emplace_back(bc, id);
    }

    // Sort by barycenter
    std::sort(barycenters.begin(), barycenters.end());

    // Update positions
    nodeIds.clear();
    int pos = 0;
    for (const auto& [bc, id] : barycenters) {
        nodeIds.push_back(id);
        graph.nodes[id].position = pos++;
    }
}

float SugiyamaLayout::barycenter(const std::string& nodeId, const Graph& graph,
                                  const std::unordered_map<int, std::vector<std::string>>& layers,
                                  bool downward) {
    float sum = 0;
    int count = 0;

    for (const auto& edge : graph.edges) {
        std::string neighbor;
        if (downward) {
            // Look at predecessors (edges coming into this node)
            if (edge.targetId == nodeId) {
                neighbor = edge.sourceId;
            }
        } else {
            // Look at successors (edges going out of this node)
            if (edge.sourceId == nodeId) {
                neighbor = edge.targetId;
            }
        }

        if (!neighbor.empty()) {
            auto* neighborNode = graph.getNode(neighbor);
            if (neighborNode) {
                sum += static_cast<float>(neighborNode->position);
                count++;
            }
        }
    }

    if (count == 0) {
        // No neighbors - keep current position
        auto* node = graph.getNode(nodeId);
        return node ? static_cast<float>(node->position) : 0.0f;
    }

    return sum / static_cast<float>(count);
}

//=============================================================================
// Phase 5: Node Positioning
//=============================================================================
void SugiyamaLayout::positionNodes(Graph& graph, YDrawBuilder* builder) {
    // First, compute node sizes based on labels
    float defaultWidth = 80.0f;
    float defaultHeight = 40.0f;

    for (auto& [id, node] : graph.nodes) {
        if (node.isDummy) {
            node.width = 0;
            node.height = 0;
            continue;
        }

        // Measure text if builder is available
        if (builder && !node.label.empty()) {
            float textWidth = builder->measureTextWidth(node.label, node.style.fontSize);
            node.width = textWidth + _params.nodePaddingX * 2;
            node.height = node.style.fontSize + _params.nodePaddingY * 2;

            // Enforce minimum size
            node.width = std::max(node.width, defaultWidth);
            node.height = std::max(node.height, defaultHeight);

            // Adjust for shape
            if (node.shape == NodeShape::Circle) {
                float diameter = std::max(node.width, node.height);
                node.width = node.height = diameter;
            } else if (node.shape == NodeShape::Diamond) {
                // Diamonds need more space
                node.width *= 1.4f;
                node.height *= 1.4f;
            }
        } else {
            node.width = defaultWidth;
            node.height = defaultHeight;
        }
    }

    // Group nodes by layer
    std::unordered_map<int, std::vector<std::string>> layers;
    int maxLayer = 0;

    for (const auto& [id, node] : graph.nodes) {
        layers[node.layer].push_back(id);
        maxLayer = std::max(maxLayer, node.layer);
    }

    // Sort each layer by position
    for (auto& [layer, nodeIds] : layers) {
        std::sort(nodeIds.begin(), nodeIds.end(), [&graph](const std::string& a, const std::string& b) {
            return graph.nodes[a].position < graph.nodes[b].position;
        });
    }

    // Assign Y coordinates (layer position)
    float currentY = _params.nodePaddingY;
    for (int layer = 0; layer <= maxLayer; layer++) {
        float maxHeight = 0;
        for (const auto& id : layers[layer]) {
            auto& node = graph.nodes[id];
            maxHeight = std::max(maxHeight, node.height);
        }

        for (const auto& id : layers[layer]) {
            auto& node = graph.nodes[id];
            node.y = currentY + maxHeight / 2;
        }

        currentY += maxHeight + _params.nodeSpacingY;
    }

    // Assign X coordinates (position within layer)
    for (int layer = 0; layer <= maxLayer; layer++) {
        const auto& nodeIds = layers[layer];
        float totalWidth = 0;
        for (const auto& id : nodeIds) {
            totalWidth += graph.nodes[id].width;
        }
        totalWidth += static_cast<float>(nodeIds.size() - 1) * _params.nodeSpacingX;

        float currentX = -totalWidth / 2;  // Center the layer
        for (const auto& id : nodeIds) {
            auto& node = graph.nodes[id];
            node.x = currentX + node.width / 2;
            currentX += node.width + _params.nodeSpacingX;
        }
    }

    // Shift all nodes to positive coordinates
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    for (const auto& [id, node] : graph.nodes) {
        minX = std::min(minX, node.x - node.width / 2);
        minY = std::min(minY, node.y - node.height / 2);
    }

    float offsetX = -minX + _params.nodePaddingX;
    float offsetY = -minY + _params.nodePaddingY;
    for (auto& [id, node] : graph.nodes) {
        node.x += offsetX;
        node.y += offsetY;
    }
}

//=============================================================================
// Phase 6: Edge Routing
//=============================================================================
void SugiyamaLayout::routeEdges(Graph& graph) {
    for (auto& edge : graph.edges) {
        auto* source = graph.getNode(edge.sourceId);
        auto* target = graph.getNode(edge.targetId);

        if (!source || !target) continue;

        edge.controlPoints.clear();

        // Simple straight line routing for now
        // For edges with dummy nodes, the control points go through the dummies
        float startX = source->x;
        float startY = source->y + source->height / 2;  // Bottom of source
        float endX = target->x;
        float endY = target->y - target->height / 2;    // Top of target

        // Add control points
        edge.controlPoints.push_back({startX, startY});
        edge.controlPoints.push_back({endX, endY});

        // Position edge label at midpoint
        edge.labelPosition.x = (startX + endX) / 2;
        edge.labelPosition.y = (startY + endY) / 2;
    }
}

//=============================================================================
// Utility
//=============================================================================
void SugiyamaLayout::computeGraphBounds(Graph& graph) {
    graph.minX = std::numeric_limits<float>::max();
    graph.minY = std::numeric_limits<float>::max();
    graph.maxX = std::numeric_limits<float>::lowest();
    graph.maxY = std::numeric_limits<float>::lowest();

    for (const auto& [id, node] : graph.nodes) {
        if (node.isDummy) continue;

        graph.minX = std::min(graph.minX, node.x - node.width / 2);
        graph.minY = std::min(graph.minY, node.y - node.height / 2);
        graph.maxX = std::max(graph.maxX, node.x + node.width / 2);
        graph.maxY = std::max(graph.maxY, node.y + node.height / 2);
    }

    // Add padding
    graph.minX -= 20;
    graph.minY -= 20;
    graph.maxX += 20;
    graph.maxY += 20;
}

void SugiyamaLayout::applyDirection(Graph& graph) {
    if (graph.direction == GraphDirection::TopToBottom) {
        // Default - no transform needed
        return;
    }

    float centerX = (graph.minX + graph.maxX) / 2;
    float centerY = (graph.minY + graph.maxY) / 2;

    for (auto& [id, node] : graph.nodes) {
        float dx = node.x - centerX;
        float dy = node.y - centerY;

        switch (graph.direction) {
            case GraphDirection::BottomToTop:
                // Flip vertically
                node.y = centerY - dy;
                break;
            case GraphDirection::LeftToRight:
                // Rotate 90 degrees: x -> y, y -> x
                node.x = centerX + dy;
                node.y = centerY + dx;
                std::swap(node.width, node.height);
                break;
            case GraphDirection::RightToLeft:
                // Rotate -90 degrees
                node.x = centerX - dy;
                node.y = centerY + dx;
                std::swap(node.width, node.height);
                break;
            default:
                break;
        }
    }

    // Update edge control points
    for (auto& edge : graph.edges) {
        for (auto& pt : edge.controlPoints) {
            float dx = pt.x - centerX;
            float dy = pt.y - centerY;

            switch (graph.direction) {
                case GraphDirection::BottomToTop:
                    pt.y = centerY - dy;
                    break;
                case GraphDirection::LeftToRight:
                    pt.x = centerX + dy;
                    pt.y = centerY + dx;
                    break;
                case GraphDirection::RightToLeft:
                    pt.x = centerX - dy;
                    pt.y = centerY + dx;
                    break;
                default:
                    break;
            }
        }

        // Update label position
        float dx = edge.labelPosition.x - centerX;
        float dy = edge.labelPosition.y - centerY;
        switch (graph.direction) {
            case GraphDirection::BottomToTop:
                edge.labelPosition.y = centerY - dy;
                break;
            case GraphDirection::LeftToRight:
                edge.labelPosition.x = centerX + dy;
                edge.labelPosition.y = centerY + dx;
                break;
            case GraphDirection::RightToLeft:
                edge.labelPosition.x = centerX - dy;
                edge.labelPosition.y = centerY + dx;
                break;
            default:
                break;
        }
    }

    // Recompute bounds
    computeGraphBounds(graph);
}

} // namespace yetty::diagram
