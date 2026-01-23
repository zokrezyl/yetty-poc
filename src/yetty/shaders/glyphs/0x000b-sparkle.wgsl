// Shader glyph: Sparkle (codepoint 1052683 / U+10100B)
// Twinkling sparkle effect - uses pixelPos for unique patterns per cell

fn shaderGlyph_1052683_hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn shaderGlyph_1052683(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let bgColor = unpackColor(bg);
    // fgColor unused - sparkle has its own golden color

    var color = vec3<f32>(0.0);
    var alpha = 0.0;

    // Use pixelPos to create unique seed per cell (divide by approximate cell size)
    let cellSeed = floor(pixelPos / 20.0);

    // Multiple sparkle points per cell
    for (var i = 0; i < 5; i++) {
        let fi = f32(i);

        // Seed includes cell position for unique pattern per cell
        let seed = vec2<f32>(fi * 1.7 + cellSeed.x * 13.37, fi * 2.3 + cellSeed.y * 7.91);

        // Random position within cell
        let pos = vec2<f32>(shaderGlyph_1052683_hash(seed), shaderGlyph_1052683_hash(seed + 1.0));

        // Twinkle phase - varies with cell position for variety
        let phase = shaderGlyph_1052683_hash(seed + 2.0) * 6.28 + (cellSeed.x + cellSeed.y) * 0.5;
        let twinkle = pow(sin(time * 3.0 + phase) * 0.5 + 0.5, 3.0);

        // Distance to sparkle
        let d = length(localUV - pos);

        // Cross shape for sparkle
        let p = localUV - pos;
        let cross = min(abs(p.x), abs(p.y));
        let star = 0.01 / (cross + 0.01) * 0.02 / (d + 0.02);

        let sparkle = star * twinkle;

        // Golden/white color
        color += vec3<f32>(1.0, 0.9, 0.6) * sparkle;
        alpha += sparkle * 0.5;
    }

    return mix(bgColor, color, clamp(alpha, 0.0, 1.0));
}
