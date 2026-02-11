#pragma once

#include <yetty/result.hpp>
#include <yetty/base/factory.h>
#include <memory>
#include <vector>

namespace yetty {

class GPUScreen;

/**
 * GPUScreenManager - Singleton registry for all GPUScreen instances.
 *
 * GPUScreens register/unregister themselves on create/destroy.
 * RPC handlers iterate screens to find cards.
 */
class GPUScreenManager : public base::Singleton<GPUScreenManager> {
public:
    using Ptr = std::shared_ptr<GPUScreenManager>;
    using GPUScreenPtr = std::shared_ptr<GPUScreen>;

    static Result<Ptr> createImpl(ContextType& ctx);

    virtual ~GPUScreenManager() = default;

    virtual void registerScreen(GPUScreenPtr screen) = 0;
    virtual void unregisterScreen(GPUScreen* screen) = 0;  // raw ptr for destructor
    virtual std::vector<GPUScreenPtr> screens() const = 0;

protected:
    GPUScreenManager() = default;
};

}  // namespace yetty
