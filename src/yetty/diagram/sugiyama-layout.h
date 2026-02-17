#pragma once

#include "layout-engine.h"
#include <unordered_set>
#include <unordered_map>

namespace yetty::diagram {

//=============================================================================
// SugiyamaLayout - Layered graph layout using Sugiyama algorithm
//
// The algorithm has these phases:
// 1. Cycle removal (make the graph a DAG)
// 2. Layer assignment (assign each node to a layer)
// 3. Dummy node insertion (for edges spanning multiple layers)
// 4. Crossing reduction (minimize edge crossings)
// 5. Node positioning (assign x coordinates)
// 6. Edge routing (compute control points)
//=============================================================================
class SugiyamaLayout : public LayoutEngine {
public:
    using Ptr = std::shared_ptr<SugiyamaLayout>;

    static Result<Ptr> create();

    Result<void> layout(Graph& graph, MeasureTextFn measureText = nullptr) override;

    void setParams(const Params& params) override { _params = params; }
    Params params() const override { return _params; }

private:
    SugiyamaLayout() = default;
    Params _params;

    // Phase 1: Remove cycles (make DAG)
    void removeCycles(Graph& graph);

    // Phase 2: Layer assignment (longest path)
    void assignLayers(Graph& graph);
    int computeMaxLayer(const std::string& nodeId, Graph& graph,
                        std::unordered_map<std::string, int>& memo);

    // Phase 3: Add dummy nodes for long edges
    void insertDummyNodes(Graph& graph);

    // Phase 4: Crossing reduction (barycenter method)
    void reduceCrossings(Graph& graph);
    void orderLayerByBarycenter(Graph& graph, int layer, bool downward,
                                 std::unordered_map<int, std::vector<std::string>>& layers);
    float barycenter(const std::string& nodeId, const Graph& graph,
                     const std::unordered_map<int, std::vector<std::string>>& layers,
                     bool downward);

    // Phase 5: Node positioning
    void positionNodes(Graph& graph, MeasureTextFn measureText);

    // Phase 6: Edge routing
    void routeEdges(Graph& graph);

    // Utility
    void computeGraphBounds(Graph& graph);
    void applyDirection(Graph& graph);  // Transform for LR/RL/BT

    // Track dummy nodes for edge routing
    std::vector<std::string> _dummyNodeIds;
    std::unordered_map<std::string, std::vector<std::string>> _edgeDummies; // edgeId -> dummy node ids
};

} // namespace yetty::diagram
