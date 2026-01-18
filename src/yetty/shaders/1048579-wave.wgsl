// Shader glyph: Wave bars (codepoint 1048579 / U+100003)
// Renders animated audio-like wave bars

fn shaderGlyph_1048579(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    let numBars = 4.0;
    let barWidth = 0.15;
    let gap = (1.0 - numBars * barWidth) / (numBars + 1.0);

    var alpha = 0.0;

    for (var i = 0.0; i < numBars; i += 1.0) {
        let barX = gap + i * (barWidth + gap) + barWidth * 0.5;
        let barDist = abs(localUV.x - barX);

        // Each bar has different phase
        let phase = i * 0.8;
        let height = 0.3 + 0.35 * sin(time * 3.0 + phase);

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
