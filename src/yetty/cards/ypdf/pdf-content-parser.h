#pragma once

#include <string>
#include <vector>
#include <cstdint>

// Forward declare pdfio types (pdfio uses _pdfio_*_s naming)
struct _pdfio_stream_s;
typedef struct _pdfio_stream_s pdfio_stream_t;

namespace yetty::card {

//=============================================================================
// PdfTextSpan - a positioned text fragment extracted from a PDF content stream
//=============================================================================
struct PdfTextSpan {
    float x;            // X position in PDF user space
    float y;            // Y position in PDF user space
    float fontSize;     // Effective font size (from Tf and text matrix)
    std::string text;   // Decoded text (UTF-8)
};

//=============================================================================
// PdfMatrix - 3x2 affine transform matrix
//
// Represents: | a b 0 |
//             | c d 0 |
//             | e f 1 |
//=============================================================================
struct PdfMatrix {
    float a = 1, b = 0, c = 0, d = 1, e = 0, f = 0;

    static PdfMatrix identity() { return {1, 0, 0, 1, 0, 0}; }

    PdfMatrix operator*(const PdfMatrix& o) const;
    void transformPoint(float ix, float iy, float& ox, float& oy) const;
};

//=============================================================================
// PdfTextState - per PDF spec 9.3
//=============================================================================
struct PdfTextState {
    float charSpacing = 0.0f;         // Tc
    float wordSpacing = 0.0f;         // Tw
    float horizontalScaling = 100.0f; // Tz (percentage)
    float leading = 0.0f;             // TL
    float fontSize = 12.0f;           // Tfs (from Tf)
    float rise = 0.0f;               // Trise
    std::string fontName;             // For logging
};

//=============================================================================
// PdfGraphicsState - saved/restored by q/Q operators
//=============================================================================
struct PdfGraphicsState {
    PdfMatrix ctm;
    PdfTextState textState;
};

//=============================================================================
// PdfContentParser - extracts positioned text from PDF content streams
//
// Usage:
//   PdfContentParser parser;
//   parser.setPageHeight(792.0f);
//   parser.parseStream(stream);
//   for (const auto& span : parser.textSpans()) { ... }
//=============================================================================
class PdfContentParser {
public:
    PdfContentParser();

    void setPageHeight(float h) { _pageHeight = h; }
    void parseStream(pdfio_stream_t* stream);
    const std::vector<PdfTextSpan>& textSpans() const { return _spans; }

private:
    void dispatchOperator(const char* op);

    // Text object operators
    void handleBT();
    void handleET();

    // Text state operators
    void handleTf();
    void handleTc();
    void handleTw();
    void handleTL();
    void handleTs();

    // Text positioning operators
    void handleTd();
    void handleTD();
    void handleTm();
    void handleTstar();

    // Text showing operators
    void handleTj();
    void handleTJ();
    void handleQuote();
    void handleDblQuote();

    // Graphics state operators
    void handleCm();
    void handleQ();   // save
    void handleQr();  // restore

    // Helpers
    std::string decodePdfString(const std::string& raw);
    void emitText(const std::string& decoded);
    float popFloat();
    std::string popString();

    // Operand stack
    std::vector<std::string> _operands;

    // Graphics state
    PdfMatrix _ctm;
    std::vector<PdfGraphicsState> _stateStack;

    // Text state
    PdfTextState _textState;
    PdfMatrix _textMatrix;
    PdfMatrix _textLineMatrix;
    bool _inTextObject = false;

    float _pageHeight = 792.0f;

    // Output
    std::vector<PdfTextSpan> _spans;
};

} // namespace yetty::card
