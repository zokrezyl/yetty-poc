#if YETTY_WEB

#include "web-display.h"
#include <spdlog/spdlog.h>

namespace yetty {

WebDisplay::WebDisplay(uint32_t id, uint32_t cols, uint32_t rows,
                       WebGPUContext::Ptr ctx, FontManager::Ptr fontManager) noexcept
    : id_(id)
    , grid_(cols, rows)
    , ctx_(ctx)
    , fontManager_(fontManager)
    , cols_(cols)
    , rows_(rows)
{
}

Result<WebDisplay::Ptr> WebDisplay::create(uint32_t id, uint32_t cols, uint32_t rows,
                                           WebGPUContext::Ptr ctx,
                                           FontManager::Ptr fontManager) noexcept
{
    auto display = Ptr(new WebDisplay(id, cols, rows, ctx, fontManager));
    if (auto res = display->init(); !res) {
        return Err<Ptr>("Failed to initialize WebDisplay", res);
    }
    return Ok(display);
}

Result<void> WebDisplay::init() noexcept
{
    // Get default font from FontManager
    font_ = fontManager_->getDefaultFont();
    if (!font_) {
        return Err<void>("No default font available in FontManager");
    }

    // Create GridRenderer
    auto rendererResult = GridRenderer::create(ctx_, fontManager_);
    if (!rendererResult) {
        return Err<void>("Failed to create GridRenderer", rendererResult);
    }
    renderer_ = *rendererResult;

    // Set cell size based on font metrics (lineHeight and typical monospace ratio)
    float lineHeight = font_->getLineHeight();
    float cellWidth = lineHeight * 0.5f;   // Typical monospace ratio
    float cellHeight = lineHeight;
    renderer_->setCellSize(cellWidth, cellHeight);

    spdlog::info("WebDisplay initialized: {}x{} grid, cell size: {}x{}",
                 cols_, rows_, cellWidth, cellHeight);

    return Ok();
}

Result<void> WebDisplay::render(WebGPUContext& ctx)
{
    if (!renderer_) {
        return Err<void>("No renderer available");
    }

    // Render the grid
    renderer_->render(grid_, cursorCol_, cursorRow_, cursorVisible_);

    return Ok();
}

void WebDisplay::setCursor(int col, int row, bool visible)
{
    cursorCol_ = col;
    cursorRow_ = row;
    cursorVisible_ = visible;
}

void WebDisplay::setCellSize(float width, float height)
{
    if (renderer_) {
        renderer_->setCellSize(width, height);
    }
}

} // namespace yetty

#endif // YETTY_WEB
