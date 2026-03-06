// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

// Overlay SDF functions reading from overlayStorage buffer.
// Only include in shaders that define @group(1) @binding(17) overlayStorage.

fn evalSDF_overlay(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);

    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let radius = overlayStorage[primOffset + 4u];
            return sdCircle(p, center, radius);
        }
        case SDF_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let round = overlayStorage[primOffset + 9u];
            return sdBox(p, center, halfSize, round);
        }
        case SDF_SEGMENT: {
            let p0 = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let p1 = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdSegment(p, p0, p1);
        }
        case SDF_TRIANGLE: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            return sdTriangle(p, v0, v1, v2);
        }
        case SDF_BEZIER2: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            return sdBezier2(p, v0, v1, v2);
        }
        case SDF_BEZIER3: {
            let v0 = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let v1 = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let v2 = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            let v3 = vec2<f32>(overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u]);
            return sdBezier3(p, v0, v1, v2, v3);
        }
        case SDF_ELLIPSE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let radii = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdEllipse(p, center, radii);
        }
        case SDF_ARC: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let sc = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let ra = overlayStorage[primOffset + 6u];
            let rb = overlayStorage[primOffset + 7u];
            return sdArc(p, center, sc, ra, rb);
        }
        case SDF_ROUNDED_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let radii = vec4<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u]);
            return sdRoundedBox(p, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let b = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdRhombus(p, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdPentagon(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdHexagon(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_STAR: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdStar(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_PIE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let c = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdPie(p, center, c, overlayStorage[primOffset + 6u]);
        }
        case SDF_RING: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let n = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdRing(p, center, n, overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
        }
        case SDF_HEART: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdHeart(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let b = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdCross(p, center, b, overlayStorage[primOffset + 6u]);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdRoundedX(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let b = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdCapsule(p, a, b, overlayStorage[primOffset + 6u]);
        }
        case SDF_MOON: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdMoon(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_EGG: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdEgg(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdChamferBox(p, center, halfSize, overlayStorage[primOffset + 6u]);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let b = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdOrientedBox(p, a, b, overlayStorage[primOffset + 6u]);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdTrapezoid(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdParallelogram(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdEquilateralTriangle(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let q = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdTriangleIsosceles(p, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdUnevenCapsule(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdOctogon(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdHexagram(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdPentagram(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdCutDisk(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let c = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let w = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            return sdHorseshoe(p, center, c, overlayStorage[primOffset + 6u], w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdVesica(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let b = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdOrientedVesica(p, a, b, overlayStorage[primOffset + 6u]);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdRoundedCross(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdParabola(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdBlobbyCross(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let wh = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdTunnel(p, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let wh = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            return sdStairs(p, center, wh, overlayStorage[primOffset + 6u]);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdQuadraticCircle(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdHyperbola(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdCoolS(p, center, overlayStorage[primOffset + 4u]);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            return sdCircleWave(p, center, overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
        }
        case SDF_COLOR_WHEEL: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let d = p - center;
            let dist = length(d);
            let ringDist = abs(dist - (overlayStorage[primOffset + 5u] + overlayStorage[primOffset + 4u]) * 0.5) - (overlayStorage[primOffset + 4u] - overlayStorage[primOffset + 5u]) * 0.5;
            return ringDist;
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(overlayStorage[primOffset + 4u], overlayStorage[primOffset + 5u]);
            let round = overlayStorage[primOffset + 14u];
            return sdBox(p, center, halfSize, round);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let radius = overlayStorage[primOffset + 4u];
            return sdCircle(p, center, radius);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let center = vec2<f32>(overlayStorage[primOffset + 2u], overlayStorage[primOffset + 3u]);
            let radius = overlayStorage[primOffset + 4u];
            return sdCircle(p, center, radius);
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
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SEGMENT: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER2: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER3: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ARC: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RHOMBUS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAR: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PIE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RING: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEART: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_X: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_MOON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EGG: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CHAMFER_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_BOX: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRAPEZOID: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARALLELOGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_UNEVEN_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTOGON: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGRAM: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CUT_DISK: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HORSESHOE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VESICA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_VESICA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARABOLA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BLOBBY_CROSS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TUNNEL: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAIRS: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_QUADRATIC_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HYPERBOLA: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COOL_S: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 5u]),
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CIRCLE_WAVE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 6u]),
                bitcast<u32>(overlayStorage[primOffset + 7u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COLOR_WHEEL: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
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
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 12u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 9u]),
                bitcast<u32>(overlayStorage[primOffset + 11u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(overlayStorage[primOffset + 8u]),
                bitcast<u32>(overlayStorage[primOffset + 10u]),
                bitcast<u32>(overlayStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        default: { return vec4<u32>(0u); }
    }
}

fn evalGradientFillColor_overlay(primOffset: u32, p: vec2<f32>) -> vec4<f32> {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_LINEAR_GRADIENT_BOX: {
            let gStart = vec2<f32>(overlayStorage[primOffset + 6u], overlayStorage[primOffset + 7u]);
            let gEnd = vec2<f32>(overlayStorage[primOffset + 8u], overlayStorage[primOffset + 9u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(p - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 10u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 11u]));
            return mix(c1, c2, t);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let gStart = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let gEnd = vec2<f32>(overlayStorage[primOffset + 7u], overlayStorage[primOffset + 8u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(p - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 9u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 10u]));
            return mix(c1, c2, t);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let gCenter = vec2<f32>(overlayStorage[primOffset + 5u], overlayStorage[primOffset + 6u]);
            let gRadius = overlayStorage[primOffset + 7u];
            let t = clamp(length(p - gCenter) / gRadius, 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 8u]));
            let c2 = unpack4x8unorm(bitcast<u32>(overlayStorage[primOffset + 9u]));
            return mix(c1, c2, t);
        }
        default: { return vec4<f32>(0.0); }
    }
}

fn primStrokeWidth_overlay(primOffset: u32) -> f32 {
    let primType = bitcast<u32>(overlayStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: { return overlayStorage[primOffset + 7u]; }
        case SDF_BOX: { return overlayStorage[primOffset + 8u]; }
        case SDF_SEGMENT: { return overlayStorage[primOffset + 8u]; }
        case SDF_TRIANGLE: { return overlayStorage[primOffset + 10u]; }
        case SDF_BEZIER2: { return overlayStorage[primOffset + 10u]; }
        case SDF_BEZIER3: { return overlayStorage[primOffset + 12u]; }
        case SDF_ELLIPSE: { return overlayStorage[primOffset + 8u]; }
        case SDF_ARC: { return overlayStorage[primOffset + 10u]; }
        case SDF_ROUNDED_BOX: { return overlayStorage[primOffset + 12u]; }
        case SDF_RHOMBUS: { return overlayStorage[primOffset + 8u]; }
        case SDF_PENTAGON: { return overlayStorage[primOffset + 7u]; }
        case SDF_HEXAGON: { return overlayStorage[primOffset + 7u]; }
        case SDF_STAR: { return overlayStorage[primOffset + 9u]; }
        case SDF_PIE: { return overlayStorage[primOffset + 9u]; }
        case SDF_RING: { return overlayStorage[primOffset + 10u]; }
        case SDF_HEART: { return overlayStorage[primOffset + 7u]; }
        case SDF_CROSS: { return overlayStorage[primOffset + 9u]; }
        case SDF_ROUNDED_X: { return overlayStorage[primOffset + 8u]; }
        case SDF_CAPSULE: { return overlayStorage[primOffset + 9u]; }
        case SDF_MOON: { return overlayStorage[primOffset + 9u]; }
        case SDF_EGG: { return overlayStorage[primOffset + 8u]; }
        case SDF_CHAMFER_BOX: { return overlayStorage[primOffset + 9u]; }
        case SDF_ORIENTED_BOX: { return overlayStorage[primOffset + 9u]; }
        case SDF_TRAPEZOID: { return overlayStorage[primOffset + 9u]; }
        case SDF_PARALLELOGRAM: { return overlayStorage[primOffset + 9u]; }
        case SDF_EQUILATERAL_TRIANGLE: { return overlayStorage[primOffset + 7u]; }
        case SDF_ISOSCELES_TRIANGLE: { return overlayStorage[primOffset + 8u]; }
        case SDF_UNEVEN_CAPSULE: { return overlayStorage[primOffset + 9u]; }
        case SDF_OCTOGON: { return overlayStorage[primOffset + 7u]; }
        case SDF_HEXAGRAM: { return overlayStorage[primOffset + 7u]; }
        case SDF_PENTAGRAM: { return overlayStorage[primOffset + 7u]; }
        case SDF_CUT_DISK: { return overlayStorage[primOffset + 8u]; }
        case SDF_HORSESHOE: { return overlayStorage[primOffset + 11u]; }
        case SDF_VESICA: { return overlayStorage[primOffset + 8u]; }
        case SDF_ORIENTED_VESICA: { return overlayStorage[primOffset + 9u]; }
        case SDF_ROUNDED_CROSS: { return overlayStorage[primOffset + 7u]; }
        case SDF_PARABOLA: { return overlayStorage[primOffset + 7u]; }
        case SDF_BLOBBY_CROSS: { return overlayStorage[primOffset + 7u]; }
        case SDF_TUNNEL: { return overlayStorage[primOffset + 8u]; }
        case SDF_STAIRS: { return overlayStorage[primOffset + 9u]; }
        case SDF_QUADRATIC_CIRCLE: { return overlayStorage[primOffset + 7u]; }
        case SDF_HYPERBOLA: { return overlayStorage[primOffset + 8u]; }
        case SDF_COOL_S: { return overlayStorage[primOffset + 7u]; }
        case SDF_CIRCLE_WAVE: { return overlayStorage[primOffset + 8u]; }
        case SDF_COLOR_WHEEL: { return overlayStorage[primOffset + 12u]; }
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
        case SDF_LINEAR_GRADIENT_BOX: { return overlayStorage[primOffset + 13u]; }
        case SDF_LINEAR_GRADIENT_CIRCLE: { return overlayStorage[primOffset + 12u]; }
        case SDF_RADIAL_GRADIENT_CIRCLE: { return overlayStorage[primOffset + 11u]; }
        default: { return 0.0; }
    }
}
