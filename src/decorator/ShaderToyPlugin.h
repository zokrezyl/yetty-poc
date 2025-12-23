#pragma once

#include "DecoratorPlugin.h"
#include <webgpu/webgpu.h>

namespace yetty {

// Plugin-specific data stored in Decorator
struct ShaderToyData {
    WGPURenderPipeline pipeline = nullptr;
    WGPUBindGroup bindGroup = nullptr;
    WGPUBuffer uniformBuffer = nullptr;
    float time = 0.0f;
    bool compiled = false;
    bool failed = false;  // Set to true if compilation failed
};

// ShaderToy plugin - renders custom WGSL fragment shaders
class ShaderToyPlugin : public DecoratorPlugin {
public:
    ShaderToyPlugin();
    ~ShaderToyPlugin() override;

    const char* getId() const override { return "shader"; }
    const char* getName() const override { return "ShaderToy"; }
    const char* getDescription() const override {
        return "Renders custom WGSL fragment shaders in terminal cells";
    }

    bool initialize(Decorator* decorator, const std::string& payload) override;
    void update(Decorator* decorator, double deltaTime) override;
    void render(Decorator* decorator, WebGPUContext& ctx,
               WGPUTextureView targetView, WGPUTextureFormat targetFormat,
               uint32_t screenWidth, uint32_t screenHeight,
               float pixelX, float pixelY, float pixelW, float pixelH) override;
    void cleanup(Decorator* decorator) override;
    void onResize(Decorator* decorator, uint32_t newWidth, uint32_t newHeight) override;

private:
    bool compileShader(Decorator* decorator, WebGPUContext& ctx,
                      WGPUTextureFormat targetFormat, const std::string& fragmentCode);

    // Vertex shader that positions the quad
    static const char* getVertexShader();
    // Wrapper for user fragment shader
    static std::string wrapFragmentShader(const std::string& userCode);
};

} // namespace yetty
