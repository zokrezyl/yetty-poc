// Effect: scanline-offset
// Index: 16
//
// Each row shifted randomly like bad TV signal
// params: P0 = strength (default 0.5), P1 = speed (default 5.0)

fn effect_scanline_offset(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 != 0.0);
    let speed = select(5.0, grid.effectP1, grid.effectP1 > 0.0);

    let row = floor(pixelPos.y / grid.cellSize.y);
    let t = globals.time * speed;

    // Random offset per row, changing over time
    let rowSeed = row * 17.31 + floor(t);
    let offset = (util_hash11(rowSeed) * 2.0 - 1.0) * strength * 30.0;

    // Some rows shift more than others
    let rowIntensity = util_hash11(row * 7.13);
    let finalOffset = offset * rowIntensity;

    return vec2<f32>(pixelPos.x + finalOffset, pixelPos.y);
}
