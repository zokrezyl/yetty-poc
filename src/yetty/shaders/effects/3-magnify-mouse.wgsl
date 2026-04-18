// Effect: magnify-mouse
// Index: 3
//
// Magnifies area around the mouse cursor position
// params: P0 = strength (default 0.5), P1 = radius in pixels (default 100.0)
//
// Accesses uniforms directly: globals.mouseX, globals.mouseY

fn effect_magnify_mouse(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 > 0.0);
    let radius = select(100.0, grid.effectP1, grid.effectP1 > 0.0);

    // Mouse center in pixels (relative to viewport)
    let center = vec2<f32>(globals.mouseX, globals.mouseY) - grid.viewportOrigin;

    // Distance from mouse
    let delta = pixelPos - center;
    let dist = length(delta);

    if (dist >= radius || dist < 0.001) {
        return pixelPos;
    }

    // Smooth magnification falloff (stronger near center)
    let t = dist / radius;  // 0 at center, 1 at edge
    let falloff = 1.0 - t * t;  // Quadratic falloff

    // Magnification: push pixels away from center (makes center appear larger)
    // factor < 1 means we sample from closer to center = magnification
    let factor = 1.0 - strength * falloff;

    let distorted = center + delta * factor;
    return distorted;
}
