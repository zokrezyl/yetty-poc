// Pulsing heart - works great as emoji or decorator
// Based on classic Shadertoy heart SDF

fn sdHeart(p: vec2<f32>) -> f32 {
    let q = vec2<f32>(abs(p.x), p.y);
    let w = q - vec2<f32>(0.25, 0.75);

    if (q.x + q.y > 1.0) {
        return sqrt(dot(w, w)) - 0.25;
    }

    let b = vec2<f32>(0.0, 1.0) - q;
    let c = vec2<f32>(-0.25, 0.75) - q;

    return sqrt(min(dot(b, b), dot(c, c))) * sign(q.x - q.y);
}

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    var p = (uv - 0.5) * 2.2;
    p.y -= 0.1;

    // Heartbeat animation
    let beat = 1.0 + 0.1 * sin(iTime * 6.0) * exp(-2.0 * fract(iTime * 1.5));
    p /= beat;

    let d = sdHeart(p);

    // Red heart with soft edge
    let color = vec3<f32>(0.9, 0.1, 0.2);
    let alpha = 1.0 - smoothstep(-0.02, 0.02, d);

    // Add glow
    let glow = 0.02 / (abs(d) + 0.02);
    let glowColor = color * glow * 0.5;

    return vec4<f32>(color * alpha + glowColor, alpha + glow * 0.3);
}
