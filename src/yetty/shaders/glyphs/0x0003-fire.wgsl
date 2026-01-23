// Shader glyph: Fire (codepoint 1052675 / U+101003)
// Procedural noise-based fire effect

fn shaderGlyph_1052675_hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn shaderGlyph_1052675_noise(p: vec2<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f);

    let a = shaderGlyph_1052675_hash(i);
    let b = shaderGlyph_1052675_hash(i + vec2<f32>(1.0, 0.0));
    let c = shaderGlyph_1052675_hash(i + vec2<f32>(0.0, 1.0));
    let d = shaderGlyph_1052675_hash(i + vec2<f32>(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

fn shaderGlyph_1052675_fbm(p: vec2<f32>) -> f32 {
    var v = 0.0;
    var a = 0.5;
    var q = p;
    for (var i = 0; i < 4; i++) {
        v += a * shaderGlyph_1052675_noise(q);
        q = q * 2.0;
        a *= 0.5;
    }
    return v;
}

fn shaderGlyph_1052675(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let bgColor = unpackColor(bg);
    // fgColor unused - fire has its own colors

    var p = localUV;
    p.y = 1.0 - p.y;  // Flip so fire goes up

    // Animate
    let n = shaderGlyph_1052675_fbm(vec2<f32>(p.x * 4.0, p.y * 3.0 - time * 2.0));

    // Fire shape - wider at bottom
    let shape = 1.0 - p.y;
    let width = 0.3 + shape * 0.4;
    let center = abs(p.x - 0.5);

    var fire = shape * (1.0 - center / width);
    fire = fire * (0.5 + n * 0.5);
    fire = clamp(fire, 0.0, 1.0);

    // Fire colors
    let r = fire;
    let g = fire * fire * 0.7;
    let b = fire * fire * fire * 0.3;

    let alpha = smoothstep(0.0, 0.2, fire);
    let fireColor = vec3<f32>(r, g, b);

    return mix(bgColor, fireColor, alpha);
}
