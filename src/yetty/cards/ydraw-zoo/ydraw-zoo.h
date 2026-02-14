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
// ControlPoint - A node in the connection graph
//=============================================================================
struct ControlPoint {
    float angle;            // radial angle from center (radians)
    float spawnTime;        // time when spawned
    float baseRadius;       // distance from center at spawn
    float animPhase;        // phase offset for breathing animation
    float animFreq;         // frequency for breathing animation
    uint32_t color;         // node color
};

//=============================================================================
// Connection - A curve/line/shape connecting two control points
//=============================================================================
struct Connection {
    uint32_t cpA, cpB;      // control point indices
    int type;               // 0=bezier, 1=segment, 2=shape at midpoint
    int shapeChoice;        // for type==2 (shape at midpoint)
    float curvature;        // bezier control point offset (-1 to 1)
    float curveAnimPhase;   // animation phase for curvature wobble
    float curveAnimFreq;    // animation frequency for curvature
    float strokeWidth;      // line thickness
    uint32_t color;         // connection color (blend of endpoints)
};

//=============================================================================
// YDrawZoo - Infinite zoom with connected control points and curves
//
// Uses YDrawBuilder for CPU staging. Control points drift outward from center
// and are connected by bezier curves (dominant), line segments, and shapes.
//
// Arguments (space-separated --key value):
//   --points N          Number of control points (default 15)
//   --connections N     Target connections per point (default 3)
//   --growth F          Exponential growth rate (default 0.65)
//   --max-dist F        Max connection distance before breaking (default 200)
//   --marker-size F     Control point marker radius (default 3)
//   --stroke-min F      Min stroke width for connections (default 0.5)
//   --stroke-max F      Max stroke width for connections (default 2.5)
//   --curve-ratio F     Proportion of bezier curves 0-1 (default 0.8)
//   --spawn-radius F,F  Spawn radius min,max (default 15,60)
//   --bg-color 0xCOLOR  Background color in ABGR hex (default 0xFF1A1A2E)
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
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args);

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
    Result<void> finalize() override;
    Result<void> dispose() override;
    void suspend() override;

    Result<void> init();

private:
    // Scene dimensions (fixed)
    static constexpr float SCENE_W = 600.0f;
    static constexpr float SCENE_H = 400.0f;
    static constexpr float CENTER_X = SCENE_W / 2.0f;
    static constexpr float CENTER_Y = SCENE_H / 2.0f;

    // Argument parsing
    void parseArgs(const std::string& args);
    std::string _argsStr;

    // Network parameters (configurable via args)
    uint32_t _targetPoints = 20;
    uint32_t _targetConnsPerCP = 3;
    float _growthRate = 0.30f;
    float _spawnRadiusMin = 20.0f;
    float _spawnRadiusMax = 140.0f;
    float _cpMarkerSize = 3.0f;
    float _maxConnectionDist = 280.0f;
    float _strokeMin = 0.5f;
    float _strokeMax = 2.5f;
    float _curveRatio = 0.80f;
    uint32_t _bgColor = 0xFF1A1A2E;

    // Control point lifecycle
    void spawnControlPoint(float time);
    void removeControlPoint(uint32_t idx);

    // Connection management
    void updateConnections(float time);

    // Shape generation (for midpoint shapes in connections)
    SDFPrimitive makeShape(int choice, float cx, float cy, float size,
                           uint32_t color, uint32_t layer);
    uint32_t randomColor();
    uint32_t blendColors(uint32_t a, uint32_t b);

    // Control point position helper
    struct Vec2f { float x, y; };
    Vec2f cpPosition(const ControlPoint& cp, float time) const;

    // GPU upload
    Result<void> uploadMetadata();

    // Builder (CPU staging)
    std::shared_ptr<YDrawBuilder> _builder;

    // CPU primitive buffer
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

    // Network state
    std::vector<ControlPoint> _controlPoints;
    std::vector<Connection> _connections;
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
