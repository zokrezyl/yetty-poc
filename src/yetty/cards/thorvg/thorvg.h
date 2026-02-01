#pragma once

#include <cstdint>
#include <string>
#include <yetty/base/factory.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card-texture-manager.h>
#include <yetty/card.h>
#include <yetty/gpu-context.h>

namespace yetty {
struct YettyContext;
}

namespace yetty::card {

class ThorVG : public TextureCard, public base::ObjectFactory<ThorVG> {
public:
  using Ptr = std::shared_ptr<ThorVG>;

  // Reuse the texture card shader glyph - ThorVG renders pixels just like Image
  static constexpr uint32_t SHADER_GLYPH = 0x100000;

  static Result<Ptr> createImpl(ContextType &ctx, const YettyContext &yettyCtx,
                                int32_t x, int32_t y, uint32_t widthCells,
                                uint32_t heightCells, const std::string &args,
                                const std::string &payload) noexcept;

  virtual ~ThorVG() = default;

protected:
  ThorVG(CardBufferManager::Ptr bufMgr, CardTextureManager::Ptr texMgr,
         const GPUContext &gpu, int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
      : TextureCard(std::move(bufMgr), std::move(texMgr), gpu, x, y, widthCells, heightCells) {}
};

} // namespace yetty::card
