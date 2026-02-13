// pdf2ydraw: Convert PDF text to YDraw binary v2 format
//
// Reads a PDF file, extracts embedded fonts, parses content streams,
// and outputs ydraw binary v2 with font uploads and text spans.
//
// Uses the PdfContentParser callback path with real TTF glyph advances
// for accurate text positioning (not the 0.6 approximation legacy path).

#include "pdf-content-parser.h"
#include <ytrace/ytrace.hpp>
#include <spdlog/sinks/basic_file_sink.h>

extern "C" {
#include <pdfio.h>
}

#include <args.hxx>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace yetty::card;

// Binary format structs (must match ydraw.cpp parsing)
struct BinHeader {
    uint32_t version = 2;
    uint32_t primCount = 0;
    uint32_t bgColor = 0xFFFFFFFF; // white
    uint32_t flags = 16;           // FLAG_UNIFORM_SCALE
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

// Extracted font data
struct FontData {
    std::string tag;
    std::vector<uint8_t> data;
    bool isIdentityH = false;  // True for Type0/Identity-H (2-byte CID encoding)
    std::unordered_map<uint16_t, uint32_t> toUnicode; // CID → Unicode codepoint
};

//=============================================================================
// TTF font metrics — parse cmap + hmtx + head tables for glyph advances
//=============================================================================

struct TtfFontMetrics {
    uint16_t unitsPerEm = 1000;
    std::vector<uint16_t> advanceWidths;             // indexed by glyphId
    std::unordered_map<uint32_t, uint16_t> cmapTable; // codepoint -> glyphId

    float getAdvance(uint32_t codepoint, float fontSize) const {
        uint16_t gid = 0;
        auto it = cmapTable.find(codepoint);
        if (it != cmapTable.end()) gid = it->second;
        uint16_t aw = (gid < advanceWidths.size()) ? advanceWidths[gid]
                     : static_cast<uint16_t>(unitsPerEm / 2);
        return static_cast<float>(aw) / static_cast<float>(unitsPerEm) * fontSize;
    }
};

static bool parseTtfMetrics(const uint8_t* data, size_t size, TtfFontMetrics& m) {
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

    uint16_t numberOfHMetrics = r16(hheaOff + 34);
    uint16_t numGlyphs = maxpOff ? r16(maxpOff + 4) : numberOfHMetrics;

    // Parse hmtx: advanceWidth per glyph
    m.advanceWidths.resize(numGlyphs, 0);
    uint16_t lastAdvance = 0;
    for (uint16_t i = 0; i < numGlyphs; i++) {
        if (i < numberOfHMetrics) {
            lastAdvance = r16(hmtxOff + static_cast<size_t>(i) * 4);
        }
        m.advanceWidths[i] = lastAdvance;
    }

    // Parse cmap: find best Unicode subtable
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
            if (pid == 3 && eid == 10) pri = 4;      // Windows full Unicode
            else if (pid == 3 && eid == 1) pri = 3;   // Windows BMP
            else if (pid == 0 && eid == 3) pri = 2;   // Unicode BMP
            else if (pid == 0) pri = 1;                // Any Unicode
            else if (pid == 1 && eid == 0) pri = 0;   // Mac Roman
            if (pri > bestPri) { bestPri = pri; bestOff = cmapOff + off; }
        }

        if (bestOff && bestOff + 2 <= size) {
            uint16_t fmt = r16(bestOff);
            if (fmt == 0) {
                // Format 0: Byte encoding table (256 uint8 glyphIds)
                if (bestOff + 6 + 256 <= size) {
                    for (uint32_t cp = 0; cp < 256; cp++) {
                        uint8_t gid = data[bestOff + 6 + cp];
                        if (gid != 0) m.cmapTable[cp] = gid;
                    }
                }
            } else if (fmt == 6) {
                // Format 6: Trimmed table mapping
                uint16_t firstCode  = r16(bestOff + 6);
                uint16_t entryCount = r16(bestOff + 8);
                for (uint16_t i = 0; i < entryCount; i++) {
                    uint16_t gid = r16(bestOff + 10 + static_cast<size_t>(i) * 2);
                    if (gid != 0) {
                        m.cmapTable[firstCode + i] = gid;
                    }
                }
            } else if (fmt == 4) {
                // Format 4: Segment mapping
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
                // Format 12: Segmented coverage (32-bit)
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

    yinfo("parseTtfMetrics: upm={} glyphs={} cmap={} hmetrics={}",
          m.unitsPerEm, numGlyphs, m.cmapTable.size(), numberOfHMetrics);
    return true;
}

// Decode one UTF-8 codepoint, advancing ptr
static uint32_t decodeUtf8(const uint8_t*& ptr, const uint8_t* end) {
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

// Re-decode Identity-H CID text using ToUnicode map.
// The content parser decoded 2-byte CIDs as individual WinAnsi bytes,
// producing pairs like {0x00, 0x51} for CID 0x0051.
// We reconstruct the CIDs and map them to Unicode via ToUnicode.
static std::string remapCidText(const std::string& text,
                                 const std::unordered_map<uint16_t, uint32_t>& toUnicode) {
    std::string out;
    out.reserve(text.size());
    const uint8_t* p = reinterpret_cast<const uint8_t*>(text.data());
    size_t len = text.size();

    // The WinAnsi decoder may have expanded some bytes to multi-byte UTF-8.
    // We need to get back to the original byte values.
    // For 0x00-0x7F: passed through as-is (single byte in UTF-8)
    // For 0x80-0x9F: expanded to 2-3 byte UTF-8 sequences (WinAnsi special chars)
    // For 0xA0-0xFF: expanded to 2-byte UTF-8 (Latin-1 supplement)
    // We need to reverse this to get original PDF bytes, then pair them as CIDs.

    // First: decode UTF-8 back to original byte values
    std::vector<uint8_t> rawBytes;
    rawBytes.reserve(len);
    size_t i = 0;
    while (i < len) {
        uint32_t cp = 0;
        uint8_t b = p[i];
        if ((b & 0x80) == 0) {
            cp = b; i++;
        } else if ((b & 0xE0) == 0xC0) {
            cp = (b & 0x1F) << 6;
            if (i + 1 < len) cp |= (p[i+1] & 0x3F);
            i += 2;
        } else if ((b & 0xF0) == 0xE0) {
            cp = (b & 0x0F) << 12;
            if (i + 1 < len) cp |= (p[i+1] & 0x3F) << 6;
            if (i + 2 < len) cp |= (p[i+2] & 0x3F);
            i += 3;
        } else {
            cp = b; i++;
        }
        // Reverse WinAnsi mapping: find original byte value
        // For codepoints 0-0x7F: same as byte value
        // For codepoints 0xA0-0xFF: same as byte value (Latin-1)
        // For WinAnsi specials (0x80-0x9F mapped): need reverse lookup
        if (cp < 0x100) {
            rawBytes.push_back(static_cast<uint8_t>(cp));
        } else {
            // WinAnsi special: reverse map. Just store 0 as placeholder.
            // These shouldn't appear in CID high bytes anyway.
            rawBytes.push_back(0);
        }
    }

    // Now pair raw bytes as 2-byte CIDs and map through ToUnicode
    for (size_t j = 0; j + 1 < rawBytes.size(); j += 2) {
        uint16_t cid = (static_cast<uint16_t>(rawBytes[j]) << 8) | rawBytes[j + 1];
        uint32_t uni = cid; // fallback: use CID as codepoint
        auto it = toUnicode.find(cid);
        if (it != toUnicode.end()) uni = it->second;

        // Encode as UTF-8
        if (uni < 0x80) {
            out += static_cast<char>(uni);
        } else if (uni < 0x800) {
            out += static_cast<char>(0xC0 | (uni >> 6));
            out += static_cast<char>(0x80 | (uni & 0x3F));
        } else if (uni < 0x10000) {
            out += static_cast<char>(0xE0 | (uni >> 12));
            out += static_cast<char>(0x80 | ((uni >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (uni & 0x3F));
        } else {
            out += static_cast<char>(0xF0 | (uni >> 18));
            out += static_cast<char>(0x80 | ((uni >> 12) & 0x3F));
            out += static_cast<char>(0x80 | ((uni >> 6) & 0x3F));
            out += static_cast<char>(0x80 | (uni & 0x3F));
        }
    }
    return out;
}

// Align to 4 bytes
static uint32_t align4(uint32_t n) { return (n + 3) & ~3u; }

static bool pdfioErrorHandler(pdfio_file_t*, const char* message, void*) {
    ywarn("pdfio: {}", message);
    return true;
}

// Parse a ToUnicode CMap stream into a CID→Unicode mapping
static void parseToUnicodeCMap(pdfio_obj_t* cmapObj,
                                std::unordered_map<uint16_t, uint32_t>& mapping) {
    pdfio_stream_t* stream = pdfioObjOpenStream(cmapObj, true);
    if (!stream) return;

    std::string data;
    uint8_t buf[4096];
    ssize_t n;
    while ((n = pdfioStreamRead(stream, buf, sizeof(buf))) > 0)
        data.append(reinterpret_cast<char*>(buf), static_cast<size_t>(n));
    pdfioStreamClose(stream);

    // Parse hex value from "<XXXX>" format
    auto parseHex = [](const char* s, size_t len) -> uint32_t {
        uint32_t v = 0;
        for (size_t i = 0; i < len; i++) {
            char c = s[i];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= c - '0';
            else if (c >= 'a' && c <= 'f') v |= c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') v |= c - 'A' + 10;
        }
        return v;
    };

    // Find hex tokens between < and >
    auto nextHex = [&](size_t& pos) -> uint32_t {
        while (pos < data.size() && data[pos] != '<') pos++;
        if (pos >= data.size()) return 0;
        pos++; // skip '<'
        size_t start = pos;
        while (pos < data.size() && data[pos] != '>') pos++;
        uint32_t v = parseHex(data.c_str() + start, pos - start);
        if (pos < data.size()) pos++; // skip '>'
        return v;
    };

    size_t pos = 0;
    while (pos < data.size()) {
        // Find "beginbfchar" or "beginbfrange"
        size_t bfchar = data.find("beginbfchar", pos);
        size_t bfrange = data.find("beginbfrange", pos);

        if (bfchar == std::string::npos && bfrange == std::string::npos) break;

        if (bfchar != std::string::npos &&
            (bfrange == std::string::npos || bfchar < bfrange)) {
            // beginbfchar: pairs of <CID> <Unicode>
            pos = bfchar + 11;
            size_t endPos = data.find("endbfchar", pos);
            if (endPos == std::string::npos) endPos = data.size();
            while (pos < endPos) {
                uint32_t cid = nextHex(pos);
                if (pos >= endPos) break;
                uint32_t uni = nextHex(pos);
                mapping[static_cast<uint16_t>(cid)] = uni;
            }
            pos = endPos + 9;
        } else {
            // beginbfrange: triples of <start> <end> <startUnicode>
            pos = bfrange + 12;
            size_t endPos = data.find("endbfrange", pos);
            if (endPos == std::string::npos) endPos = data.size();
            while (pos < endPos) {
                uint32_t startCid = nextHex(pos);
                if (pos >= endPos) break;
                uint32_t endCid = nextHex(pos);
                if (pos >= endPos) break;
                uint32_t startUni = nextHex(pos);
                for (uint32_t cid = startCid; cid <= endCid; cid++) {
                    mapping[static_cast<uint16_t>(cid)] =
                        startUni + (cid - startCid);
                }
            }
            pos = endPos + 10;
        }
    }
}

// Extract embedded fonts from a page's resources
static void extractPageFonts(pdfio_obj_t* pageObj,
                             std::unordered_map<std::string, uint32_t>& fontTagToIndex,
                             std::vector<FontData>& fonts) {
    pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
    if (!pageDict) return;

    pdfio_dict_t* resources = pdfioDictGetDict(pageDict, "Resources");
    if (!resources) {
        pdfio_obj_t* resObj = pdfioDictGetObj(pageDict, "Resources");
        if (resObj) resources = pdfioObjGetDict(resObj);
    }
    if (!resources) return;

    pdfio_dict_t* fontDict = pdfioDictGetDict(resources, "Font");
    if (!fontDict) {
        pdfio_obj_t* fontDictObj = pdfioDictGetObj(resources, "Font");
        if (fontDictObj) fontDict = pdfioObjGetDict(fontDictObj);
    }
    if (!fontDict) return;

    size_t numFonts = pdfioDictGetNumPairs(fontDict);
    for (size_t i = 0; i < numFonts; i++) {
        const char* fontTag = pdfioDictGetKey(fontDict, i);
        if (!fontTag) continue;

        std::string tag(fontTag);
        if (fontTagToIndex.count(tag) || fontTagToIndex.count("/" + tag))
            continue;

        pdfio_obj_t* fontObj = pdfioDictGetObj(fontDict, fontTag);
        if (!fontObj) continue;

        pdfio_dict_t* fontObjDict = pdfioObjGetDict(fontObj);
        if (!fontObjDict) continue;

        // Find FontDescriptor — either directly on font or via DescendantFonts
        pdfio_obj_t* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
        bool isIdentityH = false;
        pdfio_obj_t* toUnicodeObj = pdfioDictGetObj(fontObjDict, "ToUnicode");

        // Check encoding — Encoding is a Name value in PDF
        const char* encoding = pdfioDictGetName(fontObjDict, "Encoding");
        if (encoding) {
            std::string enc(encoding);
            if (enc == "Identity-H" || enc == "/Identity-H")
                isIdentityH = true;
        }

        const char* subtype = pdfioDictGetName(fontObjDict, "Subtype");

        if (!fontDescObj) {
            // Type0 (composite) fonts: FontDescriptor is in DescendantFonts[0]
            pdfio_array_t* descFonts = pdfioDictGetArray(fontObjDict, "DescendantFonts");
            if (!descFonts) {
                // Try as object reference
                pdfio_obj_t* dfObj = pdfioDictGetObj(fontObjDict, "DescendantFonts");
                if (dfObj) {
                    // The object might contain an array
                    descFonts = pdfioObjGetArray(dfObj);
                }
            }
            if (descFonts) {
                size_t arrSize = pdfioArrayGetSize(descFonts);
                if (arrSize > 0) {
                    pdfio_obj_t* cidFontObj = pdfioArrayGetObj(descFonts, 0);
                    if (cidFontObj) {
                        pdfio_dict_t* cidFontDict = pdfioObjGetDict(cidFontObj);
                        if (cidFontDict)
                            fontDescObj = pdfioDictGetObj(cidFontDict, "FontDescriptor");
                    }
                }
            }
            yinfo("Font '{}': subtype={} encoding={} descFonts={} fontDescObj={}",
                  tag, subtype ? subtype : "?", encoding ? encoding : "?",
                  descFonts != nullptr, fontDescObj != nullptr);
        }
        if (!fontDescObj) continue;

        pdfio_dict_t* fontDescDict = pdfioObjGetDict(fontDescObj);
        if (!fontDescDict) continue;

        pdfio_obj_t* fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile2");
        if (!fontFileObj)
            fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile3");
        if (!fontFileObj) continue;

        pdfio_stream_t* stream = pdfioObjOpenStream(fontFileObj, true);
        if (!stream) continue;

        std::vector<uint8_t> fontBytes;
        uint8_t buf[8192];
        ssize_t bytesRead;
        while ((bytesRead = pdfioStreamRead(stream, buf, sizeof(buf))) > 0) {
            fontBytes.insert(fontBytes.end(), buf, buf + bytesRead);
        }
        pdfioStreamClose(stream);

        if (fontBytes.empty()) continue;

        uint32_t fontIndex = static_cast<uint32_t>(fonts.size());
        fontTagToIndex[tag] = fontIndex;
        fontTagToIndex["/" + tag] = fontIndex;

        FontData fd{tag, std::move(fontBytes), isIdentityH, {}};

        // Parse ToUnicode CMap if present
        if (toUnicodeObj) {
            parseToUnicodeCMap(toUnicodeObj, fd.toUnicode);
            yinfo("Font '{}': parsed ToUnicode CMap with {} mappings",
                  tag, fd.toUnicode.size());
        }

        fonts.push_back(std::move(fd));

        yinfo("Extracted font '{}' ({} bytes) identityH={} -> fontIndex {}",
              tag, fonts.back().data.size(), isIdentityH, fontIndex);
    }
}

// Build the v2 binary blob
static std::vector<uint8_t> buildBinary(
    const std::vector<FontData>& fonts,
    const std::vector<DefaultTextSpan>& defaultSpans,
    const std::vector<CustomTextSpan>& customSpans,
    const std::string& textBlob)
{
    std::vector<uint8_t> out;

    // Header
    BinHeader header;
    out.resize(sizeof(header));
    std::memcpy(out.data(), &header, sizeof(header));

    // No primitives (primCount = 0)

    // Font section
    uint32_t fontCount = static_cast<uint32_t>(fonts.size());
    out.resize(out.size() + 4);
    std::memcpy(out.data() + out.size() - 4, &fontCount, 4);

    for (const auto& font : fonts) {
        uint32_t dataSize = static_cast<uint32_t>(font.data.size());
        uint32_t paddedSize = align4(dataSize);

        out.resize(out.size() + 4);
        std::memcpy(out.data() + out.size() - 4, &dataSize, 4);

        size_t dataStart = out.size();
        out.resize(out.size() + paddedSize, 0);
        std::memcpy(out.data() + dataStart, font.data.data(), dataSize);
    }

    // Text section
    uint32_t defCount = static_cast<uint32_t>(defaultSpans.size());
    uint32_t cusCount = static_cast<uint32_t>(customSpans.size());
    uint32_t textDataSize = static_cast<uint32_t>(textBlob.size());
    uint32_t paddedTextSize = align4(textDataSize);

    // 3 header u32s
    size_t hdrStart = out.size();
    out.resize(out.size() + 12);
    std::memcpy(out.data() + hdrStart + 0, &defCount, 4);
    std::memcpy(out.data() + hdrStart + 4, &cusCount, 4);
    std::memcpy(out.data() + hdrStart + 8, &textDataSize, 4);

    // Default spans
    if (!defaultSpans.empty()) {
        size_t spanStart = out.size();
        out.resize(out.size() + defaultSpans.size() * 32);
        std::memcpy(out.data() + spanStart, defaultSpans.data(),
                    defaultSpans.size() * 32);
    }

    // Custom spans
    if (!customSpans.empty()) {
        size_t spanStart = out.size();
        out.resize(out.size() + customSpans.size() * 32);
        std::memcpy(out.data() + spanStart, customSpans.data(),
                    customSpans.size() * 32);
    }

    // Text data blob
    if (!textBlob.empty()) {
        size_t blobStart = out.size();
        out.resize(out.size() + paddedTextSize, 0);
        std::memcpy(out.data() + blobStart, textBlob.data(), textDataSize);
    }

    return out;
}

// Base64 encode
static const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64Encode(const uint8_t* data, size_t len) {
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

int main(int argc, char** argv) {
    args::ArgumentParser parser("pdf2ydraw - Convert PDF to YDraw binary v2");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Flag oscFlag(parser, "osc", "Wrap output in OSC escape sequence", {"osc"});
    args::ValueFlag<int> xFlag(parser, "X", "Card X position (cells)", {'x'}, 0);
    args::ValueFlag<int> yFlag(parser, "Y", "Card Y position (cells)", {'y'}, 0);
    args::ValueFlag<int> wFlag(parser, "W", "Card width (cells)", {'w'}, 80);
    args::ValueFlag<int> hFlag(parser, "H", "Card height (cells)", {'H'}, 25);
    args::Positional<std::string> pdfFile(parser, "file", "PDF file to convert");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (!pdfFile) {
        std::cerr << "Error: no PDF file specified\n";
        return 1;
    }

    // In OSC mode, redirect all spdlog output to a file instead of stderr
    // (stderr output corrupts the terminal when mixed with OSC escapes)
    auto fileLogger = spdlog::basic_logger_mt("pdf2ydraw", "/tmp/pdf2ydraw.log", true);
    spdlog::set_default_logger(fileLogger);
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);

    std::string path = args::get(pdfFile);
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: file not found: " << path << "\n";
        return 1;
    }

    // Open PDF
    pdfio_file_t* pdf = pdfioFileOpen(path.c_str(), nullptr, nullptr,
                                       pdfioErrorHandler, nullptr);
    if (!pdf) {
        std::cerr << "Error: failed to open PDF: " << path << "\n";
        return 1;
    }

    int pageCount = static_cast<int>(pdfioFileGetNumPages(pdf));
    if (pageCount == 0) {
        std::cerr << "Error: PDF has no pages\n";
        pdfioFileClose(pdf);
        return 1;
    }

    yinfo("pdf2ydraw: opened '{}' with {} pages", path, pageCount);

    // Collect fonts and text spans across all pages
    std::vector<FontData> fonts;
    std::vector<TtfFontMetrics> fontMetrics;
    std::unordered_map<std::string, uint32_t> fontTagToIndex;
    std::vector<DefaultTextSpan> defaultSpans;
    std::vector<CustomTextSpan> customSpans;
    std::string textBlob;

    float yOffset = 0.0f;
    constexpr float PAGE_MARGIN = 20.0f;
    float firstPageH = 0.0f;

    for (int page = 0; page < pageCount; page++) {
        pdfio_obj_t* pageObj = pdfioFileGetPage(pdf, static_cast<size_t>(page));
        if (!pageObj) continue;

        pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
        pdfio_rect_t mediaBox = {};
        if (!pdfioDictGetRect(pageDict, "MediaBox", &mediaBox)) {
            mediaBox = {0.0, 0.0, 612.0, 792.0};
        }
        float pageH = static_cast<float>(mediaBox.y2 - mediaBox.y1);
        if (page == 0) firstPageH = pageH;

        // Extract fonts and parse their TTF metrics
        size_t fontsBefore = fonts.size();
        extractPageFonts(pageObj, fontTagToIndex, fonts);
        // Parse TTF metrics for newly extracted fonts
        fontMetrics.resize(fonts.size());
        for (size_t fi = fontsBefore; fi < fonts.size(); fi++) {
            if (!parseTtfMetrics(fonts[fi].data.data(), fonts[fi].data.size(),
                                 fontMetrics[fi])) {
                ywarn("pdf2ydraw: failed to parse TTF metrics for font '{}'",
                      fonts[fi].tag);
            }
        }

        // Parse content streams with callback for accurate glyph advances
        PdfContentParser contentParser;
        contentParser.setPageHeight(pageH);

        // Set callback that creates spans AND returns accurate advance
        contentParser.setTextEmitCallback(
            [&](const std::string& text, float posX, float posY,
                float effectiveSize, const PdfTextState& textState) -> float {
                // Flip Y: PDF bottom-left → top-left
                float sx = posX;
                float sy = yOffset + (pageH - posY);

                // Check if this font has an embedded TTF
                auto it = fontTagToIndex.find(textState.fontName);

                // For Identity-H fonts, remap CID values to Unicode
                std::string actualText = text;
                if (it != fontTagToIndex.end()) {
                    const auto& fontData = fonts[it->second];
                    if (fontData.isIdentityH && !fontData.toUnicode.empty()) {
                        actualText = remapCidText(text, fontData.toUnicode);
                    }
                }

                uint32_t textOffset = static_cast<uint32_t>(textBlob.size());
                uint32_t textLength = static_cast<uint32_t>(actualText.size());
                textBlob += actualText;

                uint32_t color = 0xFF000000; // black

                if (it != fontTagToIndex.end()) {
                    CustomTextSpan cs = {};
                    cs.x = sx;
                    cs.y = sy;
                    cs.fontSize = effectiveSize;
                    cs.color = color;
                    cs.fontIndex = it->second;
                    cs.layer = 0;
                    cs.textOffset = textOffset;
                    cs.textLength = textLength;
                    customSpans.push_back(cs);
                } else {
                    DefaultTextSpan ds = {};
                    ds.x = sx;
                    ds.y = sy;
                    ds.fontSize = effectiveSize;
                    ds.color = color;
                    ds.layer = 0;
                    ds.textOffset = textOffset;
                    ds.textLength = textLength;
                    defaultSpans.push_back(ds);
                }

                // Compute total advance using real TTF metrics
                float totalAdvance = 0.0f;
                float hScale = textState.horizontalScaling / 100.0f;

                // Get font metrics if available
                const TtfFontMetrics* metrics = nullptr;
                if (it != fontTagToIndex.end() && it->second < fontMetrics.size()) {
                    metrics = &fontMetrics[it->second];
                }

                const uint8_t* ptr = reinterpret_cast<const uint8_t*>(actualText.data());
                const uint8_t* end = ptr + actualText.size();
                while (ptr < end) {
                    uint32_t cp = decodeUtf8(ptr, end);
                    float charAdvance = metrics
                        ? metrics->getAdvance(cp, textState.fontSize)
                        : textState.fontSize * 0.6f;
                    charAdvance += textState.charSpacing;
                    if (cp == 0x20) charAdvance += textState.wordSpacing;
                    totalAdvance += charAdvance * hScale;
                }

                return totalAdvance;
            });

        size_t numStreams = pdfioPageGetNumStreams(pageObj);
        for (size_t s = 0; s < numStreams; s++) {
            pdfio_stream_t* stream = pdfioPageOpenStream(pageObj, s, true);
            if (!stream) continue;
            contentParser.parseStream(stream);
            pdfioStreamClose(stream);
        }

        yOffset += pageH;
        if (page < pageCount - 1) yOffset += PAGE_MARGIN;
    }

    pdfioFileClose(pdf);

    yinfo("pdf2ydraw: pages={} fonts={} defaultSpans={} customSpans={} textBytes={} yOffset={:.1f} firstPageH={:.1f}",
          pageCount, fonts.size(), defaultSpans.size(), customSpans.size(),
          textBlob.size(), yOffset, firstPageH);
    for (size_t i = 0; i < fonts.size(); i++)
        yinfo("  font[{}] tag='{}' bytes={} cmap={} identityH={}",
              i, fonts[i].tag, fonts[i].data.size(), fontMetrics[i].cmapTable.size(),
              fonts[i].isIdentityH);
    if (!customSpans.empty()) {
        auto& s = customSpans[0];
        yinfo("  first custom span: x={:.1f} y={:.1f} fontSize={:.1f} fontIndex={} textOff={} textLen={}",
              s.x, s.y, s.fontSize, s.fontIndex, s.textOffset, s.textLength);
    }
    if (!defaultSpans.empty()) {
        auto& s = defaultSpans[0];
        yinfo("  first default span: x={:.1f} y={:.1f} fontSize={:.1f} textOff={} textLen={}",
              s.x, s.y, s.fontSize, s.textOffset, s.textLength);
    }

    // Build binary
    auto binary = buildBinary(fonts, defaultSpans, customSpans, textBlob);

    if (oscFlag) {
        std::string b64 = base64Encode(binary.data(), binary.size());

        // Pass first page height so ydraw card can compute zoom/pan after scene bounds are known
        float pageH = firstPageH > 0.0f ? firstPageH : 792.0f;
        char header[512];
        snprintf(header, sizeof(header),
                 "\033]666666;run -c ydraw -x %d -y %d -w %d -h %d -r"
                 ";bin --fit-page-height %.1f;",
                 args::get(xFlag), args::get(yFlag),
                 args::get(wFlag), args::get(hFlag),
                 pageH);

        yinfo("binary={} bytes, b64={} bytes, header='{}'",
              binary.size(), b64.size(), std::string(header + 1));
        std::cout << header << b64 << "\033\\" << std::endl;
    } else {
        // Raw binary to stdout
        std::cout.write(reinterpret_cast<const char*>(binary.data()),
                        static_cast<std::streamsize>(binary.size()));
    }

    return 0;
}
