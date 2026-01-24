#pragma once

#include <yetty/result.hpp>
#include <yetty/tile.h>
#include <memory>
#include <webgpu/webgpu.h>

namespace yetty {

class WebGPUContext;
class YettyFontManager;
class BmFont;

class Workspace {
public:
    using Ptr = std::shared_ptr<Workspace>;

    static Result<Ptr> create(
        float width, float height,
        WebGPUContext& ctx,
        std::shared_ptr<YettyFontManager> fontManager,
        std::shared_ptr<BmFont> bitmapFont,
        WGPUBindGroupLayout sharedBindGroupLayout,
        WGPUBindGroup sharedBindGroup,
        void* uvLoop = nullptr
    );

    virtual ~Workspace() = default;

    virtual Result<void> render(WGPURenderPassEncoder pass, WebGPUContext& ctx) = 0;
    virtual Result<void> resize(float width, float height) = 0;
    virtual Result<void> setRoot(Tile::Ptr tile) = 0;
    virtual Result<Pane::Ptr> createPane() = 0;

    virtual Tile::Ptr rootTile() = 0;
    virtual float width() const = 0;
    virtual float height() const = 0;

protected:
    Workspace() = default;
};

} // namespace yetty
