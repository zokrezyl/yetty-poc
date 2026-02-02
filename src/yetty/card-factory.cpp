#include <yetty/card-factory.h>
#include <yetty/yetty-context.h>
#include "cards/image/image.h"
#include "cards/plot/plot.h"
#include "cards/hdraw/hdraw.h"
#include "cards/ydraw/ydraw.h"
#include "cards/kdraw/kdraw.h"
#include "cards/jdraw/jdraw.h"
#include "cards/ymery/ymery.h"
#include "cards/thorvg/thorvg.h"
#ifdef YETTY_CARD_PDF
#include "cards/pdf/pdf.h"
#endif
#ifdef YETTY_CARD_PYTHON
#include "cards/python/python.h"
#endif
#include <ytrace/ytrace.hpp>
#include <unordered_map>

namespace yetty {

class CardFactoryImpl : public CardFactory {
public:
    explicit CardFactoryImpl(const GPUContext& gpu)
        : _gpu(gpu)
    {}

    ~CardFactoryImpl() override = default;

    Result<void> init() {
        // Register built-in card types
        // "texture" is the canonical name, "image" is an alias for backward compatibility
        registerCard("image", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) -> Result<CardPtr> {
            auto result = card::Image::create(ctx, x, y, w, h, args, payload);
            if (!result) return std::unexpected(result.error());
            return Ok<CardPtr>(*result);
        });

        registerCard("plot", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) {
            return card::Plot::create(ctx, x, y, w, h, args, payload);
        });

        registerCard("hdraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::HDraw::create(ctx.cardManager, ctx.gpu, x, y, w, h, args, payload);
        });

        registerCard("ydraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::YDraw::create(ctx, x, y, w, h, args, payload);
        });

        registerCard("kdraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::KDraw::create(ctx, x, y, w, h, args, payload);
        });

        registerCard("jdraw", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) {
            return card::JDraw::create(ctx, x, y, w, h, args, payload);
        });

        registerCard("ymery", [](const YettyContext& ctx,
                                 int32_t x, int32_t y,
                                 uint32_t w, uint32_t h,
                                 const std::string& args,
                                 const std::string& payload) -> Result<CardPtr> {
            auto result = card::Ymery::create(ctx, x, y, w, h, args, payload);
            if (!result) return std::unexpected(result.error());
            return Ok<CardPtr>(*result);
        });

        registerCard("thorvg", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) -> Result<CardPtr> {
            auto result = card::ThorVG::create(ctx, x, y, w, h, args, payload);
            if (!result) return std::unexpected(result.error());
            return Ok<CardPtr>(*result);
        });

#ifdef YETTY_CARD_PDF
        registerCard("pdf", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) -> Result<CardPtr> {
            auto result = card::Pdf::create(ctx, x, y, w, h, args, payload);
            if (!result) return std::unexpected(result.error());
            return Ok<CardPtr>(*result);
        });
#endif

#ifdef YETTY_CARD_PYTHON
        registerCard("python", [](const YettyContext& ctx,
                                int32_t x, int32_t y,
                                uint32_t w, uint32_t h,
                                const std::string& args,
                                const std::string& payload) -> Result<CardPtr> {
            auto result = card::PythonCard::create(ctx, x, y, w, h, args, payload);
            if (!result) return std::unexpected(result.error());
            return Ok<CardPtr>(*result);
        });
#endif

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

        if (!ctx.cardManager) {
            return Err<CardPtr>("CardFactory: no CardManager in context");
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

    const GPUContext& gpuContext() const override { return _gpu; }

private:
    GPUContext _gpu;
    std::unordered_map<std::string, CreateFn> _creators;
};

// Factory implementation
Result<CardFactory::Ptr> CardFactory::createImpl(
    ContextType& ctx,
    const GPUContext& gpu) noexcept
{
    (void)ctx; // ObjectFactory context marker

    auto impl = std::make_shared<CardFactoryImpl>(gpu);
    if (auto res = impl->init(); !res) {
        return Err<Ptr>("Failed to initialize CardFactory", res);
    }
    return Ok<Ptr>(impl);
}

} // namespace yetty
