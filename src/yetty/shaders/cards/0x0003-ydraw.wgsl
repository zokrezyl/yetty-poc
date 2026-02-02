// Shader glyph: YDraw widget (codepoint 1048579 / U+100003)
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
// YDrawGlyph layout (32 bytes = 8 floats):
//   offset 0: x (f32)        - position in scene coords
//   offset 1: y (f32)
//   offset 2: width (f32)    - size in scene coords
//   offset 3: height (f32)
//   offset 4: glyphIndex (u32) - index in glyphMetadata
//   offset 5: color (u32)    - packed RGBA
//   offset 6: layer (u32)
//   offset 7: _pad (u32)
// =============================================================================

const YDRAW_FLAG_SHOW_BOUNDS: u32 = 1u;
const YDRAW_FLAG_SHOW_GRID: u32 = 2u;
const YDRAW_FLAG_SHOW_EVAL_COUNT: u32 = 4u;

// Hardcoded max entries per cell (matches C++ DEFAULT_MAX_PRIMS_PER_CELL)
const YDRAW_MAX_PRIMS_PER_CELL: u32 = 16u;

// Bit 31 set = glyph index, clear = primitive index
const GLYPH_BIT: u32 = 0x80000000u;
const INDEX_MASK: u32 = 0x7FFFFFFFu;

// =============================================================================
// Main shader function - O(1) GRID LOOKUP
// =============================================================================

fn shaderGlyph_1048579(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
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

    // Content bounds (= grid origin, never changes with zoom/pan)
    let contentMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let contentMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let contentMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let contentMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);

    // Unpack widthCells/heightCells (lower 16 bits) and pan (upper 16 bits)
    let packedWC = cardMetadata[metaOffset + 12u];
    let packedHC = cardMetadata[metaOffset + 13u];
    let widthCells = packedWC & 0xFFFFu;
    let heightCells = packedHC & 0xFFFFu;
    let panXi16 = i32(packedWC >> 16u) - select(0, 65536, (packedWC >> 16u) >= 32768u);
    let panYi16 = i32(packedHC >> 16u) - select(0, 65536, (packedHC >> 16u) >= 32768u);

    // Unpack flags (lower 16 bits) and zoom f16 (upper 16 bits)
    let packedFlags = cardMetadata[metaOffset + 14u];
    let flags = packedFlags & 0xFFFFu;
    let zoomF16 = packedFlags >> 16u;

    // f16 → f32 conversion
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

    // Reconstruct pan in scene units: panScene = i16value / 16384.0 * extent
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

    // Compute scene-space position with UNIFORM scaling (zoom-to-fit).
    // This ensures 1 scene unit is the same physical size in both axes.
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    let cardAspect = f32(widthCells) / max(f32(heightCells), 1.0);
    let viewAspect = viewW / max(viewH, 1e-6);

    // Use uniform scale: pick the axis that needs more zoom-out
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

    // Early exit if no grid
    if (gridWidth == 0u || gridHeight == 0u) {
        return bgColor;
    }

    var resultColor = bgColor;
    var evalCount = 0u;

    // Compute screen scale for MSDF anti-aliasing (use view width, not content width)
    let pixelWidth = f32(widthCells) * grid.cellSize.x;
    let screenScale = select(1.0, pixelWidth / viewW, viewW > 0.0);

    // O(1) GRID LOOKUP - use CONTENT bounds (grid origin), not view bounds
    let cellX = u32(clamp((scenePos.x - contentMinX) * invCellSize, 0.0, f32(gridWidth - 1u)));
    let cellY = u32(clamp((scenePos.y - contentMinY) * invCellSize, 0.0, f32(gridHeight - 1u)));
    let cellIndex = cellY * gridWidth + cellX;

    // Each cell stores: [entryCount][idx0][idx1]...[idxN]
    // Entries with bit 31 set are glyph indices, otherwise primitive indices
    let cellStride = 1u + YDRAW_MAX_PRIMS_PER_CELL;
    let cellOffset = gridOffset + cellIndex * cellStride;
    let cellEntryCount = bitcast<u32>(cardStorage[cellOffset]);
    let loopCount = min(cellEntryCount, YDRAW_MAX_PRIMS_PER_CELL);

    let primSize = 24u;
    let glyphSize = 8u;

    for (var i = 0u; i < loopCount; i++) {
        let rawIdx = bitcast<u32>(cardStorage[cellOffset + 1u + i]);

        if ((rawIdx & GLYPH_BIT) != 0u) {
            // ---- TEXT GLYPH (MSDF) ----
            let gi = rawIdx & INDEX_MASK;
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
        } else {
            // ---- SDF PRIMITIVE ----
            let primIdx = rawIdx;
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

            let d = evaluateSDF(primOffset, scenePos);

            let fillColorPacked = bitcast<u32>(cardStorage[primOffset + 14u]);
            if (d < 0.0 && fillColorPacked != 0u) {
                let fillColor = unpackColor(fillColorPacked);
                let alpha = clamp(-d * 2.0, 0.0, 1.0);
                resultColor = mix(resultColor, fillColor, alpha);
            }

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

    // Debug: show grid lines (use floor instead of expensive modulo)
    if ((flags & YDRAW_FLAG_SHOW_GRID) != 0u) {
        let cellSize = 1.0 / invCellSize;
        let cellPosX = (scenePos.x - contentMinX) * invCellSize;
        let cellPosY = (scenePos.y - contentMinY) * invCellSize;
        let fracX = fract(cellPosX);
        let fracY = fract(cellPosY);
        let gridLineWidth = 0.02;
        if (fracX < gridLineWidth || fracY < gridLineWidth) {
            resultColor = mix(resultColor, vec3<f32>(0.5, 0.5, 0.5), 0.5);
        }
    }

    // Debug: show evaluation count heatmap
    if ((flags & YDRAW_FLAG_SHOW_EVAL_COUNT) != 0u) {
        let t = clamp(f32(evalCount) / 4.0, 0.0, 1.0);
        return vec3<f32>(t, 1.0 - t * 0.5, 0.0);
    }

    return resultColor;
}
