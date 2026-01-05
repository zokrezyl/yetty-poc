#include "thorvg-plugin.h"
#include <yetty/yetty.h>
#include <yetty/webgpu-context.h>
#include <yetty/wgpu-compat.h>
#include <spdlog/spdlog.h>
#include <cstring>
#include <cmath>

namespace yetty {

//-----------------------------------------------------------------------------
// ThorvgPlugin
//-----------------------------------------------------------------------------

ThorvgPlugin::~ThorvgPlugin() { (void)dispose(); }

Result<PluginPtr> ThorvgPlugin::create(YettyPtr engine) noexcept {
    auto p = PluginPtr(new ThorvgPlugin(std::move(engine)));
    if (auto res = static_cast<ThorvgPlugin*>(p.get())->init(); !res) {
        return Err<PluginPtr>("Failed to init ThorvgPlugin", res);
    }
    return Ok(p);
}

Result<void> ThorvgPlugin::init() noexcept {
    // Initialize ThorVG engine with WebGPU support
    auto result = tvg::Initializer::init(0);  // Single-threaded for now
    if (result != tvg::Result::Success) {
        return Err<void>("Failed to initialize ThorVG engine");
    }
    
    uint32_t major, minor, micro;
    const char* version = tvg::Initializer::version(&major, &minor, &micro);
    spdlog::info("ThorvgPlugin: initialized ThorVG {} (WebGPU renderer)", version ? version : "unknown");
    
    _initialized = true;
    return Ok();
}

Result<void> ThorvgPlugin::dispose() {
    if (auto res = Plugin::dispose(); !res) {
        return Err<void>("Failed to dispose ThorvgPlugin", res);
    }
    
    if (_initialized) {
        tvg::Initializer::term();
    }
    
    _initialized = false;
    return Ok();
}

Result<PluginLayerPtr> ThorvgPlugin::createLayer(const std::string& payload) {
    auto layer = std::make_shared<ThorvgLayer>();
    auto result = layer->init(payload);
    if (!result) {
        return Err<PluginLayerPtr>("Failed to init ThorvgLayer", result);
    }
    return Ok<PluginLayerPtr>(layer);
}

//-----------------------------------------------------------------------------
// ThorvgLayer
//-----------------------------------------------------------------------------

ThorvgLayer::ThorvgLayer() = default;

ThorvgLayer::~ThorvgLayer() { (void)dispose(); }

Result<void> ThorvgLayer::init(const std::string& payload) {
    if (payload.empty()) {
        return Err<void>("ThorvgLayer: empty payload");
    }

    _payload = payload;
    
    // Detect mime type from payload
    std::string mimeType;
    
    // Check for Lottie JSON (starts with { or whitespace + {)
    size_t start = payload.find_first_not_of(" \t\n\r");
    if (start != std::string::npos && payload[start] == '{') {
        mimeType = "lottie";
    }
    // Check for SVG (contains <svg or <?xml ... <svg)
    else if (payload.find("<svg") != std::string::npos || 
             (payload.find("<?xml") != std::string::npos && payload.find("<svg") != std::string::npos)) {
        mimeType = "svg";
    }

    auto result = loadContent(payload, mimeType);
    if (!result) {
        return result;
    }

    spdlog::info("ThorvgLayer: loaded {}x{} content (animated: {})", 
                 _content_width, _content_height, _is_animated);
    return Ok();
}

Result<void> ThorvgLayer::loadContent(const std::string& data, const std::string& mimeType) {
    // WgCanvas will be created when WebGPU context is available (in initWebGPU)
    // For now, just parse content to get dimensions and animation info
    
    // Default size
    _content_width = 256;
    _content_height = 256;

    tvg::Picture* picture = nullptr;

    // Check if this is an animation (Lottie)
    if (mimeType == "lottie" || mimeType == "lottie+json" || mimeType == "lot") {
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("Failed to create ThorVG Animation");
        }

        picture = _animation->picture();
        if (!picture) {
            return Err<void>("Failed to get picture from Animation");
        }

        auto result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()), 
                                    "lottie", nullptr, true);
        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("Failed to load Lottie animation");
        }

        _is_animated = true;
        _total_frames = _animation->totalFrame();
        _duration = _animation->duration();
        _current_frame = 0.0f;
        
        spdlog::debug("ThorvgLayer: Lottie animation loaded - {} frames, {}s duration", 
                     _total_frames, _duration);
    } else {
        // Static picture (SVG, PNG, etc.)
        // Use Animation wrapper for consistency with WgCanvas
        _animation.reset(tvg::Animation::gen());
        if (!_animation) {
            return Err<void>("Failed to create ThorVG Animation for static content");
        }

        picture = _animation->picture();
        if (!picture) {
            return Err<void>("Failed to get picture from Animation");
        }

        const char* mime = mimeType.empty() ? nullptr : mimeType.c_str();
        auto result = picture->load(data.c_str(), static_cast<uint32_t>(data.size()), 
                                    mime, nullptr, true);
        if (result != tvg::Result::Success) {
            _animation.reset();
            return Err<void>("Failed to load static content into ThorVG Picture");
        }

        _is_animated = false;
    }

    // Get content size
    float w = 0, h = 0;
    picture->size(&w, &h);
    
    if (w > 0 && h > 0) {
        _content_width = static_cast<uint32_t>(w);
        _content_height = static_cast<uint32_t>(h);
    }

    _picture = picture;
    _content_dirty = true;

    return Ok();
}

Result<void> ThorvgLayer::initWebGPU(WebGPUContext& ctx) {
    WGPUDevice device = ctx.getDevice();
    WGPUInstance instance = ctx.getInstance();
    
    // Create render texture that ThorVG will render to
    // ThorVG's WgCanvas needs RGBA8Unorm format with specific usage flags
    WGPUTextureDescriptor texDesc = {};
    texDesc.size.width = _content_width;
    texDesc.size.height = _content_height;
    texDesc.size.depthOrArrayLayers = 1;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    // ThorVG needs RenderAttachment for drawing, we need TextureBinding for compositing
    texDesc.usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopySrc;

    _render_texture = wgpuDeviceCreateTexture(device, &texDesc);
    if (!_render_texture) return Err<void>("Failed to create ThorVG render texture");

    // Create texture view for our composite shader
    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    _render_texture_view = wgpuTextureCreateView(_render_texture, &viewDesc);
    if (!_render_texture_view) return Err<void>("Failed to create ThorVG texture view");

    // Create ThorVG WgCanvas and set the texture as target
    _canvas.reset(tvg::WgCanvas::gen());
    if (!_canvas) {
        return Err<void>("Failed to create ThorVG WgCanvas");
    }

    // Set the render target - type=1 means texture target (not surface)
    auto result = _canvas->target(device, instance, _render_texture, 
                                   _content_width, _content_height,
                                   tvg::ColorSpace::ABGR8888S, 1);
    if (result != tvg::Result::Success) {
        return Err<void>("Failed to set ThorVG WgCanvas target");
    }

    // Push the picture to canvas
    // Note: Canvas::push() takes ownership reference
    if (_picture) {
        result = _canvas->push(_picture);
        if (result != tvg::Result::Success) {
            return Err<void>("Failed to push picture to ThorVG canvas");
        }
    }

    spdlog::debug("ThorvgLayer: WebGPU canvas initialized {}x{}", _content_width, _content_height);
    return Ok();
}

Result<void> ThorvgLayer::createCompositePipeline(WebGPUContext& ctx, WGPUTextureFormat targetFormat) {
    WGPUDevice device = ctx.getDevice();

    // Create sampler for compositing
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;
    _sampler = wgpuDeviceCreateSampler(device, &samplerDesc);
    if (!_sampler) return Err<void>("Failed to create sampler");

    // Create uniform buffer for rect transform
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = 16;
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    _uniform_buffer = wgpuDeviceCreateBuffer(device, &bufDesc);
    if (!_uniform_buffer) return Err<void>("Failed to create uniform buffer");

    // Composite shader - renders ThorVG output texture as a quad
    const char* shaderCode = R"(
struct Uniforms { rect: vec4<f32>, }
@group(0) @binding(0) var<uniform> u: Uniforms;
@group(0) @binding(1) var texSampler: sampler;
@group(0) @binding(2) var tex: texture_2d<f32>;
struct VertexOutput { @builtin(position) position: vec4<f32>, @location(0) uv: vec2<f32>, }
@vertex fn vs_main(@builtin(vertex_index) vi: u32) -> VertexOutput {
    var p = array<vec2<f32>,6>(vec2(0.,0.),vec2(1.,0.),vec2(1.,1.),vec2(0.,0.),vec2(1.,1.),vec2(0.,1.));
    let pos = p[vi];
    var o: VertexOutput;
    o.position = vec4(u.rect.x + pos.x * u.rect.z, u.rect.y - pos.y * u.rect.w, 0., 1.);
    o.uv = pos;
    return o;
}
@fragment fn fs_main(@location(0) uv: vec2<f32>) -> @location(0) vec4<f32> {
    return textureSample(tex, texSampler, uv);
}
)";

    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(shaderCode);
    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);
    if (!shaderModule) return Err<void>("Failed to create shader module");

    // Bind group layout
    WGPUBindGroupLayoutEntry entries[3] = {};
    entries[0].binding = 0; entries[0].visibility = WGPUShaderStage_Vertex;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    entries[1].binding = 1; entries[1].visibility = WGPUShaderStage_Fragment;
    entries[1].sampler.type = WGPUSamplerBindingType_Filtering;
    entries[2].binding = 2; entries[2].visibility = WGPUShaderStage_Fragment;
    entries[2].texture.sampleType = WGPUTextureSampleType_Float;
    entries[2].texture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 3; bglDesc.entries = entries;
    WGPUBindGroupLayout bgl = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);
    if (!bgl) { wgpuShaderModuleRelease(shaderModule); return Err<void>("Failed to create bgl"); }

    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1; plDesc.bindGroupLayouts = &bgl;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &plDesc);

    // Bind group
    WGPUBindGroupEntry bgE[3] = {};
    bgE[0].binding = 0; bgE[0].buffer = _uniform_buffer; bgE[0].size = 16;
    bgE[1].binding = 1; bgE[1].sampler = _sampler;
    bgE[2].binding = 2; bgE[2].textureView = _render_texture_view;
    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bgl; bgDesc.entryCount = 3; bgDesc.entries = bgE;
    _bind_group = wgpuDeviceCreateBindGroup(device, &bgDesc);

    // Pipeline
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    
    WGPUFragmentState fragState = {};
    fragState.module = shaderModule; 
    fragState.entryPoint = WGPU_STR("fs_main");
    
    WGPUColorTargetState colorTarget = {};
    colorTarget.format = targetFormat; 
    colorTarget.writeMask = WGPUColorWriteMask_All;
    
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
    pipelineDesc.multisample.count = 1; 
    pipelineDesc.multisample.mask = ~0u;

    _composite_pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    wgpuShaderModuleRelease(shaderModule);
    wgpuBindGroupLayoutRelease(bgl);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!_composite_pipeline) return Err<void>("Failed to create composite pipeline");

    spdlog::debug("ThorvgLayer: composite pipeline created");
    return Ok();
}

void ThorvgLayer::renderThorvgFrame() {
    if (!_canvas || !_picture) return;

    // Update animation frame if animated
    if (_is_animated && _animation) {
        _animation->frame(_current_frame);
    }

    // Render using ThorVG's WebGPU canvas
    _canvas->update();
    _canvas->draw();
    _canvas->sync();
    
    _content_dirty = false;
}

void ThorvgLayer::setFrame(float frame) {
    if (!_is_animated) return;
    
    _current_frame = frame;
    if (_current_frame >= _total_frames) {
        _current_frame = _loop ? 0.0f : _total_frames - 1;
    }
    if (_current_frame < 0) {
        _current_frame = 0;
    }
    
    _content_dirty = true;
}

Result<void> ThorvgLayer::dispose() {
    // Release composite resources
    if (_bind_group) { wgpuBindGroupRelease(_bind_group); _bind_group = nullptr; }
    if (_composite_pipeline) { wgpuRenderPipelineRelease(_composite_pipeline); _composite_pipeline = nullptr; }
    if (_uniform_buffer) { wgpuBufferRelease(_uniform_buffer); _uniform_buffer = nullptr; }
    if (_sampler) { wgpuSamplerRelease(_sampler); _sampler = nullptr; }
    if (_render_texture_view) { wgpuTextureViewRelease(_render_texture_view); _render_texture_view = nullptr; }
    
    // Release ThorVG canvas before destroying the texture it targets
    _canvas.reset();
    
    if (_render_texture) { wgpuTextureRelease(_render_texture); _render_texture = nullptr; }
    
    _animation.reset();
    _picture = nullptr;
    
    _gpu_initialized = false;
    _content_dirty = true;
    return Ok();
}

Result<void> ThorvgLayer::render(WebGPUContext& ctx) {
    if (_failed) return Err<void>("ThorvgLayer already failed");
    if (!_visible) return Ok();
    if (!_animation) return Err<void>("ThorvgLayer has no content");

    const auto& rc = _render_context;

    // Update animation if playing
    if (_is_animated && _playing && _duration > 0) {
        _accumulated_time += rc.deltaTime;
        float fps = _total_frames / _duration;
        float frameDelta = static_cast<float>(_accumulated_time * fps);
        
        if (frameDelta >= 1.0f) {
            _current_frame += frameDelta;
            _accumulated_time = 0.0;
            
            if (_current_frame >= _total_frames) {
                if (_loop) {
                    _current_frame = std::fmod(_current_frame, _total_frames);
                } else {
                    _current_frame = _total_frames - 1;
                    _playing = false;
                }
            }
            
            _content_dirty = true;
        }
    }

    // Initialize GPU resources on first use
    if (!_gpu_initialized) {
        auto result = initWebGPU(ctx);
        if (!result) {
            _failed = true;
            return Err<void>("Failed to init WebGPU", result);
        }
        
        result = createCompositePipeline(ctx, rc.targetFormat);
        if (!result) {
            _failed = true;
            return Err<void>("Failed to create pipeline", result);
        }
        _gpu_initialized = true;
        _content_dirty = true;  // Need initial render
    }

    // Render ThorVG content if dirty
    if (_content_dirty) {
        renderThorvgFrame();
    }

    if (!_composite_pipeline || !_uniform_buffer || !_bind_group) {
        _failed = true;
        return Err<void>("ThorvgLayer pipeline not initialized");
    }

    // Calculate pixel position from cell position
    float pixelX = _x * rc.cellWidth;
    float pixelY = _y * rc.cellHeight;
    float pixelW = _width_cells * rc.cellWidth;
    float pixelH = _height_cells * rc.cellHeight;

    // Adjust for scroll offset
    if (_position_mode == PositionMode::Relative && rc.scrollOffset > 0) {
        pixelY += rc.scrollOffset * rc.cellHeight;
    }

    // Skip if off-screen
    if (rc.termRows > 0) {
        float screenPixelHeight = rc.termRows * rc.cellHeight;
        if (pixelY + pixelH <= 0 || pixelY >= screenPixelHeight) {
            return Ok();
        }
    }

    float ndcX = (pixelX / rc.screenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (pixelY / rc.screenHeight) * 2.0f;
    float ndcW = (pixelW / rc.screenWidth) * 2.0f;
    float ndcH = (pixelH / rc.screenHeight) * 2.0f;

    // Update uniform buffer if rect changed
    bool rectChanged = (ndcX != _last_rect[0] || ndcY != _last_rect[1] ||
                        ndcW != _last_rect[2] || ndcH != _last_rect[3]);
    if (rectChanged) {
        struct Uniforms { float rect[4]; } uniforms;
        uniforms.rect[0] = ndcX;
        uniforms.rect[1] = ndcY;
        uniforms.rect[2] = ndcW;
        uniforms.rect[3] = ndcH;
        wgpuQueueWriteBuffer(ctx.getQueue(), _uniform_buffer, 0, &uniforms, sizeof(uniforms));
        _last_rect[0] = ndcX;
        _last_rect[1] = ndcY;
        _last_rect[2] = ndcW;
        _last_rect[3] = ndcH;
    }

    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ctx.getDevice(), &encoderDesc);
    if (!encoder) return Err<void>("ThorvgLayer: Failed to create command encoder");

    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = rc.targetView;
    colorAttachment.loadOp = WGPULoadOp_Load;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (!pass) {
        wgpuCommandEncoderRelease(encoder);
        return Err<void>("ThorvgLayer: Failed to begin render pass");
    }

    wgpuRenderPassEncoderSetPipeline(pass, _composite_pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bind_group, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    if (!cmdBuffer) {
        wgpuCommandEncoderRelease(encoder);
        return Err<void>("ThorvgLayer: Failed to finish command encoder");
    }
    wgpuQueueSubmit(ctx.getQueue(), 1, &cmdBuffer);
    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);
    return Ok();
}

bool ThorvgLayer::renderToPass(WGPURenderPassEncoder pass, WebGPUContext& ctx) {
    if (_failed || !_visible || !_animation) return false;

    const auto& rc = _render_context;

    // Update animation if playing
    if (_is_animated && _playing && _duration > 0) {
        _accumulated_time += rc.deltaTime;
        float fps = _total_frames / _duration;
        float frameDelta = static_cast<float>(_accumulated_time * fps);
        
        if (frameDelta >= 1.0f) {
            _current_frame += frameDelta;
            _accumulated_time = 0.0;
            
            if (_current_frame >= _total_frames) {
                if (_loop) {
                    _current_frame = std::fmod(_current_frame, _total_frames);
                } else {
                    _current_frame = _total_frames - 1;
                    _playing = false;
                }
            }
            
            _content_dirty = true;
        }
    }

    // Initialize GPU resources on first use
    if (!_gpu_initialized) {
        auto result = initWebGPU(ctx);
        if (!result) {
            _failed = true;
            return false;
        }
        
        result = createCompositePipeline(ctx, rc.targetFormat);
        if (!result) {
            _failed = true;
            return false;
        }
        _gpu_initialized = true;
        _content_dirty = true;
    }

    // Render ThorVG content if dirty (animation frame changed)
    if (_content_dirty) {
        renderThorvgFrame();
    }

    if (!_composite_pipeline || !_uniform_buffer || !_bind_group) {
        _failed = true;
        return false;
    }

    // Calculate pixel position from cell position
    float pixelX = _x * rc.cellWidth;
    float pixelY = _y * rc.cellHeight;
    float pixelW = _width_cells * rc.cellWidth;
    float pixelH = _height_cells * rc.cellHeight;

    // Adjust for scroll offset
    if (_position_mode == PositionMode::Relative && rc.scrollOffset > 0) {
        pixelY += rc.scrollOffset * rc.cellHeight;
    }

    // Skip if off-screen
    if (rc.termRows > 0) {
        float screenPixelHeight = rc.termRows * rc.cellHeight;
        if (pixelY + pixelH <= 0 || pixelY >= screenPixelHeight) {
            return false;
        }
    }

    float ndcX = (pixelX / rc.screenWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (pixelY / rc.screenHeight) * 2.0f;
    float ndcW = (pixelW / rc.screenWidth) * 2.0f;
    float ndcH = (pixelH / rc.screenHeight) * 2.0f;

    // Update uniform buffer if rect changed
    bool rectChanged = (ndcX != _last_rect[0] || ndcY != _last_rect[1] ||
                        ndcW != _last_rect[2] || ndcH != _last_rect[3]);
    if (rectChanged) {
        struct Uniforms { float rect[4]; } uniforms;
        uniforms.rect[0] = ndcX;
        uniforms.rect[1] = ndcY;
        uniforms.rect[2] = ndcW;
        uniforms.rect[3] = ndcH;
        wgpuQueueWriteBuffer(ctx.getQueue(), _uniform_buffer, 0, &uniforms, sizeof(uniforms));
        _last_rect[0] = ndcX;
        _last_rect[1] = ndcY;
        _last_rect[2] = ndcW;
        _last_rect[3] = ndcH;
    }

    // Draw ThorVG rendered content into existing pass
    wgpuRenderPassEncoderSetPipeline(pass, _composite_pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bind_group, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

    return true;
}

} // namespace yetty

extern "C" {
    const char* name() { return "thorvg"; }
    yetty::Result<yetty::PluginPtr> create(yetty::YettyPtr engine) {
        return yetty::ThorvgPlugin::create(std::move(engine));
    }
}
