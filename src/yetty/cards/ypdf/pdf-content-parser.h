#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

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
    std::string fontName;  // PDF font name (e.g. "/F1", "/Helv")
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
// Path types for graphics rendering
//=============================================================================
enum class PdfPathOp : uint8_t {
    MoveTo,
    LineTo,
    CurveTo,    // cubic bezier (c operator)
    ClosePath,
};

struct PdfPathPoint {
    PdfPathOp op;
    float x, y;
    float x1, y1, x2, y2;  // control points for CurveTo
};

enum class PdfPaintMode : uint8_t {
    Stroke,
    Fill,
    FillAndStroke,
};

//=============================================================================
// PdfGraphicsState - saved/restored by q/Q operators
//=============================================================================
struct PdfGraphicsState {
    PdfMatrix ctm;
    PdfTextState textState;
    // Graphics drawing state
    float strokeR = 0, strokeG = 0, strokeB = 0;  // Stroke color (default black)
    float fillR = 0, fillG = 0, fillB = 0;        // Fill color (default black)
    float lineWidth = 1.0f;
};

//=============================================================================
// Callbacks
//=============================================================================

// Callback for glyph placement during text emission.
using TextEmitCallback = std::function<float(
    const std::string& text,
    float posX, float posY,
    float effectiveSize,
    float rotationRadians,
    const PdfTextState& textState
)>;

// Callback for painting a rectangle (optimized path for re+S/f/B).
// Coordinates are in PDF user space (before page flip).
// mode: Stroke=borders only, Fill=solid fill, FillAndStroke=both.
struct PdfRect {
    float x, y, w, h;
};
using RectPaintCallback = std::function<void(
    const PdfRect& rect,
    PdfPaintMode mode,
    float strokeR, float strokeG, float strokeB,
    float fillR, float fillG, float fillB,
    float lineWidth
)>;

// Callback for painting a line segment (for m/l/S paths).
using LinePaintCallback = std::function<void(
    float x0, float y0, float x1, float y1,
    float r, float g, float b,
    float lineWidth
)>;

//=============================================================================
// PdfContentParser
//=============================================================================
class PdfContentParser {
public:
    PdfContentParser();

    void setPageHeight(float h) { _pageHeight = h; }
    void setTextEmitCallback(TextEmitCallback cb) { _textEmitCallback = std::move(cb); }
    void setRectPaintCallback(RectPaintCallback cb) { _rectPaintCallback = std::move(cb); }
    void setLinePaintCallback(LinePaintCallback cb) { _linePaintCallback = std::move(cb); }
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
    void handleW();   // line width

    // Color operators
    void handleRG();  // stroke RGB
    void handleRg();  // fill RGB
    void handleG();   // stroke gray
    void handleGg();  // fill gray
    void handleK();   // stroke CMYK
    void handleKk();  // fill CMYK

    // Path construction operators
    void handleMoveTo();
    void handleLineTo();
    void handleCurveTo();
    void handleCurveToV();
    void handleCurveToY();
    void handleRect();
    void handleClosePath();

    // Path painting operators
    void handleStroke();
    void handleCloseStroke();
    void handleFill();
    void handleFillAndStroke();
    void handleCloseFillAndStroke();
    void handleEndPath();

    // Paint the current path
    void paintPath(PdfPaintMode mode);

    // Helpers
    std::string decodePdfString(const std::string& raw);
    void emitText(const std::string& decoded);
    float popFloat();
    std::string popString();

    // Operand stack
    std::vector<std::string> _operands;

    // Graphics state
    PdfGraphicsState _gstate;
    PdfMatrix _ctm;  // shortcut alias for _gstate.ctm
    std::vector<PdfGraphicsState> _stateStack;

    // Text state
    PdfTextState _textState;
    PdfMatrix _textMatrix;
    PdfMatrix _textLineMatrix;
    bool _inTextObject = false;

    // Path state
    std::vector<PdfPathPoint> _currentPath;
    float _currentX = 0, _currentY = 0;
    float _subpathStartX = 0, _subpathStartY = 0;

    float _pageHeight = 792.0f;

    // Callbacks
    TextEmitCallback _textEmitCallback;
    RectPaintCallback _rectPaintCallback;
    LinePaintCallback _linePaintCallback;

    // Output
    std::vector<PdfTextSpan> _spans;
};

} // namespace yetty::card
