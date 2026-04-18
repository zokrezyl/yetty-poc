// Shader glyph: DNA Helix (codepoint 1052700 / U+10101C)
// Two interleaved sine waves with dots at crossing points, scrolling vertically

fn shaderGlyph_1052700(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let cx = 0.5;
    let amplitude = 0.25;
    let freq = 6.28318;
    let scrollSpeed = 2.0;
    let dotRadius = 0.06;
    let strandWidth = 0.025;

    let y = localUV.y + time * scrollSpeed * 0.1;

    // Two strands offset by PI
    let strand1_x = cx + amplitude * sin(y * freq);
    let strand2_x = cx + amplitude * sin(y * freq + 3.14159);

    // Strand lines
    let d1 = abs(localUV.x - strand1_x);
    let d2 = abs(localUV.x - strand2_x);

    // Depth: strand1 in front when sin > 0, strand2 in front otherwise
    let phase = sin(y * freq);
    let front1 = smoothstep(strandWidth, strandWidth * 0.3, d1);
    let front2 = smoothstep(strandWidth, strandWidth * 0.3, d2);
    let back1 = smoothstep(strandWidth, strandWidth * 0.3, d1) * 0.35;
    let back2 = smoothstep(strandWidth, strandWidth * 0.3, d2) * 0.35;

    var strandAlpha = 0.0;
    if (phase > 0.0) {
        strandAlpha = max(back2, front1);
    } else {
        strandAlpha = max(back1, front2);
    }

    // Rungs between strands at regular intervals
    let rungSpacing = 1.0 / freq;
    let rungY = y - floor(y / rungSpacing) * rungSpacing;
    let rungMask = smoothstep(0.015, 0.005, abs(rungY)) +
                   smoothstep(0.015, 0.005, abs(rungY - rungSpacing * 0.5));
    let rungX = min(localUV.x, 1.0 - localUV.x);
    let withinStrands = step(min(strand1_x, strand2_x), localUV.x) *
                        step(localUV.x, max(strand1_x, strand2_x));
    let rung = rungMask * withinStrands * 0.5;

    // Dots at crossing points
    var dotAlpha = 0.0;
    for (var i = -3; i <= 3; i++) {
        let crossY_base = f32(i) * rungSpacing * 0.5;
        let crossY_screen = crossY_base - time * scrollSpeed * 0.1;
        let crossY_local = crossY_screen - floor(crossY_screen);
        let crossPos = vec2<f32>(cx, crossY_local);
        let dd = length(localUV - crossPos);
        dotAlpha = max(dotAlpha, smoothstep(dotRadius, dotRadius * 0.3, dd));
    }

    let alpha = max(max(strandAlpha, rung), dotAlpha);
    return mix(bgColor, fgColor, alpha);
}
