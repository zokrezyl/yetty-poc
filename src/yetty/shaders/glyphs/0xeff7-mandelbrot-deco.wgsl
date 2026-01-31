// Shader glyph: Mandelbrot Pattern Decoration (codepoint 1114103 / U+10FFF7)
// Credit: Shane - https://www.shadertoy.com/view/ttscWn
// Converted from GLSL to WGSL
//
// Mandelbrot pattern with iterative derivative tracking, repeat circle grids,
// fake reflective highlights, and layered depth illusion.

fn mbdeco_mod_f(x: f32, y: f32) -> f32 {
    return x - y * floor(x / y);
}

fn mbdeco_mod_v2(x: vec2<f32>, y: f32) -> vec2<f32> {
    return x - y * floor(x / y);
}

fn mbdeco_rot(a: f32) -> mat2x2<f32> {
    let c = cos(a);
    let s = sin(a);
    return mat2x2<f32>(vec2<f32>(c, s), vec2<f32>(-s, c));
}

// Complex multiplication matrix: mat * w = z * w (complex product)
fn mbdeco_cmul(z: vec2<f32>) -> mat2x2<f32> {
    return mat2x2<f32>(z, vec2<f32>(-z.y, z.x));
}

fn shaderGlyph_1114103(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);

    var col = vec3<f32>(0.0);

    // 2x2 antialiasing
    for (var j = 0; j < 2; j++) {
        for (var i = 0; i < 2; i++) {

            var p = (pixelPos + vec2<f32>(f32(i), f32(j)) * 0.5 - screenSize * 0.5) / screenSize.y;

            // Time, rotating back and forth
            let ttm = cos(sin(time / 8.0)) * 6.2831;

            // Rotate and translate canvas
            p = mbdeco_rot(ttm) * p;
            p -= vec2<f32>(cos(time / 2.0) / 2.0, sin(time / 3.0) / 5.0);

            // Zoom and offset into the Mandelbrot set
            let zm = 200.0 + sin(time / 7.0) * 50.0;
            let cc = vec2<f32>(-0.57735 + 0.004, 0.57735) + p / zm;

            var z = vec2<f32>(0.0);
            var dz = vec2<f32>(0.0);

            let iter = 128;
            var ik = 128;

            for (var k = 0; k < 128; k++) {
                // Derivative: z' = z*z'*2 + 1
                dz = mbdeco_cmul(z) * dz * 2.0 + vec2<f32>(1.0, 0.0);

                // Position: z = z*z + c
                z = mbdeco_cmul(z) * z + cc;

                if (dot(z, z) > 200.0) {
                    ik = k;
                    break;
                }
            }

            // Boundary line
            let ln = step(0.0, length(z) / 15.5 - 1.0);

            // Distance/shade via derivative
            let d_raw = sqrt(1.0 / max(length(dz), 0.0001)) * log(dot(z, z));
            let d = clamp(d_raw * 50.0, 0.0, 1.0);

            // Alternating layer direction
            let dir = select(1.0, -1.0, mbdeco_mod_f(f32(ik), 2.0) < 0.5);

            // Layer shading
            let sh = f32(iter - ik) / f32(iter);
            var tuv = z / 320.0;

            // Rotate pattern by depth for parallax
            let tm = -ttm * sh * sh * 16.0;
            tuv = mbdeco_rot(tm) * tuv;
            tuv = abs(mbdeco_mod_v2(tuv, 1.0 / 8.0) - 1.0 / 16.0);

            // Circle grid + grid boundary pattern
            var pat = smoothstep(0.0, 1.0 / length(dz), length(tuv) - 1.0 / 32.0);
            pat = min(pat, smoothstep(0.0, 1.0 / length(dz), abs(max(tuv.x, tuv.y) - 1.0 / 16.0) - 0.04 / 16.0));

            // Layer color
            var lCol = pow(min(vec3<f32>(1.5, 1.0, 1.0) * min(d * 0.85, 0.96), vec3<f32>(1.0)), vec3<f32>(1.0, 3.0, 16.0)) * 1.15;

            // Apply pattern based on layer direction
            if (dir < 0.0) {
                lCol = lCol * min(pat, ln);
            } else {
                lCol = (sqrt(lCol) * 0.5 + 0.7) * max(1.0 - pat, 1.0 - ln);
            }

            // Fake gloss/highlights
            var rd = normalize(vec3<f32>(p, 1.0));
            rd = reflect(rd, vec3<f32>(0.0, 0.0, -1.0));
            let diff = clamp(dot(z * 0.5 + 0.5, rd.xy), 0.0, 1.0) * d;

            // Reflective pattern offset
            var tuv2 = z / 200.0;
            let tm2 = -tm / 1.5 + 0.5;
            tuv2 = mbdeco_rot(tm2) * tuv2;
            tuv2 = abs(mbdeco_mod_v2(tuv2, 1.0 / 8.0) - 1.0 / 16.0);
            var pat2 = smoothstep(0.0, 1.0 / length(dz), length(tuv2) - 1.0 / 32.0);
            pat2 = min(pat2, smoothstep(0.0, 1.0 / length(dz), abs(max(tuv2.x, tuv2.y) - 1.0 / 16.0) - 0.04 / 16.0));

            // Add gloss
            lCol += mix(lCol, vec3<f32>(1.0) * ln, 0.5) * diff * diff * 0.5 * (pat2 * 0.6 + 0.6);

            // Swizzle every 6th layer
            if (mbdeco_mod_f(f32(ik), 6.0) < 0.5) {
                lCol = lCol.yxz;
            }
            lCol = mix(lCol.xzy, lCol, d / 1.2);

            // Deep black fringes for depth
            lCol = mix(lCol, vec3<f32>(0.0), (1.0 - step(0.0, -(length(z) * 0.05 * f32(ik) / f32(iter) - 1.0))) * 0.95);

            // Apply fog
            lCol = mix(vec3<f32>(0.0), lCol, sh * d);

            col += min(lCol, vec3<f32>(1.0));
        }
    }

    col /= 4.0;

    // Vignette
    let uv = pixelPos / screenSize;
    col *= pow(16.0 * (1.0 - uv.x) * (1.0 - uv.y) * uv.x * uv.y, 1.0 / 8.0) * 1.15;

    return sqrt(max(col, vec3<f32>(0.0)));
}
