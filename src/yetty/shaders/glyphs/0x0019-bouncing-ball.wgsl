// Shader glyph: Bouncing Ball (codepoint 1052697 / U+101019)
// A ball falling with gravity, bouncing off the bottom with decreasing height

fn shaderGlyph_1052697(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let fgColor = unpackColor(fg);
    let bgColor = unpackColor(bg);

    let cycleDuration = 3.0;
    let t = time - floor(time / cycleDuration) * cycleDuration;

    // Simulate bouncing: each bounce loses energy
    let gravity = 4.0;
    let restitution = 0.65;
    let floor_y = 0.12;
    let startHeight = 0.85;
    var dropHeight = startHeight - floor_y;
    var elapsed = t;
    var y = startHeight;
    var vy = 0.0;

    // Step through bounces
    for (var i = 0; i < 8; i++) {
        // Time to fall from current height: h = 0.5*g*t^2 + vy*t
        // Using quadratic: 0.5*g*t^2 - vy*t - dropHeight = 0 (falling down)
        let fallTime = (vy + sqrt(vy * vy + 2.0 * gravity * dropHeight)) / gravity;

        if (elapsed < fallTime) {
            y = floor_y + dropHeight + vy * elapsed - 0.5 * gravity * elapsed * elapsed;
            break;
        }

        elapsed -= fallTime;
        vy = (vy + gravity * fallTime) * restitution;
        dropHeight = vy * vy / (2.0 * gravity);

        if (dropHeight < 0.005) {
            y = floor_y;
            break;
        }
    }

    // Horizontal drift
    let x = 0.3 + t * 0.12;

    let pos = vec2<f32>(x, 1.0 - y);
    let dotRadius = 0.09;
    let d = length(localUV - pos);
    let alpha = smoothstep(dotRadius, dotRadius * 0.3, d);

    return mix(bgColor, fgColor, alpha);
}
