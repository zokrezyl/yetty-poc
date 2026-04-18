// Effect: vhs-tear
// Index: 17
//
// Horizontal tear like VHS tracking error
// params: P0 = strength (default 0.6), P1 = tear_height (default 50), P2 = speed (default 2.0)

fn effect_vhs_tear(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.6, grid.effectP0, grid.effectP0 != 0.0);
    let tearHeight = select(50.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(2.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;

    // Tear position moves up and down
    let tearY = (sin(t * 0.7) * 0.5 + 0.5) * gridPixelHeight;

    // Distance from tear center
    let distFromTear = abs(pixelPos.y - tearY);

    if (distFromTear > tearHeight) {
        return pixelPos;
    }

    // Tear intensity
    let tearFactor = 1.0 - distFromTear / tearHeight;
    let offset = sin(t * 10.0 + pixelPos.y * 0.1) * strength * 50.0 * tearFactor;

    // Add some noise
    let noise = (util_hash11(pixelPos.y + t) * 2.0 - 1.0) * 10.0 * tearFactor * strength;

    return vec2<f32>(pixelPos.x + offset + noise, pixelPos.y);
}
