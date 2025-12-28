// Animated rocket with flames

fn hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;
    var color = vec3<f32>(0.0);
    var alpha = 0.0;

    // Rocket body (rotated 45 degrees pointing up-right)
    let rp = vec2<f32>(p.x * 0.7 - p.y * 0.7, p.x * 0.7 + p.y * 0.7);

    // Body
    if (abs(rp.x) < 0.2 && rp.y > -0.3 && rp.y < 0.5) {
        color = vec3<f32>(0.9, 0.9, 0.95);
        alpha = 1.0;
    }

    // Nose cone
    if (rp.y > 0.3 && rp.y < 0.7 && abs(rp.x) < (0.7 - rp.y) * 0.5) {
        color = vec3<f32>(1.0, 0.3, 0.2);
        alpha = 1.0;
    }

    // Flames
    let flameY = rp.y + 0.5;
    if (flameY > 0.0 && flameY < 0.5 && abs(rp.x) < 0.15) {
        let flicker = hash(vec2<f32>(iTime * 10.0, rp.x * 100.0));
        let intensity = (0.5 - flameY) * 2.0 * (0.7 + flicker * 0.3);
        color = mix(vec3<f32>(1.0, 0.8, 0.2), vec3<f32>(1.0, 0.3, 0.1), flameY * 2.0);
        alpha = intensity;
    }

    return vec4<f32>(color, alpha);
}
