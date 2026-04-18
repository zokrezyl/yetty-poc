/**
 * CDB comparison and validation tool for MSDF glyph databases
 * Compares metadata AND validates pixel data quality
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
#include <algorithm>

extern "C" {
#include <cdb.h>
}

using yetty::MsdfGlyphData;

struct GlyphPixelStats {
    size_t totalPixels = 0;
    size_t zeroPixels = 0;      // All RGB channels are 0
    size_t blackPixels = 0;     // All RGB < 0.1
    size_t whitePixels = 0;     // All RGB > 0.9
    size_t midPixels = 0;       // At least one channel in 0.3-0.7 range (edge region)
    float minR = 1.0f, maxR = 0.0f;
    float minG = 1.0f, maxG = 0.0f;
    float minB = 1.0f, maxB = 0.0f;
    float avgR = 0.0f, avgG = 0.0f, avgB = 0.0f;
};

struct FullGlyphData {
    MsdfGlyphData header;
    std::vector<uint8_t> pixels;
    bool valid = false;
};

FullGlyphData lookupFullGlyph(struct cdb* cdb, uint32_t codepoint) {
    FullGlyphData result;

    char key[4];
    key[0] = codepoint & 0xFF;
    key[1] = (codepoint >> 8) & 0xFF;
    key[2] = (codepoint >> 16) & 0xFF;
    key[3] = (codepoint >> 24) & 0xFF;

    if (cdb_find(cdb, key, 4) <= 0) {
        return result;
    }

    size_t vlen = cdb_datalen(cdb);
    if (vlen < sizeof(MsdfGlyphData)) {
        return result;
    }

    std::vector<char> data(vlen);
    if (cdb_read(cdb, data.data(), vlen, cdb_datapos(cdb)) < 0) {
        return result;
    }

    memcpy(&result.header, data.data(), sizeof(MsdfGlyphData));

    size_t pixelBytes = vlen - sizeof(MsdfGlyphData);
    if (pixelBytes > 0) {
        result.pixels.resize(pixelBytes);
        memcpy(result.pixels.data(), data.data() + sizeof(MsdfGlyphData), pixelBytes);
    }

    result.valid = true;
    return result;
}

GlyphPixelStats analyzePixels(const std::vector<uint8_t>& pixels, uint16_t width, uint16_t height) {
    GlyphPixelStats stats;

    size_t expectedSize = width * height * 4;  // RGBA
    if (pixels.size() < expectedSize || width == 0 || height == 0) {
        return stats;
    }

    double sumR = 0, sumG = 0, sumB = 0;

    for (size_t i = 0; i < width * height; ++i) {
        uint8_t r = pixels[i * 4 + 0];
        uint8_t g = pixels[i * 4 + 1];
        uint8_t b = pixels[i * 4 + 2];

        float fr = r / 255.0f;
        float fg = g / 255.0f;
        float fb = b / 255.0f;

        stats.totalPixels++;
        sumR += fr;
        sumG += fg;
        sumB += fb;

        if (fr < stats.minR) stats.minR = fr;
        if (fg < stats.minG) stats.minG = fg;
        if (fb < stats.minB) stats.minB = fb;
        if (fr > stats.maxR) stats.maxR = fr;
        if (fg > stats.maxG) stats.maxG = fg;
        if (fb > stats.maxB) stats.maxB = fb;

        if (r == 0 && g == 0 && b == 0) {
            stats.zeroPixels++;
        }
        if (fr < 0.1f && fg < 0.1f && fb < 0.1f) {
            stats.blackPixels++;
        }
        if (fr > 0.9f && fg > 0.9f && fb > 0.9f) {
            stats.whitePixels++;
        }
        // Check for edge region (MSDF values near 0.5 indicate edges)
        if ((fr > 0.3f && fr < 0.7f) || (fg > 0.3f && fg < 0.7f) || (fb > 0.3f && fb < 0.7f)) {
            stats.midPixels++;
        }
    }

    if (stats.totalPixels > 0) {
        stats.avgR = sumR / stats.totalPixels;
        stats.avgG = sumG / stats.totalPixels;
        stats.avgB = sumB / stats.totalPixels;
    }

    return stats;
}

void printGlyphAnalysis(uint32_t cp, const FullGlyphData& glyph, const GlyphPixelStats& stats) {
    printf("  U+%04X: %ux%u pixels\n", cp, glyph.header.width, glyph.header.height);
    printf("    Zero pixels: %zu / %zu (%.1f%%)\n",
           stats.zeroPixels, stats.totalPixels,
           stats.totalPixels > 0 ? 100.0 * stats.zeroPixels / stats.totalPixels : 0.0);
    printf("    Black (<0.1): %zu, White (>0.9): %zu, Edge (0.3-0.7): %zu\n",
           stats.blackPixels, stats.whitePixels, stats.midPixels);
    printf("    R: min=%.3f max=%.3f avg=%.3f\n", stats.minR, stats.maxR, stats.avgR);
    printf("    G: min=%.3f max=%.3f avg=%.3f\n", stats.minG, stats.maxG, stats.avgG);
    printf("    B: min=%.3f max=%.3f avg=%.3f\n", stats.minB, stats.maxB, stats.avgB);
}

int main(int argc, char* argv[]) {
    const char* cdbPath = nullptr;
    bool analyze = false;
    int sampleCount = 10;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--cdb" && i + 1 < argc) {
            cdbPath = argv[++i];
        } else if (arg == "--analyze" || arg == "-a") {
            analyze = true;
        } else if (arg == "--samples" && i + 1 < argc) {
            sampleCount = std::atoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            printf("CDB Validation Tool\n\n");
            printf("Usage: %s --cdb <file.cdb> [-a] [--samples N]\n\n", argv[0]);
            printf("Options:\n");
            printf("  --cdb FILE      CDB file to analyze\n");
            printf("  -a, --analyze   Analyze pixel data quality\n");
            printf("  --samples N     Number of glyphs to sample (default: 10)\n");
            return 0;
        }
    }

    if (!cdbPath) {
        cdbPath = "/tmp/msdfgl-gpu-test/DejaVuSansMNerdFontMono-Regular-gpu.cdb";
    }

    printf("Analyzing CDB: %s\n\n", cdbPath);

    int fd = open(cdbPath, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open: %s\n", cdbPath);
        return 1;
    }

    struct cdb cdbHandle;
    cdb_init(&cdbHandle, fd);

    // Sample some common ASCII glyphs
    std::vector<uint32_t> sampleChars = {
        'A', 'B', 'H', 'O', 'W',  // Uppercase
        'a', 'e', 'g', 'o', 'x',  // Lowercase
        '0', '1', '8', '9',       // Digits
        '@', '#', '%', '&',       // Symbols
    };

    size_t totalGlyphs = 0;
    size_t emptyGlyphs = 0;
    size_t allZeroGlyphs = 0;
    size_t validMsdfGlyphs = 0;

    GlyphPixelStats globalStats;

    printf("=== GLYPH ANALYSIS ===\n\n");

    for (size_t i = 0; i < sampleChars.size() && i < (size_t)sampleCount; ++i) {
        uint32_t cp = sampleChars[i];
        auto glyph = lookupFullGlyph(&cdbHandle, cp);

        if (!glyph.valid) {
            printf("  U+%04X ('%c'): NOT FOUND\n", cp, (char)cp);
            continue;
        }

        totalGlyphs++;

        if (glyph.header.width == 0 || glyph.header.height == 0) {
            printf("  U+%04X ('%c'): EMPTY (0x0)\n", cp, (char)cp);
            emptyGlyphs++;
            continue;
        }

        auto stats = analyzePixels(glyph.pixels, glyph.header.width, glyph.header.height);

        // Accumulate global stats
        globalStats.totalPixels += stats.totalPixels;
        globalStats.zeroPixels += stats.zeroPixels;
        globalStats.blackPixels += stats.blackPixels;
        globalStats.whitePixels += stats.whitePixels;
        globalStats.midPixels += stats.midPixels;

        bool isAllZero = (stats.zeroPixels == stats.totalPixels);
        bool hasEdges = (stats.midPixels > 0);

        if (isAllZero) {
            allZeroGlyphs++;
            printf("  U+%04X ('%c'): %ux%u - ALL ZEROS! (BROKEN)\n",
                   cp, (char)cp, glyph.header.width, glyph.header.height);
        } else if (!hasEdges) {
            printf("  U+%04X ('%c'): %ux%u - NO EDGES (suspicious)\n",
                   cp, (char)cp, glyph.header.width, glyph.header.height);
            printf("         R: %.3f-%.3f, G: %.3f-%.3f, B: %.3f-%.3f\n",
                   stats.minR, stats.maxR, stats.minG, stats.maxG, stats.minB, stats.maxB);
        } else {
            validMsdfGlyphs++;
            printf("  U+%04X ('%c'): %ux%u - OK (edges: %zu, avg RGB: %.2f,%.2f,%.2f)\n",
                   cp, (char)cp, glyph.header.width, glyph.header.height,
                   stats.midPixels, stats.avgR, stats.avgG, stats.avgB);
        }

        if (analyze) {
            printGlyphAnalysis(cp, glyph, stats);
            printf("\n");
        }
    }

    cdb_free(&cdbHandle);
    close(fd);

    printf("\n=== SUMMARY ===\n");
    printf("Glyphs checked:    %zu\n", totalGlyphs);
    printf("Empty (0x0):       %zu\n", emptyGlyphs);
    printf("All zeros:         %zu\n", allZeroGlyphs);
    printf("Valid MSDF:        %zu\n", validMsdfGlyphs);

    if (globalStats.totalPixels > 0) {
        printf("\nGlobal pixel stats:\n");
        printf("  Total pixels:    %zu\n", globalStats.totalPixels);
        printf("  Zero pixels:     %zu (%.1f%%)\n", globalStats.zeroPixels,
               100.0 * globalStats.zeroPixels / globalStats.totalPixels);
        printf("  Edge pixels:     %zu (%.1f%%)\n", globalStats.midPixels,
               100.0 * globalStats.midPixels / globalStats.totalPixels);
    }

    if (allZeroGlyphs > 0) {
        printf("\n*** CRITICAL: %zu glyphs have ALL ZERO pixels! ***\n", allZeroGlyphs);
        printf("*** The MSDF generation is NOT working! ***\n");
        return 1;
    }

    if (validMsdfGlyphs == totalGlyphs - emptyGlyphs) {
        printf("\n*** All glyphs have valid MSDF data! ***\n");
    }

    return 0;
}
