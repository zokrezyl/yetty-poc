// Shader glyph: Julia set fractal (codepoint 1052686 / U+10100E)
// Tiled fractal - uses pixelPos to create seamless rendering across cells
// Mouse position controls the Julia constant (c) - different positions = different fractals

fn shaderGlyph_1052686(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // fg/bg unused - fractal has its own colors
    // Normalize coordinates to 0-1
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    let screenUV = pixelPos / screenSize;
    let mouseUV = clamp(mousePos / screenSize, vec2<f32>(0.0), vec2<f32>(1.0));

    // Julia constant controlled by mouse position
    // Map mouse (0-1) to Julia parameter space (-0.8 to 0.8)
    let cReal = (mouseUV.x - 0.5) * 1.6;  // -0.8 to 0.8
    let cImag = (mouseUV.y - 0.5) * 1.6;  // -0.8 to 0.8
    let c = vec2<f32>(cReal, cImag);

    // Zoom oscillates based on time
    let zoomLevel = 2.5 + 0.5 * sin(time * 0.3);

    // Map screen UV to complex plane coordinates
    let aspect = screenSize.x / screenSize.y;
    var z = vec2<f32>(
        (screenUV.x - 0.5) * zoomLevel * aspect,
        (screenUV.y - 0.5) * zoomLevel
    );

    // Julia iteration
    var iterations = 0u;
    let maxIterations = 80u;

    for (var i = 0u; i < maxIterations; i++) {
        let zNew = vec2<f32>(
            z.x * z.x - z.y * z.y + c.x,
            2.0 * z.x * z.y + c.y
        );
        z = zNew;

        if (dot(z, z) > 4.0) {
            break;
        }
        iterations++;
    }

    // Inside the set - dark
    if (iterations == maxIterations) {
        return vec3<f32>(0.1, 0.0, 0.0);
    }

    // Smooth coloring
    let smooth_iter = f32(iterations) - log2(log2(dot(z, z)));
    let t = smooth_iter / f32(maxIterations);

    // Warmer color scheme (different from Mandelbrot)
    let r = 0.5 + 0.5 * sin(t * 6.28318 * 3.0);
    let g = 0.5 + 0.5 * sin(t * 6.28318 * 3.0 + 1.0);
    let b = 0.5 + 0.5 * sin(t * 6.28318 * 3.0 + 2.5);

    return vec3<f32>(r, g, b);
}
