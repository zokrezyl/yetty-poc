// Post-effect: matrix
// Index: 5
//
// Green-tinted "Matrix" look with falling column shimmer.
// params[0] = green intensity (0.0-1.0, default 0.8)
// params[1] = shimmer speed (default 2.0)

fn postEffect_matrix(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let greenStr = select(0.8, params[0], params[0] > 0.0);
    let speed = select(2.0, params[1], params[1] > 0.0);

    // Convert to luminance
    let lum = dot(color, vec3<f32>(0.299, 0.587, 0.114));

    // Green-tinted monochrome
    var result = vec3<f32>(lum * 0.2, lum * greenStr, lum * 0.1);

    // Column-based shimmer (falling rain effect on brightness)
    let col = floor(pixelPos.x / 8.0);
    let colHash = fract(sin(col * 127.1) * 43758.5453);
    let shimmer = sin(pixelPos.y * 0.05 - time * speed * (0.5 + colHash)) * 0.5 + 0.5;
    result *= 0.7 + 0.3 * shimmer;

    // Slight phosphor glow
    result += vec3<f32>(0.0, 0.02, 0.0);

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
