// Shader glyph: Minkowski Tube (codepoint 1114106 / U+10FFFA)
// Traced rounded square tunnel with Voronoi-like texturing
// Based on "Simplified, Traced Minkowski Tube" by Shane from Shadertoy

fn minkowski_rot(th: f32) -> mat2x2<f32> {
    let cs = cos(th);
    let si = sin(th);
    return mat2x2<f32>(cs, -si, si, cs);
}

// 3D Voronoi-like function (Voronesque)
fn minkowski_voronesque(p_in: vec3<f32>) -> f32 {
    // Skewing the cubic grid, then determining the first vertex
    let i = floor(p_in + dot(p_in, vec3<f32>(0.333333)));
    var p = p_in - i + dot(i, vec3<f32>(0.166666));

    // Breaking the skewed cube into tetrahedra
    let i1 = step(vec3<f32>(p.y, p.z, p.x), p);
    let i2 = max(i1, 1.0 - vec3<f32>(i1.z, i1.x, i1.y));
    let i1_min = min(i1, 1.0 - vec3<f32>(i1.z, i1.x, i1.y));

    // Calculate the other three vertices
    let p1 = p - i1_min + 0.166666;
    let p2 = p - i2 + 0.333333;
    let p3 = p - 0.5;

    let rnd = vec3<f32>(7.0, 157.0, 113.0);

    // Falloff values from the skewed point to each tetrahedral point
    var v = max(vec4<f32>(0.5) - vec4<f32>(dot(p, p), dot(p1, p1), dot(p2, p2), dot(p3, p3)), vec4<f32>(0.0));

    // Assign random values to each point
    var d = vec4<f32>(dot(i, rnd), dot(i + i1_min, rnd), dot(i + i2, rnd), dot(i + 1.0, rnd));

    // Further randomize and combine
    d = fract(sin(d) * 262144.0) * v * 2.0;

    // Determine largest and second largest falloff values
    v.x = max(d.x, d.y);
    v.y = max(d.z, d.w);
    v.z = max(min(d.x, d.y), min(d.z, d.w));
    v.w = min(v.x, v.y);

    // Maximum minus second order for beveled Voronoi look
    return max(v.x, v.y) - max(v.z, v.w);
}

fn shaderGlyph_1114106(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);

    // Screen coordinates with movement
    var uv = (pixelPos - screenSize * 0.5) / screenSize.y +
             vec2<f32>(0.5 * cos(time * 0.5), 0.25 * sin(time * 0.5));

    // Unit direction ray
    var rd = normalize(vec3<f32>(uv, 1.0));

    // Subtle camera rotation
    let rotXY = minkowski_rot(sin(time * 0.25) * 0.5);
    rd = vec3<f32>(rotXY[0][0] * rd.x + rotXY[0][1] * rd.y,
                   rotXY[1][0] * rd.x + rotXY[1][1] * rd.y,
                   rd.z);
    let rotXZ = minkowski_rot(sin(time * 0.25) * 0.5);
    rd = vec3<f32>(rotXZ[0][0] * rd.x + rotXZ[0][1] * rd.z,
                   rd.y,
                   rotXZ[1][0] * rd.x + rotXZ[1][1] * rd.z);

    var col = vec3<f32>(0.0);

    // Minkowski distance for rounded square tube
    let scale = vec2<f32>(0.75, 1.0);
    let power = 6.0;

    // Surface distance calculation
    let absRdScaled = pow(abs(rd.xy) * scale, vec2<f32>(power));
    var sDist = max(dot(absRdScaled, vec2<f32>(1.0)), 1e-16);
    sDist = 1.0 / pow(sDist, 1.0 / power);

    // Surface position: sp = ro + rd * t
    let sp = vec3<f32>(0.0, 0.0, time * 2.0) + rd * sDist;

    // Minkowski normal
    let signSp = sign(sp);
    let absSp = abs(sp.xy) * scale;
    let normalXY = pow(absSp, vec2<f32>(power - 1.0));
    let sn_raw = -signSp * vec3<f32>(normalXY, 0.0);
    var sn = normalize(sn_raw);

    // Bump mapping
    let eps = 0.025;
    let c = minkowski_voronesque(sp * 2.5);

    // 3D gradient for bump
    let gr_x = minkowski_voronesque((sp - vec3<f32>(eps, 0.0, 0.0)) * 2.5);
    let gr_y = minkowski_voronesque((sp - vec3<f32>(0.0, eps, 0.0)) * 2.5);
    let gr_z = minkowski_voronesque((sp - vec3<f32>(0.0, 0.0, eps)) * 2.5);
    var gr = (vec3<f32>(gr_x, gr_y, gr_z) - c) / eps;
    gr = gr - sn * dot(sn, gr);
    sn = normalize(sn + gr * 0.1);

    // Lighting
    let lp = vec3<f32>(0.0, 0.0, time * 2.0 + 3.0);
    var ld = lp - sp;
    let dist = max(length(ld), 0.001);
    ld = ld / dist;

    // Attenuation
    let atten = min(1.5 / max(1.0 + dist * 0.25 + dist * dist * 0.5, 0.001), 1.0);

    // Diffuse and specular
    let diff = max(dot(sn, ld), 0.0);
    let spec = pow(max(dot(reflect(-ld, sn), -rd), 0.0), 16.0);

    // Fake reflection
    let ref_val = minkowski_voronesque((sp + reflect(rd, sn) * 0.5) * 2.5);

    // Red palette coloring
    let objCol = pow(min(vec3<f32>(1.5, 1.0, 1.0) * (c * 0.97 + 0.03), vec3<f32>(1.0)), vec3<f32>(1.0, 3.0, 16.0));

    // Final color
    col = (objCol * (diff + ref_val * 0.35 + 0.25 + vec3<f32>(1.0, 0.9, 0.7) * spec) +
           (c + 0.35) * vec3<f32>(0.25, 0.5, 1.0) * ref_val) * atten;

    return sqrt(clamp(col, vec3<f32>(0.0), vec3<f32>(1.0)));
}
