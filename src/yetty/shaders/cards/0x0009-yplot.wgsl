// Shader glyph: YPlot widget (codepoint 1048585 / U+100009)
// GPU-accelerated plot rendering using yfsvm bytecode interpreter
//
// =============================================================================
// Design: Composes yfsvm for expression evaluation with built-in rendering
// =============================================================================
//
// This shader supports two modes:
// 1. Expression mode: Uses yfsvm bytecode to evaluate mathematical expressions
// 2. Buffer mode: Reads float data from cardStorage and renders as line plot
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   [0]  flags(8) | funcCount(8) | glyphBase0(8) | glyphDot(8)
//   [1]  widthCells(16) | heightCells(16)
//   [2]  bytecodeOffset (u32) - expression mode
//   [3]  bytecodeSize (u32)
//   [4]  xMin (f32)
//   [5]  xMax (f32)
//   [6]  yMin (f32)
//   [7]  yMax (f32)
//   [8]  dataOffset (u32) - buffer mode: float index into cardStorage
//   [9]  dataCount (u32) - buffer mode: number of floats
//   [10] marginLeft (f32)
//   [11] marginBottom (f32)
//   [12] zoom (f32)
//   [13] centerX (f32)
//   [14] centerY (f32)
//   [15] colorTableOffset(24) | glyphMinus(8)
// =============================================================================

const YPLOT_FLAG_GRID: u32 = 1u;
const YPLOT_FLAG_AXES: u32 = 2u;
const YPLOT_FLAG_LABELS: u32 = 4u;
const YPLOT_FLAG_BUFFER: u32 = 8u;  // Buffer mode (vs expression mode)

// Colors
const YPLOT_BG_COLOR: vec3<f32> = vec3<f32>(0.102, 0.102, 0.180);  // #1A1A2E
const YPLOT_GRID_COLOR: vec3<f32> = vec3<f32>(0.267, 0.267, 0.267);  // #444444
const YPLOT_AXIS_COLOR: vec3<f32> = vec3<f32>(0.667, 0.667, 0.667);  // #AAAAAA
const YPLOT_LINE_COLOR: vec3<f32> = vec3<f32>(0.2, 0.8, 0.2);  // Green for buffer mode

fn shaderGlyph_1048585(
    localUV: vec2<f32>,
    time: f32,
    fg: u32,
    bg: u32,
    pixelPos: vec2<f32>,
    mousePos: vec2<f32>,
    lastChar: u32,
    lastCharTime: f32
) -> vec3<f32> {
    // Decode ANSI cell encoding
    let slotIndex = fg & 0xFFFFFFu;
    let metaOffset = slotIndex * 16u;  // 64 bytes = 16 u32s

    let bg24 = bg & 0xFFFFFFu;
    let relCol = bg24 & 0xFFFu;
    let relRow = (bg24 >> 12u) & 0xFFFu;

    // Try to read metadata from buffer
    let slot0 = cardMetadata[metaOffset + 0u];
    let slot1 = cardMetadata[metaOffset + 1u];

    // Detect if buffer is working by checking multiple conditions:
    // 1. flags should be 1-15 (valid flag combination including buffer mode)
    // 2. widthCells and heightCells should be 1-100 (reasonable range)
    let flagsRaw = slot0 & 0xFFu;
    let widthRaw = slot1 & 0xFFFFu;
    let heightRaw = slot1 >> 16u;
    let bufferWorking = flagsRaw >= 1u && flagsRaw <= 15u &&
                        widthRaw >= 1u && widthRaw <= 100u &&
                        heightRaw >= 1u && heightRaw <= 100u;

    // Use buffer values if working, otherwise use hardcoded defaults
    var flags = 7u;
    var funcCount = 1u;
    var glyphBase0 = 0u;
    var glyphDot = 0u;
    var glyphMinus = 0u;
    var widthCells = 1u;
    var heightCells = 1u;
    var bytecodeOffset = 0u;
    var bytecodeSize = 24u;
    var xMin = -3.14159;
    var xMax = 3.14159;
    var yMin = -1.5;
    var yMax = 1.5;
    var dataOffset = 0u;
    var dataCount = 0u;
    var marginLeft = 0.08;
    var marginBottom = 0.08;
    var zoom = 1.0;
    var centerX = 0.5;
    var centerY = 0.5;
    var colorTableOffset = 0u;

    if (bufferWorking) {
        flags = slot0 & 0xFFu;
        funcCount = (slot0 >> 8u) & 0xFFu;
        glyphBase0 = (slot0 >> 16u) & 0xFFu;
        glyphDot = (slot0 >> 24u) & 0xFFu;
        widthCells = slot1 & 0xFFFFu;
        heightCells = slot1 >> 16u;
        if (widthCells == 0u) { widthCells = 1u; }
        if (heightCells == 0u) { heightCells = 1u; }

        bytecodeOffset = cardMetadata[metaOffset + 2u];
        bytecodeSize = cardMetadata[metaOffset + 3u];
        xMin = bitcast<f32>(cardMetadata[metaOffset + 4u]);
        xMax = bitcast<f32>(cardMetadata[metaOffset + 5u]);
        yMin = bitcast<f32>(cardMetadata[metaOffset + 6u]);
        yMax = bitcast<f32>(cardMetadata[metaOffset + 7u]);
        dataOffset = cardMetadata[metaOffset + 8u];
        dataCount = cardMetadata[metaOffset + 9u];
        marginLeft = bitcast<f32>(cardMetadata[metaOffset + 10u]);
        marginBottom = bitcast<f32>(cardMetadata[metaOffset + 11u]);
        zoom = bitcast<f32>(cardMetadata[metaOffset + 12u]);
        centerX = bitcast<f32>(cardMetadata[metaOffset + 13u]);
        centerY = bitcast<f32>(cardMetadata[metaOffset + 14u]);
        let slot15 = cardMetadata[metaOffset + 15u];
        colorTableOffset = slot15 & 0xFFFFFFu;
        glyphMinus = (slot15 >> 24u) & 0xFFu;
    }

    // Compute plot area dimensions from margins
    let plotWidth = 1.0 - marginLeft - 0.02;
    let plotHeight = 1.0 - marginBottom - 0.02;

    // Compute widget-wide UV (0-1 across entire widget)
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

    // Fallback if buffer not working
    if (!bufferWorking) {
        return vec3<f32>(0.3, 0.1, 0.1);  // Dark red = buffer error
    }

    // Apply zoom and pan to the data range
    let xRange = xMax - xMin;
    let yRange = yMax - yMin;
    let zoomedXRange = xRange / zoom;
    let zoomedYRange = yRange / zoom;
    let panOffsetX = (centerX - 0.5) * xRange;
    let panOffsetY = (centerY - 0.5) * yRange;
    let viewXMin = xMin + panOffsetX + (xRange - zoomedXRange) * 0.5;
    let viewXMax = viewXMin + zoomedXRange;
    let viewYMin = yMin + panOffsetY + (yRange - zoomedYRange) * 0.5;
    let viewYMax = viewYMin + zoomedYRange;

    // Start with background
    var color = YPLOT_BG_COLOR;

    // Compute plot area boundaries
    let plotLeft = marginLeft;
    let plotBottom = marginBottom;
    let plotRight = marginLeft + plotWidth;
    let plotTop = marginBottom + plotHeight;

    // Check if we're in the plot area
    let inPlotArea = widgetUV.x >= plotLeft && widgetUV.x <= plotRight &&
                     widgetUV.y >= plotBottom && widgetUV.y <= plotTop;

    if (inPlotArea) {
        // Map widget UV to plot UV (0-1 within plot area)
        let plotUV = vec2<f32>(
            (widgetUV.x - plotLeft) / plotWidth,
            (widgetUV.y - plotBottom) / plotHeight
        );

        // Map to data coordinates (using zoomed/panned view)
        let dataX = mix(viewXMin, viewXMax, plotUV.x);
        let dataY = mix(viewYMin, viewYMax, plotUV.y);

        // Draw grid
        if ((flags & YPLOT_FLAG_GRID) != 0u) {
            color = drawGrid(color, plotUV, viewXMin, viewXMax, viewYMin, viewYMax);
        }

        // Draw axes
        if ((flags & YPLOT_FLAG_AXES) != 0u) {
            color = drawAxes(color, plotUV, viewXMin, viewXMax, viewYMin, viewYMax);
        }

        // Pixel size in data coordinates for anti-aliasing
        let pixelSizeY = (viewYMax - viewYMin) / (f32(heightCells) * 20.0);

        // Check for buffer mode
        let isBufferMode = (flags & YPLOT_FLAG_BUFFER) != 0u && dataCount > 1u;

        if (isBufferMode) {
            // =========================================================================
            // Buffer mode: render data from cardStorage as line plot
            // =========================================================================
            let lineColor = YPLOT_LINE_COLOR;

            // Map plotUV.x to data index
            let dataIndex_f = plotUV.x * f32(dataCount - 1u);
            let dataIndex = u32(floor(dataIndex_f));
            let nextIndex = min(dataIndex + 1u, dataCount - 1u);
            let interpolatedT = fract(dataIndex_f);

            // Read two adjacent data points and interpolate
            let value1 = cardStorage[dataOffset + dataIndex];
            let value2 = cardStorage[dataOffset + nextIndex];
            let interpolatedY = mix(value1, value2, interpolatedT);

            // Map y value to plot UV (using zoomed/panned view)
            let yNorm = (interpolatedY - viewYMin) / (viewYMax - viewYMin);

            // Distance from current pixel to the curve
            let dist = abs(plotUV.y - yNorm);

            // Anti-aliased line rendering
            let lineWidth = 2.0 * pixelSizeY / (viewYMax - viewYMin);
            if (dist < lineWidth) {
                let alpha = 1.0 - dist / lineWidth;
                color = mix(color, lineColor, alpha);
            }
        } else if (funcCount > 0u && bytecodeSize > 0u) {
            // =========================================================================
            // Expression mode: evaluate and draw plot lines using yfsvm
            // =========================================================================
            var samplers: array<f32, 8>;
            samplers[0] = dataX;

            // Evaluate each function
            for (var fi = 0u; fi < funcCount && fi < 8u; fi = fi + 1u) {
                // Get function color from color table
                let funcColorPacked = bitcast<u32>(cardStorage[colorTableOffset + fi]);
                let funcColor = yplot_unpackColorARGB(funcColorPacked);

                // Evaluate function at this x position
                let y = yfsvm_execute(bytecodeOffset, fi, dataX, time, samplers);

                // Map y to plot UV (using zoomed/panned view)
                let yNorm = (y - viewYMin) / (viewYMax - viewYMin);

                // Distance from current pixel to the curve
                let dist = abs(plotUV.y - yNorm);

                // Anti-aliased line rendering
                let lineWidth = 2.0 * pixelSizeY / (viewYMax - viewYMin);
                if (dist < lineWidth) {
                    let alpha = 1.0 - dist / lineWidth;
                    color = mix(color, funcColor, alpha);
                }
            }
        }
    } else {
        // Outside plot area - margin for labels
        color = YPLOT_BG_COLOR * 0.8;
    }

    // =========================================================================
    // Axis labels — rendered in margin area using MSDF glyphs
    // =========================================================================
    if (glyphBase0 > 0u) {
        let labelColor = vec3<f32>(0.7, 0.7, 0.7);
        let widgetPixelW = f32(widthCells) * grid.cellSize.x;
        let widgetPixelH = f32(heightCells) * grid.cellSize.y;
        let screenScale = widgetPixelW;

        // Glyph size: one cell height in widget UV space
        let glyphH = 1.0 / f32(heightCells);
        let glyphW = glyphH * (grid.cellSize.x / grid.cellSize.y) * 0.6;

        // Compute nice grid spacing for labels
        let visibleYRange = (viewYMax - viewYMin);
        let rawYSpacing = visibleYRange / 5.0;
        let logYSp = log2(abs(rawYSpacing)) / 3.321928;
        let magY = pow(10.0, floor(logYSp));
        let resY = abs(rawYSpacing) / magY;
        var ySpacing: f32;
        if (resY < 1.5) { ySpacing = magY; }
        else if (resY < 3.5) { ySpacing = 2.0 * magY; }
        else if (resY < 7.5) { ySpacing = 5.0 * magY; }
        else { ySpacing = 10.0 * magY; }

        let visibleXRange = (viewXMax - viewXMin);
        let rawXSpacing = visibleXRange / 5.0;
        let logXSp = log2(abs(rawXSpacing)) / 3.321928;
        let magX = pow(10.0, floor(logXSp));
        let resX = abs(rawXSpacing) / magX;
        var xSpacing: f32;
        if (resX < 1.5) { xSpacing = magX; }
        else if (resX < 3.5) { xSpacing = 2.0 * magX; }
        else if (resX < 7.5) { xSpacing = 5.0 * magX; }
        else { xSpacing = 10.0 * magX; }

        // --- Y axis labels (left margin) ---
        if (widgetUV.x < plotLeft && widgetUV.y >= plotBottom && widgetUV.y <= plotTop) {
            // Current Y position in data coordinates
            let plotUVy = (widgetUV.y - plotBottom) / plotHeight;
            let dataY = mix(viewYMin, viewYMax, plotUVy);
            let snapY = round(dataY / ySpacing) * ySpacing;

            // Convert snapped Y back to widget UV
            let snapPlotUVy = (snapY - viewYMin) / (viewYMax - viewYMin);
            let snapWidgetY = plotBottom + snapPlotUVy * plotHeight;

            let labelTop = snapWidgetY - glyphH * 0.5;
            let labelBot = snapWidgetY + glyphH * 0.5;
            if (widgetUV.y >= labelTop && widgetUV.y < labelBot) {
                let glyphUV_y = (widgetUV.y - labelTop) / glyphH;
                let rendered = renderYplotLabel(widgetUV.x, glyphUV_y, snapY, ySpacing, plotLeft, glyphW, glyphH, glyphBase0, glyphDot, glyphMinus, labelColor, color, screenScale);
                if (rendered.a > 0.01) { color = rendered.rgb; }
            }
        }

        // --- X axis labels (bottom margin) ---
        if (widgetUV.y < plotBottom && widgetUV.x >= plotLeft && widgetUV.x <= plotRight) {
            // Current X position in data coordinates
            let plotUVx = (widgetUV.x - plotLeft) / plotWidth;
            let dataX = mix(viewXMin, viewXMax, plotUVx);
            let snapX = round(dataX / xSpacing) * xSpacing;

            // Convert snapped X back to widget UV
            let snapPlotUVx = (snapX - viewXMin) / (viewXMax - viewXMin);
            let snapWidgetX = plotLeft + snapPlotUVx * plotWidth;

            // Render label centered on snapWidgetX
            var val = snapX;
            var hasSign = 0u;
            if (val < 0.0) { hasSign = 1u; val = -val; }

            var decimals = 0u;
            if (xSpacing < 0.01) { decimals = 3u; }
            else if (xSpacing < 0.1) { decimals = 2u; }
            else if (xSpacing < 1.0) { decimals = 1u; }

            let intPart = u32(floor(val));
            var intDigits = 1u;
            if (intPart >= 10u) { intDigits = 2u; }
            if (intPart >= 100u) { intDigits = 3u; }
            if (intPart >= 1000u) { intDigits = 4u; }
            let totalChars = hasSign + intDigits + select(0u, 1u + decimals, decimals > 0u);

            let labelLeft = snapWidgetX - f32(totalChars) * glyphW * 0.5;
            let labelRight = labelLeft + f32(totalChars) * glyphW;
            let labelTop = 0.003;
            let labelBot = labelTop + glyphH;

            if (widgetUV.x >= labelLeft && widgetUV.x < labelRight &&
                widgetUV.y >= labelTop && widgetUV.y < labelBot) {
                let charIdx = u32(floor((widgetUV.x - labelLeft) / glyphW));
                let charLocalUV = vec2<f32>(
                    fract((widgetUV.x - labelLeft) / glyphW),
                    (widgetUV.y - labelTop) / glyphH
                );

                let glyphIdx = getDigitGlyph(charIdx, snapX, decimals, intDigits, hasSign, glyphBase0, glyphDot, glyphMinus);
                if (glyphIdx > 0u) {
                    let r = renderMsdfGlyph(charLocalUV, glyphIdx, labelColor, color, grid.pixelRange, screenScale);
                    if (r.alpha > 0.01) { color = r.color; }
                }
            }
        }
    }

    return color;
}

// Helper: render Y-axis label at given position
fn renderYplotLabel(widgetX: f32, glyphUV_y: f32, dataVal: f32, spacing: f32, plotLeft: f32, glyphW: f32, glyphH: f32, glyphBase0: u32, glyphDot: u32, glyphMinus: u32, labelColor: vec3<f32>, bgColor: vec3<f32>, screenScale: f32) -> vec4<f32> {
    var val = dataVal;
    var hasSign = 0u;
    if (val < 0.0) { hasSign = 1u; val = -val; }

    var decimals = 0u;
    if (spacing < 0.01) { decimals = 3u; }
    else if (spacing < 0.1) { decimals = 2u; }
    else if (spacing < 1.0) { decimals = 1u; }

    let intPart = u32(floor(val));
    let fracPart = u32(round(fract(val) * pow(10.0, f32(decimals))));
    var intDigits = 1u;
    if (intPart >= 10u) { intDigits = 2u; }
    if (intPart >= 100u) { intDigits = 3u; }
    if (intPart >= 1000u) { intDigits = 4u; }
    let totalChars = hasSign + intDigits + select(0u, 1u + decimals, decimals > 0u);

    let labelRight = plotLeft - 0.003;
    let labelLeft = labelRight - f32(totalChars) * glyphW;

    if (widgetX >= labelLeft && widgetX < labelRight) {
        let charIdx = u32(floor((widgetX - labelLeft) / glyphW));
        let charLocalUV = vec2<f32>(
            fract((widgetX - labelLeft) / glyphW),
            glyphUV_y
        );

        let glyphIdx = getDigitGlyph(charIdx, dataVal, decimals, intDigits, hasSign, glyphBase0, glyphDot, glyphMinus);
        if (glyphIdx > 0u) {
            let r = renderMsdfGlyph(charLocalUV, glyphIdx, labelColor, bgColor, grid.pixelRange, screenScale);
            if (r.alpha > 0.01) {
                return vec4<f32>(r.color, 1.0);
            }
        }
    }
    return vec4<f32>(bgColor, 0.0);
}

// Helper: get glyph index for a digit at given position in a number
fn getDigitGlyph(charIdx: u32, dataVal: f32, decimals: u32, intDigits: u32, hasSign: u32, glyphBase0: u32, glyphDot: u32, glyphMinus: u32) -> u32 {
    var val = abs(dataVal);
    let intPart = u32(floor(val));
    let fracPart = u32(round(fract(val) * pow(10.0, f32(decimals))));
    let digitStart = hasSign;

    if (hasSign > 0u && charIdx == 0u) {
        return glyphMinus;
    } else if (charIdx >= digitStart && charIdx < digitStart + intDigits) {
        let digitPos = charIdx - digitStart;
        let divisor = u32(round(pow(10.0, f32(intDigits - 1u - digitPos))));
        return glyphBase0 + (intPart / divisor) % 10u;
    } else if (decimals > 0u && charIdx == digitStart + intDigits) {
        return glyphDot;
    } else if (decimals > 0u && charIdx > digitStart + intDigits) {
        let decPos = charIdx - digitStart - intDigits - 1u;
        let divisor = u32(round(pow(10.0, f32(decimals - 1u - decPos))));
        return glyphBase0 + (fracPart / divisor) % 10u;
    }
    return 0u;
}

// Draw grid lines
fn drawGrid(bg: vec3<f32>, plotUV: vec2<f32>, xMin: f32, xMax: f32, yMin: f32, yMax: f32) -> vec3<f32> {
    var color = bg;
    let gridSpacingX = 0.1;  // 10 divisions
    let gridSpacingY = 0.1;

    // Vertical grid lines
    let gridX = fract(plotUV.x / gridSpacingX);
    let lineWidthX = 0.01;
    if (gridX < lineWidthX || gridX > 1.0 - lineWidthX) {
        color = mix(color, YPLOT_GRID_COLOR, 0.5);
    }

    // Horizontal grid lines
    let gridY = fract(plotUV.y / gridSpacingY);
    let lineWidthY = 0.01;
    if (gridY < lineWidthY || gridY > 1.0 - lineWidthY) {
        color = mix(color, YPLOT_GRID_COLOR, 0.5);
    }

    return color;
}

// Draw axes
fn drawAxes(bg: vec3<f32>, plotUV: vec2<f32>, xMin: f32, xMax: f32, yMin: f32, yMax: f32) -> vec3<f32> {
    var color = bg;
    let lineWidth = 0.005;

    // Y-axis at x=0 (if visible)
    let xZero = (0.0 - xMin) / (xMax - xMin);
    if (xZero >= 0.0 && xZero <= 1.0) {
        if (abs(plotUV.x - xZero) < lineWidth) {
            color = YPLOT_AXIS_COLOR;
        }
    }

    // X-axis at y=0 (if visible)
    let yZero = (0.0 - yMin) / (yMax - yMin);
    if (yZero >= 0.0 && yZero <= 1.0) {
        if (abs(plotUV.y - yZero) < lineWidth) {
            color = YPLOT_AXIS_COLOR;
        }
    }

    // Border
    if (plotUV.x < lineWidth || plotUV.x > 1.0 - lineWidth ||
        plotUV.y < lineWidth || plotUV.y > 1.0 - lineWidth) {
        color = YPLOT_AXIS_COLOR * 0.7;
    }

    return color;
}

// Unpack color from u32 ARGB (yplot uses ARGB format for function colors)
fn yplot_unpackColorARGB(packed: u32) -> vec3<f32> {
    let r = f32((packed >> 16u) & 0xFFu) / 255.0;
    let g = f32((packed >> 8u) & 0xFFu) / 255.0;
    let b = f32(packed & 0xFFu) / 255.0;
    return vec3<f32>(r, g, b);
}
