#include "card-factory.h"
#include <ytrace/ytrace.hpp>

namespace yetty {

CardFactory::CardFactory(CardBufferManager* cardMgr)
    : cardMgr_(cardMgr)
{
}

void CardFactory::registerCard(const std::string& name, CreateFn creator) {
    if (creators_.find(name) != creators_.end()) {
        ywarn("CardFactory: overwriting existing card type '{}'", name);
    }
    creators_[name] = std::move(creator);
    yinfo("CardFactory: registered card type '{}'", name);
}

bool CardFactory::hasCard(const std::string& name) const {
    return creators_.find(name) != creators_.end();
}

std::vector<std::string> CardFactory::getRegisteredCards() const {
    std::vector<std::string> names;
    names.reserve(creators_.size());
    for (const auto& [name, _] : creators_) {
        names.push_back(name);
    }
    return names;
}

Result<CardPtr> CardFactory::createCard(
    const std::string& name,
    int32_t x, int32_t y,
    uint32_t widthCells, uint32_t heightCells,
    const std::string& args,
    const std::string& payload)
{
    auto it = creators_.find(name);
    if (it == creators_.end()) {
        return Err<CardPtr>("Unknown card type: " + name);
    }

    if (!cardMgr_) {
        return Err<CardPtr>("CardFactory: no CardBufferManager");
    }

    yinfo("CardFactory: creating card '{}' at ({},{}) size {}x{}",
          name, x, y, widthCells, heightCells);

    // Call the creator function - it constructs the card and calls init()
    auto result = it->second(cardMgr_, x, y, widthCells, heightCells, args, payload);
    if (!result) {
        yerror("CardFactory: failed to create card '{}': {}", name, error_msg(result));
        return result;
    }

    yinfo("CardFactory: created card '{}' with metadataOffset={}",
          name, (*result)->metadataOffset());

    return result;
}

} // namespace yetty
