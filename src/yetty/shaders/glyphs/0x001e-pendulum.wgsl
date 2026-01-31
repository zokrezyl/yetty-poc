// Shader glyph: Pendulum (codepoint 1052702 / U+10101E)
// A dot swinging on an arc with realistic easing

fn shaderGlyph_1052702(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let pivot = vec2<f32>(0.5, 0.1);
    let armLength = 0.55;
    let maxAngle = 0.85;
    let dotRadius = 0.08;
    let armWidth = 0.012;

    // Simple harmonic motion: angle = maxAngle * sin(time * speed)
    let speed = 2.5;
    let angle = maxAngle * sin(time * speed);

    // Bob position
    let bobPos = pivot + vec2<f32>(sin(angle), cos(angle)) * armLength;

    // Arm line: distance from point to line segment pivot->bobPos
    let pa = localUV - pivot;
    let ba = bobPos - pivot;
    let h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    let armDist = length(pa - ba * h);
    let armAlpha = smoothstep(armWidth, armWidth * 0.3, armDist) * 0.5;

    // Pivot dot
    let pivotDist = length(localUV - pivot);
    let pivotAlpha = smoothstep(0.03, 0.01, pivotDist);

    // Bob dot
    let bobDist = length(localUV - bobPos);
    let bobAlpha = smoothstep(dotRadius, dotRadius * 0.3, bobDist);

    let alpha = max(max(armAlpha, pivotAlpha), bobAlpha);
    return mix(bgColor, fgColor, alpha);
}
