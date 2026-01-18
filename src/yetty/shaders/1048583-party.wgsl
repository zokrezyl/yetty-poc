// Shader glyph: Party (codepoint 1048583 / U+100007)
// Party confetti effect

fn shaderGlyph_1048583_hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn shaderGlyph_1048583(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    let p = (localUV - 0.5) * 2.0;
    var color = bgColor;
    var alpha = 0.0;

    // Confetti particles
    for (var i = 0; i < 15; i++) {
        let fi = f32(i);
        let seed = vec2<f32>(fi * 1.23, fi * 2.34);
        let angle = shaderGlyph_1048583_hash(seed) * 5.0 - 0.5;
        let speed = 0.4 + shaderGlyph_1048583_hash(seed + 1.0) * 0.4;
        let t = fract(time * 0.7 + shaderGlyph_1048583_hash(seed + 2.0));

        let origin = vec2<f32>(-0.3, -0.5);
        let dir = vec2<f32>(cos(angle), sin(angle));
        let pos = origin + dir * speed * t + vec2<f32>(0.0, t * t * 0.8);

        if (length(p - pos) < 0.1) {
            let h = shaderGlyph_1048583_hash(seed + 3.0);
            var confettiColor: vec3<f32>;
            if (h < 0.33) { confettiColor = vec3<f32>(1.0, 0.3, 0.4); }
            else if (h < 0.66) { confettiColor = vec3<f32>(0.3, 0.8, 1.0); }
            else { confettiColor = vec3<f32>(1.0, 0.9, 0.2); }
            alpha = 1.0 - t * 0.5;
            color = confettiColor;
        }
    }

    return mix(bgColor, color, alpha);
}
