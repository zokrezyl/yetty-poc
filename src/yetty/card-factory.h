#pragma once

#include "card.h"
#include "card-buffer-manager.h"
#include <yetty/result.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace yetty {

//=============================================================================
// CardFactory - Creates card instances by type name
//
// Card types register themselves with a creator function.
// GPUScreen uses this to create cards from OSC commands.
//=============================================================================

class CardFactory {
public:
    // Creator function signature
    // Called by CardFactory::createCard after parsing OSC arguments
    using CreateFn = std::function<Result<CardPtr>(
        CardBufferManager* mgr,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload
    )>;

    explicit CardFactory(CardBufferManager* cardMgr);
    ~CardFactory() = default;

    // Non-copyable
    CardFactory(const CardFactory&) = delete;
    CardFactory& operator=(const CardFactory&) = delete;

    //=========================================================================
    // Registration
    //=========================================================================

    // Register a card type with its creator function
    void registerCard(const std::string& name, CreateFn creator);

    // Check if a card type is registered
    bool hasCard(const std::string& name) const;

    // Get list of registered card type names
    std::vector<std::string> getRegisteredCards() const;

    //=========================================================================
    // Creation
    //=========================================================================

    // Create a card instance
    // - Calls the registered creator function
    // - Creator constructs the card and calls init()
    Result<CardPtr> createCard(
        const std::string& name,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args = "",
        const std::string& payload = ""
    );

    //=========================================================================
    // Accessors
    //=========================================================================
    CardBufferManager* cardBufferManager() const { return cardMgr_; }

private:
    CardBufferManager* cardMgr_;
    std::unordered_map<std::string, CreateFn> creators_;
};

//=============================================================================
// Helper macro for card registration
//=============================================================================

// Use in card implementation file:
//   REGISTER_CARD(PlotCard, "plot")
//
// The card class must have a static create() method with signature:
//   static Result<CardPtr> create(CardBufferManager*, int32_t, int32_t,
//                                  uint32_t, uint32_t, const std::string&,
//                                  const std::string&);

#define REGISTER_CARD(CardClass, name) \
    static bool _registered_##CardClass = []() { \
        /* Registration happens when CardFactory is constructed */ \
        return true; \
    }()

} // namespace yetty
