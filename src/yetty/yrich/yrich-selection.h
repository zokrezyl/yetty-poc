#pragma once

#include "yrich-types.h"
#include <algorithm>
#include <set>
#include <variant>
#include <vector>

namespace yetty::yrich {

//=============================================================================
// Selection — represents what's currently selected in a document
//
// Different document types have different selection models:
// - Spreadsheet: cell range (A1:C10) or multiple non-contiguous ranges
// - Slides: one or more shapes/elements
// - Docs: text range (start pos, end pos) or element selection
//
// This class provides a unified interface with type-specific accessors.
//=============================================================================

// Selection types
struct NoSelection {};

struct ElementSelection {
    std::vector<ElementId> elements;  // ordered by selection time

    bool contains(ElementId id) const {
        for (auto e : elements) if (e == id) return true;
        return false;
    }

    void add(ElementId id) {
        if (!contains(id)) elements.push_back(id);
    }

    void remove(ElementId id) {
        auto it = std::find(elements.begin(), elements.end(), id);
        if (it != elements.end()) {
            elements.erase(it);
        }
    }

    void toggle(ElementId id) {
        if (contains(id)) remove(id);
        else add(id);
    }

    bool empty() const { return elements.empty(); }
    size_t count() const { return elements.size(); }
    ElementId primary() const { return elements.empty() ? INVALID_ELEMENT_ID : elements.back(); }
};

struct CellSelection {
    CellRange range;
    CellAddress active;  // the "anchor" cell within the range

    bool contains(const CellAddress& addr) const {
        return range.contains(addr);
    }

    bool isSingleCell() const {
        return range.start == range.end;
    }
};

struct TextSelection {
    ElementId elementId = INVALID_ELEMENT_ID;
    int start = 0;  // character index
    int end = 0;    // character index (start == end means cursor, no selection)

    bool hasSelection() const { return start != end; }
    bool isCursor() const { return start == end; }
    int length() const { return end > start ? end - start : start - end; }
};

//=============================================================================
// Selection — variant-based selection state
//=============================================================================
class Selection {
public:
    using State = std::variant<NoSelection, ElementSelection, CellSelection, TextSelection>;

    Selection() : _state(NoSelection{}) {}

    // Clear selection
    void clear() { _state = NoSelection{}; }

    // Check selection type
    bool isEmpty() const { return std::holds_alternative<NoSelection>(_state); }
    bool isElementSelection() const { return std::holds_alternative<ElementSelection>(_state); }
    bool isCellSelection() const { return std::holds_alternative<CellSelection>(_state); }
    bool isTextSelection() const { return std::holds_alternative<TextSelection>(_state); }

    // Access specific selection types (returns nullptr if wrong type)
    ElementSelection* asElements() {
        return std::get_if<ElementSelection>(&_state);
    }
    const ElementSelection* asElements() const {
        return std::get_if<ElementSelection>(&_state);
    }

    CellSelection* asCells() {
        return std::get_if<CellSelection>(&_state);
    }
    const CellSelection* asCells() const {
        return std::get_if<CellSelection>(&_state);
    }

    TextSelection* asText() {
        return std::get_if<TextSelection>(&_state);
    }
    const TextSelection* asText() const {
        return std::get_if<TextSelection>(&_state);
    }

    // Set selection
    void selectElement(ElementId id) {
        ElementSelection sel;
        sel.add(id);
        _state = sel;
    }

    void selectElements(const std::vector<ElementId>& ids) {
        ElementSelection sel;
        sel.elements = ids;
        _state = sel;
    }

    void selectCells(const CellRange& range, const CellAddress& active) {
        _state = CellSelection{range, active};
    }

    void selectCell(const CellAddress& addr) {
        _state = CellSelection{{addr, addr}, addr};
    }

    void selectText(ElementId elementId, int start, int end) {
        _state = TextSelection{elementId, start, end};
    }

    void setCursor(ElementId elementId, int position) {
        _state = TextSelection{elementId, position, position};
    }

    // Extend selection (shift+click behavior)
    void extendTo(ElementId id) {
        if (auto* sel = asElements()) {
            sel->add(id);
        } else {
            selectElement(id);
        }
    }

    void extendTo(const CellAddress& addr) {
        if (auto* sel = asCells()) {
            sel->range.end = addr;
        } else {
            selectCell(addr);
        }
    }

    void extendTo(int textPos) {
        if (auto* sel = asText()) {
            sel->end = textPos;
        }
    }

    // Raw state access for advanced use
    const State& state() const { return _state; }
    void setState(State s) { _state = std::move(s); }

private:
    State _state;
};

} // namespace yetty::yrich
