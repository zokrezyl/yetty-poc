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
#include <spdlog/spdlog.h>
#include <iostream>

#if defined(__ANDROID__)
//-----------------------------------------------------------------------------
// Android Entry Point
//-----------------------------------------------------------------------------
void android_main(struct android_app* app) {
    __android_log_print(ANDROID_LOG_INFO, "yetty", "Yetty Android starting...");

    auto result = yetty::Yetty::create(app);
    if (!result) {
        __android_log_print(ANDROID_LOG_ERROR, "yetty",
                           "Failed to initialize yetty: %s",
                           result.error().message().c_str());
        return;
    }
    (*result)->run();
}
#else
//-----------------------------------------------------------------------------
// Desktop Entry Point
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    auto result = yetty::Yetty::create(argc, argv);
    if (!result) {
        // Check if this is just a "help requested" or "atlas generated" exit
        std::string msg = result.error().message();
        if (msg == "Help requested" || msg == "Atlas generation complete") {
            return 0;
        }
        spdlog::error("Failed to initialize yetty: {}", yetty::error_msg(result));
        return 1;
    }
    return (*result)->run();
}
#endif
