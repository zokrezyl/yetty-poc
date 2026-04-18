// Post-effect: wave
// Index: 8
//
// Wavy distortion — shifts brightness in a sine-wave pattern.
// params[0] = amplitude (0.0-1.0, default 0.3)
// params[1] = frequency (default 0.05)
// params[2] = speed (default 2.0)

fn postEffect_wave(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let amp = select(0.3, params[0], params[0] > 0.0);
    let freq = select(0.05, params[1], params[1] > 0.0);
    let speed = select(2.0, params[2], params[2] > 0.0);

    // Horizontal wave modulates brightness
    let wave1 = sin(pixelPos.y * freq + time * speed) * amp;
    // Vertical wave adds cross-pattern
    let wave2 = sin(pixelPos.x * freq * 0.7 + time * speed * 0.8) * amp * 0.5;

    let brightness = 1.0 + wave1 + wave2;
    return clamp(color * brightness, vec3<f32>(0.0), vec3<f32>(1.0));
}
