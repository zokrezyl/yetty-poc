// Shader glyph: KDraw widget (codepoint 1048580 / U+100004)
// GPU-accelerated SDF primitives with SCREEN-SPACE TILE CULLING
// + MSDF text rendering
//
// Uses shared SDF functions from lib/distfunctions.wgsl
// Uses text rendering from lib/text.wgsl
//
// =============================================================================
// Architecture:
// =============================================================================
// Unlike HDraw (scene-space grid), KDraw uses SCREEN-SPACE tiles:
// - Tiles are always 8x8 pixels regardless of zoom level
// - Each tile has a compact list of primitives that overlap it
// - All 64 pixels in a tile share the same primitive list
// - Better cache coherency and reduced branch divergence
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  primitiveOffset (u32)
//   offset 1:  primitiveCount (u32)
//   offset 2:  tileListOffset (u32)
//   offset 3:  tileCountX (u32)
//   offset 4:  tileCountY (u32)
//   offset 5:  tileSize (u32)       - 8 pixels
//   offset 6:  glyphOffset (u32)
//   offset 7:  glyphCount (u32)
//   offset 8:  sceneMinX (f32)
//   offset 9:  sceneMinY (f32)
//   offset 10: sceneMaxX (f32)
//   offset 11: sceneMaxY (f32)
//   offset 12: widthCells (u32)
//   offset 13: heightCells (u32)
//   offset 14: flags (u32)
//   offset 15: bgColor (u32)
// =============================================================================
// KDrawGlyph layout (32 bytes = 8 floats):
//   offset 0: x (f32)        - position in scene coords
//   offset 1: y (f32)
//   offset 2: width (f32)    - size in scene coords
//   offset 3: height (f32)
//   offset 4: glyphIndex (u32) - index in glyphMetadata
//   offset 5: color (u32)    - packed RGBA
//   offset 6: layer (u32)
//   offset 7: _pad (u32)
// =============================================================================

const KDRAW_FLAG_SHOW_BOUNDS: u32 = 1u;
const KDRAW_FLAG_SHOW_TILES: u32 = 2u;
const KDRAW_FLAG_SHOW_EVAL_COUNT: u32 = 4u;

// Hardcoded max primitives per tile (matches C++ MAX_PRIMS_PER_TILE)
const KDRAW_MAX_PRIMS_PER_TILE: u32 = 16u;
const KDRAW_TILE_SIZE: u32 = 8u;

// =============================================================================
// Main shader function - TILE-BASED RENDERING
// =============================================================================

fn shaderGlyph_1048580(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode ANSI-compatible cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata
    let primitiveOffset = cardMetadata[metaOffset + 0u];
    let primitiveCount = cardMetadata[metaOffset + 1u];
    let tileListOffset = cardMetadata[metaOffset + 2u];
    let tileCountX = cardMetadata[metaOffset + 3u];
    let tileCountY = cardMetadata[metaOffset + 4u];
    let tileSize = cardMetadata[metaOffset + 5u];
    let glyphOffset = cardMetadata[metaOffset + 6u];
    let glyphCount = cardMetadata[metaOffset + 7u];
    let sceneMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let sceneMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let sceneMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let sceneMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let widthCells = cardMetadata[metaOffset + 12u];
    let heightCells = cardMetadata[metaOffset + 13u];
    let flags = cardMetadata[metaOffset + 14u];
    let bgColorPacked = cardMetadata[metaOffset + 15u];

    let bgColor = unpackColor(bgColorPacked);

    // Compute widget UV (position within entire card widget)
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // Compute scene-space position
    let scenePos = vec2<f32>(
        sceneMinX + widgetUV.x * (sceneMaxX - sceneMinX),
        sceneMinY + widgetUV.y * (sceneMaxY - sceneMinY)
    );

    // Early exit if no data
    if (primitiveCount == 0u && glyphCount == 0u) {
        return bgColor;
    }

    var resultColor = bgColor;
    var evalCount = 0u;

    // =========================================================================
    // SDF PRIMITIVE RENDERING - TILE-BASED CULLING
    // =========================================================================
    let primSize = 24u;

    // Use tile-based lookup for efficient primitive culling
    if (primitiveCount > 0u && tileCountX > 0u && tileCountY > 0u) {
        // Compute which tile this pixel belongs to
        let tileX = u32(clamp(widgetUV.x * f32(tileCountX), 0.0, f32(tileCountX - 1u)));
        let tileY = u32(clamp(widgetUV.y * f32(tileCountY), 0.0, f32(tileCountY - 1u)));
        let tileIndex = tileY * tileCountX + tileX;

        // Read tile's primitive list
        let tileStride = 1u + KDRAW_MAX_PRIMS_PER_TILE;
        let tileOffset = tileListOffset + tileIndex * tileStride;
        let tilePrimCount = bitcast<u32>(cardStorage[tileOffset]);

        // Only iterate primitives that overlap this tile
        let maxPrimsToCheck = min(tilePrimCount, KDRAW_MAX_PRIMS_PER_TILE);
        for (var i = 0u; i < maxPrimsToCheck; i++) {
            let primIdx = bitcast<u32>(cardStorage[tileOffset + 1u + i]);
            let primOffset = primitiveOffset + primIdx * primSize;

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

        // Check if pixel is within glyph bounds
        if (scenePos.x >= gx && scenePos.x < gx + gw &&
            scenePos.y >= gy && scenePos.y < gy + gh) {

            // Compute local UV within glyph (0-1)
            let glyphLocalUV = vec2<f32>(
                (scenePos.x - gx) / gw,
                (scenePos.y - gy) / gh
            );

            let gColor = unpackColor(gColorPacked);

            // Render using MSDF
            let glyphResult = renderMsdfGlyph(glyphLocalUV, gIdx, gColor, resultColor, grid.pixelRange, screenScale);

            if (glyphResult.alpha > 0.01) {
                resultColor = glyphResult.color;
            }
        }
    }

    // =========================================================================
    // DEBUG VISUALIZATIONS
    // =========================================================================

    // Debug: show tile boundaries
    if ((flags & KDRAW_FLAG_SHOW_TILES) != 0u) {
        let tileFracX = fract(widgetUV.x * f32(tileCountX));
        let tileFracY = fract(widgetUV.y * f32(tileCountY));
        let tileLineWidth = 0.05;
        if (tileFracX < tileLineWidth || tileFracY < tileLineWidth) {
            resultColor = mix(resultColor, vec3<f32>(0.0, 1.0, 1.0), 0.5);
        }
    }

    // Debug: show scene bounds
    if ((flags & KDRAW_FLAG_SHOW_BOUNDS) != 0u) {
        let edgeDist = min(
            min(scenePos.x - sceneMinX, sceneMaxX - scenePos.x),
            min(scenePos.y - sceneMinY, sceneMaxY - scenePos.y)
        );
        let edgeWidth = (sceneMaxX - sceneMinX) * 0.01;
        if (edgeDist < edgeWidth) {
            resultColor = mix(resultColor, vec3<f32>(1.0, 0.0, 0.0), 0.5);
        }
    }

    // Debug: show evaluation count heatmap
    if ((flags & KDRAW_FLAG_SHOW_EVAL_COUNT) != 0u) {
        // Also show tile primitive count in this mode for debugging
        if (tileCountX > 0u && tileCountY > 0u) {
            let tileX = u32(clamp(widgetUV.x * f32(tileCountX), 0.0, f32(tileCountX - 1u)));
            let tileY = u32(clamp(widgetUV.y * f32(tileCountY), 0.0, f32(tileCountY - 1u)));
            let tileIndex = tileY * tileCountX + tileX;
            let tileStride = 1u + KDRAW_MAX_PRIMS_PER_TILE;
            let tileOffset = tileListOffset + tileIndex * tileStride;
            let tilePrimCount = bitcast<u32>(cardStorage[tileOffset]);

            // Show tile prim count as blue channel (brighter = more prims)
            let primT = clamp(f32(tilePrimCount) / 8.0, 0.0, 1.0);
            return vec3<f32>(f32(evalCount) / 8.0, primT, primT * 0.5);
        }
        let t = clamp(f32(evalCount) / 8.0, 0.0, 1.0);
        return vec3<f32>(t, 1.0 - t * 0.5, 0.0);
    }

    return resultColor;
}
