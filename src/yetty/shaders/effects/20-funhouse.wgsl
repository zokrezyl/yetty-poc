// Effect: funhouse
// Index: 20
//
// Funhouse mirror with multiple bulges
// params: P0 = strength (default 0.4), P1 = bulge_count_x (default 3), P2 = bulge_count_y (default 2)

fn effect_funhouse(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.4, grid.effectP0, grid.effectP0 != 0.0);
    let countX = select(3.0, grid.effectP1, grid.effectP1 > 0.0);
    let countY = select(2.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    // Normalize position
    let uv = pixelPos / vec2<f32>(gridPixelWidth, gridPixelHeight);

    // Create grid of bulges using sine
    let bulgeX = sin(uv.x * countX * 6.28) * sin(uv.y * countY * 6.28);
    let bulgeY = cos(uv.x * countX * 6.28) * cos(uv.y * countY * 6.28);

    // Animate slightly
    let t = globals.time * 0.5;
    let animBulgeX = bulgeX * cos(t) + bulgeY * sin(t) * 0.3;
    let animBulgeY = bulgeY * cos(t) - bulgeX * sin(t) * 0.3;

    let offset = vec2<f32>(animBulgeX, animBulgeY) * strength * 30.0;

    return pixelPos + offset;
}
