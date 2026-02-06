#include "generator.h"

#include <args.hxx>
#include <ytrace/ytrace.hpp>

#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("MSDF Glyph Generator for yetty",
        "Generates CDB files containing pre-rendered MSDF glyphs for fast font loading.");

    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> fontPath(parser, "font",
        "Path to the font file (TTF/OTF)", args::Options::Required);

    args::Positional<std::string> outputDir(parser, "output",
        "Output directory for CDB files", args::Options::Required);

    args::ValueFlag<float> fontSize(parser, "size",
        "Font size in pixels (default: 32)", {'s', "size"}, 32.0f);

    args::ValueFlag<float> pixelRange(parser, "range",
        "MSDF pixel range for anti-aliasing (default: 4)", {'r', "range"}, 4.0f);

    args::ValueFlag<int> threads(parser, "threads",
        "Number of worker threads (default: auto)", {'j', "threads"}, 0);

    args::Flag noNerdFonts(parser, "no-nerd-fonts",
        "Exclude Nerd Font symbols from generation", {"no-nerd-fonts"});

    args::Flag cjk(parser, "cjk",
        "Include CJK characters (Chinese/Japanese/Korean) - large charset", {"cjk"});

    args::Flag allGlyphs(parser, "all",
        "Generate all glyphs found in the font (ignores charset flags)", {'a', "all"});

    args::Flag verbose(parser, "verbose",
        "Enable verbose output", {'v', "verbose"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help&) {
        std::cout << parser;
        return 0;
    } catch (const args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (const args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    // Configure generator
    yetty::msdfgen::GeneratorConfig config;
    config.fontPath = args::get(fontPath);
    config.outputDir = args::get(outputDir);
    config.fontSize = args::get(fontSize);
    config.pixelRange = args::get(pixelRange);
    config.threadCount = args::get(threads);
    config.includeNerdFonts = !noNerdFonts;
    config.includeCJK = cjk;
    config.allGlyphs = allGlyphs;
    config.verbose = verbose;

    std::cout << "MSDF Generator" << std::endl;
    std::cout << "  Font: " << config.fontPath << std::endl;
    std::cout << "  Output: " << config.outputDir << std::endl;
    std::cout << "  Size: " << config.fontSize << "px" << std::endl;
    std::cout << "  Pixel range: " << config.pixelRange << std::endl;
    if (config.allGlyphs) {
        std::cout << "  Mode: all glyphs in font" << std::endl;
    } else {
        std::cout << "  Nerd Fonts: " << (config.includeNerdFonts ? "yes" : "no") << std::endl;
        std::cout << "  CJK: " << (config.includeCJK ? "yes" : "no") << std::endl;
    }
    std::cout << std::endl;

    // Progress callback
    auto progress = [](size_t current, size_t total, const std::string& msg) {
        float pct = total > 0 ? (100.0f * current / total) : 0.0f;
        std::cout << "\r" << msg << " " << std::fixed << std::setprecision(1)
                  << pct << "% (" << current << "/" << total << ")" << std::flush;
    };

    // Run generator
    auto result = yetty::msdfgen::generate(config, progress);
    std::cout << std::endl;

    if (!result.success) {
        std::cerr << "Error: " << result.error << std::endl;
        return 1;
    }

    std::cout << "Success! Generated " << result.glyphsGenerated << " glyphs." << std::endl;
    std::cout << "Output files:" << std::endl;
    for (const auto& f : result.outputFiles) {
        std::cout << "  " << f << std::endl;
    }

    return 0;
}
