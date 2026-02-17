#include "ygui.h"
#include "../../ygui/ygui-engine.h"
#include "../../ygui/ygui-io.h"
#include "../../ygui/ygui-widgets.h"
#include "../../ydraw/ydraw-buffer.h"

// Widget types — YAML demo includes all of them
#include "../../ygui/widgets/label.h"
#include "../../ygui/widgets/button.h"
#include "../../ygui/widgets/checkbox.h"
#include "../../ygui/widgets/slider.h"
#include "../../ygui/widgets/dropdown.h"
#include "../../ygui/widgets/text-input.h"
#include "../../ygui/widgets/progress.h"
#include "../../ygui/widgets/panel.h"
#include "../../ygui/widgets/separator.h"
#include "../../ygui/widgets/selectable.h"
#include "../../ygui/widgets/choicebox.h"
#include "../../ygui/widgets/color-picker.h"
#include "../../ygui/widgets/collapsing-header.h"
#include "../../ygui/widgets/scrollbar.h"
#include "../../ygui/widgets/scroll-area.h"
#include "../../ygui/widgets/table.h"
#include "../../ygui/widgets/tabbar.h"
#include "../../ygui/widgets/popup.h"
#include "../../ygui/widgets/listbox.h"
#include "../../ygui/widgets/tooltip.h"

#include <cmath>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include <yetty/base/event-loop.h>
#include <yetty/osc-command.h>
#include <yetty/ydraw-builder.h>
#include <ytrace/ytrace.hpp>

namespace yetty::card {

//=============================================================================
// OscYGuiIO — in-process IO that formats events as OSC sequences
//=============================================================================
class OscYGuiIO : public yetty::ygui::YGuiIO {
public:
    OscYGuiIO(const std::string& cardName) : _cardName(cardName) {}

    void emitEvent(const yetty::ygui::WidgetEvent& event) override {
        std::ostringstream ss;
        ss << "\033]" << YETTY_OSC_VENDOR_ID
           << ";event;card=" << _cardName
           << ";widget=" << event.widgetId
           << ";type=" << event.eventType;
        if (!event.key.empty())
            ss << ";" << event.key << "=" << event.value;
        ss << "\033\\";
        yinfo("YGui event: {}", ss.str());
    }

private:
    std::string _cardName;
};

//=============================================================================
// YAML widget parser (demo glue — not part of the library)
//=============================================================================
static yetty::ygui::WidgetPtr parseWidget(const YAML::Node& node);

static std::string yamlStr(const YAML::Node& node, const std::string& key,
                           const std::string& def = "") {
    if (node[key]) return node[key].as<std::string>();
    return def;
}

static float yamlFloat(const YAML::Node& node, const std::string& key, float def = 0) {
    if (node[key]) return node[key].as<float>();
    return def;
}

static int yamlInt(const YAML::Node& node, const std::string& key, int def = 0) {
    if (node[key]) return node[key].as<int>();
    return def;
}

static bool yamlBool(const YAML::Node& node, const std::string& key, bool def = false) {
    if (node[key]) return node[key].as<bool>();
    return def;
}

static std::vector<std::string> yamlStringList(const YAML::Node& node, const std::string& key) {
    std::vector<std::string> result;
    if (node[key] && node[key].IsSequence()) {
        for (const auto& item : node[key])
            result.push_back(item.as<std::string>());
    }
    return result;
}

static std::vector<float> yamlFloatList(const YAML::Node& node, const std::string& key) {
    std::vector<float> result;
    if (node[key] && node[key].IsSequence()) {
        for (const auto& item : node[key])
            result.push_back(item.as<float>());
    }
    return result;
}

static uint32_t parseColor(const std::string& colorStr) {
    if (colorStr.size() > 2 && colorStr[0] == '0' && (colorStr[1] == 'x' || colorStr[1] == 'X'))
        return static_cast<uint32_t>(std::stoul(colorStr, nullptr, 16));
    if (!colorStr.empty() && colorStr[0] == '#') {
        uint32_t rgb = static_cast<uint32_t>(std::stoul(colorStr.substr(1), nullptr, 16));
        uint8_t r = (rgb >> 16) & 0xFF;
        uint8_t g = (rgb >> 8) & 0xFF;
        uint8_t b = rgb & 0xFF;
        return 0xFF000000 | (b << 16) | (g << 8) | r;
    }
    return 0xFFFFFFFF;
}

// Apply common widget properties from YAML node
static void applyCommon(yetty::ygui::Widget* w, const YAML::Node& node) {
    w->id = yamlStr(node, "id");
    w->x = yamlFloat(node, "x");
    w->y = yamlFloat(node, "y");
    if (node["w"] || node["width"])
        w->w = (node["w"] ? node["w"] : node["width"]).as<float>();
    if (node["h"] || node["height"])
        w->h = (node["h"] ? node["h"] : node["height"]).as<float>();
    w->onClick = yamlStr(node, "on_click");
    w->onChange = yamlStr(node, "on_change");

    if (yamlBool(node, "checked")) w->flags |= yetty::ygui::WIDGET_CHECKED;
    if (yamlBool(node, "disabled")) w->flags |= yetty::ygui::WIDGET_DISABLED;
    if (yamlBool(node, "open")) w->flags |= yetty::ygui::WIDGET_OPEN;

    // Parse children
    if (node["children"]) {
        for (const auto& child : node["children"]) {
            auto cw = parseWidget(child);
            if (cw) w->children.push_back(cw);
        }
    }
}

static yetty::ygui::WidgetPtr parseWidget(const YAML::Node& node) {
    using namespace yetty::ygui;
    std::string t = yamlStr(node, "type");

    if (t == "label") {
        auto w = std::make_shared<Label>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "button") {
        auto w = std::make_shared<Button>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "checkbox") {
        auto w = std::make_shared<Checkbox>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "slider") {
        auto w = std::make_shared<Slider>();
        applyCommon(w.get(), node);
        w->value = yamlFloat(node, "value");
        w->minValue = yamlFloat(node, "min");
        w->maxValue = yamlFloat(node, "max", 1);
        return w;
    }
    if (t == "dropdown") {
        auto w = std::make_shared<Dropdown>();
        applyCommon(w.get(), node);
        w->options = yamlStringList(node, "options");
        w->selectedIndex = yamlInt(node, "selected");
        return w;
    }
    if (t == "textinput") {
        auto w = std::make_shared<TextInput>();
        applyCommon(w.get(), node);
        w->text = yamlStr(node, "text");
        return w;
    }
    if (t == "progress") {
        auto w = std::make_shared<Progress>();
        applyCommon(w.get(), node);
        w->value = yamlFloat(node, "value");
        w->minValue = yamlFloat(node, "min");
        w->maxValue = yamlFloat(node, "max", 1);
        return w;
    }
    if (t == "panel") {
        auto w = std::make_shared<Panel>();
        applyCommon(w.get(), node);
        return w;
    }
    if (t == "separator" || t == "sep") {
        auto w = std::make_shared<Separator>();
        applyCommon(w.get(), node);
        return w;
    }
    if (t == "selectable") {
        auto w = std::make_shared<Selectable>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "choicebox" || t == "choice" || t == "radio") {
        auto w = std::make_shared<ChoiceBox>();
        applyCommon(w.get(), node);
        w->options = yamlStringList(node, "options");
        w->selectedIndex = yamlInt(node, "selected");
        if (!w->options.empty())
            w->h = (float)w->options.size() * 24.0f;
        return w;
    }
    if (t == "colorpicker" || t == "color") {
        auto w = std::make_shared<ColorPicker>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        if (node["color"])
            w->colorValue = parseColor(node["color"].as<std::string>());
        return w;
    }
    if (t == "header" || t == "th") {
        auto w = std::make_shared<TableHeader>();
        applyCommon(w.get(), node);
        return w;
    }
    if (t == "collapsing") {
        auto w = std::make_shared<CollapsingHeader>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "vscrollbar" || t == "vscroll") {
        auto w = std::make_shared<VScrollbar>();
        applyCommon(w.get(), node);
        w->value = yamlFloat(node, "value");
        return w;
    }
    if (t == "hscrollbar" || t == "hscroll") {
        auto w = std::make_shared<HScrollbar>();
        applyCommon(w.get(), node);
        w->value = yamlFloat(node, "value");
        return w;
    }
    if (t == "scrollarea" || t == "scroll") {
        auto w = std::make_shared<ScrollArea>();
        applyCommon(w.get(), node);
        w->contentW = yamlFloat(node, "content_w");
        w->contentH = yamlFloat(node, "content_h");
        return w;
    }
    if (t == "table") {
        auto w = std::make_shared<Table>();
        applyCommon(w.get(), node);
        w->columnWidths = yamlFloatList(node, "columns");
        w->rowHeight = yamlFloat(node, "row_height", 24);
        if (node["header_color"])
            w->headerColor = parseColor(node["header_color"].as<std::string>());
        return w;
    }
    if (t == "row" || t == "tr") {
        auto w = std::make_shared<TableRow>();
        applyCommon(w.get(), node);
        return w;
    }
    if (t == "cell" || t == "td") {
        auto w = std::make_shared<TableCell>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "tabbar" || t == "tabs") {
        auto w = std::make_shared<TabBar>();
        applyCommon(w.get(), node);
        w->activeTab = yamlInt(node, "active_tab", yamlInt(node, "active"));
        return w;
    }
    if (t == "tab" || t == "tabitem") {
        auto w = std::make_shared<TabItem>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }
    if (t == "popup" || t == "dialog" || t == "modal") {
        auto w = std::make_shared<Popup>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        w->modal = yamlBool(node, "modal");
        if (node["header_color"])
            w->headerColor = parseColor(node["header_color"].as<std::string>());
        return w;
    }
    if (t == "listbox" || t == "list") {
        auto w = std::make_shared<ListBox>();
        applyCommon(w.get(), node);
        w->options = yamlStringList(node, "options");
        w->selectedIndex = yamlInt(node, "selected");
        w->rowHeight = yamlFloat(node, "row_height", 24);
        if (!w->options.empty() && w->h == 24)
            w->h = std::min((float)w->options.size(), 5.0f) * w->rowHeight;
        return w;
    }
    if (t == "tooltip") {
        auto w = std::make_shared<Tooltip>();
        applyCommon(w.get(), node);
        w->label = yamlStr(node, "label");
        return w;
    }

    // Unknown type — return generic label
    auto w = std::make_shared<Label>();
    applyCommon(w.get(), node);
    w->label = yamlStr(node, "label", "?");
    return w;
}

static void parseWidgets(const std::string& yaml, yetty::ygui::YGuiEngine& engine) {
    try {
        YAML::Node root = YAML::Load(yaml);
        if (root["widgets"]) {
            for (const auto& node : root["widgets"]) {
                auto w = parseWidget(node);
                if (w) engine.addWidget(w);
            }
        }
    } catch (const std::exception& e) {
        yerror("YGui: YAML parse error: {}", e.what());
    }
}

//=============================================================================
// YGuiImpl — thin card wrapper
//=============================================================================
class YGuiImpl : public YGui {
public:
    YGuiImpl(const YettyContext& ctx, int32_t x, int32_t y, uint32_t widthCells,
             uint32_t heightCells, const std::string& args,
             const std::string& payload)
        : YGui(ctx.cardManager, ctx.gpu, x, y, widthCells, heightCells),
          _screenId(ctx.screenId), _fontManager(ctx.fontManager),
          _gpuAllocator(ctx.gpuAllocator), _argsStr(args), _payloadStr(payload) {
        _shaderGlyph = SHADER_GLYPH;
        _buffer = *yetty::YDrawBuffer::create();
    }

    ~YGuiImpl() override { dispose(); }

    uint32_t metadataSlotIndex() const override {
        return _metaHandle.offset / 64;
    }

    //=========================================================================
    // Lifecycle
    //=========================================================================

    Result<void> init() {
        auto metaRes = _cardMgr->allocateMetadata(64);
        if (!metaRes)
            return Err<void>("YGui: failed to allocate metadata");
        _metaHandle = *metaRes;

        auto builderRes = YDrawBuilder::create(
            _fontManager, _gpuAllocator, _buffer, _cardMgr, metadataSlotIndex());
        if (!builderRes)
            return Err<void>("YGui: failed to create builder", builderRes);
        _builder = *builderRes;
        _builder->setViewport(_widthCells, _heightCells);

        // Create IO and engine
        _io = std::make_unique<OscYGuiIO>(name());
        _engine = std::make_unique<yetty::ygui::YGuiEngine>(_buffer.get(), _io.get());

        _pixelWidth = static_cast<float>(_widthCells * 10);
        _pixelHeight = static_cast<float>(_heightCells * 20);
        _engine->setPixelSize(_pixelWidth, _pixelHeight);

        // Parse widgets from YAML payload (demo glue)
        if (!_payloadStr.empty())
            parseWidgets(_payloadStr, *_engine);

        yinfo("YGui::init: size {}x{} cells, {}x{} pixels", _widthCells,
              _heightCells, _pixelWidth, _pixelHeight);

        // Initial render
        _engine->rebuild();

        _builder->setSceneBounds(0, 0, _pixelWidth, _pixelHeight);
        _builder->setBgColor(0xFF1A1A2E);
        _builder->calculate();

        // Register for events
        if (auto res = registerForEvents(); !res)
            yerror("YGui::init: failed to register events: {}", error_msg(res));

        return Ok();
    }

    Result<void> dispose() override {
        deregisterFromEvents();
        if (_metaHandle.isValid() && _cardMgr) {
            _cardMgr->deallocateMetadata(_metaHandle);
            _metaHandle = MetadataHandle::invalid();
        }
        return Ok();
    }

    void suspend() override {}

    //=========================================================================
    // Buffer management — delegates to builder
    //=========================================================================

    void declareBufferNeeds() override {
        if (!_builder) return;
        if (auto res = _builder->declareBufferNeeds(); !res)
            yerror("YGui::declareBufferNeeds: {}", error_msg(res));
    }

    Result<void> allocateBuffers() override {
        if (!_builder) return Ok();
        return _builder->allocateBuffers();
    }

    bool needsBufferRealloc() override {
        if (!_builder) return false;
        return _builder->needsBufferRealloc();
    }

    bool needsTexture() const override {
        return _builder && _builder->hasCustomAtlas();
    }

    bool needsTextureRealloc() override {
        return _builder && _builder->needsTextureRealloc();
    }

    Result<void> allocateTextures() override {
        if (!_builder) return Ok();
        return _builder->allocateTextures();
    }

    Result<void> writeTextures() override {
        if (!_builder) return Ok();
        return _builder->writeTextures();
    }

    //=========================================================================
    // Rendering
    //=========================================================================

    void renderToStaging(float time) override {
        (void)time;
        if (!_builder || !_engine) return;
        if (_engine->isDirty()) {
            _engine->rebuild();
            _builder->calculate();
        }
    }

    Result<void> finalize() override {
        if (!_builder) return Ok();
        return _builder->writeBuffers();
    }

    //=========================================================================
    // Cell size
    //=========================================================================

    void setCellSize(float cellWidth, float cellHeight) override {
        if (_engine) _engine->setCellSize(cellWidth, cellHeight);
    }

    //=========================================================================
    // Update command
    //=========================================================================

    Result<void> update(const std::string& args,
                        const std::string& payload) override {
        std::istringstream iss(args);
        std::string token, widgetId, property, value;
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

        if (widgetId.empty())
            return Err<void>("YGui::update: missing widget id");

        auto w = _engine->findWidgetById(widgetId);
        if (!w)
            return Err<void>("YGui::update: widget not found: " + widgetId);

        // Update basic properties on the base widget
        if (property == "disabled") {
            if (value == "true" || value == "1")
                w->flags |= yetty::ygui::WIDGET_DISABLED;
            else
                w->flags &= ~yetty::ygui::WIDGET_DISABLED;
        } else if (property == "checked") {
            if (value == "true" || value == "1")
                w->flags |= yetty::ygui::WIDGET_CHECKED;
            else
                w->flags &= ~yetty::ygui::WIDGET_CHECKED;
        }

        _engine->markDirty();
        return Ok();
    }

    //=========================================================================
    // Event dispatch — receives events from EventLoop
    //=========================================================================

    Result<bool> onEvent(const base::Event& event) override {
        if (!_engine) return Ok(false);

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
                _engine->handleMouseDown(event.cardMouse.x, event.cardMouse.y,
                                         event.cardMouse.button);
                return Ok(true);
            }
            break;

        case base::Event::Type::CardMouseUp:
            if (event.cardMouse.targetId == id()) {
                _engine->handleMouseUp(event.cardMouse.x, event.cardMouse.y,
                                       event.cardMouse.button);
                return Ok(true);
            }
            break;

        case base::Event::Type::CardMouseMove:
            if (event.cardMouse.targetId == id()) {
                _engine->handleMouseMove(event.cardMouse.x, event.cardMouse.y);
                return Ok(true);
            }
            break;

        case base::Event::Type::CardScroll:
            if (event.cardScroll.targetId == id()) {
                _engine->handleMouseScroll(event.cardScroll.x, event.cardScroll.y,
                                           event.cardScroll.dx, event.cardScroll.dy);
                return Ok(true);
            }
            break;

        case base::Event::Type::CardKeyDown:
            if (event.cardKey.targetId == id()) {
                _engine->handleKeyDown(static_cast<uint32_t>(event.cardKey.key),
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
        if (!loopRes) return Ok();
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
    FontManager::Ptr _fontManager;
    GpuAllocator::Ptr _gpuAllocator;
    std::string _argsStr;
    std::string _payloadStr;

    yetty::YDrawBuffer::Ptr _buffer;
    YDrawBuilder::Ptr _builder;
    std::unique_ptr<OscYGuiIO> _io;
    std::unique_ptr<yetty::ygui::YGuiEngine> _engine;

    float _pixelWidth = 0;
    float _pixelHeight = 0;
    bool _isFocused = false;
};

//=============================================================================
// Factory
//=============================================================================

Result<CardPtr> YGui::create(const YettyContext& ctx, int32_t x, int32_t y,
                             uint32_t widthCells, uint32_t heightCells,
                             const std::string& args,
                             const std::string& payload) {
    auto card = std::make_shared<YGuiImpl>(ctx, x, y, widthCells, heightCells,
                                           args, payload);
    if (auto res = card->init(); !res)
        return Err<CardPtr>("YGui::create: init failed", res);
    return Ok<CardPtr>(card);
}

Result<YGui::Ptr> YGui::createImpl(ContextType&, const YettyContext& ctx,
                                   int32_t x, int32_t y, uint32_t widthCells,
                                   uint32_t heightCells,
                                   const std::string& args,
                                   const std::string& payload) noexcept {
    auto result = create(ctx, x, y, widthCells, heightCells, args, payload);
    if (!result)
        return Err<Ptr>("Failed to create YGui", result);
    auto gui = std::dynamic_pointer_cast<YGui>(*result);
    if (!gui)
        return Err<Ptr>("Created card is not YGui");
    return Ok(gui);
}

} // namespace yetty::card
