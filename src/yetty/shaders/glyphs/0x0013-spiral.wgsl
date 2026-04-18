// Shader glyph: Spiral (codepoint 1052691 / U+101013)
// Rotating Archimedean spiral using analytical distance

const SP_TAU: f32 = 6.28318530;

fn shaderGlyph_1052691(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    var p = (localUV - 0.5) * 2.2;
    p.y = -p.y;

    let r = length(p);
    let angle = atan2(p.y, p.x);

    // Archimedean spiral: r = growth * (theta)
    // For multi-arm: each arm offset by TAU/arms
    let arms = 3.0;
    let growth = 0.12;
    let rotation = time * 1.5;

    // Analytical distance to spiral:
    // The spiral arm nearest to polar coord (r, angle) satisfies:
    //   r_spiral = growth * (angle - rotation + TAU * k + TAU * arm/arms)
    // Solve for k: k = (r/growth - angle + rotation - TAU*arm/arms) / TAU
    // Round to nearest integer to get closest arm passage

    var minDist = 1e10;
    for (var arm = 0.0; arm < 3.0; arm += 1.0) {
        let armOffset = SP_TAU * arm / arms;
        let rawK = (r / growth - angle + rotation - armOffset) / SP_TAU;
        // Check two nearest integers
        for (var di = 0.0; di <= 1.0; di += 1.0) {
            let k = floor(rawK) + di;
            let spiralR = growth * (angle - rotation + armOffset + SP_TAU * k);
            if (spiralR > 0.0 && spiralR < 1.1) {
                // Distance between concentric circles at r and spiralR
                let d = abs(r - spiralR);
                minDist = min(minDist, d);
            }
        }
    }

    // Fade out at edges
    let fade = 1.0 - smoothstep(0.8, 1.0, r);

    let lineW = 0.04;
    let alpha = (1.0 - smoothstep(lineW - 0.02, lineW + 0.02, minDist)) * fade;

    // Soft glow
    let glow = 0.008 / (minDist + 0.008) * fade * 0.25;

    return mix(bgColor, fgColor, clamp(alpha + glow, 0.0, 1.0));
}
