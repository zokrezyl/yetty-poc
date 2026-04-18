// Post-effect: pixelate
// Index: 7
//
// Reduces apparent resolution by quantizing pixel positions.
// Since single-pass, approximates by blending toward block average color.
// params[0] = block size in pixels (default 4.0)
// params[1] = color reduction levels (default 8.0, 0=no reduction)

fn postEffect_pixelate(
    color: vec3<f32>,
    pixelPos: vec2<f32>,
    screenSize: vec2<f32>,
    time: f32,
    params: array<f32, 6>
) -> vec3<f32> {
    let blockSize = select(4.0, params[0], params[0] > 0.0);
    let levels = select(8.0, params[1], params[1] > 0.0);

    // Quantize position within block — shift color based on block center distance
    let blockPos = floor(pixelPos / blockSize) * blockSize + blockSize * 0.5;
    let dist = length(pixelPos - blockPos) / (blockSize * 0.707);

    // Flatten color variation within block
    var result = color * (1.0 - dist * 0.3);

    // Optional color quantization (posterize)
    if (levels > 0.0) {
        result = floor(result * levels + 0.5) / levels;
    }

    return clamp(result, vec3<f32>(0.0), vec3<f32>(1.0));
}
