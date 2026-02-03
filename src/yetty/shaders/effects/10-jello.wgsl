// Effect: jello
// Index: 10
//
// Wobbly jello-like animated distortion
// params: P0 = strength (default 0.3), P1 = frequency (default 3.0), P2 = speed (default 2.0)

fn effect_jello(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.3, grid.effectP0, grid.effectP0 != 0.0);
    let freq = select(3.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(2.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;

    // Multiple overlapping sine waves for organic wobble
    let wobbleX = sin(pixelPos.y / gridPixelHeight * freq * 6.28 + t) *
                  cos(pixelPos.x / gridPixelWidth * freq * 3.14 + t * 0.7) *
                  strength * 30.0;

    let wobbleY = cos(pixelPos.x / gridPixelWidth * freq * 6.28 + t * 1.3) *
                  sin(pixelPos.y / gridPixelHeight * freq * 3.14 + t * 0.9) *
                  strength * 30.0;

    return pixelPos + vec2<f32>(wobbleX, wobbleY);
}
