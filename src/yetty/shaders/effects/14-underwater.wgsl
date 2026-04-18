// Effect: underwater
// Index: 14
//
// Wavy caustics ripple like underwater view
// params: P0 = strength (default 0.3), P1 = frequency (default 4.0), P2 = speed (default 1.5)

fn effect_underwater(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.3, grid.effectP0, grid.effectP0 != 0.0);
    let freq = select(4.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(1.5, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;
    let uv = pixelPos / vec2<f32>(gridPixelWidth, gridPixelHeight);

    // Layered caustic-like waves
    let wave1 = sin(uv.x * freq * 6.28 + t) * cos(uv.y * freq * 4.0 + t * 0.7);
    let wave2 = sin(uv.y * freq * 5.0 - t * 1.1) * cos(uv.x * freq * 3.0 + t * 0.5);
    let wave3 = sin((uv.x + uv.y) * freq * 4.0 + t * 0.8);

    let distortX = (wave1 + wave2 * 0.5) * strength * 20.0;
    let distortY = (wave2 + wave3 * 0.5) * strength * 20.0;

    return pixelPos + vec2<f32>(distortX, distortY);
}
