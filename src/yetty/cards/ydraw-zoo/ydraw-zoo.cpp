#include "ydraw-zoo.h"
#include "../hdraw/hdraw.h"  // SDFPrimitive, SDFType
#include <ytrace/ytrace.hpp>
#include <cmath>
#include <chrono>

namespace yetty::card {

//=============================================================================
// Constructor
//=============================================================================

YDrawZoo::YDrawZoo(const YettyContext& ctx,
                   int32_t x, int32_t y,
                   uint32_t widthCells, uint32_t heightCells)
    : YDrawBase(ctx, x, y, widthCells, heightCells)
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    _rng.seed(static_cast<uint32_t>(seed));
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YDrawZoo::init() {
    if (auto res = initBase(); !res) {
        return res;
    }

    // Fixed scene bounds — viewport never shifts
    setSceneBounds(0, 0, SCENE_W, SCENE_H);

    // Dark background
    setBgColor(0xFF1A1A2E);  // ABGR: dark navy

    markDirty();
    return Ok();
}

//=============================================================================
// Render — procedural update each frame
//=============================================================================

Result<void> YDrawZoo::render(float time) {
    // First frame: seed initial objects with staggered ages
    if (!_initialized) {
        _lastTime = time;
        for (uint32_t i = 0; i < TARGET_OBJECTS; i++) {
            spawnObject(time - std::uniform_real_distribution<float>(0.0f, 3.0f)(_rng));
        }
        _initialized = true;
    }

    _lastTime = time;

    updateObjects(time);

    // Spawn replacements for removed objects
    while (_objects.size() < TARGET_OBJECTS) {
        spawnObject(time);
    }

    return YDrawBase::render(time);
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
// Shape generation
//=============================================================================

SDFPrimitive YDrawZoo::makeRandomShape(float cx, float cy, float size,
                                        uint32_t color, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.layer = layer;
    prim.fillColor = color;
    prim.strokeColor = 0;
    prim.strokeWidth = 0;

    int choice = std::uniform_int_distribution<int>(0, 7)(_rng);

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
    case 2: // Hexagon
        prim.type = static_cast<uint32_t>(SDFType::Hexagon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 3: // Pentagon
        prim.type = static_cast<uint32_t>(SDFType::Pentagon);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 4: // Star
        prim.type = static_cast<uint32_t>(SDFType::Star);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = 5.0f;
        prim.params[4] = 2.5f;
        break;
    case 5: // Heart
        prim.type = static_cast<uint32_t>(SDFType::Heart);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        break;
    case 6: // Egg
        prim.type = static_cast<uint32_t>(SDFType::Egg);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 0.6f;
        break;
    case 7: // Rhombus
        prim.type = static_cast<uint32_t>(SDFType::Rhombus);
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = size;
        prim.params[3] = size * 1.4f;
        break;
    }

    return prim;
}

//=============================================================================
// Object lifecycle
//=============================================================================

void YDrawZoo::spawnObject(float time) {
    float angle = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    float dist = std::uniform_real_distribution<float>(SPAWN_RADIUS_MIN, SPAWN_RADIUS_MAX)(_rng);
    float cx = CENTER_X + dist * std::cos(angle);
    float cy = CENTER_Y + dist * std::sin(angle);

    uint32_t color = randomColor();
    uint32_t layer = primitiveCount();

    SDFPrimitive prim = makeRandomShape(cx, cy, SPAWN_SIZE, color, layer);
    recomputeAABB(prim);
    uint32_t idx = addPrimitive(prim);

    ZooObject obj = {};
    obj.primIndex = idx;
    obj.angle = angle;
    obj.spawnTime = time;
    obj.baseRadius = dist;
    obj.animPhase = std::uniform_real_distribution<float>(0.0f, 6.2831853f)(_rng);
    obj.animFreq = std::uniform_real_distribution<float>(0.8f, 2.5f)(_rng);
    obj.baseColor = color;
    obj.active = true;

    _objects.push_back(obj);
}

void YDrawZoo::updateObjects(float time) {
    bool anyChanged = false;

    for (int i = static_cast<int>(_objects.size()) - 1; i >= 0; i--) {
        auto& obj = _objects[i];
        if (!obj.active) continue;

        float age = time - obj.spawnTime;
        if (age < 0.0f) age = 0.0f;

        // Exponential growth
        float scale = std::exp(GROWTH_RATE * age);
        float size = SPAWN_SIZE * scale;
        float dist = obj.baseRadius * scale;

        float cx = CENTER_X + dist * std::cos(obj.angle);
        float cy = CENTER_Y + dist * std::sin(obj.angle);

        // Cull if fully outside scene
        float margin = size * 2.0f;
        if (cx - margin > SCENE_W || cx + margin < 0 ||
            cy - margin > SCENE_H || cy + margin < 0 ||
            size > SCENE_W) {
            removeObject(static_cast<uint32_t>(i));
            continue;
        }

        // Decorative: opacity breathing
        float breathe = 0.7f + 0.3f * std::sin(time * obj.animFreq + obj.animPhase);
        uint8_t alpha = static_cast<uint8_t>(breathe * 255.0f);
        uint32_t animatedColor = (obj.baseColor & 0x00FFFFFFu)
                               | (static_cast<uint32_t>(alpha) << 24);

        if (obj.primIndex < primitiveCount()) {
            auto& prim = primitivePtr()[obj.primIndex];

            prim.params[0] = cx;
            prim.params[1] = cy;

            auto sdfType = static_cast<SDFType>(prim.type);
            switch (sdfType) {
            case SDFType::Circle:
                prim.params[2] = size;
                break;
            case SDFType::Box:
                prim.params[2] = size;
                prim.params[3] = size * 0.8f;
                prim.round = size * 0.15f;
                break;
            case SDFType::Hexagon:
            case SDFType::Pentagon:
            case SDFType::Star:
            case SDFType::Heart:
                prim.params[2] = size;
                break;
            case SDFType::Egg:
                prim.params[2] = size;
                prim.params[3] = size * 0.6f;
                break;
            case SDFType::Rhombus:
                prim.params[2] = size;
                prim.params[3] = size * 1.4f;
                break;
            default:
                break;
            }

            prim.fillColor = animatedColor;
            recomputeAABB(prim);
            anyChanged = true;
        }
    }

    if (anyChanged) {
        markPrimBufferDirty();
        markDirty();
    }
}

void YDrawZoo::removeObject(uint32_t idx) {
    if (idx >= _objects.size()) return;

    auto& obj = _objects[idx];
    if (obj.primIndex < primitiveCount()) {
        auto& prim = primitivePtr()[obj.primIndex];
        prim.fillColor = 0;
        prim.strokeColor = 0;
        prim.params[2] = 0;
        prim.params[3] = 0;
        prim.aabbMinX = prim.aabbMinY = 0;
        prim.aabbMaxX = prim.aabbMaxY = 0;
    }

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
