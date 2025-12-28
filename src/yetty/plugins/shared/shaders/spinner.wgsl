// Loading spinner - rotating arc
// Great for hourglass emoji or loading indicator

fn fs_main(uv: vec2<f32>) -> vec4<f32> {
    let p = (uv - 0.5) * 2.0;
    let d = length(p);

    // Ring
    let ring = abs(d - 0.6) - 0.12;

    // Rotating arc using angle
    let angle = atan2(p.y, p.x);
    let rotAngle = angle - iTime * 3.0;

    // Arc covers 270 degrees
    let arc = smoothstep(0.0, 0.1, rotAngle + 3.14159) *
              smoothstep(0.0, 0.1, 2.356 - rotAngle);

    // Gradient along arc for tail effect
    let grad = (rotAngle + 3.14159) / 5.5;

    var alpha = 0.0;
    if (ring < 0.0) {
        alpha = arc * grad;
    }

    // Color - cyan/blue
    let color = vec3<f32>(0.2, 0.7, 1.0);

    return vec4<f32>(color, alpha);
}
