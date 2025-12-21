// Uniform buffer for projection and view
struct Uniforms {
    projection: mat4x4<f32>,
    screenSize: vec2<f32>,
    cellSize: vec2<f32>,
    pixelRange: f32,
    _pad1: f32,
    _pad2: f32,
    _pad3: f32,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var fontTexture: texture_2d<f32>;
@group(0) @binding(2) var fontSampler: sampler;

// Per-instance vertex data
struct VertexInput {
    @location(0) position: vec2<f32>,      // Quad corner (0,0), (1,0), (0,1), (1,1)
    @location(1) cellPos: vec2<f32>,       // Grid cell position (col, row)
    @location(2) uvMin: vec2<f32>,         // Glyph UV min in atlas
    @location(3) uvMax: vec2<f32>,         // Glyph UV max in atlas
    @location(4) glyphSize: vec2<f32>,     // Glyph size in pixels
    @location(5) glyphBearing: vec2<f32>,  // Glyph bearing (offset)
    @location(6) fgColor: vec4<f32>,       // Foreground color
    @location(7) bgColor: vec4<f32>,       // Background color
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
    @location(1) fgColor: vec4<f32>,
    @location(2) bgColor: vec4<f32>,
    @location(3) cellUV: vec2<f32>,        // Position within cell (0-1)
};

@vertex
fn vs_main(input: VertexInput) -> VertexOutput {
    var output: VertexOutput;

    // Calculate cell position in pixels
    let cellPixelPos = input.cellPos * uniforms.cellSize;

    // Position glyph within cell
    // Baseline is at ~80% of cell height from top
    let baseline = uniforms.cellSize.y * 0.8;

    // glyphBearing.y is the top of the glyph relative to baseline (positive = above baseline)
    // In screen coords (Y down), top of glyph = baseline - bearing.y
    let glyphTop = baseline - input.glyphBearing.y;

    let glyphOffset = vec2<f32>(
        (uniforms.cellSize.x - input.glyphSize.x) * 0.5 + input.glyphBearing.x,
        glyphTop
    );

    // Calculate vertex position
    // input.position: (0,0)=top-left, (1,1)=bottom-right of quad
    let localPos = input.position * input.glyphSize;
    let pixelPos = cellPixelPos + glyphOffset + localPos;

    // Transform to clip space
    output.position = uniforms.projection * vec4<f32>(pixelPos, 0.0, 1.0);

    // UV interpolation - texture and UVs are already in screen space (top-left origin)
    // uvMin = top-left, uvMax = bottom-right
    output.uv = mix(input.uvMin, input.uvMax, input.position);

    output.fgColor = input.fgColor;
    output.bgColor = input.bgColor;
    output.cellUV = input.position;

    return output;
}

// MSDF median function
fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    // Sample MSDF texture
    let msdf = textureSample(fontTexture, fontSampler, input.uv);

    // Calculate signed distance
    let sd = median(msdf.r, msdf.g, msdf.b);

    // Calculate screen-space distance for anti-aliasing
    let screenTexSize = vec2<f32>(textureDimensions(fontTexture));
    let unitRange = uniforms.pixelRange / screenTexSize;
    let screenSize = uniforms.cellSize;  // Approximate glyph screen size
    let screenRange = max(screenSize.x * unitRange.x, screenSize.y * unitRange.y);

    // Apply anti-aliased edge
    let opacity = clamp((sd - 0.5) * screenRange + 0.5, 0.0, 1.0);

    // Mix foreground and background colors based on glyph coverage
    let color = mix(input.bgColor, input.fgColor, opacity);

    return color;
}

// Background quad shader - renders cell backgrounds
struct BgVertexInput {
    @location(0) position: vec2<f32>,    // Quad corner
    @location(1) cellPos: vec2<f32>,     // Grid cell position
    @location(2) bgColor: vec4<f32>,     // Background color
};

struct BgVertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) bgColor: vec4<f32>,
};

@vertex
fn vs_background(input: BgVertexInput) -> BgVertexOutput {
    var output: BgVertexOutput;

    let cellPixelPos = input.cellPos * uniforms.cellSize;
    let pixelPos = cellPixelPos + input.position * uniforms.cellSize;

    output.position = uniforms.projection * vec4<f32>(pixelPos, 0.0, 1.0);
    output.bgColor = input.bgColor;

    return output;
}

@fragment
fn fs_background(input: BgVertexOutput) -> @location(0) vec4<f32> {
    return input.bgColor;
}
