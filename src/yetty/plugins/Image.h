#pragma once

#include "../Plugin.h"
#include <webgpu/webgpu.h>

namespace yetty {

// Image plugin - renders images (PNG, JPG, etc.) using stb_image
class Image : public Plugin {
public:
    Image();
    ~Image() override;

    // Factory method for plugin creation
    static Result<PluginPtr> create();

    // Plugin interface
    const char* pluginName() const override { return "image"; }

    Result<void> init(const std::string& payload) override;
    void dispose() override;
    void update(double deltaTime) override;
    void render(WebGPUContext& ctx,
               WGPUTextureView targetView, WGPUTextureFormat targetFormat,
               uint32_t screenWidth, uint32_t screenHeight,
               float pixelX, float pixelY, float pixelW, float pixelH) override;
    void onResize(uint32_t newWidth, uint32_t newHeight) override;

private:
    Result<void> loadImage(const std::string& data);
    Result<void> createPipeline(WebGPUContext& ctx, WGPUTextureFormat targetFormat);

    // Image data
    unsigned char* imageData_ = nullptr;
    int imageWidth_ = 0;
    int imageHeight_ = 0;
    int imageChannels_ = 0;

    // WebGPU resources
    WGPURenderPipeline pipeline_ = nullptr;
    WGPUBindGroup bindGroup_ = nullptr;
    WGPUBuffer uniformBuffer_ = nullptr;
    WGPUTexture texture_ = nullptr;
    WGPUTextureView textureView_ = nullptr;
    WGPUSampler sampler_ = nullptr;

    bool initialized_ = false;
    bool failed_ = false;
};

} // namespace yetty

// C exports for dynamic loading (when compiled as separate .so)
extern "C" {
    const char* image_plugin_name();
    yetty::Result<yetty::PluginPtr> image_plugin_create();
}
