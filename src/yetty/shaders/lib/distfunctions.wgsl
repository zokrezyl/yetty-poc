// =============================================================================
// distfunctions.wgsl - Shared SDF (Signed Distance Function) library
// =============================================================================
// Based on Inigo Quilez's 2D distance functions (iquilezles.org/articles/distfunctions2d)
// Used by: hdraw (BVH), ydraw (spatial hashing)
// =============================================================================

// SDF primitive type constants are now in sdf-types.gen.wgsl (auto-generated)

// =============================================================================
// SDF Functions
// =============================================================================

fn dot2v(v: vec2<f32>) -> f32 { return dot(v, v); }

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

fn sdBezier3(p: vec2<f32>, A: vec2<f32>, B: vec2<f32>, C: vec2<f32>, D: vec2<f32>) -> f32 {
    // Cubic bezier SDF via Newton refinement on squared distance.
    // 5 starting samples, 3 Newton iterations each.
    var minDist2 = 1e20;

    for (var i = 0u; i <= 4u; i = i + 1u) {
        var t = f32(i) * 0.25;

        for (var j = 0u; j < 3u; j = j + 1u) {
            let u = 1.0 - t;
            // P(t)
            let pt = u*u*u*A + 3.0*u*u*t*B + 3.0*u*t*t*C + t*t*t*D;
            // P'(t) = 3[(1-t)²(B-A) + 2(1-t)t(C-B) + t²(D-C)]
            let dt = 3.0*(u*u*(B-A) + 2.0*u*t*(C-B) + t*t*(D-C));
            // P''(t) = 6[(1-t)(C-2B+A) + t(D-2C+B)]
            let ddt = 6.0*(u*(C - 2.0*B + A) + t*(D - 2.0*C + B));

            let dd = pt - p;
            let num = dot(dd, dt);
            let den = dot(dt, dt) + dot(dd, ddt);

            if (abs(den) > 1e-10) {
                t = clamp(t - num / den, 0.0, 1.0);
            }
        }

        let u = 1.0 - t;
        let pt = u*u*u*A + 3.0*u*u*t*B + 3.0*u*t*t*C + t*t*t*D;
        let dd = p - pt;
        minDist2 = min(minDist2, dot(dd, dd));
    }

    return sqrt(minDist2);
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
    var pp = p - center;
    pp.x = abs(pp.x);
    let l = length(pp) - r;
    let m = length(pp - c * clamp(dot(pp, c), 0.0, r));
    return max(l, m * sign(c.y * pp.x - c.x * pp.y));
}

fn sdRing(p: vec2<f32>, center: vec2<f32>, n: vec2<f32>, r: f32, th: f32) -> f32 {
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

fn sdChamferBox(p: vec2<f32>, center: vec2<f32>, b: vec2<f32>, chamfer: f32) -> f32 {
    var pp = abs(p - center) - b;
    pp = select(pp, pp.yx, pp.y > pp.x);
    pp.y = pp.y + chamfer;
    let k = 1.0 - sqrt(2.0);
    if (pp.y < 0.0 && pp.y + pp.x * k < 0.0) { return pp.x; }
    if (pp.x < pp.y) { return (pp.x + pp.y) * sqrt(0.5); }
    return length(pp);
}

fn sdOrientedBox(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>, th: f32) -> f32 {
    let l = length(b - a);
    let d = (b - a) / l;
    var q = (p - (a + b) * 0.5);
    q = mat2x2<f32>(vec2<f32>(d.x, d.y), vec2<f32>(-d.y, d.x)) * q;
    q = abs(q) - vec2<f32>(l, th) * 0.5;
    return length(max(q, vec2<f32>(0.0))) + min(max(q.x, q.y), 0.0);
}

fn sdTrapezoid(p: vec2<f32>, center: vec2<f32>, r1: f32, r2: f32, he: f32) -> f32 {
    var pp = p - center;
    let k1 = vec2<f32>(r2, he);
    let k2 = vec2<f32>(r2 - r1, 2.0 * he);
    pp.x = abs(pp.x);
    let ca = vec2<f32>(pp.x - min(pp.x, select(r2, r1, pp.y < 0.0)), abs(pp.y) - he);
    let cb = pp - k1 + k2 * clamp(dot(k1 - pp, k2) / dot2v(k2), 0.0, 1.0);
    let s = select(1.0, -1.0, cb.x < 0.0 && ca.y < 0.0);
    return s * sqrt(min(dot2v(ca), dot2v(cb)));
}

fn sdParallelogram(p: vec2<f32>, center: vec2<f32>, wi: f32, he: f32, sk: f32) -> f32 {
    var pp = p - center;
    let e = vec2<f32>(sk, he);
    pp = select(pp, -pp, pp.y < 0.0);
    var w = pp - e;
    w.x = w.x - clamp(w.x, -wi, wi);
    var d = vec2<f32>(dot(w, w), -w.y);
    let s = pp.x * e.y - pp.y * e.x;
    pp = select(pp, -pp, s < 0.0);
    var v = pp - vec2<f32>(wi, 0.0);
    v = v - e * clamp(dot(v, e) / dot(e, e), -1.0, 1.0);
    d = min(d, vec2<f32>(dot(v, v), wi * he - abs(s)));
    return sqrt(d.x) * sign(-d.y);
}

fn sdEquilateralTriangle(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    var pp = p - center;
    let k = sqrt(3.0);
    pp.x = abs(pp.x) - r;
    pp.y = pp.y + r / k;
    if (pp.x + k * pp.y > 0.0) {
        pp = vec2<f32>(pp.x - k * pp.y, -k * pp.x - pp.y) / 2.0;
    }
    pp.x = pp.x - clamp(pp.x, -2.0 * r, 0.0);
    return -length(pp) * sign(pp.y);
}

fn sdTriangleIsosceles(p: vec2<f32>, center: vec2<f32>, q: vec2<f32>) -> f32 {
    var pp = p - center;
    pp.x = abs(pp.x);
    let a = pp - q * clamp(dot(pp, q) / dot(q, q), 0.0, 1.0);
    let b = pp - q * vec2<f32>(clamp(pp.x / q.x, 0.0, 1.0), 1.0);
    let s = -sign(q.y);
    let d = min(vec2<f32>(dot(a, a), s * (pp.x * q.y - pp.y * q.x)), vec2<f32>(dot(b, b), s * (pp.y - q.y)));
    return -sqrt(d.x) * sign(d.y);
}

fn sdUnevenCapsule(p: vec2<f32>, center: vec2<f32>, r1: f32, r2: f32, h: f32) -> f32 {
    var pp = p - center;
    pp.x = abs(pp.x);
    let b = (r1 - r2) / h;
    let a = sqrt(1.0 - b * b);
    let k = dot(pp, vec2<f32>(-b, a));
    if (k < 0.0) { return length(pp) - r1; }
    if (k > a * h) { return length(pp - vec2<f32>(0.0, h)) - r2; }
    return dot(pp, vec2<f32>(a, b)) - r1;
}

fn sdOctogon(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    let k = vec3<f32>(-0.9238795325, 0.3826834323, 0.4142135623);
    var pp = abs(p - center);
    pp = pp - 2.0 * min(dot(vec2<f32>(k.x, k.y), pp), 0.0) * vec2<f32>(k.x, k.y);
    pp = pp - 2.0 * min(dot(vec2<f32>(-k.x, k.y), pp), 0.0) * vec2<f32>(-k.x, k.y);
    pp = pp - vec2<f32>(clamp(pp.x, -k.z * r, k.z * r), r);
    return length(pp) * sign(pp.y);
}

fn sdHexagram(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    let k = vec4<f32>(-0.5, 0.8660254038, 0.5773502692, 1.7320508076);
    var pp = abs(p - center);
    pp = pp - 2.0 * min(dot(k.xy, pp), 0.0) * k.xy;
    pp = pp - 2.0 * min(dot(k.yx, pp), 0.0) * k.yx;
    pp = pp - vec2<f32>(clamp(pp.x, r * k.z, r * k.w), r);
    return length(pp) * sign(pp.y);
}

fn sdPentagram(p: vec2<f32>, center: vec2<f32>, r: f32) -> f32 {
    let v1 = vec2<f32>(0.809016994, -0.587785252);
    let v2 = vec2<f32>(-0.809016994, -0.587785252);
    let v3 = vec2<f32>(0.309016994, -0.951056516);
    let k1z = 0.726542528;
    var pp = p - center;
    pp.x = abs(pp.x);
    pp = pp - 2.0 * max(dot(v1, pp), 0.0) * v1;
    pp = pp - 2.0 * max(dot(v2, pp), 0.0) * v2;
    pp.x = abs(pp.x);
    pp.y = pp.y - r;
    return length(pp - v3 * clamp(dot(pp, v3), 0.0, k1z * r)) * sign(pp.y * v3.x - pp.x * v3.y);
}

fn sdCutDisk(p: vec2<f32>, center: vec2<f32>, r: f32, h: f32) -> f32 {
    var pp = p - center;
    let w = sqrt(r * r - h * h);
    pp.x = abs(pp.x);
    let s = max((h - r) * pp.x * pp.x + w * w * (h + r - 2.0 * pp.y), h * pp.x - w * pp.y);
    if (s < 0.0) { return length(pp) - r; }
    if (pp.x < w) { return h - pp.y; }
    return length(pp - vec2<f32>(w, h));
}

fn sdHorseshoe(p: vec2<f32>, center: vec2<f32>, c: vec2<f32>, r: f32, w: vec2<f32>) -> f32 {
    var pp = p - center;
    pp.x = abs(pp.x);
    let l = length(pp);
    pp = mat2x2<f32>(vec2<f32>(-c.x, c.y), vec2<f32>(c.y, c.x)) * pp;
    pp = vec2<f32>(select(l * sign(-c.x), pp.x, pp.y > 0.0 || pp.x > 0.0), select(l, pp.y, pp.x > 0.0));
    pp = vec2<f32>(pp.x, abs(pp.y - r)) - w;
    return length(max(pp, vec2<f32>(0.0))) + min(0.0, max(pp.x, pp.y));
}

fn sdVesica(p: vec2<f32>, center: vec2<f32>, w: f32, h: f32) -> f32 {
    var pp = abs(p - center);
    let d = 0.5 * (w * w - h * h) / h;
    let cond = w * pp.y < d * (pp.x - w);
    let c = select(vec3<f32>(-d, 0.0, d + h), vec3<f32>(0.0, w, 0.0), cond);
    return length(pp - c.yx) - c.z;
}

fn sdOrientedVesica(p: vec2<f32>, a: vec2<f32>, b: vec2<f32>, w: f32) -> f32 {
    let r = 0.5 * length(b - a);
    let d = 0.5 * (r * r - w * w) / w;
    let v = (b - a) / r;
    let c = (b + a) * 0.5;
    let q = 0.5 * abs(mat2x2<f32>(vec2<f32>(v.y, -v.x), vec2<f32>(v.x, v.y)) * (p - c));
    let cond = r * q.x < d * (q.y - r);
    let h = select(vec3<f32>(-d, 0.0, d + w), vec3<f32>(0.0, r, 0.0), cond);
    return length(q - h.xy) - h.z;
}

fn sdRoundedCross(p: vec2<f32>, center: vec2<f32>, h: f32) -> f32 {
    var pp = abs(p - center);
    let k = 0.5 * (h + 1.0 / h);
    if (pp.x < 1.0 && pp.y < pp.x * (k - h) + h) {
        return k - sqrt(dot2v(pp - vec2<f32>(1.0, k)));
    }
    return sqrt(min(dot2v(pp - vec2<f32>(0.0, h)), dot2v(pp - vec2<f32>(1.0, 0.0))));
}

fn sdParabola(p: vec2<f32>, center: vec2<f32>, k: f32) -> f32 {
    var pp = p - center;
    pp.x = abs(pp.x);
    let ik = 1.0 / k;
    let pa = ik * (pp.y - 0.5 * ik) / 3.0;
    let q = 0.25 * ik * ik * pp.x;
    let h = q * q - pa * pa * pa;
    var x: f32;
    if (h > 0.0) {
        let r = pow(q + sqrt(h), 1.0 / 3.0);
        x = r + pa / r;
    } else {
        let r = sqrt(pa);
        x = 2.0 * r * cos(acos(q / (pa * r)) / 3.0);
    }
    return length(pp - vec2<f32>(x, k * x * x)) * sign(pp.x - x);
}

fn sdBlobbyCross(p: vec2<f32>, center: vec2<f32>, he: f32) -> f32 {
    var pp = abs(p - center);
    pp = vec2<f32>(abs(pp.x - pp.y), 1.0 - pp.x - pp.y) / sqrt(2.0);
    let pa = (he - pp.y - 0.25 / he) / (6.0 * he);
    let q = pp.x / (he * he * 16.0);
    let h = q * q - pa * pa * pa;
    var x: f32;
    if (h > 0.0) {
        let r = sqrt(h);
        x = pow(q + r, 1.0 / 3.0) - pow(abs(q - r), 1.0 / 3.0) * sign(r - q);
    } else {
        let r = sqrt(pa);
        x = 2.0 * r * cos(acos(q / (pa * r)) / 3.0);
    }
    x = min(x, sqrt(2.0) / 2.0);
    let z = vec2<f32>(x, he * (1.0 - 2.0 * x * x)) - pp;
    return length(z) * sign(z.y);
}

fn sdTunnel(p: vec2<f32>, center: vec2<f32>, wh: vec2<f32>) -> f32 {
    var pp = p - center;
    pp.x = abs(pp.x);
    pp.y = -pp.y;
    var q = pp - wh;
    let d1 = dot2v(vec2<f32>(max(q.x, 0.0), q.y));
    q.x = select(q.x, length(pp) - wh.x, pp.y > 0.0);
    let d2 = dot2v(vec2<f32>(q.x, max(q.y, 0.0)));
    let d = sqrt(min(d1, d2));
    return select(d, -d, max(q.x, q.y) < 0.0);
}

fn sdStairs(p: vec2<f32>, center: vec2<f32>, wh: vec2<f32>, n: f32) -> f32 {
    var pp = p - center;
    let ba = wh * n;
    var d = min(dot2v(pp - vec2<f32>(clamp(pp.x, 0.0, ba.x), 0.0)), dot2v(pp - vec2<f32>(ba.x, clamp(pp.y, 0.0, ba.y))));
    var s = sign(max(-pp.y, pp.x - ba.x));
    let dia = length(wh);
    pp = mat2x2<f32>(vec2<f32>(wh.x, wh.y), vec2<f32>(-wh.y, wh.x)) * pp / dia;
    let id = clamp(round(pp.x / dia), 0.0, n - 1.0);
    pp.x = pp.x - id * dia;
    pp = mat2x2<f32>(vec2<f32>(wh.x, -wh.y), vec2<f32>(wh.y, wh.x)) * pp / dia;
    let hh = wh.y / 2.0;
    pp.y = pp.y - hh;
    if (pp.y > hh * sign(pp.x)) { s = 1.0; }
    pp = select(-pp, pp, id < 0.5 || pp.x > 0.0);
    d = min(d, dot2v(pp - vec2<f32>(0.0, clamp(pp.y, -hh, hh))));
    d = min(d, dot2v(pp - vec2<f32>(clamp(pp.x, 0.0, wh.x), hh)));
    return sqrt(d) * s;
}

fn sdQuadraticCircle(p: vec2<f32>, center: vec2<f32>, scale: f32) -> f32 {
    var pp = abs((p - center) / scale);
    if (pp.y > pp.x) { pp = pp.yx; }
    let a = pp.x - pp.y;
    let b = pp.x + pp.y;
    let c = (2.0 * b - 1.0) / 3.0;
    let h = a * a + c * c * c;
    var t: f32;
    if (h >= 0.0) {
        let hh = sqrt(h);
        t = sign(hh - a) * pow(abs(hh - a), 1.0 / 3.0) - pow(hh + a, 1.0 / 3.0);
    } else {
        let z = sqrt(-c);
        let v = acos(a / (c * z)) / 3.0;
        t = -z * (cos(v) + sin(v) * 1.732050808);
    }
    t = t * 0.5;
    let w = vec2<f32>(-t, t) + 0.75 - t * t - pp;
    return length(w) * sign(a * a * 0.5 + b - 1.5) * scale;
}

fn sdHyperbola(p: vec2<f32>, center: vec2<f32>, k: f32, he: f32) -> f32 {
    var pp = abs(p - center);
    pp = vec2<f32>(pp.x - pp.y, pp.x + pp.y) / sqrt(2.0);
    let x2 = pp.x * pp.x / 16.0;
    let y2 = pp.y * pp.y / 16.0;
    let r = k * (4.0 * k - pp.x * pp.y) / 12.0;
    let q = (x2 - y2) * k * k;
    let h = q * q + r * r * r;
    var u: f32;
    if (h < 0.0) {
        let m = sqrt(-r);
        u = m * cos(acos(q / (r * m)) / 3.0);
    } else {
        let m = pow(sqrt(h) - q, 1.0 / 3.0);
        u = (m - r / m) / 2.0;
    }
    let w = sqrt(u + x2);
    let b2 = k * pp.y - x2 * pp.x * 2.0;
    var t = pp.x / 4.0 - w + sqrt(2.0 * x2 - u + b2 / w / 4.0);
    t = max(t, sqrt(he * he * 0.5 + k) - he / sqrt(2.0));
    let d = length(pp - vec2<f32>(t, k / t));
    return select(-d, d, pp.x * pp.y < k);
}

fn sdCoolS(p: vec2<f32>, center: vec2<f32>, scale: f32) -> f32 {
    var pp = (p - center) / scale;
    let six = select(-pp.x, pp.x, pp.y < 0.0);
    pp.x = abs(pp.x);
    pp.y = abs(pp.y) - 0.2;
    let rex = pp.x - min(round(pp.x / 0.4), 0.4);
    let aby = abs(pp.y - 0.2) - 0.6;
    var d = dot2v(vec2<f32>(six, -pp.y) - clamp(0.5 * (six - pp.y), 0.0, 0.2));
    d = min(d, dot2v(vec2<f32>(pp.x, -aby) - clamp(0.5 * (pp.x - aby), 0.0, 0.4)));
    d = min(d, dot2v(vec2<f32>(rex, pp.y - clamp(pp.y, 0.0, 0.4))));
    let s = 2.0 * pp.x + aby + abs(aby + 0.4) - 0.4;
    return sqrt(d) * sign(s) * scale;
}

fn sdCircleWave(p: vec2<f32>, center: vec2<f32>, tb: f32, ra: f32) -> f32 {
    var pp = p - center;
    let tb2 = 3.1415927 * 5.0 / 6.0 * max(tb, 0.0001);
    let co = ra * vec2<f32>(sin(tb2), cos(tb2));
    pp.x = abs((pp.x % (co.x * 4.0)) - co.x * 2.0);
    let p1 = pp;
    let p2 = vec2<f32>(abs(pp.x - 2.0 * co.x), -pp.y + 2.0 * co.y);
    let d1 = select(abs(length(p1) - ra), length(p1 - co), co.y * p1.x > co.x * p1.y);
    let d2 = select(abs(length(p2) - ra), length(p2 - co), co.y * p2.x > co.x * p2.y);
    return min(d1, d2);
}

// 3D SDF type constants are now in sdf-types.gen.wgsl (auto-generated)

// =============================================================================
// 3D SDF Distance Functions (iquilezles.org/articles/distfunctions)
// =============================================================================

fn sdSphere3D(p: vec3<f32>, r: f32) -> f32 {
    return length(p) - r;
}

fn sdBox3D(p: vec3<f32>, b: vec3<f32>) -> f32 {
    let q = abs(p) - b;
    return length(max(q, vec3<f32>(0.0))) + min(max(q.x, max(q.y, q.z)), 0.0);
}

fn sdBoxFrame3D(p: vec3<f32>, b: vec3<f32>, e: f32) -> f32 {
    let q = abs(p) - b;
    let w = abs(q + e) - e;
    return min(min(
        length(max(vec3<f32>(q.x, w.y, w.z), vec3<f32>(0.0))) + min(max(q.x, max(w.y, w.z)), 0.0),
        length(max(vec3<f32>(w.x, q.y, w.z), vec3<f32>(0.0))) + min(max(w.x, max(q.y, w.z)), 0.0)),
        length(max(vec3<f32>(w.x, w.y, q.z), vec3<f32>(0.0))) + min(max(w.x, max(w.y, q.z)), 0.0));
}

fn sdTorus3D(p: vec3<f32>, t: vec2<f32>) -> f32 {
    let q = vec2<f32>(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

fn sdCappedTorus3D(p: vec3<f32>, sc: vec2<f32>, ra: f32, rb: f32) -> f32 {
    var q = p;
    q.x = abs(q.x);
    let k = select(length(q.xy), dot(q.xy, sc), sc.y * q.x > sc.x * q.y);
    return sqrt(dot(q, q) + ra * ra - 2.0 * ra * k) - rb;
}

fn sdCylinder3D(p: vec3<f32>, r: f32, h: f32) -> f32 {
    let d = abs(vec2<f32>(length(p.xz), p.y)) - vec2<f32>(r, h);
    return min(max(d.x, d.y), 0.0) + length(max(d, vec2<f32>(0.0)));
}

fn sdCappedCylinder3D(p: vec3<f32>, h: f32, r: f32) -> f32 {
    let d = abs(vec2<f32>(length(p.xz), p.y)) - vec2<f32>(r, h);
    return min(max(d.x, d.y), 0.0) + length(max(d, vec2<f32>(0.0)));
}

fn sdRoundedCylinder3D(p: vec3<f32>, ra: f32, rb: f32, h: f32) -> f32 {
    let d = vec2<f32>(length(p.xz) - ra + rb, abs(p.y) - h);
    return min(max(d.x, d.y), 0.0) + length(max(d, vec2<f32>(0.0))) - rb;
}

fn sdVerticalCapsule3D(p: vec3<f32>, h: f32, r: f32) -> f32 {
    var q = p;
    q.y = q.y - clamp(q.y, 0.0, h);
    return length(q) - r;
}

fn sdCone3D(p: vec3<f32>, c: vec2<f32>, h: f32) -> f32 {
    let q = h * vec2<f32>(c.x / c.y, -1.0);
    let w = vec2<f32>(length(p.xz), p.y);
    let a = w - q * clamp(dot(w, q) / dot(q, q), 0.0, 1.0);
    let b = w - q * vec2<f32>(clamp(w.x / q.x, 0.0, 1.0), 1.0);
    let k = sign(q.y);
    let d = min(dot(a, a), dot(b, b));
    let s = max(k * (w.x * q.y - w.y * q.x), k * (w.y - q.y));
    return sqrt(d) * sign(s);
}

fn sdCappedCone3D(p: vec3<f32>, h: f32, r1: f32, r2: f32) -> f32 {
    let q = vec2<f32>(length(p.xz), p.y);
    let k1 = vec2<f32>(r2, h);
    let k2 = vec2<f32>(r2 - r1, 2.0 * h);
    let ca = vec2<f32>(q.x - min(q.x, select(r2, r1, q.y < 0.0)), abs(q.y) - h);
    let cb = q - k1 + k2 * clamp(dot(k1 - q, k2) / dot(k2, k2), 0.0, 1.0);
    let s = select(1.0, -1.0, cb.x < 0.0 && ca.y < 0.0);
    return s * sqrt(min(dot(ca, ca), dot(cb, cb)));
}

fn sdRoundCone3D(p: vec3<f32>, r1: f32, r2: f32, h: f32) -> f32 {
    let b = (r1 - r2) / h;
    let a = sqrt(1.0 - b * b);
    let q = vec2<f32>(length(p.xz), p.y);
    let k = dot(q, vec2<f32>(-b, a));
    if (k < 0.0) { return length(q) - r1; }
    if (k > a * h) { return length(q - vec2<f32>(0.0, h)) - r2; }
    return dot(q, vec2<f32>(a, b)) - r1;
}

fn sdPlane3D(p: vec3<f32>, n: vec3<f32>, h: f32) -> f32 {
    return dot(p, n) + h;
}

fn sdHexPrism3D(p: vec3<f32>, h: vec2<f32>) -> f32 {
    let k = vec3<f32>(-0.8660254, 0.5, 0.57735);
    var q = abs(p);
    q = vec3<f32>(q.x - 2.0 * min(dot(k.xy, q.xy), 0.0) * k.x,
                  q.y - 2.0 * min(dot(k.xy, q.xy), 0.0) * k.y,
                  q.z);
    let d = vec2<f32>(length(q.xy - vec2<f32>(clamp(q.x, -k.z * h.x, k.z * h.x), h.x)) * sign(q.y - h.x), q.z - h.y);
    return min(max(d.x, d.y), 0.0) + length(max(d, vec2<f32>(0.0)));
}

fn sdTriPrism3D(p: vec3<f32>, h: vec2<f32>) -> f32 {
    let q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) - h.x * 0.5);
}

fn sdOctahedron3D(p: vec3<f32>, s: f32) -> f32 {
    let q = abs(p);
    let m = q.x + q.y + q.z - s;
    var r: vec3<f32>;
    if (3.0 * q.x < m) { r = q.xyz; }
    else if (3.0 * q.y < m) { r = q.yzx; }
    else if (3.0 * q.z < m) { r = q.zxy; }
    else { return m * 0.57735027; }
    let k = clamp(0.5 * (r.z - r.y + s), 0.0, s);
    return length(vec3<f32>(r.x, r.y - s + k, r.z - k));
}

fn sdPyramid3D(p: vec3<f32>, h: f32) -> f32 {
    let m2 = h * h + 0.25;
    var q = p;
    q.x = abs(q.x); q.z = abs(q.z);
    if (q.z > q.x) { q = vec3<f32>(q.z, q.y, q.x); }
    q = vec3<f32>(q.x - 0.5, q.y, q.z - 0.5);
    let a = vec3<f32>(q.x - q.z * clamp((q.x - q.y) * 0.5 / m2, 0.0, 0.5),
                      q.y - h * clamp((q.y + 0.5 * h) / (h * h), 0.0, 1.0),
                      q.z);
    let b = vec3<f32>(q.z, q.y - h, q.x - 0.5);
    let s = max(-q.x, 0.0);
    return sqrt(min(dot(a, a), dot(b, b) + s * s)) * sign(max(q.y * m2 - q.x * h * 0.5, q.x));
}

fn sdEllipsoid3D(p: vec3<f32>, r: vec3<f32>) -> f32 {
    let k0 = length(p / r);
    let k1 = length(p / (r * r));
    return k0 * (k0 - 1.0) / k1;
}

fn sdRhombus3D(p: vec3<f32>, la: f32, lb: f32, h: f32, ra: f32) -> f32 {
    var q = abs(p);
    let b = vec2<f32>(la, lb);
    let ndot_val = b.x * (b.x - 2.0 * q.x) + b.y * (2.0 * q.z - b.y);
    let f = clamp(ndot_val / dot(b, b), -1.0, 1.0);
    let qq = vec2<f32>(
        length(q.xz - 0.5 * b * vec2<f32>(1.0 - f, 1.0 + f)) * sign(q.x * b.y + q.z * b.x - b.x * b.y) - ra,
        q.y - h
    );
    return min(max(qq.x, qq.y), 0.0) + length(max(qq, vec2<f32>(0.0)));
}

fn sdLink3D(p: vec3<f32>, le: f32, r1: f32, r2: f32) -> f32 {
    let q = vec3<f32>(p.x, max(abs(p.y) - le, 0.0), p.z);
    return length(vec2<f32>(length(q.xy) - r1, q.z)) - r2;
}

// =============================================================================
// 3D Primitive evaluation - reads from cardStorage
// =============================================================================

fn evaluateSDF3D(primOffset: u32, p: vec3<f32>) -> f32 {
    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);
    let pos = vec3<f32>(
        cardStorage[primOffset + 2u],
        cardStorage[primOffset + 3u],
        cardStorage[primOffset + 4u]
    );
    let q = p - pos;

    switch (primType) {
        case SDF_SPHERE_3D: {
            return sdSphere3D(q, cardStorage[primOffset + 5u]);
        }
        case SDF_BOX_3D: {
            return sdBox3D(q, vec3<f32>(cardStorage[primOffset + 5u],
                                        cardStorage[primOffset + 6u],
                                        cardStorage[primOffset + 7u]));
        }
        case SDF_BOX_FRAME_3D: {
            return sdBoxFrame3D(q, vec3<f32>(cardStorage[primOffset + 5u],
                                             cardStorage[primOffset + 6u],
                                             cardStorage[primOffset + 7u]),
                                cardStorage[primOffset + 8u]);
        }
        case SDF_TORUS_3D: {
            return sdTorus3D(q, vec2<f32>(cardStorage[primOffset + 5u],
                                          cardStorage[primOffset + 6u]));
        }
        case SDF_CAPPED_TORUS_3D: {
            return sdCappedTorus3D(q, vec2<f32>(cardStorage[primOffset + 5u],
                                                cardStorage[primOffset + 6u]),
                                   cardStorage[primOffset + 7u],
                                   cardStorage[primOffset + 8u]);
        }
        case SDF_CYLINDER_3D: {
            return sdCylinder3D(q, cardStorage[primOffset + 5u],
                                cardStorage[primOffset + 6u]);
        }
        case SDF_CAPPED_CYLINDER_3D: {
            return sdCappedCylinder3D(q, cardStorage[primOffset + 5u],
                                      cardStorage[primOffset + 6u]);
        }
        case SDF_ROUNDED_CYLINDER_3D: {
            return sdRoundedCylinder3D(q, cardStorage[primOffset + 5u],
                                        cardStorage[primOffset + 6u],
                                        cardStorage[primOffset + 7u]);
        }
        case SDF_VERTICAL_CAPSULE_3D: {
            return sdVerticalCapsule3D(q, cardStorage[primOffset + 5u],
                                        cardStorage[primOffset + 6u]);
        }
        case SDF_CONE_3D: {
            return sdCone3D(q, vec2<f32>(cardStorage[primOffset + 5u],
                                          cardStorage[primOffset + 6u]),
                            cardStorage[primOffset + 7u]);
        }
        case SDF_CAPPED_CONE_3D: {
            return sdCappedCone3D(q, cardStorage[primOffset + 5u],
                                   cardStorage[primOffset + 6u],
                                   cardStorage[primOffset + 7u]);
        }
        case SDF_ROUND_CONE_3D: {
            return sdRoundCone3D(q, cardStorage[primOffset + 5u],
                                  cardStorage[primOffset + 6u],
                                  cardStorage[primOffset + 7u]);
        }
        case SDF_PLANE_3D: {
            return sdPlane3D(q, normalize(vec3<f32>(cardStorage[primOffset + 5u],
                                                     cardStorage[primOffset + 6u],
                                                     cardStorage[primOffset + 7u])),
                             cardStorage[primOffset + 8u]);
        }
        case SDF_HEX_PRISM_3D: {
            return sdHexPrism3D(q, vec2<f32>(cardStorage[primOffset + 5u],
                                              cardStorage[primOffset + 6u]));
        }
        case SDF_TRI_PRISM_3D: {
            return sdTriPrism3D(q, vec2<f32>(cardStorage[primOffset + 5u],
                                              cardStorage[primOffset + 6u]));
        }
        case SDF_OCTAHEDRON_3D: {
            return sdOctahedron3D(q, cardStorage[primOffset + 5u]);
        }
        case SDF_PYRAMID_3D: {
            return sdPyramid3D(q, cardStorage[primOffset + 5u]);
        }
        case SDF_ELLIPSOID_3D: {
            return sdEllipsoid3D(q, vec3<f32>(cardStorage[primOffset + 5u],
                                              cardStorage[primOffset + 6u],
                                              cardStorage[primOffset + 7u]));
        }
        case SDF_RHOMBUS_3D: {
            return sdRhombus3D(q, cardStorage[primOffset + 5u],
                                cardStorage[primOffset + 6u],
                                cardStorage[primOffset + 7u],
                                cardStorage[primOffset + 8u]);
        }
        case SDF_LINK_3D: {
            return sdLink3D(q, cardStorage[primOffset + 5u],
                             cardStorage[primOffset + 6u],
                             cardStorage[primOffset + 7u]);
        }
        default: { return 1e10; }
    }
}

// =============================================================================
// 2D Primitive evaluation - reads from cardStorage
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
        case SDF_CHAMFER_BOX: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let halfSize = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let chamfer = cardStorage[primOffset + 6u];
            return sdChamferBox(p, center, halfSize, chamfer);
        }
        case SDF_ORIENTED_BOX: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let th = cardStorage[primOffset + 6u];
            return sdOrientedBox(p, a, b, th);
        }
        case SDF_TRAPEZOID: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r1 = cardStorage[primOffset + 4u];
            let r2 = cardStorage[primOffset + 5u];
            let he = cardStorage[primOffset + 6u];
            return sdTrapezoid(p, center, r1, r2, he);
        }
        case SDF_PARALLELOGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let wi = cardStorage[primOffset + 4u];
            let he = cardStorage[primOffset + 5u];
            let sk = cardStorage[primOffset + 6u];
            return sdParallelogram(p, center, wi, he, sk);
        }
        case SDF_EQUILATERAL_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdEquilateralTriangle(p, center, r);
        }
        case SDF_ISOSCELES_TRIANGLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let q = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdTriangleIsosceles(p, center, q);
        }
        case SDF_UNEVEN_CAPSULE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r1 = cardStorage[primOffset + 4u];
            let r2 = cardStorage[primOffset + 5u];
            let h = cardStorage[primOffset + 6u];
            return sdUnevenCapsule(p, center, r1, r2, h);
        }
        case SDF_OCTOGON: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdOctogon(p, center, r);
        }
        case SDF_HEXAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdHexagram(p, center, r);
        }
        case SDF_PENTAGRAM: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            return sdPentagram(p, center, r);
        }
        case SDF_CUT_DISK: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let r = cardStorage[primOffset + 4u];
            let h = cardStorage[primOffset + 5u];
            return sdCutDisk(p, center, r, h);
        }
        case SDF_HORSESHOE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let c = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let r = cardStorage[primOffset + 6u];
            let w = vec2<f32>(cardStorage[primOffset + 7u], cardStorage[primOffset + 8u]);
            return sdHorseshoe(p, center, c, r, w);
        }
        case SDF_VESICA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let w = cardStorage[primOffset + 4u];
            let h = cardStorage[primOffset + 5u];
            return sdVesica(p, center, w, h);
        }
        case SDF_ORIENTED_VESICA: {
            let a = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let b = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let w = cardStorage[primOffset + 6u];
            return sdOrientedVesica(p, a, b, w);
        }
        case SDF_ROUNDED_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let h = cardStorage[primOffset + 4u];
            return sdRoundedCross(p, center, h);
        }
        case SDF_PARABOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let k = cardStorage[primOffset + 4u];
            return sdParabola(p, center, k);
        }
        case SDF_BLOBBY_CROSS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let he = cardStorage[primOffset + 4u];
            return sdBlobbyCross(p, center, he);
        }
        case SDF_TUNNEL: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let wh = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            return sdTunnel(p, center, wh);
        }
        case SDF_STAIRS: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let wh = vec2<f32>(cardStorage[primOffset + 4u], cardStorage[primOffset + 5u]);
            let n = cardStorage[primOffset + 6u];
            return sdStairs(p, center, wh, n);
        }
        case SDF_QUADRATIC_CIRCLE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let scale = cardStorage[primOffset + 4u];
            return sdQuadraticCircle(p, center, scale);
        }
        case SDF_HYPERBOLA: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let k = cardStorage[primOffset + 4u];
            let he = cardStorage[primOffset + 5u];
            return sdHyperbola(p, center, k, he);
        }
        case SDF_COOL_S: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let scale = cardStorage[primOffset + 4u];
            return sdCoolS(p, center, scale);
        }
        case SDF_CIRCLE_WAVE: {
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let tb = cardStorage[primOffset + 4u];
            let ra = cardStorage[primOffset + 5u];
            return sdCircleWave(p, center, tb, ra);
        }
        case SDF_COLOR_WHEEL: {
            // ColorWheel: params[0-1]=center, [2]=outerR, [3]=innerR, [4]=hue
            let center = vec2<f32>(cardStorage[primOffset + 2u], cardStorage[primOffset + 3u]);
            let outerR = cardStorage[primOffset + 4u];
            let innerR = cardStorage[primOffset + 5u];
            let hue = cardStorage[primOffset + 6u];
            // Return distance to nearest part (ring or triangle)
            let d = p - center;
            let dist = length(d);
            // Distance to hue ring
            let ringDist = abs(dist - (innerR + outerR) * 0.5) - (outerR - innerR) * 0.5;
            // Simplified: just use ring for now (triangle handled in full evaluateColorWheel)
            return ringDist;
        }
        default: {
            return 1e10;
        }
    }
}

fn getPrimitiveColors(primOffset: u32) -> vec4<u32> {
    let fillColor = bitcast<u32>(cardStorage[primOffset + 14u]);
    let strokeColor = bitcast<u32>(cardStorage[primOffset + 15u]);
    let layer = bitcast<u32>(cardStorage[primOffset + 1u]);
    return vec4<u32>(fillColor, strokeColor, layer, 0u);
}

fn getPrimitiveStrokeWidth(primOffset: u32) -> f32 {
    return cardStorage[primOffset + 16u];
}
