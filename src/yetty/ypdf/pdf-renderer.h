#pragma once

#include <memory>

struct _pdfio_file_s;
typedef struct _pdfio_file_s pdfio_file_t;

namespace yetty {
class YDrawBuffer;
}

namespace yetty::card {

struct PdfRenderResult {
    float totalHeight = 0;
    float firstPageHeight = 0;
    int pageCount = 0;
};

/// Render all pages of a PDF into the given buffer.
/// Fonts, text, geometry, and scene metadata all go into the buffer.
/// No builder dependency — the buffer is self-contained.
PdfRenderResult renderPdfToBuffer(pdfio_file_t* pdf,
                                   std::shared_ptr<yetty::YDrawBuffer> buffer);

} // namespace yetty::card
