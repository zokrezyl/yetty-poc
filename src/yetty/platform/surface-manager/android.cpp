#include <yetty/platform/surface-manager.h>

#if defined(__ANDROID__)
#include "../shared/android-app-singleton.h"
#include <yetty/base/event-loop.h>
#include <yetty/base/event.h>
#include <webgpu/webgpu.h>
#include <android/native_window.h>

namespace yetty {

class AndroidSurfaceManager : public SurfaceManager {
public:
    Result<void> init() {
        auto appResult = AndroidAppSingleton::instance();
        if (!appResult) {
            return Err<void>("AndroidAppSingleton not available", appResult);
        }
        _appSingleton = *appResult;
        return Ok();
    }

    void getWindowSize(int& width, int& height) const override {
        ANativeWindow* window = _appSingleton->getWindow();
        if (window) {
            width = ANativeWindow_getWidth(window);
            height = ANativeWindow_getHeight(window);
        } else {
            width = height = 0;
        }
    }

    void getFramebufferSize(int& width, int& height) const override {
        // On Android, framebuffer size equals window size
        getWindowSize(width, height);
    }

    void getContentScale(float& xscale, float& yscale) const override {
        // TODO: Get actual density from Android
        xscale = yscale = 1.0f;
    }

    bool shouldClose() const override {
        return _shouldClose;
    }

    void setTitle(const std::string& title) override {
        (void)title; // No-op on Android
    }

    void setIcon(const unsigned char* data, size_t size) override {
        (void)data; (void)size; // App icon set via manifest
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        ANativeWindow* window = _appSingleton->getWindow();
        if (!window) return nullptr;

        WGPUSurfaceSourceAndroidNativeWindow androidSource = {};
        androidSource.chain.sType = WGPUSType_SurfaceSourceAndroidNativeWindow;
        androidSource.window = window;

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = &androidSource.chain;

        return wgpuInstanceCreateSurface(instance, &surfaceDesc);
    }

    void setShouldClose(bool close) { _shouldClose = close; }

private:
    AndroidAppSingleton::Ptr _appSingleton;
    bool _shouldClose = false;
};

// Factory
Result<SurfaceManager::Ptr> SurfaceManager::createImpl() {
    auto mgr = new AndroidSurfaceManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("AndroidSurfaceManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __ANDROID__
