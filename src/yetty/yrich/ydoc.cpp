#include "ydoc.h"
#include "../ydraw/ydraw-buffer.h"
#include <algorithm>
#include <cctype>

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
    float charWidth = _style.fontSize * 0.6f;

    // Background highlight if selected
    if (selected && !_editing) {
        buffer->addBox(layer,
            _bounds.x + _bounds.w / 2, _bounds.y + _bounds.h / 2,
            _bounds.w / 2, _bounds.h / 2,
            Color{230, 240, 255, 255}.toPacked(),
            0, 0, 0);
    }

    // Render text lines
    for (const auto& line : _lines) {
        if (line.startIndex >= static_cast<int>(_text.size())) continue;

        std::string lineText = _text.substr(line.startIndex, line.endIndex - line.startIndex);
        float textX = _bounds.x + 4;
        float textY = _bounds.y + line.y + _lineHeight * 0.75f;

        buffer->addText(textX, textY, lineText, _style.fontSize,
                        _style.color.toPacked(), layer + 1, _style.fontId);
    }

    // Selection highlight
    if (_editing && _selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);

        for (const auto& line : _lines) {
            if (line.endIndex <= start || line.startIndex >= end) continue;

            int lineSelStart = std::max(start, line.startIndex) - line.startIndex;
            int lineSelEnd = std::min(end, line.endIndex) - line.startIndex;

            float x1 = _bounds.x + 4 + lineSelStart * charWidth;
            float x2 = _bounds.x + 4 + lineSelEnd * charWidth;
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

} // namespace yetty::yrich
