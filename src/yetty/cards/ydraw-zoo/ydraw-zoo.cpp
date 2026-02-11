#include "ydraw-zoo.h"
#include "../../ydraw/ydraw-builder.h"
#include "../hdraw/hdraw.h"  // SDFPrimitive, SDFType
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>
#include <cmath>
#include <cstring>
#include <chrono>

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YDrawZoo::YDrawZoo(const YettyContext& ctx,
                   int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells)
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
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

    _builder->setSceneBounds(0, 0, SCENE_W, SCENE_H);
    _builder->setBgColor(0xFF1A1A2E);
    _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE);

    _dirty = true;
    _metadataDirty = true;
    return Ok();
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

    // First frame: seed initial objects with staggered ages
    if (!_initialized) {
        _lastTime = time;
        for (uint32_t i = 0; i < TARGET_OBJECTS; i++) {
            spawnObject(time - std::uniform_real_distribution<float>(0.0f, 3.0f)(_rng));
        }
        _initialized = true;
    }

    _lastTime = time;

    // Cull objects that have grown outside the scene
    updateObjects(time);

    // Spawn replacements for removed objects
    while (_objects.size() < TARGET_OBJECTS) {
        spawnObject(time);
    }

    // Generate prims from current object state
    _primBuffer.clear();
    for (uint32_t i = 0; i < static_cast<uint32_t>(_objects.size()); i++) {
        auto& obj = _objects[i];
        float age = time - obj.spawnTime;
        if (age < 0.0f) age = 0.0f;

        float scale = std::exp(GROWTH_RATE * age);
        float size = SPAWN_SIZE * scale;
        float dist = obj.baseRadius * scale;

        float cx = CENTER_X + dist * std::cos(obj.angle);
        float cy = CENTER_Y + dist * std::sin(obj.angle);

        // Decorative: opacity breathing
        float breathe = 0.7f + 0.3f * std::sin(time * obj.animFreq + obj.animPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 255.0f);
        uint32_t color = (obj.baseColor & 0x00FFFFFFu)
                       | (static_cast<uint32_t>(alpha) << 24);

        SDFPrimitive prim = makeShape(obj.shapeChoice, cx, cy, size, color, i);
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

SDFPrimitive YDrawZoo::make3DShape(int choice, float cx, float cy, float size,
                                    uint32_t color, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.layer = layer;
    prim.fillColor = color;
    prim.strokeColor = 0;
    prim.strokeWidth = 0;

    float x3d = scene2Dx(cx);
    float y3d = scene2Dy(cy);
    float s3d = scene2Dsize(size);

    switch (choice) {
    case 0: // Sphere
        prim.type = static_cast<uint32_t>(SDFType::Sphere3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d;
        break;
    case 1: // Box3D
        prim.type = static_cast<uint32_t>(SDFType::Box3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d;
        prim.params[4] = s3d * 0.8f;
        prim.params[5] = s3d * 0.6f;
        break;
    case 2: // Torus
        prim.type = static_cast<uint32_t>(SDFType::Torus3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d * 0.8f;
        prim.params[4] = s3d * 0.3f;
        break;
    case 3: // Octahedron
        prim.type = static_cast<uint32_t>(SDFType::Octahedron3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d;
        break;
    case 4: // Pyramid
        prim.type = static_cast<uint32_t>(SDFType::Pyramid3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d * 1.5f;
        break;
    case 5: // Cylinder
        prim.type = static_cast<uint32_t>(SDFType::Cylinder3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d * 0.7f;
        prim.params[4] = s3d;
        break;
    }

    return prim;
}

void YDrawZoo::update3DPrim(SDFPrimitive& prim, float cx, float cy, float size) {
    float x3d = scene2Dx(cx);
    float y3d = scene2Dy(cy);
    float s3d = scene2Dsize(size);

    prim.params[0] = x3d;
    prim.params[1] = y3d;

    auto sdfType = static_cast<SDFType>(prim.type);
    switch (sdfType) {
    case SDFType::Sphere3D:
    case SDFType::Octahedron3D:
        prim.params[3] = s3d;
        break;
    case SDFType::Box3D:
        prim.params[3] = s3d;
        prim.params[4] = s3d * 0.8f;
        prim.params[5] = s3d * 0.6f;
        break;
    case SDFType::Torus3D:
        prim.params[3] = s3d * 0.8f;
        prim.params[4] = s3d * 0.3f;
        break;
    case SDFType::Pyramid3D:
        prim.params[3] = s3d * 1.5f;
        break;
    case SDFType::Cylinder3D:
        prim.params[3] = s3d * 0.7f;
        prim.params[4] = s3d;
        break;
    default:
        break;
    }
}

//=============================================================================
// Object lifecycle — operates on _primBuffer (CPU staging), not GPU
//=============================================================================

void YDrawZoo::spawnObject(float time) {
    ZooObject obj = {};
    obj.shapeChoice = std::uniform_int_distribution<int>(0, 42)(_rng);
    obj.angle = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    obj.spawnTime = time;
    obj.baseRadius = std::uniform_real_distribution<float>(SPAWN_RADIUS_MIN, SPAWN_RADIUS_MAX)(_rng);
    obj.animPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    obj.animFreq = std::uniform_real_distribution<float>(0.8f, 2.5f)(_rng);
    obj.baseColor = randomColor();

    _objects.push_back(obj);
}

void YDrawZoo::updateObjects(float time) {
    // Cull objects that have grown outside the scene
    for (int i = static_cast<int>(_objects.size()) - 1; i >= 0; i--) {
        auto& obj = _objects[i];
        float age = time - obj.spawnTime;
        if (age < 0.0f) age = 0.0f;

        float scale = std::exp(GROWTH_RATE * age);
        float size = SPAWN_SIZE * scale;
        float dist = obj.baseRadius * scale;

        float cx = CENTER_X + dist * std::cos(obj.angle);
        float cy = CENTER_Y + dist * std::sin(obj.angle);

        float margin = size * 2.0f;
        if (cx - margin > SCENE_W || cx + margin < 0 ||
            cy - margin > SCENE_H || cy + margin < 0 ||
            size > SCENE_W) {
            _objects.erase(_objects.begin() + i);
        }
    }
}

void YDrawZoo::removeObject(uint32_t idx) {
    if (idx >= _objects.size()) return;
    _objects.erase(_objects.begin() + idx);
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
    (void)args;
    (void)payload;

    yinfo("YDrawZoo::create: pos=({},{}) size={}x{}", x, y, widthCells, heightCells);

    if (!ctx.cardManager) {
        return Err<CardPtr>("YDrawZoo::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDrawZoo>(ctx, x, y, widthCells, heightCells);

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
