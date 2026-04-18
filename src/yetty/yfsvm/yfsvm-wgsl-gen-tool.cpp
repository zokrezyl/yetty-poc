// yfsvm WGSL Generator Tool
//
// Generates src/yetty/shaders/lib/yfsvm.wgsl from opcode definitions.
// Run at build time to keep C++ and WGSL in sync.

#include "yfsvm-wgsl-gen.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output.wgsl>\n";
        return 1;
    }

    std::string wgsl = yetty::yfsvm::generateWGSL();

    std::ofstream out(argv[1]);
    if (!out) {
        std::cerr << "Error: cannot open " << argv[1] << " for writing\n";
        return 1;
    }

    out << wgsl;
    out.close();

    std::cout << "Generated " << argv[1] << " (" << wgsl.size() << " bytes)\n";
    return 0;
}
