#include "zoo-renderer.h"
#include "../ydraw/ydraw-buffer.h"
#include <cmath>
#include <chrono>
#include <sstream>

namespace yetty::zoo {

//=============================================================================
// Argument parsing
//=============================================================================

ZooConfig parseArgs(const std::string& args) {
    ZooConfig cfg;
    if (args.empty()) return cfg;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--points" || token == "-n") {
            uint32_t v; if (iss >> v) cfg.targetPoints = std::max(2u, std::min(v, 100u));
        } else if (token == "--connections" || token == "-k") {
            uint32_t v; if (iss >> v) cfg.targetConnsPerCP = std::max(1u, std::min(v, 10u));
        } else if (token == "--growth" || token == "-g") {
            float v; if (iss >> v) cfg.growthRate = std::max(0.05f, std::min(v, 3.0f));
        } else if (token == "--max-dist") {
            float v; if (iss >> v) cfg.maxConnectionDist = std::max(20.0f, std::min(v, 600.0f));
        } else if (token == "--marker-size") {
            float v; if (iss >> v) cfg.cpMarkerSize = std::max(0.5f, std::min(v, 20.0f));
        } else if (token == "--stroke-min") {
            float v; if (iss >> v) cfg.strokeMin = std::max(0.1f, std::min(v, 10.0f));
        } else if (token == "--stroke-max") {
            float v; if (iss >> v) cfg.strokeMax = std::max(cfg.strokeMin, std::min(v, 20.0f));
        } else if (token == "--curve-ratio") {
            float v; if (iss >> v) cfg.curveRatio = std::max(0.0f, std::min(v, 1.0f));
        } else if (token == "--spawn-radius") {
            std::string s;
            if (iss >> s) {
                auto comma = s.find(',');
                if (comma != std::string::npos) {
                    try {
                        cfg.spawnRadiusMin = std::stof(s.substr(0, comma));
                        cfg.spawnRadiusMax = std::stof(s.substr(comma + 1));
                        if (cfg.spawnRadiusMax < cfg.spawnRadiusMin)
                            std::swap(cfg.spawnRadiusMin, cfg.spawnRadiusMax);
                    } catch (...) {}
                }
            }
        } else if (token == "--bg-color") {
            std::string s;
            if (iss >> s) {
                if (s.substr(0, 2) == "0x" || s.substr(0, 2) == "0X")
                    s = s.substr(2);
                try {
                    cfg.bgColor = static_cast<uint32_t>(std::stoul(s, nullptr, 16));
                } catch (...) {}
            }
        }
    }
    return cfg;
}

//=============================================================================
// ZooRenderer
//=============================================================================

ZooRenderer::ZooRenderer(const ZooConfig& config, uint32_t seed)
    : _config(config)
{
    if (seed == 0) {
        auto s = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        _rng.seed(static_cast<uint32_t>(s));
    } else {
        _rng.seed(seed);
    }
}

void ZooRenderer::render(std::shared_ptr<YDrawBuffer> buffer, float time) {
    // First frame: seed initial control points with staggered ages
    if (!_initialized) {
        _lastTime = time;
        for (uint32_t i = 0; i < _config.targetPoints; i++) {
            spawnControlPoint(time - std::uniform_real_distribution<float>(0.0f, 3.0f)(_rng));
        }
        _initialized = true;
    }

    _lastTime = time;

    // Cull control points outside scene
    for (int i = static_cast<int>(_controlPoints.size()) - 1; i >= 0; i--) {
        auto pos = cpPosition(_controlPoints[i], time);
        float age = time - _controlPoints[i].spawnTime;
        if (age < 0.0f) age = 0.0f;
        float scale = std::exp(_config.growthRate * age);
        float margin = _config.cpMarkerSize * scale * 2.0f;
        if (pos.x - margin > _config.sceneWidth || pos.x + margin < 0 ||
            pos.y - margin > _config.sceneHeight || pos.y + margin < 0) {
            removeControlPoint(static_cast<uint32_t>(i));
        }
    }

    // Spawn replacements
    while (_controlPoints.size() < _config.targetPoints) {
        spawnControlPoint(time);
    }

    // Update connections (remove broken, add new)
    updateConnections(time);

    // Generate prims into buffer
    buffer->clear();
    buffer->setBgColor(_config.bgColor);
    buffer->setSceneBounds(0, 0, _config.sceneWidth, _config.sceneHeight);
    uint32_t layer = 0;

    // 1. Connection prims (curves/lines/shapes) — lower layers
    for (const auto& conn : _connections) {
        if (conn.cpA >= _controlPoints.size() || conn.cpB >= _controlPoints.size())
            continue;

        auto posA = cpPosition(_controlPoints[conn.cpA], time);
        auto posB = cpPosition(_controlPoints[conn.cpB], time);

        float curv = conn.curvature
                   + 0.15f * std::sin(time * conn.curveAnimFreq + conn.curveAnimPhase);

        float breathe = 0.6f + 0.4f * std::sin(time * 1.5f + conn.curveAnimPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 200.0f);
        uint32_t color = (conn.color & 0x00FFFFFFu)
                       | (static_cast<uint32_t>(alpha) << 24);

        switch (conn.type) {
        case 0: {
            float dx = posB.x - posA.x;
            float dy = posB.y - posA.y;
            float len = std::sqrt(dx * dx + dy * dy);
            if (len < 0.001f) continue;
            float perpX = -dy / len;
            float perpY = dx / len;
            float midX = (posA.x + posB.x) * 0.5f;
            float midY = (posA.y + posB.y) * 0.5f;
            float ctrlX = midX + perpX * curv * len * 0.5f;
            float ctrlY = midY + perpY * curv * len * 0.5f;
            buffer->addBezier2(layer++, posA.x, posA.y, ctrlX, ctrlY, posB.x, posB.y, 0, color, conn.strokeWidth, 0.0f);
            break;
        }
        case 1: {
            buffer->addSegment(layer++, posA.x, posA.y, posB.x, posB.y, 0, color, conn.strokeWidth, 0.0f);
            break;
        }
        case 2: {
            float midX = (posA.x + posB.x) * 0.5f;
            float midY = (posA.y + posB.y) * 0.5f;
            float dx = posB.x - posA.x;
            float dy = posB.y - posA.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            float size = std::max(3.0f, dist * 0.12f);
            addShape(buffer, conn.shapeChoice, midX, midY, size, color, layer++);
            break;
        }
        }
    }

    // 2. Control point markers (small circles, higher layers)
    for (uint32_t i = 0; i < static_cast<uint32_t>(_controlPoints.size()); i++) {
        auto& cp = _controlPoints[i];
        auto pos = cpPosition(cp, time);

        float breathe = 0.7f + 0.3f * std::sin(time * cp.animFreq + cp.animPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 255.0f);
        uint32_t color = (cp.color & 0x00FFFFFFu)
                       | (static_cast<uint32_t>(alpha) << 24);

        float age = time - cp.spawnTime;
        if (age < 0.0f) age = 0.0f;
        float scale = std::exp(_config.growthRate * age);
        float markerSize = std::min(_config.cpMarkerSize * scale, 12.0f);

        buffer->addCircle(layer++, pos.x, pos.y, markerSize, color, 0, 0.0f, 0.0f);
    }
}

//=============================================================================
// Random color generation (HSL with high saturation)
//=============================================================================

uint32_t ZooRenderer::randomColor() {
    float hue = std::uniform_real_distribution<float>(0.0f, 360.0f)(_rng);
    float sat = std::uniform_real_distribution<float>(0.6f, 1.0f)(_rng);
    float lit = std::uniform_real_distribution<float>(0.45f, 0.65f)(_rng);

    float c = (1.0f - std::fabs(2.0f * lit - 1.0f)) * sat;
    float x = c * (1.0f - std::fabs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
    float m = lit - c / 2.0f;
    float r1, g1, b1;

    if      (hue < 60)  { r1 = c; g1 = x; b1 = 0; }
    else if (hue < 120) { r1 = x; g1 = c; b1 = 0; }
    else if (hue < 180) { r1 = 0; g1 = c; b1 = x; }
    else if (hue < 240) { r1 = 0; g1 = x; b1 = c; }
    else if (hue < 300) { r1 = x; g1 = 0; b1 = c; }
    else                { r1 = c; g1 = 0; b1 = x; }

    uint8_t r = static_cast<uint8_t>((r1 + m) * 255.0f);
    uint8_t g = static_cast<uint8_t>((g1 + m) * 255.0f);
    uint8_t b = static_cast<uint8_t>((b1 + m) * 255.0f);

    return 0xFF000000u | (static_cast<uint32_t>(b) << 16)
                       | (static_cast<uint32_t>(g) << 8)
                       | static_cast<uint32_t>(r);
}

//=============================================================================
// Shape generation
//=============================================================================

void ZooRenderer::addShape(std::shared_ptr<YDrawBuffer>& buffer,
                            int choice, float cx, float cy, float size,
                            uint32_t color, uint32_t layer) {
    switch (choice) {
    case 0: buffer->addCircle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 1: buffer->addBox(layer, cx, cy, size, size * 0.8f, color, 0, 0.0f, size * 0.15f); break;
    case 2: buffer->addSegment(layer, cx - size, cy, cx + size, cy, 0, color, size * 0.15f, 0.0f); break;
    case 3: buffer->addTriangle(layer, cx, cy - size, cx - size, cy + size * 0.7f, cx + size, cy + size * 0.7f, color, 0, 0.0f, 0.0f); break;
    case 4: buffer->addBezier2(layer, cx - size, cy, cx, cy - size, cx + size, cy, 0, color, size * 0.15f, 0.0f); break;
    case 5: buffer->addEllipse(layer, cx, cy, size, size * 0.6f, color, 0, 0.0f, 0.0f); break;
    case 6: buffer->addArc(layer, cx, cy, 0.866f, 0.5f, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 7: buffer->addRoundedBox(layer, cx, cy, size, size * 0.7f, size * 0.2f, size * 0.2f, size * 0.2f, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 8: buffer->addRhombus(layer, cx, cy, size, size * 1.4f, color, 0, 0.0f, 0.0f); break;
    case 9: buffer->addPentagon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 10: buffer->addHexagon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 11: buffer->addStar(layer, cx, cy, size, 5.0f, 2.5f, color, 0, 0.0f, 0.0f); break;
    case 12: buffer->addPie(layer, cx, cy, 0.866f, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 13: buffer->addRing(layer, cx, cy, 0.866f, 0.5f, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 14: buffer->addHeart(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 15: buffer->addCross(layer, cx, cy, size, size * 0.3f, size * 0.1f, color, 0, 0.0f, 0.0f); break;
    case 16: buffer->addRoundedX(layer, cx, cy, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 17: buffer->addCapsule(layer, cx - size * 0.7f, cy, cx + size * 0.7f, cy, size * 0.3f, 0, color, size * 0.15f, 0.0f); break;
    case 18: buffer->addMoon(layer, cx, cy, size * 0.5f, size, size * 0.8f, color, 0, 0.0f, 0.0f); break;
    case 19: buffer->addEgg(layer, cx, cy, size, size * 0.6f, color, 0, 0.0f, 0.0f); break;
    case 20: buffer->addChamferBox(layer, cx, cy, size, size * 0.8f, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 21: buffer->addOrientedBox(layer, cx - size, cy - size * 0.3f, cx + size, cy + size * 0.3f, size * 0.4f, color, 0, 0.0f, 0.0f); break;
    case 22: buffer->addTrapezoid(layer, cx, cy, size * 0.8f, size * 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 23: buffer->addParallelogram(layer, cx, cy, size, size * 0.6f, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 24: buffer->addEquilateralTriangle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 25: buffer->addIsoscelesTriangle(layer, cx, cy, size * 0.7f, size * 1.2f, color, 0, 0.0f, 0.0f); break;
    case 26: buffer->addUnevenCapsule(layer, cx, cy, size * 0.5f, size * 0.3f, size * 1.2f, color, 0, 0.0f, 0.0f); break;
    case 27: buffer->addOctogon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 28: buffer->addHexagram(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 29: buffer->addPentagram(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 30: buffer->addCutDisk(layer, cx, cy, size, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 31: buffer->addHorseshoe(layer, cx, cy, 0.9636f, 0.2675f, size * 0.8f, size * 0.3f, size * 0.15f, color, 0, 0.0f, 0.0f); break;
    case 32: buffer->addVesica(layer, cx, cy, size * 0.6f, size, color, 0, 0.0f, 0.0f); break;
    case 33: buffer->addOrientedVesica(layer, cx - size * 0.5f, cy, cx + size * 0.5f, cy, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 34: buffer->addRoundedCross(layer, cx, cy, size * 0.5f, color, 0, 0.0f, 0.0f); break;
    case 35: buffer->addParabola(layer, cx, cy, 0.5f, color, 0, 0.0f, 0.0f); break;
    case 36: buffer->addBlobbyCross(layer, cx, cy, size * 0.5f, color, 0, 0.0f, 0.0f); break;
    case 37: buffer->addTunnel(layer, cx, cy, size, size * 0.7f, color, 0, 0.0f, 0.0f); break;
    case 38: buffer->addStairs(layer, cx, cy, size * 0.3f, size * 0.2f, 5.0f, color, 0, 0.0f, 0.0f); break;
    case 39: buffer->addQuadraticCircle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 40: buffer->addHyperbola(layer, cx, cy, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 41: buffer->addCoolS(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 42: buffer->addCircleWave(layer, cx, cy, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    }
}

//=============================================================================
// Control point + connection lifecycle
//=============================================================================

ZooRenderer::Vec2f ZooRenderer::cpPosition(const ControlPoint& cp, float time) const {
    float centerX = _config.sceneWidth / 2.0f;
    float centerY = _config.sceneHeight / 2.0f;
    float age = time - cp.spawnTime;
    if (age < 0.0f) age = 0.0f;
    float scale = std::exp(_config.growthRate * age);
    float dist = cp.baseRadius * scale;
    return { centerX + dist * std::cos(cp.angle),
             centerY + dist * std::sin(cp.angle) };
}

uint32_t ZooRenderer::blendColors(uint32_t a, uint32_t b) {
    uint32_t r = ((a & 0xFF) + (b & 0xFF)) / 2;
    uint32_t g = (((a >> 8) & 0xFF) + ((b >> 8) & 0xFF)) / 2;
    uint32_t bl = (((a >> 16) & 0xFF) + ((b >> 16) & 0xFF)) / 2;
    return 0xFF000000u | (bl << 16) | (g << 8) | r;
}

void ZooRenderer::spawnControlPoint(float time) {
    ControlPoint cp = {};
    cp.angle = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    cp.spawnTime = time;
    cp.baseRadius = std::uniform_real_distribution<float>(_config.spawnRadiusMin, _config.spawnRadiusMax)(_rng);
    cp.animPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    cp.animFreq = std::uniform_real_distribution<float>(0.8f, 2.5f)(_rng);
    cp.color = randomColor();
    _controlPoints.push_back(cp);
}

void ZooRenderer::removeControlPoint(uint32_t idx) {
    for (int i = static_cast<int>(_connections.size()) - 1; i >= 0; i--) {
        if (_connections[i].cpA == idx || _connections[i].cpB == idx) {
            _connections.erase(_connections.begin() + i);
        }
    }
    for (auto& conn : _connections) {
        if (conn.cpA > idx) conn.cpA--;
        if (conn.cpB > idx) conn.cpB--;
    }
    _controlPoints.erase(_controlPoints.begin() + idx);
}

void ZooRenderer::updateConnections(float time) {
    // Remove connections where endpoints drifted too far apart
    for (int i = static_cast<int>(_connections.size()) - 1; i >= 0; i--) {
        auto& conn = _connections[i];
        if (conn.cpA >= _controlPoints.size() || conn.cpB >= _controlPoints.size()) {
            _connections.erase(_connections.begin() + i);
            continue;
        }
        auto posA = cpPosition(_controlPoints[conn.cpA], time);
        auto posB = cpPosition(_controlPoints[conn.cpB], time);
        float dx = posB.x - posA.x;
        float dy = posB.y - posA.y;
        if (std::sqrt(dx * dx + dy * dy) > _config.maxConnectionDist) {
            _connections.erase(_connections.begin() + i);
        }
    }

    // Count connections per control point
    uint32_t cpCount = static_cast<uint32_t>(_controlPoints.size());
    std::vector<uint32_t> connCount(cpCount, 0);
    for (const auto& conn : _connections) {
        if (conn.cpA < cpCount) connCount[conn.cpA]++;
        if (conn.cpB < cpCount) connCount[conn.cpB]++;
    }

    // Connect under-connected control points to nearest neighbors
    for (uint32_t i = 0; i < cpCount; i++) {
        if (connCount[i] >= _config.targetConnsPerCP) continue;

        auto posI = cpPosition(_controlPoints[i], time);
        float bestDist = _config.maxConnectionDist;
        int bestJ = -1;

        for (uint32_t j = 0; j < cpCount; j++) {
            if (j == i) continue;
            if (connCount[j] >= _config.targetConnsPerCP) continue;

            bool already = false;
            for (const auto& conn : _connections) {
                if ((conn.cpA == i && conn.cpB == j) ||
                    (conn.cpA == j && conn.cpB == i)) {
                    already = true;
                    break;
                }
            }
            if (already) continue;

            auto posJ = cpPosition(_controlPoints[j], time);
            float dx = posJ.x - posI.x;
            float dy = posJ.y - posI.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < bestDist) {
                bestDist = dist;
                bestJ = static_cast<int>(j);
            }
        }

        if (bestJ >= 0) {
            Connection conn = {};
            conn.cpA = i;
            conn.cpB = static_cast<uint32_t>(bestJ);

            float r = std::uniform_real_distribution<float>(0.0f, 1.0f)(_rng);
            float segThresh = _config.curveRatio + (1.0f - _config.curveRatio) * 0.5f;
            if (r < _config.curveRatio) conn.type = 0;
            else if (r < segThresh) conn.type = 1;
            else conn.type = 2;

            conn.shapeChoice = std::uniform_int_distribution<int>(0, 42)(_rng);
            conn.curvature = std::uniform_real_distribution<float>(-0.8f, 0.8f)(_rng);
            conn.curveAnimPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
            conn.curveAnimFreq = std::uniform_real_distribution<float>(0.3f, 1.2f)(_rng);
            conn.strokeWidth = std::uniform_real_distribution<float>(_config.strokeMin, _config.strokeMax)(_rng);
            conn.color = blendColors(_controlPoints[i].color,
                                     _controlPoints[static_cast<uint32_t>(bestJ)].color);

            _connections.push_back(conn);
            connCount[i]++;
            connCount[static_cast<uint32_t>(bestJ)]++;
        }
    }
}

} // namespace yetty::zoo
