// Shader glyph: Voronoi (codepoint 1048595 / U+100013)
// Correct cell interior distances for Voronoi patterns
// Based on "Voronoi - distances" by Inigo Quilez (MIT License)
// https://iquilezles.org/articles/voronoilines

fn voronoi_hash2(p: vec2<f32>) -> vec2<f32> {
    // Procedural white noise
    return fract(sin(vec2<f32>(
        dot(p, vec2<f32>(127.1, 311.7)),
        dot(p, vec2<f32>(269.5, 183.3))
    )) * 43758.5453);
}

fn voronoi_calc(x: vec2<f32>, time: f32) -> vec3<f32> {
    let ip = floor(x);
    let fp = fract(x);

    // First pass: regular voronoi
    var mg: vec2<f32>;
    var mr: vec2<f32>;
    var md = 8.0;

    for (var j = -1; j <= 1; j++) {
        for (var i = -1; i <= 1; i++) {
            let g = vec2<f32>(f32(i), f32(j));
            var o = voronoi_hash2(ip + g);
            // Animate
            o = 0.5 + 0.5 * sin(time + 6.2831 * o);
            let r = g + o - fp;
            let d = dot(r, r);

            if (d < md) {
                md = d;
                mr = r;
                mg = g;
            }
        }
    }

    // Second pass: distance to borders
    md = 8.0;
    for (var j = -2; j <= 2; j++) {
        for (var i = -2; i <= 2; i++) {
            let g = mg + vec2<f32>(f32(i), f32(j));
            var o = voronoi_hash2(ip + g);
            // Animate
            o = 0.5 + 0.5 * sin(time + 6.2831 * o);
            let r = g + o - fp;

            if (dot(mr - r, mr - r) > 0.00001) {
                md = min(md, dot(0.5 * (mr + r), normalize(r - mr)));
            }
        }
    }

    return vec3<f32>(md, mr);
}

fn shaderGlyph_1048595(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let screenSize = vec2<f32>(globals.screenWidth, globals.screenHeight);
    let p = pixelPos / screenSize.x;

    let c = voronoi_calc(8.0 * p, time);

    // Isolines
    var col = c.x * (0.5 + 0.5 * sin(64.0 * c.x)) * vec3<f32>(1.0);

    // Borders (orange)
    col = mix(vec3<f32>(1.0, 0.6, 0.0), col, smoothstep(0.04, 0.07, c.x));

    // Feature points
    let dd = length(c.yz);
    col = mix(vec3<f32>(1.0, 0.6, 0.1), col, smoothstep(0.0, 0.12, dd));
    col = col + vec3<f32>(1.0, 0.6, 0.1) * (1.0 - smoothstep(0.0, 0.04, dd));

    return col;
}
