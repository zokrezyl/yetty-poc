#include <yetty/ydraw-writer.h>
#include "ydraw-prim-writer.gen.h"
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <unordered_map>

namespace yetty {
namespace {

//=============================================================================
// Binary v2 format structs (must match ydraw.cpp parsing)
//=============================================================================

struct BinHeader {
    uint32_t version = 3;
    uint32_t primCount = 0;
    uint32_t bgColor = 0xFFFFFFFF;
    uint32_t flags = 16; // FLAG_UNIFORM_SCALE
};

struct DefaultTextSpan {
    float x, y, fontSize;
    uint32_t color;
    uint32_t layer;
    uint32_t textOffset;
    uint32_t textLength;
    uint32_t _pad = 0;
};
static_assert(sizeof(DefaultTextSpan) == 32);

struct CustomTextSpan {
    float x, y, fontSize;
    uint32_t color;
    uint32_t fontIndex;
    uint32_t layer;
    uint32_t textOffset;
    uint32_t textLength;
};
static_assert(sizeof(CustomTextSpan) == 32);

struct RotatedTextSpan {
    float x, y, fontSize;
    float rotation;       // radians
    uint32_t color;
    uint32_t fontIndex;
    uint32_t textOffset;
    uint32_t textLength;
};
static_assert(sizeof(RotatedTextSpan) == 32);

// SDFPrimitive and SDF type IDs now come from ydraw-prim-writer.gen.h
// (generated from ydraw-primitives.yaml)

//=============================================================================
// TTF font metrics — parse cmap + hmtx + head for glyph advances
// (ported from tools/pdf2ydraw/main.cpp)
//=============================================================================

struct TtfFontMetrics {
    uint16_t unitsPerEm = 1000;
    int16_t ascender = 800;   // from hhea table (design units)
    int16_t descender = -200; // from hhea table (negative = below baseline)
    std::vector<uint16_t> advanceWidths;
    std::unordered_map<uint32_t, uint16_t> cmapTable;

    float getAdvance(uint32_t codepoint, float fontSize) const {
        uint16_t gid = 0;
        auto it = cmapTable.find(codepoint);
        if (it != cmapTable.end()) gid = it->second;
        uint16_t aw = (gid < advanceWidths.size()) ? advanceWidths[gid]
                     : static_cast<uint16_t>(unitsPerEm / 2);
        return static_cast<float>(aw) / static_cast<float>(unitsPerEm) * fontSize;
    }

    float getAscent(float fontSize) const {
        return static_cast<float>(ascender) / static_cast<float>(unitsPerEm) * fontSize;
    }

    float getDescent(float fontSize) const {
        return static_cast<float>(-descender) / static_cast<float>(unitsPerEm) * fontSize;
    }
};

bool parseTtfMetrics(const uint8_t* data, size_t size, TtfFontMetrics& m) {
    if (size < 12) return false;

    auto r16 = [&](size_t off) -> uint16_t {
        if (off + 2 > size) return 0;
        return (static_cast<uint16_t>(data[off]) << 8) | data[off + 1];
    };
    auto r32 = [&](size_t off) -> uint32_t {
        if (off + 4 > size) return 0;
        return (static_cast<uint32_t>(data[off]) << 24) |
               (static_cast<uint32_t>(data[off+1]) << 16) |
               (static_cast<uint32_t>(data[off+2]) << 8) | data[off+3];
    };

    uint16_t numTables = r16(4);
    uint32_t headOff = 0, hheaOff = 0, cmapOff = 0, hmtxOff = 0, maxpOff = 0;

    for (uint16_t i = 0; i < numTables; i++) {
        size_t entry = 12 + static_cast<size_t>(i) * 16;
        if (entry + 16 > size) break;
        uint32_t tag = r32(entry);
        uint32_t off = r32(entry + 8);
        if (tag == 0x68656164) headOff = off;       // 'head'
        else if (tag == 0x68686561) hheaOff = off;  // 'hhea'
        else if (tag == 0x636D6170) cmapOff = off;  // 'cmap'
        else if (tag == 0x686D7478) hmtxOff = off;  // 'hmtx'
        else if (tag == 0x6D617870) maxpOff = off;  // 'maxp'
    }
    if (!headOff || !hheaOff || !hmtxOff) return false;

    m.unitsPerEm = r16(headOff + 18);
    if (m.unitsPerEm == 0) m.unitsPerEm = 1000;

    // hhea table: ascender at offset 4, descender at offset 6
    m.ascender = static_cast<int16_t>(r16(hheaOff + 4));
    m.descender = static_cast<int16_t>(r16(hheaOff + 6));

    uint16_t numberOfHMetrics = r16(hheaOff + 34);
    uint16_t numGlyphs = maxpOff ? r16(maxpOff + 4) : numberOfHMetrics;

    m.advanceWidths.resize(numGlyphs, 0);
    uint16_t lastAdvance = 0;
    for (uint16_t i = 0; i < numGlyphs; i++) {
        if (i < numberOfHMetrics) {
            lastAdvance = r16(hmtxOff + static_cast<size_t>(i) * 4);
        }
        m.advanceWidths[i] = lastAdvance;
    }

    // Parse cmap
    if (cmapOff && cmapOff + 4 <= size) {
        uint16_t numSubtables = r16(cmapOff + 2);
        uint32_t bestOff = 0;
        int bestPri = -1;

        for (uint16_t i = 0; i < numSubtables; i++) {
            size_t entry = cmapOff + 4 + static_cast<size_t>(i) * 8;
            if (entry + 8 > size) break;
            uint16_t pid = r16(entry);
            uint16_t eid = r16(entry + 2);
            uint32_t off = r32(entry + 4);
            int pri = -1;
            if (pid == 3 && eid == 10) pri = 4;
            else if (pid == 3 && eid == 1) pri = 3;
            else if (pid == 0 && eid == 3) pri = 2;
            else if (pid == 0) pri = 1;
            else if (pid == 1 && eid == 0) pri = 0;
            if (pri > bestPri) { bestPri = pri; bestOff = cmapOff + off; }
        }

        if (bestOff && bestOff + 2 <= size) {
            uint16_t fmt = r16(bestOff);
            if (fmt == 0) {
                if (bestOff + 6 + 256 <= size) {
                    for (uint32_t cp = 0; cp < 256; cp++) {
                        uint8_t gid = data[bestOff + 6 + cp];
                        if (gid != 0) m.cmapTable[cp] = gid;
                    }
                }
            } else if (fmt == 6) {
                uint16_t firstCode  = r16(bestOff + 6);
                uint16_t entryCount = r16(bestOff + 8);
                for (uint16_t i = 0; i < entryCount; i++) {
                    uint16_t gid = r16(bestOff + 10 + static_cast<size_t>(i) * 2);
                    if (gid != 0) m.cmapTable[firstCode + i] = gid;
                }
            } else if (fmt == 4) {
                uint16_t segCount = r16(bestOff + 6) / 2;
                size_t endOff   = bestOff + 14;
                size_t startOff = endOff + static_cast<size_t>(segCount) * 2 + 2;
                size_t deltaOff = startOff + static_cast<size_t>(segCount) * 2;
                size_t rangeOff = deltaOff + static_cast<size_t>(segCount) * 2;

                for (uint16_t seg = 0; seg < segCount; seg++) {
                    uint16_t endCode   = r16(endOff + seg * 2);
                    uint16_t startCode = r16(startOff + seg * 2);
                    int16_t  idDelta   = static_cast<int16_t>(r16(deltaOff + seg * 2));
                    uint16_t idRange   = r16(rangeOff + seg * 2);
                    if (startCode == 0xFFFF) break;

                    for (uint32_t cp = startCode; cp <= endCode; cp++) {
                        uint16_t gid;
                        if (idRange == 0) {
                            gid = static_cast<uint16_t>((cp + idDelta) & 0xFFFF);
                        } else {
                            size_t off = rangeOff + seg * 2 + idRange + (cp - startCode) * 2;
                            if (off + 2 > size) continue;
                            gid = r16(off);
                            if (gid != 0) gid = static_cast<uint16_t>((gid + idDelta) & 0xFFFF);
                        }
                        if (gid != 0) m.cmapTable[cp] = gid;
                    }
                }
            } else if (fmt == 12) {
                uint32_t nGroups = r32(bestOff + 12);
                for (uint32_t g = 0; g < nGroups; g++) {
                    size_t gOff = bestOff + 16 + static_cast<size_t>(g) * 12;
                    if (gOff + 12 > size) break;
                    uint32_t startCode = r32(gOff);
                    uint32_t endCode   = r32(gOff + 4);
                    uint32_t startGid  = r32(gOff + 8);
                    for (uint32_t cp = startCode; cp <= endCode; cp++) {
                        m.cmapTable[cp] = static_cast<uint16_t>(startGid + (cp - startCode));
                    }
                }
            }
        }
    }

    return true;
}

//=============================================================================
// UTF-8 decoding helper
//=============================================================================

uint32_t decodeUtf8(const uint8_t*& ptr, const uint8_t* end) {
    uint32_t cp = 0;
    if ((*ptr & 0x80) == 0) {
        cp = *ptr++;
    } else if ((*ptr & 0xE0) == 0xC0) {
        cp = (*ptr++ & 0x1F) << 6;
        if (ptr < end) cp |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF0) == 0xE0) {
        cp = (*ptr++ & 0x0F) << 12;
        if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
        if (ptr < end) cp |= (*ptr++ & 0x3F);
    } else if ((*ptr & 0xF8) == 0xF0) {
        cp = (*ptr++ & 0x07) << 18;
        if (ptr < end) cp |= (*ptr++ & 0x3F) << 12;
        if (ptr < end) cp |= (*ptr++ & 0x3F) << 6;
        if (ptr < end) cp |= (*ptr++ & 0x3F);
    } else {
        ptr++;
    }
    return cp;
}

//=============================================================================
// Helpers
//=============================================================================

uint32_t align4(uint32_t v) { return (v + 3) & ~3u; }

static const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const uint8_t* data, size_t len) {
    std::string out;
    out.reserve((len + 2) / 3 * 4);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t n = static_cast<uint32_t>(data[i]) << 16;
        if (i + 1 < len) n |= static_cast<uint32_t>(data[i + 1]) << 8;
        if (i + 2 < len) n |= static_cast<uint32_t>(data[i + 2]);
        out += b64chars[(n >> 18) & 0x3F];
        out += b64chars[(n >> 12) & 0x3F];
        out += (i + 1 < len) ? b64chars[(n >> 6) & 0x3F] : '=';
        out += (i + 2 < len) ? b64chars[n & 0x3F] : '=';
    }
    return out;
}

//=============================================================================
// Font data stored per font
//=============================================================================

struct FontEntry {
    std::string name;
    std::vector<uint8_t> data;
    TtfFontMetrics metrics;
};

} // anonymous namespace

//=============================================================================
// YDrawWriterBinaryImpl
//=============================================================================

class YDrawWriterBinaryImpl : public YDrawWriterBinary {
public:
    YDrawWriterBinaryImpl() = default;

    const char* typeName() const override { return "YDrawWriterBinary"; }

    //=========================================================================
    // Font management
    //=========================================================================

    Result<int> addFont(const std::string& ttfPath) override {
        std::ifstream in(ttfPath, std::ios::binary | std::ios::ate);
        if (!in) {
            return Err<int>("YDrawWriterBinary::addFont: cannot open " + ttfPath);
        }
        auto fileSize = in.tellg();
        in.seekg(0);
        std::vector<uint8_t> data(static_cast<size_t>(fileSize));
        in.read(reinterpret_cast<char*>(data.data()),
                static_cast<std::streamsize>(fileSize));

        auto name = std::filesystem::path(ttfPath).stem().string();
        return addFontData(data.data(), data.size(), name);
    }

    Result<int> addFontData(const uint8_t* data, size_t size,
                             const std::string& name) override {
        FontEntry entry;
        entry.name = name;
        entry.data.assign(data, data + size);

        if (!parseTtfMetrics(data, size, entry.metrics)) {
            return Err<int>("YDrawWriterBinary::addFontData: "
                            "failed to parse TTF metrics for " + name);
        }

        int fontId = static_cast<int>(_fonts.size());
        _fonts.push_back(std::move(entry));
        return Ok(fontId);
    }

    //=========================================================================
    // Text
    //=========================================================================

    float addText(float x, float y, const std::string& text,
                  float fontSize, uint32_t color, int fontId) override {
        return addText(x, y, text, fontSize, color, 0u, fontId);
    }

    float addText(float x, float y, const std::string& text,
                  float fontSize, uint32_t color,
                  uint32_t layer, int fontId) override {
        if (text.empty()) return 0.0f;

        uint32_t textOffset = static_cast<uint32_t>(_textBlob.size());
        uint32_t textLength = static_cast<uint32_t>(text.size());
        _textBlob += text;

        // Compute advance
        float advance = computeAdvance(text, fontSize, fontId);

        if (fontId >= 0 && fontId < static_cast<int>(_fonts.size())) {
            CustomTextSpan span;
            span.x = x;
            span.y = y;
            span.fontSize = fontSize;
            span.color = color;
            span.fontIndex = static_cast<uint32_t>(fontId);
            span.layer = layer;
            span.textOffset = textOffset;
            span.textLength = textLength;
            _customSpans.push_back(span);
        } else {
            DefaultTextSpan span;
            span.x = x;
            span.y = y;
            span.fontSize = fontSize;
            span.color = color;
            span.layer = layer;
            span.textOffset = textOffset;
            span.textLength = textLength;
            span._pad = 0;
            _defaultSpans.push_back(span);
        }

        return advance;
    }

    float measureText(const std::string& text,
                      float fontSize, int fontId) override {
        return computeAdvance(text, fontSize, fontId);
    }

    //=========================================================================
    // Font metrics
    //=========================================================================

    float fontAscent(float fontSize, int fontId) override {
        if (fontId >= 0 && fontId < static_cast<int>(_fonts.size())) {
            return _fonts[static_cast<size_t>(fontId)].metrics.getAscent(fontSize);
        }
        return fontSize * 0.8f; // reasonable fallback
    }

    float fontDescent(float fontSize, int fontId) override {
        if (fontId >= 0 && fontId < static_cast<int>(_fonts.size())) {
            return _fonts[static_cast<size_t>(fontId)].metrics.getDescent(fontSize);
        }
        return fontSize * 0.2f; // reasonable fallback
    }

    //=========================================================================
    // Primitives — accumulate for binary output
    //=========================================================================

    void addBox(float cx, float cy, float halfW, float halfH,
                uint32_t fillColor, uint32_t strokeColor,
                float strokeWidth, float round, uint32_t layer) override {
        size_t off = _compactData.size();
        _compactData.resize(off + 10);
        sdf::writeBox(_compactData.data() + off, layer, cx, cy, halfW, halfH,
                      fillColor, strokeColor, strokeWidth, round);
        _primCount++;
    }

    void addSegment(float x0, float y0, float x1, float y1,
                    uint32_t strokeColor, float strokeWidth,
                    uint32_t layer) override {
        size_t off = _compactData.size();
        _compactData.resize(off + 10);
        sdf::writeSegment(_compactData.data() + off, layer, x0, y0, x1, y1,
                          0, strokeColor, strokeWidth, 0);
        _primCount++;
    }

    void addCircle(float cx, float cy, float radius,
                   uint32_t fillColor, uint32_t strokeColor,
                   float strokeWidth, uint32_t layer) override {
        size_t off = _compactData.size();
        _compactData.resize(off + 9);
        sdf::writeCircle(_compactData.data() + off, layer, cx, cy, radius,
                         fillColor, strokeColor, strokeWidth, 0);
        _primCount++;
    }

    //=========================================================================
    // Scene
    //=========================================================================

    void setSceneBounds(float minX, float minY,
                        float maxX, float maxY) override {
        (void)minX; (void)minY; (void)maxX; (void)maxY;
        // Scene bounds are implicit in binary v2 (computed on load)
    }

    void setBgColor(uint32_t color) override {
        _bgColor = color;
    }

    void setFlags(uint32_t flags) override {
        _flags = flags;
    }

    void addFlags(uint32_t flags) override {
        _flags |= flags;
    }

    //=========================================================================
    // Rotated text — each glyph becomes an SDF primitive
    //=========================================================================

    float addRotatedText(float x, float y, const std::string& text,
                          float fontSize, uint32_t color,
                          float rotationRadians, int fontId) override {
        if (text.empty() || fontId < 0 ||
            fontId >= static_cast<int>(_fonts.size()))
            return 0.0f;

        // Store as a span — the ydraw card resolves glyphs via builder
        RotatedTextSpan span;
        span.x = x;
        span.y = y;
        span.fontSize = fontSize;
        span.rotation = rotationRadians;
        span.color = color;
        span.fontIndex = static_cast<uint32_t>(fontId);
        span.textOffset = static_cast<uint32_t>(_textBlob.size());
        span.textLength = static_cast<uint32_t>(text.size());
        _textBlob += text;
        _rotatedSpans.push_back(span);

        return computeAdvance(text, fontSize, fontId);
    }

    //=========================================================================
    // Binary serialization
    //=========================================================================

    std::vector<uint8_t> buildBinary() const override {
        std::vector<uint8_t> out;

        // Header (v3)
        BinHeader header;
        header.primCount = _primCount;
        header.bgColor = _bgColor;
        header.flags = _flags;
        out.resize(sizeof(header));
        std::memcpy(out.data(), &header, sizeof(header));

        // totalCompactWords (u32) + compact prim data
        uint32_t totalCompactWords = static_cast<uint32_t>(_compactData.size());
        appendU32(out, totalCompactWords);

        if (!_compactData.empty()) {
            size_t primStart = out.size();
            size_t primBytes = _compactData.size() * sizeof(float);
            out.resize(primStart + primBytes);
            std::memcpy(out.data() + primStart, _compactData.data(), primBytes);
        }

        // Font section
        uint32_t fontCount = static_cast<uint32_t>(_fonts.size());
        appendU32(out, fontCount);

        for (const auto& font : _fonts) {
            uint32_t dataSize = static_cast<uint32_t>(font.data.size());
            uint32_t paddedSize = align4(dataSize);

            appendU32(out, dataSize);
            size_t dataStart = out.size();
            out.resize(out.size() + paddedSize, 0);
            std::memcpy(out.data() + dataStart, font.data.data(), dataSize);
        }

        // Text section
        uint32_t defCount = static_cast<uint32_t>(_defaultSpans.size());
        uint32_t cusCount = static_cast<uint32_t>(_customSpans.size());
        uint32_t textDataSize = static_cast<uint32_t>(_textBlob.size());
        uint32_t paddedTextSize = align4(textDataSize);

        appendU32(out, defCount);
        appendU32(out, cusCount);
        appendU32(out, textDataSize);

        // Default spans
        if (!_defaultSpans.empty()) {
            size_t start = out.size();
            out.resize(out.size() + _defaultSpans.size() * 32);
            std::memcpy(out.data() + start, _defaultSpans.data(),
                        _defaultSpans.size() * 32);
        }

        // Custom spans
        if (!_customSpans.empty()) {
            size_t start = out.size();
            out.resize(out.size() + _customSpans.size() * 32);
            std::memcpy(out.data() + start, _customSpans.data(),
                        _customSpans.size() * 32);
        }

        // Text blob
        if (!_textBlob.empty()) {
            size_t start = out.size();
            out.resize(out.size() + paddedTextSize, 0);
            std::memcpy(out.data() + start, _textBlob.data(), textDataSize);
        }

        // Rotated text spans (appended after text blob)
        uint32_t rotCount = static_cast<uint32_t>(_rotatedSpans.size());
        appendU32(out, rotCount);
        if (!_rotatedSpans.empty()) {
            size_t start = out.size();
            out.resize(out.size() + _rotatedSpans.size() * 32);
            std::memcpy(out.data() + start, _rotatedSpans.data(),
                        _rotatedSpans.size() * 32);
        }

        return out;
    }

    std::string buildBase64() const override {
        auto bin = buildBinary();
        return base64Encode(bin.data(), bin.size());
    }

private:
    float computeAdvance(const std::string& text, float fontSize,
                          int fontId) const {
        const TtfFontMetrics* metrics = nullptr;
        if (fontId >= 0 && fontId < static_cast<int>(_fonts.size())) {
            metrics = &_fonts[static_cast<size_t>(fontId)].metrics;
        }

        float advance = 0.0f;
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
        const uint8_t* end = ptr + text.size();

        while (ptr < end) {
            uint32_t cp = decodeUtf8(ptr, end);
            if (metrics) {
                advance += metrics->getAdvance(cp, fontSize);
            } else {
                advance += fontSize * 0.5f; // fallback
            }
        }

        return advance;
    }

    static void appendU32(std::vector<uint8_t>& out, uint32_t val) {
        size_t pos = out.size();
        out.resize(pos + 4);
        std::memcpy(out.data() + pos, &val, 4);
    }

    // State
    std::vector<FontEntry> _fonts;
    std::vector<DefaultTextSpan> _defaultSpans;
    std::vector<CustomTextSpan> _customSpans;
    std::vector<RotatedTextSpan> _rotatedSpans;
    std::string _textBlob;
    std::vector<float> _compactData;
    uint32_t _primCount = 0;
    uint32_t _bgColor = 0xFFFFFFFF;
    uint32_t _flags = 16; // FLAG_UNIFORM_SCALE
};

//=============================================================================
// Factory
//=============================================================================

Result<YDrawWriterBinary::Ptr> YDrawWriterBinary::createImpl() {
    return Ok<Ptr>(Ptr(new YDrawWriterBinaryImpl()));
}

} // namespace yetty
