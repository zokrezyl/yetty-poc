// Text/glyph rendering utilities
// Extracted from gpu-screen.wgsl for reuse in cards (ydraw, etc.)

// MSDF median function
fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

// Result of glyph rendering
struct GlyphRenderResult {
    color: vec3<f32>,   // Rendered color
    alpha: f32,         // Coverage (0 = no glyph, 1 = full coverage)
};

// Render a glyph at given local position
// localUV: normalized position within glyph bounds (0-1)
// glyphIndex: codepoint/index of glyph
// fgColor: foreground color for text
// bgColor: background color
// isEmoji: whether this is an emoji glyph
// scale: rendering scale factor
// pixelRange: MSDF pixel range for anti-aliasing
fn renderGlyphAtUV(
    localUV: vec2<f32>,
    glyphIndex: u32,
    fgColor: vec3<f32>,
    bgColor: vec3<f32>,
    isEmoji: bool,
    scale: f32,
    pixelRange: f32
) -> GlyphRenderResult {
    var result: GlyphRenderResult;
    result.color = bgColor;
    result.alpha = 0.0;

    // Check for card/shader glyph (Plane 16 PUA-B range)
    if (isCardGlyph(glyphIndex) || isShaderGlyph(glyphIndex)) {
        // Card/shader glyph - caller should handle this separately
        // as it needs pixelPos and mousePos
        result.color = bgColor;
        result.alpha = 0.0;
        return result;
    }

    if (isEmoji) {
        // Emoji rendering
        let emoji = emojiMetadata[glyphIndex];
        let uv = mix(emoji.uvMin, emoji.uvMax, localUV);
        let emojiColor = textureSampleLevel(emojiTexture, emojiSampler, uv, 0.0);
        result.color = mix(bgColor, emojiColor.rgb, emojiColor.a);
        result.alpha = emojiColor.a;
    } else {
        // MSDF text rendering
        let glyph = glyphMetadata[glyphIndex];
        let uv = mix(glyph.uvMin, glyph.uvMax, localUV);

        // Sample MSDF texture
        let msdf = textureSampleLevel(fontTexture, fontSampler, uv, 0.0);

        // Calculate signed distance
        let sd = median(msdf.r, msdf.g, msdf.b);

        // Apply anti-aliased edge
        let screenPxRange = pixelRange * scale;
        let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

        result.color = mix(bgColor, fgColor, alpha);
        result.alpha = alpha;
    }

    return result;
}

// Render glyph with full cell-based layout (as used in gpu-screen)
// localPx: pixel position within cell
// cellSize: cell dimensions in pixels
// glyphIndex: codepoint/index
// fgColor, bgColor: colors
// isEmoji: emoji flag
// scale: rendering scale
// pixelRange: MSDF pixel range
fn renderGlyphInCell(
    localPx: vec2<f32>,
    cellSize: vec2<f32>,
    glyphIndex: u32,
    fgColor: vec3<f32>,
    bgColor: vec3<f32>,
    isEmoji: bool,
    scale: f32,
    pixelRange: f32
) -> GlyphRenderResult {
    var result: GlyphRenderResult;
    result.color = bgColor;
    result.alpha = 0.0;

    // Check for card/shader glyph
    if (isCardGlyph(glyphIndex) || isShaderGlyph(glyphIndex)) {
        // Caller handles shader glyphs
        return result;
    }

    if (isEmoji) {
        // Emoji rendering - fill cell with aspect ratio preserved
        let emoji = emojiMetadata[glyphIndex];
        let emojiAspect = emoji.size.x / emoji.size.y;
        let cellAspect = cellSize.x / cellSize.y;

        var scaledSize: vec2<f32>;
        if (emojiAspect > cellAspect) {
            scaledSize = vec2<f32>(cellSize.x, cellSize.x / emojiAspect);
        } else {
            scaledSize = vec2<f32>(cellSize.y * emojiAspect, cellSize.y);
        }

        let offset = (cellSize - scaledSize) * 0.5;

        if (localPx.x >= offset.x && localPx.x < offset.x + scaledSize.x &&
            localPx.y >= offset.y && localPx.y < offset.y + scaledSize.y) {
            let emojiLocalPos = (localPx - offset) / scaledSize;
            let uv = mix(emoji.uvMin, emoji.uvMax, emojiLocalPos);
            let emojiColor = textureSampleLevel(emojiTexture, emojiSampler, uv, 0.0);
            result.color = mix(bgColor, emojiColor.rgb, emojiColor.a);
            result.alpha = emojiColor.a;
        }
    } else {
        // MSDF text rendering with proper baseline
        let glyph = glyphMetadata[glyphIndex];
        let scaledGlyphSize = glyph.size * scale;
        let scaledBearing = glyph.bearing * scale;

        // Baseline at 80% of cell height
        let baseline = cellSize.y * 0.8;
        let glyphTop = baseline - scaledBearing.y;
        let glyphLeft = scaledBearing.x;

        let glyphMinPx = vec2<f32>(glyphLeft, glyphTop);
        let glyphMaxPx = vec2<f32>(glyphLeft + scaledGlyphSize.x, glyphTop + scaledGlyphSize.y);

        if (localPx.x >= glyphMinPx.x && localPx.x < glyphMaxPx.x &&
            localPx.y >= glyphMinPx.y && localPx.y < glyphMaxPx.y) {
            let glyphLocalPos = (localPx - glyphMinPx) / scaledGlyphSize;
            let uv = mix(glyph.uvMin, glyph.uvMax, glyphLocalPos);

            let msdf = textureSampleLevel(fontTexture, fontSampler, uv, 0.0);
            let sd = median(msdf.r, msdf.g, msdf.b);
            let screenPxRange = pixelRange * scale;
            let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

            result.color = mix(bgColor, fgColor, alpha);
            result.alpha = alpha;
        }
    }

    return result;
}

// Simple glyph rendering for ydraw - glyph fills its entire bounds
// localUV: normalized position within glyph bounds (0-1)
// glyphIndex: codepoint/index (assumes valid MSDF glyph, not emoji/shader)
// fgColor: text color
// bgColor: background color
// pixelRange: MSDF pixel range (typically 4.0)
// screenScale: pixels per scene unit (for proper anti-aliasing)
fn renderMsdfGlyph(
    localUV: vec2<f32>,
    glyphIndex: u32,
    fgColor: vec3<f32>,
    bgColor: vec3<f32>,
    pixelRange: f32,
    screenScale: f32
) -> GlyphRenderResult {
    var result: GlyphRenderResult;
    result.color = bgColor;
    result.alpha = 0.0;

    let glyph = glyphMetadata[glyphIndex];
    let uv = mix(glyph.uvMin, glyph.uvMax, localUV);

    let msdf = textureSampleLevel(fontTexture, fontSampler, uv, 0.0);
    let sd = median(msdf.r, msdf.g, msdf.b);

    // screenScale helps compute proper anti-aliasing width
    let screenPxRange = pixelRange * screenScale;
    let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

    result.color = mix(bgColor, fgColor, alpha);
    result.alpha = alpha;

    return result;
}
