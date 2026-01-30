#pragma once

#include "types.h"
#include "data-bag.h"
#include "dispatcher.h"
#include "lang.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <imgui.h>

namespace yetty::ymery {

// Renderer - replaces the entire Widget/Composite/WidgetFactory class hierarchy.
//
// Instead of per-type widget classes, we have ONE recursive function that:
// 1. Reads the widget "type" from statics
// 2. Dispatches to the right ImGui call (begin/end)
// 3. Recurses into "body" children
// 4. Handles foreach-child iteration over data tree
// 5. Handles self-referencing/recursive widget definitions
//
// The DataBag carries both YAML statics and data tree position,
// so the renderer just reads from it and calls ImGui directly.
class Renderer {
public:
    using Ptr = std::shared_ptr<Renderer>;

    // Factory
    static Result<Ptr> create(
        LangPtr lang,
        DispatcherPtr dispatcher,
        TreeLikePtr dataTree
    );

    // Set render list (top-level widgets to render each frame)
    Result<void> setRenderList(std::vector<std::string> widgets);

    // Render all widgets in the render list (call each frame)
    Result<void> renderFrame();

    // Render a named widget by name
    Result<void> renderWidget(const std::string& widgetName);

    // Access
    LangPtr lang() const { return _lang; }
    DispatcherPtr dispatcher() const { return _dispatcher; }

private:
    Renderer() = default;

    // Core recursive render: given a DataBag with statics + data position, render it
    Result<void> _render(DataBagPtr bag);

    // Render body items (sequence from "body" static)
    Result<void> _renderBody(DataBagPtr bag);

    // Ensure children for foreach-child pattern
    Result<void> _renderForeachChild(DataBagPtr bag, const Value& templateSpec);

    // Parse a widget spec (string name or {name: {props}} dict) and render it
    Result<void> _renderSpec(DataBagPtr parentBag, const Value& spec, const std::string& ns);

    // Create a DataBag for a widget from spec
    Result<DataBagPtr> _createBagForSpec(
        DataBagPtr parentBag,
        const std::string& widgetName,
        const Dict& inlineProps,
        const std::string& ns
    );

    // Parse spec to get (widgetName, inlineProps)
    Result<std::pair<std::string, Dict>> _parseWidgetSpec(
        const Value& spec,
        const std::string& ns
    );

    // Resolve widget definition from name
    Result<Dict> _resolveWidgetDefinition(const std::string& fullName);

    // --- ImGui dispatch ---
    // Returns: is the container "open" (should we render body?)
    struct BeginResult {
        bool opened = true;
        bool clicked = false;
        bool changed = false;
    };

    Result<BeginResult> _dispatchBegin(const std::string& type, DataBagPtr bag);
    Result<void> _dispatchEnd(const std::string& type, bool wasOpened);

    // Style push/pop
    Result<int> _pushStyles(DataBagPtr bag);
    Result<void> _popStyles(int count);

    // Event handling
    Result<void> _handleEvents(DataBagPtr bag, const BeginResult& result);
    Result<void> _executeEventCommands(DataBagPtr bag, const std::string& eventName);
    Result<void> _executeEventCommand(DataBagPtr bag, const Dict& command);

    // Error rendering
    Result<void> _renderError(const std::string& msg);

    LangPtr _lang;
    DispatcherPtr _dispatcher;
    TreeLikePtr _dataTree;
    std::vector<std::string> _renderList;

    // Named data trees (from lang's data definitions)
    std::map<std::string, TreeLikePtr> _dataTrees;

    // Event handlers parsed from statics (cached per-render)
    std::map<std::string, std::vector<Dict>> _currentEventHandlers;
};

} // namespace yetty::ymery
