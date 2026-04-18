#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu.h>
#include <cstdio>
#include <cstring>

WGPUDevice g_device = nullptr;
WGPUQueue g_queue = nullptr;
WGPUSurface g_surface = nullptr;
WGPURenderPipeline g_pipeline = nullptr;
WGPUAdapter g_adapter = nullptr;
WGPUTextureFormat g_format = WGPUTextureFormat_BGRA8Unorm; // Browser default

const char* shaderCode = R"(
@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> @builtin(position) vec4f {
    var pos = array<vec2f, 6>(
        vec2f(-0.8, -0.8),
        vec2f( 0.8, -0.8),
        vec2f( 0.8,  0.8),
        vec2f(-0.8, -0.8),
        vec2f( 0.8,  0.8),
        vec2f(-0.8,  0.8)
    );
    return vec4f(pos[idx], 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
    return vec4f(1.0, 0.0, 0.0, 1.0); // RED
}
)";

static int frameCount = 0;

void render() {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(g_surface, &surfaceTexture);
    
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        printf("Failed to get surface texture: %d\n", surfaceTexture.status);
        return;
    }
    
    if (frameCount < 3) {
        printf("Frame %d: texture=%p, pipeline=%p\n", frameCount, surfaceTexture.texture, g_pipeline);
    }

    WGPUTextureView view = wgpuTextureCreateView(surfaceTexture.texture, nullptr);
    if (!view) {
        printf("Failed to create texture view!\n");
        return;
    }

    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_device, &encDesc);

    WGPURenderPassColorAttachment colorAtt = {};
    colorAtt.view = view;
    colorAtt.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;  // Required for non-3D textures
    colorAtt.loadOp = WGPULoadOp_Clear;
    colorAtt.storeOp = WGPUStoreOp_Store;
    colorAtt.clearValue = {0.0, 0.0, 0.3, 1.0}; // Dark blue background

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAtt;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    if (!pass) {
        printf("Failed to create render pass!\n");
        return;
    }
    wgpuRenderPassEncoderSetPipeline(pass, g_pipeline);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(g_queue, 1, &cmd);
    
    // No wgpuSurfacePresent on Emscripten - browser presents automatically
    
    if (frameCount < 3) {
        printf("Frame %d: submitted\n", frameCount);
    }
    frameCount++;

    wgpuCommandBufferRelease(cmd);
    wgpuCommandEncoderRelease(encoder);
    wgpuRenderPassEncoderRelease(pass);
    wgpuTextureViewRelease(view);
    wgpuTextureRelease(surfaceTexture.texture);
}

void mainLoop(void*) {
    render();
}

void initPipeline() {
    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code = {shaderCode, strlen(shaderCode)};

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslSource.chain;
    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(g_device, &shaderDesc);

    WGPUColorTargetState colorTarget = {};
    colorTarget.format = g_format;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragState = {};
    fragState.module = shader;
    fragState.entryPoint = {"fs_main", 7};
    fragState.targetCount = 1;
    fragState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.vertex.module = shader;
    pipelineDesc.vertex.entryPoint = {"vs_main", 7};
    pipelineDesc.fragment = &fragState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    g_pipeline = wgpuDeviceCreateRenderPipeline(g_device, &pipelineDesc);
    wgpuShaderModuleRelease(shader);
    
    printf("Pipeline created: %p\n", g_pipeline);
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView msg, void*, void*) {
    if (status != WGPURequestDeviceStatus_Success) {
        printf("Device request failed: %d\n", status);
        return;
    }
    
    g_device = device;
    g_queue = wgpuDeviceGetQueue(device);
    printf("Device ready: %p, Queue: %p\n", g_device, g_queue);

    // Configure surface
    WGPUSurfaceConfiguration config = {};
    config.device = g_device;
    config.format = g_format;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.width = 800;
    config.height = 600;
    config.presentMode = WGPUPresentMode_Fifo;
    config.alphaMode = WGPUCompositeAlphaMode_Opaque;
    wgpuSurfaceConfigure(g_surface, &config);
    
    initPipeline();
    
    emscripten_set_main_loop_arg(mainLoop, nullptr, 0, false);
}

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView msg, void*, void*) {
    if (status != WGPURequestAdapterStatus_Success) {
        printf("Adapter request failed: %d\n", status);
        return;
    }
    g_adapter = adapter;
    printf("Adapter ready: %p\n", adapter);
    
    // Query preferred format
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(g_surface, adapter, &caps);
    if (caps.formatCount > 0) {
        g_format = caps.formats[0];
        printf("Using surface format: %d\n", g_format);
    }

    WGPUDeviceDescriptor deviceDesc = {};
    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = onDeviceReady;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb);
}

int main() {
    printf("WebGPU Test Starting...\n");

    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);
    printf("Instance: %p\n", instance);

    // Get surface from canvas using emdawnwebgpu API
    WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource = {};
    canvasSource.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
    canvasSource.selector = { .data = "#canvas", .length = 7 };

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &canvasSource.chain;
    g_surface = wgpuInstanceCreateSurface(instance, &surfaceDesc);
    printf("Surface: %p\n", g_surface);

    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = g_surface;
    
    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = onAdapterReady;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCb);

    return 0;
}
