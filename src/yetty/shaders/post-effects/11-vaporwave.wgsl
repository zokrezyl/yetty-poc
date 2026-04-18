// Post-effect: vaporwave
// Index: 11
//
// A E S T H E T I C — pink/cyan gradient overlay with chromatic shift.
// params[0] = intensity (0.0-1.0, default 0.4)
// params[1] = gradient speed (default 0.5)

fn postEffect_vaporwave(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let intensity = select(0.4, params[0], params[0] > 0.0);
    let speed = select(0.5, params[1], params[1] > 0.0);

    let uv = pixelPos / screenSize;

    // Animated gradient: pink top-left ↔ cyan bottom-right
    let gradPhase = sin(time * speed) * 0.5 + 0.5;
    let grad = mix(uv.x, uv.y, gradPhase);

    // Pink (#ff71ce) → Cyan (#01cdfe)
    let pink = vec3<f32>(1.0, 0.443, 0.808);
    let cyan = vec3<f32>(0.004, 0.804, 0.996);
    let tint = mix(pink, cyan, grad);

    // Blend tint over original
    var result = mix(color, color * tint * 1.5, intensity);

    // Horizontal scan line for retro feel
    let scan = 0.97 + 0.03 * sin(pixelPos.y * 1.5 + time * 2.0);
    result *= scan;

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
