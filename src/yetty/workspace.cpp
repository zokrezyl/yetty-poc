#include <yetty/workspace.h>
#include <yetty/webgpu-context.h>
#include <yetty/terminal-view.h>
#include <yetty/yetty-font-manager.h>
#include <yetty/bm-font.h>

#if !YETTY_WEB && !defined(__ANDROID__)
#include <uv.h>
#endif

namespace yetty {

class WorkspaceImpl : public Workspace {
public:
    WorkspaceImpl(
        float width, float height,
        WebGPUContext& ctx,
        std::shared_ptr<YettyFontManager> fontManager,
        std::shared_ptr<BmFont> bitmapFont,
        WGPUBindGroupLayout sharedBindGroupLayout,
        WGPUBindGroup sharedBindGroup,
        void* uvLoop
    )
        : _width(width)
        , _height(height)
        , _ctx(&ctx)
        , _fontManager(fontManager)
        , _bitmapFont(bitmapFont)
        , _sharedBindGroupLayout(sharedBindGroupLayout)
        , _sharedBindGroup(sharedBindGroup)
#if !YETTY_WEB && !defined(__ANDROID__)
        , _uvLoop(static_cast<uv_loop_t*>(uvLoop))
#endif
    {}

    Result<void> render(WGPURenderPassEncoder pass, WebGPUContext& ctx) override {
        if (!_root) return Ok();
        return _root->render(pass, ctx);
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
        // Create default TerminalView
        float cellWidth = 10.0f;
        float cellHeight = 20.0f;
        uint32_t cols = static_cast<uint32_t>(_width / cellWidth);
        uint32_t rows = static_cast<uint32_t>(_height / cellHeight);
        if (cols < 1) cols = 80;
        if (rows < 1) rows = 24;

        auto viewResult = TerminalView::create(
            pane->id(), *_ctx, _fontManager, _bitmapFont,
            cols, rows, _sharedBindGroupLayout, _sharedBindGroup, _uvLoop
        );
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

    WebGPUContext* _ctx = nullptr;
    std::shared_ptr<YettyFontManager> _fontManager;
    std::shared_ptr<BmFont> _bitmapFont;
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
#if !YETTY_WEB && !defined(__ANDROID__)
    uv_loop_t* _uvLoop = nullptr;
#endif
};

Result<Workspace::Ptr> Workspace::create(
    float width, float height,
    WebGPUContext& ctx,
    std::shared_ptr<YettyFontManager> fontManager,
    std::shared_ptr<BmFont> bitmapFont,
    WGPUBindGroupLayout sharedBindGroupLayout,
    WGPUBindGroup sharedBindGroup,
    void* uvLoop
) {
    auto ws = std::make_shared<WorkspaceImpl>(
        width, height, ctx, fontManager, bitmapFont,
        sharedBindGroupLayout, sharedBindGroup, uvLoop
    );
    return Ok<Workspace::Ptr>(ws);
}

} // namespace yetty
