// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

// Overlay SDF functions for both absolute (overlay) and scrolling overlays.
// Requires bindings: overlayStorage (17), scrollingStorage (21)

fn evalSDF_overlay(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed in u32) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(overlayStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * overlay.cellSizeX, gridOffsetY * overlay.cellSizeY);
    let pAdj = p - pixelOffset;

    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let radius = overlayStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let round = overlayStorage[primOffset + 10u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_SEGMENT: {
            let p0 = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let p1 = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdSegment(pAdj, p0, p1);
        }
        case SDF_TRIANGLE: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            return sdTriangle(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER2: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            return sdBezier2(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER3: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            let v3 = vec2<f32>(overlayStorage[primOffset + 9u], overlayStorage[primOffset + 10u]);
            return sdBezier3(pAdj, v0, v1, v2, v3);
        }
        case SDF_ELLIPSE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let radii = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdEllipse(pAdj, center, radii);
        }
        case SDF_ARC: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let sc = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let ra = overlayStorage[primOffset + 7u];
            let rb = overlayStorage[primOffset + 8u];
            return sdArc(pAdj, center, sc, ra, rb);
        }
        case SDF_ROUNDED_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let radii = vec4<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u], overlayStorage[primOffset + 10u]);
            return sdRoundedBox(pAdj, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let b = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdRhombus(pAdj, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdPentagon(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdHexagon(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_STAR: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdStar(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_PIE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let c = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdPie(pAdj, center, c, overlayStorage[primOffset + 7u]);
        }
        case SDF_RING: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let n = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdRing(pAdj, center, n, overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
        }
        case SDF_HEART: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdHeart(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let b = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdCross(pAdj, center, b, overlayStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdRoundedX(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let b = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdCapsule(pAdj, a, b, overlayStorage[primOffset + 7u]);
        }
        case SDF_MOON: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdMoon(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_EGG: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdEgg(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdChamferBox(pAdj, center, halfSize, overlayStorage[primOffset + 7u]);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let b = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdOrientedBox(pAdj, a, b, overlayStorage[primOffset + 7u]);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdTrapezoid(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdParallelogram(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdEquilateralTriangle(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let q = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdTriangleIsosceles(pAdj, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdUnevenCapsule(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdOctogon(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdHexagram(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdPentagram(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdCutDisk(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let c = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let w = vec2<f32>(overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u]);
            return sdHorseshoe(pAdj, center, c, overlayStorage[primOffset + 7u], w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdVesica(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let b = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdOrientedVesica(pAdj, a, b, overlayStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdRoundedCross(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdParabola(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdBlobbyCross(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let wh = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdTunnel(pAdj, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let wh = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            return sdStairs(pAdj, center, wh, overlayStorage[primOffset + 7u]);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdQuadraticCircle(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdHyperbola(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdCoolS(pAdj, center, overlayStorage[primOffset + 5u]);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdCircleWave(pAdj, center, overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_COLOR_WHEEL: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let d = pAdj - center;
            let dist = length(d);
            let ringDist = abs(dist - (overlayStorage[primOffset + 6u] + overlayStorage[primOffset + 5u]) * 0.5) - (overlayStorage[primOffset + 5u] - overlayStorage[primOffset + 6u]) * 0.5;
            return ringDist;
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let round = overlayStorage[primOffset + 15u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let radius = overlayStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            let radius = overlayStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        default: {
            return 1e10;
        }
    }
}

fn evalSDF3D_overlay(primOffset: u32, p: vec3<f32>) -> f32 {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);

    switch (primType) {
        case SDF_SPHERE_3D: {
            let pos = vec3<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdSphere3D(p - pos, overlayStorage[primOffset + 5u]);
        }
        case SDF_BOX_3D: {
            let pos = vec3<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u], overlayStorage[primOffset + 4u]);
            return sdBox3D(p - pos, vec3<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]));
        }
        default: { return 1e10; }
    }
}

fn primColors_overlay(primOffset: u32) -> vec4<u32> {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SEGMENT: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER2: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER3: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 12u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ARC: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 12u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RHOMBUS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAR: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PIE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RING: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEART: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_X: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_MOON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EGG: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CHAMFER_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRAPEZOID: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARALLELOGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_UNEVEN_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTOGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CUT_DISK: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HORSESHOE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VESICA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_VESICA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARABOLA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BLOBBY_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TUNNEL: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAIRS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_QUADRATIC_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HYPERBOLA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COOL_S: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CIRCLE_WAVE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COLOR_WHEEL: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 12u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TEXT_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROTATED_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SPHERE_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TORUS_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CYLINDER_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VERTICAL_CAPSULE_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPPED_CONE_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTAHEDRON_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PYRAMID_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSOID_3D: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_POLYGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 3u]),
                bitcast<u32>(overlayStorage[primOffset + 4u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_POLYGON_GROUP: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 4u]),
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 13u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 12u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        default: { return vec4<u32>(0u); }
    }
}

fn evalGradientFillColor_overlay(primOffset: u32, p: vec2<f32>) -> vec4<f32> {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(overlayStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * overlay.cellSizeX, gridOffsetY * overlay.cellSizeY);
    let pAdj = p - pixelOffset;
    switch (primType) {
        case SDF_LINEAR_GRADIENT_BOX: {
            let gStart = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            let gEnd = vec2<f32>(overlayStorage[primOffset + 9u], overlayStorage[primOffset + 10u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 11u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 12u]));
            return mix(c1, c2, t);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let gStart = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            let gEnd = vec2<f32>(overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 10u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 11u]));
            return mix(c1, c2, t);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let gCenter = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            let gRadius = overlayStorage[primOffset + 8u];
            let t = clamp(length(pAdj - gCenter) / gRadius, 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 9u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 10u]));
            return mix(c1, c2, t);
        }
        default: { return vec4<f32>(0.0); }
    }
}

fn primStrokeWidth_overlay(primOffset: u32) -> f32 {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: { return overlayStorage[primOffset + 8u]; }
        case SDF_BOX: { return overlayStorage[primOffset + 9u]; }
        case SDF_SEGMENT: { return overlayStorage[primOffset + 9u]; }
        case SDF_TRIANGLE: { return overlayStorage[primOffset + 11u]; }
        case SDF_BEZIER2: { return overlayStorage[primOffset + 11u]; }
        case SDF_BEZIER3: { return overlayStorage[primOffset + 13u]; }
        case SDF_ELLIPSE: { return overlayStorage[primOffset + 9u]; }
        case SDF_ARC: { return overlayStorage[primOffset + 11u]; }
        case SDF_ROUNDED_BOX: { return overlayStorage[primOffset + 13u]; }
        case SDF_RHOMBUS: { return overlayStorage[primOffset + 9u]; }
        case SDF_PENTAGON: { return overlayStorage[primOffset + 8u]; }
        case SDF_HEXAGON: { return overlayStorage[primOffset + 8u]; }
        case SDF_STAR: { return overlayStorage[primOffset + 10u]; }
        case SDF_PIE: { return overlayStorage[primOffset + 10u]; }
        case SDF_RING: { return overlayStorage[primOffset + 11u]; }
        case SDF_HEART: { return overlayStorage[primOffset + 8u]; }
        case SDF_CROSS: { return overlayStorage[primOffset + 10u]; }
        case SDF_ROUNDED_X: { return overlayStorage[primOffset + 9u]; }
        case SDF_CAPSULE: { return overlayStorage[primOffset + 10u]; }
        case SDF_MOON: { return overlayStorage[primOffset + 10u]; }
        case SDF_EGG: { return overlayStorage[primOffset + 9u]; }
        case SDF_CHAMFER_BOX: { return overlayStorage[primOffset + 10u]; }
        case SDF_ORIENTED_BOX: { return overlayStorage[primOffset + 10u]; }
        case SDF_TRAPEZOID: { return overlayStorage[primOffset + 10u]; }
        case SDF_PARALLELOGRAM: { return overlayStorage[primOffset + 10u]; }
        case SDF_EQUILATERAL_TRIANGLE: { return overlayStorage[primOffset + 8u]; }
        case SDF_ISOSCELES_TRIANGLE: { return overlayStorage[primOffset + 9u]; }
        case SDF_UNEVEN_CAPSULE: { return overlayStorage[primOffset + 10u]; }
        case SDF_OCTOGON: { return overlayStorage[primOffset + 8u]; }
        case SDF_HEXAGRAM: { return overlayStorage[primOffset + 8u]; }
        case SDF_PENTAGRAM: { return overlayStorage[primOffset + 8u]; }
        case SDF_CUT_DISK: { return overlayStorage[primOffset + 9u]; }
        case SDF_HORSESHOE: { return overlayStorage[primOffset + 12u]; }
        case SDF_VESICA: { return overlayStorage[primOffset + 9u]; }
        case SDF_ORIENTED_VESICA: { return overlayStorage[primOffset + 10u]; }
        case SDF_ROUNDED_CROSS: { return overlayStorage[primOffset + 8u]; }
        case SDF_PARABOLA: { return overlayStorage[primOffset + 8u]; }
        case SDF_BLOBBY_CROSS: { return overlayStorage[primOffset + 8u]; }
        case SDF_TUNNEL: { return overlayStorage[primOffset + 9u]; }
        case SDF_STAIRS: { return overlayStorage[primOffset + 10u]; }
        case SDF_QUADRATIC_CIRCLE: { return overlayStorage[primOffset + 8u]; }
        case SDF_HYPERBOLA: { return overlayStorage[primOffset + 9u]; }
        case SDF_COOL_S: { return overlayStorage[primOffset + 8u]; }
        case SDF_CIRCLE_WAVE: { return overlayStorage[primOffset + 9u]; }
        case SDF_COLOR_WHEEL: { return overlayStorage[primOffset + 13u]; }
        case SDF_TEXT_GLYPH: { return overlayStorage[primOffset + 9u]; }
        case SDF_ROTATED_GLYPH: { return overlayStorage[primOffset + 12u]; }
        case SDF_SPHERE_3D: { return overlayStorage[primOffset + 8u]; }
        case SDF_BOX_3D: { return overlayStorage[primOffset + 10u]; }
        case SDF_TORUS_3D: { return overlayStorage[primOffset + 9u]; }
        case SDF_CYLINDER_3D: { return overlayStorage[primOffset + 9u]; }
        case SDF_VERTICAL_CAPSULE_3D: { return overlayStorage[primOffset + 9u]; }
        case SDF_CAPPED_CONE_3D: { return overlayStorage[primOffset + 10u]; }
        case SDF_OCTAHEDRON_3D: { return overlayStorage[primOffset + 8u]; }
        case SDF_PYRAMID_3D: { return overlayStorage[primOffset + 8u]; }
        case SDF_ELLIPSOID_3D: { return overlayStorage[primOffset + 10u]; }
        case SDF_POLYGON: { return overlayStorage[primOffset + 5u]; }
        case SDF_POLYGON_GROUP: { return overlayStorage[primOffset + 6u]; }
        case SDF_LINEAR_GRADIENT_BOX: { return overlayStorage[primOffset + 14u]; }
        case SDF_LINEAR_GRADIENT_CIRCLE: { return overlayStorage[primOffset + 13u]; }
        case SDF_RADIAL_GRADIENT_CIRCLE: { return overlayStorage[primOffset + 12u]; }
        default: { return 0.0; }
    }
}


// ============= SCROLLING OVERLAY VARIANTS =============
// Same as _overlay functions but reading from scrollingStorage/scrolling uniform

fn evalSDF_scrolling(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(scrollingStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed in u32) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(scrollingStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * scrolling.cellSizeX, gridOffsetY * scrolling.cellSizeY);
    let pAdj = p - pixelOffset;

    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let radius = scrollingStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_BOX: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let round = scrollingStorage[primOffset + 10u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_SEGMENT: {
            let p0 = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let p1 = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdSegment(pAdj, p0, p1);
        }
        case SDF_TRIANGLE: {
            let v0 = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let v1 = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let v2 = vec2<f32>(scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u]);
            return sdTriangle(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER2: {
            let v0 = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let v1 = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let v2 = vec2<f32>(scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u]);
            return sdBezier2(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER3: {
            let v0 = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let v1 = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let v2 = vec2<f32>(scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u]);
            let v3 = vec2<f32>(scrollingStorage[primOffset + 9u], scrollingStorage[primOffset + 10u]);
            return sdBezier3(pAdj, v0, v1, v2, v3);
        }
        case SDF_ELLIPSE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let radii = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdEllipse(pAdj, center, radii);
        }
        case SDF_ARC: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let sc = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let ra = scrollingStorage[primOffset + 7u];
            let rb = scrollingStorage[primOffset + 8u];
            return sdArc(pAdj, center, sc, ra, rb);
        }
        case SDF_ROUNDED_BOX: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let radii = vec4<f32>(scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u], scrollingStorage[primOffset + 9u], scrollingStorage[primOffset + 10u]);
            return sdRoundedBox(pAdj, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let b = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdRhombus(pAdj, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdPentagon(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdHexagon(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_STAR: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdStar(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
        }
        case SDF_PIE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let c = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdPie(pAdj, center, c, scrollingStorage[primOffset + 7u]);
        }
        case SDF_RING: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let n = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdRing(pAdj, center, n, scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u]);
        }
        case SDF_HEART: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdHeart(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let b = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdCross(pAdj, center, b, scrollingStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdRoundedX(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let b = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdCapsule(pAdj, a, b, scrollingStorage[primOffset + 7u]);
        }
        case SDF_MOON: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdMoon(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
        }
        case SDF_EGG: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdEgg(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdChamferBox(pAdj, center, halfSize, scrollingStorage[primOffset + 7u]);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let b = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdOrientedBox(pAdj, a, b, scrollingStorage[primOffset + 7u]);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdTrapezoid(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdParallelogram(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdEquilateralTriangle(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let q = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdTriangleIsosceles(pAdj, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdUnevenCapsule(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdOctogon(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdHexagram(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdPentagram(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdCutDisk(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let c = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let w = vec2<f32>(scrollingStorage[primOffset + 8u], scrollingStorage[primOffset + 9u]);
            return sdHorseshoe(pAdj, center, c, scrollingStorage[primOffset + 7u], w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdVesica(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let b = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdOrientedVesica(pAdj, a, b, scrollingStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdRoundedCross(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdParabola(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdBlobbyCross(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let wh = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdTunnel(pAdj, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let wh = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            return sdStairs(pAdj, center, wh, scrollingStorage[primOffset + 7u]);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdQuadraticCircle(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdHyperbola(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdCoolS(pAdj, center, scrollingStorage[primOffset + 5u]);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdCircleWave(pAdj, center, scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
        }
        case SDF_COLOR_WHEEL: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let d = pAdj - center;
            let dist = length(d);
            let ringDist = abs(dist - (scrollingStorage[primOffset + 6u] + scrollingStorage[primOffset + 5u]) * 0.5) - (scrollingStorage[primOffset + 5u] - scrollingStorage[primOffset + 6u]) * 0.5;
            return ringDist;
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u]);
            let round = scrollingStorage[primOffset + 15u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let radius = scrollingStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let center = vec2<f32>(scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            let radius = scrollingStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        default: {
            return 1e10;
        }
    }
}

fn evalSDF3D_scrolling(primOffset: u32, p: vec3<f32>) -> f32 {
    let primType = bitcast<u32>(scrollingStorage[primOffset + 0u]);

    switch (primType) {
        case SDF_SPHERE_3D: {
            let pos = vec3<f32>(scrollingStorage[primOffset + 2u], scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdSphere3D(p - pos, scrollingStorage[primOffset + 5u]);
        }
        case SDF_BOX_3D: {
            let pos = vec3<f32>(scrollingStorage[primOffset + 2u], scrollingStorage[primOffset + 3u], scrollingStorage[primOffset + 4u]);
            return sdBox3D(p - pos, vec3<f32>(scrollingStorage[primOffset + 5u], scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]));
        }
        default: { return 1e10; }
    }
}

fn primColors_scrolling(primOffset: u32) -> vec4<u32> {
    let primType = bitcast<u32>(scrollingStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SEGMENT: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER2: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER3: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 12u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ARC: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_BOX: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 12u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RHOMBUS: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGON: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGON: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAR: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PIE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RING: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEART: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CROSS: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_X: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_MOON: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EGG: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CHAMFER_BOX: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_BOX: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRAPEZOID: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARALLELOGRAM: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_UNEVEN_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTOGON: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGRAM: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGRAM: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CUT_DISK: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HORSESHOE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VESICA: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_VESICA: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_CROSS: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARABOLA: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BLOBBY_CROSS: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TUNNEL: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAIRS: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_QUADRATIC_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HYPERBOLA: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COOL_S: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CIRCLE_WAVE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COLOR_WHEEL: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 12u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TEXT_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROTATED_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SPHERE_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TORUS_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CYLINDER_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VERTICAL_CAPSULE_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPPED_CONE_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTAHEDRON_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PYRAMID_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 6u]),
                bitcast<u32>(scrollingStorage[primOffset + 7u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSOID_3D: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 8u]),
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_POLYGON: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 3u]),
                bitcast<u32>(scrollingStorage[primOffset + 4u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_POLYGON_GROUP: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 4u]),
                bitcast<u32>(scrollingStorage[primOffset + 5u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                0u);
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 13u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 10u]),
                bitcast<u32>(scrollingStorage[primOffset + 12u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(scrollingStorage[primOffset + 9u]),
                bitcast<u32>(scrollingStorage[primOffset + 11u]),
                bitcast<u32>(scrollingStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        default: { return vec4<u32>(0u); }
    }
}

fn evalGradientFillColor_scrolling(primOffset: u32, p: vec2<f32>) -> vec4<f32> {
    let primType = bitcast<u32>(scrollingStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(scrollingStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * scrolling.cellSizeX, gridOffsetY * scrolling.cellSizeY);
    let pAdj = p - pixelOffset;
    switch (primType) {
        case SDF_LINEAR_GRADIENT_BOX: {
            let gStart = vec2<f32>(scrollingStorage[primOffset + 7u], scrollingStorage[primOffset + 8u]);
            let gEnd = vec2<f32>(scrollingStorage[primOffset + 9u], scrollingStorage[primOffset + 10u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 11u]));
            let c2 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 12u]));
            return mix(c1, c2, t);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let gStart = vec2<f32>(scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
            let gEnd = vec2<f32>(scrollingStorage[primOffset + 8u], scrollingStorage[primOffset + 9u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 10u]));
            let c2 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 11u]));
            return mix(c1, c2, t);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let gCenter = vec2<f32>(scrollingStorage[primOffset + 6u], scrollingStorage[primOffset + 7u]);
            let gRadius = scrollingStorage[primOffset + 8u];
            let t = clamp(length(pAdj - gCenter) / gRadius, 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 9u]));
            let c2 = unpack4x8unorm(bitcast<u32>(scrollingStorage[primOffset + 10u]));
            return mix(c1, c2, t);
        }
        default: { return vec4<f32>(0.0); }
    }
}

fn primStrokeWidth_scrolling(primOffset: u32) -> f32 {
    let primType = bitcast<u32>(scrollingStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: { return scrollingStorage[primOffset + 8u]; }
        case SDF_BOX: { return scrollingStorage[primOffset + 9u]; }
        case SDF_SEGMENT: { return scrollingStorage[primOffset + 9u]; }
        case SDF_TRIANGLE: { return scrollingStorage[primOffset + 11u]; }
        case SDF_BEZIER2: { return scrollingStorage[primOffset + 11u]; }
        case SDF_BEZIER3: { return scrollingStorage[primOffset + 13u]; }
        case SDF_ELLIPSE: { return scrollingStorage[primOffset + 9u]; }
        case SDF_ARC: { return scrollingStorage[primOffset + 11u]; }
        case SDF_ROUNDED_BOX: { return scrollingStorage[primOffset + 13u]; }
        case SDF_RHOMBUS: { return scrollingStorage[primOffset + 9u]; }
        case SDF_PENTAGON: { return scrollingStorage[primOffset + 8u]; }
        case SDF_HEXAGON: { return scrollingStorage[primOffset + 8u]; }
        case SDF_STAR: { return scrollingStorage[primOffset + 10u]; }
        case SDF_PIE: { return scrollingStorage[primOffset + 10u]; }
        case SDF_RING: { return scrollingStorage[primOffset + 11u]; }
        case SDF_HEART: { return scrollingStorage[primOffset + 8u]; }
        case SDF_CROSS: { return scrollingStorage[primOffset + 10u]; }
        case SDF_ROUNDED_X: { return scrollingStorage[primOffset + 9u]; }
        case SDF_CAPSULE: { return scrollingStorage[primOffset + 10u]; }
        case SDF_MOON: { return scrollingStorage[primOffset + 10u]; }
        case SDF_EGG: { return scrollingStorage[primOffset + 9u]; }
        case SDF_CHAMFER_BOX: { return scrollingStorage[primOffset + 10u]; }
        case SDF_ORIENTED_BOX: { return scrollingStorage[primOffset + 10u]; }
        case SDF_TRAPEZOID: { return scrollingStorage[primOffset + 10u]; }
        case SDF_PARALLELOGRAM: { return scrollingStorage[primOffset + 10u]; }
        case SDF_EQUILATERAL_TRIANGLE: { return scrollingStorage[primOffset + 8u]; }
        case SDF_ISOSCELES_TRIANGLE: { return scrollingStorage[primOffset + 9u]; }
        case SDF_UNEVEN_CAPSULE: { return scrollingStorage[primOffset + 10u]; }
        case SDF_OCTOGON: { return scrollingStorage[primOffset + 8u]; }
        case SDF_HEXAGRAM: { return scrollingStorage[primOffset + 8u]; }
        case SDF_PENTAGRAM: { return scrollingStorage[primOffset + 8u]; }
        case SDF_CUT_DISK: { return scrollingStorage[primOffset + 9u]; }
        case SDF_HORSESHOE: { return scrollingStorage[primOffset + 12u]; }
        case SDF_VESICA: { return scrollingStorage[primOffset + 9u]; }
        case SDF_ORIENTED_VESICA: { return scrollingStorage[primOffset + 10u]; }
        case SDF_ROUNDED_CROSS: { return scrollingStorage[primOffset + 8u]; }
        case SDF_PARABOLA: { return scrollingStorage[primOffset + 8u]; }
        case SDF_BLOBBY_CROSS: { return scrollingStorage[primOffset + 8u]; }
        case SDF_TUNNEL: { return scrollingStorage[primOffset + 9u]; }
        case SDF_STAIRS: { return scrollingStorage[primOffset + 10u]; }
        case SDF_QUADRATIC_CIRCLE: { return scrollingStorage[primOffset + 8u]; }
        case SDF_HYPERBOLA: { return scrollingStorage[primOffset + 9u]; }
        case SDF_COOL_S: { return scrollingStorage[primOffset + 8u]; }
        case SDF_CIRCLE_WAVE: { return scrollingStorage[primOffset + 9u]; }
        case SDF_COLOR_WHEEL: { return scrollingStorage[primOffset + 13u]; }
        case SDF_TEXT_GLYPH: { return scrollingStorage[primOffset + 9u]; }
        case SDF_ROTATED_GLYPH: { return scrollingStorage[primOffset + 12u]; }
        case SDF_SPHERE_3D: { return scrollingStorage[primOffset + 8u]; }
        case SDF_BOX_3D: { return scrollingStorage[primOffset + 10u]; }
        case SDF_TORUS_3D: { return scrollingStorage[primOffset + 9u]; }
        case SDF_CYLINDER_3D: { return scrollingStorage[primOffset + 9u]; }
        case SDF_VERTICAL_CAPSULE_3D: { return scrollingStorage[primOffset + 9u]; }
        case SDF_CAPPED_CONE_3D: { return scrollingStorage[primOffset + 10u]; }
        case SDF_OCTAHEDRON_3D: { return scrollingStorage[primOffset + 8u]; }
        case SDF_PYRAMID_3D: { return scrollingStorage[primOffset + 8u]; }
        case SDF_ELLIPSOID_3D: { return scrollingStorage[primOffset + 10u]; }
        case SDF_POLYGON: { return scrollingStorage[primOffset + 5u]; }
        case SDF_POLYGON_GROUP: { return scrollingStorage[primOffset + 6u]; }
        case SDF_LINEAR_GRADIENT_BOX: { return scrollingStorage[primOffset + 14u]; }
        case SDF_LINEAR_GRADIENT_CIRCLE: { return scrollingStorage[primOffset + 13u]; }
        case SDF_RADIAL_GRADIENT_CIRCLE: { return scrollingStorage[primOffset + 12u]; }
        default: { return 0.0; }
    }
}


// ============= SCROLLING OVERLAY VARIANTS =============
// Same as _scrolling functions but reading from scrollingStorage/scrolling uniform
