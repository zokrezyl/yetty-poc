// Shader glyph: Smiley (codepoint 1048587 / U+10000B)
// Animated smiley face with blinking

fn shaderGlyph_1048587_sdCircle(p: vec2<f32>, r: f32) -> f32 {
    return length(p) - r;
}

fn shaderGlyph_1048587(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let bgColor = unpackColor(bg);
    // fgColor unused - smiley has its own yellow color

    let p = (localUV - 0.5) * 2.0;
    let blink = step(0.9, fract(time * 0.5));

    // Face
    let face = shaderGlyph_1048587_sdCircle(p, 0.85);
    var color = bgColor;
    var alpha = 0.0;

    if (face < 0.0) {
        color = vec3<f32>(1.0, 0.85, 0.2);
        alpha = 1.0;
    }

    // Eyes
    let eyeH = mix(0.12, 0.02, blink);
    let leftEye = shaderGlyph_1048587_sdCircle(vec2<f32>(p.x + 0.3, (p.y - 0.2) / (eyeH / 0.12)), 0.12);
    let rightEye = shaderGlyph_1048587_sdCircle(vec2<f32>(p.x - 0.3, (p.y - 0.2) / (eyeH / 0.12)), 0.12);
    if (leftEye < 0.0 || rightEye < 0.0) {
        color = vec3<f32>(0.1, 0.1, 0.1);
    }

    // Smile
    let smileDist = abs(length(p + vec2<f32>(0.0, 0.15)) - 0.5) - 0.06;
    if (smileDist < 0.0 && p.y > -0.1 && face < 0.0) {
        color = vec3<f32>(0.5, 0.25, 0.1);
    }

    return mix(bgColor, color, alpha);
}
