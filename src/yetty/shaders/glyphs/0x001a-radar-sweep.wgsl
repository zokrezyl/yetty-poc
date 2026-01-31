// Shader glyph: Radar Sweep (codepoint 1052698 / U+10101A)
// A rotating line with fading trail, like a radar screen

fn shaderGlyph_1052698(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let center = vec2<f32>(0.5, 0.5);
    let uv = localUV - center;
    let dist = length(uv);
    let angle = atan2(uv.y, uv.x);

    let sweepSpeed = 2.0;
    let sweepAngle = time * sweepSpeed;
    let trailLength = 2.0;

    // Ring mask
    let outerRadius = 0.45;
    let ringMask = smoothstep(outerRadius + 0.03, outerRadius, dist);

    // Sweep trail
    var diff = angle - sweepAngle;
    diff = diff - floor(diff / 6.28318) * 6.28318;
    let trail = smoothstep(trailLength, 0.0, diff) * ringMask;

    // Center dot
    let centerDot = smoothstep(0.05, 0.02, dist);

    // Faint circle outline
    let ring = smoothstep(0.025, 0.0, abs(dist - outerRadius)) * 0.2;

    let alpha = max(max(trail, centerDot), ring);

    return mix(bgColor, fgColor, alpha);
}
