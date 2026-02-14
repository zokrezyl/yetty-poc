#pragma once

struct _pdfio_file_s;
typedef struct _pdfio_file_s pdfio_file_t;

namespace yetty {
class YDrawWriter;
}

namespace yetty::card {

struct PdfRenderResult {
    float totalHeight = 0;
    float firstPageHeight = 0;
    int pageCount = 0;
};

/// Render all pages of a PDF to the given YDrawWriter.
/// Handles font extraction, text placement, and vector graphics.
/// Works with any YDrawWriter implementation (internal GPU or binary).
PdfRenderResult renderPdfToWriter(pdfio_file_t* pdf, yetty::YDrawWriter* writer);

} // namespace yetty::card
