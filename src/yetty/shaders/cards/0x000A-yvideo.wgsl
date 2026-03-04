// Shader glyph: YVideo card (codepoint 1048586 / U+10000A)
// Decodes YUV420 video frames to RGB using BT.601/BT.709 color matrices
//
// =============================================================================
// DESIGN: YUV420 Video Frame Rendering
// =============================================================================
//
// Video frames are stored as 3 separate texture regions in the atlas:
//   - Y plane: Full resolution luminance
//   - U plane: Half resolution (width/2, height/2) blue-difference chroma
//   - V plane: Half resolution (width/2, height/2) red-difference chroma
//
// The fragment shader samples all three planes and applies the appropriate
// color matrix (BT.601 for SD, BT.709 for HD) to convert to RGB.
//
// =============================================================================
// Per-cell encoding (ANSI true-color compatible):
//
//   fg (24 bits): Metadata SLOT INDEX (encodes byte offset / 32)
//   bg (24 bits): Relative position within widget (relCol/relRow)
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  reserved (u32)
//   offset 1:  frameWidth (u32)       - video frame width in pixels
//   offset 2:  frameHeight (u32)      - video frame height in pixels
//   offset 3:  yAtlasX (u32)          - Y plane X position in atlas
//   offset 4:  yAtlasY (u32)          - Y plane Y position in atlas
//   offset 5:  uvAtlasX (u32)         - U/V planes X position in atlas
//   offset 6:  uvAtlasY (u32)         - U/V planes Y position in atlas (V is below U)
//   offset 7:  widthCells (u32)       - widget width in cells
//   offset 8:  heightCells (u32)      - widget height in cells
//   offset 9:  zoom (f32)             - zoom level (1.0 = fit)
//   offset 10: centerX (f32)          - pan center X [0,1]
//   offset 11: centerY (f32)          - pan center Y [0,1]
//   offset 12: colorMatrix (u32)      - 0=BT.601, 1=BT.709, 2=BT.2020
//   offset 13: bgColor (u32)          - background color (packed RGBA)
//   offset 14: scaledYWidth (u32)     - scaled Y plane width
//   offset 15: scaledYHeight (u32)    - scaled Y plane height
// =============================================================================

// Color matrix constants
const MATRIX_BT601: u32 = 0u;
const MATRIX_BT709: u32 = 1u;
const MATRIX_BT2020: u32 = 2u;

// BT.601 YUV to RGB matrix (SD content)
// R = Y + 1.402 * (V - 128)
// G = Y - 0.344136 * (U - 128) - 0.714136 * (V - 128)
// B = Y + 1.772 * (U - 128)
fn yuvToRgb_BT601(y: f32, u: f32, v: f32) -> vec3<f32> {
    let yNorm = y;
    let uNorm = u - 0.5;
    let vNorm = v - 0.5;

    return vec3<f32>(
        yNorm + 1.402 * vNorm,
        yNorm - 0.344136 * uNorm - 0.714136 * vNorm,
        yNorm + 1.772 * uNorm
    );
}

// BT.709 YUV to RGB matrix (HD content)
// R = Y + 1.5748 * (V - 128)
// G = Y - 0.1873 * (U - 128) - 0.4681 * (V - 128)
// B = Y + 1.8556 * (U - 128)
fn yuvToRgb_BT709(y: f32, u: f32, v: f32) -> vec3<f32> {
    let yNorm = y;
    let uNorm = u - 0.5;
    let vNorm = v - 0.5;

    return vec3<f32>(
        yNorm + 1.5748 * vNorm,
        yNorm - 0.1873 * uNorm - 0.4681 * vNorm,
        yNorm + 1.8556 * uNorm
    );
}

// BT.2020 YUV to RGB matrix (UHD/HDR content)
fn yuvToRgb_BT2020(y: f32, u: f32, v: f32) -> vec3<f32> {
    let yNorm = y;
    let uNorm = u - 0.5;
    let vNorm = v - 0.5;

    return vec3<f32>(
        yNorm + 1.4746 * vNorm,
        yNorm - 0.1646 * uNorm - 0.5714 * vNorm,
        yNorm + 1.8814 * uNorm
    );
}

fn shaderGlyph_1048586(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // ==========================================================================
    // Decode ANSI-compatible cell encoding
    // ==========================================================================
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;  // 64-byte slots = 16 u32s per slot

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // ==========================================================================
    // Read metadata
    // ==========================================================================
    let frameWidth = cardMetadata[metaOffset + 1u];
    let frameHeight = cardMetadata[metaOffset + 2u];
    let yAtlasX = cardMetadata[metaOffset + 3u];
    let yAtlasY = cardMetadata[metaOffset + 4u];
    let uvAtlasX = cardMetadata[metaOffset + 5u];
    let uvAtlasY = cardMetadata[metaOffset + 6u];
    let widthCells = cardMetadata[metaOffset + 7u];
    let heightCells = cardMetadata[metaOffset + 8u];
    let zoom = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let centerX = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let centerY = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let colorMatrix = cardMetadata[metaOffset + 12u];
    let bgColorPacked = cardMetadata[metaOffset + 13u];
    let scaledYWidth = cardMetadata[metaOffset + 14u];
    let scaledYHeight = cardMetadata[metaOffset + 15u];

    let bgColor = unpackColor(bgColorPacked);

    // Use scaled dimensions if available
    let texWidth = select(frameWidth, scaledYWidth, scaledYWidth > 0u);
    let texHeight = select(frameHeight, scaledYHeight, scaledYHeight > 0u);

    // UV plane dimensions (half of Y plane)
    let uvWidth = texWidth / 2u;
    let uvHeight = texHeight / 2u;

    // ==========================================================================
    // Compute widget-wide UV
    // ==========================================================================
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // Apply zoom and pan
    var textureUV = (widgetUV - vec2<f32>(0.5)) / zoom + vec2<f32>(centerX, centerY);

    // Bounds check
    if (textureUV.x < 0.0 || textureUV.x > 1.0 || textureUV.y < 0.0 || textureUV.y > 1.0) {
        return bgColor;
    }

    // ==========================================================================
    // Sample Y, U, V planes from atlas
    // ==========================================================================
    let atlasSize = vec2<f32>(textureDimensions(cardImageAtlas));

    // Y plane (full resolution)
    let yUV = (vec2<f32>(f32(yAtlasX), f32(yAtlasY)) +
               textureUV * vec2<f32>(f32(texWidth), f32(texHeight))) / atlasSize;
    let yVal = textureSampleLevel(cardImageAtlas, cardImageSampler, yUV, 0.0).r;

    // U plane (half resolution, stored below Y in atlas)
    // U is at uvAtlasX, uvAtlasY
    let uUV = (vec2<f32>(f32(uvAtlasX), f32(uvAtlasY)) +
               textureUV * vec2<f32>(f32(uvWidth), f32(uvHeight))) / atlasSize;
    let uVal = textureSampleLevel(cardImageAtlas, cardImageSampler, uUV, 0.0).r;

    // V plane (half resolution, stored to the right of U in atlas)
    let vUV = (vec2<f32>(f32(uvAtlasX) + f32(uvWidth), f32(uvAtlasY)) +
               textureUV * vec2<f32>(f32(uvWidth), f32(uvHeight))) / atlasSize;
    let vVal = textureSampleLevel(cardImageAtlas, cardImageSampler, vUV, 0.0).r;

    // ==========================================================================
    // Convert YUV to RGB
    // ==========================================================================
    var rgb: vec3<f32>;
    if (colorMatrix == MATRIX_BT601) {
        rgb = yuvToRgb_BT601(yVal, uVal, vVal);
    } else if (colorMatrix == MATRIX_BT2020) {
        rgb = yuvToRgb_BT2020(yVal, uVal, vVal);
    } else {
        // Default to BT.709
        rgb = yuvToRgb_BT709(yVal, uVal, vVal);
    }

    return clamp(rgb, vec3<f32>(0.0), vec3<f32>(1.0));
}
