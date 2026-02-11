#include "ydraw-zoo.h"
#include "../../ydraw/ydraw-builder.h"
#include "../hdraw/hdraw.h"  // SDFPrimitive, SDFType
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
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _screenId(ctx.screenId)
{
    _shaderGlyph = SHADER_GLYPH;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _rng.seed(static_cast<uint32_t>(seed));

    auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
    if (builderRes) {
        _builder = *builderRes;
    } else {
        yerror("YDrawZoo: failed to create builder");
    }
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

    parseArgs(_argsStr);

    _builder->setSceneBounds(0, 0, SCENE_W, SCENE_H);
    _builder->setBgColor(_bgColor);
    _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);

    _dirty = true;
    _metadataDirty = true;
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
    if (!_primStorage.isValid()) return true;
    if (_primBuffer.size() > _primCapacity) return true;
    if (!_builder) return false;
    uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
    if (gridBytes > 0 && (!_derivedStorage.isValid() || gridBytes > _derivedStorage.size))
        return true;
    return false;
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

    // Generate prims
    _primBuffer.clear();
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

        SDFPrimitive prim = {};

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

            prim.type = static_cast<uint32_t>(SDFType::Bezier2);
            prim.params[0] = posA.x;
            prim.params[1] = posA.y;
            prim.params[2] = ctrlX;
            prim.params[3] = ctrlY;
            prim.params[4] = posB.x;
            prim.params[5] = posB.y;
            prim.fillColor = 0;
            prim.strokeColor = color;
            prim.strokeWidth = conn.strokeWidth;
            prim.layer = layer++;
            break;
        }
        case 1: { // Straight segment
            prim.type = static_cast<uint32_t>(SDFType::Segment);
            prim.params[0] = posA.x;
            prim.params[1] = posA.y;
            prim.params[2] = posB.x;
            prim.params[3] = posB.y;
            prim.fillColor = 0;
            prim.strokeColor = color;
            prim.strokeWidth = conn.strokeWidth;
            prim.layer = layer++;
            break;
        }
        case 2: { // Shape at midpoint
            float midX = (posA.x + posB.x) * 0.5f;
            float midY = (posA.y + posB.y) * 0.5f;
            float dx = posB.x - posA.x;
            float dy = posB.y - posA.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            float size = std::max(3.0f, dist * 0.12f);
            prim = makeShape(conn.shapeChoice, midX, midY, size, color, layer++);
            break;
        }
        }

        YDrawBuilder::recomputeAABB(prim);
        _primBuffer.push_back(prim);
    }

    // 2. Control point markers (small circles, higher layers)
    for (uint32_t i = 0; i < static_cast<uint32_t>(_controlPoints.size()); i++) {
        auto& cp = _controlPoints[i];
        auto pos = cpPosition(cp, time);

        float breathe = 0.7f + 0.3f * std::sin(time * cp.animFreq + cp.animPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 255.0f);
        uint32_t color = (cp.color & 0x00FFFFFFu)
                       | (static_cast<uint32_t>(alpha) << 24);

        // Scale marker with zoom effect
        float age = time - cp.spawnTime;
        if (age < 0.0f) age = 0.0f;
        float scale = std::exp(_growthRate * age);
        float markerSize = std::min(_cpMarkerSize * scale, 12.0f);

        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.params[0] = pos.x;
        prim.params[1] = pos.y;
        prim.params[2] = markerSize;
        prim.fillColor = color;
        prim.strokeColor = 0;
        prim.strokeWidth = 0;
        prim.layer = layer++;

        YDrawBuilder::recomputeAABB(prim);
        _primBuffer.push_back(prim);
    }

    // Feed to builder staging and compute grid
    _builder->clear();
    for (const auto& prim : _primBuffer) {
        _builder->addPrimitive(prim);
    }
    _builder->calculate();

    _dirty = true;
}

void YDrawZoo::declareBufferNeeds() {
    if (!_builder) return;

    // Release current GPU storage
    _derivedStorage = StorageHandle::invalid();
    _grid = nullptr;
    _gridSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;

    const auto& primStaging = _builder->primStaging();
    if (primStaging.empty()) {
        _builder->clearGridStaging();
        return;
    }

    // Reserve prim storage
    uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
    _cardMgr->bufferManager()->reserve(primSize);

    // Grid is already computed in renderToStaging via builder->calculate()
    // Reserve with 2x headroom since grid size can vary frame-to-frame
    uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
    if (gridBytes > 0) {
        _cardMgr->bufferManager()->reserve(gridBytes * 2);
    }
}

Result<void> YDrawZoo::allocateBuffers() {
    if (!_builder) return Ok();

    const auto& primStaging = _builder->primStaging();
    const auto& gridStaging = _builder->gridStaging();

    // Allocate and copy primitives
    if (!primStaging.empty()) {
        uint32_t count = static_cast<uint32_t>(primStaging.size());
        uint32_t allocBytes = count * sizeof(SDFPrimitive);
        auto primResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "prims", allocBytes);
        if (!primResult) {
            return Err<void>("YDrawZoo::allocateBuffers: prim alloc failed");
        }
        _primStorage = *primResult;
        _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
        _primCapacity = count;
        _primCount = count;
        std::memcpy(_primitives, primStaging.data(), count * sizeof(SDFPrimitive));
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }

    // Allocate and copy grid (with 2x headroom matching reserve)
    uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
    if (gridBytes > 0) {
        uint32_t allocSize = gridBytes * 2;
        auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "derived", allocSize);
        if (!storageResult) {
            return Err<void>("YDrawZoo::allocateBuffers: derived alloc failed");
        }
        _derivedStorage = *storageResult;

        uint8_t* base = _derivedStorage.data;
        std::memset(base, 0, _derivedStorage.size);
        _grid = reinterpret_cast<uint32_t*>(base);
        _gridSize = static_cast<uint32_t>(gridStaging.size());
        _gridOffset = _derivedStorage.offset / sizeof(float);
        std::memcpy(base, gridStaging.data(), gridBytes);
        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
    _gridWidth = _builder->gridWidth();
    _gridHeight = _builder->gridHeight();
    _metadataDirty = true;
    _dirty = false;  // allocation already copied staging

    return Ok();
}

Result<void> YDrawZoo::render() {
    if (!_builder) return Ok();

    if (_dirty) {
        // Copy prims to GPU
        if (_primitives && !_primBuffer.empty()) {
            uint32_t count = std::min(static_cast<uint32_t>(_primBuffer.size()), _primCapacity);
            std::memcpy(_primitives, _primBuffer.data(), count * sizeof(SDFPrimitive));
            _primCount = count;
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        // Copy pre-computed grid to GPU
        if (_derivedStorage.isValid()) {
            const auto& gridData = _builder->gridStaging();
            uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
            if (gridBytes > 0 && gridBytes <= _derivedStorage.size) {
                uint8_t* base = _derivedStorage.data;
                std::memset(base, 0, _derivedStorage.size);
                std::memcpy(base, gridData.data(), gridBytes);
                _grid = reinterpret_cast<uint32_t*>(base);
                _gridSize = static_cast<uint32_t>(gridData.size());
                _gridWidth = _builder->gridWidth();
                _gridHeight = _builder->gridHeight();
                _gridOffset = _derivedStorage.offset / sizeof(float);
                _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
            }
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        _metadataDirty = true;
        _dirty = false;
    }

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) return res;
        _metadataDirty = false;
    }

    return Ok();
}

Result<void> YDrawZoo::dispose() {
    _derivedStorage = StorageHandle::invalid();
    _grid = nullptr;
    _gridSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;
    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YDrawZoo::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
}

void YDrawZoo::suspend() {
    _derivedStorage = StorageHandle::invalid();
    _grid = nullptr;
    _gridSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;
}

Result<void> YDrawZoo::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YDrawZoo::uploadMetadata: invalid handle");
    }

    float cellSize = _builder->cellSize();

    YDrawMetadata meta = {};
    meta.primitiveOffset = _primitiveOffset;
    meta.primitiveCount = _primCount;
    meta.gridOffset = _gridOffset;
    meta.gridWidth = _gridWidth;
    meta.gridHeight = _gridHeight;
    std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
    meta.glyphOffset = 0;
    meta.glyphCount = 0;
    float sceneMinX = _builder->sceneMinX();
    float sceneMinY = _builder->sceneMinY();
    float sceneMaxX = _builder->sceneMaxX();
    float sceneMaxY = _builder->sceneMaxY();
    std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
    meta.widthCells = _widthCells & 0xFFFF;
    meta.heightCells = _heightCells & 0xFFFF;
    // zoom = 1.0f → f16 = 0x3C00
    meta.flags = (_builder->flags() & 0xFFFF) | (0x3C00u << 16);
    meta.bgColor = _builder->bgColor();

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YDrawZoo::uploadMetadata: write failed");
    }
    return Ok();
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
// Shape generation (unchanged — creates SDFPrimitive structs)
//=============================================================================

SDFPrimitive YDrawZoo::makeShape(int choice, float cx, float cy, float size,
                                  uint32_t color, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.layer = layer;
    prim.fillColor = color;
    prim.strokeColor = 0;
    prim.strokeWidth = 0;

    switch (choice) {
    case 0: // Circle
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 1: // Box
        prim.type = static_cast<uint32_t>(SDFType::Box);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.8f;
        prim.round = size * 0.15f;
        break;
    case 2: // Segment (stroke only)
        prim.type = static_cast<uint32_t>(SDFType::Segment);
        prim.params[0] = cx - size;
        prim.params[1] = cy;
        prim.params[2] = cx + size;
        prim.params[3] = cy;
        prim.fillColor = 0;
        prim.strokeColor = color;
        prim.strokeWidth = size * 0.15f;
        break;
    case 3: // Triangle
        prim.type = static_cast<uint32_t>(SDFType::Triangle);
        prim.params[0] = cx;
        prim.params[1] = cy - size;
        prim.params[2] = cx - size;
        prim.params[3] = cy + size * 0.7f;
        prim.params[4] = cx + size;
        prim.params[5] = cy + size * 0.7f;
        break;
    case 4: // Bezier2 (stroke only)
        prim.type = static_cast<uint32_t>(SDFType::Bezier2);
        prim.params[0] = cx - size;
        prim.params[1] = cy;
        prim.params[2] = cx;
        prim.params[3] = cy - size;
        prim.params[4] = cx + size;
        prim.params[5] = cy;
        prim.fillColor = 0;
        prim.strokeColor = color;
        prim.strokeWidth = size * 0.15f;
        break;
    case 5: // Ellipse (type 6)
        prim.type = static_cast<uint32_t>(SDFType::Ellipse);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.6f;
        break;
    case 6: // Arc (type 7)
        prim.type = static_cast<uint32_t>(SDFType::Arc);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.866f;  // sin(60)
        prim.params[3] = 0.5f;    // cos(60)
        prim.params[4] = size;
        prim.params[5] = size * 0.2f;
        break;
    case 7: // RoundedBox (type 8)
        prim.type = static_cast<uint32_t>(SDFType::RoundedBox);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.7f;
        prim.params[4] = size * 0.2f;
        prim.params[5] = size * 0.2f;
        prim.params[6] = size * 0.2f;
        prim.params[7] = size * 0.2f;
        break;
    case 8: // Rhombus (type 9)
        prim.type = static_cast<uint32_t>(SDFType::Rhombus);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 1.4f;
        break;
    case 9: // Pentagon (type 10)
        prim.type = static_cast<uint32_t>(SDFType::Pentagon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 10: // Hexagon (type 11)
        prim.type = static_cast<uint32_t>(SDFType::Hexagon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 11: // Star (type 12)
        prim.type = static_cast<uint32_t>(SDFType::Star);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = 5.0f;
        prim.params[4] = 2.5f;
        break;
    case 12: // Pie (type 13)
        prim.type = static_cast<uint32_t>(SDFType::Pie);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.866f;  // sin(60)
        prim.params[3] = 0.5f;    // cos(60)
        prim.params[4] = size;
        break;
    case 13: // Ring (type 14)
        prim.type = static_cast<uint32_t>(SDFType::Ring);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.866f;
        prim.params[3] = 0.5f;
        prim.params[4] = size;
        prim.params[5] = size * 0.2f;
        break;
    case 14: // Heart (type 15)
        prim.type = static_cast<uint32_t>(SDFType::Heart);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 15: // Cross (type 16)
        prim.type = static_cast<uint32_t>(SDFType::Cross);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.3f;
        prim.params[4] = size * 0.1f;
        break;
    case 16: // RoundedX (type 17)
        prim.type = static_cast<uint32_t>(SDFType::RoundedX);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.2f;
        break;
    case 17: // Capsule (type 18, stroke only)
        prim.type = static_cast<uint32_t>(SDFType::Capsule);
        prim.params[0] = cx - size * 0.7f;
        prim.params[1] = cy;
        prim.params[2] = cx + size * 0.7f;
        prim.params[3] = cy;
        prim.params[4] = size * 0.3f;
        prim.fillColor = 0;
        prim.strokeColor = color;
        prim.strokeWidth = size * 0.15f;
        break;
    case 18: // Moon (type 19)
        prim.type = static_cast<uint32_t>(SDFType::Moon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.5f;
        prim.params[3] = size;
        prim.params[4] = size * 0.8f;
        break;
    case 19: // Egg (type 20)
        prim.type = static_cast<uint32_t>(SDFType::Egg);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.6f;
        break;
    case 20: // ChamferBox (type 21)
        prim.type = static_cast<uint32_t>(SDFType::ChamferBox);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.8f;
        prim.params[4] = size * 0.2f;
        break;
    case 21: // OrientedBox (type 22)
        prim.type = static_cast<uint32_t>(SDFType::OrientedBox);
        prim.params[0] = cx - size;
        prim.params[1] = cy - size * 0.3f;
        prim.params[2] = cx + size;
        prim.params[3] = cy + size * 0.3f;
        prim.params[4] = size * 0.4f;
        break;
    case 22: // Trapezoid (type 23)
        prim.type = static_cast<uint32_t>(SDFType::Trapezoid);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.8f;
        prim.params[3] = size * 0.5f;
        prim.params[4] = size;
        break;
    case 23: // Parallelogram (type 24)
        prim.type = static_cast<uint32_t>(SDFType::Parallelogram);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.6f;
        prim.params[4] = size * 0.3f;
        break;
    case 24: // EquilateralTriangle (type 25)
        prim.type = static_cast<uint32_t>(SDFType::EquilateralTriangle);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 25: // IsoscelesTriangle (type 26)
        prim.type = static_cast<uint32_t>(SDFType::IsoscelesTriangle);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.7f;
        prim.params[3] = size * 1.2f;
        break;
    case 26: // UnevenCapsule (type 27)
        prim.type = static_cast<uint32_t>(SDFType::UnevenCapsule);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.5f;
        prim.params[3] = size * 0.3f;
        prim.params[4] = size * 1.2f;
        break;
    case 27: // Octogon (type 28)
        prim.type = static_cast<uint32_t>(SDFType::Octogon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 28: // Hexagram (type 29)
        prim.type = static_cast<uint32_t>(SDFType::Hexagram);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 29: // Pentagram (type 30)
        prim.type = static_cast<uint32_t>(SDFType::Pentagram);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 30: // CutDisk (type 31)
        prim.type = static_cast<uint32_t>(SDFType::CutDisk);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.3f;
        break;
    case 31: // Horseshoe (type 32)
        prim.type = static_cast<uint32_t>(SDFType::Horseshoe);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.9636f;  // sin(1.3)
        prim.params[3] = 0.2675f;  // cos(1.3)
        prim.params[4] = size * 0.8f;
        prim.params[5] = size * 0.3f;
        prim.params[6] = size * 0.15f;
        break;
    case 32: // Vesica (type 33)
        prim.type = static_cast<uint32_t>(SDFType::Vesica);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.6f;
        prim.params[3] = size;
        break;
    case 33: // OrientedVesica (type 34)
        prim.type = static_cast<uint32_t>(SDFType::OrientedVesica);
        prim.params[0] = cx - size * 0.5f;
        prim.params[1] = cy;
        prim.params[2] = cx + size * 0.5f;
        prim.params[3] = cy;
        prim.params[4] = size * 0.3f;
        break;
    case 34: // RoundedCross (type 35)
        prim.type = static_cast<uint32_t>(SDFType::RoundedCross);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.5f;
        break;
    case 35: // Parabola (type 36)
        prim.type = static_cast<uint32_t>(SDFType::Parabola);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.5f;
        break;
    case 36: // BlobbyCross (type 37)
        prim.type = static_cast<uint32_t>(SDFType::BlobbyCross);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.5f;
        break;
    case 37: // Tunnel (type 38)
        prim.type = static_cast<uint32_t>(SDFType::Tunnel);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.7f;
        break;
    case 38: // Stairs (type 39)
        prim.type = static_cast<uint32_t>(SDFType::Stairs);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size * 0.3f;
        prim.params[3] = size * 0.2f;
        prim.params[4] = 5.0f;
        break;
    case 39: // QuadraticCircle (type 40)
        prim.type = static_cast<uint32_t>(SDFType::QuadraticCircle);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 40: // Hyperbola (type 41)
        prim.type = static_cast<uint32_t>(SDFType::Hyperbola);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.5f;
        prim.params[3] = size;
        break;
    case 41: // CoolS (type 42)
        prim.type = static_cast<uint32_t>(SDFType::CoolS);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 42: // CircleWave (type 43)
        prim.type = static_cast<uint32_t>(SDFType::CircleWave);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = 0.5f;
        prim.params[3] = size;
        break;
    }

    return prim;
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
