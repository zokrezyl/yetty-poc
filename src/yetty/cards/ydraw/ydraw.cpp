#include "ydraw.h"
#include <yetty/ydraw-builder.h>
#include "../../ydraw/animation.h"
#include "../../ydraw/ydraw-types.gen.h"
#include "../../ydraw/ydraw-prim-writer.gen.h"
#include "../../ydraw/yaml2ydraw.h"
#include <yetty/base/event-loop.h>
#include <yetty/msdf-glyph-data.h>
#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
constexpr int GLFW_KEY_C       = 67;
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
        _buffer = *YDrawBuffer::create();
        _fontManager = ctx.fontManager;
        _gpuAllocator = ctx.gpuAllocator;
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
        _primWordOffsets.clear();
        if (_metaHandle.isValid() && _cardMgr) {
            if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
                yerror("YDrawImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primWordOffsets.clear();
    }

    void declareBufferNeeds() override {
        if (!_builder) return;

        _derivedStorage = StorageHandle::invalid();
        _grid = nullptr;
        _gridSize = 0;
        _primStorage = StorageHandle::invalid();
        _primWordOffsets.clear();

        const auto& glyphs = _builder->glyphs();

        if (_buffer->empty() && glyphs.empty()) {
            _builder->clearGridStaging();
            return;
        }

        // Reserve compact prim storage: offset table + compact data
        if (!_buffer->empty()) {
            uint32_t compactBytes = _buffer->gpuBufferSize();
            _cardMgr->bufferManager()->reserve(compactBytes);
        }

        // Calculate grid if needed
        if (_builder->gridStaging().empty()) {
            _builder->calculate();

            // Auto-compute zoom/pan to fit one page height after scene bounds are known
            if (_fitPageHeight > 0.0f) {
                float sceneMinY = _builder->sceneMinY();
                float sceneMaxY = _builder->sceneMaxY();
                float sceneH = sceneMaxY - sceneMinY;
                if (sceneH > _fitPageHeight) {
                    _viewZoom = sceneH / _fitPageHeight;
                    // Pan so viewMinY = 0 (first page top)
                    // viewMinY = centerY - viewHalfH + panY = 0
                    // panY = viewHalfH - centerY = fitPageH/2 - (min+max)/2
                    float centerY = (sceneMinY + sceneMaxY) * 0.5f;
                    _viewPanY = _fitPageHeight * 0.5f - centerY;
                }
                _fitPageHeight = 0.0f; // only apply once
            }
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

        const auto& gridStaging = _builder->gridStaging();
        const auto& glyphs = _builder->glyphs();

        // Write primitives to GPU compact format
        if (!_buffer->empty()) {
            uint32_t allocBytes = _buffer->gpuBufferSize();
            auto primResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "prims", allocBytes);
            if (!primResult) {
                return Err<void>("YDrawImpl::allocateBuffers: prim alloc failed");
            }
            _primStorage = *primResult;

            writeCompactToBuffer();

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

            // Copy grid and translate prim indices to word offsets
            _grid = reinterpret_cast<uint32_t*>(base + offset);
            _gridSize = static_cast<uint32_t>(gridStaging.size());
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            if (!gridStaging.empty()) {
                std::memcpy(base + offset, gridStaging.data(), gridBytes);
                translateGridEntries(_grid, _gridSize,
                                    _builder->gridWidth(), _builder->gridHeight());
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
        // Keep _dirty = true so render() writes the full derived storage
        // (including atlas header which needs the texture handle from Phase 2 textures)
        _dirty = true;

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

    bool needsBufferRealloc() override {
        if (_needsBufferRealloc) {
            _needsBufferRealloc = false;
            return true;
        }
        return false;
    }

    void renderToStaging(float time) override {
        if (!_builder) return;

        // Auto-start animation
        if (_animation && _animation->hasProperties() && !_animation->isPlaying()
            && !_animationStarted && !_buffer->empty()) {
            startAnimation();
        }

        // Animation update — advance time, apply to buffer, re-write GPU
        if (_animation && _animation->isPlaying() && !_buffer->empty()) {
            float dt = (_lastRenderTime < 0.0f) ? 0.0f : (time - _lastRenderTime);
            _lastRenderTime = time;
            if (_animation->advance(dt)) {
                _animation->apply();
                // Re-write GPU prim data
                if (_primStorage.isValid()) {
                    float* buf = reinterpret_cast<float*>(_primStorage.data);
                    _buffer->writeGPU(buf, _primStorage.size, _primWordOffsets);
                    _cardMgr->bufferManager()->markBufferDirty(_primStorage);
                }
                _dirty = true;
            }
        }

        // If dirty, recalculate grid
        if (_dirty && !_buffer->empty()) {
            _builder->calculate();

            uint32_t needed = computeDerivedSize();
            if (!_derivedStorage.isValid() || needed > _derivedStorage.size) {
                _needsBufferRealloc = true;
            }
        }
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();

        if (_dirty) {
            uint32_t derivedSize = computeDerivedSize();

            if (derivedSize > 0) {
                if (!_derivedStorage.isValid()) {
                    return Err<void>("YDrawImpl::render: derived storage not allocated");
                }
                if (derivedSize > _derivedStorage.size) {
                    return Err<void>("YDrawImpl::render: derived storage too small ("
                        + std::to_string(derivedSize) + " > "
                        + std::to_string(_derivedStorage.size) + ")");
                }

                uint8_t* base = _derivedStorage.data;
                std::memset(base, 0, _derivedStorage.size);
                uint32_t offset = 0;

                const auto& gridData = _builder->gridStaging();
                uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
                _grid = reinterpret_cast<uint32_t*>(base + offset);
                _gridSize = static_cast<uint32_t>(gridData.size());
                _gridWidth = _builder->gridWidth();
                _gridHeight = _builder->gridHeight();
                _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
                std::memcpy(base + offset, gridData.data(), gridBytes);
                // Translate prim indices to word offsets in grid entries
                translateGridEntries(_grid, _gridSize, _gridWidth, _gridHeight);
                offset += gridBytes;

                uint32_t glyphBytes = static_cast<uint32_t>(
                    _builder->glyphs().size() * sizeof(YDrawGlyph));
                _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
                const auto& glyphs = _builder->glyphs();
                if (!glyphs.empty()) {
                    std::memcpy(base + offset, glyphs.data(), glyphBytes);
                }
                offset += glyphBytes;

                if (_builder->hasCustomAtlas() && _atlasTextureHandle.isValid()) {
                    auto atlas = _builder->customAtlas();
                    auto atlasPos = _cardMgr->textureManager()->getAtlasPosition(
                        _atlasTextureHandle);
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

            _primitiveOffset = _primStorage.isValid()
                ? _primStorage.offset / sizeof(float) : 0;
            if (_primStorage.isValid()) {
                _cardMgr->bufferManager()->markBufferDirty(_primStorage);
            }
            _metadataDirty = true;
            _dirty = false;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) return res;
            _metadataDirty = false;
        }

        return Ok();
    }

    //=========================================================================
    // Events - zoom/pan + text selection
    //=========================================================================

    void setCellSize(float cellWidth, float cellHeight) override {
        _cellWidth = cellWidth;
        _cellHeight = cellHeight;
    }

    void cardPixelToScene(float cardX, float cardY, float& sceneX, float& sceneY) {
        float cardPixelW = static_cast<float>(_widthCells * _cellWidth);
        float cardPixelH = static_cast<float>(_heightCells * _cellHeight);
        if (cardPixelW <= 0 || cardPixelH <= 0) { sceneX = sceneY = 0; return; }

        float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
        float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
        float centerX = _builder->sceneMinX() + contentW * 0.5f;
        float centerY = _builder->sceneMinY() + contentH * 0.5f;
        float viewHalfW = contentW * 0.5f / _viewZoom;
        float viewHalfH = contentH * 0.5f / _viewZoom;
        float viewMinX = centerX - viewHalfW + _viewPanX;
        float viewMinY = centerY - viewHalfH + _viewPanY;
        float viewW = contentW / _viewZoom;
        float viewH = contentH / _viewZoom;

        float cardAspect = cardPixelW / std::max(cardPixelH, 1.0f);
        float viewAspect = viewW / std::max(viewH, 1e-6f);
        float uvX = cardX / cardPixelW;
        float uvY = cardY / cardPixelH;

        if (viewAspect < cardAspect) {
            float visibleW = viewH * cardAspect;
            float offsetX = (visibleW - viewW) * 0.5f;
            sceneX = viewMinX - offsetX + uvX * visibleW;
            sceneY = viewMinY + uvY * viewH;
        } else {
            float visibleH = viewW / cardAspect;
            float offsetY = (visibleH - viewH) * 0.5f;
            sceneX = viewMinX + uvX * viewW;
            sceneY = viewMinY - offsetY + uvY * visibleH;
        }
    }

    void uploadGlyphData() {
        if (!_derivedStorage.isValid() || _builder->glyphs().empty()) return;
        uint32_t localOff = _glyphOffset * sizeof(float) - _derivedStorage.offset;
        auto& glyphs = _builder->glyphsMut();
        std::memcpy(_derivedStorage.data + localOff, glyphs.data(),
                    glyphs.size() * sizeof(YDrawGlyph));
        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

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
                float zoomDelta = event.cardScroll.dy * 0.1f * _viewZoom;
                float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 1000.0f);
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

        // Mouse selection
        if (event.type == base::Event::Type::CardMouseDown &&
            event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
            if (_builder->glyphCount() == 0) return Ok(false);
            if (!_sortedOrderBuilt) {
                _builder->buildGlyphSortedOrder();
                _sortedOrderBuilt = true;
            }
            float sx, sy;
            cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
            int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
            _selStartSorted = idx;
            _selEndSorted = idx;
            _selecting = true;
            _builder->setSelectionRange(_selStartSorted, _selEndSorted);
            uploadGlyphData();
            return Ok(true);
        }

        if (event.type == base::Event::Type::CardMouseMove &&
            event.cardMouse.targetId == id() && _selecting) {
            float sx, sy;
            cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
            int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
            if (idx != _selEndSorted) {
                _selEndSorted = idx;
                _builder->setSelectionRange(_selStartSorted, _selEndSorted);
                uploadGlyphData();
            }
            return Ok(true);
        }

        if (event.type == base::Event::Type::CardMouseUp &&
            event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
            if (_selecting) {
                float sx, sy;
                cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
                _selEndSorted = _builder->findNearestGlyphSorted(sx, sy);
                _selecting = false;
                _builder->setSelectionRange(_selStartSorted, _selEndSorted);
                uploadGlyphData();
            }
            return Ok(true);
        }

        // Ctrl+C: copy selected text
        if (event.type == base::Event::Type::CardKeyDown &&
            event.cardKey.targetId == id() &&
            event.cardKey.key == GLFW_KEY_C &&
            (event.cardKey.mods & GLFW_MOD_CONTROL)) {
            std::string text = _builder->getSelectedText();
            if (!text.empty()) {
                auto loop = *base::EventLoop::instance();
                loop->dispatch(base::Event::copyEvent(
                    std::make_shared<std::string>(text)));
                yinfo("YDraw: copied {} bytes to clipboard", text.size());
            }
            return Ok(true);
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

        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes) {
            return Err<void>("YDrawImpl::init: failed to create builder", builderRes);
        }
        _builder = *builderRes;

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
    // Helpers
    //=========================================================================

    // computeCompactSize, writeCompactToBuffer, translateGridEntries
    // are now shared via sdf:: namespace in ydraw-prim-writer.gen.h

    void writeCompactToBuffer() {
        if (!_primStorage.isValid() || _buffer->empty()) return;
        float* buf = reinterpret_cast<float*>(_primStorage.data);
        _buffer->writeGPU(buf, _primStorage.size, _primWordOffsets);
    }

    void translateGridEntries(uint32_t* grid, uint32_t gridSize,
                              uint32_t gridW, uint32_t gridH) {
        sdf::translateGridEntries(grid, gridSize, gridW, gridH, _primWordOffsets);
    }

    uint32_t computeDerivedSize() const {
        const auto& gridData = _builder->gridStaging();
        uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(
            _builder->glyphs().size() * sizeof(YDrawGlyph));
        uint32_t total = gridBytes + glyphBytes;
        if (_builder->hasCustomAtlas()) {
            uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
            uint32_t glyphMetaBytes = static_cast<uint32_t>(
                _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
            total += atlasHeaderBytes + glyphMetaBytes;
        }
        return total;
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
        meta.primitiveCount = _buffer->primCount();
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
        if (auto res = loop->registerListener(base::Event::Type::CardMouseDown, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardMouseUp, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardMouseMove, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardKeyDown, self, 1000); !res) return res;
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
            auto res = animation::Animation::create(_buffer);
            if (res) _animation = *res;
        }
        return _animation.get();
    }

    void startAnimation() {
        if (!_animation || !_animation->hasProperties()) return;
        if (_buffer->empty()) return;

        _animation->snapshotBase();
        _animationStarted = true;
        _lastRenderTime = -1.0f;

        if (!_builder->hasExplicitBounds()) {
            float minX = _builder->sceneMinX();
            float minY = _builder->sceneMinY();
            float maxX = _builder->sceneMaxX();
            float maxY = _builder->sceneMaxY();
            _animation->expandBounds(minX, minY, maxX, maxY);
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
                    _buffer->setBgColor(static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16)));
                }
            } else if (token == "--cell-size") {
                std::string s; if (iss >> s) _builder->setGridCellSize(std::stof(s));
            } else if (token == "--max-prims-per-cell") {
                std::string s; if (iss >> s) _builder->setMaxPrimsPerCell(static_cast<uint32_t>(std::stoul(s)));
            } else if (token == "--show-bounds") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_BOUNDS);
            } else if (token == "--show-grid") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_GRID);
            } else if (token == "--show-eval-count") {
                _buffer->addFlags(YDrawBuffer::FLAG_SHOW_EVAL_COUNT);
            } else if (token == "--zoom") {
                std::string s;
                if (iss >> s) _viewZoom = std::clamp(std::stof(s), 0.1f, 1000.0f);
            } else if (token == "--pan-y") {
                std::string s;
                if (iss >> s) _viewPanY = std::stof(s);
            } else if (token == "--fit-page-height") {
                std::string s;
                if (iss >> s) _fitPageHeight = std::stof(s);
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
        if (isYaml) {
            auto res = parseYDrawYAML(_buffer, payload);
            if (!res) return Err<void>(res.error().message());
            if (*res) _animation = *res;
            return Ok();
        }
        return parseBinary(payload);
    }

    Result<void> parseBinary(const std::string& payload) {
        const size_t HEADER_SIZE = 16;

        if (payload.size() < HEADER_SIZE) return Ok();

        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());
        uint32_t version, primCount, bgColorVal, flagsVal;
        std::memcpy(&version, data + 0, 4);
        std::memcpy(&primCount, data + 4, 4);
        std::memcpy(&bgColorVal, data + 8, 4);
        std::memcpy(&flagsVal, data + 12, 4);

        if (version != 3) return Ok();

        _buffer->setBgColor(bgColorVal);
        _buffer->addFlags(flagsVal);

        // Read totalCompactWords + compact prim data
        if (payload.size() < HEADER_SIZE + 4) return Ok();
        uint32_t totalCompactWords;
        std::memcpy(&totalCompactWords, data + HEADER_SIZE, 4);

        size_t primDataStart = HEADER_SIZE + 4;
        size_t primDataBytes = static_cast<size_t>(totalCompactWords) * sizeof(float);
        if (payload.size() < primDataStart + primDataBytes) return Ok();

        // Build a serialized buffer blob: [primCount:u32][totalWords:u32][compact_data]
        size_t blobSize = 8 + primDataBytes;
        std::vector<uint8_t> blob(blobSize);
        std::memcpy(blob.data(), &primCount, 4);
        std::memcpy(blob.data() + 4, &totalCompactWords, 4);
        std::memcpy(blob.data() + 8, data + primDataStart, primDataBytes);
        if (auto res = _buffer->deserialize(blob.data(), blob.size()); !res) {
            return res;
        }

        // Font + text sections follow the prim data
        size_t textSectionOffset = primDataStart + primDataBytes;
        if (auto res = parseFontAndText(data, payload.size(), textSectionOffset); !res) {
            ywarn("parseBinary: text section failed: {}", error_msg(res));
        }

        return Ok();
    }

    Result<void> parseFontAndText(const uint8_t* data, size_t totalSize, size_t offset) {
        auto read32 = [&](uint32_t& val) -> bool {
            if (offset + 4 > totalSize) return false;
            std::memcpy(&val, data + offset, 4);
            offset += 4;
            return true;
        };
        auto readF32 = [&](float& val) -> bool {
            if (offset + 4 > totalSize) return false;
            std::memcpy(&val, data + offset, 4);
            offset += 4;
            return true;
        };

        // --- Font Section ---
        uint32_t fontCount = 0;
        if (!read32(fontCount)) return Ok();

        std::vector<int> fontIdMap(fontCount, -1);

        for (uint32_t i = 0; i < fontCount; i++) {
            uint32_t fontDataSize = 0;
            if (!read32(fontDataSize)) return Ok();

            uint32_t paddedSize = (fontDataSize + 3) & ~3u;
            if (offset + paddedSize > totalSize) return Ok();

            int bufFontId = _buffer->addFontBlob(
                data + offset, fontDataSize,
                "font_" + std::to_string(i));
            fontIdMap[i] = bufFontId;
            yinfo("parseFontAndText: registered font {} from {} bytes -> bufFontId={}",
                  i, fontDataSize, bufFontId);

            offset += paddedSize;
        }

        // --- Text Section ---
        uint32_t defaultSpanCount = 0, customSpanCount = 0, textDataSize = 0;
        if (!read32(defaultSpanCount)) return Ok();
        if (!read32(customSpanCount)) return Ok();
        if (!read32(textDataSize)) return Ok();

        // Remember where span arrays and text data start
        size_t defaultSpansStart = offset;
        size_t customSpansStart = defaultSpansStart + defaultSpanCount * 32;
        size_t textDataStart = customSpansStart + customSpanCount * 32;
        uint32_t paddedTextSize = (textDataSize + 3) & ~3u;

        if (textDataStart + paddedTextSize > totalSize) {
            ywarn("parseFontAndText: text section truncated");
            return Ok();
        }

        const char* textBlob = reinterpret_cast<const char*>(data + textDataStart);

        // Parse default text spans (use default MsMsdfFont)
        offset = defaultSpansStart;
        for (uint32_t i = 0; i < defaultSpanCount; i++) {
            float x, y, fontSize;
            uint32_t color, layer, textOffset, textLength, pad;
            if (!readF32(x) || !readF32(y) || !readF32(fontSize) ||
                !read32(color) || !read32(layer) ||
                !read32(textOffset) || !read32(textLength) || !read32(pad))
                break;

            if (textOffset + textLength > textDataSize) continue;
            std::string text(textBlob + textOffset, textLength);
            _buffer->addText(x, y, text, fontSize, color, layer, -1);
        }

        // Parse custom text spans (use uploaded fonts)
        offset = customSpansStart;
        for (uint32_t i = 0; i < customSpanCount; i++) {
            float x, y, fontSize;
            uint32_t color, fontIndex, layer, textOffset, textLength;
            if (!readF32(x) || !readF32(y) || !readF32(fontSize) ||
                !read32(color) || !read32(fontIndex) || !read32(layer) ||
                !read32(textOffset) || !read32(textLength))
                break;

            if (textOffset + textLength > textDataSize) continue;
            std::string text(textBlob + textOffset, textLength);

            int fid = (fontIndex < fontCount && fontIdMap[fontIndex] >= 0)
                      ? fontIdMap[fontIndex] : -1;
            _buffer->addText(x, y, text, fontSize, color, layer, fid);
        }

        // --- Rotated Text Spans (optional, after text blob) ---
        offset = textDataStart + paddedTextSize;
        uint32_t rotatedSpanCount = 0;
        if (read32(rotatedSpanCount) && rotatedSpanCount > 0) {
            for (uint32_t i = 0; i < rotatedSpanCount; i++) {
                float rx, ry, rFontSize, rRotation;
                uint32_t rColor, rFontIndex, rTextOffset, rTextLength;
                if (!readF32(rx) || !readF32(ry) || !readF32(rFontSize) ||
                    !readF32(rRotation) || !read32(rColor) || !read32(rFontIndex) ||
                    !read32(rTextOffset) || !read32(rTextLength))
                    break;

                if (rTextOffset + rTextLength > textDataSize) continue;
                std::string text(textBlob + rTextOffset, rTextLength);

                int fid = (rFontIndex < fontCount && fontIdMap[rFontIndex] >= 0)
                          ? fontIdMap[rFontIndex] : -1;
                _buffer->addRotatedText(rx, ry, text, rFontSize,
                                        rColor, rRotation, fid);
            }
        }

        yinfo("parseFontAndText: {} fonts, {} default spans, {} custom spans, "
              "{} rotated spans, {} text bytes",
              fontCount, defaultSpanCount, customSpanCount,
              rotatedSpanCount, textDataSize);

        return Ok();
    }

    // Old parseYAML/parseYAMLPrimitive/parseColor removed — now in yaml2ydraw.cpp
    //=========================================================================
    // State
    //=========================================================================

    YDrawBuilder::Ptr _builder;
    YDrawBuffer::Ptr _buffer;
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    // GPU compact prim storage: [offset_table (primCount words)] [compact_prim_data]
    StorageHandle _primStorage = StorageHandle::invalid();

    // Prim index → word offset relative to primDataBase (for grid translation)
    std::vector<uint32_t> _primWordOffsets;
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
    bool _needsBufferRealloc = false;

    // View
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _fitPageHeight = 0.0f; // if set, compute zoom/pan to fit one page
    float _viewPanY = 0.0f;
    bool _focused = false;

    // Cell pixel size (from setCellSize)
    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;

    // Text selection
    bool _selecting = false;
    int32_t _selStartSorted = -1;
    int32_t _selEndSorted = -1;
    bool _sortedOrderBuilt = false;

    // Animation
    animation::Animation::Ptr _animation;
    bool _animationStarted = false;
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
