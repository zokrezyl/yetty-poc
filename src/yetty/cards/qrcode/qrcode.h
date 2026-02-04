#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <yetty/base/factory.h>
#include <yetty/card.h>
#include <yetty/gpu-context.h>

namespace yetty {
struct YettyContext;
}

namespace yetty::card {

// QR Code error correction levels
enum class QRErrorLevel : uint8_t {
    L = 0,  // 7% recovery
    M = 1,  // 15% recovery
    Q = 2,  // 25% recovery
    H = 3   // 30% recovery
};

class QRCode : public Card, public base::ObjectFactory<QRCode> {
public:
    using Ptr = std::shared_ptr<QRCode>;
    static constexpr uint32_t SHADER_GLYPH = 0x100007;  // Next available after ytext (0x100006)

    static Result<Ptr> createImpl(ContextType& ctx, const YettyContext& yettyCtx,
                                  int32_t x, int32_t y, uint32_t widthCells,
                                  uint32_t heightCells, const std::string& args,
                                  const std::string& payload) noexcept;

    bool needsBuffer() const override { return true; }

    virtual ~QRCode() = default;

protected:
    QRCode(CardManager::Ptr mgr, const GPUContext& gpu, int32_t x, int32_t y,
           uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells) {}
};

} // namespace yetty::card
