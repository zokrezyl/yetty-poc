// Shared PDF → YDrawWriter rendering.
// Used by both the internal ypdf card and the external pdf2ydraw tool.

#include "pdf-renderer.h"
#include "pdf-content-parser.h"
#include <yetty/ydraw-writer.h>

extern "C" {
#include <pdfio.h>
}

#include <ytrace/ytrace.hpp>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using namespace yetty::card;

namespace {

//=============================================================================
// Font info — tracks extracted fonts and CID remapping state
//=============================================================================

struct PdfFontInfo {
    std::string tag;
    int writerId = -1;
    bool isIdentityH = false;
    std::unordered_map<uint16_t, uint32_t> toUnicode;
};

//=============================================================================
// Color helpers
//=============================================================================

uint32_t rgbToABGR(float r, float g, float b) {
    auto c = [](float f) -> uint32_t {
        return static_cast<uint32_t>(std::clamp(f, 0.0f, 1.0f) * 255.0f);
    };
    return 0xFF000000u | (c(b) << 16) | (c(g) << 8) | c(r);
}

//=============================================================================
// ToUnicode CMap parsing — maps CID → Unicode codepoint
//=============================================================================

void parseToUnicodeCMap(pdfio_obj_t* cmapObj,
                        std::unordered_map<uint16_t, uint32_t>& mapping) {
    pdfio_stream_t* stream = pdfioObjOpenStream(cmapObj, true);
    if (!stream) return;

    std::string data;
    uint8_t buf[4096];
    ssize_t n;
    while ((n = pdfioStreamRead(stream, buf, sizeof(buf))) > 0)
        data.append(reinterpret_cast<char*>(buf), static_cast<size_t>(n));
    pdfioStreamClose(stream);

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

    auto nextHex = [&](size_t& pos) -> uint32_t {
        while (pos < data.size() && data[pos] != '<') pos++;
        if (pos >= data.size()) return 0;
        pos++;
        size_t start = pos;
        while (pos < data.size() && data[pos] != '>') pos++;
        uint32_t v = parseHex(data.c_str() + start, pos - start);
        if (pos < data.size()) pos++;
        return v;
    };

    size_t pos = 0;
    while (pos < data.size()) {
        size_t bfchar = data.find("beginbfchar", pos);
        size_t bfrange = data.find("beginbfrange", pos);

        if (bfchar == std::string::npos && bfrange == std::string::npos) break;

        if (bfchar != std::string::npos &&
            (bfrange == std::string::npos || bfchar < bfrange)) {
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

//=============================================================================
// CID text remapping — re-decode Identity-H CID text using ToUnicode
//=============================================================================

std::string remapCidText(const std::string& text,
                         const std::unordered_map<uint16_t, uint32_t>& toUnicode) {
    std::string out;
    out.reserve(text.size());
    const uint8_t* p = reinterpret_cast<const uint8_t*>(text.data());
    size_t len = text.size();

    // Decode UTF-8 back to original byte values
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
        if (cp < 0x100) {
            rawBytes.push_back(static_cast<uint8_t>(cp));
        } else {
            rawBytes.push_back(0);
        }
    }

    // Pair raw bytes as 2-byte CIDs and map through ToUnicode
    for (size_t j = 0; j + 1 < rawBytes.size(); j += 2) {
        uint16_t cid = (static_cast<uint16_t>(rawBytes[j]) << 8) | rawBytes[j + 1];
        uint32_t uni = cid;
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

//=============================================================================
// Font extraction — find embedded TTF fonts in a page and register with writer
//=============================================================================

void extractPageFonts(pdfio_obj_t* pageObj,
                      std::unordered_map<std::string, size_t>& fontTagToIndex,
                      std::vector<PdfFontInfo>& fonts,
                      yetty::YDrawWriter* writer) {
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
    for (size_t fi = 0; fi < numFonts; fi++) {
        const char* fontTag = pdfioDictGetKey(fontDict, fi);
        if (!fontTag) continue;

        std::string tag(fontTag);
        if (fontTagToIndex.count(tag) || fontTagToIndex.count("/" + tag))
            continue;

        pdfio_obj_t* fontObj = pdfioDictGetObj(fontDict, fontTag);
        if (!fontObj) continue;

        pdfio_dict_t* fontObjDict = pdfioObjGetDict(fontObj);
        if (!fontObjDict) continue;

        pdfio_obj_t* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
        bool isIdentityH = false;
        pdfio_obj_t* toUnicodeObj = pdfioDictGetObj(fontObjDict, "ToUnicode");

        const char* encoding = pdfioDictGetName(fontObjDict, "Encoding");
        if (encoding) {
            std::string enc(encoding);
            if (enc == "Identity-H" || enc == "/Identity-H")
                isIdentityH = true;
        }

        if (!fontDescObj) {
            // Type0 (composite) fonts: FontDescriptor is in DescendantFonts[0]
            pdfio_array_t* descFonts = pdfioDictGetArray(fontObjDict, "DescendantFonts");
            if (!descFonts) {
                pdfio_obj_t* dfObj = pdfioDictGetObj(fontObjDict, "DescendantFonts");
                if (dfObj) descFonts = pdfioObjGetArray(dfObj);
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
        while ((bytesRead = pdfioStreamRead(stream, buf, sizeof(buf))) > 0)
            fontBytes.insert(fontBytes.end(), buf, buf + bytesRead);
        pdfioStreamClose(stream);

        if (fontBytes.empty()) continue;

        // Register font with writer
        auto res = writer->addFontData(fontBytes.data(), fontBytes.size(), tag);
        if (!res) {
            ywarn("extractPageFonts: addFontData failed for '{}': {}",
                  tag, res.error().message());
            continue;
        }

        size_t idx = fonts.size();
        fontTagToIndex[tag] = idx;
        fontTagToIndex["/" + tag] = idx;

        PdfFontInfo info;
        info.tag = tag;
        info.writerId = *res;
        info.isIdentityH = isIdentityH;

        // Parse ToUnicode CMap if present
        if (toUnicodeObj) {
            parseToUnicodeCMap(toUnicodeObj, info.toUnicode);
            yinfo("Font '{}': ToUnicode CMap with {} mappings", tag, info.toUnicode.size());
        }

        fonts.push_back(std::move(info));

        yinfo("Extracted font '{}' ({} bytes) identityH={} → writerId {}",
              tag, fontBytes.size(), isIdentityH, *res);
    }
}

} // anonymous namespace

//=============================================================================
// renderPdfToWriter — the shared PDF → YDrawWriter conversion
//=============================================================================

namespace yetty::card {

PdfRenderResult renderPdfToWriter(pdfio_file_t* pdf, yetty::YDrawWriter* writer) {
    PdfRenderResult result;

    int pageCount = static_cast<int>(pdfioFileGetNumPages(pdf));
    result.pageCount = pageCount;
    if (pageCount == 0) return result;

    std::vector<PdfFontInfo> fonts;
    std::unordered_map<std::string, size_t> fontTagToIndex;

    float yOffset = 0.0f;
    constexpr float PAGE_MARGIN = 20.0f;
    float maxWidth = 0.0f;

    for (int page = 0; page < pageCount; page++) {
        pdfio_obj_t* pageObj = pdfioFileGetPage(pdf, static_cast<size_t>(page));
        if (!pageObj) continue;

        pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
        pdfio_rect_t mediaBox = {};
        if (!pdfioDictGetRect(pageDict, "MediaBox", &mediaBox))
            mediaBox = {0.0, 0.0, 612.0, 792.0};

        float pageW = static_cast<float>(mediaBox.x2 - mediaBox.x1);
        float pageH = static_cast<float>(mediaBox.y2 - mediaBox.y1);
        if (page == 0) result.firstPageHeight = pageH;
        maxWidth = std::max(maxWidth, pageW);

        // Extract embedded fonts from this page
        extractPageFonts(pageObj, fontTagToIndex, fonts, writer);

        // Set up parser with callbacks targeting the writer
        PdfContentParser parser;
        parser.setPageHeight(pageH);

        // --- Text callback ---
        parser.setTextEmitCallback(
            [&](const std::string& text, float posX, float posY,
                float effectiveSize, float rotationRadians,
                const PdfTextState& textState) -> float {
                float sx = posX;
                float sy = yOffset + (pageH - posY);

                // Resolve font
                int fontId = -1;
                auto it = fontTagToIndex.find(textState.fontName);
                if (it != fontTagToIndex.end())
                    fontId = fonts[it->second].writerId;

                // CID remapping for Identity-H fonts
                std::string actualText = text;
                if (it != fontTagToIndex.end()) {
                    const auto& fi = fonts[it->second];
                    if (fi.isIdentityH && !fi.toUnicode.empty())
                        actualText = remapCidText(text, fi.toUnicode);
                }

                uint32_t color = 0xFF000000; // black

                // Visual placement at effectiveSize
                if (std::abs(rotationRadians) > 0.001f) {
                    // Negate rotation: PDF Y-up → screen Y-down flips angles
                    float screenRotation = -rotationRadians;
                    writer->addRotatedText(sx, sy, actualText,
                                           effectiveSize, color,
                                           screenRotation, fontId);
                } else {
                    writer->addText(sx, sy, actualText,
                                    effectiveSize, color, fontId);
                }

                // Advance for text matrix tracking uses textState.fontSize
                // (effectiveSize includes text matrix scaling, which the parser
                // applies separately via _textMatrix.a/b)
                float rawAdvance = writer->measureText(
                    actualText, textState.fontSize, fontId);

                // Apply PDF text state adjustments (charSpacing, wordSpacing,
                // horizontalScaling) per PDF spec 9.3
                float hScale = textState.horizontalScaling / 100.0f;
                int numCodepoints = 0, numSpaces = 0;
                const uint8_t* p = reinterpret_cast<const uint8_t*>(
                    actualText.data());
                const uint8_t* pEnd = p + actualText.size();
                while (p < pEnd) {
                    uint32_t cp = 0;
                    if ((*p & 0x80) == 0) { cp = *p++; }
                    else if ((*p & 0xE0) == 0xC0) { cp = *p++ & 0x1F; if (p < pEnd) cp = (cp << 6) | (*p++ & 0x3F); }
                    else if ((*p & 0xF0) == 0xE0) { p += 3; }
                    else if ((*p & 0xF8) == 0xF0) { p += 4; }
                    else { p++; }
                    numCodepoints++;
                    if (cp == 0x20) numSpaces++;
                }

                return (rawAdvance
                        + numCodepoints * textState.charSpacing
                        + numSpaces * textState.wordSpacing) * hScale;
            });

        // --- Rectangle callback ---
        parser.setRectPaintCallback(
            [&](const PdfRect& rect, PdfPaintMode mode,
                float sr, float sg, float sb,
                float fr, float fg, float fb,
                float lineWidth) {
                float x = rect.x;
                float y = yOffset + (pageH - rect.y - rect.h);
                float w = rect.w;
                float h = rect.h;

                if (mode == PdfPaintMode::Fill ||
                    mode == PdfPaintMode::FillAndStroke) {
                    uint32_t fc = rgbToABGR(fr, fg, fb);
                    writer->addBox(x + w * 0.5f, y + h * 0.5f,
                                   w * 0.5f, h * 0.5f, fc, 0, 0);
                }
                if (mode == PdfPaintMode::Stroke ||
                    mode == PdfPaintMode::FillAndStroke) {
                    uint32_t sc = rgbToABGR(sr, sg, sb);
                    writer->addSegment(x,     y,     x + w, y,     sc, lineWidth);
                    writer->addSegment(x + w, y,     x + w, y + h, sc, lineWidth);
                    writer->addSegment(x + w, y + h, x,     y + h, sc, lineWidth);
                    writer->addSegment(x,     y + h, x,     y,     sc, lineWidth);
                }
            });

        // --- Line callback ---
        parser.setLinePaintCallback(
            [&](float x0, float y0, float x1, float y1,
                float r, float g, float b, float lineWidth) {
                float sy0 = yOffset + (pageH - y0);
                float sy1 = yOffset + (pageH - y1);
                uint32_t color = rgbToABGR(r, g, b);
                writer->addSegment(x0, sy0, x1, sy1, color, lineWidth);
            });

        // Parse all content streams for this page
        size_t numStreams = pdfioPageGetNumStreams(pageObj);
        for (size_t s = 0; s < numStreams; s++) {
            pdfio_stream_t* stream = pdfioPageOpenStream(pageObj, s, true);
            if (!stream) continue;
            parser.parseStream(stream);
            pdfioStreamClose(stream);
        }

        yOffset += pageH;
        if (page < pageCount - 1) yOffset += PAGE_MARGIN;
    }

    result.totalHeight = yOffset;
    writer->setSceneBounds(0, 0, maxWidth, yOffset);
    writer->setBgColor(0xFFFFFFFF);

    yinfo("renderPdfToWriter: {} pages, {} fonts, totalHeight={:.1f}",
          pageCount, fonts.size(), yOffset);

    return result;
}

} // namespace yetty::card
