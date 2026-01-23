// Shader glyph: Mandelbrot fractal (codepoint 1052685 / U+10100D)
// Tiled fractal - uses pixelPos to create seamless rendering across cells
// Mouse position shifts the view center

fn shaderGlyph_1052685(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // fg/bg unused - fractal has its own colors
    // Normalize coordinates to 0-1
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    let screenUV = pixelPos / screenSize;
    let mouseUV = clamp(mousePos / screenSize, vec2<f32>(0.0), vec2<f32>(1.0));

    // Base center in Mandelbrot space (classic view)
    let baseCenterX = -0.5;
    let baseCenterY = 0.0;

    // Mouse shifts the center slightly (range: -0.5 to 0.5)
    let centerX = baseCenterX + (mouseUV.x - 0.5) * 1.0;
    let centerY = baseCenterY + (mouseUV.y - 0.5) * 1.0;

    // Zoom based on time - oscillates between overview and detail
    let zoomLevel = 2.0 + 1.5 * sin(time * 0.2);

    // Map screen UV to complex plane coordinates
    let aspect = screenSize.x / screenSize.y;
    var c = vec2<f32>(
        (screenUV.x - 0.5) * zoomLevel * aspect + centerX,
        (screenUV.y - 0.5) * zoomLevel + centerY
    );

    // Mandelbrot iteration
    var z = vec2<f32>(0.0, 0.0);
    var iterations = 0u;
    let maxIterations = 100u;

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
        return vec3<f32>(0.0, 0.0, 0.1);
    }

    // Smooth coloring
    let smooth_iter = f32(iterations) - log2(log2(dot(z, z)));
    let t = smooth_iter / f32(maxIterations);

    // Colorful gradient
    let r = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0);
    let g = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0 + 2.094);
    let b = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0 + 4.188);

    return vec3<f32>(r, g, b);
}
