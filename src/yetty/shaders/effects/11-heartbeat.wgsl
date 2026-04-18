// Effect: heartbeat
// Index: 11
//
// Rhythmic pulse from center with lub-dub heartbeat timing
// params: P0 = strength (default 0.3), P1 = bpm (default 72)

fn effect_heartbeat(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.3, grid.effectP0, grid.effectP0 != 0.0);
    let bpm = select(72.0, grid.effectP1, grid.effectP1 > 0.0);

    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;
    let center = vec2<f32>(gridPixelWidth * 0.5, gridPixelHeight * 0.5);

    // Heartbeat timing: lub-dub pattern
    let beatPeriod = 60.0 / bpm;
    let t = (globals.time % beatPeriod) / beatPeriod;

    // Two pulses per beat (lub at 0, dub at 0.2)
    let lub = exp(-pow((t - 0.0) * 10.0, 2.0));
    let dub = exp(-pow((t - 0.2) * 12.0, 2.0)) * 0.7;
    let pulse = (lub + dub) * strength;

    let delta = pixelPos - center;
    let dist = length(delta);
    let maxDist = length(center);

    // Pulse radiates outward
    let factor = 1.0 - pulse * (1.0 - dist / maxDist);

    return center + delta * factor;
}
