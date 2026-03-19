#pragma once

#include "yrich.h"
#include <vector>

namespace yetty::yrich {

//=============================================================================
// Paragraph — a block of text with formatting
//=============================================================================
class Paragraph : public TextElement {
public:
    Paragraph(ElementId id, float x, float y, float width);

    // Element interface
    Rect bounds() const override { return _bounds; }
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // TextElement interface
    std::string text() const override { return _text; }
    void setText(std::string_view text) override;
    TextStyle textStyle() const override { return _style; }
    void setTextStyle(const TextStyle& style) override { _style = style; }

    // Layout
    void setWidth(float w);
    void setPosition(float x, float y);
    float height() const { return _bounds.h; }

    // Editing
    void beginEdit() override;
    void endEdit() override;
    bool isEditing() const override { return _editing; }
    int cursorPosition() const override { return _cursorPos; }
    void setCursorPosition(int pos) override;
    void insertText(std::string_view text) override;
    void deleteSelection() override;

    // Selection
    int selectionStart() const override { return _selStart; }
    int selectionEnd() const override { return _selEnd; }
    void setSelection(int start, int end) override;
    std::string selectedText() const override;

private:
    void recalculateLayout();
    int positionToIndex(float x, float y) const;
    std::pair<float, float> indexToPosition(int index) const;

    Rect _bounds;
    std::string _text;
    TextStyle _style;
    float _lineHeight = 20.0f;

    // Word-wrapped lines
    struct Line {
        int startIndex;
        int endIndex;
        float y;
    };
    std::vector<Line> _lines;

    bool _editing = false;
    int _cursorPos = 0;
    int _selStart = 0;
    int _selEnd = 0;
};

using ParagraphPtr = std::shared_ptr<Paragraph>;

//=============================================================================
// YDoc — rich text document
//=============================================================================
class YDoc : public Document,
             public base::ObjectFactory<YDoc> {
public:
    using Ptr = std::shared_ptr<YDoc>;

    static Result<Ptr> createImpl();

    const char* typeName() const override { return "YDoc"; }

    //=========================================================================
    // Document setup
    //=========================================================================

    void setPageWidth(float w) { _pageWidth = w; relayout(); }
    void setPageMargin(float m) { _margin = m; relayout(); }
    float pageWidth() const { return _pageWidth; }
    float margin() const { return _margin; }

    //=========================================================================
    // Paragraph management
    //=========================================================================

    ParagraphPtr addParagraph(std::string_view text = "");
    ParagraphPtr insertParagraph(int index, std::string_view text = "");
    void removeParagraph(int index);

    ParagraphPtr paragraphAt(int index) const;
    int paragraphCount() const { return static_cast<int>(_paragraphs.size()); }

    // Find paragraph containing y position
    int paragraphIndexAt(float y) const;

    //=========================================================================
    // Cursor/selection
    //=========================================================================

    struct CursorPos {
        int paragraphIndex;
        int charIndex;
    };

    CursorPos cursor() const { return _cursor; }
    void setCursor(int paraIndex, int charIndex);
    void moveCursor(int delta, bool extend = false);
    void moveCursorLine(int delta, bool extend = false);
    void moveCursorWord(int delta, bool extend = false);

    //=========================================================================
    // Document interface
    //=========================================================================

    float contentWidth() const override { return _pageWidth; }
    float contentHeight() const override;

    void render() override;

    // Input handling
    void onMouseDown(float x, float y, MouseButton button, InputModifiers mods) override;
    void onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) override;
    void onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) override;
    void onKeyDown(Key key, InputModifiers mods) override;
    void onTextInput(std::string_view text) override;

    // Clipboard
    std::string copySelection() const override;
    void paste(std::string_view data) override;
    void deleteSelection() override;

protected:
    YDoc() = default;

    void doApplyOperation(const Operation& op) override;
    void relayout();

private:
    CursorPos positionAt(float x, float y) const;
    void ensureCursorVisible();

    float _pageWidth = 600.0f;
    float _margin = 20.0f;

    std::vector<ParagraphPtr> _paragraphs;
    CursorPos _cursor{0, 0};
    CursorPos _selectionAnchor{0, 0};
    bool _hasSelection = false;
};

} // namespace yetty::yrich
