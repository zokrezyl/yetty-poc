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

    // Set pixel origin offset (for inline positioning at cursor)
    virtual void setOriginOffset(float x, float y) = 0;

    // Scroll overlay by terminal lines (efficient - only rebuilds grid, not AABBs)
    // num_lines: number of terminal lines scrolled (positive = content moves up)
    virtual void scroll(int32_t num_lines) = 0;

    // Get content bounding box in pixels (after update)
    virtual void getContentBounds(float& minX, float& minY, float& maxX, float& maxY) const = 0;

    // Get content height in terminal cell rows (for scroll calculation)
    virtual uint32_t getContentHeightCells() const = 0;

    // Render overlay — call after main scene render (legacy, now integrated into gpu-screen)
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Check if there's anything to render
    virtual bool hasContent() const = 0;

    // Buffer accessors for integrated rendering in gpu-screen shader
    virtual WGPUBuffer getGridBuffer() const = 0;
    virtual WGPUBuffer getGlyphBuffer() const = 0;
    virtual WGPUBuffer getPrimBuffer() const = 0;
    virtual WGPUBuffer getUniformBuffer() const = 0;
    virtual uint32_t getGridBufferSize() const = 0;
    virtual uint32_t getGlyphBufferSize() const = 0;
    virtual uint32_t getPrimBufferSize() const = 0;

    // Prepare buffers for rendering (call before bind group creation)
    virtual void prepareForRender() = 0;

protected:
    ScreenDrawLayer() = default;
};

} // namespace yetty
