// Twinkling sparkle effect
// Good for sparkles emoji or magic effects

fn hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    var color = vec3<f32>(0.0);
    var alpha = 0.0;

    // Multiple sparkle points
    for (var i = 0; i < 5; i++) {
        let fi = f32(i);
        let seed = vec2<f32>(fi * 1.7, fi * 2.3);

        // Random position
        let pos = vec2<f32>(hash(seed), hash(seed + 1.0));

        // Twinkle phase
        let phase = hash(seed + 2.0) * 6.28;
        let twinkle = pow(sin(iTime * 3.0 + phase) * 0.5 + 0.5, 3.0);

        // Distance to sparkle
        let d = length(uv - pos);

        // Cross shape for sparkle
        let p = uv - pos;
        let cross = min(abs(p.x), abs(p.y));
        let star = 0.01 / (cross + 0.01) * 0.02 / (d + 0.02);

        let sparkle = star * twinkle;

        // Golden/white color
        color += vec3<f32>(1.0, 0.9, 0.6) * sparkle;
        alpha += sparkle * 0.5;
    }

    return vec4<f32>(color, clamp(alpha, 0.0, 1.0));
}
