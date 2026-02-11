#include "kdraw.h"
#include "../hdraw/hdraw.h"  // For SDFPrimitive definition
#include <yetty/ms-msdf-font.h>
#include <yetty/font-manager.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <fstream>

namespace yetty::card {

//=============================================================================
// AABB utilities (shared with hdraw/ydraw)
//=============================================================================

static void computeAABB_kdraw(SDFPrimitive& prim) {
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
        case SDFType::Ellipse: {
            prim.aabbMinX = prim.params[0] - prim.params[2] - expand;
            prim.aabbMinY = prim.params[1] - prim.params[3] - expand;
            prim.aabbMaxX = prim.params[0] + prim.params[2] + expand;
            prim.aabbMaxY = prim.params[1] + prim.params[3] + expand;
            break;
        }
        case SDFType::Bezier2: {
            prim.aabbMinX = std::min({prim.params[0], prim.params[2], prim.params[4]}) - expand;
            prim.aabbMinY = std::min({prim.params[1], prim.params[3], prim.params[5]}) - expand;
            prim.aabbMaxX = std::max({prim.params[0], prim.params[2], prim.params[4]}) + expand;
            prim.aabbMaxY = std::max({prim.params[1], prim.params[3], prim.params[5]}) + expand;
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
            prim.aabbMinX = prim.params[0] - hw;
            prim.aabbMinY = prim.params[1] - hw;
            prim.aabbMaxX = prim.params[0] + hw;
            prim.aabbMaxY = prim.params[1] + hw;
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
            // For other types, use conservative bounds
            prim.aabbMinX = -1e10f;
            prim.aabbMinY = -1e10f;
            prim.aabbMaxX = 1e10f;
            prim.aabbMaxY = 1e10f;
            break;
    }
}

//=============================================================================
// KDrawImpl - Implementation with compute shader tile culling
//=============================================================================

class KDrawImpl : public KDraw {
public:
    KDrawImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : KDraw(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _fontManager(ctx.fontManager)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        // Get default MSDF font for text rendering
        if (_fontManager) {
            _font = _fontManager->getDefaultMsMsdfFont();
        }
    }

    ~KDrawImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("KDraw::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        yinfo("KDraw::init: allocated metadata at offset {}", _metaHandle.offset);

        // Create compute pipeline for tile culling
        if (auto res = createComputePipeline(); !res) {
            return Err<void>("KDraw::init: failed to create compute pipeline");
        }

        // Parse args
        parseArgs(_argsStr);

        // Parse payload into CPU staging (GPU buffers allocated by lifecycle loops)
        _initParsing = true;
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("KDraw::init: failed to parse payload: {}", error_msg(res));
            }
        }
        _initParsing = false;

        yinfo("KDraw::init: parsed {} primitives into staging", _primStaging.size());
        _dirty = true;
        _metadataDirty = true;

        return Ok();
    }

    Result<void> dispose() override {
        // Release compute resources
        if (_tileCullPipeline) {
            wgpuComputePipelineRelease(_tileCullPipeline);
            _tileCullPipeline = nullptr;
        }
        if (_tileCullBindGroupLayout) {
            wgpuBindGroupLayoutRelease(_tileCullBindGroupLayout);
            _tileCullBindGroupLayout = nullptr;
        }
        if (_tileCullBindGroup) {
            wgpuBindGroupRelease(_tileCullBindGroup);
            _tileCullBindGroup = nullptr;
        }
        if (_tileListBuffer) {
            wgpuBufferRelease(_tileListBuffer);
            _tileListBuffer = nullptr;
        }
        if (_tileCullUniformBuffer) {
            wgpuBufferRelease(_tileCullUniformBuffer);
            _tileCullUniformBuffer = nullptr;
        }
        if (_tileCullShaderModule) {
            wgpuShaderModuleRelease(_tileCullShaderModule);
            _tileCullShaderModule = nullptr;
        }

        if (_derivedStorage.isValid() && _cardMgr) {
            _derivedStorage = StorageHandle::invalid();
            _tileLists = nullptr;
            _tileListsSize = 0;
        }

        if (_primStorage.isValid() && _cardMgr) {
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }

        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("KDraw::dispose: deallocateMetadata failed: {}", error_msg(res));
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

        // Invalidate derived storage (tile lists — will be rebuilt)
        _derivedStorage = StorageHandle::invalid();
        _tileLists = nullptr;
        _tileListsSize = 0;

        // Invalidate prim storage
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;

        yinfo("KDraw::suspend: deallocated storage, saved {} primitives to staging",
              _primStaging.size());
    }

    void calculate() {
        // Compute scene bounds from staging
        if (!_hasExplicitBounds) {
            _sceneMinX = 1e10f; _sceneMinY = 1e10f;
            _sceneMaxX = -1e10f; _sceneMaxY = -1e10f;
            for (const auto& prim : _primStaging) {
                _sceneMinX = std::min(_sceneMinX, prim.aabbMinX);
                _sceneMinY = std::min(_sceneMinY, prim.aabbMinY);
                _sceneMaxX = std::max(_sceneMaxX, prim.aabbMaxX);
                _sceneMaxY = std::max(_sceneMaxY, prim.aabbMaxY);
            }
            for (const auto& g : _glyphs) {
                _sceneMinX = std::min(_sceneMinX, g.x);
                _sceneMinY = std::min(_sceneMinY, g.y);
                _sceneMaxX = std::max(_sceneMaxX, g.x + g.width);
                _sceneMaxY = std::max(_sceneMaxY, g.y + g.height);
            }
            float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
            float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
            _sceneMinX -= padX; _sceneMinY -= padY;
            _sceneMaxX += padX; _sceneMaxY += padY;
            if (_sceneMinX >= _sceneMaxX) { _sceneMinX = 0; _sceneMaxX = 100; }
            if (_sceneMinY >= _sceneMaxY) { _sceneMinY = 0; _sceneMaxY = 100; }
        }

        // Compute tile dimensions
        const uint32_t ESTIMATED_CELL_PIXELS = 16;
        uint32_t pixelWidth = _widthCells * ESTIMATED_CELL_PIXELS;
        uint32_t pixelHeight = _heightCells * ESTIMATED_CELL_PIXELS;
        _tileCountX = (pixelWidth + TILE_SIZE - 1) / TILE_SIZE;
        _tileCountY = (pixelHeight + TILE_SIZE - 1) / TILE_SIZE;
        uint32_t totalTiles = _tileCountX * _tileCountY;
        uint32_t tileStride = 1 + MAX_PRIMS_PER_TILE;
        uint32_t tileTotalU32 = totalTiles * tileStride;

        // Build tile lists into staging from _primStaging
        _tileListsStaging.assign(tileTotalU32, 0);

        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;

        for (uint32_t ty = 0; ty < _tileCountY; ty++) {
            for (uint32_t tx = 0; tx < _tileCountX; tx++) {
                float tileMinX = _sceneMinX + (float(tx) / _tileCountX) * sceneWidth;
                float tileMaxX = _sceneMinX + (float(tx + 1) / _tileCountX) * sceneWidth;
                float tileMinY = _sceneMinY + (float(ty) / _tileCountY) * sceneHeight;
                float tileMaxY = _sceneMinY + (float(ty + 1) / _tileCountY) * sceneHeight;

                uint32_t tileIndex = ty * _tileCountX + tx;
                uint32_t tileOffset = tileIndex * tileStride;
                uint32_t count = 0;

                for (uint32_t pi = 0; pi < static_cast<uint32_t>(_primStaging.size()) && count < MAX_PRIMS_PER_TILE; pi++) {
                    const auto& prim = _primStaging[pi];
                    if (prim.aabbMaxX >= tileMinX && prim.aabbMinX <= tileMaxX &&
                        prim.aabbMaxY >= tileMinY && prim.aabbMinY <= tileMaxY) {
                        _tileListsStaging[tileOffset + 1 + count] = pi;
                        count++;
                    }
                }
                _tileListsStaging[tileOffset] = count;
            }
        }
    }

    void declareBufferNeeds() override {
        // Save existing GPU data to staging and deallocate
        if (_primStorage.isValid() && _primCount > 0) {
            _primStaging.resize(_primCount);
            std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }
        _derivedStorage = StorageHandle::invalid();
        _tileLists = nullptr;
        _tileListsSize = 0;

        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
        if (_tileCullBindGroup) {
            wgpuBindGroupRelease(_tileCullBindGroup);
            _tileCullBindGroup = nullptr;
        }

        if (_primStaging.empty() && _glyphs.empty()) {
            _tileListsStaging.clear();
            return;
        }

        // Reserve prim storage
        if (!_primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(_primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
        }

        // Only calculate if derived staging is empty (first time or data changed)
        if (_tileListsStaging.empty()) {
            using Clock = std::chrono::steady_clock;
            auto t0 = Clock::now();
            calculate();
            auto t1 = Clock::now();
            auto calcUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

            std::cout << "KDraw::calculate: " << calcUs << " us ("
                      << _primStaging.size() << " prims, " << _glyphs.size() << " glyphs, tiles="
                      << _tileCountX << "x" << _tileCountY << ")" << std::endl;
        }

        // Reserve exact derived size (tile lists + glyphs)
        uint32_t tileBytes = static_cast<uint32_t>(_tileListsStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(KDrawGlyph));
        uint32_t derivedSize = tileBytes + glyphBytes;
        if (derivedSize > 0) {
            _cardMgr->bufferManager()->reserve(derivedSize);
        }
    }

    Result<void> allocateBuffers() override {
        // Restore primitives from staging
        if (!_primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(_primStaging.size());
            if (auto res = ensurePrimCapacity(count); !res) {
                return Err<void>("KDraw::allocateBuffers: failed to allocate prim storage");
            }
            _primCount = count;
            std::memcpy(_primitives, _primStaging.data(), count * sizeof(SDFPrimitive));
            _primStaging.clear();
            _primStaging.shrink_to_fit();
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        // Allocate derived storage and copy pre-built tile lists + glyphs
        uint32_t tileBytes = static_cast<uint32_t>(_tileListsStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(KDrawGlyph));
        uint32_t derivedSize = tileBytes + glyphBytes;

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("KDraw::allocateBuffers: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            // Copy pre-built tile lists
            _tileLists = reinterpret_cast<uint32_t*>(base + offset);
            _tileListsSize = static_cast<uint32_t>(_tileListsStaging.size());
            _tileListOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_tileListsStaging.empty()) {
                std::memcpy(base + offset, _tileListsStaging.data(), tileBytes);
            }
            offset += tileBytes;

            // Copy glyphs
            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_glyphs.empty()) {
                std::memcpy(base + offset, _glyphs.data(), glyphBytes);
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        _metadataDirty = true;
        _dirty = false;  // Tile lists already built in calculate()

        yinfo("KDraw::allocateBuffers: {} primitives, {} glyphs, derived {} bytes",
              _primCount, _glyphs.size(), derivedSize);
        return Ok();
    }

    void renderToStaging(float time) override {
        (void)time;

        if (_dirty) {
            if (auto res = rebuildAndUpload(); !res) {
                yerror("KDraw::renderToStaging: rebuildAndUpload failed: {}", error_msg(res));
            }
            _dirty = false;
        }
    }

    Result<void> render() override {
        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("KDraw::render: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // KDraw-specific API
    //=========================================================================

    uint32_t addPrimitive(const SDFPrimitive& prim) override {
        // During init parsing, accumulate in CPU staging (no GPU allocation)
        if (_initParsing) {
            SDFPrimitive p = prim;
            if (p.aabbMinX == 0 && p.aabbMaxX == 0) {
                computeAABB_kdraw(p);
            }
            uint32_t idx = static_cast<uint32_t>(_primStaging.size());
            _primStaging.push_back(p);
            return idx;
        }

        if (auto res = ensurePrimCapacity(_primCount + 1); !res) {
            yerror("KDraw::addPrimitive: failed to grow storage");
            return 0;
        }
        uint32_t idx = _primCount++;
        _primitives[idx] = prim;
        if (_primitives[idx].aabbMinX == 0 && _primitives[idx].aabbMaxX == 0) {
            computeAABB_kdraw(_primitives[idx]);
        }
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        _dirty = true;
        _tileListsStaging.clear();  // Invalidate — will recalculate on next recompaction
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

    uint32_t addText(float x, float y, const std::string& text,
                     float fontSize, uint32_t color,
                     uint32_t layer) override {
        if (!_font || text.empty()) {
            return 0;
        }

        float fontBaseSize = _font->atlas()->getFontSize();
        float scale = fontSize / fontBaseSize;

        float cursorX = x;
        uint32_t glyphsAdded = 0;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t codepoint = 0;
            if ((*ptr & 0x80) == 0) {
                codepoint = *ptr++;
            } else if ((*ptr & 0xE0) == 0xC0) {
                codepoint = (*ptr++ & 0x1F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF0) == 0xE0) {
                codepoint = (*ptr++ & 0x0F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else if ((*ptr & 0xF8) == 0xF0) {
                codepoint = (*ptr++ & 0x07) << 18;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 12;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F) << 6;
                if (ptr < end) codepoint |= (*ptr++ & 0x3F);
            } else {
                ptr++;
                continue;
            }

            uint32_t glyphIndex = _font->getGlyphIndex(codepoint);
            const auto& metadata = _font->atlas()->getGlyphMetadata();
            if (glyphIndex >= metadata.size()) {
                cursorX += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];

            KDrawGlyph posGlyph = {};
            posGlyph.x = cursorX + glyph._bearingX * scale;
            posGlyph.y = y - glyph._bearingY * scale;
            posGlyph.width = glyph._sizeX * scale;
            posGlyph.height = glyph._sizeY * scale;
            posGlyph.glyphIndex = glyphIndex;
            posGlyph.color = color;
            posGlyph.layer = layer;

            _glyphs.push_back(posGlyph);
            glyphsAdded++;

            cursorX += glyph._advance * scale;
        }

        if (glyphsAdded > 0) {
            _dirty = true;
        }

        return glyphsAdded;
    }

    void clear() override {
        _primCount = 0;
        _tileListsSize = 0;
        _glyphs.clear();
        _dirty = true;
    }

    uint32_t primitiveCount() const override {
        return _primCount;
    }

    uint32_t glyphCount() const override {
        return static_cast<uint32_t>(_glyphs.size());
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

    void runTileCulling() override {
        // This will be called by the render system before fragment shading
        // For now, tile culling is done on CPU in rebuildAndUpload
        // TODO: Implement actual GPU compute shader dispatch
    }

private:
    Result<void> ensurePrimCapacity(uint32_t required) {
        if (_primStorage.isValid() && required <= _primCapacity) {
            return Ok();
        }
        uint32_t newCap = std::max(required, _primCapacity == 0 ? 64u : _primCapacity * 2);
        uint32_t newSize = newCap * sizeof(SDFPrimitive);

        auto newStorage = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "prims", newSize);
        if (!newStorage) {
            return Err<void>("KDraw: failed to allocate prim storage");
        }

        if (_primCount > 0 && _primStorage.isValid()) {
            std::memcpy(newStorage->data, _primStorage.data, _primCount * sizeof(SDFPrimitive));
        }

        _primStorage = *newStorage;
        _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
        _primCapacity = newCap;
        return Ok();
    }

    void parseArgs(const std::string& args) {
        yinfo("KDraw::parseArgs: args='{}'", args);

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
            } else if (token == "--show-bounds") {
                _flags |= FLAG_SHOW_BOUNDS;
            } else if (token == "--show-tiles") {
                _flags |= FLAG_SHOW_TILES;
            } else if (token == "--show-eval-count") {
                _flags |= FLAG_SHOW_EVAL_COUNT;
            }
        }

        _metadataDirty = true;
    }

    Result<void> parsePayload(const std::string& payload) {
        yinfo("KDraw::parsePayload: payload length={}", payload.size());

        if (payload.empty()) {
            return Ok();
        }

        // Detect if payload is YAML (text) or binary
        bool isYaml = false;
        if (payload.size() >= 4) {
            char first = payload[0];
            if (first == '#' || first == '-' || first == ' ' || first == '\n' ||
                first == '\t' || (first >= 'a' && first <= 'z') ||
                (first >= 'A' && first <= 'Z')) {
                isYaml = true;
            }
        }

        if (isYaml) {
            return parseYAML(payload);
        } else {
            return parseBinary(payload);
        }
    }

    Result<void> parseBinary(const std::string& payload) {
        const size_t HEADER_SIZE = 16;
        const size_t PRIM_SIZE = sizeof(SDFPrimitive);

        if (payload.size() < HEADER_SIZE) {
            return Ok();
        }

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());

        uint32_t version, primCount, bgColor, flags;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColor, data + 8, 4);
        std::memcpy(&flags, data + 12, 4);

        yinfo("KDraw::parseBinary: version={} primCount={} bgColor={:#010x} flags={}",
              version, primCount, bgColor, flags);

        if (version != 1 && version != 2) {
            return Ok();
        }

        _bgColor = bgColor;
        _flags |= flags;
        _metadataDirty = true;

        size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
        if (payload.size() < expectedSize) {
            primCount = static_cast<uint32_t>((payload.size() - HEADER_SIZE) / PRIM_SIZE);
        }

        const uint8_t* primData = data + HEADER_SIZE;

        if (_initParsing) {
            _primStaging.resize(primCount);
            std::memcpy(_primStaging.data(), primData, primCount * PRIM_SIZE);
            _dirty = true;
            return Ok();
        }

        if (auto res = ensurePrimCapacity(primCount); !res) {
            return Err<void>("KDraw::parseBinary: failed to allocate prim storage");
        }
        _primCount = primCount;
        std::memcpy(_primitives, primData, primCount * PRIM_SIZE);
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);

        _dirty = true;
        return Ok();
    }

    //=========================================================================
    // YAML Parsing (same as ydraw)
    //=========================================================================

    static uint32_t parseColor(const YAML::Node& node) {
        if (!node) return 0xFFFFFFFF;

        std::string str = node.as<std::string>();
        if (str.empty()) return 0xFFFFFFFF;

        if (str[0] == '#') {
            str = str.substr(1);
            if (str.size() == 3) {
                str = std::string{str[0], str[0], str[1], str[1], str[2], str[2]};
            }
            if (str.size() == 6) {
                str += "FF";
            }
            uint32_t r = std::stoul(str.substr(0, 2), nullptr, 16);
            uint32_t g = std::stoul(str.substr(2, 2), nullptr, 16);
            uint32_t b = std::stoul(str.substr(4, 2), nullptr, 16);
            uint32_t a = std::stoul(str.substr(6, 2), nullptr, 16);
            return (a << 24) | (b << 16) | (g << 8) | r;
        }
        return 0xFFFFFFFF;
    }

    Result<void> parseYAML(const std::string& yaml) {
        try {
            YAML::Node root = YAML::Load(yaml);

            if (root["background"]) {
                _bgColor = parseColor(root["background"]);
                _metadataDirty = true;
            }

            if (root["flags"]) {
                auto flagsNode = root["flags"];
                if (flagsNode.IsSequence()) {
                    for (const auto& flag : flagsNode) {
                        std::string f = flag.as<std::string>();
                        if (f == "show_bounds") _flags |= FLAG_SHOW_BOUNDS;
                        else if (f == "show_tiles") _flags |= FLAG_SHOW_TILES;
                        else if (f == "show_eval_count") _flags |= FLAG_SHOW_EVAL_COUNT;
                    }
                }
                _metadataDirty = true;
            }

            if (root["body"] && root["body"].IsSequence()) {
                for (const auto& item : root["body"]) {
                    parseYAMLPrimitive(item);
                }
            }

            _dirty = true;
            yinfo("KDraw::parseYAML: loaded {} primitives, {} glyphs",
                  _primCount, _glyphs.size());

            // Log first few primitives for debugging
            for (size_t i = 0; i < std::min(size_t(_primCount), size_t(3)); i++) {
                const auto& p = _primitives[i];
                yinfo("KDraw: prim[{}] type={} aabb=[{},{},{},{}] fill={:#010x}",
                      i, p.type, p.aabbMinX, p.aabbMinY, p.aabbMaxX, p.aabbMaxY, p.fillColor);
            }

            return Ok();

        } catch (const YAML::Exception& e) {
            return Err<void>(std::string("YAML parse error: ") + e.what());
        }
    }

    void parseYAMLPrimitive(const YAML::Node& item) {
        uint32_t layer = static_cast<uint32_t>(_primCount + _glyphs.size());

        // Text
        if (item["text"]) {
            auto t = item["text"];
            float x = 0, y = 0, fontSize = 16;
            uint32_t color = 0xFFFFFFFF;
            std::string content;

            if (t["position"] && t["position"].IsSequence()) {
                x = t["position"][0].as<float>();
                y = t["position"][1].as<float>();
            }
            if (t["font-size"]) fontSize = t["font-size"].as<float>();
            if (t["fontSize"]) fontSize = t["fontSize"].as<float>();
            if (t["color"]) color = parseColor(t["color"]);
            if (t["content"]) content = t["content"].as<std::string>();

            addText(x, y, content, fontSize, color, layer);
            return;
        }

        // Circle
        if (item["circle"]) {
            auto c = item["circle"];
            float cx = 0, cy = 0, r = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (c["position"] && c["position"].IsSequence()) {
                cx = c["position"][0].as<float>();
                cy = c["position"][1].as<float>();
            }
            if (c["radius"]) r = c["radius"].as<float>();
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

            addCircle(cx, cy, r, fill, stroke, strokeWidth, layer);
            return;
        }

        // Box
        if (item["box"]) {
            auto b = item["box"];
            float cx = 0, cy = 0, hw = 10, hh = 10, round = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (b["position"] && b["position"].IsSequence()) {
                cx = b["position"][0].as<float>();
                cy = b["position"][1].as<float>();
            }
            if (b["size"] && b["size"].IsSequence()) {
                hw = b["size"][0].as<float>() / 2;
                hh = b["size"][1].as<float>() / 2;
            }
            if (b["fill"]) fill = parseColor(b["fill"]);
            if (b["stroke"]) stroke = parseColor(b["stroke"]);
            if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
            if (b["round"]) round = b["round"].as<float>();

            addBox(cx, cy, hw, hh, fill, stroke, strokeWidth, round, layer);
            return;
        }

        // Segment
        if (item["segment"]) {
            auto s = item["segment"];
            float x0 = 0, y0 = 0, x1 = 100, y1 = 100;
            uint32_t stroke = 0xFFFFFFFF;
            float strokeWidth = 1;

            if (s["from"] && s["from"].IsSequence()) {
                x0 = s["from"][0].as<float>();
                y0 = s["from"][1].as<float>();
            }
            if (s["to"] && s["to"].IsSequence()) {
                x1 = s["to"][0].as<float>();
                y1 = s["to"][1].as<float>();
            }
            if (s["stroke"]) stroke = parseColor(s["stroke"]);
            if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();

            addSegment(x0, y0, x1, y1, stroke, strokeWidth, layer);
            return;
        }

        // Triangle
        if (item["triangle"]) {
            auto t = item["triangle"];
            float x0 = 0, y0 = 0, x1 = 50, y1 = 100, x2 = 100, y2 = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (t["p0"] && t["p0"].IsSequence()) {
                x0 = t["p0"][0].as<float>();
                y0 = t["p0"][1].as<float>();
            }
            if (t["p1"] && t["p1"].IsSequence()) {
                x1 = t["p1"][0].as<float>();
                y1 = t["p1"][1].as<float>();
            }
            if (t["p2"] && t["p2"].IsSequence()) {
                x2 = t["p2"][0].as<float>();
                y2 = t["p2"][1].as<float>();
            }
            if (t["fill"]) fill = parseColor(t["fill"]);
            if (t["stroke"]) stroke = parseColor(t["stroke"]);
            if (t["stroke-width"]) strokeWidth = t["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Triangle);
            prim.layer = layer;
            prim.params[0] = x0; prim.params[1] = y0;
            prim.params[2] = x1; prim.params[3] = y1;
            prim.params[4] = x2; prim.params[5] = y2;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Ellipse
        if (item["ellipse"]) {
            auto e = item["ellipse"];
            float cx = 0, cy = 0, rx = 20, ry = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (e["position"] && e["position"].IsSequence()) {
                cx = e["position"][0].as<float>();
                cy = e["position"][1].as<float>();
            }
            if (e["radii"] && e["radii"].IsSequence()) {
                rx = e["radii"][0].as<float>();
                ry = e["radii"][1].as<float>();
            }
            if (e["fill"]) fill = parseColor(e["fill"]);
            if (e["stroke"]) stroke = parseColor(e["stroke"]);
            if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Ellipse);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = rx; prim.params[3] = ry;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Arc
        if (item["arc"]) {
            auto a = item["arc"];
            float cx = 0, cy = 0, ra = 20, rb = 2;
            float angle = 90.0f;
            uint32_t fill = 0, stroke = 0xFFFFFFFF;
            float strokeWidth = 0;

            if (a["position"] && a["position"].IsSequence()) {
                cx = a["position"][0].as<float>();
                cy = a["position"][1].as<float>();
            }
            if (a["angle"]) angle = a["angle"].as<float>();
            if (a["radius"]) ra = a["radius"].as<float>();
            if (a["thickness"]) rb = a["thickness"].as<float>();
            if (a["fill"]) fill = parseColor(a["fill"]);
            if (a["stroke"]) stroke = parseColor(a["stroke"]);
            if (a["stroke-width"]) strokeWidth = a["stroke-width"].as<float>();

            float rad = angle * 3.14159265f / 180.0f;
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Arc);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
            prim.params[4] = ra; prim.params[5] = rb;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Pentagon
        if (item["pentagon"]) {
            auto p = item["pentagon"];
            float cx = 0, cy = 0, r = 20;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (p["position"] && p["position"].IsSequence()) {
                cx = p["position"][0].as<float>();
                cy = p["position"][1].as<float>();
            }
            if (p["radius"]) r = p["radius"].as<float>();
            if (p["fill"]) fill = parseColor(p["fill"]);
            if (p["stroke"]) stroke = parseColor(p["stroke"]);
            if (p["stroke-width"]) strokeWidth = p["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Pentagon);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Hexagon
        if (item["hexagon"]) {
            auto h = item["hexagon"];
            float cx = 0, cy = 0, r = 20;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (h["position"] && h["position"].IsSequence()) {
                cx = h["position"][0].as<float>();
                cy = h["position"][1].as<float>();
            }
            if (h["radius"]) r = h["radius"].as<float>();
            if (h["fill"]) fill = parseColor(h["fill"]);
            if (h["stroke"]) stroke = parseColor(h["stroke"]);
            if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Hexagon);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Star
        if (item["star"]) {
            auto s = item["star"];
            float cx = 0, cy = 0, r = 20;
            float n = 5, m = 2.5f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (s["position"] && s["position"].IsSequence()) {
                cx = s["position"][0].as<float>();
                cy = s["position"][1].as<float>();
            }
            if (s["radius"]) r = s["radius"].as<float>();
            if (s["points"]) n = s["points"].as<float>();
            if (s["inner"]) m = s["inner"].as<float>();
            if (s["fill"]) fill = parseColor(s["fill"]);
            if (s["stroke"]) stroke = parseColor(s["stroke"]);
            if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Star);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = r; prim.params[3] = n; prim.params[4] = m;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Pie
        if (item["pie"]) {
            auto p = item["pie"];
            float cx = 0, cy = 0, r = 20;
            float angle = 45.0f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (p["position"] && p["position"].IsSequence()) {
                cx = p["position"][0].as<float>();
                cy = p["position"][1].as<float>();
            }
            if (p["angle"]) angle = p["angle"].as<float>();
            if (p["radius"]) r = p["radius"].as<float>();
            if (p["fill"]) fill = parseColor(p["fill"]);
            if (p["stroke"]) stroke = parseColor(p["stroke"]);
            if (p["stroke-width"]) strokeWidth = p["stroke-width"].as<float>();

            float rad = angle * 3.14159265f / 180.0f;
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Pie);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
            prim.params[4] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Ring
        if (item["ring"]) {
            auto rg = item["ring"];
            float cx = 0, cy = 0, r = 20, th = 4;
            float angle = 0.0f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (rg["position"] && rg["position"].IsSequence()) {
                cx = rg["position"][0].as<float>();
                cy = rg["position"][1].as<float>();
            }
            if (rg["angle"]) angle = rg["angle"].as<float>();
            if (rg["radius"]) r = rg["radius"].as<float>();
            if (rg["thickness"]) th = rg["thickness"].as<float>();
            if (rg["fill"]) fill = parseColor(rg["fill"]);
            if (rg["stroke"]) stroke = parseColor(rg["stroke"]);
            if (rg["stroke-width"]) strokeWidth = rg["stroke-width"].as<float>();

            float rad = angle * 3.14159265f / 180.0f;
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Ring);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad);
            prim.params[4] = r; prim.params[5] = th;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Heart
        if (item["heart"]) {
            auto h = item["heart"];
            float cx = 0, cy = 0, scale = 20;
            uint32_t fill = 0xFF0000FF, stroke = 0;
            float strokeWidth = 0;

            if (h["position"] && h["position"].IsSequence()) {
                cx = h["position"][0].as<float>();
                cy = h["position"][1].as<float>();
            }
            if (h["scale"]) scale = h["scale"].as<float>();
            if (h["fill"]) fill = parseColor(h["fill"]);
            if (h["stroke"]) stroke = parseColor(h["stroke"]);
            if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Heart);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = scale;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Cross
        if (item["cross"]) {
            auto c = item["cross"];
            float cx = 0, cy = 0, w = 20, h = 5, r = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (c["position"] && c["position"].IsSequence()) {
                cx = c["position"][0].as<float>();
                cy = c["position"][1].as<float>();
            }
            if (c["size"] && c["size"].IsSequence()) {
                w = c["size"][0].as<float>();
                h = c["size"][1].as<float>();
            }
            if (c["round"]) r = c["round"].as<float>();
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Cross);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = w; prim.params[3] = h; prim.params[4] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // RoundedX
        if (item["rounded_x"]) {
            auto x = item["rounded_x"];
            float cx = 0, cy = 0, w = 20, r = 3;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (x["position"] && x["position"].IsSequence()) {
                cx = x["position"][0].as<float>();
                cy = x["position"][1].as<float>();
            }
            if (x["width"]) w = x["width"].as<float>();
            if (x["round"]) r = x["round"].as<float>();
            if (x["fill"]) fill = parseColor(x["fill"]);
            if (x["stroke"]) stroke = parseColor(x["stroke"]);
            if (x["stroke-width"]) strokeWidth = x["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::RoundedX);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = w; prim.params[3] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Capsule
        if (item["capsule"]) {
            auto c = item["capsule"];
            float x0 = 0, y0 = 0, x1 = 100, y1 = 0, r = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (c["from"] && c["from"].IsSequence()) {
                x0 = c["from"][0].as<float>();
                y0 = c["from"][1].as<float>();
            }
            if (c["to"] && c["to"].IsSequence()) {
                x1 = c["to"][0].as<float>();
                y1 = c["to"][1].as<float>();
            }
            if (c["radius"]) r = c["radius"].as<float>();
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Capsule);
            prim.layer = layer;
            prim.params[0] = x0; prim.params[1] = y0;
            prim.params[2] = x1; prim.params[3] = y1;
            prim.params[4] = r;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Rhombus
        if (item["rhombus"]) {
            auto r = item["rhombus"];
            float cx = 0, cy = 0, w = 20, h = 30;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (r["position"] && r["position"].IsSequence()) {
                cx = r["position"][0].as<float>();
                cy = r["position"][1].as<float>();
            }
            if (r["size"] && r["size"].IsSequence()) {
                w = r["size"][0].as<float>();
                h = r["size"][1].as<float>();
            }
            if (r["fill"]) fill = parseColor(r["fill"]);
            if (r["stroke"]) stroke = parseColor(r["stroke"]);
            if (r["stroke-width"]) strokeWidth = r["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Rhombus);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = w; prim.params[3] = h;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Moon
        if (item["moon"]) {
            auto m = item["moon"];
            float cx = 0, cy = 0, d = 10, ra = 20, rb = 15;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (m["position"] && m["position"].IsSequence()) {
                cx = m["position"][0].as<float>();
                cy = m["position"][1].as<float>();
            }
            if (m["distance"]) d = m["distance"].as<float>();
            if (m["radius_outer"]) ra = m["radius_outer"].as<float>();
            if (m["radius_inner"]) rb = m["radius_inner"].as<float>();
            if (m["fill"]) fill = parseColor(m["fill"]);
            if (m["stroke"]) stroke = parseColor(m["stroke"]);
            if (m["stroke-width"]) strokeWidth = m["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Moon);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = d; prim.params[3] = ra; prim.params[4] = rb;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Egg
        if (item["egg"]) {
            auto e = item["egg"];
            float cx = 0, cy = 0, ra = 20, rb = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;

            if (e["position"] && e["position"].IsSequence()) {
                cx = e["position"][0].as<float>();
                cy = e["position"][1].as<float>();
            }
            if (e["radius_bottom"]) ra = e["radius_bottom"].as<float>();
            if (e["radius_top"]) rb = e["radius_top"].as<float>();
            if (e["fill"]) fill = parseColor(e["fill"]);
            if (e["stroke"]) stroke = parseColor(e["stroke"]);
            if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();

            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Egg);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = ra; prim.params[3] = rb;
            prim.fillColor = fill;
            prim.strokeColor = stroke;
            prim.strokeWidth = strokeWidth;
            computeAABB_kdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Bezier
        if (item["bezier"]) {
            auto b = item["bezier"];
            float x0 = 0, y0 = 0, x1 = 50, y1 = 50, x2 = 100, y2 = 0;
            uint32_t stroke = 0xFFFFFFFF;
            float strokeWidth = 1;

            if (b["p0"] && b["p0"].IsSequence()) {
                x0 = b["p0"][0].as<float>();
                y0 = b["p0"][1].as<float>();
            }
            if (b["p1"] && b["p1"].IsSequence()) {
                x1 = b["p1"][0].as<float>();
                y1 = b["p1"][1].as<float>();
            }
            if (b["p2"] && b["p2"].IsSequence()) {
                x2 = b["p2"][0].as<float>();
                y2 = b["p2"][1].as<float>();
            }
            if (b["stroke"]) stroke = parseColor(b["stroke"]);
            if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();

            addBezier2(x0, y0, x1, y1, x2, y2, stroke, strokeWidth, layer);
            return;
        }
    }

    //=========================================================================
    // Compute Pipeline Setup
    //=========================================================================

    Result<void> createComputePipeline() {
        // Load compute shader source
        const char* shaderPath = CMAKE_SOURCE_DIR "/src/yetty/shaders/cards/0x0004-kdraw-compute.wgsl";
        yinfo("KDraw: Loading compute shader from: {}", shaderPath);

        std::ifstream file(shaderPath);
        if (!file.is_open()) {
            // Compute shader not found - fall back to CPU tile culling
            ywarn("KDraw: Compute shader not found, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shaderSource = buffer.str();

        // Create shader module
        WGPUShaderSourceWGSL wgslDesc = {};
        wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
        wgslDesc.code = { shaderSource.c_str(), shaderSource.size() };

        WGPUShaderModuleDescriptor moduleDesc = {};
        moduleDesc.nextInChain = &wgslDesc.chain;
        moduleDesc.label = { "KDrawTileCullShader", 20 };

        _tileCullShaderModule = wgpuDeviceCreateShaderModule(_gpu.device, &moduleDesc);
        if (!_tileCullShaderModule) {
            ywarn("KDraw: Failed to create compute shader module, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        // Create bind group layout for tile culling compute shader
        // Binding 0: Primitives (read-only storage)
        // Binding 1: Tile lists (read-write storage)
        // Binding 2: Uniforms (uniform buffer)
        std::array<WGPUBindGroupLayoutEntry, 3> layoutEntries = {};

        layoutEntries[0].binding = 0;
        layoutEntries[0].visibility = WGPUShaderStage_Compute;
        layoutEntries[0].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

        layoutEntries[1].binding = 1;
        layoutEntries[1].visibility = WGPUShaderStage_Compute;
        layoutEntries[1].buffer.type = WGPUBufferBindingType_Storage;

        layoutEntries[2].binding = 2;
        layoutEntries[2].visibility = WGPUShaderStage_Compute;
        layoutEntries[2].buffer.type = WGPUBufferBindingType_Uniform;

        WGPUBindGroupLayoutDescriptor layoutDesc = {};
        layoutDesc.label = { "KDrawTileCullLayout", 19 };
        layoutDesc.entryCount = layoutEntries.size();
        layoutDesc.entries = layoutEntries.data();

        _tileCullBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_gpu.device, &layoutDesc);
        if (!_tileCullBindGroupLayout) {
            ywarn("KDraw: Failed to create bind group layout, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        // Create pipeline layout
        WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
        pipelineLayoutDesc.bindGroupLayoutCount = 1;
        pipelineLayoutDesc.bindGroupLayouts = &_tileCullBindGroupLayout;

        WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_gpu.device, &pipelineLayoutDesc);
        if (!pipelineLayout) {
            ywarn("KDraw: Failed to create pipeline layout, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        // Create compute pipeline
        WGPUComputePipelineDescriptor pipelineDesc = {};
        pipelineDesc.label = { "KDrawTileCullPipeline", 21 };
        pipelineDesc.layout = pipelineLayout;
        pipelineDesc.compute.module = _tileCullShaderModule;
        pipelineDesc.compute.entryPoint = { "tileCull", 8 };

        _tileCullPipeline = wgpuDeviceCreateComputePipeline(_gpu.device, &pipelineDesc);
        wgpuPipelineLayoutRelease(pipelineLayout);

        if (!_tileCullPipeline) {
            ywarn("KDraw: Failed to create compute pipeline, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        // Create uniform buffer for compute shader
        WGPUBufferDescriptor uniformDesc = {};
        uniformDesc.label = { "KDrawTileCullUniforms", 21 };
        uniformDesc.size = sizeof(TileCullUniforms);
        uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;

        _tileCullUniformBuffer = wgpuDeviceCreateBuffer(_gpu.device, &uniformDesc);
        if (!_tileCullUniformBuffer) {
            ywarn("KDraw: Failed to create uniform buffer, using CPU tile culling");
            _useComputeShader = false;
            return Ok();
        }

        _useComputeShader = true;
        yinfo("KDraw: Compute pipeline created successfully");
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

        for (const auto& glyph : _glyphs) {
            _sceneMinX = std::min(_sceneMinX, glyph.x);
            _sceneMinY = std::min(_sceneMinY, glyph.y);
            _sceneMaxX = std::max(_sceneMaxX, glyph.x + glyph.width);
            _sceneMaxY = std::max(_sceneMaxY, glyph.y + glyph.height);
        }

        float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
        float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
        _sceneMinX -= padX;
        _sceneMinY -= padY;
        _sceneMaxX += padX;
        _sceneMaxY += padY;

        if (_sceneMinX >= _sceneMaxX) {
            _sceneMinX = 0;
            _sceneMaxX = 100;
        }
        if (_sceneMinY >= _sceneMaxY) {
            _sceneMinY = 0;
            _sceneMaxY = 100;
        }
    }

    void buildTileLists() {
        // CPU-based tile culling
        uint32_t tileStride = 1 + MAX_PRIMS_PER_TILE;  // [count][idx0][idx1]...

        std::memset(_tileLists, 0, _tileListsSize * sizeof(uint32_t));

        // For each tile, find primitives that overlap
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;

        for (uint32_t ty = 0; ty < _tileCountY; ty++) {
            for (uint32_t tx = 0; tx < _tileCountX; tx++) {
                // Tile bounds in scene coordinates
                float tileMinX = _sceneMinX + (float(tx) / _tileCountX) * sceneWidth;
                float tileMaxX = _sceneMinX + (float(tx + 1) / _tileCountX) * sceneWidth;
                float tileMinY = _sceneMinY + (float(ty) / _tileCountY) * sceneHeight;
                float tileMaxY = _sceneMinY + (float(ty + 1) / _tileCountY) * sceneHeight;

                uint32_t tileIndex = ty * _tileCountX + tx;
                uint32_t tileOffset = tileIndex * tileStride;
                uint32_t count = 0;

                // Test each primitive against tile bounds
                for (uint32_t pi = 0; pi < _primCount && count < MAX_PRIMS_PER_TILE; pi++) {
                    const auto& prim = _primitives[pi];

                    // AABB overlap test
                    if (prim.aabbMaxX >= tileMinX && prim.aabbMinX <= tileMaxX &&
                        prim.aabbMaxY >= tileMinY && prim.aabbMinY <= tileMaxY) {
                        _tileLists[tileOffset + 1 + count] = pi;
                        count++;
                    }
                }

                _tileLists[tileOffset] = count;
            }
        }
    }

    Result<void> rebuildAndUpload() {
        computeSceneBounds();

        // Calculate tile dimensions
        const uint32_t ESTIMATED_CELL_PIXELS = 16;
        uint32_t pixelWidth = _widthCells * ESTIMATED_CELL_PIXELS;
        uint32_t pixelHeight = _heightCells * ESTIMATED_CELL_PIXELS;
        _tileCountX = (pixelWidth + TILE_SIZE - 1) / TILE_SIZE;
        _tileCountY = (pixelHeight + TILE_SIZE - 1) / TILE_SIZE;
        uint32_t totalTiles = _tileCountX * _tileCountY;
        uint32_t tileStride = 1 + MAX_PRIMS_PER_TILE;
        uint32_t tileTotalU32 = totalTiles * tileStride;
        uint32_t tileBytes = tileTotalU32 * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(KDrawGlyph));
        uint32_t derivedTotalSize = tileBytes + glyphBytes;

        if (_derivedStorage.isValid() && derivedTotalSize > 0) {
            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            // Set up tile list pointer directly into buffer
            _tileLists = reinterpret_cast<uint32_t*>(base + offset);
            _tileListsSize = tileTotalU32;
            _tileListOffset = (_derivedStorage.offset + offset) / sizeof(float);
            offset += tileBytes;

            // Build tile lists directly into buffer
            if (_primCount > 0 && !_useComputeShader) {
                buildTileLists();
            }

            // Glyphs (small data)
            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_glyphs.empty()) {
                std::memcpy(base + offset, _glyphs.data(), glyphBytes);
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        // Primitives are already in _primStorage
        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;

        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        _metadataDirty = true;

        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("KDraw::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.tileListOffset = _tileListOffset;
        meta.tileCountX = _tileCountX;
        meta.tileCountY = _tileCountY;
        meta.tileSize = TILE_SIZE;
        meta.glyphOffset = _glyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
        std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
        meta.widthCells = _widthCells;
        meta.heightCells = _heightCells;
        meta.flags = _flags;
        meta.bgColor = _bgColor;

        yinfo("KDraw::uploadMetadata: metaOffset={} slotIndex={} primOffset={} prims={} tiles={}x{} "
              "scene=[{},{},{},{}] size={}x{} bgColor={:#010x}",
              _metaHandle.offset, _metaHandle.offset / 64, meta.primitiveOffset,
              meta.primitiveCount, meta.tileCountX, meta.tileCountY,
              _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY,
              meta.widthCells, meta.heightCells, meta.bgColor);

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("KDraw::uploadMetadata: write failed");
        }

        return Ok();
    }

    // Metadata structure (matches shader layout - 64 bytes)
    struct Metadata {
        uint32_t primitiveOffset;   // 0
        uint32_t primitiveCount;    // 4
        uint32_t tileListOffset;    // 8
        uint32_t tileCountX;        // 12
        uint32_t tileCountY;        // 16
        uint32_t tileSize;          // 20
        uint32_t glyphOffset;       // 24
        uint32_t glyphCount;        // 28
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

    // Uniforms for compute shader
    struct TileCullUniforms {
        uint32_t primitiveCount;
        uint32_t tileCountX;
        uint32_t tileCountY;
        uint32_t maxPrimsPerTile;
        float sceneMinX;
        float sceneMinY;
        float sceneMaxX;
        float sceneMaxY;
    };

    // Primitive data - direct buffer storage
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _primStorage = StorageHandle::invalid();

    // CPU staging for primitives (used during suspend/reconstruct)
    std::vector<SDFPrimitive> _primStaging;

    // Tile lists + glyphs - built directly in derived storage
    uint32_t* _tileLists = nullptr;
    uint32_t _tileListsSize = 0;  // total uint32_t elements
    StorageHandle _derivedStorage = StorageHandle::invalid();
    std::vector<uint32_t> _tileListsStaging;  // Pre-built tile lists from calculate()

    // Glyphs (small, vector OK)
    std::vector<KDrawGlyph> _glyphs;

    // Tile dimensions
    uint32_t _tileCountX = 0;
    uint32_t _tileCountY = 0;

    // Scene bounds
    float _sceneMinX = 0, _sceneMinY = 0;
    float _sceneMaxX = 100, _sceneMaxY = 100;
    bool _hasExplicitBounds = false;

    // GPU offsets (in float units)
    uint32_t _primitiveOffset = 0;
    uint32_t _tileListOffset = 0;
    uint32_t _glyphOffset = 0;

    // Rendering
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;

    // (storage handles are _primStorage and _derivedStorage above)
    bool _dirty = true;
    bool _metadataDirty = true;
    bool _initParsing = false;

    std::string _argsStr;
    std::string _payloadStr;

    // Font for text rendering
    FontManager::Ptr _fontManager;
    MsMsdfFont::Ptr _font;

    // Compute shader resources
    bool _useComputeShader = false;
    WGPUShaderModule _tileCullShaderModule = nullptr;
    WGPUBindGroupLayout _tileCullBindGroupLayout = nullptr;
    WGPUBindGroup _tileCullBindGroup = nullptr;
    WGPUComputePipeline _tileCullPipeline = nullptr;
    WGPUBuffer _tileListBuffer = nullptr;
    WGPUBuffer _tileCullUniformBuffer = nullptr;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> KDraw::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("KDraw::create: pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!ctx.cardManager) {
        yerror("KDraw::create: null CardBufferManager!");
        return Err<CardPtr>("KDraw::create: null CardBufferManager");
    }

    auto card = std::make_shared<KDrawImpl>(
        ctx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("KDraw::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("KDraw::create: init failed");
    }

    yinfo("KDraw::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<KDraw::Ptr> KDraw::createImpl(
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
        return Err<Ptr>("Failed to create KDraw", result);
    }
    auto kdraw = std::dynamic_pointer_cast<KDraw>(*result);
    if (!kdraw) {
        return Err<Ptr>("Created card is not a KDraw");
    }
    return Ok(kdraw);
}

} // namespace yetty::card
