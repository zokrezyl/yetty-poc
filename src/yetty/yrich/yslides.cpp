#include "yslides.h"
#include "../ydraw/ydraw-buffer.h"
#include <algorithm>
#include <cmath>

namespace yetty::yrich {

//=============================================================================
// Shape
//=============================================================================

Shape::Shape(ElementId id, ShapeType type, float x, float y, float w, float h)
    : Element(id)
    , _shapeType(type)
    , _bounds{x, y, w, h}
{}

void Shape::setPosition(float x, float y) {
    _bounds.x = x;
    _bounds.y = y;
}

void Shape::setSize(float w, float h) {
    _bounds.w = w;
    _bounds.h = h;
}

void Shape::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    float cx = _bounds.x + _bounds.w / 2;
    float cy = _bounds.y + _bounds.h / 2;
    float hw = _bounds.w / 2;
    float hh = _bounds.h / 2;

    switch (_shapeType) {
        case ShapeType::Rectangle:
            buffer->addBox(layer, cx, cy, hw, hh,
                           _fillColor.toPacked(),
                           _strokeColor.toPacked(),
                           _strokeWidth, _cornerRadius);
            break;

        case ShapeType::Ellipse:
            // Approximate ellipse with high corner radius
            buffer->addBox(layer, cx, cy, hw, hh,
                           _fillColor.toPacked(),
                           _strokeColor.toPacked(),
                           _strokeWidth, std::min(hw, hh));
            break;

        case ShapeType::Line:
        case ShapeType::Arrow:
            buffer->addSegment(layer,
                               _bounds.x, _bounds.y,
                               _bounds.x + _bounds.w, _bounds.y + _bounds.h,
                               0, _strokeColor.toPacked(), _strokeWidth, 0);
            break;

        default:
            break;
    }

    // Selection outline
    if (selected) {
        buffer->addBox(layer + 1, cx, cy, hw + 2, hh + 2,
                       0,  // no fill
                       Color{0, 120, 215, 255}.toPacked(),
                       2.0f, 0);
    }
}

//=============================================================================
// TextBox
//=============================================================================

TextBox::TextBox(ElementId id, float x, float y, float w, float h)
    : Shape(id, ShapeType::TextBox, x, y, w, h)
{
    _style.fontSize = 24.0f;
    _style.color = Color::black();
}

void TextBox::setText(std::string_view text) {
    _text = text;
}

void TextBox::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    // Draw shape background
    Shape::render(buffer, layer, selected);

    if (_text.empty() && !_editing) return;

    // Calculate text position based on alignment
    float textX = _bounds.x + 8;
    float textY = _bounds.y + _bounds.h / 2;

    float textWidth = _text.size() * _style.fontSize * 0.6f;

    switch (_align) {
        case Align::Left:
            textX = _bounds.x + 8;
            break;
        case Align::Center:
            textX = _bounds.x + (_bounds.w - textWidth) / 2;
            break;
        case Align::Right:
            textX = _bounds.x + _bounds.w - textWidth - 8;
            break;
    }

    switch (_valign) {
        case VAlign::Top:
            textY = _bounds.y + _style.fontSize;
            break;
        case VAlign::Middle:
            textY = _bounds.y + _bounds.h / 2 + _style.fontSize * 0.35f;
            break;
        case VAlign::Bottom:
            textY = _bounds.y + _bounds.h - 8;
            break;
    }

    // Render text
    buffer->addText(textX, textY, _text, _style.fontSize,
                    _style.color.toPacked(), layer + 2, _style.fontId);

    // Cursor
    if (_editing) {
        float charWidth = _style.fontSize * 0.6f;
        float cursorX = textX + _cursorPos * charWidth;
        float cursorY1 = textY - _style.fontSize * 0.8f;
        float cursorY2 = textY + _style.fontSize * 0.2f;

        buffer->addSegment(layer + 3, cursorX, cursorY1, cursorX, cursorY2,
                           0, Color::black().toPacked(), 2.0f, 0);

        // Selection highlight
        if (_selStart != _selEnd) {
            int start = std::min(_selStart, _selEnd);
            int end = std::max(_selStart, _selEnd);
            float selX1 = textX + start * charWidth;
            float selX2 = textX + end * charWidth;

            buffer->addBox(layer + 1,
                           (selX1 + selX2) / 2, textY - _style.fontSize * 0.3f,
                           (selX2 - selX1) / 2, _style.fontSize * 0.6f,
                           Color{100, 150, 255, 100}.toPacked(),
                           0, 0, 0);
        }
    }
}

void TextBox::beginEdit() {
    _editing = true;
    _cursorPos = static_cast<int>(_text.size());
    _selStart = _selEnd = _cursorPos;
}

void TextBox::endEdit() {
    _editing = false;
}

void TextBox::setCursorPosition(int pos) {
    _cursorPos = std::clamp(pos, 0, static_cast<int>(_text.size()));
    _selStart = _selEnd = _cursorPos;
}

void TextBox::setSelection(int start, int end) {
    _selStart = std::clamp(start, 0, static_cast<int>(_text.size()));
    _selEnd = std::clamp(end, 0, static_cast<int>(_text.size()));
    _cursorPos = _selEnd;
}

std::string TextBox::selectedText() const {
    if (_selStart == _selEnd) return "";
    int start = std::min(_selStart, _selEnd);
    int end = std::max(_selStart, _selEnd);
    return _text.substr(start, end - start);
}

void TextBox::insertText(std::string_view text) {
    if (_selStart != _selEnd) {
        int start = std::min(_selStart, _selEnd);
        int end = std::max(_selStart, _selEnd);
        _text.erase(start, end - start);
        _cursorPos = start;
    }

    _text.insert(_cursorPos, text);
    _cursorPos += static_cast<int>(text.size());
    _selStart = _selEnd = _cursorPos;
}

void TextBox::deleteSelection() {
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
}

//=============================================================================
// ImageShape
//=============================================================================

ImageShape::ImageShape(ElementId id, float x, float y, float w, float h)
    : Shape(id, ShapeType::Image, x, y, w, h)
{}

void ImageShape::render(YDrawBuffer* buffer, uint32_t layer, bool selected) {
    float cx = _bounds.x + _bounds.w / 2;
    float cy = _bounds.y + _bounds.h / 2;
    float hw = _bounds.w / 2;
    float hh = _bounds.h / 2;

    // Placeholder for image - draw gray box with X
    buffer->addBox(layer, cx, cy, hw, hh,
                   Color{220, 220, 220, 255}.toPacked(),
                   _strokeColor.toPacked(),
                   _strokeWidth, 0);

    // Draw X to indicate image placeholder
    buffer->addSegment(layer + 1,
                       _bounds.x, _bounds.y,
                       _bounds.x + _bounds.w, _bounds.y + _bounds.h,
                       0, Color{150, 150, 150, 255}.toPacked(), 1.0f, 0);
    buffer->addSegment(layer + 1,
                       _bounds.x + _bounds.w, _bounds.y,
                       _bounds.x, _bounds.y + _bounds.h,
                       0, Color{150, 150, 150, 255}.toPacked(), 1.0f, 0);

    if (selected) {
        buffer->addBox(layer + 2, cx, cy, hw + 2, hh + 2,
                       0,
                       Color{0, 120, 215, 255}.toPacked(),
                       2.0f, 0);
    }
}

//=============================================================================
// Slide
//=============================================================================

Slide::Slide(int index) : _index(index) {}

void Slide::addShape(ShapePtr shape) {
    _shapes.push_back(shape);
}

void Slide::removeShape(ElementId id) {
    _shapes.erase(
        std::remove_if(_shapes.begin(), _shapes.end(),
                       [id](const ShapePtr& s) { return s->id() == id; }),
        _shapes.end());
}

ShapePtr Slide::shapeAt(float x, float y) const {
    // Reverse iterate to get topmost shape
    for (auto it = _shapes.rbegin(); it != _shapes.rend(); ++it) {
        const auto& shape = *it;
        const Rect& b = shape->bounds();
        if (x >= b.x && x <= b.x + b.w &&
            y >= b.y && y <= b.y + b.h) {
            return shape;
        }
    }
    return nullptr;
}

ShapePtr Slide::findShape(ElementId id) const {
    for (const auto& shape : _shapes) {
        if (shape->id() == id) return shape;
    }
    return nullptr;
}

void Slide::render(YDrawBuffer* buffer, uint32_t baseLayer) {
    // Background
    float cx = 480, cy = 270;  // Center of slide
    buffer->addBox(baseLayer, cx, cy, cx, cy,
                   _bgColor.toPacked(), 0, 0, 0);

    // Render shapes
    uint32_t layer = baseLayer + 1;
    for (const auto& shape : _shapes) {
        bool selected = false;  // Selection handled by YSlides
        shape->render(buffer, layer, selected);
        layer += 4;
    }
}

//=============================================================================
// YSlides
//=============================================================================

Result<YSlides::Ptr> YSlides::createImpl() {
    auto slides = Ptr(new YSlides());
    // Start with one blank slide
    slides->addSlide();
    return slides;
}

SlidePtr YSlides::addSlide() {
    auto slide = std::make_shared<Slide>(static_cast<int>(_slides.size()));
    _slides.push_back(slide);
    markDirty();
    return slide;
}

SlidePtr YSlides::insertSlide(int index) {
    if (index < 0) index = 0;
    if (index > static_cast<int>(_slides.size())) {
        index = static_cast<int>(_slides.size());
    }

    auto slide = std::make_shared<Slide>(index);
    _slides.insert(_slides.begin() + index, slide);

    // Update indices
    for (int i = index; i < static_cast<int>(_slides.size()); ++i) {
        _slides[i]->setIndex(i);
    }

    markDirty();
    return slide;
}

void YSlides::removeSlide(int index) {
    if (index < 0 || index >= static_cast<int>(_slides.size())) return;
    if (_slides.size() <= 1) return;  // Keep at least one slide

    _slides.erase(_slides.begin() + index);

    // Update indices
    for (int i = index; i < static_cast<int>(_slides.size()); ++i) {
        _slides[i]->setIndex(i);
    }

    if (_currentSlide >= static_cast<int>(_slides.size())) {
        _currentSlide = static_cast<int>(_slides.size()) - 1;
    }

    markDirty();
}

void YSlides::duplicateSlide(int index) {
    if (index < 0 || index >= static_cast<int>(_slides.size())) return;

    auto original = _slides[index];
    auto newSlide = insertSlide(index + 1);

    // Copy background
    newSlide->setBgColor(original->bgColor());

    // Note: Shape copying would need deep clone - simplified here
    markDirty();
}

SlidePtr YSlides::slideAt(int index) const {
    if (index < 0 || index >= static_cast<int>(_slides.size())) {
        return nullptr;
    }
    return _slides[index];
}

void YSlides::setCurrentSlide(int index) {
    if (index < 0) index = 0;
    if (index >= static_cast<int>(_slides.size())) {
        index = static_cast<int>(_slides.size()) - 1;
    }
    if (_currentSlide != index) {
        stopEditingText();
        _currentSlide = index;
        clearSelection();
        markDirty();
    }
}

SlidePtr YSlides::currentSlide() const {
    return slideAt(_currentSlide);
}

void YSlides::nextSlide() {
    setCurrentSlide(_currentSlide + 1);
}

void YSlides::prevSlide() {
    setCurrentSlide(_currentSlide - 1);
}

void YSlides::goToSlide(int index) {
    setCurrentSlide(index);
}

//=============================================================================
// Shape creation
//=============================================================================

ShapePtr YSlides::addRectangle(float x, float y, float w, float h) {
    auto slide = currentSlide();
    if (!slide) return nullptr;

    auto shape = std::make_shared<Shape>(nextElementId(), ShapeType::Rectangle, x, y, w, h);
    shape->setFillColor(Color{200, 200, 255, 255});
    shape->setStrokeColor(Color::black());
    slide->addShape(shape);
    addElement(shape);
    markDirty();
    return shape;
}

ShapePtr YSlides::addEllipse(float x, float y, float w, float h) {
    auto slide = currentSlide();
    if (!slide) return nullptr;

    auto shape = std::make_shared<Shape>(nextElementId(), ShapeType::Ellipse, x, y, w, h);
    shape->setFillColor(Color{255, 200, 200, 255});
    shape->setStrokeColor(Color::black());
    slide->addShape(shape);
    addElement(shape);
    markDirty();
    return shape;
}

TextBoxPtr YSlides::addTextBox(float x, float y, float w, float h, std::string_view text) {
    auto slide = currentSlide();
    if (!slide) return nullptr;

    auto shape = std::make_shared<TextBox>(nextElementId(), x, y, w, h);
    shape->setText(text);
    shape->setFillColor(Color::transparent());
    shape->setStrokeColor(Color::transparent());
    slide->addShape(shape);
    addElement(shape);
    markDirty();
    return shape;
}

ShapePtr YSlides::addLine(float x1, float y1, float x2, float y2) {
    auto slide = currentSlide();
    if (!slide) return nullptr;

    auto shape = std::make_shared<Shape>(nextElementId(), ShapeType::Line, x1, y1, x2 - x1, y2 - y1);
    shape->setStrokeColor(Color::black());
    shape->setStrokeWidth(2.0f);
    slide->addShape(shape);
    addElement(shape);
    markDirty();
    return shape;
}

ImageShapePtr YSlides::addImage(float x, float y, float w, float h) {
    auto slide = currentSlide();
    if (!slide) return nullptr;

    auto shape = std::make_shared<ImageShape>(nextElementId(), x, y, w, h);
    slide->addShape(shape);
    addElement(shape);
    markDirty();
    return shape;
}

//=============================================================================
// Shape manipulation
//=============================================================================

void YSlides::deleteSelectedShapes() {
    auto slide = currentSlide();
    if (!slide) return;

    auto& sel = selection();
    if (auto* elemSel = std::get_if<ElementSelection>(&sel.state())) {
        for (ElementId id : elemSel->elements) {
            slide->removeShape(id);
            removeElement(id);
        }
        clearSelection();
        markDirty();
    }
}

void YSlides::bringToFront() {
    auto slide = currentSlide();
    if (!slide) return;

    auto shape = selectedShape();
    if (!shape) return;

    auto& shapes = const_cast<std::vector<ShapePtr>&>(slide->shapes());
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end() && it != shapes.end() - 1) {
        shapes.erase(it);
        shapes.push_back(shape);
        markDirty();
    }
}

void YSlides::sendToBack() {
    auto slide = currentSlide();
    if (!slide) return;

    auto shape = selectedShape();
    if (!shape) return;

    auto& shapes = const_cast<std::vector<ShapePtr>&>(slide->shapes());
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end() && it != shapes.begin()) {
        shapes.erase(it);
        shapes.insert(shapes.begin(), shape);
        markDirty();
    }
}

void YSlides::bringForward() {
    auto slide = currentSlide();
    if (!slide) return;

    auto shape = selectedShape();
    if (!shape) return;

    auto& shapes = const_cast<std::vector<ShapePtr>&>(slide->shapes());
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end() && it + 1 != shapes.end()) {
        std::iter_swap(it, it + 1);
        markDirty();
    }
}

void YSlides::sendBackward() {
    auto slide = currentSlide();
    if (!slide) return;

    auto shape = selectedShape();
    if (!shape) return;

    auto& shapes = const_cast<std::vector<ShapePtr>&>(slide->shapes());
    auto it = std::find(shapes.begin(), shapes.end(), shape);
    if (it != shapes.end() && it != shapes.begin()) {
        std::iter_swap(it, it - 1);
        markDirty();
    }
}

void YSlides::moveSelection(float dx, float dy) {
    auto& sel = selection();
    if (auto* elemSel = std::get_if<ElementSelection>(&sel.state())) {
        for (ElementId id : elemSel->elements) {
            if (auto elem = findElement(id)) {
                if (auto shape = std::dynamic_pointer_cast<Shape>(elem)) {
                    shape->setPosition(shape->bounds().x + dx, shape->bounds().y + dy);
                }
            }
        }
        markDirty();
    }
}

void YSlides::resizeSelection(float dw, float dh) {
    auto shape = selectedShape();
    if (!shape) return;

    float newW = std::max(20.0f, shape->bounds().w + dw);
    float newH = std::max(20.0f, shape->bounds().h + dh);
    shape->setSize(newW, newH);
    markDirty();
}

//=============================================================================
// Selection
//=============================================================================

ShapePtr YSlides::selectedShape() const {
    auto& sel = selection();
    if (auto* elemSel = std::get_if<ElementSelection>(&sel.state())) {
        if (!elemSel->elements.empty()) {
            if (auto elem = findElement(elemSel->elements[0])) {
                return std::dynamic_pointer_cast<Shape>(elem);
            }
        }
    }
    return nullptr;
}

std::vector<ShapePtr> YSlides::selectedShapes() const {
    std::vector<ShapePtr> result;
    auto& sel = selection();
    if (auto* elemSel = std::get_if<ElementSelection>(&sel.state())) {
        for (ElementId id : elemSel->elements) {
            if (auto elem = findElement(id)) {
                if (auto shape = std::dynamic_pointer_cast<Shape>(elem)) {
                    result.push_back(shape);
                }
            }
        }
    }
    return result;
}

void YSlides::selectShape(ElementId id, bool add) {
    auto& sel = selection();

    if (add) {
        if (auto* elemSel = std::get_if<ElementSelection>(&sel.state())) {
            ElementSelection newSel = *elemSel;
            newSel.elements.push_back(id);
            sel.setState(newSel);
        } else {
            sel.setState(ElementSelection{{id}});
        }
    } else {
        sel.setState(ElementSelection{{id}});
    }
    markDirty();
}

void YSlides::selectShapeAt(float x, float y, bool add) {
    auto slide = currentSlide();
    if (!slide) return;

    auto shape = slide->shapeAt(x, y);
    if (shape) {
        selectShape(shape->id(), add);
    } else if (!add) {
        clearSelection();
    }
}

void YSlides::selectAll() {
    auto slide = currentSlide();
    if (!slide) return;

    ElementSelection elemSel;
    for (const auto& shape : slide->shapes()) {
        elemSel.elements.push_back(shape->id());
    }
    selection().setState(elemSel);
    markDirty();
}

//=============================================================================
// Editing
//=============================================================================

TextBoxPtr YSlides::editingTextBox() const {
    return _editingShape;
}

void YSlides::startEditingText(ElementId id) {
    stopEditingText();

    if (auto elem = findElement(id)) {
        if (auto textBox = std::dynamic_pointer_cast<TextBox>(elem)) {
            _editingShape = textBox;
            textBox->beginEdit();
            markDirty();
        }
    }
}

void YSlides::stopEditingText() {
    if (_editingShape) {
        _editingShape->endEdit();
        _editingShape = nullptr;
        markDirty();
    }
}

//=============================================================================
// Presentation mode
//=============================================================================

void YSlides::startPresentation(int fromSlide) {
    _presentationMode = true;
    setCurrentSlide(fromSlide);
    clearSelection();
    stopEditingText();
    markDirty();
}

void YSlides::stopPresentation() {
    _presentationMode = false;
    markDirty();
}

//=============================================================================
// Rendering
//=============================================================================

void YSlides::render() {
    if (!_buffer) return;

    _buffer->clear();
    _buffer->setSceneBounds(0, 0, _slideWidth, _slideHeight);
    _buffer->setBgColor(Color{80, 80, 80, 255}.toPacked());

    auto slide = currentSlide();
    if (slide) {
        renderSlide(slide);
    }

    // Selection handles
    if (!_presentationMode) {
        renderSelectionHandles();
    }

    _dirty = false;
}

void YSlides::renderSlide(SlidePtr slide) {
    // Slide background
    float cx = _slideWidth / 2;
    float cy = _slideHeight / 2;

    _buffer->addBox(0, cx, cy, cx, cy,
                    slide->bgColor().toPacked(),
                    Color{200, 200, 200, 255}.toPacked(),
                    1.0f, 0);

    // Render shapes
    uint32_t layer = 1;
    auto selected = selectedShapes();
    for (const auto& shape : slide->shapes()) {
        bool isSelected = std::find(selected.begin(), selected.end(), shape) != selected.end();
        shape->render(_buffer, layer, isSelected);
        layer += 4;
    }
}

void YSlides::renderSelectionHandles() {
    auto shape = selectedShape();
    if (!shape || _editingShape) return;

    const Rect& b = shape->bounds();
    float handleSize = 6.0f;

    // Corner handles
    auto drawHandle = [&](float x, float y) {
        _buffer->addBox(100, x, y, handleSize / 2, handleSize / 2,
                        Color::white().toPacked(),
                        Color{0, 120, 215, 255}.toPacked(),
                        1.5f, 0);
    };

    drawHandle(b.x, b.y);
    drawHandle(b.x + b.w, b.y);
    drawHandle(b.x, b.y + b.h);
    drawHandle(b.x + b.w, b.y + b.h);

    // Edge handles
    drawHandle(b.x + b.w / 2, b.y);
    drawHandle(b.x + b.w / 2, b.y + b.h);
    drawHandle(b.x, b.y + b.h / 2);
    drawHandle(b.x + b.w, b.y + b.h / 2);
}

//=============================================================================
// Input handling
//=============================================================================

void YSlides::onMouseDown(float x, float y, MouseButton button, InputModifiers mods) {
    if (button != MouseButton::Left) return;

    if (_presentationMode) {
        nextSlide();
        return;
    }

    // Check for editing text box
    if (_editingShape) {
        const Rect& b = _editingShape->bounds();
        if (x < b.x || x > b.x + b.w || y < b.y || y > b.y + b.h) {
            stopEditingText();
        }
    }

    // Select shape at position
    selectShapeAt(x, y, mods.shift);

    auto shape = selectedShape();
    if (shape) {
        _dragging = true;
        _dragStartX = x;
        _dragStartY = y;
        _dragOffsetX = x - shape->bounds().x;
        _dragOffsetY = y - shape->bounds().y;
    }

    markDirty();
}

void YSlides::onMouseUp(float x, float y, MouseButton button, InputModifiers mods) {
    _dragging = false;
    _activeHandle = Handle::None;
}

void YSlides::onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) {
    if (!_dragging) return;

    auto shape = selectedShape();
    if (!shape) return;

    float newX = x - _dragOffsetX;
    float newY = y - _dragOffsetY;

    shape->setPosition(newX, newY);
    markDirty();
}

void YSlides::onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) {
    if (_presentationMode) return;

    auto slide = currentSlide();
    if (!slide) return;

    auto shape = slide->shapeAt(x, y);
    if (shape) {
        if (auto textBox = std::dynamic_pointer_cast<TextBox>(shape)) {
            startEditingText(textBox->id());
        }
    }
}

void YSlides::onKeyDown(Key key, InputModifiers mods) {
    if (_presentationMode) {
        switch (key) {
            case Key::Escape:
                stopPresentation();
                break;
            case Key::Right:
            case Key::Down:
                nextSlide();
                break;
            case Key::Left:
            case Key::Up:
                prevSlide();
                break;
            default:
                break;
        }
        return;
    }

    if (_editingShape) {
        switch (key) {
            case Key::Escape:
                stopEditingText();
                break;
            case Key::Left:
                _editingShape->setCursorPosition(_editingShape->cursorPosition() - 1);
                markDirty();
                break;
            case Key::Right:
                _editingShape->setCursorPosition(_editingShape->cursorPosition() + 1);
                markDirty();
                break;
            case Key::Backspace:
                _editingShape->deleteSelection();
                markDirty();
                break;
            case Key::Delete:
                if (_editingShape->cursorPosition() < static_cast<int>(_editingShape->text().size())) {
                    _editingShape->setCursorPosition(_editingShape->cursorPosition() + 1);
                    _editingShape->deleteSelection();
                }
                markDirty();
                break;
            default:
                break;
        }
        return;
    }

    switch (key) {
        case Key::Delete:
        case Key::Backspace:
            deleteSelectedShapes();
            break;
        case Key::Left:
            moveSelection(-10, 0);
            break;
        case Key::Right:
            moveSelection(10, 0);
            break;
        case Key::Up:
            moveSelection(0, -10);
            break;
        case Key::Down:
            moveSelection(0, 10);
            break;
        case Key::Escape:
            clearSelection();
            markDirty();
            break;
        default:
            Document::onKeyDown(key, mods);
            break;
    }
}

void YSlides::onTextInput(std::string_view text) {
    if (_editingShape) {
        _editingShape->insertText(text);
        markDirty();
    }
}

//=============================================================================
// Clipboard
//=============================================================================

std::string YSlides::copySelection() const {
    // TODO: Serialize selected shapes
    return "";
}

void YSlides::paste(std::string_view data) {
    // TODO: Deserialize and add shapes
}

void YSlides::deleteSelection() {
    deleteSelectedShapes();
}

//=============================================================================
// Operations
//=============================================================================

void YSlides::doApplyOperation(const Operation& op) {
    // TODO: Handle slide-specific operations
    Document::doApplyOperation(op);
}

} // namespace yetty::yrich
