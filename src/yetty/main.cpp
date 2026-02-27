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
#include <android_native_app_glue.h>

extern "C" void android_main(struct android_app* app) {
    __android_log_print(ANDROID_LOG_INFO, "yetty", "Yetty Android starting...");

    auto result = yetty::Yetty::create(app);
    if (!result) {
        __android_log_print(ANDROID_LOG_ERROR, "yetty",
                           "Failed to initialize yetty: %s",
                           result.error().message().c_str());
        return;
    }
    auto yetty = *result;
    (void)yetty->run();
    (void)yetty->shutdown();
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
