// Effect: earthquake
// Index: 15
//
// Random shake/jitter
// params: P0 = strength (default 0.5), P1 = frequency (default 30.0)

fn effect_earthquake(pixelPos: vec2<f32>) -> vec2<f32> {
    let strength = select(0.5, grid.effectP0, grid.effectP0 != 0.0);
    let freq = select(30.0, grid.effectP1, grid.effectP1 > 0.0);

    let t = globals.time * freq;

    // Use hash for pseudo-random shake
    let shakeX = (util_hash11(floor(t)) * 2.0 - 1.0) * strength * 15.0;
    let shakeY = (util_hash11(floor(t) + 100.0) * 2.0 - 1.0) * strength * 15.0;

    // Smooth interpolation between shake positions
    let frac = fract(t);
    let smoothFactor = frac * frac * (3.0 - 2.0 * frac);

    let nextShakeX = (util_hash11(floor(t) + 1.0) * 2.0 - 1.0) * strength * 15.0;
    let nextShakeY = (util_hash11(floor(t) + 101.0) * 2.0 - 1.0) * strength * 15.0;

    let finalX = mix(shakeX, nextShakeX, smoothFactor);
    let finalY = mix(shakeY, nextShakeY, smoothFactor);

    return pixelPos + vec2<f32>(finalX, finalY);
}
