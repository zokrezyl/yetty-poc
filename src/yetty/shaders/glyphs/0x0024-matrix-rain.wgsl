// Shader glyph: Matrix Rain (codepoint 1052708 / U+101024)
// Columns of falling dots at different speeds

fn matrixRain_hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn shaderGlyph_1052708(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let cols = 3.0;
    let rows = 6.0;
    let cellW = 1.0 / cols;
    let cellH = 1.0 / rows;

    var alpha = 0.0;

    let col = floor(localUV.x * cols);
    let cellX = (localUV.x - col * cellW) / cellW;

    // Each column has its own speed and phase
    let speed = 1.5 + matrixRain_hash(vec2<f32>(col, 0.0)) * 2.0;
    let phase = matrixRain_hash(vec2<f32>(col, 1.0)) * 10.0;
    let scroll = time * speed + phase;

    let row = floor(localUV.y * rows - scroll);
    let cellY = fract(localUV.y * rows - scroll);

    // Dot in cell center
    let dotCenter = vec2<f32>(0.5, 0.5);
    let dotPos = vec2<f32>(cellX, cellY);
    let d = length(dotPos - dotCenter);

    // Brightness fades for trailing dots
    let trailLen = 4.0;
    let rowInTrail = fract(scroll) + floor(localUV.y * rows) - floor(scroll);
    let brightness = 1.0 - clamp(rowInTrail / trailLen, 0.0, 1.0);

    // Flicker based on hash
    let flicker = step(0.3, matrixRain_hash(vec2<f32>(col, floor(row))));

    let dotAlpha = smoothstep(0.35, 0.15, d) * brightness * flicker;
    alpha = dotAlpha;

    return mix(bgColor, fgColor, alpha);
}
