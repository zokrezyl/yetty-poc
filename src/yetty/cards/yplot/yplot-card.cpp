// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "yplot-card.h"
#include <yetty/yetty-context.h>
#include <yetty/yfsvm/yfsvm.h>
#include <yetty/yfsvm/yfsvm-compiler.h>
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>
#include <cstring>
#include <algorithm>

// GLFW modifier constants
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;

namespace yetty::card {

//=============================================================================
// Metadata structure (64 bytes)
//=============================================================================

struct YPlotMetadata {
    // [0]: flags(8) | funcCount(8) | glyphBase0(8) | glyphDot(8)
    uint8_t flags;
    uint8_t funcCount;
    uint8_t glyphBase0;  // MSDF glyph index for '0'
    uint8_t glyphDot;    // MSDF glyph index for '.'

    // [1]: widthCells(16) | heightCells(16)
    uint16_t widthCells;
    uint16_t heightCells;

    // [2-3]: bytecode
    uint32_t bytecodeOffset;
    uint32_t bytecodeSize;

    // [4-7]: range
    float xMin;
    float xMax;
    float yMin;
    float yMax;

    // [8-9]: data buffer (for buffer mode)
    uint32_t dataOffset;   // float index into cardStorage
    uint32_t dataCount;    // number of floats

    // [10-11]: plot area (normalized coords)
    float marginLeft;
    float marginBottom;

    // [12-14]: view/pan
    float zoom;
    float centerX;
    float centerY;

    // [15]: colorTableOffset(24) | glyphMinus(8)
    uint32_t colorTableOffsetAndGlyph;  // Lower 24 bits = offset, upper 8 bits = glyphMinus
};
static_assert(sizeof(YPlotMetadata) == 64, "YPlotMetadata must be 64 bytes");

//=============================================================================
// YPlotCardImpl - Implementation
//=============================================================================

class YPlotCardImpl : public YPlotCard {
public:
    YPlotCardImpl(const YettyContext& ctx,
                  int32_t x, int32_t y,
                  uint32_t widthCells, uint32_t heightCells,
                  const std::string& args, const std::string& payload)
        : YPlotCard(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _fontMgr(ctx.fontManager)
        , _allocator(ctx.gpuAllocator)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        // Create YPlotState
        auto stateResult = yplot::YPlotState::create();
        if (stateResult) {
            _state = *stateResult;
        }

        // Look up glyph indices for axis label characters
        if (_fontMgr) {
            auto font = _fontMgr->getDefaultMsMsdfFont();
            if (font) {
                _glyphBase0 = font->getGlyphIndex('0');
                _glyphDot = font->getGlyphIndex('.');
                _glyphMinus = font->getGlyphIndex('-');
                ydebug("YPlotCard: glyph indices: '0'={} '.'={} '-'={}",
                      _glyphBase0, _glyphDot, _glyphMinus);
            }
        }
    }

    ~YPlotCardImpl() override {
        dispose();
    }

    //=========================================================================
    // Initialization
    //=========================================================================

    Result<void> init() {
        // Allocate metadata slot
        auto metaResult = _cardMgr->allocateMetadata(sizeof(YPlotMetadata));
        if (!metaResult) {
            return Err<void>("YPlotCard::init: failed to allocate metadata");
        }
        _metaHandle = *metaResult;

        ydebug("YPlotCard::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        if (auto res = parseArgs(_argsStr); !res) {
            ywarn("YPlotCard::init: failed to parse args: {}", error_msg(res));
        }

        // Parse payload for buffer mode (comma-separated floats)
        if (!_payloadStr.empty()) {
            if (auto res = parsePayload(_payloadStr); !res) {
                ywarn("YPlotCard::init: failed to parse payload: {}", error_msg(res));
            }
        }

        // Compile expressions if any
        if (_state && _state->functionCount() > 0) {
            if (auto res = compileExpressions(); !res) {
                return Err<void>("YPlotCard::init: failed to compile expressions", res);
            }
        }

        // NOTE: The yplot shader draws its own grid and axes.
        // Do NOT use YDrawBuilder here - it would overwrite YPlot metadata.

        // Register for events (pan/zoom via scroll)
        if (auto res = registerForEvents(); !res) {
            return Err<void>("YPlotCard::init: failed to register for events", res);
        }

        return Ok();
    }

    //=========================================================================
    // Card interface - lifecycle
    //=========================================================================

    bool needsBufferRealloc() override {
        // Need realloc if we have bytecode but no valid handle
        if (!_bytecode.empty() && !_bytecodeHandle.isValid()) return true;
        if (!_colorTable.empty() && !_colorHandle.isValid()) return true;
        if (!_data.empty() && !_dataHandle.isValid()) return true;
        return false;
    }

    void declareBufferNeeds() override {
        // Invalidate handles
        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();
        _dataHandle = BufferHandle::invalid();

        // Reserve for bytecode
        if (!_bytecode.empty()) {
            uint32_t bytecodeSize = static_cast<uint32_t>(_bytecode.size() * sizeof(uint32_t));
            _cardMgr->bufferManager()->reserve(bytecodeSize);
        }

        // Reserve for colors
        if (!_colorTable.empty()) {
            uint32_t colorSize = static_cast<uint32_t>(_colorTable.size() * sizeof(uint32_t));
            _cardMgr->bufferManager()->reserve(colorSize);
        }

        // Reserve for data buffer
        if (!_data.empty()) {
            uint32_t dataSize = static_cast<uint32_t>(_data.size() * sizeof(float));
            _cardMgr->bufferManager()->reserve(dataSize);
        }
    }

    Result<void> allocateBuffers() override {
        // Allocate bytecode buffer
        if (!_bytecode.empty()) {
            uint32_t bytecodeSize = static_cast<uint32_t>(_bytecode.size() * sizeof(uint32_t));
            auto bufResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "bytecode", bytecodeSize);
            if (!bufResult) {
                return Err<void>("YPlotCard::allocateBuffers: failed to allocate bytecode");
            }
            _bytecodeHandle = *bufResult;
            std::memcpy(_bytecodeHandle.data, _bytecode.data(), bytecodeSize);
            _cardMgr->bufferManager()->markBufferDirty(_bytecodeHandle);
        }

        // Allocate color table
        if (!_colorTable.empty()) {
            uint32_t colorSize = static_cast<uint32_t>(_colorTable.size() * sizeof(uint32_t));
            auto bufResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "colors", colorSize);
            if (!bufResult) {
                return Err<void>("YPlotCard::allocateBuffers: failed to allocate colors");
            }
            _colorHandle = *bufResult;
            std::memcpy(_colorHandle.data, _colorTable.data(), colorSize);
            _cardMgr->bufferManager()->markBufferDirty(_colorHandle);
        }

        // Allocate data buffer (for buffer mode)
        if (!_data.empty()) {
            uint32_t dataSize = static_cast<uint32_t>(_data.size() * sizeof(float));
            auto bufResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "data", dataSize);
            if (!bufResult) {
                return Err<void>("YPlotCard::allocateBuffers: failed to allocate data buffer");
            }
            _dataHandle = *bufResult;
            std::memcpy(_dataHandle.data, _data.data(), dataSize);
            _cardMgr->bufferManager()->markBufferDirty(_dataHandle);
            ydebug("YPlotCard::allocateBuffers: allocated data buffer at offset {}, {} floats",
                  _dataHandle.offset, _data.size());
        }

        _metadataDirty = true;
        return Ok();
    }

    Result<void> finalize() override {
        if (_metadataDirty) {
            uploadMetadata();
            _metadataDirty = false;
        }

        return Ok();
    }

    Result<void> dispose() override {
        // Deregister from events
        if (auto res = deregisterFromEvents(); !res) {
            ywarn("YPlotCard::dispose: failed to deregister from events");
        }

        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();
        _dataHandle = BufferHandle::invalid();

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        return Ok();
    }

    void suspend() override {
        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();
        _dataHandle = BufferHandle::invalid();
    }

    //=========================================================================
    // Update API
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
        ydebug("YPlotCard::update: args='{}' payload_len={}", args, payload.size());

        if (!args.empty()) {
            if (auto res = parseArgs(args); !res) {
                return res;
            }
        }

        // Recompile expressions
        if (_state && _state->functionCount() > 0) {
            if (auto res = compileExpressions(); !res) {
                return res;
            }
        }

        _metadataDirty = true;
        return Ok();
    }

    //=========================================================================
    // YPlot-specific API
    //=========================================================================

    Result<void> setExpressions(const std::string& multiExpr) override {
        if (!_state) {
            return Err<void>("YPlotCard: no state");
        }

        if (auto res = _state->parseExpressions(multiExpr); !res) {
            return res;
        }

        return compileExpressions();
    }

    const yplot::YPlotState& state() const override {
        // _state is always valid after constructor - return it directly
        // (create() is called in constructor and sets _state)
        return *_state;
    }

    //=========================================================================
    // EventListener interface
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        // Handle SetFocus events
        if (event.type == base::Event::Type::SetFocus) {
            if (event.setFocus.objectId == id()) {
                if (!_focused) {
                    _focused = true;
                    ydebug("YPlotCard::onEvent: focused (id={})", id());
                }
                return Ok(true);
            } else if (_focused) {
                _focused = false;
                ydebug("YPlotCard::onEvent: unfocused (id={})", id());
            }
            return Ok(false);
        }

        // Handle scroll events (only when focused)
        if (_focused && event.type == base::Event::Type::Scroll) {
            if (event.scroll.mods & GLFW_MOD_CONTROL) {
                // Ctrl+Scroll: zoom
                float zoomDelta = event.scroll.dy * 0.1f;
                float newZoom = std::clamp(_zoom + zoomDelta, 0.1f, 10.0f);
                if (newZoom != _zoom) {
                    _zoom = newZoom;
                    _state->display().zoom = _zoom;
                    _metadataDirty = true;
                    ydebug("YPlotCard::onEvent: zoom={:.2f}", _zoom);
                }
                return Ok(true);
            } else if (event.scroll.mods & GLFW_MOD_SHIFT) {
                // Shift+Scroll: pan Y
                float dy = event.scroll.dy * 0.05f / _zoom;
                _centerY = std::clamp(_centerY + dy, 0.0f, 1.0f);
                _metadataDirty = true;
                return Ok(true);
            } else {
                // Plain scroll: pan X
                float dx = event.scroll.dy * 0.05f / _zoom;
                _centerX = std::clamp(_centerX + dx, 0.0f, 1.0f);
                _state->display().centerX = _centerX;
                _metadataDirty = true;
                return Ok(true);
            }
        }

        return Ok(false);
    }

private:
    //=========================================================================
    // Event registration
    //=========================================================================

    Result<void> registerForEvents() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("YPlotCard::registerForEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;
        auto self = sharedAs<base::EventListener>();

        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
            return Err<void>("YPlotCard::registerForEvents: failed to register SetFocus", res);
        }
        if (auto res = loop->registerListener(base::Event::Type::Scroll, self, 1000); !res) {
            return Err<void>("YPlotCard::registerForEvents: failed to register Scroll", res);
        }

        ydebug("YPlotCard {} registered for events (priority 1000)", id());
        return Ok();
    }

    Result<void> deregisterFromEvents() {
        if (weak_from_this().expired()) {
            return Ok();
        }

        auto loopResult = base::EventLoop::instance();
        if (!loopResult) {
            return Err<void>("YPlotCard::deregisterFromEvents: no EventLoop instance", loopResult);
        }
        auto loop = *loopResult;

        if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
            return Err<void>("YPlotCard::deregisterFromEvents: failed to deregister", res);
        }
        return Ok();
    }

    //=========================================================================
    // Argument parsing
    //=========================================================================

    Result<void> parseArgs(const std::string& args) {
        if (!_state) {
            return Err<void>("YPlotCard: no state");
        }

        // Look for braced expression format
        size_t braceStart = args.find('{');
        size_t braceEnd = args.rfind('}');

        if (braceStart != std::string::npos && braceEnd != std::string::npos && braceEnd > braceStart) {
            std::string content = args.substr(braceStart + 1, braceEnd - braceStart - 1);
            return parseBracedArgs(content);
        }

        // Simple expression
        if (!args.empty()) {
            return _state->parseExpressions(args);
        }

        return Ok();
    }

    Result<void> parseBracedArgs(const std::string& content) {
        std::string exprPart;
        std::string attrPart;

        size_t atPos = content.find(" @");
        if (atPos != std::string::npos) {
            exprPart = content.substr(0, atPos);
            attrPart = content.substr(atPos + 1);
        } else {
            exprPart = content;
        }

        if (!exprPart.empty()) {
            if (auto res = _state->parseExpressions(exprPart); !res) {
                return res;
            }
        }

        parseAttributes(attrPart);
        return Ok();
    }

    void parseAttributes(const std::string& attrs) {
        if (attrs.empty()) return;

        std::istringstream iss(attrs);
        std::string token;

        while (std::getline(iss, token, '@')) {
            size_t start = token.find_first_not_of(" \t");
            if (start == std::string::npos) continue;
            size_t end = token.find_last_not_of(" \t");
            token = token.substr(start, end - start + 1);

            size_t eqPos = token.find('=');
            if (eqPos == std::string::npos) continue;

            std::string key = token.substr(0, eqPos);
            std::string value = token.substr(eqPos + 1);

            if (key == "range") {
                parseRange(value);
            } else if (key == "grid") {
                _state->axis().showGrid = (value == "on" || value == "true" || value == "1");
            } else if (key == "axes") {
                _state->axis().showAxes = (value == "on" || value == "true" || value == "1");
            }
        }
    }

    void parseRange(const std::string& rangeStr) {
        size_t commaPos = rangeStr.find(',');
        std::string xRange, yRange;

        if (commaPos != std::string::npos) {
            xRange = rangeStr.substr(0, commaPos);
            yRange = rangeStr.substr(commaPos + 1);
        } else {
            xRange = rangeStr;
        }

        size_t dotdot = xRange.find("..");
        if (dotdot != std::string::npos) {
            _state->range().xMin = parseNumber(xRange.substr(0, dotdot));
            _state->range().xMax = parseNumber(xRange.substr(dotdot + 2));
        }

        if (!yRange.empty()) {
            dotdot = yRange.find("..");
            if (dotdot != std::string::npos) {
                _state->range().yMin = parseNumber(yRange.substr(0, dotdot));
                _state->range().yMax = parseNumber(yRange.substr(dotdot + 2));
            }
        }
    }

    float parseNumber(const std::string& str) {
        std::string s = str;
        float sign = 1.0f;
        if (!s.empty() && s[0] == '-') {
            sign = -1.0f;
            s = s.substr(1);
        }

        if (s == "pi" || s == "PI") return sign * 3.14159265f;
        if (s == "e" || s == "E") return sign * 2.71828182f;
        if (s == "tau" || s == "TAU") return sign * 6.28318530f;

        try {
            return std::stof(str);
        } catch (...) {
            return 0.0f;
        }
    }

    //=========================================================================
    // Expression compilation
    //=========================================================================

    Result<void> compileExpressions() {
        if (!_state || _state->functionCount() == 0) {
            _bytecode.clear();
            return Ok();
        }

        auto compileResult = _state->compile();
        if (!compileResult) {
            return Err<void>("YPlotCard: failed to compile", compileResult);
        }

        _bytecode = compileResult->serialize();
        updateColorTable();
        _metadataDirty = true;

        ydebug("YPlotCard: compiled {} functions, bytecode size={}",
              _state->functionCount(), _bytecode.size());

        return Ok();
    }

    void updateColorTable() {
        _colorTable.clear();
        if (!_state) return;

        _colorTable.resize(8, 0xFFFFFFFF);
        for (uint32_t i = 0; i < _state->functionCount() && i < 8; ++i) {
            _colorTable[i] = _state->functions()[i].color;
        }
    }

    //=========================================================================
    // Buffer mode support
    //=========================================================================

    Result<void> parsePayload(const std::string& payload) {
        ydebug("YPlotCard::parsePayload: payload length={}", payload.size());

        // Parse comma or space separated float values
        std::vector<float> values;
        std::istringstream iss(payload);
        std::string token;

        // Split by comma
        while (std::getline(iss, token, ',')) {
            // Trim whitespace
            size_t start = token.find_first_not_of(" \t\n\r");
            size_t end = token.find_last_not_of(" \t\n\r");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
                try {
                    values.push_back(std::stof(token));
                } catch (const std::exception& e) {
                    ywarn("YPlotCard::parsePayload: invalid float '{}'", token);
                }
            }
        }

        ydebug("YPlotCard::parsePayload: parsed {} values", values.size());

        if (values.empty()) {
            return Err<void>("No valid data points in payload");
        }

        _data = std::move(values);

        // Calculate range if auto
        if (_autoRange) {
            calculateRange();
        }

        _metadataDirty = true;
        return Ok();
    }

    void calculateRange() {
        if (_data.empty()) {
            return;
        }

        float minVal = *std::min_element(_data.begin(), _data.end());
        float maxVal = *std::max_element(_data.begin(), _data.end());

        // Add 5% padding
        float range = maxVal - minVal;
        if (range < 0.0001f) {
            range = 1.0f;
        }
        minVal -= range * 0.05f;
        maxVal += range * 0.05f;

        // Update state range
        if (_state) {
            _state->range().yMin = minVal;
            _state->range().yMax = maxVal;
            // X range is 0..dataCount-1 for buffer mode
            _state->range().xMin = 0.0f;
            _state->range().xMax = static_cast<float>(_data.size() - 1);
        }

        ydebug("YPlotCard::calculateRange: y=[{}, {}] x=[0, {}]",
              minVal, maxVal, _data.size() - 1);
    }

    //=========================================================================
    // Metadata upload
    //=========================================================================

    void uploadMetadata() {
        if (!_metaHandle.isValid() || !_state) return;

        YPlotMetadata meta = {};

        // Flags
        meta.flags = 0;
        if (_state->axis().showGrid) meta.flags |= FLAG_GRID;
        if (_state->axis().showAxes) meta.flags |= FLAG_AXES;
        if (_state->axis().showLabels) meta.flags |= FLAG_LABELS;
        if (!_data.empty()) meta.flags |= FLAG_BUFFER;  // Buffer mode flag
        meta.funcCount = static_cast<uint8_t>(_state->functionCount());

        // Glyph indices for axis labels
        meta.glyphBase0 = static_cast<uint8_t>(_glyphBase0);
        meta.glyphDot = static_cast<uint8_t>(_glyphDot);

        // Size
        meta.widthCells = static_cast<uint16_t>(_widthCells);
        meta.heightCells = static_cast<uint16_t>(_heightCells);

        // Bytecode (for expression mode)
        if (_bytecodeHandle.isValid()) {
            meta.bytecodeOffset = _bytecodeHandle.offset / sizeof(uint32_t);
            meta.bytecodeSize = static_cast<uint32_t>(_bytecode.size());
        }

        // Range
        meta.xMin = _state->range().xMin;
        meta.xMax = _state->range().xMax;
        meta.yMin = _state->range().yMin;
        meta.yMax = _state->range().yMax;

        // Data buffer (for buffer mode)
        if (_dataHandle.isValid()) {
            meta.dataOffset = _dataHandle.offset / sizeof(float);
            meta.dataCount = static_cast<uint32_t>(_data.size());
        }

        // Plot area margins (normalized)
        meta.marginLeft = 0.08f;
        meta.marginBottom = 0.08f;

        // View/pan
        meta.zoom = _zoom;
        meta.centerX = _centerX;
        meta.centerY = _centerY;

        // Colors and glyphMinus packed into slot 15
        uint32_t colorOffset = 0;
        if (_colorHandle.isValid()) {
            colorOffset = _colorHandle.offset / sizeof(uint32_t);
        }
        // Pack: lower 24 bits = colorTableOffset, upper 8 bits = glyphMinus
        meta.colorTableOffsetAndGlyph = (colorOffset & 0xFFFFFF) | ((_glyphMinus & 0xFF) << 24);

        // Debug dump metadata
        ydebug("YPlotCard::uploadMetadata: flags={} funcCount={} widthCells={} heightCells={}",
               meta.flags, meta.funcCount, meta.widthCells, meta.heightCells);
        ydebug("YPlotCard::uploadMetadata: range=[{},{},{},{}] bytecode=[off={} size={}] data=[off={} cnt={}]",
               meta.xMin, meta.xMax, meta.yMin, meta.yMax, meta.bytecodeOffset, meta.bytecodeSize,
               meta.dataOffset, meta.dataCount);

        _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
    }

    //=========================================================================
    // Member variables
    //=========================================================================

    FontManager::Ptr _fontMgr;
    GpuAllocator::Ptr _allocator;
    std::string _argsStr;
    std::string _payloadStr;

    yplot::YPlotState::Ptr _state;

    // yfsvm bytecode
    std::vector<uint32_t> _bytecode;
    BufferHandle _bytecodeHandle = BufferHandle::invalid();

    // Color table
    std::vector<uint32_t> _colorTable;
    BufferHandle _colorHandle = BufferHandle::invalid();

    // Data buffer (for buffer mode plotting)
    std::vector<float> _data;
    BufferHandle _dataHandle = BufferHandle::invalid();
    bool _autoRange = true;

    // State
    bool _metadataDirty = true;
    bool _focused = false;

    // View/pan state
    float _zoom = 1.0f;
    float _centerX = 0.5f;
    float _centerY = 0.5f;

    // Glyph indices for axis labels
    uint32_t _glyphBase0 = 0;
    uint32_t _glyphDot = 0;
    uint32_t _glyphMinus = 0;
};

//=============================================================================
// Factory methods
//=============================================================================

Result<CardPtr> YPlotCard::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    auto impl = std::make_shared<YPlotCardImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = impl->init(); !res) {
        return Err<CardPtr>("YPlotCard::create failed", res);
    }
    return Ok(CardPtr(impl));
}

Result<YPlotCard::Ptr> YPlotCard::createImpl(
    ContextType& /*factoryCtx*/,
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    auto impl = std::make_shared<YPlotCardImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = impl->init(); !res) {
        return Err<YPlotCard::Ptr>("YPlotCard::createImpl failed", res);
    }
    return Ok(Ptr(impl));
}

} // namespace yetty::card
