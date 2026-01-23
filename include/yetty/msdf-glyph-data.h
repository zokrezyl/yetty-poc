#pragma once

#include <cstdint>

namespace yetty {

// Stored in CDB - fixed size for binary compatibility
struct MsdfGlyphData {
    uint32_t codepoint;
    uint16_t width;          // Bitmap width in pixels
    uint16_t height;         // Bitmap height in pixels
    float bearingX;          // Horizontal bearing
    float bearingY;          // Vertical bearing
    float sizeX;             // Logical size X
    float sizeY;             // Logical size Y
    float advance;           // Horizontal advance
    // Followed by: width * height * 4 bytes of RGBA pixel data
};

// CDB key format: 4-byte codepoint (little-endian)
// CDB value format: MsdfGlyphData header + RGBA pixel data

constexpr uint32_t MSDF_CDB_MAGIC = 0x4D534446; // "MSDF"
constexpr uint32_t MSDF_CDB_VERSION = 1;

} // namespace yetty
