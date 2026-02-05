#pragma once

#include "../../ydraw/ydraw-base.h"
#include <string>
#include <cstdint>

// Forward declarations for pdfio (pdfio uses _pdfio_*_s naming)
struct _pdfio_file_s;
typedef struct _pdfio_file_s pdfio_file_t;

namespace yetty::card {

//=============================================================================
// YPdf - PDF rendering card using pdfio + MSDF text
//
// Parses PDF content streams to extract text positioning and renders
// text as MSDF glyphs via YDrawBase. Uses pdfio (lightweight C library)
// for PDF parsing instead of PDFium.
//
// Subclasses YDrawBase to reuse:
//   - Spatial hash grid with O(1) lookup
//   - MSDF text rendering via addText()
//   - SDF primitives via addPrimitive()
//   - Zoom/pan support
//=============================================================================
class YPdf : public yetty::YDrawBase {
public:
    using Ptr = std::shared_ptr<YPdf>;

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    ~YPdf() override;

    const char* typeName() const override { return "ypdf"; }

    // Constructor is public for make_shared, use create() factory instead
    YPdf(const YettyContext& ctx,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells,
         const std::string& args, const std::string& payload);

private:
    Result<void> init();
    void parseArgs(const std::string& args);
    Result<void> loadPdf();
    Result<void> renderPage(int pageIndex);

    std::string _argsStr;
    std::string _payloadStr;

    // pdfio state
    pdfio_file_t* _pdfFile = nullptr;

    // Page state
    int _currentPage = 0;
    int _pageCount = 0;

    // Rendering parameters
    float _fontSize = 0.0f;  // 0 = use PDF's font sizes
    uint32_t _textColor = 0xFFE6E6E6;  // Light gray (ABGR packed)
};

} // namespace yetty::card
