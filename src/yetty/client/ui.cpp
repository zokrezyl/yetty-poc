#include "ui.h"

#include <args.hxx>
#include <yetty/rpc/rpc-client.h>

#include <iostream>
#include <string>
#include <unordered_map>

using namespace yetty::rpc;

namespace yetty::client {

// ─── ui tree ─────────────────────────────────────────────────────────────────

static Result<void> cmdUiTree(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    args::ArgumentParser parser("");
    parser.Prog(ctx.prog + " ui tree");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});

    try {
        parser.ParseArgs(begin, end);
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("ui tree: " + std::string(e.what()));
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

    auto treeResult = client->uiTree();
    if (!treeResult) {
        return Err<void>("ui tree failed", treeResult);
    }

    std::cout << *treeResult;
    return Ok();
}

// ─── ui (dispatch to subcommand) ─────────────────────────────────────────────

Result<void> cmdUi(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    using SubCmdFn = std::function<Result<void>(const CmdContext&, ArgIt, ArgIt)>;
    const std::unordered_map<std::string, SubCmdFn> subcmds = {
        {"tree", cmdUiTree},
    };

    args::ArgumentParser parser("", "Subcommands: tree");
    parser.Prog(ctx.prog + " ui");
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
        return Err("ui: " + std::string(e.what()));
    }
}

} // namespace yetty::client
