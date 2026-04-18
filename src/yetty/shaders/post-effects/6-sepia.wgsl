// Post-effect: sepia
// Index: 6
//
// Vintage sepia tone with optional film grain.
// params[0] = sepia intensity (0.0-1.0, default 0.8)
// params[1] = grain amount (0.0-1.0, default 0.1)

fn postEffect_sepia(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(0.8, params[0], params[0] > 0.0);
    let grain = select(0.1, params[1], params[1] > 0.0);

    // Sepia tone matrix
    let sepia = vec3<f32>(
        dot(color, vec3<f32>(0.393, 0.769, 0.189)),
        dot(color, vec3<f32>(0.349, 0.686, 0.168)),
        dot(color, vec3<f32>(0.272, 0.534, 0.131))
    );

    var result = mix(color, sepia, intensity);

    // Film grain
    let noise = fract(sin(dot(pixelPos + vec2<f32>(time * 37.0, time * 53.0),
                               vec2<f32>(12.9898, 78.233))) * 43758.5453);
    result += (noise - 0.5) * grain;

    // Slight vignette for vintage feel
    let uv = pixelPos / screenSize;
    let dist = length(uv - vec2<f32>(0.5)) * 1.4;
    result *= 1.0 - 0.3 * dist * dist;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
