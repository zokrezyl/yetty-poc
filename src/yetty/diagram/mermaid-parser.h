#pragma once

#include "graph-ir.h"
#include <memory>

namespace yetty::diagram {

//=============================================================================
// MermaidParser - Parses Mermaid flowchart syntax
//
// Supported syntax:
//   graph TD/LR/BT/RL
//   A[text]      - rectangle
//   A(text)      - rounded rectangle
//   A((text))    - circle
//   A{text}      - diamond
//   A{{text}}    - hexagon
//   A[(text)]    - cylinder/database
//   A([text])    - stadium
//   A[/text/]    - parallelogram (right-leaning)
//   A[\text\]    - parallelogram (left-leaning)
//   A[/text\]    - trapezoid
//   A[\text/]    - inverted trapezoid
//   A --> B      - arrow
//   A --- B      - line (no arrow)
//   A -.-> B     - dashed arrow
//   A ==> B      - thick arrow
//   A -->|label| B  - arrow with label
//   subgraph name ... end
//=============================================================================
class MermaidParser : public GraphParser {
public:
    using Ptr = std::shared_ptr<MermaidParser>;

    static Result<Ptr> create();

    Result<Graph> parse(const std::string& input) override;
    bool canParse(const std::string& input) const override;

private:
    MermaidParser() = default;

    // Internal parsing
    Result<void> parseFlowchart(const std::string& content, Graph& graph);
    Result<void> parseLine(const std::string& line, Graph& graph);

    // Parse node definition: A[text], A(text), A((text)), A{text}, etc.
    bool parseNodeDef(const std::string& token, std::string& id,
                      std::string& label, NodeShape& shape);

    // Parse edge arrow: -->, -.->. ==>, ---
    bool parseArrow(const std::string& arrow, EdgeStyle& style);

    // Extract edge label from |label| syntax
    std::string extractEdgeLabel(std::string& token);

    // Helper to trim whitespace
    static std::string trim(const std::string& s);

    // State for subgraph parsing
    std::string _currentSubgraph;
};

} // namespace yetty::diagram
