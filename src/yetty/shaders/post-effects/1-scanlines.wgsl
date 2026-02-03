// Post-effect: scanlines
// Index: 1
//
// Classic CRT scanline effect - darkens alternating rows.
// params[0] = intensity (0.0-1.0, default 0.3)
// params[1] = line width in pixels (default 2.0)

fn postEffect_scanlines(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(0.3, params[0], params[0] > 0.0);
    let lineWidth = select(2.0, params[1], params[1] > 0.0);

    // Scanline pattern - darken every other line group
    let line = floor(pixelPos.y / lineWidth);
    let scanline = fract(line * 0.5) * 2.0;  // 0 or 1

    // Subtle flicker over time
    let flicker = 1.0 - 0.02 * sin(time * 5.0);

    let darken = 1.0 - intensity * scanline;
    return color * darken * flicker;
}
