// =============================================================================
// YPaint Overlay Rendering (full-screen painters)
// Renders ypaint content from a metadata slot as a full-screen overlay.
// Returns vec4: rgb = color, a = coverage (0 = transparent, 1 = opaque)
//
// Dependencies: ypaint-sdf.gen.wgsl (evaluateYpaintSDF, ypaintPrimColors, ypaintPrimStrokeWidth)
// =============================================================================

fn renderYpaintOverlay(slotIndex: u32, pixelPos: vec2<f32>) -> vec4<f32> {
    if (slotIndex == 0u) {
        return vec4<f32>(0.0);  // Disabled
    }

    let metaOffset = slotIndex * 16u;

    // Read metadata
    let primitiveOffset = cardMetadata[metaOffset + 0u];
    let primitiveCount = cardMetadata[metaOffset + 1u];
    let gridOffset = cardMetadata[metaOffset + 2u];
    let gridWidth = cardMetadata[metaOffset + 3u];
    let gridHeight = cardMetadata[metaOffset + 4u];
    let cellSizeXY = unpack2x16float(cardMetadata[metaOffset + 5u]);
    let cellSizeX = cellSizeXY.x;
    let cellSizeY = cellSizeXY.y;
    let glyphOffset = cardMetadata[metaOffset + 6u];
    let glyphCount = cardMetadata[metaOffset + 7u];

    // Early exit if no content (no SDF primitives AND no glyphs)
    if (primitiveCount == 0u && glyphCount == 0u) {
        return vec4<f32>(0.0);
    }

    // Content bounds
    let contentMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let contentMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let contentMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let contentMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);

    // Early exit if no grid
    if (gridWidth == 0u || gridHeight == 0u || cellSizeX <= 0.0 || cellSizeY <= 0.0) {
        return vec4<f32>(0.0);
    }

    // For full-screen overlay, map pixel position directly to scene coordinates
    // The scene bounds define where content is placed
    let contentW = contentMaxX - contentMinX;
    let contentH = contentMaxY - contentMinY;

    // Scale pixel position to scene - overlay covers full grid area
    let gridPixelW = grid.gridSize.x * grid.cellSize.x;
    let gridPixelH = grid.gridSize.y * grid.cellSize.y;
    let scenePos = vec2<f32>(
        contentMinX + (pixelPos.x / gridPixelW) * contentW,
        contentMinY + (pixelPos.y / gridPixelH) * contentH
    );

    // Grid lookup - compute cell from scene position
    // Canvas stores primitives at cursorRow + localRow. Query position maps
    // directly to storage row since scenePos = visualPixelPos when bounds match.
    let invCellSizeX = 1.0 / cellSizeX;
    let invCellSizeY = 1.0 / cellSizeY;
    let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSizeX, 0.0, f32(gridWidth - 1u)));
    let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSizeY, 0.0, f32(gridHeight - 1u)));
    let cellIndex = cellY * gridWidth + cellX;

    // Variable-length grid lookup
    let packedStart = bitcast<u32>(cardStorage[gridOffset + cellIndex]);
    let cellEntryCount = bitcast<u32>(cardStorage[gridOffset + packedStart]);
    let loopCount = min(cellEntryCount, 64u);  // Safety cap

    let primDataBase = primitiveOffset + primitiveCount;
    var resultColor = vec3<f32>(0.0);
    var resultAlpha = 0.0;

    let glyphSize = 5u;  // YPaintGlyph: x, y, wh_packed, glf_packed, color
    let screenScale = 1.0;

    for (var i = 0u; i < loopCount; i++) {
        let rawIdx = bitcast<u32>(cardStorage[gridOffset + packedStart + 1u + i]);

        if ((rawIdx & 0x80000000u) != 0u) {
            // ---- TEXT GLYPH (MSDF) ----
            let gi = rawIdx & 0x7FFFFFFFu;
            let gOffset = glyphOffset + gi * glyphSize;

            let gx = cardStorage[gOffset + 0u];
            let gy = cardStorage[gOffset + 1u];
            let whPacked = bitcast<u32>(cardStorage[gOffset + 2u]);
            let gw = unpack2x16float(whPacked).x;
            let gh = unpack2x16float(whPacked).y;
            let glfPacked = bitcast<u32>(cardStorage[gOffset + 3u]);
            let gIdx = glfPacked & 0xFFFFu;
            let gColorPacked = bitcast<u32>(cardStorage[gOffset + 4u]);

            if (scenePos.x >= gx && scenePos.x < gx + gw &&
                scenePos.y >= gy && scenePos.y < gy + gh) {
                let glyphUV = vec2<f32>(
                    (scenePos.x - gx) / gw,
                    (scenePos.y - gy) / gh
                );
                let gColor = vec3<f32>(
                    f32(gColorPacked & 0xFFu) / 255.0,
                    f32((gColorPacked >> 8u) & 0xFFu) / 255.0,
                    f32((gColorPacked >> 16u) & 0xFFu) / 255.0
                );
                let gAlpha = f32((gColorPacked >> 24u) & 0xFFu) / 255.0;

                let glyphResult = renderMsdfGlyph(glyphUV, gIdx, gColor, resultColor, grid.pixelRange, screenScale);
                if (glyphResult.alpha > 0.01) {
                    resultColor = glyphResult.color;
                    resultAlpha = max(resultAlpha, glyphResult.alpha * gAlpha);
                }
            }
            continue;
        }

        // SDF primitive - rawIdx is word offset (translated by C++ in writeDerived)
        let primOff = primDataBase + rawIdx;
        let d = evaluateYpaintSDF(primOff, scenePos);

        let colors = ypaintPrimColors(primOff);
        let fillColorPacked = colors.x;
        if (d < 0.0 && fillColorPacked != 0u) {
            let fillColorAlpha = ypaintUnpackColorAlpha(fillColorPacked);
            let edgeAlpha = clamp(-d * 2.0, 0.0, 1.0);
            let alpha = edgeAlpha * fillColorAlpha.a;
            resultColor = mix(resultColor, fillColorAlpha.rgb, alpha);
            resultAlpha = max(resultAlpha, alpha);
        }

        let strokeColorPacked = colors.y;
        let strokeWidth = ypaintPrimStrokeWidth(primOff);
        if (strokeWidth > 0.0 && strokeColorPacked != 0u) {
            let strokeDist = abs(d) - strokeWidth * 0.5;
            if (strokeDist < 0.0) {
                let strokeColorAlpha = ypaintUnpackColorAlpha(strokeColorPacked);
                let edgeAlpha = clamp(-strokeDist * 2.0, 0.0, 1.0);
                let alpha = edgeAlpha * strokeColorAlpha.a;
                resultColor = mix(resultColor, strokeColorAlpha.rgb, alpha);
                resultAlpha = max(resultAlpha, alpha);
            }
        }
    }

    return vec4<f32>(resultColor, resultAlpha);
}

// Helper to unpack u32 color to vec4<f32> (with alpha)
fn ypaintUnpackColorAlpha(packed: u32) -> vec4<f32> {
    return vec4<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0,
        f32((packed >> 24u) & 0xFFu) / 255.0
    );
}
