#include "ytext.h"
#include <yetty/yecho/yecho-parser.h>
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

    // Parse arguments (may set content if braced notation used)
    parseArgs(_argsStr);

    // Parse payload only if content wasn't already set by braced notation
    if (_glyphs.empty()) {
        if (auto res = parsePayload(_payloadStr); !res) {
            return Err<void>("YText::init: payload parse failed", res);
        }
    }

    yinfo("YText::init: {} lines, {} glyphs, content={}x{}, scroll=({}, {})",
          _lines.size(), _glyphs.size(), _contentWidth, _contentHeight,
          _scrollSpeedX, _scrollSpeedY);

    return Ok();
}

//=============================================================================
// Argument parsing
//=============================================================================

bool YText::parseBracedArgs(const std::string& args) {
    // Parse braced notation: {statements: content}
    // Variables prefixed with @, statements separated with ;
    // Content after : is the text

    YEchoParser parser;
    auto spans = parser.parse(args);

    if (spans.empty()) {
        return false;
    }

    // We expect a single Block span
    const YEchoSpan* block = nullptr;
    for (const auto& span : spans) {
        if (span.type == YEchoSpan::Type::Block) {
            block = &span;
            break;
        }
    }

    if (!block) {
        return false;
    }

    // Process attributes (@ prefixed variables)
    for (const auto& attr : block->attrs) {
        const std::string& key = attr.key;
        const std::string& value = attr.value;

        if (key == "@scroll-x" || key == "@sx") {
            try { _scrollSpeedX = std::stof(value); } catch (...) {}
            yinfo("YText: scrollSpeedX={}", _scrollSpeedX);
        }
        else if (key == "@scroll-y" || key == "@sy") {
            try { _scrollSpeedY = std::stof(value); } catch (...) {}
            yinfo("YText: scrollSpeedY={}", _scrollSpeedY);
        }
        else if (key == "@loop") {
            _scrollMode = YTextScrollMode::Loop;
            yinfo("YText: scrollMode=loop");
        }
        else if (key == "@pingpong") {
            _scrollMode = YTextScrollMode::PingPong;
            yinfo("YText: scrollMode=pingpong");
        }
        else if (key == "@font-size" || key == "@fs") {
            try { _baseFontSize = std::stof(value); } catch (...) {}
            yinfo("YText: fontSize={}", _baseFontSize);
        }
        else if (key == "@font-family") {
            // TODO: font family support
            yinfo("YText: fontFamily={}", value);
        }
        else if (key == "@effect") {
            if (value == "cylinder" || value == "cylinder-h") {
                _effectMode = YTextEffectMode::CylinderH;
            } else if (value == "cylinder-v") {
                _effectMode = YTextEffectMode::CylinderV;
            } else if (value == "sphere") {
                _effectMode = YTextEffectMode::Sphere;
            } else if (value == "wave-disp" || value == "wave-disp-h") {
                _effectMode = YTextEffectMode::WaveDispH;
            } else if (value == "wave-disp-v") {
                _effectMode = YTextEffectMode::WaveDispV;
            } else if (value == "wave-proj" || value == "wave-proj-h") {
                _effectMode = YTextEffectMode::WaveProjH;
            } else if (value == "wave-proj-v") {
                _effectMode = YTextEffectMode::WaveProjV;
            } else if (value == "ripple") {
                _effectMode = YTextEffectMode::Ripple;
            } else if (value == "perspective" || value == "starwars") {
                _effectMode = YTextEffectMode::Perspective;
            }
            yinfo("YText: effect={}", value);
        }
        else if (key == "@effect-strength" || key == "@es") {
            try { _effectStrength = std::stof(value); } catch (...) {}
            yinfo("YText: effectStrength={}", _effectStrength);
        }
        else if (key == "@frequency" || key == "@freq") {
            try { _frequency = std::stof(value); } catch (...) {}
            yinfo("YText: frequency={}", _frequency);
        }
        else if (key == "@tilt-x" || key == "@tx") {
            try { _tiltX = std::stof(value); } catch (...) {}
            yinfo("YText: tiltX={}", _tiltX);
        }
        else if (key == "@tilt-y" || key == "@ty") {
            try { _tiltY = std::stof(value); } catch (...) {}
            yinfo("YText: tiltY={}", _tiltY);
        }
        else if (key == "@fg-color" || key == "@fg" || key == "@color") {
            _fgColor = parseColor(value);
            yinfo("YText: fgColor=0x{:08X}", _fgColor);
        }
        else if (key == "@bg-color" || key == "@bg") {
            _bgColor = parseColor(value);
            yinfo("YText: bgColor=0x{:08X}", _bgColor);
        }
    }

    // The content after ':' is the text - unescape \n sequences
    if (!block->content.empty()) {
        std::string text;
        text.reserve(block->content.size());
        for (size_t i = 0; i < block->content.size(); ++i) {
            if (block->content[i] == '\\' && i + 1 < block->content.size()) {
                char next = block->content[i + 1];
                if (next == 'n') {
                    text += '\n';
                    ++i;
                    continue;
                } else if (next == 't') {
                    text += '\t';
                    ++i;
                    continue;
                } else if (next == '\\') {
                    text += '\\';
                    ++i;
                    continue;
                }
            }
            text += block->content[i];
        }
        setText(text, _baseFontSize, _fgColor ? _fgColor : 0xFFFFFFFF);
        yinfo("YText: content from braced notation: '{}'", text);
    }

    return true;
}

void YText::parseArgs(const std::string& args) {
    // Check for braced notation: {statements: content}
    size_t firstNonSpace = args.find_first_not_of(" \t\n\r");
    if (firstNonSpace != std::string::npos && args[firstNonSpace] == '{') {
        if (parseBracedArgs(args)) {
            return;  // Successfully parsed braced notation
        }
    }

    // Legacy command-line style parsing
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
        } else if (token == "--perspective" || token == "--starwars") {
            _effectMode = YTextEffectMode::Perspective;
            yinfo("YText: effect=perspective");
        } else if (token == "--effect-strength" || token == "-es") {
            if (iss >> _effectStrength) {
                yinfo("YText: effectStrength={}", _effectStrength);
            }
        } else if (token == "--frequency" || token == "-freq") {
            if (iss >> _frequency) {
                yinfo("YText: frequency={}", _frequency);
            }
        } else if (token == "--tilt-x" || token == "-tx") {
            if (iss >> _tiltX) {
                yinfo("YText: tiltX={}", _tiltX);
            }
        } else if (token == "--tilt-y" || token == "-ty") {
            if (iss >> _tiltY) {
                yinfo("YText: tiltY={}", _tiltY);
            }
        } else if (token == "--fg-color" || token == "-fg") {
            std::string colorStr;
            if (iss >> colorStr) {
                _fgColor = parseColor(colorStr);
                yinfo("YText: fgColor=0x{:08X}", _fgColor);
            }
        } else if (token == "--bg-color" || token == "-bg") {
            std::string colorStr;
            if (iss >> colorStr) {
                _bgColor = parseColor(colorStr);
                yinfo("YText: bgColor=0x{:08X}", _bgColor);
            }
        }
    }
}

uint32_t YText::parseColor(const std::string& str) {
    // Handle formats: "#RRGGBB", "0xRRGGBB", "RRGGBB"
    std::string hex = str;
    if (hex.empty()) return 0;

    if (hex[0] == '#') {
        hex = hex.substr(1);
    } else if (hex.size() > 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex = hex.substr(2);
    }

    try {
        uint32_t rgb = std::stoul(hex, nullptr, 16);
        // Convert RGB to RGBA (full alpha)
        return (rgb << 8) | 0xFF;
    } catch (...) {
        return 0;  // Use default on parse error
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

    float fontBaseSize = _font->atlas()->getFontSize();
    float scale = fontSize / fontBaseSize;

    uint32_t glyphIndex = _font->getGlyphIndex(codepoint);
    const auto& metadata = _font->atlas()->getGlyphMetadata();
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
    auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "data", totalBytes);
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

    // Word 0: lineOffset(16) | lineCount(16)
    uint32_t lineCount = static_cast<uint32_t>(_lines.size());
    meta.lineOffsetAndCount = (_lineOffset & 0xFFFF) | ((lineCount & 0xFFFF) << 16);

    // Word 1: glyphOffset(16) | glyphCount(16)
    uint32_t glyphCount = static_cast<uint32_t>(_glyphs.size());
    meta.glyphOffsetAndCount = (_glyphOffset & 0xFFFF) | ((glyphCount & 0xFFFF) << 16);

    // Words 2-5: floats
    meta.contentWidth = _contentWidth;
    meta.contentHeight = _contentHeight;
    meta.scrollSpeedX = _scrollSpeedX;
    meta.scrollSpeedY = _scrollSpeedY;

    // Word 6: widthCells(12) | heightCells(12) | scrollMode(2) | effectMode(4) | reserved(2)
    meta.packedCellsAndModes = (_widthCells & 0xFFF)
                             | ((_heightCells & 0xFFF) << 12)
                             | ((static_cast<uint32_t>(_scrollMode) & 0x3) << 24)
                             | ((static_cast<uint32_t>(_effectMode) & 0xF) << 26);

    // Words 7-8: effect parameters
    meta.effectStrength = _effectStrength;
    meta.frequency = _frequency;

    // Words 9-10: colors (0 = use terminal default)
    meta.bgColor = _bgColor;
    meta.fgColor = _fgColor;

    // Words 11-12: tilt
    meta.tiltX = _tiltX;
    meta.tiltY = _tiltY;

    // Words 13-15: reserved
    meta.reserved[0] = 0;
    meta.reserved[1] = 0;
    meta.reserved[2] = 0;

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
        _cardMgr->bufferManager()->deallocateBuffer(metadataSlotIndex(), "data");
        _dataStorage = StorageHandle::invalid();
    }
    _dirty = true;
}

Result<void> YText::dispose() {
    if (_dataStorage.isValid() && _cardMgr) {
        _cardMgr->bufferManager()->deallocateBuffer(metadataSlotIndex(), "data");
        _dataStorage = StorageHandle::invalid();
    }

    if (_metaHandle.isValid() && _cardMgr) {
        _cardMgr->deallocateMetadata(_metaHandle);
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

} // namespace yetty::card
