#include <yetty/rpc/event-loop-handler.h>
#include <yetty/platform/event-loop.h>
#include <yetty/workspace.h>
#include <yetty/gpu-screen-manager.h>
#include "../gpu-screen.h"
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

    ydebug("Registered {} EventLoop handlers on Channel 0", 17);
    return Ok();
}

Result<void> registerWorkspaceHandlers(RpcServer& server, std::shared_ptr<Workspace> workspace) {
    // ui_tree: {} -> string (YAML dump of the UI tree)
    server.registerHandler(Channel::EventLoop, "ui_tree",
        [workspace](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto res = workspace->asTree(ygui::DataPath::root());
            if (!res) {
                return Err<msgpack::object_handle>(error_msg(res));
            }
            return stringResult(*res);
        });

    ydebug("Registered workspace handlers on Channel 0");
    return Ok();
}

// Helper to find screen containing a named card
static GPUScreenManager::GPUScreenPtr findScreenByCardName(const std::string& cardName) {
    auto mgrResult = GPUScreenManager::instance();
    if (!mgrResult) return nullptr;

    for (const auto& screen : (*mgrResult)->screens()) {
        if (screen->getCardByName(cardName)) {
            return screen;
        }
    }
    return nullptr;
}

// Helper to get first available screen
static GPUScreenManager::GPUScreenPtr getFirstScreen() {
    auto mgrResult = GPUScreenManager::instance();
    if (!mgrResult) return nullptr;

    auto screens = (*mgrResult)->screens();
    return screens.empty() ? nullptr : screens.front();
}

Result<void> registerTestHandlers(RpcServer& server) {
    // get_cell_size: {} -> {width: float, height: float}
    // Returns the current cell dimensions in pixels
    server.registerHandler(Channel::EventLoop, "get_cell_size",
        [](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto screen = getFirstScreen();
            if (!screen) {
                return Err<msgpack::object_handle>("No GPUScreen available");
            }

            float cellW = screen->getCellWidth();
            float cellH = screen->getCellHeight();

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);
            pk.pack_map(2);
            pk.pack("width");
            pk.pack(cellW);
            pk.pack("height");
            pk.pack(cellH);

            ydebug("get_cell_size: {}x{}", cellW, cellH);
            return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
        });

    // get_card_info: {name: string} -> {id, name, x, y, width_cells, height_cells,
    //                                   display_w, display_h, cell_w, cell_h}
    // Returns detailed information about a card
    server.registerHandler(Channel::EventLoop, "get_card_info",
        [](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            if (params.type != msgpack::type::MAP) {
                return Err<msgpack::object_handle>("get_card_info: expected map params");
            }

            std::string cardName;
            for (uint32_t i = 0; i < params.via.map.size; ++i) {
                auto& key = params.via.map.ptr[i].key;
                auto& val = params.via.map.ptr[i].val;
                if (key.type == msgpack::type::STR) {
                    std::string keyStr(key.via.str.ptr, key.via.str.size);
                    if (keyStr == "name" && val.type == msgpack::type::STR) {
                        cardName = std::string(val.via.str.ptr, val.via.str.size);
                    }
                }
            }

            if (cardName.empty()) {
                return Err<msgpack::object_handle>("get_card_info: 'name' is required");
            }

            auto screen = findScreenByCardName(cardName);
            if (!screen) {
                return Err<msgpack::object_handle>("get_card_info: card '" + cardName + "' not found");
            }

            Card* card = screen->getCardByName(cardName);
            if (!card) {
                return Err<msgpack::object_handle>("get_card_info: card '" + cardName + "' not found");
            }

            float cellW = screen->getCellWidth();
            float cellH = screen->getCellHeight();
            float displayW = static_cast<float>(card->widthCells()) * cellW;
            float displayH = static_cast<float>(card->heightCells()) * cellH;

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);
            pk.pack_map(10);
            pk.pack("id");
            pk.pack(card->metadataSlotIndex());
            pk.pack("name");
            pk.pack(card->name());
            pk.pack("x");
            pk.pack(card->x());
            pk.pack("y");
            pk.pack(card->y());
            pk.pack("width_cells");
            pk.pack(card->widthCells());
            pk.pack("height_cells");
            pk.pack(card->heightCells());
            pk.pack("display_w");
            pk.pack(displayW);
            pk.pack("display_h");
            pk.pack(displayH);
            pk.pack("cell_w");
            pk.pack(cellW);
            pk.pack("cell_h");
            pk.pack(cellH);

            ydebug("get_card_info: name='{}' pos=({},{}) cells={}x{} display={:.1f}x{:.1f}",
                   cardName, card->x(), card->y(), card->widthCells(), card->heightCells(),
                   displayW, displayH);

            return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
        });

    // compute_click_coords: {name: string, display_x: float, display_y: float}
    //                    -> {osc_x: int, osc_y: int, display_x: float, display_y: float}
    // Computes what coordinates yetty would send via OSC 777777 for a click
    // at the given display position relative to the card.
    server.registerHandler(Channel::EventLoop, "compute_click_coords",
        [](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            if (params.type != msgpack::type::MAP) {
                return Err<msgpack::object_handle>("compute_click_coords: expected map params");
            }

            std::string cardName;
            float displayX = 0.0f;
            float displayY = 0.0f;

            for (uint32_t i = 0; i < params.via.map.size; ++i) {
                auto& key = params.via.map.ptr[i].key;
                auto& val = params.via.map.ptr[i].val;
                if (key.type == msgpack::type::STR) {
                    std::string keyStr(key.via.str.ptr, key.via.str.size);
                    if (keyStr == "name" && val.type == msgpack::type::STR) {
                        cardName = std::string(val.via.str.ptr, val.via.str.size);
                    } else if (keyStr == "display_x") {
                        displayX = val.as<float>();
                    } else if (keyStr == "display_y") {
                        displayY = val.as<float>();
                    }
                }
            }

            if (cardName.empty()) {
                return Err<msgpack::object_handle>("compute_click_coords: 'name' is required");
            }

            auto screen = findScreenByCardName(cardName);
            if (!screen) {
                return Err<msgpack::object_handle>("compute_click_coords: card '" + cardName + "' not found");
            }

            Card* card = screen->getCardByName(cardName);
            if (!card) {
                return Err<msgpack::object_handle>("compute_click_coords: card '" + cardName + "' not found");
            }

            // Clamp to card's pixel dimensions (same as gpu-screen.cpp does)
            float cellW = screen->getCellWidth();
            float cellH = screen->getCellHeight();
            float cardPixelW = static_cast<float>(card->widthCells()) * cellW;
            float cardPixelH = static_cast<float>(card->heightCells()) * cellH;

            float clampedX = std::max(0.0f, std::min(displayX, cardPixelW - 1.0f));
            float clampedY = std::max(0.0f, std::min(displayY, cardPixelH - 1.0f));

            // OSC sends integer coordinates
            int oscX = static_cast<int>(clampedX);
            int oscY = static_cast<int>(clampedY);

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);
            pk.pack_map(4);
            pk.pack("osc_x");
            pk.pack(oscX);
            pk.pack("osc_y");
            pk.pack(oscY);
            pk.pack("clamped_x");
            pk.pack(clampedX);
            pk.pack("clamped_y");
            pk.pack(clampedY);

            ydebug("compute_click_coords: card='{}' input=({:.1f},{:.1f}) osc=({},{})",
                   cardName, displayX, displayY, oscX, oscY);

            return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
        });

    ydebug("Registered 3 test handlers on Channel 0");
    return Ok();
}

} // namespace rpc
} // namespace yetty
