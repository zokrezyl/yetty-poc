#include "cards.h"

#include <args.hxx>
#include <yetty/rpc/rpc-client.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace yetty::rpc;

namespace yetty::client {

// ─── cards list ─────────────────────────────────────────────────────────────

static Result<void> cmdCardsList(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    args::ArgumentParser parser("");
    parser.Prog(ctx.prog + " cards list");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});

    try {
        parser.ParseArgs(begin, end);
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("cards list: " + std::string(e.what()));
    }

    auto clientResult = RpcClient::create(ctx.socketPath);
    if (!clientResult) {
        return Err<void>("failed to create client", clientResult);
    }
    auto& client = *clientResult;

    if (auto res = client->connect(); !res) {
        return Err<void>("failed to connect", res);
    }

    // Call cards_list RPC
    msgpack::sbuffer params;
    msgpack::packer<msgpack::sbuffer> pk(&params);
    pk.pack_map(0);

    auto result = client->request(Channel::CardStream, "cards_list", params);
    if (!result) {
        return Err<void>("cards_list failed", result);
    }

    auto& obj = (*result).get();
    if (obj.type != msgpack::type::ARRAY) {
        return Err<void>("unexpected response type");
    }

    // Print header
    std::cout << std::left
              << std::setw(8) << "SLOT"
              << std::setw(20) << "NAME"
              << std::setw(20) << "TYPE"
              << "\n";
    std::cout << std::string(48, '-') << "\n";

    // Print cards
    for (uint32_t i = 0; i < obj.via.array.size; ++i) {
        auto& card = obj.via.array.ptr[i];
        if (card.type != msgpack::type::MAP) continue;

        uint32_t slotIndex = 0;
        std::string name, type;

        for (uint32_t j = 0; j < card.via.map.size; ++j) {
            auto& key = card.via.map.ptr[j].key;
            auto& val = card.via.map.ptr[j].val;
            if (key.type != msgpack::type::STR) continue;

            std::string keyStr(key.via.str.ptr, key.via.str.size);
            if (keyStr == "slot_index") {
                slotIndex = val.as<uint32_t>();
            } else if (keyStr == "name" && val.type == msgpack::type::STR) {
                name = std::string(val.via.str.ptr, val.via.str.size);
            } else if (keyStr == "type" && val.type == msgpack::type::STR) {
                type = std::string(val.via.str.ptr, val.via.str.size);
            }
        }

        std::cout << std::left
                  << std::setw(8) << slotIndex
                  << std::setw(20) << (name.empty() ? "-" : name)
                  << std::setw(20) << type
                  << "\n";
    }

    return Ok();
}

// ─── cards (dispatch to subcommand) ─────────────────────────────────────────

Result<void> cmdCards(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    using SubCmdFn = std::function<Result<void>(const CmdContext&, ArgIt, ArgIt)>;
    const std::unordered_map<std::string, SubCmdFn> subcmds = {
        {"list", cmdCardsList},
    };

    args::ArgumentParser parser("", "Subcommands: list");
    parser.Prog(ctx.prog + " cards");
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
        return Err("cards: " + std::string(e.what()));
    }
}

} // namespace yetty::client
