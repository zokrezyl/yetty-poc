#pragma once

#include <yetty/grid-cell.h>
#include <yetty/result.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace yetty {

// Forward declarations
class MsMsdfFont;
class BmFont;

namespace ygrid {

//=============================================================================
// Wire format constants
//=============================================================================

// Wire protocol cell (12 bytes) - same layout as Cell but glyph = UTF codepoint
struct WireCell {
    uint32_t codepoint;    // Unicode codepoint (NOT glyph index)
    uint8_t fgR, fgG, fgB, alpha;
    uint8_t bgR, bgG, bgB, style;
};
static_assert(sizeof(WireCell) == 12, "WireCell must be 12 bytes");

// YGRD wire format magic
static constexpr uint32_t YGRD_MAGIC = 0x59475244; // "YGRD"

// YGRD flags
static constexpr uint16_t YGRD_FLAG_FULL = 0x0001;

//=============================================================================
// Font type constants (match gpu-screen.cpp / gpu-screen.wgsl)
//=============================================================================

static constexpr uint8_t FONT_TYPE_MSDF    = 0;
static constexpr uint8_t FONT_TYPE_BITMAP  = 1;
static constexpr uint8_t FONT_TYPE_SHADER  = 2;
static constexpr uint8_t FONT_TYPE_CARD    = 3;
static constexpr uint8_t FONT_TYPE_VECTOR  = 4;
static constexpr uint8_t FONT_TYPE_COVERAGE = 5;
static constexpr uint8_t FONT_TYPE_RASTER  = 6;

//=============================================================================
// Glyph range constants
//=============================================================================

static constexpr uint32_t CARD_GLYPH_BASE     = 0x100000;
static constexpr uint32_t CARD_GLYPH_END      = 0x100FFF;
static constexpr uint32_t SHADER_GLYPH_BASE   = 0x101000;
static constexpr uint32_t SHADER_GLYPH_END    = 0x10FFFD;
static constexpr uint32_t VECTOR_GLYPH_BASE   = 0xF0000;
static constexpr uint32_t VECTOR_GLYPH_END    = 0xF0FFF;
static constexpr uint32_t COVERAGE_GLYPH_BASE = 0xF1000;
static constexpr uint32_t COVERAGE_GLYPH_END  = 0xF1FFF;
static constexpr uint32_t RASTER_GLYPH_BASE   = 0xF2000;
static constexpr uint32_t RASTER_GLYPH_END    = 0xF2FFF;

//=============================================================================
// Utility functions
//=============================================================================

// Check if codepoint is an emoji
inline bool isEmoji(uint32_t cp) {
    if (cp >= 0x1F600 && cp <= 0x1F64F) return true;
    if (cp >= 0x1F300 && cp <= 0x1F5FF) return true;
    if (cp >= 0x1F680 && cp <= 0x1F6FF) return true;
    if (cp >= 0x1F900 && cp <= 0x1F9FF) return true;
    if (cp >= 0x1FA00 && cp <= 0x1FA6F) return true;
    if (cp >= 0x1FA70 && cp <= 0x1FAFF) return true;
    if (cp >= 0x2600 && cp <= 0x26FF) return true;
    if (cp >= 0x2700 && cp <= 0x27BF) return true;
    if (cp >= 0xFE00 && cp <= 0xFE0F) return true;
    if (cp >= 0x200D && cp <= 0x200D) return true;
    return false;
}

//=============================================================================
// GridTranslator - translates wire format to internal Cell format
//=============================================================================

// Callback type for glyph lookup from fonts
using GlyphLookup = std::function<uint32_t(uint32_t codepoint, bool isBitmap)>;

// Translate a UTF codepoint to glyph index + font type
void translateCodepoint(uint32_t cp, uint32_t& glyphIdx, uint8_t& fontType,
                        const GlyphLookup& msdfLookup, const GlyphLookup& bitmapLookup);

// Parse YGRD wire format payload into cells
// Returns cols, rows on success
Result<std::pair<uint8_t, uint8_t>> parsePayload(
    const std::string& payload,
    std::vector<GridCell>& cells,
    uint8_t currentCols,
    uint8_t currentRows,
    const GlyphLookup& msdfLookup,
    const GlyphLookup& bitmapLookup,
    uint32_t defaultSpaceGlyph);

} // namespace ygrid
} // namespace yetty
