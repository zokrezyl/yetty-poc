// Minimal test: Create GPUScreen, set cells directly, render
#include <emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu.h>
#include <cstdio>

#include "yetty/gpu-screen.h"
#include "yetty/font-manager.h"
#include "yetty/shader-manager.h"
#include "yetty/gpu-allocator.h"
#include "yetty/yetty-context.h"

using namespace yetty;

WGPUDevice g_device = nullptr;
WGPUQueue g_queue = nullptr;
WGPUSurface g_surface = nullptr;
WGPUAdapter g_adapter = nullptr;
WGPUTextureFormat g_format = WGPUTextureFormat_BGRA8Unorm;

GPUScreen::Ptr g_screen;
YettyContext g_ctx;

void render() {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(g_surface, &surfaceTexture);
    
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        return;
    }

    WGPUTextureView view = wgpuTextureCreateView(surfaceTexture.texture, nullptr);

    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_device, &encDesc);

    WGPURenderPassColorAttachment colorAtt = {};
    colorAtt.view = view;
    colorAtt.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    colorAtt.loadOp = WGPULoadOp_Clear;
    colorAtt.storeOp = WGPUStoreOp_Store;
    colorAtt.clearValue = {0.1, 0.1, 0.2, 1.0};

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAtt;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    
    // Render GPUScreen
    if (g_screen) {
        g_screen->render(pass);
    }
    
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmdDesc = {};
    WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(g_queue, 1, &cmd);

    wgpuCommandBufferRelease(cmd);
    wgpuCommandEncoderRelease(encoder);
    wgpuRenderPassEncoderRelease(pass);
    wgpuTextureViewRelease(view);
    wgpuTextureRelease(surfaceTexture.texture);
}

void mainLoop(void*) {
    render();
}

void initScreen() {
    printf("Creating GPUScreen...\n");
    
    // Create screen
    auto screenResult = GPUScreen::create(g_ctx);
    if (!screenResult) {
        printf("Failed to create GPUScreen!\n");
        return;
    }
    g_screen = *screenResult;
    printf("GPUScreen created: %p\n", g_screen.get());
    
    // Resize to allocate buffers
    g_screen->resize(80, 24);
    printf("GPUScreen resized to 80x24\n");
    
    // Write some text directly
    g_screen->write("Hello from GPUScreen test!\r\n", 28);
    printf("Text written to GPUScreen\n");
    
    // Start render loop
    emscripten_set_main_loop_arg(mainLoop, nullptr, 0, false);
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView msg, void*, void*) {
    if (status != WGPURequestDeviceStatus_Success) {
        printf("Device request failed\n");
        return;
    }
    
    g_device = device;
    g_queue = wgpuDeviceGetQueue(device);
    printf("Device ready\n");

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
    
    // Setup YettyContext
    g_ctx.gpu.device = g_device;
    g_ctx.gpu.queue = g_queue;
    
    // Create allocator
    auto allocResult = GpuAllocator::create(g_device);
    if (allocResult) {
        g_ctx.globalAllocator = *allocResult;
    }
    
    // Create shader manager
    auto shaderResult = ShaderManager::create(g_device);
    if (shaderResult) {
        g_ctx.shaderManager = *shaderResult;
    }
    
    // Create font manager
    auto fontResult = FontManager::create(g_ctx.gpu, g_ctx.globalAllocator, g_ctx.shaderManager, nullptr);
    if (fontResult) {
        g_ctx.fontManager = *fontResult;
        printf("FontManager created\n");
    } else {
        printf("FontManager creation failed!\n");
    }
    
    initScreen();
}

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView msg, void*, void*) {
    if (status != WGPURequestAdapterStatus_Success) {
        printf("Adapter request failed\n");
        return;
    }
    g_adapter = adapter;
    printf("Adapter ready\n");
    
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(g_surface, adapter, &caps);
    if (caps.formatCount > 0) {
        g_format = caps.formats[0];
    }

    WGPUDeviceDescriptor deviceDesc = {};
    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = onDeviceReady;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb);
}

int main() {
    printf("GPUScreen Test Starting...\n");

    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);

    WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource = {};
    canvasSource.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
    canvasSource.selector = { .data = "#canvas", .length = 7 };

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &canvasSource.chain;
    g_surface = wgpuInstanceCreateSurface(instance, &surfaceDesc);

    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = g_surface;
    
    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = onAdapterReady;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCb);

    return 0;
}
