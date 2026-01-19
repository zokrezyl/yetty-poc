// Shader glyph: Pulse dot (codepoint 1048578 / U+100002)
// Renders a pulsing circular dot

fn shaderGlyph_1048578(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let center = vec2<f32>(0.5, 0.5);
    let uv = localUV - center;
    let dist = length(uv);

    // Pulsing radius
    let baseRadius = 0.3;
    let pulseAmount = 0.1;
    let pulseSpeed = 2.0;
    let radius = baseRadius + sin(time * pulseSpeed) * pulseAmount;

    // Soft circle
    let alpha = smoothstep(radius + 0.05, radius - 0.05, dist);

    return mix(bgColor, fgColor, alpha);
}
