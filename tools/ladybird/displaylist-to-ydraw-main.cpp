// displaylist-to-ydraw - Convert JSON DisplayList to YDraw OSC output
// Usage: displaylist-to-ydraw [options] < input.json
//        libweb-renderer URL | displaylist-to-ydraw

#include "displaylist-json.h"
#include "displaylist-to-ydraw.h"
#include "ydraw-buffer-standalone.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " [options]\n"
              << "\n"
              << "Reads JSON DisplayList from stdin, outputs YDraw OSC 666666 to stdout.\n"
              << "\n"
              << "Options:\n"
              << "  --card-id <id>      Card ID for OSC output (default: 0)\n"
              << "  --cell-width <n>    Terminal cell width (default: 80)\n"
              << "  --cell-height <n>   Terminal cell height (default: 24)\n"
              << "  --debug             Print debug info to stderr\n"
              << "  --help              Show this help\n"
              << "\n"
              << "Example:\n"
              << "  echo '{\"width\":800,\"height\":600,\"commands\":[{\"type\":\"FillRect\",\"rect\":{\"x\":10,\"y\":10,\"width\":100,\"height\":50},\"color\":{\"r\":255,\"g\":0,\"b\":0,\"a\":255}}]}' | " << progName << "\n";
}

int main(int argc, char* argv[]) {
    int cardId = 0;
    int cellWidth = 80;
    int cellHeight = 24;
    bool debug = false;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (std::strcmp(argv[i], "--card-id") == 0 && i + 1 < argc) {
            cardId = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--cell-width") == 0 && i + 1 < argc) {
            cellWidth = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--cell-height") == 0 && i + 1 < argc) {
            cellHeight = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--debug") == 0) {
            debug = true;
        } else {
            std::cerr << "Unknown option: " << argv[i] << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    // Read JSON from stdin
    std::ostringstream ss;
    ss << std::cin.rdbuf();
    std::string json = ss.str();

    if (json.empty()) {
        std::cerr << "Error: No input\n";
        return 1;
    }

    if (debug) {
        std::cerr << "Input JSON size: " << json.size() << " bytes\n";
    }

    try {
        // Parse JSON to DisplayList
        displaylist::DisplayList list = displaylist::parseJSON(json);

        if (debug) {
            std::cerr << "Parsed DisplayList: " << list.width << "x" << list.height
                      << ", " << list.commands.size() << " commands\n";
        }

        // Convert to YDraw
        ydraw::Buffer buffer;
        displaylist::DisplayListToYDraw converter(buffer);
        converter.execute(list);

        if (debug) {
            std::cerr << "Generated " << buffer.primitiveCount() << " primitives, "
                      << buffer.textCount() << " text entries\n";
        }

        // Output OSC 666666
        ydraw::outputOSC(buffer, cardId, cellWidth, cellHeight);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
