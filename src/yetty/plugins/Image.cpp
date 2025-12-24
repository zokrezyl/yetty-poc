#include "Image.h"
#include "../renderer/WebGPUContext.h"
#include <iostream>
#include <cstring>

// Note: STB_IMAGE_IMPLEMENTATION is defined in Font.cpp
#include <stb_image.h>

namespace yetty {

Image::Image() {
}

Image::~Image() {
    dispose();
}

Result<PluginPtr> Image::create() {
    return Ok<PluginPtr>(std::make_shared<Image>());
}

Result<void> Image::init(const std::string& payload) {
    if (payload.empty()) {
        return Err<void>("Image: empty payload");
    }

    // Reset state for re-initialization
    dispose();

    // Load image from payload (raw image bytes)
    auto result = loadImage(payload);
    if (!result) {
        return result;
    }

    std::cout << "Image: loaded " << imageWidth_ << "x" << imageHeight_
              << " (" << imageChannels_ << " channels)" << std::endl;
    return Ok();
}

Result<void> Image::loadImage(const std::string& data) {
    // Free any existing image data
    if (imageData_) {
        stbi_image_free(imageData_);
        imageData_ = nullptr;
    }

    // Load image from memory
    imageData_ = stbi_load_from_memory(
        reinterpret_cast<const unsigned char*>(data.data()),
        static_cast<int>(data.size()),
        &imageWidth_, &imageHeight_, &imageChannels_,
        4  // Force RGBA
    );

    if (!imageData_) {
        return Err<void>(std::string("Failed to load image: ") + stbi_failure_reason());
    }

    imageChannels_ = 4;  // We forced RGBA
    return Ok();
}

void Image::dispose() {
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
    if (sampler_) {
        wgpuSamplerRelease(sampler_);
        sampler_ = nullptr;
    }
    if (textureView_) {
        wgpuTextureViewRelease(textureView_);
        textureView_ = nullptr;
    }
    if (texture_) {
        wgpuTextureRelease(texture_);
        texture_ = nullptr;
    }
    if (imageData_) {
        stbi_image_free(imageData_);
        imageData_ = nullptr;
    }
    initialized_ = false;
}

void Image::update(double deltaTime) {
    (void)deltaTime;
    // Images are static, no update needed
}

void Image::render(WebGPUContext& ctx,
                   WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                   uint32_t screenWidth, uint32_t screenHeight,
                   float pixelX, float pixelY, float pixelW, float pixelH) {
    if (failed_ || !imageData_) return;

    // First time: create GPU resources
    if (!initialized_) {
        auto result = createPipeline(ctx, targetFormat);
        if (!result) {
            std::cerr << "Image: " << error_msg(result) << std::endl;
            failed_ = true;
            return;
        }
        initialized_ = true;
    }

    if (!pipeline_ || !uniformBuffer_ || !bindGroup_) {
        failed_ = true;
        return;
    }

    // Update uniform buffer with position
    float ndcX = (pixelX / screenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (pixelY / screenHeight) * 2.0f;
    float ndcW = (pixelW / screenWidth) * 2.0f;
    float ndcH = (pixelH / screenHeight) * 2.0f;

    struct Uniforms {
        float rect[4];  // x, y, w, h in NDC
    } uniforms;

    uniforms.rect[0] = ndcX;
    uniforms.rect[1] = ndcY;
    uniforms.rect[2] = ndcW;
    uniforms.rect[3] = ndcH;

    wgpuQueueWriteBuffer(ctx.getQueue(), uniformBuffer_, 0, &uniforms, sizeof(uniforms));

    // Create command encoder
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ctx.getDevice(), &encoderDesc);
    if (!encoder) {
        std::cerr << "Image: failed to create command encoder" << std::endl;
        return;
    }

    // Render pass
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Load;
    colorAttachment.storeOp = WGPUStoreOp_Store;

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (!pass) {
        std::cerr << "Image: failed to begin render pass" << std::endl;
        wgpuCommandEncoderRelease(encoder);
        return;
    }

    wgpuRenderPassEncoderSetPipeline(pass, pipeline_);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup_, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
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

void Image::onResize(uint32_t newWidth, uint32_t newHeight) {
    Plugin::onResize(newWidth, newHeight);
}

Result<void> Image::createPipeline(WebGPUContext& ctx, WGPUTextureFormat targetFormat) {
    WGPUDevice device = ctx.getDevice();

    // Create texture from image data
    WGPUTextureDescriptor texDesc = {};
    texDesc.size.width = imageWidth_;
    texDesc.size.height = imageHeight_;
    texDesc.size.depthOrArrayLayers = 1;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

    texture_ = wgpuDeviceCreateTexture(device, &texDesc);
    if (!texture_) {
        return Err<void>("Failed to create texture");
    }

    // Upload image data
    WGPUImageCopyTexture dst = {};
    dst.texture = texture_;
    dst.mipLevel = 0;
    dst.origin = {0, 0, 0};
    dst.aspect = WGPUTextureAspect_All;

    WGPUTextureDataLayout layout = {};
    layout.offset = 0;
    layout.bytesPerRow = imageWidth_ * 4;
    layout.rowsPerImage = imageHeight_;

    WGPUExtent3D extent = {static_cast<uint32_t>(imageWidth_),
                           static_cast<uint32_t>(imageHeight_), 1};
    wgpuQueueWriteTexture(ctx.getQueue(), &dst, imageData_,
                          imageWidth_ * imageHeight_ * 4, &layout, &extent);

    // Create texture view
    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    textureView_ = wgpuTextureCreateView(texture_, &viewDesc);
    if (!textureView_) {
        return Err<void>("Failed to create texture view");
    }

    // Create sampler
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    sampler_ = wgpuDeviceCreateSampler(device, &samplerDesc);
    if (!sampler_) {
        return Err<void>("Failed to create sampler");
    }

    // Create uniform buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = 16;  // 4 floats for rect
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    uniformBuffer_ = wgpuDeviceCreateBuffer(device, &bufDesc);
    if (!uniformBuffer_) {
        return Err<void>("Failed to create uniform buffer");
    }

    // Shader code
    const char* shaderCode = R"(
struct Uniforms {
    rect: vec4<f32>,  // x, y, w, h in NDC
}

@group(0) @binding(0) var<uniform> u: Uniforms;
@group(0) @binding(1) var texSampler: sampler;
@group(0) @binding(2) var tex: texture_2d<f32>;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
}

@vertex
fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
    var positions = array<vec2<f32>, 6>(
        vec2<f32>(0.0, 0.0),
        vec2<f32>(1.0, 0.0),
        vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 0.0),
        vec2<f32>(1.0, 1.0),
        vec2<f32>(0.0, 1.0)
    );

    let pos = positions[vertexIndex];
    let ndcX = u.rect.x + pos.x * u.rect.z;
    let ndcY = u.rect.y - pos.y * u.rect.w;

    var output: VertexOutput;
    output.position = vec4<f32>(ndcX, ndcY, 0.0, 1.0);
    output.uv = pos;
    return output;
}

@fragment
fn fs_main(@location(0) uv: vec2<f32>) -> @location(0) vec4<f32> {
    return textureSample(tex, texSampler, uv);
}
)";

    WGPUShaderModuleWGSLDescriptor wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    wgslDesc.code = shaderCode;

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);
    if (!shaderModule) {
        return Err<void>("Failed to create shader module");
    }

    // Bind group layout
    WGPUBindGroupLayoutEntry entries[3] = {};

    // Uniform buffer
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Vertex;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // Sampler
    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Fragment;
    entries[1].sampler.type = WGPUSamplerBindingType_Filtering;

    // Texture
    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Fragment;
    entries[2].texture.sampleType = WGPUTextureSampleType_Float;
    entries[2].texture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 3;
    bglDesc.entries = entries;
    WGPUBindGroupLayout bgl = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);
    if (!bgl) {
        wgpuShaderModuleRelease(shaderModule);
        return Err<void>("Failed to create bind group layout");
    }

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &bgl;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &plDesc);
    if (!pipelineLayout) {
        wgpuShaderModuleRelease(shaderModule);
        wgpuBindGroupLayoutRelease(bgl);
        return Err<void>("Failed to create pipeline layout");
    }

    // Bind group
    WGPUBindGroupEntry bgEntries[3] = {};
    bgEntries[0].binding = 0;
    bgEntries[0].buffer = uniformBuffer_;
    bgEntries[0].size = 16;

    bgEntries[1].binding = 1;
    bgEntries[1].sampler = sampler_;

    bgEntries[2].binding = 2;
    bgEntries[2].textureView = textureView_;

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bgl;
    bgDesc.entryCount = 3;
    bgDesc.entries = bgEntries;
    bindGroup_ = wgpuDeviceCreateBindGroup(device, &bgDesc);
    if (!bindGroup_) {
        wgpuShaderModuleRelease(shaderModule);
        wgpuBindGroupLayoutRelease(bgl);
        wgpuPipelineLayoutRelease(pipelineLayout);
        return Err<void>("Failed to create bind group");
    }

    // Render pipeline
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;

    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = "vs_main";

    WGPUFragmentState fragState = {};
    fragState.module = shaderModule;
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

    pipeline_ = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    wgpuShaderModuleRelease(shaderModule);
    wgpuBindGroupLayoutRelease(bgl);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!pipeline_) {
        return Err<void>("Failed to create render pipeline");
    }

    std::cout << "Image: pipeline created successfully" << std::endl;
    return Ok();
}

} // namespace yetty

// C exports for dynamic loading (when compiled as separate .so)
extern "C" {
    const char* image_plugin_name() { return "image"; }
    yetty::Result<yetty::PluginPtr> image_plugin_create() { return yetty::Image::create(); }
}
