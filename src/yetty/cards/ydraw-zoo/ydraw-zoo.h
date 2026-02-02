#pragma once

#include "../../ydraw/ydraw-base.h"
#include <yetty/base/factory.h>
#include <string>
#include <vector>
#include <cstdint>
#include <random>

namespace yetty::card {

//=============================================================================
// ZooObject - A single shape in the zoo
//=============================================================================
struct ZooObject {
    uint32_t primIndex;     // index in primitive buffer
    float angle;            // radial angle from center (radians)
    float spawnTime;        // time when spawned
    float baseRadius;       // base size of shape (unscaled)
    float animPhase;        // phase offset for decorative animation
    float animFreq;         // frequency for decorative animation
    uint32_t baseColor;     // original fill color
    bool active;
};

//=============================================================================
// YDrawZoo - Infinite zoom card with procedurally spawned shapes
//=============================================================================
class YDrawZoo : public yetty::YDrawBase,
                 public base::ObjectFactory<YDrawZoo> {
public:
    using Ptr = std::shared_ptr<YDrawZoo>;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    ~YDrawZoo() override = default;

    const char* typeName() const override { return "ydraw-zoo"; }

    YDrawZoo(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells);

    Result<void> init();
    Result<void> render(float time) override;

private:
    // Scene dimensions (fixed)
    static constexpr float SCENE_W = 600.0f;
    static constexpr float SCENE_H = 400.0f;
    static constexpr float CENTER_X = SCENE_W / 2.0f;
    static constexpr float CENTER_Y = SCENE_H / 2.0f;

    // Zoo parameters
    static constexpr uint32_t TARGET_OBJECTS = 18;
    static constexpr float GROWTH_RATE = 0.65f;       // exponential growth speed
    static constexpr float SPAWN_RADIUS_MIN = 15.0f;  // distance from center at spawn
    static constexpr float SPAWN_RADIUS_MAX = 60.0f;
    static constexpr float SPAWN_SIZE = 4.0f;         // initial shape size
    static constexpr float EXIT_SCALE = 12.0f;        // scale at which object exits

    void spawnObject(float time);
    void updateObjects(float time);
    void removeObject(uint32_t idx);
    SDFPrimitive makeRandomShape(float cx, float cy, float size,
                                 uint32_t color, uint32_t layer);
    uint32_t randomColor();

    std::vector<ZooObject> _objects;
    std::mt19937 _rng;
    float _lastTime = -1.0f;
    bool _initialized = false;
};

} // namespace yetty::card
