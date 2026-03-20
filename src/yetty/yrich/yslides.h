#pragma once

#include "yrich.h"
#include <vector>

namespace yetty::yrich {

//=============================================================================
// Shape types for slides
//=============================================================================

enum class ShapeType {
    Rectangle,
    Ellipse,
    TextBox,
    Line,
    Arrow,
    Image
};

//=============================================================================
// Shape — base class for slide elements
//=============================================================================
class Shape : public Element {
public:
    Shape(ElementId id, ShapeType type, float x, float y, float w, float h);

    // Element interface
    Rect bounds() const override { return _bounds; }
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // Shape properties
    ShapeType shapeType() const { return _shapeType; }

    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setBounds(const Rect& r) { _bounds = r; }

    // Fill and stroke
    Color fillColor() const { return _fillColor; }
    void setFillColor(Color c) { _fillColor = c; }

    Color strokeColor() const { return _strokeColor; }
    void setStrokeColor(Color c) { _strokeColor = c; }

    float strokeWidth() const { return _strokeWidth; }
    void setStrokeWidth(float w) { _strokeWidth = w; }

    // Rotation (degrees)
    float rotation() const { return _rotation; }
    void setRotation(float deg) { _rotation = deg; }

    // Corner radius (for rectangles)
    float cornerRadius() const { return _cornerRadius; }
    void setCornerRadius(float r) { _cornerRadius = r; }

protected:
    ShapeType _shapeType;
    Rect _bounds;
    Color _fillColor = Color::white();
    Color _strokeColor = Color::black();
    float _strokeWidth = 1.0f;
    float _rotation = 0.0f;
    float _cornerRadius = 0.0f;
};

using ShapePtr = std::shared_ptr<Shape>;

//=============================================================================
// TextBox — shape with editable text
//=============================================================================
class TextBox : public Shape {
public:
    TextBox(ElementId id, float x, float y, float w, float h);

    // Element interface
    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // Text methods (not inheriting from TextElement to avoid diamond)
    std::string text() const { return _text; }
    void setText(std::string_view text);
    TextStyle textStyle() const { return _style; }
    void setTextStyle(const TextStyle& style) { _style = style; }

    // Editing
    void beginEdit();
    void endEdit();
    bool isEditing() const { return _editing; }
    int cursorPosition() const { return _cursorPos; }
    void setCursorPosition(int pos);
    void insertText(std::string_view text);
    void deleteSelection();

    // Selection
    int selectionStart() const { return _selStart; }
    int selectionEnd() const { return _selEnd; }
    void setSelection(int start, int end);
    std::string selectedText() const;

    // Text alignment
    enum class Align { Left, Center, Right };
    enum class VAlign { Top, Middle, Bottom };

    Align align() const { return _align; }
    void setAlign(Align a) { _align = a; }

    VAlign valign() const { return _valign; }
    void setVAlign(VAlign v) { _valign = v; }

private:
    std::string _text;
    TextStyle _style;
    Align _align = Align::Center;
    VAlign _valign = VAlign::Middle;

    bool _editing = false;
    int _cursorPos = 0;
    int _selStart = 0;
    int _selEnd = 0;
};

using TextBoxPtr = std::shared_ptr<TextBox>;

//=============================================================================
// ImageShape — shape displaying an image
//=============================================================================
class ImageShape : public Shape {
public:
    ImageShape(ElementId id, float x, float y, float w, float h);

    void render(YDrawBuffer* buffer, uint32_t layer, bool selected) override;

    // Image data (base64 encoded or path)
    void setImageData(std::string_view data) { _imageData = data; }
    const std::string& imageData() const { return _imageData; }

    // Preserve aspect ratio
    bool preserveAspect() const { return _preserveAspect; }
    void setPreserveAspect(bool v) { _preserveAspect = v; }

private:
    std::string _imageData;
    bool _preserveAspect = true;
};

using ImageShapePtr = std::shared_ptr<ImageShape>;

//=============================================================================
// Slide — a single slide page
//=============================================================================
class Slide {
public:
    Slide(int index);

    int index() const { return _index; }
    void setIndex(int i) { _index = i; }

    // Shapes
    void addShape(ShapePtr shape);
    void removeShape(ElementId id);
    ShapePtr shapeAt(float x, float y) const;
    ShapePtr findShape(ElementId id) const;

    const std::vector<ShapePtr>& shapes() const { return _shapes; }

    // Background
    Color bgColor() const { return _bgColor; }
    void setBgColor(Color c) { _bgColor = c; }

    // Render all shapes
    void render(YDrawBuffer* buffer, uint32_t baseLayer);

private:
    int _index;
    std::vector<ShapePtr> _shapes;
    Color _bgColor = Color::white();
};

using SlidePtr = std::shared_ptr<Slide>;

//=============================================================================
// YSlides — presentation document
//=============================================================================
class YSlides : public Document,
                public base::ObjectFactory<YSlides> {
public:
    using Ptr = std::shared_ptr<YSlides>;

    static Result<Ptr> createImpl();

    const char* typeName() const override { return "YSlides"; }

    //=========================================================================
    // Slide dimensions
    //=========================================================================

    void setSlideSize(float w, float h) { _slideWidth = w; _slideHeight = h; }
    float slideWidth() const { return _slideWidth; }
    float slideHeight() const { return _slideHeight; }

    //=========================================================================
    // Slide management
    //=========================================================================

    SlidePtr addSlide();
    SlidePtr insertSlide(int index);
    void removeSlide(int index);
    void duplicateSlide(int index);

    SlidePtr slideAt(int index) const;
    int slideCount() const { return static_cast<int>(_slides.size()); }

    // Current slide
    int currentSlideIndex() const { return _currentSlide; }
    void setCurrentSlide(int index);
    SlidePtr currentSlide() const;

    // Navigation
    void nextSlide();
    void prevSlide();
    void goToSlide(int index);

    //=========================================================================
    // Shape creation on current slide
    //=========================================================================

    ShapePtr addRectangle(float x, float y, float w, float h);
    ShapePtr addEllipse(float x, float y, float w, float h);
    TextBoxPtr addTextBox(float x, float y, float w, float h, std::string_view text = "");
    ShapePtr addLine(float x1, float y1, float x2, float y2);
    ImageShapePtr addImage(float x, float y, float w, float h);

    //=========================================================================
    // Shape manipulation
    //=========================================================================

    void deleteSelectedShapes();
    void bringToFront();
    void sendToBack();
    void bringForward();
    void sendBackward();

    // Move selected shapes
    void moveSelection(float dx, float dy);

    // Resize selected shapes
    void resizeSelection(float dw, float dh);

    //=========================================================================
    // Selection
    //=========================================================================

    ShapePtr selectedShape() const;
    std::vector<ShapePtr> selectedShapes() const;
    void selectShape(ElementId id, bool add = false);
    void selectShapeAt(float x, float y, bool add = false);
    void selectAll();

    //=========================================================================
    // Editing mode
    //=========================================================================

    bool isEditingText() const { return _editingShape != nullptr; }
    TextBoxPtr editingTextBox() const;
    void startEditingText(ElementId id);
    void stopEditingText();

    //=========================================================================
    // Presentation mode
    //=========================================================================

    bool isPresentationMode() const { return _presentationMode; }
    void startPresentation(int fromSlide = 0);
    void stopPresentation();

    //=========================================================================
    // Document interface
    //=========================================================================

    float contentWidth() const override { return _slideWidth; }
    float contentHeight() const override { return _slideHeight; }

    void render() override;

    // Input handling
    void onMouseDown(float x, float y, MouseButton button, InputModifiers mods) override;
    void onMouseUp(float x, float y, MouseButton button, InputModifiers mods) override;
    void onMouseDrag(float x, float y, MouseButton button, InputModifiers mods) override;
    void onMouseDoubleClick(float x, float y, MouseButton button, InputModifiers mods) override;
    void onKeyDown(Key key, InputModifiers mods) override;
    void onTextInput(std::string_view text) override;

    // Clipboard
    std::string copySelection() const override;
    void paste(std::string_view data) override;
    void deleteSelection() override;

protected:
    YSlides() = default;

    void doApplyOperation(const Operation& op) override;

private:
    void renderSlide(SlidePtr slide);
    void renderThumbnails();
    void renderSelectionHandles();

    float _slideWidth = 960.0f;
    float _slideHeight = 540.0f;  // 16:9 aspect

    std::vector<SlidePtr> _slides;
    int _currentSlide = 0;

    // Editing state
    TextBoxPtr _editingShape;

    // Drag state
    bool _dragging = false;
    float _dragStartX = 0, _dragStartY = 0;
    float _dragOffsetX = 0, _dragOffsetY = 0;

    // Resize handles
    enum class Handle { None, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    Handle _activeHandle = Handle::None;

    // Presentation
    bool _presentationMode = false;
};

} // namespace yetty::yrich
