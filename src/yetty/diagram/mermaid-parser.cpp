#include "mermaid-parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace yetty::diagram {

Result<MermaidParser::Ptr> MermaidParser::create() {
    return Ok(Ptr(new MermaidParser()));
}

bool MermaidParser::canParse(const std::string& input) const {
    // Check for Mermaid flowchart keywords
    auto trimmed = trim(input);
    return trimmed.find("graph ") == 0 ||
           trimmed.find("flowchart ") == 0 ||
           trimmed.find("graph\n") == 0 ||
           trimmed.find("flowchart\n") == 0;
}

Result<Graph> MermaidParser::parse(const std::string& input) {
    Graph graph;
    if (auto res = parseFlowchart(input, graph); !res) {
        return Err<Graph>("Mermaid parse failed", res);
    }
    return Ok(std::move(graph));
}

Result<void> MermaidParser::parseFlowchart(const std::string& content, Graph& graph) {
    std::istringstream stream(content);
    std::string line;
    bool inGraph = false;

    while (std::getline(stream, line)) {
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '%') continue;

        // Parse graph direction
        if (line.find("graph ") == 0 || line.find("flowchart ") == 0) {
            inGraph = true;
            size_t pos = line.find(' ');
            if (pos != std::string::npos) {
                std::string dir = trim(line.substr(pos + 1));
                if (dir == "TD" || dir == "TB") {
                    graph.direction = GraphDirection::TopToBottom;
                } else if (dir == "BT") {
                    graph.direction = GraphDirection::BottomToTop;
                } else if (dir == "LR") {
                    graph.direction = GraphDirection::LeftToRight;
                } else if (dir == "RL") {
                    graph.direction = GraphDirection::RightToLeft;
                }
            }
            continue;
        }

        // Handle subgraph
        if (line.find("subgraph ") == 0) {
            size_t pos = line.find(' ');
            std::string rest = trim(line.substr(pos + 1));
            // subgraph id [label]
            size_t bracketPos = rest.find('[');
            std::string subId, subLabel;
            if (bracketPos != std::string::npos) {
                subId = trim(rest.substr(0, bracketPos));
                size_t endBracket = rest.find(']', bracketPos);
                if (endBracket != std::string::npos) {
                    subLabel = rest.substr(bracketPos + 1, endBracket - bracketPos - 1);
                }
            } else {
                subId = rest;
                subLabel = rest;
            }
            GraphCluster cluster;
            cluster.id = subId;
            cluster.label = subLabel;
            graph.addCluster(std::move(cluster));
            _currentSubgraph = subId;
            continue;
        }

        if (line == "end") {
            _currentSubgraph.clear();
            continue;
        }

        // Parse node and edge definitions
        if (inGraph) {
            if (auto res = parseLine(line, graph); !res) {
                // Non-fatal: just skip malformed lines
            }
        }
    }

    return Ok();
}

Result<void> MermaidParser::parseLine(const std::string& line, Graph& graph) {
    // Try to find an arrow in the line
    static const std::vector<std::string> arrows = {
        "-->", "-.->", "==>", "---", "-.-", "===",
        "-->>", "-.->", "==>>",
        "-->|", "-.->|", "==>|", "---|", "-.-|", "===|"
    };

    std::string edgeLabel;
    size_t arrowPos = std::string::npos;
    std::string arrowStr;

    for (const auto& arr : arrows) {
        size_t pos = line.find(arr);
        if (pos != std::string::npos && (arrowPos == std::string::npos || pos < arrowPos)) {
            arrowPos = pos;
            arrowStr = arr;
        }
    }

    if (arrowPos == std::string::npos) {
        // No arrow - might be a standalone node definition
        std::string id, label;
        NodeShape shape;
        if (parseNodeDef(trim(line), id, label, shape)) {
            if (!graph.getNode(id)) {
                GraphNode node;
                node.id = id;
                node.label = label;
                node.shape = shape;
                node.style = graph.defaultNodeStyle;
                if (!_currentSubgraph.empty()) {
                    node.clusterId = _currentSubgraph;
                    if (auto* cluster = &graph.clusters[_currentSubgraph]) {
                        cluster->nodeIds.push_back(id);
                    }
                }
                graph.addNode(std::move(node));
            }
        }
        return Ok();
    }

    // Split into source and target parts
    std::string sourcePart = trim(line.substr(0, arrowPos));
    std::string afterArrow = line.substr(arrowPos + arrowStr.length());

    // Check for edge label |label|
    if (arrowStr.back() == '|') {
        size_t endLabel = afterArrow.find('|');
        if (endLabel != std::string::npos) {
            edgeLabel = afterArrow.substr(0, endLabel);
            afterArrow = afterArrow.substr(endLabel + 1);
        }
    } else if (afterArrow.length() > 0 && afterArrow[0] == '|') {
        size_t endLabel = afterArrow.find('|', 1);
        if (endLabel != std::string::npos) {
            edgeLabel = afterArrow.substr(1, endLabel - 1);
            afterArrow = afterArrow.substr(endLabel + 1);
        }
    }

    std::string targetPart = trim(afterArrow);

    // Parse source node(s)
    std::string sourceId, sourceLabel;
    NodeShape sourceShape;
    if (!parseNodeDef(sourcePart, sourceId, sourceLabel, sourceShape)) {
        sourceId = sourcePart;
        sourceLabel = sourcePart;
        sourceShape = NodeShape::Rectangle;
    }

    // Ensure source node exists
    if (!graph.getNode(sourceId)) {
        GraphNode node;
        node.id = sourceId;
        node.label = sourceLabel;
        node.shape = sourceShape;
        node.style = graph.defaultNodeStyle;
        if (!_currentSubgraph.empty()) {
            node.clusterId = _currentSubgraph;
            graph.clusters[_currentSubgraph].nodeIds.push_back(sourceId);
        }
        graph.addNode(std::move(node));
    }

    // Parse target node(s) - might have chained edges like A --> B --> C
    std::string targetId, targetLabel;
    NodeShape targetShape;
    if (!parseNodeDef(targetPart, targetId, targetLabel, targetShape)) {
        targetId = targetPart;
        targetLabel = targetPart;
        targetShape = NodeShape::Rectangle;
    }

    // Check for chained arrows in target part
    size_t chainArrowPos = std::string::npos;
    std::string chainArrow;
    for (const auto& arr : arrows) {
        size_t pos = targetPart.find(arr);
        if (pos != std::string::npos && (chainArrowPos == std::string::npos || pos < chainArrowPos)) {
            chainArrowPos = pos;
            chainArrow = arr;
        }
    }

    if (chainArrowPos != std::string::npos) {
        // Handle chain: extract first target, then recursively parse rest
        std::string firstTarget = trim(targetPart.substr(0, chainArrowPos));
        if (!parseNodeDef(firstTarget, targetId, targetLabel, targetShape)) {
            targetId = firstTarget;
            targetLabel = firstTarget;
            targetShape = NodeShape::Rectangle;
        }
        // Recurse for the rest of the chain
        std::string chainRest = targetPart.substr(chainArrowPos);
        parseLine(targetId + " " + chainRest, graph);
    }

    // Ensure target node exists
    if (!graph.getNode(targetId)) {
        GraphNode node;
        node.id = targetId;
        node.label = targetLabel;
        node.shape = targetShape;
        node.style = graph.defaultNodeStyle;
        if (!_currentSubgraph.empty()) {
            node.clusterId = _currentSubgraph;
            graph.clusters[_currentSubgraph].nodeIds.push_back(targetId);
        }
        graph.addNode(std::move(node));
    }

    // Create edge
    GraphEdge edge;
    edge.sourceId = sourceId;
    edge.targetId = targetId;
    edge.label = edgeLabel;
    edge.style = graph.defaultEdgeStyle;
    parseArrow(arrowStr, edge.style);
    graph.addEdge(std::move(edge));

    return Ok();
}

bool MermaidParser::parseNodeDef(const std::string& token, std::string& id,
                                  std::string& label, NodeShape& shape) {
    if (token.empty()) return false;

    // Find the shape delimiters
    size_t start = std::string::npos;
    size_t end = std::string::npos;
    shape = NodeShape::Rectangle;

    // Check for different shapes - order matters (check longer patterns first)

    // Double brackets: ((text)) = circle
    size_t doubleOpen = token.find("((");
    if (doubleOpen != std::string::npos) {
        size_t doubleClose = token.find("))", doubleOpen);
        if (doubleClose != std::string::npos) {
            id = token.substr(0, doubleOpen);
            label = token.substr(doubleOpen + 2, doubleClose - doubleOpen - 2);
            shape = NodeShape::Circle;
            return !id.empty();
        }
    }

    // Double braces: {{text}} = hexagon
    size_t doubleBraceOpen = token.find("{{");
    if (doubleBraceOpen != std::string::npos) {
        size_t doubleBraceClose = token.find("}}", doubleBraceOpen);
        if (doubleBraceClose != std::string::npos) {
            id = token.substr(0, doubleBraceOpen);
            label = token.substr(doubleBraceOpen + 2, doubleBraceClose - doubleBraceOpen - 2);
            shape = NodeShape::Hexagon;
            return !id.empty();
        }
    }

    // Cylinder: [(text)] = database/cylinder
    size_t cylOpen = token.find("[(");
    if (cylOpen != std::string::npos) {
        size_t cylClose = token.find(")]", cylOpen);
        if (cylClose != std::string::npos) {
            id = token.substr(0, cylOpen);
            label = token.substr(cylOpen + 2, cylClose - cylOpen - 2);
            shape = NodeShape::Cylinder;
            return !id.empty();
        }
    }

    // Stadium: ([text]) = stadium/pill
    size_t stadOpen = token.find("([");
    if (stadOpen != std::string::npos) {
        size_t stadClose = token.find("])", stadOpen);
        if (stadClose != std::string::npos) {
            id = token.substr(0, stadOpen);
            label = token.substr(stadOpen + 2, stadClose - stadOpen - 2);
            shape = NodeShape::Stadium;
            return !id.empty();
        }
    }

    // Parallelogram right: [/text/]
    size_t paraROpen = token.find("[/");
    if (paraROpen != std::string::npos) {
        size_t paraRClose = token.find("/]", paraROpen);
        if (paraRClose != std::string::npos) {
            id = token.substr(0, paraROpen);
            label = token.substr(paraROpen + 2, paraRClose - paraROpen - 2);
            shape = NodeShape::Parallelogram;
            return !id.empty();
        }
    }

    // Parallelogram left: [\text\]
    size_t paraLOpen = token.find("[\\");
    if (paraLOpen != std::string::npos) {
        size_t paraLClose = token.find("\\]", paraLOpen);
        if (paraLClose != std::string::npos) {
            id = token.substr(0, paraLOpen);
            label = token.substr(paraLOpen + 2, paraLClose - paraLOpen - 2);
            shape = NodeShape::Parallelogram;
            return !id.empty();
        }
    }

    // Trapezoid: [/text\]
    size_t trapOpen = token.find("[/");
    if (trapOpen != std::string::npos) {
        size_t trapClose = token.find("\\]", trapOpen);
        if (trapClose != std::string::npos) {
            id = token.substr(0, trapOpen);
            label = token.substr(trapOpen + 2, trapClose - trapOpen - 2);
            shape = NodeShape::Trapezoid;
            return !id.empty();
        }
    }

    // Diamond: {text}
    size_t braceOpen = token.find('{');
    if (braceOpen != std::string::npos) {
        size_t braceClose = token.rfind('}');
        if (braceClose != std::string::npos && braceClose > braceOpen) {
            id = token.substr(0, braceOpen);
            label = token.substr(braceOpen + 1, braceClose - braceOpen - 1);
            shape = NodeShape::Diamond;
            return !id.empty();
        }
    }

    // Rounded rect: (text)
    size_t parenOpen = token.find('(');
    if (parenOpen != std::string::npos) {
        size_t parenClose = token.rfind(')');
        if (parenClose != std::string::npos && parenClose > parenOpen) {
            id = token.substr(0, parenOpen);
            label = token.substr(parenOpen + 1, parenClose - parenOpen - 1);
            shape = NodeShape::RoundedRect;
            return !id.empty();
        }
    }

    // Rectangle: [text]
    size_t bracketOpen = token.find('[');
    if (bracketOpen != std::string::npos) {
        size_t bracketClose = token.rfind(']');
        if (bracketClose != std::string::npos && bracketClose > bracketOpen) {
            id = token.substr(0, bracketOpen);
            label = token.substr(bracketOpen + 1, bracketClose - bracketOpen - 1);
            shape = NodeShape::Rectangle;
            return !id.empty();
        }
    }

    // No shape delimiters - just an ID
    id = token;
    label = token;
    shape = NodeShape::Rectangle;
    return true;
}

bool MermaidParser::parseArrow(const std::string& arrow, EdgeStyle& style) {
    // Remove trailing | if present (label syntax)
    std::string arr = arrow;
    if (!arr.empty() && arr.back() == '|') {
        arr = arr.substr(0, arr.length() - 1);
    }

    style.sourceArrow = ArrowStyle::None;
    style.targetArrow = ArrowStyle::None;
    style.lineStyle = LineStyle::Solid;

    // Check line style
    if (arr.find("-.") != std::string::npos || arr.find(".-") != std::string::npos) {
        style.lineStyle = LineStyle::Dashed;
    } else if (arr.find("==") != std::string::npos) {
        style.lineStyle = LineStyle::Thick;
        style.strokeWidth = 3.0f;
    }

    // Check arrow heads
    if (arr.find(">>") != std::string::npos) {
        style.targetArrow = ArrowStyle::Normal;
    } else if (arr.find(">") != std::string::npos) {
        style.targetArrow = ArrowStyle::Normal;
    }

    if (arr.find("<<") != std::string::npos) {
        style.sourceArrow = ArrowStyle::Normal;
    } else if (arr.length() > 0 && arr[0] == '<') {
        style.sourceArrow = ArrowStyle::Normal;
    }

    return true;
}

std::string MermaidParser::extractEdgeLabel(std::string& token) {
    size_t labelStart = token.find('|');
    if (labelStart == std::string::npos) return "";

    size_t labelEnd = token.find('|', labelStart + 1);
    if (labelEnd == std::string::npos) return "";

    std::string label = token.substr(labelStart + 1, labelEnd - labelStart - 1);
    token = token.substr(0, labelStart) + token.substr(labelEnd + 1);
    return label;
}

std::string MermaidParser::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

} // namespace yetty::diagram
