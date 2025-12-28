// Default fallback - pulsing circle

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;
    let dist = length(p);

    let pulse = 0.7 + 0.1 * sin(iTime * 2.0);
    var alpha = 0.0;

    if (dist < pulse) {
        alpha = 1.0;
    } else if (dist < pulse + 0.1) {
        alpha = (pulse + 0.1 - dist) / 0.1;
    }

    // Rainbow color based on time
    let hue = fract(iTime * 0.2);
    let c = abs(fract(hue + vec3<f32>(0.0, 0.33, 0.67)) * 6.0 - 3.0) - 1.0;
    let color = clamp(c, vec3<f32>(0.0), vec3<f32>(1.0));

    return vec4<f32>(color, alpha);
}
