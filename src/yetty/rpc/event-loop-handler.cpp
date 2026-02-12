#include <yetty/rpc/event-loop-handler.h>
#include <yetty/base/event-loop.h>
#include <yetty/workspace.h>
#include <ytrace/ytrace.hpp>

namespace yetty {
namespace rpc {

// Helper: create an ok response as msgpack object_handle
static Result<msgpack::object_handle> okResult() {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, true);
    return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
}

// Helper: create a bool response
static Result<msgpack::object_handle> boolResult(bool value) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, value);
    return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
}

// Helper: create a string response
static Result<msgpack::object_handle> stringResult(const std::string& value) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, value);
    return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
}

Result<void> registerEventLoopHandlers(RpcServer& server) {
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Err<void>("registerEventLoopHandlers: no EventLoop", loopResult);
    }
    auto eventLoop = *loopResult;

    // key_down: {key: int, mods: int, scancode: int}
    server.registerHandler(Channel::EventLoop, "key_down",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::keyDown(
                params.via.map.ptr[0].val.as<int>(),
                params.via.map.ptr[1].val.as<int>(),
                params.via.map.ptr[2].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // key_up: {key: int, mods: int, scancode: int}
    server.registerHandler(Channel::EventLoop, "key_up",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::keyUp(
                params.via.map.ptr[0].val.as<int>(),
                params.via.map.ptr[1].val.as<int>(),
                params.via.map.ptr[2].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // char: {codepoint: uint32, mods: int}
    server.registerHandler(Channel::EventLoop, "char",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto codepoint = params.via.map.ptr[0].val.as<uint32_t>();
            auto mods = params.via.map.ptr[1].val.as<int>();
            auto event = (mods != 0)
                ? base::Event::charInputWithMods(codepoint, mods)
                : base::Event::charInput(codepoint);
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // mouse_down: {x: float, y: float, button: int}
    server.registerHandler(Channel::EventLoop, "mouse_down",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::mouseDown(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // mouse_up: {x: float, y: float, button: int}
    server.registerHandler(Channel::EventLoop, "mouse_up",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::mouseUp(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // mouse_move: {x: float, y: float}
    server.registerHandler(Channel::EventLoop, "mouse_move",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::mouseMove(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // mouse_drag: {x: float, y: float, button: int}
    server.registerHandler(Channel::EventLoop, "mouse_drag",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::mouseDrag(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // scroll: {x: float, y: float, dx: float, dy: float, mods: int}
    server.registerHandler(Channel::EventLoop, "scroll",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::scrollEvent(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<float>(),
                params.via.map.ptr[3].val.as<float>(),
                params.via.map.ptr[4].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // set_focus: {object_id: uint64}
    server.registerHandler(Channel::EventLoop, "set_focus",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::focusEvent(
                params.via.map.ptr[0].val.as<uint64_t>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // resize: {width: float, height: float}
    server.registerHandler(Channel::EventLoop, "resize",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::resizeEvent(
                params.via.map.ptr[0].val.as<float>(),
                params.via.map.ptr[1].val.as<float>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // context_menu_action: {object_id: uint64, action: string, row: int, col: int}
    server.registerHandler(Channel::EventLoop, "context_menu_action",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::contextMenuAction(
                params.via.map.ptr[0].val.as<uint64_t>(),
                params.via.map.ptr[1].val.as<std::string>(),
                params.via.map.ptr[2].val.as<int>(),
                params.via.map.ptr[3].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // card_mouse_down: {target_id: uint64, x: float, y: float, button: int}
    server.registerHandler(Channel::EventLoop, "card_mouse_down",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::cardMouseDown(
                params.via.map.ptr[0].val.as<uint64_t>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<float>(),
                params.via.map.ptr[3].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // card_mouse_up: {target_id: uint64, x: float, y: float, button: int}
    server.registerHandler(Channel::EventLoop, "card_mouse_up",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::cardMouseUp(
                params.via.map.ptr[0].val.as<uint64_t>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<float>(),
                params.via.map.ptr[3].val.as<int>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // card_mouse_move: {target_id: uint64, x: float, y: float}
    server.registerHandler(Channel::EventLoop, "card_mouse_move",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::cardMouseMove(
                params.via.map.ptr[0].val.as<uint64_t>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<float>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // card_scroll: {target_id: uint64, x: float, y: float, dx: float, dy: float}
    server.registerHandler(Channel::EventLoop, "card_scroll",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::cardScrollEvent(
                params.via.map.ptr[0].val.as<uint64_t>(),
                params.via.map.ptr[1].val.as<float>(),
                params.via.map.ptr[2].val.as<float>(),
                params.via.map.ptr[3].val.as<float>(),
                params.via.map.ptr[4].val.as<float>()
            );
            auto res = eventLoop->dispatch(event);
            if (msg.type == MessageType::Request) {
                return boolResult(res.has_value() && *res);
            }
            return okResult();
        });

    // close: {object_id: uint64}
    server.registerHandler(Channel::EventLoop, "close",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::closeEvent(
                params.via.map.ptr[0].val.as<uint64_t>()
            );
            auto res = eventLoop->dispatch(event);
            if (!res) {
                return Err<msgpack::object_handle>(error_msg(res));
            }
            return okResult();
        });

    // split: {object_id: uint64, orientation: int}
    server.registerHandler(Channel::EventLoop, "split",
        [eventLoop](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            auto event = base::Event::splitPaneEvent(
                params.via.map.ptr[0].val.as<uint64_t>(),
                static_cast<uint8_t>(params.via.map.ptr[1].val.as<int>())
            );
            auto res = eventLoop->dispatch(event);
            if (!res) {
                return Err<msgpack::object_handle>(error_msg(res));
            }
            return okResult();
        });

    yinfo("Registered {} EventLoop handlers on Channel 0", 17);
    return Ok();
}

Result<void> registerWorkspaceHandlers(RpcServer& server, std::shared_ptr<Workspace> workspace) {
    // ui_tree: {} -> string (YAML dump of the UI tree)
    server.registerHandler(Channel::EventLoop, "ui_tree",
        [workspace](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto res = workspace->asTree(ymery::DataPath::root());
            if (!res) {
                return Err<msgpack::object_handle>(error_msg(res));
            }
            return stringResult(*res);
        });

    yinfo("Registered workspace handlers on Channel 0");
    return Ok();
}

} // namespace rpc
} // namespace yetty
