#include "ypdf.h"
#include "pdf-content-parser.h"
#include <yetty/base/event-loop.h>
#include <yetty/msdf-glyph-data.h>
#include <yetty/card-texture-manager.h>
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef YETTY_USE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

extern "C" {
#include <pdfio.h>
}

namespace {
constexpr int GLFW_MOD_SHIFT   = 0x0001;
constexpr int GLFW_MOD_CONTROL = 0x0002;
constexpr int GLFW_KEY_C       = 67;
constexpr float PAGE_MARGIN = 20.0f;  // Points between pages

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
// Constructor / Destructor
//=============================================================================

YPdf::YPdf(const YettyContext& ctx,
           int32_t x, int32_t y,
           uint32_t widthCells, uint32_t heightCells,
           const std::string& args, const std::string& payload)
    : Card(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _payloadStr(payload)
    , _fontManager(ctx.fontManager)
{
    _shaderGlyph = SHADER_GLYPH;

    auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
    if (builderRes) {
        _builder = *builderRes;
        _builder->addFlags(YDrawBuilder::FLAG_UNIFORM_SCALE | YDrawBuilder::FLAG_CUSTOM_ATLAS);
        _builder->setBgColor(0xFFFFFFFF);  // White background for PDF
    } else {
        yerror("YPdf: failed to create YDrawBuilder");
    }
}

YPdf::~YPdf() {
    dispose();
    if (_pdfFile) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
    }
    // Clean up temp file if created
    if (!_tempPdfPath.empty() && std::filesystem::exists(_tempPdfPath)) {
        std::error_code ec;
        std::filesystem::remove(_tempPdfPath, ec);
    }
}

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YPdf::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    yinfo("YPdf::create: pos=({},{}) size={}x{} payload_len={}",
          x, y, widthCells, heightCells, payload.size());

    auto card = std::make_shared<YPdf>(ctx, x, y, widthCells, heightCells,
                                       args, payload);

    if (auto res = card->init(); !res) {
        yerror("YPdf::create: init FAILED: {}", error_msg(res));
        return Err<CardPtr>("YPdf::create: init failed");
    }

    yinfo("YPdf::create: SUCCESS, shaderGlyph={:#x} pages={} glyphs={}",
          card->shaderGlyph(), card->_pageCount,
          card->_builder ? card->_builder->glyphCount() : 0);
    return Ok<CardPtr>(card);
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YPdf::init() {
    ydebug("YPdf::init: START");

    if (!_builder) {
        return Err<void>("YPdf::init: no builder");
    }

    // Load default serif font via builder
    std::string defaultFontPath = resolveFontPath("serif");
    if (defaultFontPath.empty()) {
        return Err<void>("YPdf::init: fontconfig could not resolve 'serif'");
    }
    auto fontRes = _builder->addFont(defaultFontPath);
    if (!fontRes) {
        return Err<void>("YPdf::init: failed to load default serif font");
    }
    yinfo("YPdf::init: loaded default serif font: {} -> fontId={}", defaultFontPath, *fontRes);

    // Allocate metadata slot
    auto metaResult = _cardMgr->allocateMetadata(sizeof(YDrawMetadata));
    if (!metaResult) {
        return Err<void>("YPdf::init: failed to allocate metadata");
    }
    _metaHandle = *metaResult;
    _dirty = true;
    _metadataDirty = true;

    if (auto res = registerForEvents(); !res) {
        ywarn("YPdf::init: event registration failed: {}", error_msg(res));
    }

    parseArgs(_argsStr);

    if (auto res = loadPdf(); !res) {
        return res;
    }

    // Set up raw-font cache directory for extracted PDF fonts
    if (_fontManager) {
        auto parentDir = std::filesystem::path(_fontManager->getCacheDir()).parent_path();
        _rawFontCacheDir = (parentDir / "raw-fonts").string();
        std::filesystem::create_directories(_rawFontCacheDir);
    }

    if (auto res = renderAllPages(); !res) {
        ywarn("YPdf::init: failed to render pages: {}", error_msg(res));
    }

    _dirty = true;
    _metadataDirty = true;
    return Ok();
}

//=============================================================================
// Args parsing
//=============================================================================

void YPdf::parseArgs(const std::string& args) {
    if (args.empty()) return;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--input" || token == "-i") {
            std::string val;
            if (iss >> val) {
                _inputSource = val;
            }
        } else if (token == "--page" || token == "-p") {
            int val;
            if (iss >> val) {
                _currentPage = std::max(0, val);
            }
        } else if (token == "--font-size") {
            float val;
            if (iss >> val) {
                _fontSize = val;
            }
        } else if (token == "--color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.size() > 2 &&
                    (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")) {
                    colorStr = colorStr.substr(2);
                }
                _textColor = static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16));
            }
        } else if (token == "--bg-color") {
            std::string colorStr;
            if (iss >> colorStr) {
                if (colorStr.size() > 2 &&
                    (colorStr.substr(0, 2) == "0x" || colorStr.substr(0, 2) == "0X")) {
                    colorStr = colorStr.substr(2);
                }
                _builder->setBgColor(static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16)));
            }
        }
    }
}

//=============================================================================
// PDF loading
//=============================================================================

static bool pdfioErrorHandler(pdfio_file_t* /*pdf*/, const char* message,
                               void* /*data*/) {
    ywarn("pdfio: {}", message);
    return true;  // Continue on error
}

Result<void> YPdf::loadPdf() {
    std::string pdfPath;

    // Determine PDF source based on -i/--input argument
    if (_inputSource == "-" || _inputSource.empty()) {
        // Read from payload (base64-decoded data passed via OSC)
        if (_payloadStr.empty()) {
            return Err<void>("YPdf::loadPdf: empty payload (use -i - to read from payload)");
        }

        // pdfio requires a file path, so write payload to a temp file
        _tempPdfPath = std::filesystem::temp_directory_path() /
                       ("ypdf_" + std::to_string(id()) + ".pdf");
        std::ofstream out(_tempPdfPath, std::ios::binary);
        if (!out) {
            return Err<void>("YPdf::loadPdf: failed to create temp file: " + _tempPdfPath.string());
        }
        out.write(_payloadStr.data(), static_cast<std::streamsize>(_payloadStr.size()));
        out.close();

        pdfPath = _tempPdfPath.string();
        yinfo("YPdf::loadPdf: wrote {} bytes to temp file '{}'", _payloadStr.size(), pdfPath);
    } else {
        // Read from file path specified via -i <path>
        if (!std::filesystem::exists(_inputSource)) {
            return Err<void>("YPdf::loadPdf: file not found: " + _inputSource);
        }
        pdfPath = _inputSource;
        yinfo("YPdf::loadPdf: loading from file '{}'", pdfPath);
    }

    _pdfFile = pdfioFileOpen(pdfPath.c_str(), /*password_cb=*/nullptr,
                              /*password_data=*/nullptr,
                              pdfioErrorHandler, /*error_data=*/nullptr);

    if (!_pdfFile) {
        return Err<void>("YPdf::loadPdf: failed to open: " + pdfPath);
    }

    _pageCount = static_cast<int>(pdfioFileGetNumPages(_pdfFile));
    if (_pageCount == 0) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
        return Err<void>("YPdf::loadPdf: PDF has no pages");
    }

    _currentPage = std::clamp(_currentPage, 0, _pageCount - 1);

    yinfo("YPdf::loadPdf: opened '{}' with {} pages", pdfPath, _pageCount);
    return Ok();
}

//=============================================================================
// Page rendering
//=============================================================================

Result<void> YPdf::renderAllPages() {
    if (!_pdfFile || !_builder) {
        return Err<void>("YPdf::renderAllPages: no document or builder");
    }

    struct PageInfo {
        float x, y, w, h;
    };
    std::vector<PageInfo> pages;
    pages.reserve(static_cast<size_t>(_pageCount));

    float maxWidth = 0.0f;
    float totalHeight = 0.0f;

    for (int i = 0; i < _pageCount; i++) {
        pdfio_obj_t* pageObj = pdfioFileGetPage(
            _pdfFile, static_cast<size_t>(i));
        if (!pageObj) continue;

        pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
        pdfio_rect_t mediaBox = {};
        if (!pdfioDictGetRect(pageDict, "MediaBox", &mediaBox)) {
            mediaBox = {0.0, 0.0, 612.0, 792.0};  // Default US Letter
        }

        float pw = static_cast<float>(mediaBox.x2 - mediaBox.x1);
        float ph = static_cast<float>(mediaBox.y2 - mediaBox.y1);

        pages.push_back({static_cast<float>(mediaBox.x1),
                         static_cast<float>(mediaBox.y1), pw, ph});
        maxWidth = std::max(maxWidth, pw);
        if (i > 0) totalHeight += PAGE_MARGIN;
        totalHeight += ph;
    }

    if (pages.empty()) {
        return Err<void>("YPdf::renderAllPages: no valid pages");
    }

    // Set explicit scene bounds on builder
    _builder->setSceneBounds(0.0f, 0.0f, maxWidth, totalHeight);

    // Initial zoom: fit first page via _fitPageHeight mechanism
    if (!pages.empty() && totalHeight > 0.0f) {
        float firstPageH = pages[0].h;
        float cardAspect = static_cast<float>(_widthCells) / std::max(static_cast<float>(_heightCells), 1.0f);
        float viewableH = maxWidth / cardAspect;
        _fitPageHeight = viewableH;
    }

    yinfo("YPdf::renderAllPages: {} pages, scene={}x{}", _pageCount, maxWidth, totalHeight);

    // Render each page at its vertical offset
    float yOffset = 0.0f;
    for (int i = 0; i < _pageCount; i++) {
        if (static_cast<size_t>(i) >= pages.size()) break;

        if (auto res = renderPage(i, yOffset); !res) {
            ywarn("YPdf::renderAllPages: page {} failed: {}", i,
                  error_msg(res));
        }

        yOffset += pages[static_cast<size_t>(i)].h + PAGE_MARGIN;
    }

    return Ok();
}

Result<void> YPdf::renderPage(int pageIndex, float yOffset) {
    pdfio_obj_t* pageObj = pdfioFileGetPage(
        _pdfFile, static_cast<size_t>(pageIndex));
    if (!pageObj) {
        return Err<void>("YPdf::renderPage: failed to get page " +
                          std::to_string(pageIndex));
    }

    pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
    pdfio_rect_t mediaBox = {};
    if (!pdfioDictGetRect(pageDict, "MediaBox", &mediaBox)) {
        mediaBox = {0.0, 0.0, 612.0, 792.0};
    }

    float pageH = static_cast<float>(mediaBox.y2 - mediaBox.y1);

    // Extract embedded fonts from this page's resources
    extractFonts(pageObj);

    // Parse content streams with TextEmitCallback for CDB-based glyph placement
    PdfContentParser parser;
    parser.setPageHeight(pageH);

    // Set the callback that uses CDB glyph advances for correct spacing
    parser.setTextEmitCallback(
        [this, yOffset, pageH](const std::string& text,
                                float posX, float posY,
                                float effectiveSize,
                                const PdfTextState& textState) -> float {
            // Flip Y: PDF origin is bottom-left, scene origin is top-left
            float sceneX = posX;
            float sceneY = yOffset + (pageH - posY);

            float fontSize = (_fontSize > 0.0f) ? _fontSize : effectiveSize;

            return placeGlyphs(text, sceneX, sceneY, fontSize, textState);
        });

    size_t numStreams = pdfioPageGetNumStreams(pageObj);
    yinfo("YPdf::renderPage: page {} has {} content streams, yOffset={}",
          pageIndex, numStreams, yOffset);

    for (size_t i = 0; i < numStreams; i++) {
        pdfio_stream_t* stream = pdfioPageOpenStream(
            pageObj, i, /*decode=*/true);
        if (!stream) continue;

        parser.parseStream(stream);
        pdfioStreamClose(stream);
    }

    return Ok();
}

//=============================================================================
// Glyph placement (CDB-based) — the core method
//
// Places glyphs using real CDB advance/bearing/size data.
// Returns total advance in text-matrix units for parser text matrix update.
//=============================================================================

float YPdf::placeGlyphs(const std::string& text,
                         float posX, float posY,
                         float effectiveSize,
                         const PdfTextState& textState) {
    if (!_builder || !_builder->hasCustomAtlas() || text.empty()) return 0.0f;

    auto atlas = _builder->customAtlas();
    float fontBaseSize = atlas->getFontSize();
    if (fontBaseSize <= 0.0f) fontBaseSize = 1.0f;

    // Two scales:
    // scaleScene: for visual glyph placement in scene coordinates
    // scaleText: for text matrix advance return value
    float scaleScene = effectiveSize / fontBaseSize;
    float scaleText = textState.fontSize / fontBaseSize;

    float hScale = textState.horizontalScaling / 100.0f;

    // Lookup fontId from PDF font name
    int fontId = 0;
    auto it = _pdfFontMap.find(textState.fontName);
    if (it != _pdfFontMap.end()) {
        fontId = it->second;
    }

    ydebug("placeGlyphs: text='{}' len={} pos=({:.1f},{:.1f}) effSize={:.1f} "
           "fontSize={:.1f} fontBase={:.1f} scaleScene={:.4f} scaleText={:.4f} "
           "hScale={:.2f} fontId={} font='{}'",
           text.substr(0, 20), text.size(), posX, posY, effectiveSize,
           textState.fontSize, fontBaseSize, scaleScene, scaleText,
           hScale, fontId, textState.fontName);

    float cursorX = posX;
    float totalTextAdvance = 0.0f;

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(text.data());
    const uint8_t* end = ptr + text.size();

    while (ptr < end) {
        // Decode UTF-8 codepoint
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

        // Load glyph from custom atlas
        uint32_t glyphIndex = atlas->loadGlyph(fontId, codepoint);

        const auto& metadata = atlas->getGlyphMetadata();
        if (glyphIndex >= metadata.size()) {
            ydebug("YPdf::placeGlyphs: MISSING glyph cp={} fontId={} glyphIdx={} metaSize={}",
                   codepoint, fontId, glyphIndex, metadata.size());
            // Fallback advance for missing glyphs
            float fallbackAdv = effectiveSize * 0.5f;
            cursorX += fallbackAdv * hScale;
            totalTextAdvance += (textState.fontSize * 0.5f) * hScale;
            continue;
        }

        const auto& glyph = metadata[glyphIndex];

        // Place glyph via YDrawGlyph into builder
        YDrawGlyph g = {};
        g.x = cursorX + glyph._bearingX * scaleScene;
        g.y = posY - glyph._bearingY * scaleScene;
        g.setSize(glyph._sizeX * scaleScene, glyph._sizeY * scaleScene);
        g.setGlyphLayerFlags(static_cast<uint16_t>(glyphIndex), 0,
                             YDrawBuilder::GLYPH_FLAG_CUSTOM_ATLAS);
        g.color = _textColor;
        _builder->glyphsMut().push_back(g);

        // Advance cursor in scene coords
        float sceneAdvance = glyph._advance * scaleScene + textState.charSpacing;
        // Advance for text matrix return
        float textAdvance = glyph._advance * scaleText + textState.charSpacing;

        // Word spacing for space character
        if (codepoint == 0x20) {
            sceneAdvance += textState.wordSpacing;
            textAdvance += textState.wordSpacing;
        }

        cursorX += sceneAdvance * hScale;
        totalTextAdvance += textAdvance * hScale;
    }

    ydebug("placeGlyphs: result totalAdvance={:.2f} cursorX={:.1f} (startX={:.1f} delta={:.1f}) "
           "glyphsNow={}",
           totalTextAdvance, cursorX, posX, cursorX - posX, _builder->glyphCount());

    return totalTextAdvance;
}

//=============================================================================
// Font extraction from PDF resources
//=============================================================================

void YPdf::extractFonts(pdfio_obj_t* pageObj) {
    if (_rawFontCacheDir.empty() || !pageObj) return;

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

    auto pdfStem = std::filesystem::path(_payloadStr).stem().string();

    size_t numFonts = pdfioDictGetNumPairs(fontDict);
    yinfo("YPdf::extractFonts: found {} font entries in page resources", numFonts);
    for (size_t i = 0; i < numFonts; i++) {
        const char* fontTag = pdfioDictGetKey(fontDict, i);
        if (!fontTag) continue;

        std::string tag(fontTag);
        if (_pdfFontMap.count(tag) || _pdfFontMap.count("/" + tag)) continue;

        pdfio_obj_t* fontObj = pdfioDictGetObj(fontDict, fontTag);
        if (!fontObj) continue;

        pdfio_dict_t* fontObjDict = pdfioObjGetDict(fontObj);
        if (!fontObjDict) continue;

        pdfio_obj_t* fontDescObj = pdfioDictGetObj(fontObjDict, "FontDescriptor");
        if (!fontDescObj) {
            ydebug("YPdf::extractFonts: font {} has no FontDescriptor", tag);
            continue;
        }

        pdfio_dict_t* fontDescDict = pdfioObjGetDict(fontDescObj);
        if (!fontDescDict) continue;

        pdfio_obj_t* fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile2");
        if (!fontFileObj) {
            fontFileObj = pdfioDictGetObj(fontDescDict, "FontFile3");
        }
        if (!fontFileObj) {
            ydebug("YPdf::extractFonts: font {} has no embedded font file", tag);
            continue;
        }

        std::string ttfPath = _rawFontCacheDir + "/" + pdfStem + "-" + tag + ".ttf";

        if (!std::filesystem::exists(ttfPath)) {
            pdfio_stream_t* stream = pdfioObjOpenStream(fontFileObj, /*decode=*/true);
            if (!stream) {
                ywarn("YPdf::extractFonts: failed to open stream for font {}", tag);
                continue;
            }

            std::vector<uint8_t> fontData;
            uint8_t buf[8192];
            ssize_t bytesRead;
            while ((bytesRead = pdfioStreamRead(stream, buf, sizeof(buf))) > 0) {
                fontData.insert(fontData.end(), buf, buf + bytesRead);
            }
            pdfioStreamClose(stream);

            if (fontData.empty()) {
                ywarn("YPdf::extractFonts: empty font stream for {}", tag);
                continue;
            }

            std::ofstream out(ttfPath, std::ios::binary);
            if (!out) {
                ywarn("YPdf::extractFonts: failed to write {}", ttfPath);
                continue;
            }
            out.write(reinterpret_cast<const char*>(fontData.data()),
                      static_cast<std::streamsize>(fontData.size()));
            out.close();

            yinfo("YPdf::extractFonts: extracted font {} -> {} ({} bytes)",
                  tag, ttfPath, fontData.size());
        }

        int fontId = addFont(ttfPath);
        if (fontId >= 0) {
            _pdfFontMap[tag] = fontId;
            _pdfFontMap["/" + tag] = fontId;
            yinfo("YPdf::extractFonts: registered font {} as fontId={}", tag, fontId);
        } else {
            ywarn("YPdf::extractFonts: addFont failed for {}", ttfPath);
        }
    }
}

//=============================================================================
// Font registration — delegates to builder
//=============================================================================

int YPdf::addFont(const std::string& ttfPath) {
    if (!_builder) return -1;

    auto result = _builder->addFont(ttfPath);
    if (!result) {
        yerror("YPdf::addFont: failed for {}", ttfPath);
        return -1;
    }
    return *result;
}

//=============================================================================
// GPU helpers
//=============================================================================

uint32_t YPdf::computeDerivedSize() const {
    if (!_builder) return 0;
    const auto& gridData = _builder->gridStaging();
    uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(
        _builder->glyphs().size() * sizeof(YDrawGlyph));
    uint32_t total = gridBytes + glyphBytes;
    if (_builder->hasCustomAtlas()) {
        uint32_t atlasHeaderBytes = 4 * sizeof(uint32_t);
        uint32_t glyphMetaBytes = static_cast<uint32_t>(
            _builder->customAtlas()->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
        total += atlasHeaderBytes + glyphMetaBytes;
    }
    return total;
}

void YPdf::uploadGlyphData() {
    if (!_derivedStorage.isValid() || !_builder || _builder->glyphs().empty()) return;
    uint32_t localOff = _glyphOffset * sizeof(float) - _derivedStorage.offset;
    auto& glyphs = _builder->glyphsMut();
    std::memcpy(_derivedStorage.data + localOff, glyphs.data(),
                glyphs.size() * sizeof(YDrawGlyph));
    _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
}

void YPdf::cardPixelToScene(float cardX, float cardY, float& sceneX, float& sceneY) {
    float cardPixelW = static_cast<float>(_widthCells * _cellWidth);
    float cardPixelH = static_cast<float>(_heightCells * _cellHeight);
    if (cardPixelW <= 0 || cardPixelH <= 0) { sceneX = sceneY = 0; return; }

    float contentW = _builder->sceneMaxX() - _builder->sceneMinX();
    float contentH = _builder->sceneMaxY() - _builder->sceneMinY();
    float centerX = _builder->sceneMinX() + contentW * 0.5f;
    float centerY = _builder->sceneMinY() + contentH * 0.5f;
    float viewHalfW = contentW * 0.5f / _viewZoom;
    float viewHalfH = contentH * 0.5f / _viewZoom;
    float viewMinX = centerX - viewHalfW + _viewPanX;
    float viewMinY = centerY - viewHalfH + _viewPanY;
    float viewW = contentW / _viewZoom;
    float viewH = contentH / _viewZoom;

    float cardAspect = cardPixelW / std::max(cardPixelH, 1.0f);
    float viewAspect = viewW / std::max(viewH, 1e-6f);
    float uvX = cardX / cardPixelW;
    float uvY = cardY / cardPixelH;

    if (viewAspect < cardAspect) {
        float visibleW = viewH * cardAspect;
        float offsetX = (visibleW - viewW) * 0.5f;
        sceneX = viewMinX - offsetX + uvX * visibleW;
        sceneY = viewMinY + uvY * viewH;
    } else {
        float visibleH = viewW / cardAspect;
        float offsetY = (visibleH - viewH) * 0.5f;
        sceneX = viewMinX + uvX * viewW;
        sceneY = viewMinY - offsetY + uvY * visibleH;
    }
}

Result<void> YPdf::uploadMetadata() {
    if (!_metaHandle.isValid() || !_builder) {
        return Err<void>("YPdf::uploadMetadata: invalid handle or builder");
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
    meta.primitiveOffset = 0;
    meta.primitiveCount = 0;
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
        return Err<void>("YPdf::uploadMetadata: write failed");
    }
    return Ok();
}

//=============================================================================
// Card lifecycle — follows YDrawImpl pattern
//=============================================================================

void YPdf::declareBufferNeeds() {
    _derivedStorage = StorageHandle::invalid();
    if (!_builder) return;

    const auto& glyphs = _builder->glyphs();
    if (glyphs.empty()) {
        _builder->clearGridStaging();
        return;
    }

    // Calculate grid if needed
    if (_builder->gridStaging().empty()) {
        _builder->calculate();

        // Auto-compute zoom/pan to fit one page height after scene bounds are known
        if (_fitPageHeight > 0.0f) {
            float sceneMinY = _builder->sceneMinY();
            float sceneMaxY = _builder->sceneMaxY();
            float sceneH = sceneMaxY - sceneMinY;
            if (sceneH > _fitPageHeight) {
                _viewZoom = sceneH / _fitPageHeight;
                float centerY = (sceneMinY + sceneMaxY) * 0.5f;
                _viewPanY = _fitPageHeight * 0.5f - centerY;
            }
            _fitPageHeight = 0.0f;  // only apply once
        }
    }

    // Reserve derived size (grid + glyphs + optional atlas)
    uint32_t derivedSize = computeDerivedSize();
    if (derivedSize > 0) {
        _cardMgr->bufferManager()->reserve(derivedSize);
    }
}

Result<void> YPdf::allocateBuffers() {
    if (!_builder) return Ok();

    const auto& gridStaging = _builder->gridStaging();
    const auto& glyphs = _builder->glyphs();

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

    if (derivedSize > 0) {
        auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
            metadataSlotIndex(), "derived", derivedSize);
        if (!storageResult) {
            return Err<void>("YPdf::allocateBuffers: derived alloc failed");
        }
        _derivedStorage = *storageResult;

        uint8_t* base = _derivedStorage.data;
        uint32_t offset = 0;

        // Copy grid
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

        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _gridWidth = _builder->gridWidth();
    _gridHeight = _builder->gridHeight();
    _metadataDirty = true;
    _dirty = true;

    return Ok();
}

Result<void> YPdf::allocateTextures() {
    if (!_builder || !_builder->hasCustomAtlas()) return Ok();

    auto atlas = _builder->customAtlas();
    uint32_t atlasW = atlas->getAtlasWidth();
    uint32_t atlasH = atlas->getAtlasHeight();
    const auto& atlasData = atlas->getAtlasData();

    if (atlasData.empty() || atlasW == 0 || atlasH == 0) return Ok();

    auto allocResult = _cardMgr->textureManager()->allocate(atlasW, atlasH);
    if (!allocResult) {
        return Err<void>("YPdf::allocateTextures: failed", allocResult);
    }
    _atlasTextureHandle = *allocResult;
    _dirty = true;
    return Ok();
}

Result<void> YPdf::writeTextures() {
    if (_atlasTextureHandle.isValid() && _builder && _builder->hasCustomAtlas()) {
        const auto& atlasData = _builder->customAtlas()->getAtlasData();
        if (!atlasData.empty()) {
            if (auto res = _cardMgr->textureManager()->write(
                    _atlasTextureHandle, atlasData.data()); !res) {
                return Err<void>("YPdf::writeTextures: write failed", res);
            }
        }
    }
    return Ok();
}

Result<void> YPdf::render() {
    if (!_builder) return Ok();

    if (_dirty) {
        uint32_t derivedSize = computeDerivedSize();

        if (derivedSize > 0) {
            if (!_derivedStorage.isValid()) {
                return Err<void>("YPdf::render: derived storage not allocated");
            }

            uint8_t* base = _derivedStorage.data;
            std::memset(base, 0, _derivedStorage.size);
            uint32_t offset = 0;

            const auto& gridData = _builder->gridStaging();
            uint32_t gridBytes = static_cast<uint32_t>(gridData.size()) * sizeof(uint32_t);
            _gridWidth = _builder->gridWidth();
            _gridHeight = _builder->gridHeight();
            _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
            std::memcpy(base + offset, gridData.data(), gridBytes);
            offset += gridBytes;

            uint32_t glyphBytes = static_cast<uint32_t>(
                _builder->glyphs().size() * sizeof(YDrawGlyph));
            _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
            const auto& glyphs = _builder->glyphs();
            if (!glyphs.empty()) {
                std::memcpy(base + offset, glyphs.data(), glyphBytes);
            }
            offset += glyphBytes;

            if (_builder->hasCustomAtlas() && _atlasTextureHandle.isValid()) {
                auto atlas = _builder->customAtlas();
                auto atlasPos = _cardMgr->textureManager()->getAtlasPosition(
                    _atlasTextureHandle);
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

        _metadataDirty = true;
        _dirty = false;
    }

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) return res;
        _metadataDirty = false;
    }

    return Ok();
}

Result<void> YPdf::dispose() {
    deregisterFromEvents();
    _derivedStorage = StorageHandle::invalid();

    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YPdf::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

void YPdf::suspend() {
    _derivedStorage = StorageHandle::invalid();
}

//=============================================================================
// Event handling - zoom/pan + text selection
//=============================================================================

Result<void> YPdf::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return Err<void>("no EventLoop", loopResult);
    auto loop = *loopResult;
    auto self = sharedAs<base::EventListener>();
    if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseDown, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseUp, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseMove, self, 1000); !res) return res;
    if (auto res = loop->registerListener(base::Event::Type::CardKeyDown, self, 1000); !res) return res;
    return Ok();
}

Result<void> YPdf::deregisterFromEvents() {
    if (weak_from_this().expired()) return Ok();
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return Ok();
    (*loopResult)->deregisterListener(sharedAs<base::EventListener>());
    return Ok();
}

Result<bool> YPdf::onEvent(const base::Event& event) {
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
            float zoomDelta = event.cardScroll.dy * 0.1f * _viewZoom;
            float newZoom = std::clamp(_viewZoom + zoomDelta, 0.1f, 1000.0f);
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

    // Mouse selection
    if (event.type == base::Event::Type::CardMouseDown &&
        event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
        if (!_builder || _builder->glyphCount() == 0) return Ok(false);
        if (!_sortedOrderBuilt) {
            _builder->buildGlyphSortedOrder();
            _sortedOrderBuilt = true;
        }
        float sx, sy;
        cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
        int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
        _selStartSorted = idx;
        _selEndSorted = idx;
        _selecting = true;
        _builder->setSelectionRange(_selStartSorted, _selEndSorted);
        uploadGlyphData();
        return Ok(true);
    }

    if (event.type == base::Event::Type::CardMouseMove &&
        event.cardMouse.targetId == id() && _selecting) {
        float sx, sy;
        cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
        int32_t idx = _builder->findNearestGlyphSorted(sx, sy);
        if (idx != _selEndSorted) {
            _selEndSorted = idx;
            _builder->setSelectionRange(_selStartSorted, _selEndSorted);
            uploadGlyphData();
        }
        return Ok(true);
    }

    if (event.type == base::Event::Type::CardMouseUp &&
        event.cardMouse.targetId == id() && event.cardMouse.button == 0) {
        if (_selecting) {
            float sx, sy;
            cardPixelToScene(event.cardMouse.x, event.cardMouse.y, sx, sy);
            _selEndSorted = _builder->findNearestGlyphSorted(sx, sy);
            _selecting = false;
            _builder->setSelectionRange(_selStartSorted, _selEndSorted);
            uploadGlyphData();
        }
        return Ok(true);
    }

    // Ctrl+C: copy selected text
    if (event.type == base::Event::Type::CardKeyDown &&
        event.cardKey.targetId == id() &&
        event.cardKey.key == GLFW_KEY_C &&
        (event.cardKey.mods & GLFW_MOD_CONTROL)) {
        std::string text = _builder->getSelectedText();
        if (!text.empty()) {
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::copyEvent(
                std::make_shared<std::string>(text)));
            yinfo("YPdf: copied {} bytes to clipboard", text.size());
        }
        return Ok(true);
    }

    return Ok(false);
}

} // namespace yetty::card
