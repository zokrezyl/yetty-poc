#include "ydraw-zoo.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/ydraw-types.gen.h"
#include "../../ydraw/ydraw-buffer.h"
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>
#include <cmath>
#include <cstring>
#include <chrono>
#include <sstream>

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YDrawZoo::YDrawZoo(const YettyContext& ctx,
                   int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells,
                   const std::string& args)
    : _argsStr(args)
    , _screenId(ctx.screenId)
    , _cardMgr(ctx.cardManager)
    , _gpu(ctx.gpu)
    , _fontManager(ctx.fontManager)
    , _gpuAllocator(ctx.gpuAllocator)
    , _x(x), _y(y)
    , _widthCells(widthCells), _heightCells(heightCells)
{
    _shaderGlyph = SHADER_GLYPH;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _rng.seed(static_cast<uint32_t>(seed));

    _buffer = *YDrawBuffer::create();
}

YDrawZoo::~YDrawZoo() { dispose(); }

//=============================================================================
// Initialization
//=============================================================================

Result<void> YDrawZoo::init() {
    auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
    if (!metaResult) {
        return Err<void>("YDrawZoo::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;

    auto builderRes = YDrawBuilder::create(
        _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
    if (!builderRes) {
        return Err<void>("YDrawZoo::init: failed to create builder", builderRes);
    }
    _builder = *builderRes;

    parseArgs(_argsStr);

    _builder->setSceneBounds(0, 0, SCENE_W, SCENE_H);
    _builder->setBgColor(_bgColor);
    _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);
    _builder->setViewport(_widthCells, _heightCells);

    return Ok();
}

//=============================================================================
// Argument parsing
//=============================================================================

void YDrawZoo::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--points" || token == "-n") {
            uint32_t v; if (iss >> v) _targetPoints = std::max(2u, std::min(v, 100u));
        } else if (token == "--connections" || token == "-k") {
            uint32_t v; if (iss >> v) _targetConnsPerCP = std::max(1u, std::min(v, 10u));
        } else if (token == "--growth" || token == "-g") {
            float v; if (iss >> v) _growthRate = std::max(0.05f, std::min(v, 3.0f));
        } else if (token == "--max-dist") {
            float v; if (iss >> v) _maxConnectionDist = std::max(20.0f, std::min(v, 600.0f));
        } else if (token == "--marker-size") {
            float v; if (iss >> v) _cpMarkerSize = std::max(0.5f, std::min(v, 20.0f));
        } else if (token == "--stroke-min") {
            float v; if (iss >> v) _strokeMin = std::max(0.1f, std::min(v, 10.0f));
        } else if (token == "--stroke-max") {
            float v; if (iss >> v) _strokeMax = std::max(_strokeMin, std::min(v, 20.0f));
        } else if (token == "--curve-ratio") {
            float v; if (iss >> v) _curveRatio = std::max(0.0f, std::min(v, 1.0f));
        } else if (token == "--spawn-radius") {
            std::string s;
            if (iss >> s) {
                auto comma = s.find(',');
                if (comma != std::string::npos) {
                    try {
                        _spawnRadiusMin = std::stof(s.substr(0, comma));
                        _spawnRadiusMax = std::stof(s.substr(comma + 1));
                        if (_spawnRadiusMax < _spawnRadiusMin)
                            std::swap(_spawnRadiusMin, _spawnRadiusMax);
                    } catch (...) {}
                }
            }
        } else if (token == "--bg-color") {
            std::string s;
            if (iss >> s) {
                if (s.substr(0, 2) == "0x" || s.substr(0, 2) == "0X")
                    s = s.substr(2);
                try {
                    _bgColor = static_cast<uint32_t>(std::stoul(s, nullptr, 16));
                } catch (...) {}
            }
        }
    }
}

//=============================================================================
// Card lifecycle
//=============================================================================

bool YDrawZoo::needsBufferRealloc() {
    if (!_builder) return false;
    return _builder->needsBufferRealloc();
}

void YDrawZoo::renderToStaging(float time) {
    if (!_builder) return;

    // First frame: seed initial control points with staggered ages
    if (!_initialized) {
        _lastTime = time;
        for (uint32_t i = 0; i < _targetPoints; i++) {
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
        float scale = std::exp(_growthRate * age);
        float margin = _cpMarkerSize * scale * 2.0f;
        if (pos.x - margin > SCENE_W || pos.x + margin < 0 ||
            pos.y - margin > SCENE_H || pos.y + margin < 0) {
            removeControlPoint(static_cast<uint32_t>(i));
        }
    }

    // Spawn replacements
    while (_controlPoints.size() < _targetPoints) {
        spawnControlPoint(time);
    }

    // Update connections (remove broken, add new)
    updateConnections(time);

    // Generate prims directly into buffer
    _buffer->clear();
    uint32_t layer = 0;

    // 1. Connection prims (curves/lines/shapes) — lower layers
    for (const auto& conn : _connections) {
        if (conn.cpA >= _controlPoints.size() || conn.cpB >= _controlPoints.size())
            continue;

        auto posA = cpPosition(_controlPoints[conn.cpA], time);
        auto posB = cpPosition(_controlPoints[conn.cpB], time);

        // Animated curvature
        float curv = conn.curvature
                   + 0.15f * std::sin(time * conn.curveAnimFreq + conn.curveAnimPhase);

        // Breathing alpha
        float breathe = 0.6f + 0.4f * std::sin(time * 1.5f + conn.curveAnimPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 200.0f);
        uint32_t color = (conn.color & 0x00FFFFFFu)
                       | (static_cast<uint32_t>(alpha) << 24);

        switch (conn.type) {
        case 0: { // Bezier curve (dominant)
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

            _buffer->addBezier2(layer++, posA.x, posA.y, ctrlX, ctrlY, posB.x, posB.y, 0, color, conn.strokeWidth, 0.0f);
            break;
        }
        case 1: { // Straight segment
            _buffer->addSegment(layer++, posA.x, posA.y, posB.x, posB.y, 0, color, conn.strokeWidth, 0.0f);
            break;
        }
        case 2: { // Shape at midpoint
            float midX = (posA.x + posB.x) * 0.5f;
            float midY = (posA.y + posB.y) * 0.5f;
            float dx = posB.x - posA.x;
            float dy = posB.y - posA.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            float size = std::max(3.0f, dist * 0.12f);
            addShape(conn.shapeChoice, midX, midY, size, color, layer++);
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
        float scale = std::exp(_growthRate * age);
        float markerSize = std::min(_cpMarkerSize * scale, 12.0f);

        _buffer->addCircle(layer++, pos.x, pos.y, markerSize, color, 0, 0.0f, 0.0f);
    }

    _builder->calculate();
}

void YDrawZoo::declareBufferNeeds() {
    if (!_builder) return;
    _builder->declareBufferNeeds();
}

Result<void> YDrawZoo::allocateBuffers() {
    if (!_builder) return Ok();
    return _builder->allocateBuffers();
}

Result<void> YDrawZoo::finalize() {
    if (!_builder) return Ok();
    return _builder->writeBuffers();
}

Result<void> YDrawZoo::dispose() {
    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YDrawZoo::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
}

void YDrawZoo::suspend() {
}

//=============================================================================
// Random color generation (HSL with high saturation)
//=============================================================================

uint32_t YDrawZoo::randomColor() {
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
// Shape generation (uses YDrawBuffer typed addXxx methods)
//=============================================================================

void YDrawZoo::addShape(int choice, float cx, float cy, float size,
                         uint32_t color, uint32_t layer) {
    switch (choice) {
    case 0: _buffer->addCircle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 1: _buffer->addBox(layer, cx, cy, size, size * 0.8f, color, 0, 0.0f, size * 0.15f); break;
    case 2: _buffer->addSegment(layer, cx - size, cy, cx + size, cy, 0, color, size * 0.15f, 0.0f); break;
    case 3: _buffer->addTriangle(layer, cx, cy - size, cx - size, cy + size * 0.7f, cx + size, cy + size * 0.7f, color, 0, 0.0f, 0.0f); break;
    case 4: _buffer->addBezier2(layer, cx - size, cy, cx, cy - size, cx + size, cy, 0, color, size * 0.15f, 0.0f); break;
    case 5: _buffer->addEllipse(layer, cx, cy, size, size * 0.6f, color, 0, 0.0f, 0.0f); break;
    case 6: _buffer->addArc(layer, cx, cy, 0.866f, 0.5f, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 7: _buffer->addRoundedBox(layer, cx, cy, size, size * 0.7f, size * 0.2f, size * 0.2f, size * 0.2f, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 8: _buffer->addRhombus(layer, cx, cy, size, size * 1.4f, color, 0, 0.0f, 0.0f); break;
    case 9: _buffer->addPentagon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 10: _buffer->addHexagon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 11: _buffer->addStar(layer, cx, cy, size, 5.0f, 2.5f, color, 0, 0.0f, 0.0f); break;
    case 12: _buffer->addPie(layer, cx, cy, 0.866f, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 13: _buffer->addRing(layer, cx, cy, 0.866f, 0.5f, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 14: _buffer->addHeart(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 15: _buffer->addCross(layer, cx, cy, size, size * 0.3f, size * 0.1f, color, 0, 0.0f, 0.0f); break;
    case 16: _buffer->addRoundedX(layer, cx, cy, size, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 17: _buffer->addCapsule(layer, cx - size * 0.7f, cy, cx + size * 0.7f, cy, size * 0.3f, 0, color, size * 0.15f, 0.0f); break;
    case 18: _buffer->addMoon(layer, cx, cy, size * 0.5f, size, size * 0.8f, color, 0, 0.0f, 0.0f); break;
    case 19: _buffer->addEgg(layer, cx, cy, size, size * 0.6f, color, 0, 0.0f, 0.0f); break;
    case 20: _buffer->addChamferBox(layer, cx, cy, size, size * 0.8f, size * 0.2f, color, 0, 0.0f, 0.0f); break;
    case 21: _buffer->addOrientedBox(layer, cx - size, cy - size * 0.3f, cx + size, cy + size * 0.3f, size * 0.4f, color, 0, 0.0f, 0.0f); break;
    case 22: _buffer->addTrapezoid(layer, cx, cy, size * 0.8f, size * 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 23: _buffer->addParallelogram(layer, cx, cy, size, size * 0.6f, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 24: _buffer->addEquilateralTriangle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 25: _buffer->addIsoscelesTriangle(layer, cx, cy, size * 0.7f, size * 1.2f, color, 0, 0.0f, 0.0f); break;
    case 26: _buffer->addUnevenCapsule(layer, cx, cy, size * 0.5f, size * 0.3f, size * 1.2f, color, 0, 0.0f, 0.0f); break;
    case 27: _buffer->addOctogon(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 28: _buffer->addHexagram(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 29: _buffer->addPentagram(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 30: _buffer->addCutDisk(layer, cx, cy, size, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 31: _buffer->addHorseshoe(layer, cx, cy, 0.9636f, 0.2675f, size * 0.8f, size * 0.3f, size * 0.15f, color, 0, 0.0f, 0.0f); break;
    case 32: _buffer->addVesica(layer, cx, cy, size * 0.6f, size, color, 0, 0.0f, 0.0f); break;
    case 33: _buffer->addOrientedVesica(layer, cx - size * 0.5f, cy, cx + size * 0.5f, cy, size * 0.3f, color, 0, 0.0f, 0.0f); break;
    case 34: _buffer->addRoundedCross(layer, cx, cy, size * 0.5f, color, 0, 0.0f, 0.0f); break;
    case 35: _buffer->addParabola(layer, cx, cy, 0.5f, color, 0, 0.0f, 0.0f); break;
    case 36: _buffer->addBlobbyCross(layer, cx, cy, size * 0.5f, color, 0, 0.0f, 0.0f); break;
    case 37: _buffer->addTunnel(layer, cx, cy, size, size * 0.7f, color, 0, 0.0f, 0.0f); break;
    case 38: _buffer->addStairs(layer, cx, cy, size * 0.3f, size * 0.2f, 5.0f, color, 0, 0.0f, 0.0f); break;
    case 39: _buffer->addQuadraticCircle(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 40: _buffer->addHyperbola(layer, cx, cy, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    case 41: _buffer->addCoolS(layer, cx, cy, size, color, 0, 0.0f, 0.0f); break;
    case 42: _buffer->addCircleWave(layer, cx, cy, 0.5f, size, color, 0, 0.0f, 0.0f); break;
    }
}

//=============================================================================
// Control point + connection lifecycle
//=============================================================================

YDrawZoo::Vec2f YDrawZoo::cpPosition(const ControlPoint& cp, float time) const {
    float age = time - cp.spawnTime;
    if (age < 0.0f) age = 0.0f;
    float scale = std::exp(_growthRate * age);
    float dist = cp.baseRadius * scale;
    return { CENTER_X + dist * std::cos(cp.angle),
             CENTER_Y + dist * std::sin(cp.angle) };
}

uint32_t YDrawZoo::blendColors(uint32_t a, uint32_t b) {
    uint32_t r = ((a & 0xFF) + (b & 0xFF)) / 2;
    uint32_t g = (((a >> 8) & 0xFF) + ((b >> 8) & 0xFF)) / 2;
    uint32_t bl = (((a >> 16) & 0xFF) + ((b >> 16) & 0xFF)) / 2;
    return 0xFF000000u | (bl << 16) | (g << 8) | r;
}

void YDrawZoo::spawnControlPoint(float time) {
    ControlPoint cp = {};
    cp.angle = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    cp.spawnTime = time;
    cp.baseRadius = std::uniform_real_distribution<float>(_spawnRadiusMin, _spawnRadiusMax)(_rng);
    cp.animPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    cp.animFreq = std::uniform_real_distribution<float>(0.8f, 2.5f)(_rng);
    cp.color = randomColor();
    _controlPoints.push_back(cp);
}

void YDrawZoo::removeControlPoint(uint32_t idx) {
    // Remove connections referencing this control point
    for (int i = static_cast<int>(_connections.size()) - 1; i >= 0; i--) {
        if (_connections[i].cpA == idx || _connections[i].cpB == idx) {
            _connections.erase(_connections.begin() + i);
        }
    }
    // Fix indices in remaining connections
    for (auto& conn : _connections) {
        if (conn.cpA > idx) conn.cpA--;
        if (conn.cpB > idx) conn.cpB--;
    }
    _controlPoints.erase(_controlPoints.begin() + idx);
}

void YDrawZoo::updateConnections(float time) {
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
        if (std::sqrt(dx * dx + dy * dy) > _maxConnectionDist) {
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
        if (connCount[i] >= _targetConnsPerCP) continue;

        auto posI = cpPosition(_controlPoints[i], time);

        float bestDist = _maxConnectionDist;
        int bestJ = -1;

        for (uint32_t j = 0; j < cpCount; j++) {
            if (j == i) continue;
            if (connCount[j] >= _targetConnsPerCP) continue;

            // Check if already connected
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

            // Type distribution: curves dominate based on _curveRatio
            float r = std::uniform_real_distribution<float>(0.0f, 1.0f)(_rng);
            float segThresh = _curveRatio + (1.0f - _curveRatio) * 0.5f;
            if (r < _curveRatio) conn.type = 0;       // bezier
            else if (r < segThresh) conn.type = 1;    // segment
            else conn.type = 2;                        // shape

            conn.shapeChoice = std::uniform_int_distribution<int>(0, 42)(_rng);
            conn.curvature = std::uniform_real_distribution<float>(-0.8f, 0.8f)(_rng);
            conn.curveAnimPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
            conn.curveAnimFreq = std::uniform_real_distribution<float>(0.3f, 1.2f)(_rng);
            conn.strokeWidth = std::uniform_real_distribution<float>(_strokeMin, _strokeMax)(_rng);
            conn.color = blendColors(_controlPoints[i].color,
                                     _controlPoints[static_cast<uint32_t>(bestJ)].color);

            _connections.push_back(conn);
            connCount[i]++;
            connCount[static_cast<uint32_t>(bestJ)]++;
        }
    }
}

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YDrawZoo::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    (void)payload;

    yinfo("YDrawZoo::create: pos=({},{}) size={}x{} args='{}'", x, y, widthCells, heightCells, args);

    if (!ctx.cardManager) {
        return Err<CardPtr>("YDrawZoo::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDrawZoo>(ctx, x, y, widthCells, heightCells, args);

    if (auto res = card->init(); !res) {
        yerror("YDrawZoo::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YDrawZoo::create: init failed");
    }

    yinfo("YDrawZoo::create: SUCCESS");
    return Ok<CardPtr>(card);
}

Result<YDrawZoo::Ptr> YDrawZoo::createImpl(
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
        return Err<Ptr>("Failed to create YDrawZoo", result);
    }
    auto zoo = std::dynamic_pointer_cast<YDrawZoo>(*result);
    if (!zoo) {
        return Err<Ptr>("Created card is not a YDrawZoo");
    }
    return Ok(zoo);
}

} // namespace yetty::card
