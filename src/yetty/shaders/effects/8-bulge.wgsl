// Effect: bulge
// Index: 8
//
// Single magnifying bulge in center
// params: P0 = strength (default 0.7), P1 = radius fraction (default 0.4 = 40% of screen)

fn effect_bulge(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.7, grid.effectP0, grid.effectP0 != 0.0);
    let radiusFrac = select(0.4, grid.effectP1, grid.effectP1 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    // Radius as fraction of smaller screen dimension
    let radius = min(gridPixelWidth, gridPixelHeight) * radiusFrac;

    let delta = pixelPos - center;
    let dist = length(delta);

    if (dist >= radius || dist < 0.001) {
        return pixelPos;
    }

    // Spherical bulge - magnifies center
    let t = dist / radius;
    let bulge = sin(t * 3.14159 * 0.5);
    let factor = 1.0 - strength * (1.0 - bulge);

    return center + delta * factor;
}
