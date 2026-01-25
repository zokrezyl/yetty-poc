#pragma once

#include <yetty/result.hpp>
#include <yetty/tile.h>
#include <yetty/gpu-context.h>
#include <memory>

namespace yetty {

class Workspace {
public:
    using Ptr = std::shared_ptr<Workspace>;

    static Result<Ptr> create(const GPUContext& gpu);

    virtual ~Workspace() = default;

    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;
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
