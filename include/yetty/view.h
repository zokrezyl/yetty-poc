#pragma once

#include <yetty/result.hpp>
#include <yetty/tile.h>
#include <yetty/base/base.h>
#include <memory>
#include <webgpu/webgpu.h>

namespace yetty {

using namespace yetty::base;

class View : public EventListener {
public:
    using Ptr = std::shared_ptr<View>;

    virtual ~View() = default;

    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Start async operations (e.g., SSH connect in terminal)
    // Called after init() when event loop is about to start.
    virtual Result<void> run() { return Ok(); }

    Rect bounds() const { return _bounds; }
    virtual void setBounds(Rect r) { _bounds = r; onBoundsChanged(); }

protected:
    View() = default;

    virtual void onBoundsChanged() {}

    Rect _bounds;
};

} // namespace yetty
