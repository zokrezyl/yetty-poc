#include <yetty/rpc/stream-handler.h>
#include <yetty/rpc/stream-types.h>
#include <yetty/gpu-screen-manager.h>
#include <yetty/card-buffer-manager.h>
#include "../gpu-screen.h"
#include <ytrace/ytrace.hpp>

namespace yetty {
namespace rpc {

// Helper to create msgpack result
static Result<msgpack::object_handle> packResult(msgpack::sbuffer& sbuf) {
    return Ok(msgpack::unpack(sbuf.data(), sbuf.size()));
}

// Helper to find screen containing a named card
static GPUScreenManager::GPUScreenPtr findScreen(const std::string& cardName) {
    auto mgrResult = GPUScreenManager::instance();
    if (!mgrResult) {
        ywarn("findScreen: GPUScreenManager not available");
        return nullptr;
    }

    auto screens = (*mgrResult)->screens();
    ydebug("findScreen: looking for '{}' in {} screens", cardName, screens.size());

    for (const auto& screen : screens) {
        if (screen->getCardByName(cardName)) {
            ydebug("findScreen: found '{}' in screen", cardName);
            return screen;
        }
    }
    ywarn("findScreen: card '{}' not found in any screen", cardName);
    return nullptr;
}

// Helper to get first available CardManager
static GPUScreenManager::GPUScreenPtr getFirstScreen() {
    auto mgrResult = GPUScreenManager::instance();
    if (!mgrResult) return nullptr;

    auto screens = (*mgrResult)->screens();
    if (screens.empty()) return nullptr;

    return screens.front();
}

static CardManager::Ptr getFirstCardManager() {
    auto screen = getFirstScreen();
    return screen ? screen->cardManager() : nullptr;
}

Result<void> registerStreamHandlers(RpcServer& server) {
    // stream_connect: {} -> {data_shm, data_size}
    server.registerHandler(Channel::CardStream, "stream_connect",
        [](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto cardManager = getFirstCardManager();
            if (!cardManager) {
                return Err<msgpack::object_handle>("No CardManager available");
            }
            auto bufMgr = cardManager->bufferManager();
            if (!bufMgr) {
                return Err<msgpack::object_handle>("No buffer manager");
            }

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);

            pk.pack_map(2);
            pk.pack("data_shm");
            if (bufMgr->usesSharedMemory()) {
                pk.pack(bufMgr->shm()->name());
            } else {
                pk.pack("");
            }
            pk.pack("data_size");
            if (bufMgr->usesSharedMemory()) {
                pk.pack(static_cast<uint64_t>(bufMgr->shm()->size()));
            } else {
                pk.pack(uint64_t(0));
            }

            yinfo("stream_connect: shm='{}' size={}",
                  bufMgr->usesSharedMemory() ? bufMgr->shm()->name() : "",
                  bufMgr->usesSharedMemory() ? bufMgr->shm()->size() : 0);

            return packResult(sbuf);
        });

    // stream_get_buffer: {name, scope} -> {offset, size, slot_index}
    server.registerHandler(Channel::CardStream, "stream_get_buffer",
        [](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            if (params.type != msgpack::type::MAP) {
                return Err<msgpack::object_handle>("stream_get_buffer: expected map params");
            }

            std::string cardName;
            std::string scope = "storage";

            for (uint32_t i = 0; i < params.via.map.size; ++i) {
                auto& key = params.via.map.ptr[i].key;
                auto& val = params.via.map.ptr[i].val;
                if (key.type == msgpack::type::STR) {
                    std::string keyStr(key.via.str.ptr, key.via.str.size);
                    if (keyStr == "name" && val.type == msgpack::type::STR) {
                        cardName = std::string(val.via.str.ptr, val.via.str.size);
                    } else if (keyStr == "scope" && val.type == msgpack::type::STR) {
                        scope = std::string(val.via.str.ptr, val.via.str.size);
                    }
                }
            }

            if (cardName.empty()) {
                return Err<msgpack::object_handle>("stream_get_buffer: 'name' is required");
            }

            auto screen = findScreen(cardName);
            if (!screen) {
                return Err<msgpack::object_handle>("stream_get_buffer: card '" + cardName + "' not found");
            }

            auto slotIndexOpt = screen->getSlotIndexByName(cardName);
            if (!slotIndexOpt) {
                return Err<msgpack::object_handle>("stream_get_buffer: card '" + cardName + "' not registered");
            }
            uint32_t slotIndex = *slotIndexOpt;

            auto bufMgr = screen->cardManager()->bufferManager();
            if (!bufMgr || !bufMgr->usesSharedMemory()) {
                return Err<msgpack::object_handle>("stream_get_buffer: shared memory not enabled");
            }

            BufferHandle handle = bufMgr->getBufferHandle(slotIndex, scope);
            if (!handle.isValid()) {
                return Err<msgpack::object_handle>("stream_get_buffer: no buffer for '" + cardName + "' scope '" + scope + "'");
            }

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);

            pk.pack_map(3);
            pk.pack("offset");
            pk.pack(handle.offset);
            pk.pack("size");
            pk.pack(handle.size);
            pk.pack("slot_index");
            pk.pack(slotIndex);

            yinfo("stream_get_buffer: card='{}' slot={} offset={} size={}",
                  cardName, slotIndex, handle.offset, handle.size);

            return packResult(sbuf);
        });

    // stream_disconnect: {} -> {}
    server.registerHandler(Channel::CardStream, "stream_disconnect",
        [](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            yinfo("stream_disconnect");
            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);
            pk.pack_map(1);
            pk.pack("ok");
            pk.pack(true);
            return packResult(sbuf);
        });

    // stream_mark_dirty: {name, scope} -> {ok}
    // Marks a buffer as dirty so it gets uploaded to GPU on next frame
    server.registerHandler(Channel::CardStream, "stream_mark_dirty",
        [](const RpcMessage& msg) -> Result<msgpack::object_handle> {
            auto& params = msg.params.get();
            if (params.type != msgpack::type::MAP) {
                return Err<msgpack::object_handle>("stream_mark_dirty: expected map params");
            }

            std::string cardName;
            std::string scope = "storage";

            for (uint32_t i = 0; i < params.via.map.size; ++i) {
                auto& key = params.via.map.ptr[i].key;
                auto& val = params.via.map.ptr[i].val;
                if (key.type == msgpack::type::STR) {
                    std::string keyStr(key.via.str.ptr, key.via.str.size);
                    if (keyStr == "name" && val.type == msgpack::type::STR) {
                        cardName = std::string(val.via.str.ptr, val.via.str.size);
                    } else if (keyStr == "scope" && val.type == msgpack::type::STR) {
                        scope = std::string(val.via.str.ptr, val.via.str.size);
                    }
                }
            }

            if (cardName.empty()) {
                return Err<msgpack::object_handle>("stream_mark_dirty: 'name' is required");
            }

            auto screen = findScreen(cardName);
            if (!screen) {
                return Err<msgpack::object_handle>("stream_mark_dirty: card '" + cardName + "' not found");
            }

            auto slotIndexOpt = screen->getSlotIndexByName(cardName);
            if (!slotIndexOpt) {
                return Err<msgpack::object_handle>("stream_mark_dirty: card '" + cardName + "' not registered");
            }
            uint32_t slotIndex = *slotIndexOpt;

            auto bufMgr = screen->cardManager()->bufferManager();
            if (!bufMgr) {
                return Err<msgpack::object_handle>("stream_mark_dirty: no buffer manager");
            }

            BufferHandle handle = bufMgr->getBufferHandle(slotIndex, scope);
            if (!handle.isValid()) {
                return Err<msgpack::object_handle>("stream_mark_dirty: no buffer for '" + cardName + "' scope '" + scope + "'");
            }

            bufMgr->markBufferDirty(handle);

            ydebug("stream_mark_dirty: card='{}' scope='{}' offset={} size={}",
                  cardName, scope, handle.offset, handle.size);

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);
            pk.pack_map(1);
            pk.pack("ok");
            pk.pack(true);
            return packResult(sbuf);
        });

    // cards_list: {} -> [{slot_index, name, type}, ...]
    server.registerHandler(Channel::CardStream, "cards_list",
        [](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto mgrResult = GPUScreenManager::instance();
            if (!mgrResult) {
                return Err<msgpack::object_handle>("No GPUScreenManager available");
            }

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);

            // Collect cards from all screens
            std::vector<std::tuple<uint32_t, std::string, std::string>> cards;
            for (const auto& screen : (*mgrResult)->screens()) {
                for (auto* card : screen->getAllCards()) {
                    cards.emplace_back(card->metadataSlotIndex(), card->name(), card->typeName());
                }
            }

            pk.pack_array(static_cast<uint32_t>(cards.size()));
            for (const auto& [slot, name, type] : cards) {
                pk.pack_map(3);
                pk.pack("slot_index");
                pk.pack(slot);
                pk.pack("name");
                pk.pack(name);
                pk.pack("type");
                pk.pack(type);
            }

            yinfo("cards_list: {} cards", cards.size());
            return packResult(sbuf);
        });

    // buffers_list: {} -> [{slot_index, card, name, offset, size}, ...]
    // "card" = card name, "name" = buffer name (was "scope")
    server.registerHandler(Channel::CardStream, "buffers_list",
        [](const RpcMessage& /*msg*/) -> Result<msgpack::object_handle> {
            auto screen = getFirstScreen();
            if (!screen) {
                return Err<msgpack::object_handle>("No GPUScreen available");
            }

            auto bufMgr = screen->cardManager()->bufferManager();
            if (!bufMgr) {
                return Err<msgpack::object_handle>("No buffer manager");
            }

            auto buffers = bufMgr->getAllBuffers();

            msgpack::sbuffer sbuf;
            msgpack::packer<msgpack::sbuffer> pk(&sbuf);

            pk.pack_array(static_cast<uint32_t>(buffers.size()));
            for (const auto& buf : buffers) {
                std::string cardName = screen->getNameBySlotIndex(buf.slotIndex);
                pk.pack_map(5);
                pk.pack("slot_index");
                pk.pack(buf.slotIndex);
                pk.pack("card");
                pk.pack(cardName);
                pk.pack("name");
                pk.pack(buf.scope);  // buffer name (internally called scope)
                pk.pack("offset");
                pk.pack(buf.offset);
                pk.pack("size");
                pk.pack(buf.size);
            }

            yinfo("buffers_list: {} buffers", buffers.size());
            return packResult(sbuf);
        });

    yinfo("Registered 5 CardStream handlers on Channel 1");
    return Ok();
}

} // namespace rpc
} // namespace yetty
