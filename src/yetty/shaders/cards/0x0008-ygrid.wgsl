// Shader glyph: YGrid card (codepoint 0x100008 / 1048584)
// Renders a terminal-like cell grid from cardStorage buffer
// Cells contain pre-translated glyph indices, colors, and attributes
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   word 0: bufferOffset (u32) - byte offset in cardStorage
//   word 1: cols (u16) | rows (u16)
//   word 2: cellWidth (f32 bits) - grid cell pixel width
//   word 3: cellHeight (f32 bits) - grid cell pixel height
//   word 4: widthCells (u16) | heightCells (u16) - card size in terminal cells
//   word 5: bgColor (u32) - default background RGBA
//   word 6: fgColor (u32) - default foreground RGBA
//   word 7: pixelRange (f32 bits) - MSDF pixel range
//   word 8: scale (f32 bits) - font scale factor
//   word 9:  viewZoom (f32 bits) - zoom factor
//   word 10: viewPanX (f32 bits) - horizontal pan in pixels
//   word 11: viewPanY (f32 bits) - vertical pan in pixels
//   words 12-15: reserved
//
// Buffer data: 3 u32s per cell (glyph, fg_packed, bg_packed), row-major
//   word 0: glyphIndex (u32) - translated glyph index
//   word 1: fg packed (fgR | fgG<<8 | fgB<<16 | alpha<<24)
//   word 2: bg packed (bgR | bgG<<8 | bgB<<16 | style<<24)
//
// Style byte (bits of bg word 24-31):
//   bits 0:   bold
//   bits 1:   italic
//   bits 2-3: underline type (0=none, 1=single, 2=double, 3=curly)
//   bit  4:   strikethrough
//   bits 5-7: font type (0=MSDF, 1=bitmap, 2=shader, 3=card, 4=vector, 5=coverage, 6=raster)
// =============================================================================

fn shaderGlyph_1048584(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode card cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata
    let bufferOffset = cardMetadata[metaOffset + 0u];
    let gridDims = cardMetadata[metaOffset + 1u];
    let gridCols = gridDims & 0xFFFFu;
    let gridRows = (gridDims >> 16u) & 0xFFFFu;

    let cellWidth = bitcast<f32>(cardMetadata[metaOffset + 2u]);
    let cellHeight = bitcast<f32>(cardMetadata[metaOffset + 3u]);

    let cardDims = cardMetadata[metaOffset + 4u];
    let cardWidthCells = cardDims & 0xFFFFu;
    let cardHeightCells = (cardDims >> 16u) & 0xFFFFu;

    let bgColorDefault = cardMetadata[metaOffset + 5u];
    let fgColorDefault = cardMetadata[metaOffset + 6u];

    let pixelRange = bitcast<f32>(cardMetadata[metaOffset + 7u]);
    let scale = bitcast<f32>(cardMetadata[metaOffset + 8u]);

    let viewZoom = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let viewPanX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let viewPanY = bitcast<f32>(cardMetadata[metaOffset + 11u]);

    // Default background color
    let defBg = unpackColor(bgColorDefault);

    // Guard against zero dimensions
    if (gridCols == 0u || gridRows == 0u || cellWidth <= 0.0 || cellHeight <= 0.0) {
        return defBg;
    }

    // Clamp zoom to sane range
    let zoom = clamp(viewZoom, 0.1, 20.0);

    // Calculate pixel position within the entire card
    let cardPixelX = f32(relCol) * grid.cellSize.x + localUV.x * grid.cellSize.x;
    let cardPixelY = f32(relRow) * grid.cellSize.y + localUV.y * grid.cellSize.y;

    // Apply view transform (pan then inverse-zoom)
    let viewPixelX = (cardPixelX - viewPanX) / zoom;
    let viewPixelY = (cardPixelY - viewPanY) / zoom;

    // Determine which grid cell this pixel falls in
    if (viewPixelX < 0.0 || viewPixelY < 0.0) {
        return defBg;
    }
    let gridCol = u32(viewPixelX / cellWidth);
    let gridRow = u32(viewPixelY / cellHeight);

    // Bounds check
    if (gridCol >= gridCols || gridRow >= gridRows) {
        return defBg;
    }

    // Read cell from cardStorage (3 floats per cell)
    let cellIdx = gridRow * gridCols + gridCol;
    let storageBase = bufferOffset / 4u + cellIdx * 3u;

    let glyphIndex = bitcast<u32>(cardStorage[storageBase + 0u]);
    let fgPacked = bitcast<u32>(cardStorage[storageBase + 1u]);
    let bgPacked = bitcast<u32>(cardStorage[storageBase + 2u]);

    // Unpack colors
    let cellFg = vec3<f32>(
        f32(fgPacked & 0xFFu) / 255.0,
        f32((fgPacked >> 8u) & 0xFFu) / 255.0,
        f32((fgPacked >> 16u) & 0xFFu) / 255.0
    );
    let cellBg = vec3<f32>(
        f32(bgPacked & 0xFFu) / 255.0,
        f32((bgPacked >> 8u) & 0xFFu) / 255.0,
        f32((bgPacked >> 16u) & 0xFFu) / 255.0
    );

    // Extract style/attributes
    let cellAttrs = (bgPacked >> 24u) & 0xFFu;
    let isBold = (cellAttrs & 0x01u) != 0u;
    let isItalic = (cellAttrs & 0x02u) != 0u;
    let underlineType = (cellAttrs >> 2u) & 0x03u;
    let hasStrikethrough = (cellAttrs & 0x10u) != 0u;
    let fontType = (cellAttrs >> 5u) & 0x07u;

    // Local pixel position within this grid cell (in view space)
    let localPxX = viewPixelX - f32(gridCol) * cellWidth;
    let localPxY = viewPixelY - f32(gridRow) * cellHeight;
    let localPx = vec2<f32>(localPxX, localPxY);

    // Effective cell size after zoom (for MSDF rendering)
    let effectiveCellW = cellWidth * zoom;
    let effectiveCellH = cellHeight * zoom;

    // Start with background
    var finalColor = cellBg;
    var hasGlyph = false;

    // Skip empty glyphs
    let skipGlyph = glyphIndex == 0u || glyphIndex == 0xFFFFu;

    if (!skipGlyph) {
        if (fontType == FONT_TYPE_BITMAP) {
            // Emoji rendering
            let emoji = emojiMetadata[glyphIndex];
            let emojiAspect = emoji.size.x / emoji.size.y;
            let cellAspect = cellWidth / cellHeight;

            var scaledSize: vec2<f32>;
            if (emojiAspect > cellAspect) {
                scaledSize = vec2<f32>(cellWidth, cellWidth / emojiAspect);
            } else {
                scaledSize = vec2<f32>(cellHeight * emojiAspect, cellHeight);
            }

            let offset = (vec2<f32>(cellWidth, cellHeight) - scaledSize) * 0.5;

            if (localPx.x >= offset.x && localPx.x < offset.x + scaledSize.x &&
                localPx.y >= offset.y && localPx.y < offset.y + scaledSize.y) {
                let emojiLocalPos = (localPx - offset) / scaledSize;
                let uv = mix(emoji.uvMin, emoji.uvMax, emojiLocalPos);
                let emojiColor = textureSampleLevel(emojiTexture, emojiSampler, uv, 0.0);
                finalColor = mix(cellBg, emojiColor.rgb, emojiColor.a);
                hasGlyph = emojiColor.a > 0.01;
            }
        } else if (fontType == FONT_TYPE_MSDF) {
            // MSDF text rendering (same logic as gpu-screen main)
            let glyph = glyphMetadata[glyphIndex];

            let scaledGlyphSize = glyph.size * scale;
            let scaledBearing = glyph.bearing * scale;

            // Baseline at 80% of cell height
            let baseline = cellHeight * 0.8;
            let glyphTop = baseline - scaledBearing.y;
            let glyphLeft = scaledBearing.x;

            let glyphMinPx = vec2<f32>(glyphLeft, glyphTop);
            let glyphMaxPx = vec2<f32>(glyphLeft + scaledGlyphSize.x, glyphTop + scaledGlyphSize.y);

            if (localPx.x >= glyphMinPx.x && localPx.x < glyphMaxPx.x &&
                localPx.y >= glyphMinPx.y && localPx.y < glyphMaxPx.y) {
                let glyphLocalPos = (localPx - glyphMinPx) / scaledGlyphSize;
                let uv = mix(glyph.uvMin, glyph.uvMax, glyphLocalPos);

                let msdf = textureSampleLevel(fontTexture, fontSampler, uv, 0.0);
                let sd = median(msdf.r, msdf.g, msdf.b);

                // Scale pixel range by zoom for crisp text at all zoom levels
                let screenPxRange = pixelRange * scale * zoom;
                let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

                finalColor = mix(cellBg, cellFg, alpha);
                hasGlyph = alpha > 0.01;
            }
        }
        // Vector/coverage/raster could be added here in future
    }

    // Text decorations
    if (underlineType > 0u) {
        if (isInUnderline(localPxY, cellHeight, underlineType, scale)) {
            finalColor = cellFg;
        }
    }

    if (hasStrikethrough) {
        if (isInStrikethrough(localPxY, cellHeight, scale)) {
            finalColor = cellFg;
        }
    }

    return finalColor;
}
