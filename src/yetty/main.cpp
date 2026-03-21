//=============================================================================
// yetty - WebGPU Terminal Emulator
//
// Main entry point. Platform dispatch only - all logic in InitManager.
//=============================================================================

#include <yetty/platform/init-manager.h>
#include <ytrace/ytrace.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>

#if defined(__ANDROID__)
#include <android_native_app_glue.h>
#include <spdlog/sinks/android_sink.h>
#endif

#if !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
static void sigint_handler(int sig) {
    ydebug("SIGINT received! (signal {})", sig);
}

static void setup_logging(int argc, char* argv[]) {
    // Pre-parse --ytrace-default-on (must be set before any ytrace usage)
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--ytrace-default-on") {
#ifdef _WIN32
            _putenv_s("YTRACE_DEFAULT_ON", "yes");
#else
            setenv("YTRACE_DEFAULT_ON", "yes", 1);
#endif
            break;
        }
    }

    // Pre-parse --ytrace-out for spdlog sink configuration
    std::string ytraceOutFile;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--ytrace-out" && i + 1 < argc) {
            ytraceOutFile = argv[i + 1];
            break;
        }
    }

    // Configure spdlog output sink
    if (!ytraceOutFile.empty()) {
        std::shared_ptr<spdlog::sinks::sink> sink;
        if (ytraceOutFile == "-") {
            sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        } else {
            sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(ytraceOutFile, true);
        }
        auto logger = std::make_shared<spdlog::logger>("yetty", sink);
        logger->set_level(spdlog::level::trace);
        spdlog::set_default_logger(logger);
    }

    spdlog::set_level(spdlog::level::trace);
    spdlog::cfg::load_env_levels();
}
#endif

#if defined(__ANDROID__)
//-----------------------------------------------------------------------------
// Android Entry Point
//-----------------------------------------------------------------------------
void android_main(android_app* app) {
    setenv("YTRACE_DEFAULT_ON", "yes", 1);

    auto android_sink = std::make_shared<spdlog::sinks::android_sink_mt>("yetty");
    auto logger = std::make_shared<spdlog::logger>("yetty", android_sink);
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    ydebug("=== YETTY ANDROID BUILD ===");

    auto initResult = yetty::createForAndroid(app);
    if (!initResult) {
        yerror("Failed to initialize platform: {}", yetty::error_msg(initResult));
        return;
    }
    auto initManager = *initResult;

    auto runResult = initManager->run();
    if (!runResult) {
        yerror("Run failed: {}", yetty::error_msg(runResult));
    }

    ydebug("android_main exiting");
}

#elif defined(__EMSCRIPTEN__)
//-----------------------------------------------------------------------------
// WebASM Entry Point
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    ydebug("=== YETTY WASM BUILD ===");
    spdlog::cfg::load_env_levels();

    auto initResult = yetty::InitManager::create();
    if (!initResult) {
        yerror("Failed to create InitManager: {}", yetty::error_msg(initResult));
        return 1;
    }
    auto initManager = *initResult;

    auto runResult = initManager->run(argc, argv);
    if (!runResult) {
        yerror("Run failed: {}", yetty::error_msg(runResult));
        return 1;
    }

    return 0;
}

#else
//-----------------------------------------------------------------------------
// Desktop/iOS Entry Point (Linux/macOS/Windows/iOS)
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    setup_logging(argc, argv);

    ydebug("=== YETTY BUILD ===");
    signal(SIGINT, sigint_handler);

    auto initResult = yetty::InitManager::create();
    if (!initResult) {
        yerror("Failed to initialize platform: {}", yetty::error_msg(initResult));
        std::cerr << "ERROR: Failed to initialize platform: " << yetty::error_msg(initResult) << std::endl;
        return 1;
    }
    auto initManager = *initResult;

    auto runResult = initManager->run(argc, argv);
    if (!runResult) {
        yerror("Run failed: {}", yetty::error_msg(runResult));
        std::cerr << "ERROR: " << yetty::error_msg(runResult) << std::endl;
        return 1;
    }

    ydebug("Main thread: InitManager::run() returned");
    return 0;
}
#endif
