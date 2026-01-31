// Shader glyph: Rain (codepoint 1052705 / U+101021)
// Small dots falling at different speeds and positions, wrapping around

fn rain_hash(n: f32) -> f32 {
    return fract(sin(n * 127.1) * 43758.5453);
}

fn shaderGlyph_1052705(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let numDrops = 8;
    let dotRadius = 0.04;

    var alpha = 0.0;

    for (var i = 0; i < 8; i++) {
        let fi = f32(i);
        let x = rain_hash(fi) * 0.8 + 0.1;
        let speed = 0.3 + rain_hash(fi + 50.0) * 0.5;
        let phase = rain_hash(fi + 100.0);

        var y = phase + time * speed;
        y = y - floor(y); // wrap [0,1)

        let pos = vec2<f32>(x, y);
        let d = length(localUV - pos);

        // Slight elongation: stretch the dot vertically for raindrop feel
        let dy = abs(localUV.y - y);
        let dx = abs(localUV.x - x);
        let elongated = length(vec2<f32>(dx, dy * 0.6));
        let dropAlpha = smoothstep(dotRadius, dotRadius * 0.2, elongated);

        // Fade near top/bottom for seamless wrap
        let fade = smoothstep(0.0, 0.08, y) * smoothstep(1.0, 0.92, y);
        alpha = max(alpha, dropAlpha * fade);
    }

    return mix(bgColor, fgColor, alpha);
}
