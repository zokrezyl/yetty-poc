//=============================================================================
// yetty - WebGPU Terminal Emulator
//
// Main entry point. Creates and runs the Yetty engine.
//
// Platform-specific entry points:
// - Desktop (Linux/macOS/Windows): main()
// - Android: android_main()
//=============================================================================

#include <yetty/yetty.h>
#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <iostream>
#include <cstring>
#include <signal.h>
#include <cstdlib>

static void sigint_handler(int sig) {
    yinfo("SIGINT received! (signal {})", sig);
}

#if YETTY_WEB
// Keep Yetty alive for the duration of the web app
// (shared_ptr prevents destruction when main() returns)
static yetty::Yetty::Ptr g_yetty;
#endif

#if defined(__ANDROID__)
//-----------------------------------------------------------------------------
// Android Entry Point
//-----------------------------------------------------------------------------
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <android_native_app_glue.h>
#include <spdlog/sinks/android_sink.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <uv.h>  // For uv_run to process libuv events

// Declared in android-platform.cpp
namespace yetty {
    void android_set_app(android_app* app);
    void android_dispatch_touch(int action, float x, float y);
    void android_dispatch_key(int action, int keyCode, int metaState);
    void android_show_soft_keyboard(android_app* app);
}

// Extract asset to file (returns true on success)
static bool extractAsset(AAssetManager* mgr, const char* assetName, const char* destPath) {
    AAsset* asset = AAssetManager_open(mgr, assetName, AASSET_MODE_STREAMING);
    if (!asset) {
        __android_log_print(ANDROID_LOG_WARN, "yetty", "Asset not found: %s", assetName);
        return false;
    }

    FILE* out = fopen(destPath, "wb");
    if (!out) {
        AAsset_close(asset);
        __android_log_print(ANDROID_LOG_ERROR, "yetty", "Cannot create: %s", destPath);
        return false;
    }

    char buf[8192];
    int bytesRead;
    while ((bytesRead = AAsset_read(asset, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, bytesRead, out);
    }

    fclose(out);
    AAsset_close(asset);

    // Make executable
    chmod(destPath, 0755);
    __android_log_print(ANDROID_LOG_INFO, "yetty", "Extracted: %s", destPath);
    return true;
}

// Recursively extract asset directory to filesystem
// Note: Android AAssetDir only lists files, not subdirectories.
// We use a list of known subdirectories to recurse into.
static void extractAssetDirRecursive(AAssetManager* mgr, const char* assetDir, const char* destDir,
                                      const std::vector<std::string>& knownSubdirs, int depth = 0);

static void extractAssetDir(AAssetManager* mgr, const char* assetDir, const char* destDir) {
    AAssetDir* dir = AAssetManager_openDir(mgr, assetDir);
    if (!dir) {
        __android_log_print(ANDROID_LOG_WARN, "yetty", "Asset dir not found: %s", assetDir);
        return;
    }

    // Create destination directory
    mkdir(destDir, 0755);

    const char* filename;
    while ((filename = AAssetDir_getNextFileName(dir)) != nullptr) {
        std::string assetPath = std::string(assetDir) + "/" + filename;
        std::string destPath = std::string(destDir) + "/" + filename;

        // Try to open as file first
        AAsset* asset = AAssetManager_open(mgr, assetPath.c_str(), AASSET_MODE_STREAMING);
        if (asset) {
            // It's a file, extract it
            FILE* out = fopen(destPath.c_str(), "wb");
            if (out) {
                char buf[8192];
                int bytesRead;
                while ((bytesRead = AAsset_read(asset, buf, sizeof(buf))) > 0) {
                    fwrite(buf, 1, bytesRead, out);
                }
                fclose(out);
            }
            AAsset_close(asset);
        }
    }

    AAssetDir_close(dir);
    __android_log_print(ANDROID_LOG_INFO, "yetty", "Extracted dir: %s -> %s", assetDir, destDir);
}

// Extract asset directory with known subdirectories (for recursive extraction)
static void extractAssetDirRecursive(AAssetManager* mgr, const char* assetDir, const char* destDir,
                                      const std::vector<std::string>& knownSubdirs, int depth) {
    // First extract files in this directory
    extractAssetDir(mgr, assetDir, destDir);

    // Then recurse into known subdirectories
    for (const auto& subdir : knownSubdirs) {
        std::string subAssetPath = std::string(assetDir) + "/" + subdir;
        std::string subDestPath = std::string(destDir) + "/" + subdir;

        // Check if this subdirectory exists by trying to open it
        AAssetDir* testDir = AAssetManager_openDir(mgr, subAssetPath.c_str());
        if (testDir) {
            AAssetDir_close(testDir);
            // Recurse with empty subdirs (will just extract files)
            extractAssetDirRecursive(mgr, subAssetPath.c_str(), subDestPath.c_str(), {}, depth + 1);
        }
    }
}

// Forward declaration for platform access
namespace yetty {
    void android_set_native_window(void* window);
}

// State for android_main event loop
struct AndroidAppState {
    android_app* app = nullptr;
    yetty::Yetty::Ptr yetty;
    bool windowReady = false;
    bool running = true;
};

// Handle Android app commands (lifecycle events)
static void handleAppCmd(android_app* app, int32_t cmd) {
    auto* state = static_cast<AndroidAppState*>(app->userData);

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            __android_log_print(ANDROID_LOG_INFO, "yetty", "APP_CMD_INIT_WINDOW");
            if (app->window) {
                state->windowReady = true;
                yetty::android_set_native_window(app->window);
            }
            break;

        case APP_CMD_TERM_WINDOW:
            __android_log_print(ANDROID_LOG_INFO, "yetty", "APP_CMD_TERM_WINDOW");
            state->windowReady = false;
            yetty::android_set_native_window(nullptr);
            break;

        case APP_CMD_DESTROY:
            __android_log_print(ANDROID_LOG_INFO, "yetty", "APP_CMD_DESTROY");
            state->running = false;
            break;

        case APP_CMD_GAINED_FOCUS:
            __android_log_print(ANDROID_LOG_INFO, "yetty", "APP_CMD_GAINED_FOCUS");
            break;

        case APP_CMD_LOST_FOCUS:
            __android_log_print(ANDROID_LOG_INFO, "yetty", "APP_CMD_LOST_FOCUS");
            break;
    }
}

// Handle input events (touch and keyboard)
static int32_t handleInputEvent(android_app* app, AInputEvent* event) {
    int32_t eventType = AInputEvent_getType(event);

    if (eventType == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
        float x = AMotionEvent_getX(event, 0);
        float y = AMotionEvent_getY(event, 0);

        yetty::android_dispatch_touch(action, x, y);

        // Show soft keyboard on tap
        if (action == AMOTION_EVENT_ACTION_UP) {
            yetty::android_show_soft_keyboard(app);
        }

        return 1;  // Event handled
    }

    if (eventType == AINPUT_EVENT_TYPE_KEY) {
        int32_t action = AKeyEvent_getAction(event);
        int32_t keyCode = AKeyEvent_getKeyCode(event);
        int32_t metaState = AKeyEvent_getMetaState(event);

        __android_log_print(ANDROID_LOG_INFO, "yetty", "Key event: action=%d keyCode=%d meta=%d", action, keyCode, metaState);

        yetty::android_dispatch_key(action, keyCode, metaState);
        return 1;  // Event handled
    }

    return 0;  // Event not handled
}

extern "C" void android_main(struct android_app* app) {
    // Enable ytrace logging by default on Android
    setenv("YTRACE_DEFAULT_ON", "yes", 1);

    // Configure spdlog to use Android logcat sink
    auto android_sink = std::make_shared<spdlog::sinks::android_sink_mt>("yetty");
    auto logger = std::make_shared<spdlog::logger>("yetty", android_sink);
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Yetty Android starting...");

    // Get app data directory
    const char* dataDir = app->activity->internalDataPath;
    AAssetManager* assetMgr = app->activity->assetManager;

    // Extract toybox
    std::string toyboxPath = std::string(dataDir) + "/toybox";
    if (access(toyboxPath.c_str(), X_OK) != 0) {
        if (assetMgr) {
            extractAsset(assetMgr, "toybox", toyboxPath.c_str());
        }
    }

    // Extract shaders directory and all subdirectories
    std::string shadersDir = std::string(dataDir) + "/shaders";
    if (assetMgr) {
        extractAssetDir(assetMgr, "shaders", shadersDir.c_str());
        // Extract all shader subdirectories
        extractAssetDir(assetMgr, "shaders/lib", (shadersDir + "/lib").c_str());
        extractAssetDir(assetMgr, "shaders/cards", (shadersDir + "/cards").c_str());
        extractAssetDir(assetMgr, "shaders/glyphs", (shadersDir + "/glyphs").c_str());
        extractAssetDir(assetMgr, "shaders/effects", (shadersDir + "/effects").c_str());
        extractAssetDir(assetMgr, "shaders/pre-effects", (shadersDir + "/pre-effects").c_str());
        extractAssetDir(assetMgr, "shaders/post-effects", (shadersDir + "/post-effects").c_str());
    }

    // Extract fonts-cdb directory
    std::string fontsCdbDir = std::string(dataDir) + "/fonts-cdb";
    if (assetMgr) {
        extractAssetDir(assetMgr, "fonts-cdb", fontsCdbDir.c_str());
    }

    // Extract font TTF file
    std::string fontPath = std::string(dataDir) + "/DejaVuSansMNerdFontMono-Regular.ttf";
    if (access(fontPath.c_str(), R_OK) != 0 && assetMgr) {
        extractAsset(assetMgr, "DejaVuSansMNerdFontMono-Regular.ttf", fontPath.c_str());
    }

    // Extract demo content (scripts, files, etc.)
    if (assetMgr) {
        // Scripts directory (demo.sh, etc.)
        std::string scriptsDir = std::string(dataDir) + "/scripts";
        extractAssetDir(assetMgr, "scripts", scriptsDir.c_str());
        // Subdirectories
        extractAssetDir(assetMgr, "scripts/cards", (scriptsDir + "/cards").c_str());
        extractAssetDir(assetMgr, "scripts/gpu-screen-ydraw", (scriptsDir + "/gpu-screen-ydraw").c_str());

        // Files directory (data files for demos)
        std::string filesDir = std::string(dataDir) + "/files";
        extractAssetDir(assetMgr, "files", filesDir.c_str());
        extractAssetDir(assetMgr, "files/shader-glyphs", (filesDir + "/shader-glyphs").c_str());

        // Glyphs directory
        std::string glyphsDir = std::string(dataDir) + "/glyphs";
        extractAssetDir(assetMgr, "glyphs", glyphsDir.c_str());

        // Presentation directory
        std::string presentDir = std::string(dataDir) + "/presentation";
        extractAssetDir(assetMgr, "presentation", presentDir.c_str());
        extractAssetDir(assetMgr, "presentation/video-1", (presentDir + "/video-1").c_str());

        // Demo outputs (pre-generated .out files from demo scripts)
        // All known subdirectories from demo/scripts/
        std::vector<std::string> demoSubdirs = {
            "cards", "cards/mermaid", "cards/plot", "cards/thorvg", "cards/ydraw",
            "cards/ygrid", "cards/ygui", "cards/yhtml", "cards/yplot",
            "effects", "gpu-screen-ydraw", "yecho"
        };
        std::string demoOutputDir = std::string(dataDir) + "/demo-output";
        extractAssetDirRecursive(assetMgr, "demo-output", demoOutputDir.c_str(), demoSubdirs);

        __android_log_print(ANDROID_LOG_INFO, "yetty", "Demo content extracted to: %s", dataDir);
    }

    // Set environment variables for yetty to find assets
    setenv("YETTY_SHADERS_DIR", shadersDir.c_str(), 1);
    setenv("YETTY_ASSETS_DIR", dataDir, 1);
    __android_log_print(ANDROID_LOG_INFO, "yetty", "YETTY_SHADERS_DIR=%s", shadersDir.c_str());
    __android_log_print(ANDROID_LOG_INFO, "yetty", "YETTY_ASSETS_DIR=%s", dataDir);

    // Check for VNC mode via system property (set with: adb shell setprop debug.yetty.vnc.headless 1)
    // Use debug.* namespace which shell has permission to write (SELinux allows it)
    char vncHeadless[PROP_VALUE_MAX] = {0};
    char vncPort[PROP_VALUE_MAX] = {0};
    int headlessLen = __system_property_get("debug.yetty.vnc.headless", vncHeadless);
    int portLen = __system_property_get("debug.yetty.vnc.port", vncPort);
    __android_log_print(ANDROID_LOG_INFO, "yetty", "VNC property check: headless='%s' len=%d port='%s' len=%d", vncHeadless, headlessLen, vncPort, portLen);
    if (vncHeadless[0] == '1') {
        setenv("YETTY_VNC_HEADLESS", "1", 1);
        __android_log_print(ANDROID_LOG_INFO, "yetty", "VNC headless mode enabled");
        if (vncPort[0] != '\0') {
            setenv("YETTY_VNC_PORT", vncPort, 1);
            __android_log_print(ANDROID_LOG_INFO, "yetty", "VNC port: %s", vncPort);
        } else {
            setenv("YETTY_VNC_PORT", "5900", 1);
            __android_log_print(ANDROID_LOG_INFO, "yetty", "VNC port: 5900 (default)");
        }
    }

    // Store app reference for Platform to extract paths
    yetty::android_set_app(app);

    // Set up app state and callbacks
    AndroidAppState state;
    state.app = app;
    app->userData = &state;
    app->onAppCmd = handleAppCmd;
    app->onInputEvent = handleInputEvent;

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Waiting for window...");

    // Wait for window to be ready
    while (!state.windowReady && state.running) {
        int events;
        android_poll_source* source;

        // Block waiting for events (-1 = wait forever)
        if (ALooper_pollOnce(-1, nullptr, &events, (void**)&source) >= 0) {
            if (source) {
                source->process(app, source);
            }
        }

        if (app->destroyRequested) {
            __android_log_print(ANDROID_LOG_INFO, "yetty", "Destroy requested during init");
            return;
        }
    }

    if (!state.windowReady) {
        __android_log_print(ANDROID_LOG_ERROR, "yetty", "Window not ready, exiting");
        return;
    }

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Window ready, creating Yetty...");

    auto result = yetty::Yetty::create(app);
    if (!result) {
        __android_log_print(ANDROID_LOG_ERROR, "yetty",
                           "Failed to initialize yetty: %s",
                           result.error().message().c_str());
        return;
    }
    state.yetty = *result;

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Yetty created, entering main loop...");

    // Android main loop - process events and render
    while (state.running && !app->destroyRequested) {
        int events;
        android_poll_source* source;

        // Poll with 0 timeout for non-blocking when active, -1 when not focused
        int timeout = state.windowReady ? 0 : -1;

        while (ALooper_pollOnce(timeout, nullptr, &events, (void**)&source) >= 0) {
            if (source) {
                source->process(app, source);
            }
            if (app->destroyRequested) {
                break;
            }
        }

        // Process libuv events for async I/O (telnet, etc.)
        uv_run(uv_default_loop(), UV_RUN_NOWAIT);

        // Render frame if window is ready
        if (state.windowReady && state.yetty) {
            (void)state.yetty->iterate();  // Single frame iteration
        }

        // Frame rate limiting - prevent CPU spin when no vsync (e.g., VNC headless)
        // ~60 FPS = 16ms per frame
        usleep(16000);
    }

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Main loop exited, shutting down...");
    if (state.yetty) {
        (void)state.yetty->shutdown();
    }

    __android_log_print(ANDROID_LOG_INFO, "yetty", "Yetty shutdown complete");
}
#else
//-----------------------------------------------------------------------------
// Desktop Entry Point
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    // Set default log level to trace for maximum debug output
    spdlog::set_level(spdlog::level::trace);

    // VERSION MARKER - change this number to verify fresh wasm loaded
    yinfo("=== YETTY BUILD VERSION 001 ===");
    spdlog::cfg::load_env_levels();

    // Debug: catch SIGINT to see if process receives it
    signal(SIGINT, sigint_handler);

    auto result = yetty::Yetty::create(argc, argv);
    if (!result) {
        std::string msg = result.error().message();
        if (msg == "Help requested") {
            return 0;
        }
        yerror("Failed to initialize yetty: {}", yetty::error_msg(result));
        return 1;
    }
    auto yetty = *result;
    auto runResult = yetty->run();
    auto shutdownResult = yetty->shutdown();
    if (!runResult) {
        yerror("Yetty run failed: {}", yetty::error_msg(runResult));
        return 1;
    }
    if (!shutdownResult) {
        yerror("Yetty shutdown failed: {}", yetty::error_msg(shutdownResult));
        return 1;
    }
    return 0;
}
#endif
