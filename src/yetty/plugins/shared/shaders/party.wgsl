// Party confetti

fn hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;
    var color = vec3<f32>(0.0);
    var alpha = 0.0;

    // Confetti particles
    for (var i = 0; i < 15; i++) {
        let fi = f32(i);
        let seed = vec2<f32>(fi * 1.23, fi * 2.34);
        let angle = hash(seed) * 5.0 - 0.5;
        let speed = 0.4 + hash(seed + 1.0) * 0.4;
        let t = fract(iTime * 0.7 + hash(seed + 2.0));

        let origin = vec2<f32>(-0.3, -0.5);
        let dir = vec2<f32>(cos(angle), sin(angle));
        let pos = origin + dir * speed * t + vec2<f32>(0.0, t * t * 0.8);

        if (length(p - pos) < 0.1) {
            let h = hash(seed + 3.0);
            if (h < 0.33) { color = vec3<f32>(1.0, 0.3, 0.4); }
            else if (h < 0.66) { color = vec3<f32>(0.3, 0.8, 1.0); }
            else { color = vec3<f32>(1.0, 0.9, 0.2); }
            alpha = 1.0 - t * 0.5;
        }
    }

    return vec4<f32>(color, alpha);
}
