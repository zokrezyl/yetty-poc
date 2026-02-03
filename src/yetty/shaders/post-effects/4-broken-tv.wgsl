// Post-effect: broken TV
// Index: 4
//
// Simulates a malfunctioning CRT with rolling bands, static noise,
// and color channel glitches.
// params[0] = glitch intensity (0.0-1.0, default 0.5)
// params[1] = noise amount (0.0-1.0, default 0.3)
// params[2] = roll speed (default 1.0)

fn postEffect_brokenTv(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let glitchStr = select(0.5, params[0], params[0] > 0.0);
    let noiseStr = select(0.3, params[1], params[1] > 0.0);
    let rollSpeed = select(1.0, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    var result = color;

    // Rolling horizontal band
    let rollPos = fract(time * rollSpeed * 0.1);
    let bandDist = abs(uv.y - rollPos);
    let bandWidth = 0.08;
    if (bandDist < bandWidth) {
        let bandFade = 1.0 - bandDist / bandWidth;
        result = mix(result, vec3<f32>(1.0) - result, bandFade * glitchStr * 0.7);
    }

    // Static noise
    let noiseHash = fract(sin(dot(pixelPos + vec2<f32>(time * 100.0, 0.0),
                                   vec2<f32>(12.9898, 78.233))) * 43758.5453);
    result = mix(result, vec3<f32>(noiseHash), noiseStr * 0.15);

    // Horizontal glitch lines (random horizontal offsets at certain rows)
    let glitchLine = floor(pixelPos.y / 3.0);
    let glitchHash = fract(sin(glitchLine * 43.758 + floor(time * 15.0) * 137.5) * 43758.5453);
    if (glitchHash > (1.0 - glitchStr * 0.1)) {
        // Color channel shift on glitch lines
        let shift = (glitchHash - 0.5) * 2.0;
        result.r = mix(result.r, result.g, abs(shift));
        result.b = mix(result.b, result.r, abs(shift) * 0.5);
    }

    // Occasional full-screen color flash
    let flashHash = fract(sin(floor(time * 3.0) * 73.156) * 43758.5453);
    if (flashHash > 0.97) {
        result = mix(result, vec3<f32>(0.0, result.g * 1.5, 0.0), glitchStr * 0.3);
    }

    // Vertical hold wobble
    let wobble = sin(uv.y * 20.0 + time * 3.0) * 0.005 * glitchStr;
    let brightness = 1.0 + wobble;
    result *= brightness;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
