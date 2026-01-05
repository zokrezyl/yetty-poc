#pragma once

#if YETTY_WEB

#include <yetty/renderable.h>
#include <yetty/font.h>
#include <yetty/result.hpp>
#include "grid.h"
#include "grid-renderer.h"

#include <memory>
#include <string>

namespace yetty {

//=============================================================================
// WebDisplay - Simple Renderable for web builds that displays a Grid
//
// This is a simplified version of Terminal that doesn't need PTY/libuv.
// It can be used to display static content or content fed from JavaScript.
//=============================================================================

class WebDisplay : public Renderable {
public:
    using Ptr = std::shared_ptr<WebDisplay>;

    // Factory - creates display with given grid size
    static Result<Ptr> create(uint32_t id, uint32_t cols, uint32_t rows,
                              WebGPUContext::Ptr ctx, FontManager::Ptr fontManager) noexcept;

    ~WebDisplay() override = default;

    // Non-copyable
    WebDisplay(const WebDisplay&) = delete;
    WebDisplay& operator=(const WebDisplay&) = delete;

    //=========================================================================
    // Renderable interface
    //=========================================================================
    uint32_t id() const override { return id_; }
    uint32_t zOrder() const override { return zOrder_; }
    const std::string& name() const override { return name_; }

    void start() override { running_ = true; }
    void stop() override { running_ = false; }
    bool isRunning() const override { return running_; }

    Result<void> render(WebGPUContext& ctx) override;

    //=========================================================================
    // WebDisplay-specific interface
    //=========================================================================

    // Grid access
    Grid& grid() { return grid_; }
    const Grid& grid() const { return grid_; }

    // Font access
    Font* font() const { return font_; }

    // Cursor control
    void setCursor(int col, int row, bool visible = true);
    int getCursorCol() const { return cursorCol_; }
    int getCursorRow() const { return cursorRow_; }
    bool isCursorVisible() const { return cursorVisible_; }

    // Cell size for scaling
    void setCellSize(float width, float height);

private:
    WebDisplay(uint32_t id, uint32_t cols, uint32_t rows,
               WebGPUContext::Ptr ctx, FontManager::Ptr fontManager) noexcept;
    Result<void> init() noexcept;

    // Identity
    uint32_t id_;
    uint32_t zOrder_ = 0;
    std::string name_ = "WebDisplay";
    bool running_ = false;

    // Display state
    Grid grid_;
    Font* font_ = nullptr;
    GridRenderer::Ptr renderer_;
    WebGPUContext::Ptr ctx_;
    FontManager::Ptr fontManager_;

    // Cursor
    int cursorCol_ = 0;
    int cursorRow_ = 0;
    bool cursorVisible_ = true;

    uint32_t cols_;
    uint32_t rows_;
};

} // namespace yetty

#endif // YETTY_WEB
