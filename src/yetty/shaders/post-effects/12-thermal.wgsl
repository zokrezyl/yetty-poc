// Post-effect: thermal camera
// Index: 12
//
// Thermal imaging false-color: dark=blue, mid=green/yellow, bright=red/white.
// params[0] = intensity (0.0-1.0, default 1.0)

fn postEffect_thermal(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(1.0, params[0], params[0] > 0.0);

    let lum = dot(color, vec3<f32>(0.299, 0.587, 0.114));

    // Thermal color ramp: black → blue → green → yellow → red → white
    var thermal: vec3<f32>;
    if (lum < 0.15) {
        thermal = mix(vec3<f32>(0.0, 0.0, 0.1), vec3<f32>(0.0, 0.0, 0.8), lum / 0.15);
    } else if (lum < 0.35) {
        thermal = mix(vec3<f32>(0.0, 0.0, 0.8), vec3<f32>(0.0, 0.8, 0.2), (lum - 0.15) / 0.2);
    } else if (lum < 0.55) {
        thermal = mix(vec3<f32>(0.0, 0.8, 0.2), vec3<f32>(1.0, 1.0, 0.0), (lum - 0.35) / 0.2);
    } else if (lum < 0.8) {
        thermal = mix(vec3<f32>(1.0, 1.0, 0.0), vec3<f32>(1.0, 0.0, 0.0), (lum - 0.55) / 0.25);
    } else {
        thermal = mix(vec3<f32>(1.0, 0.0, 0.0), vec3<f32>(1.0, 1.0, 1.0), (lum - 0.8) / 0.2);
    }

    // Slight noise for sensor feel
    let noise = fract(sin(dot(pixelPos + vec2<f32>(time * 50.0, 0.0),
                               vec2<f32>(12.9898, 78.233))) * 43758.5453);
    thermal += (noise - 0.5) * 0.03;

    return clamp(mix(color, thermal, intensity), vec3<f32>(0.0), vec3<f32>(1.0));
}
