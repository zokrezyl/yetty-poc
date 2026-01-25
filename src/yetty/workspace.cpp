#include <yetty/workspace.h>
#include <yetty/terminal-view.h>

namespace yetty {

class WorkspaceImpl : public Workspace {
public:
    explicit WorkspaceImpl(const GPUContext& gpuContext)
        : _gpuContext(gpuContext)
    {}

    Result<void> render(WGPURenderPassEncoder pass) override {
        if (!_root) return Ok();
        return _root->render(pass);
    }

    Result<void> resize(float width, float height) override {
        _width = width;
        _height = height;
        if (_root) {
            _root->setBounds({0, 0, _width, _height});
        }
        return Ok();
    }

    Result<void> setRoot(Tile::Ptr tile) override {
        _root = std::move(tile);
        if (_root && _width > 0 && _height > 0) {
            _root->setBounds({0, 0, _width, _height});
        }
        return Ok();
    }

    Result<Pane::Ptr> createPane() override {
        auto paneResult = Pane::create();
        if (!paneResult) {
            return Err<Pane::Ptr>("Failed to create pane", paneResult);
        }
        auto pane = *paneResult;

#if !YETTY_WEB && !defined(__ANDROID__)
        auto viewResult = TerminalView::create(_gpuContext);
        if (!viewResult) {
            return Err<Pane::Ptr>("Failed to create TerminalView", viewResult);
        }
        pane->pushView(*viewResult);
#endif

        return Ok(pane);
    }

    Tile::Ptr rootTile() override { return _root; }
    float width() const override { return _width; }
    float height() const override { return _height; }

private:
    Tile::Ptr _root;
    float _width = 0;
    float _height = 0;
    GPUContext _gpuContext;
};

Result<Workspace::Ptr> Workspace::create(const GPUContext& gpuContext) {
    auto ws = std::make_shared<WorkspaceImpl>(gpuContext);
    return Ok<Workspace::Ptr>(ws);
}

} // namespace yetty
