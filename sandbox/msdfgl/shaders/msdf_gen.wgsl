// MSDF Generation Shader for WebGPU
// Ported from msdfgl by Henrik Nyman
// Original msdfgen algorithm by Viktor Chlumsky

// Constants
const PI: f32 = 3.1415926535897932384626433832795;
const INFINITY_F: f32 = 3.402823466e+38;

const BLACK: u32 = 0u;
const RED: u32 = 1u;
const GREEN: u32 = 2u;
const BLUE: u32 = 4u;
const YELLOW: u32 = 3u;  // RED | GREEN
const MAGENTA: u32 = 5u; // BLUE | RED
const CYAN: u32 = 6u;    // BLUE | GREEN
const WHITE: u32 = 7u;   // RED | GREEN | BLUE

const IDX_CURR: i32 = 0;
const IDX_SHAPE: i32 = 1;
const IDX_INNER: i32 = 2;
const IDX_OUTER: i32 = 3;
const IDX_RED: i32 = 0;
const IDX_GREEN: i32 = 1;
const IDX_BLUE: i32 = 2;
const IDX_NEGATIVE: i32 = 0;
const IDX_POSITIVE: i32 = 1;
const IDX_MAX_INNER: i32 = 0;
const IDX_MAX_OUTER: i32 = 1;

// Uniforms
struct GenUniforms {
    projection: mat4x4<f32>,
    offset: vec2<f32>,
    translate: vec2<f32>,
    scale: vec2<f32>,
    range: f32,
    meta_offset: i32,
    point_offset: i32,
    glyph_height: f32,
};

@group(0) @binding(0) var<uniform> uniforms: GenUniforms;
@group(0) @binding(1) var<storage, read> metadata: array<u32>;
@group(0) @binding(2) var<storage, read> point_data: array<f32>;

// Segment structure for distance calculation
struct Segment {
    min_true: vec3<f32>,
    mins: array<vec2<f32>, 2>,
    nearest_points: i32,
    nearest_npoints: i32,
};

// Workspace for per-pixel calculations
struct Workspace {
    segments: array<Segment, 12>, // 4 * 3
    maximums: array<vec3<f32>, 2>,
    min_absolute: vec3<f32>,
};

// Vertex shader
struct VertexInput {
    @location(0) position: vec2<f32>,
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
};

@vertex
fn vs_main(input: VertexInput) -> VertexOutput {
    var output: VertexOutput;
    output.position = uniforms.projection * vec4<f32>(input.position + uniforms.offset, 1.0, 1.0);
    return output;
}

// Helper functions
fn meta_at(i: i32) -> u32 {
    return metadata[i];
}

fn point_at(i: i32) -> vec2<f32> {
    let idx = i * 2;
    return vec2<f32>(point_data[idx], point_data[idx + 1]);
}

fn orthonormal(v: vec2<f32>) -> vec2<f32> {
    let len = length(v);
    return vec2<f32>(v.y / len, -v.x / len);
}

fn cross2d(a: vec2<f32>, b: vec2<f32>) -> f32 {
    return a.x * b.y - a.y * b.x;
}

fn median3(d: vec3<f32>) -> f32 {
    return max(min(d.r, d.g), min(max(d.r, d.g), d.b));
}

fn less(a: vec2<f32>, b: vec2<f32>) -> bool {
    return abs(a.x) < abs(b.x) || (abs(a.x) == abs(b.x) && a.y < b.y);
}

fn segment_direction(points: i32, npoints: i32, param: f32) -> vec2<f32> {
    return mix(
        point_at(points + 1) - point_at(points),
        point_at(points + npoints - 1) - point_at(points + npoints - 2),
        param
    );
}

fn segment_point(points: i32, npoints: i32, param: f32) -> vec2<f32> {
    return mix(
        mix(point_at(points), point_at(points + 1), param),
        mix(point_at(points + npoints - 2), point_at(points + npoints - 1), param),
        param
    );
}

fn signed_distance_linear(p0: vec2<f32>, p1: vec2<f32>, origin: vec2<f32>) -> vec3<f32> {
    let aq = origin - p0;
    let ab = p1 - p0;
    let param = dot(aq, ab) / dot(ab, ab);
    var eq: vec2<f32>;
    if param > 0.5 {
        eq = p1 - origin;
    } else {
        eq = p0 - origin;
    }
    let endpoint_distance = length(eq);

    if param > 0.0 && param < 1.0 {
        let ortho_distance = dot(orthonormal(ab), aq);
        if abs(ortho_distance) < endpoint_distance {
            return vec3<f32>(ortho_distance, 0.0, param);
        }
    }
    return vec3<f32>(
        sign(cross2d(aq, ab)) * endpoint_distance,
        abs(dot(normalize(ab), normalize(eq))),
        param
    );
}

fn signed_distance_quad(p0: vec2<f32>, p1: vec2<f32>, p2: vec2<f32>, origin: vec2<f32>) -> vec3<f32> {
    let qa = p0 - origin;
    let ab = p1 - p0;
    let br = p2 - p1 - ab;
    let a = dot(br, br);
    let b = 3.0 * dot(ab, br);
    let c = 2.0 * dot(ab, ab) + dot(qa, br);
    let d = dot(qa, ab);

    var coeffs: array<f32, 3>;
    var solutions: i32;

    let _a = b / a;
    let a2 = _a * _a;
    let q = (a2 - 3.0 * (c / a)) / 9.0;
    let r = (_a * (2.0 * a2 - 9.0 * (c / a)) + 27.0 * (d / a)) / 54.0;
    let r2 = r * r;
    let q3 = q * q * q;

    let a_div_3 = _a / 3.0;
    var t = clamp(r / sqrt(q3), -1.0, 1.0);
    t = acos(t);

    var A = -pow(abs(r) + sqrt(max(r2 - q3, 0.0)), 1.0 / 3.0);
    if r < 0.0 {
        A = -A;
    }
    var B: f32;
    if A == 0.0 {
        B = 0.0;
    } else {
        B = q / A;
    }

    if r2 < q3 {
        let q_neg = -2.0 * sqrt(q);
        coeffs[0] = q_neg * cos(t / 3.0) - a_div_3;
        coeffs[1] = q_neg * cos((t + 2.0 * PI) / 3.0) - a_div_3;
        coeffs[2] = q_neg * cos((t - 2.0 * PI) / 3.0) - a_div_3;
        solutions = 3;
    } else {
        coeffs[0] = (A + B) - a_div_3;
        coeffs[1] = -0.5 * (A + B) - a_div_3;
        coeffs[2] = 0.5 * sqrt(3.0) * (A - B);
        if abs(coeffs[2]) < 1.0e-14 {
            solutions = 2;
        } else {
            solutions = 1;
        }
    }

    var min_distance = sign(cross2d(ab, qa)) * length(qa);
    var param = -dot(qa, ab) / dot(ab, ab);
    let distance_b = sign(cross2d(p2 - p1, p2 - origin)) * length(p2 - origin);

    if abs(distance_b) < abs(min_distance) {
        min_distance = distance_b;
        param = dot(origin - p1, p2 - p1) / dot(p2 - p1, p2 - p1);
    }

    for (var i = 0; i < solutions; i = i + 1) {
        if coeffs[i] > 0.0 && coeffs[i] < 1.0 {
            let endpoint = p0 + ab * 2.0 * coeffs[i] + br * coeffs[i] * coeffs[i];
            let dist = sign(cross2d(p2 - p0, endpoint - origin)) * length(endpoint - origin);
            if abs(dist) <= abs(min_distance) {
                min_distance = dist;
                param = coeffs[i];
            }
        }
    }

    var v = vec2<f32>(min_distance, 0.0);
    if param > 1.0 {
        v.y = abs(dot(normalize(p2 - p1), normalize(p2 - origin)));
    }
    if param < 0.0 {
        v.y = abs(dot(normalize(ab), normalize(qa)));
    }

    return vec3<f32>(v, param);
}

fn distance_to_pseudo_distance(npoints: i32, points: i32, d: vec3<f32>, p: vec2<f32>) -> vec2<f32> {
    if d.z >= 0.0 && d.z <= 1.0 {
        return d.xy;
    }

    var param_end: f32;
    if d.z < 0.0 {
        param_end = 0.0;
    } else {
        param_end = 1.0;
    }

    let dir = normalize(segment_direction(points, npoints, param_end));
    let aq = p - segment_point(points, npoints, param_end);
    let ts = dot(aq, dir);

    var result = d.xy;
    if (d.z < 0.0 && ts < 0.0) || (d.z >= 0.0 && ts > 0.0) {
        let pseudo_distance = cross2d(aq, dir);
        if abs(pseudo_distance) <= abs(d.x) {
            result.x = pseudo_distance;
            result.y = 0.0;
        }
    }
    return result;
}

// Fragment shader
@fragment
fn fs_main(@builtin(position) frag_coord: vec4<f32>) -> @location(0) vec4<f32> {
    var ws: Workspace;

    let coords = frag_coord.xy - uniforms.offset;
    var p = ((coords + 0.49) / uniforms.scale) - vec2<f32>(uniforms.translate.x, -uniforms.translate.y);
    p.y = (uniforms.glyph_height / uniforms.scale.y) - p.y;

    // Initialize workspace
    ws.maximums[0] = vec3<f32>(-INFINITY_F, -INFINITY_F, -INFINITY_F);
    ws.maximums[1] = vec3<f32>(-INFINITY_F, -INFINITY_F, -INFINITY_F);
    ws.min_absolute = vec3<f32>(-INFINITY_F, -INFINITY_F, -INFINITY_F);

    for (var i = 0; i < 12; i = i + 1) {
        ws.segments[i].mins[0].x = -INFINITY_F;
        ws.segments[i].mins[1].x = -INFINITY_F;
        ws.segments[i].min_true.x = -INFINITY_F;
        ws.segments[i].nearest_points = -1;
    }

    var point_index = uniforms.point_offset;
    var meta_index = uniforms.meta_offset;

    let ncontours = meta_at(meta_index);
    meta_index = meta_index + 1;

    for (var contour_i = 0u; contour_i < ncontours; contour_i = contour_i + 1u) {
        let winding = i32(meta_at(meta_index)) - 1;
        meta_index = meta_index + 1;
        let nsegments = meta_at(meta_index);
        meta_index = meta_index + 1;

        // Skip empty or degenerate contours
        if nsegments == 0u {
            continue;
        }

        let s_color = meta_at(meta_index);
        let s_npoints = meta_at(meta_index + 1);

        if nsegments == 1u && s_npoints == 2u {
            point_index = point_index + 2;
            meta_index = meta_index + 2;
            continue;
        }

        if nsegments == 2u && s_npoints == 2u && meta_at(meta_index + 3) == 2u {
            point_index = point_index + 4;
            meta_index = meta_index + 4;
            continue;
        }

        // Process segments
        var cur_points = point_index;
        var cur_color = meta_at(meta_index + 2 * (i32(nsegments) - 1));
        var cur_npoints = i32(meta_at(meta_index + 2 * (i32(nsegments) - 1) + 1));

        var prev_npoints: i32;
        if nsegments >= 2u {
            prev_npoints = i32(meta_at(meta_index + 2 * (i32(nsegments) - 2) + 1));
        } else {
            prev_npoints = i32(s_npoints);
        }
        var prev_points = point_index;

        for (var j = 0u; j < nsegments - 1u; j = j + 1u) {
            let npoints = i32(meta_at(meta_index + 2 * i32(j) + 1));
            cur_points = cur_points + (npoints - 1);
        }

        for (var j = 0u; j < nsegments - 2u && nsegments >= 2u; j = j + 1u) {
            let npoints = i32(meta_at(meta_index + 2 * i32(j) + 1));
            prev_points = prev_points + (npoints - 1);
        }

        var seg_color = s_color;
        var seg_npoints = i32(s_npoints);
        var seg_points = point_index;

        for (var seg_i = 0u; seg_i < nsegments; seg_i = seg_i + 1u) {
            // Calculate distance for this segment
            var d: vec3<f32>;
            if cur_npoints == 2 {
                d = signed_distance_linear(
                    point_at(cur_points),
                    point_at(cur_points + 1),
                    p
                );
            } else {
                d = signed_distance_quad(
                    point_at(cur_points),
                    point_at(cur_points + 1),
                    point_at(cur_points + 2),
                    p
                );
            }

            // Add true distances
            if (cur_color & RED) > 0u {
                add_segment_true_distance(&ws, IDX_CURR * 3 + IDX_RED, cur_npoints, cur_points, d);
            }
            if (cur_color & GREEN) > 0u {
                add_segment_true_distance(&ws, IDX_CURR * 3 + IDX_GREEN, cur_npoints, cur_points, d);
            }
            if (cur_color & BLUE) > 0u {
                add_segment_true_distance(&ws, IDX_CURR * 3 + IDX_BLUE, cur_npoints, cur_points, d);
            }

            // Check if point is facing edge and add pseudo distances
            if point_facing_edge(prev_npoints, prev_points, cur_npoints, cur_points,
                                 seg_npoints, seg_points, p, d.z) {
                let pd = distance_to_pseudo_distance(cur_npoints, cur_points, d, p);
                if (cur_color & RED) > 0u {
                    add_segment_pseudo_distance(&ws, IDX_CURR * 3 + IDX_RED, pd);
                }
                if (cur_color & GREEN) > 0u {
                    add_segment_pseudo_distance(&ws, IDX_CURR * 3 + IDX_GREEN, pd);
                }
                if (cur_color & BLUE) > 0u {
                    add_segment_pseudo_distance(&ws, IDX_CURR * 3 + IDX_BLUE, pd);
                }
            }

            prev_points = cur_points;
            prev_npoints = cur_npoints;
            cur_points = seg_points;
            cur_npoints = seg_npoints;
            cur_color = seg_color;

            seg_color = meta_at(meta_index + 2);
            meta_index = meta_index + 1;
            seg_points = seg_points + (seg_npoints - 1);
            seg_npoints = i32(meta_at(meta_index + 2));
            meta_index = meta_index + 1;
        }
        point_index = point_index + 1;

        set_contour_edge(&ws, winding, p);
    }

    let d = get_pixel_distance(&ws, p);
    return vec4<f32>(d / uniforms.range + 0.5, 1.0);
}

fn add_segment_true_distance(ws: ptr<function, Workspace>, segment_index: i32, npoints: i32, points: i32, d: vec3<f32>) {
    let is_less = less(d.xy, (*ws).segments[segment_index].min_true.xy);
    if is_less {
        (*ws).segments[segment_index].min_true = d;
        (*ws).segments[segment_index].nearest_points = points;
        (*ws).segments[segment_index].nearest_npoints = npoints;
    }
}

fn add_segment_pseudo_distance(ws: ptr<function, Workspace>, segment_index: i32, d: vec2<f32>) {
    var i: i32;
    if d.x < 0.0 {
        i = IDX_NEGATIVE;
    } else {
        i = IDX_POSITIVE;
    }
    let _d = (*ws).segments[segment_index].mins[i];
    if less(d, _d) {
        (*ws).segments[segment_index].mins[i] = d;
    }
}

fn point_facing_edge(prev_npoints: i32, prev_points: i32, cur_npoints: i32, cur_points: i32,
                     next_npoints: i32, next_points: i32, p: vec2<f32>, param: f32) -> bool {
    if param >= 0.0 && param <= 1.0 {
        return true;
    }

    let prev_edge_dir = -normalize(segment_direction(prev_points, prev_npoints, 1.0));
    var edge_param: f32;
    var edge_mult: f32;
    if param < 0.0 {
        edge_param = 0.0;
        edge_mult = 1.0;
    } else {
        edge_param = 1.0;
        edge_mult = -1.0;
    }
    let edge_dir = normalize(segment_direction(cur_points, cur_npoints, edge_param)) * edge_mult;
    let next_edge_dir = normalize(segment_direction(next_points, next_npoints, 0.0));
    let point_dir = p - segment_point(cur_points, cur_npoints, edge_param);

    var compare_dir: vec2<f32>;
    if param < 0.0 {
        compare_dir = prev_edge_dir;
    } else {
        compare_dir = next_edge_dir;
    }

    return dot(point_dir, edge_dir) >= dot(point_dir, compare_dir);
}

fn merge_segment(ws: ptr<function, Workspace>, s: i32, other: i32) {
    if less((*ws).segments[other].min_true.xy, (*ws).segments[s].min_true.xy) {
        (*ws).segments[s].min_true = (*ws).segments[other].min_true;
        (*ws).segments[s].nearest_npoints = (*ws).segments[other].nearest_npoints;
        (*ws).segments[s].nearest_points = (*ws).segments[other].nearest_points;
    }
    if less((*ws).segments[other].mins[IDX_NEGATIVE], (*ws).segments[s].mins[IDX_NEGATIVE]) {
        (*ws).segments[s].mins[IDX_NEGATIVE] = (*ws).segments[other].mins[IDX_NEGATIVE];
    }
    if less((*ws).segments[other].mins[IDX_POSITIVE], (*ws).segments[s].mins[IDX_POSITIVE]) {
        (*ws).segments[s].mins[IDX_POSITIVE] = (*ws).segments[other].mins[IDX_POSITIVE];
    }
}

fn merge_multi_segment(ws: ptr<function, Workspace>, e: i32, other: i32) {
    merge_segment(ws, e * 3 + IDX_RED, other * 3 + IDX_RED);
    merge_segment(ws, e * 3 + IDX_GREEN, other * 3 + IDX_GREEN);
    merge_segment(ws, e * 3 + IDX_BLUE, other * 3 + IDX_BLUE);
}

fn compute_distance(ws: ptr<function, Workspace>, segment_index: i32, point: vec2<f32>) -> f32 {
    var i: i32;
    if (*ws).segments[segment_index].min_true.x < 0.0 {
        i = IDX_NEGATIVE;
    } else {
        i = IDX_POSITIVE;
    }
    var min_distance = (*ws).segments[segment_index].mins[i].x;

    if (*ws).segments[segment_index].nearest_points == -1 {
        return min_distance;
    }

    let d = distance_to_pseudo_distance(
        (*ws).segments[segment_index].nearest_npoints,
        (*ws).segments[segment_index].nearest_points,
        (*ws).segments[segment_index].min_true,
        point
    );

    if abs(d.x) < abs(min_distance) {
        min_distance = d.x;
    }

    return min_distance;
}

fn get_distance(ws: ptr<function, Workspace>, segment_index: i32, point: vec2<f32>) -> vec3<f32> {
    return vec3<f32>(
        compute_distance(ws, segment_index * 3 + IDX_RED, point),
        compute_distance(ws, segment_index * 3 + IDX_GREEN, point),
        compute_distance(ws, segment_index * 3 + IDX_BLUE, point)
    );
}

fn set_contour_edge(ws: ptr<function, Workspace>, winding: i32, point: vec2<f32>) {
    let d = get_distance(ws, IDX_CURR, point);

    merge_multi_segment(ws, IDX_SHAPE, IDX_CURR);

    if winding > 0 && median3(d) >= 0.0 {
        merge_multi_segment(ws, IDX_INNER, IDX_CURR);
    }
    if winding < 0 && median3(d) <= 0.0 {
        merge_multi_segment(ws, IDX_OUTER, IDX_CURR);
    }

    var i: i32;
    if winding < 0 {
        i = IDX_MAX_INNER;
    } else {
        i = IDX_MAX_OUTER;
    }

    if median3(d) > median3((*ws).maximums[i]) {
        (*ws).maximums[i] = d;
    }
    if abs(median3(d)) < abs(median3((*ws).min_absolute)) {
        (*ws).min_absolute = d;
    }

    // Reset current segment for next contour
    for (var j = 0; j < 3; j = j + 1) {
        (*ws).segments[IDX_CURR * 3 + j].mins[0].x = -INFINITY_F;
        (*ws).segments[IDX_CURR * 3 + j].mins[1].x = -INFINITY_F;
        (*ws).segments[IDX_CURR * 3 + j].min_true.x = -INFINITY_F;
        (*ws).segments[IDX_CURR * 3 + j].nearest_points = -1;
    }
}

fn get_pixel_distance(ws: ptr<function, Workspace>, point: vec2<f32>) -> vec3<f32> {
    let shape_distance = get_distance(ws, IDX_SHAPE, point);
    let inner_distance = get_distance(ws, IDX_INNER, point);
    let outer_distance = get_distance(ws, IDX_OUTER, point);

    let inner_d = median3(inner_distance);
    let outer_d = median3(outer_distance);

    let inner = inner_d >= 0.0 && abs(inner_d) <= abs(outer_d);
    let outer = outer_d <= 0.0 && abs(outer_d) < abs(inner_d);

    if !inner && !outer {
        return shape_distance;
    }

    var d: vec3<f32>;
    var contour_distance: vec3<f32>;
    if inner {
        d = inner_distance;
        contour_distance = (*ws).maximums[IDX_MAX_INNER];
    } else {
        d = outer_distance;
        contour_distance = (*ws).maximums[IDX_MAX_OUTER];
    }

    var contour_d = median3(contour_distance);
    if abs(contour_d) < abs(outer_d) && contour_d > median3(d) {
        d = contour_distance;
    }

    contour_distance = (*ws).min_absolute;
    contour_d = median3(contour_distance);
    let d_d = median3(d);

    if abs(contour_d) < abs(d_d) {
        d = contour_distance;
    }
    if median3(d) == median3(shape_distance) {
        d = shape_distance;
    }

    return d;
}
