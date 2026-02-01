#include "event.h"

#include <args.hxx>
#include <yetty/rpc/rpc-client.h>

#include <iostream>
#include <string>
#include <cstdint>
#include <unordered_map>

using namespace yetty::rpc;

namespace yetty::client {

// ─── Event type table ────────────────────────────────────────────────────────

struct EventTypeInfo {
    const char* name;
    const char* params;
};

static constexpr EventTypeInfo eventTypes[] = {
    {"card-mouse-down",      "--target-id N --x F --y F --button N"},
    {"card-mouse-move",      "--target-id N --x F --y F"},
    {"card-mouse-up",        "--target-id N --x F --y F --button N"},
    {"card-scroll",          "--target-id N --x F --y F --dx F --dy F"},
    {"char",                 "--codepoint N [--mods N]"},
    {"close",                "--object-id N"},
    {"context-menu-action",  "--object-id N --action STR --row N --col N"},
    {"key-down",             "--key N --mods N [--scancode N]"},
    {"key-up",               "--key N --mods N [--scancode N]"},
    {"mouse-down",           "--x F --y F --button N"},
    {"mouse-drag",           "--x F --y F --button N"},
    {"mouse-move",           "--x F --y F"},
    {"mouse-up",             "--x F --y F --button N"},
    {"resize",               "--width F --height F"},
    {"scroll",               "--x F --y F --dx F --dy F [--mods N]"},
    {"set-focus",            "--object-id N"},
    {"split",                "--object-id N --orientation STR"},
};

// ─── event list ──────────────────────────────────────────────────────────────

static Result<void> cmdEventList(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    args::ArgumentParser parser("");
    parser.Prog(ctx.prog + " event list");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});

    try {
        parser.ParseArgs(begin, end);
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("event list: " + std::string(e.what()));
    }

    std::cout << "Available event types:\n\n";
    for (const auto& et : eventTypes) {
        std::cout << "  " << et.name << "\n";
        std::cout << "      " << et.params << "\n";
    }
    return Ok();
}

// ─── event send ──────────────────────────────────────────────────────────────

static Result<void> sendEvent(RpcClient& client, const std::string& type,
                              int key, int mods, int scancode,
                              float x, float y, float dx, float dy,
                              int button, uint32_t codepoint,
                              uint64_t objectId, uint64_t targetId,
                              const std::string& action, int row, int col,
                              float width, float height,
                              int orientation) {
    if      (type == "key-down")            return client.keyDown(key, mods, scancode);
    else if (type == "key-up")              return client.keyUp(key, mods, scancode);
    else if (type == "char")                return client.charInput(codepoint, mods);
    else if (type == "mouse-down")          return client.mouseDown(x, y, button);
    else if (type == "mouse-up")            return client.mouseUp(x, y, button);
    else if (type == "mouse-move")          return client.mouseMove(x, y);
    else if (type == "mouse-drag")          return client.mouseDrag(x, y, button);
    else if (type == "scroll")              return client.scroll(x, y, dx, dy, mods);
    else if (type == "set-focus")           return client.setFocus(objectId);
    else if (type == "resize")              return client.resize(width, height);
    else if (type == "context-menu-action") return client.contextMenuAction(objectId, action, row, col);
    else if (type == "card-mouse-down")     return client.cardMouseDown(targetId, x, y, button);
    else if (type == "card-mouse-up")       return client.cardMouseUp(targetId, x, y, button);
    else if (type == "card-mouse-move")     return client.cardMouseMove(targetId, x, y);
    else if (type == "card-scroll")         return client.cardScroll(targetId, x, y, dx, dy);
    else if (type == "close")              return client.close(objectId);
    else if (type == "split")              return client.split(objectId, orientation);
    else {
        return Err("unknown event type '" + type + "'. Run 'yettyc event list' for available types.");
    }
}

static Result<void> cmdEventSend(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    args::ArgumentParser parser("", "Run 'yettyc event list' for available event types.");
    parser.Prog(ctx.prog + " event send");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});

    args::Positional<std::string> eventType(parser, "type",
        "Event type (e.g. key-down, mouse-move, resize)");

    args::ValueFlag<int>         fKey(parser, "N", "Key code", {"key"});
    args::ValueFlag<int>         fMods(parser, "N", "Modifier flags", {"mods"});
    args::ValueFlag<int>         fScancode(parser, "N", "Scancode", {"scancode"});
    args::ValueFlag<float>       fX(parser, "F", "X coordinate", {"x"});
    args::ValueFlag<float>       fY(parser, "F", "Y coordinate", {"y"});
    args::ValueFlag<float>       fDx(parser, "F", "Delta X", {"dx"});
    args::ValueFlag<float>       fDy(parser, "F", "Delta Y", {"dy"});
    args::ValueFlag<int>         fButton(parser, "N", "Mouse button", {"button"});
    args::ValueFlag<uint32_t>    fCodepoint(parser, "N", "Unicode codepoint", {"codepoint"});
    args::ValueFlag<uint64_t>    fObjectId(parser, "N", "Object ID", {"object-id"});
    args::ValueFlag<uint64_t>    fTargetId(parser, "N", "Target card ID", {"target-id"});
    args::ValueFlag<std::string> fAction(parser, "STR", "Action string", {"action"});
    args::ValueFlag<int>         fRow(parser, "N", "Row", {"row"});
    args::ValueFlag<int>         fCol(parser, "N", "Column", {"col"});
    args::ValueFlag<float>       fWidth(parser, "F", "Width", {"width"});
    args::ValueFlag<float>       fHeight(parser, "F", "Height", {"height"});
    args::ValueFlag<std::string> fOrientation(parser, "STR", "Orientation (horizontal/vertical)", {"orientation"});

    try {
        parser.ParseArgs(begin, end);
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("event send: " + std::string(e.what()));
    }

    if (!eventType) {
        return Err<void>("event type required");
    }

    auto clientResult = RpcClient::create(ctx.socketPath);
    if (!clientResult) {
        return Err<void>("failed to create client", clientResult);
    }
    auto& client = *clientResult;

    auto connResult = client->connect();
    if (!connResult) {
        return Err<void>("failed to connect", connResult);
    }

    // Parse orientation string to int (0=horizontal, 1=vertical)
    int orientVal = 1; // default vertical
    if (fOrientation) {
        auto s = args::get(fOrientation);
        if (s == "horizontal" || s == "h") orientVal = 0;
        else if (s == "vertical" || s == "v") orientVal = 1;
        else {
            return Err<void>("invalid orientation '" + s + "', use 'horizontal' or 'vertical'");
        }
    }

    return sendEvent(*client, args::get(eventType),
        fKey       ? args::get(fKey)       : 0,
        fMods      ? args::get(fMods)      : 0,
        fScancode  ? args::get(fScancode)  : 0,
        fX         ? args::get(fX)         : 0.0f,
        fY         ? args::get(fY)         : 0.0f,
        fDx        ? args::get(fDx)        : 0.0f,
        fDy        ? args::get(fDy)        : 0.0f,
        fButton    ? args::get(fButton)    : 0,
        fCodepoint ? args::get(fCodepoint) : 0,
        fObjectId  ? args::get(fObjectId)  : 0,
        fTargetId  ? args::get(fTargetId)  : 0,
        fAction    ? args::get(fAction)    : "",
        fRow       ? args::get(fRow)       : 0,
        fCol       ? args::get(fCol)       : 0,
        fWidth     ? args::get(fWidth)     : 0.0f,
        fHeight    ? args::get(fHeight)    : 0.0f,
        orientVal);
}

// ─── event (dispatch to subcommand) ─────────────────────────────────────────

Result<void> cmdEvent(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    using SubCmdFn = std::function<Result<void>(const CmdContext&, ArgIt, ArgIt)>;
    const std::unordered_map<std::string, SubCmdFn> subcmds = {
        {"send", cmdEventSend},
        {"list", cmdEventList},
    };

    args::ArgumentParser parser("", "Subcommands: send, list");
    parser.Prog(ctx.prog + " event");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
    args::MapPositional<std::string, SubCmdFn> subcmd(parser, "subcommand",
        "Subcommand to run", subcmds);
    subcmd.KickOut(true);

    try {
        auto next = parser.ParseArgs(begin, end);
        if (subcmd) {
            return args::get(subcmd)(ctx, next, end);
        }
        std::cout << parser;
        return Ok();
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("event: " + std::string(e.what()));
    }
}

} // namespace yetty::client
