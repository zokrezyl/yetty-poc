// Shader glyph: Border Dots Grouped (codepoint 1052695 / U+101017)
// Three dots close together traveling as a group along the cell perimeter

fn borderDotsGrouped_perimeterPos(t: f32, w: f32, h: f32) -> vec2<f32> {
    let perimeter = 2.0 * (w + h);
    let tt = t - floor(t / perimeter) * perimeter;

    if (tt < w) {
        return vec2<f32>(tt, 0.0);
    }
    if (tt < w + h) {
        return vec2<f32>(w, tt - w);
    }
    if (tt < 2.0 * w + h) {
        return vec2<f32>(w - (tt - w - h), h);
    }
    return vec2<f32>(0.0, h - (tt - 2.0 * w - h));
}

fn shaderGlyph_1052695(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let margin = 0.08;
    let w = 1.0 - 2.0 * margin;
    let h = 1.0 - 2.0 * margin;
    let perimeter = 2.0 * (w + h);

    let uv = localUV - vec2<f32>(margin);
    let speed = 1.5;
    let dotRadius = 0.09;
    let gap = 0.12;

    var alpha = 0.0;

    // Three dots close together, moving as a group
    for (var i = 0; i < 3; i++) {
        let phase = time * speed + f32(i) * gap;
        let pos = borderDotsGrouped_perimeterPos(phase, w, h);
        let d = length(uv - pos);
        alpha = max(alpha, smoothstep(dotRadius, dotRadius * 0.4, d));
    }

    return mix(bgColor, fgColor, alpha);
}
