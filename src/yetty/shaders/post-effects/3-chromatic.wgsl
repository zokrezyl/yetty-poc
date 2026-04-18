// Post-effect: chromatic aberration
// Index: 3
//
// Simulates lens chromatic aberration by shifting color channels
// based on distance from center. Single-pass approximation using
// color intensity modulation.
// params[0] = intensity (0.0-1.0, default 0.5)
// params[1] = center x (0.0-1.0, default 0.5)
// params[2] = center y (0.0-1.0, default 0.5)

fn postEffect_chromatic(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(0.5, params[0], params[0] > 0.0);
    let cx = select(0.5, params[1], params[1] > 0.0);
    let cy = select(0.5, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    let center = vec2<f32>(cx, cy);
    let offset = uv - center;
    let dist = length(offset);

    // Angle from center determines channel shift direction
    let angle = atan2(offset.y, offset.x);

    // Modulate channels based on distance and angle
    let shift = dist * intensity * 0.3;
    var result: vec3<f32>;
    result.r = color.r * (1.0 + shift * cos(angle));
    result.g = color.g * (1.0 - shift * 0.5);
    result.b = color.b * (1.0 + shift * cos(angle + 3.14159));

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
