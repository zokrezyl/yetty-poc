#include <yetty/tile.h>
#include <yetty/view.h>
#include <ytrace/ytrace.hpp>
#include <algorithm>

namespace yetty {

// Split

Split::Split(Orientation orientation)
    : _orientation(orientation) {}

Result<void> Split::init() {
    return Ok();
}

Result<Split::Ptr> Split::create(Orientation orientation) {
    auto split = Ptr(new Split(orientation));
    if (auto r = split->init(); !r) {
        return Err<Ptr>("Failed to create Split", r);
    }
    return split;
}

Result<void> Split::setBounds(Rect r) {
    _bounds = r;

    // Propagate to children based on orientation and ratio
    if (_orientation == Orientation::Horizontal) {
        // Split horizontally: first on top, second on bottom
        float firstHeight = r.height * _ratio;
        if (_first) {
            _first->setBounds({r.x, r.y, r.width, firstHeight});
        }
        if (_second) {
            _second->setBounds({r.x, r.y + firstHeight, r.width, r.height - firstHeight});
        }
    } else {
        // Split vertically: first on left, second on right
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

Result<void> Split::render(WGPURenderPassEncoder pass) {
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

void Split::collectFrames(std::vector<FrameInfo>& frames) const {
    // Add frame around the split container itself
    FrameInfo splitFrame;
    splitFrame.rect = _bounds;
    splitFrame.r = 1.0f;  // Bright red for debugging
    splitFrame.g = 0.0f;
    splitFrame.b = 0.0f;
    splitFrame.a = 1.0f;
    splitFrame.thickness = 5.0f;
    frames.push_back(splitFrame);

    // Recurse to children
    if (_first) {
        _first->collectFrames(frames);
    }
    if (_second) {
        _second->collectFrames(frames);
    }
}

Result<void> Split::setFirst(Tile::Ptr tile) {
    _first = std::move(tile);
    if (_bounds.width > 0 && _bounds.height > 0) {
        setBounds(_bounds);
    }
    return Ok();
}

Result<void> Split::setSecond(Tile::Ptr tile) {
    _second = std::move(tile);
    if (_bounds.width > 0 && _bounds.height > 0) {
        setBounds(_bounds);
    }
    return Ok();
}

Result<void> Split::setRatio(float r) {
    if (r < 0.0f || r > 1.0f) {
        return Err("Ratio must be between 0 and 1");
    }
    _ratio = r;
    if (_bounds.width > 0 && _bounds.height > 0) {
        setBounds(_bounds);
    }
    return Ok();
}

// Pane

Pane::Pane() {}

Result<void> Pane::init() {
    registerForEvents();
    return Ok();
}

Result<Pane::Ptr> Pane::create() {
    auto pane = Ptr(new Pane());
    if (auto r = pane->init(); !r) {
        return Err<Ptr>("Failed to create Pane", r);
    }
    return pane;
}

void Pane::registerForEvents() {
    // Pane doesn't handle focus/keyboard - GPUScreen does that directly
    // Pane only tracks focus state for visual feedback (frame highlighting)
    auto loop = *EventLoop::instance();
    auto self = sharedAs<Pane>();
    loop->registerListener(Event::Type::SetFocus, self);
}

Result<bool> Pane::onEvent(const Event& event) {
    // Pane only tracks focus state for visual feedback
    if (event.type == Event::Type::SetFocus) {
        handleFocusEvent(event);
    }
    return Ok(false); // Never consume - GPUScreen handles focus/keyboard
}

void Pane::handleFocusEvent(const Event& event) {
    // Just track focus state for visual feedback (frame highlighting)
    // Pane doesn't know GPUScreen's ID - focus tracking is for future use
    (void)event;
}

Rect Pane::innerBounds() const {
    constexpr float border = 5.0f; // max of focused(5) / unfocused(4)
    return {
        _bounds.x + border,
        _bounds.y + border,
        std::max(0.0f, _bounds.width - 2 * border),
        std::max(0.0f, _bounds.height - 2 * border)
    };
}

Result<void> Pane::setBounds(Rect r) {
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

Result<void> Pane::render(WGPURenderPassEncoder pass) {
    if (auto* view = activeView()) {
        return view->render(pass);
    }
    return Ok();
}

void Pane::collectFrames(std::vector<FrameInfo>& frames) const {
    FrameInfo paneFrame;
    paneFrame.rect = _bounds;
    paneFrame.focused = _focused;

    if (_focused) {
        // Focused pane: bright yellow border
        paneFrame.r = 1.0f;
        paneFrame.g = 1.0f;
        paneFrame.b = 0.0f;
        paneFrame.a = 1.0f;
        paneFrame.thickness = 5.0f;
    } else {
        // Unfocused pane: bright green border
        paneFrame.r = 0.0f;
        paneFrame.g = 1.0f;
        paneFrame.b = 0.0f;
        paneFrame.a = 1.0f;
        paneFrame.thickness = 4.0f;
    }

    frames.push_back(paneFrame);
}

Result<void> Pane::pushView(std::shared_ptr<View> view) {
    _views.push_back(std::move(view));
    if (!_views.empty() && _bounds.width > 0 && _bounds.height > 0) {
        _views.back()->setBounds(innerBounds());
    }
    return Ok();
}

Result<void> Pane::popView() {
    if (_views.empty()) {
        return Err("No views to pop");
    }
    _views.pop_back();
    return Ok();
}

View* Pane::activeView() {
    return _views.empty() ? nullptr : _views.back().get();
}

} // namespace yetty
