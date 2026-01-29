#pragma once

#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <memory>
#include <vector>
#include <webgpu/webgpu.h>

namespace yetty {

using namespace yetty::base;

class View;

struct Rect {
    float x = 0, y = 0, width = 0, height = 0;
};

// Frame information for rendering tile borders
struct FrameInfo {
    Rect rect;
    float r = 0.4f, g = 0.4f, b = 0.4f, a = 1.0f;  // Default gray
    float thickness = 1.0f;
    bool focused = false;  // For highlight color
};

enum class Orientation {
    Horizontal,
    Vertical
};

// Base class for tiles (Split and Pane)
class Tile {
public:
    using Ptr = std::shared_ptr<Tile>;

    virtual ~Tile() = default;

    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Collect frame rectangles for rendering (recursive)
    virtual void collectFrames(std::vector<FrameInfo>& frames) const = 0;

    Rect bounds() const { return _bounds; }
    virtual Result<void> setBounds(Rect r) { _bounds = r; return Ok(); }

    bool containsPoint(float x, float y) const {
        return x >= _bounds.x && x < _bounds.x + _bounds.width &&
               y >= _bounds.y && y < _bounds.y + _bounds.height;
    }

protected:
    Rect _bounds;
    Tile() = default;
};

// Split - contains two child tiles
class Split : public Tile {
public:
    using Ptr = std::shared_ptr<Split>;

    static Result<Ptr> create(Orientation orientation);

    Result<void> render(WGPURenderPassEncoder pass) override;
    void collectFrames(std::vector<FrameInfo>& frames) const override;
    Result<void> setBounds(Rect r) override;

    Result<void> setFirst(Tile::Ptr tile);
    Result<void> setSecond(Tile::Ptr tile);
    Tile::Ptr first() const { return _first; }
    Tile::Ptr second() const { return _second; }

    Orientation orientation() const { return _orientation; }
    float ratio() const { return _ratio; }
    Result<void> setRatio(float r);

private:
    Split(Orientation orientation);
    Result<void> init();

    Orientation _orientation;
    float _ratio = 0.5f;
    Tile::Ptr _first;
    Tile::Ptr _second;
};

// Pane - leaf tile that holds views, handles events
class Pane : public Tile, public EventListener {
public:
    using Ptr = std::shared_ptr<Pane>;

    static Result<Ptr> create();

    Result<void> render(WGPURenderPassEncoder pass) override;
    void collectFrames(std::vector<FrameInfo>& frames) const override;
    Result<void> setBounds(Rect r) override;

    // EventListener
    Result<bool> onEvent(const Event& event) override;

    // id() comes from Object base class
    bool focused() const { return _focused; }

    Rect innerBounds() const;
    Result<void> pushView(std::shared_ptr<View> view);
    Result<void> popView();
    View* activeView();

private:
    Pane();
    Result<void> init();

    void registerForEvents();
    void handleFocusEvent(const Event& event);

    bool _focused = false;
    std::vector<std::shared_ptr<View>> _views;
};

} // namespace yetty
