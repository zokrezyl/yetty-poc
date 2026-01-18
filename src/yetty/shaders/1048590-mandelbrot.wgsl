// Shader glyph: Mandelbrot fractal (codepoint 1048590 / U+10000E)
// Tiled fractal - uses pixelPos to create seamless rendering across cells
// Zooms in and out based on time

fn shaderGlyph_1048590(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    // Normalize pixel position to screen UV (0-1)
    let screenUV = pixelPos / vec2<f32>(globals.screenWidth, globals.screenHeight);

    // Center point for zoom (interesting area of Mandelbrot)
    let centerX = -0.745;
    let centerY = 0.186;

    // Zoom oscillates between close-up and overview
    let zoomBase = 2.0;
    let zoomRange = 1.8;
    let zoomSpeed = 0.3;
    let zoom = zoomBase + zoomRange * sin(time * zoomSpeed);

    // Map screen UV to complex plane coordinates
    // Aspect ratio correction
    let aspect = globals.screenWidth / globals.screenHeight;
    var c = vec2<f32>(
        (screenUV.x - 0.5) * zoom * aspect + centerX,
        (screenUV.y - 0.5) * zoom + centerY
    );

    // Mandelbrot iteration: z = z² + c, starting at z = 0
    var z = vec2<f32>(0.0, 0.0);
    var iterations = 0u;
    let maxIterations = 100u;

    for (var i = 0u; i < maxIterations; i++) {
        // z² = (a + bi)² = a² - b² + 2abi
        let zNew = vec2<f32>(
            z.x * z.x - z.y * z.y + c.x,
            2.0 * z.x * z.y + c.y
        );
        z = zNew;

        // Escape condition
        if (dot(z, z) > 4.0) {
            break;
        }
        iterations++;
    }

    // Color based on iteration count
    if (iterations == maxIterations) {
        // Inside the set - use background or dark color
        return bgColor * 0.2;
    }

    // Smooth coloring for outside
    let smooth_iter = f32(iterations) - log2(log2(dot(z, z)));
    let t = smooth_iter / f32(maxIterations);

    // Create colorful gradient
    let r = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0);
    let g = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0 + 2.094);
    let b = 0.5 + 0.5 * sin(3.0 + t * 6.28318 * 2.0 + 4.188);

    return vec3<f32>(r, g, b);
}
