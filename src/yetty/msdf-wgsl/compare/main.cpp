/**
 * Compare MsdfGlyphData between two CDB files (e.g. CPU vs GPU generated)
 */

#include <yetty/msdf-glyph-data.h>

extern "C" {
#include <cdb.h>
}

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/mman.h>

using yetty::MsdfGlyphData;

struct CdbReader {
    int fd = -1;
    void* mapped = nullptr;
    size_t size = 0;
    struct cdb cdb = {};

    bool open(const char* path) {
        fd = ::open(path, O_RDONLY);
        if (fd < 0) { perror(path); return false; }

        struct stat st;
        fstat(fd, &st);
        size = st.st_size;

        mapped = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0);
        if (mapped == MAP_FAILED) { close(fd); return false; }

        cdb_init(&cdb, fd);
        return true;
    }

    bool readGlyph(uint32_t codepoint, MsdfGlyphData& header, std::vector<uint8_t>& pixels) {
        char key[4];
        key[0] = codepoint & 0xFF;
        key[1] = (codepoint >> 8) & 0xFF;
        key[2] = (codepoint >> 16) & 0xFF;
        key[3] = (codepoint >> 24) & 0xFF;

        if (cdb_find(&cdb, key, 4) <= 0) return false;

        unsigned int dlen = cdb_datalen(&cdb);
        unsigned int dpos = cdb_datapos(&cdb);

        if (dlen < sizeof(MsdfGlyphData)) return false;

        cdb_read(&cdb, reinterpret_cast<char*>(&header), sizeof(header), dpos);

        size_t pixelBytes = dlen - sizeof(MsdfGlyphData);
        pixels.resize(pixelBytes);
        if (pixelBytes > 0) {
            cdb_read(&cdb, reinterpret_cast<char*>(pixels.data()), pixelBytes,
                     dpos + sizeof(MsdfGlyphData));
        }
        return true;
    }

    ~CdbReader() {
        if (mapped && mapped != MAP_FAILED) munmap(mapped, size);
        if (fd >= 0) ::close(fd);
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

    CdbReader a, b;
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
