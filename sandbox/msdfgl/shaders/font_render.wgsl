// Font Rendering Shader for WebGPU
// Ported from msdfgl by Henrik Nyman
// Uses vertex instancing instead of geometry shaders

// Uniforms
struct RenderUniforms {
    projection: mat4x4<f32>,
    font_projection: mat4x4<f32>,
    padding: f32,
    units_per_em: f32,
    dpi: vec2<f32>,
};

@group(0) @binding(0) var<uniform> uniforms: RenderUniforms;
@group(0) @binding(1) var font_atlas: texture_2d<f32>;
@group(0) @binding(2) var font_sampler: sampler;
@group(0) @binding(3) var<storage, read> font_index: array<f32>;

// Glyph instance data (per-glyph attributes)
struct GlyphInstance {
    @location(0) position: vec2<f32>,      // x, y position
    @location(1) color: vec4<f32>,         // RGBA color (unpacked)
    @location(2) glyph_index: i32,         // Glyph atlas index
    @location(3) size: f32,                // Font size
    @location(4) y_offset: f32,            // Vertical offset
    @location(5) skewness: f32,            // Italic skew
    @location(6) strength: f32,            // Bold strength
};

// Vertex output
struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) text_pos: vec2<f32>,
    @location(1) text_color: vec4<f32>,
    @location(2) strength: f32,
};

// Quad vertices (2 triangles forming a quad)
// Vertex IDs 0-5 form two triangles: 0,1,2 and 2,1,3
fn get_quad_offset(vertex_id: u32) -> vec2<f32> {
    // BL=0, BR=1, TL=2, TR=3
    // Triangle 1: BL, BR, TL (0, 1, 2)
    // Triangle 2: TL, BR, TR (2, 1, 3)
    let indices = array<u32, 6>(0u, 1u, 2u, 2u, 1u, 3u);
    let idx = indices[vertex_id];

    var offset: vec2<f32>;
    switch idx {
        case 0u: { offset = vec2<f32>(0.0, 1.0); }  // BL
        case 1u: { offset = vec2<f32>(1.0, 1.0); }  // BR
        case 2u: { offset = vec2<f32>(0.0, 0.0); }  // TL
        case 3u: { offset = vec2<f32>(1.0, 0.0); }  // TR
        default: { offset = vec2<f32>(0.0, 0.0); }
    }
    return offset;
}

@vertex
fn vs_main(
    instance: GlyphInstance,
    @builtin(vertex_index) vertex_id: u32
) -> VertexOutput {
    var output: VertexOutput;

    output.text_color = instance.color;
    output.strength = instance.strength;

    let font_size = vec4<f32>(instance.size * uniforms.dpi / 72.0 / uniforms.units_per_em, 1.0, 1.0);

    // Read glyph data from index buffer
    let _offset = 8 * instance.glyph_index;
    let text_offset = vec2<f32>(
        font_index[_offset + 0],
        font_index[_offset + 1]
    );
    let glyph_texture_width = vec2<f32>(font_index[_offset + 2], 0.0);
    let glyph_texture_height = vec2<f32>(0.0, font_index[_offset + 3]);

    let bearing = vec4<f32>(
        font_index[_offset + 4],
        -font_index[_offset + 5],
        0.0, 0.0
    ) * font_size;

    let glyph_width = vec4<f32>(font_index[_offset + 6], 0.0, 0.0, 0.0) * font_size;
    let glyph_height = vec4<f32>(0.0, font_index[_offset + 7], 0.0, 0.0) * font_size;

    let padding_x = vec4<f32>(uniforms.padding, 0.0, 0.0, 0.0) * font_size;
    let padding_y = vec4<f32>(0.0, uniforms.padding, 0.0, 0.0) * font_size;

    // Get which corner of the quad we're processing
    let quad_offset = get_quad_offset(vertex_id);

    // Base position with y offset
    var p = vec4<f32>(instance.position, 0.0, 1.0) + vec4<f32>(0.0, instance.y_offset, 0.0, 0.0);

    // Calculate vertex position based on corner
    var _p: vec4<f32>;
    var tex_pos: vec2<f32>;

    // Interpolate between corners
    // x: 0 = left (bearing - padding), 1 = right (bearing + glyph_width + padding)
    // y: 0 = top (bearing - padding), 1 = bottom (bearing + glyph_height + padding)

    if quad_offset.x < 0.5 && quad_offset.y > 0.5 {
        // Bottom-Left
        _p = p + bearing + glyph_height - padding_x + padding_y;
        tex_pos = text_offset + glyph_texture_height;
    } else if quad_offset.x > 0.5 && quad_offset.y > 0.5 {
        // Bottom-Right
        _p = p + bearing + glyph_height + glyph_width + padding_x + padding_y;
        tex_pos = text_offset + glyph_texture_width + glyph_texture_height;
    } else if quad_offset.x < 0.5 && quad_offset.y < 0.5 {
        // Top-Left
        _p = p + bearing - padding_x - padding_y;
        tex_pos = text_offset;
    } else {
        // Top-Right
        _p = p + bearing + glyph_width + padding_x - padding_y;
        tex_pos = text_offset + glyph_texture_width;
    }

    // Apply skew for italic effect
    _p.x = _p.x + instance.skewness * (p.y - _p.y);

    output.position = uniforms.projection * _p;
    output.text_pos = tex_pos;

    return output;
}

// Fragment shader
fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    // Transform texture coordinates
    let coords = (uniforms.font_projection * vec4<f32>(input.text_pos, 0.0, 1.0)).xy;

    // Invert the strength so that 1.0 becomes bold and 0.0 becomes thin
    let threshold = 1.0 - input.strength;

    let atlas_size = vec2<f32>(textureDimensions(font_atlas));
    let px_range = 4.0;
    let msdf_unit = px_range / atlas_size;

    let s = textureSample(font_atlas, font_sampler, coords).rgb;
    var sig_dist = median(s.r, s.g, s.b) - threshold;

    // Calculate screen-space derivative for antialiasing
    let dx = dpdx(coords);
    let dy = dpdy(coords);
    let fw = sqrt(dx.x * dx.x + dy.x * dy.x + dx.y * dx.y + dy.y * dy.y);
    sig_dist = sig_dist * dot(msdf_unit, vec2<f32>(0.5 / fw));

    let opacity = clamp(sig_dist + 0.5, 0.0, 1.0);

    return mix(vec4<f32>(0.0, 0.0, 0.0, 0.0), input.text_color, opacity);
}
