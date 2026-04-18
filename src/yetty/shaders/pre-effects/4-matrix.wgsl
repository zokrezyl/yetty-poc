// Pre-effect: matrix rain
// Index: 4
//
// Falling green characters on empty cells, Matrix-style.
// Each column has an independent falling stream with random speed.
// Characters cycle randomly as they fall.
// params[0] = density (0.0-1.0, fraction of columns with rain, default 0.4)
// params[1] = speed (default 1.5)
// params[2] = trail length in rows (default 12.0)

fn preEffect_matrix(
    glyphIndex: u32,
    cellCol: f32,
    cellRow: f32,
    time: f32,
    params: array<f32, 6>
) -> u32 {
    let density = select(0.4, params[0], params[0] > 0.0);
    let speed = select(1.5, params[1], params[1] > 0.0);
    let trailLen = select(12.0, params[2], params[2] > 0.0);

    // Only affect empty/space cells
    if (glyphIndex != 0u && glyphIndex != 32u) {
        return glyphIndex;
    }

    // Per-column hash — determines if this column has a stream
    let colHash = fract(sin(cellCol * 127.1 + 71.7) * 43758.5453);
    if (colHash > density) {
        return glyphIndex;
    }

    // Per-column speed variation
    let colSpeed = speed * (0.5 + colHash * 1.5);

    // Head position falls down (increasing row over time)
    let headPos = fract(time * colSpeed * 0.08 + colHash * 100.0) * 80.0;

    // Distance from head (positive = above head = in trail)
    let dist = headPos - cellRow;

    // Wrap around for continuous rain
    var trailDist = dist;
    if (trailDist < -40.0) {
        trailDist += 80.0;
    }

    // Only show if in trail range (0 = head, trailLen = tail)
    if (trailDist < 0.0 || trailDist > trailLen) {
        return glyphIndex;
    }

    // Pick a character — cycle over time for animation
    let charPhase = floor(time * 8.0 + cellRow * 3.7 + cellCol * 7.3);
    let charHash = fract(sin(charPhase * 43.758 + cellCol * 127.1 + cellRow * 311.7) * 43758.5453);

    // Mix of ASCII chars that look "Matrix-like": digits, Latin, symbols
    let charRange = u32(charHash * 62.0);
    var ch: u32;
    if (charRange < 10u) {
        ch = 48u + charRange;        // 0-9
    } else if (charRange < 36u) {
        ch = 65u + charRange - 10u;  // A-Z
    } else {
        ch = 33u + charRange - 36u;  // symbols !-Z
    }

    return ch;
}
