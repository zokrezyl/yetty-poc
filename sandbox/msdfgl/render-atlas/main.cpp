/**
 * MSDFGL Atlas Viewer - Debug tool to visualize the atlas texture
 */

#include <msdfgl.h>
#include <yetty/wgpu-compat.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <iostream>
#include <vector>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 512;

WGPUInstance instance = nullptr;
WGPUSurface surface = nullptr;
WGPUAdapter adapter = nullptr;
WGPUDevice device = nullptr;
WGPUQueue queue = nullptr;
WGPUTextureFormat surfaceFormat = WGPUTextureFormat_BGRA8Unorm;

std::unique_ptr<msdfgl::Context> msdfContext;
std::unique_ptr<msdfgl::Font> font;

WGPURenderPipeline pipeline = nullptr;
WGPUBindGroup bindGroup = nullptr;
WGPUSampler sampler = nullptr;

bool adapterReady = false;
bool deviceReady = false;

// Simple shader that renders the atlas texture to a fullscreen quad
const char* shaderCode = R"(
@group(0) @binding(0) var atlasSampler: sampler;
@group(0) @binding(1) var atlasTexture: texture_2d<f32>;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    // Fullscreen triangle
    var pos = array<vec2<f32>, 3>(
        vec2<f32>(-1.0, -1.0),
        vec2<f32>(3.0, -1.0),
        vec2<f32>(-1.0, 3.0)
    );
    var uv = array<vec2<f32>, 3>(
        vec2<f32>(0.0, 1.0),
        vec2<f32>(2.0, 1.0),
        vec2<f32>(0.0, -1.0)
    );

    var out: VertexOutput;
    out.position = vec4<f32>(pos[idx], 0.0, 1.0);
    out.uv = uv[idx];
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    // Sample atlas and show RGB channels
    let msdf = textureSample(atlasTexture, atlasSampler, in.uv);

    // Show the raw MSDF values (shifted to visible range)
    // MSDF values are typically around 0.5, so we show them directly
    return vec4<f32>(msdf.rgb, 1.0);
}
)";

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter result,
                    WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestAdapterStatus_Success) {
        adapter = result;
    }
    adapterReady = true;
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice result,
                   WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestDeviceStatus_Success) {
        device = result;
    }
    deviceReady = true;
}

bool initWebGPU(GLFWwindow* window) {
    std::cout << "[WebGPU] Initializing..." << std::endl;

    WGPUInstanceDescriptor instanceDesc = {};
    instance = wgpuCreateInstance(&instanceDesc);

    surface = glfwCreateWindowWGPUSurface(instance, window);

    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = surface;

    WGPURequestAdapterCallbackInfo adapterCallback = {};
    adapterCallback.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCallback.callback = onAdapterReady;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCallback);
    while (!adapterReady) { glfwPollEvents(); }
    if (!adapter) return false;

    WGPUDeviceDescriptor deviceDesc = {};
    WGPURequestDeviceCallbackInfo deviceCallback = {};
    deviceCallback.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCallback.callback = onDeviceReady;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCallback);
    while (!deviceReady) { glfwPollEvents(); }
    if (!device) return false;

    queue = wgpuDeviceGetQueue(device);

    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(surface, adapter, &caps);
    surfaceFormat = caps.formats[0];

    WGPUSurfaceConfiguration surfaceConfig = {};
    surfaceConfig.device = device;
    surfaceConfig.format = surfaceFormat;
    surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
    surfaceConfig.width = WINDOW_WIDTH;
    surfaceConfig.height = WINDOW_HEIGHT;
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(surface, &surfaceConfig);

    std::cout << "[WebGPU] Initialized" << std::endl;
    return true;
}

bool initRendering() {
    auto atlas = font->getAtlas();
    if (!atlas) return false;

    std::cout << "[Render] Atlas size: " << atlas->getTextureWidth() << "x"
              << atlas->getTextureHeight() << std::endl;

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(shaderCode);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);

    // Create sampler
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    sampler = wgpuDeviceCreateSampler(device, &samplerDesc);

    // Create bind group layout
    WGPUBindGroupLayoutEntry layoutEntries[2] = {};
    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Fragment;
    layoutEntries[0].sampler.type = WGPUSamplerBindingType_Filtering;

    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Fragment;
    layoutEntries[1].texture.sampleType = WGPUTextureSampleType_Float;
    layoutEntries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 2;
    layoutDesc.entries = layoutEntries;
    WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);

    // Create bind group
    WGPUBindGroupEntry bindEntries[2] = {};
    bindEntries[0].binding = 0;
    bindEntries[0].sampler = sampler;

    bindEntries[1].binding = 1;
    bindEntries[1].textureView = atlas->getAtlasTextureView();

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = 2;
    bindGroupDesc.entries = bindEntries;
    bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);

    // Create render pipeline
    WGPUColorTargetState colorTarget = {};
    colorTarget.format = surfaceFormat;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragmentState = {};
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = WGPU_STR("fs_main");
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    pipelineDesc.fragment = &fragmentState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;

    pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    wgpuShaderModuleRelease(shaderModule);
    wgpuBindGroupLayoutRelease(bindGroupLayout);
    wgpuPipelineLayoutRelease(pipelineLayout);

    std::cout << "[Render] Pipeline created" << std::endl;
    return pipeline != nullptr;
}

void render() {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) return;

    WGPUTextureViewDescriptor viewDesc = {};
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDesc);

    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.1, 0.1, 0.1, 1.0};
#if !defined(__EMSCRIPTEN__)
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    wgpuRenderPassEncoderSetPipeline(pass, pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 3, 1, 0, 0);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuRenderPassEncoderRelease(pass);
    wgpuCommandEncoderRelease(encoder);
    wgpuTextureViewRelease(targetView);

    wgpuSurfacePresent(surface);
}

void cleanup() {
    if (bindGroup) wgpuBindGroupRelease(bindGroup);
    if (pipeline) wgpuRenderPipelineRelease(pipeline);
    if (sampler) wgpuSamplerRelease(sampler);
    msdfContext.reset();
    font.reset();
}

int main() {
    std::cout << "========================================\n"
              << "MSDFGL Atlas Viewer\n"
              << "========================================\n" << std::endl;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MSDFGL Atlas Viewer", nullptr, nullptr);

    if (!initWebGPU(window)) {
        std::cerr << "Failed to initialize WebGPU" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Create MSDFGL context
    msdfContext = std::make_unique<msdfgl::Context>(device);
    if (!msdfContext) {
        std::cerr << "Failed to create MSDFGL context" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Load font
    msdfgl::FontConfig config;
    config.scale = 2.0f;
    config.range = 4.0f;
    font = msdfContext->loadFont("../../assets/DejaVuSansMNerdFontMono-Regular.ttf", config);
    if (!font) {
        std::cerr << "Failed to load font" << std::endl;
        glfwTerminate();
        return 1;
    }

    std::cout << "[Font] Loaded, generating glyphs..." << std::endl;

    // Generate some glyphs
    std::vector<int32_t> charset;
    for (int c = 0x20; c <= 0x7E; ++c) charset.push_back(c);

    int result = font->generateGlyphList(charset);
    std::cout << "[Font] Generated " << result << " glyphs" << std::endl;

    // Initialize rendering
    if (!initRendering()) {
        std::cerr << "Failed to initialize rendering" << std::endl;
        cleanup();
        glfwTerminate();
        return 1;
    }

    std::cout << "[Main] Starting render loop..." << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        render();
    }

    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
