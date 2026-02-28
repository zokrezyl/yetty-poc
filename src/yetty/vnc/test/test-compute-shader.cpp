// Unit test for VNC tile diff compute shader
#include <webgpu/webgpu.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <atomic>

static const char* DIFF_SHADER = R"(
@group(0) @binding(0) var currTex: texture_2d<f32>;
@group(0) @binding(1) var prevTex: texture_2d<f32>;
@group(0) @binding(2) var<storage, read_write> dirtyFlags: array<u32>;

const TILE_SIZE: u32 = 64;
const PIXELS_PER_THREAD: u32 = 8;  // 64/8 = 8 pixels per thread per dimension

@compute @workgroup_size(8, 8)
fn main(@builtin(local_invocation_id) lid: vec3<u32>,
        @builtin(workgroup_id) wgid: vec3<u32>) {
    let dims = textureDimensions(currTex);
    let tilesX = (dims.x + TILE_SIZE - 1u) / TILE_SIZE;
    let tileIdx = wgid.y * tilesX + wgid.x;
    let tileStartX = wgid.x * TILE_SIZE;
    let tileStartY = wgid.y * TILE_SIZE;

    // Each thread checks an 8x8 region of pixels within the tile
    let regionStartX = tileStartX + lid.x * PIXELS_PER_THREAD;
    let regionStartY = tileStartY + lid.y * PIXELS_PER_THREAD;

    for (var dy: u32 = 0u; dy < PIXELS_PER_THREAD; dy++) {
        for (var dx: u32 = 0u; dx < PIXELS_PER_THREAD; dx++) {
            let px = regionStartX + dx;
            let py = regionStartY + dy;

            if (px >= dims.x || py >= dims.y) {
                continue;
            }

            let curr = textureLoad(currTex, vec2<u32>(px, py), 0);
            let prev = textureLoad(prevTex, vec2<u32>(px, py), 0);

            if (any(curr != prev)) {
                dirtyFlags[tileIdx] = 1u;
                return;  // Early exit once we find a difference
            }
        }
    }
}
)";

int main() {
    printf("=== Compute Shader Unit Test ===\n");

    // Create WebGPU instance
    WGPUInstanceDescriptor instanceDesc = {};
    WGPUInstance instance = wgpuCreateInstance(&instanceDesc);
    if (!instance) {
        printf("FAIL: Could not create WebGPU instance\n");
        return 1;
    }

    // Request adapter
    WGPUAdapter adapter = nullptr;
    WGPURequestAdapterOptions adapterOpts = {};
    WGPURequestAdapterCallbackInfo adapterCbInfo = {};
    adapterCbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    adapterCbInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter a, WGPUStringView, void* ud, void*) {
        if (status == WGPURequestAdapterStatus_Success) {
            *static_cast<WGPUAdapter*>(ud) = a;
        }
    };
    adapterCbInfo.userdata1 = &adapter;
    wgpuInstanceRequestAdapter(instance, &adapterOpts, adapterCbInfo);
    if (!adapter) {
        printf("FAIL: Could not get adapter\n");
        return 1;
    }

    // Request device
    WGPUDevice device = nullptr;
    WGPUDeviceDescriptor deviceDesc = {};
    WGPURequestDeviceCallbackInfo deviceCbInfo = {};
    deviceCbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceCbInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice d, WGPUStringView, void* ud, void*) {
        if (status == WGPURequestDeviceStatus_Success) {
            *static_cast<WGPUDevice*>(ud) = d;
        }
    };
    deviceCbInfo.userdata1 = &device;
    wgpuAdapterRequestDevice(adapter, &deviceDesc, deviceCbInfo);
    if (!device) {
        printf("FAIL: Could not get device\n");
        return 1;
    }

    WGPUQueue queue = wgpuDeviceGetQueue(device);
    printf("Device and queue created\n");

    // Test parameters
    const uint32_t WIDTH = 128;
    const uint32_t HEIGHT = 128;
    const uint32_t TILE_SIZE = 64;
    const uint32_t TILES_X = (WIDTH + TILE_SIZE - 1) / TILE_SIZE;  // 2
    const uint32_t TILES_Y = (HEIGHT + TILE_SIZE - 1) / TILE_SIZE; // 2
    const uint32_t NUM_TILES = TILES_X * TILES_Y; // 4

    printf("Test: %ux%u pixels, %ux%u tiles = %u total\n", WIDTH, HEIGHT, TILES_X, TILES_Y, NUM_TILES);

    // Create textures
    WGPUTextureDescriptor texDesc = {};
    texDesc.size = {WIDTH, HEIGHT, 1};
    texDesc.format = WGPUTextureFormat_BGRA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;

    WGPUTexture currTex = wgpuDeviceCreateTexture(device, &texDesc);
    WGPUTexture prevTex = wgpuDeviceCreateTexture(device, &texDesc);
    if (!currTex || !prevTex) {
        printf("FAIL: Could not create textures\n");
        return 1;
    }
    printf("Textures created\n");

    // Create dirty flags buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = NUM_TILES * sizeof(uint32_t);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc;
    WGPUBuffer dirtyFlagsBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);

    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    WGPUBuffer readbackBuffer = wgpuDeviceCreateBuffer(device, &bufDesc);

    if (!dirtyFlagsBuffer || !readbackBuffer) {
        printf("FAIL: Could not create buffers\n");
        return 1;
    }
    printf("Buffers created\n");

    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = {DIFF_SHADER, strlen(DIFF_SHADER)};
    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(device, &shaderDesc);
    if (!shader) {
        printf("FAIL: Could not create shader module\n");
        return 1;
    }
    printf("Shader module created\n");

    // Create bind group layout
    WGPUBindGroupLayoutEntry entries[3] = {};
    entries[0].binding = 0;
    entries[0].visibility = WGPUShaderStage_Compute;
    entries[0].texture.sampleType = WGPUTextureSampleType_Float;
    entries[0].texture.viewDimension = WGPUTextureViewDimension_2D;

    entries[1].binding = 1;
    entries[1].visibility = WGPUShaderStage_Compute;
    entries[1].texture.sampleType = WGPUTextureSampleType_Float;
    entries[1].texture.viewDimension = WGPUTextureViewDimension_2D;

    entries[2].binding = 2;
    entries[2].visibility = WGPUShaderStage_Compute;
    entries[2].buffer.type = WGPUBufferBindingType_Storage;
    entries[2].buffer.minBindingSize = NUM_TILES * sizeof(uint32_t);

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 3;
    bglDesc.entries = entries;
    WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &plDesc);

    // Compute pipeline
    WGPUComputePipelineDescriptor cpDesc = {};
    cpDesc.layout = pipelineLayout;
    cpDesc.compute.module = shader;
    cpDesc.compute.entryPoint = {.data = "main", .length = 4};
    WGPUComputePipeline pipeline = wgpuDeviceCreateComputePipeline(device, &cpDesc);

    if (!pipeline) {
        printf("FAIL: Could not create compute pipeline\n");
        return 1;
    }
    printf("Compute pipeline created\n");

    // Create pixel data - prevTex all black, currTex with one changed pixel in tile (0,0)
    std::vector<uint8_t> prevPixels(WIDTH * HEIGHT * 4, 0);
    std::vector<uint8_t> currPixels(WIDTH * HEIGHT * 4, 0);

    // Make them identical first (all black with alpha=255)
    for (size_t i = 0; i < prevPixels.size(); i += 4) {
        prevPixels[i+3] = 255;  // Alpha
        currPixels[i+3] = 255;
    }

    // Change ONE pixel at position (32, 32) in tile (0,0) - OUTSIDE the 8x8 region checked by shader
    // This exposes the bug: shader only checks 8x8 pixels per 64x64 tile
    uint32_t pixelX = 32;
    uint32_t pixelY = 32;
    uint32_t pixelIdx = (pixelY * WIDTH + pixelX) * 4;
    currPixels[pixelIdx + 0] = 0;    // B
    currPixels[pixelIdx + 1] = 0;    // G
    currPixels[pixelIdx + 2] = 255;  // R
    currPixels[pixelIdx + 3] = 255;  // A

    printf("Changed pixel at (%u, %u) - index %u\n", pixelX, pixelY, pixelIdx);
    printf("Pixel data: prev=%u,%u,%u,%u curr=%u,%u,%u,%u\n",
        prevPixels[pixelIdx], prevPixels[pixelIdx+1], prevPixels[pixelIdx+2], prevPixels[pixelIdx+3],
        currPixels[pixelIdx], currPixels[pixelIdx+1], currPixels[pixelIdx+2], currPixels[pixelIdx+3]);

    // Upload textures
    WGPUTexelCopyTextureInfo dst = {};
    dst.texture = prevTex;
    WGPUTexelCopyBufferLayout layout = {};
    layout.bytesPerRow = WIDTH * 4;
    layout.rowsPerImage = HEIGHT;
    WGPUExtent3D extent = {WIDTH, HEIGHT, 1};
    wgpuQueueWriteTexture(queue, &dst, prevPixels.data(), prevPixels.size(), &layout, &extent);

    dst.texture = currTex;
    wgpuQueueWriteTexture(queue, &dst, currPixels.data(), currPixels.size(), &layout, &extent);
    printf("Textures uploaded\n");

    // Create bind group
    WGPUTextureView currView = wgpuTextureCreateView(currTex, nullptr);
    WGPUTextureView prevView = wgpuTextureCreateView(prevTex, nullptr);

    WGPUBindGroupEntry bgEntries[3] = {};
    bgEntries[0].binding = 0;
    bgEntries[0].textureView = currView;
    bgEntries[1].binding = 1;
    bgEntries[1].textureView = prevView;
    bgEntries[2].binding = 2;
    bgEntries[2].buffer = dirtyFlagsBuffer;
    bgEntries[2].offset = 0;
    bgEntries[2].size = NUM_TILES * sizeof(uint32_t);

    printf("Buffer bind: buffer=%p offset=%zu size=%zu\n",
        (void*)dirtyFlagsBuffer, (size_t)bgEntries[2].offset, (size_t)bgEntries[2].size);

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bindGroupLayout;
    bgDesc.entryCount = 3;
    bgDesc.entries = bgEntries;
    WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(device, &bgDesc);
    printf("Bind group created\n");

    // Clear buffer using separate command submission
    {
        WGPUCommandEncoder clearEnc = wgpuDeviceCreateCommandEncoder(device, nullptr);
        wgpuCommandEncoderClearBuffer(clearEnc, dirtyFlagsBuffer, 0, NUM_TILES * sizeof(uint32_t));
        WGPUCommandBuffer clearCmd = wgpuCommandEncoderFinish(clearEnc, nullptr);
        wgpuQueueSubmit(queue, 1, &clearCmd);
        wgpuCommandBufferRelease(clearCmd);
        wgpuCommandEncoderRelease(clearEnc);

        // Wait for clear to complete
        std::atomic<bool> clearDone{false};
        WGPUQueueWorkDoneCallbackInfo clearCb = {};
        clearCb.mode = WGPUCallbackMode_AllowSpontaneous;
        clearCb.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud, void*) {
            *static_cast<std::atomic<bool>*>(ud) = true;
        };
        clearCb.userdata1 = &clearDone;
        wgpuQueueOnSubmittedWorkDone(queue, clearCb);
        while (!clearDone) wgpuDeviceTick(device);
        printf("Buffer cleared\n");
    }

    // Run compute shader
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, nullptr);

    // Dispatch compute
    WGPUComputePassEncoder cpass = wgpuCommandEncoderBeginComputePass(encoder, nullptr);
    wgpuComputePassEncoderSetPipeline(cpass, pipeline);
    wgpuComputePassEncoderSetBindGroup(cpass, 0, bindGroup, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(cpass, TILES_X, TILES_Y, 1);
    wgpuComputePassEncoderEnd(cpass);
    wgpuComputePassEncoderRelease(cpass);

    // Copy to readback
    wgpuCommandEncoderCopyBufferToBuffer(encoder, dirtyFlagsBuffer, 0, readbackBuffer, 0, NUM_TILES * sizeof(uint32_t));

    WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, nullptr);
    wgpuQueueSubmit(queue, 1, &cmdBuf);
    wgpuCommandBufferRelease(cmdBuf);
    wgpuCommandEncoderRelease(encoder);
    printf("Compute dispatched: %u x %u workgroups\n", TILES_X, TILES_Y);

    // Wait for queue
    std::atomic<bool> done{false};
    WGPUQueueWorkDoneCallbackInfo cbInfo = {};
    cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
    cbInfo.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud, void*) {
        *static_cast<std::atomic<bool>*>(ud) = true;
    };
    cbInfo.userdata1 = &done;
    wgpuQueueOnSubmittedWorkDone(queue, cbInfo);
    while (!done) wgpuDeviceTick(device);
    printf("Queue work done\n");

    // Map and read
    done = false;
    WGPUBufferMapCallbackInfo mapCb = {};
    mapCb.mode = WGPUCallbackMode_AllowSpontaneous;
    mapCb.callback = [](WGPUMapAsyncStatus, WGPUStringView, void* ud, void*) {
        *static_cast<std::atomic<bool>*>(ud) = true;
    };
    mapCb.userdata1 = &done;
    wgpuBufferMapAsync(readbackBuffer, WGPUMapMode_Read, 0, NUM_TILES * sizeof(uint32_t), mapCb);
    while (!done) wgpuDeviceTick(device);

    const uint32_t* flags = static_cast<const uint32_t*>(
        wgpuBufferGetConstMappedRange(readbackBuffer, 0, NUM_TILES * sizeof(uint32_t)));

    printf("\n=== RESULTS ===\n");
    for (uint32_t i = 0; i < NUM_TILES; i++) {
        printf("Tile %u: %s (flag=%u)\n", i, flags[i] ? "DIRTY" : "clean", flags[i]);
    }

    // Expected: tile 0 should be dirty (we changed pixel at 0,0)
    bool pass = (flags[0] == 1 && flags[1] == 0 && flags[2] == 0 && flags[3] == 0);
    printf("\nTEST %s\n", pass ? "PASSED" : "FAILED");

    wgpuBufferUnmap(readbackBuffer);

    // Cleanup
    wgpuBindGroupRelease(bindGroup);
    wgpuTextureViewRelease(currView);
    wgpuTextureViewRelease(prevView);
    wgpuComputePipelineRelease(pipeline);
    wgpuPipelineLayoutRelease(pipelineLayout);
    wgpuBindGroupLayoutRelease(bindGroupLayout);
    wgpuShaderModuleRelease(shader);
    wgpuBufferRelease(readbackBuffer);
    wgpuBufferRelease(dirtyFlagsBuffer);
    wgpuTextureRelease(currTex);
    wgpuTextureRelease(prevTex);
    wgpuQueueRelease(queue);
    wgpuDeviceRelease(device);
    wgpuAdapterRelease(adapter);
    wgpuInstanceRelease(instance);

    return pass ? 0 : 1;
}
