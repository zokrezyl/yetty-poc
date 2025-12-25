#include "ShaderToy.h"
#include "../renderer/WebGPUContext.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <sstream>

namespace yetty {

ShaderToy::ShaderToy() {
}

ShaderToy::~ShaderToy() {
    dispose();
}

Result<PluginPtr> ShaderToy::create() {
    return Ok<PluginPtr>(std::make_shared<ShaderToy>());
}

Result<void> ShaderToy::init(const std::string& payload) {
    if (payload.empty()) {
        return Err<void>("ShaderToy: empty payload");
    }

    // Reset state for re-initialization
    compiled_ = false;
    failed_ = false;
    time_ = 0.0f;

    std::cout << "ShaderToy: initialized with " << payload.size() << " bytes of shader code" << std::endl;
    std::cout << "ShaderToy: payload content:\n---\n" << payload << "\n---" << std::endl;
    return Ok();
}

void ShaderToy::dispose() {
    if (bindGroup_) {
        wgpuBindGroupRelease(bindGroup_);
        bindGroup_ = nullptr;
    }
    if (pipeline_) {
        wgpuRenderPipelineRelease(pipeline_);
        pipeline_ = nullptr;
    }
    if (uniformBuffer_) {
        wgpuBufferRelease(uniformBuffer_);
        uniformBuffer_ = nullptr;
    }
    compiled_ = false;
}

void ShaderToy::update(double deltaTime) {
    time_ += static_cast<float>(deltaTime);
}

void ShaderToy::render(WebGPUContext& ctx,
                       WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                       uint32_t screenWidth, uint32_t screenHeight,
                       float pixelX, float pixelY, float pixelW, float pixelH) {
    // Skip if previously failed
    if (failed_) return;

    // First time: compile shader
    if (!compiled_) {
        auto result = compileShader(ctx, targetFormat, payload_);
        if (!result) {
            std::cerr << "ShaderToy: " << error_msg(result) << std::endl;
            failed_ = true;
            return;
        }
        compiled_ = true;
    }

    if (!pipeline_ || !uniformBuffer_ || !bindGroup_) {
        failed_ = true;
        return;
    }

    // Update uniform buffer
    // rect: normalized device coordinates for the quad
    // Convert pixel coords to NDC: x: [-1, 1], y: [-1, 1] (y flipped)
    float ndcX = (pixelX / screenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (pixelY / screenHeight) * 2.0f;  // Flip Y
    float ndcW = (pixelW / screenWidth) * 2.0f;
    float ndcH = (pixelH / screenHeight) * 2.0f;

    struct Uniforms {
        float time;
        float param;
        float zoom;
        float _pad1;
        float resolution[2];
        float _pad2[2];
        float rect[4];     // x, y, w, h in NDC
        float mouse[4];    // x, y (normalized 0-1), grabbed, buttonDown
    } uniforms;

    uniforms.time = time_;
    uniforms.param = param_;
    uniforms.zoom = zoom_;
    uniforms._pad1 = 0.0f;
    uniforms.resolution[0] = pixelW;
    uniforms.resolution[1] = pixelH;
    uniforms._pad2[0] = 0.0f;
    uniforms._pad2[1] = 0.0f;
    uniforms.rect[0] = ndcX;
    uniforms.rect[1] = ndcY;
    uniforms.rect[2] = ndcW;
    uniforms.rect[3] = ndcH;
    uniforms.mouse[0] = mouseX_;
    uniforms.mouse[1] = mouseY_;
    uniforms.mouse[2] = mouseGrabbed_ ? 1.0f : 0.0f;  // Grabbed state (click held)
    uniforms.mouse[3] = mouseDown_ ? 1.0f : 0.0f;     // Current button state

    wgpuQueueWriteBuffer(ctx.getQueue(), uniformBuffer_, 0, &uniforms, sizeof(uniforms));

    // Create command encoder
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ctx.getDevice(), &encoderDesc);
    if (!encoder) {
        std::cerr << "ShaderToy: failed to create command encoder" << std::endl;
        return;
    }

    // Render pass - load existing content, don't clear
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Load;  // Preserve terminal content
    colorAttachment.storeOp = WGPUStoreOp_Store;

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (!pass) {
        std::cerr << "ShaderToy: failed to begin render pass" << std::endl;
        wgpuCommandEncoderRelease(encoder);
        return;
    }

    wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup_, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);  // Quad (2 triangles)
    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    if (cmdBuffer) {
        wgpuQueueSubmit(ctx.getQueue(), 1, &cmdBuffer);
        wgpuCommandBufferRelease(cmdBuffer);
    }
    wgpuCommandEncoderRelease(encoder);
}

void ShaderToy::onResize(uint32_t newWidth, uint32_t newHeight) {
    Plugin::onResize(newWidth, newHeight);
    // Could trigger re-render if needed
}

bool ShaderToy::onMouseMove(float localX, float localY) {
    // Normalize to 0-1 range based on pixel size
    mouseX_ = localX / static_cast<float>(pixelWidth_);
    mouseY_ = localY / static_cast<float>(pixelHeight_);
    spdlog::debug("ShaderToy::onMouseMove: local=({},{}) normalized=({},{})",
                  localX, localY, mouseX_, mouseY_);
    return true;
}

bool ShaderToy::onMouseButton(int button, bool pressed) {
    if (button == 0) {  // Left button
        mouseDown_ = pressed;
        if (pressed) {
            mouseGrabbed_ = true;
        } else {
            mouseGrabbed_ = false;
        }
        spdlog::debug("ShaderToy::onMouseButton: button={} pressed={} grabbed={}",
                      button, pressed, mouseGrabbed_);
        return true;
    }
    // Button -1 means click outside (focus lost)
    if (button == -1) {
        mouseGrabbed_ = false;
        spdlog::debug("ShaderToy::onMouseButton: focus lost, grabbed=false");
        return false;
    }
    return false;
}

bool ShaderToy::onMouseScroll(float xoffset, float yoffset, int mods) {
    (void)xoffset;

    // GLFW_MOD_CONTROL = 0x0002
    bool ctrlPressed = (mods & 0x0002) != 0;

    if (ctrlPressed) {
        // Ctrl+scroll = zoom
        zoom_ += yoffset * 0.1f;
        if (zoom_ < 0.1f) zoom_ = 0.1f;
        if (zoom_ > 5.0f) zoom_ = 5.0f;
        spdlog::debug("ShaderToy::onMouseScroll: CTRL+scroll zoom_={}", zoom_);
    } else {
        // Regular scroll = param
        param_ += yoffset * 0.1f;
        if (param_ < 0.0f) param_ = 0.0f;
        if (param_ > 1.0f) param_ = 1.0f;
        spdlog::debug("ShaderToy::onMouseScroll: scroll param_={}", param_);
    }
    return true;
}

const char* ShaderToy::getVertexShader() {
    return R"(
struct Uniforms {
    time: f32,
    param: f32,
    zoom: f32,
    _pad1: f32,
    resolution: vec2<f32>,
    _pad2: vec2<f32>,
    rect: vec4<f32>,  // x, y, w, h in NDC
    mouse: vec4<f32>, // x, y (normalized 0-1), grabbed, buttonDown
}

@group(0) @binding(0) var<uniform> u: Uniforms;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
}

@vertex
fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
    // Quad vertices in [0,1] space
    var positions = array<vec2<f32>, 6>(
        vec2<f32>(0.0, 0.0),
        vec2<f32>(1.0, 0.0),
        vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 0.0),
        vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 1.0)
    );

    let pos = positions[vertexIndex];

    // Transform to NDC using rect (x, y is top-left, w, h is size)
    let ndcX = u.rect.x + pos.x * u.rect.z;
    let ndcY = u.rect.y - pos.y * u.rect.w;  // Y goes down

    var output: VertexOutput;
    output.position = vec4<f32>(ndcX, ndcY, 0.0, 1.0);
    output.uv = pos;
    return output;
}
)";
}

std::string ShaderToy::wrapFragmentShader(const std::string& userCode) {
    std::ostringstream ss;
    ss << R"(
struct Uniforms {
    time: f32,
    param: f32,
    zoom: f32,
    _pad1: f32,
    resolution: vec2<f32>,
    _pad2: vec2<f32>,
    rect: vec4<f32>,
    mouse: vec4<f32>,  // x, y, grabbed, buttonDown
}

@group(0) @binding(0) var<uniform> u: Uniforms;

// Convenience aliases for user code
fn iTime() -> f32 { return u.time; }
fn iResolution() -> vec2<f32> { return u.resolution; }
fn iMouse() -> vec4<f32> { return u.mouse; }
fn iParam() -> f32 { return u.param; }
fn iZoom() -> f32 { return u.zoom; }
fn iGrabbed() -> bool { return u.mouse.z > 0.5; }
fn iMouseDown() -> bool { return u.mouse.w > 0.5; }

// User's shader code - should define mainImage(fragCoord: vec2<f32>) -> vec4<f32>
)" << userCode << R"(

@fragment
fn fs_main(@location(0) uv: vec2<f32>) -> @location(0) vec4<f32> {
    let fragCoord = uv * u.resolution;
    var col = mainImage(fragCoord);

    // Always draw a thin frame for visibility
    let border = 3.0;
    let res = u.resolution;
    let onBorder = fragCoord.x < border || fragCoord.x > res.x - border ||
                   fragCoord.y < border || fragCoord.y > res.y - border;

    if (onBorder) {
        if (iGrabbed()) {
            // Green frame when grabbed (mouse held)
            col = vec4<f32>(0.2, 0.9, 0.3, 1.0);
        } else {
            // Gray frame normally
            col = vec4<f32>(0.4, 0.4, 0.4, 1.0);
        }
    }

    return col;
}
)";
    return ss.str();
}

Result<void> ShaderToy::compileShader(WebGPUContext& ctx,
                                       WGPUTextureFormat targetFormat,
                                       const std::string& fragmentCode) {
    // Create uniform buffer (64 bytes: time, param, pad[2], resolution, pad[2], rect, mouse)
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = 64;
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    uniformBuffer_ = wgpuDeviceCreateBuffer(ctx.getDevice(), &bufDesc);
    if (!uniformBuffer_) {
        return Err<void>("Failed to create uniform buffer");
    }

    // Compile vertex shader
    std::string vertCode = getVertexShader();
    WGPUShaderModuleWGSLDescriptor wgslDescVert = {};
    wgslDescVert.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    wgslDescVert.code = vertCode.c_str();

    WGPUShaderModuleDescriptor shaderDescVert = {};
    shaderDescVert.nextInChain = &wgslDescVert.chain;
    WGPUShaderModule vertModule = wgpuDeviceCreateShaderModule(ctx.getDevice(), &shaderDescVert);

    // Compile fragment shader
    std::string fragCode = wrapFragmentShader(fragmentCode);
    std::cout << "ShaderToy: compiling fragment shader:\n" << fragCode << std::endl;

    WGPUShaderModuleWGSLDescriptor wgslDescFrag = {};
    wgslDescFrag.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    wgslDescFrag.code = fragCode.c_str();

    WGPUShaderModuleDescriptor shaderDescFrag = {};
    shaderDescFrag.nextInChain = &wgslDescFrag.chain;
    WGPUShaderModule fragModule = wgpuDeviceCreateShaderModule(ctx.getDevice(), &shaderDescFrag);

    if (!vertModule || !fragModule) {
        if (vertModule) wgpuShaderModuleRelease(vertModule);
        if (fragModule) wgpuShaderModuleRelease(fragModule);
        return Err<void>("Failed to create shader modules");
    }

    // Bind group layout
    WGPUBindGroupLayoutEntry bindingEntry = {};
    bindingEntry.binding = 0;
    bindingEntry.visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    bindingEntry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 1;
    bglDesc.entries = &bindingEntry;
    WGPUBindGroupLayout bgl = wgpuDeviceCreateBindGroupLayout(ctx.getDevice(), &bglDesc);
    if (!bgl) {
        wgpuShaderModuleRelease(vertModule);
        wgpuShaderModuleRelease(fragModule);
        return Err<void>("Failed to create bind group layout");
    }

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &bgl;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(ctx.getDevice(), &plDesc);
    if (!pipelineLayout) {
        wgpuShaderModuleRelease(vertModule);
        wgpuShaderModuleRelease(fragModule);
        wgpuBindGroupLayoutRelease(bgl);
        return Err<void>("Failed to create pipeline layout");
    }

    // Bind group
    WGPUBindGroupEntry bgEntry = {};
    bgEntry.binding = 0;
    bgEntry.buffer = uniformBuffer_;
    bgEntry.size = 64;

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bgl;
    bgDesc.entryCount = 1;
    bgDesc.entries = &bgEntry;
    bindGroup_ = wgpuDeviceCreateBindGroup(ctx.getDevice(), &bgDesc);
    if (!bindGroup_) {
        wgpuShaderModuleRelease(vertModule);
        wgpuShaderModuleRelease(fragModule);
        wgpuBindGroupLayoutRelease(bgl);
        wgpuPipelineLayoutRelease(pipelineLayout);
        return Err<void>("Failed to create bind group");
    }

    // Render pipeline
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;

    pipelineDesc.vertex.module = vertModule;
    pipelineDesc.vertex.entryPoint = "vs_main";

    WGPUFragmentState fragState = {};
    fragState.module = fragModule;
    fragState.entryPoint = "fs_main";

    WGPUColorTargetState colorTarget = {};
    colorTarget.format = targetFormat;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    // Alpha blending
    WGPUBlendState blend = {};
    blend.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blend.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blend.color.operation = WGPUBlendOperation_Add;
    blend.alpha.srcFactor = WGPUBlendFactor_One;
    blend.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blend.alpha.operation = WGPUBlendOperation_Add;
    colorTarget.blend = &blend;

    fragState.targetCount = 1;
    fragState.targets = &colorTarget;
    pipelineDesc.fragment = &fragState;

    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
    pipelineDesc.primitive.cullMode = WGPUCullMode_None;

    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;

    pipeline_ = wgpuDeviceCreateRenderPipeline(ctx.getDevice(), &pipelineDesc);

    // Cleanup shader modules
    wgpuShaderModuleRelease(vertModule);
    wgpuShaderModuleRelease(fragModule);
    wgpuBindGroupLayoutRelease(bgl);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!pipeline_) {
        return Err<void>("Failed to create render pipeline");
    }

    std::cout << "ShaderToy: pipeline created successfully" << std::endl;
    return Ok();
}

} // namespace yetty

// C exports for dynamic loading (when compiled as separate .so)
extern "C" {
    const char* shader_plugin_name() { return "shader"; }
    yetty::Result<yetty::PluginPtr> shader_plugin_create() { return yetty::ShaderToy::create(); }
}
