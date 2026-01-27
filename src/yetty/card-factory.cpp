#include <yetty/card-factory.h>
#include <yetty/yetty-context.h>
#include "cards/texture.h"
#include "cards/plot.h"
#include "cards/ydraw.h"
#include "cards/hdraw.h"
#include <ytrace/ytrace.hpp>
#include <unordered_map>

namespace yetty {

class CardFactoryImpl : public CardFactory {
public:
    CardFactoryImpl(const GPUContext& gpu, CardBufferManager::Ptr cardMgr)
        : _gpu(gpu)
        , _cardMgr(std::move(cardMgr))
    {}

    ~CardFactoryImpl() override = default;

    Result<void> init() {
        // Register built-in card types
        registerCard("texture", [](const YettyContext& ctx,
                                   int32_t x, int32_t y,
                                   uint32_t w, uint32_t h,
                                   const std::string& args,
                                   const std::string& payload) {
            return card::Texture::create(ctx.cardBufferManager, ctx.gpu, x, y, w, h, args, payload);
        });

        registerCard("plot", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) {
            return card::Plot::create(ctx.cardBufferManager, ctx.gpu, x, y, w, h, args, payload);
        });

        registerCard("ydraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::YDraw::create(ctx.cardBufferManager, ctx.gpu, x, y, w, h, args, payload);
        });

        registerCard("hdraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::HDraw::create(ctx, x, y, w, h, args, payload);
        });

        return Ok();
    }

    void registerCard(const std::string& name, CreateFn creator) override {
        if (_creators.find(name) != _creators.end()) {
            ywarn("CardFactory: overwriting existing card type '{}'", name);
        }
        _creators[name] = std::move(creator);
        yinfo("CardFactory: registered card type '{}'", name);
    }

    bool hasCard(const std::string& name) const override {
        return _creators.find(name) != _creators.end();
    }

    std::vector<std::string> getRegisteredCards() const override {
        std::vector<std::string> names;
        names.reserve(_creators.size());
        for (const auto& [name, _] : _creators) {
            names.push_back(name);
        }
        return names;
    }

    Result<CardPtr> createCard(
        const YettyContext& ctx,
        const std::string& name,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) override
    {
        auto it = _creators.find(name);
        if (it == _creators.end()) {
            return Err<CardPtr>("Unknown card type: " + name);
        }

        if (!ctx.cardBufferManager) {
            return Err<CardPtr>("CardFactory: no CardBufferManager in context");
        }

        yinfo("CardFactory: creating card '{}' at ({},{}) size {}x{}",
              name, x, y, widthCells, heightCells);

        // Call the creator function with full context
        auto result = it->second(ctx, x, y, widthCells, heightCells, args, payload);
        if (!result) {
            yerror("CardFactory: failed to create card '{}': {}", name, error_msg(result));
            return result;
        }

        yinfo("CardFactory: created card '{}' with metadataOffset={}",
              name, (*result)->metadataOffset());

        return result;
    }

    CardBufferManager::Ptr cardBufferManager() const override { return _cardMgr; }
    const GPUContext& gpuContext() const override { return _gpu; }

private:
    GPUContext _gpu;
    CardBufferManager::Ptr _cardMgr;
    std::unordered_map<std::string, CreateFn> _creators;
};

// Factory implementation
Result<CardFactory::Ptr> CardFactory::createImpl(
    ContextType& ctx,
    const GPUContext& gpu,
    CardBufferManager::Ptr cardMgr) noexcept
{
    (void)ctx; // ObjectFactory context marker

    auto impl = std::make_shared<CardFactoryImpl>(gpu, std::move(cardMgr));
    if (auto res = impl->init(); !res) {
        return Err<Ptr>("Failed to initialize CardFactory", res);
    }
    return Ok<Ptr>(impl);
}

} // namespace yetty
