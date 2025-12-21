#pragma once

#include "WebGPUContext.h"
#include "terminal/Grid.h"
#include "terminal/Font.h"
#include <glm/glm.hpp>
#include <vector>

namespace yetty {

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool init(WebGPUContext& ctx, Font& font);
    void resize(uint32_t width, uint32_t height);
    void render(WebGPUContext& ctx, const Grid& grid);

    void setCellSize(float width, float height);
    glm::vec2 getCellSize() const { return cellSize_; }

private:
    bool createShaderModule(WGPUDevice device);
    bool createPipelines(WGPUDevice device, WGPUTextureFormat format);
    bool createBuffers(WGPUDevice device);
    bool createBindGroup(WGPUDevice device, Font& font);

    void updateUniformBuffer(WGPUQueue queue);
    void updateVertexBuffers(WGPUQueue queue, const Grid& grid, Font& font);

    struct Uniforms {
        glm::mat4 projection;     // 64 bytes, offset 0
        glm::vec2 screenSize;     // 8 bytes, offset 64
        glm::vec2 cellSize;       // 8 bytes, offset 72
        float pixelRange;         // 4 bytes, offset 80
        float _pad1;              // 4 bytes, offset 84
        float _pad2;              // 4 bytes, offset 88
        float _pad3;              // 4 bytes, offset 92
    };  // Total: 96 bytes

    struct GlyphVertex {
        glm::vec2 position;      // Quad corner
        glm::vec2 cellPos;       // Grid position
        glm::vec2 uvMin;         // UV min
        glm::vec2 uvMax;         // UV max
        glm::vec2 glyphSize;     // Glyph size
        glm::vec2 glyphBearing;  // Glyph bearing
        glm::vec4 fgColor;       // Foreground
        glm::vec4 bgColor;       // Background
    };

    struct BgVertex {
        glm::vec2 position;
        glm::vec2 cellPos;
        glm::vec4 bgColor;
    };

    WGPUShaderModule shaderModule_ = nullptr;
    WGPURenderPipeline glyphPipeline_ = nullptr;
    WGPURenderPipeline bgPipeline_ = nullptr;
    WGPUBindGroupLayout bindGroupLayout_ = nullptr;
    WGPUBindGroup bindGroup_ = nullptr;
    WGPUPipelineLayout pipelineLayout_ = nullptr;

    WGPUBuffer uniformBuffer_ = nullptr;
    WGPUBuffer glyphVertexBuffer_ = nullptr;
    WGPUBuffer glyphIndexBuffer_ = nullptr;
    WGPUBuffer bgVertexBuffer_ = nullptr;
    WGPUBuffer bgIndexBuffer_ = nullptr;

    Uniforms uniforms_;
    glm::vec2 cellSize_ = {10.0f, 20.0f};
    uint32_t screenWidth_ = 800;
    uint32_t screenHeight_ = 600;

    uint32_t maxCells_ = 0;
    uint32_t glyphCount_ = 0;
    uint32_t bgCount_ = 0;

    Font* font_ = nullptr;
};

} // namespace yetty
