// Post-effect: glitch
// Index: 13
//
// Digital glitch — random horizontal slice color shifts and tears.
// params[0] = intensity (0.0-1.0, default 0.5)
// params[1] = slice count (default 20.0)
// params[2] = speed (default 5.0)

fn postEffect_glitch(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(0.5, params[0], params[0] > 0.0);
    let slices = select(20.0, params[1], params[1] > 0.0);
    let speed = select(5.0, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    var result = color;

    // Time-based trigger — glitches come in bursts
    let t = floor(time * speed);
    let burstHash = fract(sin(t * 43.758) * 43758.5453);
    let isGlitching = burstHash > (1.0 - intensity * 0.5);

    if (isGlitching) {
        // Horizontal slice determination
        let slice = floor(uv.y * slices);
        let sliceHash = fract(sin(slice * 127.1 + t * 311.7) * 43758.5453);

        if (sliceHash > 0.6) {
            // Color channel separation
            let shift = (sliceHash - 0.6) * 2.5 * intensity;
            result.r = mix(result.r, result.g, shift);
            result.b = mix(result.b, result.r, shift * 0.7);

            // Brightness glitch
            result *= 1.0 + (sliceHash - 0.8) * intensity * 3.0;
        }

        // Occasional full-line whiteout
        if (sliceHash > 0.95) {
            result = mix(result, vec3<f32>(1.0), intensity * 0.5);
        }

        // RGB split on some slices
        if (sliceHash > 0.7 && sliceHash < 0.85) {
            let splitAmt = intensity * 0.3;
            result = vec3<f32>(
                result.r * (1.0 + splitAmt),
                result.g,
                result.b * (1.0 - splitAmt)
            );
        }
    }

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
