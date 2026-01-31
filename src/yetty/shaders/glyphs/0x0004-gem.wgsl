// Shader glyph: Gem (codepoint 1052676 / U+101004)
// Shimmering gem/crystal effect

fn shaderGlyph_1052676(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let bgColor = unpackColor(bg);
    // fgColor unused - gem has its own colors

    let p = (localUV - 0.5) * 2.0;

    // Diamond shape
    let diamond = abs(p.x) + abs(p.y);

    if (diamond > 0.9) {
        return bgColor;
    }

    // Faceted look with gradient bands
    let facet = floor((p.x + p.y + 2.0) * 4.0) / 4.0;
    let facet2 = floor((p.x - p.y + 2.0) * 4.0) / 4.0;

    // Rainbow shimmer based on angle and time
    let angle = atan2(p.y, p.x);
    let shimmer = sin(angle * 3.0 + time * 2.0) * 0.5 + 0.5;

    // Prismatic colors
    let h = fract(facet + facet2 + shimmer * 0.3 + time * 0.1);
    let s = 0.7;
    let v = 0.8 + shimmer * 0.2;

    // HSV to RGB
    let c = v * s;
    let x = c * (1.0 - abs((h * 6.0) % 2.0 - 1.0));
    let m = v - c;

    var rgb: vec3<f32>;
    let hi = i32(h * 6.0) % 6;
    if (hi == 0) { rgb = vec3<f32>(c, x, 0.0); }
    else if (hi == 1) { rgb = vec3<f32>(x, c, 0.0); }
    else if (hi == 2) { rgb = vec3<f32>(0.0, c, x); }
    else if (hi == 3) { rgb = vec3<f32>(0.0, x, c); }
    else if (hi == 4) { rgb = vec3<f32>(x, 0.0, c); }
    else { rgb = vec3<f32>(c, 0.0, x); }
    rgb += m;

    // Edge highlight
    let edge = smoothstep(0.9, 0.7, diamond);

    return mix(bgColor, rgb, edge);
}
