#include "ypdf.h"
#include "pdf-content-parser.h"
#include <yetty/base/event-loop.h>
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
    , _globalAllocator(ctx.globalAllocator)
    , _fontManager(ctx.fontManager)
{
    _shaderGlyph = SHADER_GLYPH;
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
          card->shaderGlyph(), card->_pageCount, card->_glyphs.size());
    return Ok<CardPtr>(card);
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YPdf::init() {
    ydebug("YPdf::init: START");

    // Create own atlas for PDF fonts
    auto atlasRes = MsdfAtlas::create(_globalAllocator);
    if (!atlasRes) {
        return Err<void>("YPdf::init: failed to create atlas");
    }
    _atlas = *atlasRes;
    ydebug("YPdf::init: atlas created, {}x{}",
           _atlas->getAtlasWidth(), _atlas->getAtlasHeight());

    // Load default serif font as fontId=0
    std::string defaultFontPath = resolveFontPath("serif");
    if (defaultFontPath.empty()) {
        return Err<void>("YPdf::init: fontconfig could not resolve 'serif'");
    }
    int fontId = addFont(defaultFontPath);
    if (fontId < 0) {
        return Err<void>("YPdf::init: failed to load default serif font");
    }
    yinfo("YPdf::init: loaded default serif font: {} -> fontId={}", defaultFontPath, fontId);

    // Allocate metadata slot
    auto metaResult = _cardMgr->allocateMetadata(sizeof(Metadata));
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
                _bgColor = static_cast<uint32_t>(
                    std::stoul(colorStr, nullptr, 16));
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
    if (!_pdfFile) {
        return Err<void>("YPdf::renderAllPages: no document");
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

    // Set explicit scene bounds
    _sceneMinX = 0.0f;
    _sceneMinY = 0.0f;
    _sceneMaxX = maxWidth;
    _sceneMaxY = totalHeight;
    _hasExplicitBounds = true;

    // Initial zoom: fit first page width (show one page, not entire document)
    if (!pages.empty() && totalHeight > 0.0f) {
        float firstPageH = pages[0].h;
        float cardAspect = static_cast<float>(_widthCells) / std::max(static_cast<float>(_heightCells), 1.0f);
        float viewableH = maxWidth / cardAspect;  // scene height visible at width-fit
        float fitZoom = totalHeight / std::max(viewableH, 1.0f);
        _viewZoom = std::max(fitZoom, 1.0f);
        // Pan to top of first page
        _viewPanY = -(totalHeight * 0.5f - firstPageH * 0.5f / _viewZoom);
    }

    yinfo("YPdf::renderAllPages: {} pages, scene={}x{} initialZoom={:.2f}",
          _pageCount, maxWidth, totalHeight, _viewZoom);

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
    if (!_atlas || text.empty()) return 0.0f;

    float fontBaseSize = _atlas->getFontSize();
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

        // Load glyph from atlas
        uint32_t glyphIndex = _atlas->loadGlyph(fontId, codepoint);

        const auto& metadata = _atlas->getGlyphMetadata();
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
        if (_glyphs.size() < 5) {
            ydebug("YPdf::placeGlyphs: cp='{}' fontId={} glyphIdx={} bearing=({:.1f},{:.1f}) size=({:.1f},{:.1f}) advance={:.1f}",
                   static_cast<char>(codepoint), fontId, glyphIndex,
                   glyph._bearingX, glyph._bearingY, glyph._sizeX, glyph._sizeY, glyph._advance);
        }

        // Place glyph in scene coordinates
        PdfGlyph g = {};
        g.x = cursorX + glyph._bearingX * scaleScene;
        g.y = posY - glyph._bearingY * scaleScene;
        g.width = glyph._sizeX * scaleScene;
        g.height = glyph._sizeY * scaleScene;
        g.glyphIndex = glyphIndex;
        g.color = _textColor;
        g.layer = 0;
        _glyphs.push_back(g);

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
           totalTextAdvance, cursorX, posX, cursorX - posX, _glyphs.size());

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
// Font registration
//=============================================================================

int YPdf::addFont(const std::string& ttfPath) {
    if (!_atlas || !_fontManager) return -1;

    auto stem = std::filesystem::path(ttfPath).stem().string();
    auto cacheDir = _fontManager->getCacheDir();
    auto cdbPath = cacheDir + "/" + stem + ".cdb";

    // Check local cache
    auto it = _fontIdCache.find(cdbPath);
    if (it != _fontIdCache.end()) return it->second;

    // Generate CDB if missing
    if (!std::filesystem::exists(cdbPath)) {
        auto provider = _fontManager->getCdbProvider();
        if (!provider) {
            yerror("YPdf::addFont: no CDB provider available");
            return -1;
        }

        MsdfCdbConfig cfg;
        cfg.ttfPath = ttfPath;
        cfg.cdbPath = cdbPath;

        if (auto res = provider->generate(cfg); !res) {
            yerror("YPdf::addFont: CDB generation failed for {}: {}",
                   ttfPath, res.error().message());
            return -1;
        }
    }

    int fontId = _atlas->openCdb(cdbPath);
    if (fontId >= 0) {
        _fontIdCache[cdbPath] = fontId;
    }
    return fontId;
}

//=============================================================================
// Scene bounds computation
//=============================================================================

void YPdf::computeSceneBounds() {
    if (_hasExplicitBounds) return;

    _sceneMinX = 1e10f;
    _sceneMinY = 1e10f;
    _sceneMaxX = -1e10f;
    _sceneMaxY = -1e10f;

    for (const auto& g : _glyphs) {
        _sceneMinX = std::min(_sceneMinX, g.x);
        _sceneMinY = std::min(_sceneMinY, g.y);
        _sceneMaxX = std::max(_sceneMaxX, g.x + g.width);
        _sceneMaxY = std::max(_sceneMaxY, g.y + g.height);
    }

    float padX = (_sceneMaxX - _sceneMinX) * 0.05f;
    float padY = (_sceneMaxY - _sceneMinY) * 0.05f;
    _sceneMinX -= padX;
    _sceneMinY -= padY;
    _sceneMaxX += padX;
    _sceneMaxY += padY;

    if (_sceneMinX >= _sceneMaxX) {
        _sceneMinX = 0; _sceneMaxX = 100;
    }
    if (_sceneMinY >= _sceneMaxY) {
        _sceneMinY = 0; _sceneMaxY = 100;
    }
}

//=============================================================================
// Grid building — glyphs only (no SDF primitives)
//=============================================================================

void YPdf::buildGrid() {
    if (!_derivedStorage.isValid() || _gridWidth == 0 || _gridHeight == 0) return;

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t gridTotalU32 = _gridWidth * _gridHeight * cellStride;
    uint32_t* grid = reinterpret_cast<uint32_t*>(_derivedStorage.data);

    std::memset(grid, 0, gridTotalU32 * sizeof(uint32_t));

    uint32_t glyphsPlaced = 0;
    uint32_t glyphsDropped = 0;
    uint32_t maxCellCount = 0;

    for (uint32_t gi = 0; gi < static_cast<uint32_t>(_glyphs.size()); gi++) {
        const auto& g = _glyphs[gi];

        uint32_t cellMinX = static_cast<uint32_t>(
            std::clamp((g.x - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMaxX = static_cast<uint32_t>(
            std::clamp((g.x + g.width - _sceneMinX) / _cellSize, 0.0f, float(_gridWidth - 1)));
        uint32_t cellMinY = static_cast<uint32_t>(
            std::clamp((g.y - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));
        uint32_t cellMaxY = static_cast<uint32_t>(
            std::clamp((g.y + g.height - _sceneMinY) / _cellSize, 0.0f, float(_gridHeight - 1)));

        bool placed = false;
        for (uint32_t cy = cellMinY; cy <= cellMaxY; cy++) {
            for (uint32_t cx = cellMinX; cx <= cellMaxX; cx++) {
                uint32_t cellIndex = cy * _gridWidth + cx;
                uint32_t cellOffset = cellIndex * cellStride;
                uint32_t count = grid[cellOffset];
                if (count < MAX_ENTRIES_PER_CELL) {
                    grid[cellOffset + 1 + count] = gi | GLYPH_BIT;
                    grid[cellOffset] = count + 1;
                    placed = true;
                    if (count + 1 > maxCellCount) maxCellCount = count + 1;
                }
            }
        }
        if (placed) glyphsPlaced++;
        else glyphsDropped++;
    }

    yinfo("YPdf::buildGrid: grid={}x{} cellSize={:.1f} glyphs={} placed={} "
          "DROPPED={} maxCellCount={}/{}",
          _gridWidth, _gridHeight, _cellSize, _glyphs.size(),
          glyphsPlaced, glyphsDropped, maxCellCount, MAX_ENTRIES_PER_CELL);
}

//=============================================================================
// Compute derived storage size (grid + glyphs)
//=============================================================================

uint32_t YPdf::computeDerivedSize() const {
    if (_glyphs.empty()) return 0;

    // Estimate grid dimensions from current scene bounds
    float sceneWidth = _sceneMaxX - _sceneMinX;
    float sceneHeight = _sceneMaxY - _sceneMinY;
    float sceneArea = sceneWidth * sceneHeight;

    uint32_t gridW = 0, gridH = 0;
    if (!_glyphs.empty() && sceneArea > 0) {
        float cs = _cellSize;
        if (cs <= 0.0f) {
            float avgGlyphH = 0.0f;
            for (const auto& g : _glyphs) avgGlyphH += g.height;
            avgGlyphH /= _glyphs.size();
            cs = avgGlyphH * 2.0f;
            float minCS = std::sqrt(sceneArea / 65536.0f);
            float maxCS = std::sqrt(sceneArea / 16.0f);
            cs = std::clamp(cs, minCS, maxCS);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cs)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cs)));
        const uint32_t MAX_GRID_DIM = 2048;
        gridW = std::min(gridW, MAX_GRID_DIM);
        gridH = std::min(gridH, MAX_GRID_DIM);
    }

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t gridBytes = gridW * gridH * cellStride * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(PdfGlyph));
    uint32_t glyphMetaBytes = static_cast<uint32_t>(_atlas->getGlyphMetadata().size() * sizeof(GlyphMetadataGPU));
    return gridBytes + glyphBytes + glyphMetaBytes;
}

//=============================================================================
// Rebuild and upload — compute grid, copy data to GPU buffer
//=============================================================================

Result<void> YPdf::rebuildAndUpload() {
    computeSceneBounds();

    float sceneWidth = _sceneMaxX - _sceneMinX;
    float sceneHeight = _sceneMaxY - _sceneMinY;

    uint32_t gridW = 0, gridH = 0;
    float cellSize = _cellSize;

    if (!_glyphs.empty()) {
        float sceneArea = sceneWidth * sceneHeight;
        if (cellSize <= 0.0f) {
            float avgGlyphH = 0.0f;
            for (const auto& g : _glyphs) {
                avgGlyphH += g.height;
            }
            avgGlyphH /= _glyphs.size();
            cellSize = avgGlyphH * 2.0f;
            float minCellSize = std::sqrt(sceneArea / 65536.0f);
            float maxCellSize = std::sqrt(sceneArea / 16.0f);
            cellSize = std::clamp(cellSize, minCellSize, maxCellSize);
        }
        gridW = std::max(1u, static_cast<uint32_t>(std::ceil(sceneWidth / cellSize)));
        gridH = std::max(1u, static_cast<uint32_t>(std::ceil(sceneHeight / cellSize)));
        const uint32_t MAX_GRID_DIM = 2048;
        if (gridW > MAX_GRID_DIM) { gridW = MAX_GRID_DIM; cellSize = sceneWidth / gridW; }
        if (gridH > MAX_GRID_DIM) { gridH = MAX_GRID_DIM; cellSize = std::max(cellSize, sceneHeight / gridH); }
    }

    uint32_t cellStride = 1 + MAX_ENTRIES_PER_CELL;
    uint32_t gridTotalU32 = gridW * gridH * cellStride;
    uint32_t gridBytes = gridTotalU32 * sizeof(uint32_t);
    uint32_t glyphBytes = static_cast<uint32_t>(_glyphs.size() * sizeof(PdfGlyph));
    const auto& glyphMeta = _atlas->getGlyphMetadata();
    uint32_t glyphMetaBytes = static_cast<uint32_t>(glyphMeta.size() * sizeof(GlyphMetadataGPU));
    uint32_t derivedTotalSize = gridBytes + glyphBytes + glyphMetaBytes;

    yinfo("YPdf::rebuild: grid={}x{} cellSize={:.1f} derivedTotal={} "
          "allocated={} glyphs={} glyphMeta={} zoom={:.2f}",
          gridW, gridH, cellSize, derivedTotalSize,
          _derivedStorage.isValid() ? _derivedStorage.size : 0,
          _glyphs.size(), glyphMeta.size(), _viewZoom);

    // Allocate or reallocate derived storage if needed
    if (derivedTotalSize > 0) {
        if (!_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "derived", derivedTotalSize);
            if (!storageResult) {
                return Err<void>("YPdf::rebuild: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        } else if (derivedTotalSize > _derivedStorage.size) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "derived", derivedTotalSize);
            if (!storageResult) {
                _derivedStorage = StorageHandle::invalid();
                return Err<void>("YPdf::rebuild: failed to reallocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
    }

    if (_derivedStorage.isValid() && derivedTotalSize > 0) {
        uint8_t* base = _derivedStorage.data;
        uint32_t offset = 0;

        // Zero entire buffer
        std::memset(base, 0, _derivedStorage.size);

        _gridWidth = gridW;
        _gridHeight = gridH;
        _cellSize = cellSize;
        _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
        offset += gridBytes;

        buildGrid();

        _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
        if (!_glyphs.empty()) {
            std::memcpy(base + offset, _glyphs.data(), glyphBytes);
        }
        offset += glyphBytes;

        // Upload glyph metadata (uvMin, uvMax, etc.) for shader
        _glyphMetaOffset = (_derivedStorage.offset + offset) / sizeof(float);
        if (!glyphMeta.empty()) {
            std::memcpy(base + offset, glyphMeta.data(), glyphMetaBytes);
        }

        _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _metadataDirty = true;
    return Ok();
}

//=============================================================================
// Metadata upload — 64-byte struct with f16 zoom, i16 pan encoding
//=============================================================================

Result<void> YPdf::uploadMetadata() {
    if (!_metaHandle.isValid()) {
        return Err<void>("YPdf::uploadMetadata: invalid metadata handle");
    }

    // Pack zoom as IEEE 754 half-float
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

    float contentW = _sceneMaxX - _sceneMinX;
    float contentH = _sceneMaxY - _sceneMinY;
    int16_t panXi16 = static_cast<int16_t>(std::clamp(
        _viewPanX / std::max(contentW, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));
    int16_t panYi16 = static_cast<int16_t>(std::clamp(
        _viewPanY / std::max(contentH, 1e-6f) * 16384.0f, -32768.0f, 32767.0f));

    // Get atlas position from texture manager
    AtlasPosition atlasPos = {0, 0};
    if (_atlasTextureHandle.isValid()) {
        atlasPos = _cardMgr->textureManager()->getAtlasPosition(_atlasTextureHandle);
    }
    uint32_t msdfAtlasW = _atlas->getAtlasWidth();
    uint32_t msdfAtlasH = _atlas->getAtlasHeight();

    Metadata meta = {};
    meta.atlasXW = (atlasPos.x & 0xFFFF) | ((msdfAtlasW & 0xFFFF) << 16);
    meta.atlasYH = (atlasPos.y & 0xFFFF) | ((msdfAtlasH & 0xFFFF) << 16);
    meta.gridOffset = _gridOffset;
    meta.gridWidth = _gridWidth;
    meta.gridHeight = _gridHeight;
    std::memcpy(&meta.cellSize, &_cellSize, sizeof(float));
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_glyphs.size());
    std::memcpy(&meta.sceneMinX, &_sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &_sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &_sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &_sceneMaxY, sizeof(float));
    meta.widthCells  = (_widthCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panXi16)) << 16);
    meta.heightCells = (_heightCells & 0xFFFF) | (static_cast<uint32_t>(static_cast<uint16_t>(panYi16)) << 16);
    meta.flags = (_flags & 0xFFFF) | (zoomBits << 16);
    meta.bgColor = _bgColor;

    yinfo("YPdf::uploadMetadata: atlas=({},{}) msdfSize={}x{} grid={}x{} cellSize={} "
          "scene=[{},{},{},{}] zoom={:.2f} pan=({:.1f},{:.1f}) size={}x{} bgColor={:#010x}",
          atlasPos.x, atlasPos.y, msdfAtlasW, msdfAtlasH, meta.gridWidth, meta.gridHeight, _cellSize,
          _sceneMinX, _sceneMinY, _sceneMaxX, _sceneMaxY,
          _viewZoom, _viewPanX, _viewPanY,
          _widthCells, _heightCells, meta.bgColor);

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
        return Err<void>("YPdf::uploadMetadata: write failed");
    }

    return Ok();
}

//=============================================================================
// Card lifecycle
//=============================================================================

void YPdf::declareBufferNeeds() {
    _derivedStorage = StorageHandle::invalid();

    if (!_glyphs.empty()) {
        _cardMgr->bufferManager()->reserve(computeDerivedSize());
    }
}

Result<void> YPdf::allocateBuffers() {
    if (!_glyphs.empty()) {
        uint32_t derivedSize = computeDerivedSize();
        if (derivedSize > 0 && !_derivedStorage.isValid()) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("YPdf::allocateBuffers: failed to allocate derived storage");
            }
            _derivedStorage = *storageResult;
        }
        _dirty = true;
        yinfo("YPdf::allocateBuffers: {} glyphs, derived {} bytes",
              _glyphs.size(), derivedSize);
    }
    return Ok();
}

Result<void> YPdf::allocateTextures() {
    if (!_atlas) {
        return Ok();
    }

    uint32_t atlasW = _atlas->getAtlasWidth();
    uint32_t atlasH = _atlas->getAtlasHeight();
    const auto& atlasData = _atlas->getAtlasData();

    if (atlasData.empty() || atlasW == 0 || atlasH == 0) {
        return Ok();
    }

    // Allocate handle
    auto allocResult = _cardMgr->textureManager()->allocate(atlasW, atlasH);
    if (!allocResult) {
        return Err<void>("YPdf::allocateTextures: failed to allocate texture handle", allocResult);
    }
    _atlasTextureHandle = *allocResult;

    yinfo("YPdf::allocateTextures: atlas {}x{} -> handle id={}", atlasW, atlasH, _atlasTextureHandle.id);

    return Ok();
}

Result<void> YPdf::writeTextures() {
    if (_atlasTextureHandle.isValid() && _atlas) {
        const auto& atlasData = _atlas->getAtlasData();
        if (!atlasData.empty()) {
            if (auto res = _cardMgr->textureManager()->write(_atlasTextureHandle, atlasData.data()); !res) {
                return Err<void>("YPdf::writeTextures: write failed", res);
            }
        }
    }
    return Ok();
}

Result<void> YPdf::render() {
    if (_dirty) {
        if (auto res = rebuildAndUpload(); !res) {
            return Err<void>("YPdf::render: rebuildAndUpload failed", res);
        }
        _dirty = false;
    }

    if (_metadataDirty) {
        if (auto res = uploadMetadata(); !res) {
            return Err<void>("YPdf::render: metadata upload failed", res);
        }
        _metadataDirty = false;
    }

    return Ok();
}

Result<void> YPdf::dispose() {
    deregisterFromEvents();

    if (_derivedStorage.isValid()) {
        _derivedStorage = StorageHandle::invalid();
    }

    if (_metaHandle.isValid() && _cardMgr) {
        if (auto res = _cardMgr->deallocateMetadata(_metaHandle); !res) {
            yerror("YPdf::dispose: deallocateMetadata failed: {}", error_msg(res));
        }
        _metaHandle = MetadataHandle::invalid();
    }

    return Ok();
}

void YPdf::suspend() {
    if (_derivedStorage.isValid()) {
        _derivedStorage = StorageHandle::invalid();
    }
}

//=============================================================================
// Event handling - zoom/pan
//=============================================================================

Result<void> YPdf::registerForEvents() {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Err<void>("YPdf::registerForEvents: no EventLoop instance", loopResult);
    }
    auto loop = *loopResult;
    auto self = sharedAs<base::EventListener>();

    if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) {
        return Err<void>("YPdf::registerForEvents: failed to register SetFocus", res);
    }
    if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) {
        return Err<void>("YPdf::registerForEvents: failed to register CardScroll", res);
    }
    yinfo("YPdf card {} registered for events (priority 1000)", id());
    return Ok();
}

Result<void> YPdf::deregisterFromEvents() {
    if (weak_from_this().expired()) {
        return Ok();
    }
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Ok();
    }
    auto loop = *loopResult;
    if (auto res = loop->deregisterListener(sharedAs<base::EventListener>()); !res) {
        ywarn("YPdf::deregisterFromEvents: {}", error_msg(res));
    }
    return Ok();
}

Result<bool> YPdf::onEvent(const base::Event& event) {
    if (event.type == base::Event::Type::SetFocus) {
        if (event.setFocus.objectId == id()) {
            if (!_focused) {
                _focused = true;
                ydebug("YPdf::onEvent: focused (id={})", id());
            }
            return Ok(true);
        } else if (_focused) {
            _focused = false;
            ydebug("YPdf::onEvent: unfocused (id={})", id());
        }
        return Ok(false);
    }

    if (event.type == base::Event::Type::CardScroll &&
        event.cardScroll.targetId == id()) {

        float sceneW = _sceneMaxX - _sceneMinX;
        float sceneH = _sceneMaxY - _sceneMinY;

        if (event.cardScroll.mods & GLFW_MOD_CONTROL) {
            float zoomFactor = std::exp(event.cardScroll.dy * 0.1f);
            float newZoom = std::clamp(_viewZoom * zoomFactor, 0.1f, 5000.0f);
            if (newZoom != _viewZoom) {
                _viewZoom = newZoom;
                _metadataDirty = true;
            }
            return Ok(true);
        } else if (event.cardScroll.mods & GLFW_MOD_SHIFT) {
            float dx = event.cardScroll.dy * 0.05f * sceneW / _viewZoom;
            _viewPanX += dx;
            _metadataDirty = true;
            return Ok(true);
        } else {
            float dy = event.cardScroll.dy * 0.05f * sceneH / _viewZoom;
            _viewPanY += dy;
            _metadataDirty = true;
            return Ok(true);
        }
    }

    return Ok(false);
}

} // namespace yetty::card
