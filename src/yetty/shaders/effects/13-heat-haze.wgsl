// Effect: heat-haze
// Index: 13
//
// Shimmering distortion like hot air over asphalt
// params: P0 = strength (default 0.2), P1 = frequency (default 20.0), P2 = speed (default 3.0)

fn effect_heat_haze(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.2, grid.effectP0, grid.effectP0 != 0.0);
    let freq = select(20.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(3.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;

    // Heat rises from bottom - more distortion at top
    let heightFactor = 1.0 - pixelPos.y / gridPixelHeight;

    // High frequency shimmer
    let shimmerX = sin(pixelPos.y * freq * 0.1 + t) *
                   cos(pixelPos.y * freq * 0.07 + t * 1.3) *
                   strength * 8.0 * heightFactor;

    let shimmerY = sin(pixelPos.x * freq * 0.05 + t * 0.8) *
                   strength * 3.0 * heightFactor;

    return pixelPos + vec2<f32>(shimmerX, shimmerY);
}
