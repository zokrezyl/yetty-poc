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

// GPU-friendly glyph metadata for SSBO (40 bytes, 8-byte aligned)
struct GlyphMetadataGPU {
    float _uvMinX, _uvMinY;     // 8 bytes
    float _uvMaxX, _uvMaxY;     // 8 bytes
    float _sizeX, _sizeY;       // 8 bytes
    float _bearingX, _bearingY; // 8 bytes
    float _advance;             // 4 bytes
    float _pad;                 // 4 bytes (alignment)
};

} // namespace yetty
