#include "ydoc.h"
#include "../ydraw/ydraw-buffer.h"
#include <algorithm>
#include <cctype>
#include <ctime>

namespace yetty::yrich {

//=============================================================================
// Paragraph implementation
//=============================================================================

Paragraph::Paragraph(ElementId id, float x, float y, float width)
    : TextElement(id)
    , _bounds{x, y, width, 20.0f}
{
    _style.fontSize = 14.0f;
    _style.color = Color::black();
    _lineHeight = _style.fontSize * 1.4f;
}

void Paragraph::setText(std::string_view text) {
    _text = text;
    recalculateLayout();
}

void Paragraph::setWidth(float w) {
    _bounds.w = w;
    recalculateLayout();
}

void Paragraph::setPosition(float x, float y) {
    _bounds.x = x;
    _bounds.y = y;
}

void Paragraph::recalculateLayout() {
    _lines.clear();

    if (_text.empty()) {
        _bounds.h = _lineHeight;
        _lines.push_back({0, 0, 0});
        return;
    }

    // Simple word wrap
    float charWidth = _style.fontSize * 0.6f;
    int charsPerLine = static_cast<int>((_bounds.w - 8) / charWidth);
    if (charsPerLine < 1) charsPerLine = 1;

    int pos = 0;
    float y = 0;
    while (pos < static_cast<int>(_text.size())) {
        int lineEnd = pos + charsPerLine;
        if (lineEnd >= static_cast<int>(_text.size())) {
            lineEnd = static_cast<int>(_text.size());
        } else {
            // Find word break
            int breakPos = lineEnd;
            while (breakPos > pos && !std::isspace(_text[breakPos])) {
                breakPos--;
            }
            if (breakPos > pos) {
                lineEnd = breakPos + 1;
            }
        }

        _lines.push_back({pos, lineEnd, y});
        y += _lineHeight;
        pos = lineEnd;

        // Skip whitespace at start of new line
        while (pos < static_cast<int>(_text.size()) && _text[pos] == ' ') {
            pos++;
        }
    }

    _bounds.h = std::max(_lineHeight, y);
}

void Paragraph::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    float defaultCharWidth = _style.fontSize * 0.6f;

    // Background highlight if selected
    if (selected && !_editing) {
        buffer->addBox(layer,
            _bounds.x + _bounds.w / 2, _bounds.y + _bounds.h / 2,
            _bounds.w / 2, _bounds.h / 2,
            Color{230, 240, 255, 255}.toPacked(),
            0, 0, 0);
    }

    // Render text lines with formatting
    for (const auto& line : _lines) {
        if (line.startIndex >= static_cast<int>(_text.size())) continue;

        float textX = _bounds.x + 4;
        float textY = _bounds.y + line.y + _lineHeight * 0.75f;

        // If no runs, render with default style
        if (_runs.empty()) {
            std::string lineText = _text.substr(line.startIndex, line.endIndex - line.startIndex);
            buffer->addText(textX, textY, lineText, _style.fontSize,
                            _style.color.toPacked(), layer + 1, _style.fontId);
        } else {
            // Render each run that intersects this line
            for (const auto& run : _runs) {
                if (run.end <= line.startIndex || run.start >= line.endIndex) continue;

                int runLineStart = std::max(run.start, line.startIndex);
                int runLineEnd = std::min(run.end, line.endIndex);
                if (runLineStart >= runLineEnd) continue;

                std::string runText = _text.substr(runLineStart, runLineEnd - runLineStart);

                float charWidth = run.style.fontSize * 0.6f;
                float runX = textX + (runLineStart - line.startIndex) * charWidth;
                float runY = textY;

                // Adjust font ID for bold/italic (assuming fontId 1 = bold, 2 = italic, 3 = bold+italic)
                int fontId = run.style.fontId;
                if (run.style.bold() && run.style.italic()) {
                    fontId = 3;
                } else if (run.style.bold()) {
                    fontId = 1;
                } else if (run.style.italic()) {
                    fontId = 2;
                }

                buffer->addText(runX, runY, runText, run.style.fontSize,
                                run.style.color.toPacked(), layer + 1, fontId);

                // Draw underline if needed
                if (run.style.underline()) {
                    float ulY = textY + 3;  // Below baseline
                    float ulX1 = runX;
                    float ulX2 = runX + runText.size() * charWidth;
                    buffer->addSegment(layer + 1, ulX1, ulY, ulX2, ulY,
                                       0, run.style.color.toPacked(), 1.0f, 0);
                }

                // Draw strikethrough if needed
                if (run.style.strike()) {
                    float stY = textY - run.style.fontSize * 0.3f;
                    float stX1 = runX;
                    float stX2 = runX + runText.size() * charWidth;
                    buffer->addSegment(layer + 1, stX1, stY, stX2, stY,
                                       0, run.style.color.toPacked(), 1.0f, 0);
                }
            }
        }
    }

    // Selection highlight
    if (_editing && _selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);

        for (const auto& line : _lines) {
            if (line.endIndex <= start || line.startIndex >= end) continue;

            int lineSelStart = std::max(start, line.startIndex) - line.startIndex;
            int lineSelEnd = std::min(end, line.endIndex) - line.startIndex;

            float x1 = _bounds.x + 4 + lineSelStart * defaultCharWidth;
            float x2 = _bounds.x + 4 + lineSelEnd * defaultCharWidth;
            float y = _bounds.y + line.y;

            buffer->addBox(layer,
                (x1 + x2) / 2, y + _lineHeight / 2,
                (x2 - x1) / 2, _lineHeight / 2,
                Color{100, 150, 255, 100}.toPacked(),
                0, 0, 0);
        }
    }

    // Cursor
    if (_editing && _selStart == _selEnd) {
        auto [cx, cy] = indexToPosition(_cursorPos);
        buffer->addSegment(layer + 2,
            cx, cy,
            cx, cy + _lineHeight,
            0, Color::black().toPacked(), 1.5f, 0);
    }
}

void Paragraph::beginEdit() {
    _editing = true;
    _cursorPos = static_cast<int>(_text.size());
    _selStart = _selEnd = _cursorPos;
}

void Paragraph::endEdit() {
    _editing = false;
}

void Paragraph::setCursorPosition(int pos) {
    _cursorPos = std::clamp(pos, 0, static_cast<int>(_text.size()));
    _selStart = _selEnd = _cursorPos;
}

void Paragraph::setSelection(int start, int end) {
    _selStart = std::clamp(start, 0, static_cast<int>(_text.size()));
    _selEnd = std::clamp(end, 0, static_cast<int>(_text.size()));
    _cursorPos = _selEnd;
}

std::string Paragraph::selectedText() const {
    if (_selStart == _selEnd) return "";
    int start = std::min(_selStart, _selEnd);
    int end = std::max(_selStart, _selEnd);
    return _text.substr(start, end - start);
}

void Paragraph::insertText(std::string_view text) {
    // Delete selection first
    if (_selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);
        _text.erase(start, end - start);
        _cursorPos = start;
    }

    _text.insert(_cursorPos, text);
    _cursorPos += static_cast<int>(text.size());
    _selStart = _selEnd = _cursorPos;
    recalculateLayout();
}

void Paragraph::deleteSelection() {
    if (_selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);
        _text.erase(start, end - start);
        _cursorPos = start;
    } else if (_cursorPos > 0) {
        _text.erase(_cursorPos - 1, 1);
        _cursorPos--;
    }
    _selStart = _selEnd = _cursorPos;
    recalculateLayout();
}

std::pair<float, float> Paragraph::indexToPosition(int index) const {
    float charWidth = _style.fontSize * 0.6f;

    for (const auto& line : _lines) {
        if (index >= line.startIndex && index <= line.endIndex) {
            float x = _bounds.x + 4 + (index - line.startIndex) * charWidth;
            float y = _bounds.y + line.y;
            return {x, y};
        }
    }

    // End of text
    if (!_lines.empty()) {
        const auto& lastLine = _lines.back();
        float x = _bounds.x + 4 + (lastLine.endIndex - lastLine.startIndex) * charWidth;
        float y = _bounds.y + lastLine.y;
        return {x, y};
    }

    return {_bounds.x + 4, _bounds.y};
}

int Paragraph::positionToIndex(float x, float y) const {
    float charWidth = _style.fontSize * 0.6f;
    float relY = y - _bounds.y;

    // Find line
    int lineIdx = static_cast<int>(relY / _lineHeight);
    if (lineIdx < 0) lineIdx = 0;
    if (lineIdx >= static_cast<int>(_lines.size())) {
        lineIdx = static_cast<int>(_lines.size()) - 1;
    }

    if (_lines.empty()) return 0;

    const auto& line = _lines[lineIdx];
    float relX = x - _bounds.x - 4;
    int charIdx = static_cast<int>(relX / charWidth + 0.5f);
    charIdx = std::clamp(charIdx, 0, line.endIndex - line.startIndex);

    return line.startIndex + charIdx;
}

//=============================================================================
// Paragraph formatting
//=============================================================================

TextStyle Paragraph::styleAt(int pos) const {
    pos = std::clamp(pos, 0, static_cast<int>(_text.size()));

    for (const auto& run : _runs) {
        if (pos >= run.start && pos < run.end) {
            return run.style;
        }
    }
    return _style;
}

void Paragraph::normalizeRuns() {
    if (_runs.empty()) {
        // Create single run covering all text
        if (!_text.empty()) {
            _runs.push_back({0, static_cast<int>(_text.size()), _style});
        }
        return;
    }

    // Merge adjacent runs with same style
    std::vector<TextRun> merged;
    for (const auto& run : _runs) {
        if (run.start >= run.end) continue;

        if (!merged.empty() &&
            merged.back().end == run.start &&
            merged.back().style.format == run.style.format &&
            merged.back().style.fontSize == run.style.fontSize &&
            merged.back().style.color.toPacked() == run.style.color.toPacked()) {
            merged.back().end = run.end;
        } else {
            merged.push_back(run);
        }
    }
    _runs = std::move(merged);
}

void Paragraph::splitRunAt(int pos) {
    if (pos <= 0 || pos >= static_cast<int>(_text.size())) return;

    for (size_t i = 0; i < _runs.size(); ++i) {
        auto& run = _runs[i];
        if (pos > run.start && pos < run.end) {
            TextRun newRun = run;
            newRun.start = pos;
            run.end = pos;
            _runs.insert(_runs.begin() + i + 1, newRun);
            return;
        }
    }
}

void Paragraph::applyFormat(int start, int end, TextFormat format) {
    if (start >= end) return;
    start = std::max(0, start);
    end = std::min(static_cast<int>(_text.size()), end);

    // Initialize runs if empty
    if (_runs.empty() && !_text.empty()) {
        _runs.push_back({0, static_cast<int>(_text.size()), _style});
    }

    // Split runs at boundaries
    splitRunAt(start);
    splitRunAt(end);

    // Apply format to runs in range
    for (auto& run : _runs) {
        if (run.start >= start && run.end <= end) {
            run.style.format = run.style.format | format;
        }
    }

    normalizeRuns();
}

void Paragraph::removeFormat(int start, int end, TextFormat format) {
    if (start >= end) return;
    start = std::max(0, start);
    end = std::min(static_cast<int>(_text.size()), end);

    if (_runs.empty()) return;

    splitRunAt(start);
    splitRunAt(end);

    for (auto& run : _runs) {
        if (run.start >= start && run.end <= end) {
            run.style.format = run.style.format & ~format;
        }
    }

    normalizeRuns();
}

void Paragraph::toggleFormat(int start, int end, TextFormat format) {
    if (start >= end) return;

    // Check if all text in range has the format
    bool allHaveFormat = true;
    for (int i = start; i < end; ++i) {
        if (!hasFormat(styleAt(i).format, format)) {
            allHaveFormat = false;
            break;
        }
    }

    if (allHaveFormat) {
        removeFormat(start, end, format);
    } else {
        applyFormat(start, end, format);
    }
}

void Paragraph::setFontSize(int start, int end, float size) {
    if (start >= end) return;
    start = std::max(0, start);
    end = std::min(static_cast<int>(_text.size()), end);

    if (_runs.empty() && !_text.empty()) {
        _runs.push_back({0, static_cast<int>(_text.size()), _style});
    }

    splitRunAt(start);
    splitRunAt(end);

    for (auto& run : _runs) {
        if (run.start >= start && run.end <= end) {
            run.style.fontSize = size;
        }
    }

    normalizeRuns();
    recalculateLayout();
}

void Paragraph::setColor(int start, int end, Color color) {
    if (start >= end) return;
    start = std::max(0, start);
    end = std::min(static_cast<int>(_text.size()), end);

    if (_runs.empty() && !_text.empty()) {
        _runs.push_back({0, static_cast<int>(_text.size()), _style});
    }

    splitRunAt(start);
    splitRunAt(end);

    for (auto& run : _runs) {
        if (run.start >= start && run.end <= end) {
            run.style.color = color;
        }
    }

    normalizeRuns();
}

//=============================================================================
// YDoc implementation
//=============================================================================

Result<YDoc::Ptr> YDoc::createImpl() {
    return Ptr(new YDoc());
}

ParagraphPtr YDoc::addParagraph(std::string_view text) {
    float y = contentHeight();
    auto para = std::make_shared<Paragraph>(
        nextElementId(), _margin, y, _pageWidth - 2 * _margin);
    para->setText(text);
    _paragraphs.push_back(para);
    addElement(para);
    return para;
}

ParagraphPtr YDoc::insertParagraph(int index, std::string_view text) {
    if (index < 0) index = 0;
    if (index > static_cast<int>(_paragraphs.size())) {
        index = static_cast<int>(_paragraphs.size());
    }

    auto para = std::make_shared<Paragraph>(
        nextElementId(), _margin, 0, _pageWidth - 2 * _margin);
    para->setText(text);
    _paragraphs.insert(_paragraphs.begin() + index, para);
    addElement(para);
    relayout();
    return para;
}

void YDoc::removeParagraph(int index) {
    if (index < 0 || index >= static_cast<int>(_paragraphs.size())) return;

    removeElement(_paragraphs[index]->id());
    _paragraphs.erase(_paragraphs.begin() + index);
    relayout();
}

ParagraphPtr YDoc::paragraphAt(int index) const {
    if (index < 0 || index >= static_cast<int>(_paragraphs.size())) {
        return nullptr;
    }
    return _paragraphs[index];
}

int YDoc::paragraphIndexAt(float y) const {
    for (int i = 0; i < static_cast<int>(_paragraphs.size()); ++i) {
        auto& para = _paragraphs[i];
        if (y >= para->bounds().y && y < para->bounds().y + para->bounds().h) {
            return i;
        }
    }
    return _paragraphs.empty() ? -1 : static_cast<int>(_paragraphs.size()) - 1;
}

void YDoc::relayout() {
    float y = _margin;
    for (auto& para : _paragraphs) {
        para->setPosition(_margin, y);
        para->setWidth(_pageWidth - 2 * _margin);
        y += para->height() + 8;  // paragraph spacing
    }
    markDirty();
}

float YDoc::contentHeight() const {
    if (_paragraphs.empty()) return _margin * 2;

    auto& last = _paragraphs.back();
    return last->bounds().y + last->height() + _margin;
}

void YDoc::setCursor(int paraIndex, int charIndex) {
    paraIndex = std::clamp(paraIndex, 0, std::max(0, paragraphCount() - 1));
    if (auto para = paragraphAt(paraIndex)) {
        charIndex = std::clamp(charIndex, 0, static_cast<int>(para->text().size()));
    }
    _cursor = {paraIndex, charIndex};
    _selectionAnchor = _cursor;
    _hasSelection = false;
    markDirty();
}

void YDoc::moveCursor(int delta, bool extend) {
    auto para = paragraphAt(_cursor.paragraphIndex);
    if (!para) return;

    int newPos = _cursor.charIndex + delta;

    if (newPos < 0) {
        // Move to previous paragraph
        if (_cursor.paragraphIndex > 0) {
            _cursor.paragraphIndex--;
            para = paragraphAt(_cursor.paragraphIndex);
            _cursor.charIndex = static_cast<int>(para->text().size());
        } else {
            _cursor.charIndex = 0;
        }
    } else if (newPos > static_cast<int>(para->text().size())) {
        // Move to next paragraph
        if (_cursor.paragraphIndex < paragraphCount() - 1) {
            _cursor.paragraphIndex++;
            _cursor.charIndex = 0;
        } else {
            _cursor.charIndex = static_cast<int>(para->text().size());
        }
    } else {
        _cursor.charIndex = newPos;
    }

    if (!extend) {
        _selectionAnchor = _cursor;
        _hasSelection = false;
    } else {
        _hasSelection = true;
    }

    markDirty();
}

void YDoc::moveCursorLine(int delta, bool extend) {
    // Simple: move to prev/next paragraph
    int newPara = _cursor.paragraphIndex + delta;
    newPara = std::clamp(newPara, 0, paragraphCount() - 1);

    if (auto para = paragraphAt(newPara)) {
        _cursor.paragraphIndex = newPara;
        _cursor.charIndex = std::min(_cursor.charIndex,
                                      static_cast<int>(para->text().size()));
    }

    if (!extend) {
        _selectionAnchor = _cursor;
        _hasSelection = false;
    } else {
        _hasSelection = true;
    }

    markDirty();
}

void YDoc::moveCursorWord(int delta, bool extend) {
    auto para = paragraphAt(_cursor.paragraphIndex);
    if (!para) return;

    const std::string& text = para->text();
    int pos = _cursor.charIndex;

    if (delta > 0) {
        // Move forward to end of word
        while (pos < static_cast<int>(text.size()) && !std::isalnum(text[pos])) pos++;
        while (pos < static_cast<int>(text.size()) && std::isalnum(text[pos])) pos++;
    } else {
        // Move backward to start of word
        while (pos > 0 && !std::isalnum(text[pos - 1])) pos--;
        while (pos > 0 && std::isalnum(text[pos - 1])) pos--;
    }

    _cursor.charIndex = pos;

    if (!extend) {
        _selectionAnchor = _cursor;
        _hasSelection = false;
    } else {
        _hasSelection = true;
    }

    markDirty();
}

YDoc::CursorPos YDoc::positionAt(float x, float y) const {
    int paraIdx = paragraphIndexAt(y);
    if (paraIdx < 0) return {0, 0};

    auto para = paragraphAt(paraIdx);
    if (!para) return {paraIdx, 0};

    // Use paragraph's internal hit testing
    float relX = x - para->bounds().x;
    float relY = y - para->bounds().y;

    float charWidth = para->textStyle().fontSize * 0.6f;
    float lineHeight = para->textStyle().fontSize * 1.4f;

    int line = static_cast<int>(relY / lineHeight);
    int charIdx = static_cast<int>((relX - 4) / charWidth + 0.5f);

    // Approximate character index
    int charsPerLine = static_cast<int>((para->bounds().w - 8) / charWidth);
    int idx = line * charsPerLine + charIdx;
    idx = std::clamp(idx, 0, static_cast<int>(para->text().size()));

    return {paraIdx, idx};
}

void YDoc::doApplyOperation(const Operation& op) {
    // Handle text operations
    switch (op.type()) {
        case OperationType::TextInsert: {
            auto& data = std::get<TextInsertOp>(op.data());
            // Find paragraph by element ID
            for (auto& para : _paragraphs) {
                if (para->id() == data.id) {
                    std::string text = para->text();
                    text.insert(data.position, data.text);
                    para->setText(text);
                    break;
                }
            }
            relayout();
            break;
        }
        case OperationType::TextDelete: {
            auto& data = std::get<TextDeleteOp>(op.data());
            for (auto& para : _paragraphs) {
                if (para->id() == data.id) {
                    std::string text = para->text();
                    text.erase(data.position, data.length);
                    para->setText(text);
                    break;
                }
            }
            relayout();
            break;
        }
        default:
            Document::doApplyOperation(op);
            break;
    }
}

void YDoc::render() {
    if (!_buffer) return;

    _buffer->clear();
    _buffer->setSceneBounds(0, 0, contentWidth(), contentHeight());
    _buffer->setBgColor(_bgColor.toPacked());

    // Page background
    _buffer->addBox(0,
        _pageWidth / 2, contentHeight() / 2,
        _pageWidth / 2, contentHeight() / 2,
        Color::white().toPacked(),
        Color{200, 200, 200, 255}.toPacked(),
        1.0f, 0);

    // Render paragraphs
    uint32_t layer = 1;
    for (int i = 0; i < static_cast<int>(_paragraphs.size()); ++i) {
        bool isCurrentPara = (i == _cursor.paragraphIndex);
        _paragraphs[i]->render(_buffer, layer, isCurrentPara);
        layer += 4;
    }

    // Render cursor if current paragraph is editing
    if (auto para = paragraphAt(_cursor.paragraphIndex)) {
        if (!para->isEditing()) {
            // Draw cursor even when not editing
            float charWidth = para->textStyle().fontSize * 0.6f;
            float lineHeight = para->textStyle().fontSize * 1.4f;

            int charsPerLine = static_cast<int>((para->bounds().w - 8) / charWidth);
            if (charsPerLine < 1) charsPerLine = 1;

            int line = _cursor.charIndex / charsPerLine;
            int col = _cursor.charIndex % charsPerLine;

            float cx = para->bounds().x + 4 + col * charWidth;
            float cy = para->bounds().y + line * lineHeight;

            _buffer->addSegment(layer,
                cx, cy, cx, cy + lineHeight,
                0, Color{0, 100, 200, 255}.toPacked(), 2.0f, 0);
        }
    }

    _dirty = false;
}

void YDoc::onMouseDown(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto pos = positionAt(x, y);
    _cursor = pos;

    if (!mods.shift) {
        _selectionAnchor = pos;
        _hasSelection = false;
    } else {
        _hasSelection = true;
    }

    markDirty();
}

void YDoc::onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    _cursor = positionAt(x, y);
    _hasSelection = true;
    markDirty();
}

void YDoc::onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    // Select word
    auto pos = positionAt(x, y);
    auto para = paragraphAt(pos.paragraphIndex);
    if (!para) return;

    const std::string& text = para->text();
    int start = pos.charIndex;
    int end = pos.charIndex;

    // Find word boundaries
    while (start > 0 && std::isalnum(text[start - 1])) start--;
    while (end < static_cast<int>(text.size()) && std::isalnum(text[end])) end++;

    _cursor = {pos.paragraphIndex, end};
    _selectionAnchor = {pos.paragraphIndex, start};
    _hasSelection = (start != end);

    markDirty();
}

void YDoc::onKeyDown(Key key, InputModifiers mods) {
    bool extend = mods.shift;

    switch (key) {
        case Key::Left:
            if (mods.ctrl) moveCursorWord(-1, extend);
            else moveCursor(-1, extend);
            break;
        case Key::Right:
            if (mods.ctrl) moveCursorWord(1, extend);
            else moveCursor(1, extend);
            break;
        case Key::Up:
            moveCursorLine(-1, extend);
            break;
        case Key::Down:
            moveCursorLine(1, extend);
            break;
        case Key::Home:
            _cursor.charIndex = 0;
            if (!extend) { _selectionAnchor = _cursor; _hasSelection = false; }
            else _hasSelection = true;
            markDirty();
            break;
        case Key::End:
            if (auto para = paragraphAt(_cursor.paragraphIndex)) {
                _cursor.charIndex = static_cast<int>(para->text().size());
            }
            if (!extend) { _selectionAnchor = _cursor; _hasSelection = false; }
            else _hasSelection = true;
            markDirty();
            break;
        case Key::Backspace:
            deleteSelection();
            break;
        case Key::Delete:
            if (!_hasSelection) {
                moveCursor(1, true);
            }
            deleteSelection();
            break;
        case Key::Enter: {
            // Split paragraph or add new one
            auto para = paragraphAt(_cursor.paragraphIndex);
            if (para) {
                std::string text = para->text();
                std::string before = text.substr(0, _cursor.charIndex);
                std::string after = text.substr(_cursor.charIndex);
                para->setText(before);
                insertParagraph(_cursor.paragraphIndex + 1, after);
                _cursor.paragraphIndex++;
                _cursor.charIndex = 0;
                _selectionAnchor = _cursor;
                _hasSelection = false;
            }
            break;
        }
        default:
            Document::onKeyDown(key, mods);
            break;
    }
}

void YDoc::onTextInput(std::string_view text) {
    if (_hasSelection) {
        deleteSelection();
    }

    auto para = paragraphAt(_cursor.paragraphIndex);
    if (!para) {
        // Create first paragraph
        para = addParagraph("");
        _cursor = {0, 0};
    }

    std::string content = para->text();
    content.insert(_cursor.charIndex, text);
    para->setText(content);

    _cursor.charIndex += static_cast<int>(text.size());
    _selectionAnchor = _cursor;
    _hasSelection = false;

    relayout();
    markDirty();
}

std::string YDoc::copySelection() const {
    if (!_hasSelection) return "";

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    std::string result;
    for (int i = start.paragraphIndex; i <= end.paragraphIndex; ++i) {
        auto para = paragraphAt(i);
        if (!para) continue;

        int startIdx = (i == start.paragraphIndex) ? start.charIndex : 0;
        int endIdx = (i == end.paragraphIndex) ? end.charIndex : static_cast<int>(para->text().size());

        result += para->text().substr(startIdx, endIdx - startIdx);
        if (i < end.paragraphIndex) result += "\n";
    }

    return result;
}

void YDoc::paste(std::string_view data) {
    if (_hasSelection) {
        deleteSelection();
    }

    // Split by newlines
    size_t pos = 0;
    while (pos < data.size()) {
        size_t nl = data.find('\n', pos);
        if (nl == std::string_view::npos) nl = data.size();

        std::string_view line = data.substr(pos, nl - pos);

        auto para = paragraphAt(_cursor.paragraphIndex);
        if (para) {
            std::string content = para->text();
            content.insert(_cursor.charIndex, line);
            para->setText(content);
            _cursor.charIndex += static_cast<int>(line.size());
        }

        if (nl < data.size()) {
            // Insert new paragraph
            auto para = paragraphAt(_cursor.paragraphIndex);
            if (para) {
                std::string text = para->text();
                std::string after = text.substr(_cursor.charIndex);
                para->setText(text.substr(0, _cursor.charIndex));
                insertParagraph(_cursor.paragraphIndex + 1, after);
                _cursor.paragraphIndex++;
                _cursor.charIndex = 0;
            }
        }

        pos = nl + 1;
    }

    _selectionAnchor = _cursor;
    _hasSelection = false;
    relayout();
    markDirty();
}

void YDoc::deleteSelection() {
    if (!_hasSelection) {
        // Delete char before cursor
        auto para = paragraphAt(_cursor.paragraphIndex);
        if (!para) return;

        if (_cursor.charIndex > 0) {
            std::string text = para->text();
            text.erase(_cursor.charIndex - 1, 1);
            para->setText(text);
            _cursor.charIndex--;
        } else if (_cursor.paragraphIndex > 0) {
            // Merge with previous paragraph
            auto prevPara = paragraphAt(_cursor.paragraphIndex - 1);
            if (prevPara) {
                int newCursor = static_cast<int>(prevPara->text().size());
                std::string merged = prevPara->text() + para->text();
                prevPara->setText(merged);
                removeParagraph(_cursor.paragraphIndex);
                _cursor.paragraphIndex--;
                _cursor.charIndex = newCursor;
            }
        }
    } else {
        CursorPos start = _selectionAnchor;
        CursorPos end = _cursor;
        if (start.paragraphIndex > end.paragraphIndex ||
            (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
            std::swap(start, end);
        }

        if (start.paragraphIndex == end.paragraphIndex) {
            // Same paragraph
            auto para = paragraphAt(start.paragraphIndex);
            if (para) {
                std::string text = para->text();
                text.erase(start.charIndex, end.charIndex - start.charIndex);
                para->setText(text);
            }
        } else {
            // Multiple paragraphs
            auto startPara = paragraphAt(start.paragraphIndex);
            auto endPara = paragraphAt(end.paragraphIndex);

            if (startPara && endPara) {
                std::string merged = startPara->text().substr(0, start.charIndex) +
                                     endPara->text().substr(end.charIndex);
                startPara->setText(merged);

                // Remove paragraphs in between
                for (int i = end.paragraphIndex; i > start.paragraphIndex; --i) {
                    removeParagraph(i);
                }
            }
        }

        _cursor = start;
        _hasSelection = false;
    }

    _selectionAnchor = _cursor;
    relayout();
    markDirty();
}

//=============================================================================
// YDoc formatting
//=============================================================================

void YDoc::toggleBold() {
    if (!_hasSelection) return;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    // Only support same-paragraph selection for now
    if (start.paragraphIndex == end.paragraphIndex) {
        auto para = paragraphAt(start.paragraphIndex);
        if (para) {
            para->toggleFormat(start.charIndex, end.charIndex, TextFormat::Bold);
            markDirty();
        }
    }
}

void YDoc::toggleItalic() {
    if (!_hasSelection) return;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex == end.paragraphIndex) {
        auto para = paragraphAt(start.paragraphIndex);
        if (para) {
            para->toggleFormat(start.charIndex, end.charIndex, TextFormat::Italic);
            markDirty();
        }
    }
}

void YDoc::toggleUnderline() {
    if (!_hasSelection) return;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex == end.paragraphIndex) {
        auto para = paragraphAt(start.paragraphIndex);
        if (para) {
            para->toggleFormat(start.charIndex, end.charIndex, TextFormat::Underline);
            markDirty();
        }
    }
}

void YDoc::setFontSize(float size) {
    if (!_hasSelection) return;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex == end.paragraphIndex) {
        auto para = paragraphAt(start.paragraphIndex);
        if (para) {
            para->setFontSize(start.charIndex, end.charIndex, size);
            relayout();
            markDirty();
        }
    }
}

void YDoc::setTextColor(Color color) {
    if (!_hasSelection) return;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex == end.paragraphIndex) {
        auto para = paragraphAt(start.paragraphIndex);
        if (para) {
            para->setColor(start.charIndex, end.charIndex, color);
            markDirty();
        }
    }
}

bool YDoc::selectionHasBold() const {
    if (!_hasSelection) return false;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex != end.paragraphIndex) return false;

    auto para = paragraphAt(start.paragraphIndex);
    if (!para) return false;

    for (int i = start.charIndex; i < end.charIndex; ++i) {
        if (!para->styleAt(i).bold()) return false;
    }
    return true;
}

bool YDoc::selectionHasItalic() const {
    if (!_hasSelection) return false;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex != end.paragraphIndex) return false;

    auto para = paragraphAt(start.paragraphIndex);
    if (!para) return false;

    for (int i = start.charIndex; i < end.charIndex; ++i) {
        if (!para->styleAt(i).italic()) return false;
    }
    return true;
}

bool YDoc::selectionHasUnderline() const {
    if (!_hasSelection) return false;

    CursorPos start = _selectionAnchor;
    CursorPos end = _cursor;
    if (start.paragraphIndex > end.paragraphIndex ||
        (start.paragraphIndex == end.paragraphIndex && start.charIndex > end.charIndex)) {
        std::swap(start, end);
    }

    if (start.paragraphIndex != end.paragraphIndex) return false;

    auto para = paragraphAt(start.paragraphIndex);
    if (!para) return false;

    for (int i = start.charIndex; i < end.charIndex; ++i) {
        if (!para->styleAt(i).underline()) return false;
    }
    return true;
}

//=============================================================================
// Table implementation
//=============================================================================

Table::Table(ElementId id, float x, float y, int rows, int cols)
    : Element(id)
    , _bounds{x, y, 0, 0}
    , _rows(rows)
    , _cols(cols)
{
    _cells.resize(rows * cols);
    _colWidths.resize(cols, 100.0f);  // Default 100px width
    _rowHeights.resize(rows, 30.0f);  // Default 30px height
    recalculateBounds();
}

void Table::recalculateBounds() {
    float totalWidth = 0;
    for (float w : _colWidths) totalWidth += w;

    float totalHeight = 0;
    for (float h : _rowHeights) totalHeight += h;

    _bounds.w = totalWidth;
    _bounds.h = totalHeight;
}

void Table::resize(int rows, int cols) {
    std::vector<std::string> newCells(rows * cols);

    // Copy existing cells
    for (int r = 0; r < std::min(rows, _rows); ++r) {
        for (int c = 0; c < std::min(cols, _cols); ++c) {
            newCells[r * cols + c] = _cells[r * _cols + c];
        }
    }

    _cells = std::move(newCells);
    _rows = rows;
    _cols = cols;

    _colWidths.resize(cols, 100.0f);
    _rowHeights.resize(rows, 30.0f);
    recalculateBounds();
}

const std::string& Table::cell(int row, int col) const {
    static std::string empty;
    if (row < 0 || row >= _rows || col < 0 || col >= _cols) return empty;
    return _cells[row * _cols + col];
}

void Table::setCell(int row, int col, std::string_view text) {
    if (row < 0 || row >= _rows || col < 0 || col >= _cols) return;
    _cells[row * _cols + col] = text;
}

float Table::columnWidth(int col) const {
    if (col < 0 || col >= _cols) return 0;
    return _colWidths[col];
}

void Table::setColumnWidth(int col, float width) {
    if (col < 0 || col >= _cols) return;
    _colWidths[col] = std::max(20.0f, width);
    recalculateBounds();
}

float Table::rowHeight(int row) const {
    if (row < 0 || row >= _rows) return 0;
    return _rowHeights[row];
}

void Table::setRowHeight(int row, float height) {
    if (row < 0 || row >= _rows) return;
    _rowHeights[row] = std::max(20.0f, height);
    recalculateBounds();
}

void Table::selectCell(int row, int col) {
    _selectedCell.row = std::clamp(row, 0, _rows - 1);
    _selectedCell.col = std::clamp(col, 0, _cols - 1);
}

void Table::beginEdit() {
    _editing = true;
    _cursorPos = static_cast<int>(cell(_selectedCell.row, _selectedCell.col).size());
}

void Table::endEdit() {
    _editing = false;
}

void Table::setPosition(float x, float y) {
    _bounds.x = x;
    _bounds.y = y;
}

void Table::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    float y = _bounds.y;

    for (int row = 0; row < _rows; ++row) {
        float x = _bounds.x;
        float rowH = _rowHeights[row];

        for (int col = 0; col < _cols; ++col) {
            float colW = _colWidths[col];
            float cx = x + colW / 2;
            float cy = y + rowH / 2;

            // Cell background
            bool isSelected = (row == _selectedCell.row && col == _selectedCell.col);
            Color bgColor = isSelected ? Color{230, 240, 255, 255} : Color::white();

            buffer->addBox(layer, cx, cy, colW / 2, rowH / 2,
                           bgColor.toPacked(),
                           Color{180, 180, 180, 255}.toPacked(),
                           1.0f, 0);

            // Cell text
            const std::string& text = cell(row, col);
            if (!text.empty()) {
                buffer->addText(x + 4, y + rowH * 0.7f, text, 12.0f,
                                Color::black().toPacked(), layer + 1, 0);
            }

            x += colW;
        }
        y += rowH;
    }

    // Selection outline
    if (selected) {
        float cx = _bounds.x + _bounds.w / 2;
        float cy = _bounds.y + _bounds.h / 2;
        buffer->addBox(layer + 2, cx, cy, _bounds.w / 2 + 2, _bounds.h / 2 + 2,
                       0,
                       Color{0, 120, 215, 255}.toPacked(),
                       2.0f, 0);
    }
}

//=============================================================================
// InlineImage implementation
//=============================================================================

InlineImage::InlineImage(ElementId id, float x, float y, float width, float height)
    : Element(id)
    , _bounds{x, y, width, height}
{}

void InlineImage::setSize(float w, float h) {
    _bounds.w = w;
    _bounds.h = h;
}

void InlineImage::setPosition(float x, float y) {
    _bounds.x = x;
    _bounds.y = y;
}

void InlineImage::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    float cx = _bounds.x + _bounds.w / 2;
    float cy = _bounds.y + _bounds.h / 2;

    // Image placeholder
    buffer->addBox(layer, cx, cy, _bounds.w / 2, _bounds.h / 2,
                   Color{240, 240, 240, 255}.toPacked(),
                   Color{180, 180, 180, 255}.toPacked(),
                   1.0f, 0);

    // Placeholder icon (X pattern)
    buffer->addSegment(layer + 1,
                       _bounds.x + 10, _bounds.y + 10,
                       _bounds.x + _bounds.w - 10, _bounds.y + _bounds.h - 10,
                       0, Color{150, 150, 150, 255}.toPacked(), 1.0f, 0);
    buffer->addSegment(layer + 1,
                       _bounds.x + _bounds.w - 10, _bounds.y + 10,
                       _bounds.x + 10, _bounds.y + _bounds.h - 10,
                       0, Color{150, 150, 150, 255}.toPacked(), 1.0f, 0);

    // Alt text
    if (!_altText.empty()) {
        buffer->addText(_bounds.x + 10, _bounds.y + _bounds.h / 2, _altText,
                        10.0f, Color{100, 100, 100, 255}.toPacked(), layer + 1, 0);
    }

    // Caption
    if (!_caption.empty()) {
        float captionY = _bounds.y + _bounds.h + 15;
        float captionX = _bounds.x;

        if (_align == Align::Center) {
            captionX = _bounds.x + _bounds.w / 2 - _caption.size() * 3;
        } else if (_align == Align::Right) {
            captionX = _bounds.x + _bounds.w - _caption.size() * 6;
        }

        buffer->addText(captionX, captionY, _caption,
                        11.0f, Color{80, 80, 80, 255}.toPacked(), layer + 1, 0);
    }

    // Selection outline
    if (selected) {
        buffer->addBox(layer + 2, cx, cy, _bounds.w / 2 + 2, _bounds.h / 2 + 2,
                       0,
                       Color{0, 120, 215, 255}.toPacked(),
                       2.0f, 0);
    }
}

//=============================================================================
// YDoc: Tables
//=============================================================================

TablePtr YDoc::insertTable(int paragraphIndex, int rows, int cols) {
    float y = _margin;
    if (paragraphIndex > 0 && paragraphIndex < static_cast<int>(_paragraphs.size())) {
        auto prevPara = _paragraphs[paragraphIndex - 1];
        y = prevPara->bounds().y + prevPara->height() + 8;
    }

    auto table = std::make_shared<Table>(nextElementId(), _margin, y, rows, cols);
    _tables.push_back(table);
    addElement(table);
    relayout();
    return table;
}

void YDoc::removeTable(ElementId id) {
    auto it = std::find_if(_tables.begin(), _tables.end(),
                           [id](const TablePtr& t) { return t->id() == id; });
    if (it != _tables.end()) {
        removeElement(id);
        _tables.erase(it);
        relayout();
    }
}

TablePtr YDoc::tableAt(int index) const {
    if (index < 0 || index >= static_cast<int>(_tables.size())) return nullptr;
    return _tables[index];
}

//=============================================================================
// YDoc: Images
//=============================================================================

InlineImagePtr YDoc::insertImage(int paragraphIndex, float width, float height) {
    float y = _margin;
    if (paragraphIndex > 0 && paragraphIndex < static_cast<int>(_paragraphs.size())) {
        auto prevPara = _paragraphs[paragraphIndex - 1];
        y = prevPara->bounds().y + prevPara->height() + 8;
    }

    auto image = std::make_shared<InlineImage>(nextElementId(), _margin, y, width, height);
    _images.push_back(image);
    addElement(image);
    relayout();
    return image;
}

void YDoc::removeImage(ElementId id) {
    auto it = std::find_if(_images.begin(), _images.end(),
                           [id](const InlineImagePtr& img) { return img->id() == id; });
    if (it != _images.end()) {
        removeElement(id);
        _images.erase(it);
        relayout();
    }
}

InlineImagePtr YDoc::imageAt(int index) const {
    if (index < 0 || index >= static_cast<int>(_images.size())) return nullptr;
    return _images[index];
}

//=============================================================================
// YDoc: Comments
//=============================================================================

CommentPtr YDoc::addComment(int paragraphIndex, int startPos, int endPos,
                            std::string_view text, std::string_view author) {
    auto comment = std::make_shared<Comment>();
    comment->id = nextElementId();
    comment->paragraphIndex = paragraphIndex;
    comment->startPos = startPos;
    comment->endPos = endPos;
    comment->text = text;
    comment->author = author;
    comment->timestamp = static_cast<uint64_t>(time(nullptr));
    comment->resolved = false;

    _comments.push_back(comment);
    markDirty();
    return comment;
}

void YDoc::resolveComment(ElementId id) {
    for (auto& comment : _comments) {
        if (comment->id == id) {
            comment->resolved = true;
            markDirty();
            return;
        }
    }
}

void YDoc::deleteComment(ElementId id) {
    auto it = std::find_if(_comments.begin(), _comments.end(),
                           [id](const CommentPtr& c) { return c->id == id; });
    if (it != _comments.end()) {
        _comments.erase(it);
        markDirty();
    }
}

void YDoc::replyToComment(ElementId id, std::string_view text, std::string_view author) {
    for (auto& comment : _comments) {
        if (comment->id == id) {
            Comment reply;
            reply.id = nextElementId();
            reply.text = text;
            reply.author = author;
            reply.timestamp = static_cast<uint64_t>(time(nullptr));
            comment->replies.push_back(reply);
            markDirty();
            return;
        }
    }
}

std::vector<CommentPtr> YDoc::commentsInRange(int paragraphIndex, int startPos, int endPos) const {
    std::vector<CommentPtr> result;
    for (const auto& comment : _comments) {
        if (comment->paragraphIndex == paragraphIndex &&
            comment->startPos < endPos && comment->endPos > startPos) {
            result.push_back(comment);
        }
    }
    return result;
}

//=============================================================================
// YDoc: Version history
//=============================================================================

void YDoc::saveVersion(std::string_view description) {
    Version v;
    v.timestamp = static_cast<uint64_t>(time(nullptr));
    v.author = localSession() ? localSession()->userName() : "Unknown";
    v.description = description;
    v.operationIndex = _opLog.size();

    _versions.push_back(v);
}

void YDoc::restoreVersion(int versionIndex) {
    if (versionIndex < 0 || versionIndex >= static_cast<int>(_versions.size())) return;

    // TODO: Implement full version restoration
    // This would require storing snapshots or replaying operations from the start
    // For now, this is a placeholder

    markDirty();
}

} // namespace yetty::yrich
