/**
 * MSDFGL WebGPU Example - Terminal Grid Rendering
 *
 * Renders a terminal-like grid using MSDF text rendering.
 * Uses real Unicode codepoints as glyph indices.
 */

#include <msdfgl.h>
#include <yetty/wgpu-compat.h>
#include <yetty/msdf-glyph-data.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <atomic>

// Window dimensions
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

// Grid dimensions
constexpr int GRID_COLS = 120;
constexpr int GRID_ROWS = 40;

// WebGPU globals
WGPUInstance instance = nullptr;
WGPUSurface surface = nullptr;
WGPUAdapter adapter = nullptr;
WGPUDevice device = nullptr;
WGPUQueue queue = nullptr;
WGPUTextureFormat surfaceFormat = WGPUTextureFormat_BGRA8Unorm;

// MSDFGL context and font
std::unique_ptr<msdfgl::Context> msdfContext;
std::unique_ptr<msdfgl::Font> font;

// Rendering resources
WGPURenderPipeline renderPipeline = nullptr;
WGPUBindGroup bindGroup = nullptr;
WGPUBuffer uniformBuffer = nullptr;
WGPUBuffer cellBuffer = nullptr;
WGPUBuffer glyphMetadataBuffer = nullptr;
WGPUSampler fontSampler = nullptr;

bool adapterReady = false;
bool deviceReady = false;

// Cell structure (12 bytes, matches shader)
struct Cell {
    uint32_t glyph;  // Unicode codepoint
    uint32_t fg;     // Packed RGBA
    uint32_t bg;     // Packed RGB + attrs
};

// Grid uniforms (matches shader)
struct GridUniforms {
    float projection[16];     // 64 bytes
    float screenWidth;        // 4 bytes
    float screenHeight;       // 4 bytes
    float cellWidth;          // 4 bytes
    float cellHeight;         // 4 bytes
    float gridCols;           // 4 bytes
    float gridRows;           // 4 bytes
    float pixelRange;         // 4 bytes
    float scale;              // 4 bytes
    float atlasWidth;         // 4 bytes
    float atlasHeight;        // 4 bytes
    float _pad[2];            // 8 bytes padding
};

// Glyph metadata for shader (40 bytes, matches GlyphMetadataGPU)
struct GlyphMetadata {
    float uvMinX, uvMinY;     // 8 bytes
    float uvMaxX, uvMaxY;     // 8 bytes
    float sizeX, sizeY;       // 8 bytes
    float bearingX, bearingY; // 8 bytes
    float advance;            // 4 bytes
    float _pad;               // 4 bytes
};

// Map from codepoint to glyph metadata
std::map<uint32_t, GlyphMetadata> glyphMap;
std::vector<GlyphMetadata> glyphMetadataArray;
uint32_t maxCodepoint = 0;

// Terminal content
std::vector<Cell> cells;

// Simple MSDF shader - debug version
const char* shaderSource = R"(
struct GridUniforms {
    projection: mat4x4<f32>,
    screenSize: vec2<f32>,
    cellSize: vec2<f32>,
    gridSize: vec2<f32>,
    pixelRange: f32,
    scale: f32,
    atlasSize: vec2<f32>,
    _pad: vec2<f32>,
};

struct GlyphMetadata {
    uvMin: vec2<f32>,
    uvMax: vec2<f32>,
    size: vec2<f32>,
    bearing: vec2<f32>,
    advance: f32,
    _pad: f32,
};

struct Cell {
    glyph: u32,
    fg: u32,
    bg: u32,
};

@group(0) @binding(0) var<uniform> grid: GridUniforms;
@group(0) @binding(1) var fontTexture: texture_2d<f32>;
@group(0) @binding(2) var fontSampler: sampler;
@group(0) @binding(3) var<storage, read> glyphMetadata: array<GlyphMetadata>;
@group(0) @binding(4) var<storage, read> cellBuffer: array<Cell>;

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) vertexIndex: u32) -> VertexOutput {
    // Full-screen triangle
    var positions = array<vec2<f32>, 3>(
        vec2<f32>(-1.0, -1.0),
        vec2<f32>(3.0, -1.0),
        vec2<f32>(-1.0, 3.0)
    );
    var output: VertexOutput;
    output.position = vec4<f32>(positions[vertexIndex], 0.0, 1.0);
    return output;
}

fn median(r: f32, g: f32, b: f32) -> f32 {
    return max(min(r, g), min(max(r, g), b));
}

fn unpackColor(packed: u32) -> vec3<f32> {
    return vec3<f32>(
        f32(packed & 0xFFu) / 255.0,
        f32((packed >> 8u) & 0xFFu) / 255.0,
        f32((packed >> 16u) & 0xFFu) / 255.0
    );
}

@fragment
fn fs_main(input: VertexOutput) -> @location(0) vec4<f32> {
    let pixelPos = input.position.xy;

    // DEBUG: Just show a gradient to verify rendering works
    // return vec4<f32>(pixelPos.x / 1280.0, pixelPos.y / 720.0, 0.5, 1.0);

    // Grid bounds
    let gridPixelWidth = grid.gridSize.x * grid.cellSize.x;
    let gridPixelHeight = grid.gridSize.y * grid.cellSize.y;

    // Outside grid - dark background
    if (pixelPos.x >= gridPixelWidth || pixelPos.y >= gridPixelHeight) {
        return vec4<f32>(0.05, 0.05, 0.05, 1.0);
    }

    // Calculate cell position
    let cellCol = floor(pixelPos.x / grid.cellSize.x);
    let cellRow = floor(pixelPos.y / grid.cellSize.y);
    let cellIndex = u32(cellRow) * u32(grid.gridSize.x) + u32(cellCol);

    // Get cell data
    let cell = cellBuffer[cellIndex];
    let glyphIndex = cell.glyph;
    let fgColor = unpackColor(cell.fg);
    let bgColor = unpackColor(cell.bg);

    // Position within cell
    let localPx = vec2<f32>(
        pixelPos.x - cellCol * grid.cellSize.x,
        pixelPos.y - cellRow * grid.cellSize.y
    );

    // Start with background
    var finalColor = bgColor;

    // Render glyph if valid
    if (glyphIndex > 32u && glyphIndex < arrayLength(&glyphMetadata)) {
        let glyph = glyphMetadata[glyphIndex];

        // Check if glyph has valid size (non-zero)
        if (glyph.size.x > 0.0 && glyph.size.y > 0.0) {
            // Calculate glyph position within cell
            let scaledSize = glyph.size * grid.scale;
            let scaledBearing = glyph.bearing * grid.scale;

            // Baseline at 80% of cell height
            let baseline = grid.cellSize.y * 0.8;
            let glyphTop = baseline - scaledBearing.y;
            let glyphLeft = scaledBearing.x;

            // Glyph bounds
            let glyphMin = vec2<f32>(glyphLeft, glyphTop);
            let glyphMax = vec2<f32>(glyphLeft + scaledSize.x, glyphTop + scaledSize.y);

            // Check if inside glyph bounds
            if (localPx.x >= glyphMin.x && localPx.x < glyphMax.x &&
                localPx.y >= glyphMin.y && localPx.y < glyphMax.y) {
                // Calculate UV
                let glyphLocalPos = (localPx - glyphMin) / scaledSize;
                let uv = mix(glyph.uvMin, glyph.uvMax, glyphLocalPos);

                // Sample MSDF texture
                let msdf = textureSample(fontTexture, fontSampler, uv);
                let sd = median(msdf.r, msdf.g, msdf.b);

                // Anti-aliased edge
                let screenPxRange = grid.pixelRange * grid.scale;
                let alpha = clamp((sd - 0.5) * screenPxRange + 0.5, 0.0, 1.0);

                // Blend foreground over background
                finalColor = mix(bgColor, fgColor, alpha);
            }
        }
    }

    return vec4<f32>(finalColor, 1.0);
}
)";

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter result,
                    WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestAdapterStatus_Success) {
        adapter = result;
    }
    adapterReady = true;
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice result,
                   WGPUStringView message, void* userdata1, void* userdata2) {
    if (status == WGPURequestDeviceStatus_Success) {
        device = result;
    }
    deviceReady = true;
}

bool initWebGPU(GLFWwindow* window) {
    std::cout << "[WebGPU] Initializing..." << std::endl;

    WGPUInstanceDescriptor instanceDesc = {};
    instance = wgpuCreateInstance(&instanceDesc);
    if (!instance) return false;

    surface = glfwCreateWindowWGPUSurface(instance, window);
    if (!surface) return false;

    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.compatibleSurface = surface;
    adapterOpts.powerPreference = WGPUPowerPreference_HighPerformance;

    WGPURequestAdapterCallbackInfo adapterCallback = {};
    adapterCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    adapterCallback.callback = onAdapterReady;

    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCallback);
    while (!adapterReady) wgpuInstanceProcessEvents(instance);
    if (!adapter) return false;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.label = WGPU_STR("msdfgl-example");

    // Add error callback
    deviceDesc.uncapturedErrorCallbackInfo.callback = [](WGPUDevice const* dev, WGPUErrorType type, WGPUStringView message, void* userdata1, void* userdata2) {
        std::cerr << "[WebGPU ERROR] Type=" << static_cast<int>(type);
        if (message.data && message.length > 0) {
            std::cerr << " " << std::string(message.data, message.length);
        }
        std::cerr << std::endl;
    };

    WGPURequestDeviceCallbackInfo deviceCallback = {};
    deviceCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    deviceCallback.callback = onDeviceReady;

    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCallback);
    while (!deviceReady) wgpuInstanceProcessEvents(instance);
    if (!device) return false;

    queue = wgpuDeviceGetQueue(device);

    // Configure surface
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(surface, adapter, &caps);
    if (caps.formatCount > 0) surfaceFormat = caps.formats[0];

    WGPUSurfaceConfiguration surfaceConfig = {};
    surfaceConfig.device = device;
    surfaceConfig.format = surfaceFormat;
    surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
    surfaceConfig.width = WINDOW_WIDTH;
    surfaceConfig.height = WINDOW_HEIGHT;
    surfaceConfig.presentMode = WGPUPresentMode_Fifo;
    surfaceConfig.alphaMode = WGPUCompositeAlphaMode_Auto;

    wgpuSurfaceConfigure(surface, &surfaceConfig);
    wgpuSurfaceCapabilitiesFreeMembers(caps);

    std::cout << "[WebGPU] Initialized" << std::endl;
    return true;
}

bool initMSDFGL(const char* fontPath) {
    std::cout << "[MSDFGL] Loading font: " << fontPath << std::endl;

    msdfgl::ContextConfig config;
    config.dpiX = 96.0f;
    config.dpiY = 96.0f;

    try {
        msdfContext = std::make_unique<msdfgl::Context>(device, config);
    } catch (const std::exception& e) {
        std::cerr << "[MSDFGL] Failed: " << e.what() << std::endl;
        return false;
    }

    msdfgl::FontConfig fontConfig;
    fontConfig.range = 4.0f;
    fontConfig.scale = 2.0f;

    font = msdfContext->loadFont(fontPath, fontConfig);
    if (!font) return false;

    std::cout << "[MSDFGL] Font loaded" << std::endl;
    return true;
}

bool generateAtlas() {
    std::cout << "[ATLAS] Generating glyphs..." << std::endl;

    auto startTime = std::chrono::high_resolution_clock::now();

    // Generate ASCII + extended Latin + box drawing
    std::vector<uint32_t> charset;

    // ASCII printable
    for (uint32_t c = 0x20; c <= 0x7E; ++c) charset.push_back(c);
    // Latin-1 Supplement
    for (uint32_t c = 0xA0; c <= 0xFF; ++c) charset.push_back(c);
    // Box drawing
    for (uint32_t c = 0x2500; c <= 0x257F; ++c) charset.push_back(c);
    // Block elements
    for (uint32_t c = 0x2580; c <= 0x259F; ++c) charset.push_back(c);

    size_t successCount = 0;
    for (uint32_t cp : charset) {
        if (font->generateGlyph(cp) > 0) {
            successCount++;
            if (cp > maxCodepoint) maxCodepoint = cp;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(endTime - startTime).count();

    auto atlas = font->getAtlas();
    std::cout << "[ATLAS] Generated " << successCount << " glyphs in "
              << std::fixed << std::setprecision(2) << elapsed << "s" << std::endl;
    std::cout << "[ATLAS] Size: " << atlas->getTextureWidth() << "x"
              << atlas->getTextureHeight() << std::endl;

    return true;
}

bool buildGlyphMetadata() {
    std::cout << "[METADATA] Building glyph metadata..." << std::endl;

    auto atlas = font->getAtlas();
    if (!atlas) return false;

    float atlasW = static_cast<float>(atlas->getTextureWidth());
    float atlasH = static_cast<float>(atlas->getTextureHeight());

    // Resize to hold all codepoints up to maxCodepoint
    glyphMetadataArray.resize(maxCodepoint + 1);
    memset(glyphMetadataArray.data(), 0, glyphMetadataArray.size() * sizeof(GlyphMetadata));

    // Read index buffer to get glyph positions
    WGPUBuffer indexBuffer = atlas->getIndexBuffer();
    size_t glyphCount = atlas->getGlyphCount();
    size_t indexBufferSize = glyphCount * sizeof(msdfgl::IndexEntry);

    // Create staging buffer
    WGPUBufferDescriptor stagingDesc = {};
    stagingDesc.size = indexBufferSize;
    stagingDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    WGPUBuffer stagingBuffer = wgpuDeviceCreateBuffer(device, &stagingDesc);

    // Copy
    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encDesc);
    wgpuCommandEncoderCopyBufferToBuffer(encoder, indexBuffer, 0, stagingBuffer, 0, indexBufferSize);
    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);
    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);

    // Wait and map
    std::atomic<bool> done{false};
    WGPUQueueWorkDoneCallbackInfo workDoneCallback = {};
    workDoneCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    workDoneCallback.callback = [](WGPUQueueWorkDoneStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        *static_cast<std::atomic<bool>*>(userdata1) = true;
    };
    workDoneCallback.userdata1 = &done;
    wgpuQueueOnSubmittedWorkDone(queue, workDoneCallback);
    while (!done) wgpuInstanceProcessEvents(instance);

    std::atomic<bool> mapDone{false};
    WGPUBufferMapCallbackInfo mapCallback = {};
    mapCallback.mode = WGPUCallbackMode_AllowProcessEvents;
    mapCallback.callback = [](WGPUMapAsyncStatus status, WGPUStringView message, void* userdata1, void* userdata2) {
        *static_cast<std::atomic<bool>*>(userdata1) = true;
    };
    mapCallback.userdata1 = &mapDone;
    wgpuBufferMapAsync(stagingBuffer, WGPUMapMode_Read, 0, indexBufferSize, mapCallback);
    while (!mapDone) wgpuInstanceProcessEvents(instance);

    const msdfgl::IndexEntry* indexData = static_cast<const msdfgl::IndexEntry*>(
        wgpuBufferGetConstMappedRange(stagingBuffer, 0, indexBufferSize));

    if (!indexData) {
        std::cerr << "[METADATA] ERROR: Failed to get mapped range!" << std::endl;
        wgpuBufferRelease(stagingBuffer);
        return false;
    }

    // Debug: print first few index entries
    std::cout << "[METADATA] Index buffer has " << glyphCount << " entries" << std::endl;
    for (size_t i = 0; i < std::min(size_t(5), glyphCount); i++) {
        const msdfgl::IndexEntry& e = indexData[i];
        std::cout << "[METADATA] IndexEntry[" << i << "]: offset=[" << e.offsetX << "," << e.offsetY
                  << "] size=[" << e.sizeX << "x" << e.sizeY << "]" << std::endl;
    }

    // Build metadata for each generated glyph
    int foundCount = 0;
    for (uint32_t cp = 0; cp <= maxCodepoint; ++cp) {
        msdfgl::MapItem* item = font->getGlyph(static_cast<int32_t>(cp));
        if (!item) continue;
        if (item->index < 0) continue;
        if (static_cast<size_t>(item->index) >= glyphCount) continue;

        const msdfgl::IndexEntry& entry = indexData[item->index];

        GlyphMetadata& meta = glyphMetadataArray[cp];
        meta.uvMinX = entry.offsetX / atlasW;
        meta.uvMinY = entry.offsetY / atlasH;
        meta.uvMaxX = (entry.offsetX + entry.sizeX) / atlasW;
        meta.uvMaxY = (entry.offsetY + entry.sizeY) / atlasH;
        meta.sizeX = entry.sizeX;
        meta.sizeY = entry.sizeY;
        meta.bearingX = entry.bearingX / 64.0f;
        meta.bearingY = entry.bearingY / 64.0f;
        meta.advance = item->advance[0] / 64.0f;
        foundCount++;
    }
    std::cout << "[METADATA] Found " << foundCount << " glyphs in map" << std::endl;

    wgpuBufferUnmap(stagingBuffer);
    wgpuBufferRelease(stagingBuffer);

    std::cout << "[METADATA] Built metadata for " << (maxCodepoint + 1) << " codepoints" << std::endl;

    // Debug: print a few glyph entries
    for (uint32_t cp : {'A', 'H', 'e', 'l', 'o'}) {
        const GlyphMetadata& m = glyphMetadataArray[cp];
        std::cout << "[DEBUG] Glyph '" << (char)cp << "' (U+" << std::hex << cp << std::dec << "): "
                  << "uv=[" << m.uvMinX << "," << m.uvMinY << "]-[" << m.uvMaxX << "," << m.uvMaxY << "] "
                  << "size=[" << m.sizeX << "x" << m.sizeY << "] "
                  << "bearing=[" << m.bearingX << "," << m.bearingY << "]" << std::endl;
    }

    return true;
}

void fillTerminalContent() {
    cells.resize(GRID_COLS * GRID_ROWS);

    // Color palette
    auto packColor = [](uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) -> uint32_t {
        return r | (g << 8) | (b << 16) | (a << 24);
    };

    uint32_t black = packColor(0, 0, 0);
    uint32_t white = packColor(255, 255, 255);
    uint32_t green = packColor(0, 255, 0);
    uint32_t cyan = packColor(0, 255, 255);
    uint32_t yellow = packColor(255, 255, 0);
    uint32_t gray = packColor(128, 128, 128);
    uint32_t darkGray = packColor(30, 30, 30);

    // Fill with spaces
    for (auto& cell : cells) {
        cell.glyph = ' ';
        cell.fg = white;
        cell.bg = darkGray;
    }

    // Helper to write text
    auto writeText = [&](int row, int col, const char* text, uint32_t fg, uint32_t bg) {
        int i = 0;
        while (text[i] && col + i < GRID_COLS) {
            int idx = row * GRID_COLS + col + i;
            cells[idx].glyph = static_cast<uint32_t>(static_cast<unsigned char>(text[i]));
            cells[idx].fg = fg;
            cells[idx].bg = bg;
            i++;
        }
    };

    // Header
    writeText(0, 0, " MSDFGL WebGPU Terminal Example ", yellow, packColor(0, 0, 128));

    // Box drawing demo
    writeText(2, 2, "Box Drawing:", cyan, darkGray);
    // Top border
    cells[3 * GRID_COLS + 2].glyph = 0x250C;  // ┌
    for (int i = 3; i < 20; i++) cells[3 * GRID_COLS + i].glyph = 0x2500;  // ─
    cells[3 * GRID_COLS + 20].glyph = 0x2510;  // ┐
    // Sides
    for (int row = 4; row < 8; row++) {
        cells[row * GRID_COLS + 2].glyph = 0x2502;  // │
        cells[row * GRID_COLS + 20].glyph = 0x2502;  // │
    }
    // Bottom border
    cells[8 * GRID_COLS + 2].glyph = 0x2514;  // └
    for (int i = 3; i < 20; i++) cells[8 * GRID_COLS + i].glyph = 0x2500;  // ─
    cells[8 * GRID_COLS + 20].glyph = 0x2518;  // ┘
    // Content inside box
    writeText(5, 4, "Hello, World!", green, darkGray);
    writeText(6, 4, "MSDF Rendering", white, darkGray);

    // ASCII table
    writeText(10, 2, "ASCII Characters:", cyan, darkGray);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 16; j++) {
            int ch = 33 + i * 16 + j;
            if (ch < 127) {
                int idx = (12 + i) * GRID_COLS + 2 + j * 2;
                cells[idx].glyph = ch;
                cells[idx].fg = (i % 2 == 0) ? green : cyan;
            }
        }
    }

    // Progress bar with block elements
    writeText(20, 2, "Progress:", cyan, darkGray);
    for (int i = 0; i < 20; i++) {
        int idx = 20 * GRID_COLS + 12 + i;
        if (i < 15) {
            cells[idx].glyph = 0x2588;  // █ Full block
            cells[idx].fg = green;
        } else {
            cells[idx].glyph = 0x2591;  // ░ Light shade
            cells[idx].fg = gray;
        }
    }
    writeText(20, 33, "75%", white, darkGray);

    // Some sample text
    writeText(22, 2, "The quick brown fox jumps over the lazy dog.", white, darkGray);
    writeText(23, 2, "0123456789 !@#$%^&*() []{}|\\;:'\",.<>/?", gray, darkGray);

    // Latin extended
    writeText(25, 2, "Latin: ", cyan, darkGray);
    writeText(25, 9, "\xC0\xC1\xC2\xC3\xC4\xC5 \xC8\xC9\xCA\xCB \xD1 \xD6 \xDC \xDF", white, darkGray);

    // Status line
    for (int i = 0; i < GRID_COLS; i++) {
        cells[(GRID_ROWS - 1) * GRID_COLS + i].bg = packColor(0, 0, 80);
    }
    writeText(GRID_ROWS - 1, 2, "Press ESC to exit", white, packColor(0, 0, 80));
}

bool initRendering() {
    std::cout << "[RENDER] Initializing rendering pipeline..." << std::endl;

    auto atlas = font->getAtlas();
    if (!atlas) return false;

    // Create shader module
    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code = WGPU_STR(shaderSource);

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgslSource);
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(device, &shaderDesc);

    // Create uniform buffer
    WGPUBufferDescriptor uniformBufDesc = {};
    uniformBufDesc.size = sizeof(GridUniforms);
    uniformBufDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    uniformBuffer = wgpuDeviceCreateBuffer(device, &uniformBufDesc);

    // Create cell buffer
    WGPUBufferDescriptor cellBufDesc = {};
    cellBufDesc.size = cells.size() * sizeof(Cell);
    cellBufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    cellBuffer = wgpuDeviceCreateBuffer(device, &cellBufDesc);

    // Upload cells
    wgpuQueueWriteBuffer(queue, cellBuffer, 0, cells.data(), cells.size() * sizeof(Cell));

    // Create glyph metadata buffer
    WGPUBufferDescriptor metaBufDesc = {};
    metaBufDesc.size = glyphMetadataArray.size() * sizeof(GlyphMetadata);
    metaBufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    glyphMetadataBuffer = wgpuDeviceCreateBuffer(device, &metaBufDesc);

    // Upload glyph metadata
    wgpuQueueWriteBuffer(queue, glyphMetadataBuffer, 0,
                         glyphMetadataArray.data(),
                         glyphMetadataArray.size() * sizeof(GlyphMetadata));

    // Create sampler (non-filtering for RGBA32Float texture)
    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.magFilter = WGPUFilterMode_Nearest;
    samplerDesc.minFilter = WGPUFilterMode_Nearest;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Nearest;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;
    fontSampler = wgpuDeviceCreateSampler(device, &samplerDesc);

    // Create bind group layout
    WGPUBindGroupLayoutEntry layoutEntries[5] = {};

    // Uniform buffer
    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Fragment;
    layoutEntries[0].buffer.type = WGPUBufferBindingType_Uniform;

    // Font texture (RGBA32Float is unfilterable)
    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Fragment;
    layoutEntries[1].texture.sampleType = WGPUTextureSampleType_UnfilterableFloat;
    layoutEntries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

    // Sampler (non-filtering for unfilterable texture)
    layoutEntries[2].binding = 2;
    layoutEntries[2].visibility = WGPUShaderStage_Fragment;
    layoutEntries[2].sampler.type = WGPUSamplerBindingType_NonFiltering;

    // Glyph metadata
    layoutEntries[3].binding = 3;
    layoutEntries[3].visibility = WGPUShaderStage_Fragment;
    layoutEntries[3].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    // Cell buffer
    layoutEntries[4].binding = 4;
    layoutEntries[4].visibility = WGPUShaderStage_Fragment;
    layoutEntries[4].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor layoutDesc = {};
    layoutDesc.entryCount = 5;
    layoutDesc.entries = layoutEntries;
    WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &layoutDesc);

    // Create bind group
    WGPUBindGroupEntry bgEntries[5] = {};

    bgEntries[0].binding = 0;
    bgEntries[0].buffer = uniformBuffer;
    bgEntries[0].size = sizeof(GridUniforms);

    bgEntries[1].binding = 1;
    bgEntries[1].textureView = atlas->getAtlasTextureView();

    bgEntries[2].binding = 2;
    bgEntries[2].sampler = fontSampler;

    bgEntries[3].binding = 3;
    bgEntries[3].buffer = glyphMetadataBuffer;
    bgEntries[3].size = glyphMetadataArray.size() * sizeof(GlyphMetadata);

    bgEntries[4].binding = 4;
    bgEntries[4].buffer = cellBuffer;
    bgEntries[4].size = cells.size() * sizeof(Cell);

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bindGroupLayout;
    bgDesc.entryCount = 5;
    bgDesc.entries = bgEntries;
    bindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor pipelineLayoutDesc = {};
    pipelineLayoutDesc.bindGroupLayoutCount = 1;
    pipelineLayoutDesc.bindGroupLayouts = &bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &pipelineLayoutDesc);

    // Create render pipeline
    WGPUColorTargetState colorTarget = {};
    colorTarget.format = surfaceFormat;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragmentState = {};
    fragmentState.module = shaderModule;
    fragmentState.entryPoint = WGPU_STR("fs_main");
    fragmentState.targetCount = 1;
    fragmentState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shaderModule;
    pipelineDesc.vertex.entryPoint = WGPU_STR("vs_main");
    pipelineDesc.fragment = &fragmentState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;

    renderPipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

    wgpuShaderModuleRelease(shaderModule);
    wgpuBindGroupLayoutRelease(bindGroupLayout);
    wgpuPipelineLayoutRelease(pipelineLayout);

    std::cout << "[RENDER] Pipeline initialized" << std::endl;
    return true;
}

void updateUniforms() {
    auto atlas = font->getAtlas();

    GridUniforms uniforms = {};

    // Identity projection
    memset(uniforms.projection, 0, sizeof(uniforms.projection));
    uniforms.projection[0] = 1.0f;
    uniforms.projection[5] = 1.0f;
    uniforms.projection[10] = 1.0f;
    uniforms.projection[15] = 1.0f;

    uniforms.screenWidth = WINDOW_WIDTH;
    uniforms.screenHeight = WINDOW_HEIGHT;

    // Calculate cell size based on window and grid
    uniforms.cellWidth = static_cast<float>(WINDOW_WIDTH) / GRID_COLS;
    uniforms.cellHeight = static_cast<float>(WINDOW_HEIGHT) / GRID_ROWS;

    uniforms.gridCols = GRID_COLS;
    uniforms.gridRows = GRID_ROWS;
    uniforms.pixelRange = 4.0f;
    uniforms.scale = uniforms.cellHeight / 32.0f;  // Scale to fit cell
    uniforms.atlasWidth = atlas ? static_cast<float>(atlas->getTextureWidth()) : 1.0f;
    uniforms.atlasHeight = atlas ? static_cast<float>(atlas->getTextureHeight()) : 1.0f;

    wgpuQueueWriteBuffer(queue, uniformBuffer, 0, &uniforms, sizeof(uniforms));
}

void render() {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);

    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        return;
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = surfaceFormat;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.baseMipLevel = 0;
    viewDesc.mipLevelCount = 1;
    viewDesc.baseArrayLayer = 0;
    viewDesc.arrayLayerCount = 1;
    viewDesc.aspect = WGPUTextureAspect_All;
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDesc);

    WGPUCommandEncoderDescriptor encDesc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encDesc);

    WGPURenderPassColorAttachment colorAttachment = {};
    colorAttachment.view = targetView;
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.0, 0.0, 0.0, 1.0};

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    wgpuRenderPassEncoderSetPipeline(pass, renderPipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 3, 1, 0, 0);  // Full-screen triangle
    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);

    WGPUCommandBufferDescriptor cmdBufDesc = {};
    WGPUCommandBuffer cmdBuffer = wgpuCommandEncoderFinish(encoder, &cmdBufDesc);
    wgpuQueueSubmit(queue, 1, &cmdBuffer);

    wgpuCommandBufferRelease(cmdBuffer);
    wgpuCommandEncoderRelease(encoder);
    wgpuTextureViewRelease(targetView);

    wgpuSurfacePresent(surface);
}

void cleanup() {
    if (bindGroup) wgpuBindGroupRelease(bindGroup);
    if (renderPipeline) wgpuRenderPipelineRelease(renderPipeline);
    if (uniformBuffer) wgpuBufferRelease(uniformBuffer);
    if (cellBuffer) wgpuBufferRelease(cellBuffer);
    if (glyphMetadataBuffer) wgpuBufferRelease(glyphMetadataBuffer);
    if (fontSampler) wgpuSamplerRelease(fontSampler);

    font.reset();
    msdfContext.reset();

    if (queue) wgpuQueueRelease(queue);
    if (device) wgpuDeviceRelease(device);
    if (adapter) wgpuAdapterRelease(adapter);
    if (surface) wgpuSurfaceRelease(surface);
    if (instance) wgpuInstanceRelease(instance);
}

int main(int argc, char* argv[]) {
    std::string fontPath = "assets/DejaVuSansMNerdFontMono-Regular.ttf";

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--font" && i + 1 < argc) {
            fontPath = argv[++i];
        }
    }

    std::cout << "========================================" << std::endl;
    std::cout << "MSDFGL WebGPU Terminal Example" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Font: " << fontPath << std::endl;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "MSDFGL Terminal", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    if (!initWebGPU(window)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!initMSDFGL(fontPath.c_str())) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!generateAtlas()) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    if (!buildGlyphMetadata()) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    fillTerminalContent();

    if (!initRendering()) {
        cleanup();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    updateUniforms();

    std::cout << "\n[READY] Rendering... Press ESC to exit\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        render();
    }

    cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
