// Shader glyph: HG SDF (codepoint 1114104 / U+10FFF8)
// Mercury's HG_SDF - GLSL library for building signed distance bounds
// Converted to WGSL. Original: https://mercury.sexy/hg_sdf
// Released under MIT License OR CC-BY-NC-4.0 by Mercury Demogroup
// Copyright (c) 2011-2021 Mercury Demogroup

////////////////////////////////////////////////////////////////
//
//             HELPER FUNCTIONS
//
////////////////////////////////////////////////////////////////

const HG_PI: f32 = 3.14159265;
const HG_TAU: f32 = 6.28318530;
const HG_PHI: f32 = 1.61803398;

fn hg_mod_f(x: f32, y: f32) -> f32 {
    return x - y * floor(x / y);
}

fn hg_mod_v2(x: vec2<f32>, y: vec2<f32>) -> vec2<f32> {
    return x - y * floor(x / y);
}

fn hg_mod_v3(x: vec3<f32>, y: vec3<f32>) -> vec3<f32> {
    return x - y * floor(x / y);
}

fn hg_sgn(x: f32) -> f32 {
    return select(1.0, -1.0, x < 0.0);
}

fn hg_sgn2(v: vec2<f32>) -> vec2<f32> {
    return vec2<f32>(select(1.0, -1.0, v.x < 0.0), select(1.0, -1.0, v.y < 0.0));
}

fn hg_vmax2(v: vec2<f32>) -> f32 {
    return max(v.x, v.y);
}

fn hg_vmax3(v: vec3<f32>) -> f32 {
    return max(max(v.x, v.y), v.z);
}

fn hg_vmax4(v: vec4<f32>) -> f32 {
    return max(max(v.x, v.y), max(v.z, v.w));
}

fn hg_vmin2(v: vec2<f32>) -> f32 {
    return min(v.x, v.y);
}

fn hg_vmin3(v: vec3<f32>) -> f32 {
    return min(min(v.x, v.y), v.z);
}

fn hg_vmin4(v: vec4<f32>) -> f32 {
    return min(min(v.x, v.y), min(v.z, v.w));
}

////////////////////////////////////////////////////////////////
//
//             PRIMITIVE DISTANCE FUNCTIONS
//
////////////////////////////////////////////////////////////////

fn hg_fSphere(p: vec3<f32>, r: f32) -> f32 {
    return length(p) - r;
}

fn hg_fPlane(p: vec3<f32>, n: vec3<f32>, distanceFromOrigin: f32) -> f32 {
    return dot(p, n) + distanceFromOrigin;
}

fn hg_fBoxCheap(p: vec3<f32>, b: vec3<f32>) -> f32 {
    return hg_vmax3(abs(p) - b);
}

fn hg_fBox(p: vec3<f32>, b: vec3<f32>) -> f32 {
    let d = abs(p) - b;
    return length(max(d, vec3<f32>(0.0))) + hg_vmax3(min(d, vec3<f32>(0.0)));
}

fn hg_fBox2Cheap(p: vec2<f32>, b: vec2<f32>) -> f32 {
    return hg_vmax2(abs(p) - b);
}

fn hg_fBox2(p: vec2<f32>, b: vec2<f32>) -> f32 {
    let d = abs(p) - b;
    return length(max(d, vec2<f32>(0.0))) + hg_vmax2(min(d, vec2<f32>(0.0)));
}

fn hg_fCorner(p: vec2<f32>) -> f32 {
    return length(max(p, vec2<f32>(0.0))) + hg_vmax2(min(p, vec2<f32>(0.0)));
}

fn hg_fCylinder(p: vec3<f32>, r: f32, height: f32) -> f32 {
    var d = length(p.xz) - r;
    d = max(d, abs(p.y) - height);
    return d;
}

fn hg_fCapsule(p: vec3<f32>, r: f32, c: f32) -> f32 {
    return mix(length(p.xz) - r, length(vec3<f32>(p.x, abs(p.y) - c, p.z)) - r, step(c, abs(p.y)));
}

fn hg_fLineSegment(p: vec3<f32>, a: vec3<f32>, b: vec3<f32>) -> f32 {
    let ab = b - a;
    let t = clamp(dot(p - a, ab) / dot(ab, ab), 0.0, 1.0);
    return length((ab * t + a) - p);
}

fn hg_fCapsuleLine(p: vec3<f32>, a: vec3<f32>, b: vec3<f32>, r: f32) -> f32 {
    return hg_fLineSegment(p, a, b) - r;
}

fn hg_fTorus(p: vec3<f32>, smallRadius: f32, largeRadius: f32) -> f32 {
    return length(vec2<f32>(length(p.xz) - largeRadius, p.y)) - smallRadius;
}

fn hg_fCircle(p: vec3<f32>, r: f32) -> f32 {
    let l = length(p.xz) - r;
    return length(vec2<f32>(p.y, l));
}

fn hg_fDisc(p: vec3<f32>, r: f32) -> f32 {
    let l = length(p.xz) - r;
    if (l < 0.0) {
        return abs(p.y);
    }
    return length(vec2<f32>(p.y, l));
}

fn hg_fHexagonCircumcircle(p: vec3<f32>, h: vec2<f32>) -> f32 {
    let q = abs(p);
    return max(q.y - h.y, max(q.x * sqrt(3.0) * 0.5 + q.z * 0.5, q.z) - h.x);
}

fn hg_fHexagonIncircle(p: vec3<f32>, h: vec2<f32>) -> f32 {
    return hg_fHexagonCircumcircle(p, vec2<f32>(h.x * sqrt(3.0) * 0.5, h.y));
}

fn hg_fCone(p: vec3<f32>, radius: f32, height: f32) -> f32 {
    let q = vec2<f32>(length(p.xz), p.y);
    let tip = q - vec2<f32>(0.0, height);
    let mantleDir = normalize(vec2<f32>(height, radius));
    let mantle = dot(tip, mantleDir);
    var d = max(mantle, -q.y);
    let projected = dot(tip, vec2<f32>(mantleDir.y, -mantleDir.x));

    if (q.y > height && projected < 0.0) {
        d = max(d, length(tip));
    }

    if (q.x > radius && projected > length(vec2<f32>(height, radius))) {
        d = max(d, length(q - vec2<f32>(radius, 0.0)));
    }
    return d;
}

////////////////////////////////////////////////////////////////
//
//             DOMAIN MANIPULATION OPERATORS
//
////////////////////////////////////////////////////////////////

fn hg_pR(p: vec2<f32>, a: f32) -> vec2<f32> {
    return cos(a) * p + sin(a) * vec2<f32>(p.y, -p.x);
}

fn hg_pR45(p: vec2<f32>) -> vec2<f32> {
    return (p + vec2<f32>(p.y, -p.x)) * sqrt(0.5);
}

struct HgMod1Result {
    p: f32,
    cell: f32,
}

fn hg_pMod1(p: f32, size: f32) -> HgMod1Result {
    let halfsize = size * 0.5;
    let c = floor((p + halfsize) / size);
    let mp = hg_mod_f(p + halfsize, size) - halfsize;
    return HgMod1Result(mp, c);
}

struct HgModPolarResult {
    p: vec2<f32>,
    cell: f32,
}

fn hg_pModPolar(p: vec2<f32>, repetitions: f32) -> HgModPolarResult {
    let angle = HG_TAU / repetitions;
    var a = atan2(p.y, p.x) + angle / 2.0;
    let r = length(p);
    var c = floor(a / angle);
    a = hg_mod_f(a, angle) - angle / 2.0;
    let result_p = vec2<f32>(cos(a), sin(a)) * r;
    if (abs(c) >= repetitions / 2.0) {
        c = abs(c);
    }
    return HgModPolarResult(result_p, c);
}

struct HgMirrorResult {
    p: f32,
    s: f32,
}

fn hg_pMirror(p: f32, dist: f32) -> HgMirrorResult {
    let s = hg_sgn(p);
    return HgMirrorResult(abs(p) - dist, s);
}

struct HgReflectResult {
    p: vec3<f32>,
    s: f32,
}

fn hg_pReflect(p: vec3<f32>, planeNormal: vec3<f32>, offset: f32) -> HgReflectResult {
    let t = dot(p, planeNormal) + offset;
    var rp = p;
    if (t < 0.0) {
        rp = p - (2.0 * t) * planeNormal;
    }
    return HgReflectResult(rp, hg_sgn(t));
}

////////////////////////////////////////////////////////////////
//
//             OBJECT COMBINATION OPERATORS
//
////////////////////////////////////////////////////////////////

fn hg_fOpUnionChamfer(a: f32, b: f32, r: f32) -> f32 {
    return min(min(a, b), (a - r + b) * sqrt(0.5));
}

fn hg_fOpIntersectionChamfer(a: f32, b: f32, r: f32) -> f32 {
    return max(max(a, b), (a + r + b) * sqrt(0.5));
}

fn hg_fOpDifferenceChamfer(a: f32, b: f32, r: f32) -> f32 {
    return hg_fOpIntersectionChamfer(a, -b, r);
}

fn hg_fOpUnionRound(a: f32, b: f32, r: f32) -> f32 {
    let u = max(vec2<f32>(r - a, r - b), vec2<f32>(0.0));
    return max(r, min(a, b)) - length(u);
}

fn hg_fOpIntersectionRound(a: f32, b: f32, r: f32) -> f32 {
    let u = max(vec2<f32>(r + a, r + b), vec2<f32>(0.0));
    return min(-r, max(a, b)) + length(u);
}

fn hg_fOpDifferenceRound(a: f32, b: f32, r: f32) -> f32 {
    return hg_fOpIntersectionRound(a, -b, r);
}

fn hg_fOpUnionStairs(a: f32, b: f32, r: f32, n: f32) -> f32 {
    let s = r / n;
    let u = b - r;
    return min(min(a, b), 0.5 * (u + a + abs(hg_mod_f(u - a + s, 2.0 * s) - s)));
}

fn hg_fOpIntersectionStairs(a: f32, b: f32, r: f32, n: f32) -> f32 {
    return -hg_fOpUnionStairs(-a, -b, r, n);
}

fn hg_fOpDifferenceStairs(a: f32, b: f32, r: f32, n: f32) -> f32 {
    return -hg_fOpUnionStairs(-a, b, r, n);
}

fn hg_fOpUnionSoft(a: f32, b: f32, r: f32) -> f32 {
    let e = max(r - abs(a - b), 0.0);
    return min(a, b) - e * e * 0.25 / r;
}

fn hg_fOpPipe(a: f32, b: f32, r: f32) -> f32 {
    return length(vec2<f32>(a, b)) - r;
}

fn hg_fOpEngrave(a: f32, b: f32, r: f32) -> f32 {
    return max(a, (a + r - abs(b)) * sqrt(0.5));
}

fn hg_fOpGroove(a: f32, b: f32, ra: f32, rb: f32) -> f32 {
    return max(a, min(a + ra, rb - abs(b)));
}

fn hg_fOpTongue(a: f32, b: f32, ra: f32, rb: f32) -> f32 {
    return min(a, max(a - ra, abs(b) - rb));
}

////////////////////////////////////////////////////////////////
//
//             DEMO SCENE
//
////////////////////////////////////////////////////////////////

fn hg_scene(p_in: vec3<f32>, time: f32) -> f32 {
    var p = p_in;

    // Slow tumble rotation
    let pxz = hg_pR(p.xz, time * 0.4);
    p = vec3<f32>(pxz.x, p.y, pxz.y);
    let pxy = hg_pR(p.xy, time * 0.25);
    p = vec3<f32>(pxy.x, pxy.y, p.z);

    // Breathing factor
    let breath = sin(time * 0.7) * 0.08 + 1.0;

    // Rounded box intersected with sphere
    var d = hg_fBox(p, vec3<f32>(1.0) * breath);
    let sph = hg_fSphere(p, 1.3 * breath);
    d = hg_fOpIntersectionRound(d, sph, 0.15);

    // Subtract three perpendicular cylinders
    let cx = hg_fCylinder(p, 0.35, 3.0);
    let cy = hg_fCylinder(vec3<f32>(p.y, p.x, p.z), 0.35, 3.0);
    let cz = hg_fCylinder(vec3<f32>(p.z, p.y, p.x), 0.35, 3.0);
    d = hg_fOpDifferenceRound(d, min(cx, min(cy, cz)), 0.08);

    // Orbiting torus
    var tp = p;
    let tpyz = hg_pR(tp.yz, time * 0.5);
    tp = vec3<f32>(tp.x, tpyz.x, tpyz.y);
    let torus = hg_fTorus(tp, 0.1, 1.7);
    d = hg_fOpUnionRound(d, torus, 0.04);

    // Second torus on different axis
    var tp2 = p;
    let tp2xz = hg_pR(tp2.xz, time * 0.3 + 1.0);
    tp2 = vec3<f32>(tp2xz.x, tp2.y, tp2xz.y);
    let torus2 = hg_fTorus(vec3<f32>(tp2.y, tp2.x, tp2.z), 0.08, 1.8);
    d = hg_fOpUnionRound(d, torus2, 0.04);

    return d;
}

fn hg_getNormal(p: vec3<f32>, time: f32) -> vec3<f32> {
    let e = 0.001;
    return normalize(vec3<f32>(
        hg_scene(p + vec3<f32>(e, 0.0, 0.0), time) - hg_scene(p - vec3<f32>(e, 0.0, 0.0), time),
        hg_scene(p + vec3<f32>(0.0, e, 0.0), time) - hg_scene(p - vec3<f32>(0.0, e, 0.0), time),
        hg_scene(p + vec3<f32>(0.0, 0.0, e), time) - hg_scene(p - vec3<f32>(0.0, 0.0, e), time)
    ));
}

fn hg_calcAO(p: vec3<f32>, n: vec3<f32>, time: f32) -> f32 {
    var ao = 0.0;
    for (var i = 1; i <= 5; i++) {
        let dist = f32(i) * 0.08;
        ao += (dist - hg_scene(p + n * dist, time)) / (1.0 + dist);
    }
    return clamp(1.0 - ao * 2.0, 0.0, 1.0);
}

fn shaderGlyph_1114104(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    let uv = (pixelPos - screenSize * 0.5) / screenSize.y;

    // Camera
    let ro = vec3<f32>(0.0, 0.0, -4.5);
    let rd = normalize(vec3<f32>(uv, 1.5));

    // Raymarch
    var t = 0.0;
    for (var i = 0; i < 80; i++) {
        let d = hg_scene(ro + rd * t, time);
        if (abs(d) < 0.001 || t > 20.0) {
            break;
        }
        t += d;
    }

    // Background
    if (t > 20.0) {
        let bg_grad = 0.03 + 0.02 * (uv.y + 0.5);
        return vec3<f32>(bg_grad, bg_grad, bg_grad * 1.3);
    }

    let pos = ro + rd * t;
    let nor = hg_getNormal(pos, time);
    let ao = hg_calcAO(pos, nor, time);

    // Lighting
    let lightDir = normalize(vec3<f32>(1.0, 2.0, -1.5));
    let diff = max(dot(nor, lightDir), 0.0);
    let spec = pow(max(dot(reflect(-lightDir, nor), -rd), 0.0), 32.0);
    let amb = 0.15;
    let fre = pow(1.0 - abs(dot(nor, -rd)), 3.0);

    // Color from normal direction
    let baseCol = vec3<f32>(0.55, 0.35, 0.15) + nor * vec3<f32>(0.15, 0.2, 0.25);

    var col = baseCol * (diff + amb) +
              vec3<f32>(1.0, 0.9, 0.7) * spec * 0.8 +
              vec3<f32>(0.2, 0.4, 0.6) * fre * 0.3;
    col = col * ao;

    return sqrt(clamp(col, vec3<f32>(0.0), vec3<f32>(1.0)));
}
