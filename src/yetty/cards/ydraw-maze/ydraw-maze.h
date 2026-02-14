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
// YDrawMaze - Procedural maze with animated solver
//
// Generates a random maze using recursive backtracker, then animates an
// actor traversing the solution path. When the actor reaches the goal,
// a new maze is generated.
//
// Arguments (space-separated --key value):
//   --cols N            Maze columns (default 15)
//   --rows N            Maze rows (default 10)
//   --speed F           Actor movement speed in cells/sec (default 4)
//   --wall-color 0xCOL  Wall color ABGR hex (default 0xFF808080)
//   --actor-color 0xCOL Actor color ABGR hex (default 0xFF00CCFF)
//   --start-color 0xCOL Start marker color (default 0xFF00FF00)
//   --end-color 0xCOL   End marker color (default 0xFF0000FF)
//   --bg-color 0xCOL    Background color (default 0xFF1A1A2E)
//   --wall-width F      Wall thickness (default 1.5)
//=============================================================================
class YDrawMaze : public Card,
                  public base::ObjectFactory<YDrawMaze> {
public:
    using Ptr = std::shared_ptr<YDrawMaze>;

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

    YDrawMaze(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args);

    ~YDrawMaze() override;

    const char* typeName() const override { return "ydraw-maze"; }

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

    // Maze cell walls bitmask
    static constexpr uint8_t WALL_N = 1;
    static constexpr uint8_t WALL_S = 2;
    static constexpr uint8_t WALL_E = 4;
    static constexpr uint8_t WALL_W = 8;

    // Argument parsing
    void parseArgs(const std::string& args);
    std::string _argsStr;

    // Configurable parameters
    uint32_t _mazeCols = 15;
    uint32_t _mazeRows = 10;
    float _actorSpeed = 4.0f;       // cells per second
    float _wallWidth = 1.5f;
    uint32_t _wallColor = 0xFF808080;
    uint32_t _actorColor = 0xFF00CCFF;
    uint32_t _startColor = 0xFF00FF00;
    uint32_t _endColor = 0xFF0000FF;
    uint32_t _bgColor = 0xFF1A1A2E;

    // Maze generation (recursive backtracker)
    void generateMaze();
    uint8_t& cell(uint32_t col, uint32_t row);
    uint8_t cell(uint32_t col, uint32_t row) const;

    // Pathfinding (BFS)
    void solveMaze();

    // Rendering
    void buildPrims(float time);
    float cellX(uint32_t col) const;
    float cellY(uint32_t row) const;

    // GPU upload
    Result<void> uploadMetadata();

    // Builder (CPU staging)
    std::shared_ptr<YDrawBuilder> _builder;

    // CPU primitive buffer
    std::vector<SDFPrimitive> _primBuffer;

    // Maze state
    std::vector<uint8_t> _grid;     // walls per cell (_mazeCols * _mazeRows)
    std::vector<std::pair<uint32_t, uint32_t>> _solutionPath;  // (col, row) from start to end
    uint32_t _startCol = 0, _startRow = 0;
    uint32_t _endCol = 0, _endRow = 0;
    float _cellW = 0, _cellH = 0;
    float _originX = 0, _originY = 0;

    // Actor animation
    float _solveStartTime = 0.0f;
    bool _mazeReady = false;

    // GPU state
    StorageHandle _primStorage = StorageHandle::invalid();
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;

    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t* _gridBuf = nullptr;
    uint32_t _gridBufSize = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;

    // RNG
    std::mt19937 _rng;
    float _lastTime = -1.0f;
    bool _initialized = false;

    // Dirty flags
    bool _dirty = true;
    bool _metadataDirty = true;

    // Screen ID
    base::ObjectId _screenId = 0;
};

} // namespace yetty::card
