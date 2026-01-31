// Shader glyph: Crosshair Pulse (codepoint 1052707 / U+101023)
// A crosshair that pulses in size

fn shaderGlyph_1052707(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let center = vec2<f32>(0.5, 0.5);
    let uv = localUV - center;

    let pulse = 0.8 + 0.2 * sin(time * 3.0);
    let lineWidth = 0.018;
    let armLength = 0.35 * pulse;
    let gapRadius = 0.08 * pulse;
    let ringRadius = 0.25 * pulse;
    let ringWidth = 0.02;

    let dist = length(uv);
    let ax = abs(uv.x);
    let ay = abs(uv.y);

    // Crosshair arms: horizontal and vertical lines with gap in center
    let hLine = step(ay, lineWidth) * step(gapRadius, ax) * step(ax, armLength);
    let vLine = step(ax, lineWidth) * step(gapRadius, ay) * step(ay, armLength);

    // Ring
    let ring = smoothstep(ringWidth, ringWidth * 0.3, abs(dist - ringRadius));

    // Center dot
    let centerDot = smoothstep(0.03, 0.01, dist);

    let alpha = max(max(hLine, vLine), max(ring, centerDot));
    return mix(bgColor, fgColor, alpha);
}
