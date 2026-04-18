// Shader glyph: Signal Bars (codepoint 1052712 / U+101028)
// Animated signal strength bars pulsing up and down

fn signalBars_hash(n: f32) -> f32 {
    return fract(sin(n * 127.1) * 43758.5453);
}

fn shaderGlyph_1052712(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let numBars = 4.0;
    let barGap = 0.04;
    let totalW = 0.7;
    let barW = (totalW - barGap * (numBars - 1.0)) / numBars;
    let startX = (1.0 - totalW) * 0.5;
    let baseY = 0.85;
    let maxH = 0.7;

    var alpha = 0.0;

    for (var i = 0; i < 4; i++) {
        let fi = f32(i);
        let barX = startX + fi * (barW + barGap);

        // Each bar has a base height that increases left to right
        let baseH = (fi + 1.0) / numBars * maxH;

        // Animate: wave pattern
        let wave = sin(time * 2.5 - fi * 0.8) * 0.5 + 0.5;
        let h = baseH * (0.3 + 0.7 * wave);

        let inX = step(barX, localUV.x) * step(localUV.x, barX + barW);
        let inY = step(baseY - h, localUV.y) * step(localUV.y, baseY);

        alpha = max(alpha, inX * inY);
    }

    return mix(bgColor, fgColor, alpha);
}
