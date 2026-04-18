// Effect: wandering-wart
// Index: 5
//
// Single pulsing magnification that wanders in pseudo-random directions
// params: P0 = strength (default 0.5), P1 = radius (default 120), P2 = speed (default 1.0)
//
// Accesses uniforms directly: globals.time, grid.*

fn effect_wandering_wart(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 > 0.0);
    let baseRadius = select(120.0, grid.effectP1, grid.effectP1 > 0.0);
    let speed = select(1.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    // Wandering position using multiple sine waves with different frequencies
    // Creates smooth, organic, pseudo-random movement
    let t = globals.time * speed;

    let wanderX = 0.5 + 0.3 * sin(t * 0.7) + 0.15 * sin(t * 1.3 + 2.0) + 0.05 * sin(t * 2.9 + 5.0);
    let wanderY = 0.5 + 0.3 * sin(t * 0.5 + 1.0) + 0.15 * sin(t * 1.1 + 3.0) + 0.05 * sin(t * 2.3 + 4.0);

    let centerX = wanderX * gridPixelWidth;
    let centerY = wanderY * gridPixelHeight;
    let center = vec2<f32>(centerX, centerY);

    // Pulsing radius
    let pulse = 0.8 + 0.2 * sin(t * 2.5);
    let radius = baseRadius * pulse;

    // Distance from wart
    let delta = pixelPos - center;
    let dist = length(delta);

    if (dist >= radius || dist < 0.001) {
        return pixelPos;
    }

    let ratio = dist / radius;
    let falloff = 1.0 - ratio * ratio;

    // Pulsing strength
    let pulseStrength = strength * (0.7 + 0.3 * sin(t * 3.0));
    let factor = 1.0 - pulseStrength * falloff;

    let distorted = center + delta * factor;
    return distorted;
}
