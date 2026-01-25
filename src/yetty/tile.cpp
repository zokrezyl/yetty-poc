#include <yetty/tile.h>
#include <yetty/view.h>

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
    auto loop = EventLoop::instance();
    auto self = sharedAs<Pane>();

    // All panes register for mouse events to detect clicks
    loop->registerListener(Event::Type::MouseDown, self);

    // All panes register for focus events to know when to activate/deactivate
    loop->registerListener(Event::Type::FocusPane, self);

    // First pane created gets initial focus
    static bool firstPane = true;
    if (firstPane) {
        firstPane = false;
        _focused = true;
        loop->registerListener(Event::Type::KeyDown, self);
        loop->registerListener(Event::Type::KeyUp, self);
        loop->registerListener(Event::Type::Char, self);
    }
}

Result<bool> Pane::onEvent(const Event& event) {
    switch (event.type) {
        case Event::Type::FocusPane:
            handleFocusEvent(event);
            return Ok(false); // Don't consume - all panes need to see this

        case Event::Type::MouseDown:
            return Ok(handleMouseEvent(event));

        case Event::Type::KeyDown:
        case Event::Type::KeyUp:
        case Event::Type::Char:
            return Ok(handleKeyEvent(event));

        default:
            return Ok(false);
    }
}

void Pane::handleFocusEvent(const Event& event) {
    auto loop = EventLoop::instance();
    auto self = sharedAs<Pane>();

    bool shouldBeFocused = (event.focus.objectId == id());

    if (shouldBeFocused && !_focused) {
        // Gaining focus - register for keyboard
        _focused = true;
        loop->registerListener(Event::Type::KeyDown, self);
        loop->registerListener(Event::Type::KeyUp, self);
        loop->registerListener(Event::Type::Char, self);
    } else if (!shouldBeFocused && _focused) {
        // Losing focus - deregister from keyboard
        _focused = false;
        loop->deregisterListener(Event::Type::KeyDown, self);
        loop->deregisterListener(Event::Type::KeyUp, self);
        loop->deregisterListener(Event::Type::Char, self);
    }
}

bool Pane::handleMouseEvent(const Event& event) {
    if (!containsPoint(event.mouse.x, event.mouse.y)) {
        return false;
    }

    // Click in our bounds - broadcast focus event
    auto loop = EventLoop::instance();
    loop->broadcast(Event::focusPane(id()));

    // Forward to view with local coordinates
    if (auto* view = activeView()) {
        Event localEvent = event;
        localEvent.mouse.x = event.mouse.x - _bounds.x;
        localEvent.mouse.y = event.mouse.y - _bounds.y;
        view->onEvent(localEvent);
    }

    return true;
}

bool Pane::handleKeyEvent(const Event& event) {
    if (!_focused) return false;

    auto* view = activeView();
    if (!view) return false;

    view->onEvent(event);
    return true;
}

Result<void> Pane::setBounds(Rect r) {
    _bounds = r;
    if (auto* view = activeView()) {
        view->setBounds(r);
    }
    return Ok();
}

Result<void> Pane::render(WGPURenderPassEncoder pass) {
    if (auto* view = activeView()) {
        return view->render(pass);
    }
    return Ok();
}

Result<void> Pane::pushView(std::shared_ptr<View> view) {
    _views.push_back(std::move(view));
    if (!_views.empty() && _bounds.width > 0 && _bounds.height > 0) {
        _views.back()->setBounds(_bounds);
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
