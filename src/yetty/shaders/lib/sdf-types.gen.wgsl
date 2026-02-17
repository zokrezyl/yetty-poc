// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT

const SDF_CIRCLE: u32 = 0u;
const SDF_BOX: u32 = 1u;
const SDF_SEGMENT: u32 = 2u;
const SDF_TRIANGLE: u32 = 3u;
const SDF_BEZIER2: u32 = 4u;
const SDF_BEZIER3: u32 = 5u;
const SDF_ELLIPSE: u32 = 6u;
const SDF_ARC: u32 = 7u;
const SDF_ROUNDED_BOX: u32 = 8u;
const SDF_RHOMBUS: u32 = 9u;
const SDF_PENTAGON: u32 = 10u;
const SDF_HEXAGON: u32 = 11u;
const SDF_STAR: u32 = 12u;
const SDF_PIE: u32 = 13u;
const SDF_RING: u32 = 14u;
const SDF_HEART: u32 = 15u;
const SDF_CROSS: u32 = 16u;
const SDF_ROUNDED_X: u32 = 17u;
const SDF_CAPSULE: u32 = 18u;
const SDF_MOON: u32 = 19u;
const SDF_EGG: u32 = 20u;
const SDF_CHAMFER_BOX: u32 = 21u;
const SDF_ORIENTED_BOX: u32 = 22u;
const SDF_TRAPEZOID: u32 = 23u;
const SDF_PARALLELOGRAM: u32 = 24u;
const SDF_EQUILATERAL_TRIANGLE: u32 = 25u;
const SDF_ISOSCELES_TRIANGLE: u32 = 26u;
const SDF_UNEVEN_CAPSULE: u32 = 27u;
const SDF_OCTOGON: u32 = 28u;
const SDF_HEXAGRAM: u32 = 29u;
const SDF_PENTAGRAM: u32 = 30u;
const SDF_CUT_DISK: u32 = 31u;
const SDF_HORSESHOE: u32 = 32u;
const SDF_VESICA: u32 = 33u;
const SDF_ORIENTED_VESICA: u32 = 34u;
const SDF_ROUNDED_CROSS: u32 = 35u;
const SDF_PARABOLA: u32 = 36u;
const SDF_BLOBBY_CROSS: u32 = 37u;
const SDF_TUNNEL: u32 = 38u;
const SDF_STAIRS: u32 = 39u;
const SDF_QUADRATIC_CIRCLE: u32 = 40u;
const SDF_HYPERBOLA: u32 = 41u;
const SDF_COOL_S: u32 = 42u;
const SDF_CIRCLE_WAVE: u32 = 43u;
const SDF_COLOR_WHEEL: u32 = 44u;
const SDF_TEXT_GLYPH: u32 = 64u;
const SDF_ROTATED_GLYPH: u32 = 65u;
const SDF_SPHERE_3D: u32 = 100u;
const SDF_BOX_3D: u32 = 101u;
const SDF_BOX_FRAME_3D: u32 = 102u;
const SDF_TORUS_3D: u32 = 103u;
const SDF_CAPPED_TORUS_3D: u32 = 104u;
const SDF_CYLINDER_3D: u32 = 105u;
const SDF_CAPPED_CYLINDER_3D: u32 = 106u;
const SDF_ROUNDED_CYLINDER_3D: u32 = 107u;
const SDF_VERTICAL_CAPSULE_3D: u32 = 108u;
const SDF_CONE_3D: u32 = 109u;
const SDF_CAPPED_CONE_3D: u32 = 110u;
const SDF_ROUND_CONE_3D: u32 = 111u;
const SDF_PLANE_3D: u32 = 112u;
const SDF_HEX_PRISM_3D: u32 = 113u;
const SDF_TRI_PRISM_3D: u32 = 114u;
const SDF_OCTAHEDRON_3D: u32 = 115u;
const SDF_PYRAMID_3D: u32 = 116u;
const SDF_ELLIPSOID_3D: u32 = 117u;
const SDF_RHOMBUS_3D: u32 = 118u;
const SDF_LINK_3D: u32 = 119u;
const SDF_PLOT: u32 = 128u;
const SDF_IMAGE: u32 = 129u;

fn evalSDF(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);

    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let radius = cardStorage[primOffset + 4u];
            return sdCircle(p, center, radius);
        }
        case SDF_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let round = cardStorage[primOffset + 9u];
            return sdBox(p, center, halfSize, round);
        }
        case SDF_SEGMENT: {
            let p0 = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let p1 = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdSegment(p, p0, p1);
        }
        case SDF_TRIANGLE: {
            let v0 = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
            return sdTriangle(p, v0, v1, v2);
        }
        case SDF_BEZIER2: {
            let v0 = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
            return sdBezier2(p, v0, v1, v2);
        }
        case SDF_BEZIER3: {
            let v0 = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
            let v3 = vec2<f32>(cardStorage[primOffset + 8u], cardStorage[primOffset + 9u]);
            return sdBezier3(p, v0, v1, v2, v3);
        }
        case SDF_ELLIPSE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let radii = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdEllipse(p, center, radii);
        }
        case SDF_ARC: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let sc = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let ra = cardStorage[primOffset + 6u];
            let rb = cardStorage[primOffset + 7u];
            return sdArc(p, center, sc, ra, rb);
        }
        case SDF_ROUNDED_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let radii = vec4<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u], cardStorage[primOffset + 8u], cardStorage[primOffset + 9u]);
            return sdRoundedBox(p, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdRhombus(p, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdPentagon(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdHexagon(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_STAR: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdStar(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_PIE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let c = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdPie(p, center, c, cardStorage[primOffset + 6u]);
        }
        case SDF_RING: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let n = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdRing(p, center, n, cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_HEART: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdHeart(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdCross(p, center, b, cardStorage[primOffset + 6u]);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdRoundedX(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdCapsule(p, a, b, cardStorage[primOffset + 6u]);
        }
        case SDF_MOON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdMoon(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_EGG: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdEgg(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdChamferBox(p, center, halfSize, cardStorage[primOffset + 6u]);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdOrientedBox(p, a, b, cardStorage[primOffset + 6u]);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdTrapezoid(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdParallelogram(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdEquilateralTriangle(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let q = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdTriangleIsosceles(p, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdUnevenCapsule(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdOctogon(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdHexagram(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdPentagram(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdCutDisk(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let c = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let w = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            return sdHorseshoe(p, center, c, cardStorage[primOffset + 6u], w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdVesica(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdOrientedVesica(p, a, b, cardStorage[primOffset + 6u]);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdRoundedCross(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdParabola(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdBlobbyCross(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let wh = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdTunnel(p, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let wh = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdStairs(p, center, wh, cardStorage[primOffset + 6u]);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdQuadraticCircle(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdHyperbola(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdCoolS(p, center, cardStorage[primOffset + 4u]);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            return sdCircleWave(p, center, cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
        }
        case SDF_COLOR_WHEEL: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let d = p - center;
            let dist = length(d);
            let ringDist = abs(dist - (cardStorage[primOffset + 5u] + cardStorage[primOffset + 4u]) * 0.5) - (cardStorage[primOffset + 4u] - cardStorage[primOffset + 5u]) * 0.5;
            return ringDist;
        }
        default: {
            return 1e10;
        }
    }
}

fn evalSDF3D(primOffset: u32, p: vec3<f32>) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);

    switch (primType) {
        case SDF_SPHERE_3D: {
            let pos = vec3<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdSphere3D(p - pos, cardStorage[primOffset + 5u]);
        }
        case SDF_BOX_3D: {
            let pos = vec3<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdBox3D(p - pos, vec3<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]));
        }
        default: { return 1e10; }
    }
}

fn primColors(primOffset: u32) -> vec4<u32> {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SEGMENT: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER2: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER3: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ARC: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RHOMBUS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAR: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PIE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RING: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEART: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_X: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_MOON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EGG: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CHAMFER_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRAPEZOID: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARALLELOGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_UNEVEN_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTOGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CUT_DISK: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HORSESHOE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VESICA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_VESICA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARABOLA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BLOBBY_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TUNNEL: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAIRS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_QUADRATIC_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HYPERBOLA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COOL_S: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CIRCLE_WAVE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COLOR_WHEEL: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TEXT_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROTATED_GLYPH: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SPHERE_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TORUS_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CYLINDER_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VERTICAL_CAPSULE_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPPED_CONE_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTAHEDRON_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PYRAMID_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSOID_3D: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        default: { return vec4<u32>(0u); }
    }
}

fn primStrokeWidth(primOffset: u32) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: { return cardStorage[primOffset + 7u]; }
        case SDF_BOX: { return cardStorage[primOffset + 8u]; }
        case SDF_SEGMENT: { return cardStorage[primOffset + 8u]; }
        case SDF_TRIANGLE: { return cardStorage[primOffset + 10u]; }
        case SDF_BEZIER2: { return cardStorage[primOffset + 10u]; }
        case SDF_BEZIER3: { return cardStorage[primOffset + 12u]; }
        case SDF_ELLIPSE: { return cardStorage[primOffset + 8u]; }
        case SDF_ARC: { return cardStorage[primOffset + 10u]; }
        case SDF_ROUNDED_BOX: { return cardStorage[primOffset + 12u]; }
        case SDF_RHOMBUS: { return cardStorage[primOffset + 8u]; }
        case SDF_PENTAGON: { return cardStorage[primOffset + 7u]; }
        case SDF_HEXAGON: { return cardStorage[primOffset + 7u]; }
        case SDF_STAR: { return cardStorage[primOffset + 9u]; }
        case SDF_PIE: { return cardStorage[primOffset + 9u]; }
        case SDF_RING: { return cardStorage[primOffset + 10u]; }
        case SDF_HEART: { return cardStorage[primOffset + 7u]; }
        case SDF_CROSS: { return cardStorage[primOffset + 9u]; }
        case SDF_ROUNDED_X: { return cardStorage[primOffset + 8u]; }
        case SDF_CAPSULE: { return cardStorage[primOffset + 9u]; }
        case SDF_MOON: { return cardStorage[primOffset + 9u]; }
        case SDF_EGG: { return cardStorage[primOffset + 8u]; }
        case SDF_CHAMFER_BOX: { return cardStorage[primOffset + 9u]; }
        case SDF_ORIENTED_BOX: { return cardStorage[primOffset + 9u]; }
        case SDF_TRAPEZOID: { return cardStorage[primOffset + 9u]; }
        case SDF_PARALLELOGRAM: { return cardStorage[primOffset + 9u]; }
        case SDF_EQUILATERAL_TRIANGLE: { return cardStorage[primOffset + 7u]; }
        case SDF_ISOSCELES_TRIANGLE: { return cardStorage[primOffset + 8u]; }
        case SDF_UNEVEN_CAPSULE: { return cardStorage[primOffset + 9u]; }
        case SDF_OCTOGON: { return cardStorage[primOffset + 7u]; }
        case SDF_HEXAGRAM: { return cardStorage[primOffset + 7u]; }
        case SDF_PENTAGRAM: { return cardStorage[primOffset + 7u]; }
        case SDF_CUT_DISK: { return cardStorage[primOffset + 8u]; }
        case SDF_HORSESHOE: { return cardStorage[primOffset + 11u]; }
        case SDF_VESICA: { return cardStorage[primOffset + 8u]; }
        case SDF_ORIENTED_VESICA: { return cardStorage[primOffset + 9u]; }
        case SDF_ROUNDED_CROSS: { return cardStorage[primOffset + 7u]; }
        case SDF_PARABOLA: { return cardStorage[primOffset + 7u]; }
        case SDF_BLOBBY_CROSS: { return cardStorage[primOffset + 7u]; }
        case SDF_TUNNEL: { return cardStorage[primOffset + 8u]; }
        case SDF_STAIRS: { return cardStorage[primOffset + 9u]; }
        case SDF_QUADRATIC_CIRCLE: { return cardStorage[primOffset + 7u]; }
        case SDF_HYPERBOLA: { return cardStorage[primOffset + 8u]; }
        case SDF_COOL_S: { return cardStorage[primOffset + 7u]; }
        case SDF_CIRCLE_WAVE: { return cardStorage[primOffset + 8u]; }
        case SDF_COLOR_WHEEL: { return cardStorage[primOffset + 12u]; }
        case SDF_TEXT_GLYPH: { return cardStorage[primOffset + 9u]; }
        case SDF_ROTATED_GLYPH: { return cardStorage[primOffset + 12u]; }
        case SDF_SPHERE_3D: { return cardStorage[primOffset + 8u]; }
        case SDF_BOX_3D: { return cardStorage[primOffset + 10u]; }
        case SDF_TORUS_3D: { return cardStorage[primOffset + 9u]; }
        case SDF_CYLINDER_3D: { return cardStorage[primOffset + 9u]; }
        case SDF_VERTICAL_CAPSULE_3D: { return cardStorage[primOffset + 9u]; }
        case SDF_CAPPED_CONE_3D: { return cardStorage[primOffset + 10u]; }
        case SDF_OCTAHEDRON_3D: { return cardStorage[primOffset + 8u]; }
        case SDF_PYRAMID_3D: { return cardStorage[primOffset + 8u]; }
        case SDF_ELLIPSOID_3D: { return cardStorage[primOffset + 10u]; }
        default: { return 0.0; }
    }
}
