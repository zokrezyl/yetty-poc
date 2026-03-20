#include <yetty/platform/surface-manager.h>

#if YETTY_IOS

#include "../shared/ios-app-singleton.h"
#include <ytrace/ytrace.hpp>
#include <webgpu/webgpu.h>
#include <chrono>

#import <QuartzCore/CAMetalLayer.h>

namespace yetty {

class IosSurfaceManager : public SurfaceManager {
public:
    Result<void> init() {
        auto singletonResult = IosAppSingleton::instance();
        if (!singletonResult) {
            return Err<void>("IosAppSingleton not available", singletonResult);
        }
        _appSingleton = *singletonResult;
        _startTime = std::chrono::steady_clock::now();

        ydebug("IosSurfaceManager initialized");
        return Ok();
    }

    void getWindowSize(int& width, int& height) const override {
        _appSingleton->getDrawableSize(width, height);
    }

    void getFramebufferSize(int& width, int& height) const override {
        getWindowSize(width, height);
    }

    void getContentScale(float& xscale, float& yscale) const override {
        float scale = _appSingleton->getContentScale();
        xscale = yscale = scale;
    }

    bool shouldClose() const override {
        return _shouldClose;
    }

    void setTitle(const std::string& title) override {
        (void)title;  // iOS apps don't have window titles
    }

    void setIcon(const unsigned char* data, size_t size) override {
        (void)data; (void)size;  // iOS app icons are in bundle
    }

    void setCursor(CursorType type) override {
        (void)type;  // No cursor on iOS
    }

    double getTime() const override {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(now - _startTime).count();
    }

    void requestRender() override {
        // CADisplayLink handles rendering - nothing to do
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        CAMetalLayer* metalLayer = _appSingleton->getMetalLayer();
        if (!metalLayer) {
            yerror("iOS: Cannot create surface - no Metal layer");
            return nullptr;
        }

        WGPUSurfaceSourceMetalLayer metalSource = {};
        metalSource.chain.sType = WGPUSType_SurfaceSourceMetalLayer;
        metalSource.layer = (__bridge void*)metalLayer;

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = &metalSource.chain;

        WGPUSurface surface = wgpuInstanceCreateSurface(instance, &surfaceDesc);
        if (surface) {
            yinfo("iOS: Created WebGPU surface from Metal layer");
        } else {
            yerror("iOS: Failed to create WebGPU surface");
        }
        return surface;
    }

    void setShouldClose(bool close) { _shouldClose = close; }

private:
    IosAppSingleton::Ptr _appSingleton;
    bool _shouldClose = false;
    std::chrono::steady_clock::time_point _startTime;
};

// Factory
Result<SurfaceManager::Ptr> SurfaceManager::createImpl() {
    auto mgr = new IosSurfaceManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("IosSurfaceManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // YETTY_IOS
