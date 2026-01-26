#pragma once

#include <yetty/result.hpp>
#include <yetty/yetty-context.h>
#include <yetty/base/event-listener.h>
#include <memory>

namespace yetty {

// Terminal - owns PTY + GPUScreen
// Handles shell communication and rendering
class Terminal : public base::EventListener {
public:
    using Ptr = std::shared_ptr<Terminal>;

    static Result<Ptr> create(const YettyContext& ctx) noexcept;

    virtual ~Terminal() = default;

    virtual void stop() = 0;
    virtual bool isRunning() const = 0;

    // Rendering
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Resize (updates both PTY and GPUScreen)
    virtual void resize(uint32_t cols, uint32_t rows) = 0;

    // Viewport (pixel coordinates within window)
    virtual void setViewport(float x, float y, float width, float height) = 0;

    // Cell size (from font metrics * zoom)
    virtual float getCellWidth() const = 0;
    virtual float getCellHeight() const = 0;

protected:
    Terminal() = default;
};

} // namespace yetty
