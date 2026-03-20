#pragma once

#include "yrich.h"
#include <vector>

namespace yetty::yrich {

//=============================================================================
// TextRun — a contiguous run of text with uniform formatting
//=============================================================================
struct TextRun {
    int start = 0;          // Start index in paragraph text
    int end = 0;            // End index (exclusive)
    TextStyle style;        // Formatting for this run

    int length() const { return end - start; }
};

//=============================================================================
// Paragraph — a block of text with rich formatting
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

    //=========================================================================
    // Rich text formatting
    //=========================================================================

    // Get formatting at position
    TextStyle styleAt(int pos) const;

    // Apply formatting to range
    void applyFormat(int start, int end, TextFormat format);
    void removeFormat(int start, int end, TextFormat format);
    void toggleFormat(int start, int end, TextFormat format);

    // Set style property for range
    void setFontSize(int start, int end, float size);
    void setColor(int start, int end, Color color);

    // Get runs (for rendering)
    const std::vector<TextRun>& runs() const { return _runs; }

private:
    void recalculateLayout();
    void normalizeRuns();
    void splitRunAt(int pos);
    int positionToIndex(float x, float y) const;
    std::pair<float, float> indexToPosition(int index) const;

    Rect _bounds;
    std::string _text;
    TextStyle _style;           // Default style
    std::vector<TextRun> _runs; // Formatted runs
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
// Table — embedded table in document
//=============================================================================
class Table : public Element {
public:
    Table(ElementId id, float x, float y, int rows, int cols);

    // Element interface
    Rect bounds() const override { return _bounds; }
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // Table structure
    int rows() const { return _rows; }
    int cols() const { return _cols; }
    void resize(int rows, int cols);

    // Cell access
    const std::string& cell(int row, int col) const;
    void setCell(int row, int col, std::string_view text);

    // Column widths
    float columnWidth(int col) const;
    void setColumnWidth(int col, float width);

    // Row heights
    float rowHeight(int row) const;
    void setRowHeight(int row, float height);

    // Selection
    struct CellPos { int row, col; };
    CellPos selectedCell() const { return _selectedCell; }
    void selectCell(int row, int col);

    // Editing
    bool isEditing() const { return _editing; }
    void beginEdit();
    void endEdit();

    void setPosition(float x, float y);
    float height() const { return _bounds.h; }

private:
    void recalculateBounds();

    Rect _bounds;
    int _rows, _cols;
    std::vector<std::string> _cells;
    std::vector<float> _colWidths;
    std::vector<float> _rowHeights;

    CellPos _selectedCell{0, 0};
    bool _editing = false;
    int _cursorPos = 0;
};

using TablePtr = std::shared_ptr<Table>;

//=============================================================================
// InlineImage — embedded image in document
//=============================================================================
class InlineImage : public Element {
public:
    InlineImage(ElementId id, float x, float y, float width, float height);

    // Element interface
    Rect bounds() const override { return _bounds; }
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // Image source (base64 data or path)
    void setSource(std::string_view src) { _source = src; }
    const std::string& source() const { return _source; }

    // Alt text
    void setAltText(std::string_view alt) { _altText = alt; }
    const std::string& altText() const { return _altText; }

    // Caption
    void setCaption(std::string_view caption) { _caption = caption; }
    const std::string& caption() const { return _caption; }

    // Sizing
    void setSize(float w, float h);
    void setPosition(float x, float y);
    float height() const { return _bounds.h + (_caption.empty() ? 0 : 20); }

    // Alignment
    enum class Align { Left, Center, Right };
    Align align() const { return _align; }
    void setAlign(Align a) { _align = a; }

private:
    Rect _bounds;
    std::string _source;
    std::string _altText;
    std::string _caption;
    Align _align = Align::Center;
};

using InlineImagePtr = std::shared_ptr<InlineImage>;

//=============================================================================
// Comment — annotation attached to text range
//=============================================================================
struct Comment {
    ElementId id;
    int paragraphIndex;
    int startPos;
    int endPos;
    std::string text;
    std::string author;
    uint64_t timestamp;
    bool resolved = false;
    std::vector<Comment> replies;
};

using CommentPtr = std::shared_ptr<Comment>;

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

    //=========================================================================
    // Table management
    //=========================================================================

    TablePtr insertTable(int paragraphIndex, int rows, int cols);
    void removeTable(ElementId id);
    TablePtr tableAt(int index) const;
    int tableCount() const { return static_cast<int>(_tables.size()); }

    //=========================================================================
    // Image management
    //=========================================================================

    InlineImagePtr insertImage(int paragraphIndex, float width, float height);
    void removeImage(ElementId id);
    InlineImagePtr imageAt(int index) const;
    int imageCount() const { return static_cast<int>(_images.size()); }

    //=========================================================================
    // Comments
    //=========================================================================

    CommentPtr addComment(int paragraphIndex, int startPos, int endPos,
                          std::string_view text, std::string_view author);
    void resolveComment(ElementId id);
    void deleteComment(ElementId id);
    void replyToComment(ElementId id, std::string_view text, std::string_view author);
    std::vector<CommentPtr> commentsInRange(int paragraphIndex, int startPos, int endPos) const;
    const std::vector<CommentPtr>& allComments() const { return _comments; }

    //=========================================================================
    // Version history
    //=========================================================================

    struct Version {
        uint64_t timestamp;
        std::string author;
        std::string description;
        uint64_t operationIndex;  // Index in operation log
    };

    void saveVersion(std::string_view description);
    void restoreVersion(int versionIndex);
    const std::vector<Version>& versions() const { return _versions; }
    int versionCount() const { return static_cast<int>(_versions.size()); }

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
    // Formatting (Ctrl+B, Ctrl+I, Ctrl+U)
    //=========================================================================

    void toggleBold();
    void toggleItalic();
    void toggleUnderline();
    void setFontSize(float size);
    void setTextColor(Color color);

    // Check if selection has format
    bool selectionHasBold() const;
    bool selectionHasItalic() const;
    bool selectionHasUnderline() const;

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
    std::vector<TablePtr> _tables;
    std::vector<InlineImagePtr> _images;
    std::vector<CommentPtr> _comments;
    std::vector<Version> _versions;

    CursorPos _cursor{0, 0};
    CursorPos _selectionAnchor{0, 0};
    bool _hasSelection = false;
};

} // namespace yetty::yrich
