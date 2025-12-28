// Glowing pulsing star

fn sdStar(p: vec2<f32>, r: f32, n: i32, m: f32) -> f32 {
    let an = 3.141593 / f32(n);
    let en = 3.141593 / m;
    let acs = vec2<f32>(cos(an), sin(an));
    let ecs = vec2<f32>(cos(en), sin(en));

    var bn = atan2(p.x, p.y) % (2.0 * an) - an;
    var q = length(p) * vec2<f32>(cos(bn), abs(sin(bn)));

    q = q - r * acs;
    q = q + ecs * clamp(-dot(q, ecs), 0.0, r * acs.y / ecs.y);

    return length(q) * sign(q.x);
}

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;

    // Pulsing glow
    let pulse = 0.8 + 0.2 * sin(iTime * 3.0);
    let glow = 1.0 + 0.3 * sin(iTime * 5.0);

    // Star shape
    let star = sdStar(p, 0.5 * pulse, 5, 2.5);

    var color = vec3<f32>(0.0);
    var alpha = 0.0;

    // Outer glow
    if (star < 0.3) {
        let glowIntensity = (0.3 - star) / 0.3;
        color = vec3<f32>(1.0, 0.9, 0.5) * glowIntensity * glow * 0.5;
        alpha = glowIntensity * 0.5;
    }

    // Star body
    if (star < 0.0) {
        color = vec3<f32>(1.0, 0.95, 0.4) * glow;
        alpha = 1.0;
    }

    return vec4<f32>(color, alpha);
}
