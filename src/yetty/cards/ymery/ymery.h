#pragma once

#include <cstdint>
#include <string>
#include <yetty/base/factory.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card.h>
#include <yetty/gpu-context.h>

namespace yetty {
struct YettyContext;
}

namespace yetty::card {

class Ymery : public TextureCard, public base::ObjectFactory<Ymery> {
public:
    using Ptr = std::shared_ptr<Ymery>;
    static constexpr uint32_t SHADER_GLYPH = 0x100000; // reuse texture shader

    static Result<Ptr> createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                  int32_t x, int32_t y,
                                  uint32_t widthCells, uint32_t heightCells,
                                  const std::string& args,
                                  const std::string& payload) noexcept;

    virtual ~Ymery() = default;

protected:
    Ymery(CardBufferManager::Ptr mgr, const GPUContext& gpu,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells)
        : TextureCard(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

} // namespace yetty::card
