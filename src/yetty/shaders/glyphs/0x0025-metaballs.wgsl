// Shader glyph: Metaballs (codepoint 1052709 / U+101025)
// Blobby shapes that merge and split as they float around

fn shaderGlyph_1052709(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let uv = localUV;
    let r = 0.012;

    // Four blobs orbiting at different speeds and phases
    let b0 = vec2<f32>(0.5 + 0.25 * sin(time * 0.9), 0.5 + 0.3 * cos(time * 0.7));
    let b1 = vec2<f32>(0.5 + 0.2 * cos(time * 1.1 + 1.0), 0.5 + 0.25 * sin(time * 0.8 + 2.0));
    let b2 = vec2<f32>(0.5 + 0.3 * sin(time * 0.6 + 3.0), 0.5 + 0.2 * cos(time * 1.3 + 1.5));
    let b3 = vec2<f32>(0.5 + 0.15 * cos(time * 1.4 + 0.5), 0.5 + 0.35 * sin(time * 0.5 + 4.0));

    // Metaball field: sum of 1/distance^2
    let d0 = length(uv - b0);
    let d1 = length(uv - b1);
    let d2 = length(uv - b2);
    let d3 = length(uv - b3);

    let field = r / (d0 * d0) + r / (d1 * d1) + r / (d2 * d2) + r / (d3 * d3);

    let threshold = 1.0;
    let alpha = smoothstep(threshold - 0.3, threshold + 0.3, field);

    return mix(bgColor, fgColor, alpha);
}
