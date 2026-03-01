// Minimal test: render colored cells using yetty's shader approach
#include <emscripten.h>
#include <webgpu/webgpu.h>
#include <cstdio>
#include <cstring>
#include <cstdint>

WGPUDevice g_device = nullptr;
WGPUQueue g_queue = nullptr;
WGPUSurface g_surface = nullptr;
WGPURenderPipeline g_pipeline = nullptr;
WGPUBindGroup g_bindGroup = nullptr;
WGPUBuffer g_cellBuffer = nullptr;
WGPUBuffer g_uniformBuffer = nullptr;
WGPUTextureFormat g_format = WGPUTextureFormat_BGRA8Unorm;

const int COLS = 80;
const int ROWS = 24;

struct Cell {
    uint32_t glyph;
    uint8_t fgR, fgG, fgB;
    uint8_t bgR, bgG, bgB;
    uint8_t alpha;
    uint8_t style;
};

struct Uniforms {
    float screenWidth, screenHeight;
    float cellWidth, cellHeight;
    float cols, rows;
    float pad1, pad2;
};

// Simple shader - just fills cells with their bg color
const char* shaderCode = R"(
struct Uniforms {
    screenSize: vec2f,
    cellSize: vec2f,
    gridSize: vec2f,
    pad: vec2f,
};

struct Cell {
    glyph: u32,
    fgR: u32, fgG: u32, fgB: u32,
    bgR: u32, bgG: u32, bgB: u32,
    alpha: u32,
    style: u32,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var<storage, read> cells: array<Cell>;

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) cellCoord: vec2f,
};

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    let cellIdx = idx / 6u;
    let vertIdx = idx % 6u;
    
    let col = f32(cellIdx % u32(uniforms.gridSize.x));
    let row = f32(cellIdx / u32(uniforms.gridSize.x));
    
    var offsets = array<vec2f, 6>(
        vec2f(0.0, 0.0), vec2f(1.0, 0.0), vec2f(1.0, 1.0),
        vec2f(0.0, 0.0), vec2f(1.0, 1.0), vec2f(0.0, 1.0)
    );
    
    let cellPos = vec2f(col, row) + offsets[vertIdx];
    let pixelPos = cellPos * uniforms.cellSize;
    let ndcPos = (pixelPos / uniforms.screenSize) * 2.0 - 1.0;
    
    var out: VertexOutput;
    out.position = vec4f(ndcPos.x, -ndcPos.y, 0.0, 1.0);
    out.cellCoord = vec2f(col, row);
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    let col = u32(in.cellCoord.x);
    let row = u32(in.cellCoord.y);
    let idx = row * u32(uniforms.gridSize.x) + col;
    let cell = cells[idx];
    
    // If glyph is set, use fg color; otherwise bg
    if (cell.glyph != 0u && cell.glyph != 32u) {
        return vec4f(f32(cell.fgR)/255.0, f32(cell.fgG)/255.0, f32(cell.fgB)/255.0, 1.0);
    }
    return vec4f(f32(cell.bgR)/255.0, f32(cell.bgG)/255.0, f32(cell.bgB)/255.0, 1.0);
}
)";

Cell g_cells[COLS * ROWS];

void fillCells() {
    const char* text = "Hello from WebGPU Cell Test!";
    int len = strlen(text);
    
    for (int i = 0; i < COLS * ROWS; i++) {
        g_cells[i].glyph = ' ';
        g_cells[i].fgR = 200; g_cells[i].fgG = 200; g_cells[i].fgB = 200;
        g_cells[i].bgR = 20; g_cells[i].bgG = 20; g_cells[i].bgB = 40;
        g_cells[i].alpha = 255;
        g_cells[i].style = 0;
    }
    
    // Write text on row 5
    for (int i = 0; i < len && i < COLS; i++) {
        g_cells[5 * COLS + i + 5].glyph = text[i];
        g_cells[5 * COLS + i + 5].fgR = 255;
        g_cells[5 * COLS + i + 5].fgG = 255;
        g_cells[5 * COLS + i + 5].fgB = 0;
    }
    
    // Red block on row 10
    for (int i = 10; i < 20; i++) {
        g_cells[10 * COLS + i].glyph = '#';
        g_cells[10 * COLS + i].fgR = 255;
        g_cells[10 * COLS + i].fgG = 0;
        g_cells[10 * COLS + i].fgB = 0;
    }
    
    printf("Cells filled: %d total, text='%s'\n", COLS * ROWS, text);
}

void render() {
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(g_surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal) return;

    WGPUTextureView view = wgpuTextureCreateView(surfaceTexture.texture, nullptr);
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(g_device, nullptr);

    WGPURenderPassColorAttachment colorAtt = {};
    colorAtt.view = view;
    colorAtt.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    colorAtt.loadOp = WGPULoadOp_Clear;
    colorAtt.storeOp = WGPUStoreOp_Store;
    colorAtt.clearValue = {0.05, 0.05, 0.1, 1.0};

    WGPURenderPassDescriptor passDesc = {};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAtt;

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &passDesc);
    wgpuRenderPassEncoderSetPipeline(pass, g_pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, g_bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, COLS * ROWS * 6, 1, 0, 0);
    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, nullptr);
    wgpuQueueSubmit(g_queue, 1, &cmd);

    wgpuCommandBufferRelease(cmd);
    wgpuCommandEncoderRelease(encoder);
    wgpuRenderPassEncoderRelease(pass);
    wgpuTextureViewRelease(view);
    wgpuTextureRelease(surfaceTexture.texture);
}

void mainLoop(void*) { render(); }

void initPipeline() {
    // Create shader
    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code = {shaderCode, strlen(shaderCode)};
    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslSource.chain;
    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(g_device, &shaderDesc);
    printf("Shader created: %p\n", shader);

    // Create uniform buffer
    Uniforms uniforms = {800, 600, 10, 25, (float)COLS, (float)ROWS, 0, 0};
    WGPUBufferDescriptor ubDesc = {};
    ubDesc.size = sizeof(Uniforms);
    ubDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    g_uniformBuffer = wgpuDeviceCreateBuffer(g_device, &ubDesc);
    wgpuQueueWriteBuffer(g_queue, g_uniformBuffer, 0, &uniforms, sizeof(uniforms));

    // Create cell buffer
    fillCells();
    WGPUBufferDescriptor cbDesc = {};
    cbDesc.size = sizeof(g_cells);
    cbDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    g_cellBuffer = wgpuDeviceCreateBuffer(g_device, &cbDesc);
    wgpuQueueWriteBuffer(g_queue, g_cellBuffer, 0, g_cells, sizeof(g_cells));
    printf("Cell buffer created, size=%zu\n", sizeof(g_cells));

    // Create bind group layout
    WGPUBindGroupLayoutEntry entries[2] = {};
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    entries[0].buffer.type = WGPUBufferBindingType_Uniform;
    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    entries[1].buffer.type = WGPUBufferBindingType_ReadOnlyStorage;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 2;
    bglDesc.entries = entries;
    WGPUBindGroupLayout bgl = wgpuDeviceCreateBindGroupLayout(g_device, &bglDesc);

    // Create bind group
    WGPUBindGroupEntry bgEntries[2] = {};
    bgEntries[0].binding = 0;
    bgEntries[0].buffer = g_uniformBuffer;
    bgEntries[0].size = sizeof(Uniforms);
    bgEntries[1].binding = 1;
    bgEntries[1].buffer = g_cellBuffer;
    bgEntries[1].size = sizeof(g_cells);

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bgl;
    bgDesc.entryCount = 2;
    bgDesc.entries = bgEntries;
    g_bindGroup = wgpuDeviceCreateBindGroup(g_device, &bgDesc);
    printf("Bind group created: %p\n", g_bindGroup);

    // Create pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &bgl;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(g_device, &plDesc);

    // Create pipeline
    WGPUColorTargetState colorTarget = {};
    colorTarget.format = g_format;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragState = {};
    fragState.module = shader;
    fragState.entryPoint = {"fs_main", 7};
    fragState.targetCount = 1;
    fragState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;
    pipelineDesc.vertex.module = shader;
    pipelineDesc.vertex.entryPoint = {"vs_main", 7};
    pipelineDesc.fragment = &fragState;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = 0xFFFFFFFF;

    g_pipeline = wgpuDeviceCreateRenderPipeline(g_device, &pipelineDesc);
    printf("Pipeline created: %p\n", g_pipeline);

    emscripten_set_main_loop_arg(mainLoop, nullptr, 0, false);
}

void onDeviceReady(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView, void*, void*) {
    if (status != WGPURequestDeviceStatus_Success) return;
    g_device = device;
    g_queue = wgpuDeviceGetQueue(device);
    printf("Device ready\n");

    WGPUSurfaceConfiguration config = {};
    config.device = g_device;
    config.format = g_format;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.width = 800;
    config.height = 600;
    config.presentMode = WGPUPresentMode_Fifo;
    config.alphaMode = WGPUCompositeAlphaMode_Opaque;
    wgpuSurfaceConfigure(g_surface, &config);

    initPipeline();
}

void onAdapterReady(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView, void*, void*) {
    if (status != WGPURequestAdapterStatus_Success) return;
    printf("Adapter ready\n");
    
    WGPUSurfaceCapabilities caps = {};
    wgpuSurfaceGetCapabilities(g_surface, adapter, &caps);
    if (caps.formatCount > 0) g_format = caps.formats[0];

    WGPURequestDeviceCallbackInfo cb = {};
    cb.mode = WGPUCallbackMode_AllowSpontaneous;
    cb.callback = onDeviceReady;
    wgpuAdapterRequestDevice(adapter, nullptr, cb);
}

int main() {
    printf("Cell Test Starting...\n");
    WGPUInstance instance = wgpuCreateInstance(nullptr);

    WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource = {};
    canvasSource.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
    canvasSource.selector = {.data = "#canvas", .length = 7};
    WGPUSurfaceDescriptor surfaceDesc = {};
    surfaceDesc.nextInChain = &canvasSource.chain;
    g_surface = wgpuInstanceCreateSurface(instance, &surfaceDesc);

    WGPURequestAdapterOptions opts = {};
    opts.compatibleSurface = g_surface;
    WGPURequestAdapterCallbackInfo cb = {};
    cb.mode = WGPUCallbackMode_AllowSpontaneous;
    cb.callback = onAdapterReady;
    wgpuInstanceRequestAdapter(instance, &opts, cb);

    return 0;
}
