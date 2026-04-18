// Effect: magnify-cursor
// Index: 2
//
// Magnifies area around the text cursor position
// params: P0 = strength (default 0.5), P1 = radius in cells (default 5.0)
//
// Accesses uniforms directly: grid.cursorPos, grid.cellSize

fn effect_magnify_cursor(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 > 0.0);
    let radiusCells = select(5.0, grid.effectP1, grid.effectP1 > 0.0);

    // Cursor center in pixels
    let cursorCenterX = (grid.cursorPos.x + 0.5) * grid.cellSize.x;
    let cursorCenterY = (grid.cursorPos.y + 0.5) * grid.cellSize.y;
    let center = vec2<f32>(cursorCenterX, cursorCenterY);

    // Radius in pixels
    let radius = radiusCells * max(grid.cellSize.x, grid.cellSize.y);

    // Distance from cursor
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
