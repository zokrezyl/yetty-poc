#include "ydraw.h"
#include "../../ydraw/animation.h"
#include "../hdraw/hdraw.h"  // For SDFPrimitive, SDFType
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <cmath>
#include <cstring>

namespace yetty::card {

//=============================================================================
// Constructor
//=============================================================================

YDraw::YDraw(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
    : YDrawBase(ctx, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _payloadStr(payload)
{}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YDraw::init() {
    if (auto res = initBase(); !res) {
        return res;
    }

    parseArgs(_argsStr);

    setInitParsing(true);
    if (!_payloadStr.empty()) {
        if (auto res = parsePayload(_payloadStr); !res) {
            ywarn("YDraw::init: failed to parse payload: {}", error_msg(res));
        }
    }
    setInitParsing(false);

    ydebug("YDraw::init: prims={} glyphs={} payload_len={}",
           totalPendingPrimitives(), glyphCount(), _payloadStr.size());
    yinfo("YDraw::init: parsed {} primitives, {} glyphs",
          totalPendingPrimitives(), glyphCount());

    markDirty();

    // Start animation if properties were defined
    // (startAnimation snapshots base prims — must happen after allocateBuffers,
    //  which happens after init. So we defer to first render by calling play()
    //  and letting render() handle the snapshot.)
    // Actually, startAnimation needs _primitives which are set after allocateBuffers.
    // We'll start it after first render by checking in render if animation has props but no base.

    return Ok();
}

//=============================================================================
// Args parsing
//=============================================================================

void YDraw::parseArgs(const std::string& args) {
    yinfo("YDraw::parseArgs: args='{}'", args);

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--bg-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                    colorStr = colorStr.substr(2);
                }
                setBgColor(static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16)));
            }
        } else if (token == "--cell-size") {
            std::string sizeStr;
            if (iss >> sizeStr) {
                setGridCellSize(std::stof(sizeStr));
            }
        } else if (token == "--max-prims-per-cell") {
            std::string maxStr;
            if (iss >> maxStr) {
                setMaxPrimsPerCell(static_cast<uint32_t>(std::stoul(maxStr)));
            }
        } else if (token == "--show-bounds") {
            addFlags(FLAG_SHOW_BOUNDS);
        } else if (token == "--show-grid") {
            addFlags(FLAG_SHOW_GRID);
        } else if (token == "--show-eval-count") {
            addFlags(FLAG_SHOW_EVAL_COUNT);
        }
    }
}

//=============================================================================
// Payload detection
//=============================================================================

Result<void> YDraw::parsePayload(const std::string& payload) {
    yinfo("YDraw::parsePayload: payload length={}", payload.size());

    if (payload.empty()) {
        return Ok();
    }

    bool isYaml = false;
    if (payload.size() >= 4) {
        char first = payload[0];
        if (first == '#' || first == '-' || first == ' ' || first == '\n' ||
            first == '\t' || (first >= 'a' && first <= 'z') ||
            (first >= 'A' && first <= 'Z')) {
            isYaml = true;
        }
    }

    if (isYaml) {
        return parseYAML(payload);
    } else {
        return parseBinary(payload);
    }
}

//=============================================================================
// Binary parsing
//=============================================================================

Result<void> YDraw::parseBinary(const std::string& payload) {
    const size_t HEADER_SIZE = 16;
    const size_t PRIM_SIZE = sizeof(SDFPrimitive);

    if (payload.size() < HEADER_SIZE) {
        ywarn("YDraw::parseBinary: payload too small for header ({})", payload.size());
        return Ok();
    }

    const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());

    uint32_t version, primCount, bgColorVal, flagsVal;
    std::memcpy(&version, data + 0, 4);
    std::memcpy(&primCount, data + 4, 4);
    std::memcpy(&bgColorVal, data + 8, 4);
    std::memcpy(&flagsVal, data + 12, 4);

    yinfo("YDraw::parseBinary: version={} primCount={} bgColor={:#010x} flags={}",
          version, primCount, bgColorVal, flagsVal);

    if (version != 1 && version != 2) {
        ywarn("YDraw::parseBinary: unknown version {}", version);
        return Ok();
    }

    setBgColor(bgColorVal);
    addFlags(flagsVal);

    size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
    if (payload.size() < expectedSize) {
        ywarn("YDraw::parseBinary: payload too small ({} < {})",
              payload.size(), expectedSize);
        primCount = static_cast<uint32_t>((payload.size() - HEADER_SIZE) / PRIM_SIZE);
    }

    const uint8_t* primData = data + HEADER_SIZE;

    // During init parsing, addPrimitive routes to staging
    for (uint32_t i = 0; i < primCount; i++) {
        SDFPrimitive prim;
        std::memcpy(&prim, primData + i * PRIM_SIZE, PRIM_SIZE);
        addPrimitive(prim);
    }

    yinfo("YDraw::parseBinary: loaded {} primitives", primCount);
    return Ok();
}

//=============================================================================
// YAML parsing
//=============================================================================

/* static */ uint32_t YDraw::parseColor(const YAML::Node& node) {
    if (!node) return 0xFFFFFFFF;

    std::string str = node.as<std::string>();
    if (str.empty()) return 0xFFFFFFFF;

    if (str[0] == '#') {
        str = str.substr(1);
        if (str.size() == 3) {
            str = std::string{str[0], str[0], str[1], str[1], str[2], str[2]};
        }
        if (str.size() == 6) {
            str += "FF";
        }
        uint32_t r = std::stoul(str.substr(0, 2), nullptr, 16);
        uint32_t g = std::stoul(str.substr(2, 2), nullptr, 16);
        uint32_t b = std::stoul(str.substr(4, 2), nullptr, 16);
        uint32_t a = std::stoul(str.substr(6, 2), nullptr, 16);
        return (a << 24) | (b << 16) | (g << 8) | r;  // ABGR
    }
    return 0xFFFFFFFF;
}

Result<void> YDraw::parseYAML(const std::string& yaml) {
    try {
        YAML::Node root = YAML::Load(yaml);
        // IMPORTANT: check root type BEFORE any string key access on root,
        // because yaml-cpp converts sequences to maps when you access string keys.
        bool rootIsSequence = root.IsSequence();

        // Helper: parse primitive, then check for animate block
        auto parsePrimWithAnim = [this](const YAML::Node& item) -> Result<void> {
            uint32_t primBefore = totalPendingPrimitives();
            if (auto res = parseYAMLPrimitive(item); !res) {
                return res;
            }
            uint32_t primAfter = totalPendingPrimitives();

            // If a primitive was added, check for animate block
            if (primAfter > primBefore && item.IsMap()) {
                for (auto it = item.begin(); it != item.end(); ++it) {
                    auto shapeNode = it->second;
                    if (shapeNode.IsMap() && shapeNode["animate"]) {
                        parseAnimateBlock(shapeNode["animate"], primBefore);
                    }
                }
            }
            return Ok();
        };

        // Helper: parse root-level settings from a map node
        auto parseSettings = [this](const YAML::Node& node) {
            if (node["background"]) {
                setBgColor(parseColor(node["background"]));
            }
            if (node["flags"]) {
                auto flagsNode = node["flags"];
                if (flagsNode.IsSequence()) {
                    for (const auto& flag : flagsNode) {
                        std::string f = flag.as<std::string>();
                        if (f == "show_bounds") addFlags(FLAG_SHOW_BOUNDS);
                        else if (f == "show_grid") addFlags(FLAG_SHOW_GRID);
                        else if (f == "show_eval_count") addFlags(FLAG_SHOW_EVAL_COUNT);
                    }
                } else if (flagsNode.IsScalar()) {
                    std::string f = flagsNode.as<std::string>();
                    if (f == "show_bounds") addFlags(FLAG_SHOW_BOUNDS);
                    else if (f == "show_grid") addFlags(FLAG_SHOW_GRID);
                    else if (f == "show_eval_count") addFlags(FLAG_SHOW_EVAL_COUNT);
                }
            }
            if (node["animation"]) {
                auto animNode = node["animation"];
                auto* anim = animation();
                if (animNode["duration"]) anim->setDuration(animNode["duration"].as<float>());
                if (animNode["loop"]) anim->setLoop(animNode["loop"].as<bool>());
                if (animNode["speed"]) anim->setSpeed(animNode["speed"].as<float>());
            }
        };

        if (rootIsSequence) {
            for (const auto& doc : root) {
                if (doc.IsMap()) {
                    parseSettings(doc);
                    if (doc["body"] && doc["body"].IsSequence()) {
                        for (const auto& item : doc["body"]) {
                            if (auto res = parsePrimWithAnim(item); !res) {
                                return res;
                            }
                        }
                    }
                }
            }
        } else if (root.IsMap()) {
            parseSettings(root);
            if (root["body"] && root["body"].IsSequence()) {
                for (const auto& item : root["body"]) {
                    if (auto res = parsePrimWithAnim(item); !res) {
                        return res;
                    }
                }
            }
        }

        markDirty();
        yinfo("YDraw::parseYAML: loaded {} primitives, {} glyphs",
              primitiveCount(), glyphCount());

        return Ok();

    } catch (const YAML::Exception& e) {
        return Err<void>(std::string("YAML parse error: ") + e.what());
    }
}

Result<void> YDraw::parseYAMLPrimitive(const YAML::Node& item) {
    uint32_t layer = primitiveCount() + glyphCount();

    // Text
    if (item["text"]) {
        auto t = item["text"];
        float x = 0, y = 0, fontSize = 16;
        uint32_t color = 0xFFFFFFFF;
        std::string content;
        int fontId = 0;

        if (t["position"] && t["position"].IsSequence()) {
            x = t["position"][0].as<float>();
            y = t["position"][1].as<float>();
        }
        if (t["font-size"]) fontSize = t["font-size"].as<float>();
        if (t["fontSize"]) fontSize = t["fontSize"].as<float>();
        if (t["color"]) color = parseColor(t["color"]);
        if (t["content"]) content = t["content"].as<std::string>();
        if (t["font"]) {
            auto ttfPath = t["font"].as<std::string>();
            fontId = addFont(ttfPath);
            if (fontId < 0) {
                return Err<void>("failed to load font: " + ttfPath);
            }
        }

        addText(x, y, content, fontSize, color, layer, fontId);
        return Ok();
    }

    // Circle
    if (item["circle"]) {
        auto c = item["circle"];
        float cx = 0, cy = 0, r = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (c["position"] && c["position"].IsSequence()) {
            cx = c["position"][0].as<float>();
            cy = c["position"][1].as<float>();
        }
        if (c["radius"]) r = c["radius"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

        addCircle(cx, cy, r, fill, stroke, strokeWidth, layer);
        return Ok();
    }

    // Box
    if (item["box"]) {
        auto b = item["box"];
        float cx = 0, cy = 0, hw = 10, hh = 10, round = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (b["position"] && b["position"].IsSequence()) {
            cx = b["position"][0].as<float>();
            cy = b["position"][1].as<float>();
        }
        if (b["size"] && b["size"].IsSequence()) {
            hw = b["size"][0].as<float>() / 2;
            hh = b["size"][1].as<float>() / 2;
        }
        if (b["fill"]) fill = parseColor(b["fill"]);
        if (b["stroke"]) stroke = parseColor(b["stroke"]);
        if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
        if (b["round"]) round = b["round"].as<float>();

        addBox(cx, cy, hw, hh, fill, stroke, strokeWidth, round, layer);
        return Ok();
    }

    // Segment
    if (item["segment"]) {
        auto s = item["segment"];
        float x0 = 0, y0 = 0, x1 = 100, y1 = 100;
        uint32_t stroke = 0xFFFFFFFF;
        float strokeWidth = 1;

        if (s["from"] && s["from"].IsSequence()) {
            x0 = s["from"][0].as<float>();
            y0 = s["from"][1].as<float>();
        }
        if (s["to"] && s["to"].IsSequence()) {
            x1 = s["to"][0].as<float>();
            y1 = s["to"][1].as<float>();
        }
        if (s["stroke"]) stroke = parseColor(s["stroke"]);
        if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();

        addSegment(x0, y0, x1, y1, stroke, strokeWidth, layer);
        return Ok();
    }

    // Triangle
    if (item["triangle"]) {
        auto t = item["triangle"];
        float x0 = 0, y0 = 0, x1 = 50, y1 = 100, x2 = 100, y2 = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (t["p0"] && t["p0"].IsSequence()) {
            x0 = t["p0"][0].as<float>();
            y0 = t["p0"][1].as<float>();
        }
        if (t["p1"] && t["p1"].IsSequence()) {
            x1 = t["p1"][0].as<float>();
            y1 = t["p1"][1].as<float>();
        }
        if (t["p2"] && t["p2"].IsSequence()) {
            x2 = t["p2"][0].as<float>();
            y2 = t["p2"][1].as<float>();
        }
        if (t["fill"]) fill = parseColor(t["fill"]);
        if (t["stroke"]) stroke = parseColor(t["stroke"]);
        if (t["stroke-width"]) strokeWidth = t["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Triangle);
        prim.layer = layer;
        prim.params[0] = x0; prim.params[1] = y0;
        prim.params[2] = x1; prim.params[3] = y1;
        prim.params[4] = x2; prim.params[5] = y2;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Bezier
    if (item["bezier"]) {
        auto b = item["bezier"];
        float x0 = 0, y0 = 0, x1 = 50, y1 = 50, x2 = 100, y2 = 0;
        uint32_t stroke = 0xFFFFFFFF;
        float strokeWidth = 1;

        if (b["p0"] && b["p0"].IsSequence()) {
            x0 = b["p0"][0].as<float>();
            y0 = b["p0"][1].as<float>();
        }
        if (b["p1"] && b["p1"].IsSequence()) {
            x1 = b["p1"][0].as<float>();
            y1 = b["p1"][1].as<float>();
        }
        if (b["p2"] && b["p2"].IsSequence()) {
            x2 = b["p2"][0].as<float>();
            y2 = b["p2"][1].as<float>();
        }
        if (b["stroke"]) stroke = parseColor(b["stroke"]);
        if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();

        addBezier2(x0, y0, x1, y1, x2, y2, stroke, strokeWidth, layer);
        return Ok();
    }

    // Ellipse
    if (item["ellipse"]) {
        auto e = item["ellipse"];
        float cx = 0, cy = 0, rx = 20, ry = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radii"] && e["radii"].IsSequence()) {
            rx = e["radii"][0].as<float>();
            ry = e["radii"][1].as<float>();
        }
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Ellipse);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = rx; prim.params[3] = ry;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Arc
    if (item["arc"]) {
        auto a = item["arc"];
        float cx = 0, cy = 0, ra = 20, rb = 2;
        float angle = 90.0f;
        uint32_t fill = 0, stroke = 0xFFFFFFFF;
        float strokeWidth = 0;

        if (a["position"] && a["position"].IsSequence()) {
            cx = a["position"][0].as<float>();
            cy = a["position"][1].as<float>();
        }
        if (a["angle"]) angle = a["angle"].as<float>();
        if (a["radius"]) ra = a["radius"].as<float>();
        if (a["thickness"]) rb = a["thickness"].as<float>();
        if (a["fill"]) fill = parseColor(a["fill"]);
        if (a["stroke"]) stroke = parseColor(a["stroke"]);
        if (a["stroke-width"]) strokeWidth = a["stroke-width"].as<float>();

        float rad = angle * 3.14159265f / 180.0f;
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Arc);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
        prim.params[4] = ra; prim.params[5] = rb;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Pentagon
    if (item["pentagon"]) {
        auto p = item["pentagon"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (p["position"] && p["position"].IsSequence()) {
            cx = p["position"][0].as<float>();
            cy = p["position"][1].as<float>();
        }
        if (p["radius"]) r = p["radius"].as<float>();
        if (p["fill"]) fill = parseColor(p["fill"]);
        if (p["stroke"]) stroke = parseColor(p["stroke"]);
        if (p["stroke-width"]) strokeWidth = p["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Pentagon);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Hexagon
    if (item["hexagon"]) {
        auto h = item["hexagon"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (h["position"] && h["position"].IsSequence()) {
            cx = h["position"][0].as<float>();
            cy = h["position"][1].as<float>();
        }
        if (h["radius"]) r = h["radius"].as<float>();
        if (h["fill"]) fill = parseColor(h["fill"]);
        if (h["stroke"]) stroke = parseColor(h["stroke"]);
        if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Hexagon);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Star
    if (item["star"]) {
        auto s = item["star"];
        float cx = 0, cy = 0, r = 20;
        float n = 5, m = 2.5f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (s["position"] && s["position"].IsSequence()) {
            cx = s["position"][0].as<float>();
            cy = s["position"][1].as<float>();
        }
        if (s["radius"]) r = s["radius"].as<float>();
        if (s["points"]) n = s["points"].as<float>();
        if (s["inner"]) m = s["inner"].as<float>();
        if (s["fill"]) fill = parseColor(s["fill"]);
        if (s["stroke"]) stroke = parseColor(s["stroke"]);
        if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Star);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r; prim.params[3] = n; prim.params[4] = m;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Pie
    if (item["pie"]) {
        auto p = item["pie"];
        float cx = 0, cy = 0, r = 20;
        float angle = 45.0f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (p["position"] && p["position"].IsSequence()) {
            cx = p["position"][0].as<float>();
            cy = p["position"][1].as<float>();
        }
        if (p["angle"]) angle = p["angle"].as<float>();
        if (p["radius"]) r = p["radius"].as<float>();
        if (p["fill"]) fill = parseColor(p["fill"]);
        if (p["stroke"]) stroke = parseColor(p["stroke"]);
        if (p["stroke-width"]) strokeWidth = p["stroke-width"].as<float>();

        float rad = angle * 3.14159265f / 180.0f;
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Pie);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
        prim.params[4] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Ring
    if (item["ring"]) {
        auto rg = item["ring"];
        float cx = 0, cy = 0, r = 20, th = 4;
        float angle = 0.0f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (rg["position"] && rg["position"].IsSequence()) {
            cx = rg["position"][0].as<float>();
            cy = rg["position"][1].as<float>();
        }
        if (rg["angle"]) angle = rg["angle"].as<float>();
        if (rg["radius"]) r = rg["radius"].as<float>();
        if (rg["thickness"]) th = rg["thickness"].as<float>();
        if (rg["fill"]) fill = parseColor(rg["fill"]);
        if (rg["stroke"]) stroke = parseColor(rg["stroke"]);
        if (rg["stroke-width"]) strokeWidth = rg["stroke-width"].as<float>();

        float rad = angle * 3.14159265f / 180.0f;
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Ring);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
        prim.params[4] = r; prim.params[5] = th;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Heart
    if (item["heart"]) {
        auto h = item["heart"];
        float cx = 0, cy = 0, scale = 20;
        uint32_t fill = 0xFF0000FF, stroke = 0;
        float strokeWidth = 0;

        if (h["position"] && h["position"].IsSequence()) {
            cx = h["position"][0].as<float>();
            cy = h["position"][1].as<float>();
        }
        if (h["scale"]) scale = h["scale"].as<float>();
        if (h["fill"]) fill = parseColor(h["fill"]);
        if (h["stroke"]) stroke = parseColor(h["stroke"]);
        if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Heart);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = scale;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Cross
    if (item["cross"]) {
        auto c = item["cross"];
        float cx = 0, cy = 0, w = 20, h = 5, r = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (c["position"] && c["position"].IsSequence()) {
            cx = c["position"][0].as<float>();
            cy = c["position"][1].as<float>();
        }
        if (c["size"] && c["size"].IsSequence()) {
            w = c["size"][0].as<float>();
            h = c["size"][1].as<float>();
        }
        if (c["round"]) r = c["round"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Cross);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = w; prim.params[3] = h; prim.params[4] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // RoundedX
    if (item["rounded_x"]) {
        auto x = item["rounded_x"];
        float cx = 0, cy = 0, w = 20, r = 3;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (x["position"] && x["position"].IsSequence()) {
            cx = x["position"][0].as<float>();
            cy = x["position"][1].as<float>();
        }
        if (x["width"]) w = x["width"].as<float>();
        if (x["round"]) r = x["round"].as<float>();
        if (x["fill"]) fill = parseColor(x["fill"]);
        if (x["stroke"]) stroke = parseColor(x["stroke"]);
        if (x["stroke-width"]) strokeWidth = x["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::RoundedX);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = w; prim.params[3] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Capsule (2D or 3D - detect by checking for 'height' key or 3D position)
    if (item["capsule"]) {
        auto c = item["capsule"];

        // 3D capsule: has 'height' key or 3-element position
        bool is3D = c["height"].IsDefined();
        if (!is3D && c["position"] && c["position"].IsSequence() && c["position"].size() > 2) {
            is3D = true;
        }

        if (is3D) {
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::VerticalCapsule3D);
            prim.layer = layer;
            if (c["position"] && c["position"].IsSequence()) {
                prim.params[0] = c["position"][0].as<float>();
                prim.params[1] = c["position"][1].as<float>();
                prim.params[2] = c["position"].size() > 2 ? c["position"][2].as<float>() : 0.0f;
            }
            if (c["height"]) prim.params[3] = c["height"].as<float>();
            else prim.params[3] = 0.3f;
            if (c["radius"]) prim.params[4] = c["radius"].as<float>();
            else prim.params[4] = 0.1f;
            if (c["fill"]) prim.fillColor = parseColor(c["fill"]);
            if (c["stroke"]) prim.strokeColor = parseColor(c["stroke"]);
            if (c["stroke-width"]) prim.strokeWidth = c["stroke-width"].as<float>();
            addPrimitive(prim);
            return Ok();
        }

        // 2D capsule
        float x0 = 0, y0 = 0, x1 = 100, y1 = 0, r = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (c["from"] && c["from"].IsSequence()) {
            x0 = c["from"][0].as<float>();
            y0 = c["from"][1].as<float>();
        }
        if (c["to"] && c["to"].IsSequence()) {
            x1 = c["to"][0].as<float>();
            y1 = c["to"][1].as<float>();
        }
        if (c["radius"]) r = c["radius"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Capsule);
        prim.layer = layer;
        prim.params[0] = x0; prim.params[1] = y0;
        prim.params[2] = x1; prim.params[3] = y1;
        prim.params[4] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Rhombus
    if (item["rhombus"]) {
        auto r = item["rhombus"];
        float cx = 0, cy = 0, w = 20, h = 30;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (r["position"] && r["position"].IsSequence()) {
            cx = r["position"][0].as<float>();
            cy = r["position"][1].as<float>();
        }
        if (r["size"] && r["size"].IsSequence()) {
            w = r["size"][0].as<float>();
            h = r["size"][1].as<float>();
        }
        if (r["fill"]) fill = parseColor(r["fill"]);
        if (r["stroke"]) stroke = parseColor(r["stroke"]);
        if (r["stroke-width"]) strokeWidth = r["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Rhombus);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = w; prim.params[3] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Moon
    if (item["moon"]) {
        auto m = item["moon"];
        float cx = 0, cy = 0, d = 10, ra = 20, rb = 15;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (m["position"] && m["position"].IsSequence()) {
            cx = m["position"][0].as<float>();
            cy = m["position"][1].as<float>();
        }
        if (m["distance"]) d = m["distance"].as<float>();
        if (m["radius_outer"]) ra = m["radius_outer"].as<float>();
        if (m["radius_inner"]) rb = m["radius_inner"].as<float>();
        if (m["fill"]) fill = parseColor(m["fill"]);
        if (m["stroke"]) stroke = parseColor(m["stroke"]);
        if (m["stroke-width"]) strokeWidth = m["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Moon);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = d; prim.params[3] = ra; prim.params[4] = rb;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Egg
    if (item["egg"]) {
        auto e = item["egg"];
        float cx = 0, cy = 0, ra = 20, rb = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;

        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius_bottom"]) ra = e["radius_bottom"].as<float>();
        if (e["radius_top"]) rb = e["radius_top"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Egg);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = ra; prim.params[3] = rb;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Chamfer Box
    if (item["chamfer_box"]) {
        auto e = item["chamfer_box"];
        float cx = 0, cy = 0, hx = 30, hy = 20, chamfer = 5;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["half_size"] && e["half_size"].IsSequence()) {
            hx = e["half_size"][0].as<float>();
            hy = e["half_size"][1].as<float>();
        }
        if (e["chamfer"]) chamfer = e["chamfer"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::ChamferBox);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = hx; prim.params[3] = hy;
        prim.params[4] = chamfer;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Oriented Box
    if (item["oriented_box"]) {
        auto e = item["oriented_box"];
        float ax = 0, ay = 0, bx = 30, by = 20, thickness = 5;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["a"] && e["a"].IsSequence()) {
            ax = e["a"][0].as<float>();
            ay = e["a"][1].as<float>();
        }
        if (e["b"] && e["b"].IsSequence()) {
            bx = e["b"][0].as<float>();
            by = e["b"][1].as<float>();
        }
        if (e["thickness"]) thickness = e["thickness"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::OrientedBox);
        prim.layer = layer;
        prim.params[0] = ax; prim.params[1] = ay;
        prim.params[2] = bx; prim.params[3] = by;
        prim.params[4] = thickness;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Trapezoid
    if (item["trapezoid"]) {
        auto e = item["trapezoid"];
        float cx = 0, cy = 0, r1 = 30, r2 = 15, he = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["r1"]) r1 = e["r1"].as<float>();
        if (e["r2"]) r2 = e["r2"].as<float>();
        if (e["height"]) he = e["height"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Trapezoid);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r1; prim.params[3] = r2;
        prim.params[4] = he;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Parallelogram
    if (item["parallelogram"]) {
        auto e = item["parallelogram"];
        float cx = 0, cy = 0, wi = 30, he = 20, sk = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["width"]) wi = e["width"].as<float>();
        if (e["height"]) he = e["height"].as<float>();
        if (e["skew"]) sk = e["skew"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Parallelogram);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = wi; prim.params[3] = he;
        prim.params[4] = sk;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Equilateral Triangle
    if (item["equilateral_triangle"]) {
        auto e = item["equilateral_triangle"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::EquilateralTriangle);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Isosceles Triangle
    if (item["isosceles_triangle"]) {
        auto e = item["isosceles_triangle"];
        float cx = 0, cy = 0, hw = 15, he = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["half_width"]) hw = e["half_width"].as<float>();
        if (e["height"]) he = e["height"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::IsoscelesTriangle);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = hw; prim.params[3] = he;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Uneven Capsule
    if (item["uneven_capsule"]) {
        auto e = item["uneven_capsule"];
        float cx = 0, cy = 0, r1 = 10, r2 = 5, h = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["r1"]) r1 = e["r1"].as<float>();
        if (e["r2"]) r2 = e["r2"].as<float>();
        if (e["height"]) h = e["height"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::UnevenCapsule);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r1; prim.params[3] = r2;
        prim.params[4] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Octogon
    if (item["octogon"]) {
        auto e = item["octogon"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Octogon);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Hexagram
    if (item["hexagram"]) {
        auto e = item["hexagram"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Hexagram);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Pentagram
    if (item["pentagram"]) {
        auto e = item["pentagram"];
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Pentagram);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Cut Disk
    if (item["cut_disk"]) {
        auto e = item["cut_disk"];
        float cx = 0, cy = 0, r = 20, h = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["h"]) h = e["h"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::CutDisk);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = r; prim.params[3] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Horseshoe
    if (item["horseshoe"]) {
        auto e = item["horseshoe"];
        float cx = 0, cy = 0, angle = 1.0f, r = 20, w1 = 5, w2 = 5;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["angle"]) angle = e["angle"].as<float>();
        if (e["radius"]) r = e["radius"].as<float>();
        if (e["width"] && e["width"].IsSequence()) {
            w1 = e["width"][0].as<float>();
            w2 = e["width"][1].as<float>();
        }
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Horseshoe);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = std::sin(angle); prim.params[3] = std::cos(angle);
        prim.params[4] = r;
        prim.params[5] = w1; prim.params[6] = w2;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Vesica
    if (item["vesica"]) {
        auto e = item["vesica"];
        float cx = 0, cy = 0, w = 30, h = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["width"]) w = e["width"].as<float>();
        if (e["height"]) h = e["height"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Vesica);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = w; prim.params[3] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Oriented Vesica
    if (item["oriented_vesica"]) {
        auto e = item["oriented_vesica"];
        float ax = 0, ay = 0, bx = 30, by = 20, w = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["a"] && e["a"].IsSequence()) {
            ax = e["a"][0].as<float>();
            ay = e["a"][1].as<float>();
        }
        if (e["b"] && e["b"].IsSequence()) {
            bx = e["b"][0].as<float>();
            by = e["b"][1].as<float>();
        }
        if (e["width"]) w = e["width"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::OrientedVesica);
        prim.layer = layer;
        prim.params[0] = ax; prim.params[1] = ay;
        prim.params[2] = bx; prim.params[3] = by;
        prim.params[4] = w;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Rounded Cross
    if (item["rounded_cross"]) {
        auto e = item["rounded_cross"];
        float cx = 0, cy = 0, h = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["h"]) h = e["h"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::RoundedCross);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Parabola
    if (item["parabola"]) {
        auto e = item["parabola"];
        float cx = 0, cy = 0, k = 1.0f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["k"]) k = e["k"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Parabola);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = k;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Blobby Cross
    if (item["blobby_cross"]) {
        auto e = item["blobby_cross"];
        float cx = 0, cy = 0, he = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["he"]) he = e["he"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::BlobbyCross);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = he;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Tunnel
    if (item["tunnel"]) {
        auto e = item["tunnel"];
        float cx = 0, cy = 0, w = 30, h = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["width"]) w = e["width"].as<float>();
        if (e["height"]) h = e["height"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Tunnel);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = w; prim.params[3] = h;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Stairs
    if (item["stairs"]) {
        auto e = item["stairs"];
        float cx = 0, cy = 0, sw = 10, sh = 5, n = 5;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["step_width"]) sw = e["step_width"].as<float>();
        if (e["step_height"]) sh = e["step_height"].as<float>();
        if (e["count"]) n = e["count"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Stairs);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = sw; prim.params[3] = sh;
        prim.params[4] = n;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Quadratic Circle
    if (item["quadratic_circle"]) {
        auto e = item["quadratic_circle"];
        float cx = 0, cy = 0, scale = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["scale"]) scale = e["scale"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::QuadraticCircle);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = scale;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Hyperbola
    if (item["hyperbola"]) {
        auto e = item["hyperbola"];
        float cx = 0, cy = 0, k = 1.0f, he = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["k"]) k = e["k"].as<float>();
        if (e["he"]) he = e["he"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Hyperbola);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = k; prim.params[3] = he;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Cool S
    if (item["cool_s"]) {
        auto e = item["cool_s"];
        float cx = 0, cy = 0, scale = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["scale"]) scale = e["scale"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::CoolS);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = scale;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // Circle Wave
    if (item["circle_wave"]) {
        auto e = item["circle_wave"];
        float cx = 0, cy = 0, tb = 1.0f, ra = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>();
            cy = e["position"][1].as<float>();
        }
        if (e["tb"]) tb = e["tb"].as<float>();
        if (e["ra"]) ra = e["ra"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::CircleWave);
        prim.layer = layer;
        prim.params[0] = cx; prim.params[1] = cy;
        prim.params[2] = tb; prim.params[3] = ra;
        prim.fillColor = fill;
        prim.strokeColor = stroke;
        prim.strokeWidth = strokeWidth;
        addPrimitive(prim);
        return Ok();
    }

    // =========================================================================
    // 3D Primitives (raymarched)
    // =========================================================================

    // Helper to parse 3D position
    auto parse3DPosition = [](const YAML::Node& node, float* params) {
        if (node["position"] && node["position"].IsSequence()) {
            params[0] = node["position"][0].as<float>();
            params[1] = node["position"][1].as<float>();
            params[2] = node["position"].size() > 2 ? node["position"][2].as<float>() : 0.0f;
        }
    };

    // Sphere
    if (item["sphere"]) {
        auto s = item["sphere"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Sphere3D);
        prim.layer = layer;
        parse3DPosition(s, prim.params);
        if (s["radius"]) prim.params[3] = s["radius"].as<float>();
        else prim.params[3] = 0.25f;
        if (s["fill"]) prim.fillColor = parseColor(s["fill"]);
        if (s["stroke"]) prim.strokeColor = parseColor(s["stroke"]);
        if (s["stroke-width"]) prim.strokeWidth = s["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Box3D
    if (item["box3d"]) {
        auto b = item["box3d"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Box3D);
        prim.layer = layer;
        parse3DPosition(b, prim.params);
        if (b["size"] && b["size"].IsSequence()) {
            prim.params[3] = b["size"][0].as<float>();
            prim.params[4] = b["size"][1].as<float>();
            prim.params[5] = b["size"].size() > 2 ? b["size"][2].as<float>() : b["size"][0].as<float>();
        } else {
            prim.params[3] = prim.params[4] = prim.params[5] = 0.2f;
        }
        if (b["fill"]) prim.fillColor = parseColor(b["fill"]);
        if (b["stroke"]) prim.strokeColor = parseColor(b["stroke"]);
        if (b["stroke-width"]) prim.strokeWidth = b["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Torus
    if (item["torus"]) {
        auto t = item["torus"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Torus3D);
        prim.layer = layer;
        parse3DPosition(t, prim.params);
        if (t["major-radius"]) prim.params[3] = t["major-radius"].as<float>();
        else prim.params[3] = 0.2f;
        if (t["minor-radius"]) prim.params[4] = t["minor-radius"].as<float>();
        else prim.params[4] = 0.08f;
        if (t["fill"]) prim.fillColor = parseColor(t["fill"]);
        if (t["stroke"]) prim.strokeColor = parseColor(t["stroke"]);
        if (t["stroke-width"]) prim.strokeWidth = t["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Cylinder
    if (item["cylinder"]) {
        auto c = item["cylinder"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Cylinder3D);
        prim.layer = layer;
        parse3DPosition(c, prim.params);
        if (c["radius"]) prim.params[3] = c["radius"].as<float>();
        else prim.params[3] = 0.15f;
        if (c["height"]) prim.params[4] = c["height"].as<float>();
        else prim.params[4] = 0.3f;
        if (c["fill"]) prim.fillColor = parseColor(c["fill"]);
        if (c["stroke"]) prim.strokeColor = parseColor(c["stroke"]);
        if (c["stroke-width"]) prim.strokeWidth = c["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Capsule (3D vertical capsule)
    if (item["capsule3d"]) {
        auto c = item["capsule3d"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::VerticalCapsule3D);
        prim.layer = layer;
        parse3DPosition(c, prim.params);
        if (c["height"]) prim.params[3] = c["height"].as<float>();
        else prim.params[3] = 0.3f;
        if (c["radius"]) prim.params[4] = c["radius"].as<float>();
        else prim.params[4] = 0.1f;
        if (c["fill"]) prim.fillColor = parseColor(c["fill"]);
        if (c["stroke"]) prim.strokeColor = parseColor(c["stroke"]);
        if (c["stroke-width"]) prim.strokeWidth = c["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Cone (capped cone)
    if (item["cone"]) {
        auto c = item["cone"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::CappedCone3D);
        prim.layer = layer;
        parse3DPosition(c, prim.params);
        if (c["height"]) prim.params[3] = c["height"].as<float>();
        else prim.params[3] = 0.35f;
        if (c["radius1"]) prim.params[4] = c["radius1"].as<float>();
        else prim.params[4] = 0.2f;
        if (c["radius2"]) prim.params[5] = c["radius2"].as<float>();
        else prim.params[5] = 0.05f;
        if (c["fill"]) prim.fillColor = parseColor(c["fill"]);
        if (c["stroke"]) prim.strokeColor = parseColor(c["stroke"]);
        if (c["stroke-width"]) prim.strokeWidth = c["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Octahedron
    if (item["octahedron"]) {
        auto o = item["octahedron"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Octahedron3D);
        prim.layer = layer;
        parse3DPosition(o, prim.params);
        if (o["size"]) prim.params[3] = o["size"].as<float>();
        else prim.params[3] = 0.3f;
        if (o["fill"]) prim.fillColor = parseColor(o["fill"]);
        if (o["stroke"]) prim.strokeColor = parseColor(o["stroke"]);
        if (o["stroke-width"]) prim.strokeWidth = o["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Pyramid
    if (item["pyramid"]) {
        auto p = item["pyramid"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Pyramid3D);
        prim.layer = layer;
        parse3DPosition(p, prim.params);
        if (p["height"]) prim.params[3] = p["height"].as<float>();
        else prim.params[3] = 0.5f;
        if (p["fill"]) prim.fillColor = parseColor(p["fill"]);
        if (p["stroke"]) prim.strokeColor = parseColor(p["stroke"]);
        if (p["stroke-width"]) prim.strokeWidth = p["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    // Ellipsoid
    if (item["ellipsoid"]) {
        auto e = item["ellipsoid"];
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Ellipsoid3D);
        prim.layer = layer;
        parse3DPosition(e, prim.params);
        if (e["radii"] && e["radii"].IsSequence()) {
            prim.params[3] = e["radii"][0].as<float>();
            prim.params[4] = e["radii"][1].as<float>();
            prim.params[5] = e["radii"].size() > 2 ? e["radii"][2].as<float>() : e["radii"][0].as<float>();
        } else {
            prim.params[3] = 0.3f; prim.params[4] = 0.2f; prim.params[5] = 0.15f;
        }
        if (e["fill"]) prim.fillColor = parseColor(e["fill"]);
        if (e["stroke"]) prim.strokeColor = parseColor(e["stroke"]);
        if (e["stroke-width"]) prim.strokeWidth = e["stroke-width"].as<float>();
        addPrimitive(prim);
        return Ok();
    }

    return Ok();
}

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YDraw::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YDraw::create: pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!ctx.cardManager) {
        yerror("YDraw::create: null CardBufferManager!");
        return Err<CardPtr>("YDraw::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDraw>(
        ctx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("YDraw::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YDraw::create: init failed");
    }

    yinfo("YDraw::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<YDraw::Ptr> YDraw::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create YDraw", result);
    }
    auto ydraw = std::dynamic_pointer_cast<YDraw>(*result);
    if (!ydraw) {
        return Err<Ptr>("Created card is not an YDraw");
    }
    return Ok(ydraw);
}

//=============================================================================
// Animation parsing
//=============================================================================

void YDraw::parseAnimateBlock(const YAML::Node& animNode, uint32_t primIndex) {
    if (!animNode.IsSequence()) return;

    auto* anim = animation();

    for (const auto& propNode : animNode) {
        if (!propNode["property"] || !propNode["keyframes"]) continue;

        std::string propStr = propNode["property"].as<std::string>();

        animation::PropertyType propType;
        if      (propStr == "position")       propType = animation::PropertyType::Position;
        else if (propStr == "scale")          propType = animation::PropertyType::Scale;
        else if (propStr == "fill_opacity")   propType = animation::PropertyType::FillOpacity;
        else if (propStr == "fill_color")     propType = animation::PropertyType::FillColor;
        else if (propStr == "stroke_opacity") propType = animation::PropertyType::StrokeOpacity;
        else if (propStr == "stroke_width")   propType = animation::PropertyType::StrokeWidth;
        else if (propStr == "radius")         propType = animation::PropertyType::Radius;
        else {
            ywarn("YDraw::parseAnimateBlock: unknown property '{}'", propStr);
            continue;
        }

        animation::AnimatedProperty prop;
        prop.type = propType;
        prop.primitiveIndex = primIndex;

        for (const auto& kfNode : propNode["keyframes"]) {
            animation::Keyframe kf = {};

            // Support both {t: 0.0, v: [x,y]} and {time: 0.0, value: [x,y]}
            if (kfNode["t"]) kf.time = kfNode["t"].as<float>();
            else if (kfNode["time"]) kf.time = kfNode["time"].as<float>();

            YAML::Node valNode;
            if (kfNode["v"]) valNode = kfNode["v"];
            else if (kfNode["value"]) valNode = kfNode["value"];

            if (valNode.IsSequence()) {
                kf.componentCount = static_cast<uint8_t>(
                    std::min(static_cast<size_t>(4), valNode.size()));
                for (uint8_t i = 0; i < kf.componentCount; i++) {
                    kf.value[i] = valNode[i].as<float>();
                }
            } else if (valNode.IsDefined()) {
                kf.componentCount = 1;
                kf.value[0] = valNode.as<float>();
            }

            prop.keyframes.push_back(kf);
        }

        anim->addProperty(std::move(prop));
    }
}

} // namespace yetty::card
