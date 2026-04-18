// Effect: wave
// Index: 19
//
// Horizontal or vertical sine wave distortion
// params: P0 = strength (default 0.4), P1 = frequency (default 3.0), P2 = speed (default 2.0), P3 = direction (0=horizontal, 1=vertical)

fn effect_wave(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.4, grid.effectP0, grid.effectP0 != 0.0);
    let freq = select(3.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(2.0, grid.effectP2, grid.effectP2 > 0.0);
    let vertical = grid.effectP3 > 0.5;

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;

    if (vertical) {
        // Vertical wave - displaces X based on Y
        let wave = sin(pixelPos.y / gridPixelHeight * freq * 6.28 + t) * strength * 30.0;
        return vec2<f32>(pixelPos.x + wave, pixelPos.y);
    } else {
        // Horizontal wave - displaces Y based on X
        let wave = sin(pixelPos.x / gridPixelWidth * freq * 6.28 + t) * strength * 30.0;
        return vec2<f32>(pixelPos.x, pixelPos.y + wave);
    }
}
