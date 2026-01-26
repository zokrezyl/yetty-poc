#pragma once

#include <yetty/view.h>
#include <yetty/yetty-context.h>
#include <memory>

namespace yetty {

class TerminalView : public View {
public:
    using Ptr = std::shared_ptr<TerminalView>;

    static Result<Ptr> create(const YettyContext& ctx) noexcept;

    ~TerminalView() override = default;

protected:
    TerminalView() = default;
};

} // namespace yetty
