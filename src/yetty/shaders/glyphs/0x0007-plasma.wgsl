// Shader glyph: Plasma (codepoint 1052679 / U+101007)
// Classic plasma effect - seamlessly tiles across screen using pixelPos

fn shaderGlyph_1052679(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // fgColor/bgColor unused - plasma has its own colors
    // Use pixel position for seamless tiling across cells
    // Scale down for nice pattern size
    let p = pixelPos * 0.02;
    let t = time * 0.5;

    var v = 0.0;
    v += sin(p.x + t);
    v += sin(p.y + t * 0.5);
    v += sin(p.x + p.y + t * 0.3);
    v += sin(sqrt(p.x * p.x + p.y * p.y) * 0.5 + t);
    v /= 4.0;

    // Rainbow colors
    let r = sin(v * 3.14159 + 0.0) * 0.5 + 0.5;
    let g = sin(v * 3.14159 + 2.094) * 0.5 + 0.5;
    let b = sin(v * 3.14159 + 4.188) * 0.5 + 0.5;

    return vec3<f32>(r, g, b);
}
