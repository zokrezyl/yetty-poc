#include <yetty/terminal-view.h>
#include "terminal.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

class TerminalViewImpl : public TerminalView {
public:
    TerminalViewImpl() = default;

    ~TerminalViewImpl() override {
        if (terminal_) {
            terminal_->stop();
        }
    }

    Result<void> init(const GPUContext& gpuContext) {
        auto terminalResult = Terminal::create(gpuContext);
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
        if (!terminal_) {
            return Ok(false);
        }

        if (event.type == Event::Type::Char) {
            terminal_->sendKey(event.chr.codepoint, 0);
            return Ok(true);
        }

        if (event.type == Event::Type::KeyDown) {
            terminal_->sendSpecialKey(event.key.key, event.key.mods);
            return Ok(true);
        }

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

Result<TerminalView::Ptr> TerminalView::create(const GPUContext& gpuContext) noexcept {
    auto view = std::make_shared<TerminalViewImpl>();
    if (auto res = view->init(gpuContext); !res) {
        return Err<Ptr>("Failed to init TerminalView", res);
    }
    return Ok<Ptr>(view);
}

} // namespace yetty
