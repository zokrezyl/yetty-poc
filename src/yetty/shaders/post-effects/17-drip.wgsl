// Post-effect: drip
// Index: 17
//
// Text bleeds/drips downward through background pixels.
// Any pixel whose color matches its cell's bg color is "background" —
// drip source color down from text above.
//
// params[0] = speed (drip animation speed, default 0.8)
// params[1] = max_drip (max cells to drip down, default 10.0)
// params[2] = fade (how quickly drip fades, default 0.8)
// params[3] = glyph_mode (0 = color streaks, 1 = render actual glyphs)

fn postEffect_drip(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let speed = select(0.8, params[0], params[0] > 0.0);
    let maxDrip = select(10.0, params[1], params[1] > 0.0);
    let fadePower = select(0.8, params[2], params[2] > 0.0);
    let glyphMode = params[3] > 0.5;

    let vpPixelPos = pixelPos - grid.viewportOrigin;
    let cellCol = floor(vpPixelPos.x / grid.cellSize.x);
    let cellRow = floor(vpPixelPos.y / grid.cellSize.y);

    let cols = i32(grid.gridSize.x);
    let rows = i32(grid.gridSize.y);
    let col = i32(cellCol);
    let row = i32(cellRow);

    if (col < 0 || row < 0 || col >= cols || row >= rows) {
        return color;
    }

    // Get this cell's bg color
    let cellIdx = u32(row * cols + col);
    let cellBg = cellBuffer[cellIdx].bg;
    let bgColor = vec3<f32>(
        f32(cellBg & 0xFFu) / 255.0,
        f32((cellBg >> 8u) & 0xFFu) / 255.0,
        f32((cellBg >> 16u) & 0xFFu) / 255.0
    );

    // If this pixel is NOT background, leave it alone
    let diff = abs(color - bgColor);
    if (diff.r + diff.g + diff.b > 0.01) {
        return color;
    }

    // This pixel is background — scan upward to find a non-space cell
    var srcRow = -1;
    var srcGlyph = 0u;
    var srcFg = 0u;
    let maxScan = i32(maxDrip) + 5;
    var scanLimit = row - 1;
    if (scanLimit > row - maxScan) {
        scanLimit = row - maxScan;
    }
    if (scanLimit < 0) {
        scanLimit = 0;
    }

    for (var r = row - 1; r >= scanLimit; r = r - 1) {
        let idx = u32(r * cols + col);
        let g = cellBuffer[idx].glyph;
        if (g != grid.spaceGlyph && g != 0u && g != 32u && g != 0xFFFFu) {
            srcRow = r;
            srcGlyph = g;
            srcFg = cellBuffer[idx].fg;
            break;
        }
    }

    if (srcRow < 0) {
        return color;
    }

    // Distance from source text (in cells)
    let dist = f32(row - srcRow);

    // Per-column animation
    let colHash = fract(sin(f32(col) * 127.1 + 71.7) * 43758.5453);
    let colHash2 = fract(sin(f32(col) * 311.7 + 43.1) * 43758.5453);
    let phase = time * speed * (0.5 + colHash * 0.5) + colHash2 * 6.28;
    let dripLen = maxDrip * (0.5 + 0.5 * sin(phase));

    if (dist > dripLen) {
        return color;
    }

    let fade = pow(1.0 - dist / dripLen, fadePower);

    // Extract source foreground color
    let srcColor = vec3<f32>(
        f32(srcFg & 0xFFu) / 255.0,
        f32((srcFg >> 8u) & 0xFFu) / 255.0,
        f32((srcFg >> 16u) & 0xFFu) / 255.0
    );

    if (glyphMode) {
        let localPx = vec2<f32>(
            vpPixelPos.x - cellCol * grid.cellSize.x,
            vpPixelPos.y - cellRow * grid.cellSize.y
        );

        let glyphResult = renderGlyphInCell(
            localPx,
            grid.cellSize,
            srcGlyph,
            srcColor * fade,
            vec3<f32>(0.0),
            false,
            grid.scale,
            grid.pixelRange
        );

        return color + glyphResult.color * glyphResult.alpha * fade;
    } else {
        let localY = (vpPixelPos.y - cellRow * grid.cellSize.y) / grid.cellSize.y;
        let vertFade = mix(1.0, 0.3, localY);
        return color + srcColor * fade * vertFade * 0.7;
    }
}
