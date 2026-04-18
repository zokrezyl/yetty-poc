// Effect: melt
// Index: 18
//
// Content dripping/melting downward
// params: P0 = strength (default 0.5), P1 = drip_frequency (default 8.0), P2 = speed (default 1.0)

fn effect_melt(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 != 0.0);
    let dripFreq = select(8.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(1.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;
    let col = pixelPos.x / gridPixelWidth;

    // Each column drips at different rate
    let colSeed = floor(col * dripFreq);
    let dripPhase = util_hash11(colSeed) * 6.28;
    let dripSpeed = 0.5 + util_hash11(colSeed + 50.0) * 1.0;

    // Drip amount increases toward bottom
    let heightFactor = pixelPos.y / gridPixelHeight;
    let drip = sin(t * dripSpeed + dripPhase) * 0.5 + 0.5;
    let meltOffset = drip * strength * 40.0 * heightFactor;

    // Add some horizontal wobble
    let wobble = sin(pixelPos.y * 0.1 + t * 2.0) * strength * 5.0 * heightFactor;

    return vec2<f32>(pixelPos.x + wobble, pixelPos.y - meltOffset);
}
