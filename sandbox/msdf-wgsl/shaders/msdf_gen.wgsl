// MSDF Generation Compute Shader
// Based on msdfgen algorithm by Viktor Chlumsky

const PI: f32 = 3.14159265358979323846;
const INFINITY_F: f32 = 1e20;

// Color constants for edge coloring
const BLACK: u32 = 0u;
const RED: u32 = 1u;
const GREEN: u32 = 2u;
const BLUE: u32 = 4u;
const YELLOW: u32 = 3u;  // RED | GREEN
const MAGENTA: u32 = 5u; // BLUE | RED
const CYAN: u32 = 6u;    // BLUE | GREEN
const WHITE: u32 = 7u;   // RED | GREEN | BLUE

// Uniforms for current glyph
struct GlyphUniforms {
    atlas_offset: vec2<u32>,  // Where to write in atlas
    glyph_size: vec2<u32>,    // Size of glyph in pixels
    translate: vec2<f32>,     // Translation (padding offset)
    scale: f32,               // Scale factor
    range: f32,               // Distance field range
    meta_offset: u32,         // Offset into metadata buffer
    point_offset: u32,        // Offset into points buffer
    glyph_height: f32,        // Glyph height for Y flip
    _padding: u32,
};

@group(0) @binding(0) var<uniform> uniforms: GlyphUniforms;
@group(0) @binding(1) var<storage, read> metadata: array<u32>;
@group(0) @binding(2) var<storage, read> points: array<f32>;
@group(0) @binding(3) var output_texture: texture_storage_2d<rgba32float, write>;

// Get point from buffer (each point is 2 floats)
fn get_point(idx: u32) -> vec2<f32> {
    let base = idx * 2u;
    return vec2<f32>(points[base], points[base + 1u]);
}

// Get metadata value
fn get_meta(idx: u32) -> u32 {
    return metadata[idx];
}

// 2D cross product
fn cross2d(a: vec2<f32>, b: vec2<f32>) -> f32 {
    return a.x * b.y - a.y * b.x;
}

// Equivalent to msdfgen's nonZeroSign: returns -1 for v <= 0, +1 for v > 0
// Unlike sign(), never returns 0 — prevents zero-distance artifacts on tangent extensions
fn nonZeroSign(v: f32) -> f32 {
    return select(-1.0, 1.0, v > 0.0);
}

// Signed distance to a line segment
// Returns vec4: (signed_dist, ortho, clamped_t, ext_param)
fn distance_to_line(p0: vec2<f32>, p1: vec2<f32>, origin: vec2<f32>) -> vec4<f32> {
    let aq = origin - p0;
    let ab = p1 - p0;
    let param = dot(aq, ab) / dot(ab, ab);  // unclamped parameter
    let t = clamp(param, 0.0, 1.0);
    let closest = p0 + t * ab;
    let to_origin = origin - closest;
    let dist = length(to_origin);

    let sign_val = -nonZeroSign(cross2d(ab, to_origin));
    let signed_dist = sign_val * dist;

    var ortho = 0.0;
    if param <= 0.0 {
        ortho = abs(dot(normalize(ab), normalize(aq)));
    } else if param >= 1.0 {
        ortho = abs(dot(normalize(ab), normalize(origin - p1)));
    }

    return vec4<f32>(signed_dist, ortho, t, param);
}

// Signed distance to a quadratic bezier curve with pseudo-distance at endpoints
fn distance_to_quad(p0: vec2<f32>, p1: vec2<f32>, p2: vec2<f32>, origin: vec2<f32>) -> vec3<f32> {
    let qa = p0 - origin;
    let ab = p1 - p0;
    let br = p2 - p1 - ab;

    let a = dot(br, br);
    let b = 3.0 * dot(ab, br);
    let c = 2.0 * dot(ab, ab) + dot(qa, br);
    let d = dot(qa, ab);

    // Solve cubic equation for closest point parameter
    var solutions: array<f32, 3>;
    var num_solutions = 0;

    if abs(a) > 1e-10 {
        let _a = b / a;
        let a2 = _a * _a;
        let q = (a2 - 3.0 * (c / a)) / 9.0;
        let r = (_a * (2.0 * a2 - 9.0 * (c / a)) + 27.0 * (d / a)) / 54.0;
        let r2 = r * r;
        let q3 = q * q * q;
        let a_div_3 = _a / 3.0;

        if r2 < q3 {
            let t = acos(clamp(r / sqrt(q3), -1.0, 1.0));
            let q_neg = -2.0 * sqrt(q);
            solutions[0] = q_neg * cos(t / 3.0) - a_div_3;
            solutions[1] = q_neg * cos((t + 2.0 * PI) / 3.0) - a_div_3;
            solutions[2] = q_neg * cos((t - 2.0 * PI) / 3.0) - a_div_3;
            num_solutions = 3;
        } else {
            var A = -pow(abs(r) + sqrt(max(r2 - q3, 0.0)), 1.0 / 3.0);
            if r < 0.0 { A = -A; }
            var B = 0.0;
            if abs(A) > 1e-10 { B = q / A; }
            solutions[0] = (A + B) - a_div_3;
            solutions[1] = -0.5 * (A + B) - a_div_3;
            let imag = 0.5 * sqrt(3.0) * (A - B);
            if abs(imag) < 1e-10 {
                num_solutions = 2;
            } else {
                num_solutions = 1;
            }
        }
    } else if abs(b) > 1e-10 {
        // Quadratic case
        let disc = c * c - 4.0 * b * d;
        if disc >= 0.0 {
            let sq = sqrt(disc);
            solutions[0] = (-c + sq) / (2.0 * b);
            solutions[1] = (-c - sq) / (2.0 * b);
            num_solutions = 2;
        }
    } else if abs(c) > 1e-10 {
        solutions[0] = -d / c;
        num_solutions = 1;
    }

    // Tangent directions at endpoints (with degenerate fallback)
    var dir0 = ab;
    if dot(dir0, dir0) < 1e-10 {
        dir0 = p2 - p0;
    }
    var dir1 = p2 - p1;
    if dot(dir1, dir1) < 1e-10 {
        dir1 = p2 - p0;
    }

    // Start with endpoint p0 (t=0)
    // msdfgen: nonZeroSign(crossProduct(direction(0), qa)) * qa.length()
    var min_dist = nonZeroSign(cross2d(dir0, qa)) * length(qa);
    // Extended parameter for pseudo-distance (matches CPU's param computation)
    var ext_param = -dot(qa, dir0) / dot(dir0, dir0);

    // Check endpoint p2 (t=1)
    // msdfgen: nonZeroSign(crossProduct(direction(1), p2-origin)) * |p2-origin|
    let p2q = p2 - origin;
    let dist_end = nonZeroSign(cross2d(dir1, p2q)) * length(p2q);
    if abs(dist_end) < abs(min_dist) {
        min_dist = dist_end;
        ext_param = dot(origin - p1, dir1) / dot(dir1, dir1);
    }

    // Check interior solutions
    for (var i = 0; i < num_solutions; i = i + 1) {
        let t = solutions[i];
        if t > 0.0 && t < 1.0 {
            // Point on curve: B(t) = p0 + 2*t*ab + t^2*br
            let qe = qa + ab * 2.0 * t + br * t * t;
            let tangent = ab + br * t;
            let dist = nonZeroSign(cross2d(tangent, qe)) * length(qe);
            if abs(dist) <= abs(min_dist) {
                min_dist = dist;
                ext_param = t;
            }
        }
    }

    // Pseudo-distance at endpoints (matches msdfgen's distanceToPerpendicularDistance)
    if ext_param < 0.0 {
        let dir = normalize(dir0);
        let ep = origin - p0;
        let ts = dot(ep, dir);
        if ts < 0.0 {
            let perp = cross2d(ep, dir);
            if abs(perp) <= abs(min_dist) {
                min_dist = perp;
            }
        }
    } else if ext_param > 1.0 {
        let dir = normalize(dir1);
        let ep = origin - p2;
        let ts = dot(ep, dir);
        if ts > 0.0 {
            let perp = cross2d(ep, dir);
            if abs(perp) <= abs(min_dist) {
                min_dist = perp;
            }
        }
    }

    var ortho = 0.0;
    if ext_param > 1.0 {
        ortho = abs(dot(normalize(dir1), normalize(p2q)));
    }
    if ext_param < 0.0 {
        ortho = abs(dot(normalize(dir0), normalize(qa)));
    }

    return vec3<f32>(min_dist, ortho, clamp(ext_param, 0.0, 1.0));
}

// Get direction of segment at parameter t
fn segment_direction(point_idx: u32, npoints: u32, t: f32) -> vec2<f32> {
    if npoints == 2u {
        let p0 = get_point(point_idx);
        let p1 = get_point(point_idx + 1u);
        return p1 - p0;
    } else {
        let p0 = get_point(point_idx);
        let p1 = get_point(point_idx + 1u);
        let p2 = get_point(point_idx + 2u);
        let tangent = mix(p1 - p0, p2 - p1, t);
        if dot(tangent, tangent) < 1e-10 {
            return p2 - p0;
        }
        return tangent;
    }
}

// Main MSDF calculation
@compute @workgroup_size(8, 8, 1)
fn main(@builtin(global_invocation_id) global_id: vec3<u32>) {
    let pixel_x = global_id.x;
    let pixel_y = global_id.y;

    // Check bounds
    if pixel_x >= uniforms.glyph_size.x || pixel_y >= uniforms.glyph_size.y {
        return;
    }

    // Calculate position in glyph space
    let pixel_pos = vec2<f32>(f32(pixel_x) + 0.5, f32(pixel_y) + 0.5);
    var p = (pixel_pos / uniforms.scale) - uniforms.translate;
    p.y = (uniforms.glyph_height / uniforms.scale) - p.y;

    // Initialize distances for R, G, B channels
    var min_dist_r = INFINITY_F;
    var min_dist_g = INFINITY_F;
    var min_dist_b = INFINITY_F;

    // Track minimum absolute distance for each channel
    var min_abs_r = INFINITY_F;
    var min_abs_g = INFINITY_F;
    var min_abs_b = INFINITY_F;

    // Read glyph structure from metadata
    var meta_idx = uniforms.meta_offset;
    var point_idx = uniforms.point_offset;

    let ncontours = get_meta(meta_idx);
    meta_idx = meta_idx + 1u;

    // Process each contour
    for (var contour_i = 0u; contour_i < ncontours; contour_i = contour_i + 1u) {
        let winding = i32(get_meta(meta_idx)) - 1;
        meta_idx = meta_idx + 1u;
        let nsegments = get_meta(meta_idx);
        meta_idx = meta_idx + 1u;

        if nsegments == 0u {
            continue;
        }

        // Skip degenerate contours
        let first_npoints = get_meta(meta_idx + 1u);
        if nsegments == 1u && first_npoints == 2u {
            point_idx = point_idx + 2u;
            meta_idx = meta_idx + 2u;
            continue;
        }

        // Process each segment
        let segment_meta_start = meta_idx;
        let segment_point_start = point_idx;

        for (var seg_i = 0u; seg_i < nsegments; seg_i = seg_i + 1u) {
            let color = get_meta(meta_idx);
            meta_idx = meta_idx + 1u;
            let npoints = get_meta(meta_idx);
            meta_idx = meta_idx + 1u;

            // Calculate distance to this segment
            var d: vec3<f32>;
            if npoints == 2u {
                let p0 = get_point(point_idx);
                let p1 = get_point(point_idx + 1u);
                d = distance_to_line(p0, p1, p);
            } else {
                let p0 = get_point(point_idx);
                let p1 = get_point(point_idx + 1u);
                let p2 = get_point(point_idx + 2u);
                d = distance_to_quad(p0, p1, p2, p);
            }

            let signed_dist = d.x;
            let abs_dist = abs(signed_dist);

            // Update per-channel minimum distances
            if (color & RED) != 0u {
                if abs_dist < min_abs_r {
                    min_abs_r = abs_dist;
                    min_dist_r = signed_dist;
                }
            }
            if (color & GREEN) != 0u {
                if abs_dist < min_abs_g {
                    min_abs_g = abs_dist;
                    min_dist_g = signed_dist;
                }
            }
            if (color & BLUE) != 0u {
                if abs_dist < min_abs_b {
                    min_abs_b = abs_dist;
                    min_dist_b = signed_dist;
                }
            }

            point_idx = point_idx + npoints - 1u;
        }
        point_idx = point_idx + 1u;
    }

    // Handle case where no edges were found for a channel (use overall minimum)
    let min_overall = min(min(min_abs_r, min_abs_g), min_abs_b);
    if min_abs_r >= INFINITY_F { min_dist_r = select(-min_overall, min_overall, min_dist_g > 0.0 || min_dist_b > 0.0); }
    if min_abs_g >= INFINITY_F { min_dist_g = select(-min_overall, min_overall, min_dist_r > 0.0 || min_dist_b > 0.0); }
    if min_abs_b >= INFINITY_F { min_dist_b = select(-min_overall, min_overall, min_dist_r > 0.0 || min_dist_g > 0.0); }

    // Normalize to 0-1 range centered at 0.5
    // msdfgen convention: output = dist / (2 * range) + 0.5
    let inv_range = 1.0 / (2.0 * uniforms.range);
    let normalized = vec3<f32>(
        min_dist_r * inv_range + 0.5,
        min_dist_g * inv_range + 0.5,
        min_dist_b * inv_range + 0.5
    );

    // Write to atlas
    let atlas_pos = vec2<i32>(
        i32(uniforms.atlas_offset.x + pixel_x),
        i32(uniforms.atlas_offset.y + pixel_y)
    );

    textureStore(output_texture, atlas_pos, vec4<f32>(normalized, 1.0));
}
