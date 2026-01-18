// Shader glyph: Julia set fractal (codepoint 1048591 / U+10000F)
// Tiled fractal - uses pixelPos to create seamless rendering across cells
// The Julia constant animates over time for morphing effect

fn shaderGlyph_1048591(localUV: vec2<f32>, time: f32, fgColor: vec3<f32>, bgColor: vec3<f32>, pixelPos: vec2<f32>) -> vec3<f32> {
    // Normalize pixel position to screen UV (0-1)
    let screenUV = pixelPos / vec2<f32>(globals.screenWidth, globals.screenHeight);

    // Julia set constant - animate it for morphing effect
    // These values trace an interesting path in the parameter space
    let cReal = -0.7 + 0.3 * sin(time * 0.2);
    let cImag = 0.27015 + 0.2 * cos(time * 0.15);
    let c = vec2<f32>(cReal, cImag);

    // Zoom oscillates
    let zoomBase = 2.5;
    let zoomRange = 1.0;
    let zoomSpeed = 0.25;
    let zoom = zoomBase + zoomRange * sin(time * zoomSpeed);

    // Map screen UV to complex plane coordinates
    let aspect = globals.screenWidth / globals.screenHeight;
    var z = vec2<f32>(
        (screenUV.x - 0.5) * zoom * aspect,
        (screenUV.y - 0.5) * zoom
    );

    // Julia iteration: z = z² + c, starting at z = pixel coordinate
    var iterations = 0u;
    let maxIterations = 80u;

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
        // Inside the set
        return bgColor * 0.1;
    }

    // Smooth coloring
    let smooth_iter = f32(iterations) - log2(log2(dot(z, z)));
    let t = smooth_iter / f32(maxIterations);

    // Different color scheme from Mandelbrot - warmer tones
    let r = 0.5 + 0.5 * sin(t * 6.28318 * 3.0);
    let g = 0.5 + 0.5 * sin(t * 6.28318 * 3.0 + 1.0);
    let b = 0.5 + 0.5 * sin(t * 6.28318 * 3.0 + 2.5);

    return vec3<f32>(r, g, b);
}
