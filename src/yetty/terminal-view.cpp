#include <yetty/terminal-view.h>
#include "terminal.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

class TerminalViewImpl : public TerminalView {
public:
    TerminalViewImpl() = default;

    Result<void> onShutdown() override {
        if (terminal_) {
            return terminal_->shutdown();
        }
        return Ok();
    }

    Result<void> init(const YettyContext& ctx) {
        auto terminalResult = Terminal::create(ctx);
        if (!terminalResult) {
            return Err<void>("Failed to create Terminal", terminalResult);
        }
        terminal_ = *terminalResult;
        yinfo("TerminalView created");
        return Ok();
    }

    Result<void> render(WGPURenderPassEncoder pass) override {
        if (!terminal_) {
            return Ok();
        }
        return terminal_->render(pass);
    }

    Result<bool> onEvent(const Event& event) override {
        // Terminal handles keyboard events directly via EventLoop registration
        // TerminalView only handles view-specific events
        (void)event;
        return Ok(false);
    }

    void setBounds(Rect r) override {
        View::setBounds(r);
        onBoundsChanged();
    }

protected:
    void onBoundsChanged() override {
        if (!terminal_) {
            return;
        }

        // Just set viewport - GPUScreen calculates cols/rows from bounds and cell size
        terminal_->setViewport(_bounds.x, _bounds.y, _bounds.width, _bounds.height);
    }

private:
    Terminal::Ptr terminal_;
};

Result<TerminalView::Ptr> TerminalView::create(const YettyContext& ctx) noexcept {
    auto view = std::make_shared<TerminalViewImpl>();
    if (auto res = view->init(ctx); !res) {
        return Err<Ptr>("Failed to init TerminalView", res);
    }
    return Ok<Ptr>(view);
}

} // namespace yetty
