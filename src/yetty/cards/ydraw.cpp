#include "ydraw.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace yetty::card {

//=============================================================================
// Morton code utilities
//=============================================================================

// Expand 10-bit integer to 20-bit integer with zeros interleaved
static uint32_t expandBits(uint32_t v) {
    v = (v | (v << 16)) & 0x030000FF;
    v = (v | (v <<  8)) & 0x0300F00F;
    v = (v | (v <<  4)) & 0x030C30C3;
    v = (v | (v <<  2)) & 0x09249249;
    return v;
}

// Compute Morton code for 2D point (10 bits per axis = 1024x1024 grid)
static uint32_t morton2D(float x, float y,
                         float sceneMinX, float sceneMinY,
                         float sceneMaxX, float sceneMaxY) {
    // Normalize to 0-1
    float rangeX = sceneMaxX - sceneMinX;
    float rangeY = sceneMaxY - sceneMinY;
    if (rangeX < 1e-6f) rangeX = 1.0f;
    if (rangeY < 1e-6f) rangeY = 1.0f;

    float nx = (x - sceneMinX) / rangeX;
    float ny = (y - sceneMinY) / rangeY;

    // Clamp and convert to integer grid
    uint32_t ix = static_cast<uint32_t>(std::clamp(nx, 0.0f, 1.0f) * 1023.0f);
    uint32_t iy = static_cast<uint32_t>(std::clamp(ny, 0.0f, 1.0f) * 1023.0f);

    // Interleave bits
    return (expandBits(ix) << 1) | expandBits(iy);
}

//=============================================================================
// AABB utilities
//=============================================================================

static void computeAABB(SDFPrimitive& prim) {
    float expand = prim.strokeWidth * 0.5f;

    switch (static_cast<SDFType>(prim.type)) {
        case SDFType::Circle: {
            // params: [cx, cy, radius]
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Box: {
            // params: [cx, cy, halfW, halfH]
            float hw = prim.params[2] + prim.round + expand;
            float hh = prim.params[3] + prim.round + expand;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::Segment: {
            // params: [x0, y0, x1, y1]
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - expand;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - expand;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + expand;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + expand;
            break;
        }
        case SDFType::Triangle: {
            // params: [x0, y0, x1, y1, x2, y2]
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier2: {
            // params: [x0, y0, x1, y1, x2, y2] - quadratic bezier
            // Conservative AABB using control points
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
            break;
        }
        case SDFType::Bezier3: {
            // params: [x0, y0, x1, y1, x2, y2, x3, y3] - cubic bezier
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4], prim.params[6]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5], prim.params[7]}) + expand;
            break;
        }
        case SDFType::Ellipse: {
            // params: [cx, cy, rx, ry]
            prim.aabbMinX = prim.params[0] - prim.params[2] - expand;
            prim.aabbMinY = prim.params[1] - prim.params[3] - expand;
            prim.aabbMaxX = prim.params[0] + prim.params[2] + expand;
            prim.aabbMaxY = prim.params[1] + prim.params[3] + expand;
            break;
        }
        case SDFType::Arc: {
            // params: [cx, cy, sc.x, sc.y, ra, rb]
            float r = std::max(prim.params[4], prim.params[5]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::RoundedBox: {
            // params: [cx, cy, halfW, halfH, r0, r1, r2, r3]
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
            // params: [cx, cy, bx, by] - half diagonals
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
            // params: [cx, cy, r]
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Star: {
            // params: [cx, cy, r, n, m]
            float r = prim.params[2] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Pie: {
            // params: [cx, cy, sin, cos, r]
            float r = prim.params[4] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Ring: {
            // params: [cx, cy, nx, ny, r, th]
            float r = prim.params[4] + prim.params[5] + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Heart: {
            // params: [cx, cy, scale]
            float s = prim.params[2] * 1.5f + expand;  // Heart extends ~1.5 units
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Cross: {
            // params: [cx, cy, bx, by, r]
            float hw = std::max(prim.params[2], prim.params[3]) + expand;
            float hh = hw;
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hh;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hh;
            break;
        }
        case SDFType::RoundedX: {
            // params: [cx, cy, w, r]
            float s = prim.params[2] + prim.params[3] + expand;
            prim.aabbMinX = prim.params[0] - s;
            prim.aabbMinY = prim.params[1] - s;
            prim.aabbMaxX = prim.params[0] + s;
            prim.aabbMaxY = prim.params[1] + s;
            break;
        }
        case SDFType::Capsule: {
            // params: [x0, y0, x1, y1, r]
            float r = prim.params[4] + expand;
            prim.aabbMinX = std::min(prim.params[0], prim.params[2]) - r;
            prim.aabbMinY = std::min(prim.params[1], prim.params[3]) - r;
            prim.aabbMaxX = std::max(prim.params[0], prim.params[2]) + r;
            prim.aabbMaxY = std::max(prim.params[1], prim.params[3]) + r;
            break;
        }
        case SDFType::Moon: {
            // params: [cx, cy, d, ra, rb]
            float r = std::max(prim.params[3], prim.params[4]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r + prim.params[2];
            prim.aabbMaxY = prim.params[1] + r;
            break;
        }
        case SDFType::Egg: {
            // params: [cx, cy, ra, rb]
            float r = std::max(prim.params[2], prim.params[3]) + expand;
            prim.aabbMinX = prim.params[0] - r;
            prim.aabbMinY = prim.params[1] - r;
            prim.aabbMaxX = prim.params[0] + r;
            prim.aabbMaxY = prim.params[1] + r + prim.params[2];
            break;
        }
        default:
            // Unknown type - use large bounds
            prim.aabbMinX = -1e10f;
            prim.aabbMinY = -1e10f;
            prim.aabbMaxX = 1e10f;
            prim.aabbMaxY = 1e10f;
            break;
    }
}

//=============================================================================
// YDrawImpl - Implementation of YDraw
//=============================================================================

class YDrawImpl : public YDraw {
public:
    YDrawImpl(CardBufferManager::Ptr mgr, const GPUContext& gpu,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YDraw(std::move(mgr), gpu, x, y, widthCells, heightCells)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~YDrawImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() override {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("YDraw::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("YDraw::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Parse payload if provided (binary format from Python)
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("YDraw::init: failed to parse payload: {}", error_msg(res));
            }
        }

        // Build BVH and upload primitives to GPU storage
        if (!_primitives.empty()) {
            yinfo("YDraw::init: calling rebuildAndUpload for {} primitives", _primitives.size());
            rebuildAndUpload();
            _dirty = false;
        }

        // Upload metadata (now with correct scene bounds and offsets)
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("YDraw::init: failed to upload metadata");
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

        ydebug("YDraw::update called, _dirty={} _metadataDirty={} primCount={}",
               _dirty, _metadataDirty, _primitives.size());

        if (_dirty) {
            ydebug("YDraw::update: calling rebuildAndUpload()");
            rebuildAndUpload();
            _dirty = false;
        }

        if (_metadataDirty) {
            ydebug("YDraw::update: calling uploadMetadata()");
            uploadMetadata();
            _metadataDirty = false;
        }
    }

    //=========================================================================
    // YDraw-specific API
    //=========================================================================

    uint32_t addPrimitive(const SDFPrimitive& prim) override {
        uint32_t idx = static_cast<uint32_t>(_primitives.size());
        _primitives.push_back(prim);

        // Compute AABB if not set
        if (_primitives.back().aabbMinX == 0 && _primitives.back().aabbMaxX == 0) {
            computeAABB(_primitives.back());
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

    uint32_t addTextSpan(float x, float y, const std::string& text,
                         float fontSize, uint32_t color,
                         uint32_t fontId, uint32_t layer) override {
        uint32_t idx = static_cast<uint32_t>(_textSpans.size());

        TextSpan span = {};
        span.layer = layer;
        span.x = x;
        span.y = y;
        span.fontSize = fontSize;
        span.color = color;
        span.textOffset = static_cast<uint32_t>(_textBuffer.size());
        span.textLength = static_cast<uint32_t>(text.size());
        span.fontId = fontId;

        _textSpans.push_back(span);
        _textBuffer.insert(_textBuffer.end(), text.begin(), text.end());

        _dirty = true;
        return idx;
    }

    void clear() override {
        _primitives.clear();
        _textSpans.clear();
        _textBuffer.clear();
        _bvhNodes.clear();
        _sortedIndices.clear();
        _dirty = true;
    }

    uint32_t primitiveCount() const override {
        return static_cast<uint32_t>(_primitives.size());
    }

    uint32_t textSpanCount() const override {
        return static_cast<uint32_t>(_textSpans.size());
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

private:
    void parseArgs(const std::string& args) {
        yinfo("YDraw::parseArgs: args='{}'", args);

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X") {
                        colorStr = colorStr.substr(2);
                    }
                    // Color is already in ABGR format from Python plugin
                    _bgColor = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
                    yinfo("YDraw::parseArgs: bgColor={:#010x}", _bgColor);
                }
            } else if (token == "--show-bounds") {
                _flags |= FLAG_SHOW_BOUNDS;
            } else if (token == "--show-bvh") {
                _flags |= FLAG_SHOW_BVH;
            }
        }

        _metadataDirty = true;
    }

    Result<void> parsePayload(const std::string& payload) {
        yinfo("YDraw::parsePayload: payload length={}", payload.size());

        if (payload.empty()) {
            yinfo("YDraw::parsePayload: empty payload, nothing to parse");
            return Ok();
        }

        // Binary format from Python plugin:
        // Header: [version (u32)][primitive_count (u32)][bg_color (u32)][flags (u32)]
        // Primitives: [SDFPrimitive * primitive_count] (each 96 bytes)
        const size_t HEADER_SIZE = 16;
        const size_t PRIM_SIZE = sizeof(SDFPrimitive);

        if (payload.size() < HEADER_SIZE) {
            ywarn("YDraw::parsePayload: payload too small for header ({})", payload.size());
            return Ok();
        }

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());

        // Read header
        uint32_t version, primCount, bgColor, flags;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColor, data + 8, 4);
        std::memcpy(&flags, data + 12, 4);

        yinfo("YDraw::parsePayload: version={} primCount={} bgColor={:#010x} flags={}",
              version, primCount, bgColor, flags);

        if (version != 1) {
            ywarn("YDraw::parsePayload: unknown version {}, expected 1", version);
            return Ok();
        }

        // Update background color and flags from payload
        _bgColor = bgColor;
        _flags |= flags;
        _metadataDirty = true;

        // Read primitives
        size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
        if (payload.size() < expectedSize) {
            ywarn("YDraw::parsePayload: payload too small ({} < {})",
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

            yinfo("YDraw::parsePayload: prim[{}] type={} layer={} fill={:#010x} aabb=[{},{},{},{}]",
                  i, prim.type, prim.layer, prim.fillColor,
                  prim.aabbMinX, prim.aabbMinY, prim.aabbMaxX, prim.aabbMaxY);
        }

        _dirty = true;
        yinfo("YDraw::parsePayload: loaded {} primitives", _primitives.size());

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

    void buildBVH() {
        _bvhNodes.clear();
        _sortedIndices.clear();

        if (_primitives.empty()) {
            return;
        }

        // Compute scene bounds
        computeSceneBounds();

        // Compute Morton codes for each primitive's center
        std::vector<std::pair<uint32_t, uint32_t>> mortonPairs; // (morton, primIndex)
        for (uint32_t i = 0; i < _primitives.size(); i++) {
            float cx = (_primitives[i].aabbMinX + _primitives[i].aabbMaxX) * 0.5f;
            float cy = (_primitives[i].aabbMinY + _primitives[i].aabbMaxY) * 0.5f;
            uint32_t morton = morton2D(cx, cy, _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);
            mortonPairs.push_back({morton, i});
        }

        // Sort by Morton code
        std::sort(mortonPairs.begin(), mortonPairs.end());

        // Build sorted indices
        _sortedIndices.reserve(mortonPairs.size());
        for (const auto& p : mortonPairs) {
            _sortedIndices.push_back(p.second);
        }

        // Build BVH recursively
        buildBVHRecursive(0, static_cast<uint32_t>(mortonPairs.size()));

        yinfo("YDraw::buildBVH: {} primitives -> {} BVH nodes",
              _primitives.size(), _bvhNodes.size());
    }

    uint32_t buildBVHRecursive(uint32_t start, uint32_t end) {
        uint32_t nodeIndex = static_cast<uint32_t>(_bvhNodes.size());
        _bvhNodes.push_back(BVHNode{});
        // NOTE: Do NOT take a reference here - recursive calls may reallocate the vector!

        uint32_t count = end - start;

        if (count <= 4) {
            // Leaf node - safe to access by index since no recursive calls
            _bvhNodes[nodeIndex].primIndex = start;
            _bvhNodes[nodeIndex].primCount = count;
            _bvhNodes[nodeIndex].leftChild = 0xFFFFFFFF;
            _bvhNodes[nodeIndex].rightChild = 0xFFFFFFFF;

            // Compute AABB from primitives
            float aabbMinX = 1e10f;
            float aabbMinY = 1e10f;
            float aabbMaxX = -1e10f;
            float aabbMaxY = -1e10f;

            for (uint32_t i = start; i < end; i++) {
                const auto& prim = _primitives[_sortedIndices[i]];
                aabbMinX = std::min(aabbMinX, prim.aabbMinX);
                aabbMinY = std::min(aabbMinY, prim.aabbMinY);
                aabbMaxX = std::max(aabbMaxX, prim.aabbMaxX);
                aabbMaxY = std::max(aabbMaxY, prim.aabbMaxY);
            }

            _bvhNodes[nodeIndex].aabbMinX = aabbMinX;
            _bvhNodes[nodeIndex].aabbMinY = aabbMinY;
            _bvhNodes[nodeIndex].aabbMaxX = aabbMaxX;
            _bvhNodes[nodeIndex].aabbMaxY = aabbMaxY;
        } else {
            // Internal node
            uint32_t mid = start + count / 2;

            // Do recursive calls first - they may reallocate _bvhNodes
            uint32_t leftChild = buildBVHRecursive(start, mid);
            uint32_t rightChild = buildBVHRecursive(mid, end);

            // Now safe to access _bvhNodes[nodeIndex] - no more recursive calls
            _bvhNodes[nodeIndex].leftChild = leftChild;
            _bvhNodes[nodeIndex].rightChild = rightChild;
            _bvhNodes[nodeIndex].primCount = 0;

            // AABB is union of children
            const auto& left = _bvhNodes[leftChild];
            const auto& right = _bvhNodes[rightChild];
            _bvhNodes[nodeIndex].aabbMinX = std::min(left.aabbMinX, right.aabbMinX);
            _bvhNodes[nodeIndex].aabbMinY = std::min(left.aabbMinY, right.aabbMinY);
            _bvhNodes[nodeIndex].aabbMaxX = std::max(left.aabbMaxX, right.aabbMaxX);
            _bvhNodes[nodeIndex].aabbMaxY = std::max(left.aabbMaxY, right.aabbMaxY);
        }

        return nodeIndex;
    }

    void rebuildAndUpload() {
        // Build BVH
        buildBVH();

        // Calculate storage requirements
        // Layout: [primitives][bvhNodes][sortedIndices][textSpans][textBuffer]
        uint32_t primSize = static_cast<uint32_t>(_primitives.size() * sizeof(SDFPrimitive));
        uint32_t bvhSize = static_cast<uint32_t>(_bvhNodes.size() * sizeof(BVHNode));
        uint32_t indicesSize = static_cast<uint32_t>(_sortedIndices.size() * sizeof(uint32_t));
        uint32_t textSpanSize = static_cast<uint32_t>(_textSpans.size() * sizeof(TextSpan));
        uint32_t textBufSize = static_cast<uint32_t>(_textBuffer.size());

        uint32_t totalSize = primSize + bvhSize + indicesSize + textSpanSize + textBufSize;

        if (totalSize == 0) {
            return; // Nothing to upload
        }

        // Deallocate old storage
        if (_storageHandle.isValid()) {
            _cardMgr->deallocateStorage(_storageHandle);
            _storageHandle = StorageHandle::invalid();
        }

        // Allocate new storage
        auto storageResult = _cardMgr->allocateStorage(totalSize);
        if (!storageResult) {
            yerror("YDraw::rebuildAndUpload: failed to allocate {} bytes", totalSize);
            return;
        }
        _storageHandle = *storageResult;

        yinfo("YDraw::rebuildAndUpload: allocated {} bytes at offset {}",
              totalSize, _storageHandle.offset);

        // Upload data
        uint32_t offset = 0;

        // Primitives
        _primitiveOffset = _storageHandle.offset / sizeof(float);
        ydebug("YDraw::rebuildAndUpload: primitiveOffset={} (bytes={}) primCount={} primSize={}",
               _primitiveOffset, _storageHandle.offset, _primitives.size(), primSize);
        if (!_primitives.empty()) {
            auto res = _cardMgr->writeStorage(_storageHandle, _primitives.data(), primSize);
            if (!res) {
                yerror("YDraw::rebuildAndUpload: writeStorage FAILED: {}", error_msg(res));
            } else {
                ydebug("YDraw::rebuildAndUpload: writeStorage SUCCESS, first prim type={} aabb=[{},{},{},{}]",
                       _primitives[0].type, _primitives[0].aabbMinX, _primitives[0].aabbMinY,
                       _primitives[0].aabbMaxX, _primitives[0].aabbMaxY);
            }
        }
        offset += primSize;

        // BVH nodes
        _bvhOffset = (_storageHandle.offset + offset) / sizeof(float);
        if (!_bvhNodes.empty()) {
            StorageHandle bvhHandle = _storageHandle;
            bvhHandle.offset += offset;
            _cardMgr->writeStorage(bvhHandle, _bvhNodes.data(), bvhSize);
        }
        offset += bvhSize;

        // Sorted indices
        _sortedIndicesOffset = (_storageHandle.offset + offset) / sizeof(float);
        if (!_sortedIndices.empty()) {
            StorageHandle indicesHandle = _storageHandle;
            indicesHandle.offset += offset;
            _cardMgr->writeStorage(indicesHandle, _sortedIndices.data(), indicesSize);
        }
        offset += indicesSize;

        // Text spans
        _textSpanOffset = (_storageHandle.offset + offset) / sizeof(float);
        if (!_textSpans.empty()) {
            StorageHandle textSpanHandle = _storageHandle;
            textSpanHandle.offset += offset;
            _cardMgr->writeStorage(textSpanHandle, _textSpans.data(), textSpanSize);
        }
        offset += textSpanSize;

        // Text buffer
        _textBufferOffset = (_storageHandle.offset + offset) / sizeof(float);
        if (!_textBuffer.empty()) {
            StorageHandle textBufHandle = _storageHandle;
            textBufHandle.offset += offset;
            _cardMgr->writeStorage(textBufHandle, _textBuffer.data(), textBufSize);
        }

        _metadataDirty = true;
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("YDraw::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = static_cast<uint32_t>(_primitives.size());
        meta.textSpanOffset = _textSpanOffset;
        meta.textSpanCount = static_cast<uint32_t>(_textSpans.size());
        meta.bvhOffset = _bvhOffset;
        meta.bvhNodeCount = static_cast<uint32_t>(_bvhNodes.size());
        meta.glyphOffset = 0;  // TODO: positioned glyphs
        meta.glyphCount = 0;
        meta.sceneMinX = _sceneMinX;
        meta.sceneMinY = _sceneMinY;
        meta.sceneMaxX = _sceneMaxX;
        meta.sceneMaxY = _sceneMaxY;
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.flags = _flags;
        meta.bgColor = _bgColor;

        yinfo("YDraw::uploadMetadata: metaOffset={} slotIndex={} prims={} primOffset={} "
              "bvhNodes={} bvhOffset={} spans={} "
              "scene=[{},{},{},{}] size={}x{} bgColor={:#010x}",
              _metaHandle.offset, _metaHandle.offset / 32,
              meta.primitiveCount, meta.primitiveOffset,
              meta.bvhNodeCount, meta.bvhOffset,
              meta.textSpanCount, meta.sceneMinX, meta.sceneMinY,
              meta.sceneMaxX, meta.sceneMaxY, meta.widthCells, meta.heightCells,
              meta.bgColor);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("YDraw::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout)
    struct Metadata {
        uint32_t primitiveOffset;
        uint32_t primitiveCount;
        uint32_t textSpanOffset;
        uint32_t textSpanCount;
        uint32_t bvhOffset;
        uint32_t bvhNodeCount;
        uint32_t glyphOffset;
        uint32_t glyphCount;
        float sceneMinX;
        float sceneMinY;
        float sceneMaxX;
        float sceneMaxY;
        uint32_t widthCells;
        uint32_t heightCells;
        uint32_t flags;
        uint32_t bgColor;
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    // Primitive data
    std::vector<SDFPrimitive> _primitives;
    std::vector<BVHNode> _bvhNodes;
    std::vector<uint32_t> _sortedIndices;

    // Text data
    std::vector<TextSpan> _textSpans;
    std::vector<uint8_t> _textBuffer;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // GPU offsets (in float units)
    uint32_t _primitiveOffset = 0;
    uint32_t _bvhOffset = 0;
    uint32_t _sortedIndicesOffset = 0;
    uint32_t _textSpanOffset = 0;
    uint32_t _textBufferOffset = 0;

    // Rendering
    uint32_t _bgColor = 0xFF2E1A1A;     // Dark blue-ish background (ABGR format)
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

Result<CardPtr> YDraw::create(
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YDraw::create: ENTERED pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!mgr) {
        yerror("YDraw::create: null CardBufferManager!");
        return Err<CardPtr>("YDraw::create: null CardBufferManager");
    }

    auto card = std::make_shared<YDrawImpl>(
        std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);

    yinfo("YDraw::create: calling init()...");
    if (auto res = card->init(); !res) {
        yerror("YDraw::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YDraw::create: init failed");
    }

    yinfo("YDraw::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<YDraw::Ptr> YDraw::createImpl(
    ContextType& ctx,
    CardBufferManager::Ptr mgr,
    const GPUContext& gpu,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx; // ObjectFactory context marker

    auto result = create(std::move(mgr), gpu, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        return Err<Ptr>("Failed to create YDraw", result);
    }
    // Dynamic cast from CardPtr to YDraw::Ptr
    auto ydraw = std::dynamic_pointer_cast<YDraw>(*result);
    if (!ydraw) {
        return Err<Ptr>("Created card is not a YDraw");
    }
    return Ok(ydraw);
}

} // namespace yetty::card
