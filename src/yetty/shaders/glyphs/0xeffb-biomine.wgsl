// Shader glyph: Biomine (codepoint 1114107 / U+10FFFB)
// Raymarched biocooling system / alien hatchery scene
// Based on "Biomine" by Shane from Shadertoy
// Uses pixelPos for seamless rendering across cells

const BIOMINE_FAR: f32 = 50.0;

fn biomine_hash(n: f32) -> f32 {
    return fract(cos(n) * 45758.5453);
}

fn biomine_rot2(a: f32) -> mat2x2<f32> {
    let v = sin(vec2<f32>(1.570796, 0.0) + a);
    return mat2x2<f32>(v.x, v.y, -v.y, v.x);
}

fn biomine_noise3D(p_in: vec3<f32>) -> f32 {
    let s = vec3<f32>(7.0, 157.0, 113.0);
    let ip = floor(p_in);
    var p = p_in - ip;
    var h = vec4<f32>(0.0, s.y, s.z, s.y + s.z) + dot(ip, s);
    p = p * p * (3.0 - 2.0 * p);
    let h1 = fract(sin(h) * 43758.5453);
    let h2 = fract(sin(h + s.x) * 43758.5453);
    var hm = mix(h1, h2, p.x);
    let hxy = mix(vec2<f32>(hm.x, hm.z), vec2<f32>(hm.y, hm.w), p.y);
    return mix(hxy.x, hxy.y, p.z);
}

fn biomine_drawSphere(p_in: vec3<f32>) -> f32 {
    let p = fract(p_in) - 0.5;
    return dot(p, p);
}

fn biomine_cellTile(p_in: vec3<f32>) -> f32 {
    var p = p_in;
    var d: vec4<f32>;
    d.x = biomine_drawSphere(p - vec3<f32>(0.81, 0.62, 0.53));
    let pxy1 = vec2<f32>(p.y - p.x, p.y + p.x) * 0.7071;
    p = vec3<f32>(pxy1.x, pxy1.y, p.z);
    d.y = biomine_drawSphere(p - vec3<f32>(0.39, 0.2, 0.11));
    let pyz1 = vec2<f32>(p.z - p.y, p.z + p.y) * 0.7071;
    p = vec3<f32>(p.x, pyz1.x, pyz1.y);
    d.z = biomine_drawSphere(p - vec3<f32>(0.62, 0.24, 0.06));
    let pxz1 = vec2<f32>(p.z - p.x, p.z + p.x) * 0.7071;
    p = vec3<f32>(pxz1.x, p.y, pxz1.y);
    d.w = biomine_drawSphere(p - vec3<f32>(0.2, 0.82, 0.64));

    var v: vec4<f32>;
    v.x = min(d.x, d.z);
    v.y = min(d.y, d.w);
    v.z = min(max(d.x, d.y), max(d.z, d.w));
    v.w = max(v.x, v.y);

    return (min(v.z, v.w) - min(v.x, v.y)) * 2.66;
}

fn biomine_path(z: f32) -> vec2<f32> {
    let a = sin(z * 0.11);
    let b = cos(z * 0.14);
    return vec2<f32>(a * 4.0 - b * 1.5, b * 1.7 + a * 1.5);
}

fn biomine_smaxP(a: f32, b: f32, s: f32) -> f32 {
    let h = clamp(0.5 + 0.5 * (a - b) / s, 0.0, 1.0);
    return mix(b, a, h) + h * (1.0 - h) * s;
}

struct BiomineMapResult {
    dist: f32,
    objID: f32,
}

fn biomine_map(p_in: vec3<f32>, time: f32) -> BiomineMapResult {
    var p = p_in;
    p = vec3<f32>(p.x - biomine_path(p.z).x, p.y - biomine_path(p.z).y, p.z);
    p = p + cos(vec3<f32>(p.z, p.x, p.y) * 1.5707963) * 0.2;

    let d = dot(cos(p * 1.5707963), sin(vec3<f32>(p.y, p.z, p.x) * 1.5707963)) + 1.0;

    let bio = d + 0.25 + dot(sin(p * 1.0 + time * 6.283 + sin(vec3<f32>(p.y, p.z, p.x) * 0.5)), vec3<f32>(0.033));

    let tunBase = 3.25 - length(vec2<f32>(p.x, p.y - 1.0)) + 0.5 * cos(p.z * 3.14159 / 32.0);
    let tun = biomine_smaxP(tunBase, 0.75 - d, 1.0) - abs(1.5 - d) * 0.375;

    var result: BiomineMapResult;
    result.objID = step(tun, bio);
    result.dist = min(tun, bio);
    return result;
}

fn biomine_bumpSurf3D(p: vec3<f32>, saveID: f32) -> f32 {
    let noi = biomine_noise3D(p * 96.0);

    if (saveID > 0.5) {
        let sf = biomine_cellTile(p * 0.75);
        let vor = biomine_cellTile(p * 1.5);
        return sf * 0.66 + (vor * 0.94 + noi * 0.06) * 0.34;
    } else {
        let ps = p / 3.0;
        let ct = biomine_cellTile(ps * 2.0 + sin(ps * 12.0) * 0.5) * 0.66 +
                 biomine_cellTile(ps * 6.0 + sin(ps * 36.0) * 0.5) * 0.34;
        return (1.0 - smoothstep(-0.2, 0.25, ct)) * 0.9 + noi * 0.1;
    }
}

fn biomine_doBumpMap(p: vec3<f32>, nor: vec3<f32>, bumpfactor: f32, saveID: f32) -> vec3<f32> {
    let e = 0.001;
    let ref_val = biomine_bumpSurf3D(p, saveID);
    let grad = vec3<f32>(
        biomine_bumpSurf3D(p - vec3<f32>(e, 0.0, 0.0), saveID),
        biomine_bumpSurf3D(p - vec3<f32>(0.0, e, 0.0), saveID),
        biomine_bumpSurf3D(p - vec3<f32>(0.0, 0.0, e), saveID)
    ) - ref_val;
    let grad_adj = grad / e - nor * dot(nor, grad / e);
    return normalize(nor + grad_adj * bumpfactor);
}

fn biomine_trace(ro: vec3<f32>, rd: vec3<f32>, time: f32) -> BiomineMapResult {
    var t = 0.0;
    var result: BiomineMapResult;
    result.objID = 0.0;

    for (var i = 0; i < 72; i++) {
        let mapRes = biomine_map(ro + rd * t, time);
        let h = mapRes.dist;
        result.objID = mapRes.objID;

        if (abs(h) < 0.002 * (t * 0.125 + 1.0) || t > BIOMINE_FAR) {
            break;
        }
        t += step(h, 1.0) * h * 0.2 + h * 0.5;
    }

    result.dist = min(t, BIOMINE_FAR);
    return result;
}

fn biomine_getNormal(p: vec3<f32>, time: f32) -> vec3<f32> {
    let e = 0.002;
    return normalize(vec3<f32>(
        biomine_map(p + vec3<f32>(e, 0.0, 0.0), time).dist - biomine_map(p - vec3<f32>(e, 0.0, 0.0), time).dist,
        biomine_map(p + vec3<f32>(0.0, e, 0.0), time).dist - biomine_map(p - vec3<f32>(0.0, e, 0.0), time).dist,
        biomine_map(p + vec3<f32>(0.0, 0.0, e), time).dist - biomine_map(p - vec3<f32>(0.0, 0.0, e), time).dist
    ));
}

fn biomine_thickness(p: vec3<f32>, n: vec3<f32>, maxDist: f32, falloff: f32, time: f32) -> f32 {
    let nbIte = 6.0;
    var ao = 0.0;

    for (var i = 1.0; i < nbIte + 0.5; i += 1.0) {
        let l = (i * 0.75 + fract(cos(i) * 45758.5453) * 0.25) / nbIte * maxDist;
        ao += (l + biomine_map(p - n * l, time).dist) / pow(1.0 + l, falloff);
    }

    return clamp(1.0 - ao / nbIte, 0.0, 1.0);
}

fn biomine_calculateAO(p: vec3<f32>, n: vec3<f32>, time: f32) -> f32 {
    var ao = 0.0;
    let maxDist = 4.0;
    let nbIte = 6.0;

    for (var i = 1.0; i < nbIte + 0.5; i += 1.0) {
        let l = (i + biomine_hash(i)) * 0.5 / nbIte * maxDist;
        ao += (l - biomine_map(p + n * l, time).dist) / (1.0 + l);
    }

    return clamp(1.0 - ao / nbIte, 0.0, 1.0);
}

fn biomine_eMap(rd_in: vec3<f32>, time: f32) -> vec3<f32> {
    var rd = rd_in;
    rd.y += time;
    rd = rd / 3.0;

    let ct = biomine_cellTile(rd * 2.0 + sin(rd * 12.0) * 0.5) * 0.66 +
             biomine_cellTile(rd * 6.0 + sin(rd * 36.0) * 0.5) * 0.34;
    let texCol = vec3<f32>(0.25, 0.2, 0.15) * (1.0 - smoothstep(-0.1, 0.3, ct)) +
                 vec3<f32>(0.02, 0.02, 0.53) / 6.0;
    return smoothstep(vec3<f32>(0.0), vec3<f32>(1.0), texCol);
}

fn shaderGlyph_1114107(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    let uv = (pixelPos - screenSize * 0.5) / screenSize.y;

    // Camera setup
    var camPos = vec3<f32>(0.0, 1.0, time * 2.0);
    var lookAt = camPos + vec3<f32>(0.0, 0.0, 0.1);
    var lightPos = camPos + vec3<f32>(0.0, 0.5, 5.0);

    // Apply path
    let lookAtPath = biomine_path(lookAt.z);
    let camPosPath = biomine_path(camPos.z);
    let lightPosPath = biomine_path(lightPos.z);
    lookAt = vec3<f32>(lookAt.x + lookAtPath.x, lookAt.y + lookAtPath.y, lookAt.z);
    camPos = vec3<f32>(camPos.x + camPosPath.x, camPos.y + camPosPath.y, camPos.z);
    lightPos = vec3<f32>(lightPos.x + lightPosPath.x, lightPos.y + lightPosPath.y, lightPos.z);

    // Ray direction
    let FOV = 3.14159265 / 2.0;
    let forward = normalize(lookAt - camPos);
    let right = normalize(vec3<f32>(forward.z, 0.0, -forward.x));
    let up = cross(forward, right);

    var rd = normalize(forward + FOV * uv.x * right + FOV * uv.y * up);

    // Camera swivel
    let rotMat = biomine_rot2(biomine_path(lookAt.z).x / 16.0);
    rd = vec3<f32>(rotMat[0][0] * rd.x + rotMat[0][1] * rd.y,
                   rotMat[1][0] * rd.x + rotMat[1][1] * rd.y,
                   rd.z);

    // Raymarch
    let traceResult = biomine_trace(camPos, rd, time);
    let t = traceResult.dist;
    let saveID = traceResult.objID;

    var sceneCol = vec3<f32>(0.0);

    if (t < BIOMINE_FAR) {
        let sp = t * rd + camPos;
        var sn = biomine_getNormal(sp, time);

        // Bump mapping
        if (saveID > 0.5) {
            sn = biomine_doBumpMap(sp, sn, 0.2, saveID);
        } else {
            sn = biomine_doBumpMap(sp, sn, 0.008, saveID);
        }

        let ao = biomine_calculateAO(sp, sn, time);

        var ld = lightPos - sp;
        let distlpsp = max(length(ld), 0.001);
        ld = ld / distlpsp;

        let atten = 1.0 / (1.0 + distlpsp * 0.25);
        let ambience = 0.5;
        let diff = max(dot(sn, ld), 0.0);
        let spec = pow(max(dot(reflect(-ld, sn), -rd), 0.0), 32.0);
        let fre = pow(clamp(dot(sn, rd) + 1.0, 0.0, 1.0), 1.0);

        // Texturing
        var texCol: vec3<f32>;
        if (saveID > 0.5) {
            // Tunnel walls
            texCol = vec3<f32>(0.3) * (biomine_noise3D(sp * 32.0) * 0.66 + biomine_noise3D(sp * 64.0) * 0.34) *
                     (1.0 - biomine_cellTile(sp * 16.0) * 0.75);
            texCol = texCol * (smoothstep(-0.1, 0.5, biomine_cellTile(sp * 0.75) * 0.66 + biomine_cellTile(sp * 1.5) * 0.34) * 0.85 + 0.15);
        } else {
            // Biotubes
            let sps = sp / 3.0;
            let ct = biomine_cellTile(sps * 2.0 + sin(sps * 12.0) * 0.5) * 0.66 +
                     biomine_cellTile(sps * 6.0 + sin(sps * 36.0) * 0.5) * 0.34;
            texCol = vec3<f32>(0.35, 0.25, 0.2) * (1.0 - smoothstep(-0.1, 0.25, ct)) + vec3<f32>(0.1, 0.01, 0.004);
        }

        // Translucency
        let hf = normalize(ld + sn);
        let th = biomine_thickness(sp, sn, 1.0, 1.0, time);
        let tdiff = pow(clamp(dot(rd, -hf), 0.0, 1.0), 1.0);
        let trans = pow((tdiff + 0.0) * th, 4.0);

        // Combine lighting
        sceneCol = texCol * (diff + ambience) + vec3<f32>(0.7, 0.9, 1.0) * spec;
        if (saveID < 0.5) {
            sceneCol = sceneCol + vec3<f32>(0.7, 0.9, 1.0) * spec * spec;
        }
        sceneCol = sceneCol + texCol * vec3<f32>(0.8, 0.95, 1.0) * pow(fre, 4.0) * 2.0;
        sceneCol = sceneCol + vec3<f32>(1.0, 0.07, 0.15) * trans * 1.5;

        // Fake reflection/refraction for biotubes
        if (saveID < 0.5) {
            let ref_vec = reflect(rd, sn);
            var em = biomine_eMap(ref_vec, time);
            sceneCol = sceneCol + em * 0.5;
            let refr_vec = refract(rd, sn, 1.0 / 1.3);
            em = biomine_eMap(refr_vec, time);
            sceneCol = sceneCol + em * vec3<f32>(2.0, 0.2, 0.3) * 1.5;
        }

        sceneCol = sceneCol * atten * ao;
    }

    // Sky blend
    let sky = vec3<f32>(2.0, 0.9, 0.8);
    sceneCol = mix(sky, sceneCol, 1.0 / (t * t / BIOMINE_FAR / BIOMINE_FAR * 8.0 + 1.0));

    return sqrt(clamp(sceneCol, vec3<f32>(0.0), vec3<f32>(1.0)));
}
