// Effect: barrel
// Index: 6
//
// Barrel/pincushion distortion like CRT monitor curvature
// params: P0 = strength (default 0.3, negative = pincushion)

fn effect_barrel(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.3, grid.effectP0, grid.effectP0 != 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    // Normalize to -1 to 1
    let uv = (pixelPos - center) / center;
    let r2 = dot(uv, uv);

    // Barrel distortion: r' = r * (1 + k * r^2)
    let distortion = 1.0 + strength * r2;
    let distorted_uv = uv * distortion;

    return distorted_uv * center + center;
}
