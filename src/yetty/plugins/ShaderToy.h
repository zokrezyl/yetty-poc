#pragma once

#include "../Plugin.h"
#include <webgpu/webgpu.h>

namespace yetty {

// ShaderToy plugin - renders custom WGSL fragment shaders
class ShaderToy : public Plugin {
public:
    ShaderToy();
    ~ShaderToy() override;

    // Factory method for plugin creation
    static Result<PluginPtr> create();

    // Plugin interface
    const char* pluginName() const override { return "shader"; }

    Result<void> init(const std::string& payload) override;
    void dispose() override;
    void update(double deltaTime) override;
    void render(WebGPUContext& ctx,
               WGPUTextureView targetView, WGPUTextureFormat targetFormat,
               uint32_t screenWidth, uint32_t screenHeight,
               float pixelX, float pixelY, float pixelW, float pixelH) override;
    void onResize(uint32_t newWidth, uint32_t newHeight) override;

    // Input handling
    bool onMouseMove(float localX, float localY) override;
    bool onMouseButton(int button, bool pressed) override;
    bool onMouseScroll(float xoffset, float yoffset, int mods) override;
    bool wantsMouse() const override { return true; }

private:
    Result<void> compileShader(WebGPUContext& ctx,
                               WGPUTextureFormat targetFormat,
                               const std::string& fragmentCode);
    void renderFocusFrame(WebGPUContext& ctx,
                          WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                          uint32_t screenWidth, uint32_t screenHeight,
                          float pixelX, float pixelY, float pixelW, float pixelH);

    // Vertex shader that positions the quad
    static const char* getVertexShader();
    // Wrapper for user fragment shader
    static std::string wrapFragmentShader(const std::string& userCode);

    // WebGPU resources
    WGPURenderPipeline pipeline_ = nullptr;
    WGPUBindGroup bindGroup_ = nullptr;
    WGPUBuffer uniformBuffer_ = nullptr;
    WGPURenderPipeline framePipeline_ = nullptr;

    float time_ = 0.0f;
    bool compiled_ = false;
    bool failed_ = false;

    // Mouse state (in local coordinates, normalized 0-1)
    float mouseX_ = 0.0f;
    float mouseY_ = 0.0f;
    bool mouseDown_ = false;
    bool mouseGrabbed_ = false;  // True while mouse button held

    // Scroll-controlled parameters (exposed to shader)
    float param_ = 0.5f;   // wheel scroll
    float zoom_ = 1.0f;    // ctrl+wheel zoom

    // Frame pipeline for visual feedback
    bool frameCompiled_ = false;
    WGPURenderPipeline framePipeline2_ = nullptr;
    WGPUBindGroup frameBindGroup_ = nullptr;
    WGPUBuffer frameUniformBuffer_ = nullptr;

    Result<void> compileFramePipeline(WebGPUContext& ctx, WGPUTextureFormat targetFormat);
    void renderFrame(WebGPUContext& ctx, WGPUTextureView targetView,
                     uint32_t screenWidth, uint32_t screenHeight,
                     float pixelX, float pixelY, float pixelW, float pixelH);
};

} // namespace yetty

// C exports for dynamic loading (when compiled as separate .so)
extern "C" {
    const char* shader_plugin_name();
    yetty::Result<yetty::PluginPtr> shader_plugin_create();
}
