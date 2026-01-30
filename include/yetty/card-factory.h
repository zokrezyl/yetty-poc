#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/gpu-context.h>
#include <yetty/result.hpp>
#include <functional>
#include <string>
#include <vector>

namespace yetty {

struct YettyContext;  // Forward declaration

/**
 * CardFactory - Creates card instances by type name
 *
 * Card types register themselves with a creator function.
 * GPUScreen uses this to create cards from OSC commands.
 *
 * Created via ObjectFactory with GPUContext.
 * Stored in YettyContext for access by GPUScreen.
 */
class CardFactory : public base::ObjectFactory<CardFactory> {
public:
    using Ptr = std::shared_ptr<CardFactory>;

    // Creator function signature - receives full YettyContext
    using CreateFn = std::function<Result<CardPtr>(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload
    )>;

    // Factory method
    static Result<Ptr> createImpl(
        ContextType& ctx,
        const GPUContext& gpu) noexcept;

    virtual ~CardFactory() = default;

    //=========================================================================
    // Registration
    //=========================================================================

    // Register a card type with its creator function
    virtual void registerCard(const std::string& name, CreateFn creator) = 0;

    // Check if a card type is registered
    virtual bool hasCard(const std::string& name) const = 0;

    // Get list of registered card type names
    virtual std::vector<std::string> getRegisteredCards() const = 0;

    //=========================================================================
    // Creation
    //=========================================================================

    // Create a card instance
    virtual Result<CardPtr> createCard(
        const YettyContext& ctx,
        const std::string& name,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args = "",
        const std::string& payload = "") = 0;

    //=========================================================================
    // Accessors
    //=========================================================================
    virtual const GPUContext& gpuContext() const = 0;

protected:
    CardFactory() = default;
};

} // namespace yetty
