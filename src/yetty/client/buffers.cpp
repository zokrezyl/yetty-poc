#include "buffers.h"

#include <args.hxx>
#include <yetty/rpc/rpc-client.h>

#include <iomanip>
#include <iostream>
#include <string>

using namespace yetty::rpc;

namespace yetty::client {

Result<void> cmdBuffers(const CmdContext& ctx, ArgIt begin, ArgIt end) {
    args::ArgumentParser parser("List card buffers");
    parser.Prog(ctx.prog + " buffers");
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
    args::ValueFlag<std::string> cardFlag(parser, "card", "Filter by card name", {'c', "card"});

    try {
        parser.ParseArgs(begin, end);
    } catch (const args::Help&) {
        std::cout << parser;
        return Ok();
    } catch (const args::Error& e) {
        return Err("buffers: " + std::string(e.what()));
    }

    std::string filterCard = cardFlag ? args::get(cardFlag) : "";

    auto clientResult = RpcClient::create(ctx.socketPath);
    if (!clientResult) {
        return Err<void>("failed to create client", clientResult);
    }
    auto& client = *clientResult;

    if (auto res = client->connect(); !res) {
        return Err<void>("failed to connect", res);
    }

    // Call buffers_list RPC
    msgpack::sbuffer params;
    msgpack::packer<msgpack::sbuffer> pk(&params);
    pk.pack_map(0);

    auto result = client->request(Channel::CardStream, "buffers_list", params);
    if (!result) {
        return Err<void>("buffers_list failed", result);
    }

    auto& obj = (*result).get();
    if (obj.type != msgpack::type::ARRAY) {
        return Err<void>("unexpected response type");
    }

    // Print header
    std::cout << std::left
              << std::setw(8) << "SLOT"
              << std::setw(20) << "CARD"
              << std::setw(12) << "NAME"
              << std::right
              << std::setw(12) << "OFFSET"
              << std::setw(12) << "SIZE"
              << "\n";
    std::cout << std::string(64, '-') << "\n";

    // Print buffers
    for (uint32_t i = 0; i < obj.via.array.size; ++i) {
        auto& buf = obj.via.array.ptr[i];
        if (buf.type != msgpack::type::MAP) continue;

        uint32_t slotIndex = 0, offset = 0, size = 0;
        std::string cardName, bufferName;

        for (uint32_t j = 0; j < buf.via.map.size; ++j) {
            auto& key = buf.via.map.ptr[j].key;
            auto& val = buf.via.map.ptr[j].val;
            if (key.type != msgpack::type::STR) continue;

            std::string keyStr(key.via.str.ptr, key.via.str.size);
            if (keyStr == "slot_index") {
                slotIndex = val.as<uint32_t>();
            } else if (keyStr == "card" && val.type == msgpack::type::STR) {
                cardName = std::string(val.via.str.ptr, val.via.str.size);
            } else if (keyStr == "name" && val.type == msgpack::type::STR) {
                bufferName = std::string(val.via.str.ptr, val.via.str.size);
            } else if (keyStr == "offset") {
                offset = val.as<uint32_t>();
            } else if (keyStr == "size") {
                size = val.as<uint32_t>();
            }
        }

        // Filter by card name if specified
        if (!filterCard.empty() && cardName != filterCard) {
            continue;
        }

        std::cout << std::left
                  << std::setw(8) << slotIndex
                  << std::setw(20) << (cardName.empty() ? "-" : cardName)
                  << std::setw(12) << bufferName
                  << std::right
                  << std::setw(12) << offset
                  << std::setw(12) << size
                  << "\n";
    }

    return Ok();
}

} // namespace yetty::client
