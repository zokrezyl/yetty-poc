// Shader glyph: Plot widget (codepoint 1048577 / U+100001)
// Card-based plot rendering - uses cardMetadata and cardStorage buffers
//
// =============================================================================
// DESIGN: ANSI-Compatible Cell Encoding for Scroll-Independent Widgets
// =============================================================================
//
// Problem: Cards/widgets span multiple terminal cells. When terminal scrolls,
// the widget must move with it. Direct buffer manipulation breaks this.
//
// Solution: Encode card data in standard ANSI true-color sequences (24-bit RGB).
// This allows card cells to flow through vterm's normal line handling, which
// properly manages scrolling, newlines, and scrollback buffer.
//
// Per-cell encoding (fits in ANSI true-color RGB, 24 bits each):
//
//   fg (24 bits): Metadata SLOT INDEX (not byte offset!)
//     - Metadata is always allocated in 32-byte aligned slots
//     - slotIndex = byteOffset / 32
//     - With 24 bits: 2^24 slots × 32 bytes = 512MB addressable metadata
//     - Shader converts: metaOffset_u32 = slotIndex * 8 (32 bytes = 8 u32s)
//
//   bg (24 bits): Relative position within widget
//     - 12 bits for relCol (0-4095), 12 bits for relRow (0-4095)
//     - Encoding: bg24 = (relRow << 12) | relCol
//     - Max widget size: 4096 × 4096 cells (way more than any terminal)
//     - This makes widgets scroll-independent: each cell knows its position
//       within the widget regardless of where the widget is on screen
//
// Why not use full 32-bit fg/bg?
//   - ANSI true-color only provides 24-bit RGB per color
//   - We want cards to work with standard terminal escape sequences
//   - This enables: OSC → create card → output ANSI sequences → vterm handles rest
//
// =============================================================================
//
// Metadata layout (64 bytes = 16 u32 slots):
//   [0]  plotType(8) | flags(8) | pad(16)
//   [1]  widthCells(16) | heightCells(16)
//   [2]  dataOffset (u32) - float index into cardStorage
//   [3]  dataCount (u32)
//   [4]  minValue (f32)
//   [5]  maxValue (f32)
//   [6]  lineColor (u32 RGBA)
//   [7]  fillColor (u32 RGBA)
//   [8]  bgColor (u32 RGBA)
//   [9]  zoom (f32)
//   [10] centerX (f32)
//   [11] centerY (f32)
//   [12] glyphBase0(8) | glyphDot(8) | glyphMinus(8) | pad(8)
//   [13-15] reserved

const PLOT_TYPE_LINE: u32 = 0u;
const PLOT_TYPE_BAR: u32 = 1u;
const PLOT_TYPE_SCATTER: u32 = 2u;
const PLOT_TYPE_AREA: u32 = 3u;

const PLOT_FLAG_GRID: u32 = 1u;
const PLOT_FLAG_AXES: u32 = 2u;

fn shaderGlyph_1048577(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>, lastChar: u32, lastCharTime: f32) -> vec3<f32> {
    // ==========================================================================
    // Decode ANSI-compatible cell encoding (see header comments for design)
    // ==========================================================================

    // fg (24 bits used): metadata slot index
    // Slot index = byteOffset / 64, so multiply by 16 to get u32 index (64 bytes = 16 u32s)
    let slotIndex = fg & 0xFFFFFFu;  // Mask to 24 bits (ANSI true-color limit)
    let metaOffset = slotIndex * 16u;

    // bg (24 bits used): relative position within widget
    // Lower 12 bits = relCol, upper 12 bits = relRow
    let bg24 = bg & 0xFFFFFFu;       // Mask to 24 bits
    let relCol = bg24 & 0xFFFu;      // Bits 0-11: column (0-4095)
    let relRow = (bg24 >> 12u) & 0xFFFu;  // Bits 12-23: row (0-4095)

    // Read metadata from cardMetadata buffer (new packed layout)
    let slot0 = cardMetadata[metaOffset + 0u];
    let plotType = slot0 & 0xFFu;
    let flags = (slot0 >> 8u) & 0xFFu;
    let slot1 = cardMetadata[metaOffset + 1u];
    let widthCells = slot1 & 0xFFFFu;
    let heightCells = (slot1 >> 16u) & 0xFFFFu;
    let dataOffset = cardMetadata[metaOffset + 2u];
    let dataCount = cardMetadata[metaOffset + 3u];
    let minValue = bitcast<f32>(cardMetadata[metaOffset + 4u]);
    let maxValue = bitcast<f32>(cardMetadata[metaOffset + 5u]);
    let lineColorPacked = cardMetadata[metaOffset + 6u];
    let fillColorPacked = cardMetadata[metaOffset + 7u];
    let bgColorPacked = cardMetadata[metaOffset + 8u];
    let zoomRaw = bitcast<f32>(cardMetadata[metaOffset + 9u]);
    let centerXRaw = bitcast<f32>(cardMetadata[metaOffset + 10u]);
    let centerYRaw = bitcast<f32>(cardMetadata[metaOffset + 11u]);
    let glyphSlot = cardMetadata[metaOffset + 12u];
    let glyphBase0 = glyphSlot & 0xFFu;
    let glyphDot = (glyphSlot >> 8u) & 0xFFu;
    let glyphMinus = (glyphSlot >> 16u) & 0xFFu;

    // Handle legacy (zero = no zoom metadata)
    let effectiveZoom = select(zoomRaw, 1.0, zoomRaw <= 0.0);
    let effectiveCenterX = select(centerXRaw, 0.5, zoomRaw <= 0.0);
    let effectiveCenterY = select(centerYRaw, 0.5, zoomRaw <= 0.0);

    let lineColor = unpackColor(lineColorPacked);
    let fillColor = unpackColor(fillColorPacked);
    let bgColor = unpackColor(bgColorPacked);

    // If no data, just show background with optional grid
    if (dataCount == 0u) {
        return bgColor;
    }

    // Compute widget-wide UV using relative cell position + local UV within cell
    // This makes the widget scroll-independent
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) / vec2<f32>(f32(widthCells), f32(heightCells));

    var color = bgColor;
    let padding = 0.05;  // 5% padding on each side
    let plotArea = vec4<f32>(padding, padding, 1.0 - padding, 1.0 - padding);
    let plotWidth = plotArea.z - plotArea.x;
    let plotHeight = plotArea.w - plotArea.y;

    // Normalize UV within plot area (can be outside 0-1 in padding region)
    let plotUV = vec2<f32>(
        (widgetUV.x - plotArea.x) / plotWidth,
        (widgetUV.y - plotArea.y) / plotHeight
    );

    // Apply zoom/pan: transform plotUV through viewport
    let zoomedUV = (plotUV - vec2(0.5)) / effectiveZoom + vec2(effectiveCenterX, effectiveCenterY);

    // =========================================================================
    // Grid spacing in DATA VALUE space so labels show nice numbers
    // =========================================================================
    let valueRange = maxValue - minValue;
    let safeValueRange = select(valueRange, 1.0, abs(valueRange) < 0.0001);

    // Y axis: visible value range depends on zoom
    let visibleValueRange = abs(safeValueRange) / effectiveZoom;
    let rawYSpacing = visibleValueRange / 5.0;
    let logYSp = log2(rawYSpacing) / 3.321928;
    let magY = pow(10.0, floor(logYSp));
    let resY = rawYSpacing / magY;
    var ySpacing: f32;
    if (resY < 1.5) { ySpacing = magY; }
    else if (resY < 3.5) { ySpacing = 2.0 * magY; }
    else if (resY < 7.5) { ySpacing = 5.0 * magY; }
    else { ySpacing = 10.0 * magY; }

    // X axis: visible normalized range
    let visibleXRange = 1.0 / effectiveZoom;
    let rawXSpacing = visibleXRange / 5.0;
    let logXSp = log2(rawXSpacing) / 3.321928;
    let magX = pow(10.0, floor(logXSp));
    let resX = rawXSpacing / magX;
    var xSpacing: f32;
    if (resX < 1.5) { xSpacing = magX; }
    else if (resX < 3.5) { xSpacing = 2.0 * magX; }
    else if (resX < 7.5) { xSpacing = 5.0 * magX; }
    else { xSpacing = 10.0 * magX; }

    // Convert current position to data value for Y grid
    // zoomedUV.y: 0=top(maxValue), 1=bottom(minValue)
    // dataValue = maxValue - zoomedUV.y * valueRange  (i.e. plotY = 1-zoomedUV.y maps to normalized)
    let currentYValue = maxValue - zoomedUV.y * safeValueRange;

    // Line thickness scales inversely with zoom so lines stay consistent on screen
    let yGridThickValue = 0.003 * abs(safeValueRange) / effectiveZoom;
    let xGridThickNorm = 0.003 / effectiveZoom;
    let yAxisThickValue = 0.005 * abs(safeValueRange) / effectiveZoom;
    let xAxisThickNorm = 0.005 / effectiveZoom;

    // Snap Y to nearest nice VALUE
    let snapYValue = round(currentYValue / ySpacing) * ySpacing;
    // Snap X in normalized space
    let snapXNorm = round(zoomedUV.x / xSpacing) * xSpacing;

    let insidePlot = widgetUV.x >= plotArea.x && widgetUV.x <= plotArea.z &&
                     widgetUV.y >= plotArea.y && widgetUV.y <= plotArea.w;

    // Draw grid and axes only inside plot area
    if (insidePlot) {
        if ((flags & PLOT_FLAG_GRID) != 0u) {
            let gridColorDark = mix(bgColor, lineColor, 0.15);
            // X grid lines (in normalized space)
            if (abs(zoomedUV.x - snapXNorm) < xGridThickNorm) {
                color = gridColorDark;
            }
            // Y grid lines (in value space)
            if (abs(currentYValue - snapYValue) < yGridThickValue) {
                color = gridColorDark;
            }
        }

        if ((flags & PLOT_FLAG_AXES) != 0u) {
            let axisColor = mix(bgColor, lineColor, 0.5);
            // Y axis (x=0 in normalized space)
            if (abs(zoomedUV.x) < xAxisThickNorm) {
                color = axisColor;
            }
            // X axis (value=0 line)
            if (abs(currentYValue) < yAxisThickValue) {
                color = axisColor;
            }
        }
    }

    // =========================================================================
    // Axis labels — rendered in padding area using MSDF glyphs
    // =========================================================================
    if (glyphBase0 > 0u) {
        let labelColor = mix(bgColor, lineColor, 0.6);
        let widgetPixelW = f32(widthCells) * grid.cellSize.x;
        let widgetPixelH = f32(heightCells) * grid.cellSize.y;
        let screenScale = widgetPixelW;

        // Glyph size: one cell height in widget UV space
        let glyphH = 1.0 / f32(heightCells);
        let glyphW = glyphH * (grid.cellSize.x / grid.cellSize.y) * 0.6; // narrower than cell

        // --- Y axis labels (left padding) ---
        if (widgetUV.x < plotArea.x && widgetUV.y >= plotArea.y && widgetUV.y <= plotArea.w) {
            // Convert snapped value back to widget Y
            let snapNormY = (maxValue - snapYValue) / safeValueRange; // normalized 0-1
            let snapPlotUVy = (snapNormY - effectiveCenterY) * effectiveZoom + 0.5;
            let snapWidgetY = plotArea.y + snapPlotUVy * plotHeight;

            let labelTop = snapWidgetY - glyphH * 0.5;
            let labelBot = snapWidgetY + glyphH * 0.5;
            if (widgetUV.y >= labelTop && widgetUV.y < labelBot) {
                let dataVal = snapYValue;
                let glyphUV_y = (widgetUV.y - labelTop) / glyphH;

                var val = dataVal;
                var hasSign = 0u;
                if (val < 0.0) { hasSign = 1u; val = -val; }

                // Decimal places based on value spacing
                var decimals = 0u;
                if (ySpacing < 0.01) { decimals = 3u; }
                else if (ySpacing < 0.1) { decimals = 2u; }
                else if (ySpacing < 1.0) { decimals = 1u; }

                let intPart = u32(floor(val));
                let fracPart = u32(round(fract(val) * pow(10.0, f32(decimals))));
                var intDigits = 1u;
                if (intPart >= 10u) { intDigits = 2u; }
                if (intPart >= 100u) { intDigits = 3u; }
                if (intPart >= 1000u) { intDigits = 4u; }
                let totalChars = hasSign + intDigits + select(0u, 1u + decimals, decimals > 0u);

                let labelRight = plotArea.x - 0.003;
                let labelLeft = labelRight - f32(totalChars) * glyphW;

                if (widgetUV.x >= labelLeft && widgetUV.x < labelRight) {
                    let charIdx = u32(floor((widgetUV.x - labelLeft) / glyphW));
                    let charLocalUV = vec2<f32>(
                        fract((widgetUV.x - labelLeft) / glyphW),
                        glyphUV_y
                    );

                    var glyphIdx = 0u;
                    let digitStart = hasSign;
                    if (hasSign > 0u && charIdx == 0u) {
                        glyphIdx = glyphMinus;
                    } else if (charIdx >= digitStart && charIdx < digitStart + intDigits) {
                        let digitPos = charIdx - digitStart;
                        let divisor = u32(round(pow(10.0, f32(intDigits - 1u - digitPos))));
                        glyphIdx = glyphBase0 + (intPart / divisor) % 10u;
                    } else if (decimals > 0u && charIdx == digitStart + intDigits) {
                        glyphIdx = glyphDot;
                    } else if (decimals > 0u && charIdx > digitStart + intDigits) {
                        let decPos = charIdx - digitStart - intDigits - 1u;
                        let divisor = u32(round(pow(10.0, f32(decimals - 1u - decPos))));
                        glyphIdx = glyphBase0 + (fracPart / divisor) % 10u;
                    }

                    if (glyphIdx > 0u) {
                        let r = renderMsdfGlyph(charLocalUV, glyphIdx, labelColor, color, grid.pixelRange, screenScale);
                        if (r.alpha > 0.01) { color = r.color; }
                    }
                }
            }
        }

        // --- X axis labels (bottom padding) ---
        if (widgetUV.y > plotArea.w && widgetUV.x >= plotArea.x && widgetUV.x <= plotArea.z) {
            // X axis shows data index: snapXNorm * (dataCount-1)
            let dataVal = snapXNorm * f32(dataCount - 1u);

            // Convert snapped normalized X back to widget X
            let snapPlotUVx = (snapXNorm - effectiveCenterX) * effectiveZoom + 0.5;
            let snapWidgetX = plotArea.x + snapPlotUVx * plotWidth;

            var val = dataVal;
            var hasSign = 0u;
            if (val < 0.0) { hasSign = 1u; val = -val; }

            var decimals = 0u;
            let xDataSpacing = xSpacing * f32(dataCount - 1u);
            if (xDataSpacing < 0.01) { decimals = 3u; }
            else if (xDataSpacing < 0.1) { decimals = 2u; }
            else if (xDataSpacing < 1.0) { decimals = 1u; }

            let intPart = u32(floor(val));
            let fracPart = u32(round(fract(val) * pow(10.0, f32(decimals))));
            var intDigits = 1u;
            if (intPart >= 10u) { intDigits = 2u; }
            if (intPart >= 100u) { intDigits = 3u; }
            if (intPart >= 1000u) { intDigits = 4u; }
            let totalChars = hasSign + intDigits + select(0u, 1u + decimals, decimals > 0u);

            let labelLeft = snapWidgetX - f32(totalChars) * glyphW * 0.5;
            let labelRight = labelLeft + f32(totalChars) * glyphW;
            let labelTop = plotArea.w + 0.003;
            let labelBot = labelTop + glyphH;

            if (widgetUV.x >= labelLeft && widgetUV.x < labelRight &&
                widgetUV.y >= labelTop && widgetUV.y < labelBot) {
                let charIdx = u32(floor((widgetUV.x - labelLeft) / glyphW));
                let charLocalUV = vec2<f32>(
                    fract((widgetUV.x - labelLeft) / glyphW),
                    (widgetUV.y - labelTop) / glyphH
                );

                var glyphIdx = 0u;
                let digitStart = hasSign;
                if (hasSign > 0u && charIdx == 0u) {
                    glyphIdx = glyphMinus;
                } else if (charIdx >= digitStart && charIdx < digitStart + intDigits) {
                    let digitPos = charIdx - digitStart;
                    let divisor = u32(round(pow(10.0, f32(intDigits - 1u - digitPos))));
                    glyphIdx = glyphBase0 + (intPart / divisor) % 10u;
                } else if (decimals > 0u && charIdx == digitStart + intDigits) {
                    glyphIdx = glyphDot;
                } else if (decimals > 0u && charIdx > digitStart + intDigits) {
                    let decPos = charIdx - digitStart - intDigits - 1u;
                    let divisor = u32(round(pow(10.0, f32(decimals - 1u - decPos))));
                    glyphIdx = glyphBase0 + (fracPart / divisor) % 10u;
                }

                if (glyphIdx > 0u) {
                    let r = renderMsdfGlyph(charLocalUV, glyphIdx, labelColor, color, grid.pixelRange, screenScale);
                    if (r.alpha > 0.01) { color = r.color; }
                }
            }
        }
    }

    // If outside plot area (and not handled by labels above), return
    if (!insidePlot) {
        return color;
    }

    // If zoomed UV is outside [0,1] data range, skip data drawing (grid/axes already drawn)
    if (zoomedUV.x < 0.0 || zoomedUV.x > 1.0 || zoomedUV.y < 0.0 || zoomedUV.y > 1.0) {
        return color;
    }

    // Calculate which data point(s) we're near
    let dataX = zoomedUV.x * f32(dataCount - 1u);
    let dataIndex = u32(floor(dataX));
    let nextIndex = min(dataIndex + 1u, dataCount - 1u);
    let t = fract(dataX);

    // Get data values
    let value1 = cardStorage[dataOffset + dataIndex];
    let value2 = cardStorage[dataOffset + nextIndex];

    // Normalize to 0-1 range
    let range = maxValue - minValue;
    let safeRange = select(range, 1.0, range == 0.0);
    let normalizedValue1 = (value1 - minValue) / safeRange;
    let normalizedValue2 = (value2 - minValue) / safeRange;

    // Interpolate for smooth line
    let interpolatedValue = mix(normalizedValue1, normalizedValue2, t);

    // Y is inverted (0 at top, 1 at bottom in UV), but we want 0 at bottom
    let plotY = 1.0 - zoomedUV.y;

    if (plotType == PLOT_TYPE_LINE) {
        // Line plot: draw line between points
        let lineWidth = 0.025;
        let dist = abs(plotY - interpolatedValue);
        if (dist < lineWidth) {
            let alpha = 1.0 - dist / lineWidth;
            color = mix(color, lineColor, alpha);
        }
    } else if (plotType == PLOT_TYPE_BAR) {
        // Bar chart
        let barWidth = 0.8 / f32(dataCount);
        let barCenter = (f32(dataIndex) + 0.5) / f32(dataCount);
        let barLeft = barCenter - barWidth * 0.5;
        let barRight = barCenter + barWidth * 0.5;

        if (zoomedUV.x >= barLeft && zoomedUV.x <= barRight && plotY <= normalizedValue1) {
            color = fillColor;
            // Bar outline
            if (abs(zoomedUV.x - barLeft) < 0.01 || abs(zoomedUV.x - barRight) < 0.01 ||
                abs(plotY - normalizedValue1) < 0.015) {
                color = lineColor;
            }
        }
    } else if (plotType == PLOT_TYPE_SCATTER) {
        // Scatter plot: draw points
        let pointRadius = 0.04;
        let pointX = (f32(dataIndex) + 0.5) / f32(dataCount);
        let pointY = normalizedValue1;
        let dist = distance(vec2<f32>(zoomedUV.x, plotY), vec2<f32>(pointX, pointY));
        if (dist < pointRadius) {
            let alpha = 1.0 - dist / pointRadius;
            color = mix(color, lineColor, alpha * alpha);
        }
        // Also check next point
        let nextPointX = (f32(nextIndex) + 0.5) / f32(dataCount);
        let nextPointY = normalizedValue2;
        let nextDist = distance(vec2<f32>(zoomedUV.x, plotY), vec2<f32>(nextPointX, nextPointY));
        if (nextDist < pointRadius) {
            let alpha = 1.0 - nextDist / pointRadius;
            color = mix(color, lineColor, alpha * alpha);
        }
    } else if (plotType == PLOT_TYPE_AREA) {
        // Area chart: filled under line
        if (plotY <= interpolatedValue) {
            color = fillColor;
        }
        // Line on top
        let lineWidth = 0.02;
        let dist = abs(plotY - interpolatedValue);
        if (dist < lineWidth) {
            let alpha = 1.0 - dist / lineWidth;
            color = mix(color, lineColor, alpha);
        }
    }

    return color;
}
