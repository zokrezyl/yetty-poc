// Shader glyph: DVD Bounce (codepoint 1052714 / U+10102A)
// A small shape bouncing around the cell, classic screensaver style

fn dvdBounce_mod(x: f32, y: f32) -> f32 {
    return x - y * floor(x / y);
}

fn shaderGlyph_1052714(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let margin = 0.12;
    let rangeX = 1.0 - 2.0 * margin;
    let rangeY = 1.0 - 2.0 * margin;

    // Triangle wave bounce
    let speedX = 0.35;
    let speedY = 0.23;
    let phaseX = dvdBounce_mod(time * speedX, rangeX * 2.0);
    let phaseY = dvdBounce_mod(time * speedY, rangeY * 2.0);
    let ballX = margin + select(phaseX, rangeX * 2.0 - phaseX, phaseX > rangeX);
    let ballY = margin + select(phaseY, rangeY * 2.0 - phaseY, phaseY > rangeY);

    let ballPos = vec2<f32>(ballX, ballY);

    // Draw a small diamond shape
    let rel = abs(localUV - ballPos);
    let diamondSize = 0.08;
    let diamondDist = rel.x + rel.y;
    var alpha = smoothstep(diamondSize, diamondSize * 0.5, diamondDist);

    // Faint trail
    let dx = speedX * select(1.0, -1.0, phaseX > rangeX);
    let dy = speedY * select(1.0, -1.0, phaseY > rangeY);
    let dir = normalize(vec2<f32>(dx, dy));
    for (var i = 1; i < 4; i++) {
        let trailPos = ballPos - dir * f32(i) * 0.05;
        let td = abs(localUV - trailPos);
        let tDist = td.x + td.y;
        let tSize = diamondSize * (1.0 - f32(i) * 0.2);
        let tAlpha = smoothstep(tSize, tSize * 0.5, tDist) * (1.0 - f32(i) * 0.3);
        alpha = max(alpha, tAlpha * 0.3);
    }

    return mix(bgColor, fgColor, alpha);
}
