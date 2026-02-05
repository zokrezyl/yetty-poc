// Shader glyph: YPdf widget (codepoint 1048584 / U+100008)
// GPU text-only rendering with O(1) UNIFORM GRID SPATIAL HASHING
// + MSDF text rendering for PDF content
//
// Uses text rendering from lib/text.wgsl
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  primitiveOffset (u32) — always 0 for ypdf
//   offset 1:  primitiveCount (u32)  — always 0 for ypdf
//   offset 2:  gridOffset (u32)
//   offset 3:  gridWidth (u32)
//   offset 4:  gridHeight (u32)
//   offset 5:  cellSize (f32)
//   offset 6:  glyphOffset (u32)   - offset of glyph data in cardStorage
//   offset 7:  glyphCount (u32)    - number of text glyphs
//   offset 8:  sceneMinX (f32)     - content bounds (= grid origin)
//   offset 9:  sceneMinY (f32)
//   offset 10: sceneMaxX (f32)
//   offset 11: sceneMaxY (f32)
//   offset 12: widthCells  [15:0] = widthCells,  [31:16] = panX (i16 fixedpoint)
//   offset 13: heightCells [15:0] = heightCells, [31:16] = panY (i16 fixedpoint)
//   offset 14: flags       [15:0] = flags,       [31:16] = zoom (f16)
//   offset 15: bgColor (u32)
//
// Pan fixedpoint: panScene = i16value / 16384.0 * sceneExtent
// Zoom f16: IEEE 754 half-float
// =============================================================================
// PdfGlyph layout (32 bytes = 8 floats):
//   offset 0: x (f32)        - position in scene coords
//   offset 1: y (f32)
//   offset 2: width (f32)    - size in scene coords
//   offset 3: height (f32)
//   offset 4: glyphIndex (u32) - index in glyphMetadata
//   offset 5: color (u32)    - packed RGBA
//   offset 6: layer (u32)
//   offset 7: _pad (u32)
// =============================================================================

const YPDF_MAX_ENTRIES_PER_CELL: u32 = 16u;

// Bit 31 set = glyph index
const YPDF_GLYPH_BIT: u32 = 0x80000000u;
const YPDF_INDEX_MASK: u32 = 0x7FFFFFFFu;

// =============================================================================
// Main shader function - O(1) GRID LOOKUP (text only)
// =============================================================================

fn shaderGlyph_1048584(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode ANSI-compatible cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata
    let gridOffset = cardMetadata[metaOffset + 2u];
    let gridWidth = cardMetadata[metaOffset + 3u];
    let gridHeight = cardMetadata[metaOffset + 4u];
    let cellSize = bitcast<f32>(cardMetadata[metaOffset + 5u]);
    let glyphOffset = cardMetadata[metaOffset + 6u];
    let glyphCount = cardMetadata[metaOffset + 7u];
    let invCellSize = select(0.0, 1.0 / cellSize, cellSize > 0.0);

    // Content bounds (= grid origin)
    let contentMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let contentMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let contentMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let contentMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);

    // Unpack widthCells/heightCells and pan
    let packedWC = cardMetadata[metaOffset + 12u];
    let packedHC = cardMetadata[metaOffset + 13u];
    let widthCells = packedWC & 0xFFFFu;
    let heightCells = packedHC & 0xFFFFu;
    let panXi16 = i32(packedWC >> 16u) - select(0, 65536, (packedWC >> 16u) >= 32768u);
    let panYi16 = i32(packedHC >> 16u) - select(0, 65536, (packedHC >> 16u) >= 32768u);

    // Unpack flags and zoom f16
    let packedFlags = cardMetadata[metaOffset + 14u];
    let zoomF16 = packedFlags >> 16u;

    // f16 -> f32 conversion
    let f16sign = (zoomF16 >> 15u) & 1u;
    let f16exp  = (zoomF16 >> 10u) & 0x1Fu;
    let f16mant = zoomF16 & 0x3FFu;
    let f32bits = select(
        (f16sign << 31u) | ((f16exp + 112u) << 23u) | (f16mant << 13u),
        0u,
        f16exp == 0u
    );
    let zoom = select(bitcast<f32>(f32bits), 1.0, f16exp == 0u);

    let bgColorPacked = cardMetadata[metaOffset + 15u];
    let bgColor = unpackColor(bgColorPacked);

    // Content extent
    let contentW = contentMaxX - contentMinX;
    let contentH = contentMaxY - contentMinY;

    // Reconstruct pan in scene units
    let panX = f32(panXi16) / 16384.0 * contentW;
    let panY = f32(panYi16) / 16384.0 * contentH;

    // Compute view bounds: zoom around content center, then apply pan
    let centerX = (contentMinX + contentMaxX) * 0.5;
    let centerY = (contentMinY + contentMaxY) * 0.5;
    let viewHalfW = contentW * 0.5 / zoom;
    let viewHalfH = contentH * 0.5 / zoom;
    let viewMinX = centerX - viewHalfW + panX;
    let viewMinY = centerY - viewHalfH + panY;
    let viewMaxX = centerX + viewHalfW + panX;
    let viewMaxY = centerY + viewHalfH + panY;
    let viewW = viewMaxX - viewMinX;
    let viewH = viewMaxY - viewMinY;

    // Compute scene-space position with UNIFORM scaling (zoom-to-fit)
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    let cardAspect = f32(widthCells) / max(f32(heightCells), 1.0);
    let viewAspect = viewW / max(viewH, 1e-6);

    var scenePos: vec2<f32>;
    if (viewAspect < cardAspect) {
        let visibleW = viewH * cardAspect;
        let offsetX = (visibleW - viewW) * 0.5;
        scenePos = vec2<f32>(
            viewMinX - offsetX + widgetUV.x * visibleW,
            viewMinY + widgetUV.y * viewH
        );
    } else {
        let visibleH = viewW / cardAspect;
        let offsetY = (visibleH - viewH) * 0.5;
        scenePos = vec2<f32>(
            viewMinX + widgetUV.x * viewW,
            viewMinY - offsetY + widgetUV.y * visibleH
        );
    }

    var resultColor = bgColor;

    // Early exit if no grid
    if (gridWidth == 0u || gridHeight == 0u) {
        return resultColor;
    }

    // Screen scale for MSDF anti-aliasing
    let pixelWidth = f32(widthCells) * grid.cellSize.x;
    let screenScale = select(1.0, pixelWidth / viewW, viewW > 0.0);

    // O(1) GRID LOOKUP
    let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSize, 0.0, f32(gridWidth - 1u)));
    let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSize, 0.0, f32(gridHeight - 1u)));
    let cellIndex = cellY * gridWidth + cellX;

    let cellStride = 1u + YPDF_MAX_ENTRIES_PER_CELL;
    let cellOffset = gridOffset + cellIndex * cellStride;
    let cellEntryCount = bitcast<u32>(cardStorage[cellOffset]);
    let loopCount = min(cellEntryCount, YPDF_MAX_ENTRIES_PER_CELL);

    let glyphSize = 8u;  // PdfGlyph = 32 bytes = 8 floats

    for (var i = 0u; i < loopCount; i++) {
        let rawIdx = bitcast<u32>(cardStorage[cellOffset + 1u + i]);
        let gi = rawIdx & YPDF_INDEX_MASK;
        let gOffset = glyphOffset + gi * glyphSize;

        let gx = cardStorage[gOffset + 0u];
        let gy = cardStorage[gOffset + 1u];
        let gw = cardStorage[gOffset + 2u];
        let gh = cardStorage[gOffset + 3u];
        let gIdx = bitcast<u32>(cardStorage[gOffset + 4u]);
        let gColorPacked = bitcast<u32>(cardStorage[gOffset + 5u]);

        if (scenePos.x >= gx && scenePos.x < gx + gw &&
            scenePos.y >= gy && scenePos.y < gy + gh) {

            let glyphUV = vec2<f32>(
                (scenePos.x - gx) / gw,
                (scenePos.y - gy) / gh
            );
            let gColor = unpackColor(gColorPacked);
            let glyphResult = renderMsdfGlyph(glyphUV, gIdx, gColor, resultColor, grid.pixelRange, screenScale);
            if (glyphResult.alpha > 0.01) {
                resultColor = glyphResult.color;
            }
        }
    }

    return resultColor;
}
