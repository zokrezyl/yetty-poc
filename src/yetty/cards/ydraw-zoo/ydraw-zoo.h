#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <string>
#include <vector>
#include <cstdint>
#include <random>

namespace yetty {
class YDrawBuilder;
namespace card { struct SDFPrimitive; }
}

namespace yetty::card {

//=============================================================================
// ZooObject - A single shape in the zoo
//=============================================================================
struct ZooObject {
    int shapeChoice;        // shape type index (0-42 for 2D)
    float angle;            // radial angle from center (radians)
    float spawnTime;        // time when spawned
    float baseRadius;       // distance from center at spawn
    float animPhase;        // phase offset for decorative animation
    float animFreq;         // frequency for decorative animation
    uint32_t baseColor;     // original fill color
};

//=============================================================================
// YDrawZoo - Infinite zoom card with procedurally spawned shapes
//
// Uses YDrawBuilder for CPU staging. All procedural work (object lifecycle,
// primitive generation) happens in renderToStaging. GPU writes in render().
//=============================================================================
class YDrawZoo : public Card,
                 public base::ObjectFactory<YDrawZoo> {
public:
    using Ptr = std::shared_ptr<YDrawZoo>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;

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

    YDrawZoo(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells);

    ~YDrawZoo() override;

    const char* typeName() const override { return "ydraw-zoo"; }

    //=========================================================================
    // Card lifecycle
    //=========================================================================
    bool needsBuffer() const override { return true; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    bool needsBufferRealloc() override;
    void renderToStaging(float time) override;
    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> render() override;
    Result<void> dispose() override;
    void suspend() override;

    Result<void> init();

private:
    // Scene dimensions (fixed)
    static constexpr float SCENE_W = 600.0f;
    static constexpr float SCENE_H = 400.0f;
    static constexpr float CENTER_X = SCENE_W / 2.0f;
    static constexpr float CENTER_Y = SCENE_H / 2.0f;

    // Zoo parameters
    static constexpr uint32_t TARGET_OBJECTS = 18;
    static constexpr float GROWTH_RATE = 0.65f;
    static constexpr float SPAWN_RADIUS_MIN = 15.0f;
    static constexpr float SPAWN_RADIUS_MAX = 60.0f;
    static constexpr float SPAWN_SIZE = 4.0f;
    static constexpr float EXIT_SCALE = 12.0f;

    // Object lifecycle (pure CPU — works on _primBuffer staging)
    void spawnObject(float time);
    void updateObjects(float time);
    void removeObject(uint32_t idx);

    // Shape generation (deterministic — choice selected at spawn time)
    SDFPrimitive makeShape(int choice, float cx, float cy, float size,
                           uint32_t color, uint32_t layer);
    SDFPrimitive make3DShape(int choice, float cx, float cy, float size,
                             uint32_t color, uint32_t layer);
    void update3DPrim(SDFPrimitive& prim, float cx, float cy, float size);
    uint32_t randomColor();

    // GPU upload
    Result<void> uploadMetadata();

    // 3D coordinate mapping
    float scene2Dx(float cx) const { return (cx / SCENE_W - 0.5f) * 2.0f * (SCENE_W / SCENE_H); }
    float scene2Dy(float cy) const { return -(cy / SCENE_H - 0.5f) * 2.0f; }
    float scene2Dsize(float s) const { return s / SCENE_H * 2.0f; }

    // Builder (CPU staging)
    std::shared_ptr<YDrawBuilder> _builder;

    // CPU primitive buffer — objects point into this
    std::vector<SDFPrimitive> _primBuffer;

    // GPU state
    StorageHandle _primStorage = StorageHandle::invalid();
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;

    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;

    // Object state
    std::vector<ZooObject> _objects;
    std::mt19937 _rng;
    float _lastTime = -1.0f;
    bool _initialized = false;

    // Dirty flags
    bool _dirty = true;
    bool _metadataDirty = true;

    // Screen ID for events
    base::ObjectId _screenId = 0;
};

} // namespace yetty::card
