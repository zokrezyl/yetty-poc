#include "ygrid.h"
#include "../../gpu-screen.h"
#include "../../ygrid/ygrid.h"
#include <yetty/yetty-context.h>
#include <yetty/font-manager.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/bm-font.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <yetty/wgpu-compat.h>
#include <cstring>
#include <sstream>
#include <algorithm>

namespace yetty::card {

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
} // namespace

//=============================================================================
// YGridImpl
//=============================================================================

class YGridImpl : public YGrid {
public:
    YGridImpl(const YettyContext& ctx,
              int32_t x, int32_t y,
              uint32_t widthCells, uint32_t heightCells,
              const std::string& args, const std::string& payload)
        : YGrid(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _ctx(ctx)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;
    }

    ~YGridImpl() override {
        dispose();
    }

    const char* typeName() const override { return "ygrid"; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }

    Result<bool> onEvent(const base::Event& event) override {
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    registerInputEvents();
                }
            } else if (_focused) {
                _focused = false;
                deregisterInputEvents();
            }
            // Don't consume — GPUScreen needs SetFocus to track _focusedCardId
            return Ok(false);
        }

        if (event.type == base::Event::Type::CardScroll &&
            event.cardScroll.targetId == id()) {

            float cardW = _gridCols * _cellWidth;
            float cardH = _gridRows * _cellHeight;

            if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
                // Zoom
                float zoomDelta = event.cardScroll.dy * 0.1f * _viewZoom;
                _viewZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 20.0f);
            } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
                // Horizontal pan
                _viewPanX += event.cardScroll.dy * 0.05f * cardW / _viewZoom;
            } else {
                // Vertical pan
                _viewPanY += event.cardScroll.dy * 0.05f * cardH / _viewZoom;
            }
            _metadataDirty = true;
            return Ok(true);
        }

        return Ok(false);
    }

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
        if (!metaResult) {
            return Err<void>("YGrid::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        // Get fonts from context
        if (_ctx.fontManager) {
            _msdfFont = _ctx.fontManager->getDefaultMsMsdfFont();
            _bitmapFont = _ctx.fontManager->getDefaultBmFont();
        }

        // Register for focus events
        if (auto res = registerForEvents(); !res) {
            ywarn("YGrid::init: event registration failed: {}", error_msg(res));
        }

        // Parse args
        parseArgs(_argsStr);

        // Parse initial payload if provided
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                return Err<void>("YGrid::init: failed to parse payload", res);
            }
        } else {
            // Default: initialize empty grid matching card dimensions
            _gridCols = _widthCells;
            _gridRows = _heightCells;
            _cells.resize(_gridCols * _gridRows);
            // Fill with spaces
            uint32_t spaceGlyph = _msdfFont ? _msdfFont->getGlyphIndex(' ') : 0;
            for (auto& cell : _cells) {
                cell = {spaceGlyph, 200, 200, 200, 255, 0, 0, 0,
                        static_cast<uint8_t>(ygrid::FONT_TYPE_MSDF << 5)};
            }
        }

        _dirty = true;
        _metadataDirty = true;
        return Ok();
    }

    Result<void> update(const std::string& args, const std::string& payload) override {
        if (!payload.empty()) {
            if (auto res = parsePayload(payload); !res) {
                return Err<void>("YGrid::update: failed to parse payload", res);
            }
        }
        return Ok();
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        _bufferHandle = BufferHandle::invalid();
        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }
        _cells.clear();
        return Ok();
    }

    void suspend() override {
        _bufferHandle = BufferHandle::invalid();
    }

    void declareBufferNeeds() override {
        if (!_cells.empty()) {
            // 3 u32s per cell (glyph, fg_packed, bg_packed) = 12 bytes per cell
            _cardMgr->bufferManager()->reserve(_cells.size() * 3 * sizeof(uint32_t));
        }
    }

    Result<void> allocateBuffers() override {
        if (_cells.empty()) return Ok();

        size_t bufferSize = _cells.size() * 3 * sizeof(uint32_t);
        auto bufResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "grid", bufferSize);
        if (!bufResult) {
            return Err<void>("YGrid::allocateBuffers: failed to allocate buffer");
        }
        _bufferHandle = *bufResult;

        // Pack cells into buffer: 3 floats per cell (stored as u32 via bitcast)
        auto* dst = reinterpret_cast<uint32_t*>(_bufferHandle.data);
        for (size_t i = 0; i < _cells.size(); i++) {
            const auto& c = _cells[i];
            dst[i * 3 + 0] = c.glyph;
            dst[i * 3 + 1] = c.fgR | (static_cast<uint32_t>(c.fgG) << 8) |
                              (static_cast<uint32_t>(c.fgB) << 16) |
                              (static_cast<uint32_t>(c.alpha) << 24);
            dst[i * 3 + 2] = c.bgR | (static_cast<uint32_t>(c.bgG) << 8) |
                              (static_cast<uint32_t>(c.bgB) << 16) |
                              (static_cast<uint32_t>(c.style) << 24);
        }
        _cardMgr->bufferManager()->markBufferDirty(_bufferHandle);

        _metadataDirty = true;
        _dirty = false;

        return Ok();
    }

    void setCellSize(float cellWidth, float cellHeight) override {
        if (_cellWidth != cellWidth || _cellHeight != cellHeight) {
            _cellWidth = cellWidth;
            _cellHeight = cellHeight;
            _metadataDirty = true;
        }
    }

    Result<void> finalize() override {
        // Re-upload cell data to existing buffer when dirty (no reallocation)
        if (_dirty && _bufferHandle.isValid() && !_cells.empty()) {
            auto* dst = reinterpret_cast<uint32_t*>(_bufferHandle.data);
            for (size_t i = 0; i < _cells.size(); i++) {
                const auto& c = _cells[i];
                dst[i * 3 + 0] = c.glyph;
                dst[i * 3 + 1] = c.fgR | (static_cast<uint32_t>(c.fgG) << 8) |
                                  (static_cast<uint32_t>(c.fgB) << 16) |
                                  (static_cast<uint32_t>(c.alpha) << 24);
                dst[i * 3 + 2] = c.bgR | (static_cast<uint32_t>(c.bgG) << 8) |
                                  (static_cast<uint32_t>(c.bgB) << 16) |
                                  (static_cast<uint32_t>(c.style) << 24);
            }
            _cardMgr->bufferManager()->markBufferDirty(_bufferHandle);
            _dirty = false;
        }
        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) {
                return Err<void>("YGrid::finalize: metadata upload failed", res);
            }
            _metadataDirty = false;
        }
        return Ok();
    }

private:
    // Metadata layout (64 bytes = 16 u32s)
    struct Metadata {
        uint32_t bufferOffset;   // Word 0: byte offset in cardStorage
        uint32_t gridDims;       // Word 1: cols (u16) | rows (u16)
        uint32_t cellWidthBits;  // Word 2: cell pixel width (as float bits)
        uint32_t cellHeightBits; // Word 3: cell pixel height (as float bits)
        uint32_t cardDims;       // Word 4: widthCells (u16) | heightCells (u16)
        uint32_t bgColor;        // Word 5: default bg color
        uint32_t fgColor;        // Word 6: default fg color
        uint32_t pixelRangeBits; // Word 7: MSDF pixel range (as float bits)
        uint32_t scaleBits;      // Word 8: font scale (as float bits)
        uint32_t viewZoomBits;   // Word 9: view zoom (as float bits)
        uint32_t viewPanXBits;   // Word 10: view pan X (as float bits)
        uint32_t viewPanYBits;   // Word 11: view pan Y (as float bits)
        uint32_t reserved[4];    // Words 12-15
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    const YettyContext& _ctx;
    std::string _argsStr;
    std::string _payloadStr;

    // Font references
    MsMsdfFont::Ptr _msdfFont;
    BmFont::Ptr _bitmapFont;

    // Grid state
    uint8_t _gridCols = 0;
    uint8_t _gridRows = 0;
    std::vector<yetty::GridCell> _cells; // Post-translation cells (glyph indices, not codepoints)

    // GPU handles
    BufferHandle _bufferHandle = BufferHandle::invalid();

    // Cell size (from terminal)
    float _cellWidth = 0.0f;
    float _cellHeight = 0.0f;

    // Default colors
    uint32_t _bgColor = 0x000000FF;
    uint32_t _fgColor = 0xC8C8C8FF;

    bool _dirty = true;
    bool _metadataDirty = true;
    bool _focused = false;

    // View state
    float _viewZoom = 1.0f;
    float _viewPanX = 0.0f;
    float _viewPanY = 0.0f;

    void parseArgs(const std::string& args) {
        std::istringstream iss(args);
        std::string token;
        while (iss >> token) {
            size_t eq = token.find('=');
            if (eq == std::string::npos) continue;
            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);

            if (key == "bg") {
                _bgColor = parseColor(val);
            } else if (key == "fg") {
                _fgColor = parseColor(val);
            } else if (key == "cols") {
                _gridCols = static_cast<uint8_t>(std::stoul(val));
            } else if (key == "rows") {
                _gridRows = static_cast<uint8_t>(std::stoul(val));
            }
        }
    }

    uint32_t parseColor(const std::string& val) {
        if (val.empty()) return 0x000000FF;
        std::string hex = val;
        if (hex[0] == '#') hex = hex.substr(1);
        if (hex.size() == 3) {
            hex = std::string{hex[0], hex[0], hex[1], hex[1], hex[2], hex[2]};
        }
        if (hex.size() != 6) return 0x000000FF;
        uint32_t rgb = std::stoul(hex, nullptr, 16);
        return (rgb << 8) | 0xFF;
    }

    Result<void> parsePayload(const std::string& payload) {
        // Create glyph lookup callbacks
        ygrid::GlyphLookup msdfLookup = nullptr;
        ygrid::GlyphLookup bitmapLookup = nullptr;

        if (_msdfFont) {
            msdfLookup = [this](uint32_t cp, bool) -> uint32_t {
                return _msdfFont->getGlyphIndex(cp);
            };
        }
        if (_bitmapFont) {
            bitmapLookup = [this](uint32_t cp, bool) -> uint32_t {
                return _bitmapFont->getGlyphIndex(cp);
            };
        }

        uint32_t defaultSpaceGlyph = _msdfFont ? _msdfFont->getGlyphIndex(' ') : 0;

        auto result = ygrid::parsePayload(payload, _cells, _gridCols, _gridRows,
                                          msdfLookup, bitmapLookup, defaultSpaceGlyph);
        if (!result) {
            return Err<void>(error_msg(result));
        }

        auto [newCols, newRows] = *result;
        _gridCols = newCols;
        _gridRows = newRows;

        _dirty = true;
        _metadataDirty = true;
        return Ok();
    }

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return Err<void>("no EventLoop", loopResult);
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
        return Ok();
    }

    void registerInputEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        loop->registerListener(base::Event::Type::CardScroll, self, 1000);
    }

    void deregisterInputEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();
        loop->deregisterListener(base::Event::Type::CardScroll, self);
    }

    void deregisterFromEvents() {
        if (weak_from_this().expired()) return;
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
    }

    Result<void> uploadMetadata() {
        Metadata meta{};
        meta.bufferOffset = _bufferHandle.isValid() ? _bufferHandle.offset : 0;
        meta.gridDims = _gridCols | (static_cast<uint32_t>(_gridRows) << 16);

        float cw = _cellWidth;
        float ch = _cellHeight;
        std::memcpy(&meta.cellWidthBits, &cw, 4);
        std::memcpy(&meta.cellHeightBits, &ch, 4);

        meta.cardDims = _widthCells | (_heightCells << 16);
        meta.bgColor = _bgColor;
        meta.fgColor = _fgColor;

        // Get pixel range and scale from MSDF font if available
        float pixelRange = 4.0f;
        float scale = 1.0f;
        if (_msdfFont && _msdfFont->atlas()) {
            pixelRange = _msdfFont->atlas()->getPixelRange();
        }
        // Scale: match gpu-screen's calculation (baseCellHeight / lineHeight)
        if (_msdfFont && _msdfFont->atlas() && _cellHeight > 0) {
            float lineHeight = _msdfFont->atlas()->getLineHeight();
            if (lineHeight > 0) {
                scale = _cellHeight / lineHeight;
            }
        }
        std::memcpy(&meta.pixelRangeBits, &pixelRange, 4);
        std::memcpy(&meta.scaleBits, &scale, 4);

        std::memcpy(&meta.viewZoomBits, &_viewZoom, 4);
        std::memcpy(&meta.viewPanXBits, &_viewPanX, 4);
        std::memcpy(&meta.viewPanYBits, &_viewPanY, 4);

        _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
        return Ok();
    }
};

//=============================================================================
// Factory
//=============================================================================

Result<YGrid::Ptr> YGrid::createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                     int32_t x, int32_t y, uint32_t widthCells,
                                     uint32_t heightCells, const std::string& args,
                                     const std::string& payload) noexcept {
    auto impl = std::make_shared<YGridImpl>(yettyCtx, x, y, widthCells, heightCells, args, payload);

    if (auto res = impl->init(); !res) {
        return Err<YGrid::Ptr>("YGrid::createImpl: init failed", res);
    }

    return Ok<YGrid::Ptr>(std::move(impl));
}

} // namespace yetty::card
