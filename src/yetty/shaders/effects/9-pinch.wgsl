// Effect: pinch
// Index: 9
//
// Pinch distortion - pulls content toward center
// params: P0 = strength (default 0.5), P1 = radius (default 150)

fn effect_pinch(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 != 0.0);
    let radius = select(150.0, grid.effectP1, grid.effectP1 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    let delta = pixelPos - center;
    let dist = length(delta);

    if (dist >= radius || dist < 0.001) {
        return pixelPos;
    }

    // Inverse of bulge - pull toward center
    let t = dist / radius;
    let pinch = 1.0 - cos(t * 3.14159 * 0.5);
    let factor = 1.0 + strength * pinch;

    return center + delta * factor;
}
