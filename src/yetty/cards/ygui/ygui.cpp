#include "ygui.h"
#include "../hdraw/hdraw.h" // For SDFPrimitive, SDFType
#include <cmath>
#include <cstring>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <yetty/base/event-loop.h>
#include <yetty/osc-command.h>
#include <yetty/ydraw-builder.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

//=============================================================================
// YGuiImpl
//=============================================================================

class YGuiImpl : public YGui {
public:
  YGuiImpl(const YettyContext &ctx, int32_t x, int32_t y, uint32_t widthCells,
           uint32_t heightCells, const std::string &args,
           const std::string &payload)
      : YGui(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells),
        _screenId(ctx.screenId), _argsStr(args), _payloadStr(payload) {
    _shaderGlyph = SHADER_GLYPH;

    // Create YDrawBuilder for proper text/primitive rendering
    auto builderRes =
        YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
    if (builderRes) {
      _builder = *builderRes;
    } else {
      yerror("YGuiImpl: failed to create builder");
    }
  }

  ~YGuiImpl() override { dispose(); }

  uint32_t metadataSlotIndex() const override {
    return _metaHandle.offset / 64;
  }

  //=========================================================================
  // Lifecycle
  //=========================================================================

  Result<void> init() {
    if (!_builder) {
      return Err<void>("YGui: builder not available");
    }

    // Allocate metadata
    auto metaRes = _cardMgr->allocateMetadata(64);
    if (!metaRes)
      return Err<void>("YGui: failed to allocate metadata");
    _metaHandle = *metaRes;

    // Parse widget tree from payload (YAML)
    if (!_payloadStr.empty()) {
      parseWidgets(_payloadStr);
    }

    yinfo("YGui::init: parsed {} widgets", _widgets.size());

    // Calculate pixel dimensions (use cell sizes approx 10x20)
    _pixelWidth = static_cast<float>(_widthCells * 10);
    _pixelHeight = static_cast<float>(_heightCells * 20);

    yinfo("YGui::init: size {}x{} cells, {}x{} pixels", _widthCells,
          _heightCells, _pixelWidth, _pixelHeight);

    // Set scene bounds for builder
    _builder->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
    _builder->setBgColor(0xFF1A1A2E); // Dark theme background

    // Build initial primitives
    rebuildPrimitives();

    yinfo("YGui::init: built {} prims, {} glyphs", _builder->primitiveCount(),
          _builder->glyphCount());

    // Register for events
    if (auto res = registerForEvents(); !res) {
      yerror("YGui::init: failed to register events: {}", error_msg(res));
    }

    return Ok();
  }

  Result<void> dispose() override {
    deregisterFromEvents();
    _primStorage = StorageHandle::invalid();
    _derivedStorage = StorageHandle::invalid();
    if (_metaHandle.isValid() && _cardMgr) {
      _cardMgr->deallocateMetadata(_metaHandle);
      _metaHandle = MetadataHandle::invalid();
    }
    return Ok();
  }

  void suspend() override {
    _primStorage = StorageHandle::invalid();
    _derivedStorage = StorageHandle::invalid();
  }

  //=========================================================================
  // Buffer management
  //=========================================================================

  void declareBufferNeeds() override {
    if (!_builder)
      return;

    _primStorage = StorageHandle::invalid();
    _derivedStorage = StorageHandle::invalid();

    const auto &primStaging = _builder->primStaging();
    const auto &glyphs = _builder->glyphs();

    if (primStaging.empty() && glyphs.empty()) {
      _builder->clearGridStaging();
      return;
    }

    // Reserve prim storage
    if (!primStaging.empty()) {
      uint32_t primSize =
          static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
      _cardMgr->bufferManager()->reserve(primSize);
    }

    // Calculate grid
    if (_builder->gridStaging().empty()) {
      _builder->calculate();
    }

    // Reserve derived storage (grid + glyphs)
    uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) *
                         sizeof(uint32_t);
    uint32_t glyphBytes =
        static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
    uint32_t derivedSize = gridBytes + glyphBytes;

    if (derivedSize > 0) {
      _cardMgr->bufferManager()->reserve(derivedSize);
    }
  }

  Result<void> allocateBuffers() override {
    if (!_builder)
      return Ok();

    const auto &primStaging = _builder->primStaging();
    const auto &gridStaging = _builder->gridStaging();
    const auto &glyphs = _builder->glyphs();

    // Allocate primitives
    if (!primStaging.empty()) {
      uint32_t count = static_cast<uint32_t>(primStaging.size());
      uint32_t allocBytes = count * sizeof(SDFPrimitive);
      auto primResult = _cardMgr->bufferManager()->allocateBuffer(
          metadataSlotIndex(), "prims", allocBytes);
      if (!primResult) {
        return Err<void>("YGui: prim alloc failed");
      }
      _primStorage = *primResult;
      std::memcpy(_primStorage.data, primStaging.data(),
                  count * sizeof(SDFPrimitive));
      _primCount = count;
      _cardMgr->bufferManager()->markBufferDirty(_primStorage);
    }

    // Allocate derived storage (grid + glyphs)
    uint32_t gridBytes =
        static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
    uint32_t glyphBytes =
        static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
    uint32_t derivedSize = gridBytes + glyphBytes;

    if (derivedSize > 0) {
      auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
          metadataSlotIndex(), "derived", derivedSize);
      if (!storageResult) {
        return Err<void>("YGui: derived alloc failed");
      }
      _derivedStorage = *storageResult;

      uint8_t *base = _derivedStorage.data;
      uint32_t offset = 0;

      // Copy grid
      _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
      if (!gridStaging.empty()) {
        std::memcpy(base + offset, gridStaging.data(), gridBytes);
      }
      offset += gridBytes;

      // Copy glyphs
      _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
      if (!glyphs.empty()) {
        std::memcpy(base + offset, glyphs.data(), glyphBytes);
      }

      _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }

    _metadataDirty = true;
    return Ok();
  }

  bool needsBufferRealloc() override {
    if (!_builder)
      return false;
    const auto &primStaging = _builder->primStaging();
    // Need realloc if no storage or storage too small
    if (!primStaging.empty() && !_primStorage.isValid())
      return true;
    if (_primStorage.isValid()) {
      uint32_t needed =
          static_cast<uint32_t>(primStaging.size() * sizeof(SDFPrimitive));
      if (needed > _primStorage.size)
        return true;
    }
    return false;
  }

  //=========================================================================
  // Rendering
  //=========================================================================

  Result<void> finalize() override {
    if (!_builder)
      return Ok();

    if (_dirty) {
      rebuildPrimitives();
      _dirty = false;
      _metadataDirty = true;
    }

    if (_metadataDirty) {
      if (auto res = uploadMetadata(); !res)
        return res;
      _metadataDirty = false;
    }

    return Ok();
  }

  //=========================================================================
  // Cell size for coordinate conversion
  //=========================================================================

  void setCellSize(float cellWidth, float cellHeight) override {
    _cellWidthF = cellWidth;
    _cellHeightF = cellHeight;
    // Keep scene bounds in YAML/widget coordinate space (10x20 cell size)
    // Don't update _pixelWidth/_pixelHeight or scene bounds here
    // The coordinate conversion in toWidgetX/toWidgetY handles the mapping
    _dirty = true;
  }

  //=========================================================================
  // Event handling - receives pixel coordinates directly
  //=========================================================================

  // Convert actual pixel coordinates to nominal widget coordinates
  // (widgets are defined assuming 10x20 cell size)
  float toWidgetX(float px) const { return px * 10.0f / _cellWidthF; }
  float toWidgetY(float py) const { return py * 20.0f / _cellHeightF; }

  void handleMouseMove(float px, float py) {
    // Convert to widget coordinate system
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    // Update hover state
    for (auto &w : _widgets) {
      updateHoverRecursive(w, wx, wy);
    }

    // Track slider/scrollbar drag while pressed
    if (_pressed) {
      if (_pressed->type == WidgetType::Slider) {
        updateSliderValue(_pressed, wx);
      } else if (_pressed->type == WidgetType::VScrollbar) {
        updateVScrollValue(_pressed, wy);
      } else if (_pressed->type == WidgetType::HScrollbar) {
        updateHScrollValue(_pressed, wx);
      }
    }

    // Handle ScrollArea scrollbar drag
    if (_scrollDragWidget) {
      Widget *w = _scrollDragWidget;
      float scrollbarW = 12;
      float scrollbarH = 12;
      float viewW = w->w - scrollbarW;
      float viewH = w->h - scrollbarH;
      float contentW = w->contentW > 0 ? w->contentW : viewW;
      float contentH = w->contentH > 0 ? w->contentH : viewH;

      if (_scrollDragVertical && contentH > viewH) {
        float maxScrollY = contentH - viewH;
        float trackH = w->h - scrollbarH;
        float thumbH = std::max(20.0f, trackH * viewH / contentH);
        float localY = wy - w->y;
        float clickRatio = (localY - thumbH / 2) / (trackH - thumbH);
        w->scrollY = std::clamp(clickRatio * maxScrollY, 0.0f, maxScrollY);
      } else if (!_scrollDragVertical && contentW > viewW) {
        float maxScrollX = contentW - viewW;
        float trackW = w->w - scrollbarW;
        float thumbW = std::max(20.0f, trackW * viewW / contentW);
        float localX = wx - w->x;
        float clickRatio = (localX - thumbW / 2) / (trackW - thumbW);
        w->scrollX = std::clamp(clickRatio * maxScrollX, 0.0f, maxScrollX);
      }
      _dirty = true;
    }

    // Update hover option for open dropdown
    if (_openDropdown && _openDropdown->isOpen()) {
      _hoverOptionIdx = getDropdownOptionAt(_openDropdown, wx, wy);
      ydebug("YGui::handleMouseMove dropdown open, px={:.1f} py={:.1f} -> "
             "wx={:.1f} wy={:.1f} hoverIdx={}",
             px, py, wx, wy, _hoverOptionIdx);
    }

    // Update hover for choicebox options
    _hoverChoiceIdx = -1;
    _hoverChoiceWidget = nullptr;
    for (auto &w : _widgets) {
      if (w->type == WidgetType::ChoiceBox && w->isHover()) {
        int idx = getChoiceBoxOptionAt(w.get(), wx, wy);
        if (idx >= 0) {
          _hoverChoiceIdx = idx;
          _hoverChoiceWidget = w.get();
        }
        break;
      }
    }

    // Update hover for listbox items
    _hoverListIdx = -1;
    _hoverListWidget = nullptr;
    for (auto &w : _widgets) {
      auto hit = findListBoxAt(w, wx, wy);
      if (hit.widget) {
        Widget *listbox = hit.widget;
        float itemH = listbox->rowHeight > 0 ? listbox->rowHeight : 24;
        // hitY is in content coordinates, listbox->y is widget position
        float localY = hit.hitY - listbox->y + listbox->scrollY;
        int idx = static_cast<int>(localY / itemH);
        if (idx >= 0 && idx < static_cast<int>(listbox->options.size())) {
          _hoverListIdx = idx;
          _hoverListWidget = listbox;
        }
        break;
      }
    }

    _dirty = true;
  }

  void handleMouseDown(float px, float py, int button) {
    if (button != 0)
      return; // Left click only

    // Convert to widget coordinate system
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    ydebug("YGui::handleMouseDown px={:.1f} py={:.1f} -> wx={:.1f} wy={:.1f}",
           px, py, wx, wy);

    // Handle dropdown: check if clicking on open dropdown's options
    if (_openDropdown && _openDropdown->isOpen()) {
      int optionIdx = getDropdownOptionAt(_openDropdown, wx, wy);
      if (optionIdx >= 0) {
        ydebug("YGui: selected dropdown option {}", optionIdx);
        _openDropdown->selectedIndex = optionIdx;
        _openDropdown->flags &= ~WIDGET_OPEN;
        emitEvent(_openDropdown, "change");
        _openDropdown = nullptr;
        _dirty = true;
        return;
      }
      // Check if clicking on the dropdown button itself (to close it)
      if (_openDropdown->contains(wx, wy)) {
        ydebug("YGui: closing dropdown by clicking on button");
        _openDropdown->flags &= ~WIDGET_OPEN;
        _openDropdown = nullptr;
        _dirty = true;
        return; // Don't reopen it
      }
      // Clicked outside dropdown and button - close it
      _openDropdown->flags &= ~WIDGET_OPEN;
      _openDropdown = nullptr;
      _dirty = true;
    }

    // Find clicked widget (returns widget and effective hit coordinates)
    HitResult hit = {nullptr, 0, 0};
    for (auto &w : _widgets) {
      hit = findWidgetAt(w, wx, wy);
      if (hit.widget) {
        ydebug("YGui: clicked widget id={} type={} at hitX={:.1f} hitY={:.1f}",
               hit.widget->id, static_cast<int>(hit.widget->type), hit.hitX,
               hit.hitY);
        break;
      }
    }

    Widget *clicked = hit.widget;
    float hitX = hit.hitX; // Use these for position-dependent click handling
    float hitY = hit.hitY;

    if (!clicked)
      return;

    // Update focus
    if (_focused != clicked) {
      if (_focused)
        _focused->flags &= ~WIDGET_FOCUSED;
      _focused = clicked;
      _focused->flags |= WIDGET_FOCUSED;
    }

    clicked->flags |= WIDGET_PRESSED;
    _pressed = clicked;

    // Handle widget-specific click
    switch (clicked->type) {
    case WidgetType::Button:
      // Click handled on mouse up
      break;

    case WidgetType::Checkbox:
      clicked->flags ^= WIDGET_CHECKED;
      emitEvent(clicked, "change");
      break;

    case WidgetType::Dropdown:
      clicked->flags ^= WIDGET_OPEN;
      if (clicked->isOpen()) {
        _openDropdown = clicked;
        _hoverOptionIdx = -1; // Reset hover, will update on mouse move
      } else {
        _openDropdown = nullptr;
      }
      ydebug("YGui: dropdown {} open={}", clicked->id, clicked->isOpen());
      break;

    case WidgetType::Slider:
      updateSliderValue(clicked, hitX);
      break;

    case WidgetType::VScrollbar:
      updateVScrollValue(clicked, hitY);
      break;

    case WidgetType::HScrollbar:
      updateHScrollValue(clicked, hitX);
      break;

    case WidgetType::ChoiceBox: {
      // Find which radio button was clicked
      float optH = 24;
      int optIdx = static_cast<int>((hitY - clicked->y) / optH);
      if (optIdx >= 0 && optIdx < static_cast<int>(clicked->options.size())) {
        clicked->selectedIndex = optIdx;
        emitEvent(clicked, "change");
      }
      break;
    }

    case WidgetType::Selectable:
      clicked->flags ^= WIDGET_CHECKED;
      emitEvent(clicked, "click");
      break;

    case WidgetType::CollapsingHeader:
      clicked->flags ^= WIDGET_OPEN;
      emitEvent(clicked, "click");
      break;

    case WidgetType::ScrollArea: {
      // Check if clicking on scrollbars
      float scrollbarW = 12;
      float scrollbarH = 12;
      float viewW = clicked->w - scrollbarW;
      float viewH = clicked->h - scrollbarH;
      float contentW = clicked->contentW > 0 ? clicked->contentW : viewW;
      float contentH = clicked->contentH > 0 ? clicked->contentH : viewH;

      float localX = hitX - clicked->x;
      float localY = hitY - clicked->y;

      // Vertical scrollbar region
      if (localX >= viewW && contentH > viewH) {
        float maxScrollY = contentH - viewH;
        float trackH = clicked->h - scrollbarH;
        float thumbH = std::max(20.0f, trackH * viewH / contentH);
        float clickRatio = (localY - thumbH / 2) / (trackH - thumbH);
        clicked->scrollY =
            std::clamp(clickRatio * maxScrollY, 0.0f, maxScrollY);
        _scrollDragWidget = clicked;
        _scrollDragVertical = true;
        _dirty = true;
        break;
      }

      // Horizontal scrollbar region
      if (localY >= viewH && contentW > viewW) {
        float maxScrollX = contentW - viewW;
        float trackW = clicked->w - scrollbarW;
        float thumbW = std::max(20.0f, trackW * viewW / contentW);
        float clickRatio = (localX - thumbW / 2) / (trackW - thumbW);
        clicked->scrollX =
            std::clamp(clickRatio * maxScrollX, 0.0f, maxScrollX);
        _scrollDragWidget = clicked;
        _scrollDragVertical = false;
        _dirty = true;
        break;
      }
      break;
    }

    case WidgetType::TabBar: {
      // Find which tab was clicked
      float tabX = clicked->x + 4;
      int tabIdx = 0;
      for (auto &child : clicked->children) {
        float tabW = child->w > 0 ? child->w : 80;
        if (hitX >= tabX && hitX < tabX + tabW) {
          clicked->activeTab = tabIdx;
          emitEvent(clicked, "change");
          break;
        }
        tabX += tabW + 4;
        tabIdx++;
      }
      break;
    }

    case WidgetType::Popup: {
      // Toggle popup open state (or close button handling)
      clicked->flags ^= WIDGET_OPEN;
      emitEvent(clicked, "click");
      break;
    }

    case WidgetType::ListBox: {
      // Find which item was clicked
      float itemH = clicked->rowHeight;
      float localY = hitY - clicked->y + clicked->scrollY;
      int itemIdx = static_cast<int>(localY / itemH);
      if (itemIdx >= 0 && itemIdx < static_cast<int>(clicked->options.size())) {
        clicked->selectedIndex = itemIdx;
        emitEvent(clicked, "change");
      }
      break;
    }

    case WidgetType::TableRow: {
      // Row click - could be used for selection
      clicked->flags ^= WIDGET_CHECKED;
      emitEvent(clicked, "click");
      break;
    }

    default:
      break;
    }

    _dirty = true;
  }

  void handleMouseUp(float px, float py, int button) {
    if (button != 0)
      return;

    // Convert to widget coordinate system
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    if (_pressed) {
      if (_pressed->type == WidgetType::Button && _pressed->contains(wx, wy)) {
        emitEvent(_pressed, "click");
      }
      _pressed->flags &= ~WIDGET_PRESSED;
      _pressed = nullptr;
      _dirty = true;
    }

    // Clear ScrollArea drag state
    _scrollDragWidget = nullptr;
  }

  void handleMouseScroll(float px, float py, float deltaX, float deltaY) {
    (void)deltaX;

    // Convert to widget coordinate system
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    // Find widget under cursor
    Widget *target = nullptr;
    for (auto &w : _widgets) {
      auto hit = findWidgetAt(w, wx, wy);
      if (hit.widget) {
        target = hit.widget;
        break;
      }
    }

    if (target) {
      if (target->type == WidgetType::Slider) {
        float range = target->maxValue - target->minValue;
        float delta = deltaY * range * 0.05f;
        target->value = std::clamp(target->value + delta, target->minValue,
                                   target->maxValue);
        emitEvent(target, "change");
        _dirty = true;
      } else if (target->type == WidgetType::ScrollArea) {
        // Scroll the scroll area
        float scrollbarW = 12;
        float scrollbarH = 12;
        float viewW = target->w - scrollbarW;
        float viewH = target->h - scrollbarH;
        float contentW = target->contentW > 0 ? target->contentW : viewW;
        float contentH = target->contentH > 0 ? target->contentH : viewH;
        float maxScrollX = std::max(0.0f, contentW - viewW);
        float maxScrollY = std::max(0.0f, contentH - viewH);

        // Scroll 20 pixels per wheel tick
        target->scrollY =
            std::clamp(target->scrollY - deltaY * 20.0f, 0.0f, maxScrollY);
        target->scrollX =
            std::clamp(target->scrollX - deltaX * 20.0f, 0.0f, maxScrollX);
        emitEvent(target, "scroll");
        _dirty = true;
      } else if (target->type == WidgetType::ListBox) {
        // Scroll the listbox
        float itemH = target->rowHeight;
        float contentH = target->options.size() * itemH;
        float maxScroll = std::max(0.0f, contentH - target->h);
        target->scrollY =
            std::clamp(target->scrollY - deltaY * itemH, 0.0f, maxScroll);
        _dirty = true;
      }
    }
  }

  void handleKeyDown(uint32_t key, int mods) {
    if (!_focused)
      return;

    if (_focused->type == WidgetType::TextInput) {
      handleTextInput(_focused, key, mods);
      _dirty = true;
    }
  }

  //=========================================================================
  // Update command
  //=========================================================================

  Result<void> update(const std::string &args,
                      const std::string &payload) override {
    // Parse args: widget=id property=value
    std::istringstream iss(args);
    std::string token;
    std::string widgetId;
    std::string property;
    std::string value;

    while (iss >> token) {
      auto eq = token.find('=');
      if (eq != std::string::npos) {
        std::string key = token.substr(0, eq);
        std::string val = token.substr(eq + 1);
        if (key == "widget" || key == "id")
          widgetId = val;
        else if (key == "property" || key == "prop")
          property = val;
        else if (key == "value" || key == "val")
          value = val;
      }
    }

    if (widgetId.empty()) {
      return Err<void>("YGui::update: missing widget id");
    }

    // Find widget
    Widget *w = findWidgetById(widgetId);
    if (!w) {
      return Err<void>("YGui::update: widget not found: " + widgetId);
    }

    // Update property
    if (property == "value") {
      w->value = std::stof(value);
    } else if (property == "text" || property == "label") {
      w->label = value;
    } else if (property == "checked") {
      if (value == "true" || value == "1")
        w->flags |= WIDGET_CHECKED;
      else
        w->flags &= ~WIDGET_CHECKED;
    } else if (property == "disabled") {
      if (value == "true" || value == "1")
        w->flags |= WIDGET_DISABLED;
      else
        w->flags &= ~WIDGET_DISABLED;
    } else if (property == "selected") {
      w->selectedIndex = std::stoi(value);
    }

    _dirty = true;
    return Ok();
  }

private:
  //=========================================================================
  // Widget parsing from YAML
  //=========================================================================

  void parseWidgets(const std::string &yaml) {
    try {
      YAML::Node root = YAML::Load(yaml);
      if (root["widgets"]) {
        for (const auto &node : root["widgets"]) {
          auto w = parseWidget(node);
          if (w)
            _widgets.push_back(w);
        }
      }
    } catch (const std::exception &e) {
      yerror("YGui: YAML parse error: {}", e.what());
    }
  }

  WidgetPtr parseWidget(const YAML::Node &node) {
    auto w = std::make_shared<Widget>();

    if (node["id"])
      w->id = node["id"].as<std::string>();
    if (node["type"]) {
      std::string t = node["type"].as<std::string>();
      if (t == "label")
        w->type = WidgetType::Label;
      else if (t == "button")
        w->type = WidgetType::Button;
      else if (t == "checkbox")
        w->type = WidgetType::Checkbox;
      else if (t == "slider")
        w->type = WidgetType::Slider;
      else if (t == "dropdown")
        w->type = WidgetType::Dropdown;
      else if (t == "textinput")
        w->type = WidgetType::TextInput;
      else if (t == "progress")
        w->type = WidgetType::Progress;
      else if (t == "panel")
        w->type = WidgetType::Panel;
      else if (t == "vbox")
        w->type = WidgetType::VBox;
      else if (t == "hbox")
        w->type = WidgetType::HBox;
      else if (t == "choicebox" || t == "choice" || t == "radio")
        w->type = WidgetType::ChoiceBox;
      else if (t == "separator" || t == "sep")
        w->type = WidgetType::Separator;
      else if (t == "colorpicker" || t == "color")
        w->type = WidgetType::ColorPicker;
      else if (t == "selectable")
        w->type = WidgetType::Selectable;
      else if (t == "collapsing" || t == "header")
        w->type = WidgetType::CollapsingHeader;
      else if (t == "vscrollbar" || t == "vscroll")
        w->type = WidgetType::VScrollbar;
      else if (t == "hscrollbar" || t == "hscroll")
        w->type = WidgetType::HScrollbar;
      else if (t == "scrollarea" || t == "scroll")
        w->type = WidgetType::ScrollArea;
      // Table widgets
      else if (t == "table")
        w->type = WidgetType::Table;
      else if (t == "row" || t == "tr")
        w->type = WidgetType::TableRow;
      else if (t == "cell" || t == "td")
        w->type = WidgetType::TableCell;
      else if (t == "header" || t == "th")
        w->type = WidgetType::TableHeader;
      // Additional widgets
      else if (t == "tabbar" || t == "tabs")
        w->type = WidgetType::TabBar;
      else if (t == "tab" || t == "tabitem")
        w->type = WidgetType::TabItem;
      else if (t == "popup" || t == "dialog" || t == "modal")
        w->type = WidgetType::Popup;
      else if (t == "listbox" || t == "list")
        w->type = WidgetType::ListBox;
      else if (t == "tooltip")
        w->type = WidgetType::Tooltip;
    }

    if (node["x"])
      w->x = node["x"].as<float>();
    if (node["y"])
      w->y = node["y"].as<float>();
    if (node["w"] || node["width"])
      w->w = (node["w"] ? node["w"] : node["width"]).as<float>();
    if (node["h"] || node["height"])
      w->h = (node["h"] ? node["h"] : node["height"]).as<float>();
    if (node["label"])
      w->label = node["label"].as<std::string>();
    if (node["text"])
      w->text = node["text"].as<std::string>();
    if (node["value"])
      w->value = node["value"].as<float>();
    if (node["min"])
      w->minValue = node["min"].as<float>();
    if (node["max"])
      w->maxValue = node["max"].as<float>();
    if (node["checked"] && node["checked"].as<bool>())
      w->flags |= WIDGET_CHECKED;
    if (node["disabled"] && node["disabled"].as<bool>())
      w->flags |= WIDGET_DISABLED;

    if (node["options"]) {
      for (const auto &opt : node["options"]) {
        w->options.push_back(opt.as<std::string>());
      }
    }
    if (node["selected"])
      w->selectedIndex = node["selected"].as<int>();

    // ColorPicker color value (hex string like "0xFFRRGGBB" or "#RRGGBB")
    if (node["color"]) {
      std::string colorStr = node["color"].as<std::string>();
      if (colorStr.size() > 2 && colorStr[0] == '0' &&
          (colorStr[1] == 'x' || colorStr[1] == 'X')) {
        w->colorValue =
            static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
      } else if (colorStr.size() > 0 && colorStr[0] == '#') {
        // #RRGGBB -> 0xFFBBGGRR (ABGR)
        uint32_t rgb =
            static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
        uint8_t r = (rgb >> 16) & 0xFF;
        uint8_t g = (rgb >> 8) & 0xFF;
        uint8_t b = rgb & 0xFF;
        w->colorValue = 0xFF000000 | (b << 16) | (g << 8) | r;
      }
    }

    // CollapsingHeader initial state
    if (node["open"] && node["open"].as<bool>())
      w->flags |= WIDGET_OPEN;

    if (node["on_click"])
      w->onClick = node["on_click"].as<std::string>();
    if (node["on_change"])
      w->onChange = node["on_change"].as<std::string>();

    // Parse children
    if (node["children"]) {
      for (const auto &child : node["children"]) {
        auto cw = parseWidget(child);
        if (cw)
          w->children.push_back(cw);
      }
    }

    // Auto-calculate height for widgets with options
    if (w->type == WidgetType::ChoiceBox && !w->options.empty()) {
      // Each option is 24 pixels tall
      w->h = static_cast<float>(w->options.size()) * 24.0f;
    }

    // ScrollArea content size
    if (node["content_w"])
      w->contentW = node["content_w"].as<float>();
    if (node["content_h"])
      w->contentH = node["content_h"].as<float>();

    // Table properties
    if (node["columns"] && node["columns"].IsSequence()) {
      for (const auto &col : node["columns"]) {
        w->columnWidths.push_back(col.as<float>());
      }
    }
    if (node["row_height"])
      w->rowHeight = node["row_height"].as<float>();

    // Tab properties
    if (node["active_tab"])
      w->activeTab = node["active_tab"].as<int>();
    if (node["active"])
      w->activeTab = node["active"].as<int>();

    // Tooltip
    if (node["tooltip"])
      w->tooltip = node["tooltip"].as<std::string>();

    // Popup properties
    if (node["modal"] && node["modal"].as<bool>())
      w->modal = true;

    // Header color
    if (node["header_color"]) {
      std::string colorStr = node["header_color"].as<std::string>();
      if (colorStr[0] == '#') {
        uint32_t rgb =
            static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
        uint8_t r = (rgb >> 16) & 0xFF;
        uint8_t g = (rgb >> 8) & 0xFF;
        uint8_t b = rgb & 0xFF;
        w->headerColor = 0xFF000000 | (b << 16) | (g << 8) | r;
      }
    }

    // Auto-calculate ListBox height
    if (w->type == WidgetType::ListBox && !w->options.empty() && w->h == 24) {
      w->h =
          std::min(static_cast<float>(w->options.size()), 5.0f) * w->rowHeight;
    }

    return w;
  }

  //=========================================================================
  // Primitive building
  //=========================================================================

  void rebuildPrimitives() {
    if (!_builder)
      return;

    _builder->clear();
    _builder->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
    _builder->setBgColor(0xFF1A1A2E);

    float offsetY = 0;
    for (auto &w : _widgets) {
      renderWidget(w.get(), 0, offsetY);
    }

    // Render open dropdown overlay last (on top)
    if (_openDropdown && _openDropdown->isOpen()) {
      renderDropdownOptions(_openDropdown);
    }

    // Calculate grid after all primitives are added
    _builder->calculate();

    // Copy staging to storage if already allocated
    if (_primStorage.isValid()) {
      const auto &primStaging = _builder->primStaging();
      _primCount = static_cast<uint32_t>(primStaging.size());
      if (_primCount > 0 &&
          _primCount * sizeof(SDFPrimitive) <= _primStorage.size) {
        std::memcpy(_primStorage.data, primStaging.data(),
                    _primCount * sizeof(SDFPrimitive));
        _cardMgr->bufferManager()->markBufferDirty(_primStorage);
      }
    }

    // Update derived storage (grid + glyphs)
    if (_derivedStorage.isValid()) {
      const auto &gridStaging = _builder->gridStaging();
      const auto &glyphs = _builder->glyphs();

      uint8_t *base = _derivedStorage.data;
      uint32_t offset = 0;

      uint32_t gridBytes =
          static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
      uint32_t glyphBytes =
          static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));

      _gridOffset = (_derivedStorage.offset + offset) / sizeof(float);
      if (!gridStaging.empty()) {
        std::memcpy(base + offset, gridStaging.data(), gridBytes);
      }
      offset += gridBytes;

      _glyphOffset = (_derivedStorage.offset + offset) / sizeof(float);
      if (!glyphs.empty()) {
        std::memcpy(base + offset, glyphs.data(), glyphBytes);
      }

      _cardMgr->bufferManager()->markBufferDirty(_derivedStorage);
    }
  }

  void renderWidget(Widget *w, float offsetX, float offsetY) {
    float x = w->x + offsetX;
    float y = w->y + offsetY;

    switch (w->type) {
    case WidgetType::Label:
      addText(w->label, x, y, w->fgColor);
      break;

    case WidgetType::Button:
      addBox(x, y, w->w, w->h, w->isPressed() ? w->accentColor : w->bgColor, 4);
      addText(w->label, x + 8, y + 4, w->fgColor);
      if (w->isHover())
        addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
      break;

    case WidgetType::Checkbox: {
      float boxSize = 16;
      addBox(x, y + 2, boxSize, boxSize, w->bgColor, 2);
      if (w->isChecked()) {
        addBox(x + 3, y + 5, boxSize - 6, boxSize - 6, w->accentColor, 2);
      }
      addText(w->label, x + boxSize + 8, y + 2, w->fgColor);
      if (w->isHover())
        addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
      break;
    }

    case WidgetType::Slider: {
      // Track
      float trackH = 4;
      float trackY = y + (w->h - trackH) / 2;
      addBox(x, trackY, w->w, trackH, w->bgColor, 2);

      // Fill
      float pct = (w->value - w->minValue) / (w->maxValue - w->minValue);
      float fillW = pct * w->w;
      addBox(x, trackY, fillW, trackH, w->accentColor, 2);

      // Handle
      float handleW = 12;
      float handleX = x + fillW - handleW / 2;
      addBox(handleX, y, handleW, w->h, w->accentColor, 6);
      break;
    }

    case WidgetType::Dropdown: {
      addBox(x, y, w->w, w->h, w->bgColor, 4);
      std::string text = w->selectedIndex < static_cast<int>(w->options.size())
                             ? w->options[w->selectedIndex]
                             : "";
      addText(text, x + 8, y + 4, w->fgColor);
      // Chevron arrow - size proportional to font (14px -> ~8px arrow)
      float arrowSize = 8;
      float arrowX = x + w->w - 16 - arrowSize / 2;
      float arrowY = y + w->h / 2;
      if (w->isOpen()) {
        // Up arrow (chevron pointing up)
        addTriangle(arrowX, arrowY + arrowSize / 3, arrowX + arrowSize,
                    arrowY + arrowSize / 3, arrowX + arrowSize / 2,
                    arrowY - arrowSize / 3, w->fgColor);
      } else {
        // Down arrow (chevron pointing down)
        addTriangle(arrowX, arrowY - arrowSize / 3, arrowX + arrowSize,
                    arrowY - arrowSize / 3, arrowX + arrowSize / 2,
                    arrowY + arrowSize / 3, w->fgColor);
      }
      if (w->isHover())
        addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
      break;
    }

    case WidgetType::TextInput: {
      addBox(x, y, w->w, w->h, w->bgColor, 4);
      addText(w->text, x + 8, y + 4, w->fgColor);
      if (w->isFocused()) {
        // Cursor
        float cursorX = x + 8 + w->cursorPos * 8; // Approximate
        addBox(cursorX, y + 4, 2, w->h - 8, w->fgColor, 0);
      }
      if (w->isFocused())
        addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
      break;
    }

    case WidgetType::Progress: {
      addBox(x, y, w->w, w->h, w->bgColor, 4);
      float pct = (w->value - w->minValue) / (w->maxValue - w->minValue);
      addBox(x + 2, y + 2, (w->w - 4) * pct, w->h - 4, w->accentColor, 2);
      break;
    }

    case WidgetType::Panel:
      addBox(x, y, w->w, w->h, w->bgColor, 8);
      for (auto &child : w->children) {
        renderWidget(child.get(), x, y);
      }
      break;

    case WidgetType::VBox: {
      float cy = y;
      for (auto &child : w->children) {
        child->x = x;
        child->y = cy;
        renderWidget(child.get(), 0, 0);
        cy += child->h + 4; // spacing
      }
      break;
    }

    case WidgetType::HBox: {
      float cx = x;
      for (auto &child : w->children) {
        child->x = cx;
        child->y = y;
        renderWidget(child.get(), 0, 0);
        cx += child->w + 4; // spacing
      }
      break;
    }

    case WidgetType::ChoiceBox: {
      // Radio button group - shows all options vertically
      float optH = 24;
      float cy = y;
      for (size_t i = 0; i < w->options.size(); i++) {
        bool isSelected = (static_cast<int>(i) == w->selectedIndex);
        bool isHovered =
            (_hoverChoiceWidget == w && static_cast<int>(i) == _hoverChoiceIdx);
        float radioX = x;
        float radioY = cy + 4;
        float radioSize = 14;
        float centerX = radioX + radioSize / 2;
        float centerY = radioY + radioSize / 2;

        // Outer circle (always visible)
        addCircleOutline(centerX, centerY, radioSize / 2,
                         isHovered ? w->accentColor : 0xFF666677);

        // Inner dot if selected
        if (isSelected) {
          addCircle(centerX, centerY, radioSize / 4, w->accentColor);
        }

        // Hover highlight (smaller dot when hovering non-selected)
        if (isHovered && !isSelected) {
          addCircle(centerX, centerY, radioSize / 6, 0xFF555566);
        }

        // Label
        addText(w->options[i], x + radioSize + 8, cy + 4, w->fgColor);
        cy += optH;
      }
      break;
    }

    case WidgetType::Separator: {
      // Horizontal line
      float lineY = y + w->h / 2;
      addBox(x, lineY - 1, w->w, 2, 0xFF444455, 0);
      break;
    }

    case WidgetType::ColorPicker: {
      // Full color wheel if widget is large enough, otherwise just preview box
      if (w->w >= 80 && w->h >= 80) {
        // Convert ABGR color to HSV
        uint8_t r = w->colorValue & 0xFF;
        uint8_t g = (w->colorValue >> 8) & 0xFF;
        uint8_t b = (w->colorValue >> 16) & 0xFF;
        float rf = r / 255.0f, gf = g / 255.0f, bf = b / 255.0f;
        float cmax = std::max({rf, gf, bf});
        float cmin = std::min({rf, gf, bf});
        float delta = cmax - cmin;
        float hue = 0, sat = 0, val = cmax;
        if (delta > 0) {
          sat = delta / cmax;
          if (cmax == rf)
            hue = std::fmod((gf - bf) / delta + 6.0f, 6.0f) / 6.0f;
          else if (cmax == gf)
            hue = ((bf - rf) / delta + 2.0f) / 6.0f;
          else
            hue = ((rf - gf) / delta + 4.0f) / 6.0f;
        }
        float radius = std::min(w->w, w->h) / 2.0f - 4.0f;
        float cx = x + w->w / 2.0f;
        float cy = y + w->h / 2.0f;
        _builder->addColorWheel(cx, cy, radius, radius * 0.7f, hue, sat, val,
                                6.0f, _builder->primitiveCount());
      } else {
        // Small preview box + label
        float boxSize = 20;
        addBox(x, y + 2, boxSize, boxSize, w->colorValue, 2);
        addBoxOutline(x, y + 2, boxSize, boxSize, 0xFF666666, 2);
        if (!w->label.empty()) {
          addText(w->label, x + boxSize + 8, y + 4, w->fgColor);
        }
        if (w->isHover())
          addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
      }
      break;
    }

    case WidgetType::Selectable: {
      // Clickable row that can be selected
      if (w->isChecked()) {
        addBox(x, y, w->w, w->h, w->accentColor, 2);
      } else if (w->isHover()) {
        addBox(x, y, w->w, w->h, 0xFF333344, 2);
      }
      addText(w->label, x + 8, y + 4, w->fgColor);
      break;
    }

    case WidgetType::CollapsingHeader: {
      // Expandable section header with arrow
      addBox(x, y, w->w, w->h, w->bgColor, 4);

      // Arrow indicator - size proportional to font (14px -> ~8px arrow)
      float arrowSize = 8;
      float arrowX = x + 10;
      float arrowY = y + w->h / 2;
      if (w->isOpen()) {
        // Down arrow (expanded)
        addTriangle(arrowX, arrowY - arrowSize / 3, arrowX + arrowSize,
                    arrowY - arrowSize / 3, arrowX + arrowSize / 2,
                    arrowY + arrowSize / 3, w->fgColor);
      } else {
        // Right arrow (collapsed)
        addTriangle(arrowX, arrowY - arrowSize / 2, arrowX,
                    arrowY + arrowSize / 2, arrowX + arrowSize * 0.7f, arrowY,
                    w->fgColor);
      }

      addText(w->label, x + 26, y + 4, w->fgColor);
      if (w->isHover())
        addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);

      // Render children if open
      if (w->isOpen()) {
        float cy = y + w->h + 4;
        for (auto &child : w->children) {
          child->x = x + 16; // Indent children
          child->y = cy;
          renderWidget(child.get(), 0, 0);
          cy += child->h + 4;
        }
      }
      break;
    }

    case WidgetType::VScrollbar: {
      // Vertical scrollbar: track + thumb
      float trackW = w->w > 0 ? w->w : 12;
      addBox(x, y, trackW, w->h, 0xFF222233, trackW / 2); // Track

      // Thumb position based on value (0-1 range)
      float thumbH = std::max(20.0f, w->h * 0.2f); // Min 20px or 20% of track
      float trackRange = w->h - thumbH;
      float thumbY = y + w->value * trackRange;
      uint32_t thumbColor = w->isPressed()
                                ? w->accentColor
                                : (w->isHover() ? 0xFF555566 : 0xFF444455);
      addBox(x + 2, thumbY, trackW - 4, thumbH, thumbColor, (trackW - 4) / 2);
      break;
    }

    case WidgetType::HScrollbar: {
      // Horizontal scrollbar: track + thumb
      float trackH = w->h > 0 ? w->h : 12;
      addBox(x, y, w->w, trackH, 0xFF222233, trackH / 2); // Track

      // Thumb position based on value (0-1 range)
      float thumbW = std::max(20.0f, w->w * 0.2f); // Min 20px or 20% of track
      float trackRange = w->w - thumbW;
      float thumbX = x + w->value * trackRange;
      uint32_t thumbColor = w->isPressed()
                                ? w->accentColor
                                : (w->isHover() ? 0xFF555566 : 0xFF444455);
      addBox(thumbX, y + 2, thumbW, trackH - 4, thumbColor, (trackH - 4) / 2);
      break;
    }

    case WidgetType::ScrollArea: {
      // Scrollable container - render background
      addBox(x, y, w->w, w->h, w->bgColor, 4);

      // Calculate scroll ranges
      float scrollbarW = 12;
      float scrollbarH = 12;
      float viewW = w->w - scrollbarW;
      float viewH = w->h - scrollbarH;
      float contentW = w->contentW > 0 ? w->contentW : viewW;
      float contentH = w->contentH > 0 ? w->contentH : viewH;
      float maxScrollX = std::max(0.0f, contentW - viewW);
      float maxScrollY = std::max(0.0f, contentH - viewH);

      // Render children with scroll offset
      for (auto &child : w->children) {
        // Position children relative to scroll area, offset by scroll
        float childX = x + child->x - w->scrollX;
        float childY = y + child->y - w->scrollY;

        // Simple clipping: only render if at least partially visible
        if (childX + child->w > x && childX < x + viewW &&
            childY + child->h > y && childY < y + viewH) {
          renderWidget(child.get(), x - w->scrollX, y - w->scrollY);
        }
      }

      // Render vertical scrollbar if content is taller than view
      if (contentH > viewH) {
        float trackX = x + w->w - scrollbarW;
        float trackH = w->h - scrollbarH;
        addBox(trackX, y, scrollbarW, trackH, 0xFF222233, scrollbarW / 2);

        float thumbH = std::max(20.0f, trackH * viewH / contentH);
        float thumbRange = trackH - thumbH;
        float thumbY =
            y + (maxScrollY > 0 ? (w->scrollY / maxScrollY) * thumbRange : 0);
        addBox(trackX + 2, thumbY, scrollbarW - 4, thumbH, 0xFF444455,
               (scrollbarW - 4) / 2);
      }

      // Render horizontal scrollbar if content is wider than view
      if (contentW > viewW) {
        float trackY = y + w->h - scrollbarH;
        float trackW = w->w - scrollbarW;
        addBox(x, trackY, trackW, scrollbarH, 0xFF222233, scrollbarH / 2);

        float thumbW = std::max(20.0f, trackW * viewW / contentW);
        float thumbRange = trackW - thumbW;
        float thumbX =
            x + (maxScrollX > 0 ? (w->scrollX / maxScrollX) * thumbRange : 0);
        addBox(thumbX, trackY + 2, thumbW, scrollbarH - 4, 0xFF444455,
               (scrollbarH - 4) / 2);
      }

      // Corner fill
      if (contentW > viewW && contentH > viewH) {
        addBox(x + w->w - scrollbarW, y + w->h - scrollbarH, scrollbarW,
               scrollbarH, 0xFF1A1A2E, 0);
      }
      break;
    }

    case WidgetType::Table: {
      // Table container with header and rows
      addBox(x, y, w->w, w->h, w->bgColor, 2);
      addBoxOutline(x, y, w->w, w->h, 0xFF555566, 2);

      float rowH = w->rowHeight > 0 ? w->rowHeight : 24;
      float cy = y;
      for (auto &child : w->children) {
        // Render header/row inline with proper column handling
        float childH = child->h > 0 ? child->h : rowH;
        bool isHeader = (child->type == WidgetType::TableHeader);

        // Row background
        if (isHeader) {
          addBox(x, cy, w->w, childH, w->headerColor, 0);
        } else if (child->isHover()) {
          addBox(x, cy, w->w, childH, 0xFF333344, 0);
        }

        // Bottom border
        uint32_t borderColor = isHeader ? 0xFF555566 : 0xFF444455;
        addBox(x, cy + childH - 1, w->w, 1, borderColor, 0);

        // Render cells
        float cx = x;
        size_t colIdx = 0;
        for (auto &cell : child->children) {
          float colW =
              (colIdx < w->columnWidths.size()) ? w->columnWidths[colIdx] : 80;
          // Vertical separator
          if (colIdx > 0) {
            addBox(cx, cy, 1, childH, borderColor, 0);
          }
          // Cell text
          uint32_t textColor = isHeader ? 0xFFFFFFFF : w->fgColor;
          if (!cell->label.empty()) {
            addText(cell->label, cx + 6, cy + 4, textColor);
          }
          cx += colW;
          colIdx++;
        }

        cy += childH;
      }
      break;
    }

    case WidgetType::TableHeader:
    case WidgetType::TableRow:
    case WidgetType::TableCell: {
      // These are now handled inline in Table case
      // But keep this for standalone use
      if (w->type == WidgetType::TableCell) {
        addText(w->label, x, y, w->fgColor);
      }
      break;
    }

    case WidgetType::TabBar: {
      // Tab bar with horizontal tabs
      addBox(x, y, w->w, w->h, w->bgColor, 4);

      float tabX = x + 4;
      int tabIdx = 0;
      for (auto &child : w->children) {
        bool isActive = (tabIdx == w->activeTab);
        float tabW = child->w > 0 ? child->w : 80;
        float tabH = w->h - 4;

        // Tab background
        if (isActive) {
          addBox(tabX, y + 2, tabW, tabH, w->accentColor, 4);
        } else if (child->isHover()) {
          addBox(tabX, y + 2, tabW, tabH, 0xFF333344, 4);
        }

        // Tab label
        addText(child->label, tabX + 8, y + 6,
                isActive ? 0xFFFFFFFF : 0xFFAAAAAA);

        tabX += tabW + 4;
        tabIdx++;
      }
      break;
    }

    case WidgetType::TabItem: {
      // Tab content panel (rendered when active)
      if (w->isOpen()) {
        addBox(x, y, w->w, w->h, w->bgColor, 4);
        float cy = y + 4;
        for (auto &child : w->children) {
          renderWidget(child.get(), x + 4, cy);
          cy += child->h + 4;
        }
      }
      break;
    }

    case WidgetType::Popup: {
      // Modal popup/dialog
      if (w->isOpen()) {
        // Semi-transparent overlay if modal
        if (w->modal) {
          addBox(0, 0, _pixelWidth, _pixelHeight, 0x80000000, 0);
        }
        // Popup background with shadow effect
        addBox(x + 4, y + 4, w->w, w->h, 0x40000000, 8); // Shadow
        addBox(x, y, w->w, w->h, w->bgColor, 8);
        addBoxOutline(x, y, w->w, w->h, w->accentColor, 8);

        // Title bar if label is set
        if (!w->label.empty()) {
          addBox(x, y, w->w, 28, w->headerColor, 8);
          addText(w->label, x + 8, y + 6, w->fgColor);
        }

        // Render children
        float contentY = y + (w->label.empty() ? 8 : 32);
        for (auto &child : w->children) {
          renderWidget(child.get(), x + 8, contentY);
          contentY += child->h + 4;
        }
      }
      break;
    }

    case WidgetType::ListBox: {
      // Scrollable list of selectable items
      addBox(x, y, w->w, w->h, w->bgColor, 4);
      addBoxOutline(x, y, w->w, w->h, 0xFF444455, 4);

      float itemH = w->rowHeight > 0 ? w->rowHeight : 24;
      float contentH = w->options.size() * itemH;
      bool needsScroll = contentH > w->h;
      float scrollbarW = needsScroll ? 10 : 0;
      float contentW = w->w - scrollbarW - 4; // Account for scrollbar
      float maxScroll = std::max(0.0f, contentH - w->h);

      // Clamp scroll position
      w->scrollY = std::clamp(w->scrollY, 0.0f, maxScroll);

      int startIdx = static_cast<int>(w->scrollY / itemH);
      int visibleCount = static_cast<int>(std::ceil(w->h / itemH)) + 1;
      int endIdx = std::min(startIdx + visibleCount,
                            static_cast<int>(w->options.size()));

      for (int i = startIdx; i < endIdx; i++) {
        float itemY = y + i * itemH - w->scrollY;
        // Clip to widget bounds
        if (itemY + itemH <= y || itemY >= y + w->h)
          continue;

        bool isSelected = (i == w->selectedIndex);
        bool isHovered = (_hoverListWidget == w && i == _hoverListIdx);

        if (isSelected) {
          addBox(x + 2, itemY + 1, contentW, itemH - 2, w->accentColor, 2);
        } else if (isHovered) {
          addBox(x + 2, itemY + 1, contentW, itemH - 2, 0xFF333344, 2);
        }

        addText(w->options[i], x + 8, itemY + 4, w->fgColor);
      }

      // Scrollbar
      if (needsScroll) {
        float trackH = w->h - 4;
        float thumbH = std::max(20.0f, trackH * w->h / contentH);
        float thumbY =
            y + 2 +
            (maxScroll > 0 ? (w->scrollY / maxScroll) * (trackH - thumbH) : 0);

        addBox(x + w->w - scrollbarW, y + 2, scrollbarW - 2, trackH, 0xFF222233,
               4);
        addBox(x + w->w - scrollbarW + 1, thumbY, scrollbarW - 4, thumbH,
               0xFF555566, 3);
      }
      break;
    }

    case WidgetType::Tooltip: {
      // Tooltip is rendered separately when hovering
      // This case handles if tooltip is used as a standalone widget
      if (!w->label.empty()) {
        addBox(x, y, w->w, w->h, 0xF0222233, 4);
        addBoxOutline(x, y, w->w, w->h, 0xFF666677, 4);
        addText(w->label, x + 6, y + 4, w->fgColor);
      }
      break;
    }
    }

    // Render tooltip on hover (for any widget with tooltip set)
    if (w->isHover() && !w->tooltip.empty()) {
      float tipX = x + w->w + 4;
      float tipY = y;
      float tipW = static_cast<float>(w->tooltip.length()) * 7 + 12;
      float tipH = 22;
      addBox(tipX, tipY, tipW, tipH, 0xF0222233, 4);
      addBoxOutline(tipX, tipY, tipW, tipH, 0xFF666677, 4);
      addText(w->tooltip, tipX + 6, tipY + 4, 0xFFFFFFFF);
    }
  }

  void renderDropdownOptions(Widget *w) {
    float x = w->x;
    float y = w->y + w->h;
    float optH = 24;

    // Draw dropdown list background
    float listH = static_cast<float>(w->options.size()) * optH;
    addBox(x, y, w->w, listH, 0xFF1E1E2E, 4); // Darker background

    for (size_t i = 0; i < w->options.size(); i++) {
      float optY = y + i * optH;
      bool isSelected = (static_cast<int>(i) == w->selectedIndex);
      bool isHovered = (static_cast<int>(i) == _hoverOptionIdx);

      // Background: hover takes priority, then selected
      if (isHovered) {
        addBox(x + 2, optY + 2, w->w - 4, optH - 4, w->accentColor, 2);
      } else if (isSelected) {
        // Subtle highlight for currently selected item
        addBox(x + 2, optY + 2, w->w - 4, optH - 4, 0xFF2A2A3E, 2);
      }

      // Text
      addText(w->options[i], x + 8, optY + 4, w->fgColor);

      // Checkmark for selected item
      if (isSelected) {
        addText("\xE2\x9C\x93", x + w->w - 20, optY + 4, w->accentColor); // ✓
      }
    }
  }

  //=========================================================================
  // SDF primitive helpers - using YDrawBuilder
  //=========================================================================

  void addBox(float x, float y, float w, float h, uint32_t color,
              float radius) {
    if (!_builder)
      return;

    SDFPrimitive p = {};
    p.type = static_cast<uint32_t>(SDFType::RoundedBox);
    p.layer = _builder->primitiveCount();
    p.params[0] = x + w / 2; // center x
    p.params[1] = y + h / 2; // center y
    p.params[2] = w / 2;     // half-width
    p.params[3] = h / 2;     // half-height
    p.params[4] = radius;    // corner radii (all same)
    p.params[5] = radius;
    p.params[6] = radius;
    p.params[7] = radius;
    p.fillColor = color;
    p.strokeColor = 0;
    p.strokeWidth = 0;
    p.round = 0;
    p.aabbMinX = x;
    p.aabbMinY = y;
    p.aabbMaxX = x + w;
    p.aabbMaxY = y + h;
    _builder->addPrimitive(p);
  }

  void addBoxOutline(float x, float y, float w, float h, uint32_t color,
                     float radius) {
    if (!_builder)
      return;

    SDFPrimitive p = {};
    p.type = static_cast<uint32_t>(SDFType::RoundedBox);
    p.layer = _builder->primitiveCount();
    p.params[0] = x + w / 2;
    p.params[1] = y + h / 2;
    p.params[2] = w / 2;
    p.params[3] = h / 2;
    p.params[4] = radius;
    p.params[5] = radius;
    p.params[6] = radius;
    p.params[7] = radius;
    p.fillColor = 0; // No fill
    p.strokeColor = color;
    p.strokeWidth = 2;
    p.round = 0;
    p.aabbMinX = x - 2;
    p.aabbMinY = y - 2;
    p.aabbMaxX = x + w + 2;
    p.aabbMaxY = y + h + 2;
    _builder->addPrimitive(p);
  }

  void addText(const std::string &text, float x, float y, uint32_t color) {
    if (!_builder || text.empty())
      return;

    // Use builder's proper MSDF text rendering
    float fontSize = 14.0f; // Default font size for UI
    if (auto res =
            _builder->addText(x, y + fontSize * 0.8f, text, fontSize, color, 0);
        !res) {
      // Fallback: if text rendering fails, silently ignore
      ywarn("YGui::addText failed: {}", error_msg(res));
    }
  }

  void addTriangle(float x0, float y0, float x1, float y1, float x2, float y2,
                   uint32_t color) {
    if (!_builder)
      return;

    SDFPrimitive p = {};
    p.type = static_cast<uint32_t>(SDFType::Triangle);
    p.layer = _builder->primitiveCount();
    p.params[0] = x0;
    p.params[1] = y0;
    p.params[2] = x1;
    p.params[3] = y1;
    p.params[4] = x2;
    p.params[5] = y2;
    p.fillColor = color;
    p.strokeColor = 0;
    p.strokeWidth = 0;
    p.round = 0;
    // Compute AABB
    p.aabbMinX = std::min({x0, x1, x2});
    p.aabbMinY = std::min({y0, y1, y2});
    p.aabbMaxX = std::max({x0, x1, x2});
    p.aabbMaxY = std::max({y0, y1, y2});
    _builder->addPrimitive(p);
  }

  void addCircle(float cx, float cy, float radius, uint32_t color) {
    if (!_builder)
      return;

    SDFPrimitive p = {};
    p.type = static_cast<uint32_t>(SDFType::Circle);
    p.layer = _builder->primitiveCount();
    p.params[0] = cx;
    p.params[1] = cy;
    p.params[2] = radius;
    p.fillColor = color;
    p.strokeColor = 0;
    p.strokeWidth = 0;
    p.round = 0;
    p.aabbMinX = cx - radius;
    p.aabbMinY = cy - radius;
    p.aabbMaxX = cx + radius;
    p.aabbMaxY = cy + radius;
    _builder->addPrimitive(p);
  }

  void addCircleOutline(float cx, float cy, float radius, uint32_t color) {
    if (!_builder)
      return;

    SDFPrimitive p = {};
    p.type = static_cast<uint32_t>(SDFType::Circle);
    p.layer = _builder->primitiveCount();
    p.params[0] = cx;
    p.params[1] = cy;
    p.params[2] = radius;
    p.fillColor = 0; // No fill
    p.strokeColor = color;
    p.strokeWidth = 1.5f;
    p.round = 0;
    p.aabbMinX = cx - radius - 2;
    p.aabbMinY = cy - radius - 2;
    p.aabbMaxX = cx + radius + 2;
    p.aabbMaxY = cy + radius + 2;
    _builder->addPrimitive(p);
  }

  //=========================================================================
  // Event helpers
  //=========================================================================

  void updateHoverRecursive(WidgetPtr &w, float px, float py) {
    bool hover = w->contains(px, py);
    if (hover)
      w->flags |= WIDGET_HOVER;
    else
      w->flags &= ~WIDGET_HOVER;

    // For ScrollArea, transform coords to content space for children
    if (w->type == WidgetType::ScrollArea && hover) {
      float contentX = (px - w->x) + w->scrollX;
      float contentY = (py - w->y) + w->scrollY;
      for (auto &child : w->children) {
        updateHoverContent(child, contentX, contentY);
      }
    } else {
      for (auto &child : w->children) {
        updateHoverRecursive(child, px, py);
      }
    }
  }

  void updateHoverContent(WidgetPtr &w, float contentX, float contentY) {
    bool hover = w->contains(contentX, contentY);
    if (hover)
      w->flags |= WIDGET_HOVER;
    else
      w->flags &= ~WIDGET_HOVER;

    // Handle nested ScrollArea
    if (w->type == WidgetType::ScrollArea && hover) {
      float nestedX = (contentX - w->x) + w->scrollX;
      float nestedY = (contentY - w->y) + w->scrollY;
      for (auto &child : w->children) {
        updateHoverContent(child, nestedX, nestedY);
      }
    } else {
      for (auto &child : w->children) {
        updateHoverContent(child, contentX, contentY);
      }
    }
  }

  struct HitResult {
    Widget *widget = nullptr;
    float hitX = 0; // Coordinates in the widget's coordinate space
    float hitY = 0;
  };

  HitResult findWidgetAt(WidgetPtr &w, float px, float py) {
    // For ScrollArea, transform mouse coords to content coords and check
    // children
    if (w->type == WidgetType::ScrollArea && w->contains(px, py)) {
      // Convert to content coordinates (relative to ScrollArea content)
      float contentX = (px - w->x) + w->scrollX;
      float contentY = (py - w->y) + w->scrollY;
      // Check children first (they're on top)
      for (auto &child : w->children) {
        auto hit = findWidgetAtContent(child, contentX, contentY);
        if (hit.widget)
          return hit;
      }
      return {w.get(), px, py};
    }

    // Check children first (they're on top)
    for (auto &child : w->children) {
      auto hit = findWidgetAt(child, px, py);
      if (hit.widget)
        return hit;
    }
    if (w->contains(px, py))
      return {w.get(), px, py};
    return {nullptr, 0, 0};
  }

  // Helper for checking widgets in content coordinates (inside ScrollArea)
  HitResult findWidgetAtContent(WidgetPtr &w, float contentX, float contentY) {
    // For nested ScrollAreas, recurse with content coordinate transform
    if (w->type == WidgetType::ScrollArea && w->contains(contentX, contentY)) {
      float nestedX = (contentX - w->x) + w->scrollX;
      float nestedY = (contentY - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findWidgetAtContent(child, nestedX, nestedY);
        if (hit.widget)
          return hit;
      }
      return {w.get(), contentX, contentY};
    }

    // Check children first
    for (auto &child : w->children) {
      auto hit = findWidgetAtContent(child, contentX, contentY);
      if (hit.widget)
        return hit;
    }
    if (w->contains(contentX, contentY))
      return {w.get(), contentX, contentY};
    return {nullptr, 0, 0};
  }

  Widget *findWidgetById(const std::string &id) {
    for (auto &w : _widgets) {
      Widget *found = findWidgetByIdRecursive(w, id);
      if (found)
        return found;
    }
    return nullptr;
  }

  Widget *findWidgetByIdRecursive(WidgetPtr &w, const std::string &id) {
    if (w->id == id)
      return w.get();
    for (auto &child : w->children) {
      Widget *found = findWidgetByIdRecursive(child, id);
      if (found)
        return found;
    }
    return nullptr;
  }

  int getDropdownOptionAt(Widget *w, float wx, float wy) {
    float x = w->x;
    float y = w->y + w->h; // Options start below the dropdown button
    float optH = 24;

    ydebug("getDropdownOptionAt: wx={:.1f} wy={:.1f} dropdown at x={:.1f} "
           "y={:.1f} w={:.1f}",
           wx, wy, x, y, w->w);

    for (size_t i = 0; i < w->options.size(); i++) {
      float oy = y + i * optH;
      if (wx >= x && wx < x + w->w && wy >= oy && wy < oy + optH) {
        ydebug("  -> option {} hit (oy={:.1f})", i, oy);
        return static_cast<int>(i);
      }
    }
    return -1;
  }

  int getChoiceBoxOptionAt(Widget *w, float wx, float wy) {
    float x = w->x;
    float y = w->y;
    float optH = 24;

    for (size_t i = 0; i < w->options.size(); i++) {
      float oy = y + i * optH;
      if (wx >= x && wx < x + w->w && wy >= oy && wy < oy + optH) {
        return static_cast<int>(i);
      }
    }
    return -1;
  }

  struct ListBoxHit {
    Widget *widget = nullptr;
    float hitY = 0; // Y coordinate in widget's coordinate space
  };

  // Find a ListBox widget at the given coordinates, recursively searching
  // through ScrollAreas
  ListBoxHit findListBoxAt(WidgetPtr &w, float wx, float wy) {
    // Handle ScrollArea - adjust coordinates for scroll offset
    if (w->type == WidgetType::ScrollArea && w->contains(wx, wy)) {
      // Convert to content coordinates
      float contentX = (wx - w->x) + w->scrollX;
      float contentY = (wy - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findListBoxAtContent(child, contentX, contentY);
        if (hit.widget)
          return hit;
      }
    }
    // Check children (not inside ScrollArea)
    for (auto &child : w->children) {
      auto hit = findListBoxAt(child, wx, wy);
      if (hit.widget)
        return hit;
    }
    // Check this widget
    if (w->type == WidgetType::ListBox && w->contains(wx, wy)) {
      return {w.get(), wy};
    }
    return {nullptr, 0};
  }

  // Helper for finding ListBox in content coordinates (inside ScrollArea)
  ListBoxHit findListBoxAtContent(WidgetPtr &w, float contentX,
                                  float contentY) {
    // Handle nested ScrollArea
    if (w->type == WidgetType::ScrollArea && w->contains(contentX, contentY)) {
      float nestedX = (contentX - w->x) + w->scrollX;
      float nestedY = (contentY - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findListBoxAtContent(child, nestedX, nestedY);
        if (hit.widget)
          return hit;
      }
    }
    // Check children
    for (auto &child : w->children) {
      auto hit = findListBoxAtContent(child, contentX, contentY);
      if (hit.widget)
        return hit;
    }
    // Check this widget
    if (w->type == WidgetType::ListBox && w->contains(contentX, contentY)) {
      return {w.get(), contentY};
    }
    return {nullptr, 0};
  }

  void updateSliderValue(Widget *w, float wx) {
    float pct = (wx - w->x) / w->w;
    ydebug("Slider update: wx={:.1f} w->x={:.1f} w->w={:.1f} pct={:.3f} "
           "cellW={:.1f}",
           wx, w->x, w->w, pct, _cellWidthF);
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = w->minValue + pct * (w->maxValue - w->minValue);
    emitEvent(w, "change");
  }

  void updateVScrollValue(Widget *w, float wy) {
    // Account for thumb size (20% of track or min 20px)
    float thumbH = std::max(20.0f, w->h * 0.2f);
    float trackRange = w->h - thumbH;
    if (trackRange <= 0)
      return;
    float pct = (wy - w->y - thumbH / 2) / trackRange;
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = pct; // Scrollbars use 0-1 range
    emitEvent(w, "change");
  }

  void updateHScrollValue(Widget *w, float wx) {
    // Account for thumb size (20% of track or min 20px)
    float thumbW = std::max(20.0f, w->w * 0.2f);
    float trackRange = w->w - thumbW;
    if (trackRange <= 0)
      return;
    float pct = (wx - w->x - thumbW / 2) / trackRange;
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = pct; // Scrollbars use 0-1 range
    emitEvent(w, "change");
  }

  void handleTextInput(Widget *w, uint32_t codepoint, int mods) {
    if (codepoint == 8) { // Backspace
      if (w->cursorPos > 0 && !w->text.empty()) {
        w->text.erase(w->cursorPos - 1, 1);
        w->cursorPos--;
      }
    } else if (codepoint == 127) { // Delete
      if (w->cursorPos < w->text.size()) {
        w->text.erase(w->cursorPos, 1);
      }
    } else if (codepoint >= 32 && codepoint < 127) {
      w->text.insert(w->cursorPos, 1, static_cast<char>(codepoint));
      w->cursorPos++;
    }
    emitEvent(w, "change");
  }

  void emitEvent(Widget *w, const std::string &eventType) {
    std::string eventName;
    if (eventType == "click")
      eventName = w->onClick;
    else if (eventType == "change")
      eventName = w->onChange;

    if (eventName.empty())
      return;

    // Emit OSC event
    // Format: event;card=<name>;widget=<id>;type=<type>;value=<value>
    std::ostringstream ss;
    ss << "\033]" << YETTY_OSC_VENDOR_ID << ";event;card=" << name()
       << ";widget=" << w->id << ";type=" << eventType;

    if (w->type == WidgetType::Slider || w->type == WidgetType::Progress ||
        w->type == WidgetType::VScrollbar ||
        w->type == WidgetType::HScrollbar) {
      ss << ";value=" << w->value;
    } else if (w->type == WidgetType::Checkbox ||
               w->type == WidgetType::Selectable) {
      ss << ";checked=" << (w->isChecked() ? "true" : "false");
    } else if (w->type == WidgetType::Dropdown ||
               w->type == WidgetType::ChoiceBox) {
      ss << ";selected=" << w->selectedIndex;
      if (w->selectedIndex < static_cast<int>(w->options.size())) {
        ss << ";value=" << w->options[w->selectedIndex];
      }
    } else if (w->type == WidgetType::TextInput) {
      ss << ";text=" << w->text;
    } else if (w->type == WidgetType::CollapsingHeader) {
      ss << ";open=" << (w->isOpen() ? "true" : "false");
    } else if (w->type == WidgetType::ColorPicker) {
      ss << ";color=0x" << std::hex << w->colorValue;
    }

    ss << "\033\\";

    // Write to stdout (or use a callback)
    yinfo("YGui event: {}", ss.str());
  }

  //=========================================================================
  // Metadata - uses YDrawMetadata format for shader compatibility
  //=========================================================================

  Result<void> uploadMetadata() {
    if (!_metaHandle.isValid()) {
      return Err<void>("YGui::uploadMetadata: invalid handle");
    }
    if (!_builder)
      return Ok();

    float sceneMinX = _builder->sceneMinX();
    float sceneMinY = _builder->sceneMinY();
    float sceneMaxX = _builder->sceneMaxX();
    float sceneMaxY = _builder->sceneMaxY();
    float cellSize = _builder->cellSize();

    YDrawMetadata meta = {};
    meta.primitiveOffset =
        _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
    meta.primitiveCount = _primCount;
    meta.gridOffset = _gridOffset;
    meta.gridWidth = _builder->gridWidth();
    meta.gridHeight = _builder->gridHeight();
    std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(_builder->glyphs().size());
    std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
    meta.widthCells = _widthCells & 0xFFFF;
    meta.heightCells = _heightCells & 0xFFFF;
    meta.flags = _builder->flags() & 0xFFFF;
    meta.bgColor = _builder->bgColor();

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
        !res) {
      return Err<void>("YGui::uploadMetadata: write failed");
    }
    return Ok();
  }

  //=========================================================================
  // Event dispatch - receives events from EventLoop
  //=========================================================================

  Result<bool> onEvent(const base::Event &event) override {
    switch (event.type) {
    case base::Event::Type::SetFocus:
      if (event.setFocus.objectId == id()) {
        _isFocused = true;
        yinfo("YGui: got focus");
        return Ok(true);
      } else if (_isFocused) {
        _isFocused = false;
      }
      return Ok(false);

    case base::Event::Type::CardMouseDown:
      if (event.cardMouse.targetId == id()) {
        yinfo("YGui::onEvent CardMouseDown x={:.1f} y={:.1f}",
              event.cardMouse.x, event.cardMouse.y);
        handleMouseDown(event.cardMouse.x, event.cardMouse.y,
                        event.cardMouse.button);
        return Ok(true);
      }
      break;

    case base::Event::Type::CardMouseUp:
      if (event.cardMouse.targetId == id()) {
        handleMouseUp(event.cardMouse.x, event.cardMouse.y,
                      event.cardMouse.button);
        return Ok(true);
      }
      break;

    case base::Event::Type::CardMouseMove:
      if (event.cardMouse.targetId == id()) {
        handleMouseMove(event.cardMouse.x, event.cardMouse.y);
        return Ok(true);
      }
      break;

    case base::Event::Type::CardScroll:
      if (event.cardScroll.targetId == id()) {
        handleMouseScroll(event.cardScroll.x, event.cardScroll.y,
                          event.cardScroll.dx, event.cardScroll.dy);
        return Ok(true);
      }
      break;

    case base::Event::Type::CardKeyDown:
      if (event.cardKey.targetId == id()) {
        handleKeyDown(static_cast<uint32_t>(event.cardKey.key),
                      event.cardKey.mods);
        return Ok(true);
      }
      break;

    default:
      break;
    }
    return Ok(false);
  }

  //=========================================================================
  // Event registration
  //=========================================================================

  Result<void> registerForEvents() {
    auto loopRes = base::EventLoop::instance();
    if (!loopRes)
      return Ok(); // No event loop
    auto loop = *loopRes;
    auto self = sharedAs<base::EventListener>();
    if (auto res =
            loop->registerListener(base::Event::Type::SetFocus, self, 1000);
        !res)
      return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseDown,
                                          self, 1000);
        !res)
      return res;
    if (auto res =
            loop->registerListener(base::Event::Type::CardMouseUp, self, 1000);
        !res)
      return res;
    if (auto res = loop->registerListener(base::Event::Type::CardMouseMove,
                                          self, 1000);
        !res)
      return res;
    if (auto res =
            loop->registerListener(base::Event::Type::CardScroll, self, 1000);
        !res)
      return res;
    if (auto res =
            loop->registerListener(base::Event::Type::CardKeyDown, self, 1000);
        !res)
      return res;
    return Ok();
  }

  void deregisterFromEvents() {
    if (weak_from_this().expired())
      return;
    auto loopRes = base::EventLoop::instance();
    if (!loopRes)
      return;
    (*loopRes)->deregisterListener(sharedAs<base::EventListener>());
  }

  //=========================================================================
  // Members
  //=========================================================================

  uint32_t _screenId = 0;
  std::string _argsStr;
  std::string _payloadStr;

  std::vector<WidgetPtr> _widgets;

  // YDrawBuilder for proper SDF/MSDF rendering
  YDrawBuilder::Ptr _builder;

  // GPU storage handles
  StorageHandle _primStorage = StorageHandle::invalid();
  StorageHandle _derivedStorage = StorageHandle::invalid();

  // Buffer offsets (in floats)
  uint32_t _gridOffset = 0;
  uint32_t _glyphOffset = 0;
  uint32_t _primCount = 0;

  float _pixelWidth = 0;
  float _pixelHeight = 0;

  Widget *_focused = nullptr;
  Widget *_pressed = nullptr;
  Widget *_openDropdown = nullptr;
  Widget *_hoverChoiceWidget = nullptr;
  Widget *_hoverListWidget = nullptr;  // ListBox being hovered
  Widget *_scrollDragWidget = nullptr; // ScrollArea being dragged
  bool _scrollDragVertical = false;    // true=vertical, false=horizontal
  int _hoverOptionIdx = -1;
  int _hoverChoiceIdx = -1;
  int _hoverListIdx = -1; // ListBox item hover index

  bool _dirty = true;
  bool _metadataDirty = true;
  bool _isFocused = false;

  // Cell size for coordinate conversion (float for precision)
  float _cellWidthF = 10.0f;
  float _cellHeightF = 20.0f;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YGui::create(const YettyContext &ctx, int32_t x, int32_t y,
                             uint32_t widthCells, uint32_t heightCells,
                             const std::string &args,
                             const std::string &payload) {
  auto card = std::make_shared<YGuiImpl>(ctx, x, y, widthCells, heightCells,
                                         args, payload);
  if (auto res = card->init(); !res) {
    return Err<CardPtr>("YGui::create: init failed", res);
  }
  return Ok<CardPtr>(card);
}

Result<YGui::Ptr> YGui::createImpl(ContextType &, const YettyContext &ctx,
                                   int32_t x, int32_t y, uint32_t widthCells,
                                   uint32_t heightCells,
                                   const std::string &args,
                                   const std::string &payload) noexcept {
  auto result = create(ctx, x, y, widthCells, heightCells, args, payload);
  if (!result)
    return Err<Ptr>("Failed to create YGui", result);
  auto gui = std::dynamic_pointer_cast<YGui>(*result);
  if (!gui)
    return Err<Ptr>("Created card is not YGui");
  return Ok(gui);
}

} // namespace yetty::card
