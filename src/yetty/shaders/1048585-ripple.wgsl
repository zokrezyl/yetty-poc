// Shader glyph: Ripple (codepoint 1048585 / U+100009)
// Water ripple effect - concentric animated rings

fn shaderGlyph_1048585(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    let p = (localUV - 0.5) * 2.0;
    let d = length(p);

    // Ripple waves
    let wave = sin(d * 15.0 - time * 4.0) * 0.5 + 0.5;
    let fade = 1.0 - smoothstep(0.0, 1.0, d);

    // Blue water color
    let base = vec3<f32>(0.1, 0.3, 0.8);
    let highlight = vec3<f32>(0.4, 0.7, 1.0);

    let waterColor = mix(base, highlight, wave * fade);
    let alpha = fade * (0.6 + wave * 0.4);

    return mix(bgColor, waterColor, alpha);
}
