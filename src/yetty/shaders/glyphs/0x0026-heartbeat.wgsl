// Shader glyph: Heartbeat (codepoint 1052710 / U+101026)
// ECG-style waveform scrolling horizontally

fn heartbeat_wave(x: f32) -> f32 {
    // ECG-like waveform over one cycle [0, 1]
    // Flat baseline with a sharp spike
    let t = fract(x);

    // P wave (small bump)
    let p = 0.08 * smoothstep(0.05, 0.1, t) * smoothstep(0.2, 0.15, t);

    // QRS complex (sharp spike)
    let q = -0.05 * smoothstep(0.28, 0.32, t) * smoothstep(0.36, 0.32, t);
    let r = 0.45 * smoothstep(0.32, 0.37, t) * smoothstep(0.42, 0.37, t);
    let s = -0.1 * smoothstep(0.38, 0.43, t) * smoothstep(0.48, 0.43, t);

    // T wave (broad bump)
    let tw = 0.12 * smoothstep(0.5, 0.58, t) * smoothstep(0.72, 0.64, t);

    return p + q + r + s + tw;
}

fn shaderGlyph_1052710(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let scrollSpeed = 0.4;
    let x = localUV.x + time * scrollSpeed;
    let baseline = 0.5;
    let waveY = baseline - heartbeat_wave(x);
    let lineWidth = 0.03;

    let dist = abs(localUV.y - waveY);
    let alpha = smoothstep(lineWidth, lineWidth * 0.3, dist);

    return mix(bgColor, fgColor, alpha);
}
