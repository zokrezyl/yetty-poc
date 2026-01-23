#include <yetty/widget-frame-renderer.h>
#include <yetty/webgpu-context.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>
#include <cstring>

namespace yetty {

//-----------------------------------------------------------------------------
// WGSL Shader for frame and icon rendering
//-----------------------------------------------------------------------------
static const char* FRAME_SHADER = R"(
struct Uniforms {
    rect: vec4<f32>,       // x, y, w, h in NDC
    color: vec4<f32>,
    screenSize: vec2<f32>,
    thickness: f32,
    iconType: f32,         // 0=frame, 1=close(X), 2=stop(||), 3=play(>), 4=filled
}

@group(0) @binding(0) var<uniform> u: Uniforms;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
}

@vertex fn vs_main(@builtin(vertex_index) vi: u32) -> VertexOutput {
    var positions = array<vec2<f32>, 6>(
        vec2<f32>(0.0, 0.0), vec2<f32>(1.0, 0.0), vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 0.0), vec2<f32>(1.0, 1.0), vec2<f32>(0.0, 1.0)
    );
    let pos = positions[vi];

    var out: VertexOutput;
    out.position = vec4<f32>(
        u.rect.x + pos.x * u.rect.z,
        u.rect.y - pos.y * u.rect.w,
        0.0, 1.0
    );
    out.uv = pos;
    return out;
}

@fragment fn fs_main(@location(0) uv: vec2<f32>) -> @location(0) vec4<f32> {
    let pixelW = u.rect.z * u.screenSize.x * 0.5;
    let pixelH = u.rect.w * u.screenSize.y * 0.5;
    let pixelX = uv.x * pixelW;
    let pixelY = uv.y * pixelH;

    let iconType = i32(u.iconType + 0.5);  // Round to nearest integer

    // Frame rendering (iconType == 0)
    if (iconType == 0) {
        let t = u.thickness;
        let onBorder = pixelX < t || pixelX > pixelW - t ||
                       pixelY < t || pixelY > pixelH - t;
        if (onBorder) {
            return u.color;
        }
        discard;
    }

    // Close button - X (iconType == 1)
    if (iconType == 1) {
        // Draw X shape - two diagonal lines
        let margin = pixelW * 0.2;
        let thickness = pixelW * 0.15;
        let innerW = pixelW - margin * 2.0;
        let innerH = pixelH - margin * 2.0;

        // Local coordinates within margin
        let lx = pixelX - margin;
        let ly = pixelY - margin;

        if (lx >= 0.0 && lx <= innerW && ly >= 0.0 && ly <= innerH) {
            // Normalize to 0-1
            let nx = lx / innerW;
            let ny = ly / innerH;

            // Distance from main diagonal (top-left to bottom-right)
            let d1 = abs(nx - ny) * innerW;
            // Distance from anti-diagonal (top-right to bottom-left)
            let d2 = abs(nx + ny - 1.0) * innerW;

            if (d1 < thickness || d2 < thickness) {
                return u.color;
            }
        }
        discard;
    }

    // Pause button - || (iconType == 2)
    if (iconType == 2) {
        let m = pixelW * 0.25;   // side margin
        let tm = pixelH * 0.2;   // top/bottom margin
        let bw = pixelW * 0.18;  // bar width
        let gap = pixelW * 0.14; // gap between bars

        // Left bar
        if (pixelX >= m && pixelX <= m + bw &&
            pixelY >= tm && pixelY <= pixelH - tm) {
            return u.color;
        }
        // Right bar
        if (pixelX >= m + bw + gap && pixelX <= m + bw + gap + bw &&
            pixelY >= tm && pixelY <= pixelH - tm) {
            return u.color;
        }
        discard;
    }

    // Play button - > (iconType == 3)
    if (iconType == 3) {
        let m = pixelW * 0.3;    // side margin
        let tm = pixelH * 0.2;   // top/bottom margin
        let h = pixelH - tm * 2.0;
        let w = pixelW - m * 2.0;

        // Triangle pointing right
        if (pixelX >= m && pixelY >= tm && pixelY <= pixelH - tm) {
            let cy = pixelH * 0.5;           // center Y
            let dy = abs(pixelY - cy);       // distance from center
            let maxDy = h * 0.5;
            let ratio = 1.0 - dy / maxDy;    // 1 at center, 0 at edges
            let maxX = m + w * ratio;
            if (pixelX <= maxX) {
                return u.color;
            }
        }
        discard;
    }

    // Filled rectangle (iconType == 4)
    if (iconType == 4) {
        return u.color;
    }

    discard;
}
)";

//-----------------------------------------------------------------------------
// Creation and initialization
//-----------------------------------------------------------------------------

Result<std::unique_ptr<WidgetFrameRenderer>> WidgetFrameRenderer::create(
    WGPUDevice device,
    WGPUTextureFormat format
) {
    auto renderer = std::unique_ptr<WidgetFrameRenderer>(new WidgetFrameRenderer());
    if (auto res = renderer->init(device, format); !res) {
        return Err<std::unique_ptr<WidgetFrameRenderer>>("Failed to init WidgetFrameRenderer", res);
    }
    return Ok(std::move(renderer));
}

WidgetFrameRenderer::~WidgetFrameRenderer() {
    for (auto bg : bindGroups_) {
        if (bg) wgpuBindGroupRelease(bg);
    }
    if (bindGroupLayout_) wgpuBindGroupLayoutRelease(bindGroupLayout_);
    for (auto buf : uniformBuffers_) {
        if (buf) wgpuBufferRelease(buf);
    }
    if (framePipeline_) wgpuRenderPipelineRelease(framePipeline_);
}

std::pair<WGPUBuffer, WGPUBindGroup> WidgetFrameRenderer::getNextUniformBuffer() {
    size_t idx = drawIndex_ % MAX_DRAWS_PER_FRAME;
    drawIndex_++;
    return {uniformBuffers_[idx], bindGroups_[idx]};
}

Result<void> WidgetFrameRenderer::init(WGPUDevice device, WGPUTextureFormat format) {
    device_ = device;

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(FRAME_SHADER);
    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);
    if (!shaderModule) {
        return Err<void>("Failed to create frame shader module");
    }

    // Create bind group layout first
    WGPUBindGroupLayoutEntry entry = {};
    entry.binding = 0;
    entry.visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    entry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 1;
    layoutDesc.entries = &entry;
    bindGroupLayout_ = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);
    if (!bindGroupLayout_) {
        wgpuShaderModuleRelease(shaderModule);
        return Err<void>("Failed to create frame bind group layout");
    }

    // Create pool of uniform buffers and bind groups
    uniformBuffers_.resize(MAX_DRAWS_PER_FRAME);
    bindGroups_.resize(MAX_DRAWS_PER_FRAME);

    for (size_t i = 0; i < MAX_DRAWS_PER_FRAME; i++) {
        // Create uniform buffer (48 bytes: rect(16) + color(16) + screenSize(8) + thickness(4) + iconType(4))
        WGPUBufferDescriptor bufDesc = {};
        bufDesc.size = 48;
        bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        uniformBuffers_[i] = wgpuDeviceCreateBuffer(device, &bufDesc);
        if (!uniformBuffers_[i]) {
            wgpuShaderModuleRelease(shaderModule);
            return Err<void>("Failed to create frame uniform buffer");
        }

        // Create bind group for this buffer
        WGPUBindGroupEntry bgEntry = {};
        bgEntry.binding = 0;
        bgEntry.buffer = uniformBuffers_[i];
        bgEntry.size = 48;

        WGPUBindGroupDescriptor bgDesc = {};
        bgDesc.layout = bindGroupLayout_;
        bgDesc.entryCount = 1;
        bgDesc.entries = &bgEntry;
        bindGroups_[i] = wgpuDeviceCreateBindGroup(device, &bgDesc);
        if (!bindGroups_[i]) {
            wgpuShaderModuleRelease(shaderModule);
            return Err<void>("Failed to create frame bind group");
        }
    }
    yinfo("GPU_ALLOC WidgetFrameRenderer: {} uniformBuffers of 48 bytes each = {} bytes total",
          MAX_DRAWS_PER_FRAME, MAX_DRAWS_PER_FRAME * 48);

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &bindGroupLayout_;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);

    // Create render pipeline with alpha blending
    WGPUBlendState blend = {};
    blend.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blend.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blend.color.operation = WGPUBlendOperation_Add;
    blend.alpha.srcFactor = WGPUBlendFactor_One;
    blend.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blend.alpha.operation = WGPUBlendOperation_Add;

    WGPUColorTargetState colorTarget = {};
    colorTarget.format = format;
    colorTarget.blend = &blend;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragment = {};
    fragment.module = shaderModule;
    fragment.entryPoint = WGPU_STR("fs_main");
    fragment.targetCount = 1;
    fragment.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.fragment = &fragment;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    framePipeline_ = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    wgpuPipelineLayoutRelease(pipelineLayout);
    wgpuShaderModuleRelease(shaderModule);

    if (!framePipeline_) {
        return Err<void>("Failed to create frame pipeline");
    }

    initialized_ = true;
    yinfo("WidgetFrameRenderer initialized");
    return Ok();
}

//-----------------------------------------------------------------------------
// Frame rendering
//-----------------------------------------------------------------------------

void WidgetFrameRenderer::renderFrame(
    WGPURenderPassEncoder pass,
    WGPUQueue queue,
    uint32_t screenWidth,
    uint32_t screenHeight,
    float pixelX, float pixelY,
    float pixelW, float pixelH,
    float r, float g, float b, float a,
    float thickness
) {
    if (!initialized_ || !pass) return;

    // Convert pixel coords to NDC
    float ndcX = (pixelX / screenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (pixelY / screenHeight) * 2.0f;
    float ndcW = (pixelW / screenWidth) * 2.0f;
    float ndcH = (pixelH / screenHeight) * 2.0f;

    // Update uniforms
    struct Uniforms {
        float rect[4];
        float color[4];
        float screenSize[2];
        float thickness;
        float iconType;
    } uniforms;

    uniforms.rect[0] = ndcX;
    uniforms.rect[1] = ndcY;
    uniforms.rect[2] = ndcW;
    uniforms.rect[3] = ndcH;
    uniforms.color[0] = r;
    uniforms.color[1] = g;
    uniforms.color[2] = b;
    uniforms.color[3] = a;
    uniforms.screenSize[0] = static_cast<float>(screenWidth);
    uniforms.screenSize[1] = static_cast<float>(screenHeight);
    uniforms.thickness = thickness;
    uniforms.iconType = 0.0f;  // Frame

    auto [buffer, bindGroup] = getNextUniformBuffer();
    wgpuQueueWriteBuffer(queue, buffer, 0, &uniforms, sizeof(uniforms));

    wgpuRenderPassEncoderSetPipeline(pass, framePipeline_);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
}

//-----------------------------------------------------------------------------
// Toolbox rendering
//-----------------------------------------------------------------------------

void WidgetFrameRenderer::renderToolbox(
    WGPURenderPassEncoder pass,
    WGPUQueue queue,
    uint32_t screenWidth,
    uint32_t screenHeight,
    float widgetPixelX, float widgetPixelY,
    float widgetPixelW, float widgetPixelH,
    bool isRunning
) {
    if (!initialized_ || !pass) return;

    const float btnSize = TOOLBOX_BUTTON_SIZE;
    const float padding = TOOLBOX_PADDING;
    const float margin = TOOLBOX_MARGIN;

    // Toolbox position: ABOVE the widget, at the right side
    float toolboxW = btnSize * 2 + padding * 3;
    float toolboxH = btnSize + padding * 2;
    float toolboxX = widgetPixelX + widgetPixelW - toolboxW;
    float toolboxY = widgetPixelY - toolboxH - margin;  // Above the widget

    // Helper to render a filled rectangle (iconType=4)
    auto renderFilled = [&](float x, float y, float w, float h, float r, float g, float b, float a) {
        float ndcX = (x / screenWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (y / screenHeight) * 2.0f;
        float ndcW = (w / screenWidth) * 2.0f;
        float ndcH = (h / screenHeight) * 2.0f;

        struct Uniforms {
            float rect[4];
            float color[4];
            float screenSize[2];
            float thickness;
            float iconType;
        } uniforms;

        uniforms.rect[0] = ndcX;
        uniforms.rect[1] = ndcY;
        uniforms.rect[2] = ndcW;
        uniforms.rect[3] = ndcH;
        uniforms.color[0] = r;
        uniforms.color[1] = g;
        uniforms.color[2] = b;
        uniforms.color[3] = a;
        uniforms.screenSize[0] = static_cast<float>(screenWidth);
        uniforms.screenSize[1] = static_cast<float>(screenHeight);
        uniforms.thickness = 1.0f;
        uniforms.iconType = 4.0f;  // Filled rectangle

        auto [buffer, bindGroup] = getNextUniformBuffer();
        wgpuQueueWriteBuffer(queue, buffer, 0, &uniforms, sizeof(uniforms));
        wgpuRenderPassEncoderSetPipeline(pass, framePipeline_);
        wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
        wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    };

    // Draw toolbox background (filled rectangle)
    renderFilled(toolboxX, toolboxY, toolboxW, toolboxH, 0.1f, 0.1f, 0.15f, 0.9f);

    // Draw a thin border around toolbox
    renderFrame(pass, queue, screenWidth, screenHeight,
                toolboxX, toolboxY, toolboxW, toolboxH,
                0.4f, 0.4f, 0.5f, 0.8f, 1.0f);

    // Helper to render icon button
    auto renderButton = [&](float x, float y, float iconType, float r, float g, float b) {
        float ndcX = (x / screenWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (y / screenHeight) * 2.0f;
        float ndcW = (btnSize / screenWidth) * 2.0f;
        float ndcH = (btnSize / screenHeight) * 2.0f;

        struct Uniforms {
            float rect[4];
            float color[4];
            float screenSize[2];
            float thickness;
            float iconType;
        } uniforms;

        uniforms.rect[0] = ndcX;
        uniforms.rect[1] = ndcY;
        uniforms.rect[2] = ndcW;
        uniforms.rect[3] = ndcH;
        uniforms.color[0] = r;
        uniforms.color[1] = g;
        uniforms.color[2] = b;
        uniforms.color[3] = 1.0f;
        uniforms.screenSize[0] = static_cast<float>(screenWidth);
        uniforms.screenSize[1] = static_cast<float>(screenHeight);
        uniforms.thickness = 1.0f;
        uniforms.iconType = iconType;

        auto [buf, bg] = getNextUniformBuffer();
        wgpuQueueWriteBuffer(queue, buf, 0, &uniforms, sizeof(uniforms));
        wgpuRenderPassEncoderSetPipeline(pass, framePipeline_);
        wgpuRenderPassEncoderSetBindGroup(pass, 0, bg, 0, nullptr);
        wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    };

    // Stop/Start button (left)
    float btn1X = toolboxX + padding;
    float btn1Y = toolboxY + padding;
    if (isRunning) {
        renderButton(btn1X, btn1Y, 2.0f, 1.0f, 0.8f, 0.2f);  // Yellow pause ||
    } else {
        renderButton(btn1X, btn1Y, 3.0f, 0.2f, 1.0f, 0.4f);  // Green play >
    }

    // Close button (right)
    float btn2X = toolboxX + padding + btnSize + padding;
    float btn2Y = toolboxY + padding;
    renderButton(btn2X, btn2Y, 1.0f, 1.0f, 0.3f, 0.3f);  // Red X
}

//-----------------------------------------------------------------------------
// Hit testing
//-----------------------------------------------------------------------------

ToolboxButton WidgetFrameRenderer::hitTestToolbox(
    float clickX, float clickY,
    float widgetPixelX, float widgetPixelY,
    float widgetPixelW, float widgetPixelH
) {
    const float btnSize = TOOLBOX_BUTTON_SIZE;
    const float padding = TOOLBOX_PADDING;
    const float margin = TOOLBOX_MARGIN;

    // Toolbox position: ABOVE the widget, at the right side
    float toolboxW = btnSize * 2 + padding * 3;
    float toolboxH = btnSize + padding * 2;
    float toolboxX = widgetPixelX + widgetPixelW - toolboxW;
    float toolboxY = widgetPixelY - toolboxH - margin;  // Above the widget

    // Check if click is inside toolbox
    if (clickX < toolboxX || clickX > toolboxX + toolboxW ||
        clickY < toolboxY || clickY > toolboxY + toolboxH) {
        return ToolboxButton::None;
    }

    // Check stop/start button
    float btn1X = toolboxX + padding;
    float btn1Y = toolboxY + padding;
    if (clickX >= btn1X && clickX <= btn1X + btnSize &&
        clickY >= btn1Y && clickY <= btn1Y + btnSize) {
        return ToolboxButton::StopStart;
    }

    // Check close button
    float btn2X = toolboxX + padding + btnSize + padding;
    float btn2Y = toolboxY + padding;
    if (clickX >= btn2X && clickX <= btn2X + btnSize &&
        clickY >= btn2Y && clickY <= btn2Y + btnSize) {
        return ToolboxButton::Close;
    }

    return ToolboxButton::None;
}

} // namespace yetty
