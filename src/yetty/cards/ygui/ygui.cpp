#include "ygui.h"
#include "../../ydraw/ydraw-builder.h"
#include "../hdraw/hdraw.h"  // For SDFPrimitive, SDFType
#include <yetty/base/event-loop.h>
#include <yetty/osc-command.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
#include <cstring>
#include <cmath>
#include <sstream>

namespace yetty::card {

//=============================================================================
// YGuiImpl
//=============================================================================

class YGuiImpl : public YGui {
public:
    YGuiImpl(const YettyContext& ctx,
             int32_t x, int32_t y,
             uint32_t widthCells, uint32_t heightCells,
             const std::string& args, const std::string& payload)
        : YGui(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells)
        , _screenId(ctx.screenId)
        , _argsStr(args)
        , _payloadStr(payload)
    {
        _shaderGlyph = SHADER_GLYPH;

        // Create YDrawBuilder for proper text/primitive rendering
        auto builderRes = YDrawBuilder::create(ctx.fontManager, ctx.globalAllocator);
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
        if (!metaRes) return Err<void>("YGui: failed to allocate metadata");
        _metaHandle = *metaRes;

        // Parse widget tree from payload (YAML)
        if (!_payloadStr.empty()) {
            parseWidgets(_payloadStr);
        }

        yinfo("YGui::init: parsed {} widgets", _widgets.size());

        // Calculate pixel dimensions (use cell sizes approx 10x20)
        _pixelWidth = static_cast<float>(_widthCells * 10);
        _pixelHeight = static_cast<float>(_heightCells * 20);

        yinfo("YGui::init: size {}x{} cells, {}x{} pixels",
              _widthCells, _heightCells, _pixelWidth, _pixelHeight);

        // Set scene bounds for builder
        _builder->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
        _builder->setBgColor(0xFF1A1A2E);  // Dark theme background

        // Build initial primitives
        rebuildPrimitives();

        yinfo("YGui::init: built {} prims, {} glyphs",
              _builder->primitiveCount(), _builder->glyphCount());

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
        if (!_builder) return;

        _primStorage = StorageHandle::invalid();
        _derivedStorage = StorageHandle::invalid();

        const auto& primStaging = _builder->primStaging();
        const auto& glyphs = _builder->glyphs();

        if (primStaging.empty() && glyphs.empty()) {
            _builder->clearGridStaging();
            return;
        }

        // Reserve prim storage
        if (!primStaging.empty()) {
            uint32_t primSize = static_cast<uint32_t>(primStaging.size()) * sizeof(SDFPrimitive);
            _cardMgr->bufferManager()->reserve(primSize);
        }

        // Calculate grid
        if (_builder->gridStaging().empty()) {
            _builder->calculate();
        }

        // Reserve derived storage (grid + glyphs)
        uint32_t gridBytes = static_cast<uint32_t>(_builder->gridStaging().size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            _cardMgr->bufferManager()->reserve(derivedSize);
        }
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();

        const auto& primStaging = _builder->primStaging();
        const auto& gridStaging = _builder->gridStaging();
        const auto& glyphs = _builder->glyphs();

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
            std::memcpy(_primStorage.data, primStaging.data(), count * sizeof(SDFPrimitive));
            _primCount = count;
            _cardMgr->bufferManager()->markBufferDirty(_primStorage);
        }

        // Allocate derived storage (grid + glyphs)
        uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
        uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));
        uint32_t derivedSize = gridBytes + glyphBytes;

        if (derivedSize > 0) {
            auto storageResult = _cardMgr->bufferManager()->allocateBuffer(
                metadataSlotIndex(), "derived", derivedSize);
            if (!storageResult) {
                return Err<void>("YGui: derived alloc failed");
            }
            _derivedStorage = *storageResult;

            uint8_t* base = _derivedStorage.data;
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
        if (!_builder) return false;
        const auto& primStaging = _builder->primStaging();
        // Need realloc if no storage or storage too small
        if (!primStaging.empty() && !_primStorage.isValid()) return true;
        if (_primStorage.isValid()) {
            uint32_t needed = static_cast<uint32_t>(primStaging.size() * sizeof(SDFPrimitive));
            if (needed > _primStorage.size) return true;
        }
        return false;
    }

    //=========================================================================
    // Rendering
    //=========================================================================

    Result<void> finalize() override {
        if (!_builder) return Ok();

        if (_dirty) {
            rebuildPrimitives();
            _dirty = false;
            _metadataDirty = true;
        }

        if (_metadataDirty) {
            if (auto res = uploadMetadata(); !res) return res;
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
    float toWidgetX(float px) const {
        return px * 10.0f / _cellWidthF;
    }
    float toWidgetY(float py) const {
        return py * 20.0f / _cellHeightF;
    }

    void handleMouseMove(float px, float py) {
        // Convert to widget coordinate system
        float wx = toWidgetX(px);
        float wy = toWidgetY(py);

        // Update hover state
        for (auto& w : _widgets) {
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
            Widget* w = _scrollDragWidget;
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
                float clickRatio = (localY - thumbH/2) / (trackH - thumbH);
                w->scrollY = std::clamp(clickRatio * maxScrollY, 0.0f, maxScrollY);
            } else if (!_scrollDragVertical && contentW > viewW) {
                float maxScrollX = contentW - viewW;
                float trackW = w->w - scrollbarW;
                float thumbW = std::max(20.0f, trackW * viewW / contentW);
                float localX = wx - w->x;
                float clickRatio = (localX - thumbW/2) / (trackW - thumbW);
                w->scrollX = std::clamp(clickRatio * maxScrollX, 0.0f, maxScrollX);
            }
            _dirty = true;
        }

        // Update hover option for open dropdown
        if (_openDropdown && _openDropdown->isOpen()) {
            _hoverOptionIdx = getDropdownOptionAt(_openDropdown, wx, wy);
            ydebug("YGui::handleMouseMove dropdown open, px={:.1f} py={:.1f} -> wx={:.1f} wy={:.1f} hoverIdx={}",
                   px, py, wx, wy, _hoverOptionIdx);
        }

        // Update hover for choicebox options
        _hoverChoiceIdx = -1;
        _hoverChoiceWidget = nullptr;
        for (auto& w : _widgets) {
            if (w->type == WidgetType::ChoiceBox && w->isHover()) {
                int idx = getChoiceBoxOptionAt(w.get(), wx, wy);
                if (idx >= 0) {
                    _hoverChoiceIdx = idx;
                    _hoverChoiceWidget = w.get();
                }
                break;
            }
        }

        _dirty = true;
    }

    void handleMouseDown(float px, float py, int button) {
        if (button != 0) return;  // Left click only

        // Convert to widget coordinate system
        float wx = toWidgetX(px);
        float wy = toWidgetY(py);

        ydebug("YGui::handleMouseDown px={:.1f} py={:.1f} -> wx={:.1f} wy={:.1f}", px, py, wx, wy);

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
                return;  // Don't reopen it
            }
            // Clicked outside dropdown and button - close it
            _openDropdown->flags &= ~WIDGET_OPEN;
            _openDropdown = nullptr;
            _dirty = true;
        }

        // Find clicked widget
        Widget* clicked = nullptr;
        for (auto& w : _widgets) {
            clicked = findWidgetAt(w, wx, wy);
            if (clicked) {
                ydebug("YGui: clicked widget id={} type={}", clicked->id, static_cast<int>(clicked->type));
                break;
            }
        }

        if (!clicked) return;

        // Update focus
        if (_focused != clicked) {
            if (_focused) _focused->flags &= ~WIDGET_FOCUSED;
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
                    _hoverOptionIdx = -1;  // Reset hover, will update on mouse move
                } else {
                    _openDropdown = nullptr;
                }
                ydebug("YGui: dropdown {} open={}", clicked->id, clicked->isOpen());
                break;

            case WidgetType::Slider:
                updateSliderValue(clicked, wx);
                break;

            case WidgetType::VScrollbar:
                updateVScrollValue(clicked, wy);
                break;

            case WidgetType::HScrollbar:
                updateHScrollValue(clicked, wx);
                break;

            case WidgetType::ChoiceBox: {
                // Find which radio button was clicked
                float optH = 24;
                int optIdx = static_cast<int>((wy - clicked->y) / optH);
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

                float localX = wx - clicked->x;
                float localY = wy - clicked->y;

                // Vertical scrollbar region
                if (localX >= viewW && contentH > viewH) {
                    float maxScrollY = contentH - viewH;
                    float trackH = clicked->h - scrollbarH;
                    float thumbH = std::max(20.0f, trackH * viewH / contentH);
                    float clickRatio = (localY - thumbH/2) / (trackH - thumbH);
                    clicked->scrollY = std::clamp(clickRatio * maxScrollY, 0.0f, maxScrollY);
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
                    float clickRatio = (localX - thumbW/2) / (trackW - thumbW);
                    clicked->scrollX = std::clamp(clickRatio * maxScrollX, 0.0f, maxScrollX);
                    _scrollDragWidget = clicked;
                    _scrollDragVertical = false;
                    _dirty = true;
                    break;
                }
                break;
            }

            default:
                break;
        }

        _dirty = true;
    }

    void handleMouseUp(float px, float py, int button) {
        if (button != 0) return;

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

        // Find widget under cursor
        Widget* target = nullptr;
        for (auto& w : _widgets) {
            target = findWidgetAt(w, px, py);
            if (target) break;
        }

        if (target) {
            if (target->type == WidgetType::Slider) {
                float range = target->maxValue - target->minValue;
                float delta = deltaY * range * 0.05f;
                target->value = std::clamp(target->value + delta, target->minValue, target->maxValue);
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
                target->scrollY = std::clamp(target->scrollY - deltaY * 20.0f, 0.0f, maxScrollY);
                target->scrollX = std::clamp(target->scrollX - deltaX * 20.0f, 0.0f, maxScrollX);
                emitEvent(target, "scroll");
                _dirty = true;
            }
        }
    }

    void handleKeyDown(uint32_t key, int mods) {
        if (!_focused) return;

        if (_focused->type == WidgetType::TextInput) {
            handleTextInput(_focused, key, mods);
            _dirty = true;
        }
    }

    //=========================================================================
    // Update command
    //=========================================================================

    Result<void> update(const std::string& args, const std::string& payload) override {
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
                if (key == "widget" || key == "id") widgetId = val;
                else if (key == "property" || key == "prop") property = val;
                else if (key == "value" || key == "val") value = val;
            }
        }

        if (widgetId.empty()) {
            return Err<void>("YGui::update: missing widget id");
        }

        // Find widget
        Widget* w = findWidgetById(widgetId);
        if (!w) {
            return Err<void>("YGui::update: widget not found: " + widgetId);
        }

        // Update property
        if (property == "value") {
            w->value = std::stof(value);
        } else if (property == "text" || property == "label") {
            w->label = value;
        } else if (property == "checked") {
            if (value == "true" || value == "1") w->flags |= WIDGET_CHECKED;
            else w->flags &= ~WIDGET_CHECKED;
        } else if (property == "disabled") {
            if (value == "true" || value == "1") w->flags |= WIDGET_DISABLED;
            else w->flags &= ~WIDGET_DISABLED;
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

    void parseWidgets(const std::string& yaml) {
        try {
            YAML::Node root = YAML::Load(yaml);
            if (root["widgets"]) {
                for (const auto& node : root["widgets"]) {
                    auto w = parseWidget(node);
                    if (w) _widgets.push_back(w);
                }
            }
        } catch (const std::exception& e) {
            yerror("YGui: YAML parse error: {}", e.what());
        }
    }

    WidgetPtr parseWidget(const YAML::Node& node) {
        auto w = std::make_shared<Widget>();

        if (node["id"]) w->id = node["id"].as<std::string>();
        if (node["type"]) {
            std::string t = node["type"].as<std::string>();
            if (t == "label") w->type = WidgetType::Label;
            else if (t == "button") w->type = WidgetType::Button;
            else if (t == "checkbox") w->type = WidgetType::Checkbox;
            else if (t == "slider") w->type = WidgetType::Slider;
            else if (t == "dropdown") w->type = WidgetType::Dropdown;
            else if (t == "textinput") w->type = WidgetType::TextInput;
            else if (t == "progress") w->type = WidgetType::Progress;
            else if (t == "panel") w->type = WidgetType::Panel;
            else if (t == "vbox") w->type = WidgetType::VBox;
            else if (t == "hbox") w->type = WidgetType::HBox;
            else if (t == "choicebox" || t == "choice" || t == "radio") w->type = WidgetType::ChoiceBox;
            else if (t == "separator" || t == "sep") w->type = WidgetType::Separator;
            else if (t == "colorpicker" || t == "color") w->type = WidgetType::ColorPicker;
            else if (t == "selectable") w->type = WidgetType::Selectable;
            else if (t == "collapsing" || t == "header") w->type = WidgetType::CollapsingHeader;
            else if (t == "vscrollbar" || t == "vscroll") w->type = WidgetType::VScrollbar;
            else if (t == "hscrollbar" || t == "hscroll") w->type = WidgetType::HScrollbar;
            else if (t == "scrollarea" || t == "scroll") w->type = WidgetType::ScrollArea;
        }

        if (node["x"]) w->x = node["x"].as<float>();
        if (node["y"]) w->y = node["y"].as<float>();
        if (node["w"] || node["width"]) w->w = (node["w"] ? node["w"] : node["width"]).as<float>();
        if (node["h"] || node["height"]) w->h = (node["h"] ? node["h"] : node["height"]).as<float>();
        if (node["label"]) w->label = node["label"].as<std::string>();
        if (node["text"]) w->text = node["text"].as<std::string>();
        if (node["value"]) w->value = node["value"].as<float>();
        if (node["min"]) w->minValue = node["min"].as<float>();
        if (node["max"]) w->maxValue = node["max"].as<float>();
        if (node["checked"] && node["checked"].as<bool>()) w->flags |= WIDGET_CHECKED;
        if (node["disabled"] && node["disabled"].as<bool>()) w->flags |= WIDGET_DISABLED;

        if (node["options"]) {
            for (const auto& opt : node["options"]) {
                w->options.push_back(opt.as<std::string>());
            }
        }
        if (node["selected"]) w->selectedIndex = node["selected"].as<int>();

        // ColorPicker color value (hex string like "0xFFRRGGBB" or "#RRGGBB")
        if (node["color"]) {
            std::string colorStr = node["color"].as<std::string>();
            if (colorStr.size() > 2 && colorStr[0] == '0' && (colorStr[1] == 'x' || colorStr[1] == 'X')) {
                w->colorValue = static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
            } else if (colorStr.size() > 0 && colorStr[0] == '#') {
                // #RRGGBB -> 0xFFBBGGRR (ABGR)
                uint32_t rgb = static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
                uint8_t r = (rgb >> 16) & 0xFF;
                uint8_t g = (rgb >> 8) & 0xFF;
                uint8_t b = rgb & 0xFF;
                w->colorValue = 0xFF000000 | (b << 16) | (g << 8) | r;
            }
        }

        // CollapsingHeader initial state
        if (node["open"] && node["open"].as<bool>()) w->flags |= WIDGET_OPEN;

        if (node["on_click"]) w->onClick = node["on_click"].as<std::string>();
        if (node["on_change"]) w->onChange = node["on_change"].as<std::string>();

        // Parse children
        if (node["children"]) {
            for (const auto& child : node["children"]) {
                auto cw = parseWidget(child);
                if (cw) w->children.push_back(cw);
            }
        }

        // Auto-calculate height for widgets with options
        if (w->type == WidgetType::ChoiceBox && !w->options.empty()) {
            // Each option is 24 pixels tall
            w->h = static_cast<float>(w->options.size()) * 24.0f;
        }

        // ScrollArea content size
        if (node["content_w"]) w->contentW = node["content_w"].as<float>();
        if (node["content_h"]) w->contentH = node["content_h"].as<float>();

        return w;
    }

    //=========================================================================
    // Primitive building
    //=========================================================================

    void rebuildPrimitives() {
        if (!_builder) return;

        _builder->clear();
        _builder->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
        _builder->setBgColor(0xFF1A1A2E);

        float offsetY = 0;
        for (auto& w : _widgets) {
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
            const auto& primStaging = _builder->primStaging();
            _primCount = static_cast<uint32_t>(primStaging.size());
            if (_primCount > 0 && _primCount * sizeof(SDFPrimitive) <= _primStorage.size) {
                std::memcpy(_primStorage.data, primStaging.data(), _primCount * sizeof(SDFPrimitive));
                _cardMgr->bufferManager()->markBufferDirty(_primStorage);
            }
        }

        // Update derived storage (grid + glyphs)
        if (_derivedStorage.isValid()) {
            const auto& gridStaging = _builder->gridStaging();
            const auto& glyphs = _builder->glyphs();

            uint8_t* base = _derivedStorage.data;
            uint32_t offset = 0;

            uint32_t gridBytes = static_cast<uint32_t>(gridStaging.size()) * sizeof(uint32_t);
            uint32_t glyphBytes = static_cast<uint32_t>(glyphs.size() * sizeof(YDrawGlyph));

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

    void renderWidget(Widget* w, float offsetX, float offsetY) {
        float x = w->x + offsetX;
        float y = w->y + offsetY;

        switch (w->type) {
            case WidgetType::Label:
                addText(w->label, x, y, w->fgColor);
                break;

            case WidgetType::Button:
                addBox(x, y, w->w, w->h, w->isPressed() ? w->accentColor : w->bgColor, 4);
                addText(w->label, x + 8, y + 4, w->fgColor);
                if (w->isHover()) addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
                break;

            case WidgetType::Checkbox: {
                float boxSize = 16;
                addBox(x, y + 2, boxSize, boxSize, w->bgColor, 2);
                if (w->isChecked()) {
                    addBox(x + 3, y + 5, boxSize - 6, boxSize - 6, w->accentColor, 2);
                }
                addText(w->label, x + boxSize + 8, y + 2, w->fgColor);
                if (w->isHover()) addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
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
                    ? w->options[w->selectedIndex] : "";
                addText(text, x + 8, y + 4, w->fgColor);
                // Chevron arrow - size proportional to font (14px -> ~8px arrow)
                float arrowSize = 8;
                float arrowX = x + w->w - 16 - arrowSize/2;
                float arrowY = y + w->h / 2;
                if (w->isOpen()) {
                    // Up arrow (chevron pointing up)
                    addTriangle(arrowX, arrowY + arrowSize/3,
                               arrowX + arrowSize, arrowY + arrowSize/3,
                               arrowX + arrowSize/2, arrowY - arrowSize/3,
                               w->fgColor);
                } else {
                    // Down arrow (chevron pointing down)
                    addTriangle(arrowX, arrowY - arrowSize/3,
                               arrowX + arrowSize, arrowY - arrowSize/3,
                               arrowX + arrowSize/2, arrowY + arrowSize/3,
                               w->fgColor);
                }
                if (w->isHover()) addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
                break;
            }

            case WidgetType::TextInput: {
                addBox(x, y, w->w, w->h, w->bgColor, 4);
                addText(w->text, x + 8, y + 4, w->fgColor);
                if (w->isFocused()) {
                    // Cursor
                    float cursorX = x + 8 + w->cursorPos * 8;  // Approximate
                    addBox(cursorX, y + 4, 2, w->h - 8, w->fgColor, 0);
                }
                if (w->isFocused()) addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);
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
                for (auto& child : w->children) {
                    renderWidget(child.get(), x, y);
                }
                break;

            case WidgetType::VBox: {
                float cy = y;
                for (auto& child : w->children) {
                    child->x = x;
                    child->y = cy;
                    renderWidget(child.get(), 0, 0);
                    cy += child->h + 4;  // spacing
                }
                break;
            }

            case WidgetType::HBox: {
                float cx = x;
                for (auto& child : w->children) {
                    child->x = cx;
                    child->y = y;
                    renderWidget(child.get(), 0, 0);
                    cx += child->w + 4;  // spacing
                }
                break;
            }

            case WidgetType::ChoiceBox: {
                // Radio button group - shows all options vertically
                float optH = 24;
                float cy = y;
                for (size_t i = 0; i < w->options.size(); i++) {
                    bool isSelected = (static_cast<int>(i) == w->selectedIndex);
                    bool isHovered = (_hoverChoiceWidget == w && static_cast<int>(i) == _hoverChoiceIdx);
                    float radioX = x;
                    float radioY = cy + 4;
                    float radioSize = 14;
                    float centerX = radioX + radioSize/2;
                    float centerY = radioY + radioSize/2;

                    // Outer circle (always visible)
                    addCircleOutline(centerX, centerY, radioSize/2,
                                    isHovered ? w->accentColor : 0xFF666677);

                    // Inner dot if selected
                    if (isSelected) {
                        addCircle(centerX, centerY, radioSize/4, w->accentColor);
                    }

                    // Hover highlight (smaller dot when hovering non-selected)
                    if (isHovered && !isSelected) {
                        addCircle(centerX, centerY, radioSize/6, 0xFF555566);
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
                // Color preview box + label
                float boxSize = 20;
                addBox(x, y + 2, boxSize, boxSize, w->colorValue, 2);
                addBoxOutline(x, y + 2, boxSize, boxSize, 0xFF666666, 2);
                if (!w->label.empty()) {
                    addText(w->label, x + boxSize + 8, y + 4, w->fgColor);
                }
                if (w->isHover()) addBoxOutline(x, y + 2, boxSize, boxSize, w->accentColor, 2);
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
                    addTriangle(arrowX, arrowY - arrowSize/3,
                               arrowX + arrowSize, arrowY - arrowSize/3,
                               arrowX + arrowSize/2, arrowY + arrowSize/3,
                               w->fgColor);
                } else {
                    // Right arrow (collapsed)
                    addTriangle(arrowX, arrowY - arrowSize/2,
                               arrowX, arrowY + arrowSize/2,
                               arrowX + arrowSize*0.7f, arrowY,
                               w->fgColor);
                }

                addText(w->label, x + 26, y + 4, w->fgColor);
                if (w->isHover()) addBoxOutline(x, y, w->w, w->h, w->accentColor, 4);

                // Render children if open
                if (w->isOpen()) {
                    float cy = y + w->h + 4;
                    for (auto& child : w->children) {
                        child->x = x + 16;  // Indent children
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
                addBox(x, y, trackW, w->h, 0xFF222233, trackW/2);  // Track

                // Thumb position based on value (0-1 range)
                float thumbH = std::max(20.0f, w->h * 0.2f);  // Min 20px or 20% of track
                float trackRange = w->h - thumbH;
                float thumbY = y + w->value * trackRange;
                uint32_t thumbColor = w->isPressed() ? w->accentColor :
                                     (w->isHover() ? 0xFF555566 : 0xFF444455);
                addBox(x + 2, thumbY, trackW - 4, thumbH, thumbColor, (trackW-4)/2);
                break;
            }

            case WidgetType::HScrollbar: {
                // Horizontal scrollbar: track + thumb
                float trackH = w->h > 0 ? w->h : 12;
                addBox(x, y, w->w, trackH, 0xFF222233, trackH/2);  // Track

                // Thumb position based on value (0-1 range)
                float thumbW = std::max(20.0f, w->w * 0.2f);  // Min 20px or 20% of track
                float trackRange = w->w - thumbW;
                float thumbX = x + w->value * trackRange;
                uint32_t thumbColor = w->isPressed() ? w->accentColor :
                                     (w->isHover() ? 0xFF555566 : 0xFF444455);
                addBox(thumbX, y + 2, thumbW, trackH - 4, thumbColor, (trackH-4)/2);
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
                for (auto& child : w->children) {
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
                    addBox(trackX, y, scrollbarW, trackH, 0xFF222233, scrollbarW/2);

                    float thumbH = std::max(20.0f, trackH * viewH / contentH);
                    float thumbRange = trackH - thumbH;
                    float thumbY = y + (maxScrollY > 0 ? (w->scrollY / maxScrollY) * thumbRange : 0);
                    addBox(trackX + 2, thumbY, scrollbarW - 4, thumbH, 0xFF444455, (scrollbarW-4)/2);
                }

                // Render horizontal scrollbar if content is wider than view
                if (contentW > viewW) {
                    float trackY = y + w->h - scrollbarH;
                    float trackW = w->w - scrollbarW;
                    addBox(x, trackY, trackW, scrollbarH, 0xFF222233, scrollbarH/2);

                    float thumbW = std::max(20.0f, trackW * viewW / contentW);
                    float thumbRange = trackW - thumbW;
                    float thumbX = x + (maxScrollX > 0 ? (w->scrollX / maxScrollX) * thumbRange : 0);
                    addBox(thumbX, trackY + 2, thumbW, scrollbarH - 4, 0xFF444455, (scrollbarH-4)/2);
                }

                // Corner fill
                if (contentW > viewW && contentH > viewH) {
                    addBox(x + w->w - scrollbarW, y + w->h - scrollbarH,
                           scrollbarW, scrollbarH, 0xFF1A1A2E, 0);
                }
                break;
            }
        }
    }

    void renderDropdownOptions(Widget* w) {
        float x = w->x;
        float y = w->y + w->h;
        float optH = 24;

        // Draw dropdown list background
        float listH = static_cast<float>(w->options.size()) * optH;
        addBox(x, y, w->w, listH, 0xFF1E1E2E, 4);  // Darker background

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
                addText("\xE2\x9C\x93", x + w->w - 20, optY + 4, w->accentColor);  // ✓
            }
        }
    }

    //=========================================================================
    // SDF primitive helpers - using YDrawBuilder
    //=========================================================================

    void addBox(float x, float y, float w, float h, uint32_t color, float radius) {
        if (!_builder) return;

        SDFPrimitive p = {};
        p.type = static_cast<uint32_t>(SDFType::RoundedBox);
        p.layer = _builder->primitiveCount();
        p.params[0] = x + w / 2;  // center x
        p.params[1] = y + h / 2;  // center y
        p.params[2] = w / 2;      // half-width
        p.params[3] = h / 2;      // half-height
        p.params[4] = radius;     // corner radii (all same)
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

    void addBoxOutline(float x, float y, float w, float h, uint32_t color, float radius) {
        if (!_builder) return;

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
        p.fillColor = 0;          // No fill
        p.strokeColor = color;
        p.strokeWidth = 2;
        p.round = 0;
        p.aabbMinX = x - 2;
        p.aabbMinY = y - 2;
        p.aabbMaxX = x + w + 2;
        p.aabbMaxY = y + h + 2;
        _builder->addPrimitive(p);
    }

    void addText(const std::string& text, float x, float y, uint32_t color) {
        if (!_builder || text.empty()) return;

        // Use builder's proper MSDF text rendering
        float fontSize = 14.0f;  // Default font size for UI
        if (auto res = _builder->addText(x, y + fontSize * 0.8f, text, fontSize, color, 0); !res) {
            // Fallback: if text rendering fails, silently ignore
            ywarn("YGui::addText failed: {}", error_msg(res));
        }
    }

    void addTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint32_t color) {
        if (!_builder) return;

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
        if (!_builder) return;

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
        if (!_builder) return;

        SDFPrimitive p = {};
        p.type = static_cast<uint32_t>(SDFType::Circle);
        p.layer = _builder->primitiveCount();
        p.params[0] = cx;
        p.params[1] = cy;
        p.params[2] = radius;
        p.fillColor = 0;          // No fill
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

    void updateHoverRecursive(WidgetPtr& w, float px, float py) {
        bool hover = w->contains(px, py);
        if (hover) w->flags |= WIDGET_HOVER;
        else w->flags &= ~WIDGET_HOVER;

        for (auto& child : w->children) {
            updateHoverRecursive(child, px, py);
        }
    }

    Widget* findWidgetAt(WidgetPtr& w, float px, float py) {
        // Check children first (they're on top)
        for (auto& child : w->children) {
            Widget* found = findWidgetAt(child, px, py);
            if (found) return found;
        }
        if (w->contains(px, py)) return w.get();
        return nullptr;
    }

    Widget* findWidgetById(const std::string& id) {
        for (auto& w : _widgets) {
            Widget* found = findWidgetByIdRecursive(w, id);
            if (found) return found;
        }
        return nullptr;
    }

    Widget* findWidgetByIdRecursive(WidgetPtr& w, const std::string& id) {
        if (w->id == id) return w.get();
        for (auto& child : w->children) {
            Widget* found = findWidgetByIdRecursive(child, id);
            if (found) return found;
        }
        return nullptr;
    }

    int getDropdownOptionAt(Widget* w, float wx, float wy) {
        float x = w->x;
        float y = w->y + w->h;  // Options start below the dropdown button
        float optH = 24;

        ydebug("getDropdownOptionAt: wx={:.1f} wy={:.1f} dropdown at x={:.1f} y={:.1f} w={:.1f}",
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

    int getChoiceBoxOptionAt(Widget* w, float wx, float wy) {
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

    void updateSliderValue(Widget* w, float wx) {
        float pct = (wx - w->x) / w->w;
        ydebug("Slider update: wx={:.1f} w->x={:.1f} w->w={:.1f} pct={:.3f} cellW={:.1f}",
               wx, w->x, w->w, pct, _cellWidthF);
        pct = std::clamp(pct, 0.0f, 1.0f);
        w->value = w->minValue + pct * (w->maxValue - w->minValue);
        emitEvent(w, "change");
    }

    void updateVScrollValue(Widget* w, float wy) {
        // Account for thumb size (20% of track or min 20px)
        float thumbH = std::max(20.0f, w->h * 0.2f);
        float trackRange = w->h - thumbH;
        if (trackRange <= 0) return;
        float pct = (wy - w->y - thumbH/2) / trackRange;
        pct = std::clamp(pct, 0.0f, 1.0f);
        w->value = pct;  // Scrollbars use 0-1 range
        emitEvent(w, "change");
    }

    void updateHScrollValue(Widget* w, float wx) {
        // Account for thumb size (20% of track or min 20px)
        float thumbW = std::max(20.0f, w->w * 0.2f);
        float trackRange = w->w - thumbW;
        if (trackRange <= 0) return;
        float pct = (wx - w->x - thumbW/2) / trackRange;
        pct = std::clamp(pct, 0.0f, 1.0f);
        w->value = pct;  // Scrollbars use 0-1 range
        emitEvent(w, "change");
    }

    void handleTextInput(Widget* w, uint32_t codepoint, int mods) {
        if (codepoint == 8) {  // Backspace
            if (w->cursorPos > 0 && !w->text.empty()) {
                w->text.erase(w->cursorPos - 1, 1);
                w->cursorPos--;
            }
        } else if (codepoint == 127) {  // Delete
            if (w->cursorPos < w->text.size()) {
                w->text.erase(w->cursorPos, 1);
            }
        } else if (codepoint >= 32 && codepoint < 127) {
            w->text.insert(w->cursorPos, 1, static_cast<char>(codepoint));
            w->cursorPos++;
        }
        emitEvent(w, "change");
    }

    void emitEvent(Widget* w, const std::string& eventType) {
        std::string eventName;
        if (eventType == "click") eventName = w->onClick;
        else if (eventType == "change") eventName = w->onChange;

        if (eventName.empty()) return;

        // Emit OSC event
        // Format: event;card=<name>;widget=<id>;type=<type>;value=<value>
        std::ostringstream ss;
        ss << "\033]" << YETTY_OSC_VENDOR_ID
           << ";event;card=" << name()
           << ";widget=" << w->id
           << ";type=" << eventType;

        if (w->type == WidgetType::Slider || w->type == WidgetType::Progress ||
            w->type == WidgetType::VScrollbar || w->type == WidgetType::HScrollbar) {
            ss << ";value=" << w->value;
        } else if (w->type == WidgetType::Checkbox || w->type == WidgetType::Selectable) {
            ss << ";checked=" << (w->isChecked() ? "true" : "false");
        } else if (w->type == WidgetType::Dropdown || w->type == WidgetType::ChoiceBox) {
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
        if (!_builder) return Ok();

        float sceneMinX = _builder->sceneMinX();
        float sceneMinY = _builder->sceneMinY();
        float sceneMaxX = _builder->sceneMaxX();
        float sceneMaxY = _builder->sceneMaxY();
        float cellSize = _builder->cellSize();

        YDrawMetadata meta = {};
        meta.primitiveOffset = _primStorage.isValid() ? _primStorage.offset / sizeof(float) : 0;
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

        if (auto res = _cardMgr->writeMetadata(_metaHandle, &meta, sizeof(meta)); !res) {
            return Err<void>("YGui::uploadMetadata: write failed");
        }
        return Ok();
    }

    //=========================================================================
    // Event dispatch - receives events from EventLoop
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
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
                    handleMouseDown(event.cardMouse.x, event.cardMouse.y, event.cardMouse.button);
                    return Ok(true);
                }
                break;

            case base::Event::Type::CardMouseUp:
                if (event.cardMouse.targetId == id()) {
                    handleMouseUp(event.cardMouse.x, event.cardMouse.y, event.cardMouse.button);
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
                    handleKeyDown(static_cast<uint32_t>(event.cardKey.key), event.cardKey.mods);
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
        if (!loopRes) return Ok();  // No event loop
        auto loop = *loopRes;
        auto self = sharedAs<base::EventListener>();
        if (auto res = loop->registerListener(base::Event::Type::SetFocus, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardMouseDown, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardMouseUp, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardMouseMove, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardScroll, self, 1000); !res) return res;
        if (auto res = loop->registerListener(base::Event::Type::CardKeyDown, self, 1000); !res) return res;
        return Ok();
    }

    void deregisterFromEvents() {
        if (weak_from_this().expired()) return;
        auto loopRes = base::EventLoop::instance();
        if (!loopRes) return;
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

    Widget* _focused = nullptr;
    Widget* _pressed = nullptr;
    Widget* _openDropdown = nullptr;
    Widget* _hoverChoiceWidget = nullptr;
    Widget* _scrollDragWidget = nullptr;  // ScrollArea being dragged
    bool _scrollDragVertical = false;     // true=vertical, false=horizontal
    int _hoverOptionIdx = -1;
    int _hoverChoiceIdx = -1;

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

Result<CardPtr> YGui::create(
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    auto card = std::make_shared<YGuiImpl>(ctx, x, y, widthCells, heightCells, args, payload);
    if (auto res = card->init(); !res) {
        return Err<CardPtr>("YGui::create: init failed", res);
    }
    return Ok<CardPtr>(card);
}

Result<YGui::Ptr> YGui::createImpl(
    ContextType&,
    const YettyContext& ctx,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload) noexcept
{
    auto result = create(ctx, x, y, widthCells, heightCells, args, payload);
    if (!result) return Err<Ptr>("Failed to create YGui", result);
    auto gui = std::dynamic_pointer_cast<YGui>(*result);
    if (!gui) return Err<Ptr>("Created card is not YGui");
    return Ok(gui);
}

} // namespace yetty::card
