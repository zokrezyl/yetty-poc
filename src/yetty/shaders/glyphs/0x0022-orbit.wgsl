// Shader glyph: Orbit (codepoint 1052706 / U+101022)
// A small moon orbiting a larger central planet

fn shaderGlyph_1052706(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let center = vec2<f32>(0.5, 0.5);
    let planetRadius = 0.12;
    let moonRadius = 0.06;
    let orbitRadius = 0.32;
    let orbitSpeed = 1.8;

    // Orbit path (faint ellipse)
    let orbitRX = orbitRadius;
    let orbitRY = orbitRadius * 0.4;

    let angle = time * orbitSpeed;
    let moonPos = center + vec2<f32>(cos(angle) * orbitRX, sin(angle) * orbitRY);

    // Depth: moon behind planet when sin(angle) > 0
    let moonBehind = sin(angle) > 0.0;

    // Faint orbit path
    let orbitDist = length(vec2<f32>((localUV.x - center.x) / orbitRX, (localUV.y - center.y) / orbitRY));
    let orbitLine = smoothstep(0.04, 0.0, abs(orbitDist - 1.0)) * 0.15;

    // Planet
    let planetDist = length(localUV - center);
    let planetAlpha = smoothstep(planetRadius, planetRadius * 0.5, planetDist);

    // Moon
    let moonDist = length(localUV - moonPos);
    let moonAlpha = smoothstep(moonRadius, moonRadius * 0.3, moonDist);

    var alpha: f32;
    if (moonBehind) {
        // Moon behind: draw moon first, planet on top
        alpha = max(orbitLine, max(moonAlpha * 0.6, planetAlpha));
    } else {
        alpha = max(orbitLine, max(planetAlpha, moonAlpha));
    }

    return mix(bgColor, fgColor, alpha);
}
