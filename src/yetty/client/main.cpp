#include "commands.h"
#include "buffers.h"
#include "cards.h"
#include "event.h"
#include "ui.h"

#include <args.hxx>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace yetty;
using namespace yetty::client;

// Read default socket path from $YETTY_SOCKET env var
static std::string defaultSocketPath() {
    if (auto* env = std::getenv("YETTY_SOCKET")) {
        return env;
    }
    return "";
}

int main(int argc, const char** argv) {
    const std::unordered_map<std::string, CmdFn> commands = {
        {"buffers", cmdBuffers},
        {"cards",   cmdCards},
        {"event",   cmdEvent},
        {"ui",      cmdUi},
    };

    const std::vector<std::string> args(argv + 1, argv + argc);
    args::ArgumentParser parser("yettyc", "yetty RPC client");
    parser.Prog(argv[0]);
    parser.Epilog(
        "commands:\n"
        "  buffers  List card buffers (buffers [-c card])\n"
        "  cards    List cards (cards list)\n"
        "  event    Send events to the terminal (split, close, etc.)\n"
        "  ui       Query the UI tree (ui tree)\n"
    );
    args::HelpFlag help(parser, "help", "Show this help", {'h', "help"});
    args::ValueFlag<std::string> socketFlag(parser, "path",
        "Socket path (default: $YETTY_SOCKET)", {'s', "socket"}, defaultSocketPath());
    args::MapPositional<std::string, CmdFn> command(parser, "command",
        "Command to run", commands);
    command.KickOut(true);

    try {
        auto next = parser.ParseArgs(args);
        if (command) {
            auto socketPath = args::get(socketFlag);
            if (socketPath.empty()) {
                std::cerr << "error: no socket path. Set $YETTY_SOCKET or use --socket.\n";
                return 1;
            }
            CmdContext ctx{argv[0], socketPath};
            auto result = args::get(command)(ctx, next, args.end());
            if (!result) {
                std::cerr << "error: " << error_msg(result) << "\n";
                return 1;
            }
            return 0;
        }
        std::cout << parser;
        return 0;
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::Error& e) {
        std::cerr << e.what() << "\n";
        std::cerr << parser;
        return 1;
    }
}
