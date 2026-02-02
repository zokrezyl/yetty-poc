#include <yetty/terminal-view.h>
#include <yetty/widget-frame-renderer.h>
#include <yetty/workspace.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>

namespace yetty {

class WorkspaceImpl : public Workspace {
public:
  explicit WorkspaceImpl(const YettyContext &ctx) : _ctx(ctx) {}

  Result<void> onShutdown() override {
    if (_root) {
      return _root->shutdown();
    }
    return Ok();
  }

  Result<void> init() {
    // Create frame renderer for tiles
    auto frameResult =
        WidgetFrameRenderer::create(_ctx.gpu.device, _ctx.gpu.surfaceFormat);
    if (!frameResult) {
      ywarn("Failed to create frame renderer for workspace: {}",
            frameResult.error().message());
    } else {
      _frameRenderer = std::shared_ptr<WidgetFrameRenderer>(
          frameResult->release());
      yinfo("Workspace frame renderer initialized");
    }

    registerForEvents();
    return Ok();
  }

  Result<void> render(WGPURenderPassEncoder pass) override {
    if (_root) {
      // Update render context before rendering (screen size may have changed)
      updateTileRenderContext();

      if (_frameRenderer) {
        _frameRenderer->resetDrawIndex();
      }

      if (auto r = _root->render(pass); !r) {
        return r;
      }
    }
    return Ok();
  }

  Result<void> resize(float width, float height) override {
    _width = width;
    _height = height;
    if (_root) {
      _root->setBounds({0, 0, _width, _height});
    }
    return Ok();
  }

  Result<void> setRoot(Tile::Ptr tile) override {
    _root = std::move(tile);
    if (_root) {
      updateTileRenderContext();
      if (_width > 0 && _height > 0) {
        _root->setBounds({0, 0, _width, _height});
      }
    }
    return Ok();
  }

  Result<Pane::Ptr> createPane() override {
    auto paneResult = Pane::create();
    if (!paneResult) {
      return Err<Pane::Ptr>("Failed to create pane", paneResult);
    }
    auto pane = *paneResult;

#if !YETTY_WEB && !defined(__ANDROID__)
    auto viewResult = TerminalView::create(_ctx);
    if (!viewResult) {
      return Err<Pane::Ptr>("Failed to create TerminalView", viewResult);
    }
    pane->pushView(*viewResult);
#endif

    return Ok(pane);
  }

  // ─── Event handling ───────────────────────────────────────────────────

  Result<bool> onEvent(const Event &event) override {
    if (event.type == Event::Type::Close) {
      auto targetId = event.closeEv.objectId;
      yinfo("Workspace received Close event for object {}", targetId);
      auto r = closeTarget(targetId);
      if (!r) {
        return Err<bool>("closeTarget failed", r);
      }
      return Ok(true);
    }
    if (event.type == Event::Type::SplitPane) {
      auto targetId = event.splitPane.objectId;
      auto orient = event.splitPane.orientation == 0
          ? Orientation::Horizontal : Orientation::Vertical;
      yinfo("Workspace received SplitPane event for pane {}, orientation={}",
            targetId, event.splitPane.orientation);
      auto r = splitPane(targetId, orient);
      if (!r) {
        return Err<bool>("splitPane failed", r);
      }
      return Ok(true);
    }

    // Handle CommandKey events for workspace-level commands (tmux-style)
    if (event.type == Event::Type::CommandKey) {
      uint32_t cp = event.cmdKey.codepoint;
      ydebug("Workspace received CommandKey: codepoint={}", cp);

      // '%' - Split vertical (creates horizontal layout - side by side)
      if (cp == '%') {
        auto focusedPane = findFocusedPane(_root);
        if (focusedPane) {
          yinfo("Split pane {} vertically (Ctrl+b %%)", focusedPane->id());
          auto r = splitPane(focusedPane->id(), Orientation::Vertical);
          if (!r) {
            ywarn("splitPane failed: {}", r.error().message());
          }
          return Ok(true);
        }
      }

      // '"' - Split horizontal (creates vertical layout - top/bottom)
      if (cp == '"') {
        auto focusedPane = findFocusedPane(_root);
        if (focusedPane) {
          yinfo("Split pane {} horizontally (Ctrl+b \")", focusedPane->id());
          auto r = splitPane(focusedPane->id(), Orientation::Horizontal);
          if (!r) {
            ywarn("splitPane failed: {}", r.error().message());
          }
          return Ok(true);
        }
      }

      // Not handled - let other handlers try
      return Ok(false);
    }

    return Ok(false);
  }

  // ─── Close ────────────────────────────────────────────────────────────

  Result<void> closeTarget(ObjectId targetId) override {
    if (!_root) {
      return Err("No root tile");
    }

    // Close workspace itself
    if (targetId == id()) {
      yinfo("Closing workspace {}", id());
      _root = nullptr;
      return Ok();
    }

    // Close root tile directly
    if (_root->id() == targetId) {
      yinfo("Closing root tile {}", targetId);
      _root = nullptr;
      return Ok();
    }

    // Check if target is a view inside a pane
    auto pane = findPaneContainingView(_root, targetId);
    if (pane) {
      pane->closeView(targetId);
      // If pane has no more views, close the pane too
      if (pane->viewCount() == 0) {
        yinfo("Pane {} has no views left, closing pane", pane->id());
        return closeTileInTree(pane->id());
      }
      return Ok();
    }

    // Target is a tile (Split or Pane) in the tree
    return closeTileInTree(targetId);
  }

  // ─── Split ────────────────────────────────────────────────────────────

  Result<void> splitPane(ObjectId paneId, Orientation orientation) override {
    if (!_root) {
      return Err("No root tile");
    }

    // Find the target pane
    auto target = findTileById(_root, paneId);
    if (!target) {
      return Err("Pane not found for split");
    }

    // Create new split and new sibling pane
    auto splitResult = Split::create(orientation);
    if (!splitResult) {
      return Err<void>("Failed to create Split", splitResult);
    }
    auto split = *splitResult;

    auto newPaneResult = createPane();
    if (!newPaneResult) {
      return Err<void>("Failed to create new Pane", newPaneResult);
    }
    auto newPane = *newPaneResult;

    split->setFirst(target);
    split->setSecond(newPane);

    // Replace target in parent
    if (_root->id() == paneId) {
      // Target was the root
      setRoot(split);
    } else {
      // Find parent split and replace
      auto parentSplit = findParentSplit(_root, paneId);
      if (!parentSplit) {
        return Err("Could not find parent of target pane");
      }
      parentSplit->replaceTile(paneId, split);
      // Re-layout
      if (_bounds_valid()) {
        _root->setBounds({0, 0, _width, _height});
      }
    }

    return Ok();
  }

  Tile::Ptr rootTile() override { return _root; }
  float width() const override { return _width; }
  float height() const override { return _height; }

private:
  void registerForEvents() {
    auto loop = *EventLoop::instance();
    auto self = sharedAs<Workspace>();
    loop->registerListener(Event::Type::Close, self);
    loop->registerListener(Event::Type::SplitPane, self);
    loop->registerListener(Event::Type::CommandKey, self);
  }

  bool _bounds_valid() const {
    return _width > 0 && _height > 0;
  }

  // DFS: find tile by ID
  Tile::Ptr findTileById(Tile::Ptr tile, ObjectId targetId) {
    if (!tile) return nullptr;
    if (tile->id() == targetId) return tile;
    if (auto split = std::dynamic_pointer_cast<Split>(tile)) {
      if (auto found = findTileById(split->first(), targetId)) return found;
      if (auto found = findTileById(split->second(), targetId)) return found;
    }
    return nullptr;
  }

  // DFS: find the Split that is parent of targetId
  std::shared_ptr<Split> findParentSplit(Tile::Ptr tile, ObjectId targetId) {
    auto split = std::dynamic_pointer_cast<Split>(tile);
    if (!split) return nullptr;
    if ((split->first() && split->first()->id() == targetId) ||
        (split->second() && split->second()->id() == targetId)) {
      return split;
    }
    if (auto found = findParentSplit(split->first(), targetId)) return found;
    if (auto found = findParentSplit(split->second(), targetId)) return found;
    return nullptr;
  }

  // DFS: find pane that contains a view with given ID
  std::shared_ptr<Pane> findPaneContainingView(Tile::Ptr tile, ObjectId viewId) {
    if (!tile) return nullptr;
    if (auto pane = std::dynamic_pointer_cast<Pane>(tile)) {
      if (pane->hasView(viewId)) return pane;
      return nullptr;
    }
    if (auto split = std::dynamic_pointer_cast<Split>(tile)) {
      if (auto found = findPaneContainingView(split->first(), viewId)) return found;
      if (auto found = findPaneContainingView(split->second(), viewId)) return found;
    }
    return nullptr;
  }

  // DFS: find the focused pane
  std::shared_ptr<Pane> findFocusedPane(Tile::Ptr tile) {
    if (!tile) return nullptr;
    if (auto pane = std::dynamic_pointer_cast<Pane>(tile)) {
      if (pane->focused()) return pane;
      return nullptr;
    }
    if (auto split = std::dynamic_pointer_cast<Split>(tile)) {
      if (auto found = findFocusedPane(split->first())) return found;
      if (auto found = findFocusedPane(split->second())) return found;
    }
    return nullptr;
  }

  // Remove a tile from the tree, promoting its sibling
  Result<void> closeTileInTree(ObjectId targetId) {
    if (!_root || _root->id() == targetId) {
      _root = nullptr;
      return Ok();
    }

    auto parentSplit = findParentSplit(_root, targetId);
    if (!parentSplit) {
      return Err("Could not find parent of target tile");
    }

    // Determine sibling (the one NOT being closed)
    Tile::Ptr sibling;
    if (parentSplit->first() && parentSplit->first()->id() == targetId) {
      sibling = parentSplit->second();
    } else {
      sibling = parentSplit->first();
    }

    // Replace parentSplit with sibling in grandparent
    if (_root->id() == parentSplit->id()) {
      // Parent split was root
      setRoot(sibling);
    } else {
      auto grandparent = findParentSplit(_root, parentSplit->id());
      if (!grandparent) {
        return Err("Could not find grandparent split");
      }
      grandparent->replaceTile(parentSplit->id(), sibling);
      if (_bounds_valid()) {
        _root->setBounds({0, 0, _width, _height});
      }
    }

    return Ok();
  }

  void updateTileRenderContext() {
    if (_root) {
      TileRenderContext renderCtx;
      renderCtx.queue = _ctx.gpu.queue;
      renderCtx.screenWidth = static_cast<uint32_t>(_width);
      renderCtx.screenHeight = static_cast<uint32_t>(_height);
      renderCtx.frameRenderer = _frameRenderer;
      _root->setRenderContext(renderCtx);
    }
  }

  Tile::Ptr _root;
  float _width = 0;
  float _height = 0;
  YettyContext _ctx;
  std::shared_ptr<WidgetFrameRenderer> _frameRenderer;
};

// ─── TreeLike implementations (Workspace) ──────────────────────────────────

using namespace ymery;

Result<std::vector<std::string>> Workspace::getChildrenNames(const DataPath& path) {
    if (!path.isRoot()) {
        // Navigate into root tile
        auto name = path.asList()[0];
        if (name == "root" && rootTile()) {
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            return rootTile()->getChildrenNames(rest);
        }
        return Err<std::vector<std::string>>("Workspace: child '" + name + "' not found");
    }
    std::vector<std::string> names;
    if (rootTile()) names.push_back("root");
    return Ok(names);
}

Result<Dict> Workspace::getMetadata(const DataPath& path) {
    if (!path.isRoot()) {
        auto name = path.asList()[0];
        if (name == "root" && rootTile()) {
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            return rootTile()->getMetadata(rest);
        }
        return Err<Dict>("Workspace: child '" + name + "' not found");
    }
    return Ok(Dict{
        {"id", id()},
        {"type", std::string("workspace")},
        {"width", width()},
        {"height", height()},
    });
}

Result<std::vector<std::string>> Workspace::getMetadataKeys(const DataPath& path) {
    if (!path.isRoot()) {
        auto name = path.asList()[0];
        if (name == "root" && rootTile()) {
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            return rootTile()->getMetadataKeys(rest);
        }
        return Err<std::vector<std::string>>("Workspace: child '" + name + "' not found");
    }
    return Ok(std::vector<std::string>{"id", "type", "width", "height"});
}

Result<Value> Workspace::get(const DataPath& path) {
    if (path.isRoot()) return Err<Value>("Workspace::get: need a key");
    auto meta = *getMetadata(path.dirname());
    auto key = path.filename();
    auto it = meta.find(key);
    if (it == meta.end()) return Err<Value>("Workspace::get: key '" + key + "' not found");
    return Ok(it->second);
}

Result<void> Workspace::set(const DataPath& /*path*/, const Value& /*value*/) {
    return Err("Workspace: read-only");
}

Result<void> Workspace::addChild(const DataPath& /*path*/, const std::string& /*name*/, const Dict& /*data*/) {
    return Err("Workspace: addChild not supported");
}

// Helper: convert Dict to YAML::Node (same as in tile.cpp)
static YAML::Node workspaceDictToYaml(const Dict& dict) {
    YAML::Node node(YAML::NodeType::Map);
    for (const auto& [k, v] : dict) {
        if (auto s = getAs<std::string>(v)) node[k] = *s;
        else if (auto i = getAs<int>(v)) node[k] = *i;
        else if (auto u = getAs<uint64_t>(v)) node[k] = *u;
        else if (auto d = getAs<double>(v)) node[k] = *d;
        else if (auto b = getAs<bool>(v)) node[k] = *b;
        else if (auto f = getAs<float>(v)) node[k] = *f;
    }
    return node;
}

Result<std::string> Workspace::asTree(const DataPath& path, int depth) {
    if (!path.isRoot()) {
        auto name = path.asList()[0];
        if (name == "root" && rootTile()) {
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            return rootTile()->asTree(rest, depth);
        }
        return Err<std::string>("Workspace: child '" + name + "' not found");
    }

    YAML::Node yaml(YAML::NodeType::Map);
    yaml["metadata"] = workspaceDictToYaml(Dict{
        {"id", id()},
        {"type", std::string("workspace")},
        {"width", width()},
        {"height", height()},
    });

    if (depth != 0 && rootTile()) {
        int childDepth = (depth > 0) ? depth - 1 : -1;
        auto r = rootTile()->asTree(DataPath::root(), childDepth);
        if (r) {
            YAML::Node children(YAML::NodeType::Map);
            children["root"] = YAML::Load(*r);
            yaml["children"] = children;
        }
    }

    return Ok(YAML::Dump(yaml));
}

Result<Workspace::Ptr> Workspace::createImpl(ContextType&, const YettyContext &ctx) {
  auto impl = Ptr(new WorkspaceImpl(ctx));
  if (auto res = static_cast<WorkspaceImpl*>(impl.get())->init(); !res) {
    return Err<Ptr>("Failed to initialize Workspace", res);
  }
  return Ok(std::move(impl));
}

} // namespace yetty
