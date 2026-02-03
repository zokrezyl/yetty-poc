// Pre-effect: rain characters
// Index: 2
//
// Replaces some glyphs with falling rain-like characters (|, /, \, :).
// params[0] = density (0.0-1.0, default 0.15)
// params[1] = speed (default 2.0)

fn preEffect_rain(
    glyphIndex: u32,
    cellCol: f32,
    cellRow: f32,
    time: f32,
    params: array<f32, 6>
) -> u32 {
    let density = select(0.15, params[0], params[0] > 0.0);
    let speed = select(2.0, params[1], params[1] > 0.0);

    // Only affect empty/space cells
    if (glyphIndex != 0u && glyphIndex != 32u) {
        return glyphIndex;
    }

    // Column hash — only some columns have rain
    let colHash = fract(sin(cellCol * 127.1) * 43758.5453);
    if (colHash > density) {
        return glyphIndex;
    }

    // Raindrop position (animated row)
    let dropPos = fract(cellRow / 40.0 - time * speed * 0.05 - colHash * 5.0) * 40.0;
    if (dropPos < 3.0) {
        // Rain characters: | : . '
        let charSelect = u32(fract(colHash * 999.9) * 4.0);
        let chars = array<u32, 4>(124u, 58u, 46u, 39u);  // | : . '
        return chars[charSelect];
    }

    return glyphIndex;
}
