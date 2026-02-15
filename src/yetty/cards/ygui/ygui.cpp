#include "ygui.h"
#include "../../ygui/ygui-parser.h"
#include "../../ygui/ygui-renderer.h"
#include "../hdraw/hdraw.h" // For SDFPrimitive, YDrawMetadata
#include <cmath>
#include <cstring>
#include <sstream>
#include <yetty/base/event-loop.h>
#include <yetty/osc-command.h>
#include <yetty/result.hpp>
#include <yetty/ydraw-builder.h>
#include <yetty/ydraw-writer.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

// Import ygui types
using ygui::Widget;
using ygui::WidgetPtr;
using ygui::WidgetType;
using ygui::YGuiRenderer;
using ygui::YGuiRenderState;

//=============================================================================
// YGuiImpl - Card implementation using ygui library
//=============================================================================

class YGuiImpl : public YGui {
public:
  YGuiImpl(const YettyContext &ctx, int32_t x, int32_t y, uint32_t widthCells,
           uint32_t heightCells, const std::string &args,
           const std::string &payload)
      : YGui(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells),
        _screenId(ctx.screenId), _argsStr(args), _payloadStr(payload) {
    _shaderGlyph = SHADER_GLYPH;

    // Create YDrawWriterInternal for rendering
    auto writerRes =
        YDrawWriterInternal::create(ctx.fontManager, ctx.globalAllocator);
    if (writerRes) {
      _writer = *writerRes;
      _renderer = std::make_unique<YGuiRenderer>(_writer);
    } else {
      yerror("YGuiImpl: failed to create writer");
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
    if (!_writer) {
      return Err<void>("YGui: writer not available");
    }

    // Allocate metadata
    auto metaRes = _cardMgr->allocateMetadata(64);
    if (!metaRes)
      return Err<void>("YGui: failed to allocate metadata");
    _metaHandle = *metaRes;

    // Parse widget tree from payload (YAML) using ygui library
    if (!_payloadStr.empty()) {
      _widgets = ygui::parseWidgets(_payloadStr);
    }

    yinfo("YGui::init: parsed {} widgets", _widgets.size());

    // Calculate pixel dimensions (use cell sizes approx 10x20)
    _pixelWidth = static_cast<float>(_widthCells * 10);
    _pixelHeight = static_cast<float>(_heightCells * 20);

    yinfo("YGui::init: size {}x{} cells, {}x{} pixels", _widthCells,
          _heightCells, _pixelWidth, _pixelHeight);

    // Set scene bounds for renderer
    _renderer->setSceneBounds(_pixelWidth, _pixelHeight);

    // Build initial primitives
    rebuildPrimitives();

    auto builder = _writer->builder();
    yinfo("YGui::init: built {} prims, {} glyphs", builder->primitiveCount(),
          builder->glyphCount());

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
    if (!_writer)
      return;

    auto builder = _writer->builder();
    _primStorage = StorageHandle::invalid();
    _derivedStorage = StorageHandle::invalid();

    const auto &primStaging = builder->primStaging();
    const auto &glyphs = builder->glyphs();

    if (primStaging.empty() && glyphs.empty()) {
      builder->clearGridStaging();
      return;
    }

    // Reserve prim storage
    if (!primStaging.empty()) {
      uint32_t primSize =
          static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
      _cardMgr->bufferManager()->reserve(primSize);
    }

    // Calculate grid
    if (builder->gridStaging().empty()) {
      builder->calculate();
    }

    // Reserve derived storage (grid + glyphs)
    uint32_t gridBytes =
        static_cast<uint32_t>(builder->gridStaging().size()) * sizeof(uint32_t);
    uint32_t glyphBytes =
        static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
    uint32_t derivedSize = gridBytes + glyphBytes;

    if (derivedSize > 0) {
      _cardMgr->bufferManager()->reserve(derivedSize);
    }
  }

  Result<void> allocateBuffers() override {
    if (!_writer)
      return Ok();

    auto builder = _writer->builder();
    const auto &primStaging = builder->primStaging();
    const auto &gridStaging = builder->gridStaging();
    const auto &glyphs = builder->glyphs();

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
    if (!_writer)
      return false;
    auto builder = _writer->builder();
    const auto &primStaging = builder->primStaging();
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
    if (!_writer)
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
    _dirty = true;
  }

  //=========================================================================
  // Event handling - receives pixel coordinates directly
  //=========================================================================

  // Convert actual pixel coordinates to nominal widget coordinates
  float toWidgetX(float px) const { return px * 10.0f / _cellWidthF; }
  float toWidgetY(float py) const { return py * 20.0f / _cellHeightF; }

  void handleMouseMove(float px, float py) {
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
    if (_renderState.openDropdown && _renderState.openDropdown->isOpen()) {
      _renderState.hoverOptionIdx =
          getDropdownOptionAt(_renderState.openDropdown, wx, wy);
    }

    // Update hover for choicebox options
    _renderState.hoverChoiceIdx = -1;
    _renderState.hoverChoiceWidget = nullptr;
    for (auto &w : _widgets) {
      if (w->type == WidgetType::ChoiceBox && w->isHover()) {
        int idx = getChoiceBoxOptionAt(w.get(), wx, wy);
        if (idx >= 0) {
          _renderState.hoverChoiceIdx = idx;
          _renderState.hoverChoiceWidget = w.get();
        }
        break;
      }
    }

    // Update hover for listbox items
    _renderState.hoverListIdx = -1;
    _renderState.hoverListWidget = nullptr;
    for (auto &w : _widgets) {
      auto hit = findListBoxAt(w, wx, wy);
      if (hit.widget) {
        Widget *listbox = hit.widget;
        float itemH = listbox->rowHeight > 0 ? listbox->rowHeight : 24;
        float localY = hit.hitY - listbox->y + listbox->scrollY;
        int idx = static_cast<int>(localY / itemH);
        if (idx >= 0 && idx < static_cast<int>(listbox->options.size())) {
          _renderState.hoverListIdx = idx;
          _renderState.hoverListWidget = listbox;
        }
        break;
      }
    }

    _dirty = true;
  }

  void handleMouseDown(float px, float py, int button) {
    if (button != 0)
      return;

    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    // Handle dropdown: check if clicking on open dropdown's options
    if (_renderState.openDropdown && _renderState.openDropdown->isOpen()) {
      int optionIdx = getDropdownOptionAt(_renderState.openDropdown, wx, wy);
      if (optionIdx >= 0) {
        _renderState.openDropdown->selectedIndex = optionIdx;
        _renderState.openDropdown->flags &= ~ygui::WIDGET_OPEN;
        emitEvent(_renderState.openDropdown, "change");
        _renderState.openDropdown = nullptr;
        _dirty = true;
        return;
      }
      if (_renderState.openDropdown->contains(wx, wy)) {
        _renderState.openDropdown->flags &= ~ygui::WIDGET_OPEN;
        _renderState.openDropdown = nullptr;
        _dirty = true;
        return;
      }
      _renderState.openDropdown->flags &= ~ygui::WIDGET_OPEN;
      _renderState.openDropdown = nullptr;
      _dirty = true;
    }

    // Find clicked widget
    HitResult hit = {nullptr, 0, 0};
    for (auto &w : _widgets) {
      hit = findWidgetAt(w, wx, wy);
      if (hit.widget)
        break;
    }

    Widget *clicked = hit.widget;
    float hitX = hit.hitX;
    float hitY = hit.hitY;

    if (!clicked)
      return;

    // Update focus
    if (_focused != clicked) {
      if (_focused)
        _focused->flags &= ~ygui::WIDGET_FOCUSED;
      _focused = clicked;
      _focused->flags |= ygui::WIDGET_FOCUSED;
    }

    clicked->flags |= ygui::WIDGET_PRESSED;
    _pressed = clicked;

    // Handle widget-specific click
    switch (clicked->type) {
    case WidgetType::Button:
      break;

    case WidgetType::Checkbox:
      clicked->flags ^= ygui::WIDGET_CHECKED;
      emitEvent(clicked, "change");
      break;

    case WidgetType::Dropdown:
      clicked->flags ^= ygui::WIDGET_OPEN;
      if (clicked->isOpen()) {
        _renderState.openDropdown = clicked;
        _renderState.hoverOptionIdx = -1;
      } else {
        _renderState.openDropdown = nullptr;
      }
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
      float optH = 24;
      int optIdx = static_cast<int>((hitY - clicked->y) / optH);
      if (optIdx >= 0 && optIdx < static_cast<int>(clicked->options.size())) {
        clicked->selectedIndex = optIdx;
        emitEvent(clicked, "change");
      }
      break;
    }

    case WidgetType::Selectable:
      clicked->flags ^= ygui::WIDGET_CHECKED;
      emitEvent(clicked, "click");
      break;

    case WidgetType::CollapsingHeader:
      clicked->flags ^= ygui::WIDGET_OPEN;
      emitEvent(clicked, "click");
      break;

    case WidgetType::ScrollArea: {
      float scrollbarW = 12;
      float scrollbarH = 12;
      float viewW = clicked->w - scrollbarW;
      float viewH = clicked->h - scrollbarH;
      float contentW = clicked->contentW > 0 ? clicked->contentW : viewW;
      float contentH = clicked->contentH > 0 ? clicked->contentH : viewH;

      float localX = hitX - clicked->x;
      float localY = hitY - clicked->y;

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

    case WidgetType::Popup:
      clicked->flags ^= ygui::WIDGET_OPEN;
      emitEvent(clicked, "click");
      break;

    case WidgetType::ListBox: {
      float itemH = clicked->rowHeight;
      float localY = hitY - clicked->y + clicked->scrollY;
      int itemIdx = static_cast<int>(localY / itemH);
      if (itemIdx >= 0 && itemIdx < static_cast<int>(clicked->options.size())) {
        clicked->selectedIndex = itemIdx;
        emitEvent(clicked, "change");
      }
      break;
    }

    case WidgetType::TableRow:
      clicked->flags ^= ygui::WIDGET_CHECKED;
      emitEvent(clicked, "click");
      break;

    default:
      break;
    }

    _dirty = true;
  }

  void handleMouseUp(float px, float py, int button) {
    if (button != 0)
      return;

    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

    if (_pressed) {
      if (_pressed->type == WidgetType::Button && _pressed->contains(wx, wy)) {
        emitEvent(_pressed, "click");
      }
      _pressed->flags &= ~ygui::WIDGET_PRESSED;
      _pressed = nullptr;
      _dirty = true;
    }

    _scrollDragWidget = nullptr;
  }

  void handleMouseScroll(float px, float py, float deltaX, float deltaY) {
    float wx = toWidgetX(px);
    float wy = toWidgetY(py);

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
        float scrollbarW = 12;
        float scrollbarH = 12;
        float viewW = target->w - scrollbarW;
        float viewH = target->h - scrollbarH;
        float contentW = target->contentW > 0 ? target->contentW : viewW;
        float contentH = target->contentH > 0 ? target->contentH : viewH;
        float maxScrollX = std::max(0.0f, contentW - viewW);
        float maxScrollY = std::max(0.0f, contentH - viewH);

        target->scrollY =
            std::clamp(target->scrollY - deltaY * 20.0f, 0.0f, maxScrollY);
        target->scrollX =
            std::clamp(target->scrollX - deltaX * 20.0f, 0.0f, maxScrollX);
        emitEvent(target, "scroll");
        _dirty = true;
      } else if (target->type == WidgetType::ListBox) {
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

    Widget *w = findWidgetById(widgetId);
    if (!w) {
      return Err<void>("YGui::update: widget not found: " + widgetId);
    }

    if (property == "value") {
      w->value = std::stof(value);
    } else if (property == "text" || property == "label") {
      w->label = value;
    } else if (property == "checked") {
      if (value == "true" || value == "1")
        w->flags |= ygui::WIDGET_CHECKED;
      else
        w->flags &= ~ygui::WIDGET_CHECKED;
    } else if (property == "disabled") {
      if (value == "true" || value == "1")
        w->flags |= ygui::WIDGET_DISABLED;
      else
        w->flags &= ~ygui::WIDGET_DISABLED;
    } else if (property == "selected") {
      w->selectedIndex = std::stoi(value);
    }

    _dirty = true;
    return Ok();
  }

private:
  //=========================================================================
  // Primitive building using renderer
  //=========================================================================

  void rebuildPrimitives() {
    if (!_renderer || !_writer)
      return;

    // Update render state
    _renderState.pixelWidth = _pixelWidth;
    _renderState.pixelHeight = _pixelHeight;

    // Render widgets using the library renderer
    _renderer->setSceneBounds(_pixelWidth, _pixelHeight);
    _renderer->render(_widgets, _renderState);

    // Get builder for grid calculation and buffer updates
    auto builder = _writer->builder();
    builder->calculate();

    // Copy staging to storage if already allocated
    if (_primStorage.isValid()) {
      const auto &primStaging = builder->primStaging();
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
      const auto &gridStaging = builder->gridStaging();
      const auto &glyphs = builder->glyphs();

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

  //=========================================================================
  // Event helpers
  //=========================================================================

  void updateHoverRecursive(WidgetPtr &w, float px, float py) {
    bool hover = w->contains(px, py);
    if (hover)
      w->flags |= ygui::WIDGET_HOVER;
    else
      w->flags &= ~ygui::WIDGET_HOVER;

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
      w->flags |= ygui::WIDGET_HOVER;
    else
      w->flags &= ~ygui::WIDGET_HOVER;

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
    float hitX = 0;
    float hitY = 0;
  };

  HitResult findWidgetAt(WidgetPtr &w, float px, float py) {
    if (w->type == WidgetType::ScrollArea && w->contains(px, py)) {
      float contentX = (px - w->x) + w->scrollX;
      float contentY = (py - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findWidgetAtContent(child, contentX, contentY);
        if (hit.widget)
          return hit;
      }
      return {w.get(), px, py};
    }

    for (auto &child : w->children) {
      auto hit = findWidgetAt(child, px, py);
      if (hit.widget)
        return hit;
    }
    if (w->contains(px, py))
      return {w.get(), px, py};
    return {nullptr, 0, 0};
  }

  HitResult findWidgetAtContent(WidgetPtr &w, float contentX, float contentY) {
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
    float y = w->y + w->h;
    float optH = 24;

    for (size_t i = 0; i < w->options.size(); i++) {
      float oy = y + i * optH;
      if (wx >= x && wx < x + w->w && wy >= oy && wy < oy + optH) {
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
    float hitY = 0;
  };

  ListBoxHit findListBoxAt(WidgetPtr &w, float wx, float wy) {
    if (w->type == WidgetType::ScrollArea && w->contains(wx, wy)) {
      float contentX = (wx - w->x) + w->scrollX;
      float contentY = (wy - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findListBoxAtContent(child, contentX, contentY);
        if (hit.widget)
          return hit;
      }
    }
    for (auto &child : w->children) {
      auto hit = findListBoxAt(child, wx, wy);
      if (hit.widget)
        return hit;
    }
    if (w->type == WidgetType::ListBox && w->contains(wx, wy)) {
      return {w.get(), wy};
    }
    return {nullptr, 0};
  }

  ListBoxHit findListBoxAtContent(WidgetPtr &w, float contentX,
                                  float contentY) {
    if (w->type == WidgetType::ScrollArea && w->contains(contentX, contentY)) {
      float nestedX = (contentX - w->x) + w->scrollX;
      float nestedY = (contentY - w->y) + w->scrollY;
      for (auto &child : w->children) {
        auto hit = findListBoxAtContent(child, nestedX, nestedY);
        if (hit.widget)
          return hit;
      }
    }
    for (auto &child : w->children) {
      auto hit = findListBoxAtContent(child, contentX, contentY);
      if (hit.widget)
        return hit;
    }
    if (w->type == WidgetType::ListBox && w->contains(contentX, contentY)) {
      return {w.get(), contentY};
    }
    return {nullptr, 0};
  }

  void updateSliderValue(Widget *w, float wx) {
    float pct = (wx - w->x) / w->w;
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = w->minValue + pct * (w->maxValue - w->minValue);
    emitEvent(w, "change");
  }

  void updateVScrollValue(Widget *w, float wy) {
    float thumbH = std::max(20.0f, w->h * 0.2f);
    float trackRange = w->h - thumbH;
    if (trackRange <= 0)
      return;
    float pct = (wy - w->y - thumbH / 2) / trackRange;
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = pct;
    emitEvent(w, "change");
  }

  void updateHScrollValue(Widget *w, float wx) {
    float thumbW = std::max(20.0f, w->w * 0.2f);
    float trackRange = w->w - thumbW;
    if (trackRange <= 0)
      return;
    float pct = (wx - w->x - thumbW / 2) / trackRange;
    pct = std::clamp(pct, 0.0f, 1.0f);
    w->value = pct;
    emitEvent(w, "change");
  }

  void handleTextInput(Widget *w, uint32_t codepoint, int mods) {
    (void)mods;
    if (codepoint == 8) {
      if (w->cursorPos > 0 && !w->text.empty()) {
        w->text.erase(w->cursorPos - 1, 1);
        w->cursorPos--;
      }
    } else if (codepoint == 127) {
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
    yinfo("YGui event: {}", ss.str());
  }

  //=========================================================================
  // Metadata
  //=========================================================================

  Result<void> uploadMetadata() {
    if (!_metaHandle.isValid()) {
      return Err<void>("YGui::uploadMetadata: invalid handle");
    }
    if (!_writer)
      return Ok();

    auto builder = _writer->builder();
    float sceneMinX = builder->sceneMinX();
    float sceneMinY = builder->sceneMinY();
    float sceneMaxX = builder->sceneMaxX();
    float sceneMaxY = builder->sceneMaxY();
    float cellSize = builder->cellSize();

    YDrawMetadata meta = {};
    meta.primitiveOffset =
        _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
    meta.primitiveCount = _primCount;
    meta.gridOffset = _gridOffset;
    meta.gridWidth = builder->gridWidth();
    meta.gridHeight = builder->gridHeight();
    std::memcpy(&meta.cellSize, &cellSize, sizeof(float));
    meta.glyphOffset = _glyphOffset;
    meta.glyphCount = static_cast<uint32_t>(builder->glyphs().size());
    std::memcpy(&meta.sceneMinX, &sceneMinX, sizeof(float));
    std::memcpy(&meta.sceneMinY, &sceneMinY, sizeof(float));
    std::memcpy(&meta.sceneMaxX, &sceneMaxX, sizeof(float));
    std::memcpy(&meta.sceneMaxY, &sceneMaxY, sizeof(float));
    meta.widthCells = _widthCells & 0xFFFF;
    meta.heightCells = _heightCells & 0xFFFF;
    meta.flags = builder->flags() & 0xFFFF;
    meta.bgColor = builder->bgColor();

    if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta));
        !res) {
      return Err<void>("YGui::uploadMetadata: write failed");
    }
    return Ok();
  }

  //=========================================================================
  // Event dispatch
  //=========================================================================

  Result<bool> onEvent(const base::Event &event) override {
    switch (event.type) {
    case base::Event::Type::SetFocus:
      if (event.setFocus.objectId == id()) {
        _isFocused = true;
        return Ok(true);
      } else if (_isFocused) {
        _isFocused = false;
      }
      return Ok(false);

    case base::Event::Type::CardMouseDown:
      if (event.cardMouse.targetId == id()) {
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
      return Ok();
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

  // YDrawWriter and renderer
  YDrawWriterInternal::Ptr _writer;
  std::unique_ptr<YGuiRenderer> _renderer;
  YGuiRenderState _renderState;

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
  Widget *_scrollDragWidget = nullptr;
  bool _scrollDragVertical = false;

  bool _dirty = true;
  bool _metadataDirty = true;
  bool _isFocused = false;

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
