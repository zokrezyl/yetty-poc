// Prevent Windows min/max macros from conflicting with std::min/std::max
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "yplot-card.h"
#include <yetty/yetty-context.h>
#include <yetty/yfsvm/yfsvm.h>
#include <yetty/yfsvm/yfsvm-compiler.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <cmath>
#include <cstring>

namespace yetty::card {

//=============================================================================
// Metadata structure (64 bytes)
//=============================================================================

struct YPlotMetadata {
    // [0]: flags(8) | funcCount(8) | pad(16)
    uint8_t flags;
    uint8_t funcCount;
    uint16_t _pad0;

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

    // [8-11]: plot area (normalized coords)
    float marginLeft;
    float marginBottom;
    float plotWidth;
    float plotHeight;

    // [12-14]: animation/view
    float time;
    float zoom;
    float centerX;

    // [15]: color table
    uint32_t colorTableOffset;
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

        // Create YDrawBuffer for decoration
        auto bufferResult = YDrawBuffer::create();
        if (bufferResult) {
            _drawBuffer = *bufferResult;
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

        yinfo("YPlotCard::init: allocated metadata at offset {}", _metaHandle.offset);

        // Parse args
        if (auto res = parseArgs(_argsStr); !res) {
            ywarn("YPlotCard::init: failed to parse args: {}", error_msg(res));
        }

        // Compile expressions if any
        if (_state && _state->functionCount() > 0) {
            if (auto res = compileExpressions(); !res) {
                return Err<void>("YPlotCard::init: failed to compile expressions", res);
            }
        }

        // Build decoration (axes, grid, labels)
        if (_state && _drawBuffer) {
            _state->buildDecoration(*_drawBuffer, _widthCells, _heightCells);
        }

        // Create YDrawBuilder for the decoration
        if (_drawBuffer && _fontMgr && _allocator) {
            auto builderResult = YDrawBuilder::create(
                _fontMgr, _allocator, _drawBuffer, _cardMgr, metadataSlotIndex());
            if (builderResult) {
                _drawBuilder = *builderResult;
                _drawBuilder->setViewport(_widthCells, _heightCells);
                _drawBuilder->calculate();
            }
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
        if (_drawBuilder && _drawBuilder->needsBufferRealloc()) return true;
        return false;
    }

    void declareBufferNeeds() override {
        // Invalidate handles
        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();

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

        // Let ydraw builder declare its needs
        if (_drawBuilder) {
            _drawBuilder->declareBufferNeeds();
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

        // Let ydraw builder allocate
        if (_drawBuilder) {
            if (auto res = _drawBuilder->allocateBuffers(); !res) {
                return res;
            }
        }

        _metadataDirty = true;
        return Ok();
    }

    Result<void> finalize() override {
        if (_metadataDirty) {
            uploadMetadata();
            _metadataDirty = false;
        }

        // Let ydraw builder write buffers
        if (_drawBuilder) {
            if (auto res = _drawBuilder->writeBuffers(); !res) {
                return res;
            }
        }

        return Ok();
    }

    Result<void> dispose() override {
        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();
        _drawBuilder.reset();

        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }

        return Ok();
    }

    void suspend() override {
        _bytecodeHandle = BufferHandle::invalid();
        _colorHandle = BufferHandle::invalid();
        // YDrawBuilder doesn't have suspend - handles are invalidated when card is suspended
    }

    //=========================================================================
    // Update API
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
        yinfo("YPlotCard::update: args='{}' payload_len={}", args, payload.size());

        if (!args.empty()) {
            if (auto res = parseArgs(args); !res) {
                return res;
            }
        }

        // Recompile and rebuild
        if (_state && _state->functionCount() > 0) {
            if (auto res = compileExpressions(); !res) {
                return res;
            }
        }

        // Rebuild decoration
        if (_state && _drawBuffer) {
            _drawBuffer->clear();
            _state->buildDecoration(*_drawBuffer, _widthCells, _heightCells);
            if (_drawBuilder) {
                _drawBuilder->calculate();
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

private:
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

        yinfo("YPlotCard: compiled {} functions, bytecode size={}",
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
        meta.funcCount = static_cast<uint8_t>(_state->functionCount());

        // Size
        meta.widthCells = static_cast<uint16_t>(_widthCells);
        meta.heightCells = static_cast<uint16_t>(_heightCells);

        // Bytecode
        if (_bytecodeHandle.isValid()) {
            meta.bytecodeOffset = _bytecodeHandle.offset / sizeof(uint32_t);
            meta.bytecodeSize = static_cast<uint32_t>(_bytecode.size());
        }

        // Range
        meta.xMin = _state->range().xMin;
        meta.xMax = _state->range().xMax;
        meta.yMin = _state->range().yMin;
        meta.yMax = _state->range().yMax;

        // Plot area margins (normalized)
        meta.marginLeft = 0.08f;
        meta.marginBottom = 0.08f;
        meta.plotWidth = 1.0f - 0.08f - 0.02f;
        meta.plotHeight = 1.0f - 0.08f - 0.02f;

        // Animation
        meta.time = _currentTime;
        meta.zoom = _state->display().zoom;
        meta.centerX = _state->display().centerX;

        // Colors
        if (_colorHandle.isValid()) {
            meta.colorTableOffset = _colorHandle.offset / sizeof(uint32_t);
        }

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

    // YDraw components for decoration
    YDrawBuffer::Ptr _drawBuffer;
    YDrawBuilder::Ptr _drawBuilder;

    // yfsvm bytecode
    std::vector<uint32_t> _bytecode;
    BufferHandle _bytecodeHandle = BufferHandle::invalid();

    // Color table
    std::vector<uint32_t> _colorTable;
    BufferHandle _colorHandle = BufferHandle::invalid();

    // State
    bool _metadataDirty = true;
    float _currentTime = 0.0f;
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
