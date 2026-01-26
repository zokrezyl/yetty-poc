#pragma once

#include <yetty/result.hpp>
#include <yetty/tile.h>
#include <yetty/yetty-font-manager.h>
#include <yetty/bm-font.h>
#include "terminal.h"
#include <memory>
#include <webgpu/webgpu.h>

namespace yetty {

class WebGPUContext;
class Cell;

// GPU renderer for terminal cells
// Doesn't care if cells come from local or shared memory
class TerminalRenderer {
public:
    using Ptr = std::shared_ptr<TerminalRenderer>;

    static Result<Ptr> create(
        WebGPUContext& ctx,
        YettyFontManager::Ptr fontManager,
        BmFont::Ptr bitmapFont,
        uint32_t cols,
        uint32_t rows,
        WGPUBindGroupLayout sharedBindGroupLayout,
        WGPUBindGroup sharedBindGroup
    ) noexcept;

    ~TerminalRenderer();

    Result<void> render(
        WGPURenderPassEncoder pass,
        WebGPUContext& ctx,
        const Cell* cells,
        uint32_t cols,
        uint32_t rows,
        CursorState cursor,
        Rect bounds
    );

    Result<void> resize(uint32_t cols, uint32_t rows);

    float cellWidth() const;
    float cellHeight() const;

private:
    TerminalRenderer();

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace yetty
