#include "ygrid.h"
#include <cstring>

namespace yetty::ygrid {

void translateCodepoint(uint32_t cp, uint32_t& glyphIdx, uint8_t& fontType,
                        const GlyphLookup& msdfLookup, const GlyphLookup& bitmapLookup) {
    if (cp == 0) cp = ' ';

    if (cp >= VECTOR_GLYPH_BASE && cp <= VECTOR_GLYPH_END) {
        fontType = FONT_TYPE_VECTOR;
        glyphIdx = cp;
    } else if (cp >= COVERAGE_GLYPH_BASE && cp <= COVERAGE_GLYPH_END) {
        fontType = FONT_TYPE_COVERAGE;
        glyphIdx = cp;
    } else if (cp >= RASTER_GLYPH_BASE && cp <= RASTER_GLYPH_END) {
        fontType = FONT_TYPE_RASTER;
        glyphIdx = cp;
    } else if (isEmoji(cp) && bitmapLookup) {
        fontType = FONT_TYPE_BITMAP;
        glyphIdx = bitmapLookup(cp, true);
    } else if (msdfLookup) {
        fontType = FONT_TYPE_MSDF;
        glyphIdx = msdfLookup(cp, false);
    } else {
        fontType = FONT_TYPE_MSDF;
        glyphIdx = cp;
    }
}

Result<std::pair<uint8_t, uint8_t>> parsePayload(
    const std::string& payload,
    std::vector<GridCell>& cells,
    uint8_t currentCols,
    uint8_t currentRows,
    const GlyphLookup& msdfLookup,
    const GlyphLookup& bitmapLookup,
    uint32_t defaultSpaceGlyph) {

    if (payload.size() < 8) {
        return Err<std::pair<uint8_t, uint8_t>>("YGrid: payload too small for header");
    }

    const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.data());
    size_t offset = 0;

    // Read header
    uint32_t magic;
    std::memcpy(&magic, data + offset, 4); offset += 4;
    if (magic != YGRD_MAGIC) {
        return Err<std::pair<uint8_t, uint8_t>>("YGrid: invalid magic");
    }

    uint16_t flags;
    std::memcpy(&flags, data + offset, 2); offset += 2;
    uint8_t cols = data[offset++];
    uint8_t rows = data[offset++];

    bool fullUpdate = (flags & YGRD_FLAG_FULL) != 0;

    auto initCells = [&](uint8_t c, uint8_t r) {
        cells.resize(static_cast<size_t>(c) * r);
        for (auto& cell : cells) {
            cell = {defaultSpaceGlyph, 200, 200, 200, 255, 0, 0, 0,
                    static_cast<uint8_t>(FONT_TYPE_MSDF << 5)};
        }
    };

    if (fullUpdate) {
        // Resize grid if dimensions changed
        if (cols != currentCols || rows != currentRows) {
            currentCols = cols;
            currentRows = rows;
            initCells(currentCols, currentRows);
        }

        size_t expectedSize = offset + static_cast<size_t>(currentCols) * currentRows * sizeof(WireCell);
        if (payload.size() < expectedSize) {
            return Err<std::pair<uint8_t, uint8_t>>("YGrid: payload too small for full grid");
        }

        const WireCell* wireCells = reinterpret_cast<const WireCell*>(data + offset);
        for (uint32_t i = 0; i < static_cast<uint32_t>(currentCols) * currentRows; i++) {
            const auto& wc = wireCells[i];
            uint32_t glyphIdx;
            uint8_t fontType;
            translateCodepoint(wc.codepoint, glyphIdx, fontType, msdfLookup, bitmapLookup);

            // Preserve text attrs (bits 0-4) from wire, set font type (bits 5-7)
            uint8_t style = (wc.style & 0x1F) | ((fontType & 0x07) << 5);

            cells[i] = {glyphIdx, wc.fgR, wc.fgG, wc.fgB, wc.alpha,
                        wc.bgR, wc.bgG, wc.bgB, style};
        }
    } else {
        // Partial update
        if (payload.size() < offset + 4) {
            return Err<std::pair<uint8_t, uint8_t>>("YGrid: payload too small for partial header");
        }

        // Ensure grid is initialized
        if (currentCols == 0 || currentRows == 0) {
            currentCols = cols;
            currentRows = rows;
            initCells(currentCols, currentRows);
        }

        uint32_t count;
        std::memcpy(&count, data + offset, 4); offset += 4;

        // Each entry: row(u8) + col(u8) + WireCell(12) = 14 bytes
        size_t entrySize = 2 + sizeof(WireCell);
        if (payload.size() < offset + count * entrySize) {
            return Err<std::pair<uint8_t, uint8_t>>("YGrid: payload too small for partial entries");
        }

        for (uint32_t i = 0; i < count; i++) {
            uint8_t row = data[offset++];
            uint8_t col = data[offset++];

            WireCell wc;
            std::memcpy(&wc, data + offset, sizeof(WireCell));
            offset += sizeof(WireCell);

            if (row < currentRows && col < currentCols) {
                uint32_t glyphIdx;
                uint8_t fontType;
                translateCodepoint(wc.codepoint, glyphIdx, fontType, msdfLookup, bitmapLookup);

                uint8_t style = (wc.style & 0x1F) | ((fontType & 0x07) << 5);

                size_t idx = static_cast<size_t>(row) * currentCols + col;
                cells[idx] = {glyphIdx, wc.fgR, wc.fgG, wc.fgB, wc.alpha,
                              wc.bgR, wc.bgG, wc.bgB, style};
            }
        }
    }

    return Ok(std::make_pair(currentCols, currentRows));
}

} // namespace yetty::ygrid
