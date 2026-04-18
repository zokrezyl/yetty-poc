// Shader glyph: Wave bars (codepoint 1052674 / U+101002)
// Renders animated audio-like wave bars
// Uses pixelPos.x for phase bias so waves flow across tiled cells

fn shaderGlyph_1052674(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let numBars = 4.0;
    let barWidth = 0.15;
    let gap = (1.0 - numBars * barWidth) / (numBars + 1.0);

    // X bias from pixel position - wave flows across screen
    let xBias = pixelPos.x * 0.02;

    var alpha = 0.0;

    for (var i = 0.0; i < numBars; i += 1.0) {
        let barX = gap + i * (barWidth + gap) + barWidth * 0.5;
        let barDist = abs(localUV.x - barX);

        // Each bar has different phase, plus x position bias for traveling wave
        let phase = i * 0.8 + xBias;
        let height = 0.3 + 0.35 * sin(time * 3.0 - phase);

        // Bar from bottom, centered vertically
        let barBottom = 0.5 - height;
        let barTop = 0.5 + height;

        let inBar = step(barDist, barWidth * 0.5) *
                    step(barBottom, localUV.y) *
                    step(localUV.y, barTop);

        alpha = max(alpha, inBar);
    }

    return mix(bgColor, fgColor, alpha);
}
