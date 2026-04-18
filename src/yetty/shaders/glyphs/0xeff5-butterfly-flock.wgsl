// Shader glyph: Butterfly Flock (codepoint 1114101 / U+10FFF5)
// Credit: Shadertoy community
// Converted from GLSL to WGSL
//
// Raymarched flock of butterflies with flapping wings, procedural coloring,
// and pre-step culling for performance. iChannel0 (noise) replaced with
// procedural noise from lib/util.wgsl. iChannel1 (background) replaced
// with a simple sky gradient.

const BFLY_OBJECTS: i32 = 40;
const BFLY_CACHED: i32 = 5;
const BFLY_OBJECT_SIZE: f32 = 0.5;
const BFLY_MOUSE_ROTATE_SPEED: f32 = 5.0;

const BFLY_MATERIAL_BODY: f32 = 0.0;
const BFLY_MATERIAL_WING: f32 = 1.0;

struct BflyObject {
    pos: vec3<f32>,
    rad: f32,
    index: i32,
};

struct BflyTraceResult {
    color: vec4<f32>,
};

struct BflyMapResult {
    dist_mat: vec2<f32>,
    localPos: vec3<f32>,
    hitIndex: i32,
};

// Module-scope arrays for the object lists
var<private> bfly_sdObjects: array<BflyObject, 40>;
var<private> bfly_cachedObjects: array<BflyObject, 5>;
var<private> bfly_maxCacheIndex: i32;
var<private> bfly_time: f32;

fn bfly_rotx(a: f32) -> mat3x3<f32> {
    let c = cos(a);
    let s = sin(a);
    return mat3x3<f32>(
        vec3<f32>(1.0, 0.0, 0.0),
        vec3<f32>(0.0, c, -s),
        vec3<f32>(0.0, s, c)
    );
}

fn bfly_roty(a: f32) -> mat3x3<f32> {
    let c = cos(a);
    let s = sin(a);
    return mat3x3<f32>(
        vec3<f32>(c, 0.0, s),
        vec3<f32>(0.0, 1.0, 0.0),
        vec3<f32>(-s, 0.0, c)
    );
}

fn bfly_rotz(a: f32) -> mat3x3<f32> {
    let c = cos(a);
    let s = sin(a);
    return mat3x3<f32>(
        vec3<f32>(c, -s, 0.0),
        vec3<f32>(s, c, 0.0),
        vec3<f32>(0.0, 0.0, 1.0)
    );
}

fn bfly_udBox(p: vec3<f32>, b: vec3<f32>) -> f32 {
    return length(max(abs(p) - b, vec3<f32>(0.0)));
}

fn bfly_sdHexPrism(p: vec3<f32>, h: vec2<f32>) -> f32 {
    let q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + q.y * 0.5, q.y) - h.x);
}

fn bfly_getModel(pos: vec3<f32>, index: i32) -> vec2<f32> {
    let phase = f32(index);

    let bl = (sin(pos.z * 12.0 - 5.0) * 0.5 + 0.5) + 0.3;
    let body = bfly_sdHexPrism(pos, vec2<f32>(BFLY_OBJECT_SIZE * 0.04 * bl, BFLY_OBJECT_SIZE * 0.2));

    let l = length(pos);
    let wx = max(abs(l * 6.0 + 0.2) - 0.4, 0.0);
    let sl = 1.5 * abs(sin(wx)) + 0.05;

    let wing = vec3<f32>(BFLY_OBJECT_SIZE * 0.5, BFLY_OBJECT_SIZE * 0.01, BFLY_OBJECT_SIZE * 0.25 * sl);

    let w1 = bfly_udBox(bfly_rotz(sin(bfly_time * 22.0 + phase)) * pos - vec3<f32>(BFLY_OBJECT_SIZE * 0.5, 0.0, 0.0), wing);
    let w2 = bfly_udBox(bfly_rotz(-sin(bfly_time * 22.0 + phase)) * pos + vec3<f32>(BFLY_OBJECT_SIZE * 0.5, 0.0, 0.0), wing);

    var id = BFLY_MATERIAL_BODY;
    if (w1 < body || w2 < body) {
        id = BFLY_MATERIAL_WING;
    }

    var m = min(body, w1);
    m = min(m, w2);

    return vec2<f32>(m, id);
}

fn bfly_map(rp: vec3<f32>) -> BflyMapResult {
    var m = 9999.0;
    var ret = vec2<f32>(m, 0.0);
    var localPos = vec3<f32>(0.0);
    var hitIndex = 0;

    for (var i = 0; i < BFLY_CACHED; i++) {
        if (i <= bfly_maxCacheIndex) {
            let lp = rp - bfly_cachedObjects[i].pos;
            let mat = bfly_getModel(lp, bfly_cachedObjects[i].index);
            if (mat.x < m) {
                m = mat.x;
                ret = mat;
                localPos = lp;
                hitIndex = bfly_cachedObjects[i].index;
            }
        }
    }

    return BflyMapResult(ret, localPos, hitIndex);
}

fn bfly_prestep(ro: vec3<f32>, rp: vec3<f32>, rd: vec3<f32>, rd90x: vec3<f32>, rd90y: vec3<f32>) -> f32 {
    bfly_maxCacheIndex = -1;
    var m = 99999.0;

    for (var i = 0; i < BFLY_OBJECTS; i++) {
        let sp = -ro + bfly_sdObjects[i].pos;

        let distToPlaneY = abs(dot(rd90y, sp));
        let distToPlaneX = abs(dot(rd90x, sp));

        var distanceToPlanes = max(distToPlaneY, distToPlaneX);
        distanceToPlanes = distanceToPlanes - bfly_sdObjects[i].rad;

        let mat = bfly_getModel(rp - bfly_sdObjects[i].pos * (1.0 + distanceToPlanes), bfly_sdObjects[i].index);
        m = min(m, mat.x);

        if (distanceToPlanes <= 0.0) {
            bfly_maxCacheIndex += 1;
            if (bfly_maxCacheIndex < BFLY_CACHED) {
                bfly_cachedObjects[bfly_maxCacheIndex] = bfly_sdObjects[i];
            } else {
                return m;
            }
        }
    }

    return m;
}

fn bfly_trace(rp_in: vec3<f32>, rd: vec3<f32>) -> vec4<f32> {
    let ro = rp_in;
    var color = vec4<f32>(0.0);

    let tmp = normalize(cross(rd, vec3<f32>(0.0, 1.0, 0.0)));
    let up = normalize(cross(rd, tmp));
    let right = cross(rd, up);

    var travel = bfly_prestep(ro, rp_in, rd, right, up);
    var rp = rp_in + travel * rd;

    for (var i = 0; i < 50; i++) {
        let res = bfly_map(rp);
        let dist = res.dist_mat.x;

        if (dist <= 0.0) {
            let id = res.dist_mat.y;
            let indx = f32(res.hitIndex);
            let c1 = sin(indx * 0.1) * 0.5 + 0.5;
            let c2 = abs(cos(abs(res.localPos.z * 15.0)) + sin(abs(res.localPos.x) * 15.0));
            let c3 = cos(indx * 0.4);
            color = vec4<f32>(id, c2 * id, c1 * id, 1.0) * abs(sin(indx * 0.1));
            color.a = 1.0;
            return color;
        }

        let dst = max(0.01, dist);
        travel += dst;
        rp += rd * dst;
        if (travel > 30.0) {
            return color;
        }
    }

    return color;
}

// Sky gradient replacement for iChannel1 background
fn bfly_sky(rd: vec3<f32>) -> vec4<f32> {
    let t = rd.y * 0.5 + 0.5;
    let sky = mix(vec3<f32>(0.6, 0.7, 0.9), vec3<f32>(0.15, 0.2, 0.4), t);
    return vec4<f32>(sky, 1.0);
}

fn shaderGlyph_1114101(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    bfly_time = time;

    var uv = pixelPos / screenSize - vec2<f32>(0.5);
    uv.y /= screenSize.x / screenSize.y;

    var mouse = mousePos / screenSize - vec2<f32>(0.5);
    if (mouse.x == -0.5 && mouse.y == -0.5) {
        mouse = vec2<f32>(0.0);
    }
    mouse *= BFLY_MOUSE_ROTATE_SPEED;

    // Initialize butterfly positions from procedural noise (replaces iChannel0 texture)
    for (var i = 0; i < BFLY_OBJECTS; i++) {
        let noiseUV = sin(time * 0.001) + 0.21 * vec2<f32>(f32(i));
        let noiseVal = util_colorNoise(noiseUV) - vec3<f32>(0.5);
        var p = noiseVal;
        p = bfly_roty(time * 2.0) * p;
        p.z += (sin(time) * 0.5 + 0.5) * 1.0;
        p.x *= 1.0 + (sin(time * 0.1) * 0.5 + 0.5) * 0.25;
        p.y *= 1.0 + (cos(time * 0.1) * 0.5 + 0.5) * 0.25;
        bfly_sdObjects[i] = BflyObject(p * 10.0, BFLY_OBJECT_SIZE, i);
    }

    let rp = vec3<f32>(0.0, 0.0, 1.0);
    var rd = normalize(vec3<f32>(uv, 0.3));
    rd = bfly_rotx(mouse.y) * rd;
    rd = bfly_roty(mouse.x) * rd;

    var fragColor = bfly_trace(rp, rd);

    // Background: sky gradient (replaces iChannel1 texture)
    let bgRd = bfly_roty(3.14159 * 0.5) * rd;
    fragColor = mix(fragColor, bfly_sky(bgRd), 1.0 - fragColor.a);

    // Color grading
    let luma = (fragColor.r + fragColor.g + fragColor.b) * 0.33;
    fragColor -= luma * vec4<f32>(0.9, 0.5, 0.0, 1.0) * clamp(rd.y - 0.05, 0.0, 1.0);
    fragColor += vec4<f32>(0.2, 0.4, 0.0, 0.0) * abs(clamp(rd.y, -1.0, 0.0));

    // Frame vignette
    fragColor = mix(fragColor, vec4<f32>(0.0), 1.0 - smoothstep(0.5, 0.45, abs(uv.x)));
    fragColor = mix(fragColor, vec4<f32>(0.0), 1.0 - smoothstep(0.28, 0.2, abs(uv.y)));

    return max(fragColor.rgb, vec3<f32>(0.0));
}
