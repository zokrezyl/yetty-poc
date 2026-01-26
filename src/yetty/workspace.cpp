#include <yetty/workspace.h>
#include <yetty/terminal-view.h>
#include <yetty/widget-frame-renderer.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

class WorkspaceImpl : public Workspace {
public:
    explicit WorkspaceImpl(const YettyContext& ctx)
        : _ctx(ctx)
    {}

    Result<void> init() {
        // Create frame renderer
        auto frameResult = WidgetFrameRenderer::create(
            _ctx.gpu.device, _ctx.gpu.surfaceFormat);
        if (!frameResult) {
            ywarn("Failed to create WidgetFrameRenderer for workspace: {}",
                  frameResult.error().message());
            // Non-fatal - frames just won't render
        } else {
            _frameRenderer = std::move(*frameResult);
            yinfo("Workspace WidgetFrameRenderer initialized");
        }
        return Ok();
    }

    Result<void> render(WGPURenderPassEncoder pass) override {
        // First: render tile content
        if (_root) {
            if (auto r = _root->render(pass); !r) {
                return r;
            }
        }

        // Then: render tile frames on top (overlay)
        renderFrames(pass);

        return Ok();
    }

private:
    void renderFrames(WGPURenderPassEncoder pass) {
        if (!_frameRenderer || !_root) {
            return;
        }

        // Reset viewport to full screen (tiles may have changed it)
        wgpuRenderPassEncoderSetViewport(pass, 0, 0, _width, _height, 0.0f, 1.0f);
        wgpuRenderPassEncoderSetScissorRect(pass, 0, 0,
            static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));

        _frameRenderer->resetDrawIndex();

        // Collect all frames from tile tree
        std::vector<FrameInfo> frames;
        _root->collectFrames(frames);

        // Render each frame
        uint32_t screenW = static_cast<uint32_t>(_width);
        uint32_t screenH = static_cast<uint32_t>(_height);

        for (const auto& frame : frames) {
            _frameRenderer->renderFrame(
                pass,
                _ctx.gpu.queue,
                screenW, screenH,
                frame.rect.x, frame.rect.y,
                frame.rect.width, frame.rect.height,
                frame.r, frame.g, frame.b, frame.a,
                frame.thickness
            );
        }
    }

public:

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
        auto viewResult = TerminalView::create(_ctx);
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
    YettyContext _ctx;
    std::unique_ptr<WidgetFrameRenderer> _frameRenderer;
};

Result<Workspace::Ptr> Workspace::create(const YettyContext& ctx) {
    auto ws = std::make_shared<WorkspaceImpl>(ctx);
    if (auto res = ws->init(); !res) {
        return Err<Workspace::Ptr>("Failed to initialize Workspace", res);
    }
    return Ok<Workspace::Ptr>(ws);
}

} // namespace yetty
