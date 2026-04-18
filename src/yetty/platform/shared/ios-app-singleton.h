#pragma once

#include <yetty/base/factory.h>
#include <atomic>
#include <string>

#if YETTY_IOS

#ifdef __OBJC__
@class CAMetalLayer;
#else
typedef void CAMetalLayer;
#endif

namespace yetty {

// IosAppSingleton - owns iOS app state
//
// Internal glue for iOS platform. All iOS managers access
// platform state through this singleton.
//
// Thread-safe: uses atomics for cross-thread access (main thread sets,
// render thread reads).
//
class IosAppSingleton : public base::Singleton<IosAppSingleton> {
public:
    virtual ~IosAppSingleton() = default;

    // Called from main thread
    void setMetalLayer(CAMetalLayer* layer);
    void setContentScale(float scale);
    void setDrawableSize(int width, int height);

    // Called from any thread
    CAMetalLayer* getMetalLayer() const;
    float getContentScale() const;
    void getDrawableSize(int& width, int& height) const;

    // iOS paths
    std::string getCachesDir() const;
    std::string getBundleResourcePath() const;

    static Result<Ptr> createImpl();

protected:
    IosAppSingleton() = default;

private:
    std::atomic<CAMetalLayer*> _metalLayer{nullptr};
    std::atomic<float> _contentScale{1.0f};
    std::atomic<int> _width{0};
    std::atomic<int> _height{0};
};

} // namespace yetty

#endif // YETTY_IOS
