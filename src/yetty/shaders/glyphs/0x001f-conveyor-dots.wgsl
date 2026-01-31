// Shader glyph: Conveyor Dots (codepoint 1052703 / U+10101F)
// Dots moving across the cell, seamlessly wrapping horizontally

fn shaderGlyph_1052703(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let dotRadius = 0.08;
    let numDots = 4;
    let spacing = 1.0 / f32(numDots);
    let speed = 0.4;
    let cy = 0.5;

    var alpha = 0.0;

    for (var i = 0; i < 4; i++) {
        // Each dot starts at evenly spaced positions, drifts right and wraps
        var x = f32(i) * spacing + time * speed;
        x = x - floor(x); // wrap to [0, 1)

        let pos = vec2<f32>(x, cy);
        let d = length(localUV - pos);
        alpha = max(alpha, smoothstep(dotRadius, dotRadius * 0.3, d));
    }

    return mix(bgColor, fgColor, alpha);
}
