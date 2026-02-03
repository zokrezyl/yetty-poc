// Effect: swirl
// Index: 7
//
// Spiral vortex distortion around center
// params: P0 = strength (default 1.0), P1 = radius (default 200), P2 = animate speed (default 0, 0=static)

fn effect_swirl(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(1.0, grid.effectP0, grid.effectP0 != 0.0);
    let radius = select(200.0, grid.effectP1, grid.effectP1 > 0.0);
    let animSpeed = grid.effectP2;

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    let delta = pixelPos - center;
    let dist = length(delta);

    if (dist >= radius) {
        return pixelPos;
    }

    // Angle increases toward center
    let t = 1.0 - dist / radius;
    let angle = t * t * strength * 3.14159 + animSpeed * globals.time;

    // Rotate
    let cosA = cos(angle);
    let sinA = sin(angle);
    let rotated = vec2<f32>(
        delta.x * cosA - delta.y * sinA,
        delta.x * sinA + delta.y * cosA
    );

    return center + rotated;
}
