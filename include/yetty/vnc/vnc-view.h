#pragma once

#include <yetty/view.h>
#include <yetty/yetty-context.h>
#include <memory>
#include <string>

namespace yetty::vnc {

class VncClient;

class VncView : public View {
public:
    using Ptr = std::shared_ptr<VncView>;

    static Result<Ptr> create(const YettyContext& ctx, const std::string& host, uint16_t port) noexcept;

    ~VncView() override;

    // View interface
    Result<void> render(WGPURenderPassEncoder pass) override;
    Result<bool> onEvent(const base::Event& event) override;
    void setBounds(Rect r) override;

protected:
    VncView() = default;
    void onBoundsChanged() override;

    // Deferred connection - connect when bounds are first set
    Result<void> connectWithBounds();

    std::shared_ptr<VncClient> _client;
    std::string _host;
    uint16_t _port = 0;
    YettyContext _ctx;
};

} // namespace yetty::vnc
