#include "yspreadsheet.h"
#include "../ydraw/ydraw-buffer.h"
#include <algorithm>

namespace yetty::yrich {

//=============================================================================
// Cell implementation
//=============================================================================

Cell::Cell(ElementId id, const CellAddress& addr, float x, float y, float w, float h)
    : TextElement(id)
    , _address(addr)
    , _bounds{x, y, w, h}
{
    _style.fontSize = 12.0f;
    _style.color = Color::black();
    _border.width = 1.0f;
    _border.color = Color{200, 200, 200, 255};
}

void Cell::setText(std::string_view text) {
    _text = text;
    _cursorPos = static_cast<int>(_text.size());
}

void Cell::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    // Background
    uint32_t fill = _fillColor.toPacked();
    if (selected && !_editing) {
        fill = Color{220, 235, 255, 255}.toPacked();
    }

    buffer->addBox(layer,
        _bounds.x + _bounds.w / 2, _bounds.y + _bounds.h / 2,
        _bounds.w / 2, _bounds.h / 2,
        fill,
        _border.color.toPacked(),
        _border.width,
        0);

    // Text
    if (!_text.empty()) {
        float textX = _bounds.x + 4;
        float textY = _bounds.y + _bounds.h / 2 + _style.fontSize / 3;

        if (_hAlign == HAlign::Center) {
            textX = _bounds.x + _bounds.w / 2;
        } else if (_hAlign == HAlign::Right) {
            textX = _bounds.x + _bounds.w - 4;
        }

        buffer->addText(textX, textY, _text, _style.fontSize,
                        _style.color.toPacked(), layer + 1, _style.fontId);
    }

    // Cursor when editing
    if (_editing) {
        float cursorX = _bounds.x + 4 + _cursorPos * (_style.fontSize * 0.6f);
        buffer->addSegment(layer + 2,
            cursorX, _bounds.y + 2,
            cursorX, _bounds.y + _bounds.h - 2,
            0, Color::black().toPacked(), 1.0f, 0);
    }

    // Selection border
    if (selected) {
        buffer->addBox(layer + 3,
            _bounds.x + _bounds.w / 2, _bounds.y + _bounds.h / 2,
            _bounds.w / 2, _bounds.h / 2,
            0,
            Color{0, 100, 200, 255}.toPacked(),
            2.0f,
            0);
    }
}

void Cell::beginEdit() {
    _editing = true;
    _cursorPos = static_cast<int>(_text.size());
    _selStart = 0;
    _selEnd = static_cast<int>(_text.size());
}

void Cell::endEdit() {
    _editing = false;
    _selStart = _selEnd = _cursorPos;
}

void Cell::setCursorPosition(int pos) {
    _cursorPos = std::clamp(pos, 0, static_cast<int>(_text.size()));
}

void Cell::insertText(std::string_view text) {
    if (_selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);
        _text.erase(start, end - start);
        _cursorPos = start;
        _selStart = _selEnd = _cursorPos;
    }

    _text.insert(_cursorPos, text);
    _cursorPos += static_cast<int>(text.size());
    _selStart = _selEnd = _cursorPos;
}

void Cell::deleteSelection() {
    if (_selStart == _selEnd) {
        if (_cursorPos > 0) {
            _text.erase(_cursorPos - 1, 1);
            _cursorPos--;
        }
    } else {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);
        _text.erase(start, end - start);
        _cursorPos = start;
    }
    _selStart = _selEnd = _cursorPos;
}

//=============================================================================
// YSpreadsheet implementation
//=============================================================================

Result<YSpreadsheet::Ptr> YSpreadsheet::createImpl() {
    return Ptr(new YSpreadsheet());
}

void YSpreadsheet::setGridSize(int rows, int cols) {
    _rowCount = rows;
    _colCount = cols;
    invalidateCache();
    markDirty();
}

void YSpreadsheet::setRowHeight(int row, float h) {
    _rowHeights[row] = h;
    invalidateCache();
    recalculateCellBounds();
    markDirty();
}

void YSpreadsheet::setColWidth(int col, float w) {
    _colWidths[col] = w;
    invalidateCache();
    recalculateCellBounds();
    markDirty();
}

float YSpreadsheet::rowHeight(int row) const {
    auto it = _rowHeights.find(row);
    return it != _rowHeights.end() ? it->second : _defaultRowHeight;
}

float YSpreadsheet::colWidth(int col) const {
    auto it = _colWidths.find(col);
    return it != _colWidths.end() ? it->second : _defaultColWidth;
}

void YSpreadsheet::rebuildCache() const {
    if (_cacheValid) return;

    _rowYCache.resize(_rowCount + 1);
    _colXCache.resize(_colCount + 1);

    float y = 0;
    for (int r = 0; r <= _rowCount; ++r) {
        _rowYCache[r] = y;
        if (r < _rowCount) {
            auto it = _rowHeights.find(r);
            y += (it != _rowHeights.end()) ? it->second : _defaultRowHeight;
        }
    }

    float x = 0;
    for (int c = 0; c <= _colCount; ++c) {
        _colXCache[c] = x;
        if (c < _colCount) {
            auto it = _colWidths.find(c);
            x += (it != _colWidths.end()) ? it->second : _defaultColWidth;
        }
    }

    _cacheValid = true;
}

float YSpreadsheet::rowY(int row) const {
    rebuildCache();
    if (row < 0) return 0;
    if (row >= static_cast<int>(_rowYCache.size())) return _rowYCache.back();
    return _rowYCache[row];
}

float YSpreadsheet::colX(int col) const {
    rebuildCache();
    if (col < 0) return 0;
    if (col >= static_cast<int>(_colXCache.size())) return _colXCache.back();
    return _colXCache[col];
}

float YSpreadsheet::contentWidth() const {
    rebuildCache();
    return _colXCache.empty() ? 0 : _colXCache.back();
}

float YSpreadsheet::contentHeight() const {
    rebuildCache();
    return _rowYCache.empty() ? 0 : _rowYCache.back();
}

CellPtr YSpreadsheet::cellAt(const CellAddress& addr) const {
    auto it = _cells.find(addr);
    return it != _cells.end() ? it->second : nullptr;
}

CellPtr YSpreadsheet::ensureCell(const CellAddress& addr) {
    auto it = _cells.find(addr);
    if (it != _cells.end()) return it->second;

    float x = colX(addr.col);
    float y = rowY(addr.row);
    float w = colWidth(addr.col);
    float h = rowHeight(addr.row);

    auto cell = std::make_shared<Cell>(nextElementId(), addr, x, y, w, h);
    _cells[addr] = cell;

    // Don't use addElement (which syncs) - just add locally
    _elements[cell->id()] = cell;

    return cell;
}

void YSpreadsheet::setCellValue(const CellAddress& addr, std::string_view value) {
    auto cell = ensureCell(addr);
    std::string oldValue = cell->text();

    // Create operation
    CellSetOp opData;
    opData.addr = addr;
    opData.oldValue = oldValue;
    opData.newValue = std::string(value);

    auto op = createOperation(OperationType::CellSet, opData);

    // Apply
    cell->setText(value);
    _opLog.append(op);

    // Sync
    if (_syncCallback) {
        _syncCallback({op});
    }

    markDirty();
}

std::string YSpreadsheet::cellValue(const CellAddress& addr) const {
    auto cell = cellAt(addr);
    return cell ? cell->text() : "";
}

void YSpreadsheet::recalculateCellBounds() {
    for (auto& [addr, cell] : _cells) {
        float x = colX(addr.col);
        float y = rowY(addr.row);
        float w = colWidth(addr.col);
        float h = rowHeight(addr.row);
        cell->setBounds({x, y, w, h});
    }
}

CellAddress YSpreadsheet::cellAddressAt(float x, float y) const {
    rebuildCache();

    int col = 0;
    for (int c = 0; c < _colCount; ++c) {
        if (_colXCache[c + 1] > x) {
            col = c;
            break;
        }
        col = c;
    }

    int row = 0;
    for (int r = 0; r < _rowCount; ++r) {
        if (_rowYCache[r + 1] > y) {
            row = r;
            break;
        }
        row = r;
    }

    return {row, col};
}

//=============================================================================
// Operation handling
//=============================================================================

void YSpreadsheet::doApplyOperation(const Operation& op) {
    switch (op.type()) {
        case OperationType::CellSet: {
            auto& data = std::get<CellSetOp>(op.data());
            auto cell = ensureCell(data.addr);
            cell->setText(data.newValue);
            break;
        }
        case OperationType::CellClear: {
            auto& data = std::get<CellClearOp>(op.data());
            for (int r = data.range.start.row; r <= data.range.end.row; ++r) {
                for (int c = data.range.start.col; c <= data.range.end.col; ++c) {
                    if (auto cell = cellAt({r, c})) {
                        cell->setText("");
                    }
                }
            }
            break;
        }
        default:
            Document::doApplyOperation(op);
            break;
    }
}

//=============================================================================
// Selection
//=============================================================================

void YSpreadsheet::selectCell(const CellAddress& addr) {
    _localSelection.selectCell(addr);
    broadcastSelection();
    markDirty();
}

void YSpreadsheet::selectRange(const CellRange& range) {
    _localSelection.selectCells(range, range.start);
    broadcastSelection();
    markDirty();
}

void YSpreadsheet::extendSelectionTo(const CellAddress& addr) {
    _localSelection.extendTo(addr);
    broadcastSelection();
    markDirty();
}

CellAddress YSpreadsheet::activeCell() const {
    if (auto* sel = _localSelection.asCells()) {
        return sel->active;
    }
    return {0, 0};
}

std::optional<CellRange> YSpreadsheet::selectedRange() const {
    if (auto* sel = _localSelection.asCells()) {
        return sel->range;
    }
    return std::nullopt;
}

void YSpreadsheet::moveActive(int dRow, int dCol, bool extend) {
    auto active = activeCell();
    int newRow = std::clamp(active.row + dRow, 0, _rowCount - 1);
    int newCol = std::clamp(active.col + dCol, 0, _colCount - 1);

    if (extend) {
        extendSelectionTo({newRow, newCol});
    } else {
        selectCell({newRow, newCol});
    }
}

void YSpreadsheet::moveActiveToEdge(int dRow, int dCol, bool extend) {
    auto active = activeCell();
    int newRow = active.row;
    int newCol = active.col;

    if (dRow < 0) newRow = 0;
    else if (dRow > 0) newRow = _rowCount - 1;
    if (dCol < 0) newCol = 0;
    else if (dCol > 0) newCol = _colCount - 1;

    if (extend) {
        extendSelectionTo({newRow, newCol});
    } else {
        selectCell({newRow, newCol});
    }
}

//=============================================================================
// Rendering
//=============================================================================

void YSpreadsheet::render() {
    if (!_buffer) return;

    _buffer->clear();
    _buffer->setSceneBounds(0, 0, contentWidth(), contentHeight());
    _buffer->setBgColor(_bgColor.toPacked());

    // Grid lines
    uint32_t gridColor = Color{220, 220, 220, 255}.toPacked();

    for (int r = 0; r <= _rowCount; ++r) {
        float y = rowY(r);
        _buffer->addSegment(0, 0, y, contentWidth(), y, 0, gridColor, 1.0f, 0);
    }

    for (int c = 0; c <= _colCount; ++c) {
        float x = colX(c);
        _buffer->addSegment(0, x, 0, x, contentHeight(), 0, gridColor, 1.0f, 0);
    }

    // Cells
    auto range = selectedRange();
    uint32_t layer = 1;
    for (const auto& [addr, cell] : _cells) {
        bool selected = range && range->contains(addr);
        cell->render(_buffer, layer, selected);
        layer += 4;
    }

    // Active cell highlight (even if no Cell object)
    if (range) {
        auto active = activeCell();
        if (!cellAt(active)) {
            float x = colX(active.col);
            float y = rowY(active.row);
            float w = colWidth(active.col);
            float h = rowHeight(active.row);

            _buffer->addBox(layer,
                x + w / 2, y + h / 2, w / 2, h / 2,
                Color{220, 235, 255, 255}.toPacked(),
                Color{0, 100, 200, 255}.toPacked(),
                2.0f, 0);
        }
    }

    // Remote user selections
    renderPresence();

    _dirty = false;
}

void YSpreadsheet::renderPresence() {
    uint32_t layer = 10000;  // High layer for presence

    _sessions.forEachSession([&](const SessionPtr& session) {
        if (session == localSession()) return;

        auto& selState = session->selection();
        if (auto* cellSel = std::get_if<CellSelection>(&selState)) {
            // Render remote user's selection with their color
            Color userColor = session->userColor();
            userColor.a = 100;  // Semi-transparent

            for (int r = cellSel->range.start.row; r <= cellSel->range.end.row; ++r) {
                for (int c = cellSel->range.start.col; c <= cellSel->range.end.col; ++c) {
                    float x = colX(c);
                    float y = rowY(r);
                    float w = colWidth(c);
                    float h = rowHeight(r);

                    _buffer->addBox(layer,
                        x + w / 2, y + h / 2, w / 2, h / 2,
                        userColor.toPacked(),
                        session->userColor().toPacked(),
                        1.0f, 0);
                }
            }
            layer++;
        }
    });
}

//=============================================================================
// Input handling
//=============================================================================

void YSpreadsheet::onMouseDown(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    if (_editingCell) {
        _editingCell->endEdit();
        _editingCell = nullptr;
    }

    auto addr = cellAddressAt(x, y);

    if (mods.shift) {
        extendSelectionTo(addr);
    } else {
        selectCell(addr);
    }
}

void YSpreadsheet::onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto addr = cellAddressAt(x, y);
    extendSelectionTo(addr);
}

void YSpreadsheet::onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    auto addr = cellAddressAt(x, y);
    auto cell = ensureCell(addr);
    cell->beginEdit();
    _editingCell = cell;
    selectCell(addr);
}

void YSpreadsheet::onKeyDown(Key key, InputModifiers mods) {
    if (_editingCell) {
        switch (key) {
            case Key::Enter:
            case Key::Tab:
                _editingCell->endEdit();
                _editingCell = nullptr;
                if (key == Key::Enter) moveActive(1, 0);
                else moveActive(0, 1);
                return;
            case Key::Escape:
                _editingCell->endEdit();
                _editingCell = nullptr;
                return;
            case Key::Backspace:
                _editingCell->deleteSelection();
                markDirty();
                return;
            case Key::Left:
                _editingCell->setCursorPosition(_editingCell->cursorPosition() - 1);
                markDirty();
                return;
            case Key::Right:
                _editingCell->setCursorPosition(_editingCell->cursorPosition() + 1);
                markDirty();
                return;
            default:
                break;
        }
    }

    bool extend = mods.shift;
    bool jump = mods.ctrl;

    switch (key) {
        case Key::Up:
            if (jump) moveActiveToEdge(-1, 0, extend);
            else moveActive(-1, 0, extend);
            break;
        case Key::Down:
            if (jump) moveActiveToEdge(1, 0, extend);
            else moveActive(1, 0, extend);
            break;
        case Key::Left:
            if (jump) moveActiveToEdge(0, -1, extend);
            else moveActive(0, -1, extend);
            break;
        case Key::Right:
            if (jump) moveActiveToEdge(0, 1, extend);
            else moveActive(0, 1, extend);
            break;
        case Key::Home:
            if (mods.ctrl) selectCell({0, 0});
            else moveActiveToEdge(0, -1, extend);
            break;
        case Key::End:
            if (mods.ctrl) selectCell({_rowCount - 1, _colCount - 1});
            else moveActiveToEdge(0, 1, extend);
            break;
        case Key::PageUp:
            moveActive(-20, 0, extend);
            break;
        case Key::PageDown:
            moveActive(20, 0, extend);
            break;
        case Key::Tab:
            moveActive(0, mods.shift ? -1 : 1);
            break;
        case Key::Enter:
            moveActive(mods.shift ? -1 : 1, 0);
            break;
        case Key::Delete:
            deleteSelection();
            break;
        case Key::F2: {
            auto cell = ensureCell(activeCell());
            cell->beginEdit();
            _editingCell = cell;
            markDirty();
            break;
        }
        default:
            Document::onKeyDown(key, mods);
            break;
    }
}

void YSpreadsheet::onTextInput(std::string_view text) {
    if (_editingCell) {
        _editingCell->insertText(text);
        markDirty();
    } else {
        auto cell = ensureCell(activeCell());
        cell->setText("");
        cell->beginEdit();
        cell->insertText(text);
        _editingCell = cell;
        markDirty();
    }
}

//=============================================================================
// Clipboard
//=============================================================================

std::string YSpreadsheet::copySelection() const {
    auto range = selectedRange();
    if (!range) return "";

    std::string result;
    for (int r = range->start.row; r <= range->end.row; ++r) {
        for (int c = range->start.col; c <= range->end.col; ++c) {
            if (c > range->start.col) result += '\t';
            result += cellValue({r, c});
        }
        if (r < range->end.row) result += '\n';
    }
    return result;
}

void YSpreadsheet::paste(std::string_view data) {
    auto active = activeCell();
    int row = active.row;
    int col = active.col;

    size_t pos = 0;
    while (pos < data.size()) {
        size_t lineEnd = data.find('\n', pos);
        if (lineEnd == std::string_view::npos) lineEnd = data.size();

        std::string_view line = data.substr(pos, lineEnd - pos);
        int c = col;

        size_t cellPos = 0;
        while (cellPos < line.size()) {
            size_t tabPos = line.find('\t', cellPos);
            if (tabPos == std::string_view::npos) tabPos = line.size();

            std::string_view val = line.substr(cellPos, tabPos - cellPos);
            if (row < _rowCount && c < _colCount) {
                setCellValue({row, c}, val);
            }

            cellPos = tabPos + 1;
            c++;
        }

        pos = lineEnd + 1;
        row++;
    }
}

void YSpreadsheet::deleteSelection() {
    auto range = selectedRange();
    if (!range) return;

    // Create clear operation
    CellClearOp opData;
    opData.range = *range;

    // Save old values for undo
    for (int r = range->start.row; r <= range->end.row; ++r) {
        for (int c = range->start.col; c <= range->end.col; ++c) {
            if (auto cell = cellAt({r, c})) {
                if (!cell->text().empty()) {
                    opData.oldValues.push_back({{r, c}, cell->text()});
                }
            }
        }
    }

    auto op = createOperation(OperationType::CellClear, opData);

    // Apply
    for (int r = range->start.row; r <= range->end.row; ++r) {
        for (int c = range->start.col; c <= range->end.col; ++c) {
            if (auto cell = cellAt({r, c})) {
                cell->setText("");
            }
        }
    }

    _opLog.append(op);

    if (_syncCallback) {
        _syncCallback({op});
    }

    markDirty();
}

} // namespace yetty::yrich
