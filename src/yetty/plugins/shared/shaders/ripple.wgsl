// Water ripple effect - concentric animated rings
// Good for water emoji or as decorator

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;
    let d = length(p);

    // Ripple waves
    let wave = sin(d * 15.0 - iTime * 4.0) * 0.5 + 0.5;
    let fade = 1.0 - smoothstep(0.0, 1.0, d);

    // Blue water color
    let base = vec3<f32>(0.1, 0.3, 0.8);
    let highlight = vec3<f32>(0.4, 0.7, 1.0);

    let color = mix(base, highlight, wave * fade);
    let alpha = fade * (0.6 + wave * 0.4);

    return vec4<f32>(color, alpha);
}
