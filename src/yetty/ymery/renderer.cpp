#include "renderer.h"
#include "data-tree.h"
#include "simple-data-tree.h"
#include <imgui.h>
#include <ytrace/ytrace.hpp>

namespace yetty::ymery {

// ---- Factory ----

Result<Renderer::Ptr> Renderer::create(
    LangPtr lang,
    DispatcherPtr dispatcher,
    TreeLikePtr dataTree,
    std::map<std::string, TreeLikePtr> externalTrees
) {
    auto r = Ptr(new Renderer());
    r->_lang = lang;
    r->_dispatcher = dispatcher;
    r->_dataTree = dataTree;
    r->_dataTrees["data"] = dataTree;

    // Merge external trees (e.g., config from YettyContext)
    for (auto& [name, tree] : externalTrees) {
        r->_dataTrees[name] = std::move(tree);
        yinfo("Renderer: registered external tree '{}'", name);
    }

    // Determine render list from app config
    const auto& app = lang->appConfig();
    std::string rootWidgetName;
    auto rwIt = app.find("root-widget");
    if (rwIt != app.end()) {
        if (auto name = getAs<std::string>(rwIt->second)) {
            rootWidgetName = *name;
            r->_renderList.push_back(*name);
        }
    }

    // Pre-create data trees from top-level lang data definitions
    for (const auto& [treeName, treeDef] : lang->dataDefinitions()) {
        auto argIt = treeDef.find("arg");
        Dict arg;
        if (argIt != treeDef.end()) {
            if (auto a = getAs<Dict>(argIt->second)) arg = *a;
        }
        auto treeRes = DataTree::create(arg);
        if (treeRes) {
            r->_dataTrees[treeName] = *treeRes;
            yinfo("Renderer: pre-created top-level data tree '{}'", treeName);
        }
    }

    // Pre-create data trees from root widget definition so they persist across frames
    std::string mainDataKey = "data";
    if (!rootWidgetName.empty()) {
        // Qualify the name: try "module.name" for all known modules
        std::string qualifiedName;
        const auto& defs = lang->widgetDefinitions();
        for (const auto& [name, def] : defs) {
            size_t dot = name.rfind('.');
            if (dot != std::string::npos && name.substr(dot + 1) == rootWidgetName) {
                qualifiedName = name;
                break;
            }
        }
        // Also try exact match
        if (qualifiedName.empty() && defs.count(rootWidgetName)) {
            qualifiedName = rootWidgetName;
        }

        if (!qualifiedName.empty()) {
            const auto& widgetDef = defs.at(qualifiedName);

            // Process "data" section to create persistent trees
            auto dataIt = widgetDef.find("data");
            if (dataIt != widgetDef.end()) {
                if (auto dataDict = getAs<Dict>(dataIt->second)) {
                    for (const auto& [treeName, treeDef] : *dataDict) {
                        auto defDict = getAs<Dict>(treeDef);
                        if (!defDict) continue;

                        std::string treeType = "data-tree";
                        auto typeIt = defDict->find("type");
                        if (typeIt != defDict->end()) {
                            if (auto t = getAs<std::string>(typeIt->second)) treeType = *t;
                        }

                        Dict arg;
                        auto argIt = defDict->find("arg");
                        if (argIt != defDict->end()) {
                            if (auto a = getAs<Dict>(argIt->second)) arg = *a;
                        }

                        if (treeType == "data-tree") {
                            auto treeRes = DataTree::create(arg);
                            if (treeRes) {
                                r->_dataTrees[treeName] = *treeRes;
                                yinfo("Renderer: pre-created data tree '{}' from root widget", treeName);
                            }
                        } else if (treeType == "simple-data-tree") {
                            auto treeRes = SimpleDataTree::create();
                            if (treeRes) {
                                r->_dataTrees[treeName] = *treeRes;
                            }
                        }
                    }
                }
            }

            // Process "main-data" to set correct main data key
            auto mainDataIt = widgetDef.find("main-data");
            if (mainDataIt != widgetDef.end()) {
                if (auto key = getAs<std::string>(mainDataIt->second)) {
                    mainDataKey = *key;
                }
            }
        }
    }

    // Create root DataBag once — persists across frames
    auto bagRes = DataBag::create(
        dispatcher,
        r->_dataTrees,
        mainDataKey,
        DataPath::root(),
        {} // empty statics for root
    );
    if (!bagRes) {
        return Err<Ptr>("Renderer::create: failed to create root DataBag", bagRes);
    }
    r->_rootBag = *bagRes;

    return r;
}

Result<void> Renderer::setRenderList(std::vector<std::string> widgets) {
    _renderList = std::move(widgets);
    return Ok();
}

// ---- Frame entry point ----

Result<void> Renderer::renderFrame() {
    static bool first = true;
    if (first) {
        ydebug("renderFrame: {} widgets in render list", _renderList.size());
        for (const auto& n : _renderList) ydebug("  renderList: '{}'", n);
        first = false;
    }
    for (const auto& name : _renderList) {
        if (auto res = renderWidget(name); !res) {
            return Err<void>("renderFrame: failed to render widget '" + name + "'", res);
        }
    }
    return Ok();
}

Result<void> Renderer::renderWidget(const std::string& widgetName) {
    // Use persistent root DataBag
    return _renderSpec(_rootBag, Value(widgetName), "app");
}

// ---- Core recursive render ----

Result<void> Renderer::_render(DataBagPtr bag) {
    // Get type
    std::string type = "composite";
    auto typeRes = bag->getStatic("type");
    if (typeRes) {
        if (auto t = getAs<std::string>(*typeRes)) {
            type = *t;
            // Strip plugin prefix (e.g. "imgui.button" -> "button")
            auto dot = type.rfind('.');
            if (dot != std::string::npos) {
                type = type.substr(dot + 1);
            }
        }
    }
    ydebug("_render: type='{}' uid='{}'", type, bag->uidStr());

    // Parse event handlers
    _currentEventHandlers.clear();
    auto ehRes = bag->getStatic("event-handlers");
    if (ehRes) {
        if (auto handlers = getAs<Dict>(*ehRes)) {
            for (const auto& [eventName, commandsVal] : *handlers) {
                if (auto commands = getAs<List>(commandsVal)) {
                    for (const auto& cmd : *commands) {
                        if (auto cmdDict = getAs<Dict>(cmd)) {
                            _currentEventHandlers[eventName].push_back(*cmdDict);
                        }
                    }
                } else if (auto cmdDict = getAs<Dict>(commandsVal)) {
                    _currentEventHandlers[eventName].push_back(*cmdDict);
                } else if (auto cmdStr = getAs<std::string>(commandsVal)) {
                    Dict simpleCmd;
                    simpleCmd[*cmdStr] = Value{};
                    _currentEventHandlers[eventName].push_back(simpleCmd);
                }
            }
        }
    }

    // Push styles
    auto styleCountRes = _pushStyles(bag);
    if (!styleCountRes) {
        return Err<void>("_render: failed to push styles", styleCountRes);
    }
    int styleCount = *styleCountRes;

    // Dispatch begin (ImGui call)
    auto beginRes = _dispatchBegin(type, bag);
    if (!beginRes) {
        _popStyles(styleCount);
        return Err<void>("_render: dispatchBegin failed", beginRes);
    }
    BeginResult result = *beginRes;

    // Tooltip
    if (ImGui::IsItemHovered()) {
        auto tooltipRes = bag->getStatic("tooltip");
        if (tooltipRes) {
            if (auto text = getAs<std::string>(*tooltipRes)) {
                ImGui::SetTooltip("%s", text->c_str());
            }
        }
    }

    // Events (on-click, on-hover, etc.)
    if (auto res = _handleEvents(bag, result); !res) {
        return Err<void>("_render: handleEvents failed", res);
    }

    // Render body if container is open
    bool isContainer = (type == "window" || type == "child" || type == "group" ||
                        type == "tree-node" || type == "tab-bar" || type == "tab-item" ||
                        type == "menu-bar" || type == "menu" ||
                        type == "popup" || type == "popup-modal" || type == "tooltip" ||
                        type == "collapsing-header" || type == "composite" ||
                        type == "main-menu-bar" || type == "table" ||
                        type == "row" || type == "column" ||
                        type == "table-row" || type == "table-column" ||
                        type == "dockable-window" || type == "docking-main-window");

    if (isContainer && result.opened) {
        if (auto res = _renderBody(bag); !res) {
            // Log but don't fail — still need to call dispatchEnd
            ydebug("_render: renderBody failed: {}", res.error().to_string());
        }
    }

    // Also render "body" for button when clicked (ymery pattern: button body = on-click popup etc.)
    if (type == "button" && result.clicked) {
        if (auto res = _renderBody(bag); !res) {
            ydebug("_render: button body failed: {}", res.error().to_string());
        }
    }

    // Dispatch end
    if (auto res = _dispatchEnd(type, result.opened); !res) {
        _popStyles(styleCount);
        return Err<void>("_render: dispatchEnd failed", res);
    }

    // Pop styles
    if (auto res = _popStyles(styleCount); !res) {
        return Err<void>("_render: popStyles failed", res);
    }

    return Ok();
}

// ---- Body rendering ----

Result<void> Renderer::_renderBody(DataBagPtr bag) {
    auto bodyRes = bag->getStatic("body");
    if (!bodyRes) {
        ydebug("_renderBody: no body found");
        return Ok();
    }

    auto bodyVal = *bodyRes;
    if (!bodyVal.has_value()) {
        ydebug("_renderBody: body has no value");
        return Ok();
    }

    // Determine namespace from bag
    std::string ns = "app"; // default

    // Normalize body to list
    List bodyList;
    if (auto list = getAs<List>(bodyVal)) {
        bodyList = *list;
    } else if (auto str = getAs<std::string>(bodyVal)) {
        bodyList.push_back(bodyVal);
    } else if (auto dict = getAs<Dict>(bodyVal)) {
        bodyList.push_back(bodyVal);
    } else {
        return Err<void>("_renderBody: body is not list/string/dict");
    }

    ydebug("_renderBody: {} items", bodyList.size());

    for (const auto& item : bodyList) {
        // Check for foreach-child
        if (auto dict = getAs<Dict>(item)) {
            auto fcIt = dict->find("foreach-child");
            if (fcIt != dict->end()) {
                ydebug("_renderBody: found foreach-child");
                if (auto res = _renderForeachChild(bag, fcIt->second); !res) {
                    return Err<void>("_renderBody: foreach-child failed", res);
                }
                continue;
            }
        }

        // Regular widget
        if (auto res = _renderSpec(bag, item, ns); !res) {
            return Err<void>("_renderBody: renderSpec failed", res);
        }
    }

    return Ok();
}

Result<void> Renderer::_renderForeachChild(DataBagPtr bag, const Value& templateSpec) {
    auto childrenRes = bag->getChildrenNames();
    if (!childrenRes) {
        return Err<void>("_renderForeachChild: getChildrenNames failed", childrenRes);
    }

    auto childNames = *childrenRes;
    ydebug("_renderForeachChild: {} children at path '{}'",
           childNames.size(), bag->getDataPathStr() ? *bag->getDataPathStr() : "?");

    // Get the widget spec from the template
    Value widgetSpec;
    if (auto list = getAs<List>(templateSpec)) {
        if (!list->empty()) {
            widgetSpec = (*list)[0];
        }
    } else {
        widgetSpec = templateSpec;
    }

    std::string ns = "app";

    for (const auto& childName : childNames) {
        ImGui::PushID(childName.c_str());

        // Clone spec and inject data-path
        if (auto specDict = getAs<Dict>(widgetSpec)) {
            Dict newSpec = *specDict;
            // Find the widget key and add data-path
            for (auto& [wkey, wval] : newSpec) {
                if (auto innerDict = getAs<Dict>(wval)) {
                    Dict newInner = *innerDict;
                    newInner["data-path"] = Value(childName);
                    newSpec[wkey] = Value(newInner);
                } else {
                    Dict newInner;
                    newInner["data-path"] = Value(childName);
                    newSpec[wkey] = Value(newInner);
                }
                break;
            }
            if (auto res = _renderSpec(bag, Value(newSpec), ns); !res) {
                ImGui::PopID();
                return Err<void>("_renderForeachChild: renderSpec failed for child '" + childName + "'", res);
            }
        } else if (auto specStr = getAs<std::string>(widgetSpec)) {
            Dict newSpec;
            Dict inner;
            inner["data-path"] = Value(childName);
            newSpec[*specStr] = Value(inner);
            if (auto res = _renderSpec(bag, Value(newSpec), ns); !res) {
                ImGui::PopID();
                return Err<void>("_renderForeachChild: renderSpec failed for child '" + childName + "'", res);
            }
        } else {
            if (auto res = _renderSpec(bag, widgetSpec, ns); !res) {
                ImGui::PopID();
                return Err<void>("_renderForeachChild: renderSpec failed", res);
            }
        }

        ImGui::PopID();
    }

    return Ok();
}

// ---- Spec parsing and widget creation ----

Result<void> Renderer::_renderSpec(DataBagPtr parentBag, const Value& spec, const std::string& ns) {
    // If spec is a list, render each item
    if (auto specList = getAs<List>(spec)) {
        for (const auto& item : *specList) {
            if (auto res = _renderSpec(parentBag, item, ns); !res) {
                return Err<void>("_renderSpec: list item failed", res);
            }
        }
        return Ok();
    }

    // Parse spec to get name + inline props
    auto parseRes = _parseWidgetSpec(spec, ns);
    if (!parseRes) {
        return Err<void>("_renderSpec: failed to parse spec", parseRes);
    }

    auto [widgetName, inlineProps] = *parseRes;
    ydebug("_renderSpec: '{}' ({} props)", widgetName, inlineProps.size());

    // Resolve definition
    auto defRes = _resolveWidgetDefinition(widgetName);
    if (!defRes) {
        _renderError("Unknown widget: " + widgetName);
        return Err<void>("_renderSpec: unknown widget '" + widgetName + "'", defRes);
    }

    Dict widgetDef = *defRes;

    // Merge inline props
    for (const auto& [key, value] : inlineProps) {
        widgetDef[key] = value;
    }

    // Create DataBag
    auto bagRes = _createBagForSpec(parentBag, widgetName, widgetDef, ns);
    if (!bagRes) {
        _renderError("Failed to create DataBag for: " + widgetName);
        return Err<void>("_renderSpec: failed to create DataBag for '" + widgetName + "'", bagRes);
    }

    ImGui::PushID(widgetName.c_str());
    auto renderRes = _render(*bagRes);
    ImGui::PopID();

    if (!renderRes) {
        return Err<void>("_renderSpec: render failed for '" + widgetName + "'", renderRes);
    }

    return Ok();
}

Result<std::pair<std::string, Dict>> Renderer::_parseWidgetSpec(
    const Value& spec,
    const std::string& ns
) {
    Dict inlineProps;

    // String spec: "button" or "app.button"
    if (auto specStr = getAs<std::string>(spec)) {
        std::string name = *specStr;
        if (name.find('.') == std::string::npos) {
            name = ns + "." + name;
        }
        return Ok(std::make_pair(name, inlineProps));
    }

    // Dict spec: {button: {label: "Click"}}
    if (auto specDict = getAs<Dict>(spec)) {
        if (specDict->empty()) {
            return Err<std::pair<std::string, Dict>>("empty dict spec");
        }

        std::string name;
        for (const auto& [key, value] : *specDict) {
            if (key != "data-path") {
                name = key;
                if (name.find('.') == std::string::npos) {
                    name = ns + "." + name;
                }
                if (auto props = getAs<Dict>(value)) {
                    inlineProps = *props;
                }
                break;
            }
        }

        if (name.empty()) {
            return Err<std::pair<std::string, Dict>>("no widget key in spec");
        }

        // Check for sibling data-path
        auto dpIt = specDict->find("data-path");
        if (dpIt != specDict->end()) {
            inlineProps["data-path"] = dpIt->second;
        }

        return Ok(std::make_pair(name, inlineProps));
    }

    return Err<std::pair<std::string, Dict>>("invalid spec type");
}

Result<Dict> Renderer::_resolveWidgetDefinition(const std::string& fullName) {
    const auto& defs = _lang->widgetDefinitions();
    auto it = defs.find(fullName);
    if (it != defs.end()) {
        return Ok(it->second);
    }

    // For built-in ImGui types, create a minimal definition
    // Extract the widget type name (after last dot)
    size_t dotPos = fullName.rfind('.');
    std::string typeName = (dotPos != std::string::npos)
        ? fullName.substr(dotPos + 1)
        : fullName;

    // Known built-in types that don't need YAML definitions
    static const std::vector<std::string> builtins = {
        "text", "button", "small-button", "checkbox", "window", "group",
        "input-text", "input-int", "input-float",
        "slider-int", "slider-float", "drag-int", "drag-float",
        "combo", "listbox", "selectable", "radio-button",
        "separator", "spacing", "same-line",
        "child", "collapsing-header", "tab-bar", "tab-item", "tree-node",
        "popup", "popup-modal", "tooltip", "menu-bar", "menu", "menu-item",
        "main-menu-bar", "color-edit", "color-button", "progress-bar",
        "column", "next-column", "row", "indent", "bullet-text", "separator-text",
        "table", "table-row", "table-column",
        "imgui-main-window", "composite",
        "docking-main-window", "docking-split", "dockable-window"
    };

    for (const auto& b : builtins) {
        if (typeName == b) {
            Dict def;
            def["type"] = Value(typeName);
            return Ok(def);
        }
    }

    return Err<Dict>("widget '" + fullName + "' not found");
}

Result<DataBagPtr> Renderer::_createBagForSpec(
    DataBagPtr parentBag,
    const std::string& widgetName,
    const Dict& widgetDef,
    const std::string& ns
) {
    // Create statics from widget definition (excluding data-path)
    Dict statics;
    for (const auto& [key, value] : widgetDef) {
        if (key != "data-path") {
            statics[key] = value;
        }
    }

    // Get data-path from definition
    std::string dataPathSpec;
    auto dpIt = widgetDef.find("data-path");
    if (dpIt != widgetDef.end()) {
        if (auto pathStr = getAs<std::string>(dpIt->second)) {
            dataPathSpec = *pathStr;
        }
    }

    // Inherit from parent
    return parentBag->inherit(dataPathSpec, statics);
}

// ---- ImGui dispatch: begin ----

static ImGuiWindowFlags _resolveWindowFlags(DataBagPtr bag) {
    ImGuiWindowFlags flags = 0;
    auto check = [&](const char* key, ImGuiWindowFlags flag) {
        auto res = bag->getStatic(key);
        if (res) {
            if (auto b = getAs<bool>(*res); b && *b) flags |= flag;
        }
    };
    check("no-title-bar", ImGuiWindowFlags_NoTitleBar);
    check("no-resize", ImGuiWindowFlags_NoResize);
    check("no-move", ImGuiWindowFlags_NoMove);
    check("no-scrollbar", ImGuiWindowFlags_NoScrollbar);
    check("no-collapse", ImGuiWindowFlags_NoCollapse);
    check("no-background", ImGuiWindowFlags_NoBackground);
    check("always-auto-resize", ImGuiWindowFlags_AlwaysAutoResize);
    check("menu-bar", ImGuiWindowFlags_MenuBar);
    check("horizontal-scrollbar", ImGuiWindowFlags_HorizontalScrollbar);
    return flags;
}

static std::string _getLabel(DataBagPtr bag, const std::string& fallback = "") {
    // First try static label (most widgets have static labels)
    auto staticRes = bag->getStatic("label");
    if (staticRes) {
        if (auto s = getAs<std::string>(*staticRes)) return *s;
    }
    // Then try dynamic label from data tree
    auto res = bag->get("label");
    if (res) {
        if (auto s = getAs<std::string>(*res)) return *s;
    }
    return fallback;
}

static std::string _getTitle(DataBagPtr bag, const std::string& fallback = "Window") {
    auto res = bag->getStatic("title");
    if (res) {
        if (auto s = getAs<std::string>(*res)) return *s;
    }
    // Fall back to label
    return _getLabel(bag, fallback);
}

static std::string _getContent(DataBagPtr bag) {
    auto res = bag->get("content");
    if (res) {
        if (auto s = getAs<std::string>(*res)) return *s;
    }
    return _getLabel(bag);
}

Result<Renderer::BeginResult> Renderer::_dispatchBegin(const std::string& type, DataBagPtr bag) {
    BeginResult r;
    std::string label = _getLabel(bag);

    if (type == "text") {
        std::string content = _getContent(bag);
        ydebug("dispatch text: '{}'", content);
        ImGui::TextUnformatted(content.c_str());
    }
    else if (type == "bullet-text") {
        ImGui::BulletText("%s", _getContent(bag).c_str());
    }
    else if (type == "separator-text") {
        ImGui::SeparatorText(_getContent(bag).c_str());
    }
    else if (type == "button") {
        r.clicked = ImGui::Button(label.c_str());
    }
    else if (type == "small-button") {
        r.clicked = ImGui::SmallButton(label.c_str());
    }
    else if (type == "separator") {
        ImGui::Separator();
    }
    else if (type == "same-line") {
        ImGui::SameLine();
    }
    else if (type == "spacing") {
        ImGui::Spacing();
    }
    else if (type == "indent") {
        float w = 0;
        auto res = bag->getStatic("width");
        if (res) { if (auto d = getAs<double>(*res)) w = (float)*d; }
        ImGui::Indent(w);
    }
    else if (type == "checkbox") {
        bool v = false;
        auto vr = bag->get("value");
        if (vr) { if (auto b = getAs<bool>(*vr)) v = *b; }
        r.changed = ImGui::Checkbox(label.c_str(), &v);
        if (r.changed) bag->set("value", Value(v));
    }
    else if (type == "tree-node") {
        // Check if this node has children in the data tree OR a static body
        auto childNames = bag->getChildrenNames();
        auto bodyRes = bag->getStatic("body");
        bool hasDataChildren = childNames && !(*childNames).empty();
        bool hasStaticBody = bodyRes && (*bodyRes).has_value();
        bool isLeaf = !hasDataChildren && !hasStaticBody;
        if (isLeaf) {
            ImGui::TreeNodeEx(label.c_str(),
                ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
            r.opened = false;  // no children to render, no TreePop needed
        } else {
            r.opened = ImGui::TreeNode(label.c_str());
        }
    }
    else if (type == "window" || type == "imgui-main-window" || type == "dockable-window") {
        std::string title = _getTitle(bag);
        ImGuiWindowFlags flags = _resolveWindowFlags(bag);
        ydebug("dispatch window: title='{}' flags={}", title, (int)flags);
        r.opened = ImGui::Begin(title.c_str(), nullptr, flags);
        ydebug("dispatch window: opened={}", r.opened);
    }
    else if (type == "collapsing-header") {
        ImGuiTreeNodeFlags flags = 0;
        auto dr = bag->getStatic("default-open");
        if (dr) { if (auto b = getAs<bool>(*dr); b && *b) flags |= ImGuiTreeNodeFlags_DefaultOpen; }
        // Also check "flags" list for "default-open"
        auto flagsRes = bag->getStatic("flags");
        if (flagsRes) {
            if (auto flagsList = getAs<List>(*flagsRes)) {
                for (const auto& f : *flagsList) {
                    if (auto s = getAs<std::string>(f); s && *s == "default-open") {
                        flags |= ImGuiTreeNodeFlags_DefaultOpen;
                    }
                }
            }
        }
        ydebug("dispatch collapsing-header: label='{}' flags={}", label, (int)flags);
        r.opened = ImGui::CollapsingHeader(label.c_str(), flags);
    }
    else if (type == "tab-bar") {
        r.opened = ImGui::BeginTabBar("##tabbar");
    }
    else if (type == "tab-item") {
        r.opened = ImGui::BeginTabItem(label.c_str());
    }
    else if (type == "menu-bar") {
        r.opened = ImGui::BeginMenuBar();
    }
    else if (type == "main-menu-bar") {
        r.opened = ImGui::BeginMainMenuBar();
    }
    else if (type == "menu") {
        r.opened = ImGui::BeginMenu(label.c_str());
    }
    else if (type == "menu-item") {
        r.clicked = ImGui::MenuItem(label.c_str());
    }
    else if (type == "group") {
        ImGui::BeginGroup();
    }
    else if (type == "child") {
        float w = 0, h = 0;
        auto sr = bag->getStatic("size");
        if (sr) {
            if (auto list = getAs<List>(*sr); list && list->size() >= 2) {
                if (auto dw = getAs<double>((*list)[0])) w = (float)*dw;
                else if (auto iw = getAs<int>((*list)[0])) w = (float)*iw;
                if (auto dh = getAs<double>((*list)[1])) h = (float)*dh;
                else if (auto ih = getAs<int>((*list)[1])) h = (float)*ih;
            }
        }
        bool border = false;
        auto br = bag->getStatic("border");
        if (br) { if (auto b = getAs<bool>(*br)) border = *b; }
        r.opened = ImGui::BeginChild("##child", ImVec2(w, h), border ? ImGuiChildFlags_Borders : 0);
    }
    else if (type == "popup") {
        std::string popupId = label;
        auto pid = bag->getStatic("popup-id");
        if (pid) { if (auto s = getAs<std::string>(*pid)) popupId = *s; }
        r.opened = ImGui::BeginPopup(popupId.c_str());
    }
    else if (type == "popup-modal") {
        std::string popupId = label;
        auto pid = bag->getStatic("popup-id");
        if (pid) { if (auto s = getAs<std::string>(*pid)) popupId = *s; }
        r.opened = ImGui::BeginPopupModal(popupId.c_str());
    }
    else if (type == "tooltip") {
        ImGui::BeginTooltip();
    }
    else if (type == "selectable") {
        bool selected = false;
        auto sr = bag->get("selected");
        if (sr) { if (auto b = getAs<bool>(*sr)) selected = *b; }
        r.clicked = ImGui::Selectable(label.c_str(), &selected);
        if (r.clicked) bag->set("selected", Value(selected));
    }
    else if (type == "progress-bar") {
        float fraction = 0;
        auto fr = bag->get("fraction");
        if (fr) { if (auto d = getAs<double>(*fr)) fraction = (float)*d; }
        ImGui::ProgressBar(fraction);
    }
    else if (type == "color-edit") {
        float col[4] = {0, 0, 0, 1};
        auto cr = bag->get("color");
        if (cr) {
            if (auto list = getAs<List>(*cr); list && list->size() >= 3) {
                if (auto d = getAs<double>((*list)[0])) col[0] = (float)*d;
                if (auto d = getAs<double>((*list)[1])) col[1] = (float)*d;
                if (auto d = getAs<double>((*list)[2])) col[2] = (float)*d;
                if (list->size() >= 4) { if (auto d = getAs<double>((*list)[3])) col[3] = (float)*d; }
            }
        }
        r.changed = ImGui::ColorEdit4(label.c_str(), col);
    }
    else if (type == "input-text") {
        std::string buf(256, '\0');
        auto vr = bag->get("value");
        if (vr) { if (auto s = getAs<std::string>(*vr)) buf = *s; buf.resize(256); }
        if (ImGui::InputText(label.c_str(), buf.data(), buf.size())) {
            bag->set("value", Value(std::string(buf.c_str())));
            r.changed = true;
        }
    }
    else if (type == "slider-int") {
        int v = 0, mn = 0, mx = 100;
        auto vr = bag->get("value"); if (vr) { if (auto i = getAs<int>(*vr)) v = *i; }
        auto mnr = bag->getStatic("min"); if (mnr) { if (auto i = getAs<int>(*mnr)) mn = *i; }
        auto mxr = bag->getStatic("max"); if (mxr) { if (auto i = getAs<int>(*mxr)) mx = *i; }
        r.changed = ImGui::SliderInt(label.c_str(), &v, mn, mx);
        if (r.changed) bag->set("value", Value(v));
    }
    else if (type == "slider-float") {
        float v = 0, mn = 0, mx = 1;
        auto vr = bag->get("value"); if (vr) { if (auto d = getAs<double>(*vr)) v = (float)*d; }
        auto mnr = bag->getStatic("min"); if (mnr) { if (auto d = getAs<double>(*mnr)) mn = (float)*d; }
        auto mxr = bag->getStatic("max"); if (mxr) { if (auto d = getAs<double>(*mxr)) mx = (float)*d; }
        r.changed = ImGui::SliderFloat(label.c_str(), &v, mn, mx);
        if (r.changed) bag->set("value", Value((double)v));
    }
    else if (type == "drag-int") {
        int v = 0;
        auto vr = bag->get("value"); if (vr) { if (auto i = getAs<int>(*vr)) v = *i; }
        r.changed = ImGui::DragInt(label.c_str(), &v);
        if (r.changed) bag->set("value", Value(v));
    }
    else if (type == "drag-float") {
        float v = 0;
        auto vr = bag->get("value"); if (vr) { if (auto d = getAs<double>(*vr)) v = (float)*d; }
        r.changed = ImGui::DragFloat(label.c_str(), &v);
        if (r.changed) bag->set("value", Value((double)v));
    }
    else if (type == "radio-button") {
        bool active = false;
        auto ar = bag->get("active"); if (ar) { if (auto b = getAs<bool>(*ar)) active = *b; }
        r.clicked = ImGui::RadioButton(label.c_str(), active);
    }
    else if (type == "table") {
        int columns = 1;
        auto cr = bag->getStatic("columns"); if (cr) { if (auto i = getAs<int>(*cr)) columns = *i; }
        r.opened = ImGui::BeginTable("##table", columns);
    }
    else if (type == "table-row") {
        ImGui::TableNextRow();
        r.opened = true;  // Allow body to render
    }
    else if (type == "table-column") {
        ImGui::TableNextColumn();
        r.opened = true;  // Allow body to render
    }
    else if (type == "next-column") {
        ImGui::NextColumn();
    }
    else if (type == "row") {
        // In table context, this acts as TableNextRow
        ImGui::TableNextRow();
        r.opened = true;
    }
    else if (type == "column") {
        // In table context, this acts as TableNextColumn
        ImGui::TableNextColumn();
        r.opened = true;
    }
    else if (type == "composite") {
        // No ImGui call, just render body
    }
    else if (type == "docking-main-window" || type == "docking-split") {
        // These are HelloImGui-specific, treat as composites here
    }

    return Ok(r);
}

// ---- ImGui dispatch: end ----

Result<void> Renderer::_dispatchEnd(const std::string& type, bool wasOpened) {
    // Types that ALWAYS need End (even if Begin returned false)
    if (type == "window" || type == "imgui-main-window" || type == "dockable-window") {
        ImGui::End();
    }
    else if (type == "child") {
        ImGui::EndChild();
    }
    else if (type == "group") {
        ImGui::EndGroup();
    }
    // Types that only End when opened
    else if (wasOpened) {
        if (type == "tree-node")       ImGui::TreePop();
        else if (type == "tab-bar")    ImGui::EndTabBar();
        else if (type == "tab-item")   ImGui::EndTabItem();
        else if (type == "menu-bar")   ImGui::EndMenuBar();
        else if (type == "main-menu-bar") ImGui::EndMainMenuBar();
        else if (type == "menu")       ImGui::EndMenu();
        else if (type == "popup")      ImGui::EndPopup();
        else if (type == "popup-modal") ImGui::EndPopup();
        else if (type == "tooltip")    ImGui::EndTooltip();
        else if (type == "table")      ImGui::EndTable();
    }
    // collapsing-header, composite, separator, text, button, etc: no end call
    return Ok();
}

// ---- Styles ----

static int _getImguiColorIdx(const std::string& name) {
    static const std::map<std::string, int> colorMap = {
        {"text", ImGuiCol_Text},
        {"text-disabled", ImGuiCol_TextDisabled},
        {"window-bg", ImGuiCol_WindowBg},
        {"child-bg", ImGuiCol_ChildBg},
        {"popup-bg", ImGuiCol_PopupBg},
        {"border", ImGuiCol_Border},
        {"frame-bg", ImGuiCol_FrameBg},
        {"frame-bg-hovered", ImGuiCol_FrameBgHovered},
        {"frame-bg-active", ImGuiCol_FrameBgActive},
        {"button", ImGuiCol_Button},
        {"button-hovered", ImGuiCol_ButtonHovered},
        {"button-active", ImGuiCol_ButtonActive},
        {"header", ImGuiCol_Header},
        {"header-hovered", ImGuiCol_HeaderHovered},
        {"header-active", ImGuiCol_HeaderActive},
        {"slider-grab", ImGuiCol_SliderGrab},
        {"slider-grab-active", ImGuiCol_SliderGrabActive},
    };
    auto it = colorMap.find(name);
    return it != colorMap.end() ? it->second : -1;
}

Result<int> Renderer::_pushStyles(DataBagPtr bag) {
    int count = 0;
    auto styleRes = bag->getStatic("style");
    if (!styleRes) return Ok(0);

    auto styleDict = getAs<Dict>(*styleRes);
    if (!styleDict) return Ok(0);

    for (const auto& [name, val] : *styleDict) {
        int idx = _getImguiColorIdx(name);
        if (idx >= 0) {
            if (auto colorList = getAs<List>(val)) {
                if (colorList->size() >= 3) {
                    float r = 1, g = 1, b = 1, a = 1;
                    if (auto rv = getAs<double>((*colorList)[0])) r = (float)*rv;
                    if (auto gv = getAs<double>((*colorList)[1])) g = (float)*gv;
                    if (auto bv = getAs<double>((*colorList)[2])) b = (float)*bv;
                    if (colorList->size() >= 4) {
                        if (auto av = getAs<double>((*colorList)[3])) a = (float)*av;
                    }
                    ImGui::PushStyleColor(idx, ImVec4(r, g, b, a));
                    count++;
                }
            }
        }
    }

    return Ok(count);
}

Result<void> Renderer::_popStyles(int count) {
    if (count > 0) {
        ImGui::PopStyleColor(count);
    }
    return Ok();
}

// ---- Events ----

Result<void> Renderer::_handleEvents(DataBagPtr bag, const BeginResult& result) {
    if (result.clicked) {
        if (auto res = _executeEventCommands(bag, "on-click"); !res) {
            return Err<void>("_handleEvents: on-click failed", res);
        }
    }
    if (ImGui::IsItemHovered()) {
        if (auto res = _executeEventCommands(bag, "on-hover"); !res) {
            return Err<void>("_handleEvents: on-hover failed", res);
        }
    }
    if (result.changed) {
        if (auto res = _executeEventCommands(bag, "on-change"); !res) {
            return Err<void>("_handleEvents: on-change failed", res);
        }
    }
    return Ok();
}

Result<void> Renderer::_executeEventCommands(DataBagPtr bag, const std::string& eventName) {
    auto it = _currentEventHandlers.find(eventName);
    if (it == _currentEventHandlers.end()) return Ok();

    for (const auto& command : it->second) {
        if (auto res = _executeEventCommand(bag, command); !res) {
            return Err<void>("_executeEventCommands: command failed for event '" + eventName + "'", res);
        }
    }
    return Ok();
}

Result<void> Renderer::_executeEventCommand(DataBagPtr bag, const Dict& command) {
    for (const auto& [cmdType, cmdData] : command) {
        if (cmdType == "dispatch-event") {
            Dict event;
            if (auto name = getAs<std::string>(cmdData)) {
                event["name"] = cmdData;
            } else if (auto eventDict = getAs<Dict>(cmdData)) {
                event = *eventDict;
            }
            _dispatcher->dispatchEvent(event);
        } else if (cmdType == "close") {
            ImGui::CloseCurrentPopup();
        } else if (cmdType == "open-popup") {
            if (auto popupId = getAs<std::string>(cmdData)) {
                ImGui::OpenPopup(popupId->c_str());
            }
        }
        break; // Only process first command key
    }
    return Ok();
}

// ---- Error rendering ----

Result<void> Renderer::_renderError(const std::string& msg) {
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
    ImGui::TextWrapped("Error: %s", msg.c_str());
    ImGui::PopStyleColor();
    return Ok();
}

} // namespace yetty::ymery
