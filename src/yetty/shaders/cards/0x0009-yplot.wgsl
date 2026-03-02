// Shader glyph: YPlot widget (codepoint 1048585 / U+100009)
// GPU-accelerated plot rendering using yfsvm bytecode interpreter
//
// =============================================================================
// Design: Composes yfsvm for expression evaluation with built-in rendering
// =============================================================================
//
// This shader:
// 1. Reads yplot metadata from cardMetadata
// 2. Reads yfsvm bytecode from cardStorage
// 3. For each pixel in the plot area, evaluates expressions via yfsvm_execute()
// 4. Renders grid, axes, and plot lines
//
// =============================================================================
// Metadata layout (64 bytes = 16 u32s):
//   [0]  flags(8) | funcCount(8) | pad(16)
//   [1]  widthCells(16) | heightCells(16)
//   [2]  bytecodeOffset (u32)
//   [3]  bytecodeSize (u32)
//   [4]  xMin (f32)
//   [5]  xMax (f32)
//   [6]  yMin (f32)
//   [7]  yMax (f32)
//   [8]  marginLeft (f32)
//   [9]  marginBottom (f32)
//   [10] plotWidth (f32)
//   [11] plotHeight (f32)
//   [12] time (f32)
//   [13] zoom (f32)
//   [14] centerX (f32)
//   [15] colorTableOffset (u32)
// =============================================================================

const YPLOT_FLAG_GRID: u32 = 1u;
const YPLOT_FLAG_AXES: u32 = 2u;
const YPLOT_FLAG_LABELS: u32 = 4u;

// Colors
const YPLOT_BG_COLOR: vec3<f32> = vec3<f32>(0.102, 0.102, 0.180);  // #1A1A2E
const YPLOT_GRID_COLOR: vec3<f32> = vec3<f32>(0.267, 0.267, 0.267);  // #444444
const YPLOT_AXIS_COLOR: vec3<f32> = vec3<f32>(0.667, 0.667, 0.667);  // #AAAAAA

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

    // Detect if buffer is working: slot0 should have flags (lower 8 bits non-zero)
    let bufferWorking = (slot0 & 0xFFu) != 0u;

    // Use buffer values if working, otherwise use hardcoded defaults
    var flags = 7u;
    var funcCount = 1u;
    var widthCells = 1u;
    var heightCells = 1u;
    var bytecodeOffset = 0u;
    var bytecodeSize = 24u;
    var xMin = -3.14159;
    var xMax = 3.14159;
    var yMin = -1.5;
    var yMax = 1.5;
    var marginLeft = 0.08;
    var marginBottom = 0.08;
    var plotWidth = 0.9;
    var plotHeight = 0.9;
    var currentTime = time;
    var zoom = 1.0;
    var centerX = 0.0;
    var colorTableOffset = 6u;

    if (bufferWorking) {
        flags = slot0 & 0xFFu;
        funcCount = (slot0 >> 8u) & 0xFFu;
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
        marginLeft = bitcast<f32>(cardMetadata[metaOffset + 8u]);
        marginBottom = bitcast<f32>(cardMetadata[metaOffset + 9u]);
        plotWidth = bitcast<f32>(cardMetadata[metaOffset + 10u]);
        plotHeight = bitcast<f32>(cardMetadata[metaOffset + 11u]);
        currentTime = bitcast<f32>(cardMetadata[metaOffset + 12u]);
        zoom = bitcast<f32>(cardMetadata[metaOffset + 13u]);
        centerX = bitcast<f32>(cardMetadata[metaOffset + 14u]);
        colorTableOffset = cardMetadata[metaOffset + 15u];
    }

    // Compute widget-wide UV (0-1 across entire widget)
    let widgetUV = (vec2<f32>(f32(relCol), f32(relRow)) + localUV) /
                   vec2<f32>(f32(widthCells), f32(heightCells));

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

        // Map to data coordinates
        let dataX = mix(xMin, xMax, plotUV.x);
        let dataY = mix(yMin, yMax, plotUV.y);

        // Draw grid
        if ((flags & YPLOT_FLAG_GRID) != 0u) {
            color = drawGrid(color, plotUV, xMin, xMax, yMin, yMax);
        }

        // Draw axes
        if ((flags & YPLOT_FLAG_AXES) != 0u) {
            color = drawAxes(color, plotUV, xMin, xMax, yMin, yMax);
        }

        // Evaluate and draw plot lines using yfsvm
        if (funcCount > 0u && bytecodeSize > 0u) {
            var samplers: array<f32, 8>;
            samplers[0] = dataX;

            // Pixel size in data coordinates for anti-aliasing
            let pixelSizeY = (yMax - yMin) / (f32(heightCells) * 20.0);

            // Evaluate each function
            for (var fi = 0u; fi < funcCount && fi < 8u; fi = fi + 1u) {
                // Get function color from color table
                let funcColorPacked = bitcast<u32>(cardStorage[colorTableOffset + fi]);
                let funcColor = yplot_unpackColorARGB(funcColorPacked);

                // Evaluate function at this x position
                let y = yfsvm_execute(bytecodeOffset, fi, dataX, currentTime, samplers);

                // Map y to plot UV
                let yNorm = (y - yMin) / (yMax - yMin);

                // Distance from current pixel to the curve
                let dist = abs(plotUV.y - yNorm);

                // Anti-aliased line rendering
                let lineWidth = 2.0 * pixelSizeY / (yMax - yMin);
                if (dist < lineWidth) {
                    let alpha = 1.0 - dist / lineWidth;
                    color = mix(color, funcColor, alpha);
                }
            }
        }
    } else {
        // Outside plot area - margin for labels
        // TODO: Render axis labels here using text
        color = YPLOT_BG_COLOR * 0.8;
    }

    return color;
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
