/**
 * Compare MsdfGlyphData between two CDB files (e.g. CPU vs GPU generated)
 */

#include <yetty/msdf-glyph-data.h>
#include <yetty/cdb-wrapper.h>

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

using yetty::MsdfGlyphData;

struct CdbFileReader {
    yetty::CdbReader::Ptr reader;

    bool open(const char* path) {
        reader = yetty::CdbReader::open(path);
        return reader != nullptr;
    }

    bool readGlyph(uint32_t codepoint, MsdfGlyphData& header, std::vector<uint8_t>& pixels) {
        auto data = reader->get(codepoint);
        if (!data) return false;

        if (data->size() < sizeof(MsdfGlyphData)) return false;

        std::memcpy(&header, data->data(), sizeof(header));

        size_t pixelBytes = data->size() - sizeof(MsdfGlyphData);
        pixels.resize(pixelBytes);
        if (pixelBytes > 0) {
            std::memcpy(pixels.data(), data->data() + sizeof(MsdfGlyphData), pixelBytes);
        }
        return true;
    }
};

void printHeader(const char* label, const MsdfGlyphData& h) {
    printf("  %-6s codepoint=%-6u w=%-4u h=%-4u bearingX=%-10.4f bearingY=%-10.4f sizeX=%-10.4f sizeY=%-10.4f advance=%-10.4f\n",
           label, h.codepoint, h.width, h.height,
           h.bearingX, h.bearingY, h.sizeX, h.sizeY, h.advance);
}

void printPixelSample(const char* label, const std::vector<uint8_t>& pixels, int width) {
    printf("  %-6s pixels (first row, first 4 px RGBA): ", label);
    int n = std::min((int)pixels.size(), width * 4);
    n = std::min(n, 16);
    for (int i = 0; i < n; i++) {
        printf("%3u ", pixels[i]);
        if ((i + 1) % 4 == 0) printf("| ");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <cdb-a> <cdb-b> [codepoints...]\n"
                        "  Default codepoints: 32(space) 65(A) 97(a) 48(0) 124(|)\n", argv[0]);
        return 1;
    }

    CdbFileReader a, b;
    if (!a.open(argv[1])) { fprintf(stderr, "Failed to open: %s\n", argv[1]); return 1; }
    if (!b.open(argv[2])) { fprintf(stderr, "Failed to open: %s\n", argv[2]); return 1; }

    printf("A: %s\n", argv[1]);
    printf("B: %s\n\n", argv[2]);

    std::vector<uint32_t> codepoints = {32, 65, 97, 48, 124};
    if (argc > 3) {
        codepoints.clear();
        for (int i = 3; i < argc; i++)
            codepoints.push_back(std::stoul(argv[i]));
    }

    for (uint32_t cp : codepoints) {
        printf("=== Codepoint %u (0x%04X) '%c' ===\n", cp, cp, cp >= 32 && cp < 127 ? (char)cp : '?');

        MsdfGlyphData ha{}, hb{};
        std::vector<uint8_t> pa, pb;

        bool foundA = a.readGlyph(cp, ha, pa);
        bool foundB = b.readGlyph(cp, hb, pb);

        if (!foundA) printf("  A: NOT FOUND\n");
        else {
            printHeader("A:", ha);
            if (!pa.empty()) printPixelSample("A:", pa, ha.width);
        }

        if (!foundB) printf("  B: NOT FOUND\n");
        else {
            printHeader("B:", hb);
            if (!pb.empty()) printPixelSample("B:", pb, hb.width);
        }

        if (foundA && foundB) {
            bool match = (ha.width == hb.width && ha.height == hb.height &&
                          ha.bearingX == hb.bearingX && ha.bearingY == hb.bearingY &&
                          ha.sizeX == hb.sizeX && ha.sizeY == hb.sizeY &&
                          ha.advance == hb.advance);
            printf("  %s\n", match ? "MATCH" : "DIFFER");
        }
        printf("\n");
    }

    return 0;
}
