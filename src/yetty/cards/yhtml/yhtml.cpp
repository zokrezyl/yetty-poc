#include "yhtml.h"
#include "html-container.h"
#include "http-fetcher.h"
#include "../../ydraw/ydraw-builder.h"
#include "../hdraw/hdraw.h"  // For SDFPrimitive
#include <yetty/base/event-loop.h>
#include <yetty/msdf-glyph-data.h>
#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <litehtml.h>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstring>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

// Resolve a font family to TTF path via fontconfig
std::string resolveFontPath(const char* family, int weight = 400, bool italic = false) {
    std::string result;
#ifdef YETTY_USE_FONTCONFIG
    FcConfig* config = FcInitLoadConfigAndFonts();
    if (!config) return result;

    FcPattern* pattern = FcPatternCreate();
    if (!pattern) {
        FcConfigDestroy(config);
        return result;
    }

    FcPatternAddString(pattern, FC_FAMILY, reinterpret_cast<const FcChar8*>(family));

    int fcWeight = FC_WEIGHT_REGULAR;
    if (weight <= 300) fcWeight = FC_WEIGHT_LIGHT;
    else if (weight <= 400) fcWeight = FC_WEIGHT_REGULAR;
    else if (weight <= 500) fcWeight = FC_WEIGHT_MEDIUM;
    else if (weight <= 700) fcWeight = FC_WEIGHT_BOLD;
    else fcWeight = FC_WEIGHT_BLACK;
    FcPatternAddInteger(pattern, FC_WEIGHT, fcWeight);
    FcPatternAddInteger(pattern, FC_SLANT, italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN);

    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult fcResult;
    FcPattern* match = FcFontMatch(config, pattern, &fcResult);
    FcPatternDestroy(pattern);

    if (match && fcResult == FcResultMatch) {
        FcChar8* fontPath = nullptr;
        if (FcPatternGetString(match, FC_FILE, 0, &fontPath) == FcResultMatch) {
            result = reinterpret_cast<const char*>(fontPath);
        }
        FcPatternDestroy(match);
    }
    FcConfigDestroy(config);
#else
    (void)family; (void)weight; (void)italic;
#endif
    return result;
}
}

namespace yetty::card {

//=============================================================================
// YHtmlImpl - full implementation with builder, GPU buffers, litehtml
//=============================================================================

class YHtmlImpl : public YHtml {
public:
    YHtmlImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YHtml(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
        auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
        if (builderRes) {
            _builder = *builderRes;
        } else {
            yerror("YHtmlImpl: failed to create builder");
        }
    }

    ~YHtmlImpl() override { dispose(); }

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
                yerror("YHtmlImpl::dispose: deallocateMetadata failed: {}", error_msg(res));
            }
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {
        if (_primStorage.isValid() && _primCount > 0 && _builder) {
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
            // Still need to reserve atlas space if atlas exists
            if (_builder->hasCustomAtlas()) {
                uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
                uint32_t glyphMetaBytes = static_cast<uint32_t>(
                    _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
                uint32_t atlasSize = atlasHeaderBytes + glyphMetaBytes;
                if (atlasSize > 0) {
                    _cardMgr->bufferManager()->reserve(atlasSize);
                }
            }
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

        yinfo("YHtmlImpl::declareBufferNeeds: primStaging={} gridStaging={} glyphs={} derivedSize={} gridBytes={} glyphBytes={}",
              primStaging.size(), _builder->gridStaging().size(), glyphs.size(),
              derivedSize, gridBytes, glyphBytes);

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
                return Err<void>("YHtmlImpl::allocateBuffers: prim alloc failed");
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

        yinfo("YHtmlImpl::allocateBuffers: gridStaging={} glyphs={} derivedSize={} gridBytes={} glyphBytes={}",
              gridStaging.size(), glyphs.size(), derivedSize, gridBytes, glyphBytes);

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                yerror("YHtmlImpl::allocateBuffers: derived alloc FAILED: derivedSize={}", derivedSize);
                return Err<void>("YHtmlImpl::allocateBuffers: derived alloc failed");
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
            return Err<void>("YHtmlImpl::allocateTextures: failed", allocResult);
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
                    return Err<void>("YHtmlImpl::writeTextures: write failed", res);
                }
            }
        }
        return Ok();
    }

    Result<void> render(float /*time*/) override {
        if (!_builder) return Ok();

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
            return Err<void>("YHtmlImpl::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        if (auto res = registerForEvents(); !res) {
            ywarn("YHtmlImpl::init: event registration failed: {}", error_msg(res));
        }

        // White background for HTML
        _builder->setBgColor(0xFFFFFFFF);

        // Load default sans-serif font (creates custom atlas via builder)
        std::string defaultFontPath = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
        auto fontResult = _builder->addFont(defaultFontPath);
        if (!fontResult) {
            return Err<void>("YHtmlImpl::init: failed to load default sans-serif font");
        }
        yinfo("YHtmlImpl::init: loaded default sans-serif font: {} -> fontId={}", defaultFontPath, *fontResult);

        // Create HTTP fetcher
        auto fetcherResult = HttpFetcher::create();
        if (!fetcherResult) {
            return Err<void>("YHtmlImpl: failed to create HttpFetcher");
        }
        _fetcher = std::move(*fetcherResult);

        parseArgs(_argsStr);

        // Load HTML content
        if (auto res = loadContent(); !res) {
            ywarn("YHtmlImpl::init: failed to load content: {}", error_msg(res));
        }

        // Render HTML immediately
        if (!_htmlContent.empty()) {
            if (auto res = renderHtml(); !res) {
                ywarn("YHtmlImpl::init: render failed: {}", error_msg(res));
            }
        }

        yinfo("YHtmlImpl::init: {} prims, {} glyphs",
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
        _builder->computeSceneBoundsFromPrims(_primitives, _primCount);

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
                    return Err<void>("YHtmlImpl::rebuild: derived alloc failed");
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
            return Err<void>("YHtmlImpl::uploadMetadata: invalid handle");
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
            return Err<void>("YHtmlImpl::uploadMetadata: write failed");
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
    // Args parsing
    //=========================================================================

    void parseArgs(const std::string& args) {
        if (args.empty()) return;

        std::istringstream iss(args);
        std::string token;

        while (iss >> token) {
            if (token == "-i" || token == "--input") {
                std::string val;
                if (iss >> val) _inputSource = val;
            } else if (token == "--font-size") {
                float val;
                if (iss >> val) _fontSize = val;
            } else if (token == "--bg-color") {
                std::string colorStr;
                if (iss >> colorStr) {
                    if (colorStr.size() > 2 &&
                        (colorStr.substr(0, 2) == "0x" ||
                         colorStr.substr(0, 2) == "0X")) {
                        colorStr = colorStr.substr(2);
                    }
                    _builder->setBgColor(static_cast<uint32_t>(
                        std::stoul(colorStr, nullptr, 16)));
                }
            }
        }
    }

    //=========================================================================
    // Content loading
    //=========================================================================

    Result<void> loadContent() {
        if (_inputSource == "-" || _inputSource.empty()) {
            // "-" means payload, empty also falls back to payload
            if (_payloadStr.empty()) {
                return Err<void>("YHtmlImpl::loadContent: no payload");
            }
            yinfo("YHtmlImpl::loadContent: using payload ({} bytes)", _payloadStr.size());
            _htmlContent = _payloadStr;
        } else if (HttpFetcher::isUrl(_inputSource)) {
            _fetcher->setBaseUrl(_inputSource);
            yinfo("YHtmlImpl::loadContent: fetching URL: {}", _inputSource);
            auto body = _fetcher->fetch(_inputSource);
            if (!body) {
                return Err<void>("YHtmlImpl::loadContent: failed to fetch URL");
            }
            _htmlContent = std::move(*body);
        } else {
            yinfo("YHtmlImpl::loadContent: reading file: {}", _inputSource);
            std::ifstream file(_inputSource);
            if (!file) {
                return Err<void>("YHtmlImpl::loadContent: failed to open file");
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            _htmlContent = buffer.str();
        }

        if (_htmlContent.empty()) {
            return Err<void>("YHtmlImpl::loadContent: empty content");
        }

        yinfo("YHtmlImpl::loadContent: {} bytes", _htmlContent.size());
        return Ok();
    }

    //=========================================================================
    // HTML rendering via litehtml
    //=========================================================================

    Result<void> renderHtml() {
        if (_htmlContent.empty()) {
            return Err<void>("YHtmlImpl::renderHtml: no content loaded");
        }

        // Create container that targets the builder
        auto containerResult = HtmlContainer::create(
            _builder.get(), nullptr, _fontSize, _fetcher.get());
        if (!containerResult) {
            return Err<void>("YHtmlImpl::renderHtml: failed to create HtmlContainer");
        }
        _container = std::move(*containerResult);

        int viewW = static_cast<int>(_viewWidth);
        int viewH = static_cast<int>(_viewWidth * 1.5f);
        _container->setViewportSize(viewW, viewH);

        _document = litehtml::document::createFromString(
            _htmlContent.c_str(), _container.get());

        if (!_document) {
            return Err<void>("YHtmlImpl::renderHtml: failed to parse HTML");
        }

        _document->render(viewW);

        int docHeight = _document->height();
        _container->setViewportSize(viewW, docHeight);

        yinfo("YHtmlImpl::renderHtml: BEFORE draw: viewW={} docHeight={} prims={} glyphs={}",
              viewW, docHeight, _builder->primitiveCount(), _builder->glyphCount());

        litehtml::position clip(0, 0, viewW, docHeight);
        _document->draw(0, 0, 0, &clip);

        yinfo("YHtmlImpl::renderHtml: AFTER draw: prims={} glyphs={}",
              _builder->primitiveCount(), _builder->glyphCount());

        return Ok();
    }

    //=========================================================================
    // Members
    //=========================================================================

    // Builder
    YDrawBuilder::Ptr _builder;
    base::ObjectId _screenId = 0;
    std::string _argsStr;
    std::string _payloadStr;

    // GPU state
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

    // View zoom/pan
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;
    bool _focused = false;

    // HTTP client
    std::shared_ptr<HttpFetcher> _fetcher;

    // HTML state
    std::string _htmlContent;
    std::string _inputSource;
    std::shared_ptr<HtmlContainer> _container;
    std::shared_ptr<litehtml::document> _document;
    float _fontSize = 16.0f;
    float _viewWidth = 600.0f;
};

//=============================================================================
// Factory
//=============================================================================

Result<YHtml::Ptr> YHtml::createImpl(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YHtml::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto card = std::make_shared<YHtmlImpl>(ctx, x, y, widthCells, heightCells,
                                             args, payload);

    if (auto res = card->init(); !res) {
        yerror("YHtml::create: init FAILED: {}", error_msg(res));
        return Err<Ptr>("YHtml::create: init failed");
    }

    yinfo("YHtml::create: SUCCESS");
    return Ok(std::move(card));
}

} // namespace yetty::card
