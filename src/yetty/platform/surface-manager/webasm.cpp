#include <yetty/platform/surface-manager.h>

#if defined(__EMSCRIPTEN__)

#include <yetty/result.hpp>
#include <emscripten/html5.h>
#include <webgpu/webgpu.h>

namespace yetty {

class WebSurfaceManager : public SurfaceManager {
public:
    Result<void> init() {
        // Get initial canvas size
        emscripten_get_canvas_element_size("#canvas", &_width, &_height);
        return Ok();
    }

    void getWindowSize(int& width, int& height) const override {
        width = _width;
        height = _height;
    }

    void getFramebufferSize(int& width, int& height) const override {
        width = _width;
        height = _height;
    }

    void getContentScale(float& xscale, float& yscale) const override {
        double ratio = emscripten_get_device_pixel_ratio();
        xscale = yscale = static_cast<float>(ratio);
    }

    bool shouldClose() const override {
        return false;  // Web apps don't close
    }

    void setTitle(const std::string& title) override {
        EM_ASM({ document.title = UTF8ToString($0); }, title.c_str());
    }

    void setIcon(const unsigned char* data, size_t size) override {
        (void)data; (void)size;  // Favicon set via HTML
    }

    void setCursor(CursorType type) override {
        const char* cursor = "default";
        switch (type) {
            case CursorType::Arrow: cursor = "default"; break;
            case CursorType::IBeam: cursor = "text"; break;
            case CursorType::Hand: cursor = "pointer"; break;
            case CursorType::ResizeH: cursor = "ew-resize"; break;
            case CursorType::ResizeV: cursor = "ns-resize"; break;
            case CursorType::Hidden: cursor = "none"; break;
        }
        EM_ASM({ document.body.style.cursor = UTF8ToString($0); }, cursor);
    }

    double getTime() const override {
        return emscripten_get_now() / 1000.0;
    }

    void requestRender() override {
        // Browser handles this via requestAnimationFrame
    }

    WGPUSurface createWGPUSurface(WGPUInstance instance) override {
        WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource = {};
        canvasSource.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
        canvasSource.selector = { .data = "#canvas", .length = 7 };

        WGPUSurfaceDescriptor surfaceDesc = {};
        surfaceDesc.nextInChain = &canvasSource.chain;

        return wgpuInstanceCreateSurface(instance, &surfaceDesc);
    }

    // Called by InitManager resize callback to update dimensions
    void updateSize(int width, int height) {
        _width = width;
        _height = height;
    }

private:
    int _width = 800;
    int _height = 600;
};

// Factory
Result<SurfaceManager::Ptr> SurfaceManager::createImpl() {
    auto mgr = new WebSurfaceManager();
    if (auto res = mgr->init(); !res) {
        delete mgr;
        return Err<Ptr>("WebSurfaceManager init failed", res);
    }
    return Ok(Ptr(mgr));
}

} // namespace yetty

#endif // __EMSCRIPTEN__
