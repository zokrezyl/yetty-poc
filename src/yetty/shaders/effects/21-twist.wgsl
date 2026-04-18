// Effect: twist
// Index: 21
//
// Rotational distortion increasing toward edges
// params: P0 = strength (default 0.5), P1 = animate speed (default 0, 0=static)

fn effect_twist(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 != 0.0);
    let animSpeed = grid.effectP1;

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    let delta = pixelPos - center;
    let dist = length(delta);
    let maxDist = length(center);

    // Twist angle increases with distance from center
    let normalizedDist = dist / maxDist;
    let angle = normalizedDist * normalizedDist * strength * 3.14159 + animSpeed * globals.time;

    // Rotate
    let cosA = cos(angle);
    let sinA = sin(angle);
    let rotated = vec2<f32>(
        delta.x * cosA - delta.y * sinA,
        delta.x * sinA + delta.y * cosA
    );

    return center + rotated;
}
