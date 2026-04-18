// Shader glyph: Equalizer (codepoint 1052701 / U+10101D)
// 4 vertical bars bouncing at different pseudo-random heights

fn eq_hash(n: f32) -> f32 {
    return fract(sin(n * 43.1) * 43758.5453);
}

fn shaderGlyph_1052701(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let numBars = 4;
    let barWidth = 0.12;
    let gap = (1.0 - f32(numBars) * barWidth) / f32(numBars + 1);
    let bottom = 0.88;

    var alpha = 0.0;

    for (var i = 0; i < 4; i++) {
        let fi = f32(i);
        let barCenterX = gap + barWidth * 0.5 + fi * (barWidth + gap);

        // Layered sine waves for organic bouncing
        let speed1 = 2.0 + eq_hash(fi) * 2.0;
        let speed2 = 3.5 + eq_hash(fi + 10.0) * 2.0;
        let speed3 = 1.3 + eq_hash(fi + 20.0) * 1.5;
        let h = 0.3 + 0.25 * abs(sin(time * speed1 + fi * 1.7)) +
                       0.15 * abs(sin(time * speed2 + fi * 2.3)) +
                       0.1 * abs(sin(time * speed3 + fi * 0.9));
        let barHeight = clamp(h, 0.1, 0.75);

        let inX = step(abs(localUV.x - barCenterX), barWidth * 0.5);
        let inY = step(bottom - barHeight, localUV.y) * step(localUV.y, bottom);
        alpha = max(alpha, inX * inY);
    }

    return mix(bgColor, fgColor, alpha);
}
