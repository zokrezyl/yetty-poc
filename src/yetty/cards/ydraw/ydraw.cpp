#include "ydraw.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <chrono>
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
    YDrawImpl(CardManager::Ptr mgr, const GPUContext& gpu,
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

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("YDraw::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("YDraw::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        parseArgs(_argsStr);

        // Parse payload into CPU staging (GPU buffers allocated via lifecycle loops)
        _initParsing = true;
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("YDraw::init: failed to parse payload: {}", error_msg(res));
            }
        }
        _initParsing = false;
        yinfo("YDraw::init: parsed {} primitives into staging", _primStaging.size());

        _dirty = true;
        _metadataDirty = true;

        return Ok();
    }

    Result<void> dispose() override {
        if (_derivedStorage.isValid() && _cardMgr) {
            if (auto res = _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage); !res) {
                yerror("YDraw::dispose: deallocateBuffer (derived) failed: {}", error_msg(res));
            }
            _derivedStorage = StorageHandle::invalid();
            _bvhNodes = nullptr;
            _sortedIndices = nullptr;
            _bvhNodeCount = 0;
        }

        if (_primStorage.isValid() && _cardMgr) {
            if (auto res = _cardMgr->bufferManager()->deallocateBuffer(_primStorage); !res) {
                yerror("YDraw::dispose: deallocateBuffer (prims) failed: {}", error_msg(res));
            }
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }

        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("YDraw::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }

        _primStaging.clear();
        _primStaging.shrink_to_fit();

        return Ok();
    }

    void suspend() override {
        // Save primitives from GPU buffer to CPU staging
        if (_primStorage.isValid() && _primCount > 0) {
            _primStaging.resize(_primCount);
            std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }

        // Deallocate derived storage (BVH, sorted indices — will be rebuilt)
        if (_derivedStorage.isValid()) {
            _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
            _derivedStorage = StorageHandle::invalid();
            _bvhNodes = nullptr;
            _sortedIndices = nullptr;
            _bvhNodeCount = 0;
        }

        // Deallocate prim storage
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }

        yinfo("YDraw::suspend: deallocated storage, saved {} primitives to staging",
              _primStaging.size());
    }

    uint32_t estimateDerivedSize() const {
        // BVH: ~2*primCount nodes, sorted indices: primCount
        uint32_t bvhSize = (2 * _primCount + 1) * sizeof(BVHNode);
        uint32_t indicesSize = _primCount * sizeof(uint32_t);
        uint32_t textSpanSize = static_cast<uint32_t>(_textSpans.size() * sizeof(TextSpan));
        uint32_t textBufSize = static_cast<uint32_t>(_textBuffer.size());
        return bvhSize + indicesSize + textSpanSize + textBufSize;
    }

    void declareBufferNeeds() override {
        // Save existing GPU data to staging and deallocate
        if (_primStorage.isValid() && _primCount > 0) {
            _primStaging.resize(_primCount);
            std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }
        uint32_t lastDerivedSize = _derivedStorage.size;
        if (_derivedStorage.isValid()) {
            _cardMgr->bufferManager()->deallocateBuffer(_derivedStorage);
            _derivedStorage = StorageHandle::invalid();
            _bvhNodes = nullptr;
            _sortedIndices = nullptr;
        }
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }
        // Reserve prim + derived
        if (!_primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(_primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
            if (lastDerivedSize > 0) {
                _cardMgr->bufferManager()->reserve(lastDerivedSize);
            } else {
                // First time: estimate from staging data
                uint32_t n = static_cast<uint32_t>(_primStaging.size());
                uint32_t est = (2 * n + 1) * sizeof(BVHNode) + n * sizeof(uint32_t)
                    + static_cast<uint32_t>(_textSpans.size() * sizeof(TextSpan))
                    + static_cast<uint32_t>(_textBuffer.size());
                _cardMgr->bufferManager()->reserve(est);
            }
        }
    }

    Result<void> allocateBuffers() override {
        if (!_primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(_primStaging.size());
            if (auto res = ensurePrimCapacity(count); !res) {
                return Err<void>("YDraw::allocateBuffers: failed to allocate prim storage");
            }
            _primCount = count;
            std::memcpy(_primitives, _primStaging.data(), count * sizeof(SDFPrimitive));
            _primStaging.clear();
            _primStaging.shrink_to_fit();
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);

            // Allocate derived storage (BVH + indices + text)
            uint32_t derivedSize = estimateDerivedSize();
            if (derivedSize > 0) {
                auto storageResult = _cardMgr->bufferManager()->allocateBuffer(derivedSize);
                if (!storageResult) {
                    return Err<void>("YDraw::allocateBuffers: failed to allocate derived storage");
                }
                _derivedStorage = *storageResult;
            }

            _dirty = true;
            yinfo("YDraw::allocateBuffers: reconstructed {} primitives, derived {} bytes", count, derivedSize);
        }
        return Ok();
    }

    Result<void> render(float time) override {
        (void)time;
        using Clock = std::chrono::steady_clock;
        auto tRenderStart = Clock::now();
        bool didRebuild = false, didMeta = false;

        ydebug("YDraw::update called, _dirty={} _metadataDirty={} primCount={}",
               _dirty, _metadataDirty, _primCount);

        if (_dirty) {
            ydebug("YDraw::update: calling rebuildAndUpload()");
            if (auto res = rebuildAndUpload(); !res) {
                return Err<void>("YDraw::update: rebuildAndUpload failed", res);
            }
            _dirty = false;
            didRebuild = true;
        }
        auto tAfterRebuild = Clock::now();

        if (_metadataDirty) {
            ydebug("YDraw::update: calling uploadMetadata()");
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("YDraw::update: metadata upload failed", res);
            }
            _metadataDirty = false;
            didMeta = true;
        }
        auto tAfterMeta = Clock::now();

        if (didRebuild || didMeta) {
            auto us = [](auto a, auto b) { return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count(); };
            yinfo("YDraw::render: rebuild={} us, metadata={} us, total={} us",
                  us(tRenderStart, tAfterRebuild), us(tAfterRebuild, tAfterMeta),
                  us(tRenderStart, tAfterMeta));
        }

        return Ok();
    }

    //=========================================================================
    // YDraw-specific API
    //=========================================================================

    uint32_t addPrimitive(const SDFPrimitive& prim) override {
        if (_initParsing) {
            SDFPrimitive p = prim;
            if (p.aabbMinX == 0 && p.aabbMaxX == 0) {
                computeAABB(p);
            }
            uint32_t idx = static_cast<uint32_t>(_primStaging.size());
            _primStaging.push_back(p);
            return idx;
        }
        if (auto res = ensurePrimCapacity(_primCount + 1); !res) {
            yerror("YDraw::addPrimitive: failed to grow storage");
            return 0;
        }
        uint32_t idx = _primCount++;
        _primitives[idx] = prim;

        // Compute AABB if not set
        if (_primitives[idx].aabbMinX == 0 && _primitives[idx].aabbMaxX == 0) {
            computeAABB(_primitives[idx]);
        }

        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
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
        _primCount = 0;
        _bvhNodeCount = 0;
        _textSpans.clear();
        _textBuffer.clear();
        _dirty = true;
    }

    uint32_t primitiveCount() const override {
        return _primCount;
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

        // Allocate buffer and copy directly from payload (no intermediate vector)
        if (auto res = ensurePrimCapacity(primCount); !res) {
            return Err<void>("YDraw::parsePayload: failed to allocate prim storage");
        }
        _primCount = primCount;
        std::memcpy(_primitives, primData, primCount * PRIM_SIZE);
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);

        for (uint32_t i = 0; i < primCount; i++) {
            yinfo("YDraw::parsePayload: prim[{}] type={} layer={} fill={:#010x} aabb=[{},{},{},{}]",
                  i, _primitives[i].type, _primitives[i].layer, _primitives[i].fillColor,
                  _primitives[i].aabbMinX, _primitives[i].aabbMinY, _primitives[i].aabbMaxX, _primitives[i].aabbMaxY);
        }

        _dirty = true;
        yinfo("YDraw::parsePayload: loaded {} primitives", _primCount);

        return Ok();
    }

    void computeSceneBounds() {
        if (_hasExplicitBounds) return;

        _sceneMinX = 1e10f;
        _sceneMinY = 1e10f;
        _sceneMaxX = -1e10f;
        _sceneMaxY = -1e10f;

        for (uint32_t i = 0; i < _primCount; i++) {
            _sceneMinX = std::min(_sceneMinX, _primitives[i].aabbMinX);
            _sceneMinY = std::min(_sceneMinY, _primitives[i].aabbMinY);
            _sceneMaxX = std::max(_sceneMaxX, _primitives[i].aabbMaxX);
            _sceneMaxY = std::max(_sceneMaxY, _primitives[i].aabbMaxY);
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

    // Build BVH into temporary vectors, returning sorted indices and nodes.
    // Called from rebuildAndUpload BEFORE buffer allocation so we know exact sizes.
    void buildBVH(std::vector<BVHNode>& outNodes, std::vector<uint32_t>& outSortedIndices) {
        if (_primCount == 0) return;

        auto t0 = std::chrono::steady_clock::now();

        // Compute Morton codes for spatial sorting
        std::vector<std::pair<uint32_t, uint32_t>> mortonPairs;
        mortonPairs.reserve(_primCount);
        for (uint32_t i = 0; i < _primCount; i++) {
            float cx = (_primitives[i].aabbMinX + _primitives[i].aabbMaxX) * 0.5f;
            float cy = (_primitives[i].aabbMinY + _primitives[i].aabbMaxY) * 0.5f;
            uint32_t morton = morton2D(cx, cy, _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY);
            mortonPairs.push_back({morton, i});
        }
        std::sort(mortonPairs.begin(), mortonPairs.end());

        outSortedIndices.resize(_primCount);
        for (uint32_t i = 0; i < _primCount; i++) {
            outSortedIndices[i] = mortonPairs[i].second;
        }

        // Build BVH recursively
        outNodes.reserve(_primCount);  // reasonable estimate
        buildBVHRecursive(outNodes, outSortedIndices, 0, _primCount);

        auto t1 = std::chrono::steady_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

        ydebug("YDraw::buildBVH (CPU): {} primitives -> {} BVH nodes, took {} us",
              _primCount, static_cast<uint32_t>(outNodes.size()), us);
    }

    uint32_t buildBVHRecursive(std::vector<BVHNode>& nodes,
                               const std::vector<uint32_t>& sortedIndices,
                               uint32_t start, uint32_t end) {
        uint32_t nodeIndex = static_cast<uint32_t>(nodes.size());
        nodes.push_back(BVHNode{});

        uint32_t count = end - start;

        if (count <= 4) {
            // Leaf node
            nodes[nodeIndex].primIndex = start;
            nodes[nodeIndex].primCount = count;
            nodes[nodeIndex].leftChild = 0xFFFFFFFF;
            nodes[nodeIndex].rightChild = 0xFFFFFFFF;

            float aabbMinX = 1e10f, aabbMinY = 1e10f;
            float aabbMaxX = -1e10f, aabbMaxY = -1e10f;
            for (uint32_t i = start; i < end; i++) {
                const auto& prim = _primitives[sortedIndices[i]];
                aabbMinX = std::min(aabbMinX, prim.aabbMinX);
                aabbMinY = std::min(aabbMinY, prim.aabbMinY);
                aabbMaxX = std::max(aabbMaxX, prim.aabbMaxX);
                aabbMaxY = std::max(aabbMaxY, prim.aabbMaxY);
            }
            nodes[nodeIndex].aabbMinX = aabbMinX;
            nodes[nodeIndex].aabbMinY = aabbMinY;
            nodes[nodeIndex].aabbMaxX = aabbMaxX;
            nodes[nodeIndex].aabbMaxY = aabbMaxY;
        } else {
            uint32_t mid = start + count / 2;
            uint32_t leftChild = buildBVHRecursive(nodes, sortedIndices, start, mid);
            uint32_t rightChild = buildBVHRecursive(nodes, sortedIndices, mid, end);

            nodes[nodeIndex].leftChild = leftChild;
            nodes[nodeIndex].rightChild = rightChild;
            nodes[nodeIndex].primCount = 0;

            const auto& left = nodes[leftChild];
            const auto& right = nodes[rightChild];
            nodes[nodeIndex].aabbMinX = std::min(left.aabbMinX, right.aabbMinX);
            nodes[nodeIndex].aabbMinY = std::min(left.aabbMinY, right.aabbMinY);
            nodes[nodeIndex].aabbMaxX = std::max(left.aabbMaxX, right.aabbMaxX);
            nodes[nodeIndex].aabbMaxY = std::max(left.aabbMaxY, right.aabbMaxY);
        }

        return nodeIndex;
    }

    Result<void> ensurePrimCapacity(uint32_t required) {
        if (_primStorage.isValid() && required <= _primCapacity) {
            return Ok();
        }
        uint32_t newCap = std::max(required, _primCapacity == 0 ? 64u : _primCapacity * 2);
        uint32_t newSize = newCap * sizeof(SDFPrimitive);

        auto newStorage = _cardMgr->bufferManager()->allocateBuffer(newSize);
        if (!newStorage) {
            return Err<void>("YDraw: failed to allocate prim storage");
        }

        if (_primCount > 0 && _primStorage.isValid()) {
            std::memcpy(newStorage->data, _primStorage.data, _primCount * sizeof(SDFPrimitive));
        }
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->deallocateBuffer(_primStorage);
        }

        _primStorage = *newStorage;
        _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
        _primCapacity = newCap;
        return Ok();
    }

    Result<void> rebuildAndUpload() {
        auto _rebuildT0 = std::chrono::steady_clock::now();
        if (_primCount == 0 && _textSpans.empty()) {
            return Ok();
        }

        // Compute scene bounds from primitives already in buffer
        computeSceneBounds();

        // Build BVH into temp storage first to determine exact sizes
        std::vector<BVHNode> tempBvhNodes;
        std::vector<uint32_t> tempSortedIndices;
        if (_primCount > 0) {
            buildBVH(tempBvhNodes, tempSortedIndices);
        }

        // Now we know exact sizes
        _bvhNodeCount = static_cast<uint32_t>(tempBvhNodes.size());
        uint32_t bvhSize = _bvhNodeCount * sizeof(BVHNode);
        uint32_t indicesSize = _primCount * sizeof(uint32_t);
        uint32_t textSpanSize = static_cast<uint32_t>(_textSpans.size() * sizeof(TextSpan));
        uint32_t textBufSize = static_cast<uint32_t>(_textBuffer.size());
        uint32_t derivedTotalSize = bvhSize + indicesSize + textSpanSize + textBufSize;

        if (_derivedStorage.isValid() && derivedTotalSize > 0) {
            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            // Copy BVH nodes into buffer (exact fit, no wasted space)
            _bvhNodes = reinterpret_cast<BVHNode*>(base + offset);
            _bvhOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (_bvhNodeCount > 0) {
                std::memcpy(_bvhNodes, tempBvhNodes.data(), bvhSize);
            }
            offset += bvhSize;

            // Copy sorted indices into buffer (right after BVH nodes)
            _sortedIndices = reinterpret_cast<uint32_t*>(base + offset);
            _sortedIndicesOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (_primCount > 0) {
                std::memcpy(_sortedIndices, tempSortedIndices.data(), indicesSize);
            }
            offset += indicesSize;

            // Text spans
            _textSpanOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_textSpans.empty()) {
                std::memcpy(base + offset, _textSpans.data(), textSpanSize);
            }
            offset += textSpanSize;

            // Text buffer
            _textBufferOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_textBuffer.empty()) {
                std::memcpy(base + offset, _textBuffer.data(), textBufSize);
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        // Primitives are already in _primStorage - just update offset
        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;

        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        _metadataDirty = true;

        auto _rebuildT1 = std::chrono::steady_clock::now();
        auto _rebuildUs = std::chrono::duration_cast<std::chrono::microseconds>(_rebuildT1 - _rebuildT0).count();
        yinfo("YDraw::rebuildAndUpload: {} prims, {} BVH nodes, derived {} bytes, took {} us",
              _primCount, _bvhNodeCount, derivedTotalSize, _rebuildUs);

        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("YDraw::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.textSpanOffset = _textSpanOffset;
        meta.textSpanCount = static_cast<uint32_t>(_textSpans.size());
        meta.bvhOffset = _bvhOffset;
        meta.bvhNodeCount = _bvhNodeCount;
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

    // Primitive data - direct buffer storage (no intermediate vector)
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _primStorage = StorageHandle::invalid();

    // CPU staging for primitives (used during suspend/reconstruct)
    std::vector<SDFPrimitive> _primStaging;

    // BVH data - built directly in derived storage buffer
    BVHNode* _bvhNodes = nullptr;
    uint32_t _bvhNodeCount = 0;
    uint32_t* _sortedIndices = nullptr;
    StorageHandle _derivedStorage = StorageHandle::invalid();

    // Text data (small, vector OK)
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
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;

    bool _dirty = true;
    bool _metadataDirty = true;
    bool _initParsing = false;

    std::string _argsStr;
    std::string _payloadStr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YDraw::create(
    CardManager::Ptr mgr,
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
    CardManager::Ptr mgr,
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
