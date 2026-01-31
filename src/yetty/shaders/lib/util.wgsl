// Utility functions: noise, random, hashing
// Available to all shader glyphs via automatic lib loading.

// Hash functions based on Dave Hoskins' hash functions
// https://www.shadertoy.com/view/4djSRW

fn util_hash11(p_in: f32) -> f32 {
    var p = fract(p_in * 0.1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

fn util_hash21(p: vec2<f32>) -> f32 {
    var p3 = fract(vec3<f32>(p.x, p.y, p.x) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

fn util_hash31(p: vec3<f32>) -> f32 {
    var p3 = fract(p * 0.1031);
    p3 += dot(p3, p3.zyx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

fn util_hash22(p: vec2<f32>) -> vec2<f32> {
    var p3 = fract(vec3<f32>(p.x, p.y, p.x) * vec3<f32>(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);
}

fn util_hash33(p: vec3<f32>) -> vec3<f32> {
    var p3 = fract(p * vec3<f32>(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yxz + 33.33);
    return fract((p3.xxy + p3.yxx) * p3.zyx);
}

// Value noise (smooth random) - 2D
fn util_valueNoise(p: vec2<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f); // smoothstep interpolation

    return mix(
        mix(util_hash21(i + vec2<f32>(0.0, 0.0)),
            util_hash21(i + vec2<f32>(1.0, 0.0)), u.x),
        mix(util_hash21(i + vec2<f32>(0.0, 1.0)),
            util_hash21(i + vec2<f32>(1.0, 1.0)), u.x),
        u.y
    );
}

// Value noise - 3D (useful for replacing texture lookups)
fn util_valueNoise3(p: vec3<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (3.0 - 2.0 * f);

    return mix(
        mix(mix(util_hash31(i + vec3<f32>(0.0, 0.0, 0.0)),
                util_hash31(i + vec3<f32>(1.0, 0.0, 0.0)), u.x),
            mix(util_hash31(i + vec3<f32>(0.0, 1.0, 0.0)),
                util_hash31(i + vec3<f32>(1.0, 1.0, 0.0)), u.x), u.y),
        mix(mix(util_hash31(i + vec3<f32>(0.0, 0.0, 1.0)),
                util_hash31(i + vec3<f32>(1.0, 0.0, 1.0)), u.x),
            mix(util_hash31(i + vec3<f32>(0.0, 1.0, 1.0)),
                util_hash31(i + vec3<f32>(1.0, 1.0, 1.0)), u.x), u.y),
        u.z
    );
}

// Colored noise: returns vec3, replacement for texture(iChannel0, uv).rgb
// Each channel uses a different hash seed offset
fn util_colorNoise(uv: vec2<f32>) -> vec3<f32> {
    return vec3<f32>(
        util_valueNoise(uv * 256.0),
        util_valueNoise(uv * 256.0 + vec2<f32>(37.0, 17.0)),
        util_valueNoise(uv * 256.0 + vec2<f32>(59.0, 83.0))
    );
}
