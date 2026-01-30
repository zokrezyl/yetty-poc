// Shader glyph: Orbit Dots (codepoint 1052694 / U+101016)
// Dots orbiting an elliptical path, bunching up on one side
// Inspired by the Windows loading animation

fn shaderGlyph_1052694(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let center = vec2<f32>(0.5, 0.5);
    let radiusX = 0.44;
    let radiusY = 0.38;
    let speed = 2.5;
    let dotRadius = 0.08;
    let dotSpacing = 0.18;
    let easeStrength = 0.8;

    var alpha = 0.0;

    for (var i = 0; i < 5; i++) {
        let rawAngle = time * speed - f32(i) * dotSpacing;
        // Non-linear easing: fast on right, slow/bunching on left (at PI)
        let easedAngle = rawAngle + easeStrength * sin(rawAngle);

        let pos = center + vec2<f32>(
            cos(easedAngle) * radiusX,
            sin(easedAngle) * radiusY
        );
        let d = length(localUV - pos);
        alpha = max(alpha, smoothstep(dotRadius, dotRadius * 0.3, d));
    }

    return mix(bgColor, fgColor, alpha);
}
