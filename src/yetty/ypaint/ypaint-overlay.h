#pragma once

#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <yetty/ypaint/painter.h>
#include <webgpu/webgpu.h>
#include <memory>

namespace yetty {

struct YettyContext;
class GpuMemoryManager;

namespace ypaint {

/// YPaintOverlay — renders a Painter's content as fullscreen transparent overlay
/// Similar to ScreenDrawLayer but uses ypaint data format and SDF evaluation.
/// Rendered in the same pass as terminal, with alpha blending on top.
class YPaintOverlay : public base::Object,
                      public base::ObjectFactory<YPaintOverlay> {
public:
    using Ptr = std::shared_ptr<YPaintOverlay>;

    static Result<Ptr> createImpl(const YettyContext& yettyCtx,
                                  Painter::Ptr painter) noexcept;

    virtual ~YPaintOverlay() = default;

    // Update display size (viewport)
    virtual void updateDisplaySize(uint32_t width, uint32_t height) = 0;

    // Render overlay — call after terminal render, in same render pass
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Check if there's content to render
    virtual bool hasContent() const = 0;

    // Get the underlying painter
    virtual Painter::Ptr painter() const = 0;

protected:
    YPaintOverlay() = default;
};

} // namespace ypaint
} // namespace yetty
