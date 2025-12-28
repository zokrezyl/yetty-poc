//=============================================================================
// Yetty Android Native Activity
//=============================================================================
// Entry point for Android using native_app_glue
// Uses WebGPU (wgpu-native with Vulkan backend) for rendering
// Uses BusyBox ash as the shell

#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include "yetty/renderer/webgpu-context.h"
#include "yetty/renderer/text-renderer.h"
#include "yetty/terminal/grid.h"
#include "yetty/terminal/font.h"
#include "yetty/terminal/terminal.h"

#include <webgpu/webgpu.h>

#include <cmath>
#include <ctime>
#include <cstring>
#include <string>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>

#define LOG_TAG "yetty"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

//-----------------------------------------------------------------------------
// Application State
//-----------------------------------------------------------------------------
struct AndroidAppState {
    struct android_app* app = nullptr;

    // WebGPU
    WGPUInstance instance = nullptr;
    WGPUSurface surface = nullptr;
    WGPUAdapter adapter = nullptr;
    WGPUDevice device = nullptr;
    WGPUQueue queue = nullptr;
    WGPUTextureFormat surfaceFormat = WGPUTextureFormat_BGRA8Unorm;

    // Rendering
    std::unique_ptr<yetty::WebGPUContext> ctx;
    std::unique_ptr<yetty::TextRenderer> renderer;
    std::unique_ptr<yetty::Font> font;
    std::unique_ptr<yetty::Terminal> terminal;

    // Window dimensions
    int32_t width = 0;
    int32_t height = 0;

    // Cell dimensions
    float cellWidth = 0.0f;
    float cellHeight = 0.0f;
    uint32_t cols = 80;
    uint32_t rows = 24;

    // Touch state
    float touchX = 0.0f;
    float touchY = 0.0f;
    bool touching = false;
    double touchDownTime = 0.0;

    // Selection
    bool selecting = false;

    // State flags
    bool initialized = false;
    bool running = false;

    // BusyBox paths
    std::string dataDir;
    std::string busyboxPath;
};

static AndroidAppState g_state;

//-----------------------------------------------------------------------------
// Utility Functions
//-----------------------------------------------------------------------------
static double getTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec) / 1000000000.0;
}

static std::string getInternalDataPath(struct android_app* app) {
    return std::string(app->activity->internalDataPath);
}

//-----------------------------------------------------------------------------
// BusyBox Setup
//-----------------------------------------------------------------------------
static bool extractAsset(struct android_app* app, const char* assetName, const char* destPath) {
    AAssetManager* assetManager = app->activity->assetManager;
    AAsset* asset = AAssetManager_open(assetManager, assetName, AASSET_MODE_BUFFER);

    if (!asset) {
        LOGE("Failed to open asset: %s", assetName);
        return false;
    }

    off_t size = AAsset_getLength(asset);
    const void* buffer = AAsset_getBuffer(asset);

    FILE* file = fopen(destPath, "wb");
    if (!file) {
        LOGE("Failed to create file: %s", destPath);
        AAsset_close(asset);
        return false;
    }

    fwrite(buffer, 1, size, file);
    fclose(file);
    AAsset_close(asset);

    // Make executable
    chmod(destPath, 0755);

    LOGI("Extracted asset %s to %s", assetName, destPath);
    return true;
}

static bool setupBusybox(struct android_app* app) {
    g_state.dataDir = getInternalDataPath(app);
    g_state.busyboxPath = g_state.dataDir + "/busybox";

    // Check if busybox already exists
    if (access(g_state.busyboxPath.c_str(), X_OK) == 0) {
        LOGI("BusyBox already installed at %s", g_state.busyboxPath.c_str());
        return true;
    }

    // Extract from assets
    if (!extractAsset(app, "busybox", g_state.busyboxPath.c_str())) {
        LOGE("Failed to extract BusyBox");
        return false;
    }

    LOGI("BusyBox installed at %s", g_state.busyboxPath.c_str());
    return true;
}

//-----------------------------------------------------------------------------
// WebGPU Initialization
//-----------------------------------------------------------------------------
static bool initWebGPU(ANativeWindow* window) {
    LOGI("Initializing WebGPU...");

    // Create instance
    WGPUInstanceDescriptor instanceDesc = {};
    g_state.instance = wgpuCreateInstance(&instanceDesc);
    if (!g_state.instance) {
        LOGE("Failed to create WebGPU instance");
        return false;
    }

    // Create surface from Android window
    WGPUSurfaceDescriptorFromAndroidNativeWindow androidSurfaceDesc = {};
    androidSurfaceDesc.chain.sType = WGPUSType_SurfaceDescriptorFromAndroidNativeWindow;
    androidSurfaceDesc.window = window;

    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = reinterpret_cast<const WGPUChainedStruct*>(&androidSurfaceDesc);

    g_state.surface = wgpuInstanceCreateSurface(g_state.instance, &surfaceDesc);
    if (!g_state.surface) {
        LOGE("Failed to create WebGPU surface");
        return false;
    }

    // Request adapter
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = g_state.surface;
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    struct AdapterUserData {
        WGPUAdapter adapter = nullptr;
        bool done = false;
    } adapterData;

    wgpuInstanceRequestAdapter(g_state.instance, &adapterOpts,
        [](WGPURequestAdapterStatus status, WGPUAdapter adapter, const char* message, void* userdata) {
            auto* data = static_cast<AdapterUserData*>(userdata);
            if (status == WGPURequestAdapterStatus_Success) {
                data->adapter = adapter;
            } else {
                LOGE("Failed to get adapter: %s", message ? message : "unknown error");
            }
            data->done = true;
        }, &adapterData);

    // Wait for adapter (synchronous for simplicity)
    while (!adapterData.done) {
        // In a real app, you'd want async handling
    }

    g_state.adapter = adapterData.adapter;
    if (!g_state.adapter) {
        LOGE("Failed to get WebGPU adapter");
        return false;
    }

    // Request device
    WGPUDeviceDescriptor deviceDesc = {};

    struct DeviceUserData {
        WGPUDevice device = nullptr;
        bool done = false;
    } deviceData;

    wgpuAdapterRequestDevice(g_state.adapter, &deviceDesc,
        [](WGPURequestDeviceStatus status, WGPUDevice device, const char* message, void* userdata) {
            auto* data = static_cast<DeviceUserData*>(userdata);
            if (status == WGPURequestDeviceStatus_Success) {
                data->device = device;
            } else {
                LOGE("Failed to get device: %s", message ? message : "unknown error");
            }
            data->done = true;
        }, &deviceData);

    while (!deviceData.done) {
        // Wait for device
    }

    g_state.device = deviceData.device;
    if (!g_state.device) {
        LOGE("Failed to get WebGPU device");
        return false;
    }

    g_state.queue = wgpuDeviceGetQueue(g_state.device);

    LOGI("WebGPU initialized successfully");
    return true;
}

static bool configureSurface() {
    g_state.width = ANativeWindow_getWidth(g_state.app->window);
    g_state.height = ANativeWindow_getHeight(g_state.app->window);

    LOGI("Window size: %dx%d", g_state.width, g_state.height);

    // Get surface capabilities
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(g_state.surface, g_state.adapter, &caps);
    if (caps.formatCount > 0) {
        g_state.surfaceFormat = caps.formats[0];
    }
    wgpuSurfaceCapabilitiesFreeMembers(caps);

    // Configure surface
    WGPUSurfaceConfiguration config = {};
    config.device = g_state.device;
    config.format = g_state.surfaceFormat;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    config.width = g_state.width;
    config.height = g_state.height;
    config.presentMode = WGPUPresentMode_Fifo;  // VSync

    wgpuSurfaceConfigure(g_state.surface, &config);

    LOGI("Surface configured with format %d", g_state.surfaceFormat);
    return true;
}

//-----------------------------------------------------------------------------
// Terminal Initialization
//-----------------------------------------------------------------------------
static bool initTerminal() {
    LOGI("Initializing terminal...");

    // TODO: Load font atlas from assets
    // For now, use hardcoded cell dimensions
    g_state.cellWidth = 20.0f;
    g_state.cellHeight = 40.0f;

    // Calculate terminal dimensions based on screen size
    g_state.cols = static_cast<uint32_t>(g_state.width / g_state.cellWidth);
    g_state.rows = static_cast<uint32_t>(g_state.height / g_state.cellHeight);

    // Ensure minimum dimensions
    if (g_state.cols < 40) g_state.cols = 40;
    if (g_state.rows < 10) g_state.rows = 10;

    LOGI("Terminal dimensions: %ux%u", g_state.cols, g_state.rows);

    // Set up environment for BusyBox shell
    setenv("TERM", "xterm-256color", 1);
    setenv("HOME", g_state.dataDir.c_str(), 1);
    setenv("PATH", g_state.dataDir.c_str(), 1);
    setenv("SHELL", g_state.busyboxPath.c_str(), 1);

    // TODO: Create terminal with Font - for now skip
    // Need to load font atlas from assets first
    // g_state.terminal = std::make_unique<yetty::Terminal>(
    //     g_state.cols, g_state.rows, g_state.font.get());

    // Terminal creation is skipped for now (needs Font)
    LOGI("Terminal initialization skipped (TODO: load font)");
    return true;
}

//-----------------------------------------------------------------------------
// Cleanup
//-----------------------------------------------------------------------------
static void cleanup() {
    LOGI("Cleaning up...");

    g_state.terminal.reset();
    g_state.renderer.reset();
    g_state.font.reset();
    g_state.ctx.reset();

    if (g_state.queue) {
        wgpuQueueRelease(g_state.queue);
        g_state.queue = nullptr;
    }
    if (g_state.device) {
        wgpuDeviceRelease(g_state.device);
        g_state.device = nullptr;
    }
    if (g_state.adapter) {
        wgpuAdapterRelease(g_state.adapter);
        g_state.adapter = nullptr;
    }
    if (g_state.surface) {
        wgpuSurfaceRelease(g_state.surface);
        g_state.surface = nullptr;
    }
    if (g_state.instance) {
        wgpuInstanceRelease(g_state.instance);
        g_state.instance = nullptr;
    }

    g_state.initialized = false;
}

//-----------------------------------------------------------------------------
// Input Handling
//-----------------------------------------------------------------------------
static int32_t handleInput(struct android_app* app, AInputEvent* event) {
    int32_t eventType = AInputEvent_getType(event);

    if (eventType == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);

        // Convert touch position to terminal cell
        int col = static_cast<int>(x / g_state.cellWidth);
        int row = static_cast<int>(y / g_state.cellHeight);

        switch (action) {
            case AMOTION_EVENT_ACTION_DOWN:
                g_state.touchX = x;
                g_state.touchY = y;
                g_state.touching = true;
                g_state.touchDownTime = getTime();

                // Start selection
                if (g_state.terminal) {
                    g_state.terminal->startSelection(row, col);
                    g_state.selecting = true;
                }
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                if (g_state.selecting && g_state.terminal) {
                    g_state.terminal->extendSelection(row, col);
                }
                g_state.touchX = x;
                g_state.touchY = y;
                break;

            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_CANCEL:
                g_state.touching = false;
                g_state.selecting = false;
                break;
        }

        return 1;
    }

    if (eventType == AINPUT_EVENT_TYPE_KEY) {
        int32_t keyCode = AKeyEvent_getKeyCode(event);
        int32_t action = AKeyEvent_getAction(event);

        if (action == AKEY_EVENT_ACTION_DOWN && g_state.terminal) {
            // Handle Android key events
            // TODO: Map Android keycodes to VTerm keys
            switch (keyCode) {
                case AKEYCODE_ENTER:
                    g_state.terminal->sendKey('\r', VTERM_MOD_NONE);
                    break;
                case AKEYCODE_DEL:  // Backspace
                    g_state.terminal->sendKey(VTERM_KEY_BACKSPACE, VTERM_MOD_NONE);
                    break;
                case AKEYCODE_DPAD_UP:
                    g_state.terminal->sendKey(VTERM_KEY_UP, VTERM_MOD_NONE);
                    break;
                case AKEYCODE_DPAD_DOWN:
                    g_state.terminal->sendKey(VTERM_KEY_DOWN, VTERM_MOD_NONE);
                    break;
                case AKEYCODE_DPAD_LEFT:
                    g_state.terminal->sendKey(VTERM_KEY_LEFT, VTERM_MOD_NONE);
                    break;
                case AKEYCODE_DPAD_RIGHT:
                    g_state.terminal->sendKey(VTERM_KEY_RIGHT, VTERM_MOD_NONE);
                    break;
                default:
                    // Try to get Unicode character
                    // Note: Full keyboard support needs soft keyboard integration
                    break;
            }
        }

        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
// App Command Handler
//-----------------------------------------------------------------------------
static void handleCmd(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW");
            if (app->window != nullptr && !g_state.initialized) {
                // Set up BusyBox
                if (!setupBusybox(app)) {
                    LOGE("Failed to setup BusyBox");
                    return;
                }

                // Initialize WebGPU
                if (!initWebGPU(app->window)) {
                    LOGE("Failed to initialize WebGPU");
                    return;
                }

                // Configure surface
                if (!configureSurface()) {
                    LOGE("Failed to configure surface");
                    return;
                }

                // Initialize terminal
                if (!initTerminal()) {
                    LOGE("Failed to initialize terminal");
                    return;
                }

                g_state.initialized = true;
                g_state.running = true;
                LOGI("Yetty initialized successfully");
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");
            g_state.running = false;
            cleanup();
            break;

        case APP_CMD_GAINED_FOCUS:
            LOGI("APP_CMD_GAINED_FOCUS");
            // TODO: Show soft keyboard
            break;

        case APP_CMD_LOST_FOCUS:
            LOGI("APP_CMD_LOST_FOCUS");
            // TODO: Hide soft keyboard
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGI("APP_CMD_CONFIG_CHANGED");
            // Handle orientation change, etc.
            if (g_state.initialized && app->window) {
                // Reconfigure surface with new dimensions
                configureSurface();

                // Resize terminal
                uint32_t newCols = static_cast<uint32_t>(g_state.width / g_state.cellWidth);
                uint32_t newRows = static_cast<uint32_t>(g_state.height / g_state.cellHeight);
                if (g_state.terminal && (newCols != g_state.cols || newRows != g_state.rows)) {
                    g_state.cols = newCols;
                    g_state.rows = newRows;
                    g_state.terminal->resize(newCols, newRows);
                }
            }
            break;
    }
}

//-----------------------------------------------------------------------------
// Render Frame
//-----------------------------------------------------------------------------
static void renderFrame() {
    if (!g_state.initialized || !g_state.running) return;

    // Update terminal (read PTY output)
    if (g_state.terminal) {
        g_state.terminal->update();
    }

    // Get current surface texture
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(g_state.surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
        LOGW("Failed to get surface texture: %d", surfaceTexture.status);
        return;
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = g_state.surfaceFormat;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    WGPUTextureView textureView = wgpuTextureCreateView(surfaceTexture.texture, &viewDesc);

    // Create command encoder
    WGPUCommandEncoderDescriptor encoderDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_state.device, &encoderDesc);

    // Begin render pass
    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = textureView;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.0f, 0.0f, 0.0f, 1.0f};  // Black background

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);

    // TODO: Render terminal grid using TextRenderer
    // This requires setting up the text renderer pipeline

    wgpuRenderPassEncoderEnd(renderPass);
    wgpuRenderPassEncoderRelease(renderPass);

    // Submit commands
    WGPUCommandBufferDescriptor cmdBufferDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufferDesc);
    wgpuQueueSubmit(g_state.queue, 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);
    wgpuTextureViewRelease(textureView);

    // Present
    wgpuSurfacePresent(g_state.surface);
}

} // anonymous namespace

//-----------------------------------------------------------------------------
// Android Main Entry Point
//-----------------------------------------------------------------------------
void android_main(struct android_app* app) {
    LOGI("Yetty Android starting...");

    g_state.app = app;
    app->onAppCmd = handleCmd;
    app->onInputEvent = handleInput;

    // Main loop
    while (true) {
        int events;
        struct android_poll_source* source;

        // Poll for events
        // Use timeout of 0 when running (non-blocking), -1 when paused (blocking)
        int timeout = g_state.running ? 0 : -1;

        while (ALooper_pollAll(timeout, nullptr, &events, reinterpret_cast<void**>(&source)) >= 0) {
            if (source != nullptr) {
                source->process(app, source);
            }

            if (app->destroyRequested) {
                LOGI("Destroy requested, cleaning up...");
                cleanup();
                return;
            }
        }

        // Render frame
        if (g_state.running) {
            renderFrame();
        }
    }
}
