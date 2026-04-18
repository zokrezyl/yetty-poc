// Post-effect: night vision
// Index: 10
//
// Green monochrome night-vision goggles with noise and vignette.
// params[0] = green intensity (0.0-1.0, default 0.9)
// params[1] = noise amount (0.0-1.0, default 0.15)
// params[2] = vignette radius (0.0-1.0, default 0.6)

fn postEffect_nightVision(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let greenStr = select(0.9, params[0], params[0] > 0.0);
    let noiseStr = select(0.15, params[1], params[1] > 0.0);
    let vigRadius = select(0.6, params[2], params[2] > 0.0);

    // Luminance
    let lum = dot(color, vec3<f32>(0.299, 0.587, 0.114));

    // Amplified green monochrome
    var result = vec3<f32>(lum * 0.1, lum * greenStr * 1.5, lum * 0.05);

    // Animated noise
    let noise = fract(sin(dot(pixelPos + vec2<f32>(time * 120.0, time * 90.0),
                               vec2<f32>(12.9898, 78.233))) * 43758.5453);
    result += vec3<f32>(0.0, (noise - 0.5) * noiseStr, 0.0);

    // Circular vignette (binocular look)
    let uv = pixelPos / screenSize;
    let center = vec2<f32>(0.5, 0.5);
    let dist = length(uv - center);
    let vig = smoothstep(vigRadius + 0.3, vigRadius, dist);
    result *= vig;

    // Scanline overlay
    let scanline = 0.95 + 0.05 * sin(pixelPos.y * 2.0);
    result *= scanline;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
