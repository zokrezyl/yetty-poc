#include <yetty/terminal-view.h>
#include <yetty/widget-frame-renderer.h>
#include <yetty/workspace.h>
#include <ytrace/ytrace.hpp>

namespace yetty {

class WorkspaceImpl : public Workspace {
public:
  explicit WorkspaceImpl(const YettyContext &ctx) : _ctx(ctx) {}

  Result<void> init() {
    // Create frame renderer for tiles
    auto frameResult =
        WidgetFrameRenderer::create(_ctx.gpu.device, _ctx.gpu.surfaceFormat);
    if (!frameResult) {
      ywarn("Failed to create frame renderer for workspace: {}",
            frameResult.error().message());
    } else {
      _frameRenderer = std::shared_ptr<WidgetFrameRenderer>(
          frameResult->release());
      yinfo("Workspace frame renderer initialized");
    }
    return Ok();
  }

  Result<void> render(WGPURenderPassEncoder pass) override {
    if (_root) {
      // Update render context before rendering (screen size may have changed)
      updateTileRenderContext();

      if (_frameRenderer) {
        _frameRenderer->resetDrawIndex();
      }

      if (auto r = _root->render(pass); !r) {
        return r;
      }
    }
    return Ok();
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
    if (_root) {
      updateTileRenderContext();
      if (_width > 0 && _height > 0) {
        _root->setBounds({0, 0, _width, _height});
      }
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
  void updateTileRenderContext() {
    if (_root) {
      TileRenderContext renderCtx;
      renderCtx.queue = _ctx.gpu.queue;
      renderCtx.screenWidth = static_cast<uint32_t>(_width);
      renderCtx.screenHeight = static_cast<uint32_t>(_height);
      renderCtx.frameRenderer = _frameRenderer;
      _root->setRenderContext(renderCtx);
    }
  }

  Tile::Ptr _root;
  float _width = 0;
  float _height = 0;
  YettyContext _ctx;
  std::shared_ptr<WidgetFrameRenderer> _frameRenderer;
};

Result<Workspace::Ptr> Workspace::createImpl(ContextType&, const YettyContext &ctx) {
  auto impl = Ptr(new WorkspaceImpl(ctx));
  if (auto res = static_cast<WorkspaceImpl*>(impl.get())->init(); !res) {
    return Err<Ptr>("Failed to initialize Workspace", res);
  }
  return Ok(std::move(impl));
}

} // namespace yetty
