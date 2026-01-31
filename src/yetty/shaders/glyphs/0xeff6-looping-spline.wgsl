// Shader glyph: Looping Spline (codepoint 1114102 / U+10FFF6)
// Credit: Sébastien Bérubé
// License: Creative Commons Non-commercial (NC)
// Converted from GLSL to WGSL
//
// Catmull-Rom spline with cyclic path, iterative distance field visualization,
// moving points along the curve, and control point display.

const LSPLINE_POINT_COUNT: i32 = 8;

struct LSplineCtrlPts {
    p: array<vec2<f32>, 8>,
};

fn lspline_pointArray(i: i32, ctrlPts: LSplineCtrlPts) -> vec2<f32> {
    let idx = ((i % LSPLINE_POINT_COUNT) + LSPLINE_POINT_COUNT) % LSPLINE_POINT_COUNT;
    return ctrlPts.p[idx];
}

fn lspline_catmullRom(fTime: f32, ctrlPts: LSplineCtrlPts) -> vec2<f32> {
    var t = fTime;
    let n = f32(LSPLINE_POINT_COUNT);

    let idxOffset = i32(t * n);
    let p1 = lspline_pointArray(idxOffset, ctrlPts);
    let p2 = lspline_pointArray(idxOffset + 1, ctrlPts);
    let p3 = lspline_pointArray(idxOffset + 2, ctrlPts);
    let p4 = lspline_pointArray(idxOffset + 3, ctrlPts);

    t *= n;
    t = t - floor(t);

    let val = 0.5 * ((-p1 + 3.0 * p2 - 3.0 * p3 + p4) * t * t * t
               + (2.0 * p1 - 5.0 * p2 + 4.0 * p3 - p4) * t * t
               + (-p1 + p3) * t
               + 2.0 * p2);
    return val;
}

fn lspline_distanceToLineSeg(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>) -> f32 {
    let ap = p - a;
    let ab = b - a;
    let e = a + clamp(dot(ap, ab) / dot(ab, ab), 0.0, 1.0) * ab;
    return length(p - e);
}

fn lspline_debugDistanceField(uv: vec2<f32>, ctrlPts: LSplineCtrlPts) -> vec2<f32> {
    let MAX_DIST = 10000.0;
    var bestX = 0.0;

    // Primary (rough) estimate: 2 line segments per control point
    let primarySegLength = 1.0 / 16.0; // 1/(POINT_COUNT*2+1 - 1) = 1/16
    var pA = lspline_catmullRom(0.0, ctrlPts);
    var minRoughDist = MAX_DIST;
    var x = 0.0;

    for (var i = 0; i < 17; i++) { // POINT_COUNT*2+1 = 17
        let pB = lspline_catmullRom(x, ctrlPts);
        let d = lspline_distanceToLineSeg(uv, pA, pB);
        pA = pB;
        if (d < minRoughDist) {
            bestX = x;
            minRoughDist = d;
        }
        x += primarySegLength;
        x = min(x, 0.99999);
    }

    // Secondary (smooth) estimate: refine around best segment
    x = max(bestX - 1.25 * primarySegLength, 0.0);
    var minDist = MAX_DIST;
    pA = lspline_catmullRom(x, ctrlPts);

    for (var i = 0; i < 14; i++) {
        let pB = lspline_catmullRom(x, ctrlPts);
        let d = lspline_distanceToLineSeg(uv, pA, pB);
        pA = pB;
        if (d < minDist) {
            bestX = x;
            minDist = d;
        }
        x += 1.5 / 13.0 * primarySegLength; // 1.5/(iter2-1) * primarySegLength
        x = min(x, 0.99999);
    }

    return vec2<f32>(minDist, minRoughDist);
}

fn shaderGlyph_1114102(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);

    var ctrlPts: LSplineCtrlPts;
    ctrlPts.p[0] = vec2<f32>(0.10, 0.25);
    ctrlPts.p[1] = vec2<f32>(0.2, 0.1);
    ctrlPts.p[2] = vec2<f32>(0.6, 0.35);
    ctrlPts.p[3] = vec2<f32>(0.4, 0.1);
    ctrlPts.p[4] = vec2<f32>(0.8, 0.35);
    ctrlPts.p[5] = vec2<f32>(0.6, 0.55);
    ctrlPts.p[6] = vec2<f32>(0.5, 0.45);
    ctrlPts.p[7] = vec2<f32>(0.3, 0.49);

    // Mouse interaction: drag control point 2
    if (mousePos.x > 0.0 && mousePos.y > 0.0) {
        ctrlPts.p[2] = mousePos / screenSize.xx;
    }

    let uv = pixelPos / screenSize.xx;

    let fTime = time * 0.15;
    let pA = lspline_catmullRom(fract(fTime), ctrlPts);
    let pB = lspline_catmullRom(fract(fTime + 0.02), ctrlPts);

    // Compute distance field
    let dSeg = lspline_debugDistanceField(uv, ctrlPts);

    // Draw distance field background
    var c = vec3<f32>(dSeg.x * 7.0 + smoothstep(0.20, 0.3, abs(fract(dSeg.x * 20.0) - 0.5)));

    // Draw the spline (cyan rough, red refined)
    c = mix(vec3<f32>(0.0, 0.8, 0.9), c, smoothstep(-0.005, 0.0035, dSeg.y));
    c = mix(vec3<f32>(1.0, 0.0, 0.0), c, smoothstep(0.0, 0.0025, dSeg.x));

    // Draw each control point (blue)
    var minDistP = 10000.0;
    for (var i = 0; i < 8; i++) {
        let ctrl_pt = lspline_pointArray(i, ctrlPts);
        minDistP = min(length(uv - ctrl_pt), minDistP);
    }
    c = mix(vec3<f32>(0.0, 0.0, 1.0), c, smoothstep(0.008, 0.011, minDistP));

    // Draw moving points (green with white center)
    c = mix(vec3<f32>(0.0, 0.7, 0.0), c, smoothstep(0.008, 0.011, length(uv - pA)));
    c = mix(vec3<f32>(0.0, 0.7, 0.0), c, smoothstep(0.008, 0.011, length(uv - pB)));
    c = mix(vec3<f32>(1.0, 1.0, 1.0), c, smoothstep(0.004, 0.006, length(uv - pB)));

    return c;
}
