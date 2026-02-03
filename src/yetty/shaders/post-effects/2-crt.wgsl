// Post-effect: CRT monitor
// Index: 2
//
// Simulates an old CRT monitor with vignette, scanlines, and color bleed.
// params[0] = vignette intensity (0.0-1.0, default 0.4)
// params[1] = scanline intensity (0.0-1.0, default 0.15)
// params[2] = color bleed amount (0.0-1.0, default 0.3)

fn postEffect_crt(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let vignetteStr = select(0.4, params[0], params[0] > 0.0);
    let scanlineStr = select(0.15, params[1], params[1] > 0.0);
    let colorBleed = select(0.3, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    var result = color;

    // Vignette - darken edges
    let center = vec2<f32>(0.5, 0.5);
    let dist = length(uv - center) * 1.4;  // 1.4 normalizes corner distance
    let vignette = 1.0 - vignetteStr * dist * dist;
    result *= vignette;

    // Scanlines
    let scanline = sin(pixelPos.y * 3.14159) * 0.5 + 0.5;
    result *= 1.0 - scanlineStr * (1.0 - scanline);

    // Subtle RGB sub-pixel simulation
    let subpixel = fract(pixelPos.x / 3.0);
    if (subpixel < 0.333) {
        result.r *= 1.0 + colorBleed * 0.2;
        result.b *= 1.0 - colorBleed * 0.1;
    } else if (subpixel < 0.666) {
        result.g *= 1.0 + colorBleed * 0.2;
    } else {
        result.b *= 1.0 + colorBleed * 0.2;
        result.r *= 1.0 - colorBleed * 0.1;
    }

    // Subtle brightness flicker
    let flicker = 1.0 - 0.01 * sin(time * 8.0) - 0.005 * sin(time * 17.3);
    result *= flicker;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
