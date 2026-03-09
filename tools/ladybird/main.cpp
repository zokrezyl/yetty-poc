// ladybird-ydraw: Test YDraw buffer standalone output
//
// Outputs OSC 666666 sequences for yetty's card system

#include "ydraw-buffer-standalone.h"

#include <iostream>
#include <string>

void printUsage(const char* progname) {
    std::cerr << "Usage: " << progname << " [options]\n";
    std::cerr << "\nOptions:\n";
    std::cerr << "  --width W      Set viewport width (default: 800)\n";
    std::cerr << "  --height H     Set viewport height (default: 600)\n";
    std::cerr << "  --cells-w W    Set terminal cell width (default: 80)\n";
    std::cerr << "  --cells-h H    Set terminal cell height (default: 30)\n";
    std::cerr << "  --help         Show this help\n";
    std::cerr << "\nOutputs OSC 666666 sequence with YDraw primitives.\n";
}

void generateTestPattern(ydraw::Buffer& buffer, float width, float height) {
    using namespace ydraw;

    // White background
    buffer.addBox(0, 0, width, height, packColor(255, 255, 255));

    // Header bar (dark gray)
    buffer.addBox(0, 0, width, 60, packColor(51, 51, 51));

    // Navigation buttons (rounded boxes)
    buffer.addRoundedBox(10, 15, 30, 30, 4, packColor(70, 70, 70));
    buffer.addRoundedBox(50, 15, 30, 30, 4, packColor(70, 70, 70));
    buffer.addRoundedBox(90, 15, 30, 30, 4, packColor(70, 70, 70));

    // URL bar
    buffer.addRoundedBox(130, 15, width - 150, 30, 15, packColor(45, 45, 45));

    // Page title
    buffer.addText("LibWeb YDraw Renderer", 20, 80, 24, packColor(0, 0, 0));

    // Subtitle
    buffer.addText("Web rendering to terminal via YDraw", 20, 120, 16, packColor(100, 100, 100));

    // Content boxes
    buffer.addRoundedBox(20, 160, width/2 - 30, 200, 8, packColor(240, 240, 240));
    buffer.addRoundedBox(width/2 + 10, 160, width/2 - 30, 200, 8, packColor(230, 245, 255));

    // Box shadow demo
    buffer.addRoundedBox(25, 405, 200, 100, 8, packColor(0, 0, 0, 50)); // Shadow
    buffer.addRoundedBox(20, 400, 200, 100, 8, packColor(255, 255, 255));

    // Lines demo
    buffer.addSegment(250, 420, 450, 420, 2, packColor(0, 100, 200));
    buffer.addSegment(250, 450, 450, 450, 3, packColor(200, 100, 0));

    // Circle demo
    buffer.addCircle(540, 440, 40, packColor(100, 200, 100));

    // Gradient approximation (multiple boxes)
    for (int i = 0; i < 10; i++) {
        float x = 20 + i * 20;
        uint8_t r = static_cast<uint8_t>(255 - i * 15);
        uint8_t b = static_cast<uint8_t>(100 + i * 15);
        buffer.addBox(x, 520, 20, 50, packColor(r, 100, b));
    }

    // Footer
    buffer.addText("Powered by YDraw SDF Rendering", 20, height - 30, 12, packColor(150, 150, 150));
}

int main(int argc, char** argv) {
    float width = 800;
    float height = 600;
    int cellsW = 80;
    int cellsH = 30;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--width" && i + 1 < argc) {
            width = std::stof(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            height = std::stof(argv[++i]);
        } else if (arg == "--cells-w" && i + 1 < argc) {
            cellsW = std::stoi(argv[++i]);
        } else if (arg == "--cells-h" && i + 1 < argc) {
            cellsH = std::stoi(argv[++i]);
        }
    }

    // Create YDraw buffer and generate test pattern
    ydraw::Buffer buffer;
    buffer.setSize(width, height);

    generateTestPattern(buffer, width, height);

    // Output as OSC sequence
    ydraw::outputOSC(buffer, 1, cellsW, cellsH);

    std::cerr << "Rendered " << buffer.primitiveCount() << " primitives, "
              << buffer.textCount() << " text spans\n";

    return 0;
}
