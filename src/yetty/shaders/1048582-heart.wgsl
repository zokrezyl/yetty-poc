// Shader glyph: Heart (codepoint 1048582 / U+100006)
// Pulsing heart effect

fn shaderGlyph_1048582_sdHeart(p: vec2<f32>) -> f32 {
    let q = vec2<f32>(abs(p.x), p.y);
    let w = q - vec2<f32>(0.25, 0.75);

    if (q.x + q.y > 1.0) {
        return sqrt(dot(w, w)) - 0.25;
    }

    let b = vec2<f32>(0.0, 1.0) - q;
    let c = vec2<f32>(-0.25, 0.75) - q;

    return sqrt(min(dot(b, b), dot(c, c))) * sign(q.x - q.y);
}

fn shaderGlyph_1048582(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    var p = (localUV - 0.5) * 2.2;
    p.y -= 0.1;

    // Heartbeat animation
    let beat = 1.0 + 0.1 * sin(time * 6.0) * exp(-2.0 * fract(time * 1.5));
    p /= beat;

    let d = shaderGlyph_1048582_sdHeart(p);

    // Red heart with soft edge
    let heartColor = vec3<f32>(0.9, 0.1, 0.2);
    let alpha = 1.0 - smoothstep(-0.02, 0.02, d);

    // Add glow
    let glow = 0.02 / (abs(d) + 0.02);
    let glowColor = heartColor * glow * 0.5;

    let finalAlpha = alpha + glow * 0.3;
    let finalColor = heartColor * alpha + glowColor;

    return mix(bgColor, finalColor, clamp(finalAlpha, 0.0, 1.0));
}
