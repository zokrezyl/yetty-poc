/**
 * Card Runner - Standalone interactive card renderer
 *
 * Renders a single card without the terminal overhead.
 * Useful for fast iteration when developing card shaders.
 *
 * Usage: card-runner <card-name> "<args>"
 * Example: card-runner yplot "{f=sin(x) @range=-pi..pi,-1..1 @grid=on}"
 */

#include <args.hxx>
#include <yetty/wgpu-compat.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <ytrace/ytrace.hpp>

#include <iostream>
#include <chrono>
#include <cstring>

// Globals
static int g_windowWidth = 800;
static int g_windowHeight = 600;

static WGPUInstance g_instance = nullptr;
static WGPUSurface g_surface = nullptr;
static WGPUAdapter g_adapter = nullptr;
static WGPUDevice g_device = nullptr;
static WGPUQueue g_queue = nullptr;
static WGPUTextureFormat g_surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
static WGPURenderPipeline g_pipeline = nullptr;
static WGPUBuffer g_uniformBuffer = nullptr;
static WGPUBindGroup g_bindGroup = nullptr;
static WGPUBindGroupLayout g_bindGroupLayout = nullptr;

// Uniform data (must match shader struct)
struct Uniforms {
    float time;
    float width;
    float height;
    float _pad;
};

// Plot test shader - demonstrates yplot-like rendering directly
static const char* g_testShaderCode = R"(
// Uniforms for animation
struct Uniforms {
    time: f32,
    width: f32,
    height: f32,
    _pad: f32,
}

@group(0) @binding(0) var<uniform> uniforms: Uniforms;

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> @builtin(position) vec4f {
    // Fullscreen quad (2 triangles, 6 vertices)
    var pos = array<vec2f, 6>(
        vec2f(-1.0, -1.0),
        vec2f( 1.0, -1.0),
        vec2f( 1.0,  1.0),
        vec2f(-1.0, -1.0),
        vec2f( 1.0,  1.0),
        vec2f(-1.0,  1.0)
    );
    return vec4f(pos[idx], 0.0, 1.0);
}

// Plot constants
const PI: f32 = 3.14159265359;
const BG_COLOR: vec3f = vec3f(0.102, 0.102, 0.180);      // #1A1A2E
const GRID_COLOR: vec3f = vec3f(0.267, 0.267, 0.267);    // #444
const AXIS_COLOR: vec3f = vec3f(0.667, 0.667, 0.667);    // #AAA
const LINE_COLOR: vec3f = vec3f(1.0, 0.4, 0.2);          // Orange

@fragment
fn fs_main(@builtin(position) fragCoord: vec4f) -> @location(0) vec4f {
    let uv = fragCoord.xy / vec2f(uniforms.width, uniforms.height);

    // Margins (normalized)
    let marginLeft = 0.08;
    let marginRight = 0.05;
    let marginBottom = 0.1;
    let marginTop = 0.05;

    // Plot area
    let plotLeft = marginLeft;
    let plotRight = 1.0 - marginRight;
    let plotBottom = marginBottom;
    let plotTop = 1.0 - marginTop;
    let plotWidth = plotRight - plotLeft;
    let plotHeight = plotTop - plotBottom;

    // Check if in plot area
    let inPlot = uv.x >= plotLeft && uv.x <= plotRight &&
                 uv.y >= plotBottom && uv.y <= plotTop;

    // Start with background
    var color = BG_COLOR;

    if (inPlot) {
        // Map to plot UV (0-1 in plot area)
        let plotUV = vec2f(
            (uv.x - plotLeft) / plotWidth,
            (uv.y - plotBottom) / plotHeight
        );

        // Data range: x from -PI to PI, y from -1.5 to 1.5
        let xMin = -PI;
        let xMax = PI;
        let yMin = -1.5;
        let yMax = 1.5;

        let dataX = mix(xMin, xMax, plotUV.x);
        let dataY = mix(yMin, yMax, plotUV.y);

        // Draw grid
        let gridSpacing = 0.1;
        let gridX = fract(plotUV.x / gridSpacing);
        let gridY = fract(plotUV.y / gridSpacing);
        let lineWidth = 0.02;
        if (gridX < lineWidth || gridX > 1.0 - lineWidth ||
            gridY < lineWidth || gridY > 1.0 - lineWidth) {
            color = mix(color, GRID_COLOR, 0.5);
        }

        // Draw axes
        let axisWidth = 0.005;
        // Y-axis at x=0
        let xZero = (0.0 - xMin) / (xMax - xMin);
        if (abs(plotUV.x - xZero) < axisWidth) {
            color = AXIS_COLOR;
        }
        // X-axis at y=0
        let yZero = (0.0 - yMin) / (yMax - yMin);
        if (abs(plotUV.y - yZero) < axisWidth) {
            color = AXIS_COLOR;
        }

        // Evaluate sin(x + time) at this x
        let y = sin(dataX + uniforms.time);

        // Map y to plot UV
        let yNorm = (y - yMin) / (yMax - yMin);

        // Distance from pixel to curve
        let dist = abs(plotUV.y - yNorm);

        // Anti-aliased line
        let pixelSize = 1.0 / uniforms.height;
        let lineThickness = 3.0 * pixelSize / plotHeight;
        if (dist < lineThickness) {
            let alpha = 1.0 - dist / lineThickness;
            color = mix(color, LINE_COLOR, alpha);
        }

        // Second function: cos(x - time) in blue
        let y2 = cos(dataX - uniforms.time);
        let yNorm2 = (y2 - yMin) / (yMax - yMin);
        let dist2 = abs(plotUV.y - yNorm2);
        if (dist2 < lineThickness) {
            let alpha = 1.0 - dist2 / lineThickness;
            color = mix(color, vec3f(0.2, 0.6, 1.0), alpha);
        }

        // Border
        if (plotUV.x < axisWidth || plotUV.x > 1.0 - axisWidth ||
            plotUV.y < axisWidth || plotUV.y > 1.0 - axisWidth) {
            color = AXIS_COLOR * 0.7;
        }
    } else {
        // Margin area - slightly darker
        color = BG_COLOR * 0.8;
    }

    return vec4f(color, 1.0);
}
)";

static bool createPipeline() {
    // Create uniform buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = sizeof(Uniforms);
    bufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    bufDesc.mappedAtCreation = false;
    g_uniformBuffer = wgpuDeviceCreateBuffer(g_device, &bufDesc);
    if (!g_uniformBuffer) {
        yerror("Failed to create uniform buffer");
        return false;
    }

    // Create bind group layout
    WGPUBindGroupLayoutEntry layoutEntry = {};
    layoutEntry.binding = 0;
    layoutEntry.visibility = WGPUShaderStage_Fragment;
    layoutEntry.buffer.type = WGPUBufferBindingType_Uniform;
    layoutEntry.buffer.minBindingSize = sizeof(Uniforms);

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 1;
    layoutDesc.entries = &layoutEntry;
    g_bindGroupLayout = wgpuDeviceCreateBindGroupLayout(g_device, &layoutDesc);
    if (!g_bindGroupLayout) {
        yerror("Failed to create bind group layout");
        return false;
    }

    // Create bind group
    WGPUBindGroupEntry bindEntry = {};
    bindEntry.binding = 0;
    bindEntry.buffer = g_uniformBuffer;
    bindEntry.offset = 0;
    bindEntry.size = sizeof(Uniforms);

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = g_bindGroupLayout;
    bindGroupDesc.entryCount = 1;
    bindGroupDesc.entries = &bindEntry;
    g_bindGroup = wgpuDeviceCreateBindGroup(g_device, &bindGroupDesc);
    if (!g_bindGroup) {
        yerror("Failed to create bind group");
        return false;
    }

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &g_bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(g_device, &pipelineLayoutDesc);

    // Create shader module
    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code = {g_testShaderCode, strlen(g_testShaderCode)};

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslSource.chain;
    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(g_device, &shaderDesc);
    if (!shader) {
        yerror("Failed to create test shader module");
        return false;
    }

    // Color target state
    WGPUColorTargetState colorTarget = {};
    colorTarget.format = g_surfaceFormat;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    // Fragment state
    WGPUFragmentState fragState = {};
    fragState.module = shader;
    fragState.entryPoint = {"fs_main", 7};
    fragState.targetCount = 1;
    fragState.targets = &colorTarget;

    // Pipeline descriptor
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shader;
    pipelineDesc.vertex.entryPoint = {"vs_main", 7};
    pipelineDesc.fragment = &fragState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    g_pipeline = wgpuDeviceCreateRenderPipeline(g_device, &pipelineDesc);
    wgpuShaderModuleRelease(shader);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!g_pipeline) {
        yerror("Failed to create test pipeline");
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    // Parse arguments
    args::ArgumentParser parser("Card Runner - standalone card renderer");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::Positional<std::string> cardName(parser, "card", "Card type (e.g. yplot, ydraw)");
    args::Positional<std::string> cardArgs(parser, "args", "Card arguments");
    args::ValueFlag<int> widthFlag(parser, "width", "Window width", {'W', "width"}, 800);
    args::ValueFlag<int> heightFlag(parser, "height", "Window height", {'H', "height"}, 600);
    args::ValueFlag<int> cellsW(parser, "cells-w", "Card width in cells", {'w'}, 60);
    args::ValueFlag<int> cellsH(parser, "cells-h", "Card height in cells", {'h'}, 30);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << "\n" << parser;
        return 1;
    }

    if (!cardName) {
        std::cerr << "Error: card type required\n" << parser;
        return 1;
    }

    g_windowWidth = args::get(widthFlag);
    g_windowHeight = args::get(heightFlag);
    uint32_t cardWidthCells = static_cast<uint32_t>(args::get(cellsW));
    uint32_t cardHeightCells = static_cast<uint32_t>(args::get(cellsH));
    std::string card = args::get(cardName);
    std::string argsStr = cardArgs ? args::get(cardArgs) : "";

    yinfo("Card Runner: card='{}' args='{}' size={}x{} cells", card, argsStr,
          cardWidthCells, cardHeightCells);

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight,
                                          ("Card Runner: " + card).c_str(),
                                          nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    // Create WebGPU instance
    WGPUInstanceDescriptor instanceDesc = {};
    g_instance = wgpuCreateInstance(&instanceDesc);
    if (!g_instance) {
        std::cerr << "Failed to create WebGPU instance\n";
        return 1;
    }

    // Create surface from GLFW window
    g_surface = glfwCreateWindowWGPUSurface(g_instance, window);
    if (!g_surface) {
        std::cerr << "Failed to create WebGPU surface\n";
        return 1;
    }

    // Request adapter (synchronous via callback)
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = g_surface;
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                            WGPUStringView message, void* userdata1, void* userdata2) {
        (void)message; (void)userdata2;
        if (status == WGPURequestAdapterStatus_Success) {
            *static_cast<WGPUAdapter*>(userdata1) = adapter;
        }
    };
    adapterCb.userdata1 = &g_adapter;
    wgpuInstanceRequestAdapter(g_instance, &adapterOpts, adapterCb);

    // Poll until adapter is ready
    while (!g_adapter) {
        wgpuInstanceProcessEvents(g_instance);
    }

    if (!g_adapter) {
        std::cerr << "Failed to get WebGPU adapter\n";
        return 1;
    }
    yinfo("Card Runner: adapter obtained");

    // Request device
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = WGPU_STR("card-runner device");

    // Set up error callback in device descriptor
    deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const* device, WGPUErrorType type,
                                                          WGPUStringView message, void* userdata1, void* userdata2) {
        (void)device; (void)userdata1; (void)userdata2;
        std::cerr << "WebGPU error (" << static_cast<int>(type) << "): "
                  << (message.data ? std::string(message.data, message.length) : "unknown") << "\n";
    };

    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                           WGPUStringView message, void* userdata1, void* userdata2) {
        (void)message; (void)userdata2;
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(userdata1) = device;
        }
    };
    deviceCb.userdata1 = &g_device;
    wgpuAdapterRequestDevice(g_adapter, &deviceDesc, deviceCb);

    while (!g_device) {
        wgpuInstanceProcessEvents(g_instance);
    }

    if (!g_device) {
        std::cerr << "Failed to get WebGPU device\n";
        return 1;
    }
    yinfo("Card Runner: device obtained");

    g_queue = wgpuDeviceGetQueue(g_device);

    // Configure surface
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(g_surface, g_adapter, &caps);
    if (caps.formatCount > 0) {
        g_surfaceFormat = caps.formats[0];
    }

    WGPUSurfaceConfiguration surfaceConfig = {};
    surfaceConfig.device = g_device;
    surfaceConfig.format = g_surfaceFormat;
    surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
    surfaceConfig.width = static_cast<uint32_t>(g_windowWidth);
    surfaceConfig.height = static_cast<uint32_t>(g_windowHeight);
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    wgpuSurfaceConfigure(g_surface, &surfaceConfig);

    // Create pipeline and resources
    if (!createPipeline()) {
        std::cerr << "Failed to create pipeline\n";
        return 1;
    }
    yinfo("Card Runner: pipeline created");

    // Main render loop
    yinfo("Card Runner: entering render loop");
    yinfo("Card Runner: rendering animated sin/cos plot demo");
    yinfo("Card Runner: TODO: integrate actual '{}' card rendering", card);

    auto startTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Update window size
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        if (w != g_windowWidth || h != g_windowHeight) {
            g_windowWidth = w;
            g_windowHeight = h;
            surfaceConfig.width = static_cast<uint32_t>(w);
            surfaceConfig.height = static_cast<uint32_t>(h);
            wgpuSurfaceConfigure(g_surface, &surfaceConfig);
        }

        // Update uniforms
        auto now = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float>(now - startTime).count();

        Uniforms uniforms = {
            elapsed,
            static_cast<float>(g_windowWidth),
            static_cast<float>(g_windowHeight),
            0.0f
        };
        wgpuQueueWriteBuffer(g_queue, g_uniformBuffer, 0, &uniforms, sizeof(uniforms));

        // Get surface texture
        WGPUSurfaceTexture surfaceTexture;
        wgpuSurfaceGetCurrentTexture(g_surface, &surfaceTexture);
        if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
            surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
            continue;
        }

        WGPUTextureView textureView = wgpuTextureCreateView(surfaceTexture.texture, nullptr);

        // Create command encoder
        WGPUCommandEncoderDescriptor encDesc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_device, &encDesc);

        // Render pass
        WGPURenderPassColorAttachment colorAttachment = {};
        colorAttachment.view = textureView;
        colorAttachment.loadOp = WGPULoadOp_Clear;
        colorAttachment.storeOp = WGPUStoreOp_Store;
        colorAttachment.clearValue = {0.1, 0.1, 0.15, 1.0};
        colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

        WGPURenderPassDescriptor passDesc = {};
        passDesc.colorAttachmentCount = 1;
        passDesc.colorAttachments = &colorAttachment;

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);

        // Draw
        wgpuRenderPassEncoderSetPipeline(pass, g_pipeline);
        wgpuRenderPassEncoderSetBindGroup(pass, 0, g_bindGroup, 0, nullptr);
        wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);

        // Submit
        WGPUCommandBufferDescriptor cmdBufDesc = {};
        WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
        wgpuQueueSubmit(g_queue, 1, &cmdBuf);

        // Present
        wgpuSurfacePresent(g_surface);

        // Cleanup
        wgpuCommandBufferRelease(cmdBuf);
        wgpuCommandEncoderRelease(encoder);
        wgpuTextureViewRelease(textureView);
        wgpuTextureRelease(surfaceTexture.texture);
    }

    // Cleanup
    yinfo("Card Runner: cleaning up");

    if (g_bindGroup) wgpuBindGroupRelease(g_bindGroup);
    if (g_bindGroupLayout) wgpuBindGroupLayoutRelease(g_bindGroupLayout);
    if (g_uniformBuffer) wgpuBufferRelease(g_uniformBuffer);
    if (g_pipeline) wgpuRenderPipelineRelease(g_pipeline);
    wgpuQueueRelease(g_queue);
    wgpuDeviceRelease(g_device);
    wgpuAdapterRelease(g_adapter);
    wgpuSurfaceRelease(g_surface);
    wgpuInstanceRelease(g_instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    yinfo("Card Runner: exit");
    return 0;
}
