// Post-effect: emboss
// Index: 14
//
// Emboss/relief effect — emphasizes edges by luminance gradient approximation.
// params[0] = strength (0.0-2.0, default 1.0)
// params[1] = angle in radians (default 0.785 = 45 degrees)

fn postEffect_emboss(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let strength = select(1.0, params[0], params[0] > 0.0);
    let angle = select(0.785, params[1], params[1] > 0.0);

    let lum = dot(color, vec3<f32>(0.299, 0.587, 0.114));

    // Approximate local gradient using position-based hash
    // (can't sample neighbors, so use luminance + position trick)
    let dx = cos(angle);
    let dy = sin(angle);
    let gradient = sin(lum * 50.0 + pixelPos.x * dx * 0.5 + pixelPos.y * dy * 0.5);

    let embossed = 0.5 + gradient * strength * 0.5;
    return clamp(vec3<f32>(embossed) * color * 1.5, vec3<f32>(0.0), vec3<f32>(1.0));
}
