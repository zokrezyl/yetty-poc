// Shader glyph: Worm (codepoint 1052696 / U+101018)
// A worm of 6 dots wandering randomly, bouncing off cell borders

fn worm_hash(n: f32) -> f32 {
    return fract(sin(n * 127.1) * 43758.5453);
}

fn worm_simulate(time: f32) -> vec2<f32> {
    let margin = 0.12;
    let lo = margin;
    let hi = 1.0 - margin;
    let speed = 0.4;

    var pos = vec2<f32>(0.5, 0.5);
    var bounceCount = 0u;
    var angle = worm_hash(0.0) * 6.28318;
    var dir = normalize(vec2<f32>(cos(angle), sin(angle)));
    var remaining = time * speed;

    for (var i = 0u; i < 80u; i++) {
        if (remaining <= 0.001) {
            break;
        }

        // Time to each wall
        var tWall = remaining;
        if (dir.x > 0.001) {
            tWall = min(tWall, (hi - pos.x) / dir.x);
        } else if (dir.x < -0.001) {
            tWall = min(tWall, (lo - pos.x) / dir.x);
        }
        if (dir.y > 0.001) {
            tWall = min(tWall, (hi - pos.y) / dir.y);
        } else if (dir.y < -0.001) {
            tWall = min(tWall, (lo - pos.y) / dir.y);
        }
        tWall = max(tWall, 0.0);

        if (tWall >= remaining) {
            pos += dir * remaining;
            break;
        }

        pos += dir * tWall;
        remaining -= tWall;
        bounceCount += 1u;
        pos = clamp(pos, vec2<f32>(lo), vec2<f32>(hi));

        // New random direction, forced inward at walls
        angle = worm_hash(f32(bounceCount)) * 6.28318;
        dir = vec2<f32>(cos(angle), sin(angle));
        let eps = 0.002;
        if (pos.x <= lo + eps) { dir.x = abs(dir.x); }
        if (pos.x >= hi - eps) { dir.x = -abs(dir.x); }
        if (pos.y <= lo + eps) { dir.y = abs(dir.y); }
        if (pos.y >= hi - eps) { dir.y = -abs(dir.y); }
        dir = normalize(dir);
    }

    return clamp(pos, vec2<f32>(lo), vec2<f32>(hi));
}

fn shaderGlyph_1052696(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    // Wrap time so bounce simulation stays bounded
    let cycleTime = 30.0;
    let t = time - floor(time / cycleTime) * cycleTime;

    let segmentDelay = 0.18;
    let headRadius = 0.09;

    var alpha = 0.0;

    // 6 segments: head is biggest, tail tapers
    for (var i = 0; i < 6; i++) {
        let segTime = max(t - f32(i) * segmentDelay, 0.0);
        let pos = worm_simulate(segTime);
        let radius = headRadius - f32(i) * 0.007;
        let d = length(localUV - pos);
        alpha = max(alpha, smoothstep(radius, radius * 0.3, d));
    }

    return mix(bgColor, fgColor, alpha);
}
