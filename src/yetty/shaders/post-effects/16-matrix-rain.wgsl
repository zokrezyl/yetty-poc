// Post-effect: matrix rain (MSDF rendered)
// Index: 16
//
// Falling green characters rendered using the actual MSDF font atlas,
// overlaid on top of terminal content. Each column has independent
// falling streams with random speed and character cycling.
//
// params[0] = density (0.0-1.0, fraction of columns, default 0.5)
// params[1] = speed (default 1.5)
// params[2] = trail length in cells (default 15.0)
// params[3] = brightness (0.0-1.0, default 0.9)
// params[4] = bg darken (0.0-1.0, how much to darken non-rain areas, default 0.5)

fn postEffect_matrixRain(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let density = select(0.5, params[0], params[0] > 0.0);
    let speed = select(1.5, params[1], params[1] > 0.0);
    let trailLen = select(15.0, params[2], params[2] > 0.0);
    let brightness = select(0.9, params[3], params[3] > 0.0);
    let bgDarken = select(0.5, params[4], params[4] > 0.0);

    // Compute cell coordinates from pixel position
    let vpPixelPos = pixelPos - grid.viewportOrigin;
    let cellCol = floor(vpPixelPos.x / grid.cellSize.x);
    let cellRow = floor(vpPixelPos.y / grid.cellSize.y);

    // Local pixel within cell
    let localPx = vec2<f32>(
        vpPixelPos.x - cellCol * grid.cellSize.x,
        vpPixelPos.y - cellRow * grid.cellSize.y
    );

    // Outside grid — passthrough
    if (cellCol < 0.0 || cellRow < 0.0 ||
        cellCol >= grid.gridSize.x || cellRow >= grid.gridSize.y) {
        return color;
    }

    // Darken background for matrix atmosphere
    var result = color * (1.0 - bgDarken);

    // Check multiple independent streams per column for denser rain
    var totalAlpha = 0.0;
    var rainColor = vec3<f32>(0.0);

    for (var stream = 0u; stream < 3u; stream++) {
        let streamF = f32(stream);

        // Per-column, per-stream hash
        let colHash = fract(sin(cellCol * 127.1 + streamF * 543.7 + 71.7) * 43758.5453);
        let colHash2 = fract(sin(cellCol * 311.7 + streamF * 197.3 + 43.1) * 43758.5453);

        // Skip if column doesn't have rain in this stream
        if (colHash > density * (1.0 - streamF * 0.25)) {
            continue;
        }

        // Per-stream speed variation
        let streamSpeed = speed * (0.4 + colHash2 * 1.2 + streamF * 0.3);

        // Head position falls down (increasing row)
        let totalRows = grid.gridSize.y;
        let headPos = fract(time * streamSpeed * 0.06 + colHash * 50.0 + streamF * 17.3) * (totalRows + trailLen);

        // Distance from head: 0 = head (brightest), positive = trail behind
        let dist = headPos - cellRow;

        // Not in trail range
        if (dist < 0.0 || dist > trailLen) {
            continue;
        }

        // Trail fade: head is brightest, tail fades out
        let fade = 1.0 - dist / trailLen;
        let fadeCurve = fade * fade;  // Quadratic falloff for nice tail

        // Pick a random glyph — cycle characters over time at the head
        let charSpeed = select(8.0, 4.0, dist > 1.0);  // Head chars change fast, trail slower
        let charPhase = floor(time * charSpeed + cellRow * 3.7 + cellCol * 7.3 + streamF * 13.0);
        let charHash = fract(sin(charPhase * 43.758 + cellCol * 127.1 + cellRow * 311.7 + streamF * 73.1) * 43758.5453);

        // Map to printable ASCII glyph indices (33-126)
        let glyphIdx = u32(charHash * 93.0) + 33u;

        // Render the glyph using MSDF
        let green = vec3<f32>(0.1, brightness, 0.1);
        let black = vec3<f32>(0.0);
        let glyphResult = renderGlyphInCell(
            localPx,
            grid.cellSize,
            glyphIdx,
            green,
            black,
            false,
            grid.scale,
            grid.pixelRange
        );

        // Head character is white-green, trail is green
        var charColor = glyphResult.color;
        if (dist < 1.0) {
            // Head: white-ish green
            charColor = mix(charColor, vec3<f32>(0.8, 1.0, 0.8), glyphResult.alpha * 0.6);
        }

        // Accumulate with fade
        let contribution = glyphResult.alpha * fadeCurve;
        rainColor += charColor * contribution;
        totalAlpha += contribution;
    }

    // Blend rain over darkened background
    totalAlpha = clamp(totalAlpha, 0.0, 1.0);
    result = mix(result, rainColor / max(totalAlpha, 0.001), totalAlpha);

    // Subtle green tint on everything
    result = mix(result, result * vec3<f32>(0.6, 1.0, 0.6), 0.15);

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
