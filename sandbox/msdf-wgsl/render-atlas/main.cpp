/**
 * MSDF-WGSL Atlas Viewer - Debug tool to visualize the atlas texture
 */

#include <msdf-wgsl.h>
#include <yetty/wgpu-compat.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <iostream>
#include <vector>
#include <cstring>

int windowWidth = 1280;
int windowHeight = 720;

WGPUInstance instance = nullptr;
WGPUSurface surface = nullptr;
WGPUAdapter adapter = nullptr;
WGPUDevice device = nullptr;
WGPUQueue queue = nullptr;
WGPUTextureFormat surfaceFormat = WGPUTextureFormat_BGRA8Unorm;

std::unique_ptr<msdf::Context> msdfContext;
std::unique_ptr<msdf::Font> font;

WGPURenderPipeline pipeline = nullptr;
WGPUBindGroup bindGroup = nullptr;
WGPUSampler sampler = nullptr;
WGPUBuffer uniformBuffer = nullptr;

// Pan and zoom state
float panX = 0.0f;
float panY = 0.0f;
float zoom = 1.0f;  // Will be set based on atlas size after loading
bool dragging = false;
double lastMouseX = 0, lastMouseY = 0;

bool adapterReady = false;
bool deviceReady = false;

// Simple shader that renders the atlas texture to a fullscreen quad
const char* shaderCode = R"(
struct Uniforms {
    pan: vec2<f32>,
    zoom: f32,
    aspect: f32,
    atlas_aspect: f32,
    _padding: vec3<f32>,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var atlasSampler: sampler;
@group(0) @binding(2) var atlasTexture: texture_2d<f32>;

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
    // Screen coordinates centered at 0,0
    var screen = (in.uv - 0.5) * 2.0;  // -1 to 1

    // Apply window aspect ratio (so circles stay circular)
    screen.x *= uniforms.aspect;

    // Apply zoom (higher zoom = see more)
    screen = screen * uniforms.zoom;

    // Apply pan
    screen = screen + uniforms.pan * 2.0;

    // Convert to atlas UV space, accounting for atlas aspect ratio
    var uv = screen;
    uv.x = uv.x / uniforms.atlas_aspect;  // Scale X by atlas aspect
    uv = uv * 0.5 + 0.5;  // Back to 0-1 range

    // Clamp UV to valid range for sampling
    let sample_uv = clamp(uv, vec2<f32>(0.0), vec2<f32>(1.0));

    // Sample atlas
    let msdf = textureSample(atlasTexture, atlasSampler, sample_uv);

    // Compute median of RGB channels (standard MSDF decoding)
    let r = msdf.r;
    let g = msdf.g;
    let b = msdf.b;
    let median = max(min(r, g), min(max(r, g), b));

    // Check bounds
    let in_bounds = uv.x >= 0.0 && uv.x <= 1.0 && uv.y >= 0.0 && uv.y <= 1.0;
    let bg_color = vec4<f32>(0.15, 0.15, 0.15, 1.0);

    // Display median as grayscale
    return select(bg_color, vec4<f32>(median, median, median, 1.0), in_bounds);
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

void onUncapturedError(WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
    const char* typeStr = "Unknown";
    switch(type) {
        case WGPUErrorType_Validation: typeStr = "Validation"; break;
        case WGPUErrorType_OutOfMemory: typeStr = "OutOfMemory"; break;
        case WGPUErrorType_Internal: typeStr = "Internal"; break;
        default: break;
    }
    std::cerr << "[WebGPU ERROR] " << typeStr << ": ";
    if (message.data && message.length > 0) {
        std::cerr << std::string(message.data, message.length);
    }
    std::cerr << std::endl;
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

    // Set up error callback in device descriptor
    WGPUUncapturedErrorCallbackInfo errorInfo = {};
    errorInfo.callback = onUncapturedError;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.uncapturedErrorCallbackInfo = errorInfo;

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
    surfaceConfig.width = windowWidth;
    surfaceConfig.height = windowHeight;
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(surface, &surfaceConfig);

    std::cout << "[WebGPU] Initialized" << std::endl;
    return true;
}

bool initRendering() {
    auto* atlas = font->getAtlas();
    if (!atlas) return false;

    std::cout << "[Render] Atlas size: " << atlas->getWidth() << "x"
              << atlas->getHeight() << std::endl;

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = WGPU_STR(shaderCode);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);

    // Create sampler - using nearest filter since RGBA32Float is unfilterable
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.magFilter = WGPUFilterMode_Nearest;
    samplerDesc.minFilter = WGPUFilterMode_Nearest;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;
    sampler = wgpuDeviceCreateSampler(device, &samplerDesc);

    // Create uniform buffer (48 bytes due to WGSL alignment rules)
    WGPUBufferDescriptor uniformDesc = {};
    uniformDesc.size = 48;
    uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    uniformBuffer = wgpuDeviceCreateBuffer(device, &uniformDesc);

    // Create bind group layout
    WGPUBindGroupLayoutEntry layoutEntries[3] = {};

    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Fragment;
    layoutEntries[0].buffer.type = WGPUBufferBindingType_Uniform;

    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Fragment;
    layoutEntries[1].sampler.type = WGPUSamplerBindingType_NonFiltering;

    layoutEntries[2].binding = 2;
    layoutEntries[2].visibility = WGPUShaderStage_Fragment;
    layoutEntries[2].texture.sampleType = WGPUTextureSampleType_UnfilterableFloat;
    layoutEntries[2].texture.viewDimension = WGPUTextureViewDimension_2D;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 3;
    layoutDesc.entries = layoutEntries;
    WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);

    // Create bind group
    WGPUBindGroupEntry bindEntries[3] = {};

    bindEntries[0].binding = 0;
    bindEntries[0].buffer = uniformBuffer;
    bindEntries[0].size = 48;

    bindEntries[1].binding = 1;
    bindEntries[1].sampler = sampler;

    bindEntries[2].binding = 2;
    bindEntries[2].textureView = atlas->getTextureView();

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = 3;
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

void updateUniforms() {
    auto* atlas = font->getAtlas();
    float atlasAspect = static_cast<float>(atlas->getWidth()) / static_cast<float>(atlas->getHeight());
    float windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    // Must match WGSL alignment: vec3 requires 16-byte alignment
    struct alignas(16) {
        float panX, panY;       // 8 bytes at offset 0
        float zoom;             // 4 bytes at offset 8
        float aspect;           // 4 bytes at offset 12
        float atlasAspect;      // 4 bytes at offset 16
        float _pad1[3];         // 12 bytes at offset 20 (aligns next to 32)
        float padding[4];       // 16 bytes at offset 32 (vec3 + struct padding)
    } uniforms = {panX, panY, zoom, windowAspect, atlasAspect, {0, 0, 0}, {0, 0, 0, 0}};

    wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &uniforms, 48);
}

void render() {
    updateUniforms();

    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) return;

    // Pass nullptr to use default view descriptor
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, nullptr);

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
    if (uniformBuffer) wgpuBufferRelease(uniformBuffer);
    if (bindGroup) wgpuBindGroupRelease(bindGroup);
    if (pipeline) wgpuRenderPipelineRelease(pipeline);
    if (sampler) wgpuSamplerRelease(sampler);
    msdfContext.reset();
    font.reset();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return;  // Minimized

    windowWidth = width;
    windowHeight = height;

    // Reconfigure surface
    WGPUSurfaceConfiguration surfaceConfig = {};
    surfaceConfig.device = device;
    surfaceConfig.format = surfaceFormat;
    surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
    surfaceConfig.width = windowWidth;
    surfaceConfig.height = windowHeight;
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(surface, &surfaceConfig);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Get mouse position in normalized coordinates (-1 to 1)
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float normX = (static_cast<float>(mouseX) / windowWidth - 0.5f) * 2.0f;
    float normY = -(static_cast<float>(mouseY) / windowHeight - 0.5f) * 2.0f;  // Flip Y

    // Apply window aspect to get screen coordinates
    normX *= static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    // Calculate point in world space before zoom
    float worldX = normX * zoom + panX;
    float worldY = normY * zoom + panY;

    // Apply zoom
    float zoomFactor = (yoffset > 0) ? 0.8f : 1.25f;  // Scroll up = zoom in (smaller zoom value)
    float newZoom = zoom * zoomFactor;
    newZoom = std::max(0.1f, std::min(newZoom, 200.0f));

    // Adjust pan so the world point stays under the cursor
    panX = worldX - normX * newZoom;
    panY = worldY - normY * newZoom;

    zoom = newZoom;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        dragging = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging) {
        float dx = static_cast<float>(xpos - lastMouseX) / windowWidth;
        float dy = static_cast<float>(ypos - lastMouseY) / windowHeight;
        panX += dx / zoom;
        panY -= dy / zoom;
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

void printUsage(const char* prog) {
    std::cout << "MSDF-WGSL Atlas Viewer\n\n"
              << "Usage: " << prog << " [options]\n\n"
              << "Options:\n"
              << "  -f, --font FILE    Font file to load (default: system DejaVuSansMono)\n"
              << "  -n, --num N        Number of glyphs to generate (default: 95 - printable ASCII)\n"
              << "  --nerd             Include Nerd Font symbols\n"
              << "  -h, --help         Show this help\n\n"
              << "Controls:\n"
              << "  Mouse drag         Pan the view\n"
              << "  Mouse wheel        Zoom in/out\n"
              << "  R                  Reset view\n"
              << "  ESC                Exit\n";
}

int main(int argc, char* argv[]) {
    std::string fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
    int numGlyphs = 95;  // Printable ASCII
    bool nerdFonts = false;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if ((arg == "-f" || arg == "--font") && i + 1 < argc) {
            fontPath = argv[++i];
        } else if ((arg == "-n" || arg == "--num") && i + 1 < argc) {
            numGlyphs = std::atoi(argv[++i]);
        } else if (arg == "--nerd") {
            nerdFonts = true;
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }

    std::cout << "========================================\n"
              << "MSDF-WGSL Atlas Viewer\n"
              << "========================================\n"
              << "Font: " << fontPath << "\n"
              << "Glyphs: " << numGlyphs << "\n"
              << "========================================\n" << std::endl;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "MSDF-WGSL Atlas Viewer", nullptr, nullptr);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    if (!initWebGPU(window)) {
        std::cerr << "Failed to initialize WebGPU" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Set shader path relative to executable
    msdf::setShaderPath("shaders/msdf_gen.wgsl");

    // Create MSDF context
    try {
        msdfContext = std::make_unique<msdf::Context>(device, instance);
    } catch (const std::exception& e) {
        std::cerr << "Failed to create MSDF context: " << e.what() << std::endl;
        glfwTerminate();
        return 1;
    }

    // Load font
    msdf::FontConfig config;
    config.scale = 2.0f;
    config.range = 4.0f;
    font = msdfContext->loadFont(fontPath, config);
    if (!font) {
        std::cerr << "Failed to load font: " << fontPath << std::endl;
        glfwTerminate();
        return 1;
    }

    std::cout << "[Font] Loaded, generating glyphs..." << std::endl;

    // Generate glyphs
    std::vector<uint32_t> charset;
    if (nerdFonts) {
        charset = msdf::getDefaultCharset(true);
        if (numGlyphs > 0 && static_cast<size_t>(numGlyphs) < charset.size()) {
            charset.resize(numGlyphs);
        }
    } else {
        // Printable ASCII
        for (uint32_t c = 0x20; c <= 0x7E && charset.size() < static_cast<size_t>(numGlyphs); ++c) {
            charset.push_back(c);
        }
    }

    int result = msdfContext->generateGlyphs(*font, charset);
    std::cout << "[Font] Generated " << result << " glyphs" << std::endl;

    // Set initial zoom to fit atlas height in window
    auto* atlas = font->getAtlas();
    float atlasAspect = static_cast<float>(atlas->getWidth()) / static_cast<float>(atlas->getHeight());
    zoom = atlasAspect / 5.0f;  // Show about 1/5 of the atlas width
    std::cout << "[View] Atlas aspect: " << atlasAspect << ", initial zoom: " << zoom << std::endl;

    // Initialize rendering
    if (!initRendering()) {
        std::cerr << "Failed to initialize rendering" << std::endl;
        cleanup();
        glfwTerminate();
        return 1;
    }

    std::cout << "[Main] Starting render loop...\n"
              << "  Drag to pan, scroll to zoom, R to reset, ESC to exit\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            panX = panY = 0.0f;
            auto* atlas = font->getAtlas();
            float atlasAspect = static_cast<float>(atlas->getWidth()) / static_cast<float>(atlas->getHeight());
            zoom = atlasAspect / 5.0f;
        }

        render();
    }

    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
