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
const SDF_POLYGON: u32 = 130u;
const SDF_POLYGON_GROUP: u32 = 131u;
const SDF_LINEAR_GRADIENT_BOX: u32 = 132u;
const SDF_LINEAR_GRADIENT_CIRCLE: u32 = 133u;
const SDF_RADIAL_GRADIENT_CIRCLE: u32 = 134u;

fn evalSDF(primOffset: u32, p: vec2<f32>) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed in u32) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(cardStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * grid.cellSize.x, gridOffsetY * grid.cellSize.y);
    let pAdj = p - pixelOffset;

    switch (primType) {
        case SDF_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let radius = cardStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let round = cardStorage[primOffset + 10u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_SEGMENT: {
            let p0 = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let p1 = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdSegment(pAdj, p0, p1);
        }
        case SDF_TRIANGLE: {
            let v0 = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            return sdTriangle(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER2: {
            let v0 = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            return sdBezier2(pAdj, v0, v1, v2);
        }
        case SDF_BEZIER3: {
            let v0 = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let v1 = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let v2 = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            let v3 = vec2<f32>(cardStorage[primOffset + 9u], cardStorage[primOffset + 10u]);
            return sdBezier3(pAdj, v0, v1, v2, v3);
        }
        case SDF_ELLIPSE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let radii = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdEllipse(pAdj, center, radii);
        }
        case SDF_ARC: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let sc = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let ra = cardStorage[primOffset + 7u];
            let rb = cardStorage[primOffset + 8u];
            return sdArc(pAdj, center, sc, ra, rb);
        }
        case SDF_ROUNDED_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let radii = vec4<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u], cardStorage[primOffset + 9u], cardStorage[primOffset + 10u]);
            return sdRoundedBox(pAdj, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let b = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdRhombus(pAdj, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdPentagon(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdHexagon(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_STAR: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdStar(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_PIE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let c = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdPie(pAdj, center, c, cardStorage[primOffset + 7u]);
        }
        case SDF_RING: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let n = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdRing(pAdj, center, n, cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
        }
        case SDF_HEART: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdHeart(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let b = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdCross(pAdj, center, b, cardStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdRoundedX(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let b = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdCapsule(pAdj, a, b, cardStorage[primOffset + 7u]);
        }
        case SDF_MOON: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdMoon(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_EGG: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdEgg(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdChamferBox(pAdj, center, halfSize, cardStorage[primOffset + 7u]);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let b = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdOrientedBox(pAdj, a, b, cardStorage[primOffset + 7u]);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdTrapezoid(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdParallelogram(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdEquilateralTriangle(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let q = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdTriangleIsosceles(pAdj, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdUnevenCapsule(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdOctogon(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdHexagram(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdPentagram(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdCutDisk(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let c = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let w = vec2<f32>(cardStorage[primOffset + 8u], cardStorage[primOffset + 9u]);
            return sdHorseshoe(pAdj, center, c, cardStorage[primOffset + 7u], w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdVesica(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let b = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdOrientedVesica(pAdj, a, b, cardStorage[primOffset + 7u]);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdRoundedCross(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdParabola(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdBlobbyCross(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let wh = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdTunnel(pAdj, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let wh = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            return sdStairs(pAdj, center, wh, cardStorage[primOffset + 7u]);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdQuadraticCircle(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdHyperbola(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdCoolS(pAdj, center, cardStorage[primOffset + 5u]);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            return sdCircleWave(pAdj, center, cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
        }
        case SDF_COLOR_WHEEL: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let d = pAdj - center;
            let dist = length(d);
            let ringDist = abs(dist - (cardStorage[primOffset + 6u] + cardStorage[primOffset + 5u]) * 0.5) - (cardStorage[primOffset + 5u] - cardStorage[primOffset + 6u]) * 0.5;
            return ringDist;
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 5u], cardStorage[primOffset + 6u]);
            let round = cardStorage[primOffset + 15u];
            return sdBox(pAdj, center, halfSize, round);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let radius = cardStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 3u], cardStorage[primOffset + 4u]);
            let radius = cardStorage[primOffset + 5u];
            return sdCircle(pAdj, center, radius);
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
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_SEGMENT: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER2: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BEZIER3: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 12u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ELLIPSE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ARC: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 12u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RHOMBUS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAR: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PIE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_RING: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEART: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_X: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_MOON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EGG: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CHAMFER_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TRAPEZOID: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARALLELOGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_UNEVEN_CAPSULE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_OCTOGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HEXAGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PENTAGRAM: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CUT_DISK: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HORSESHOE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_VESICA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ORIENTED_VESICA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_ROUNDED_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_PARABOLA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_BLOBBY_CROSS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_TUNNEL: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_STAIRS: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_QUADRATIC_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_HYPERBOLA: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COOL_S: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 6u]),
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_CIRCLE_WAVE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 7u]),
                bitcast<u32>(cardStorage[primOffset + 8u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_COLOR_WHEEL: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 12u]),
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
        case SDF_POLYGON: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 3u]),
                bitcast<u32>(cardStorage[primOffset + 4u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_POLYGON_GROUP: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 4u]),
                bitcast<u32>(cardStorage[primOffset + 5u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                0u);
        }
        case SDF_LINEAR_GRADIENT_BOX: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 13u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 10u]),
                bitcast<u32>(cardStorage[primOffset + 12u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            return vec4<u32>(
                bitcast<u32>(cardStorage[primOffset + 9u]),
                bitcast<u32>(cardStorage[primOffset + 11u]),
                bitcast<u32>(cardStorage[primOffset + 1u]),
                1u);  // flag=1 indicates gradient type
        }
        default: { return vec4<u32>(0u); }
    }
}

fn isGradientPrim(primType: u32) -> bool {
    switch (primType) {
        case SDF_LINEAR_GRADIENT_BOX: { return true; }
        case SDF_LINEAR_GRADIENT_CIRCLE: { return true; }
        case SDF_RADIAL_GRADIENT_CIRCLE: { return true; }
        default: { return false; }
    }
}

fn evalGradientFillColor(primOffset: u32, p: vec2<f32>) -> vec4<f32> {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    // Read grid offset (u16,u16 packed) and convert to pixel offset
    let gridOffsetPacked = bitcast<u32>(cardStorage[primOffset + 2u]);
    let gridOffsetX = f32(gridOffsetPacked & 0xFFFFu);
    let gridOffsetY = f32(gridOffsetPacked >> 16u);
    let pixelOffset = vec2<f32>(gridOffsetX * grid.cellSize.x, gridOffsetY * grid.cellSize.y);
    let pAdj = p - pixelOffset;
    switch (primType) {
        case SDF_LINEAR_GRADIENT_BOX: {
            let gStart = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            let gEnd = vec2<f32>(cardStorage[primOffset + 9u], cardStorage[primOffset + 10u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 11u]));
            let c2 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 12u]));
            return mix(c1, c2, t);
        }
        case SDF_LINEAR_GRADIENT_CIRCLE: {
            let gStart = vec2<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
            let gEnd = vec2<f32>(cardStorage[primOffset + 8u], cardStorage[primOffset + 9u]);
            let gDir = gEnd - gStart;
            let gLen = length(gDir);
            let t = clamp(dot(pAdj - gStart, gDir) / (gLen * gLen), 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 10u]));
            let c2 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 11u]));
            return mix(c1, c2, t);
        }
        case SDF_RADIAL_GRADIENT_CIRCLE: {
            let gCenter = vec2<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u]);
            let gRadius = cardStorage[primOffset + 8u];
            let t = clamp(length(pAdj - gCenter) / gRadius, 0.0, 1.0);
            let c1 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 9u]));
            let c2 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + 10u]));
            return mix(c1, c2, t);
        }
        default: { return vec4<f32>(0.0); }
    }
}

fn primStrokeWidth(primOffset: u32) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    switch (primType) {
        case SDF_CIRCLE: { return cardStorage[primOffset + 8u]; }
        case SDF_BOX: { return cardStorage[primOffset + 9u]; }
        case SDF_SEGMENT: { return cardStorage[primOffset + 9u]; }
        case SDF_TRIANGLE: { return cardStorage[primOffset + 11u]; }
        case SDF_BEZIER2: { return cardStorage[primOffset + 11u]; }
        case SDF_BEZIER3: { return cardStorage[primOffset + 13u]; }
        case SDF_ELLIPSE: { return cardStorage[primOffset + 9u]; }
        case SDF_ARC: { return cardStorage[primOffset + 11u]; }
        case SDF_ROUNDED_BOX: { return cardStorage[primOffset + 13u]; }
        case SDF_RHOMBUS: { return cardStorage[primOffset + 9u]; }
        case SDF_PENTAGON: { return cardStorage[primOffset + 8u]; }
        case SDF_HEXAGON: { return cardStorage[primOffset + 8u]; }
        case SDF_STAR: { return cardStorage[primOffset + 10u]; }
        case SDF_PIE: { return cardStorage[primOffset + 10u]; }
        case SDF_RING: { return cardStorage[primOffset + 11u]; }
        case SDF_HEART: { return cardStorage[primOffset + 8u]; }
        case SDF_CROSS: { return cardStorage[primOffset + 10u]; }
        case SDF_ROUNDED_X: { return cardStorage[primOffset + 9u]; }
        case SDF_CAPSULE: { return cardStorage[primOffset + 10u]; }
        case SDF_MOON: { return cardStorage[primOffset + 10u]; }
        case SDF_EGG: { return cardStorage[primOffset + 9u]; }
        case SDF_CHAMFER_BOX: { return cardStorage[primOffset + 10u]; }
        case SDF_ORIENTED_BOX: { return cardStorage[primOffset + 10u]; }
        case SDF_TRAPEZOID: { return cardStorage[primOffset + 10u]; }
        case SDF_PARALLELOGRAM: { return cardStorage[primOffset + 10u]; }
        case SDF_EQUILATERAL_TRIANGLE: { return cardStorage[primOffset + 8u]; }
        case SDF_ISOSCELES_TRIANGLE: { return cardStorage[primOffset + 9u]; }
        case SDF_UNEVEN_CAPSULE: { return cardStorage[primOffset + 10u]; }
        case SDF_OCTOGON: { return cardStorage[primOffset + 8u]; }
        case SDF_HEXAGRAM: { return cardStorage[primOffset + 8u]; }
        case SDF_PENTAGRAM: { return cardStorage[primOffset + 8u]; }
        case SDF_CUT_DISK: { return cardStorage[primOffset + 9u]; }
        case SDF_HORSESHOE: { return cardStorage[primOffset + 12u]; }
        case SDF_VESICA: { return cardStorage[primOffset + 9u]; }
        case SDF_ORIENTED_VESICA: { return cardStorage[primOffset + 10u]; }
        case SDF_ROUNDED_CROSS: { return cardStorage[primOffset + 8u]; }
        case SDF_PARABOLA: { return cardStorage[primOffset + 8u]; }
        case SDF_BLOBBY_CROSS: { return cardStorage[primOffset + 8u]; }
        case SDF_TUNNEL: { return cardStorage[primOffset + 9u]; }
        case SDF_STAIRS: { return cardStorage[primOffset + 10u]; }
        case SDF_QUADRATIC_CIRCLE: { return cardStorage[primOffset + 8u]; }
        case SDF_HYPERBOLA: { return cardStorage[primOffset + 9u]; }
        case SDF_COOL_S: { return cardStorage[primOffset + 8u]; }
        case SDF_CIRCLE_WAVE: { return cardStorage[primOffset + 9u]; }
        case SDF_COLOR_WHEEL: { return cardStorage[primOffset + 13u]; }
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
        case SDF_POLYGON: { return cardStorage[primOffset + 5u]; }
        case SDF_POLYGON_GROUP: { return cardStorage[primOffset + 6u]; }
        case SDF_LINEAR_GRADIENT_BOX: { return cardStorage[primOffset + 14u]; }
        case SDF_LINEAR_GRADIENT_CIRCLE: { return cardStorage[primOffset + 13u]; }
        case SDF_RADIAL_GRADIENT_CIRCLE: { return cardStorage[primOffset + 12u]; }
        default: { return 0.0; }
    }
}
