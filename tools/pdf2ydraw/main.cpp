// pdf2ydraw: Convert PDF text to YDraw binary v2 format
//
// Reads a PDF file, extracts embedded fonts, parses content streams,
// and outputs ydraw binary v2 with font uploads and text spans.
//
// Uses the PdfContentParser callback path with real TTF glyph advances
// for accurate text positioning (not the 0.6 approximation legacy path).

#include "pdf-content-parser.h"
#include <ytrace/ytrace.hpp>

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

// Align to 4 bytes
static uint32_t align4(uint32_t n) { return (n + 3) & ~3u; }

static bool pdfioErrorHandler(pdfio_file_t*, const char* message, void*) {
    ywarn("pdfio: {}", message);
    return true;
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

        pdfio_obj_t* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
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
        fonts.push_back({tag, std::move(fontBytes)});

        yinfo("Extracted font '{}' ({} bytes) -> fontIndex {}",
              tag, fonts.back().data.size(), fontIndex);
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

    // Suppress logging in OSC mode — log output on stderr corrupts the terminal
    // Write diagnostics to /tmp/pdf2ydraw.log instead
    FILE* logFile = nullptr;
    if (oscFlag) {
        ydisable_all();
        logFile = fopen("/tmp/pdf2ydraw.log", "w");
    }

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

                uint32_t textOffset = static_cast<uint32_t>(textBlob.size());
                uint32_t textLength = static_cast<uint32_t>(text.size());
                textBlob += text;

                uint32_t color = 0xFF000000; // black

                // Check if this font has an embedded TTF
                auto it = fontTagToIndex.find(textState.fontName);
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

                const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
                const uint8_t* end = ptr + text.size();
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

    yinfo("pdf2ydraw: {} fonts, {} default spans, {} custom spans, {} text bytes",
          fonts.size(), defaultSpans.size(), customSpans.size(), textBlob.size());
    if (logFile) {
        fprintf(logFile, "pages=%d fonts=%zu defaultSpans=%zu customSpans=%zu textBytes=%zu yOffset=%.1f firstPageH=%.1f\n",
                pageCount, fonts.size(), defaultSpans.size(), customSpans.size(),
                textBlob.size(), yOffset, firstPageH);
        for (size_t i = 0; i < fonts.size(); i++)
            fprintf(logFile, "  font[%zu] tag='%s' bytes=%zu cmap=%zu\n",
                    i, fonts[i].tag.c_str(), fonts[i].data.size(), fontMetrics[i].cmapTable.size());
        if (!customSpans.empty()) {
            auto& s = customSpans[0];
            fprintf(logFile, "  first custom span: x=%.1f y=%.1f fontSize=%.1f fontIndex=%u textOff=%u textLen=%u\n",
                    s.x, s.y, s.fontSize, s.fontIndex, s.textOffset, s.textLength);
        }
        if (!defaultSpans.empty()) {
            auto& s = defaultSpans[0];
            fprintf(logFile, "  first default span: x=%.1f y=%.1f fontSize=%.1f textOff=%u textLen=%u\n",
                    s.x, s.y, s.fontSize, s.textOffset, s.textLength);
        }
        fflush(logFile);
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

        if (logFile) {
            fprintf(logFile, "binary=%zu bytes, b64=%zu bytes, header='%s'\n",
                    binary.size(), b64.size(), header + 1); // skip ESC
            fclose(logFile);
            logFile = nullptr;
        }
        std::cout << header << b64 << "\033\\" << std::endl;
    } else {
        // Raw binary to stdout
        std::cout.write(reinterpret_cast<const char*>(binary.data()),
                        static_cast<std::streamsize>(binary.size()));
    }

    return 0;
}
