// Classic plasma effect - colorful animated pattern
// Works great as background decorator

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = uv * 3.0;
    let t = iTime * 0.5;

    var v = 0.0;
    v += sin(p.x + t);
    v += sin(p.y + t * 0.5);
    v += sin(p.x + p.y + t * 0.3);
    v += sin(sqrt(p.x * p.x + p.y * p.y) + t);
    v /= 4.0;

    // Rainbow colors
    let r = sin(v * 3.14159 + 0.0) * 0.5 + 0.5;
    let g = sin(v * 3.14159 + 2.094) * 0.5 + 0.5;
    let b = sin(v * 3.14159 + 4.188) * 0.5 + 0.5;

    return vec4<f32>(r, g, b, 1.0);
}
