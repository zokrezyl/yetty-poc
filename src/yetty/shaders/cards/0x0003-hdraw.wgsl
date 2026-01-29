// Shader glyph: HDraw widget (codepoint 1048579 / U+100003)
// GPU-accelerated SDF primitives with O(1) UNIFORM GRID SPATIAL HASHING
// + MSDF text rendering
//
// Uses shared SDF functions from lib/distfunctions.wgsl
// Uses text rendering from lib/text.wgsl
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  primitiveOffset (u32)
//   offset 1:  primitiveCount (u32)
//   offset 2:  gridOffset (u32)
//   offset 3:  gridWidth (u32)
//   offset 4:  gridHeight (u32)
//   offset 5:  cellSize (f32)
//   offset 6:  glyphOffset (u32)   - offset of glyph data in cardStorage
//   offset 7:  glyphCount (u32)    - number of text glyphs
//   offset 8:  sceneMinX (f32)
//   offset 9:  sceneMinY (f32)
//   offset 10: sceneMaxX (f32)
//   offset 11: sceneMaxY (f32)
//   offset 12: widthCells (u32)
//   offset 13: heightCells (u32)
//   offset 14: flags (u32)
//   offset 15: bgColor (u32)
// =============================================================================
// HDrawGlyph layout (32 bytes = 8 floats):
//   offset 0: x (f32)        - position in scene coords
//   offset 1: y (f32)
//   offset 2: width (f32)    - size in scene coords
//   offset 3: height (f32)
//   offset 4: glyphIndex (u32) - index in glyphMetadata
//   offset 5: color (u32)    - packed RGBA
//   offset 6: layer (u32)
//   offset 7: _pad (u32)
// =============================================================================

const HDRAW_FLAG_SHOW_BOUNDS: u32 = 1u;
const HDRAW_FLAG_SHOW_GRID: u32 = 2u;
const HDRAW_FLAG_SHOW_EVAL_COUNT: u32 = 4u;

// Hardcoded max primitives per cell (matches C++ DEFAULT_MAX_PRIMS_PER_CELL)
const HDRAW_MAX_PRIMS_PER_CELL: u32 = 16u;

// =============================================================================
// Main shader function - O(1) GRID LOOKUP
// =============================================================================

fn shaderGlyph_1048579(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // Decode ANSI-compatible cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata (minimize reads - only what we need)
    let primitiveOffset = cardMetadata[metaOffset + 0u];
    let gridOffset = cardMetadata[metaOffset + 2u];
    let gridWidth = cardMetadata[metaOffset + 3u];
    let gridHeight = cardMetadata[metaOffset + 4u];
    let cellSize = bitcast<f32>(cardMetadata[metaOffset + 5u]);
    let glyphOffset = cardMetadata[metaOffset + 6u];
    let glyphCount = cardMetadata[metaOffset + 7u];
    let invCellSize = select(0.0, 1.0 / cellSize, cellSize > 0.0);
    let sceneMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let sceneMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let sceneMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let sceneMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let widthCells = cardMetadata[metaOffset + 12u];
    let heightCells = cardMetadata[metaOffset + 13u];
    let flags = cardMetadata[metaOffset + 14u];
    let bgColorPacked = cardMetadata[metaOffset + 15u];

    let bgColor = unpackColor(bgColorPacked);

    // Compute scene-space position
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    let scenePos = vec2<f32>(
        sceneMinX + widgetUV.x * (sceneMaxX - sceneMinX),
        sceneMinY + widgetUV.y * (sceneMaxY - sceneMinY)
    );

    // Early exit only if no primitives AND no glyphs
    if ((gridWidth == 0u || gridHeight == 0u) && glyphCount == 0u) {
        return bgColor;
    }

    var resultColor = bgColor;
    var evalCount = 0u;

    // SDF primitive rendering (only if grid exists)
    if (gridWidth > 0u && gridHeight > 0u) {
        // O(1) GRID LOOKUP - find which cell this pixel is in
        // Use multiplication by invCellSize instead of division (faster!)
        let cellX = u32(clamp((scenePos.x - sceneMinX) * invCellSize, 0.0, f32(gridWidth - 1u)));
        let cellY = u32(clamp((scenePos.y - sceneMinY) * invCellSize, 0.0, f32(gridHeight - 1u)));
        let cellIndex = cellY * gridWidth + cellX;

        // Each cell stores: [primCount][primIdx0][primIdx1]...[primIdxN]
        let cellStride = 1u + HDRAW_MAX_PRIMS_PER_CELL;
        let cellOffset = gridOffset + cellIndex * cellStride;

        // Read number of primitives in this cell
        let cellPrimCount = bitcast<u32>(cardStorage[cellOffset]);

        // Render primitives in this cell only
        let primSize = 24u;
        let loopCount = min(cellPrimCount, HDRAW_MAX_PRIMS_PER_CELL);

        for (var i = 0u; i < loopCount; i++) {
            let primIdx = bitcast<u32>(cardStorage[cellOffset + 1u + i]);
            let primOffset = primitiveOffset + primIdx * primSize;

            // Per-primitive AABB check (fast rejection)
            let primMinX = cardStorage[primOffset + 18u];
            let primMaxX = cardStorage[primOffset + 20u];
            if (scenePos.x < primMinX || scenePos.x > primMaxX) {
                continue;
            }
            let primMinY = cardStorage[primOffset + 19u];
            let primMaxY = cardStorage[primOffset + 21u];
            if (scenePos.y < primMinY || scenePos.y > primMaxY) {
                continue;
            }

            evalCount++;

            // Evaluate SDF
            let d = evaluateSDF(primOffset, scenePos);

            // Fill (only if inside and has fill color)
            let fillColorPacked = bitcast<u32>(cardStorage[primOffset + 14u]);
            if (d < 0.0 && fillColorPacked != 0u) {
                let fillColor = unpackColor(fillColorPacked);
                let alpha = clamp(-d * 2.0, 0.0, 1.0);
                resultColor = mix(resultColor, fillColor, alpha);
            }

            // Stroke (only if has stroke)
            let strokeColorPacked = bitcast<u32>(cardStorage[primOffset + 15u]);
            let strokeWidth = cardStorage[primOffset + 16u];
            if (strokeWidth > 0.0 && strokeColorPacked != 0u) {
                let strokeDist = abs(d) - strokeWidth * 0.5;
                if (strokeDist < 0.0) {
                    let strokeColor = unpackColor(strokeColorPacked);
                    let alpha = clamp(-strokeDist * 2.0, 0.0, 1.0);
                    resultColor = mix(resultColor, strokeColor, alpha);
                }
            }
        }
    }

    // =========================================================================
    // TEXT GLYPH RENDERING (MSDF)
    // =========================================================================
    // HDrawGlyph layout: x, y, width, height, glyphIndex, color, layer, _pad (8 floats = 32 bytes)
    let glyphSize = 8u;

    // Compute screen scale for proper MSDF anti-aliasing
    let sceneWidth = sceneMaxX - sceneMinX;
    let pixelWidth = f32(widthCells) * grid.cellSize.x;
    let screenScale = select(1.0, pixelWidth / sceneWidth, sceneWidth > 0.0);

    for (var gi = 0u; gi < glyphCount; gi++) {
        let gOffset = glyphOffset + gi * glyphSize;

        // Read glyph data
        let gx = cardStorage[gOffset + 0u];
        let gy = cardStorage[gOffset + 1u];
        let gw = cardStorage[gOffset + 2u];
        let gh = cardStorage[gOffset + 3u];
        let gIdx = bitcast<u32>(cardStorage[gOffset + 4u]);
        let gColorPacked = bitcast<u32>(cardStorage[gOffset + 5u]);
        // layer at offset 6, _pad at offset 7 - not used yet

        // Check if pixel is within glyph bounds
        if (scenePos.x >= gx && scenePos.x < gx + gw &&
            scenePos.y >= gy && scenePos.y < gy + gh) {

            // Compute local UV within glyph (0-1)
            let localUV = vec2<f32>(
                (scenePos.x - gx) / gw,
                (scenePos.y - gy) / gh
            );

            let gColor = unpackColor(gColorPacked);

            // Render using MSDF
            let glyphResult = renderMsdfGlyph(localUV, gIdx, gColor, resultColor, grid.pixelRange, screenScale);

            if (glyphResult.alpha > 0.01) {
                resultColor = glyphResult.color;
            }
        }
    }

    // Debug: show grid lines (use floor instead of expensive modulo)
    if ((flags & HDRAW_FLAG_SHOW_GRID) != 0u) {
        let cellSize = 1.0 / invCellSize;
        let cellPosX = (scenePos.x - sceneMinX) * invCellSize;
        let cellPosY = (scenePos.y - sceneMinY) * invCellSize;
        let fracX = fract(cellPosX);
        let fracY = fract(cellPosY);
        let gridLineWidth = 0.02;
        if (fracX < gridLineWidth || fracY < gridLineWidth) {
            resultColor = mix(resultColor, vec3<f32>(0.5, 0.5, 0.5), 0.5);
        }
    }

    // Debug: show evaluation count heatmap
    if ((flags & HDRAW_FLAG_SHOW_EVAL_COUNT) != 0u) {
        let t = clamp(f32(evalCount) / 4.0, 0.0, 1.0);
        return vec3<f32>(t, 1.0 - t * 0.5, 0.0);
    }

    return resultColor;
}
