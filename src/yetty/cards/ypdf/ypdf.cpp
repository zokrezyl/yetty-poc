#include "ypdf.h"
#include "pdf-content-parser.h"
#include <ytrace/ytrace.hpp>
#include <sstream>
#include <algorithm>
#include <cmath>

extern "C" {
#include <pdfio.h>
}

namespace yetty::card {

//=============================================================================
// Constructor / Destructor
//=============================================================================

YPdf::YPdf(const YettyContext& ctx,
           int32_t x, int32_t y,
           uint32_t widthCells, uint32_t heightCells,
           const std::string& args, const std::string& payload)
    : YDrawBase(ctx, x, y, widthCells, heightCells)
    , _argsStr(args)
    , _payloadStr(payload)
{}

YPdf::~YPdf() {
    if (_pdfFile) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
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

    yinfo("YPdf::create: SUCCESS, shaderGlyph={:#x} pages={} prims={} glyphs={}",
          card->shaderGlyph(), card->_pageCount,
          card->primitiveCount(), card->glyphCount());
    return Ok<CardPtr>(card);
}

//=============================================================================
// Initialization
//=============================================================================

Result<void> YPdf::init() {
    if (auto res = initBase(); !res) {
        return res;
    }

    parseArgs(_argsStr);

    if (auto res = loadPdf(); !res) {
        return res;
    }

    setInitParsing(true);
    if (auto res = renderPage(_currentPage); !res) {
        ywarn("YPdf::init: failed to render page {}: {}", _currentPage,
              error_msg(res));
    }
    setInitParsing(false);

    markDirty();
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
        if (token == "--page" || token == "-p") {
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
                setBgColor(static_cast<uint32_t>(
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
    if (_payloadStr.empty()) {
        return Err<void>("YPdf::loadPdf: empty payload");
    }

    // Payload is a file path
    _pdfFile = pdfioFileOpen(_payloadStr.c_str(), /*password_cb=*/nullptr,
                              /*password_data=*/nullptr,
                              pdfioErrorHandler, /*error_data=*/nullptr);

    if (!_pdfFile) {
        return Err<void>("YPdf::loadPdf: failed to open: " + _payloadStr);
    }

    _pageCount = static_cast<int>(pdfioFileGetNumPages(_pdfFile));
    if (_pageCount == 0) {
        pdfioFileClose(_pdfFile);
        _pdfFile = nullptr;
        return Err<void>("YPdf::loadPdf: PDF has no pages");
    }

    _currentPage = std::clamp(_currentPage, 0, _pageCount - 1);

    yinfo("YPdf::loadPdf: opened '{}' with {} pages",
          _payloadStr, _pageCount);
    return Ok();
}

//=============================================================================
// Page rendering
//=============================================================================

Result<void> YPdf::renderPage(int pageIndex) {
    if (!_pdfFile) {
        return Err<void>("YPdf::renderPage: no document");
    }

    pdfio_obj_t* pageObj = pdfioFileGetPage(
        _pdfFile, static_cast<size_t>(pageIndex));
    if (!pageObj) {
        return Err<void>("YPdf::renderPage: failed to get page " +
                          std::to_string(pageIndex));
    }

    // Get page MediaBox for scene bounds
    pdfio_dict_t* pageDict = pdfioObjGetDict(pageObj);
    pdfio_rect_t mediaBox = {};

    if (!pdfioDictGetRect(pageDict, "MediaBox", &mediaBox)) {
        // Default to US Letter
        mediaBox = {0.0, 0.0, 612.0, 792.0};
    }

    float pageW = static_cast<float>(mediaBox.x2 - mediaBox.x1);
    float pageH = static_cast<float>(mediaBox.y2 - mediaBox.y1);

    // Scene bounds: PDF Y-up → scene Y-down (flip at emission)
    setSceneBounds(
        static_cast<float>(mediaBox.x1), 0.0f,
        static_cast<float>(mediaBox.x2), pageH);

    // Parse content streams
    PdfContentParser parser;
    parser.setPageHeight(pageH);

    size_t numStreams = pdfioPageGetNumStreams(pageObj);
    yinfo("YPdf::renderPage: page {} has {} content streams",
          pageIndex, numStreams);

    for (size_t i = 0; i < numStreams; i++) {
        pdfio_stream_t* stream = pdfioPageOpenStream(
            pageObj, i, /*decode=*/true);
        if (!stream) continue;

        parser.parseStream(stream);
        pdfioStreamClose(stream);
    }

    // Emit text spans as MSDF glyphs
    const auto& spans = parser.textSpans();
    yinfo("YPdf::renderPage: extracted {} text spans", spans.size());

    for (const auto& span : spans) {
        // Flip Y: PDF origin is bottom-left, scene origin is top-left
        float sceneX = span.x;
        float sceneY = pageH - span.y;

        // Use the span's font size, or override if user specified
        float fontSize = (_fontSize > 0.0f) ? _fontSize : span.fontSize;

        addText(sceneX, sceneY, span.text, fontSize, _textColor, 0);
    }

    return Ok();
}

} // namespace yetty::card
