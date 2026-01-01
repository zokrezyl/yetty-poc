//=============================================================================
// yetty - WebGPU Terminal Emulator
//
// Main entry point. Creates and runs the Yetty engine.
//=============================================================================

#include <yetty/yetty.h>
#include <spdlog/spdlog.h>
#include <iostream>

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
