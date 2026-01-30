#include <yetty/tile.h>
#include <yetty/view.h>
#include <yetty/widget-frame-renderer.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>

namespace yetty {

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

    Result<void> render(WGPURenderPassEncoder pass) override {
        // Render frame around the split
        renderFrame(pass, 1.0f, 0.0f, 0.0f, 1.0f, 5.0f);

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

    Result<void> render(WGPURenderPassEncoder pass) override {
        // Render frame border
        if (_focused) {
            renderFrame(pass, 1.0f, 1.0f, 0.0f, 1.0f, 5.0f);
        } else {
            renderFrame(pass, 0.0f, 1.0f, 0.0f, 1.0f, 4.0f);
        }

        if (auto* view = activeView()) {
            return view->render(pass);
        }
        return Ok();
    }

    Result<void> setBounds(Rect r) override {
        _bounds = r;
        if (auto* view = activeView()) {
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
        }
        return Ok(false);
    }

    bool focused() const override { return _focused; }

    Rect innerBounds() const override {
        constexpr float border = 5.0f;
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

    View* activeView() override {
        return _views.empty() ? nullptr : _views.back().get();
    }

private:
    void registerForEvents() {
        auto loop = *EventLoop::instance();
        auto self = sharedAs<Pane>();
        loop->registerListener(Event::Type::SetFocus, self);
    }

    void handleFocusEvent(const Event& event) {
        (void)event;
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
