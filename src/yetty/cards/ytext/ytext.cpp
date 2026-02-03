#include "ytext.h"
#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <sstream>

namespace yetty::card {

//=============================================================================
// Factory (ObjectFactory provides create() automatically)
//=============================================================================

Result<YText::Ptr> YText::createImpl(
    ContextType& ctx,
    const YettyContext& yettyCtx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    (void)ctx;  // ObjectFactory context marker

    if (!yettyCtx.cardManager) {
        return Err<Ptr>("YText::createImpl: null CardManager");
    }

    auto card = std::make_shared<YText>(yettyCtx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<Ptr>("YText::createImpl: init failed", res);
    }
    return Ok<Ptr>(card);
}

//=============================================================================
// Constructor
//=============================================================================

YText::YText(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
    , _fontManager(ctx.fontManager)
    , _argsStr(args)
    , _payloadStr(payload)
{
    _shaderGlyph = SHADER_GLYPH;
    if (_fontManager) {
        _font = _fontManager->getDefaultMsMsdfFont();
    }
}

Result<void> YText::init() {
    // Allocate metadata slot
    auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
    if (!metaResult) {
        return Err<void>("YText::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;

    // Parse arguments
    parseArgs(_argsStr);

    // Parse payload
    if (auto res = parsePayload(_payloadStr); !res) {
        return Err<void>("YText::init: payload parse failed", res);
    }

    yinfo("YText::init: {} lines, {} glyphs, content={}x{}, scroll=({}, {})",
          _lines.size(), _glyphs.size(), _contentWidth, _contentHeight,
          _scrollSpeedX, _scrollSpeedY);

    return Ok();
}

//=============================================================================
// Argument parsing
//=============================================================================

void YText::parseArgs(const std::string& args) {
    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--scroll-x" || token == "-sx") {
            if (iss >> _scrollSpeedX) {
                yinfo("YText: scrollSpeedX={}", _scrollSpeedX);
            }
        } else if (token == "--scroll-y" || token == "-sy") {
            if (iss >> _scrollSpeedY) {
                yinfo("YText: scrollSpeedY={}", _scrollSpeedY);
            }
        } else if (token == "--loop") {
            _scrollMode = YTextScrollMode::Loop;
        } else if (token == "--pingpong") {
            _scrollMode = YTextScrollMode::PingPong;
        } else if (token == "--font-size" || token == "-fs") {
            iss >> _baseFontSize;
        } else if (token == "--cylinder" || token == "--cylinder-h") {
            _effectMode = YTextEffectMode::CylinderH;
            yinfo("YText: effect=cylinderH");
        } else if (token == "--cylinder-v") {
            _effectMode = YTextEffectMode::CylinderV;
            yinfo("YText: effect=cylinderV");
        } else if (token == "--sphere") {
            _effectMode = YTextEffectMode::Sphere;
            yinfo("YText: effect=sphere");
        } else if (token == "--wave-disp" || token == "--wave-disp-h") {
            _effectMode = YTextEffectMode::WaveDispH;
            yinfo("YText: effect=waveDispH");
        } else if (token == "--wave-disp-v") {
            _effectMode = YTextEffectMode::WaveDispV;
            yinfo("YText: effect=waveDispV");
        } else if (token == "--wave-proj" || token == "--wave-proj-h") {
            _effectMode = YTextEffectMode::WaveProjH;
            yinfo("YText: effect=waveProjH");
        } else if (token == "--wave-proj-v") {
            _effectMode = YTextEffectMode::WaveProjV;
            yinfo("YText: effect=waveProjV");
        } else if (token == "--ripple") {
            _effectMode = YTextEffectMode::Ripple;
            yinfo("YText: effect=ripple");
        } else if (token == "--effect-strength" || token == "-es") {
            if (iss >> _effectStrength) {
                yinfo("YText: effectStrength={}", _effectStrength);
            }
        } else if (token == "--tilt-x" || token == "-tx") {
            if (iss >> _tiltX) {
                yinfo("YText: tiltX={}", _tiltX);
            }
        } else if (token == "--tilt-y" || token == "-ty") {
            if (iss >> _tiltY) {
                yinfo("YText: tiltY={}", _tiltY);
            }
        }
    }
}

Result<void> YText::parsePayload(const std::string& payload) {
    if (payload.empty()) {
        // Demo text
        setText("Hello, YText!\nThis is GPU-animated scrolling text.\nNo CPU rebuild needed!",
                _baseFontSize, 0xFFFFFFFF);
        return Ok();
    }

    // Simple text payload - interpret as plain text
    setText(payload, _baseFontSize, 0xFFFFFFFF);
    return Ok();
}

//=============================================================================
// Text building API
//=============================================================================

void YText::beginLine() {
    _curLineX = 0.0f;
    _curLineMaxAscender = 0.0f;
    _curLineMaxDescender = 0.0f;
    _curLineStartGlyph = static_cast<uint32_t>(_glyphs.size());
}

void YText::addGlyph(uint32_t codepoint, float fontSize, uint32_t color) {
    if (!_font) return;

    float fontBaseSize = _font->getFontSize();
    float scale = fontSize / fontBaseSize;

    uint32_t glyphIndex = _font->getGlyphIndex(codepoint);
    const auto& metadata = _font->getGlyphMetadata();
    if (glyphIndex >= metadata.size()) {
        // Unknown glyph - use space width
        _curLineX += fontSize * 0.5f;
        return;
    }

    const auto& glyph = metadata[glyphIndex];

    // Track line metrics
    float ascender = glyph._bearingY * scale;
    float descender = (glyph._sizeY - glyph._bearingY) * scale;
    _curLineMaxAscender = std::max(_curLineMaxAscender, ascender);
    _curLineMaxDescender = std::max(_curLineMaxDescender, descender);

    // Pack color (24-bit) and scale (8-bit) into colorAndScale
    // Scale is encoded as: scaleIndex = clamp(scale * 16, 0, 255)
    uint8_t scaleIndex = static_cast<uint8_t>(std::clamp(scale * 16.0f, 0.0f, 255.0f));
    uint32_t colorAndScale = (color & 0xFFFFFF00) | scaleIndex;

    YTextGlyph g;
    g.xOffset = _curLineX + glyph._bearingX * scale;
    g.width = glyph._sizeX * scale;
    g.glyphIndex = glyphIndex;
    g.colorAndScale = colorAndScale;

    _glyphs.push_back(g);

    _curLineX += glyph._advance * scale;
    _contentWidth = std::max(_contentWidth, _curLineX);
}

void YText::addText(const std::string& text, float fontSize, uint32_t color) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
    const uint8_t* end = ptr + text.size();

    while (ptr < end) {
        // Decode UTF-8
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

        if (codepoint == '\n') {
            endLine();
            beginLine();
        } else if (codepoint >= 32) {
            addGlyph(codepoint, fontSize, color);
        }
    }
}

void YText::endLine() {
    uint32_t glyphCount = static_cast<uint32_t>(_glyphs.size()) - _curLineStartGlyph;

    // Ensure minimum line height
    float lineHeight = _curLineMaxAscender + _curLineMaxDescender;
    if (lineHeight < _baseFontSize * 0.5f) {
        lineHeight = _baseFontSize * 1.2f;
    }

    YTextLine line;
    line.startGlyph = _curLineStartGlyph;
    line.glyphCount = glyphCount;
    line.yOffset = _curLineY;
    line.lineHeight = lineHeight;

    _lines.push_back(line);

    _curLineY += lineHeight;
    _contentHeight = _curLineY;
}

void YText::setText(const std::string& text, float fontSize, uint32_t color) {
    _lines.clear();
    _glyphs.clear();
    _contentWidth = 0.0f;
    _contentHeight = 0.0f;
    _curLineY = 0.0f;

    beginLine();
    addText(text, fontSize, color);
    endLine();

    _dirty = true;
    _metadataDirty = true;
}

//=============================================================================
// Card lifecycle
//=============================================================================

void YText::declareBufferNeeds() {
    uint32_t lineBytes = static_cast<uint32_t>(_lines.size()) * sizeof(YTextLine);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size()) * sizeof(YTextGlyph);
    uint32_t totalBytes = lineBytes + glyphBytes;

    if (totalBytes > 0) {
        _cardMgr->bufferManager()->reserve(totalBytes);
    }
}

Result<void> YText::allocateBuffers() {
    if (_lines.empty()) {
        return Ok();
    }

    uint32_t lineBytes = static_cast<uint32_t>(_lines.size()) * sizeof(YTextLine);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size()) * sizeof(YTextGlyph);
    uint32_t totalBytes = lineBytes + glyphBytes;

    if (totalBytes == 0) {
        return Ok();
    }

    // Allocate storage
    auto storageResult = _cardMgr->bufferManager()->allocateBuffer(totalBytes);
    if (!storageResult) {
        return Err<void>("YText::allocateBuffers: failed to allocate storage");
    }
    _dataStorage = *storageResult;

    // Upload data
    if (auto res = uploadData(); !res) {
        return res;
    }

    _dirty = false;
    _metadataDirty = true;

    yinfo("YText::allocateBuffers: {} bytes ({} lines, {} glyphs)",
          totalBytes, _lines.size(), _glyphs.size());

    return Ok();
}

Result<void> YText::uploadData() {
    if (!_dataStorage.isValid()) {
        return Ok();
    }

    uint8_t* base = _dataStorage.data;
    uint32_t offset = 0;

    // Write lines
    _lineOffset = (_dataStorage.offset + offset) / sizeof(float);
    uint32_t lineBytes = static_cast<uint32_t>(_lines.size()) * sizeof(YTextLine);
    if (!_lines.empty()) {
        std::memcpy(base + offset, _lines.data(), lineBytes);
    }
    offset += lineBytes;

    // Write glyphs
    _glyphOffset = (_dataStorage.offset + offset) / sizeof(float);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size()) * sizeof(YTextGlyph);
    if (!_glyphs.empty()) {
        std::memcpy(base + offset, _glyphs.data(), glyphBytes);
    }

    _cardMgr->bufferManager()->markBufferDirty(_dataStorage);

    return Ok();
}

Result<void> YText::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YText::uploadMetadata: invalid metadata handle");
    }

    Metadata meta = {};
    meta.lineOffset = _lineOffset;
    meta.lineCount = static_cast<uint32_t>(_lines.size());
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
    meta.contentWidth = _contentWidth;
    meta.contentHeight = _contentHeight;
    meta.scrollSpeedX = _scrollSpeedX;
    meta.scrollSpeedY = _scrollSpeedY;
    meta.scrollMode = static_cast<uint32_t>(_scrollMode);
    meta.widthCells = _widthCells;
    meta.heightCells = _heightCells;
    meta.bgColor = _bgColor;
    meta.effectMode = static_cast<uint32_t>(_effectMode);
    meta.effectStrength = _effectStrength;
    meta.tiltX = _tiltX;
    meta.tiltY = _tiltY;

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YText::uploadMetadata: write failed");
    }

    return Ok();
}

Result<void> YText::render(float time) {
    (void)time;

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("YText::render: metadata upload failed", res);
        }
        _metadataDirty = false;
    }

    return Ok();
}

void YText::suspend() {
    if (_dataStorage.isValid()) {
        _cardMgr->bufferManager()->deallocateBuffer(_dataStorage);
        _dataStorage = StorageHandle::invalid();
    }
    _dirty = true;
}

Result<void> YText::dispose() {
    if (_dataStorage.isValid() && _cardMgr) {
        _cardMgr->bufferManager()->deallocateBuffer(_dataStorage);
        _dataStorage = StorageHandle::invalid();
    }

    if (_metaHandle.isValid() && _cardMgr) {
        _cardMgr->deallocateMetadata(_metaHandle);
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

} // namespace yetty::card
