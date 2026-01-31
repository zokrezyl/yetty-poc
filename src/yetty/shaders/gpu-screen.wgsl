// Shared uniforms (managed by Yetty, group 0)
struct SharedUniforms {
    time: f32,           // 4 bytes - animation time
    deltaTime: f32,      // 4 bytes
    screenWidth: f32,    // 4 bytes
    screenHeight: f32,   // 4 bytes
    mouseX: f32,         // 4 bytes
    mouseY: f32,         // 4 bytes
    lastChar: u32,       // 4 bytes - last typed character glyph index
    lastCharTime: f32,   // 4 bytes - time when last character was typed
};

// Grid uniforms (managed by renderer, group 1)
struct GridUniforms {
    projection: mat4x4<f32>,   // 64 bytes
    screenSize: vec2<f32>,     // 8 bytes
    cellSize: vec2<f32>,       // 8 bytes
    gridSize: vec2<f32>,       // 8 bytes (cols, rows)
    pixelRange: f32,           // 4 bytes
    scale: f32,                // 4 bytes
    cursorPos: vec2<f32>,      // 8 bytes (col, row)
    cursorVisible: f32,        // 4 bytes
    cursorShape: f32,          // 4 bytes (1=block, 2=underline, 3=bar)
    viewportOrigin: vec2<f32>, // 8 bytes - viewport origin in framebuffer
    cursorBlink: f32,          // 4 bytes (0=no blink, 1=blink)
    _pad2: f32,                // 4 bytes padding for alignment
};

// Glyph metadata (40 bytes per glyph, matches C++ GlyphMetadataGPU)
struct GlyphMetadata {
    uvMin: vec2<f32>,      // 8 bytes
    uvMax: vec2<f32>,      // 8 bytes
    size: vec2<f32>,       // 8 bytes (glyph size in pixels)
    bearing: vec2<f32>,    // 8 bytes (glyph offset from baseline)
    advance: f32,          // 4 bytes
    _pad: f32,             // 4 bytes
};

// Emoji glyph metadata (32 bytes per emoji, matches C++ EmojiGlyphMetadata)
struct EmojiGlyphMetadata {
    uvMin: vec2<f32>,      // 8 bytes
    uvMax: vec2<f32>,      // 8 bytes
    size: vec2<f32>,       // 8 bytes (glyph size in pixels at atlas resolution)
    _pad: vec2<f32>,       // 8 bytes padding
};

// Cell structure - matches C++ Cell struct (12 bytes)
// Uploaded directly from CPU, zero-copy
struct Cell {
    glyph: u32,    // 4 bytes - glyph index (room for shader-based glyphs)
    fg: u32,       // 4 bytes - packed: fgR | (fgG << 8) | (fgB << 16) | (alpha << 24)
    bg: u32,       // 4 bytes - packed: bgR | (bgG << 8) | (bgB << 16) | (style << 24)
};

// Shared bindings (group 0 - managed by Yetty)
@group(0) @binding(0) var<uniform> globals: SharedUniforms;
@group(0) @binding(1) var<storage, read> cardMetadata: array<u32>;
@group(0) @binding(2) var<storage, read> cardStorage: array<f32>;
// Card image atlas - populated by compute shader from cardImageData buffer
// Contains only visible card images, recycled per frame
@group(0) @binding(3) var cardImageAtlas: texture_2d<f32>;
@group(0) @binding(4) var cardImageSampler: sampler;
// Card image data buffer - raw RGBA8 pixels stored by ImageCard
// Fragment shader can read directly from here (bypassing atlas for simplicity)
@group(0) @binding(5) var<storage, read> cardImageData: array<u32>;

// Grid bindings (group 1 - managed by renderer)
@group(1) @binding(0) var<uniform> grid: GridUniforms;
@group(1) @binding(1) var fontTexture: texture_2d<f32>;
@group(1) @binding(2) var fontSampler: sampler;
@group(1) @binding(3) var<storage, read> glyphMetadata: array<GlyphMetadata>;
@group(1) @binding(4) var<storage, read> cellBuffer: array<Cell>;  // Cell buffer - zero copy from CPU

// Emoji bindings (group 1 continued)
@group(1) @binding(5) var emojiTexture: texture_2d<f32>;       // RGBA8Unorm - color emoji atlas
@group(1) @binding(6) var emojiSampler: sampler;
@group(1) @binding(7) var<storage, read> emojiMetadata: array<EmojiGlyphMetadata>;

// Attribute bit masks (matches CellAttrs in grid.h)
const ATTR_BOLD: u32 = 0x01u;           // Bit 0
const ATTR_ITALIC: u32 = 0x02u;         // Bit 1
const ATTR_UNDERLINE_MASK: u32 = 0x0Cu; // Bits 2-3 (0=none, 1=single, 2=double, 3=curly)
const ATTR_STRIKETHROUGH: u32 = 0x10u;  // Bit 4
const ATTR_FONT_TYPE_MASK: u32 = 0xE0u; // Bits 5-7 - font type

// Font type constants (bits 5-7 of style byte)
const FONT_TYPE_MSDF: u32 = 0u;     // Default text rendering
const FONT_TYPE_BITMAP: u32 = 1u;   // Bitmap fonts (emoji, color fonts)
const FONT_TYPE_SHADER: u32 = 2u;   // Single-cell shader glyphs
const FONT_TYPE_CARD: u32 = 3u;     // Multi-cell card glyphs

// Legacy alias for backward compatibility
const ATTR_EMOJI: u32 = 0x20u;      // Bit 5 - same position as FONT_TYPE_BITMAP

// Shader glyph constants (matches grid.h)
// Uses Unicode Plane 16 PUA-B: U+100000 - U+10FFFD (codepoint = glyph index)
const CARD_GLYPH_BASE: u32 = 0x100000u;     // 1048576 decimal - card range start
const CARD_GLYPH_END: u32 = 0x100FFFu;      // 1052671 decimal - card range end
const SHADER_GLYPH_BASE: u32 = 0x101000u;   // 1052672 decimal - shader glyph range start
const SHADER_GLYPH_END: u32 = 0x10FFFDu;    // 1114109 decimal - shader glyph range end
const SHADER_GLYPH_START: u32 = 0x100000u;  // Legacy alias

// Vertex input/output
struct VertexInput {
    @location(0) position: vec2<f32>,
};

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
};

@vertex
fn vs_main(input: VertexInput) -> VertexOutput {
    var output: VertexOutput;
    // Pass through clip-space coordinates directly
    output.position = vec4<f32>(input.position, 0.0, 1.0);
    return output;
}

// Note: median() function moved to lib/text.wgsl

// Helper function to check if pixel is in underline region
fn isInUnderline(localY: f32, cellHeight: f32, underlineType: u32, scale: f32) -> bool {
    // Underline position: slightly below baseline (baseline is at 80%)
    let baseline = cellHeight * 0.8;
    let underlineY = baseline + 2.0 * scale;
    let lineThickness = max(1.0, 1.5 * scale);

    if (underlineType == 1u) {
        // Single underline
        return localY >= underlineY && localY < underlineY + lineThickness;
    } else if (underlineType == 2u) {
        // Double underline
        let gap = lineThickness * 1.5;
        let inFirst = localY >= underlineY && localY < underlineY + lineThickness;
        let inSecond = localY >= underlineY + gap && localY < underlineY + gap + lineThickness;
        return inFirst || inSecond;
    } else if (underlineType == 3u) {
        // Curly/wavy underline (simplified sine wave)
        let waveHeight = lineThickness * 1.5;
        let waveFreq = 0.15;  // Wave frequency
        // We can't access localX here easily, so we'll handle curly differently
        // For now, treat curly as a thicker single underline (TODO: proper wavy)
        return localY >= underlineY && localY < underlineY + lineThickness * 2.0;
    }
    return false;
}

// Helper for curly underline with X position
fn isInCurlyUnderline(localX: f32, localY: f32, cellWidth: f32, cellHeight: f32, scale: f32) -> bool {
    let baseline = cellHeight * 0.8;
    let underlineY = baseline + 2.0 * scale;
    let lineThickness = max(1.0, 1.5 * scale);
    let waveHeight = lineThickness * 1.2;
    let waveFreq = 3.14159 * 2.0 / (cellWidth * 0.5);  // One full wave per half cell

    // Calculate wave offset
    let waveOffset = sin(localX * waveFreq) * waveHeight;
    let waveCenterY = underlineY + waveHeight;  // Center the wave

    return localY >= waveCenterY + waveOffset - lineThickness * 0.5 &&
           localY < waveCenterY + waveOffset + lineThickness * 0.5;
}

// Helper function to check if pixel is in strikethrough region
fn isInStrikethrough(localY: f32, cellHeight: f32, scale: f32) -> bool {
    // Strikethrough at middle of cell (approximately 45% from top for x-height)
    let strikeY = cellHeight * 0.45;
    let lineThickness = max(1.0, 1.5 * scale);
    return localY >= strikeY && localY < strikeY + lineThickness;
}

// Check if glyph index is a shader glyph (in Plane 16 PUA-B range)
fn isShaderGlyph(glyphIndex: u32) -> bool {
    return glyphIndex >= SHADER_GLYPH_START && glyphIndex <= SHADER_GLYPH_END;
}

// Check if glyph index is a card glyph
fn isCardGlyph(glyphIndex: u32) -> bool {
    return glyphIndex >= CARD_GLYPH_BASE && glyphIndex <= CARD_GLYPH_END;
}

// Check if glyph is any procedural glyph (card or shader)
fn isProceduralGlyph(glyphIndex: u32) -> bool {
    return glyphIndex >= CARD_GLYPH_BASE && glyphIndex <= SHADER_GLYPH_END;
}

// Extract font type from cell attrs (bits 5-7)
fn getFontType(attrs: u32) -> u32 {
    return (attrs >> 5u) & 0x7u;
}

// ==== SHADER GLYPH FUNCTIONS (injected by loader) ====
// SHADER_GLYPH_FUNCTIONS_PLACEHOLDER

// Helper to unpack u32 color to vec3<f32>
fn unpackColor(packed: u32) -> vec3<f32> {
    return vec3<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0
    );
}

// Helper to unpack u32 color to vec4<f32> (with alpha)
fn unpackColorAlpha(packed: u32) -> vec4<f32> {
    return vec4<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0,
        f32((packed >> 24u) & 0xFFu) / 255.0
    );
}

// Dispatch to shader glyph by codepoint
// Returns rendered color, or bgColor if glyph not found
// pixelPos: absolute pixel coordinates on screen (for tiled effects like fractals)
// mousePos: current mouse position in pixels
// fg/bg: raw packed u32 from cell - cards use as metadata index, regular glyphs unpack to color
fn renderShaderGlyph(glyphIndex: u32, localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // SHADER_GLYPH_DISPATCH_PLACEHOLDER
    // (loader generates: if glyphIndex == 1052672u { return shaderGlyph_1048577(...); } else if ...)
    return unpackColor(bg);  // Fallback if no shader glyph matches
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    // Convert fragment position to pixel coordinates
    let pixelPos = input.position.xy;

    // Calculate grid dimensions in pixels
    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    // Check if outside the grid area
    if (pixelPos.x >= gridPixelWidth || pixelPos.y >= gridPixelHeight) {
        return vec4<f32>(0.1, 0.1, 0.1, 1.0);  // Background color
    }

    // Calculate which cell we're in
    let cellCol = floor(pixelPos.x / grid.cellSize.x);
    let cellRow = floor(pixelPos.y / grid.cellSize.y);
    let cellCoord = vec2<i32>(i32(cellCol), i32(cellRow));

    // Position within the cell (pixels from top-left of cell)
    let localPxBase = vec2<f32>(
        pixelPos.x - cellCol * grid.cellSize.x,
        pixelPos.y - cellRow * grid.cellSize.y
    );

    // Get cell data from storage buffer (zero-copy from CPU)
    let cellIndex = u32(cellCoord.y) * u32(grid.gridSize.x) + u32(cellCoord.x);
    let cell = cellBuffer[cellIndex];

    // Unpack cell data
    var glyphIndex = cell.glyph;
    let fgColor = vec4<f32>(
        f32(cell.fg & 0xFFu) / 255.0,
        f32((cell.fg >> 8u) & 0xFFu) / 255.0,
        f32((cell.fg >> 16u) & 0xFFu) / 255.0,
        f32((cell.fg >> 24u) & 0xFFu) / 255.0
    );
    let bgColor = vec4<f32>(
        f32(cell.bg & 0xFFu) / 255.0,
        f32((cell.bg >> 8u) & 0xFFu) / 255.0,
        f32((cell.bg >> 16u) & 0xFFu) / 255.0,
        1.0  // bg alpha always 1.0
    );
    let cellAttrs = (cell.bg >> 24u) & 0xFFu;  // style is in high byte of bg

    // Extract attribute flags
    let underlineType = (cellAttrs & ATTR_UNDERLINE_MASK) >> 2u;
    let hasStrikethrough = (cellAttrs & ATTR_STRIKETHROUGH) != 0u;
    let fontType = getFontType(cellAttrs);
    // Backward compatible: if fontType is BITMAP or legacy emoji bit is set
    let isEmoji = fontType == FONT_TYPE_BITMAP;

    // Check if cursor should be rendered on this cell
    let cursorCol = i32(grid.cursorPos.x);
    let cursorRow = i32(grid.cursorPos.y);
    let isCursor = grid.cursorVisible > 0.5 &&
                   cellCoord.x == cursorCol &&
                   cellCoord.y == cursorRow;

    // Handle wide character continuation (0xFFFE) - look at previous cell
    var cellOffset = 0.0;
    if (glyphIndex == 0xFFFEu) {
        if (cellCoord.x > 0) {
            let prevCellIndex = u32(cellCoord.y) * u32(grid.gridSize.x) + u32(cellCoord.x - 1);
            glyphIndex = cellBuffer[prevCellIndex].glyph;
            cellOffset = grid.cellSize.x;  // Offset by one cell width to continue rendering
        } else {
            // Edge case: continuation at column 0 (shouldn't happen)
            var resultColor = bgColor.rgb;
            if (isCursor) {
                resultColor = vec3<f32>(1.0, 1.0, 1.0) - resultColor;
            }
            return vec4<f32>(resultColor, 1.0);
        }
    }

    // Current pixel position within cell (offset for wide char continuation)
    let localPx = localPxBase + vec2<f32>(cellOffset, 0.0);

    // Start with background color
    var finalColor = bgColor.rgb;
    var hasGlyph = false;

    // Check if we should render a glyph
    // Note: for emojis, index 0 is valid (first emoji in atlas), so don't skip it
    let skipGlyph = (!isEmoji && glyphIndex == 0u) || glyphIndex == 0xFFFFu ||
                    (glyphIndex >= 0xF000u && glyphIndex <= 0xFFFDu);

    if (!skipGlyph) {
        // Check for card/shader glyph first (Plane 16 PUA-B range)
        if (isCardGlyph(glyphIndex) || isShaderGlyph(glyphIndex)) {
            // Card or shader glyph: render via shader function
            let localUV = localPxBase / grid.cellSize;  // Normalize to 0-1
            let mousePos = vec2<f32>(globals.mouseX, globals.mouseY);
            finalColor = renderShaderGlyph(glyphIndex, localUV, globals.time, cell.fg, cell.bg, pixelPos, mousePos, globals.lastChar, globals.lastCharTime);
            hasGlyph = true;
        } else if (isEmoji) {
            // Emoji rendering: glyphIndex is the emoji index in emojiMetadata array
            let emoji = emojiMetadata[glyphIndex];

            // Emoji fills the cell, centered and scaled to fit
            // emoji.size contains the actual glyph size in atlas pixels
            let emojiAspect = emoji.size.x / emoji.size.y;
            let cellAspect = grid.cellSize.x / grid.cellSize.y;

            // Scale emoji to fit within cell while maintaining aspect ratio
            var scaledSize: vec2<f32>;
            if (emojiAspect > cellAspect) {
                // Emoji is wider - fit to width
                scaledSize = vec2<f32>(grid.cellSize.x, grid.cellSize.x / emojiAspect);
            } else {
                // Emoji is taller - fit to height
                scaledSize = vec2<f32>(grid.cellSize.y * emojiAspect, grid.cellSize.y);
            }

            // Center in cell
            let offset = (grid.cellSize - scaledSize) * 0.5;

            // Check if inside emoji bounds
            if (localPx.x >= offset.x && localPx.x < offset.x + scaledSize.x &&
                localPx.y >= offset.y && localPx.y < offset.y + scaledSize.y) {
                // Calculate UV for sampling
                let emojiLocalPos = (localPx - offset) / scaledSize;
                let uv = mix(emoji.uvMin, emoji.uvMax, emojiLocalPos);

                // Sample emoji texture (pre-rendered RGBA)
                let emojiColor = textureSampleLevel(emojiTexture, emojiSampler, uv, 0.0);

                // Alpha blend emoji over background
                finalColor = mix(bgColor.rgb, emojiColor.rgb, emojiColor.a);
                hasGlyph = emojiColor.a > 0.01;
            }
        } else {
            // MSDF text rendering
            let glyph = glyphMetadata[glyphIndex];

            // Calculate glyph position within cell
            let scaledGlyphSize = glyph.size * grid.scale;
            let scaledBearing = glyph.bearing * grid.scale;

            // Baseline at 80% of cell height
            let baseline = grid.cellSize.y * 0.8;
            let glyphTop = baseline - scaledBearing.y;
            let glyphLeft = scaledBearing.x;

            // Glyph bounds in cell pixel space
            let glyphMinPx = vec2<f32>(glyphLeft, glyphTop);
            let glyphMaxPx = vec2<f32>(glyphLeft + scaledGlyphSize.x, glyphTop + scaledGlyphSize.y);

            // Check if inside glyph bounds
            if (localPx.x >= glyphMinPx.x && localPx.x < glyphMaxPx.x &&
                localPx.y >= glyphMinPx.y && localPx.y < glyphMaxPx.y) {
                // Calculate UV for MSDF sampling
                let glyphLocalPos = (localPx - glyphMinPx) / scaledGlyphSize;
                let uv = mix(glyph.uvMin, glyph.uvMax, glyphLocalPos);

                // Sample MSDF texture
                let msdf = textureSampleLevel(fontTexture, fontSampler, uv, 0.0);

                // Calculate signed distance
                let sd = median(msdf.r, msdf.g, msdf.b);

                // Apply anti-aliased edge
                let screenPxRange = grid.pixelRange * grid.scale;
                let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

                // Blend foreground over background
                finalColor = mix(bgColor.rgb, fgColor.rgb, alpha);
                hasGlyph = alpha > 0.01;
            }
        }
    }

    // Apply text decorations (underline and strikethrough)
    let localY = localPxBase.y;  // Use base position, not offset
    let localX = localPxBase.x;

    // Check underline
    if (underlineType > 0u) {
        var inUnderline = false;
        if (underlineType == 3u) {
            // Curly underline needs X position
            inUnderline = isInCurlyUnderline(localX, localY, grid.cellSize.x, grid.cellSize.y, grid.scale);
        } else {
            inUnderline = isInUnderline(localY, grid.cellSize.y, underlineType, grid.scale);
        }
        if (inUnderline) {
            finalColor = fgColor.rgb;
        }
    }

    // Check strikethrough
    if (hasStrikethrough) {
        if (isInStrikethrough(localY, grid.cellSize.y, grid.scale)) {
            finalColor = fgColor.rgb;
        }
    }

    // Cursor rendering: shape + blink
    if (isCursor) {
        // Blink: skip drawing cursor during off-phase (1Hz, 50% duty)
        var cursorOn = true;
        if (grid.cursorBlink > 0.5) {
            cursorOn = fract(globals.time) < 0.5;
        }

        if (cursorOn) {
            let localUV = localPxBase / grid.cellSize;
            let shape = i32(grid.cursorShape);
            var drawCursor = false;

            if (shape == 2) {
                // Underline: bottom 15% of cell
                drawCursor = localUV.y > 0.85;
            } else if (shape == 3) {
                // Bar: left 10% of cell
                drawCursor = localUV.x < 0.1;
            } else {
                // Block (1 or default): full cell
                drawCursor = true;
            }

            if (drawCursor) {
                finalColor = vec3<f32>(1.0, 1.0, 1.0) - finalColor;
            }
        }
    }

    return vec4<f32>(finalColor, 1.0);
}
