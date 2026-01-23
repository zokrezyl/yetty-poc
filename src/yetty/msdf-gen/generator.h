#pragma once

#include <yetty/msdf-glyph-data.h>
#include <string>
#include <vector>
#include <functional>
#include <cstdint>

namespace yetty {
namespace msdfgen {

// Progress callback: (current, total, message)
using ProgressCallback = std::function<void(size_t, size_t, const std::string&)>;

// Configuration for MSDF generation
struct GeneratorConfig {
    std::string fontPath;           // Path to font file (TTF/OTF)
    std::string outputDir;          // Output directory for CDB files
    float fontSize = 32.0f;         // Font size in pixels
    float pixelRange = 4.0f;        // MSDF pixel range (higher = better AA)
    int threadCount = 0;            // Thread count (0 = auto-detect)
    bool includeNerdFonts = true;   // Include Nerd Font symbols
    bool verbose = false;           // Verbose output
};

// Result of generation
struct GeneratorResult {
    bool success = false;
    std::string error;
    size_t glyphsGenerated = 0;
    std::vector<std::string> outputFiles;
};

// Generate MSDF glyph databases for a font
// Produces separate CDB files per variant: font-regular.cdb, font-bold.cdb, etc.
GeneratorResult generate(const GeneratorConfig& config,
                         ProgressCallback progress = nullptr);

// Get default charset to generate (Basic Latin + Extended + Nerd Fonts)
std::vector<uint32_t> getDefaultCharset(bool includeNerdFonts = true);

} // namespace msdfgen
} // namespace yetty
