// Post-effect: rain
// Index: 15
//
// Falling rain streaks overlay on the terminal.
// params[0] = density (0.0-1.0, default 0.3)
// params[1] = speed (default 3.0)
// params[2] = streak length (default 0.15)

fn postEffect_rain(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let density = select(0.3, params[0], params[0] > 0.0);
    let speed = select(3.0, params[1], params[1] > 0.0);
    let streakLen = select(0.15, params[2], params[2] > 0.0);

    let uv = pixelPos / screenSize;
    var result = color;

    // Multiple rain layers for depth
    for (var layer = 0u; layer < 3u; layer++) {
        let layerF = f32(layer);
        let layerScale = 1.0 + layerF * 0.5;
        let layerSpeed = speed * (1.0 + layerF * 0.3);
        let layerAlpha = (0.3 - layerF * 0.08) * density;

        // Column hash determines which columns have rain
        let col = floor(uv.x * screenSize.x / (3.0 * layerScale));
        let colHash = fract(sin(col * 127.1 + layerF * 311.7) * 43758.5453);

        if (colHash < density) {
            // Raindrop position (falling)
            let dropSpeed = layerSpeed * (0.7 + colHash * 0.6);
            let dropY = fract(-uv.y + time * dropSpeed * 0.1 + colHash * 10.0);

            // Streak shape
            if (dropY < streakLen) {
                let streakFade = 1.0 - dropY / streakLen;
                let brightness = streakFade * layerAlpha;
                result += vec3<f32>(0.4, 0.5, 0.7) * brightness;
            }
        }
    }

    // Slight blue-grey tint for rainy atmosphere
    result = mix(result, result * vec3<f32>(0.85, 0.9, 1.0), density * 0.3);

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
