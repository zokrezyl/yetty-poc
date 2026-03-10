#pragma once

#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <webgpu/webgpu.h>
#include <memory>
#include <string>

namespace yetty {

class YDrawBuffer;
class YDrawBuilder;
struct YettyContext;

/// ScreenDrawLayer — renders ydraw primitives as a transparent overlay
/// on top of the terminal content at pixel coordinates.
/// Unlike cards (which render inside terminal cells), this draws directly
/// to screen coordinates, allowing for arbitrary positioning.
class ScreenDrawLayer : public base::Object,
                        public base::ObjectFactory<ScreenDrawLayer> {
public:
    using Ptr = std::shared_ptr<ScreenDrawLayer>;

    static Result<Ptr> createImpl(ContextType& ctx,
                                   const YettyContext& yettyCtx) noexcept;

    virtual ~ScreenDrawLayer() = default;

    // Display management
    virtual void updateDisplaySize(uint32_t width, uint32_t height) = 0;

    // Set cell size for grid alignment with terminal
    virtual void setCellSize(float cellWidth, float cellHeight) = 0;

    // Clear all drawings
    virtual void clear() = 0;

    // Update with binary payload (same format as ydraw card)
    virtual Result<void> update(const std::string& args, const std::string& payload) = 0;

    // Render overlay — call after main scene render
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Check if there's anything to render
    virtual bool hasContent() const = 0;

protected:
    ScreenDrawLayer() = default;
};

} // namespace yetty
