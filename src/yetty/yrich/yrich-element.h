#pragma once

#include "yrich-types.h"
#include <memory>
#include <string>
#include <vector>

namespace yetty {
class YDrawBuffer;
}

namespace yetty::yrich {

class Element;
using ElementPtr = std::shared_ptr<Element>;

//=============================================================================
// Element — base class for all document elements
//
// Elements are the building blocks of YRich documents:
// - Spreadsheet: cells, merged regions
// - Slides: shapes, text boxes, images
// - Docs: paragraphs, inline elements
//
// Each element:
// - Has a unique ID within the document
// - Knows its bounding box
// - Can render itself to YDrawBuffer
// - Can be selected, edited, moved
//=============================================================================
class Element : public std::enable_shared_from_this<Element> {
public:
    explicit Element(ElementId id) : _id(id) {}
    virtual ~Element() = default;

    // Identity
    ElementId id() const { return _id; }

    // Geometry
    virtual Rect bounds() const = 0;
    virtual bool hitTest(float x, float y) const { return bounds().contains(x, y); }

    // Selection state (managed by document, not stored here)
    // Elements render differently when selected

    // Rendering — element draws itself to the buffer
    // layer: base layer for this element
    // selected: whether to render selection indicators
    virtual void render(YDrawBuffer* buffer, uint32_t layer, bool selected) = 0;

    // Editing — element-specific edit mode
    virtual bool isEditable() const { return false; }
    virtual void beginEdit() {}
    virtual void endEdit() {}
    virtual bool isEditing() const { return false; }

    // Text input during edit mode
    virtual void insertText(std::string_view text) {}
    virtual void deleteSelection() {}

    // Serialization (for undo/redo and persistence)
    virtual std::string serialize() const { return {}; }
    virtual void deserialize(std::string_view data) {}

protected:
    ElementId _id;
};

//=============================================================================
// TextElement — element that contains editable text
//=============================================================================
class TextElement : public Element {
public:
    using Element::Element;

    // Text content
    virtual std::string text() const = 0;
    virtual void setText(std::string_view text) = 0;

    // Text style
    virtual TextStyle textStyle() const = 0;
    virtual void setTextStyle(const TextStyle& style) = 0;

    // Cursor position within text (for editing)
    virtual int cursorPosition() const { return 0; }
    virtual void setCursorPosition(int pos) {}

    // Text selection within element
    virtual int selectionStart() const { return 0; }
    virtual int selectionEnd() const { return 0; }
    virtual void setSelection(int start, int end) {}
    virtual std::string selectedText() const { return {}; }

    bool isEditable() const override { return true; }
};

//=============================================================================
// ShapeElement — element with geometric shape (for slides)
//=============================================================================
class ShapeElement : public Element {
public:
    using Element::Element;

    // Shape geometry
    virtual void setPosition(float x, float y) = 0;
    virtual void setSize(float w, float h) = 0;

    // Style
    virtual Color fillColor() const = 0;
    virtual void setFillColor(const Color& color) = 0;
    virtual BorderStyle borderStyle() const = 0;
    virtual void setBorderStyle(const BorderStyle& style) = 0;

    // Resize handles
    virtual std::vector<Rect> resizeHandles() const { return {}; }
    virtual int hitTestHandle(float x, float y) const { return -1; }
};

} // namespace yetty::yrich
