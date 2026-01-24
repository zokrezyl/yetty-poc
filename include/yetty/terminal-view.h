#pragma once

#include <yetty/view.h>
#include <yetty/yetty-font-manager.h>
#include <yetty/bm-font.h>
#include <memory>
#include <webgpu/webgpu.h>

#if !YETTY_WEB && !defined(__ANDROID__)
struct uv_loop_s;
typedef struct uv_loop_s uv_loop_t;
#endif

namespace yetty {

class WebGPUContext;

class TerminalView : public View {
public:
    using Ptr = std::shared_ptr<TerminalView>;

    static Result<Ptr> create(
        base::ObjectId paneId,
        WebGPUContext& ctx,
        YettyFontManager::Ptr fontManager,
        BmFont::Ptr bitmapFont,
        uint32_t cols,
        uint32_t rows,
        WGPUBindGroupLayout sharedBindGroupLayout,
        WGPUBindGroup sharedBindGroup
#if !YETTY_WEB && !defined(__ANDROID__)
        , uv_loop_t* loop
#endif
    ) noexcept;

    ~TerminalView() override;

    Result<void> render(WGPURenderPassEncoder pass, WebGPUContext& ctx) override;
    Result<bool> onEvent(const Event& event) override;
    void setBounds(Rect r) override;

protected:
    void onBoundsChanged() override;

private:
    TerminalView();

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace yetty
