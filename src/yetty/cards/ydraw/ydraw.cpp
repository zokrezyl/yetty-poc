#include "ydraw.h"
#include "../../ydraw/ydraw-builder.h"
#include "../../ydraw/animation.h"
#include "../hdraw/hdraw.h"  // For SDFPrimitive, SDFType
#include <yetty/base/event-loop.h>
#include <yetty/msdf-glyph-data.h>
#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <cmath>
#include <cstring>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
}

namespace yetty::card {

//=============================================================================
// YDrawImpl - full implementation with builder, GPU buffers, animation
//=============================================================================

class YDrawImpl : public YDraw {
public:
    YDrawImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YDraw(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
        if (builderRes) {
            _builder = *builderRes;
        } else {
            yerror("YDrawImpl: failed to create builder");
        }
    }

    ~YDrawImpl() override { dispose(); }

    //=========================================================================
    // Card lifecycle
    //=========================================================================

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("YDrawImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        if (_primStorage.isValid() && _primCount > 0 && _builder) {
            // Save GPU prims back to builder staging
            auto& staging = _builder->primStagingMut();
            staging.resize(_primCount);
            std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;
    }

    void declareBufferNeeds() override {
        if (!_builder) return;

        // Save GPU prims back to builder staging before dealloc
        if (_primStorage.isValid() && _primCount > 0) {
            auto& staging = _builder->primStagingMut();
            if (staging.empty()) {
                staging.resize(_primCount);
                std::memcpy(staging.data(), _primitives, _primCount * sizeof(SDFPrimitive));
            } else {
                std::vector<SDFPrimitive> merged(_primCount);
                std::memcpy(merged.data(), _primitives, _primCount * sizeof(SDFPrimitive));
                merged.insert(merged.end(), staging.begin(), staging.end());
                staging = std::move(merged);
            }
        }
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primitives = nullptr;
        _primCount = 0;
        _primCapacity = 0;

        const auto& primStaging = _builder->primStaging();
        const auto& glyphs = _builder->glyphs();

        if (primStaging.empty() && glyphs.empty()) {
            _builder->clearGridStaging();
            return;
        }

        // Reserve prim storage
        if (!primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
        }

        // Calculate grid if needed
        if (_builder->gridStaging().empty()) {
            _builder->calculate();
        }

        // Reserve derived size (grid + glyphs + optional atlas)
        uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (_builder->hasCustomAtlas()) {
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
            uint32_t glyphMetaBytes = static_cast<uint32_t>(
                _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
            derivedSize += atlasHeaderBytes + glyphMetaBytes;
        }

        if (derivedSize > 0) {
            _cardMgr->bufferManager()->reserve(derivedSize);
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();

        const auto& primStaging = _builder->primStaging();
        const auto& gridStaging = _builder->gridStaging();
        const auto& glyphs = _builder->glyphs();

        // Restore primitives from staging
        if (!primStaging.empty()) {
            uint32_t count = static_cast<uint32_t>(primStaging.size());
            uint32_t allocBytes = count * sizeof(SDFPrimitive);
            auto primResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "prims", allocBytes);
            if (!primResult) {
                return Err<void>("YDrawImpl::allocateBuffers: prim alloc failed");
            }
            _primStorage = *primResult;
            _primitives = reinterpret_cast<SDFPrimitive*>(_primStorage.data);
            _primCapacity = count;
            _primCount = count;
            std::memcpy(_primitives, primStaging.data(), count * sizeof(SDFPrimitive));
            _builder->primStagingMut().clear();
            _builder->primStagingMut().shrink_to_fit();
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        // Allocate derived storage (grid + glyphs + atlas)
        uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (_builder->hasCustomAtlas()) {
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
            uint32_t glyphMetaBytes = static_cast<uint32_t>(
                _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
            derivedSize += atlasHeaderBytes + glyphMetaBytes;
        }

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("YDrawImpl::allocateBuffers: derived alloc failed");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            // Copy grid
            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridStaging.size());
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!gridStaging.empty()) {
                std::memcpy(base + offset, gridStaging.data(), gridBytes);
            }
            offset += gridBytes;

            // Copy glyphs
            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }
            offset += glyphBytes;

            if (_builder->hasCustomAtlas()) {
                _atlasHeaderOffset = offset;
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        _gridWidth = _builder->gridWidth();
        _gridHeight = _builder->gridHeight();
        _metadataDirty = true;
        _dirty = false;

        return Ok();
    }

    Result<void> allocateTextures() override {
        if (!_builder || !_builder->hasCustomAtlas()) return Ok();

        auto atlas = _builder->customAtlas();
        uint32_t atlasW = atlas->getAtlasWidth();
        uint32_t atlasH = atlas->getAtlasHeight();
        const auto& atlasData = atlas->getAtlasData();

        if (atlasData.empty() || atlasW == 0 || atlasH == 0) return Ok();

        auto allocResult = _cardMgr->textureManager()->allocate(atlasW, atlasH);
        if (!allocResult) {
            return Err<void>("YDrawImpl::allocateTextures: failed", allocResult);
        }
        _atlasTextureHandle = *allocResult;
        _dirty = true;
        return Ok();
    }

    Result<void> writeTextures() override {
        if (_atlasTextureHandle.isValid() && _builder && _builder->hasCustomAtlas()) {
            const auto& atlasData = _builder->customAtlas()->getAtlasData();
            if (!atlasData.empty()) {
                if (auto res = _cardMgr->textureManager()->write(
                        _atlasTextureHandle, atlasData.data()); !res) {
                    return Err<void>("YDrawImpl::writeTextures: write failed", res);
                }
            }
        }
        return Ok();
    }

    Result<void> render(float time) override {
        if (!_builder) return Ok();

        // Auto-start animation
        if (_animation && _animation->hasProperties() && !_animation->isPlaying()
            && _basePrimitives.empty() && _primitives && _primCount > 0) {
            startAnimation();
        }

        // Animation update
        if (_animation && _animation->isPlaying() && _primitives && _primCount > 0) {
            float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
            _lastRenderTime = time;
            if (_animation->advance(dt)) {
                _animation->apply(_basePrimitives.data(), _primitives, _primCount);
                for (uint32_t i = 0; i < _primCount; i++) {
                    YDrawBuilder::recomputeAABB(_primitives[i]);
                }
                _cardMgr->bufferManager()->markBufferDirty(_primStorage);
                _dirty = true;
            }
        }

        if (_dirty) {
            if (auto res = rebuildAndUpload(); !res) return res;
            _dirty = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) return res;
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // Events - zoom/pan
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                _focused = true;
                return Ok(true);
            } else if (_focused) {
                _focused = false;
            }
            return Ok(false);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {
            float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
            float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                float zoomDelta = event.cardScroll.dy * 0.1f;
                float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 20.0f);
                if (newZoom != _viewZoom) {
                    _viewZoom = newZoom;
                    _metadataDirty = true;
                }
                return Ok(true);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                _viewPanX += event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            } else {
                _viewPanY += event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

    //=========================================================================
    // Init
    //=========================================================================

    Result<void> init() {
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
        if (!metaResult) {
            return Err<void>("YDrawImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        if (auto res = registerForEvents(); !res) {
            ywarn("YDrawImpl::init: event registration failed: {}", error_msg(res));
        }

        parseArgs(_argsStr);

        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("YDrawImpl::init: payload parse failed: {}", error_msg(res));
            }
        }

        yinfo("YDrawImpl::init: {} prims, {} glyphs",
              _builder->primitiveCount(), _builder->glyphCount());

        _dirty = true;
        _metadataDirty = true;
        return Ok();
    }

private:
    //=========================================================================
    // GPU rebuild
    //=========================================================================

    Result<void> rebuildAndUpload() {
        // Compute scene bounds from GPU-resident prims
        _builder->computeSceneBoundsFromPrims(_primitives, _primCount);

        float sceneW = _builder->sceneMaxX() - _builder->sceneMinX();
        float sceneH = _builder->sceneMaxY() - _builder->sceneMinY();

        _builder->computeGridDims(_primitives, _primCount);
        _builder->buildGridFromPrims(_primitives, _primCount);

        const auto& gridData = _builder->gridStaging();
        uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(
            _builder->glyphs().size() * sizeof(YDrawGlyph));
        uint32_t derivedTotalSize = gridBytes + glyphBytes;

        if (_builder->hasCustomAtlas()) {
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
            uint32_t glyphMetaBytes = static_cast<uint32_t>(
                _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
            derivedTotalSize += atlasHeaderBytes + glyphMetaBytes;
        }

        if (derivedTotalSize > 0) {
            if (!_derivedStorage.isValid() || derivedTotalSize > _derivedStorage.size) {
                auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                    metadataSlotIndex(), "derived", derivedTotalSize);
                if (!storageResult) {
                    return Err<void>("YDrawImpl::rebuild: derived alloc failed");
                }
                _derivedStorage = *storageResult;
            }
        }

        if (_derivedStorage.isValid() && derivedTotalSize > 0) {
            uint8_t* base = _derivedStorage.data;
            std::memset(base, 0, _derivedStorage.size);
            uint32_t offset = 0;

            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridData.size());
            _gridWidth = _builder->gridWidth();
            _gridHeight = _builder->gridHeight();
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            std::memcpy(base + offset, gridData.data(), gridBytes);
            offset += gridBytes;

            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            const auto& glyphs = _builder->glyphs();
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }
            offset += glyphBytes;

            if (_builder->hasCustomAtlas() && _atlasTextureHandle.isValid()) {
                auto atlas = _builder->customAtlas();
                auto atlasPos = _cardMgr->textureManager()->getAtlasPosition(_atlasTextureHandle);
                uint32_t msdfAtlasW = atlas->getAtlasWidth();
                uint32_t msdfAtlasH = atlas->getAtlasHeight();
                const auto& glyphMeta = atlas->getGlyphMetadata();

                uint32_t atlasHeader[4] = {
                    (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16),
                    (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16),
                    static_cast<uint32_t>(glyphMeta.size()),
                    0
                };
                std::memcpy(base + offset, atlasHeader, sizeof(atlasHeader));
                offset += sizeof(atlasHeader);

                if (!glyphMeta.empty()) {
                    uint32_t metaBytes = static_cast<uint32_t>(
                        glyphMeta.size() * sizeof(GlyphMetadataGPU));
                    std::memcpy(base + offset, glyphMeta.data(), metaBytes);
                }
            }

            _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
        }

        _primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
        if (_primStorage.isValid()) {
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }
        _metadataDirty = true;
        return Ok();
    }

    Result<void> uploadMetadata() {
        if (!_metaHandle.isValid()) {
            return Err<void>("YDrawImpl::uploadMetadata: invalid handle");
        }

        // Pack zoom as f16 in upper 16 bits of flags
        uint32_t zoomBits;
        {
            uint32_t f32bits;
            std::memcpy(&f32bits, &_viewZoom, sizeof(float));
            uint32_t sign = (f32bits >> 16) & 0x8000;
            int32_t  exp  = ((f32bits >> 23) & 0xFF) - 127 + 15;
            uint32_t mant = (f32bits >> 13) & 0x3FF;
            if (exp <= 0) { exp = 0; mant = 0; }
            else if (exp >= 31) { exp = 31; mant = 0; }
            zoomBits = sign | (static_cast<uint32_t>(exp) << 10) | mant;
        }

        float sceneMinX = _builder->sceneMinX();
        float sceneMinY = _builder->sceneMinY();
        float sceneMaxX = _builder->sceneMaxX();
        float sceneMaxY = _builder->sceneMaxY();
        float contentW = sceneMaxX - sceneMinX;
        float contentH = sceneMaxY - sceneMinY;

        int16_t panXi16 = static_cast<int16_t>(std::clamp(
            _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
        int16_t panYi16 = static_cast<int16_t>(std::clamp(
            _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

        float cellSize = _builder->cellSize();

        YDrawMetadata meta = {};
        meta.primitiveOffset = _primitiveOffset;
        meta.primitiveCount = _primCount;
        meta.gridOffset = _gridOffset;
        meta.gridWidth = _gridWidth;
        meta.gridHeight = _gridHeight;
        std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
        meta.glyphOffset = _glyphOffset;
        meta.glyphCount = static_cast<uint32_t>(_builder->glyphs().size());
        std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
        std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
        std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
        std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
        meta.widthCells  = (_widthCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
        meta.heightCells = (_heightCells & 0xFFFF) |
                           (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
        meta.flags = (_builder->flags() & 0xFFFF) | (zoomBits << 16);
        meta.bgColor = _builder->bgColor();

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("YDrawImpl::uploadMetadata: write failed");
        }
        return Ok();
    }

    //=========================================================================
    // Events
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Err<void>("no EventLoop", loopResult);
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) return Ok();
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Ok();
        (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
        return Ok();
    }

    //=========================================================================
    // Animation
    //=========================================================================

    animation::Animation* anim() {
        if (!_animation) {
            _animation = std::make_unique<animation::Animation>();
        }
        return _animation.get();
    }

    void startAnimation() {
        if (!_animation || !_animation->hasProperties()) return;
        if (!_primitives || _primCount == 0) return;

        _basePrimitives.assign(_primitives, _primitives + _primCount);
        _lastRenderTime = -1.0f;

        if (!_builder->hasExplicitBounds()) {
            float minX = 1e10f, minY = 1e10f, maxX = -1e10f, maxY = -1e10f;
            for (uint32_t i = 0; i < _primCount; i++) {
                minX = std::min(minX, _primitives[i].aabbMinX);
                minY = std::min(minY, _primitives[i].aabbMinY);
                maxX = std::max(maxX, _primitives[i].aabbMaxX);
                maxY = std::max(maxY, _primitives[i].aabbMaxY);
            }
            _animation->expandBounds(_basePrimitives.data(), _primCount,
                                     minX, minY, maxX, maxY);
            float padX = (maxX - minX) * 0.05f;
            float padY = (maxY - minY) * 0.05f;
            _builder->setSceneBounds(minX - padX, minY - padY,
                                     maxX + padX, maxY + padY);
        }
        _animation->play();
    }

    //=========================================================================
    // Parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")
                        colorStr = colorStr.substr(2);
                    _builder->setBgColor(static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16)));
                }
            } else if (token == "--cell-size") {
                std::string s; if (iss >> s) _builder->setGridCellSize(std::stof(s));
            } else if (token == "--max-prims-per-cell") {
                std::string s; if (iss >> s) _builder->setMaxPrimsPerCell(static_cast<uint32_t>(std::stoul(s)));
            } else if (token == "--show-bounds") {
                _builder->addFlags(YDrawBuilder::FLAG_SHOW_BOUNDS);
            } else if (token == "--show-grid") {
                _builder->addFlags(YDrawBuilder::FLAG_SHOW_GRID);
            } else if (token == "--show-eval-count") {
                _builder->addFlags(YDrawBuilder::FLAG_SHOW_EVAL_COUNT);
            }
        }
    }

    Result<void> parsePayload(const std::string& payload) {
        if (payload.empty()) return Ok();
        bool isYaml = false;
        if (payload.size() >= 4) {
            char first = payload[0];
            if (first == '#' || first == '-' || first == ' ' || first == '\n' ||
                first == '\t' || (first >= 'a' && first <= 'z') ||
                (first >= 'A' && first <= 'Z')) {
                isYaml = true;
            }
        }
        return isYaml ? parseYAML(payload) : parseBinary(payload);
    }

    Result<void> parseBinary(const std::string& payload) {
        const size_t HEADER_SIZE = 16;
        const size_t PRIM_SIZE = sizeof(SDFPrimitive);

        if (payload.size() < HEADER_SIZE) return Ok();

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());
        uint32_t version, primCount, bgColorVal, flagsVal;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColorVal, data + 8, 4);
        std::memcpy(&flagsVal, data + 12, 4);

        if (version != 1 && version != 2) return Ok();

        _builder->setBgColor(bgColorVal);
        _builder->addFlags(flagsVal);

        size_t expectedSize = HEADER_SIZE + primCount * PRIM_SIZE;
        if (payload.size() < expectedSize) {
            primCount = static_cast<uint32_t>((payload.size() - HEADER_SIZE) / PRIM_SIZE);
        }

        const uint8_t* primData = data + HEADER_SIZE;
        for (uint32_t i = 0; i < primCount; i++) {
            SDFPrimitive prim;
            std::memcpy(&prim, primData + i * PRIM_SIZE, PRIM_SIZE);
            _builder->addPrimitive(prim);
        }
        return Ok();
    }

    Result<void> parseYAML(const std::string& yaml) {
        try {
            YAML::Node root = YAML::Load(yaml);
            bool rootIsSequence = root.IsSequence();

            auto parsePrimWithAnim = [this](const YAML::Node& item) -> Result<void> {
                uint32_t primBefore = _builder->primitiveCount();
                if (auto res = parseYAMLPrimitive(item); !res) return res;
                uint32_t primAfter = _builder->primitiveCount();

                if (primAfter > primBefore && item.IsMap()) {
                    for (auto it = item.begin(); it != item.end(); ++it) {
                        auto shapeNode = it->second;
                        if (shapeNode.IsMap() && shapeNode["animate"]) {
                            parseAnimateBlock(shapeNode["animate"], primBefore);
                        }
                    }
                }
                return Ok();
            };

            auto parseSettings = [this](const YAML::Node& node) {
                if (node["background"]) {
                    _builder->setBgColor(parseColor(node["background"]));
                }
                if (node["flags"]) {
                    auto flagsNode = node["flags"];
                    if (flagsNode.IsSequence()) {
                        for (const auto& flag : flagsNode) {
                            std::string f = flag.as<std::string>();
                            if (f == "show_bounds") _builder->addFlags(YDrawBuilder::FLAG_SHOW_BOUNDS);
                            else if (f == "show_grid") _builder->addFlags(YDrawBuilder::FLAG_SHOW_GRID);
                            else if (f == "show_eval_count") _builder->addFlags(YDrawBuilder::FLAG_SHOW_EVAL_COUNT);
                        }
                    } else if (flagsNode.IsScalar()) {
                        std::string f = flagsNode.as<std::string>();
                        if (f == "show_bounds") _builder->addFlags(YDrawBuilder::FLAG_SHOW_BOUNDS);
                        else if (f == "show_grid") _builder->addFlags(YDrawBuilder::FLAG_SHOW_GRID);
                        else if (f == "show_eval_count") _builder->addFlags(YDrawBuilder::FLAG_SHOW_EVAL_COUNT);
                    }
                }
                if (node["animation"]) {
                    auto animNode = node["animation"];
                    auto* a = anim();
                    if (animNode["duration"]) a->setDuration(animNode["duration"].as<float>());
                    if (animNode["loop"]) a->setLoop(animNode["loop"].as<bool>());
                    if (animNode["speed"]) a->setSpeed(animNode["speed"].as<float>());
                }
            };

            if (rootIsSequence) {
                for (const auto& doc : root) {
                    if (doc.IsMap()) {
                        parseSettings(doc);
                        if (doc["body"] && doc["body"].IsSequence()) {
                            for (const auto& item : doc["body"]) {
                                if (auto res = parsePrimWithAnim(item); !res) return res;
                            }
                        }
                    }
                }
            } else if (root.IsMap()) {
                parseSettings(root);
                if (root["body"] && root["body"].IsSequence()) {
                    for (const auto& item : root["body"]) {
                        if (auto res = parsePrimWithAnim(item); !res) return res;
                    }
                }
            }

            return Ok();
        } catch (const YAML::Exception& e) {
            return Err<void>(std::string("YAML parse error: ") + e.what());
        }
    }

    Result<void> parseYAMLPrimitive(const YAML::Node& item) {
        uint32_t layer = _builder->primitiveCount() + _builder->glyphCount();

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
            if (t["font"]) {
                auto ttfPath = t["font"].as<std::string>();
                auto fontResult = _builder->addFont(ttfPath);
                if (!fontResult) return Err<void>("failed to load font: " + ttfPath);
                _builder->addText(x, y, content, fontSize, color, layer, *fontResult);
            } else {
                _builder->addText(x, y, content, fontSize, color, layer);
            }
            return Ok();
        }

        // Circle
        if (item["circle"]) {
            auto c = item["circle"];
            float cx = 0, cy = 0, r = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (c["position"] && c["position"].IsSequence()) {
                cx = c["position"][0].as<float>(); cy = c["position"][1].as<float>();
            }
            if (c["radius"]) r = c["radius"].as<float>();
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
            _builder->addCircle(cx, cy, r, fill, stroke, strokeWidth, layer);
            return Ok();
        }

        // Box
        if (item["box"]) {
            auto b = item["box"];
            float cx = 0, cy = 0, hw = 10, hh = 10, round = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (b["position"] && b["position"].IsSequence()) {
                cx = b["position"][0].as<float>(); cy = b["position"][1].as<float>();
            }
            if (b["size"] && b["size"].IsSequence()) {
                hw = b["size"][0].as<float>() / 2; hh = b["size"][1].as<float>() / 2;
            }
            if (b["fill"]) fill = parseColor(b["fill"]);
            if (b["stroke"]) stroke = parseColor(b["stroke"]);
            if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
            if (b["round"]) round = b["round"].as<float>();
            _builder->addBox(cx, cy, hw, hh, fill, stroke, strokeWidth, round, layer);
            return Ok();
        }

        // Segment
        if (item["segment"]) {
            auto s = item["segment"];
            float x0 = 0, y0 = 0, x1 = 100, y1 = 100;
            uint32_t stroke = 0xFFFFFFFF;
            float strokeWidth = 1;
            if (s["from"] && s["from"].IsSequence()) {
                x0 = s["from"][0].as<float>(); y0 = s["from"][1].as<float>();
            }
            if (s["to"] && s["to"].IsSequence()) {
                x1 = s["to"][0].as<float>(); y1 = s["to"][1].as<float>();
            }
            if (s["stroke"]) stroke = parseColor(s["stroke"]);
            if (s["stroke-width"]) strokeWidth = s["stroke-width"].as<float>();
            _builder->addSegment(x0, y0, x1, y1, stroke, strokeWidth, layer);
            return Ok();
        }

        // Triangle
        if (item["triangle"]) {
            auto t = item["triangle"];
            float x0 = 0, y0 = 0, x1 = 50, y1 = 100, x2 = 100, y2 = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (t["p0"] && t["p0"].IsSequence()) { x0 = t["p0"][0].as<float>(); y0 = t["p0"][1].as<float>(); }
            if (t["p1"] && t["p1"].IsSequence()) { x1 = t["p1"][0].as<float>(); y1 = t["p1"][1].as<float>(); }
            if (t["p2"] && t["p2"].IsSequence()) { x2 = t["p2"][0].as<float>(); y2 = t["p2"][1].as<float>(); }
            if (t["fill"]) fill = parseColor(t["fill"]);
            if (t["stroke"]) stroke = parseColor(t["stroke"]);
            if (t["stroke-width"]) strokeWidth = t["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Triangle);
            prim.layer = layer;
            prim.params[0] = x0; prim.params[1] = y0;
            prim.params[2] = x1; prim.params[3] = y1;
            prim.params[4] = x2; prim.params[5] = y2;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Bezier
        if (item["bezier"]) {
            auto b = item["bezier"];
            float x0 = 0, y0 = 0, x1 = 50, y1 = 50, x2 = 100, y2 = 0;
            uint32_t stroke = 0xFFFFFFFF;
            float strokeWidth = 1;
            if (b["p0"] && b["p0"].IsSequence()) { x0 = b["p0"][0].as<float>(); y0 = b["p0"][1].as<float>(); }
            if (b["p1"] && b["p1"].IsSequence()) { x1 = b["p1"][0].as<float>(); y1 = b["p1"][1].as<float>(); }
            if (b["p2"] && b["p2"].IsSequence()) { x2 = b["p2"][0].as<float>(); y2 = b["p2"][1].as<float>(); }
            if (b["stroke"]) stroke = parseColor(b["stroke"]);
            if (b["stroke-width"]) strokeWidth = b["stroke-width"].as<float>();
            _builder->addBezier2(x0, y0, x1, y1, x2, y2, stroke, strokeWidth, layer);
            return Ok();
        }

        // Ellipse
        if (item["ellipse"]) {
            auto e = item["ellipse"];
            float cx = 0, cy = 0, rx = 20, ry = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (e["position"] && e["position"].IsSequence()) {
                cx = e["position"][0].as<float>(); cy = e["position"][1].as<float>();
            }
            if (e["radii"] && e["radii"].IsSequence()) {
                rx = e["radii"][0].as<float>(); ry = e["radii"][1].as<float>();
            }
            if (e["fill"]) fill = parseColor(e["fill"]);
            if (e["stroke"]) stroke = parseColor(e["stroke"]);
            if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Ellipse);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy;
            prim.params[2] = rx; prim.params[3] = ry;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Arc
        if (item["arc"]) {
            auto a = item["arc"];
            float cx = 0, cy = 0, ra = 20, rb = 2, angle = 90.0f;
            uint32_t fill = 0, stroke = 0xFFFFFFFF;
            float strokeWidth = 0;
            if (a["position"] && a["position"].IsSequence()) {
                cx = a["position"][0].as<float>(); cy = a["position"][1].as<float>();
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
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Generic shape helper: center+radius shapes
        auto parseRadiusShape = [&](const char* name, SDFType type) -> bool {
            if (!item[name]) return false;
            auto n = item[name];
            float cx = 0, cy = 0, r = 20;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (n["position"] && n["position"].IsSequence()) {
                cx = n["position"][0].as<float>(); cy = n["position"][1].as<float>();
            }
            if (n["radius"]) r = n["radius"].as<float>();
            if (n["fill"]) fill = parseColor(n["fill"]);
            if (n["stroke"]) stroke = parseColor(n["stroke"]);
            if (n["stroke-width"]) strokeWidth = n["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(type);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy; prim.params[2] = r;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return true;
        };

        if (parseRadiusShape("pentagon", SDFType::Pentagon)) return Ok();
        if (parseRadiusShape("hexagon", SDFType::Hexagon)) return Ok();
        if (parseRadiusShape("equilateral_triangle", SDFType::EquilateralTriangle)) return Ok();
        if (parseRadiusShape("octogon", SDFType::Octogon)) return Ok();
        if (parseRadiusShape("hexagram", SDFType::Hexagram)) return Ok();
        if (parseRadiusShape("pentagram", SDFType::Pentagram)) return Ok();
        if (parseRadiusShape("quadratic_circle", SDFType::QuadraticCircle)) return Ok();
        if (parseRadiusShape("cool_s", SDFType::CoolS)) return Ok();

        // Star
        if (item["star"]) {
            auto s = item["star"];
            float cx = 0, cy = 0, r = 20, n = 5, m = 2.5f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (s["position"] && s["position"].IsSequence()) {
                cx = s["position"][0].as<float>(); cy = s["position"][1].as<float>();
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
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Pie
        if (item["pie"]) {
            auto p = item["pie"];
            float cx = 0, cy = 0, r = 20, angle = 45.0f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (p["position"] && p["position"].IsSequence()) {
                cx = p["position"][0].as<float>(); cy = p["position"][1].as<float>();
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
            prim.params[2] = std::sin(rad); prim.params[3] = std::cos(rad); prim.params[4] = r;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Ring
        if (item["ring"]) {
            auto rg = item["ring"];
            float cx = 0, cy = 0, r = 20, th = 4, angle = 0.0f;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (rg["position"] && rg["position"].IsSequence()) {
                cx = rg["position"][0].as<float>(); cy = rg["position"][1].as<float>();
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
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Heart
        if (item["heart"]) {
            auto h = item["heart"];
            float cx = 0, cy = 0, scale = 20;
            uint32_t fill = 0xFF0000FF, stroke = 0;
            float strokeWidth = 0;
            if (h["position"] && h["position"].IsSequence()) {
                cx = h["position"][0].as<float>(); cy = h["position"][1].as<float>();
            }
            if (h["scale"]) scale = h["scale"].as<float>();
            if (h["fill"]) fill = parseColor(h["fill"]);
            if (h["stroke"]) stroke = parseColor(h["stroke"]);
            if (h["stroke-width"]) strokeWidth = h["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Heart);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy; prim.params[2] = scale;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Cross
        if (item["cross"]) {
            auto c = item["cross"];
            float cx = 0, cy = 0, w = 20, h = 5, r = 0;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (c["position"] && c["position"].IsSequence()) {
                cx = c["position"][0].as<float>(); cy = c["position"][1].as<float>();
            }
            if (c["size"] && c["size"].IsSequence()) {
                w = c["size"][0].as<float>(); h = c["size"][1].as<float>();
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
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // RoundedX
        if (item["rounded_x"]) {
            auto x = item["rounded_x"];
            float cx = 0, cy = 0, w = 20, r = 3;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (x["position"] && x["position"].IsSequence()) {
                cx = x["position"][0].as<float>(); cy = x["position"][1].as<float>();
            }
            if (x["width"]) w = x["width"].as<float>();
            if (x["round"]) r = x["round"].as<float>();
            if (x["fill"]) fill = parseColor(x["fill"]);
            if (x["stroke"]) stroke = parseColor(x["stroke"]);
            if (x["stroke-width"]) strokeWidth = x["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::RoundedX);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy; prim.params[2] = w; prim.params[3] = r;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Capsule (2D or 3D)
        if (item["capsule"]) {
            auto c = item["capsule"];
            bool is3D = c["height"].IsDefined();
            if (!is3D && c["position"] && c["position"].IsSequence() && c["position"].size() > 2) is3D = true;
            if (is3D) {
                SDFPrimitive prim = {};
                prim.type = static_cast<uint32_t>(SDFType::VerticalCapsule3D);
                prim.layer = layer;
                if (c["position"] && c["position"].IsSequence()) {
                    prim.params[0] = c["position"][0].as<float>();
                    prim.params[1] = c["position"][1].as<float>();
                    prim.params[2] = c["position"].size() > 2 ? c["position"][2].as<float>() : 0.0f;
                }
                prim.params[3] = c["height"] ? c["height"].as<float>() : 0.3f;
                prim.params[4] = c["radius"] ? c["radius"].as<float>() : 0.1f;
                if (c["fill"]) prim.fillColor = parseColor(c["fill"]);
                if (c["stroke"]) prim.strokeColor = parseColor(c["stroke"]);
                if (c["stroke-width"]) prim.strokeWidth = c["stroke-width"].as<float>();
                _builder->addPrimitive(prim);
                return Ok();
            }
            float x0 = 0, y0 = 0, x1 = 100, y1 = 0, r = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (c["from"] && c["from"].IsSequence()) { x0 = c["from"][0].as<float>(); y0 = c["from"][1].as<float>(); }
            if (c["to"] && c["to"].IsSequence()) { x1 = c["to"][0].as<float>(); y1 = c["to"][1].as<float>(); }
            if (c["radius"]) r = c["radius"].as<float>();
            if (c["fill"]) fill = parseColor(c["fill"]);
            if (c["stroke"]) stroke = parseColor(c["stroke"]);
            if (c["stroke-width"]) strokeWidth = c["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Capsule);
            prim.layer = layer;
            prim.params[0] = x0; prim.params[1] = y0;
            prim.params[2] = x1; prim.params[3] = y1; prim.params[4] = r;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Rhombus
        if (item["rhombus"]) {
            auto r = item["rhombus"];
            float cx = 0, cy = 0, w = 20, h = 30;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (r["position"] && r["position"].IsSequence()) {
                cx = r["position"][0].as<float>(); cy = r["position"][1].as<float>();
            }
            if (r["size"] && r["size"].IsSequence()) {
                w = r["size"][0].as<float>(); h = r["size"][1].as<float>();
            }
            if (r["fill"]) fill = parseColor(r["fill"]);
            if (r["stroke"]) stroke = parseColor(r["stroke"]);
            if (r["stroke-width"]) strokeWidth = r["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Rhombus);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy; prim.params[2] = w; prim.params[3] = h;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Moon
        if (item["moon"]) {
            auto m = item["moon"];
            float cx = 0, cy = 0, d = 10, ra = 20, rb = 15;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (m["position"] && m["position"].IsSequence()) {
                cx = m["position"][0].as<float>(); cy = m["position"][1].as<float>();
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
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Egg
        if (item["egg"]) {
            auto e = item["egg"];
            float cx = 0, cy = 0, ra = 20, rb = 10;
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (e["position"] && e["position"].IsSequence()) {
                cx = e["position"][0].as<float>(); cy = e["position"][1].as<float>();
            }
            if (e["radius_bottom"]) ra = e["radius_bottom"].as<float>();
            if (e["radius_top"]) rb = e["radius_top"].as<float>();
            if (e["fill"]) fill = parseColor(e["fill"]);
            if (e["stroke"]) stroke = parseColor(e["stroke"]);
            if (e["stroke-width"]) strokeWidth = e["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(SDFType::Egg);
            prim.layer = layer;
            prim.params[0] = cx; prim.params[1] = cy; prim.params[2] = ra; prim.params[3] = rb;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            _builder->addPrimitive(prim);
            return Ok();
        }

        // Generic helper for remaining 2D shapes with center+params
        auto parseGenericPrim = [&](const char* name, SDFType type,
                                    auto paramParser) -> bool {
            if (!item[name]) return false;
            auto n = item[name];
            uint32_t fill = 0, stroke = 0;
            float strokeWidth = 0;
            if (n["fill"]) fill = parseColor(n["fill"]);
            if (n["stroke"]) stroke = parseColor(n["stroke"]);
            if (n["stroke-width"]) strokeWidth = n["stroke-width"].as<float>();
            SDFPrimitive prim = {};
            prim.type = static_cast<uint32_t>(type);
            prim.layer = layer;
            prim.fillColor = fill; prim.strokeColor = stroke; prim.strokeWidth = strokeWidth;
            paramParser(n, prim);
            _builder->addPrimitive(prim);
            return true;
        };

        if (parseGenericPrim("chamfer_box", SDFType::ChamferBox, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["half_size"] && n["half_size"].IsSequence()) { p.params[2] = n["half_size"][0].as<float>(); p.params[3] = n["half_size"][1].as<float>(); }
            if (n["chamfer"]) p.params[4] = n["chamfer"].as<float>(); else p.params[4] = 5;
        })) return Ok();

        if (parseGenericPrim("oriented_box", SDFType::OrientedBox, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["a"] && n["a"].IsSequence()) { p.params[0] = n["a"][0].as<float>(); p.params[1] = n["a"][1].as<float>(); }
            if (n["b"] && n["b"].IsSequence()) { p.params[2] = n["b"][0].as<float>(); p.params[3] = n["b"][1].as<float>(); } else { p.params[2] = 30; p.params[3] = 20; }
            if (n["thickness"]) p.params[4] = n["thickness"].as<float>(); else p.params[4] = 5;
        })) return Ok();

        if (parseGenericPrim("trapezoid", SDFType::Trapezoid, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["r1"]) p.params[2] = n["r1"].as<float>(); else p.params[2] = 30;
            if (n["r2"]) p.params[3] = n["r2"].as<float>(); else p.params[3] = 15;
            if (n["height"]) p.params[4] = n["height"].as<float>(); else p.params[4] = 20;
        })) return Ok();

        if (parseGenericPrim("parallelogram", SDFType::Parallelogram, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["width"]) p.params[2] = n["width"].as<float>(); else p.params[2] = 30;
            if (n["height"]) p.params[3] = n["height"].as<float>(); else p.params[3] = 20;
            if (n["skew"]) p.params[4] = n["skew"].as<float>(); else p.params[4] = 10;
        })) return Ok();

        if (parseGenericPrim("isosceles_triangle", SDFType::IsoscelesTriangle, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["half_width"]) p.params[2] = n["half_width"].as<float>(); else p.params[2] = 15;
            if (n["height"]) p.params[3] = n["height"].as<float>(); else p.params[3] = 20;
        })) return Ok();

        if (parseGenericPrim("uneven_capsule", SDFType::UnevenCapsule, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["r1"]) p.params[2] = n["r1"].as<float>(); else p.params[2] = 10;
            if (n["r2"]) p.params[3] = n["r2"].as<float>(); else p.params[3] = 5;
            if (n["height"]) p.params[4] = n["height"].as<float>(); else p.params[4] = 20;
        })) return Ok();

        if (parseGenericPrim("cut_disk", SDFType::CutDisk, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["radius"]) p.params[2] = n["radius"].as<float>(); else p.params[2] = 20;
            if (n["h"]) p.params[3] = n["h"].as<float>(); else p.params[3] = 10;
        })) return Ok();

        if (parseGenericPrim("horseshoe", SDFType::Horseshoe, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            float angle = 1.0f; if (n["angle"]) angle = n["angle"].as<float>();
            p.params[2] = std::sin(angle); p.params[3] = std::cos(angle);
            if (n["radius"]) p.params[4] = n["radius"].as<float>(); else p.params[4] = 20;
            if (n["width"] && n["width"].IsSequence()) { p.params[5] = n["width"][0].as<float>(); p.params[6] = n["width"][1].as<float>(); }
            else { p.params[5] = 5; p.params[6] = 5; }
        })) return Ok();

        if (parseGenericPrim("vesica", SDFType::Vesica, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["width"]) p.params[2] = n["width"].as<float>(); else p.params[2] = 30;
            if (n["height"]) p.params[3] = n["height"].as<float>(); else p.params[3] = 20;
        })) return Ok();

        if (parseGenericPrim("oriented_vesica", SDFType::OrientedVesica, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["a"] && n["a"].IsSequence()) { p.params[0] = n["a"][0].as<float>(); p.params[1] = n["a"][1].as<float>(); }
            if (n["b"] && n["b"].IsSequence()) { p.params[2] = n["b"][0].as<float>(); p.params[3] = n["b"][1].as<float>(); } else { p.params[2] = 30; p.params[3] = 20; }
            if (n["width"]) p.params[4] = n["width"].as<float>(); else p.params[4] = 10;
        })) return Ok();

        if (parseGenericPrim("rounded_cross", SDFType::RoundedCross, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["h"]) p.params[2] = n["h"].as<float>(); else p.params[2] = 20;
        })) return Ok();

        if (parseGenericPrim("parabola", SDFType::Parabola, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["k"]) p.params[2] = n["k"].as<float>(); else p.params[2] = 1.0f;
        })) return Ok();

        if (parseGenericPrim("blobby_cross", SDFType::BlobbyCross, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["he"]) p.params[2] = n["he"].as<float>(); else p.params[2] = 20;
        })) return Ok();

        if (parseGenericPrim("tunnel", SDFType::Tunnel, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["width"]) p.params[2] = n["width"].as<float>(); else p.params[2] = 30;
            if (n["height"]) p.params[3] = n["height"].as<float>(); else p.params[3] = 20;
        })) return Ok();

        if (parseGenericPrim("stairs", SDFType::Stairs, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["step_width"]) p.params[2] = n["step_width"].as<float>(); else p.params[2] = 10;
            if (n["step_height"]) p.params[3] = n["step_height"].as<float>(); else p.params[3] = 5;
            if (n["count"]) p.params[4] = n["count"].as<float>(); else p.params[4] = 5;
        })) return Ok();

        if (parseGenericPrim("hyperbola", SDFType::Hyperbola, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["k"]) p.params[2] = n["k"].as<float>(); else p.params[2] = 1.0f;
            if (n["he"]) p.params[3] = n["he"].as<float>(); else p.params[3] = 20;
        })) return Ok();

        if (parseGenericPrim("circle_wave", SDFType::CircleWave, [](const YAML::Node& n, SDFPrimitive& p) {
            if (n["position"] && n["position"].IsSequence()) { p.params[0] = n["position"][0].as<float>(); p.params[1] = n["position"][1].as<float>(); }
            if (n["tb"]) p.params[2] = n["tb"].as<float>(); else p.params[2] = 1.0f;
            if (n["ra"]) p.params[3] = n["ra"].as<float>(); else p.params[3] = 20;
        })) return Ok();

        // 3D primitives
        auto parse3DPos = [](const YAML::Node& n, float* params) {
            if (n["position"] && n["position"].IsSequence()) {
                params[0] = n["position"][0].as<float>();
                params[1] = n["position"][1].as<float>();
                params[2] = n["position"].size() > 2 ? n["position"][2].as<float>() : 0.0f;
            }
        };

        if (parseGenericPrim("sphere", SDFType::Sphere3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["radius"] ? n["radius"].as<float>() : 0.25f;
        })) return Ok();

        if (parseGenericPrim("box3d", SDFType::Box3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            if (n["size"] && n["size"].IsSequence()) {
                p.params[3] = n["size"][0].as<float>();
                p.params[4] = n["size"][1].as<float>();
                p.params[5] = n["size"].size() > 2 ? n["size"][2].as<float>() : n["size"][0].as<float>();
            } else { p.params[3] = p.params[4] = p.params[5] = 0.2f; }
        })) return Ok();

        if (parseGenericPrim("torus", SDFType::Torus3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["major-radius"] ? n["major-radius"].as<float>() : 0.2f;
            p.params[4] = n["minor-radius"] ? n["minor-radius"].as<float>() : 0.08f;
        })) return Ok();

        if (parseGenericPrim("cylinder", SDFType::Cylinder3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["radius"] ? n["radius"].as<float>() : 0.15f;
            p.params[4] = n["height"] ? n["height"].as<float>() : 0.3f;
        })) return Ok();

        if (parseGenericPrim("capsule3d", SDFType::VerticalCapsule3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["height"] ? n["height"].as<float>() : 0.3f;
            p.params[4] = n["radius"] ? n["radius"].as<float>() : 0.1f;
        })) return Ok();

        if (parseGenericPrim("cone", SDFType::CappedCone3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["height"] ? n["height"].as<float>() : 0.35f;
            p.params[4] = n["radius1"] ? n["radius1"].as<float>() : 0.2f;
            p.params[5] = n["radius2"] ? n["radius2"].as<float>() : 0.05f;
        })) return Ok();

        if (parseGenericPrim("octahedron", SDFType::Octahedron3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["size"] ? n["size"].as<float>() : 0.3f;
        })) return Ok();

        if (parseGenericPrim("pyramid", SDFType::Pyramid3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            p.params[3] = n["height"] ? n["height"].as<float>() : 0.5f;
        })) return Ok();

        if (parseGenericPrim("ellipsoid", SDFType::Ellipsoid3D, [&](const YAML::Node& n, SDFPrimitive& p) {
            parse3DPos(n, p.params);
            if (n["radii"] && n["radii"].IsSequence()) {
                p.params[3] = n["radii"][0].as<float>();
                p.params[4] = n["radii"][1].as<float>();
                p.params[5] = n["radii"].size() > 2 ? n["radii"][2].as<float>() : n["radii"][0].as<float>();
            } else { p.params[3] = 0.3f; p.params[4] = 0.2f; p.params[5] = 0.15f; }
        })) return Ok();

        return Ok();
    }

    void parseAnimateBlock(const YAML::Node& animNode, uint32_t primIndex) {
        if (!animNode.IsSequence()) return;
        auto* a = anim();
        for (const auto& propNode : animNode) {
            if (!propNode["property"] || !propNode["keyframes"]) continue;
            std::string propStr = propNode["property"].as<std::string>();
            animation::PropertyType propType;
            if      (propStr == "position")       propType = animation::PropertyType::Position;
            else if (propStr == "scale")          propType = animation::PropertyType::Scale;
            else if (propStr == "fill_opacity")   propType = animation::PropertyType::FillOpacity;
            else if (propStr == "fill_color")     propType = animation::PropertyType::FillColor;
            else if (propStr == "stroke_opacity") propType = animation::PropertyType::StrokeOpacity;
            else if (propStr == "stroke_width")   propType = animation::PropertyType::StrokeWidth;
            else if (propStr == "radius")         propType = animation::PropertyType::Radius;
            else continue;

            animation::AnimatedProperty prop;
            prop.type = propType;
            prop.primitiveIndex = primIndex;
            for (const auto& kfNode : propNode["keyframes"]) {
                animation::Keyframe kf = {};
                if (kfNode["t"]) kf.time = kfNode["t"].as<float>();
                else if (kfNode["time"]) kf.time = kfNode["time"].as<float>();
                YAML::Node valNode;
                if (kfNode["v"]) valNode = kfNode["v"];
                else if (kfNode["value"]) valNode = kfNode["value"];
                if (valNode.IsSequence()) {
                    kf.componentCount = static_cast<uint8_t>(std::min(static_cast<size_t>(4), valNode.size()));
                    for (uint8_t i = 0; i < kf.componentCount; i++) kf.value[i] = valNode[i].as<float>();
                } else if (valNode.IsDefined()) {
                    kf.componentCount = 1;
                    kf.value[0] = valNode.as<float>();
                }
                prop.keyframes.push_back(kf);
            }
            a->addProperty(std::move(prop));
        }
    }

    static uint32_t parseColor(const YAML::Node& node) {
        if (!node) return 0xFFFFFFFF;
        std::string str = node.as<std::string>();
        if (str.empty()) return 0xFFFFFFFF;
        if (str[0] == '#') {
            str = str.substr(1);
            if (str.size() == 3) str = std::string{str[0], str[0], str[1], str[1], str[2], str[2]};
            if (str.size() == 6) str += "FF";
            uint32_t r = std::stoul(str.substr(0, 2), nullptr, 16);
            uint32_t g = std::stoul(str.substr(2, 2), nullptr, 16);
            uint32_t b = std::stoul(str.substr(4, 2), nullptr, 16);
            uint32_t a = std::stoul(str.substr(6, 2), nullptr, 16);
            return (a << 24) | (b << 16) | (g << 8) | r;
        }
        return 0xFFFFFFFF;
    }

    //=========================================================================
    // State
    //=========================================================================

    YDrawBuilder::Ptr _builder;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    // GPU
    StorageHandle _primStorage = StorageHandle::invalid();
    SDFPrimitive* _primitives = nullptr;
    uint32_t _primCount = 0;
    uint32_t _primCapacity = 0;
    StorageHandle _derivedStorage = StorageHandle::invalid();
    uint32_t* _grid = nullptr;
    uint32_t _gridSize = 0;
    uint32_t _primitiveOffset = 0;
    uint32_t _gridOffset = 0;
    uint32_t _glyphOffset = 0;
    uint32_t _gridWidth = 0;
    uint32_t _gridHeight = 0;
    TextureHandle _atlasTextureHandle = TextureHandle::invalid();
    uint32_t _atlasHeaderOffset = 0;
    bool _dirty = true;
    bool _metadataDirty = true;

    // View
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;

    // Animation
    std::unique_ptr<animation::Animation> _animation;
    std::vector<SDFPrimitive> _basePrimitives;
    float _lastRenderTime = -1.0f;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YDraw::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    if (!ctx.cardManager) {
        return Err<CardPtr>("YDraw::create: null CardManager");
    }
    auto card = std::make_shared<YDrawImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("YDraw::create: init failed");
    }
    return Ok<CardPtr>(card);
}

Result<YDraw::Ptr> YDraw::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;
    auto result = create(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create YDraw", result);
    auto ydraw = std::dynamic_pointer_cast<YDraw>(*result);
    if (!ydraw) return Err<Ptr>("Created card is not a YDraw");
    return Ok(ydraw);
}

} // namespace yetty::card
