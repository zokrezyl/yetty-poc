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

        if (root["background"]) {
            setBgColor(parseColor(root["background"]));
        }

        if (root["flags"]) {
            auto flagsNode = root["flags"];
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

        // Parse animation settings
        if (root["animation"]) {
            auto animNode = root["animation"];
            auto* anim = animation();
            if (animNode["duration"]) anim->setDuration(animNode["duration"].as<float>());
            if (animNode["loop"]) anim->setLoop(animNode["loop"].as<bool>());
            if (animNode["speed"]) anim->setSpeed(animNode["speed"].as<float>());
        }

        // Helper: parse primitive, then check for animate block
        auto parsePrimWithAnim = [this](const YAML::Node& item) {
            uint32_t primBefore = totalPendingPrimitives();
            parseYAMLPrimitive(item);
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
        };

        if (root["body"] && root["body"].IsSequence()) {
            for (const auto& item : root["body"]) {
                parsePrimWithAnim(item);
            }
        }

        if (root.IsSequence()) {
            for (const auto& doc : root) {
                if (doc["body"] && doc["body"].IsSequence()) {
                    for (const auto& item : doc["body"]) {
                        parsePrimWithAnim(item);
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

void YDraw::parseYAMLPrimitive(const YAML::Node& item) {
    uint32_t layer = primitiveCount() + glyphCount();

    // Text
    if (item["text"]) {
        auto t = item["text"];
        float x = 0, y = 0, fontSize = 16;
        uint32_t color = 0xFFFFFFFF;
        std::string content;

        if (t["position"] && t["position"].IsSequence()) {
            x = t["position"][0].as<float>();
            y = t["position"][1].as<float>();
        }
        if (t["font-size"]) fontSize = t["font-size"].as<float>();
        if (t["fontSize"]) fontSize = t["fontSize"].as<float>();
        if (t["color"]) color = parseColor(t["color"]);
        if (t["content"]) content = t["content"].as<std::string>();

        addText(x, y, content, fontSize, color, layer);
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
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
        return;
    }

    // Capsule
    if (item["capsule"]) {
        auto c = item["capsule"];
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
        return;
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
        return;
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
        return;
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
        return;
    }
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
