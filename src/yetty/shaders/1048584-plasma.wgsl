// Shader glyph: Plasma (codepoint 1048584 / U+100008)
// Classic plasma effect - colorful animated pattern

fn shaderGlyph_1048584(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    let p = localUV * 3.0;
    let t = time * 0.5;

    var v = 0.0;
    v += sin(p.x + t);
    v += sin(p.y + t * 0.5);
    v += sin(p.x + p.y + t * 0.3);
    v += sin(sqrt(p.x * p.x + p.y * p.y) + t);
    v /= 4.0;

    // Rainbow colors
    let r = sin(v * 3.14159 + 0.0) * 0.5 + 0.5;
    let g = sin(v * 3.14159 + 2.094) * 0.5 + 0.5;
    let b = sin(v * 3.14159 + 4.188) * 0.5 + 0.5;

    return vec3<f32>(r, g, b);
}
