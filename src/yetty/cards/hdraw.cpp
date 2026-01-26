#include "hdraw.h"
#include "ydraw.h"  // For SDFPrimitive definition
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace yetty::card {

//=============================================================================
// AABB utilities (shared with ydraw)
//=============================================================================

static void computeAABB_hdraw(SDFPrimitive& prim) {
    float expand = prim.strokeWidth * 0.5f;

    switch (static_cast<SDFType>(prim.type)) {
        case SDFType::Circle: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Box: {
            float hw = prim.params[2] + prim.round + expand;
            float hh = prim.params[3] + prim.round + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Segment: {
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - expand;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - expand;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + expand;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + expand;
            break;
        }
        case SDFType::Triangle: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier2: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier3: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) + expand;
            break;
        }
        case SDFType::Ellipse: {
            prim.aabbMinX = prim.params[0] - prim.params[2] - expand;
            prim.aabbMinY = prim.params[1] - prim.params[3] - expand;
            prim.aabbMaxX = prim.params[0] + prim.params[2] + expand;
            prim.aabbMaxY = prim.params[1] + prim.params[3] + expand;
            break;
        }
        case SDFType::Arc: {
            float r = std::max(prim.params[4], prim.params[5]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::RoundedBox: {
            float maxR = std::max({prim.params[4], prim.params[5],
                                   prim.params[6], prim.params[7]});
            float hw = prim.params[2] + maxR + expand;
            float hh = prim.params[3] + maxR + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Rhombus: {
            float hw = prim.params[2] + expand;
            float hh = prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Pentagon:
        case SDFType::Hexagon: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Star: {
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Pie: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Ring: {
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Heart: {
            float s = prim.params[2] * 1.5f + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Cross: {
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = hw;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::RoundedX: {
            float s = prim.params[2] + prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Capsule: {
            float r = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - r;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - r;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + r;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + r;
            break;
        }
        case SDFType::Moon: {
            float r = std::max(prim.params[3], prim.params[4]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r + prim.params[2];
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Egg: {
            float r = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r + prim.params[2];
            break;
        }
        default:
            prim.aabbMinX = -1e10f;
            prim.aabbMinY = -1e10f;
            prim.aabbMaxX = 1e10f;
            prim.aabbMaxY = 1e10f;
            break;
    }
}

//=============================================================================
// HDrawImpl - Implementation of HDraw with spatial hashing
//=============================================================================

class HDrawImpl : public HDraw {
public:
    HDrawImpl(CardBufferManager::Ptr mgr, const GPUContext& gpu,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : HDraw(std::move(mgr), gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~HDrawImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() override {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("HDraw::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("HDraw::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Parse payload if provided (binary format from Python)
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("HDraw::init: failed to parse payload: {}", error_msg(res));
            }
        }

        // Build grid and upload primitives to GPU storage
        if (!_primitives.empty()) {
            yinfo("HDraw::init: calling rebuildAndUpload for {} primitives", _primitives.size());
            rebuildAndUpload();
            _dirty = false;
        }

        // Upload metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("HDraw::init: failed to upload metadata");
        }
        _metadataDirty = false;

        return Ok();
    }

    void dispose() override {
        if (_storageHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateStorage(_storageHandle);
            _storageHandle = StorageHandle::invalid();
        }

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }
    }

    void update(float time) override {
        (void)time;

        if (_dirty) {
            rebuildAndUpload();
            _dirty = false;
        }

        if (_metadataDirty) {
            uploadMetadata();
            _metadataDirty = false;
        }
    }

    //=========================================================================
    // HDraw-specific API
    //=========================================================================

    uint32_t addPrimitive(const SDFPrimitive& prim) override {
        uint32_t idx = static_cast<uint32_t>(_primitives.size());
        _primitives.push_back(prim);

        // Compute AABB if not set
        if (_primitives.back().aabbMinX == 0 && _primitives.back().aabbMaxX == 0) {
            computeAABB_hdraw(_primitives.back());
        }

        _dirty = true;
        return idx;
    }

    uint32_t addCircle(float cx, float cy, float radius,
                       uint32_t fillColor, uint32_t strokeColor,
                       float strokeWidth, uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Circle);
        prim.layer = layer;
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = radius;
        prim.fillColor = fillColor;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    uint32_t addBox(float cx, float cy, float halfW, float halfH,
                    uint32_t fillColor, uint32_t strokeColor,
                    float strokeWidth, float round, uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Box);
        prim.layer = layer;
        prim.params[0] = cx;
        prim.params[1] = cy;
        prim.params[2] = halfW;
        prim.params[3] = halfH;
        prim.fillColor = fillColor;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        prim.round = round;
        return addPrimitive(prim);
    }

    uint32_t addSegment(float x0, float y0, float x1, float y1,
                        uint32_t strokeColor, float strokeWidth,
                        uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Segment);
        prim.layer = layer;
        prim.params[0] = x0;
        prim.params[1] = y0;
        prim.params[2] = x1;
        prim.params[3] = y1;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    uint32_t addBezier2(float x0, float y0, float x1, float y1,
                        float x2, float y2,
                        uint32_t strokeColor, float strokeWidth,
                        uint32_t layer) override {
        SDFPrimitive prim = {};
        prim.type = static_cast<uint32_t>(SDFType::Bezier2);
        prim.layer = layer;
        prim.params[0] = x0;
        prim.params[1] = y0;
        prim.params[2] = x1;
        prim.params[3] = y1;
        prim.params[4] = x2;
        prim.params[5] = y2;
        prim.strokeColor = strokeColor;
        prim.strokeWidth = strokeWidth;
        return addPrimitive(prim);
    }

    void clear() override {
        _primitives.clear();
        _grid.clear();
        _dirty = true;
    }

    uint32_t primitiveCount() const override {
        return static_cast<uint32_t>(_primitives.size());
    }

    void markDirty() override {
        _dirty = true;
    }

    void setBgColor(uint32_t color) override {
        _bgColor = color;
        _metadataDirty = true;
    }

    uint32_t bgColor() const override { return _bgColor; }

    void setSceneBounds(float minX, float minY, float maxX, float maxY) override {
        _sceneMinX = minX;
        _sceneMinY = minY;
        _sceneMaxX = maxX;
        _sceneMaxY = maxY;
        _hasExplicitBounds = true;
        _dirty = true;
    }

    bool hasExplicitBounds() const override { return _hasExplicitBounds; }

    void setCellSize(float size) override {
        _cellSize = size;
        _dirty = true;
    }

    float cellSize() const override { return _cellSize; }

private:
    void parseArgs(const std::string& args) {
        yinfo("HDraw::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    _bgColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
                }
            } else if (token == "--cell-size") {
                std::string sizeStr;
                if (iss >> sizeStr) {
                    _cellSize = std::stof(sizeStr);
                }
            } else if (token == "--max-prims-per-cell") {
                std::string maxStr;
                if (iss >> maxStr) {
                    _maxPrimsPerCell = static_cast<uint32_t>(std::stoul(maxStr));
                }
            } else if (token == "--show-bounds") {
                _flags |= FLAG_SHOW_BOUNDS;
            } else if (token == "--show-grid") {
                _flags |= FLAG_SHOW_GRID;
            } else if (token == "--show-eval-count") {
                _flags |= FLAG_SHOW_EVAL_COUNT;
            }
        }

        _metadataDirty = true;
    }

    Result<void> parsePayload(const std::string& payload) {
        yinfo("HDraw::parsePayload: payload length={}", payload.size());

        if (payload.empty()) {
            return Ok();
        }

        // Binary format from Python plugin:
        // Header: [version (u32)][primitive_count (u32)][bg_color (u32)][flags (u32)]
        // Primitives: [SDFPrimitive * primitive_count] (each 96 bytes)
        const size_t HEADER_SIZE = 16;
        const size_t PRIM_SIZE = sizeof(SDFPrimitive);

        if (payload.size() < HEADER_SIZE) {
            ywarn("HDraw::parsePayload: payload too small for header ({})", payload.size());
            return Ok();
        }

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());

        // Read header
        uint32_t version, primCount, bgColor, flags;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColor, data + 8, 4);
        std::memcpy(&flags, data + 12, 4);

        yinfo("HDraw::parsePayload: version={} primCount={} bgColor={:#010x} flags={}",
              version, primCount, bgColor, flags);

        if (version != 1) {
            ywarn("HDraw::parsePayload: unknown version {}, expected 1", version);
            return Ok();
        }

        _bgColor = bgColor;
        _flags |= flags;
        _metadataDirty = true;

        // Read primitives
        size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
        if (payload.size() < expectedSize) {
            ywarn("HDraw::parsePayload: payload too small ({} < {})",
                  payload.size(), expectedSize);
            primCount = static_cast<uint32_t>((payload.size() - HEADER_SIZE) / PRIM_SIZE);
        }

        const uint8_t* primData = data + HEADER_SIZE;
        _primitives.clear();
        _primitives.reserve(primCount);

        for (uint32_t i = 0; i < primCount; i++) {
            SDFPrimitive prim;
            std::memcpy(&prim, primData + i * PRIM_SIZE, PRIM_SIZE);
            _primitives.push_back(prim);
        }

        _dirty = true;
        yinfo("HDraw::parsePayload: loaded {} primitives", _primitives.size());

        return Ok();
    }

    void computeSceneBounds() {
        if (_hasExplicitBounds) return;

        _sceneMinX = 1e10f;
        _sceneMinY = 1e10f;
        _sceneMaxX = -1e10f;
        _sceneMaxY = -1e10f;

        for (const auto& prim : _primitives) {
            _sceneMinX = std::min(_sceneMinX, prim.aabbMinX);
            _sceneMinY = std::min(_sceneMinY, prim.aabbMinY);
            _sceneMaxX = std::max(_sceneMaxX, prim.aabbMaxX);
            _sceneMaxY = std::max(_sceneMaxY, prim.aabbMaxY);
        }

        // Add padding
        float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
        float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
        _sceneMinX -= padX;
        _sceneMinY -= padY;
        _sceneMaxX += padX;
        _sceneMaxY += padY;

        // Ensure valid bounds
        if (_sceneMinX >= _sceneMaxX) {
            _sceneMinX = 0;
            _sceneMaxX = 100;
        }
        if (_sceneMinY >= _sceneMaxY) {
            _sceneMinY = 0;
            _sceneMaxY = 100;
        }
    }

    void buildGrid() {
        _grid.clear();

        if (_primitives.empty()) {
            _gridWidth = 0;
            _gridHeight = 0;
            return;
        }

        // Compute scene bounds
        computeSceneBounds();

        // Compute grid dimensions
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;

        _gridWidth = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / _cellSize)));
        _gridHeight = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / _cellSize)));

        // Limit grid size to avoid excessive memory
        const uint32_t MAX_GRID_DIM = 256;
        if (_gridWidth > MAX_GRID_DIM) {
            _gridWidth = MAX_GRID_DIM;
            _cellSize = sceneWidth / _gridWidth;
        }
        if (_gridHeight > MAX_GRID_DIM) {
            _gridHeight = MAX_GRID_DIM;
            _cellSize = std::max(_cellSize, sceneHeight / _gridHeight);
        }

        uint32_t totalCells = _gridWidth * _gridHeight;
        uint32_t cellStride = 1 + _maxPrimsPerCell;  // [count][idx0][idx1]...

        // Initialize grid with zeros
        _grid.resize(totalCells * cellStride, 0);

        // Assign each primitive to cells it overlaps
        for (uint32_t primIdx = 0; primIdx < _primitives.size(); primIdx++) {
            const auto& prim = _primitives[primIdx];

            // Find cell range that overlaps this primitive's AABB
            uint32_t cellMinX = static_cast<uint32_t>(
                std::clamp((prim.aabbMinX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
            uint32_t cellMaxX = static_cast<uint32_t>(
                std::clamp((prim.aabbMaxX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
            uint32_t cellMinY = static_cast<uint32_t>(
                std::clamp((prim.aabbMinY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
            uint32_t cellMaxY = static_cast<uint32_t>(
                std::clamp((prim.aabbMaxY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

            // Add primitive to each overlapping cell
            for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
                for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                    uint32_t cellIndex = cy * _gridWidth + cx;
                    uint32_t cellOffset = cellIndex * cellStride;

                    // Read current count
                    uint32_t count = _grid[cellOffset];

                    // Add if not full
                    if (count < _maxPrimsPerCell) {
                        _grid[cellOffset + 1 + count] = primIdx;
                        _grid[cellOffset] = count + 1;
                    }
                }
            }
        }

        yinfo("HDraw::buildGrid: {} primitives -> {}x{} grid ({} cells), cellSize={}",
              _primitives.size(), _gridWidth, _gridHeight, totalCells, _cellSize);
    }

    void rebuildAndUpload() {
        // Build spatial hash grid
        buildGrid();

        // Calculate storage requirements
        // Layout: [primitives][grid]
        uint32_t primSize = static_cast<uint32_t>(_primitives.size() * sizeof(SDFPrimitive));
        uint32_t gridSize = static_cast<uint32_t>(_grid.size() * sizeof(uint32_t));

        uint32_t totalSize = primSize + gridSize;

        if (totalSize == 0) {
            return;
        }

        // Deallocate old storage
        if (_storageHandle.isValid()) {
            _cardMgr->deallocateStorage(_storageHandle);
            _storageHandle = StorageHandle::invalid();
        }

        // Allocate new storage
        auto storageResult = _cardMgr->allocateStorage(totalSize);
        if (!storageResult) {
            yerror("HDraw::rebuildAndUpload: failed to allocate {} bytes", totalSize);
            return;
        }
        _storageHandle = *storageResult;

        yinfo("HDraw::rebuildAndUpload: allocated {} bytes at offset {}",
              totalSize, _storageHandle.offset);

        // Upload data
        uint32_t offset = 0;

        // Primitives
        _primitiveOffset = _storageHandle.offset / sizeof(float);
        if (!_primitives.empty()) {
            _cardMgr->writeStorage(_storageHandle, _primitives.data(), primSize);
        }
        offset += primSize;

        // Grid
        _gridOffset = (_storageHandle.offset + offset) / sizeof(float);
        if (!_grid.empty()) {
            StorageHandle gridHandle = _storageHandle;
            gridHandle.offset += offset;
            _cardMgr->writeStorage(gridHandle, _grid.data(), gridSize);
        }

        _metadataDirty = true;
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("HDraw::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = static_cast<uint32_t>(_primitives.size());
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));  // Store float as u32 bits
        meta.maxPrimsPerCell = _maxPrimsPerCell;
        meta._reserved = 0;
        std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.flags = _flags;
        meta.bgColor = _bgColor;

        yinfo("HDraw::uploadMetadata: metaOffset={} prims={} grid={}x{} cellSize={} "
              "scene=[{},{},{},{}] size={}x{} bgColor={:#010x}",
              _metaHandle.offset, meta.primitiveCount, meta.gridWidth, meta.gridHeight,
              _cellSize, _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY,
              meta.widthCells, meta.heightCells, meta.bgColor);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("HDraw::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout - 64 bytes)
    struct Metadata {
        uint32_t primitiveOffset;   // 0
        uint32_t primitiveCount;    // 4
        uint32_t gridOffset;        // 8
        uint32_t gridWidth;         // 12
        uint32_t gridHeight;        // 16
        uint32_t cellSize;          // 20 (f32 stored as bits)
        uint32_t maxPrimsPerCell;   // 24
        uint32_t _reserved;         // 28
        uint32_t sceneMinX;         // 32 (f32 stored as bits)
        uint32_t sceneMinY;         // 36 (f32 stored as bits)
        uint32_t sceneMaxX;         // 40 (f32 stored as bits)
        uint32_t sceneMaxY;         // 44 (f32 stored as bits)
        uint32_t widthCells;        // 48
        uint32_t heightCells;       // 52
        uint32_t flags;             // 56
        uint32_t bgColor;           // 60
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    // Primitive data
    std::vector<SDFPrimitive> _primitives;

    // Spatial hash grid
    // Layout: for each cell [primCount][primIdx0][primIdx1]...[primIdxN]
    std::vector<uint32_t> _grid;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // Grid parameters
    float _cellSize = DEFAULT_CELL_SIZE;
    uint32_t _maxPrimsPerCell = DEFAULT_MAX_PRIMS_PER_CELL;

    // GPU offsets (in float units)
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;

    // Rendering
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;

    StorageHandle _storageHandle = StorageHandle::invalid();
    bool _dirty = true;
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> HDraw::create(
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("HDraw::create: pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("HDraw::create: null CardBufferManager!");
        return Err<CardPtr>("HDraw::create: null CardBufferManager");
    }

    auto card = std::make_shared<HDrawImpl>(
        std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("HDraw::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("HDraw::create: init failed");
    }

    yinfo("HDraw::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<HDraw::Ptr> HDraw::createImpl(
    ContextType& ctx,
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;

    auto result = create(std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create HDraw", result);
    }
    auto hdraw = std::dynamic_pointer_cast<HDraw>(*result);
    if (!hdraw) {
        return Err<Ptr>("Created card is not an HDraw");
    }
    return Ok(hdraw);
}

} // namespace yetty::card
