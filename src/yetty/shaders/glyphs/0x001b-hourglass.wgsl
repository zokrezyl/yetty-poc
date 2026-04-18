// Shader glyph: Hourglass (codepoint 1052699 / U+10101B)
// Sand falling through a narrow center, then a visible 180° flip rotation

fn hourglass_hash(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453);
}

fn hourglass_draw(uv_in: vec2<f32>, progress: f32, fgColor: vec3<f32>, bgColor: vec3<f32>) -> vec3<f32> {
    let uv = uv_in;
    let cx = 0.5;
    let waistY = 0.5;
    let topWidth = 0.35;
    let waistWidth = 0.04;
    let dy = abs(uv.y - waistY);
    let width = mix(waistWidth, topWidth, smoothstep(0.0, 0.4, dy));

    // Glass outline
    let distToEdge = abs(abs(uv.x - cx) - width);
    let outline = smoothstep(0.02, 0.005, distToEdge) *
                  step(0.08, dy + 0.02);

    // Top and bottom caps
    let topCap = smoothstep(0.02, 0.005, abs(uv.y - 0.1)) *
                 step(abs(uv.x - cx), topWidth + 0.01);
    let botCap = smoothstep(0.02, 0.005, abs(uv.y - 0.9)) *
                 step(abs(uv.x - cx), topWidth + 0.01);

    // Sand in top half: level drops with progress
    let topSandLevel = mix(0.15, 0.5, progress);
    let topSandWidth = mix(waistWidth, topWidth, smoothstep(0.0, 0.4, abs(uv.y - waistY)));
    let topSand = step(uv.y, waistY) *
                  step(topSandLevel, uv.y) *
                  step(abs(uv.x - cx), topSandWidth - 0.02);

    // Sand in bottom half: pile grows with progress
    let botSandLevel = mix(0.9, 0.5, progress);
    let botSandWidth = mix(waistWidth, topWidth, smoothstep(0.0, 0.4, abs(uv.y - waistY)));
    let botSand = step(waistY, uv.y) *
                  step(botSandLevel, uv.y) *
                  step(abs(uv.x - cx), botSandWidth - 0.02);

    // Falling stream through waist
    let stream = step(abs(uv.x - cx), 0.015) *
                 step(topSandLevel - 0.02, uv.y) *
                 step(uv.y, botSandLevel + 0.02) *
                 step(progress, 0.98);

    let glass = max(outline, max(topCap, botCap)) * 0.6;
    let sand = max(topSand, max(botSand, stream));
    let alpha = max(glass, sand);

    return mix(bgColor, fgColor, alpha);
}

fn shaderGlyph_1052699(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let sandTime = 3.0;
    let flipTime = 0.8;
    let halfCycle = sandTime + flipTime;
    let totalCycle = halfCycle * 2.0;
    let rawT = time - floor(time / totalCycle) * totalCycle;

    // Which half are we in?
    let inSecondHalf = rawT >= halfCycle;
    let halfT = select(rawT, rawT - halfCycle, inSecondHalf);

    // Within each half: sand phase then flip phase
    let inFlip = halfT >= sandTime;
    let progress = select(halfT / sandTime, 1.0, inFlip);

    if (inFlip) {
        // During flip: rotate the whole hourglass with frozen sand state
        let ft = (halfT - sandTime) / flipTime;
        let eased = ft * ft * (3.0 - 2.0 * ft); // smoothstep
        let baseAngle = select(0.0, 3.14159, inSecondHalf);
        let angle = baseAngle + eased * 3.14159;

        let center = vec2<f32>(0.5, 0.5);
        let rel = localUV - center;
        let ca = cos(angle);
        let sa = sin(angle);
        let rotated = vec2<f32>(rel.x * ca - rel.y * sa, rel.x * sa + rel.y * ca) + center;

        // First flip: sand is at bottom (progress=1). Second flip: sand is at top (progress=0).
        let frozenProgress = select(1.0, 0.0, inSecondHalf);
        return hourglass_draw(rotated, frozenProgress, fgColor, bgColor);
    }

    // Sand phase: no rotation, sand always drains top -> bottom in screen space
    // The glass shape is vertically symmetric so it looks the same either way
    return hourglass_draw(localUV, progress, fgColor, bgColor);
}
