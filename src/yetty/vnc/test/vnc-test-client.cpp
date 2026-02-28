// VNC Test Client - Independent test client with input generation and stats
// Uses libuv for event loop integration with VncClient
// Usage: vnc-test-client [--host HOST] [--port PORT]

#include "../vnc-client.h"
#include "../protocol.h"
#include <yetty/base/event-loop.h>
#include <yetty/wgpu-compat.h>
#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>
#include <args.hxx>
#include <webgpu/webgpu.h>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <uv.h>

#include <chrono>
#include <thread>
#include <random>
#include <csignal>
#include <cstring>
#include <atomic>
#include <sys/socket.h>

using namespace yetty::vnc;

namespace {

std::atomic<bool> g_running{true};

void signalHandler(int) {
    yinfo("Received signal, stopping...");
    g_running = false;
}

// Client state for GLFW callbacks
struct ClientState {
    VncClient* client = nullptr;
    WGPUSurface surface = nullptr;
    WGPUDevice device = nullptr;
    WGPUTextureFormat surfaceFormat = WGPUTextureFormat_BGRA8Unorm;
    int windowWidth = 1280;
    int windowHeight = 720;
    uint8_t cellHeight = 20;  // Default cell height
    bool needsReconfigure = false;
    static constexpr uint8_t MIN_CELL_HEIGHT = 8;
    static constexpr uint8_t MAX_CELL_HEIGHT = 64;
};
ClientState g_clientState;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    if (width > 0 && height > 0) {
        yinfo("Window resized to {}x{}", width, height);
        g_clientState.windowWidth = width;
        g_clientState.windowHeight = height;
        g_clientState.needsReconfigure = true;
        if (g_clientState.client) {
            g_clientState.client->sendResize(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
        }
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Ctrl+wheel = zoom (change cell size)
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        if (g_clientState.client) {
            int delta = (yoffset > 0) ? 2 : -2;  // Zoom step
            int newHeight = g_clientState.cellHeight + delta;
            newHeight = std::max((int)ClientState::MIN_CELL_HEIGHT,
                                 std::min((int)ClientState::MAX_CELL_HEIGHT, newHeight));
            if (newHeight != g_clientState.cellHeight) {
                g_clientState.cellHeight = static_cast<uint8_t>(newHeight);
                yinfo("Zoom: cellHeight={}", g_clientState.cellHeight);
                g_clientState.client->sendCellSize(g_clientState.cellHeight);
            }
        }
    }
}

// Stats tracking
struct Stats {
    uint64_t framesRendered = 0;
    uint64_t inputsSent = 0;
    double lastReportTime = 0;
    double startTime = 0;

    void report(double now) {
        double elapsed = now - lastReportTime;
        if (elapsed >= 1.0) {
            double fps = framesRendered / (now - startTime);
            yinfo("Stats: frames={} inputs={} fps={:.1f}",
                  framesRendered, inputsSent, fps);
            lastReportTime = now;
        }
    }
};

// Input generator - creates synthetic keyboard and mouse events
class InputGenerator {
public:
    InputGenerator() : _rng(std::random_device{}()) {}

    void setClient(VncClient* client) { _client = client; }

    void update(double time, int windowWidth, int windowHeight) {
        if (!_client) return;

        // Vary input rate based on time - create "waves"
        double wave = std::sin(time * 0.5) * 0.5 + 0.5;  // 0 to 1
        double inputsPerSecond = 5 + wave * 45;  // 5 to 50 inputs/sec

        double interval = 1.0 / inputsPerSecond;
        if (time - _lastInputTime < interval) return;
        _lastInputTime = time;

        // Mostly send text input to see it on terminal
        std::uniform_int_distribution<int> typeDist(0, 10);
        int type = typeDist(_rng);

        if (type < 2) {
            sendMouseMove(windowWidth, windowHeight);
        } else {
            // 80% text input
            sendTextInput();
        }
    }

    uint64_t inputCount() const { return _inputCount; }

private:
    void sendMouseMove(int w, int h) {
        std::uniform_int_distribution<int> xDist(0, w - 1);
        std::uniform_int_distribution<int> yDist(0, h - 1);
        _client->sendMouseMove(xDist(_rng), yDist(_rng));
        _inputCount++;
    }

    void sendMouseClick(int w, int h) {
        std::uniform_int_distribution<int> xDist(0, w - 1);
        std::uniform_int_distribution<int> yDist(0, h - 1);
        std::uniform_int_distribution<int> btnDist(0, 2);

        int16_t x = xDist(_rng);
        int16_t y = yDist(_rng);
        MouseButton btn = static_cast<MouseButton>(btnDist(_rng));

        _client->sendMouseButton(x, y, btn, true);
        _client->sendMouseButton(x, y, btn, false);
        _inputCount++;
    }

    void sendKeyPress() {
        std::uniform_int_distribution<int> keyDist('a', 'z');
        uint32_t key = keyDist(_rng);
        _client->sendKeyDown(key, key, 0);
        _client->sendKeyUp(key, key, 0);
        _inputCount++;
    }

    void sendTextInput() {
        std::uniform_int_distribution<int> lenDist(3, 8);
        std::uniform_int_distribution<int> charDist('A', 'Z');

        int len = lenDist(_rng);
        // Red color + ">>>" prefix to clearly identify client text
        std::string text = "\033[1;31m>>>CLIENT>>> ";
        for (int i = 0; i < len; i++) {
            text += static_cast<char>(charDist(_rng));
        }
        text += "\033[0m\n";  // Reset color + newline
        yinfo("SENDING TEXT: {}", text);
        _client->sendTextInput(text.data(), text.size());
        _inputCount++;
    }

    VncClient* _client = nullptr;
    std::mt19937 _rng;
    double _lastInputTime = 0;
    uint64_t _inputCount = 0;
};

} // anonymous namespace

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    // Parse arguments
    args::ArgumentParser parser("vnc-test-client", "VNC test client with input generation");
    args::HelpFlag help(parser, "help", "Show help", {'h', "help"});
    args::ValueFlag<std::string> hostFlag(parser, "host", "Server host (default localhost)", {"host"}, "localhost");
    args::ValueFlag<uint16_t> portFlag(parser, "port", "Server port (default 5900)", {'p', "port"}, 5900);
    args::Flag noInputFlag(parser, "no-input", "Disable input generation", {"no-input"});
    args::Flag testRenderFlag(parser, "test-render", "Test rendering with random bitmap (no server needed)", {"test-render"});
    args::ValueFlag<int> widthFlag(parser, "width", "Window width (default 1280)", {'W', "width"}, 1280);
    args::ValueFlag<int> heightFlag(parser, "height", "Window height (default 720)", {'H', "height"}, 720);

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::string host = args::get(hostFlag);
    uint16_t port = args::get(portFlag);
    bool generateInput = !args::get(noInputFlag);
    bool testRender = args::get(testRenderFlag);
    int windowWidth = args::get(widthFlag);
    int windowHeight = args::get(heightFlag);

    yinfo("=== VNC Test Client ===");
    if (testRender) {
        yinfo("TEST RENDER MODE - no server connection, random bitmap");
    } else {
        yinfo("Server: {}:{}", host, port);
    }
    yinfo("Window: {}x{}", windowWidth, windowHeight);
    yinfo("Input generation: {}", generateInput ? "enabled" : "disabled");

    // Signal handling
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // Initialize GLFW
    if (!glfwInit()) {
        yerror("Failed to initialize GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "VNC Test Client", nullptr, nullptr);
    if (!window) {
        const char* errDesc;
        int errCode = glfwGetError(&errDesc);
        yerror("Failed to create window: {} ({})", errDesc ? errDesc : "unknown", errCode);
        glfwTerminate();
        return 1;
    }
    yinfo("Window created: {}", (void*)window);
    glfwShowWindow(window);

    // Setup GLFW callbacks for resize and zoom
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Create WebGPU instance
    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);
    if (!instance) {
        yerror("Failed to create WebGPU instance");
        return 1;
    }

    // Create surface
    WGPUSurface surface = glfwCreateWindowWGPUSurface(instance, window);
    if (!surface) {
        yerror("Failed to create surface");
        return 1;
    }
    yinfo("Surface created: {}", (void*)surface);

    // Request adapter
    WGPUAdapter adapter = nullptr;
    bool adapterReady = false;
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = surface;

    struct AdapterData { WGPUAdapter* adapter; bool* ready; };
    AdapterData adapterData = {&adapter, &adapterReady};

    WGPURequestAdapterCallbackInfo adapterCb = {};
    adapterCb.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCb.callback = [](WGPURequestAdapterStatus status, WGPUAdapter result,
                            WGPUStringView, void* userdata1, void*) {
        auto* data = static_cast<AdapterData*>(userdata1);
        if (status == WGPURequestAdapterStatus_Success) {
            *data->adapter = result;
        }
        *data->ready = true;
    };
    adapterCb.userdata1 = &adapterData;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCb);

    // Wait for adapter
    while (!adapterReady) { glfwPollEvents(); }
    if (!adapter) {
        yerror("Failed to get adapter");
        return 1;
    }
    yinfo("Got adapter");

    // Request device with error callback
    WGPUDevice device = nullptr;
    bool deviceReady = false;

    // Set up uncaptured error callback
    WGPUUncapturedErrorCallbackInfo errorInfo = {};
    errorInfo.callback = [](WGPUDevice const*, WGPUErrorType type, WGPUStringView message, void*, void*) {
        yerror("WebGPU error (type {}): {}", (int)type,
               message.data ? std::string(message.data, message.length) : "no message");
    };

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.uncapturedErrorCallbackInfo = errorInfo;

    struct DeviceData { WGPUDevice* device; bool* ready; };
    DeviceData deviceData = {&device, &deviceReady};

    WGPURequestDeviceCallbackInfo deviceCb = {};
    deviceCb.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCb.callback = [](WGPURequestDeviceStatus status, WGPUDevice result,
                           WGPUStringView message, void* userdata1, void*) {
        auto* data = static_cast<DeviceData*>(userdata1);
        if (status == WGPURequestDeviceStatus_Success) {
            *data->device = result;
        } else {
            yerror("Device request failed (status {}): {}", (int)status,
                   message.data ? std::string(message.data, message.length) : "no message");
        }
        *data->ready = true;
    };
    deviceCb.userdata1 = &deviceData;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCb);

    // Wait for device
    while (!deviceReady) { glfwPollEvents(); }
    if (!device) {
        yerror("Failed to get device");
        return 1;
    }
    yinfo("Got device");

    WGPUQueue queue = wgpuDeviceGetQueue(device);

    // Get surface format
    WGPUSurfaceCapabilities caps = {};
    WGPUStatus capStatus = wgpuSurfaceGetCapabilities(surface, adapter, &caps);
    if (capStatus != WGPUStatus_Success) {
        yerror("Failed to get surface capabilities: {}", (int)capStatus);
        return 1;
    }
    if (caps.formatCount == 0) {
        yerror("No surface formats available!");
        return 1;
    }
    WGPUTextureFormat surfaceFormat = caps.formats[0];
    yinfo("Surface format: {}, present modes: {}", (int)surfaceFormat, caps.presentModeCount);

    // Configure surface
    WGPUSurfaceConfiguration surfaceConfig = {};
    surfaceConfig.device = device;
    surfaceConfig.format = surfaceFormat;
    surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
    surfaceConfig.width = windowWidth;
    surfaceConfig.height = windowHeight;
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(surface, &surfaceConfig);

    // Verify surface works by getting a test texture
    WGPUSurfaceTexture testSurfaceTex;
    wgpuSurfaceGetCurrentTexture(surface, &testSurfaceTex);
    if (testSurfaceTex.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        testSurfaceTex.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        yerror("Surface test failed! Status: {}", (int)testSurfaceTex.status);
        return 1;
    }
    yinfo("Surface test OK, texture={}", (void*)testSurfaceTex.texture);

    // Store in client state for resize handling
    g_clientState.surface = surface;
    g_clientState.device = device;
    g_clientState.surfaceFormat = surfaceFormat;
    g_clientState.windowWidth = windowWidth;
    g_clientState.windowHeight = windowHeight;

    yinfo("WebGPU initialized");

    // Test render mode: create random bitmap texture directly
    WGPUTexture testTexture = nullptr;
    WGPUTextureView testTextureView = nullptr;
    WGPUSampler testSampler = nullptr;
    WGPUBindGroup testBindGroup = nullptr;
    WGPUBindGroupLayout testBindGroupLayout = nullptr;
    WGPURenderPipeline testPipeline = nullptr;

    std::shared_ptr<VncClient> vncClient;
    InputGenerator inputGen;
    uv_loop_t* uvLoop = nullptr;

    if (testRender) {
        yinfo("Creating test texture {}x{}", windowWidth, windowHeight);

        // Create texture
        WGPUTextureDescriptor texDesc = {};
        texDesc.size = {(uint32_t)windowWidth, (uint32_t)windowHeight, 1};
        texDesc.format = WGPUTextureFormat_BGRA8Unorm;
        texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
        texDesc.mipLevelCount = 1;
        texDesc.sampleCount = 1;
        texDesc.dimension = WGPUTextureDimension_2D;
        testTexture = wgpuDeviceCreateTexture(device, &texDesc);

        // Fill with random colored pattern
        std::vector<uint8_t> pixels(windowWidth * windowHeight * 4);
        std::mt19937 rng(42);
        for (int y = 0; y < windowHeight; y++) {
            for (int x = 0; x < windowWidth; x++) {
                int idx = (y * windowWidth + x) * 4;
                // Colorful gradient + noise
                pixels[idx + 0] = (x * 255 / windowWidth) ^ (rng() & 0x3F);  // B
                pixels[idx + 1] = (y * 255 / windowHeight) ^ (rng() & 0x3F); // G
                pixels[idx + 2] = ((x + y) * 128 / (windowWidth + windowHeight)) ^ (rng() & 0x3F); // R
                pixels[idx + 3] = 255; // A
            }
        }

        // Upload
        WGPUTexelCopyTextureInfo dst = {};
        dst.texture = testTexture;
        WGPUTexelCopyBufferLayout layout = {};
        layout.bytesPerRow = windowWidth * 4;
        layout.rowsPerImage = windowHeight;
        WGPUExtent3D extent = {(uint32_t)windowWidth, (uint32_t)windowHeight, 1};
        wgpuQueueWriteTexture(queue, &dst, pixels.data(), pixels.size(), &layout, &extent);

        testTextureView = wgpuTextureCreateView(testTexture, nullptr);

        // Create sampler
        WGPUSamplerDescriptor samplerDesc = {};
        samplerDesc.magFilter = WGPUFilterMode_Linear;
        samplerDesc.minFilter = WGPUFilterMode_Linear;
        samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
        samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
        samplerDesc.maxAnisotropy = 1;
        testSampler = wgpuDeviceCreateSampler(device, &samplerDesc);

        // Create shader with texture sampling
        const char* shaderSrc = R"(
struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
}

@vertex fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    var positions = array<vec2<f32>, 6>(
        vec2<f32>(-1.0, -1.0), vec2<f32>(1.0, -1.0), vec2<f32>(-1.0, 1.0),
        vec2<f32>(-1.0, 1.0), vec2<f32>(1.0, -1.0), vec2<f32>(1.0, 1.0)
    );
    var uvs = array<vec2<f32>, 6>(
        vec2<f32>(0.0, 1.0), vec2<f32>(1.0, 1.0), vec2<f32>(0.0, 0.0),
        vec2<f32>(0.0, 0.0), vec2<f32>(1.0, 1.0), vec2<f32>(1.0, 0.0)
    );
    var out: VertexOutput;
    out.position = vec4<f32>(positions[idx], 0.0, 1.0);
    out.uv = uvs[idx];
    return out;
}

@group(0) @binding(0) var tex: texture_2d<f32>;
@group(0) @binding(1) var samp: sampler;

@fragment fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return textureSample(tex, samp, in.uv);
}
)";
        WGPUShaderSourceWGSL wgslDesc = {};
        wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
        wgslDesc.code = WGPU_STR(shaderSrc);
        WGPUShaderModuleDescriptor shaderDesc = {};
        shaderDesc.nextInChain = &wgslDesc.chain;
        WGPUShaderModule shader = wgpuDeviceCreateShaderModule(device, &shaderDesc);
        if (!shader) {
            yerror("Failed to create shader module!");
            return 1;
        }

        // Bind group layout for texture + sampler
        WGPUBindGroupLayoutEntry layoutEntries[2] = {};
        layoutEntries[0].binding = 0;
        layoutEntries[0].visibility = WGPUShaderStage_Fragment;
        layoutEntries[0].texture.sampleType = WGPUTextureSampleType_Float;
        layoutEntries[0].texture.viewDimension = WGPUTextureViewDimension_2D;
        layoutEntries[1].binding = 1;
        layoutEntries[1].visibility = WGPUShaderStage_Fragment;
        layoutEntries[1].sampler.type = WGPUSamplerBindingType_Filtering;

        WGPUBindGroupLayoutDescriptor bglDesc = {};
        bglDesc.entryCount = 2;
        bglDesc.entries = layoutEntries;
        testBindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);

        // Pipeline layout
        WGPUPipelineLayoutDescriptor plDesc = {};
        plDesc.bindGroupLayoutCount = 1;
        plDesc.bindGroupLayouts = &testBindGroupLayout;
        WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &plDesc);

        // Create bind group
        WGPUBindGroupEntry bgEntries[2] = {};
        bgEntries[0].binding = 0;
        bgEntries[0].textureView = testTextureView;
        bgEntries[1].binding = 1;
        bgEntries[1].sampler = testSampler;

        WGPUBindGroupDescriptor bgDesc = {};
        bgDesc.layout = testBindGroupLayout;
        bgDesc.entryCount = 2;
        bgDesc.entries = bgEntries;
        testBindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);

        // Render pipeline
        WGPURenderPipelineDescriptor pipelineDesc = {};
        pipelineDesc.layout = pipelineLayout;
        pipelineDesc.vertex.module = shader;
        pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");

        WGPUColorTargetState colorTarget = {};
        colorTarget.format = surfaceFormat;
        colorTarget.writeMask = WGPUColorWriteMask_All;

        WGPUFragmentState fragment = {};
        fragment.module = shader;
        fragment.entryPoint = WGPU_STR("fs_main");
        fragment.targetCount = 1;
        fragment.targets = &colorTarget;
        pipelineDesc.fragment = &fragment;
        pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
        pipelineDesc.multisample.count = 1;
        pipelineDesc.multisample.mask = 0xFFFFFFFF;

        testPipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
        wgpuShaderModuleRelease(shader);
        wgpuPipelineLayoutRelease(pipelineLayout);

        if (!testPipeline) {
            yerror("Failed to create render pipeline!");
            return 1;
        }

        yinfo("Test render resources created - pipeline={}", (void*)testPipeline);

    } else {
        // Normal VNC client mode
        // Initialize EventLoop singleton (required for VncClient socket polling)
        auto loopRes = yetty::base::EventLoop::instance();
        if (!loopRes) {
            yerror("Failed to create EventLoop: {}", loopRes.error().message());
            return 1;
        }
        yinfo("EventLoop initialized");

        // Create VNC client
        vncClient = std::make_shared<VncClient>(device, queue, surfaceFormat);

        // Connect to server
        auto connectRes = vncClient->connect(host, port);
        if (!connectRes) {
            yerror("Failed to connect: {}", connectRes.error().message());
            return 1;
        }
        yinfo("Connected to {}:{}", host, port);

        // Setup client state for GLFW callbacks
        g_clientState.client = vncClient.get();

        // Send initial resize and cell size
        vncClient->sendResize(static_cast<uint16_t>(windowWidth), static_cast<uint16_t>(windowHeight));
        vncClient->sendCellSize(g_clientState.cellHeight);

        // Setup input generator
        if (generateInput) {
            inputGen.setClient(vncClient.get());
        }

        // Get libuv loop (same one used by EventLoop)
        uvLoop = uv_default_loop();
    }

    // Stats
    Stats stats;
    stats.startTime = glfwGetTime();
    stats.lastReportTime = stats.startTime;

    // Main loop - event driven, render only when tiles arrive
    while (!glfwWindowShouldClose(window) && g_running) {
        glfwPollEvents();

        // Reconfigure surface if window was resized
        if (g_clientState.needsReconfigure && g_clientState.surface) {
            yinfo("Reconfiguring surface to {}x{}", g_clientState.windowWidth, g_clientState.windowHeight);
            WGPUSurfaceConfiguration surfaceConfig = {};
            surfaceConfig.device = g_clientState.device;
            surfaceConfig.format = g_clientState.surfaceFormat;
            surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
            surfaceConfig.width = g_clientState.windowWidth;
            surfaceConfig.height = g_clientState.windowHeight;
            surfaceConfig.presentMode = WGPUPresentMode_Fifo;
            surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;
            wgpuSurfaceConfigure(g_clientState.surface, &surfaceConfig);
            windowWidth = g_clientState.windowWidth;
            windowHeight = g_clientState.windowHeight;
            g_clientState.needsReconfigure = false;
        }

        // Run libuv event loop - blocks until socket has data or timeout
        if (uvLoop) {
            // Use UV_RUN_ONCE to block until event or timeout
            uv_run(uvLoop, UV_RUN_NOWAIT);
        }

        double now = glfwGetTime();

        // Generate input with wave pattern
        if (generateInput) {
            inputGen.update(now, windowWidth, windowHeight);
            stats.inputsSent = inputGen.inputCount();
        }

        // Update VNC texture - returns true if new tiles were received
        bool needsRender = false;
        if (vncClient) {
            auto res = vncClient->updateTexture();
            if (res) {
                needsRender = *res;
            }
            // Don't render if VNC frame size doesn't match window size (avoid stretching)
            if (vncClient->width() != g_clientState.windowWidth ||
                vncClient->height() != g_clientState.windowHeight) {
                needsRender = false;  // Wait for server to send frame at correct size
            }
        } else if (testRender) {
            needsRender = true;  // Always render in test mode
        }

        // Only render if we have new content
        if (!needsRender) {
            // No new tiles, just poll and continue
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        // Get surface texture
        WGPUSurfaceTexture surfaceTexture;
        wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);
        if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
            surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
            ydebug("Surface texture status: {}", (int)surfaceTexture.status);
            continue;
        }

        WGPUTextureView view = wgpuTextureCreateView(surfaceTexture.texture, nullptr);

        // Create render pass
        WGPUCommandEncoderDescriptor encDesc = {};
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encDesc);

        WGPURenderPassColorAttachment colorAtt = {};
        colorAtt.view = view;
        colorAtt.loadOp = WGPULoadOp_Clear;
        colorAtt.storeOp = WGPUStoreOp_Store;
        colorAtt.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;

        // Use different clear colors to prove rendering works
        if (testRender) {
            colorAtt.clearValue = {0.0, 1.0, 0.0, 1.0};
        } else {
            colorAtt.clearValue = {0.1, 0.1, 0.1, 1.0};  // Dark gray background
        }

        WGPURenderPassDescriptor passDesc = {};
        passDesc.colorAttachmentCount = 1;
        passDesc.colorAttachments = &colorAtt;

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);

        // Render frame
        if (testRender) {
            wgpuRenderPassEncoderSetPipeline(pass, testPipeline);
            wgpuRenderPassEncoderSetBindGroup(pass, 0, testBindGroup, 0, nullptr);
            wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
        } else {
            vncClient->render(pass);
        }

        wgpuRenderPassEncoderEnd(pass);
        wgpuRenderPassEncoderRelease(pass);

        WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, nullptr);
        wgpuQueueSubmit(queue, 1, &cmd);
        wgpuCommandBufferRelease(cmd);
        wgpuCommandEncoderRelease(encoder);

        wgpuTextureViewRelease(view);
        wgpuSurfacePresent(surface);

        // Update stats
        stats.framesRendered++;
        stats.report(now);
    }

    yinfo("Shutting down...");

    if (vncClient) {
        vncClient->disconnect();
        vncClient.reset();
    }

    // Cleanup test render resources
    if (testPipeline) wgpuRenderPipelineRelease(testPipeline);
    if (testBindGroup) wgpuBindGroupRelease(testBindGroup);
    if (testBindGroupLayout) wgpuBindGroupLayoutRelease(testBindGroupLayout);
    if (testSampler) wgpuSamplerRelease(testSampler);
    if (testTextureView) wgpuTextureViewRelease(testTextureView);
    if (testTexture) wgpuTextureRelease(testTexture);

    wgpuQueueRelease(queue);
    wgpuDeviceRelease(device);
    wgpuAdapterRelease(adapter);
    wgpuSurfaceRelease(surface);
    wgpuInstanceRelease(instance);

    glfwDestroyWindow(window);
    glfwTerminate();

    yinfo("Done");
    return 0;
}
