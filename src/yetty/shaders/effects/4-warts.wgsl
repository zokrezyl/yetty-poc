// Effect: warts
// Index: 4
//
// Pulsing magnification "warts" at random positions
// params: P0 = strength (default 0.4), P1 = wart count (default 5), P2 = radius (default 80)
//
// Uses: util_hash11 from lib/util.wgsl

fn effect_warts(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.4, grid.effectP0, grid.effectP0 > 0.0);
    let wartCount = i32(select(5.0, grid.effectP1, grid.effectP1 > 0.0));
    let baseRadius = select(80.0, grid.effectP2, grid.effectP2 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    var distortedPos = pixelPos;

    // Apply multiple warts
    for (var i = 0; i < wartCount; i++) {
        let seed = f32(i) * 17.31;

        // Random position (changes slowly over time)
        let timeSlot = floor(globals.time * 0.3 + seed);
        let posX = util_hash11(timeSlot + seed) * gridPixelWidth;
        let posY = util_hash11(timeSlot + seed + 100.0) * gridPixelHeight;
        let center = vec2<f32>(posX, posY);

        // Pulsing radius
        let pulse = 0.7 + 0.3 * sin(globals.time * 2.0 + seed * 3.0);
        let radius = baseRadius * pulse;

        // Distance from this wart
        let delta = distortedPos - center;
        let dist = length(delta);

        if (dist < radius && dist > 0.001) {
            let t = dist / radius;
            let falloff = 1.0 - t * t;

            // Pulsing strength
            let pulseStrength = strength * (0.5 + 0.5 * sin(globals.time * 3.0 + seed * 5.0));
            let factor = 1.0 - pulseStrength * falloff;

            distortedPos = center + (distortedPos - center) * factor;
        }
    }

    return distortedPos;
}
