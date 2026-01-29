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
// Metadata layout (48 bytes, fits in 64-byte slot):
//   offset 0:  plotType (u32)   - 0=line, 1=bar, 2=scatter, 3=area
//   offset 4:  dataOffset (u32) - float index into cardStorage (NOT byte offset!)
//   offset 8:  dataCount (u32)  - number of data points
//   offset 12: minValue (f32)   - data minimum for Y axis
//   offset 16: maxValue (f32)   - data maximum for Y axis
//   offset 20: lineColor (u32)  - packed RGBA for line/points
//   offset 24: fillColor (u32)  - packed RGBA for fill/bars
//   offset 28: flags (u32)      - bit 0: show grid, bit 1: show axes
//   offset 32: widthCells (u32) - widget width in cells
//   offset 36: heightCells (u32)- widget height in cells
//   offset 40: bgColor (u32)    - packed RGBA for background

const PLOT_TYPE_LINE: u32 = 0u;
const PLOT_TYPE_BAR: u32 = 1u;
const PLOT_TYPE_SCATTER: u32 = 2u;
const PLOT_TYPE_AREA: u32 = 3u;

const PLOT_FLAG_GRID: u32 = 1u;
const PLOT_FLAG_AXES: u32 = 2u;

fn shaderGlyph_1048577(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
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

    // Read metadata from cardMetadata buffer
    let plotType = cardMetadata[metaOffset + 0u];
    let dataOffset = cardMetadata[metaOffset + 1u];
    let dataCount = cardMetadata[metaOffset + 2u];
    let minValue = bitcast<f32>(cardMetadata[metaOffset + 3u]);
    let maxValue = bitcast<f32>(cardMetadata[metaOffset + 4u]);
    let lineColorPacked = cardMetadata[metaOffset + 5u];
    let fillColorPacked = cardMetadata[metaOffset + 6u];
    let flags = cardMetadata[metaOffset + 7u];
    let widthCells = cardMetadata[metaOffset + 8u];
    let heightCells = cardMetadata[metaOffset + 9u];
    let bgColorPacked = cardMetadata[metaOffset + 10u];

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

    // Draw grid if enabled
    if ((flags & PLOT_FLAG_GRID) != 0u) {
        let gridLines = 4u;
        let gridColorDark = mix(bgColor, lineColor, 0.15);

        // Horizontal grid lines
        for (var i = 0u; i <= gridLines; i++) {
            let y = plotArea.y + f32(i) * (plotArea.w - plotArea.y) / f32(gridLines);
            if (abs(widgetUV.y - y) < 0.008) {
                color = gridColorDark;
            }
        }

        // Vertical grid lines
        for (var i = 0u; i <= gridLines; i++) {
            let x = plotArea.x + f32(i) * (plotArea.z - plotArea.x) / f32(gridLines);
            if (abs(widgetUV.x - x) < 0.008) {
                color = gridColorDark;
            }
        }
    }

    // Draw axes if enabled
    if ((flags & PLOT_FLAG_AXES) != 0u) {
        let axisColor = mix(bgColor, lineColor, 0.5);
        // Y axis (left edge of plot area)
        if (abs(widgetUV.x - plotArea.x) < 0.012 && widgetUV.y >= plotArea.y && widgetUV.y <= plotArea.w) {
            color = axisColor;
        }
        // X axis (bottom edge of plot area)
        if (abs(widgetUV.y - plotArea.w) < 0.012 && widgetUV.x >= plotArea.x && widgetUV.x <= plotArea.z) {
            color = axisColor;
        }
    }

    // Check if we're inside the plot area
    if (widgetUV.x < plotArea.x || widgetUV.x > plotArea.z ||
        widgetUV.y < plotArea.y || widgetUV.y > plotArea.w) {
        return color;
    }

    // Normalize UV within plot area
    let plotUV = vec2<f32>(
        (widgetUV.x - plotArea.x) / (plotArea.z - plotArea.x),
        (widgetUV.y - plotArea.y) / (plotArea.w - plotArea.y)
    );

    // Calculate which data point(s) we're near
    let dataX = plotUV.x * f32(dataCount - 1u);
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
    let plotY = 1.0 - plotUV.y;

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

        if (plotUV.x >= barLeft && plotUV.x <= barRight && plotY <= normalizedValue1) {
            color = fillColor;
            // Bar outline
            if (abs(plotUV.x - barLeft) < 0.01 || abs(plotUV.x - barRight) < 0.01 ||
                abs(plotY - normalizedValue1) < 0.015) {
                color = lineColor;
            }
        }
    } else if (plotType == PLOT_TYPE_SCATTER) {
        // Scatter plot: draw points
        let pointRadius = 0.04;
        let pointX = (f32(dataIndex) + 0.5) / f32(dataCount);
        let pointY = normalizedValue1;
        let dist = distance(vec2<f32>(plotUV.x, plotY), vec2<f32>(pointX, pointY));
        if (dist < pointRadius) {
            let alpha = 1.0 - dist / pointRadius;
            color = mix(color, lineColor, alpha * alpha);
        }
        // Also check next point
        let nextPointX = (f32(nextIndex) + 0.5) / f32(dataCount);
        let nextPointY = normalizedValue2;
        let nextDist = distance(vec2<f32>(plotUV.x, plotY), vec2<f32>(nextPointX, nextPointY));
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
