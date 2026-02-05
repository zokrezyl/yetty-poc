#include "pdf-content-parser.h"
#include <cstring>
#include <cstdlib>
#include <cmath>

extern "C" {
#include <pdfio.h>
}

namespace yetty::card {

//=============================================================================
// PdfMatrix
//=============================================================================

PdfMatrix PdfMatrix::operator*(const PdfMatrix& o) const {
    PdfMatrix r;
    r.a = a * o.a + b * o.c;
    r.b = a * o.b + b * o.d;
    r.c = c * o.a + d * o.c;
    r.d = c * o.b + d * o.d;
    r.e = e * o.a + f * o.c + o.e;
    r.f = e * o.b + f * o.d + o.f;
    return r;
}

void PdfMatrix::transformPoint(float ix, float iy, float& ox, float& oy) const {
    ox = a * ix + c * iy + e;
    oy = b * ix + d * iy + f;
}

//=============================================================================
// PdfContentParser
//=============================================================================

PdfContentParser::PdfContentParser() = default;

//=============================================================================
// Operand helpers
//=============================================================================

float PdfContentParser::popFloat() {
    if (_operands.empty()) return 0.0f;
    float v = std::stof(_operands.back());
    _operands.pop_back();
    return v;
}

std::string PdfContentParser::popString() {
    if (_operands.empty()) return {};
    std::string v = std::move(_operands.back());
    _operands.pop_back();
    return v;
}

//=============================================================================
// Main parse loop
//=============================================================================

void PdfContentParser::parseStream(pdfio_stream_t* stream) {
    char token[4096];

    while (pdfioStreamGetToken(stream, token, sizeof(token))) {
        char first = token[0];

        if (first == '(' || first == '<' || first == '/') {
            // String literal, hex string, or name
            _operands.emplace_back(token);
        } else if (first == '[') {
            _operands.emplace_back("[");
        } else if (first == ']') {
            _operands.emplace_back("]");
        } else if ((first >= '0' && first <= '9') || first == '-' ||
                   first == '+' || first == '.') {
            // Numeric operand
            _operands.emplace_back(token);
        } else if ((first >= 'A' && first <= 'Z') ||
                   (first >= 'a' && first <= 'z') ||
                   first == '\'' || first == '"') {
            // Operator keyword
            dispatchOperator(token);
            _operands.clear();
        }
    }
}

//=============================================================================
// Operator dispatch
//=============================================================================

void PdfContentParser::dispatchOperator(const char* op) {
    // Text object
    if (std::strcmp(op, "BT") == 0) { handleBT(); return; }
    if (std::strcmp(op, "ET") == 0) { handleET(); return; }

    // Text state
    if (std::strcmp(op, "Tf") == 0) { handleTf(); return; }
    if (std::strcmp(op, "Tc") == 0) { handleTc(); return; }
    if (std::strcmp(op, "Tw") == 0) { handleTw(); return; }
    if (std::strcmp(op, "TL") == 0) { handleTL(); return; }
    if (std::strcmp(op, "Ts") == 0) { handleTs(); return; }

    // Text positioning
    if (std::strcmp(op, "Td") == 0) { handleTd(); return; }
    if (std::strcmp(op, "TD") == 0) { handleTD(); return; }
    if (std::strcmp(op, "Tm") == 0) { handleTm(); return; }
    if (std::strcmp(op, "T*") == 0) { handleTstar(); return; }

    // Text showing
    if (std::strcmp(op, "Tj") == 0) { handleTj(); return; }
    if (std::strcmp(op, "TJ") == 0) { handleTJ(); return; }
    if (std::strcmp(op, "'") == 0)  { handleQuote(); return; }
    if (std::strcmp(op, "\"") == 0) { handleDblQuote(); return; }

    // Graphics state
    if (std::strcmp(op, "cm") == 0) { handleCm(); return; }
    if (std::strcmp(op, "q") == 0)  { handleQ(); return; }
    if (std::strcmp(op, "Q") == 0)  { handleQr(); return; }

    // All other operators silently ignored for MVP
}

//=============================================================================
// Text object operators
//=============================================================================

void PdfContentParser::handleBT() {
    _inTextObject = true;
    _textMatrix = PdfMatrix::identity();
    _textLineMatrix = PdfMatrix::identity();
}

void PdfContentParser::handleET() {
    _inTextObject = false;
}

//=============================================================================
// Text state operators
//=============================================================================

void PdfContentParser::handleTf() {
    // Operands: /FontName fontSize
    if (_operands.size() >= 2) {
        float size = std::stof(_operands[_operands.size() - 1]);
        std::string name = _operands[_operands.size() - 2];
        _textState.fontSize = size;
        _textState.fontName = std::move(name);
    }
}

void PdfContentParser::handleTc() {
    if (!_operands.empty())
        _textState.charSpacing = std::stof(_operands.back());
}

void PdfContentParser::handleTw() {
    if (!_operands.empty())
        _textState.wordSpacing = std::stof(_operands.back());
}

void PdfContentParser::handleTL() {
    if (!_operands.empty())
        _textState.leading = std::stof(_operands.back());
}

void PdfContentParser::handleTs() {
    if (!_operands.empty())
        _textState.rise = std::stof(_operands.back());
}

//=============================================================================
// Text positioning operators
//=============================================================================

void PdfContentParser::handleTd() {
    // Operands: tx ty
    if (_operands.size() >= 2) {
        float ty = std::stof(_operands[_operands.size() - 1]);
        float tx = std::stof(_operands[_operands.size() - 2]);

        PdfMatrix translate = {1, 0, 0, 1, tx, ty};
        _textLineMatrix = translate * _textLineMatrix;
        _textMatrix = _textLineMatrix;
    }
}

void PdfContentParser::handleTD() {
    // Same as: -ty TL  tx ty Td
    if (_operands.size() >= 2) {
        float ty = std::stof(_operands[_operands.size() - 1]);
        _textState.leading = -ty;
        handleTd();
    }
}

void PdfContentParser::handleTm() {
    // Operands: a b c d e f
    if (_operands.size() >= 6) {
        size_t base = _operands.size() - 6;
        PdfMatrix m;
        m.a = std::stof(_operands[base + 0]);
        m.b = std::stof(_operands[base + 1]);
        m.c = std::stof(_operands[base + 2]);
        m.d = std::stof(_operands[base + 3]);
        m.e = std::stof(_operands[base + 4]);
        m.f = std::stof(_operands[base + 5]);
        _textMatrix = m;
        _textLineMatrix = m;
    }
}

void PdfContentParser::handleTstar() {
    // T* is equivalent to: 0 -TL Td
    _operands.push_back("0");
    _operands.push_back(std::to_string(-_textState.leading));
    handleTd();
}

//=============================================================================
// Text showing operators
//=============================================================================

void PdfContentParser::handleTj() {
    if (_operands.empty()) return;

    // Find the last string operand
    std::string raw = _operands.back();

    // Strip parentheses if present (pdfio may include them)
    if (raw.size() >= 2 && raw.front() == '(' && raw.back() == ')') {
        raw = raw.substr(1, raw.size() - 2);
    }

    emitText(decodePdfString(raw));
}

void PdfContentParser::handleTJ() {
    // Operands on stack: [ (string) num (string) num ... ]
    bool inArray = false;

    for (const auto& op : _operands) {
        if (op == "[") { inArray = true; continue; }
        if (op == "]") { inArray = false; continue; }
        if (!inArray) continue;

        if (!op.empty() && (op.front() == '(' || op.front() == '<')) {
            // String element
            std::string raw = op;
            if (raw.size() >= 2 && raw.front() == '(' && raw.back() == ')') {
                raw = raw.substr(1, raw.size() - 2);
            }
            emitText(decodePdfString(raw));
        } else {
            // Numeric adjustment (thousandths of text space unit)
            // Negative values move right, positive move left
            float adj = 0.0f;
            try { adj = std::stof(op); } catch (...) { continue; }
            float displacement = -adj / 1000.0f * _textState.fontSize;
            float hScale = _textState.horizontalScaling / 100.0f;
            displacement *= hScale;
            _textMatrix.e += displacement * _textMatrix.a;
            _textMatrix.f += displacement * _textMatrix.b;
        }
    }
}

void PdfContentParser::handleQuote() {
    // ' is: T*  string Tj
    // Save the string operand, do T*, then show
    std::string saved;
    if (!_operands.empty()) {
        saved = _operands.back();
        _operands.pop_back();
    }
    handleTstar();
    if (!saved.empty()) {
        _operands.push_back(std::move(saved));
        handleTj();
    }
}

void PdfContentParser::handleDblQuote() {
    // " is: aw Tw  ac Tc  string '
    if (_operands.size() >= 3) {
        size_t n = _operands.size();
        _textState.wordSpacing = std::stof(_operands[n - 3]);
        _textState.charSpacing = std::stof(_operands[n - 2]);

        std::string saved = _operands[n - 1];
        _operands.clear();
        handleTstar();
        _operands.push_back(std::move(saved));
        handleTj();
    }
}

//=============================================================================
// Graphics state operators
//=============================================================================

void PdfContentParser::handleCm() {
    if (_operands.size() >= 6) {
        size_t base = _operands.size() - 6;
        PdfMatrix m;
        m.a = std::stof(_operands[base + 0]);
        m.b = std::stof(_operands[base + 1]);
        m.c = std::stof(_operands[base + 2]);
        m.d = std::stof(_operands[base + 3]);
        m.e = std::stof(_operands[base + 4]);
        m.f = std::stof(_operands[base + 5]);
        _ctm = m * _ctm;
    }
}

void PdfContentParser::handleQ() {
    // Save graphics state
    PdfGraphicsState saved;
    saved.ctm = _ctm;
    saved.textState = _textState;
    _stateStack.push_back(saved);
}

void PdfContentParser::handleQr() {
    // Restore graphics state
    if (!_stateStack.empty()) {
        const auto& saved = _stateStack.back();
        _ctm = saved.ctm;
        _textState = saved.textState;
        _stateStack.pop_back();
    }
}

//=============================================================================
// Text emission
//=============================================================================

void PdfContentParser::emitText(const std::string& decoded) {
    if (decoded.empty() || !_inTextObject) return;

    // Compute text rendering matrix: Trm = [fontSize 0 0 fontSize 0 rise] x Tm x CTM
    PdfMatrix fontMatrix = {_textState.fontSize, 0, 0, _textState.fontSize,
                            0, _textState.rise};
    PdfMatrix trm = fontMatrix * _textMatrix * _ctm;

    // Position in PDF user space
    float posX, posY;
    trm.transformPoint(0, 0, posX, posY);

    // Effective font size from the matrix
    float effectiveSize = std::sqrt(trm.b * trm.b + trm.d * trm.d);
    if (effectiveSize < 0.5f) effectiveSize = _textState.fontSize;

    PdfTextSpan span;
    span.x = posX;
    span.y = posY;
    span.fontSize = effectiveSize;
    span.text = decoded;
    _spans.push_back(span);

    // Advance text matrix by text width
    // Monospace approximation: each character advances by fontSize * 0.6
    float hScale = _textState.horizontalScaling / 100.0f;

    // Count UTF-8 characters (not bytes)
    size_t charCount = 0;
    for (size_t i = 0; i < decoded.size(); ) {
        uint8_t ch = static_cast<uint8_t>(decoded[i]);
        if (ch < 0x80) i += 1;
        else if (ch < 0xE0) i += 2;
        else if (ch < 0xF0) i += 3;
        else i += 4;
        charCount++;
    }

    for (size_t i = 0; i < charCount; i++) {
        float charWidth = _textState.fontSize * 0.6f;
        float advance = (charWidth + _textState.charSpacing) * hScale;

        // Extra word spacing for space characters (approximate: check first char)
        if (i < decoded.size() && decoded[i] == ' ') {
            advance += _textState.wordSpacing * hScale;
        }

        _textMatrix.e += advance * _textMatrix.a;
        _textMatrix.f += advance * _textMatrix.b;
    }
}

//=============================================================================
// WinAnsi/Latin1 to UTF-8 decoder
//=============================================================================

std::string PdfContentParser::decodePdfString(const std::string& raw) {
    // Windows-1252 (WinAnsi) high byte mapping to Unicode
    static const uint16_t winAnsiHigh[32] = {
        0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,  // 80-87
        0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,  // 88-8F
        0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,  // 90-97
        0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178,  // 98-9F
    };

    std::string result;
    result.reserve(raw.size());

    for (size_t i = 0; i < raw.size(); i++) {
        uint8_t ch = static_cast<uint8_t>(raw[i]);

        // Handle PDF escape sequences
        if (raw[i] == '\\' && i + 1 < raw.size()) {
            char next = raw[i + 1];
            switch (next) {
                case 'n':  result += '\n'; i++; continue;
                case 'r':  result += '\r'; i++; continue;
                case 't':  result += '\t'; i++; continue;
                case 'b':  result += '\b'; i++; continue;
                case 'f':  result += '\f'; i++; continue;
                case '(':  result += '(';  i++; continue;
                case ')':  result += ')';  i++; continue;
                case '\\': result += '\\'; i++; continue;
                default:
                    // Octal escape: \NNN
                    if (next >= '0' && next <= '7') {
                        unsigned val = next - '0';
                        i++;
                        if (i + 1 < raw.size() && raw[i + 1] >= '0' && raw[i + 1] <= '7') {
                            val = val * 8 + (raw[++i] - '0');
                            if (i + 1 < raw.size() && raw[i + 1] >= '0' && raw[i + 1] <= '7') {
                                val = val * 8 + (raw[++i] - '0');
                            }
                        }
                        ch = static_cast<uint8_t>(val & 0xFF);
                    } else {
                        i++;
                        continue;
                    }
            }
        }

        // Map byte to Unicode codepoint
        uint32_t codepoint;
        if (ch < 0x80) {
            codepoint = ch;
        } else if (ch <= 0x9F) {
            codepoint = winAnsiHigh[ch - 0x80];
        } else {
            // 0xA0-0xFF: same as Unicode Latin-1 Supplement
            codepoint = ch;
        }

        // Encode as UTF-8
        if (codepoint < 0x80) {
            result += static_cast<char>(codepoint);
        } else if (codepoint < 0x800) {
            result += static_cast<char>(0xC0 | (codepoint >> 6));
            result += static_cast<char>(0x80 | (codepoint & 0x3F));
        } else {
            result += static_cast<char>(0xE0 | (codepoint >> 12));
            result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (codepoint & 0x3F));
        }
    }

    return result;
}

} // namespace yetty::card
