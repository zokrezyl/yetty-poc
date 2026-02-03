// Shader glyph: YText widget (codepoint 1048582 / U+100006)
// GPU-animated scrolling text with projection effects
//
// Architecture:
//   1. SCROLL: moves viewport over content (seamless loop, pingpong, clamp)
//   2. PROJECTION: distorts how viewport appears (flat, cylinder, sphere, wave)
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s) - PACKED FORMAT:
//   Word 0:  lineOffset(16) | lineCount(16)
//   Word 1:  glyphOffset(16) | glyphCount(16)
//   Word 2:  contentWidth (float)
//   Word 3:  contentHeight (float)
//   Word 4:  scrollSpeedX (float)
//   Word 5:  scrollSpeedY (float)
//   Word 6:  widthCells(12) | heightCells(12) | scrollMode(2) | effectMode(4) | reserved(2)
//   Word 7:  effectStrength (float)
//   Word 8:  frequency (float)
//   Word 9:  bgColor (RGBA, 0=use default)
//   Word 10: fgColor (RGBA, 0=use default)
//   Word 11: tiltX (float)
//   Word 12: tiltY (float)
//   Word 13-15: reserved
// =============================================================================

const YTEXT_SCROLL_CLAMP: u32 = 0u;
const YTEXT_SCROLL_LOOP: u32 = 1u;
const YTEXT_SCROLL_PINGPONG: u32 = 2u;

// Projection modes
const YTEXT_PROJ_FLAT: u32 = 0u;        // Default: no distortion
const YTEXT_PROJ_CYLINDER_H: u32 = 1u;  // Horizontal cylinder (vertical scroll)
const YTEXT_PROJ_CYLINDER_V: u32 = 2u;  // Vertical cylinder (horizontal scroll)
const YTEXT_PROJ_SPHERE: u32 = 3u;      // Sphere
const YTEXT_PROJ_WAVE_DISP_H: u32 = 4u; // Horizontal wave displacement
const YTEXT_PROJ_WAVE_DISP_V: u32 = 5u; // Vertical wave displacement
const YTEXT_PROJ_WAVE_PROJ_H: u32 = 6u; // Horizontal wave projection (foreshortening)
const YTEXT_PROJ_WAVE_PROJ_V: u32 = 7u; // Vertical wave projection (foreshortening)
const YTEXT_PROJ_RIPPLE: u32 = 8u;      // Concentric water ripple
const YTEXT_PROJ_PERSPECTIVE: u32 = 9u; // Star Wars crawl perspective

const PI: f32 = 3.14159265359;

// =============================================================================
// Binary search for line
// =============================================================================
fn ytextFindLine(lineOffset: u32, lineCount: u32, y: f32) -> i32 {
    if (lineCount == 0u) { return -1; }
    var lo = 0u;
    var hi = lineCount;
    while (lo < hi) {
        let mid = (lo + hi) / 2u;
        let lineOff = lineOffset + mid * 4u;
        let lineY = cardStorage[lineOff + 2u];
        let lineH = cardStorage[lineOff + 3u];
        if (y < lineY) { hi = mid; }
        else if (y >= lineY + lineH) { lo = mid + 1u; }
        else { return i32(mid); }
    }
    return -1;
}

// =============================================================================
// Projection result
// =============================================================================
struct ProjectionResult {
    uv: vec2<f32>,      // Distorted UV for content lookup
    lighting: f32,      // Brightness multiplier
    visible: bool,      // Whether this point is visible
}

// =============================================================================
// PROJECTION: Flat surface (default - no distortion)
// =============================================================================
fn projectFlat(uv: vec2<f32>) -> ProjectionResult {
    var r: ProjectionResult;
    r.uv = uv;
    r.lighting = 1.0;
    r.visible = true;
    return r;
}

// =============================================================================
// PROJECTION: Horizontal Cylinder (drum with horizontal axis)
// For vertical scrolling - top/bottom edges curve away
// Creates foreshortening: edges show more content (compressed)
// =============================================================================
fn projectCylinderH(uv: vec2<f32>, strength: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    // Map screen Y from [0,1] to [-1,1]
    let screenY = (uv.y - 0.5) * 2.0;

    // The visible arc of cylinder: strength=1 means full hemisphere (90°)
    let maxAngle = PI * 0.5 * strength;

    // Screen Y = sin(angle) for orthographic projection of cylinder
    // So angle = asin(screenY * sin(maxAngle))
    let sinInput = screenY * sin(maxAngle);

    if (abs(sinInput) > 0.99) {
        r.visible = false;
        r.uv = uv;
        r.lighting = 0.2;
        return r;
    }

    let angle = asin(sinInput);

    // Texture Y = angle normalized to [0,1]
    // This creates foreshortening: d(textureY)/d(screenY) = 1/cos(angle)
    // At edges (angle near ±90°), cos→0, so derivative→∞ = compression
    let textureY = (angle / maxAngle + 1.0) * 0.5;

    r.uv.x = uv.x;
    r.uv.y = textureY;

    // Lighting: Lambert shading, brightest at center
    r.lighting = mix(1.0, cos(angle), strength * 0.6);

    return r;
}

// =============================================================================
// PROJECTION: Vertical Cylinder (drum with vertical axis)
// For horizontal scrolling - left/right edges curve away
// =============================================================================
fn projectCylinderV(uv: vec2<f32>, strength: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let screenX = (uv.x - 0.5) * 2.0;
    let maxAngle = PI * 0.5 * strength;
    let sinInput = screenX * sin(maxAngle);

    if (abs(sinInput) > 0.99) {
        r.visible = false;
        r.uv = uv;
        r.lighting = 0.2;
        return r;
    }

    let angle = asin(sinInput);
    let textureX = (angle / maxAngle + 1.0) * 0.5;

    r.uv.x = textureX;
    r.uv.y = uv.y;
    r.lighting = mix(1.0, cos(angle), strength * 0.6);

    return r;
}

// =============================================================================
// PROJECTION: Sphere
// Both X and Y curve away at edges - like text on a globe
// =============================================================================
fn projectSphere(uv: vec2<f32>, strength: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let screen = (uv - 0.5) * 2.0;  // [-1, 1]
    let maxAngle = PI * 0.5 * strength;
    let sinMax = sin(maxAngle);

    // Check if on visible hemisphere
    let r2 = dot(screen, screen);
    if (r2 > 1.0) {
        r.visible = false;
        r.uv = uv;
        r.lighting = 0.2;
        return r;
    }

    // Apply spherical projection to both axes
    let sinX = screen.x * sinMax;
    let sinY = screen.y * sinMax;

    if (abs(sinX) > 0.99 || abs(sinY) > 0.99) {
        r.visible = false;
        r.uv = uv;
        r.lighting = 0.2;
        return r;
    }

    let angleX = asin(sinX);
    let angleY = asin(sinY);

    r.uv.x = (angleX / maxAngle + 1.0) * 0.5;
    r.uv.y = (angleY / maxAngle + 1.0) * 0.5;

    // Spherical lighting
    let cosX = cos(angleX);
    let cosY = cos(angleY);
    r.lighting = mix(1.0, cosX * cosY, strength * 0.7);

    return r;
}

// =============================================================================
// DISPLACEMENT: Horizontal Wave
// Ripples propagate along X, displace Y (no foreshortening)
// =============================================================================
fn projectWaveDispH(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.08;

    // Animated wave
    let phase = uv.x * freq * PI * 2.0 + time * 4.0;
    let wave = sin(phase);

    r.uv.x = uv.x;
    r.uv.y = uv.y + wave * amplitude;

    // Clamp to valid range (content will handle wrapping)
    r.uv.y = clamp(r.uv.y, 0.0, 1.0);

    // Lighting based on wave normal
    let slope = cos(phase) * amplitude * freq * PI * 2.0;
    r.lighting = 1.0 - abs(slope) * 0.4;

    return r;
}

// =============================================================================
// DISPLACEMENT: Vertical Wave
// Ripples propagate along Y, displace X (no foreshortening)
// =============================================================================
fn projectWaveDispV(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.08;

    let phase = uv.y * freq * PI * 2.0 + time * 4.0;
    let wave = sin(phase);

    r.uv.x = uv.x + wave * amplitude;
    r.uv.y = uv.y;

    r.uv.x = clamp(r.uv.x, 0.0, 1.0);

    let slope = cos(phase) * amplitude * freq * PI * 2.0;
    r.lighting = 1.0 - abs(slope) * 0.4;

    return r;
}

// =============================================================================
// PSEUDO-PROJECTION: Horizontal Wave with Foreshortening
// Text appears painted on a wavy surface - compressed on slopes
// =============================================================================
fn projectWaveProjH(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.15;

    // Wave phase at this X position
    let phase = uv.x * freq * PI * 2.0 + time * 3.0;

    // Wave slope (derivative): dz/dx = A * freq * 2π * cos(phase)
    let slope = amplitude * freq * PI * 2.0 * cos(phase);

    // Foreshortening factor: arc length derivative = sqrt(1 + slope²)
    // Text compresses where slope is steep
    let foreshorten = sqrt(1.0 + slope * slope);

    // Integrate foreshortening to get texture coordinate
    // Approximation: stretch UV.x based on local foreshortening
    // Center the effect around 0.5
    let centerX = uv.x - 0.5;
    let stretchedX = centerX * foreshorten;
    r.uv.x = clamp(stretchedX + 0.5, 0.0, 1.0);
    r.uv.y = uv.y;

    // Lighting based on surface normal facing viewer
    // Normal faces viewer when slope is 0, tilts away as slope increases
    r.lighting = 1.0 / foreshorten;

    return r;
}

// =============================================================================
// PSEUDO-PROJECTION: Vertical Wave with Foreshortening
// Text appears painted on a wavy surface - compressed on slopes
// =============================================================================
fn projectWaveProjV(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let freq = select(3.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.15;

    let phase = uv.y * freq * PI * 2.0 + time * 3.0;
    let slope = amplitude * freq * PI * 2.0 * cos(phase);
    let foreshorten = sqrt(1.0 + slope * slope);

    r.uv.x = uv.x;
    let centerY = uv.y - 0.5;
    let stretchedY = centerY * foreshorten;
    r.uv.y = clamp(stretchedY + 0.5, 0.0, 1.0);

    r.lighting = 1.0 / foreshorten;

    return r;
}

// =============================================================================
// PSEUDO-PROJECTION: Concentric Ripple (Water Drop)
// Radial waves emanating from center with foreshortening
// =============================================================================
fn projectRipple(uv: vec2<f32>, strength: f32, time: f32, frequency: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    let freq = select(4.0, frequency, frequency > 0.0);
    let amplitude = strength * 0.12;

    // Distance from center
    let center = vec2<f32>(0.5, 0.5);
    let delta = uv - center;
    let dist = length(delta);

    // Avoid division by zero at center
    if (dist < 0.001) {
        r.uv = uv;
        r.lighting = 1.0;
        return r;
    }

    // Radial direction (normalized)
    let radial = delta / dist;

    // Ripple phase: waves move outward from center
    let phase = dist * freq * PI * 2.0 - time * 5.0;

    // Radial slope (derivative of wave height w.r.t. distance)
    let slope = amplitude * freq * PI * 2.0 * cos(phase);

    // Foreshortening in radial direction
    let foreshorten = sqrt(1.0 + slope * slope);

    // Stretch UV radially based on foreshortening
    let stretchedDist = dist * foreshorten;
    let stretchedDelta = radial * stretchedDist;

    r.uv = clamp(center + stretchedDelta, vec2<f32>(0.0), vec2<f32>(1.0));

    // Lighting - darker on slopes
    r.lighting = 1.0 / foreshorten;

    return r;
}

// =============================================================================
// PROJECTION: Star Wars Perspective Crawl
// Text recedes into the distance - larger at bottom, smaller at top
// Classic opening crawl effect with vanishing point
// =============================================================================
fn projectPerspective(uv: vec2<f32>, strength: f32, tiltX: f32, tiltY: f32) -> ProjectionResult {
    var r: ProjectionResult;
    r.visible = true;

    // Strength controls how extreme the perspective is
    // tiltX controls horizontal vanishing point offset (-1 to 1)
    // tiltY controls the perspective angle/depth

    // Perspective factor: 1.0 at bottom (y=1), smaller at top (y=0)
    // This creates the "receding into distance" effect
    let depth = mix(0.3, 1.0, 1.0 - uv.y * strength);

    // Vanishing point (default: top center)
    let vanishX = 0.5 + tiltX * 0.3;

    // Apply perspective transformation
    // X coordinate converges toward vanishing point as we go up
    let perspectiveX = vanishX + (uv.x - vanishX) * depth;

    // Y coordinate: map through perspective (compress more at top)
    // Use a non-linear mapping for more realistic perspective
    let perspectiveY = 1.0 - pow(1.0 - uv.y, mix(1.0, 2.0, strength));

    r.uv.x = clamp(perspectiveX, 0.0, 1.0);
    r.uv.y = clamp(perspectiveY, 0.0, 1.0);

    // Lighting: darker at the top (farther away), brighter at bottom
    r.lighting = mix(0.4, 1.0, depth);

    return r;
}

// =============================================================================
// Projection dispatcher
// =============================================================================
fn applyProjection(uv: vec2<f32>, mode: u32, strength: f32, param1: f32, time: f32, tiltX: f32, tiltY: f32) -> ProjectionResult {
    let s = clamp(strength, 0.0, 1.0);

    switch mode {
        case YTEXT_PROJ_CYLINDER_H: { return projectCylinderH(uv, s); }
        case YTEXT_PROJ_CYLINDER_V: { return projectCylinderV(uv, s); }
        case YTEXT_PROJ_SPHERE: { return projectSphere(uv, s); }
        case YTEXT_PROJ_WAVE_DISP_H: { return projectWaveDispH(uv, s, time, param1); }
        case YTEXT_PROJ_WAVE_DISP_V: { return projectWaveDispV(uv, s, time, param1); }
        case YTEXT_PROJ_WAVE_PROJ_H: { return projectWaveProjH(uv, s, time, param1); }
        case YTEXT_PROJ_WAVE_PROJ_V: { return projectWaveProjV(uv, s, time, param1); }
        case YTEXT_PROJ_RIPPLE: { return projectRipple(uv, s, time, param1); }
        case YTEXT_PROJ_PERSPECTIVE: { return projectPerspective(uv, s, tiltX, tiltY); }
        default: { return projectFlat(uv); }
    }
}

// =============================================================================
// Main shader
// =============================================================================
fn shaderGlyph_1048582(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Read packed metadata
    // Word 0: lineOffset(16) | lineCount(16)
    let word0 = cardMetadata[metaOffset + 0u];
    let lineOffset = word0 & 0xFFFFu;
    let lineCount = (word0 >> 16u) & 0xFFFFu;

    // Word 1: glyphOffset(16) | glyphCount(16)
    let word1 = cardMetadata[metaOffset + 1u];
    let glyphOffset = word1 & 0xFFFFu;
    let glyphCount = (word1 >> 16u) & 0xFFFFu;

    // Words 2-5: floats
    let contentWidth = bitcast<f32>(cardMetadata[metaOffset + 2u]);
    let contentHeight = bitcast<f32>(cardMetadata[metaOffset + 3u]);
    let scrollSpeedX = bitcast<f32>(cardMetadata[metaOffset + 4u]);
    let scrollSpeedY = bitcast<f32>(cardMetadata[metaOffset + 5u]);

    // Word 6: widthCells(12) | heightCells(12) | scrollMode(2) | effectMode(4) | reserved(2)
    let word6 = cardMetadata[metaOffset + 6u];
    let widthCells = word6 & 0xFFFu;
    let heightCells = (word6 >> 12u) & 0xFFFu;
    let scrollMode = (word6 >> 24u) & 0x3u;
    let projectionMode = (word6 >> 26u) & 0xFu;

    // Words 7-8: effect parameters
    let projectionStrength = bitcast<f32>(cardMetadata[metaOffset + 7u]);
    let frequency = bitcast<f32>(cardMetadata[metaOffset + 8u]);

    // Words 9-10: colors (alpha=0 means use terminal default)
    let bgColorPacked = cardMetadata[metaOffset + 9u];
    let fgColorPacked = cardMetadata[metaOffset + 10u];

    // Words 11-12: tilt (for future Star Wars effect)
    let tiltX = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let tiltY = bitcast<f32>(cardMetadata[metaOffset + 12u]);

    // Resolve colors: alpha=0 means use terminal default
    let bgColor = select(unpackColor(bgColorPacked), unpackColor(grid.defaultBg), (bgColorPacked & 0xFFu) == 0u);
    let fgColor = select(unpackColor(fgColorPacked), unpackColor(grid.defaultFg), (fgColorPacked & 0xFFu) == 0u);

    if (lineCount == 0u || glyphCount == 0u) {
        return bgColor;
    }

    let viewWidthPx = f32(widthCells) * grid.cellSize.x;
    let viewHeightPx = f32(heightCells) * grid.cellSize.y;

    // Step 1: Screen UV (0-1 within the card)
    let screenUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // Step 2: Apply PROJECTION (flat, cylinder, sphere, wave, perspective)
    let proj = applyProjection(screenUV, projectionMode, projectionStrength, frequency, time, tiltX, tiltY);

    if (!proj.visible) {
        return bgColor;
    }

    // Step 3: Convert projected UV to pixel coordinates
    let viewPixel = proj.uv * vec2<f32>(viewWidthPx, viewHeightPx);

    // Step 4: Compute SCROLL offset
    var scrollX = time * scrollSpeedX;
    var scrollY = time * scrollSpeedY;

    if (scrollMode == YTEXT_SCROLL_PINGPONG) {
        if (contentWidth > viewWidthPx) {
            let range = contentWidth - viewWidthPx;
            let cycle = abs(scrollX) % (range * 2.0);
            scrollX = select(cycle, range * 2.0 - cycle, cycle > range);
        } else { scrollX = 0.0; }
        if (contentHeight > viewHeightPx) {
            let range = contentHeight - viewHeightPx;
            let cycle = abs(scrollY) % (range * 2.0);
            scrollY = select(cycle, range * 2.0 - cycle, cycle > range);
        } else { scrollY = 0.0; }
    } else if (scrollMode != YTEXT_SCROLL_LOOP) {
        if (contentWidth > viewWidthPx) {
            scrollX = clamp(scrollX, 0.0, contentWidth - viewWidthPx);
        } else { scrollX = 0.0; }
        if (contentHeight > viewHeightPx) {
            scrollY = clamp(scrollY, 0.0, contentHeight - viewHeightPx);
        } else { scrollY = 0.0; }
    }

    // Step 5: Content position = view pixel + scroll
    var contentPos = viewPixel + vec2<f32>(scrollX, scrollY);

    // Step 6: Seamless loop wrapping
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

    // Step 7: Find line (binary search)
    let lineIdx = ytextFindLine(lineOffset, lineCount, contentPos.y);
    if (lineIdx < 0) {
        return bgColor;
    }

    let lineOff = lineOffset + u32(lineIdx) * 4u;
    let lineStartGlyph = bitcast<u32>(cardStorage[lineOff + 0u]);
    let lineGlyphCount = bitcast<u32>(cardStorage[lineOff + 1u]);
    let lineY = cardStorage[lineOff + 2u];
    let lineHeight = cardStorage[lineOff + 3u];

    // Step 8: Find glyph (linear search within line)
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
            gColor = gColor * proj.lighting;  // Apply projection lighting

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
