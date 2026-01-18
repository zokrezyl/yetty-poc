// Shader glyph: Spinner (codepoint 1048577 / U+100001)
// Renders an animated spinning indicator

fn shaderGlyph_1048577(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    let center = vec2<f32>(0.5, 0.5);
    let uv = localUV - center;

    // Polar coordinates
    let dist = length(uv);
    let angle = atan2(uv.y, uv.x);

    // Ring parameters
    let innerRadius = 0.25;
    let outerRadius = 0.45;
    let ringMask = smoothstep(innerRadius - 0.05, innerRadius, dist) *
                   smoothstep(outerRadius + 0.05, outerRadius, dist);

    // Rotating arc
    let rotSpeed = 3.0;
    let arcLength = 2.5;  // radians
    let rotAngle = time * rotSpeed;

    // Create arc that fades
    var arcAngle = angle - rotAngle;
    arcAngle = arcAngle - floor(arcAngle / 6.28318) * 6.28318;  // mod 2*PI
    let arcMask = smoothstep(0.0, 0.3, arcAngle) * smoothstep(arcLength, arcLength - 0.3, arcAngle);

    let alpha = ringMask * arcMask;
    return mix(bgColor, fgColor, alpha);
}
