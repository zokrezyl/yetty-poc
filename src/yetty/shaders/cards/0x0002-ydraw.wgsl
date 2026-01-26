// Shader glyph: YDraw widget (codepoint 1048578 / U+100002)
// GPU-accelerated SDF primitives + text rendering with BVH acceleration
//
// =============================================================================
// DESIGN: BVH-Accelerated SDF Rendering
// =============================================================================
//
// This shader renders compositions of:
//   - SDF primitives (circle, box, segment, bezier, etc.)
//   - MSDF text glyphs (TODO: add font texture binding)
//
// BVH (Bounding Volume Hierarchy) is used for spatial acceleration:
//   - Each primitive has an AABB (axis-aligned bounding box)
//   - BVH tree allows skipping entire groups of primitives
//   - Reduces per-pixel evaluation from O(N) to O(log N)
//
// =============================================================================
// Per-cell encoding (ANSI true-color compatible):
//
//   fg (24 bits): Metadata SLOT INDEX (encodes byte offset / 32)
//   bg (24 bits): Relative position within widget
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  primitiveOffset (u32)  - float index in cardStorage
//   offset 1:  primitiveCount (u32)
//   offset 2:  textSpanOffset (u32)   - float index in cardStorage
//   offset 3:  textSpanCount (u32)
//   offset 4:  bvhOffset (u32)        - float index of BVH nodes
//   offset 5:  bvhNodeCount (u32)
//   offset 6:  glyphOffset (u32)      - positioned glyphs (unused for now)
//   offset 7:  glyphCount (u32)
//   offset 8:  sceneMinX (f32)
//   offset 9:  sceneMinY (f32)
//   offset 10: sceneMaxX (f32)
//   offset 11: sceneMaxY (f32)
//   offset 12: widthCells (u32)
//   offset 13: heightCells (u32)
//   offset 14: flags (u32)
//   offset 15: bgColor (u32)
//
// =============================================================================
// SDFPrimitive layout (96 bytes = 24 floats):
//   offset 0:  type (u32)
//   offset 1:  layer (u32)
//   offset 2-13: params[12] (f32)
//   offset 14: fillColor (u32)
//   offset 15: strokeColor (u32)
//   offset 16: strokeWidth (f32)
//   offset 17: round (f32)
//   offset 18: aabbMinX (f32)
//   offset 19: aabbMinY (f32)
//   offset 20: aabbMaxX (f32)
//   offset 21: aabbMaxY (f32)
//   offset 22-23: _pad
//
// =============================================================================
// BVHNode layout (32 bytes = 8 floats):
//   offset 0: aabbMinX (f32)
//   offset 1: aabbMinY (f32)
//   offset 2: aabbMaxX (f32)
//   offset 3: aabbMaxY (f32)
//   offset 4: leftChild (u32)
//   offset 5: rightChild (u32)
//   offset 6: primIndex (u32)
//   offset 7: primCount (u32)
// =============================================================================

// SDF primitive types
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

const YDRAW_FLAG_SHOW_BOUNDS: u32 = 1u;
const YDRAW_FLAG_SHOW_BVH: u32 = 2u;
const YDRAW_FLAG_SHOW_EVAL_COUNT: u32 = 4u;  // Heatmap of primitive evaluations per pixel

// Note: unpackColor() and unpackColorAlpha() are defined globally in shaders.wgsl
// They expect colors packed as 0xAABBGGRR (ABGR format, common in graphics)

// =============================================================================
// SDF Functions
// =============================================================================

fn sdCircle(p: vec2<f32>, center: vec2<f32>, radius: f32) -> f32 {
    return length(p - center) - radius;
}

fn sdBox(p: vec2<f32>, center: vec2<f32>, halfSize: vec2<f32>, round: f32) -> f32 {
    let d = abs(p - center) - halfSize + round;
    return length(max(d, vec2<f32>(0.0))) + min(max(d.x, d.y), 0.0) - round;
}

fn sdSegment(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>) -> f32 {
    let pa = p - a;
    let ba = b - a;
    let h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

fn sdTriangle(p: vec2<f32>, p0: vec2<f32>, p1: vec2<f32>, p2: vec2<f32>) -> f32 {
    let e0 = p1 - p0;
    let e1 = p2 - p1;
    let e2 = p0 - p2;
    let v0 = p - p0;
    let v1 = p - p1;
    let v2 = p - p2;
    let pq0 = v0 - e0 * clamp(dot(v0, e0) / dot(e0, e0), 0.0, 1.0);
    let pq1 = v1 - e1 * clamp(dot(v1, e1) / dot(e1, e1), 0.0, 1.0);
    let pq2 = v2 - e2 * clamp(dot(v2, e2) / dot(e2, e2), 0.0, 1.0);
    let s = sign(e0.x * e2.y - e0.y * e2.x);
    let d = min(min(
        vec2<f32>(dot(pq0, pq0), s * (v0.x * e0.y - v0.y * e0.x)),
        vec2<f32>(dot(pq1, pq1), s * (v1.x * e1.y - v1.y * e1.x))),
        vec2<f32>(dot(pq2, pq2), s * (v2.x * e2.y - v2.y * e2.x)));
    return -sqrt(d.x) * sign(d.y);
}

fn sdBezier2(p: vec2<f32>, A: vec2<f32>, B: vec2<f32>, C: vec2<f32>) -> f32 {
    let a = B - A;
    let b = A - 2.0 * B + C;
    let c = a * 2.0;
    let d = A - p;
    let kk = 1.0 / dot(b, b);
    let kx = kk * dot(a, b);
    let ky = kk * (2.0 * dot(a, a) + dot(d, b)) / 3.0;
    let kz = kk * dot(d, a);
    var res = 0.0;
    let pp = ky - kx * kx;
    let pq = kx * (2.0 * kx * kx - 3.0 * ky) + kz;
    let p2 = pp * pp * pp;
    let q2 = pq * pq;
    let h = q2 + 4.0 * p2;
    if (h >= 0.0) {
        let hh = sqrt(h);
        let x = (vec2<f32>(hh, -hh) - pq) / 2.0;
        let uv = sign(x) * pow(abs(x), vec2<f32>(1.0 / 3.0));
        let t = clamp(uv.x + uv.y - kx, 0.0, 1.0);
        res = dot(d + (c + b * t) * t, d + (c + b * t) * t);
    } else {
        let z = sqrt(-pp);
        let v = acos(pq / (pp * z * 2.0)) / 3.0;
        let m = cos(v);
        let n = sin(v) * 1.732050808;
        let t = clamp(vec3<f32>(m + m, -n - m, n - m) * z - kx, vec3<f32>(0.0), vec3<f32>(1.0));
        res = min(dot(d + (c + b * t.x) * t.x, d + (c + b * t.x) * t.x),
                  dot(d + (c + b * t.y) * t.y, d + (c + b * t.y) * t.y));
    }
    return sqrt(res);
}

fn sdEllipse(p: vec2<f32>, center: vec2<f32>, ab: vec2<f32>) -> f32 {
    let pp = abs(p - center);
    let pab = pp / ab;
    let abab = ab * ab;
    if (pab.x > pab.y) {
        let l = abab.y - abab.x;
        let m = ab.x * pp.x / l;
        let n = ab.y * pp.y / l;
        let m2 = m * m;
        let n2 = n * n;
        let c = (m2 + n2 - 1.0) / 3.0;
        let c3 = c * c * c;
        let q = c3 + m2 * n2 * 2.0;
        let d = c3 + m2 * n2;
        let g = m + m * n2;
        var co: f32;
        if (d < 0.0) {
            let h = acos(q / c3) / 3.0;
            let s = cos(h);
            let t = sin(h) * sqrt(3.0);
            let rx = sqrt(-c * (s + t + 2.0) + m2);
            let ry = sqrt(-c * (s - t + 2.0) + m2);
            co = (ry + sign(l) * rx + abs(g) / (rx * ry) - m) / 2.0;
        } else {
            let h = 2.0 * m * n * sqrt(d);
            let s = sign(q + h) * pow(abs(q + h), 1.0 / 3.0);
            let u = sign(q - h) * pow(abs(q - h), 1.0 / 3.0);
            let rx = -s - u - c * 4.0 + 2.0 * m2;
            let ry = (s - u) * sqrt(3.0);
            let rm = sqrt(rx * rx + ry * ry);
            co = (ry / sqrt(rm - rx) + 2.0 * g / rm - m) / 2.0;
        }
        let r = ab * vec2<f32>(co, sqrt(1.0 - co * co));
        return length(r - pp) * sign(pp.y - r.y);
    } else {
        let l = abab.x - abab.y;
        let m = ab.y * pp.y / l;
        let n = ab.x * pp.x / l;
        let m2 = m * m;
        let n2 = n * n;
        let c = (m2 + n2 - 1.0) / 3.0;
        let c3 = c * c * c;
        let q = c3 + m2 * n2 * 2.0;
        let d = c3 + m2 * n2;
        let g = m + m * n2;
        var co: f32;
        if (d < 0.0) {
            let h = acos(q / c3) / 3.0;
            let s = cos(h);
            let t = sin(h) * sqrt(3.0);
            let rx = sqrt(-c * (s + t + 2.0) + m2);
            let ry = sqrt(-c * (s - t + 2.0) + m2);
            co = (ry + sign(l) * rx + abs(g) / (rx * ry) - m) / 2.0;
        } else {
            let h = 2.0 * m * n * sqrt(d);
            let s = sign(q + h) * pow(abs(q + h), 1.0 / 3.0);
            let u = sign(q - h) * pow(abs(q - h), 1.0 / 3.0);
            let rx = -s - u - c * 4.0 + 2.0 * m2;
            let ry = (s - u) * sqrt(3.0);
            let rm = sqrt(rx * rx + ry * ry);
            co = (ry / sqrt(rm - rx) + 2.0 * g / rm - m) / 2.0;
        }
        let r = ab * vec2<f32>(sqrt(1.0 - co * co), co);
        return length(r - pp) * sign(pp.x - r.x);
    }
}

fn sdArc(p: vec2<f32>, center: vec2<f32>, sc: vec2<f32>, ra: f32, rb: f32) -> f32 {
    // sc is sin/cos of the arc's aperture angle
    var pp = abs(p - center);
    if (sc.y * pp.x > sc.x * pp.y) {
        return length(pp - sc * ra) - rb;
    } else {
        return abs(length(pp) - ra) - rb;
    }
}

fn sdRoundedBox(p: vec2<f32>, center: vec2<f32>, b: vec2<f32>, r: vec4<f32>) -> f32 {
    var pp = p - center;
    var rr = r;
    rr = select(rr.zw, rr.xy, pp.x > 0.0).xyxy;
    rr.x = select(rr.y, rr.x, pp.y > 0.0);
    let q = abs(pp) - b + rr.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, vec2<f32>(0.0))) - rr.x;
}

fn sdRhombus(p: vec2<f32>, center: vec2<f32>, b: vec2<f32>) -> f32 {
    var pp = abs(p - center);
    var bb = vec2<f32>(b.x, -b.y);
    let h = clamp((dot(bb, pp) + bb.y * bb.y) / dot(bb, bb), 0.0, 1.0);
    pp -= bb * vec2<f32>(h, h - 1.0);
    return length(pp) * sign(pp.x);
}

fn sdPentagon(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    let k = vec3<f32>(0.809016994, 0.587785252, 0.726542528);
    var pp = p - center;
    pp.x = abs(pp.x);
    pp -= 2.0 * min(dot(vec2<f32>(-k.x, k.y), pp), 0.0) * vec2<f32>(-k.x, k.y);
    pp -= 2.0 * min(dot(vec2<f32>(k.x, k.y), pp), 0.0) * vec2<f32>(k.x, k.y);
    pp -= vec2<f32>(clamp(pp.x, -r * k.z, r * k.z), r);
    return length(pp) * sign(pp.y);
}

fn sdHexagon(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    let k = vec3<f32>(-0.866025404, 0.5, 0.577350269);
    var pp = abs(p - center);
    pp -= 2.0 * min(dot(k.xy, pp), 0.0) * k.xy;
    pp -= vec2<f32>(clamp(pp.x, -k.z * r, k.z * r), r);
    return length(pp) * sign(pp.y);
}

fn sdStar(p: vec2<f32>, center: vec2<f32>, r: f32, n: f32, m: f32) -> f32 {
    // n = number of points, m = pointiness (2 to n)
    let an = 3.141593 / n;
    let en = 3.141593 / m;
    let acs = vec2<f32>(cos(an), sin(an));
    let ecs = vec2<f32>(cos(en), sin(en));
    var pp = p - center;
    let bn = (atan2(pp.x, pp.y) % (2.0 * an)) - an;
    pp = length(pp) * vec2<f32>(cos(bn), abs(sin(bn)));
    pp -= r * acs;
    pp += ecs * clamp(-dot(pp, ecs), 0.0, r * acs.y / ecs.y);
    return length(pp) * sign(pp.x);
}

fn sdPie(p: vec2<f32>, center: vec2<f32>, c: vec2<f32>, r: f32) -> f32 {
    // c = sin/cos of aperture angle
    var pp = p - center;
    pp.x = abs(pp.x);
    let l = length(pp) - r;
    let m = length(pp - c * clamp(dot(pp, c), 0.0, r));
    return max(l, m * sign(c.y * pp.x - c.x * pp.y));
}

fn sdRing(p: vec2<f32>, center: vec2<f32>, n: vec2<f32>, r: f32, th: f32) -> f32 {
    // n = normal direction (sin/cos), th = thickness
    var pp = p - center;
    pp.x = abs(pp.x);
    pp = mat2x2<f32>(n.x, n.y, -n.y, n.x) * pp;
    return max(abs(length(pp) - r) - th * 0.5,
               length(vec2<f32>(pp.x, max(0.0, abs(r - pp.y) - th * 0.5))) * sign(pp.x));
}

fn sdHeart(p: vec2<f32>, center: vec2<f32>, scale: f32) -> f32 {
    var pp = (p - center) / scale;
    pp.x = abs(pp.x);
    if (pp.y + pp.x > 1.0) {
        return (length(pp - vec2<f32>(0.25, 0.75)) - sqrt(2.0) / 4.0) * scale;
    }
    return (sqrt(min(dot(pp - vec2<f32>(0.0, 1.0), pp - vec2<f32>(0.0, 1.0)),
                     dot(pp - 0.5 * max(pp.x + pp.y, 0.0), pp - 0.5 * max(pp.x + pp.y, 0.0)))) *
            sign(pp.x - pp.y)) * scale;
}

fn sdCross(p: vec2<f32>, center: vec2<f32>, b: vec2<f32>, r: f32) -> f32 {
    var pp = abs(p - center);
    pp = select(pp, pp.yx, pp.y > pp.x);
    let q = pp - b;
    let k = max(q.y, q.x);
    let w = select(vec2<f32>(b.y - pp.x, -k), q, k > 0.0);
    return sign(k) * length(max(w, vec2<f32>(0.0))) + r;
}

fn sdRoundedX(p: vec2<f32>, center: vec2<f32>, w: f32, r: f32) -> f32 {
    let pp = abs(p - center);
    return length(pp - min(pp.x + pp.y, w) * 0.5) - r;
}

fn sdCapsule(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>, r: f32) -> f32 {
    let pa = p - a;
    let ba = b - a;
    let h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - r;
}

fn sdMoon(p: vec2<f32>, center: vec2<f32>, d: f32, ra: f32, rb: f32) -> f32 {
    var pp = p - center;
    pp.y = abs(pp.y);
    let a = (ra * ra - rb * rb + d * d) / (2.0 * d);
    let b = sqrt(max(ra * ra - a * a, 0.0));
    if (d * (pp.x * b - pp.y * a) > d * d * max(b - pp.y, 0.0)) {
        return length(pp - vec2<f32>(a, b));
    }
    return max(length(pp) - ra, -(length(pp - vec2<f32>(d, 0.0)) - rb));
}

fn sdEgg(p: vec2<f32>, center: vec2<f32>, ra: f32, rb: f32) -> f32 {
    // ra = bottom radius, rb = top radius
    var pp = p - center;
    pp.x = abs(pp.x);
    let r = ra - rb;
    let k = select(length(pp) / r, (rb * pp.y - ra * pp.x) / (ra * rb), pp.y < 0.0);
    if (k < 0.0) {
        return length(pp) - ra;
    }
    if (k > ra) {
        return length(pp - vec2<f32>(0.0, ra)) - rb;
    }
    return length(pp - vec2<f32>(-r, 0.0)) - (ra + rb - r);
}

// =============================================================================
// Primitive evaluation
// =============================================================================

fn evaluateSDF(primOffset: u32, p: vec2<f32>) -> f32 {
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
            let round = cardStorage[primOffset + 17u];
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
            let radii = vec4<f32>(cardStorage[primOffset + 6u], cardStorage[primOffset + 7u],
                                  cardStorage[primOffset + 8u], cardStorage[primOffset + 9u]);
            return sdRoundedBox(p, center, halfSize, radii);
        }
        case SDF_RHOMBUS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdRhombus(p, center, b);
        }
        case SDF_PENTAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdPentagon(p, center, r);
        }
        case SDF_HEXAGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdHexagon(p, center, r);
        }
        case SDF_STAR: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            let n = cardStorage[primOffset + 5u];
            let m = cardStorage[primOffset + 6u];
            return sdStar(p, center, r, n, m);
        }
        case SDF_PIE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let c = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let r = cardStorage[primOffset + 6u];
            return sdPie(p, center, c, r);
        }
        case SDF_RING: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let n = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let r = cardStorage[primOffset + 6u];
            let th = cardStorage[primOffset + 7u];
            return sdRing(p, center, n, r, th);
        }
        case SDF_HEART: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let scale = cardStorage[primOffset + 4u];
            return sdHeart(p, center, scale);
        }
        case SDF_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let r = cardStorage[primOffset + 6u];
            return sdCross(p, center, b, r);
        }
        case SDF_ROUNDED_X: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let w = cardStorage[primOffset + 4u];
            let r = cardStorage[primOffset + 5u];
            return sdRoundedX(p, center, w, r);
        }
        case SDF_CAPSULE: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let r = cardStorage[primOffset + 6u];
            return sdCapsule(p, a, b, r);
        }
        case SDF_MOON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let d = cardStorage[primOffset + 4u];
            let ra = cardStorage[primOffset + 5u];
            let rb = cardStorage[primOffset + 6u];
            return sdMoon(p, center, d, ra, rb);
        }
        case SDF_EGG: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let ra = cardStorage[primOffset + 4u];
            let rb = cardStorage[primOffset + 5u];
            return sdEgg(p, center, ra, rb);
        }
        default: {
            return 1e10;
        }
    }
}

fn getPrimitiveColors(primOffset: u32) -> vec4<u32> {
    // Returns: (fillColor, strokeColor, layer, 0)
    let fillColor = bitcast<u32>(cardStorage[primOffset + 14u]);
    let strokeColor = bitcast<u32>(cardStorage[primOffset + 15u]);
    let layer = bitcast<u32>(cardStorage[primOffset + 1u]);
    return vec4<u32>(fillColor, strokeColor, layer, 0u);
}

fn getPrimitiveStrokeWidth(primOffset: u32) -> f32 {
    return cardStorage[primOffset + 16u];
}

// =============================================================================
// BVH Traversal
// =============================================================================

fn aabbContains(nodeOffset: u32, p: vec2<f32>) -> bool {
    let minX = cardStorage[nodeOffset + 0u];
    let minY = cardStorage[nodeOffset + 1u];
    let maxX = cardStorage[nodeOffset + 2u];
    let maxY = cardStorage[nodeOffset + 3u];
    return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
}

fn getBVHNodeChildren(nodeOffset: u32) -> vec4<u32> {
    // Returns: (leftChild, rightChild, primIndex, primCount)
    return vec4<u32>(
        bitcast<u32>(cardStorage[nodeOffset + 4u]),
        bitcast<u32>(cardStorage[nodeOffset + 5u]),
        bitcast<u32>(cardStorage[nodeOffset + 6u]),
        bitcast<u32>(cardStorage[nodeOffset + 7u])
    );
}

// =============================================================================
// Main shader function
// =============================================================================

fn shaderGlyph_1048578(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    // ==========================================================================
    // Decode ANSI-compatible cell encoding
    // ==========================================================================
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 8u;  // slotIndex = byteOffset/32, so *8 gives u32 index

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // ==========================================================================
    // Read metadata
    // ==========================================================================
    let primitiveOffset = cardMetadata[metaOffset + 0u];
    let primitiveCount = cardMetadata[metaOffset + 1u];
    let textSpanOffset = cardMetadata[metaOffset + 2u];
    let textSpanCount = cardMetadata[metaOffset + 3u];
    let bvhOffset = cardMetadata[metaOffset + 4u];
    let bvhNodeCount = cardMetadata[metaOffset + 5u];
    let glyphOffset = cardMetadata[metaOffset + 6u];
    let glyphCount = cardMetadata[metaOffset + 7u];
    let sceneMinX = bitcast<f32>(cardMetadata[metaOffset + 8u]);
    let sceneMinY = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let sceneMaxX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let sceneMaxY = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let widthCells = cardMetadata[metaOffset + 12u];
    let heightCells = cardMetadata[metaOffset + 13u];
    let flags = cardMetadata[metaOffset + 14u];
    let bgColorPacked = cardMetadata[metaOffset + 15u];

    let bgColor = unpackColor(bgColorPacked);

    // ==========================================================================
    // Compute scene-space position
    // ==========================================================================
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // Map UV to scene coordinates
    let scenePos = vec2<f32>(
        sceneMinX + widgetUV.x * (sceneMaxX - sceneMinX),
        sceneMinY + widgetUV.y * (sceneMaxY - sceneMinY)
    );

    // ==========================================================================
    // Debug: show primitive count as color gradient
    // ==========================================================================
    if (primitiveCount == 0u) {
        // No primitives - show magenta to indicate this state
        return vec3<f32>(1.0, 0.0, 1.0);
    }


    // ==========================================================================
    // Render primitives using BVH traversal (O(log N) instead of O(N))
    // ==========================================================================
    var resultColor = bgColor;

    let primSize = 24u;  // SDFPrimitive is 96 bytes = 24 floats
    let bvhNodeSize = 8u;  // BVHNode is 32 bytes = 8 floats

    // Calculate sortedIndices offset (after BVH nodes)
    let sortedIndicesOffset = bvhOffset + bvhNodeCount * bvhNodeSize;

    // Stack-based BVH traversal (avoid recursion, max depth 32)
    var stack: array<u32, 32>;
    var stackPtr = 0;

    // Push root node (index 0)
    if (bvhNodeCount > 0u) {
        stack[stackPtr] = 0u;
        stackPtr++;
    }

    var bestLayer = 0xFFFFFFFFu;

    while (stackPtr > 0) {
        stackPtr--;
        let nodeIdx = stack[stackPtr];
        let nodeOffset = bvhOffset + nodeIdx * bvhNodeSize;

        // Read BVH node AABB
        let nodeMinX = cardStorage[nodeOffset + 0u];
        let nodeMinY = cardStorage[nodeOffset + 1u];
        let nodeMaxX = cardStorage[nodeOffset + 2u];
        let nodeMaxY = cardStorage[nodeOffset + 3u];

        // AABB test - skip entire subtree if pixel is outside
        if (scenePos.x < nodeMinX || scenePos.x > nodeMaxX ||
            scenePos.y < nodeMinY || scenePos.y > nodeMaxY) {
            continue;
        }

        // Read node children/primitive info
        let leftChild = bitcast<u32>(cardStorage[nodeOffset + 4u]);
        let rightChild = bitcast<u32>(cardStorage[nodeOffset + 5u]);
        let primIndex = bitcast<u32>(cardStorage[nodeOffset + 6u]);
        let primCount = bitcast<u32>(cardStorage[nodeOffset + 7u]);

        if (primCount > 0u) {
            // Leaf node - evaluate primitives
            for (var i = 0u; i < primCount; i++) {
                // Get primitive index from sorted indices
                let sortedIdx = bitcast<u32>(cardStorage[sortedIndicesOffset + primIndex + i]);
                let primOffset = primitiveOffset + sortedIdx * primSize;

                // Evaluate SDF
                let d = evaluateSDF(primOffset, scenePos);
                let colors = getPrimitiveColors(primOffset);
                let fillColor = unpackColor(colors.x);
                let strokeColor = unpackColor(colors.y);
                let layer = colors.z;
                let strokeWidth = getPrimitiveStrokeWidth(primOffset);

                // Fill (inside the shape)
                if (d < 0.0 && colors.x != 0u) {
                    let alpha = clamp(-d * 2.0, 0.0, 1.0);  // Anti-aliasing
                    if (layer <= bestLayer) {
                        resultColor = mix(resultColor, fillColor, alpha);
                        bestLayer = layer;
                    }
                }

                // Stroke (on the edge)
                if (strokeWidth > 0.0 && colors.y != 0u) {
                    let strokeDist = abs(d) - strokeWidth * 0.5;
                    if (strokeDist < 0.0) {
                        let alpha = clamp(-strokeDist * 2.0, 0.0, 1.0);
                        resultColor = mix(resultColor, strokeColor, alpha);
                    }
                }
            }
        } else {
            // Internal node - push children onto stack
            // Check for valid children (0xFFFFFFFF means no child)
            if (rightChild != 0xFFFFFFFFu && stackPtr < 31) {
                stack[stackPtr] = rightChild;
                stackPtr++;
            }
            if (leftChild != 0xFFFFFFFFu && stackPtr < 31) {
                stack[stackPtr] = leftChild;
                stackPtr++;
            }
        }
    }

    // ==========================================================================
    // Debug: show bounds if enabled
    // ==========================================================================
    if ((flags & YDRAW_FLAG_SHOW_BOUNDS) != 0u) {
        // Draw scene bounds
        let edgeDist = min(
            min(scenePos.x - sceneMinX, sceneMaxX - scenePos.x),
            min(scenePos.y - sceneMinY, sceneMaxY - scenePos.y)
        );
        let edgeWidth = (sceneMaxX - sceneMinX) * 0.01;
        if (edgeDist < edgeWidth) {
            resultColor = mix(resultColor, vec3<f32>(1.0, 0.0, 0.0), 0.5);
        }
    }

    return resultColor;
}
