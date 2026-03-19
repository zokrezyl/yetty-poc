#pragma once

#include "yrich.h"
#include <map>
#include <optional>
#include <string>

namespace yetty::yrich {

//=============================================================================
// Cell — a single spreadsheet cell
//=============================================================================
class Cell : public TextElement {
public:
    Cell(ElementId id, const CellAddress& addr, float x, float y, float w, float h);

    // Element interface
    Rect bounds() const override { return _bounds; }
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // TextElement interface
    std::string text() const override { return _text; }
    void setText(std::string_view text) override;
    TextStyle textStyle() const override { return _style; }
    void setTextStyle(const TextStyle& style) override { _style = style; }

    // Cell-specific
    CellAddress address() const { return _address; }
    void setBounds(const Rect& r) { _bounds = r; }

    // Editing
    void beginEdit() override;
    void endEdit() override;
    bool isEditing() const override { return _editing; }
    int cursorPosition() const override { return _cursorPos; }
    void setCursorPosition(int pos) override;
    void insertText(std::string_view text) override;
    void deleteSelection() override;

    // Formula support (future)
    bool hasFormula() const { return !_formula.empty(); }
    std::string formula() const { return _formula; }
    void setFormula(std::string_view formula) { _formula = formula; }

    // Styling
    Color fillColor() const { return _fillColor; }
    void setFillColor(const Color& c) { _fillColor = c; }
    BorderStyle borderStyle() const { return _border; }
    void setBorderStyle(const BorderStyle& b) { _border = b; }

    // Alignment
    enum class HAlign { Left, Center, Right };
    enum class VAlign { Top, Middle, Bottom };
    HAlign hAlign() const { return _hAlign; }
    void setHAlign(HAlign a) { _hAlign = a; }
    VAlign vAlign() const { return _vAlign; }
    void setVAlign(VAlign a) { _vAlign = a; }

private:
    CellAddress _address;
    Rect _bounds;
    std::string _text;
    std::string _formula;
    TextStyle _style;
    Color _fillColor = Color::white();
    BorderStyle _border;
    HAlign _hAlign = HAlign::Left;
    VAlign _vAlign = VAlign::Middle;

    bool _editing = false;
    int _cursorPos = 0;
    int _selStart = 0;
    int _selEnd = 0;
};

using CellPtr = std::shared_ptr<Cell>;

//=============================================================================
// YSpreadsheet — spreadsheet document
//=============================================================================
class YSpreadsheet : public Document,
                     public base::ObjectFactory<YSpreadsheet> {
public:
    using Ptr = std::shared_ptr<YSpreadsheet>;

    static Result<Ptr> createImpl();

    const char* typeName() const override { return "YSpreadsheet"; }

    //=========================================================================
    // Grid configuration
    //=========================================================================

    void setGridSize(int rows, int cols);
    int rowCount() const { return _rowCount; }
    int colCount() const { return _colCount; }

    // Default dimensions
    void setDefaultRowHeight(float h) { _defaultRowHeight = h; }
    void setDefaultColWidth(float w) { _defaultColWidth = w; }
    float defaultRowHeight() const { return _defaultRowHeight; }
    float defaultColWidth() const { return _defaultColWidth; }

    // Per-row/col dimensions
    void setRowHeight(int row, float h);
    void setColWidth(int col, float w);
    float rowHeight(int row) const;
    float colWidth(int col) const;

    // Get row/col position (cumulative)
    float rowY(int row) const;
    float colX(int col) const;

    //=========================================================================
    // Cell access
    //=========================================================================

    CellPtr cellAt(const CellAddress& addr) const;
    CellPtr cellAt(int row, int col) const { return cellAt({row, col}); }

    // Get or create cell
    CellPtr ensureCell(const CellAddress& addr);
    CellPtr ensureCell(int row, int col) { return ensureCell({row, col}); }

    // Set cell value (creates cell if needed)
    void setCellValue(const CellAddress& addr, std::string_view value);
    void setCellValue(int row, int col, std::string_view value) {
        setCellValue({row, col}, value);
    }

    // Get cell value (empty string if cell doesn't exist)
    std::string cellValue(const CellAddress& addr) const;
    std::string cellValue(int row, int col) const { return cellValue({row, col}); }

    //=========================================================================
    // Selection (cell-based)
    //=========================================================================

    void selectCell(const CellAddress& addr);
    void selectCell(int row, int col) { selectCell({row, col}); }
    void selectRange(const CellRange& range);
    void extendSelectionTo(const CellAddress& addr);

    CellAddress activeCell() const;
    std::optional<CellRange> selectedRange() const;

    //=========================================================================
    // Navigation
    //=========================================================================

    void moveActive(int dRow, int dCol, bool extend = false);
    void moveActiveToEdge(int dRow, int dCol, bool extend = false);

    //=========================================================================
    // Document interface
    //=========================================================================

    float contentWidth() const override;
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

    //=========================================================================
    // Hit testing
    //=========================================================================

    CellAddress cellAddressAt(float x, float y) const;

protected:
    YSpreadsheet() = default;

    // Operation handling
    void doApplyOperation(const Operation& op) override;

    // Presence rendering
    void renderPresence() override;

private:
    // Find cell element by address
    CellPtr findCellByAddress(const CellAddress& addr) const;

    // Recalculate cell bounds after resize
    void recalculateCellBounds();

    // Grid
    int _rowCount = 100;
    int _colCount = 26;
    float _defaultRowHeight = 24.0f;
    float _defaultColWidth = 80.0f;

    std::map<int, float> _rowHeights;
    std::map<int, float> _colWidths;

    // Cached cumulative positions (for fast lookup)
    mutable std::vector<float> _rowYCache;
    mutable std::vector<float> _colXCache;
    mutable bool _cacheValid = false;

    void invalidateCache() { _cacheValid = false; }
    void rebuildCache() const;

    // Cell storage (by address for fast lookup)
    std::map<CellAddress, CellPtr> _cells;

    // Editing state
    CellPtr _editingCell;
};

} // namespace yetty::yrich
