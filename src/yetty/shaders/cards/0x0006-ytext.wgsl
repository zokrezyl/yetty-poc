// Shader glyph: YText widget (codepoint 1048582 / U+100006)
// GPU-animated scrolling text with time-based animation and 3D effects
//
// Architecture:
//   - Text organized into lines (binary search for Y)
//   - Each line contains glyphs (linear search for X)
//   - Scroll computed from: time * scrollSpeed
//   - 3D effects: cylinder, sphere, wave with proper projection math
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   offset 0:  lineOffset (u32)     - offset to line data in cardStorage
//   offset 1:  lineCount (u32)      - number of lines
//   offset 2:  glyphOffset (u32)    - offset to glyph data in cardStorage
//   offset 3:  glyphCount (u32)     - total glyphs
//   offset 4:  contentWidth (f32)   - total content width
//   offset 5:  contentHeight (f32)  - total content height
//   offset 6:  scrollSpeedX (f32)   - horizontal scroll (pixels/sec)
//   offset 7:  scrollSpeedY (f32)   - vertical scroll (pixels/sec)
//   offset 8:  scrollMode (u32)     - 0=clamp, 1=loop, 2=pingpong
//   offset 9:  widthCells (u32)
//   offset 10: heightCells (u32)
//   offset 11: bgColor (u32)
//   offset 12: effectMode (u32)     - 0=none, 1=cylinderH, 2=cylinderV, 3=sphere, 4=waveH, 5=waveV
//   offset 13: effectStrength (f32) - effect intensity
//   offset 14: tiltX (f32)          - tilt/frequency parameter
//   offset 15: tiltY (f32)          - tilt/frequency parameter
//
// =============================================================================

const YTEXT_SCROLL_CLAMP: u32 = 0u;
const YTEXT_SCROLL_LOOP: u32 = 1u;
const YTEXT_SCROLL_PINGPONG: u32 = 2u;

const YTEXT_EFFECT_NONE: u32 = 0u;
const YTEXT_EFFECT_CYLINDER_H: u32 = 1u;
const YTEXT_EFFECT_CYLINDER_V: u32 = 2u;
const YTEXT_EFFECT_SPHERE: u32 = 3u;
const YTEXT_EFFECT_WAVE_H: u32 = 4u;
const YTEXT_EFFECT_WAVE_V: u32 = 5u;

const PI: f32 = 3.14159265359;

// =============================================================================
// Binary search to find line at Y position
// =============================================================================
fn ytextFindLine(lineOffset: u32, lineCount: u32, y: f32) -> i32 {
    if (lineCount == 0u) {
        return -1;
    }

    var lo = 0u;
    var hi = lineCount;

    while (lo < hi) {
        let mid = (lo + hi) / 2u;
        let lineOff = lineOffset + mid * 4u;
        let lineY = cardStorage[lineOff + 2u];
        let lineH = cardStorage[lineOff + 3u];

        if (y < lineY) {
            hi = mid;
        } else if (y >= lineY + lineH) {
            lo = mid + 1u;
        } else {
            return i32(mid);
        }
    }
    return -1;
}

// =============================================================================
// 3D Effect Result
// =============================================================================
struct Effect3DResult {
    uv: vec2<f32>,
    lighting: f32,
    visible: bool,
}

// =============================================================================
// Horizontal Cylinder Effect (drum rotating vertically)
// Text wraps around a horizontal cylinder like a rotary drum
// =============================================================================
fn applyCylinderH(uv: vec2<f32>, strength: f32) -> Effect3DResult {
    var result: Effect3DResult;
    result.visible = true;

    // Map screen Y to angle on cylinder
    // Screen Y in [0,1] maps to angle in [-PI/2 * strength, PI/2 * strength]
    let centered_y = uv.y - 0.5;  // [-0.5, 0.5]

    // The visible arc of the cylinder
    let maxAngle = PI * 0.5 * strength;
    let angle = centered_y * 2.0 * maxAngle;

    // Visibility check - beyond visible arc
    if (abs(angle) > PI * 0.5) {
        result.visible = false;
        result.uv = uv;
        result.lighting = 0.0;
        return result;
    }

    let cosAngle = cos(angle);
    let sinAngle = sin(angle);

    // For proper cylinder projection:
    // - X stays the same (cylinder axis is horizontal)
    // - Y maps through the cylinder arc
    // The texture Y should map linearly to the angle
    result.uv.x = uv.x;
    result.uv.y = (angle / maxAngle) * 0.5 + 0.5;  // Back to [0,1]

    // Lighting: Lambert shading - surface normal faces viewer at center
    result.lighting = mix(1.0, cosAngle, strength * 0.7);

    return result;
}

// =============================================================================
// Vertical Cylinder Effect (drum rotating horizontally)
// =============================================================================
fn applyCylinderV(uv: vec2<f32>, strength: f32) -> Effect3DResult {
    var result: Effect3DResult;
    result.visible = true;

    let centered_x = uv.x - 0.5;
    let maxAngle = PI * 0.5 * strength;
    let angle = centered_x * 2.0 * maxAngle;

    if (abs(angle) > PI * 0.5) {
        result.visible = false;
        result.uv = uv;
        result.lighting = 0.0;
        return result;
    }

    let cosAngle = cos(angle);

    result.uv.x = (angle / maxAngle) * 0.5 + 0.5;
    result.uv.y = uv.y;
    result.lighting = mix(1.0, cosAngle, strength * 0.7);

    return result;
}

// =============================================================================
// Sphere Effect
// =============================================================================
fn applySphere(uv: vec2<f32>, strength: f32) -> Effect3DResult {
    var result: Effect3DResult;
    result.visible = true;

    let centered = uv - vec2<f32>(0.5, 0.5);
    let maxAngle = PI * 0.5 * strength;

    // Map to spherical angles
    let angleX = centered.x * 2.0 * maxAngle;
    let angleY = centered.y * 2.0 * maxAngle;

    // Check if we're on the visible hemisphere
    let r2 = (centered.x * centered.x + centered.y * centered.y) * 4.0;
    if (r2 > 1.0 / (strength * strength + 0.01)) {
        result.visible = false;
        result.uv = uv;
        result.lighting = 0.0;
        return result;
    }

    let cosX = cos(angleX);
    let cosY = cos(angleY);

    result.uv.x = (angleX / maxAngle) * 0.5 + 0.5;
    result.uv.y = (angleY / maxAngle) * 0.5 + 0.5;

    // Spherical lighting
    let normalZ = cosX * cosY;
    result.lighting = mix(1.0, max(normalZ, 0.2), strength * 0.8);

    return result;
}

// =============================================================================
// Horizontal Wave Effect (ripples propagating along X)
// =============================================================================
fn applyWaveH(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> Effect3DResult {
    var result: Effect3DResult;
    result.visible = true;

    // Wave parameters
    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.15;

    // Sinusoidal displacement in Y based on X position and time
    let phase = uv.x * freq * PI * 2.0 - time * 2.0;
    let displacement = sin(phase) * amplitude;

    result.uv.x = uv.x;
    result.uv.y = uv.y + displacement;

    // Clamp to valid range
    if (result.uv.y < 0.0 || result.uv.y > 1.0) {
        result.visible = false;
        result.lighting = 1.0;
        return result;
    }

    // Lighting based on wave slope (normal)
    let slope = cos(phase) * amplitude * freq * PI * 2.0;
    let normalZ = 1.0 / sqrt(1.0 + slope * slope);
    result.lighting = mix(1.0, normalZ, strength * 0.5);

    return result;
}

// =============================================================================
// Vertical Wave Effect (ripples propagating along Y)
// =============================================================================
fn applyWaveV(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> Effect3DResult {
    var result: Effect3DResult;
    result.visible = true;

    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.15;

    let phase = uv.y * freq * PI * 2.0 - time * 2.0;
    let displacement = sin(phase) * amplitude;

    result.uv.x = uv.x + displacement;
    result.uv.y = uv.y;

    if (result.uv.x < 0.0 || result.uv.x > 1.0) {
        result.visible = false;
        result.lighting = 1.0;
        return result;
    }

    let slope = cos(phase) * amplitude * freq * PI * 2.0;
    let normalZ = 1.0 / sqrt(1.0 + slope * slope);
    result.lighting = mix(1.0, normalZ, strength * 0.5);

    return result;
}

// =============================================================================
// Apply 3D Effect dispatcher
// =============================================================================
fn applyEffect3D(
    uv: vec2<f32>,
    effectMode: u32,
    effectStrength: f32,
    param1: f32,
    param2: f32,
    time: f32
) -> Effect3DResult {
    var result: Effect3DResult;
    result.uv = uv;
    result.lighting = 1.0;
    result.visible = true;

    let strength = clamp(effectStrength, 0.0, 1.0);

    if (effectMode == YTEXT_EFFECT_NONE) {
        return result;
    } else if (effectMode == YTEXT_EFFECT_CYLINDER_H) {
        return applyCylinderH(uv, strength);
    } else if (effectMode == YTEXT_EFFECT_CYLINDER_V) {
        return applyCylinderV(uv, strength);
    } else if (effectMode == YTEXT_EFFECT_SPHERE) {
        return applySphere(uv, strength);
    } else if (effectMode == YTEXT_EFFECT_WAVE_H) {
        return applyWaveH(uv, strength, time, param1);
    } else if (effectMode == YTEXT_EFFECT_WAVE_V) {
        return applyWaveV(uv, strength, time, param1);
    }

    return result;
}

// =============================================================================
// Main shader function
// =============================================================================
fn shaderGlyph_1048582(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // Decode cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read metadata
    let lineOffset = cardMetadata[metaOffset + 0u];
    let lineCount = cardMetadata[metaOffset + 1u];
    let glyphOffset = cardMetadata[metaOffset + 2u];
    let glyphCount = cardMetadata[metaOffset + 3u];
    let contentWidth = bitcast<f32>(cardMetadata[metaOffset + 4u]);
    let contentHeight = bitcast<f32>(cardMetadata[metaOffset + 5u]);
    let scrollSpeedX = bitcast<f32>(cardMetadata[metaOffset + 6u]);
    let scrollSpeedY = bitcast<f32>(cardMetadata[metaOffset + 7u]);
    let scrollMode = cardMetadata[metaOffset + 8u];
    let widthCells = cardMetadata[metaOffset + 9u];
    let heightCells = cardMetadata[metaOffset + 10u];
    let bgColorPacked = cardMetadata[metaOffset + 11u];
    let effectMode = cardMetadata[metaOffset + 12u];
    let effectStrength = bitcast<f32>(cardMetadata[metaOffset + 13u]);
    let param1 = bitcast<f32>(cardMetadata[metaOffset + 14u]);
    let param2 = bitcast<f32>(cardMetadata[metaOffset + 15u]);

    let bgColor = unpackColor(bgColorPacked);

    if (lineCount == 0u || glyphCount == 0u) {
        return bgColor;
    }

    let viewWidthPx = f32(widthCells) * grid.cellSize.x;
    let viewHeightPx = f32(heightCells) * grid.cellSize.y;

    // Compute normalized UV within card (0-1)
    let cardUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                 vec2<f32>(f32(widthCells), f32(heightCells));

    // Apply 3D effect
    let effect = applyEffect3D(cardUV, effectMode, effectStrength, param1, param2, time);

    if (!effect.visible) {
        return bgColor;
    }

    // Convert distorted UV to pixel position
    let cardPixel = effect.uv * vec2<f32>(viewWidthPx, viewHeightPx);

    // Compute scroll
    var scrollX = time * scrollSpeedX;
    var scrollY = time * scrollSpeedY;

    if (scrollMode == YTEXT_SCROLL_PINGPONG) {
        if (contentWidth > viewWidthPx) {
            let range = contentWidth - viewWidthPx;
            let cycle = abs(scrollX) % (range * 2.0);
            scrollX = select(cycle, range * 2.0 - cycle, cycle > range);
        } else {
            scrollX = 0.0;
        }
        if (contentHeight > viewHeightPx) {
            let range = contentHeight - viewHeightPx;
            let cycle = abs(scrollY) % (range * 2.0);
            scrollY = select(cycle, range * 2.0 - cycle, cycle > range);
        } else {
            scrollY = 0.0;
        }
    } else if (scrollMode != YTEXT_SCROLL_LOOP) {
        if (contentWidth > viewWidthPx) {
            scrollX = clamp(scrollX, 0.0, contentWidth - viewWidthPx);
        } else {
            scrollX = 0.0;
        }
        if (contentHeight > viewHeightPx) {
            scrollY = clamp(scrollY, 0.0, contentHeight - viewHeightPx);
        } else {
            scrollY = 0.0;
        }
    }

    var contentPos = cardPixel + vec2<f32>(scrollX, scrollY);

    // Seamless loop wrapping
    if (scrollMode == YTEXT_SCROLL_LOOP) {
        if (contentHeight > 0.0) {
            contentPos.y = contentPos.y % contentHeight;
            if (contentPos.y < 0.0) { contentPos.y += contentHeight; }
        }
        if (contentWidth > 0.0) {
            contentPos.x = contentPos.x % contentWidth;
            if (contentPos.x < 0.0) { contentPos.x += contentWidth; }
        }
    }

    // Find line
    let lineIdx = ytextFindLine(lineOffset, lineCount, contentPos.y);
    if (lineIdx < 0) {
        return bgColor;
    }

    let lineOff = lineOffset + u32(lineIdx) * 4u;
    let lineStartGlyph = bitcast<u32>(cardStorage[lineOff + 0u]);
    let lineGlyphCount = bitcast<u32>(cardStorage[lineOff + 1u]);
    let lineY = cardStorage[lineOff + 2u];
    let lineHeight = cardStorage[lineOff + 3u];

    // Find glyph
    for (var i = 0u; i < lineGlyphCount; i++) {
        let gOff = glyphOffset + (lineStartGlyph + i) * 4u;

        let gXOffset = cardStorage[gOff + 0u];
        let gWidth = cardStorage[gOff + 1u];
        let gIdx = bitcast<u32>(cardStorage[gOff + 2u]);
        let gColorAndScale = bitcast<u32>(cardStorage[gOff + 3u]);

        if (contentPos.x >= gXOffset && contentPos.x < gXOffset + gWidth) {
            let gColorPacked = gColorAndScale & 0xFFFFFF00u;
            let scaleIndex = gColorAndScale & 0xFFu;
            let scale = f32(scaleIndex) / 16.0;

            var gColor = unpackColor(gColorPacked | 0xFFu);
            gColor = gColor * effect.lighting;

            let glyph = glyphMetadata[gIdx];
            let baseline = lineY + lineHeight * 0.75;
            let glyphTop = baseline - glyph.bearing.y * scale;
            let glyphHeight = glyph.size.y * scale;

            if (contentPos.y < glyphTop || contentPos.y >= glyphTop + glyphHeight) {
                return bgColor;
            }

            let glyphLocalX = (contentPos.x - gXOffset) / gWidth;
            let glyphLocalY = (contentPos.y - glyphTop) / glyphHeight;
            let glyphUV = vec2<f32>(glyphLocalX, glyphLocalY);

            let screenScale = viewWidthPx / max(contentWidth, 1.0) * scale;
            let result = renderMsdfGlyph(glyphUV, gIdx, gColor, bgColor, grid.pixelRange, screenScale);

            if (result.alpha > 0.01) {
                return result.color;
            }
            return bgColor;
        }
    }

    return bgColor;
}
