// Shader glyph: Plot widget (codepoint 1048592 / U+100010)
// Card-based plot rendering - uses cardMetadata and cardStorage buffers
// The fg u32 contains the metadata offset into cardMetadata buffer
//
// Metadata layout (64 bytes, fits in SLOT_64):
//   offset 0:  plotType (u32)   - 0=line, 1=bar, 2=scatter, 3=area
//   offset 4:  dataOffset (u32) - float index into cardStorage (NOT byte offset!)
//   offset 8:  dataCount (u32)  - number of data points
//   offset 12: minValue (f32)   - data minimum for Y axis
//   offset 16: maxValue (f32)   - data maximum for Y axis
//   offset 20: lineColor (u32)  - packed RGBA for line/points
//   offset 24: fillColor (u32)  - packed RGBA for fill/bars
//   offset 28: flags (u32)      - bit 0: show grid, bit 1: show axes
//   offset 32: widgetCol (u32)  - widget's column position in grid
//   offset 36: widgetRow (u32)  - widget's row position in grid
//   offset 40: widthCells (u32) - widget width in cells
//   offset 44: heightCells (u32)- widget height in cells

const PLOT_TYPE_LINE: u32 = 0u;
const PLOT_TYPE_BAR: u32 = 1u;
const PLOT_TYPE_SCATTER: u32 = 2u;
const PLOT_TYPE_AREA: u32 = 3u;

const PLOT_FLAG_GRID: u32 = 1u;
const PLOT_FLAG_AXES: u32 = 2u;

fn shaderGlyph_1048592(localUV: vec2<f32>, time: f32, fg: u32, bg: u32, pixelPos: vec2<f32>, mousePos: vec2<f32>) -> vec3<f32> {
    let bgColor = unpackColor(bg);

    // fg contains the metadata offset (divided by 4 to get u32 index)
    let metaOffset = fg / 4u;

    // Read metadata from cardMetadata buffer
    let plotType = cardMetadata[metaOffset + 0u];
    let dataOffset = cardMetadata[metaOffset + 1u];
    let dataCount = cardMetadata[metaOffset + 2u];
    let minValue = bitcast<f32>(cardMetadata[metaOffset + 3u]);
    let maxValue = bitcast<f32>(cardMetadata[metaOffset + 4u]);
    let lineColorPacked = cardMetadata[metaOffset + 5u];
    let fillColorPacked = cardMetadata[metaOffset + 6u];
    let flags = cardMetadata[metaOffset + 7u];
    let widgetCol = cardMetadata[metaOffset + 8u];
    let widgetRow = cardMetadata[metaOffset + 9u];
    let widthCells = cardMetadata[metaOffset + 10u];
    let heightCells = cardMetadata[metaOffset + 11u];

    let lineColor = unpackColor(lineColorPacked);
    let fillColor = unpackColor(fillColorPacked);

    // If no data, just show background with optional grid
    if (dataCount == 0u) {
        return bgColor;
    }

    // Compute widget-wide UV (not per-cell UV)
    // widgetPixelPos = top-left corner of widget in pixels
    let widgetPixelX = f32(widgetCol) * grid.cellSize.x;
    let widgetPixelY = f32(widgetRow) * grid.cellSize.y;
    // widgetPixelSize = total widget size in pixels
    let widgetPixelW = f32(widthCells) * grid.cellSize.x;
    let widgetPixelH = f32(heightCells) * grid.cellSize.y;
    // Compute UV across entire widget (0-1 range)
    let widgetUV = (pixelPos - vec2<f32>(widgetPixelX, widgetPixelY)) / vec2<f32>(widgetPixelW, widgetPixelH);

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
