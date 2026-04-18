// Pre-effect: wave text
// Index: 3
//
// Shifts glyph indices in a wave pattern — characters seem to "ripple".
// params[0] = intensity (0.0-1.0, default 0.3)
// params[1] = frequency (default 0.3)
// params[2] = speed (default 2.0)

fn preEffect_wave(
    glyphIndex: u32,
    cellCol: f32,
    cellRow: f32,
    time: f32,
    params: array<f32, 6>
) -> u32 {
    let intensity = select(0.3, params[0], params[0] > 0.0);
    let freq = select(0.3, params[1], params[1] > 0.0);
    let speed = select(2.0, params[2], params[2] > 0.0);

    // Only affect printable ASCII
    if (glyphIndex < 33u || glyphIndex > 126u) {
        return glyphIndex;
    }

    // Wave-based offset to glyph index
    let wave = sin(cellCol * freq + cellRow * freq * 0.7 + time * speed);
    let offset = i32(wave * intensity * 10.0);

    // Wrap within printable ASCII range
    var newGlyph = i32(glyphIndex) + offset;
    newGlyph = ((newGlyph - 33) % 94 + 94) % 94 + 33;

    return u32(newGlyph);
}
