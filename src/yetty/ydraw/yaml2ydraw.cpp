#include "yaml2ydraw.h"
#include "ydraw-buffer.h"
#include <yaml-cpp/yaml.h>
#include <cmath>
#include <fstream>
#include <filesystem>

namespace yetty {

//=============================================================================
// Helpers
//=============================================================================

static uint32_t parseColor(const YAML::Node& node) {
    if (!node) return 0xFFFFFFFF;
    std::string str = node.as<std::string>();
    if (str.empty()) return 0xFFFFFFFF;
    if (str[0] == '#') {
        str = str.substr(1);
        if (str.size() == 3) str = std::string{str[0], str[0], str[1], str[1], str[2], str[2]};
        if (str.size() == 6) str += "FF";
        uint32_t r = std::stoul(str.substr(0, 2), nullptr, 16);
        uint32_t g = std::stoul(str.substr(2, 2), nullptr, 16);
        uint32_t b = std::stoul(str.substr(4, 2), nullptr, 16);
        uint32_t a = std::stoul(str.substr(6, 2), nullptr, 16);
        return (a << 24) | (b << 16) | (g << 8) | r;
    }
    return 0xFFFFFFFF;
}

static animation::Animation* getAnim(animation::Animation::Ptr& anim,
                                      YDrawBuffer::Ptr buffer) {
    if (!anim) {
        auto res = animation::Animation::create(buffer);
        if (res) anim = *res;
    }
    return anim.get();
}

//=============================================================================
// parseAnimateBlock
//=============================================================================

static void parseAnimateBlock(animation::Animation::Ptr& anim,
                              YDrawBuffer::Ptr buffer,
                              const YAML::Node& animNode,
                              uint32_t primIndex) {
    if (!animNode.IsSequence()) return;
    auto* a = getAnim(anim, buffer);
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
        else continue;

        animation::AnimatedProperty prop;
        prop.type = propType;
        prop.primitiveId = YDrawBuffer::AUTO_ID_BASE + primIndex;
        for (const auto& kfNode : propNode["keyframes"]) {
            animation::Keyframe kf = {};
            if (kfNode["t"]) kf.time = kfNode["t"].as<float>();
            else if (kfNode["time"]) kf.time = kfNode["time"].as<float>();
            YAML::Node valNode;
            if (kfNode["v"]) valNode = kfNode["v"];
            else if (kfNode["value"]) valNode = kfNode["value"];
            if (valNode.IsSequence()) {
                kf.componentCount = static_cast<uint8_t>(std::min(static_cast<size_t>(4), valNode.size()));
                for (uint8_t i = 0; i < kf.componentCount; i++) kf.value[i] = valNode[i].as<float>();
            } else if (valNode.IsDefined()) {
                kf.componentCount = 1;
                kf.value[0] = valNode.as<float>();
            }
            prop.keyframes.push_back(kf);
        }
        a->addProperty(std::move(prop));
    }
}

//=============================================================================
// parseYAMLPrimitive
//=============================================================================

static Result<void> parseYAMLPrimitive(YDrawBuffer* buffer,
                                       const YAML::Node& item) {
    uint32_t layer = buffer->primCount() + buffer->textSpanCount();

    // Text
    if (item["text"]) {
        auto t = item["text"];
        float x = 0, y = 0, fontSize = 16;
        float rotation = 0;
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
        if (t["rotation"]) rotation = t["rotation"].as<float>();

        int fontId = -1;
        if (t["font"]) {
            auto ttfPath = t["font"].as<std::string>();
            std::ifstream f(ttfPath, std::ios::binary | std::ios::ate);
            if (!f) return Err<void>("failed to read font: " + ttfPath);
            auto sz = f.tellg();
            f.seekg(0);
            std::vector<uint8_t> ttfData(static_cast<size_t>(sz));
            f.read(reinterpret_cast<char*>(ttfData.data()), sz);
            auto stem = std::filesystem::path(ttfPath).stem().string();
            fontId = buffer->addFontBlob(ttfData.data(), ttfData.size(), stem);
        }

        if (std::abs(rotation) > 0.001f) {
            float angleRadians = rotation * (3.14159265358979323846f / 180.0f);
            buffer->addRotatedText(x, y, content, fontSize, color, angleRadians, fontId);
        } else {
            buffer->addText(x, y, content, fontSize, color, layer, fontId);
        }
        return Ok();
    }

    // Circle
    if (item["circle"]) {
        auto c = item["circle"];
        float cx = 0, cy = 0, r = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (c["position"] && c["position"].IsSequence()) {
            cx = c["position"][0].as<float>(); cy = c["position"][1].as<float>();
        }
        if (c["radius"]) r = c["radius"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
        buffer->addCircle(layer, cx, cy, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Box
    if (item["box"]) {
        auto b = item["box"];
        float cx = 0, cy = 0, hw = 10, hh = 10, round = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (b["position"] && b["position"].IsSequence()) {
            cx = b["position"][0].as<float>(); cy = b["position"][1].as<float>();
        }
        if (b["size"] && b["size"].IsSequence()) {
            hw = b["size"][0].as<float>() / 2; hh = b["size"][1].as<float>() / 2;
        }
        if (b["fill"]) fill = parseColor(b["fill"]);
        if (b["stroke"]) stroke = parseColor(b["stroke"]);
        if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
        if (b["round"]) round = b["round"].as<float>();
        buffer->addBox(layer, cx, cy, hw, hh, fill, stroke, strokeWidth, round);
        return Ok();
    }

    // Segment
    if (item["segment"]) {
        auto s = item["segment"];
        float x0 = 0, y0 = 0, x1 = 100, y1 = 100;
        uint32_t stroke = 0xFFFFFFFF;
        float strokeWidth = 1;
        if (s["from"] && s["from"].IsSequence()) {
            x0 = s["from"][0].as<float>(); y0 = s["from"][1].as<float>();
        }
        if (s["to"] && s["to"].IsSequence()) {
            x1 = s["to"][0].as<float>(); y1 = s["to"][1].as<float>();
        }
        if (s["stroke"]) stroke = parseColor(s["stroke"]);
        if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();
        buffer->addSegment(layer, x0, y0, x1, y1, 0, stroke, strokeWidth, 0);
        return Ok();
    }

    // Triangle
    if (item["triangle"]) {
        auto t = item["triangle"];
        float x0 = 0, y0 = 0, x1 = 50, y1 = 100, x2 = 100, y2 = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (t["p0"] && t["p0"].IsSequence()) { x0 = t["p0"][0].as<float>(); y0 = t["p0"][1].as<float>(); }
        if (t["p1"] && t["p1"].IsSequence()) { x1 = t["p1"][0].as<float>(); y1 = t["p1"][1].as<float>(); }
        if (t["p2"] && t["p2"].IsSequence()) { x2 = t["p2"][0].as<float>(); y2 = t["p2"][1].as<float>(); }
        if (t["fill"]) fill = parseColor(t["fill"]);
        if (t["stroke"]) stroke = parseColor(t["stroke"]);
        if (t["stroke-width"]) strokeWidth = t["stroke-width"].as<float>();
        buffer->addTriangle(layer, x0, y0, x1, y1, x2, y2, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Bezier
    if (item["bezier"]) {
        auto b = item["bezier"];
        float x0 = 0, y0 = 0, x1 = 50, y1 = 50, x2 = 100, y2 = 0;
        uint32_t stroke = 0xFFFFFFFF;
        float strokeWidth = 1;
        if (b["p0"] && b["p0"].IsSequence()) { x0 = b["p0"][0].as<float>(); y0 = b["p0"][1].as<float>(); }
        if (b["p1"] && b["p1"].IsSequence()) { x1 = b["p1"][0].as<float>(); y1 = b["p1"][1].as<float>(); }
        if (b["p2"] && b["p2"].IsSequence()) { x2 = b["p2"][0].as<float>(); y2 = b["p2"][1].as<float>(); }
        if (b["stroke"]) stroke = parseColor(b["stroke"]);
        if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
        buffer->addBezier2(layer, x0, y0, x1, y1, x2, y2, 0, stroke, strokeWidth, 0);
        return Ok();
    }

    // Ellipse
    if (item["ellipse"]) {
        auto e = item["ellipse"];
        float cx = 0, cy = 0, rx = 20, ry = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>(); cy = e["position"][1].as<float>();
        }
        if (e["radii"] && e["radii"].IsSequence()) {
            rx = e["radii"][0].as<float>(); ry = e["radii"][1].as<float>();
        }
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        buffer->addEllipse(layer, cx, cy, rx, ry, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Arc
    if (item["arc"]) {
        auto a = item["arc"];
        float cx = 0, cy = 0, ra = 20, rb = 2, angle = 90.0f;
        uint32_t fill = 0, stroke = 0xFFFFFFFF;
        float strokeWidth = 0;
        if (a["position"] && a["position"].IsSequence()) {
            cx = a["position"][0].as<float>(); cy = a["position"][1].as<float>();
        }
        if (a["angle"]) angle = a["angle"].as<float>();
        if (a["radius"]) ra = a["radius"].as<float>();
        if (a["thickness"]) rb = a["thickness"].as<float>();
        if (a["fill"]) fill = parseColor(a["fill"]);
        if (a["stroke"]) stroke = parseColor(a["stroke"]);
        if (a["stroke-width"]) strokeWidth = a["stroke-width"].as<float>();
        float rad = angle * 3.14159265f / 180.0f;
        buffer->addArc(layer, cx, cy, std::sin(rad), std::cos(rad), ra, rb, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Radius shapes (center+radius)
    {
        float cx = 0, cy = 0, r = 20;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        auto doRadius = [&](const char* name) -> bool {
            if (!item[name]) return false;
            auto n = item[name];
            cx = 0; cy = 0; r = 20; fill = 0; stroke = 0; strokeWidth = 0;
            if (n["position"] && n["position"].IsSequence()) {
                cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>();
            }
            if (n["radius"]) r = n["radius"].as<float>();
            if (n["fill"]) fill = parseColor(n["fill"]);
            if (n["stroke"]) stroke = parseColor(n["stroke"]);
            if (n["stroke-width"]) strokeWidth = n["stroke-width"].as<float>();
            return true;
        };
        if (doRadius("pentagon"))             { buffer->addPentagon(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("hexagon"))              { buffer->addHexagon(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("equilateral_triangle")) { buffer->addEquilateralTriangle(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("octogon"))              { buffer->addOctogon(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("hexagram"))             { buffer->addHexagram(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("pentagram"))            { buffer->addPentagram(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("quadratic_circle"))     { buffer->addQuadraticCircle(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
        if (doRadius("cool_s"))               { buffer->addCoolS(layer, cx, cy, r, fill, stroke, strokeWidth, 0); return Ok(); }
    }

    // Star
    if (item["star"]) {
        auto s = item["star"];
        float cx = 0, cy = 0, r = 20, n = 5, m = 2.5f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (s["position"] && s["position"].IsSequence()) {
            cx = s["position"][0].as<float>(); cy = s["position"][1].as<float>();
        }
        if (s["radius"]) r = s["radius"].as<float>();
        if (s["points"]) n = s["points"].as<float>();
        if (s["inner"]) m = s["inner"].as<float>();
        if (s["fill"]) fill = parseColor(s["fill"]);
        if (s["stroke"]) stroke = parseColor(s["stroke"]);
        if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();
        buffer->addStar(layer, cx, cy, r, n, m, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Pie
    if (item["pie"]) {
        auto p = item["pie"];
        float cx = 0, cy = 0, r = 20, angle = 45.0f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (p["position"] && p["position"].IsSequence()) {
            cx = p["position"][0].as<float>(); cy = p["position"][1].as<float>();
        }
        if (p["angle"]) angle = p["angle"].as<float>();
        if (p["radius"]) r = p["radius"].as<float>();
        if (p["fill"]) fill = parseColor(p["fill"]);
        if (p["stroke"]) stroke = parseColor(p["stroke"]);
        if (p["stroke-width"]) strokeWidth = p["stroke-width"].as<float>();
        float rad = angle * 3.14159265f / 180.0f;
        buffer->addPie(layer, cx, cy, std::sin(rad), std::cos(rad), r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Ring
    if (item["ring"]) {
        auto rg = item["ring"];
        float cx = 0, cy = 0, r = 20, th = 4, angle = 0.0f;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (rg["position"] && rg["position"].IsSequence()) {
            cx = rg["position"][0].as<float>(); cy = rg["position"][1].as<float>();
        }
        if (rg["angle"]) angle = rg["angle"].as<float>();
        if (rg["radius"]) r = rg["radius"].as<float>();
        if (rg["thickness"]) th = rg["thickness"].as<float>();
        if (rg["fill"]) fill = parseColor(rg["fill"]);
        if (rg["stroke"]) stroke = parseColor(rg["stroke"]);
        if (rg["stroke-width"]) strokeWidth = rg["stroke-width"].as<float>();
        float rad = angle * 3.14159265f / 180.0f;
        buffer->addRing(layer, cx, cy, std::sin(rad), std::cos(rad), r, th, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Heart
    if (item["heart"]) {
        auto h = item["heart"];
        float cx = 0, cy = 0, scale = 20;
        uint32_t fill = 0xFF0000FF, stroke = 0;
        float strokeWidth = 0;
        if (h["position"] && h["position"].IsSequence()) {
            cx = h["position"][0].as<float>(); cy = h["position"][1].as<float>();
        }
        if (h["scale"]) scale = h["scale"].as<float>();
        if (h["fill"]) fill = parseColor(h["fill"]);
        if (h["stroke"]) stroke = parseColor(h["stroke"]);
        if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();
        buffer->addHeart(layer, cx, cy, scale, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Cross
    if (item["cross"]) {
        auto c = item["cross"];
        float cx = 0, cy = 0, w = 20, h = 5, r = 0;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (c["position"] && c["position"].IsSequence()) {
            cx = c["position"][0].as<float>(); cy = c["position"][1].as<float>();
        }
        if (c["size"] && c["size"].IsSequence()) {
            w = c["size"][0].as<float>(); h = c["size"][1].as<float>();
        }
        if (c["round"]) r = c["round"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
        buffer->addCross(layer, cx, cy, w, h, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // RoundedX
    if (item["rounded_x"]) {
        auto x = item["rounded_x"];
        float cx = 0, cy = 0, w = 20, r = 3;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (x["position"] && x["position"].IsSequence()) {
            cx = x["position"][0].as<float>(); cy = x["position"][1].as<float>();
        }
        if (x["width"]) w = x["width"].as<float>();
        if (x["round"]) r = x["round"].as<float>();
        if (x["fill"]) fill = parseColor(x["fill"]);
        if (x["stroke"]) stroke = parseColor(x["stroke"]);
        if (x["stroke-width"]) strokeWidth = x["stroke-width"].as<float>();
        buffer->addRoundedX(layer, cx, cy, w, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Capsule (2D or 3D)
    if (item["capsule"]) {
        auto c = item["capsule"];
        bool is3D = c["height"].IsDefined();
        if (!is3D && c["position"] && c["position"].IsSequence() && c["position"].size() > 2) is3D = true;
        if (is3D) {
            float px = 0, py = 0, pz = 0;
            if (c["position"] && c["position"].IsSequence()) {
                px = c["position"][0].as<float>();
                py = c["position"][1].as<float>();
                pz = c["position"].size() > 2 ? c["position"][2].as<float>() : 0.0f;
            }
            float height = c["height"] ? c["height"].as<float>() : 0.3f;
            float radius = c["radius"] ? c["radius"].as<float>() : 0.1f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
            buffer->addVerticalCapsule3D(layer, px, py, pz, height, radius, fill, stroke, strokeWidth, 0);
            return Ok();
        }
        float x0 = 0, y0 = 0, x1 = 100, y1 = 0, r = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (c["from"] && c["from"].IsSequence()) { x0 = c["from"][0].as<float>(); y0 = c["from"][1].as<float>(); }
        if (c["to"] && c["to"].IsSequence()) { x1 = c["to"][0].as<float>(); y1 = c["to"][1].as<float>(); }
        if (c["radius"]) r = c["radius"].as<float>();
        if (c["fill"]) fill = parseColor(c["fill"]);
        if (c["stroke"]) stroke = parseColor(c["stroke"]);
        if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
        buffer->addCapsule(layer, x0, y0, x1, y1, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Rhombus
    if (item["rhombus"]) {
        auto r = item["rhombus"];
        float cx = 0, cy = 0, w = 20, h = 30;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (r["position"] && r["position"].IsSequence()) {
            cx = r["position"][0].as<float>(); cy = r["position"][1].as<float>();
        }
        if (r["size"] && r["size"].IsSequence()) {
            w = r["size"][0].as<float>(); h = r["size"][1].as<float>();
        }
        if (r["fill"]) fill = parseColor(r["fill"]);
        if (r["stroke"]) stroke = parseColor(r["stroke"]);
        if (r["stroke-width"]) strokeWidth = r["stroke-width"].as<float>();
        buffer->addRhombus(layer, cx, cy, w, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Moon
    if (item["moon"]) {
        auto m = item["moon"];
        float cx = 0, cy = 0, d = 10, ra = 20, rb = 15;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (m["position"] && m["position"].IsSequence()) {
            cx = m["position"][0].as<float>(); cy = m["position"][1].as<float>();
        }
        if (m["distance"]) d = m["distance"].as<float>();
        if (m["radius_outer"]) ra = m["radius_outer"].as<float>();
        if (m["radius_inner"]) rb = m["radius_inner"].as<float>();
        if (m["fill"]) fill = parseColor(m["fill"]);
        if (m["stroke"]) stroke = parseColor(m["stroke"]);
        if (m["stroke-width"]) strokeWidth = m["stroke-width"].as<float>();
        buffer->addMoon(layer, cx, cy, d, ra, rb, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Egg
    if (item["egg"]) {
        auto e = item["egg"];
        float cx = 0, cy = 0, ra = 20, rb = 10;
        uint32_t fill = 0, stroke = 0;
        float strokeWidth = 0;
        if (e["position"] && e["position"].IsSequence()) {
            cx = e["position"][0].as<float>(); cy = e["position"][1].as<float>();
        }
        if (e["radius_bottom"]) ra = e["radius_bottom"].as<float>();
        if (e["radius_top"]) rb = e["radius_top"].as<float>();
        if (e["fill"]) fill = parseColor(e["fill"]);
        if (e["stroke"]) stroke = parseColor(e["stroke"]);
        if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
        buffer->addEgg(layer, cx, cy, ra, rb, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Common style parser for shapes
    auto parseStyle = [&](const YAML::Node& n, uint32_t& fill, uint32_t& stroke, float& strokeWidth) {
        fill = 0; stroke = 0; strokeWidth = 0;
        if (n["fill"]) fill = parseColor(n["fill"]);
        if (n["stroke"]) stroke = parseColor(n["stroke"]);
        if (n["stroke-width"]) strokeWidth = n["stroke-width"].as<float>();
    };

    // ChamferBox
    if (item["chamfer_box"]) {
        auto n = item["chamfer_box"];
        float cx = 0, cy = 0, hw = 0, hh = 0, chamfer = 5;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["half_size"] && n["half_size"].IsSequence()) { hw = n["half_size"][0].as<float>(); hh = n["half_size"][1].as<float>(); }
        if (n["chamfer"]) chamfer = n["chamfer"].as<float>();
        buffer->addChamferBox(layer, cx, cy, hw, hh, chamfer, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // OrientedBox
    if (item["oriented_box"]) {
        auto n = item["oriented_box"];
        float ax = 0, ay = 0, bx = 30, by = 20, th = 5;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["a"] && n["a"].IsSequence()) { ax = n["a"][0].as<float>(); ay = n["a"][1].as<float>(); }
        if (n["b"] && n["b"].IsSequence()) { bx = n["b"][0].as<float>(); by = n["b"][1].as<float>(); }
        if (n["thickness"]) th = n["thickness"].as<float>();
        buffer->addOrientedBox(layer, ax, ay, bx, by, th, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Trapezoid
    if (item["trapezoid"]) {
        auto n = item["trapezoid"];
        float cx = 0, cy = 0, r1 = 30, r2 = 15, he = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["r1"]) r1 = n["r1"].as<float>();
        if (n["r2"]) r2 = n["r2"].as<float>();
        if (n["height"]) he = n["height"].as<float>();
        buffer->addTrapezoid(layer, cx, cy, r1, r2, he, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Parallelogram
    if (item["parallelogram"]) {
        auto n = item["parallelogram"];
        float cx = 0, cy = 0, wi = 30, he = 20, sk = 10;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["width"]) wi = n["width"].as<float>();
        if (n["height"]) he = n["height"].as<float>();
        if (n["skew"]) sk = n["skew"].as<float>();
        buffer->addParallelogram(layer, cx, cy, wi, he, sk, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // IsoscelesTriangle
    if (item["isosceles_triangle"]) {
        auto n = item["isosceles_triangle"];
        float cx = 0, cy = 0, qx = 15, qy = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["half_width"]) qx = n["half_width"].as<float>();
        if (n["height"]) qy = n["height"].as<float>();
        buffer->addIsoscelesTriangle(layer, cx, cy, qx, qy, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // UnevenCapsule
    if (item["uneven_capsule"]) {
        auto n = item["uneven_capsule"];
        float cx = 0, cy = 0, r1 = 10, r2 = 5, h = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["r1"]) r1 = n["r1"].as<float>();
        if (n["r2"]) r2 = n["r2"].as<float>();
        if (n["height"]) h = n["height"].as<float>();
        buffer->addUnevenCapsule(layer, cx, cy, r1, r2, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // CutDisk
    if (item["cut_disk"]) {
        auto n = item["cut_disk"];
        float cx = 0, cy = 0, r = 20, h = 10;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["radius"]) r = n["radius"].as<float>();
        if (n["h"]) h = n["h"].as<float>();
        buffer->addCutDisk(layer, cx, cy, r, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Horseshoe
    if (item["horseshoe"]) {
        auto n = item["horseshoe"];
        float cx = 0, cy = 0, angle = 1.0f, r = 20, wx = 5, wy = 5;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["angle"]) angle = n["angle"].as<float>();
        if (n["radius"]) r = n["radius"].as<float>();
        if (n["width"] && n["width"].IsSequence()) { wx = n["width"][0].as<float>(); wy = n["width"][1].as<float>(); }
        buffer->addHorseshoe(layer, cx, cy, std::sin(angle), std::cos(angle), r, wx, wy, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Vesica
    if (item["vesica"]) {
        auto n = item["vesica"];
        float cx = 0, cy = 0, w = 30, h = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["width"]) w = n["width"].as<float>();
        if (n["height"]) h = n["height"].as<float>();
        buffer->addVesica(layer, cx, cy, w, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // OrientedVesica
    if (item["oriented_vesica"]) {
        auto n = item["oriented_vesica"];
        float ax = 0, ay = 0, bx = 30, by = 20, w = 10;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["a"] && n["a"].IsSequence()) { ax = n["a"][0].as<float>(); ay = n["a"][1].as<float>(); }
        if (n["b"] && n["b"].IsSequence()) { bx = n["b"][0].as<float>(); by = n["b"][1].as<float>(); }
        if (n["width"]) w = n["width"].as<float>();
        buffer->addOrientedVesica(layer, ax, ay, bx, by, w, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // RoundedCross
    if (item["rounded_cross"]) {
        auto n = item["rounded_cross"];
        float cx = 0, cy = 0, h = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["h"]) h = n["h"].as<float>();
        buffer->addRoundedCross(layer, cx, cy, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Parabola
    if (item["parabola"]) {
        auto n = item["parabola"];
        float cx = 0, cy = 0, k = 1.0f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["k"]) k = n["k"].as<float>();
        buffer->addParabola(layer, cx, cy, k, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // BlobbyCross
    if (item["blobby_cross"]) {
        auto n = item["blobby_cross"];
        float cx = 0, cy = 0, he = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["he"]) he = n["he"].as<float>();
        buffer->addBlobbyCross(layer, cx, cy, he, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Tunnel
    if (item["tunnel"]) {
        auto n = item["tunnel"];
        float cx = 0, cy = 0, w = 30, h = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["width"]) w = n["width"].as<float>();
        if (n["height"]) h = n["height"].as<float>();
        buffer->addTunnel(layer, cx, cy, w, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Stairs
    if (item["stairs"]) {
        auto n = item["stairs"];
        float cx = 0, cy = 0, sw = 10, sh = 5, count = 5;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["step_width"]) sw = n["step_width"].as<float>();
        if (n["step_height"]) sh = n["step_height"].as<float>();
        if (n["count"]) count = n["count"].as<float>();
        buffer->addStairs(layer, cx, cy, sw, sh, count, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Hyperbola
    if (item["hyperbola"]) {
        auto n = item["hyperbola"];
        float cx = 0, cy = 0, k = 1.0f, he = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["k"]) k = n["k"].as<float>();
        if (n["he"]) he = n["he"].as<float>();
        buffer->addHyperbola(layer, cx, cy, k, he, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // CircleWave
    if (item["circle_wave"]) {
        auto n = item["circle_wave"];
        float cx = 0, cy = 0, tb = 1.0f, ra = 20;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        if (n["position"] && n["position"].IsSequence()) { cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>(); }
        if (n["tb"]) tb = n["tb"].as<float>();
        if (n["ra"]) ra = n["ra"].as<float>();
        buffer->addCircleWave(layer, cx, cy, tb, ra, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // 3D position helper
    auto parse3DPos = [](const YAML::Node& n, float& px, float& py, float& pz) {
        px = py = pz = 0;
        if (n["position"] && n["position"].IsSequence()) {
            px = n["position"][0].as<float>();
            py = n["position"][1].as<float>();
            pz = n["position"].size() > 2 ? n["position"][2].as<float>() : 0.0f;
        }
    };

    // Sphere3D
    if (item["sphere"]) {
        auto n = item["sphere"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float r = n["radius"] ? n["radius"].as<float>() : 0.25f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addSphere3D(layer, px, py, pz, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Box3D
    if (item["box3d"]) {
        auto n = item["box3d"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float bx = 0.2f, by = 0.2f, bz = 0.2f;
        if (n["size"] && n["size"].IsSequence()) {
            bx = n["size"][0].as<float>();
            by = n["size"][1].as<float>();
            bz = n["size"].size() > 2 ? n["size"][2].as<float>() : bx;
        }
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addBox3D(layer, px, py, pz, bx, by, bz, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Torus3D
    if (item["torus"]) {
        auto n = item["torus"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float majorR = n["major-radius"] ? n["major-radius"].as<float>() : 0.2f;
        float minorR = n["minor-radius"] ? n["minor-radius"].as<float>() : 0.08f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addTorus3D(layer, px, py, pz, majorR, minorR, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Cylinder3D
    if (item["cylinder"]) {
        auto n = item["cylinder"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float r = n["radius"] ? n["radius"].as<float>() : 0.15f;
        float h = n["height"] ? n["height"].as<float>() : 0.3f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addCylinder3D(layer, px, py, pz, r, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // VerticalCapsule3D
    if (item["capsule3d"]) {
        auto n = item["capsule3d"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float h = n["height"] ? n["height"].as<float>() : 0.3f;
        float r = n["radius"] ? n["radius"].as<float>() : 0.1f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addVerticalCapsule3D(layer, px, py, pz, h, r, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // CappedCone3D
    if (item["cone"]) {
        auto n = item["cone"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float h = n["height"] ? n["height"].as<float>() : 0.35f;
        float r1 = n["radius1"] ? n["radius1"].as<float>() : 0.2f;
        float r2 = n["radius2"] ? n["radius2"].as<float>() : 0.05f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addCappedCone3D(layer, px, py, pz, h, r1, r2, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Octahedron3D
    if (item["octahedron"]) {
        auto n = item["octahedron"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float s = n["size"] ? n["size"].as<float>() : 0.3f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addOctahedron3D(layer, px, py, pz, s, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Pyramid3D
    if (item["pyramid"]) {
        auto n = item["pyramid"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float h = n["height"] ? n["height"].as<float>() : 0.5f;
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addPyramid3D(layer, px, py, pz, h, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    // Ellipsoid3D
    if (item["ellipsoid"]) {
        auto n = item["ellipsoid"];
        float px, py, pz; parse3DPos(n, px, py, pz);
        float rx = 0.3f, ry = 0.2f, rz = 0.15f;
        if (n["radii"] && n["radii"].IsSequence()) {
            rx = n["radii"][0].as<float>();
            ry = n["radii"][1].as<float>();
            rz = n["radii"].size() > 2 ? n["radii"][2].as<float>() : rx;
        }
        uint32_t fill, stroke; float strokeWidth;
        parseStyle(n, fill, stroke, strokeWidth);
        buffer->addEllipsoid3D(layer, px, py, pz, rx, ry, rz, fill, stroke, strokeWidth, 0);
        return Ok();
    }

    return Ok();
}

//=============================================================================
// parseYDrawYAML
//=============================================================================

Result<animation::Animation::Ptr> parseYDrawYAML(YDrawBuffer::Ptr buffer,
                                                  const std::string& yaml) {
    animation::Animation::Ptr animation;
    try {
        YAML::Node root = YAML::Load(yaml);
        bool rootIsSequence = root.IsSequence();

        auto parsePrimWithAnim = [&](const YAML::Node& item) -> Result<void> {
            uint32_t primBefore = buffer->primCount();
            if (auto res = parseYAMLPrimitive(buffer.get(), item); !res) return res;
            uint32_t primAfter = buffer->primCount();

            if (primAfter > primBefore && item.IsMap()) {
                for (auto it = item.begin(); it != item.end(); ++it) {
                    auto shapeNode = it->second;
                    if (shapeNode.IsMap() && shapeNode["animate"]) {
                        parseAnimateBlock(animation, buffer, shapeNode["animate"], primBefore);
                    }
                }
            }
            return Ok();
        };

        auto parseSettings = [&](const YAML::Node& node) {
            if (node["background"]) {
                buffer->setBgColor(parseColor(node["background"]));
            }
            if (node["flags"]) {
                auto flagsNode = node["flags"];
                if (flagsNode.IsSequence()) {
                    for (const auto& flag : flagsNode) {
                        std::string f = flag.as<std::string>();
                        if (f == "show_bounds") buffer->addFlags(YDrawBuffer::FLAG_SHOW_BOUNDS);
                        else if (f == "show_grid") buffer->addFlags(YDrawBuffer::FLAG_SHOW_GRID);
                        else if (f == "show_eval_count") buffer->addFlags(YDrawBuffer::FLAG_SHOW_EVAL_COUNT);
                    }
                } else if (flagsNode.IsScalar()) {
                    std::string f = flagsNode.as<std::string>();
                    if (f == "show_bounds") buffer->addFlags(YDrawBuffer::FLAG_SHOW_BOUNDS);
                    else if (f == "show_grid") buffer->addFlags(YDrawBuffer::FLAG_SHOW_GRID);
                    else if (f == "show_eval_count") buffer->addFlags(YDrawBuffer::FLAG_SHOW_EVAL_COUNT);
                }
            }
            if (node["animation"]) {
                auto animNode = node["animation"];
                auto* a = getAnim(animation, buffer);
                if (animNode["duration"]) a->setDuration(animNode["duration"].as<float>());
                if (animNode["loop"]) a->setLoop(animNode["loop"].as<bool>());
                if (animNode["speed"]) a->setSpeed(animNode["speed"].as<float>());
            }
        };

        if (rootIsSequence) {
            for (const auto& doc : root) {
                if (doc.IsMap()) {
                    parseSettings(doc);
                    if (doc["body"] && doc["body"].IsSequence()) {
                        for (const auto& item : doc["body"]) {
                            if (auto res = parsePrimWithAnim(item); !res)
                                return Err<animation::Animation::Ptr>(res.error().message());
                        }
                    }
                }
            }
        } else if (root.IsMap()) {
            parseSettings(root);
            if (root["body"] && root["body"].IsSequence()) {
                for (const auto& item : root["body"]) {
                    if (auto res = parsePrimWithAnim(item); !res)
                        return Err<animation::Animation::Ptr>(res.error().message());
                }
            }
        }

        return Ok(std::move(animation));
    } catch (const YAML::Exception& e) {
        return Err<animation::Animation::Ptr>(std::string("YAML parse error: ") + e.what());
    }
}

} // namespace yetty
