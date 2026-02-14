#include "ydraw-maze.h"
#include <yetty/ydraw-builder.h>
#include "../hdraw/hdraw.h"  // SDFPrimitive, SDFType
#include <yetty/msdf-glyph-data.h>
#include <ytrace/ytrace.hpp>
#include <cmath>
#include <cstring>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YDrawMaze::YDrawMaze(const YettyContext& ctx,
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
        yerror("YDrawMaze: failed to create builder");
    }
}

YDrawMaze::~YDrawMaze() { dispose(); }

//=============================================================================
// Initialization
//=============================================================================

Result<void> YDrawMaze::init() {
    auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
    if (!metaResult) {
        return Err<void>("YDrawMaze::init: failed to allocate metadata");
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

void YDrawMaze::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--cols") {
            uint32_t v; if (iss >> v) _mazeCols = std::max(3u, std::min(v, 80u));
        } else if (token == "--rows") {
            uint32_t v; if (iss >> v) _mazeRows = std::max(3u, std::min(v, 50u));
        } else if (token == "--speed") {
            float v; if (iss >> v) _actorSpeed = std::max(0.5f, std::min(v, 50.0f));
        } else if (token == "--wall-width") {
            float v; if (iss >> v) _wallWidth = std::max(0.3f, std::min(v, 5.0f));
        } else if (token == "--wall-color" || token == "--actor-color" ||
                   token == "--start-color" || token == "--end-color" ||
                   token == "--bg-color") {
            std::string s;
            if (iss >> s) {
                if (s.substr(0, 2) == "0x" || s.substr(0, 2) == "0X")
                    s = s.substr(2);
                try {
                    uint32_t c = static_cast<uint32_t>(std::stoul(s, nullptr, 16));
                    if (token == "--wall-color") _wallColor = c;
                    else if (token == "--actor-color") _actorColor = c;
                    else if (token == "--start-color") _startColor = c;
                    else if (token == "--end-color") _endColor = c;
                    else if (token == "--bg-color") _bgColor = c;
                } catch (...) {}
            }
        }
    }
}

//=============================================================================
// Maze generation — recursive backtracker (DFS)
//=============================================================================

uint8_t& YDrawMaze::cell(uint32_t col, uint32_t row) {
    return _grid[row * _mazeCols + col];
}

uint8_t YDrawMaze::cell(uint32_t col, uint32_t row) const {
    return _grid[row * _mazeCols + col];
}

void YDrawMaze::generateMaze() {
    uint32_t total = _mazeCols * _mazeRows;
    _grid.assign(total, WALL_N | WALL_S | WALL_E | WALL_W);

    // Compute cell dimensions to fit scene with padding
    float pad = 10.0f;
    float usableW = SCENE_W - 2.0f * pad;
    float usableH = SCENE_H - 2.0f * pad;
    _cellW = usableW / static_cast<float>(_mazeCols);
    _cellH = usableH / static_cast<float>(_mazeRows);
    _originX = pad;
    _originY = pad;

    // Start and end positions
    _startCol = 0;
    _startRow = 0;
    _endCol = _mazeCols - 1;
    _endRow = _mazeRows - 1;

    // DFS with explicit stack
    std::vector<bool> visited(total, false);
    std::stack<std::pair<uint32_t, uint32_t>> stack;

    visited[0] = true;
    stack.push({0, 0});

    // Direction offsets: N, S, E, W
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {-1, 1, 0, 0};
    const uint8_t wallRemove[] = {WALL_N, WALL_S, WALL_E, WALL_W};
    const uint8_t wallOpposite[] = {WALL_S, WALL_N, WALL_W, WALL_E};

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();

        // Find unvisited neighbors
        int neighbors[4];
        int ncount = 0;
        for (int d = 0; d < 4; d++) {
            int nx = static_cast<int>(cx) + dx[d];
            int ny = static_cast<int>(cy) + dy[d];
            if (nx >= 0 && nx < static_cast<int>(_mazeCols) &&
                ny >= 0 && ny < static_cast<int>(_mazeRows) &&
                !visited[ny * _mazeCols + nx]) {
                neighbors[ncount++] = d;
            }
        }

        if (ncount == 0) {
            stack.pop();
            continue;
        }

        // Pick random neighbor
        int d = neighbors[std::uniform_int_distribution<int>(0, ncount - 1)(_rng)];
        uint32_t nx = static_cast<uint32_t>(static_cast<int>(cx) + dx[d]);
        uint32_t ny = static_cast<uint32_t>(static_cast<int>(cy) + dy[d]);

        // Remove walls between current and neighbor
        cell(cx, cy) &= ~wallRemove[d];
        cell(nx, ny) &= ~wallOpposite[d];

        visited[ny * _mazeCols + nx] = true;
        stack.push({nx, ny});
    }
}

//=============================================================================
// Pathfinding — BFS
//=============================================================================

void YDrawMaze::solveMaze() {
    uint32_t total = _mazeCols * _mazeRows;
    std::vector<int> parent(total, -1);
    std::vector<bool> visited(total, false);
    std::queue<uint32_t> q;

    uint32_t startIdx = _startRow * _mazeCols + _startCol;
    uint32_t endIdx = _endRow * _mazeCols + _endCol;

    visited[startIdx] = true;
    q.push(startIdx);

    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {-1, 1, 0, 0};
    const uint8_t wallCheck[] = {WALL_N, WALL_S, WALL_E, WALL_W};

    while (!q.empty()) {
        uint32_t idx = q.front();
        q.pop();

        if (idx == endIdx) break;

        uint32_t cx = idx % _mazeCols;
        uint32_t cy = idx / _mazeCols;

        for (int d = 0; d < 4; d++) {
            // Can't go through wall
            if (_grid[idx] & wallCheck[d]) continue;

            int nx = static_cast<int>(cx) + dx[d];
            int ny = static_cast<int>(cy) + dy[d];
            if (nx < 0 || nx >= static_cast<int>(_mazeCols) ||
                ny < 0 || ny >= static_cast<int>(_mazeRows)) continue;

            uint32_t nIdx = static_cast<uint32_t>(ny) * _mazeCols + static_cast<uint32_t>(nx);
            if (visited[nIdx]) continue;

            visited[nIdx] = true;
            parent[nIdx] = static_cast<int>(idx);
            q.push(nIdx);
        }
    }

    // Reconstruct path
    _solutionPath.clear();
    int cur = static_cast<int>(endIdx);
    while (cur >= 0) {
        uint32_t cx = static_cast<uint32_t>(cur) % _mazeCols;
        uint32_t cy = static_cast<uint32_t>(cur) / _mazeCols;
        _solutionPath.push_back({cx, cy});
        cur = parent[cur];
    }
    std::reverse(_solutionPath.begin(), _solutionPath.end());
}

//=============================================================================
// Coordinate helpers
//=============================================================================

float YDrawMaze::cellX(uint32_t col) const {
    return _originX + (static_cast<float>(col) + 0.5f) * _cellW;
}

float YDrawMaze::cellY(uint32_t row) const {
    return _originY + (static_cast<float>(row) + 0.5f) * _cellH;
}

//=============================================================================
// Prim generation
//=============================================================================

void YDrawMaze::buildPrims(float time) {
    _primBuffer.clear();
    uint32_t layer = 0;

    // 1. Wall segments
    for (uint32_t row = 0; row < _mazeRows; row++) {
        for (uint32_t col = 0; col < _mazeCols; col++) {
            uint8_t w = cell(col, row);
            float x0 = _originX + static_cast<float>(col) * _cellW;
            float y0 = _originY + static_cast<float>(row) * _cellH;
            float x1 = x0 + _cellW;
            float y1 = y0 + _cellH;

            auto addWall = [&](float ax, float ay, float bx, float by) {
                SDFPrimitive prim = {};
                prim.type = static_cast<uint32_t>(SDFType::Segment);
                prim.params[0] = ax;
                prim.params[1] = ay;
                prim.params[2] = bx;
                prim.params[3] = by;
                prim.fillColor = 0;
                prim.strokeColor = _wallColor;
                prim.strokeWidth = _wallWidth;
                prim.layer = layer++;
                YDrawBuilder::recomputeAABB(prim);
                _primBuffer.push_back(prim);
            };

            // North wall (only draw for row 0 to avoid doubles)
            if ((w & WALL_N) && row == 0) addWall(x0, y0, x1, y0);
            // South wall
            if (w & WALL_S) addWall(x0, y1, x1, y1);
            // West wall (only draw for col 0 to avoid doubles)
            if ((w & WALL_W) && col == 0) addWall(x0, y0, x0, y1);
            // East wall
            if (w & WALL_E) addWall(x1, y0, x1, y1);
        }
    }

    // 2. Start marker (circle)
    {
        float markerR = std::min(_cellW, _cellH) * 0.3f;
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.params[0] = cellX(_startCol);
        prim.params[1] = cellY(_startRow);
        prim.params[2] = markerR;
        prim.fillColor = _startColor;
        prim.strokeColor = 0;
        prim.strokeWidth = 0;
        prim.layer = layer++;
        YDrawBuilder::recomputeAABB(prim);
        _primBuffer.push_back(prim);
    }

    // 3. End marker (star)
    {
        float markerR = std::min(_cellW, _cellH) * 0.35f;
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Star);
        prim.params[0] = cellX(_endCol);
        prim.params[1] = cellY(_endRow);
        prim.params[2] = markerR;
        prim.params[3] = 5.0f;   // 5 points
        prim.params[4] = 2.2f;   // inner/outer ratio
        prim.fillColor = _endColor;
        prim.strokeColor = 0;
        prim.strokeWidth = 0;
        prim.layer = layer++;
        YDrawBuilder::recomputeAABB(prim);
        _primBuffer.push_back(prim);
    }

    // 4. Trail — draw visited portion of solution path
    if (!_solutionPath.empty()) {
        float elapsed = time - _solveStartTime;
        float totalCells = elapsed * _actorSpeed;
        uint32_t cellsTraversed = static_cast<uint32_t>(totalCells);
        cellsTraversed = std::min(cellsTraversed, static_cast<uint32_t>(_solutionPath.size() - 1));

        // Draw trail segments
        uint32_t trailColor = (_actorColor & 0x00FFFFFFu) | 0x60000000u; // semi-transparent
        for (uint32_t i = 0; i < cellsTraversed && i + 1 < _solutionPath.size(); i++) {
            auto [c0, r0] = _solutionPath[i];
            auto [c1, r1] = _solutionPath[i + 1];

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Segment);
            prim.params[0] = cellX(c0);
            prim.params[1] = cellY(r0);
            prim.params[2] = cellX(c1);
            prim.params[3] = cellY(r1);
            prim.fillColor = 0;
            prim.strokeColor = trailColor;
            prim.strokeWidth = std::min(_cellW, _cellH) * 0.15f;
            prim.layer = layer++;
            YDrawBuilder::recomputeAABB(prim);
            _primBuffer.push_back(prim);
        }
    }

    // 5. Actor (animated position along solution path)
    if (!_solutionPath.empty()) {
        float elapsed = time - _solveStartTime;
        float progress = elapsed * _actorSpeed;
        uint32_t idx = static_cast<uint32_t>(progress);
        float frac = progress - static_cast<float>(idx);

        float ax, ay;
        if (idx >= _solutionPath.size() - 1) {
            // Arrived at end
            auto [c, r] = _solutionPath.back();
            ax = cellX(c);
            ay = cellY(r);
        } else {
            // Interpolate between cells
            auto [c0, r0] = _solutionPath[idx];
            auto [c1, r1] = _solutionPath[idx + 1];
            ax = cellX(c0) + (cellX(c1) - cellX(c0)) * frac;
            ay = cellY(r0) + (cellY(r1) - cellY(r0)) * frac;
        }

        float actorR = std::min(_cellW, _cellH) * 0.28f;

        // Actor body (circle)
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.params[0] = ax;
        prim.params[1] = ay;
        prim.params[2] = actorR;
        prim.fillColor = _actorColor;
        prim.strokeColor = 0;
        prim.strokeWidth = 0;
        prim.layer = layer++;
        YDrawBuilder::recomputeAABB(prim);
        _primBuffer.push_back(prim);
    }
}

//=============================================================================
// Card lifecycle
//=============================================================================

bool YDrawMaze::needsBufferRealloc() {
    if (!_primStorage.isValid()) return true;
    if (_primBuffer.size() > _primCapacity) return true;
    if (!_builder) return false;
    uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
    if (gridBytes > 0 && (!_derivedStorage.isValid() || gridBytes > _derivedStorage.size))
        return true;
    return false;
}

void YDrawMaze::renderToStaging(float time) {
    if (!_builder) return;

    if (!_initialized) {
        _lastTime = time;
        generateMaze();
        solveMaze();
        _solveStartTime = time;
        _mazeReady = true;
        _initialized = true;
    }

    _lastTime = time;

    // Check if actor reached the end — generate new maze
    if (_mazeReady && !_solutionPath.empty()) {
        float elapsed = time - _solveStartTime;
        float totalTime = static_cast<float>(_solutionPath.size() - 1) / _actorSpeed;
        if (elapsed > totalTime + 1.0f) {  // 1 second pause at the end
            generateMaze();
            solveMaze();
            _solveStartTime = time;
        }
    }

    // Build all primitives
    buildPrims(time);

    // Feed to builder staging and compute grid
    _builder->clear();
    for (const auto& prim : _primBuffer) {
        _builder->addPrimitive(prim);
    }
    _builder->calculate();

    _dirty = true;
}

void YDrawMaze::declareBufferNeeds() {
    if (!_builder) return;

    _derivedStorage = StorageHandle::invalid();
    _gridBuf = nullptr;
    _gridBufSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;

    const auto& primStaging = _builder->primStaging();
    if (primStaging.empty()) {
        _builder->clearGridStaging();
        return;
    }

    uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
    _cardMgr->bufferManager()->reserve(primSize);

    uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
    if (gridBytes > 0) {
        _cardMgr->bufferManager()->reserve(gridBytes * 2);
    }
}

Result<void> YDrawMaze::allocateBuffers() {
    if (!_builder) return Ok();

    const auto& primStaging = _builder->primStaging();
    const auto& gridStaging = _builder->gridStaging();

    if (!primStaging.empty()) {
        uint32_t count = static_cast<uint32_t>(primStaging.size());
        uint32_t allocBytes = count * sizeof(SDFPrimitive);
        auto primResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "prims", allocBytes);
        if (!primResult) {
            return Err<void>("YDrawMaze::allocateBuffers: prim alloc failed");
        }
        _primStorage = *primResult;
        _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
        _primCapacity = count;
        _primCount = count;
        std::memcpy(_primitives, primStaging.data(), count * sizeof(SDFPrimitive));
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }

    uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
    if (gridBytes > 0) {
        uint32_t allocSize = gridBytes * 2;
        auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "derived", allocSize);
        if (!storageResult) {
            return Err<void>("YDrawMaze::allocateBuffers: derived alloc failed");
        }
        _derivedStorage = *storageResult;

        uint8_t* base = _derivedStorage.data;
        std::memset(base, 0, _derivedStorage.size);
        _gridBuf = reinterpret_cast<uint32_t*>(base);
        _gridBufSize = static_cast<uint32_t>(gridStaging.size());
        _gridOffset = _derivedStorage.offset / sizeof(float);
        std::memcpy(base, gridStaging.data(), gridBytes);
        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
    _gridWidth = _builder->gridWidth();
    _gridHeight = _builder->gridHeight();
    _metadataDirty = true;
    _dirty = false;

    return Ok();
}

Result<void> YDrawMaze::finalize() {
    if (!_builder) return Ok();

    if (_dirty) {
        if (_primitives && !_primBuffer.empty()) {
            uint32_t count = std::min(static_cast<uint32_t>(_primBuffer.size()), _primCapacity);
            std::memcpy(_primitives, _primBuffer.data(), count * sizeof(SDFPrimitive));
            _primCount = count;
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        if (_derivedStorage.isValid()) {
            const auto& gridData = _builder->gridStaging();
            uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
            if (gridBytes > 0 && gridBytes <= _derivedStorage.size) {
                uint8_t* base = _derivedStorage.data;
                std::memset(base, 0, _derivedStorage.size);
                std::memcpy(base, gridData.data(), gridBytes);
                _gridBuf = reinterpret_cast<uint32_t*>(base);
                _gridBufSize = static_cast<uint32_t>(gridData.size());
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

Result<void> YDrawMaze::dispose() {
    _derivedStorage = StorageHandle::invalid();
    _gridBuf = nullptr;
    _gridBufSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;
    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YDrawMaze::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
}

void YDrawMaze::suspend() {
    _derivedStorage = StorageHandle::invalid();
    _gridBuf = nullptr;
    _gridBufSize = 0;
    _primStorage = StorageHandle::invalid();
    _primitives = nullptr;
    _primCount = 0;
    _primCapacity = 0;
}

//=============================================================================
// GPU metadata upload
//=============================================================================

Result<void> YDrawMaze::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YDrawMaze::uploadMetadata: invalid handle");
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
    meta.flags = (_builder->flags() & 0xFFFF) | (0x3C00u << 16);
    meta.bgColor = _builder->bgColor();

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YDrawMaze::uploadMetadata: write failed");
    }
    return Ok();
}

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YDrawMaze::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    (void)payload;

    yinfo("YDrawMaze::create: pos=({},{}) size={}x{} args='{}'", x, y, widthCells, heightCells, args);

    if (!ctx.cardManager) {
        return Err<CardPtr>("YDrawMaze::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDrawMaze>(ctx, x, y, widthCells, heightCells, args);

    if (auto res = card->init(); !res) {
        yerror("YDrawMaze::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YDrawMaze::create: init failed");
    }

    yinfo("YDrawMaze::create: SUCCESS");
    return Ok<CardPtr>(card);
}

Result<YDrawMaze::Ptr> YDrawMaze::createImpl(
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
        return Err<Ptr>("Failed to create YDrawMaze", result);
    }
    auto maze = std::dynamic_pointer_cast<YDrawMaze>(*result);
    if (!maze) {
        return Err<Ptr>("Created card is not a YDrawMaze");
    }
    return Ok(maze);
}

} // namespace yetty::card
