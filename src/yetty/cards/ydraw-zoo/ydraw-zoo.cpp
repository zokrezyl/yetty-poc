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

    // Greedy pre-allocation: reserve headroom so mid-render addPrimitive() calls
    // succeed without triggering buffer repack events every frame.
    setPrimCapacityHint(TARGET_OBJECTS * 3);

    // Dark background
    setBgColor(0xFF1A1A2E);  // ABGR: dark navy

    // Uniform scaling so SDF shapes (circles, stars, etc.) aren't distorted
    addFlags(FLAG_UNIFORM_SCALE);

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

    int choice = std::uniform_int_distribution<int>(0, 42)(_rng);

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

SDFPrimitive YDrawZoo::makeRandom3DShape(float cx, float cy, float size,
                                          uint32_t color, uint32_t layer) {
    SDFPrimitive prim = {};
    prim.layer = layer;
    prim.fillColor = color;
    prim.strokeColor = 0;
    prim.strokeWidth = 0;

    float x3d = scene2Dx(cx);
    float y3d = scene2Dy(cy);
    float s3d = scene2Dsize(size);

    int choice = std::uniform_int_distribution<int>(0, 5)(_rng);

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
        prim.params[3] = s3d * 0.8f;  // major radius
        prim.params[4] = s3d * 0.3f;  // minor radius
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
        prim.params[3] = s3d * 1.5f;  // height
        break;
    case 5: // Cylinder
        prim.type = static_cast<uint32_t>(SDFType::Cylinder3D);
        prim.params[0] = x3d;
        prim.params[1] = y3d;
        prim.params[2] = 0.0f;
        prim.params[3] = s3d * 0.7f;  // radius
        prim.params[4] = s3d;          // height
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
    // params[2] stays at 0 (z position)

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

            if (prim.type >= 100) {
                // 3D shape: map 2D scene coords to 3D space
                update3DPrim(prim, cx, cy, size);
            } else {
                // 2D shape: update based on type
                auto sdfType = static_cast<SDFType>(prim.type);
                switch (sdfType) {
                // Endpoint-based shapes: recompute all points from cx/cy/size
                case SDFType::Segment:
                    prim.params[0] = cx - size;
                    prim.params[1] = cy;
                    prim.params[2] = cx + size;
                    prim.params[3] = cy;
                    prim.strokeWidth = size * 0.15f;
                    break;
                case SDFType::Bezier2:
                    prim.params[0] = cx - size;
                    prim.params[1] = cy;
                    prim.params[2] = cx;
                    prim.params[3] = cy - size;
                    prim.params[4] = cx + size;
                    prim.params[5] = cy;
                    prim.strokeWidth = size * 0.15f;
                    break;
                case SDFType::Triangle:
                    prim.params[0] = cx;
                    prim.params[1] = cy - size;
                    prim.params[2] = cx - size;
                    prim.params[3] = cy + size * 0.7f;
                    prim.params[4] = cx + size;
                    prim.params[5] = cy + size * 0.7f;
                    break;
                case SDFType::Capsule:
                    prim.params[0] = cx - size * 0.7f;
                    prim.params[1] = cy;
                    prim.params[2] = cx + size * 0.7f;
                    prim.params[3] = cy;
                    prim.params[4] = size * 0.3f;
                    prim.strokeWidth = size * 0.1f;
                    break;
                case SDFType::OrientedBox:
                    prim.params[0] = cx - size;
                    prim.params[1] = cy - size * 0.3f;
                    prim.params[2] = cx + size;
                    prim.params[3] = cy + size * 0.3f;
                    prim.params[4] = size * 0.4f;
                    break;
                case SDFType::OrientedVesica:
                    prim.params[0] = cx - size * 0.5f;
                    prim.params[1] = cy;
                    prim.params[2] = cx + size * 0.5f;
                    prim.params[3] = cy;
                    prim.params[4] = size * 0.3f;
                    break;

                // Center-based shapes: set cx/cy then size params
                default:
                    prim.params[0] = cx;
                    prim.params[1] = cy;

                    switch (sdfType) {
                    // Simple radius shapes: params[2] = size
                    case SDFType::Circle:
                    case SDFType::Pentagon:
                    case SDFType::Hexagon:
                    case SDFType::Heart:
                    case SDFType::EquilateralTriangle:
                    case SDFType::Octogon:
                    case SDFType::Hexagram:
                    case SDFType::Pentagram:
                    case SDFType::QuadraticCircle:
                    case SDFType::CoolS:
                        prim.params[2] = size;
                        break;
                    case SDFType::Star:
                        prim.params[2] = size;
                        // params[3]=5.0, params[4]=2.5 stay fixed
                        break;
                    case SDFType::Box:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.8f;
                        prim.round = size * 0.15f;
                        break;
                    case SDFType::Rhombus:
                        prim.params[2] = size;
                        prim.params[3] = size * 1.4f;
                        break;
                    case SDFType::Egg:
                    case SDFType::Ellipse:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.6f;
                        break;
                    case SDFType::Arc:
                        // params[2],[3] = sin/cos stay fixed
                        prim.params[4] = size;
                        prim.params[5] = size * 0.2f;
                        break;
                    case SDFType::RoundedBox:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.7f;
                        prim.params[4] = size * 0.2f;
                        prim.params[5] = size * 0.2f;
                        prim.params[6] = size * 0.2f;
                        prim.params[7] = size * 0.2f;
                        break;
                    case SDFType::ChamferBox:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.8f;
                        prim.params[4] = size * 0.2f;
                        break;
                    case SDFType::Trapezoid:
                        prim.params[2] = size * 0.8f;
                        prim.params[3] = size * 0.5f;
                        prim.params[4] = size;
                        break;
                    case SDFType::Parallelogram:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.6f;
                        prim.params[4] = size * 0.3f;
                        break;
                    case SDFType::IsoscelesTriangle:
                        prim.params[2] = size * 0.7f;
                        prim.params[3] = size * 1.2f;
                        break;
                    case SDFType::UnevenCapsule:
                        prim.params[2] = size * 0.5f;
                        prim.params[3] = size * 0.3f;
                        prim.params[4] = size * 1.2f;
                        break;
                    case SDFType::CutDisk:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.3f;
                        break;
                    case SDFType::Horseshoe:
                        // params[2],[3] = sin/cos stay fixed
                        prim.params[4] = size * 0.8f;
                        prim.params[5] = size * 0.3f;
                        prim.params[6] = size * 0.15f;
                        break;
                    case SDFType::Vesica:
                        prim.params[2] = size * 0.6f;
                        prim.params[3] = size;
                        break;
                    case SDFType::RoundedCross:
                    case SDFType::BlobbyCross:
                        prim.params[2] = size * 0.5f;
                        break;
                    case SDFType::Parabola:
                        // params[2] stays fixed at 0.5
                        break;
                    case SDFType::Tunnel:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.7f;
                        break;
                    case SDFType::Stairs:
                        prim.params[2] = size * 0.3f;
                        prim.params[3] = size * 0.2f;
                        // params[4]=5.0 stays fixed
                        break;
                    case SDFType::Hyperbola:
                        // params[2] stays fixed at 0.5
                        prim.params[3] = size;
                        break;
                    case SDFType::CircleWave:
                        // params[2] stays fixed at 0.5
                        prim.params[3] = size;
                        break;
                    case SDFType::Cross:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.3f;
                        prim.params[4] = size * 0.1f;
                        break;
                    case SDFType::RoundedX:
                        prim.params[2] = size;
                        prim.params[3] = size * 0.2f;
                        break;
                    case SDFType::Pie:
                        // params[2],[3] = sin/cos stay fixed
                        prim.params[4] = size;
                        break;
                    case SDFType::Ring:
                        // params[2],[3] = sin/cos stay fixed
                        prim.params[4] = size;
                        prim.params[5] = size * 0.2f;
                        break;
                    case SDFType::Moon:
                        prim.params[2] = size * 0.5f;
                        prim.params[3] = size;
                        prim.params[4] = size * 0.8f;
                        break;
                    default:
                        prim.params[2] = size;
                        break;
                    }
                    break;
                }
            }

            // Stroke-only shapes: animate strokeColor instead of fillColor
            if (prim.fillColor == 0 && prim.strokeColor != 0) {
                prim.strokeColor = animatedColor;
            } else {
                prim.fillColor = animatedColor;
            }
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
