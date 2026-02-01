#include <yetty/tile.h>
#include <yetty/view.h>
#include <yetty/widget-frame-renderer.h>
#include <ytrace/ytrace.hpp>
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <sstream>

namespace yetty {

using namespace ymery;

// ─── TreeLike base implementations (Tile) ────────────────────────────────────

Result<std::vector<std::string>> Tile::getChildrenNames(const DataPath& /*path*/) {
    return Ok(std::vector<std::string>{});
}

Result<Dict> Tile::getMetadata(const DataPath& path) {
    if (!path.isRoot()) {
        return Err<Dict>("Tile::getMetadata: no children to navigate");
    }
    return Ok(tileMetadata());
}

Result<std::vector<std::string>> Tile::getMetadataKeys(const DataPath& path) {
    if (!path.isRoot()) {
        return Err<std::vector<std::string>>("Tile::getMetadataKeys: no children to navigate");
    }
    auto meta = tileMetadata();
    std::vector<std::string> keys;
    for (const auto& [k, _] : meta) {
        keys.push_back(k);
    }
    return Ok(keys);
}

Result<Value> Tile::get(const DataPath& path) {
    if (path.isRoot()) {
        return Err<Value>("Tile::get: need a key");
    }
    auto meta = tileMetadata();
    auto key = path.filename();
    auto it = meta.find(key);
    if (it == meta.end()) {
        return Err<Value>("Tile::get: key '" + key + "' not found");
    }
    return Ok(it->second);
}

Result<void> Tile::set(const DataPath& /*path*/, const Value& /*value*/) {
    return Err("Tile: read-only");
}

Result<void> Tile::addChild(const DataPath& /*path*/, const std::string& /*name*/, const Dict& /*data*/) {
    return Err("Tile: addChild not supported");
}

// Helper: convert Dict metadata to YAML::Node
static YAML::Node dictToYaml(const Dict& dict) {
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

Result<std::string> Tile::asTree(const DataPath& path, int /*depth*/) {
    if (!path.isRoot()) {
        return Err<std::string>("Tile::asTree: leaf node, no children");
    }
    YAML::Node yaml(YAML::NodeType::Map);
    yaml["metadata"] = dictToYaml(tileMetadata());
    return Ok(YAML::Dump(yaml));
}

Dict Tile::tileMetadata() const {
    return Dict{
        {"id", id()},
        {"type", std::string("tile")},
    };
}

// Tile base - frame rendering helper
void Tile::renderFrame(WGPURenderPassEncoder pass,
                       float r, float g, float b, float a,
                       float thickness) {
    if (!_renderCtx.frameRenderer || !_renderCtx.queue) {
        return;
    }
    _renderCtx.frameRenderer->renderFrame(
        pass, _renderCtx.queue,
        _renderCtx.screenWidth, _renderCtx.screenHeight,
        _bounds.x, _bounds.y, _bounds.width, _bounds.height,
        r, g, b, a, thickness);
}

// SplitImpl

class SplitImpl : public Split {
public:
    explicit SplitImpl(Orientation orientation)
        : _orientation(orientation) {}

    Result<void> init() { return Ok(); }

    Result<void> onShutdown() override {
        Result<void> result = Ok();
        if (_first) {
            if (auto res = _first->shutdown(); !res) {
                result = Err<void>("Failed to shutdown first tile", res);
            }
        }
        if (_second) {
            if (auto res = _second->shutdown(); !res) {
                result = Err<void>("Failed to shutdown second tile", res);
            }
        }
        return result;
    }

    Result<void> render(WGPURenderPassEncoder pass) override {
        // Render frame around the split
        renderFrame(pass, 1.0f, 0.0f, 0.0f, 1.0f, 2.0f);

        if (_first) {
            if (auto r = _first->render(pass); !r) {
                return Err<void>("Failed to render first tile", r);
            }
        }
        if (_second) {
            if (auto r = _second->render(pass); !r) {
                return Err<void>("Failed to render second tile", r);
            }
        }
        return Ok();
    }

    Result<void> setBounds(Rect r) override {
        _bounds = r;

        if (_orientation == Orientation::Horizontal) {
            float firstHeight = r.height * _ratio;
            if (_first) {
                _first->setBounds({r.x, r.y, r.width, firstHeight});
            }
            if (_second) {
                _second->setBounds({r.x, r.y + firstHeight, r.width, r.height - firstHeight});
            }
        } else {
            float firstWidth = r.width * _ratio;
            if (_first) {
                _first->setBounds({r.x, r.y, firstWidth, r.height});
            }
            if (_second) {
                _second->setBounds({r.x + firstWidth, r.y, r.width - firstWidth, r.height});
            }
        }
        return Ok();
    }

    void setRenderContext(const TileRenderContext &ctx) override {
        _renderCtx = ctx;
        if (_first) _first->setRenderContext(ctx);
        if (_second) _second->setRenderContext(ctx);
    }

    Result<void> setFirst(Tile::Ptr tile) override {
        _first = std::move(tile);
        if (_first) _first->setRenderContext(_renderCtx);
        if (_bounds.width > 0 && _bounds.height > 0) {
            setBounds(_bounds);
        }
        return Ok();
    }

    Result<void> setSecond(Tile::Ptr tile) override {
        _second = std::move(tile);
        if (_second) _second->setRenderContext(_renderCtx);
        if (_bounds.width > 0 && _bounds.height > 0) {
            setBounds(_bounds);
        }
        return Ok();
    }

    Tile::Ptr first() const override { return _first; }
    Tile::Ptr second() const override { return _second; }
    Orientation orientation() const override { return _orientation; }
    float ratio() const override { return _ratio; }

    Result<void> setRatio(float r) override {
        if (r < 0.0f || r > 1.0f) {
            return Err("Ratio must be between 0 and 1");
        }
        _ratio = r;
        if (_bounds.width > 0 && _bounds.height > 0) {
            setBounds(_bounds);
        }
        return Ok();
    }

    // ─── TreeLike overrides ─────────────────────────────────────────────

    Result<std::vector<std::string>> getChildrenNames(const DataPath& path) override {
        if (!path.isRoot()) {
            // Navigate into child
            auto name = path.asList()[0];
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            if (name == "first" && _first) return _first->getChildrenNames(rest);
            if (name == "second" && _second) return _second->getChildrenNames(rest);
            return Err<std::vector<std::string>>("Split: child '" + name + "' not found");
        }
        std::vector<std::string> names;
        if (_first) names.push_back("first");
        if (_second) names.push_back("second");
        return Ok(names);
    }

    Result<Dict> getMetadata(const DataPath& path) override {
        if (!path.isRoot()) {
            auto name = path.asList()[0];
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            if (name == "first" && _first) return _first->getMetadata(rest);
            if (name == "second" && _second) return _second->getMetadata(rest);
            return Err<Dict>("Split: child '" + name + "' not found");
        }
        return Ok(tileMetadata());
    }

    Dict tileMetadata() const override {
        return Dict{
            {"id", id()},
            {"type", std::string("split")},
            {"orientation", std::string(_orientation == Orientation::Horizontal ? "horizontal" : "vertical")},
            {"ratio", _ratio},
        };
    }

    Result<std::string> asTree(const DataPath& path, int depth) override {
        if (!path.isRoot()) {
            auto name = path.asList()[0];
            DataPath rest(std::vector<std::string>(path.asList().begin() + 1, path.asList().end()));
            if (name == "first" && _first) return _first->asTree(rest, depth);
            if (name == "second" && _second) return _second->asTree(rest, depth);
            return Err<std::string>("Split: child '" + name + "' not found");
        }

        YAML::Node yaml(YAML::NodeType::Map);
        yaml["metadata"] = dictToYaml(tileMetadata());

        if (depth != 0) {
            int childDepth = (depth > 0) ? depth - 1 : -1;
            YAML::Node children(YAML::NodeType::Map);
            if (_first) {
                auto r = _first->asTree(DataPath::root(), childDepth);
                if (r) children["first"] = YAML::Load(*r);
            }
            if (_second) {
                auto r = _second->asTree(DataPath::root(), childDepth);
                if (r) children["second"] = YAML::Load(*r);
            }
            yaml["children"] = children;
        }

        return Ok(YAML::Dump(yaml));
    }

    // ─── Tree manipulation ──────────────────────────────────────────────

    Result<void> replaceTile(ObjectId targetId, Tile::Ptr replacement) override {
        if (_first && _first->id() == targetId) {
            _first = std::move(replacement);
            if (_first) _first->setRenderContext(_renderCtx);
            if (_bounds.width > 0 && _bounds.height > 0) {
                setBounds(_bounds);
            }
            return Ok();
        }
        if (_second && _second->id() == targetId) {
            _second = std::move(replacement);
            if (_second) _second->setRenderContext(_renderCtx);
            if (_bounds.width > 0 && _bounds.height > 0) {
                setBounds(_bounds);
            }
            return Ok();
        }
        // Recurse into Split children
        if (auto split = std::dynamic_pointer_cast<Split>(_first)) {
            auto r = split->replaceTile(targetId, std::move(replacement));
            if (r) return r;
        }
        if (auto split = std::dynamic_pointer_cast<Split>(_second)) {
            return split->replaceTile(targetId, std::move(replacement));
        }
        return Err("Tile not found for replacement");
    }

private:
    Orientation _orientation;
    float _ratio = 0.5f;
    Tile::Ptr _first;
    Tile::Ptr _second;
};

Result<Split::Ptr> Split::createImpl(Orientation orientation) {
    auto impl = Ptr(new SplitImpl(orientation));
    if (auto r = static_cast<SplitImpl*>(impl.get())->init(); !r) {
        return Err<Ptr>("Failed to create Split", r);
    }
    return Ok(std::move(impl));
}

// PaneImpl

class PaneImpl : public Pane {
public:
    PaneImpl() = default;

    Result<void> init() {
        registerForEvents();
        return Ok();
    }

    Result<void> onShutdown() override {
        Result<void> result = Ok();

        // Deregister from EventLoop
        auto loop = *EventLoop::instance();
        loop->deregisterListener(sharedAs<EventListener>());

        // Shutdown all views
        for (auto& view : _views) {
            if (auto res = view->shutdown(); !res) {
                result = Err<void>("Failed to shutdown view", res);
            }
        }
        return result;
    }

    Result<void> render(WGPURenderPassEncoder pass) override {
        // Render frame border
        if (_focused) {
            renderFrame(pass, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f);
        } else {
            renderFrame(pass, 0.0f, 1.0f, 0.0f, 1.0f, 2.0f);
        }

        if (auto view = activeView()) {
            return view->render(pass);
        }
        return Ok();
    }

    Result<void> setBounds(Rect r) override {
        _bounds = r;
        if (auto view = activeView()) {
            Rect inner = innerBounds();
            yinfo("Pane::setBounds outer=({},{} {}x{}) inner=({},{} {}x{})",
                  r.x, r.y, r.width, r.height,
                  inner.x, inner.y, inner.width, inner.height);
            if (inner.width > 0 && inner.height > 0) {
                view->setBounds(inner);
            }
        }
        return Ok();
    }

    Result<bool> onEvent(const Event& event) override {
        if (event.type == Event::Type::SetFocus) {
            handleFocusEvent(event);
        } else if (event.type == Event::Type::Close) {
            if (event.closeEv.objectId == id()) {
                yinfo("Pane {} received Close event", id());
                _views.clear();
                return Ok(true);
            }
            // Check if close targets one of our views
            for (auto& v : _views) {
                if (v->id() == event.closeEv.objectId) {
                    yinfo("Pane {} closing view {}", id(), v->id());
                    closeView(event.closeEv.objectId);
                    return Ok(true);
                }
            }
        }
        return Ok(false);
    }

    bool focused() const override { return _focused; }

    Rect innerBounds() const override {
        constexpr float border = 2.0f;
        return {
            _bounds.x + border,
            _bounds.y + border,
            std::max(0.0f, _bounds.width - 2 * border),
            std::max(0.0f, _bounds.height - 2 * border)
        };
    }

    Result<void> pushView(std::shared_ptr<View> view) override {
        _views.push_back(std::move(view));
        if (!_views.empty() && _bounds.width > 0 && _bounds.height > 0) {
            _views.back()->setBounds(innerBounds());
        }
        return Ok();
    }

    Result<void> popView() override {
        if (_views.empty()) {
            return Err("No views to pop");
        }
        _views.pop_back();
        return Ok();
    }

    std::shared_ptr<View> activeView() override {
        return _views.empty() ? nullptr : _views.back();
    }

    Result<void> closeView(ObjectId viewId) override {
        auto it = std::find_if(_views.begin(), _views.end(),
            [viewId](const auto& v) { return v->id() == viewId; });
        if (it == _views.end()) {
            return Err("View not found in pane");
        }
        _views.erase(it);
        return Ok();
    }

    size_t viewCount() const override { return _views.size(); }

    // ─── TreeLike overrides ─────────────────────────────────────────────

    Result<std::vector<std::string>> getChildrenNames(const DataPath& path) override {
        if (!path.isRoot()) {
            return Err<std::vector<std::string>>("Pane: cannot navigate into views");
        }
        std::vector<std::string> names;
        for (const auto& v : _views) {
            names.push_back("view-" + std::to_string(v->id()));
        }
        return Ok(names);
    }

    Result<Dict> getMetadata(const DataPath& path) override {
        if (!path.isRoot()) {
            // Navigate into a view child
            auto name = path.asList()[0];
            for (const auto& v : _views) {
                if ("view-" + std::to_string(v->id()) == name) {
                    return Ok(Dict{
                        {"id", v->id()},
                        {"type", std::string("view")},
                    });
                }
            }
            return Err<Dict>("Pane: child '" + name + "' not found");
        }
        return Ok(tileMetadata());
    }

    Dict tileMetadata() const override {
        return Dict{
            {"id", id()},
            {"type", std::string("pane")},
            {"focused", _focused},
            {"viewCount", static_cast<int>(_views.size())},
        };
    }

    Result<std::string> asTree(const DataPath& path, int depth) override {
        if (!path.isRoot()) {
            return Err<std::string>("Pane::asTree: sub-path not supported");
        }

        YAML::Node yaml(YAML::NodeType::Map);
        yaml["metadata"] = dictToYaml(tileMetadata());

        if (depth != 0 && !_views.empty()) {
            YAML::Node children(YAML::NodeType::Map);
            for (const auto& v : _views) {
                YAML::Node viewNode(YAML::NodeType::Map);
                YAML::Node viewMeta(YAML::NodeType::Map);
                viewMeta["id"] = v->id();
                viewMeta["type"] = "view";
                viewNode["metadata"] = viewMeta;
                children["view-" + std::to_string(v->id())] = viewNode;
            }
            yaml["children"] = children;
        }

        return Ok(YAML::Dump(yaml));
    }

    bool hasView(ObjectId viewId) const override {
        return std::any_of(_views.begin(), _views.end(),
            [viewId](const auto& v) { return v->id() == viewId; });
    }

private:
    void registerForEvents() {
        auto loop = *EventLoop::instance();
        auto self = sharedAs<Pane>();
        loop->registerListener(Event::Type::SetFocus, self);
        loop->registerListener(Event::Type::Close, self);
    }

    void handleFocusEvent(const Event& event) {
        auto view = activeView();
        _focused = view && event.setFocus.objectId == view->id();
    }

    bool _focused = false;
    std::vector<std::shared_ptr<View>> _views;
};

Result<Pane::Ptr> Pane::createImpl() {
    auto impl = Ptr(new PaneImpl());
    if (auto r = static_cast<PaneImpl*>(impl.get())->init(); !r) {
        return Err<Ptr>("Failed to create Pane", r);
    }
    return Ok(std::move(impl));
}

} // namespace yetty
