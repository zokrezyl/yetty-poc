// Pre-effect: scramble
// Index: 1
//
// Randomly swaps glyph indices to create a "glitchy text" look.
// params[0] = intensity (0.0-1.0, fraction of cells affected)
// params[1] = speed (default 1.0)

fn preEffect_scramble(
    glyphIndex: u32,
    cellCol: f32,
    cellRow: f32,
    time: f32,
    params: array<f32, 6>
) -> u32 {
    let intensity = select(0.3, params[0], params[0] > 0.0);
    let speed = select(1.0, params[1], params[1] > 0.0);

    // Hash based on cell position and time
    let t = floor(time * speed * 8.0);
    let h = fract(sin(cellCol * 127.1 + cellRow * 311.7 + t * 758.5) * 43758.5453);

    // Only affect a fraction of cells based on intensity
    if (h < intensity && glyphIndex > 32u && glyphIndex < 127u) {
        // Remap to another printable ASCII character
        let newGlyph = u32(fract(sin(h * 999.9 + t) * 43758.5453) * 94.0) + 33u;
        return newGlyph;
    }
    return glyphIndex;
}
