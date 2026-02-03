// Effect: drunk
// Index: 12
//
// Slow swaying distortion like you're drunk
// params: P0 = strength (default 0.4), P1 = speed (default 0.5)

fn effect_drunk(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.4, grid.effectP0, grid.effectP0 != 0.0);
    let speed = select(0.5, grid.effectP1, grid.effectP1 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    let t = globals.time * speed;

    // Slow, overlapping waves for drunk sway
    let swayX = sin(t * 0.7) * 20.0 +
                sin(t * 1.1 + 1.0) * 15.0 +
                sin(t * 0.3 + 2.0) * 10.0;

    let swayY = cos(t * 0.5) * 15.0 +
                cos(t * 0.9 + 1.5) * 10.0 +
                cos(t * 0.2 + 3.0) * 8.0;

    // More sway at edges
    let edgeFactor = length(pixelPos / vec2<f32>(gridPixelWidth, gridPixelHeight) - 0.5) * 2.0;

    return pixelPos + vec2<f32>(swayX, swayY) * strength * (0.5 + edgeFactor * 0.5);
}
