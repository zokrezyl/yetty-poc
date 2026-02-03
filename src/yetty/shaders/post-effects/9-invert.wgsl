// Post-effect: invert
// Index: 9
//
// Color inversion with optional partial blend.
// params[0] = invert amount (0.0-1.0, default 1.0)

fn postEffect_invert(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let amount = select(1.0, params[0], params[0] > 0.0);
    return mix(color, vec3<f32>(1.0) - color, amount);
}
