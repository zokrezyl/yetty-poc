#include <yetty/vnc/vnc-view.h>
#include "vnc-client.h"
#include <yetty/base/event-loop.h>
#include <ytrace/ytrace.hpp>

namespace yetty::vnc {

// Default cell height if not specified
constexpr uint8_t DEFAULT_CELL_HEIGHT = 20;

VncView::~VncView() = default;

Result<VncView::Ptr> VncView::create(const YettyContext& ctx, const std::string& host, uint16_t port) noexcept {
    auto view = Ptr(new VncView());
    view->_host = host;
    view->_port = port;
    view->_ctx = ctx;
    // Don't create client or connect yet - wait for setBounds() to know the view size
    yinfo("VncView initialized for {}:{} - waiting for bounds before connect", host, port);
    return Ok<Ptr>(std::move(view));
}

Result<void> VncView::connectWithBounds() {
    if (_client) {
        return Ok();  // Already connected
    }

    uint16_t width = static_cast<uint16_t>(_bounds.width);
    uint16_t height = static_cast<uint16_t>(_bounds.height);

    if (width == 0 || height == 0) {
        return Err<void>("Cannot connect with zero bounds");
    }

    // Create client with view size - texture is created immediately
    _client = std::make_shared<VncClient>(_ctx.gpu.device, _ctx.gpu.queue, _ctx.gpu.surfaceFormat, width, height);

    // Connect to server
    if (auto res = _client->connect(_host, _port); !res) {
        _client = nullptr;
        return Err<void>("Failed to connect VNC client", res);
    }

    // Send initial size + cell height FIRST - server adjusts before sending frames
    _client->sendResize(width, height);
    _client->sendCellSize(DEFAULT_CELL_HEIGHT);

    // Set up frame received callback for immediate screen refresh
    _client->onFrameReceived = []() {
        auto loopResult = base::EventLoop::instance();
        if (!loopResult) return;
        (*loopResult)->dispatch(base::Event::screenUpdateEvent());
    };

    yinfo("VncView connected to {}:{} with bounds {}x{}", _host, _port, width, height);
    return Ok();
}

Result<void> VncView::render(WGPURenderPassEncoder pass) {
    if (!_client) {
        return Ok();
    }

    // Update texture with any pending tiles
    if (auto res = _client->updateTexture(); !res) {
        ywarn("VncView: updateTexture failed");
    }

    // Render the frame
    return _client->render(pass);
}

Result<bool> VncView::onEvent(const base::Event& event) {
    if (!_client) {
        return Ok(false);
    }

    switch (event.type) {
        case base::Event::Type::KeyDown:
            _client->sendKeyDown(
                static_cast<uint32_t>(event.key.key),
                static_cast<uint32_t>(event.key.scancode),
                static_cast<uint8_t>(event.key.mods));
            return Ok(true);

        case base::Event::Type::KeyUp:
            _client->sendKeyUp(
                static_cast<uint32_t>(event.key.key),
                static_cast<uint32_t>(event.key.scancode),
                static_cast<uint8_t>(event.key.mods));
            return Ok(true);

        case base::Event::Type::Char:
            _client->sendCharWithMods(event.chr.codepoint, static_cast<uint8_t>(event.chr.mods));
            return Ok(true);

        case base::Event::Type::MouseMove: {
            // Convert to view-local coordinates
            int16_t x = static_cast<int16_t>(event.mouse.x - _bounds.x);
            int16_t y = static_cast<int16_t>(event.mouse.y - _bounds.y);
            _client->sendMouseMove(x, y);
            return Ok(true);
        }

        case base::Event::Type::MouseDown: {
            int16_t x = static_cast<int16_t>(event.mouse.x - _bounds.x);
            int16_t y = static_cast<int16_t>(event.mouse.y - _bounds.y);
            MouseButton btn = static_cast<MouseButton>(event.mouse.button);
            _client->sendMouseButton(x, y, btn, true);
            return Ok(true);
        }

        case base::Event::Type::MouseUp: {
            int16_t x = static_cast<int16_t>(event.mouse.x - _bounds.x);
            int16_t y = static_cast<int16_t>(event.mouse.y - _bounds.y);
            MouseButton btn = static_cast<MouseButton>(event.mouse.button);
            _client->sendMouseButton(x, y, btn, false);
            return Ok(true);
        }

        case base::Event::Type::Scroll: {
            int16_t x = static_cast<int16_t>(event.scroll.x - _bounds.x);
            int16_t y = static_cast<int16_t>(event.scroll.y - _bounds.y);
            _client->sendMouseScroll(x, y,
                static_cast<int16_t>(event.scroll.dx),
                static_cast<int16_t>(event.scroll.dy));
            return Ok(true);
        }

        default:
            return Ok(false);
    }
}

void VncView::setBounds(Rect r) {
    View::setBounds(r);
    onBoundsChanged();
}

void VncView::onBoundsChanged() {
    if (!_client) {
        // First time bounds are set - connect now
        if (_bounds.width > 0 && _bounds.height > 0) {
            if (auto res = connectWithBounds(); !res) {
                ywarn("VncView: failed to connect: {}", res.error().message());
            }
        }
        return;
    }

    // Client exists - send resize to server
    if (_bounds.width > 0 && _bounds.height > 0) {
        _client->sendResize(
            static_cast<uint16_t>(_bounds.width),
            static_cast<uint16_t>(_bounds.height)
        );
        yinfo("VncView: sent resize {}x{}", _bounds.width, _bounds.height);
    }
}

} // namespace yetty::vnc
