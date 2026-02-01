#include "hdraw.h"
#include "../ydraw/ydraw.h"  // For SDFPrimitive definition
#include <yetty/ms-msdf-font.h>
#include <yetty/font-manager.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
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
    HDrawImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : HDraw(ctx.cardManager->bufferManager(), ctx.gpu, x, y, widthCells, heightCells)
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

    ~HDrawImpl() override {
        dispose();
    }

    //=========================================================================
    // Card interface
    //=========================================================================

    Result<void> init() {
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

        // Build grid and upload primitives/glyphs to GPU storage
        if (_primCount > 0 || !_glyphs.empty()) {
            yinfo("HDraw::init: calling rebuildAndUpload for {} primitives, {} glyphs",
                  _primCount, _glyphs.size());
            if (auto res = rebuildAndUpload(); !res) {
                return Err<void>("HDraw::init: rebuildAndUpload failed", res);
            }
            _dirty = false;
        }

        // Upload metadata
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("HDraw::init: failed to upload metadata");
        }
        _metadataDirty = false;

        return Ok();
    }

    Result<void> dispose() override {
        _primStaging.clear();
        _primStaging.shrink_to_fit();

        if (_derivedStorage.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateBuffer(_derivedStorage); !res) {
                yerror("HDraw::dispose: deallocateBuffer (derived) failed: {}", error_msg(res));
            }
            _derivedStorage = StorageHandle::invalid();
            _grid = nullptr;
            _gridSize = 0;
        }

        if (_primStorage.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateBuffer(_primStorage); !res) {
                yerror("HDraw::dispose: deallocateBuffer (prims) failed: {}", error_msg(res));
            }
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }

        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("HDraw::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }

        return Ok();
    }

    void suspend() override {
        // Save primitives from GPU storage to CPU staging
        if (_primStorage.isValid() && _primCount > 0) {
            _primStaging.resize(_primCount);
            std::memcpy(_primStaging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }

        // Deallocate derived storage (grid + glyphs)
        if (_derivedStorage.isValid()) {
            _cardMgr->deallocateBuffer(_derivedStorage);
            _derivedStorage = StorageHandle::invalid();
            _grid = nullptr;
            _gridSize = 0;
        }

        // Deallocate primitive storage
        if (_primStorage.isValid()) {
            _cardMgr->deallocateBuffer(_primStorage);
            _primStorage = StorageHandle::invalid();
            _primitives = nullptr;
            _primCount = 0;
            _primCapacity = 0;
        }

        yinfo("HDraw::suspend: deallocated storage, saved {} primitives to staging", _primStaging.size());
    }

    Result<void> update(float time) override {
        (void)time;

        // Reconstruct storage after suspend
        if (!_primStorage.isValid() && !_primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(_primStaging.size());
            if (auto res = ensurePrimCapacity(count); !res) {
                return Err<void>("HDraw::update: failed to re-allocate prim storage");
            }
            _primCount = count;
            std::memcpy(_primitives, _primStaging.data(), count * sizeof(SDFPrimitive));
            _primStaging.clear();
            _primStaging.shrink_to_fit();
            _cardMgr->markBufferDirty(_primStorage);
            _dirty = true;
            yinfo("HDraw::update: reconstructed {} primitives from staging", count);
        }

        if (_dirty) {
            if (auto res = rebuildAndUpload(); !res) {
                return Err<void>("HDraw::update: rebuildAndUpload failed", res);
            }
            _dirty = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("HDraw::update: metadata upload failed", res);
            }
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // HDraw-specific API
    //=========================================================================

    uint32_t addPrimitive(const SDFPrimitive& prim) override {
        if (auto res = ensurePrimCapacity(_primCount + 1); !res) {
            yerror("HDraw::addPrimitive: failed to grow storage");
            return 0;
        }
        uint32_t idx = _primCount++;
        _primitives[idx] = prim;
        if (_primitives[idx].aabbMinX == 0 && _primitives[idx].aabbMaxX == 0) {
            computeAABB_hdraw(_primitives[idx]);
        }
        _cardMgr->markBufferDirty(_primStorage);
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

    uint32_t addText(float x, float y, const std::string& text,
                     float fontSize, uint32_t color,
                     uint32_t layer) override {
        if (!_font || text.empty()) {
            return 0;
        }

        // Get font metrics
        float fontBaseSize = _font->getFontSize();  // Size the glyphs were generated at
        float scale = fontSize / fontBaseSize;

        // Layout text horizontally
        float cursorX = x;
        uint32_t glyphsAdded = 0;

        // Iterate through UTF-8 text
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            // Decode UTF-8 codepoint
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
                ptr++;  // Invalid byte, skip
                continue;
            }

            // Get glyph index from font
            uint32_t glyphIndex = _font->getGlyphIndex(codepoint);
            if (glyphIndex == 0 && codepoint != ' ') {
                // Try to load the glyph (triggers lazy loading)
                glyphIndex = _font->getGlyphIndex(codepoint);
            }

            // Get glyph metrics
            const auto& metadata = _font->getGlyphMetadata();
            if (glyphIndex >= metadata.size()) {
                // Use space advance as fallback
                cursorX += fontSize * 0.5f;
                continue;
            }

            const auto& glyph = metadata[glyphIndex];

            // Create positioned glyph
            HDrawGlyph posGlyph = {};
            posGlyph.x = cursorX + glyph._bearingX * scale;
            posGlyph.y = y - glyph._bearingY * scale;  // Y is typically from baseline
            posGlyph.width = glyph._sizeX * scale;
            posGlyph.height = glyph._sizeY * scale;
            posGlyph.glyphIndex = glyphIndex;
            posGlyph.color = color;
            posGlyph.layer = layer;

            _glyphs.push_back(posGlyph);
            glyphsAdded++;

            // Advance cursor
            cursorX += glyph._advance * scale;
        }

        if (glyphsAdded > 0) {
            _dirty = true;
        }

        return glyphsAdded;
    }

    void clear() override {
        _primCount = 0;
        _gridSize = 0;
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

    void setCellSize(float size) override {
        _cellSize = size;
        _dirty = true;
    }

    float cellSize() const override { return _cellSize; }

private:
    Result<void> ensurePrimCapacity(uint32_t required) {
        if (_primStorage.isValid() && required <= _primCapacity) {
            return Ok();
        }
        uint32_t newCap = std::max(required, _primCapacity == 0 ? 64u : _primCapacity * 2);
        uint32_t newSize = newCap * sizeof(SDFPrimitive);

        auto newStorage = _cardMgr->allocateBuffer(newSize);
        if (!newStorage) {
            return Err<void>("HDraw: failed to allocate prim storage");
        }

        if (_primCount > 0 && _primStorage.isValid()) {
            std::memcpy(newStorage->data, _primStorage.data, _primCount * sizeof(SDFPrimitive));
        }
        if (_primStorage.isValid()) {
            _cardMgr->deallocateBuffer(_primStorage);
        }

        _primStorage = *newStorage;
        _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
        _primCapacity = newCap;
        return Ok();
    }

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

        // Detect if payload is YAML (text) or binary
        // YAML starts with printable characters like '#', '-', or whitespace
        // Binary starts with version number (little-endian u32)
        bool isYaml = false;
        if (payload.size() >= 4) {
            char first = payload[0];
            // YAML typically starts with '#', '-', whitespace, or letter
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
        // Binary format from Python plugin:
        // Header: [version (u32)][primitive_count (u32)][bg_color (u32)][flags (u32)]
        // Primitives: [SDFPrimitive * primitive_count] (each 96 bytes)
        const size_t HEADER_SIZE = 16;
        const size_t PRIM_SIZE = sizeof(SDFPrimitive);

        if (payload.size() < HEADER_SIZE) {
            ywarn("HDraw::parseBinary: payload too small for header ({})", payload.size());
            return Ok();
        }

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());

        // Read header
        uint32_t version, primCount, bgColor, flags;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColor, data + 8, 4);
        std::memcpy(&flags, data + 12, 4);

        yinfo("HDraw::parseBinary: version={} primCount={} bgColor={:#010x} flags={}",
              version, primCount, bgColor, flags);

        if (version != 1 && version != 2) {
            ywarn("HDraw::parseBinary: unknown version {}", version);
            return Ok();
        }

        _bgColor = bgColor;
        _flags |= flags;
        _metadataDirty = true;

        // Read primitives
        size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
        if (payload.size() < expectedSize) {
            ywarn("HDraw::parseBinary: payload too small ({} < {})",
                  payload.size(), expectedSize);
            primCount = static_cast<uint32_t>((payload.size() - HEADER_SIZE) / PRIM_SIZE);
        }

        const uint8_t* primData = data + HEADER_SIZE;

        // Allocate buffer and copy directly from payload
        if (auto res = ensurePrimCapacity(primCount); !res) {
            return Err<void>("HDraw::parseBinary: failed to allocate prim storage");
        }
        _primCount = primCount;
        std::memcpy(_primitives, primData, primCount * PRIM_SIZE);
        _cardMgr->markBufferDirty(_primStorage);

        _dirty = true;
        yinfo("HDraw::parseBinary: loaded {} primitives", _primCount);

        return Ok();
    }

    //=========================================================================
    // YAML Parsing
    //=========================================================================

    static uint32_t parseColor(const YAML::Node& node) {
        if (!node) return 0xFFFFFFFF;

        std::string str = node.as<std::string>();
        if (str.empty()) return 0xFFFFFFFF;

        if (str[0] == '#') {
            // Parse #RRGGBB or #RRGGBBAA
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
            return (a << 24) | (b << 16) | (g << 8) | r;  // ABGR
        }
        return 0xFFFFFFFF;
    }

    Result<void> parseYAML(const std::string& yaml) {
        try {
            YAML::Node root = YAML::Load(yaml);

            // Parse background color
            if (root["background"]) {
                _bgColor = parseColor(root["background"]);
                _metadataDirty = true;
            }

            // Parse flags
            if (root["flags"]) {
                auto flagsNode = root["flags"];
                if (flagsNode.IsSequence()) {
                    for (const auto& flag : flagsNode) {
                        std::string f = flag.as<std::string>();
                        if (f == "show_bounds") _flags |= FLAG_SHOW_BOUNDS;
                        else if (f == "show_grid") _flags |= FLAG_SHOW_GRID;
                        else if (f == "show_eval_count") _flags |= FLAG_SHOW_EVAL_COUNT;
                    }
                } else if (flagsNode.IsScalar()) {
                    std::string f = flagsNode.as<std::string>();
                    if (f == "show_bounds") _flags |= FLAG_SHOW_BOUNDS;
                    else if (f == "show_grid") _flags |= FLAG_SHOW_GRID;
                    else if (f == "show_eval_count") _flags |= FLAG_SHOW_EVAL_COUNT;
                }
                _metadataDirty = true;
            }

            // Parse body
            if (root["body"] && root["body"].IsSequence()) {
                for (const auto& item : root["body"]) {
                    parseYAMLPrimitive(item);
                }
            }

            // Also check for list format
            if (root.IsSequence()) {
                for (const auto& doc : root) {
                    if (doc["body"] && doc["body"].IsSequence()) {
                        for (const auto& item : doc["body"]) {
                            parseYAMLPrimitive(item);
                        }
                    }
                }
            }

            _dirty = true;
            yinfo("HDraw::parseYAML: loaded {} primitives, {} glyphs",
                  _primCount, _glyphs.size());

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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
            addPrimitive(prim);
            return;
        }

        // Heart
        if (item["heart"]) {
            auto h = item["heart"];
            float cx = 0, cy = 0, scale = 20;
            uint32_t fill = 0xFF0000FF, stroke = 0;  // Default red
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
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
            computeAABB_hdraw(prim);
            addPrimitive(prim);
            return;
        }
    }

    void computeSceneBounds() {
        if (_hasExplicitBounds) return;

        _sceneMinX = 1e10f;
        _sceneMinY = 1e10f;
        _sceneMaxX = -1e10f;
        _sceneMaxY = -1e10f;

        // Include primitive bounds
        for (uint32_t i = 0; i < _primCount; i++) {
            _sceneMinX = std::min(_sceneMinX, _primitives[i].aabbMinX);
            _sceneMinY = std::min(_sceneMinY, _primitives[i].aabbMinY);
            _sceneMaxX = std::max(_sceneMaxX, _primitives[i].aabbMaxX);
            _sceneMaxY = std::max(_sceneMaxY, _primitives[i].aabbMaxY);
        }

        // Include glyph bounds
        for (const auto& glyph : _glyphs) {
            _sceneMinX = std::min(_sceneMinX, glyph.x);
            _sceneMinY = std::min(_sceneMinY, glyph.y);
            _sceneMaxX = std::max(_sceneMaxX, glyph.x + glyph.width);
            _sceneMaxY = std::max(_sceneMaxY, glyph.y + glyph.height);
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
        // Grid buffer already allocated by rebuildAndUpload
        // _grid, _gridSize, _gridWidth, _gridHeight, _cellSize are set

        if (_primCount == 0 || _gridSize == 0) {
            return;
        }

        uint32_t totalCells = _gridWidth * _gridHeight;
        uint32_t cellStride = 1 + _maxPrimsPerCell;

        // Zero the grid
        std::memset(_grid, 0, _gridSize * sizeof(uint32_t));

        // Assign each primitive to cells it overlaps
        for (uint32_t primIdx = 0; primIdx < _primCount; primIdx++) {
            const auto& prim = _primitives[primIdx];

            uint32_t cellMinX = static_cast<uint32_t>(
                std::clamp((prim.aabbMinX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
            uint32_t cellMaxX = static_cast<uint32_t>(
                std::clamp((prim.aabbMaxX - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
            uint32_t cellMinY = static_cast<uint32_t>(
                std::clamp((prim.aabbMinY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
            uint32_t cellMaxY = static_cast<uint32_t>(
                std::clamp((prim.aabbMaxY - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

            for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
                for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                    uint32_t cellIndex = cy * _gridWidth + cx;
                    uint32_t cellOffset = cellIndex * cellStride;
                    uint32_t count = _grid[cellOffset];
                    if (count < _maxPrimsPerCell) {
                        _grid[cellOffset + 1 + count] = primIdx;
                        _grid[cellOffset] = count + 1;
                    }
                }
            }
        }

        yinfo("HDraw::buildGrid: {} primitives -> {}x{} grid ({} cells), cellSize={}",
              _primCount, _gridWidth, _gridHeight, totalCells, _cellSize);
    }

    Result<void> rebuildAndUpload() {
        // Compute scene bounds from primitives already in buffer
        computeSceneBounds();

        // Pre-calculate grid dimensions to know derived storage size
        float sceneWidth = _sceneMaxX - _sceneMinX;
        float sceneHeight = _sceneMaxY - _sceneMinY;

        uint32_t gridW = 0, gridH = 0;
        float cellSize = _cellSize;

        if (_primCount > 0) {
            float sceneArea = sceneWidth * sceneHeight;
            if (cellSize <= 0.0f) {
                float avgPrimArea = 0.0f;
                for (uint32_t i = 0; i < _primCount; i++) {
                    float w = _primitives[i].aabbMaxX - _primitives[i].aabbMinX;
                    float h = _primitives[i].aabbMaxY - _primitives[i].aabbMinY;
                    avgPrimArea += w * h;
                }
                avgPrimArea /= _primCount;
                cellSize = std::sqrt(avgPrimArea) * 1.5f;
                float minCellSize = std::sqrt(sceneArea / 65536.0f);
                float maxCellSize = std::sqrt(sceneArea / 16.0f);
                cellSize = std::clamp(cellSize, minCellSize, maxCellSize);
            }
            gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cellSize)));
            gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cellSize)));
            const uint32_t MAX_GRID_DIM = 512;
            if (gridW > MAX_GRID_DIM) { gridW = MAX_GRID_DIM; cellSize = sceneWidth / gridW; }
            if (gridH > MAX_GRID_DIM) { gridH = MAX_GRID_DIM; cellSize = std::max(cellSize, sceneHeight / gridH); }
        }

        uint32_t cellStride = 1 + _maxPrimsPerCell;
        uint32_t gridTotalU32 = gridW * gridH * cellStride;
        uint32_t gridBytes = gridTotalU32 * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(HDrawGlyph));
        uint32_t derivedTotalSize = gridBytes + glyphBytes;

        // Deallocate old derived storage
        if (_derivedStorage.isValid()) {
            if (auto res = _cardMgr->deallocateBuffer(_derivedStorage); !res) {
                return Err<void>("HDraw::rebuildAndUpload: deallocateBuffer failed");
            }
            _derivedStorage = StorageHandle::invalid();
            _grid = nullptr;
            _gridSize = 0;
        }

        if (derivedTotalSize > 0) {
            auto storageResult = _cardMgr->allocateBuffer(derivedTotalSize);
            if (!storageResult) {
                return Err<void>("HDraw::rebuildAndUpload: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            // Set up grid pointer directly into buffer
            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = gridTotalU32;
            _gridWidth = gridW;
            _gridHeight = gridH;
            _cellSize = cellSize;
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            offset += gridBytes;

            // Build grid directly into buffer
            if (_primCount > 0) {
                buildGrid();
            }

            // Glyphs (small data)
            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!_glyphs.empty()) {
                std::memcpy(base + offset, _glyphs.data(), glyphBytes);
            }

            _cardMgr->markBufferDirty(_derivedStorage);
        }

        // Primitives are already in _primStorage
        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;

        if (_primStorage.isValid()) {
            _cardMgr->markBufferDirty(_primStorage);
        }

        _metadataDirty = true;

        yinfo("HDraw::rebuildAndUpload: {} prims, {}x{} grid, {} glyphs, derived {} bytes",
              _primCount, _gridWidth, _gridHeight, _glyphs.size(), derivedTotalSize);

        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("HDraw::uploadMetadata: invalid metadata handle");
        }

        Metadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));  // Store float as u32 bits
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
        uint32_t glyphOffset;       // 24 - offset of glyph data in storage
        uint32_t glyphCount;        // 28 - number of glyphs
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

    // Primitive data - direct buffer storage
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _primStorage = StorageHandle::invalid();
    std::vector<SDFPrimitive> _primStaging;  // CPU staging for suspend/resume

    // Grid + glyphs - built directly in derived storage
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;  // total uint32_t elements
    StorageHandle _derivedStorage = StorageHandle::invalid();

    // Glyphs (small, vector OK)
    std::vector<HDrawGlyph> _glyphs;

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
    uint32_t _glyphOffset = 0;

    // Rendering
    uint32_t _bgColor = 0xFF2E1A1A;
    uint32_t _flags = 0;

    bool _dirty = true;
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;

    // Font for text rendering
    FontManager::Ptr _fontManager;
    MsMsdfFont::Ptr _font;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> HDraw::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("HDraw::create: pos=({},{}) size={}x{} args='{}' payload_len={}",
          x, y, widthCells, heightCells, args, payload.size());

    if (!ctx.cardManager) {
        yerror("HDraw::create: null CardBufferManager!");
        return Err<CardPtr>("HDraw::create: null CardBufferManager");
    }

    auto card = std::make_shared<HDrawImpl>(
        ctx, x, y, widthCells, heightCells, args, payload);

    if (auto res = card->init(); !res) {
        yerror("HDraw::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("HDraw::create: init failed");
    }

    yinfo("HDraw::create: SUCCESS, shaderGlyph={:#x}", card->shaderGlyph());
    return Ok<CardPtr>(card);
}

Result<HDraw::Ptr> HDraw::createImpl(
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
        return Err<Ptr>("Failed to create HDraw", result);
    }
    auto hdraw = std::dynamic_pointer_cast<HDraw>(*result);
    if (!hdraw) {
        return Err<Ptr>("Created card is not an HDraw");
    }
    return Ok(hdraw);
}

} // namespace yetty::card
