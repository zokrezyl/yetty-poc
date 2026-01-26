/**
 * CDB comparison tool for MSDF glyph databases
 * Compares ALL glyphs between CPU-generated and GPU-generated CDB files
 */

#include <yetty/msdf-glyph-data.h>

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

extern "C" {
#include <cdb.h>
}

using yetty::MsdfGlyphData;

// Same charset as generators
std::vector<uint32_t> getDefaultCharset(bool includeNerdFonts) {
    std::vector<uint32_t> charset;

    // Basic Latin (ASCII printable)
    for (uint32_t c = 0x20; c <= 0x7E; ++c) charset.push_back(c);

    // Latin-1 Supplement
    for (uint32_t c = 0xA0; c <= 0xFF; ++c) charset.push_back(c);

    // Latin Extended-A
    for (uint32_t c = 0x100; c <= 0x17F; ++c) charset.push_back(c);

    // Latin Extended-B (partial)
    for (uint32_t c = 0x180; c <= 0x24F; ++c) charset.push_back(c);

    // Greek and Coptic
    for (uint32_t c = 0x370; c <= 0x3FF; ++c) charset.push_back(c);

    // Cyrillic
    for (uint32_t c = 0x400; c <= 0x4FF; ++c) charset.push_back(c);

    // General Punctuation
    for (uint32_t c = 0x2000; c <= 0x206F; ++c) charset.push_back(c);

    // Currency Symbols
    for (uint32_t c = 0x20A0; c <= 0x20CF; ++c) charset.push_back(c);

    // Arrows
    for (uint32_t c = 0x2190; c <= 0x21FF; ++c) charset.push_back(c);

    // Mathematical Operators
    for (uint32_t c = 0x2200; c <= 0x22FF; ++c) charset.push_back(c);

    // Box Drawing
    for (uint32_t c = 0x2500; c <= 0x257F; ++c) charset.push_back(c);

    // Block Elements
    for (uint32_t c = 0x2580; c <= 0x259F; ++c) charset.push_back(c);

    // Geometric Shapes
    for (uint32_t c = 0x25A0; c <= 0x25FF; ++c) charset.push_back(c);

    if (includeNerdFonts) {
        // Powerline symbols
        for (uint32_t c = 0xE0A0; c <= 0xE0D7; ++c) charset.push_back(c);

        // Seti-UI + Custom
        for (uint32_t c = 0xE5FA; c <= 0xE6AC; ++c) charset.push_back(c);

        // Devicons
        for (uint32_t c = 0xE700; c <= 0xE7C5; ++c) charset.push_back(c);

        // Font Awesome Extension
        for (uint32_t c = 0xE200; c <= 0xE2A9; ++c) charset.push_back(c);

        // Material Design Icons
        for (uint32_t c = 0xF0001; c <= 0xF1AF0; ++c) charset.push_back(c);

        // Weather Icons
        for (uint32_t c = 0xE300; c <= 0xE3E3; ++c) charset.push_back(c);

        // Octicons
        for (uint32_t c = 0xF400; c <= 0xF532; ++c) charset.push_back(c);

        // Codicons
        for (uint32_t c = 0xEA60; c <= 0xEBEB; ++c) charset.push_back(c);
    }

    return charset;
}

bool lookupGlyph(struct cdb* cdb, uint32_t codepoint, MsdfGlyphData& out, size_t& pixelSize) {
    char key[4];
    key[0] = codepoint & 0xFF;
    key[1] = (codepoint >> 8) & 0xFF;
    key[2] = (codepoint >> 16) & 0xFF;
    key[3] = (codepoint >> 24) & 0xFF;

    if (cdb_find(cdb, key, 4) <= 0) {
        return false;
    }

    size_t vlen = cdb_datalen(cdb);
    if (vlen < sizeof(MsdfGlyphData)) {
        return false;
    }

    char* data = new char[vlen];
    if (cdb_read(cdb, data, vlen, cdb_datapos(cdb)) < 0) {
        delete[] data;
        return false;
    }

    memcpy(&out, data, sizeof(MsdfGlyphData));
    pixelSize = vlen - sizeof(MsdfGlyphData);

    delete[] data;
    return true;
}

int main(int argc, char* argv[]) {
    const char* cpuCdb = nullptr;
    const char* gpuCdb = nullptr;
    bool verbose = false;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--cpu" && i + 1 < argc) {
            cpuCdb = argv[++i];
        } else if (arg == "--gpu" && i + 1 < argc) {
            gpuCdb = argv[++i];
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "--help" || arg == "-h") {
            printf("CDB Comparison Tool\n\n");
            printf("Usage: %s [--cpu <cpu.cdb>] [--gpu <gpu.cdb>] [-v]\n\n", argv[0]);
            printf("Compares ALL glyph metadata between two CDB files.\n");
            printf("  -v, --verbose  Show details for each mismatched glyph\n");
            return 0;
        }
    }

    if (!cpuCdb || !gpuCdb) {
        cpuCdb = "/home/misi/.cache/yetty/msdf-font-cache/DejaVuSansMNerdFontMono-Regular.cdb";
        gpuCdb = "/tmp/msdfgl-gpu-test/DejaVuSansMNerdFontMono-Regular-gpu.cdb";
    }

    printf("Comparing CDB files:\n");
    printf("  CPU: %s\n", cpuCdb);
    printf("  GPU: %s\n\n", gpuCdb);

    // Open both files
    int cpuFd = open(cpuCdb, O_RDONLY);
    int gpuFd = open(gpuCdb, O_RDONLY);
    if (cpuFd < 0) {
        fprintf(stderr, "Failed to open CPU CDB: %s\n", cpuCdb);
        return 1;
    }
    if (gpuFd < 0) {
        fprintf(stderr, "Failed to open GPU CDB: %s\n", gpuCdb);
        close(cpuFd);
        return 1;
    }

    struct cdb cpuCdbHandle, gpuCdbHandle;
    cdb_init(&cpuCdbHandle, cpuFd);
    cdb_init(&gpuCdbHandle, gpuFd);

    auto charset = getDefaultCharset(true);
    printf("Checking %zu glyphs...\n\n", charset.size());

    size_t totalChecked = 0;
    size_t bothFound = 0;
    size_t cpuOnly = 0;
    size_t gpuOnly = 0;
    size_t neitherFound = 0;
    size_t bearingMismatch = 0;
    size_t advanceMismatch = 0;
    size_t sizeMismatch = 0;
    size_t perfectMatch = 0;

    for (uint32_t cp : charset) {
        MsdfGlyphData cpuGlyph, gpuGlyph;
        size_t cpuPixels, gpuPixels;

        bool hasCpu = lookupGlyph(&cpuCdbHandle, cp, cpuGlyph, cpuPixels);
        bool hasGpu = lookupGlyph(&gpuCdbHandle, cp, gpuGlyph, gpuPixels);

        totalChecked++;

        if (!hasCpu && !hasGpu) {
            neitherFound++;
            continue;
        }
        if (hasCpu && !hasGpu) {
            cpuOnly++;
            if (verbose) printf("  [CPU only] U+%04X\n", cp);
            continue;
        }
        if (!hasCpu && hasGpu) {
            gpuOnly++;
            if (verbose) printf("  [GPU only] U+%04X\n", cp);
            continue;
        }

        bothFound++;

        // Compare values (1.5 pixel tolerance for bearings due to different computation methods)
        // msdf-gen uses glyph bounds, msdfgl uses FreeType metrics - slight differences expected
        bool bearingOk = (std::abs(cpuGlyph.bearingX - gpuGlyph.bearingX) <= 1.5f) &&
                         (std::abs(cpuGlyph.bearingY - gpuGlyph.bearingY) <= 1.5f);
        bool advanceOk = std::abs(cpuGlyph.advance - gpuGlyph.advance) <= 0.001f;
        bool sizeOk = (cpuGlyph.width == gpuGlyph.width) && (cpuGlyph.height == gpuGlyph.height);

        if (!bearingOk) {
            bearingMismatch++;
            if (verbose) {
                printf("  [BEARING] U+%04X: CPU=(%.3f,%.3f) GPU=(%.3f,%.3f)\n",
                       cp, cpuGlyph.bearingX, cpuGlyph.bearingY,
                       gpuGlyph.bearingX, gpuGlyph.bearingY);
            }
        }
        if (!advanceOk) {
            advanceMismatch++;
            if (verbose) {
                printf("  [ADVANCE] U+%04X: CPU=%.3f GPU=%.3f\n",
                       cp, cpuGlyph.advance, gpuGlyph.advance);
            }
        }
        if (!sizeOk) {
            sizeMismatch++;
            if (verbose) {
                printf("  [SIZE] U+%04X: CPU=%ux%u GPU=%ux%u\n",
                       cp, cpuGlyph.width, cpuGlyph.height,
                       gpuGlyph.width, gpuGlyph.height);
            }
        }

        if (bearingOk && advanceOk) {
            perfectMatch++;
        }
    }

    cdb_free(&cpuCdbHandle);
    cdb_free(&gpuCdbHandle);
    close(cpuFd);
    close(gpuFd);

    printf("=== SUMMARY ===\n");
    printf("Total glyphs checked: %zu\n", totalChecked);
    printf("Found in both:        %zu\n", bothFound);
    printf("CPU only:             %zu\n", cpuOnly);
    printf("GPU only:             %zu\n", gpuOnly);
    printf("Neither (missing):    %zu\n", neitherFound);
    printf("\n");
    printf("Bearing matches:      %zu / %zu (%.1f%%)\n",
           bothFound - bearingMismatch, bothFound,
           100.0 * (bothFound - bearingMismatch) / bothFound);
    printf("Advance matches:      %zu / %zu (%.1f%%)\n",
           bothFound - advanceMismatch, bothFound,
           100.0 * (bothFound - advanceMismatch) / bothFound);
    printf("Size matches:         %zu / %zu (%.1f%%)\n",
           bothFound - sizeMismatch, bothFound,
           100.0 * (bothFound - sizeMismatch) / bothFound);
    printf("\n");
    printf("Perfect matches (bearing+advance): %zu / %zu (%.1f%%)\n",
           perfectMatch, bothFound, 100.0 * perfectMatch / bothFound);

    if (bearingMismatch > 0 || advanceMismatch > 0) {
        printf("\n*** MISMATCHES DETECTED! Use -v for details ***\n");
        return 1;
    }

    printf("\n*** ALL GLYPHS MATCH! ***\n");
    return 0;
}
