#include "vnc-server.h"
#include <ytrace/ytrace.hpp>
#include <turbojpeg.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>
#include <chrono>
#include <uv.h>  // Use uv_default_loop() - same as yetty's EventLoop

namespace yetty::vnc {

using yetty::Result;
using yetty::Ok;
using yetty::Err;

VncServer::VncServer(WGPUDevice device, WGPUQueue queue)
    : _device(device), _queue(queue) {
    _jpegCompressor = tjInitCompress();

    // Initialize libuv async handle using the same default loop as yetty's EventLoop
    auto* async = new uv_async_t();
    async->data = this;
    uv_async_init(uv_default_loop(), async, [](uv_async_t* handle) {
        // This callback runs on the main thread when GPU work completes
        // The VncServer will check _gpuWorkDone flag on next sendFrame call
        auto* self = static_cast<VncServer*>(handle->data);
        (void)self;  // Flag is already set by GPU callback
    });
    _uvAsync = async;
}

// Compute shader for tile diff detection
// Each workgroup handles one tile (64x64), 8x8 threads each check 8x8 pixels = full coverage
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

VncServer::~VncServer() {
    stop();

    // Close libuv async handle
    if (_uvAsync) {
        auto* async = static_cast<uv_async_t*>(_uvAsync);
        uv_close(reinterpret_cast<uv_handle_t*>(async), [](uv_handle_t* h) {
            delete reinterpret_cast<uv_async_t*>(h);
        });
        _uvAsync = nullptr;
    }

    if (_prevTexture) wgpuTextureRelease(_prevTexture);
    if (_dirtyFlagsBuffer) wgpuBufferRelease(_dirtyFlagsBuffer);
    if (_dirtyFlagsReadback) wgpuBufferRelease(_dirtyFlagsReadback);
    if (_tileReadbackBuffer) wgpuBufferRelease(_tileReadbackBuffer);
    if (_diffPipeline) wgpuComputePipelineRelease(_diffPipeline);
    if (_diffBindGroup) wgpuBindGroupRelease(_diffBindGroup);
    if (_diffBindGroupLayout) wgpuBindGroupLayoutRelease(_diffBindGroupLayout);
    if (_jpegCompressor) tjDestroy(static_cast<tjhandle>(_jpegCompressor));
}

Result<void> VncServer::start(uint16_t port) {
    if (_running) return Err<void>("Server already running");

    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0) return Err<void>("Failed to create socket");

    int opt = 1;
    setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(_serverFd);
        _serverFd = -1;
        return Err<void>("Failed to bind to port " + std::to_string(port));
    }

    if (listen(_serverFd, 5) < 0) {
        close(_serverFd);
        _serverFd = -1;
        return Err<void>("Failed to listen");
    }

    _port = port;
    _running = true;
    _acceptThread = std::thread(&VncServer::acceptLoop, this);

    yinfo("VNC server listening on port {}", port);
    return Ok();
}

void VncServer::stop() {
    _running = false;

    if (_serverFd >= 0) {
        shutdown(_serverFd, SHUT_RDWR);
        close(_serverFd);
        _serverFd = -1;
    }

    // Close all client connections
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (int fd : _clients) {
            shutdown(fd, SHUT_RDWR);
            close(fd);
        }
        _clients.clear();
        _clientCount = 0;
    }

    if (_acceptThread.joinable()) {
        _acceptThread.join();
    }

    _clientInputBuffers.clear();
}

void VncServer::acceptLoop() {
    while (_running) {
        struct pollfd pfd = {_serverFd, POLLIN, 0};
        int ret = poll(&pfd, 1, 100);  // 100ms timeout
        if (ret <= 0) continue;

        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientFd = accept(_serverFd, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0) continue;

        // Disable Nagle
        int flag = 1;
        setsockopt(clientFd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

        // Keep socket blocking - we use MSG_DONTWAIT for non-blocking input reads

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
        yinfo("VNC client connected from {}", clientIp);

        {
            std::lock_guard<std::mutex> lock(_clientsMutex);
            _clients.push_back(clientFd);
            _clientCount = _clients.size();
            _clientInputBuffers[clientFd] = ClientInputBuffer{};
            _forceFullFrame = true;  // Send full frame to new client
        }
    }
}

Result<void> VncServer::sendToClient(int clientFd, const void* data, size_t size) {
    const uint8_t* ptr = static_cast<const uint8_t*>(data);
    size_t remaining = size;
    int retries = 0;
    while (remaining > 0) {
        ssize_t sent = send(clientFd, ptr, remaining, MSG_NOSIGNAL);
        if (sent <= 0) {
            if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                // Buffer full, wait a bit and retry
                if (++retries > 100) {
                    ywarn("VNC: send timeout after {} retries", retries);
                    return Err<void>("Send timeout");
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            return Err<void>("Send failed");
        }
        ptr += sent;
        remaining -= sent;
        retries = 0;
    }
    return Ok();
}

Result<void> VncServer::ensureResources(uint32_t width, uint32_t height) {
    if (_lastWidth == width && _lastHeight == height && _prevTexture) {
        return Ok();
    }

    // Release old resources
    if (_prevTexture) { wgpuTextureRelease(_prevTexture); _prevTexture = nullptr; }
    if (_dirtyFlagsBuffer) { wgpuBufferRelease(_dirtyFlagsBuffer); _dirtyFlagsBuffer = nullptr; }
    if (_dirtyFlagsReadback) { wgpuBufferRelease(_dirtyFlagsReadback); _dirtyFlagsReadback = nullptr; }
    if (_diffBindGroup) { wgpuBindGroupRelease(_diffBindGroup); _diffBindGroup = nullptr; }
    if (_tileReadbackBuffer) { wgpuBufferRelease(_tileReadbackBuffer); _tileReadbackBuffer = nullptr; _tileReadbackBufferSize = 0; }

    _lastWidth = width;
    _lastHeight = height;
    _tilesX = tiles_x(width);
    _tilesY = tiles_y(height);
    _dirtyTiles.resize(_tilesX * _tilesY);

    // Create previous frame texture (for comparison)
    WGPUTextureDescriptor texDesc = {};
    texDesc.size = {width, height, 1};
    texDesc.format = WGPUTextureFormat_BGRA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;
    _prevTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_prevTexture) return Err<void>("Failed to create prev texture");

    // Create dirty flags buffer (1 uint32 per tile)
    // Needs Storage for compute shader, CopySrc to copy to readback, CopyDst for ClearBuffer
    uint32_t numTiles = _tilesX * _tilesY;
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = numTiles * sizeof(uint32_t);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc | WGPUBufferUsage_CopyDst;
    _dirtyFlagsBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_dirtyFlagsBuffer) return Err<void>("Failed to create dirty flags buffer");

    // Create readback buffer for dirty flags
    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    _dirtyFlagsReadback = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_dirtyFlagsReadback) return Err<void>("Failed to create dirty flags readback");

    // Create compute pipeline if needed
    if (!_diffPipeline) {
        if (auto res = createDiffPipeline(); !res) return res;
    }

    yinfo("VNC server resources: {}x{}, {} tiles", width, height, numTiles);
    return Ok();
}

Result<void> VncServer::createDiffPipeline() {
    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = {DIFF_SHADER, strlen(DIFF_SHADER)};
    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;
    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!shader) return Err<void>("Failed to create diff shader");

    // Bind group layout: currTex, prevTex, dirtyFlags
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

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 3;
    bglDesc.entries = entries;
    _diffBindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &_diffBindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &plDesc);

    // Compute pipeline
    WGPUComputePipelineDescriptor cpDesc = {};
    cpDesc.layout = pipelineLayout;
    cpDesc.compute.module = shader;
    cpDesc.compute.entryPoint = {.data = "main", .length = 4};

    _diffPipeline = wgpuDeviceCreateComputePipeline(_device, &cpDesc);

    wgpuShaderModuleRelease(shader);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!_diffPipeline) return Err<void>("Failed to create diff pipeline");
    return Ok();
}

Result<void> VncServer::encodeTile(uint16_t tx, uint16_t ty,
                                    std::vector<uint8_t>& outData, Encoding& outEncoding) {
    const uint8_t* pixels = _cpuPixels ? _cpuPixels : _gpuReadbackPixels.data();
    if (!pixels || (_cpuPixels == nullptr && _gpuReadbackPixels.empty())) {
        return Err<void>("No pixels for encoding");
    }

    uint32_t startX = tx * TILE_SIZE;
    uint32_t startY = ty * TILE_SIZE;
    uint32_t tileW = std::min((uint32_t)TILE_SIZE, _lastWidth - startX);
    uint32_t tileH = std::min((uint32_t)TILE_SIZE, _lastHeight - startY);

    // Extract tile pixels from framebuffer
    std::vector<uint8_t> tilePixels(TILE_SIZE * TILE_SIZE * 4);
    uint32_t srcStride = _lastWidth * 4;
    for (uint32_t y = 0; y < tileH; y++) {
        const uint8_t* src = pixels + (startY + y) * srcStride + startX * 4;
        uint8_t* dst = tilePixels.data() + y * TILE_SIZE * 4;
        std::memcpy(dst, src, tileW * 4);
    }

    uint32_t rawSize = TILE_SIZE * TILE_SIZE * 4;

    // Try JPEG compression
    unsigned char* jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int result = tjCompress2(
        static_cast<tjhandle>(_jpegCompressor),
        tilePixels.data(),
        TILE_SIZE, 0, TILE_SIZE,
        TJPF_BGRA,
        &jpegBuf, &jpegSize,
        TJSAMP_420, 80,
        TJFLAG_FASTDCT
    );

    if (result == 0 && jpegSize < rawSize * 0.8) {
        outData.assign(jpegBuf, jpegBuf + jpegSize);
        outEncoding = Encoding::JPEG;
        tjFree(jpegBuf);
    } else {
        if (jpegBuf) tjFree(jpegBuf);
        outData = std::move(tilePixels);
        outEncoding = Encoding::RAW;
    }

    return Ok();
}

Result<void> VncServer::sendFrame(WGPUTexture texture, const uint8_t* cpuPixels, uint32_t width, uint32_t height) {
    if (_clientCount == 0) return Ok();  // No clients, skip

    _cpuPixels = cpuPixels;
    _cpuPixelsSize = width * height * 4;

    // If no CPU pixels provided, prepare to read back from GPU
    if (!cpuPixels) {
        _gpuReadbackPixels.resize(width * height * 4);
    }

    // Single tick to process any pending GPU callbacks - no busy wait!
    wgpuDeviceTick(_device);

    // If dimensions changed, reset state machine to re-create resources
    if (width != _lastWidth || height != _lastHeight) {
        ydebug("VNC sendFrame: size changed {}x{} -> {}x{}, resetting state",
               _lastWidth, _lastHeight, width, height);
        _captureState = CaptureState::IDLE;
        _gpuWorkDone = true;
    }

    ydebug("VNC sendFrame: state={} gpuWorkDone={}",
           static_cast<int>(_captureState), _gpuWorkDone.load());

    // State machine for async GPU operations
    switch (_captureState) {
        case CaptureState::IDLE: {
            if (auto res = ensureResources(width, height); !res) {
                return res;
            }

            // Check for full refresh
            _framesSinceFullRefresh++;
            if (_framesSinceFullRefresh >= FULL_REFRESH_INTERVAL) {
                _forceFullFrame = true;
                _framesSinceFullRefresh = 0;
            }

            WGPUExtent3D extent = {width, height, 1};

            if (_forceFullFrame) {
                // Skip GPU diff, mark all dirty
                std::fill(_dirtyTiles.begin(), _dirtyTiles.end(), true);
                _forceFullFrame = false;

                // Copy current to prev for next frame comparison
                WGPUCommandEncoder copyEnc = wgpuDeviceCreateCommandEncoder(_device, nullptr);
                WGPUTexelCopyTextureInfo srcCopy = {};
                srcCopy.texture = texture;
                WGPUTexelCopyTextureInfo dstCopy = {};
                dstCopy.texture = _prevTexture;
                wgpuCommandEncoderCopyTextureToTexture(copyEnc, &srcCopy, &dstCopy, &extent);
                WGPUCommandBuffer copyCmd = wgpuCommandEncoderFinish(copyEnc, nullptr);
                wgpuQueueSubmit(_queue, 1, &copyCmd);
                wgpuCommandBufferRelease(copyCmd);
                wgpuCommandEncoderRelease(copyEnc);

                _captureState = CaptureState::READY_TO_SEND;
                break;
            }

            // Store texture for async processing
            _pendingTexture = texture;

            // Create bind group for this frame
            if (_diffBindGroup) wgpuBindGroupRelease(_diffBindGroup);

            WGPUTextureView currView = wgpuTextureCreateView(texture, nullptr);
            WGPUTextureView prevView = wgpuTextureCreateView(_prevTexture, nullptr);

            WGPUBindGroupEntry entries[3] = {};
            entries[0].binding = 0;
            entries[0].textureView = currView;
            entries[1].binding = 1;
            entries[1].textureView = prevView;
            entries[2].binding = 2;
            entries[2].buffer = _dirtyFlagsBuffer;
            entries[2].size = _tilesX * _tilesY * sizeof(uint32_t);

            WGPUBindGroupDescriptor bgDesc = {};
            bgDesc.layout = _diffBindGroupLayout;
            bgDesc.entryCount = 3;
            bgDesc.entries = entries;
            _diffBindGroup = wgpuDeviceCreateBindGroup(_device, &bgDesc);

            wgpuTextureViewRelease(currView);
            wgpuTextureViewRelease(prevView);

            // Clear dirty flags buffer in separate submission (required for Dawn)
            {
                WGPUCommandEncoder clearEnc = wgpuDeviceCreateCommandEncoder(_device, nullptr);
                wgpuCommandEncoderClearBuffer(clearEnc, _dirtyFlagsBuffer, 0, _tilesX * _tilesY * sizeof(uint32_t));
                WGPUCommandBuffer clearCmd = wgpuCommandEncoderFinish(clearEnc, nullptr);
                wgpuQueueSubmit(_queue, 1, &clearCmd);
                wgpuCommandBufferRelease(clearCmd);
                wgpuCommandEncoderRelease(clearEnc);
            }

            // Register async callback for clear completion
            _gpuWorkDone = false;
            WGPUQueueWorkDoneCallbackInfo clearCb = {};
            clearCb.mode = WGPUCallbackMode_AllowSpontaneous;
            clearCb.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud1, void* ud2) {
                auto* self = static_cast<VncServer*>(ud1);
                self->_gpuWorkDone = true;
                // Wake up libuv main loop
                if (self->_uvAsync) {
                    uv_async_send(static_cast<uv_async_t*>(self->_uvAsync));
                }
            };
            clearCb.userdata1 = this;
            wgpuQueueOnSubmittedWorkDone(_queue, clearCb);

            _captureState = CaptureState::WAITING_CLEAR;
            return Ok();  // Return immediately, don't block!
        }

        case CaptureState::WAITING_CLEAR: {
            if (!_gpuWorkDone) {
                return Ok();  // Still waiting, return without blocking
            }

            // Clear is done, now run compute shader
            WGPUExtent3D extent = {width, height, 1};

            WGPUCommandEncoderDescriptor encDesc = {};
            WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

            WGPUComputePassDescriptor cpDesc = {};
            WGPUComputePassEncoder cpass = wgpuCommandEncoderBeginComputePass(encoder, &cpDesc);
            wgpuComputePassEncoderSetPipeline(cpass, _diffPipeline);
            wgpuComputePassEncoderSetBindGroup(cpass, 0, _diffBindGroup, 0, nullptr);
            wgpuComputePassEncoderDispatchWorkgroups(cpass, _tilesX, _tilesY, 1);
            wgpuComputePassEncoderEnd(cpass);
            wgpuComputePassEncoderRelease(cpass);

            // Copy dirty flags to readback buffer
            wgpuCommandEncoderCopyBufferToBuffer(encoder, _dirtyFlagsBuffer, 0,
                _dirtyFlagsReadback, 0, _tilesX * _tilesY * sizeof(uint32_t));

            WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, nullptr);
            wgpuQueueSubmit(_queue, 1, &cmdBuf);
            wgpuCommandBufferRelease(cmdBuf);
            wgpuCommandEncoderRelease(encoder);

            // Copy current texture to prev for next frame comparison
            WGPUCommandEncoder copyEnc = wgpuDeviceCreateCommandEncoder(_device, nullptr);
            WGPUTexelCopyTextureInfo srcCopy = {};
            srcCopy.texture = _pendingTexture;
            WGPUTexelCopyTextureInfo dstCopy = {};
            dstCopy.texture = _prevTexture;
            wgpuCommandEncoderCopyTextureToTexture(copyEnc, &srcCopy, &dstCopy, &extent);
            WGPUCommandBuffer copyCmd = wgpuCommandEncoderFinish(copyEnc, nullptr);
            wgpuQueueSubmit(_queue, 1, &copyCmd);
            wgpuCommandBufferRelease(copyCmd);
            wgpuCommandEncoderRelease(copyEnc);

            // Register async callback for compute completion
            _gpuWorkDone = false;
            WGPUQueueWorkDoneCallbackInfo cbInfo = {};
            cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
            cbInfo.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud1, void*) {
                auto* self = static_cast<VncServer*>(ud1);
                self->_gpuWorkDone = true;
                if (self->_uvAsync) {
                    uv_async_send(static_cast<uv_async_t*>(self->_uvAsync));
                }
            };
            cbInfo.userdata1 = this;
            wgpuQueueOnSubmittedWorkDone(_queue, cbInfo);

            _captureState = CaptureState::WAITING_COMPUTE;
            return Ok();
        }

        case CaptureState::WAITING_COMPUTE: {
            if (!_gpuWorkDone) {
                return Ok();  // Still waiting
            }

            // Compute is done, now map the buffer
            _gpuWorkDone = false;
            WGPUBufferMapCallbackInfo cbInfo = {};
            cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
            cbInfo.callback = [](WGPUMapAsyncStatus s, WGPUStringView, void* ud1, void* ud2) {
                auto* self = static_cast<VncServer*>(ud1);
                self->_mapStatus = s;
                self->_gpuWorkDone = true;
                if (self->_uvAsync) {
                    uv_async_send(static_cast<uv_async_t*>(self->_uvAsync));
                }
            };
            cbInfo.userdata1 = this;
            wgpuBufferMapAsync(_dirtyFlagsReadback, WGPUMapMode_Read, 0,
                _tilesX * _tilesY * sizeof(uint32_t), cbInfo);

            _captureState = CaptureState::WAITING_MAP;
            return Ok();
        }

        case CaptureState::WAITING_MAP: {
            if (!_gpuWorkDone) {
                return Ok();  // Still waiting
            }

            if (_mapStatus != WGPUMapAsyncStatus_Success) {
                ywarn("VNC dirty flags map failed");
                _captureState = CaptureState::IDLE;
                return Ok();
            }

            // Read dirty flags
            const uint32_t* flags = static_cast<const uint32_t*>(
                wgpuBufferGetConstMappedRange(_dirtyFlagsReadback, 0, _tilesX * _tilesY * sizeof(uint32_t)));

            for (uint32_t i = 0; i < _tilesX * _tilesY; i++) {
                _dirtyTiles[i] = (flags[i] != 0);
            }
            wgpuBufferUnmap(_dirtyFlagsReadback);

            _captureState = CaptureState::READY_TO_SEND;
            // Fall through to send
            [[fallthrough]];
        }

        case CaptureState::READY_TO_SEND:
            // Reset state for next frame
            _captureState = CaptureState::IDLE;
            break;
    }

    // If no CPU pixels, read back from GPU (still synchronous for simplicity)
    if (!_cpuPixels && !_gpuReadbackPixels.empty()) {
        uint32_t bytesPerPixel = 4;
        uint32_t alignedBytesPerRow = (width * bytesPerPixel + 255) & ~255;
        uint32_t bufSize = alignedBytesPerRow * height;

        // Create or recreate temp readback buffer if size changed
        if (!_tileReadbackBuffer || _tileReadbackBufferSize != bufSize) {
            if (_tileReadbackBuffer) {
                wgpuBufferRelease(_tileReadbackBuffer);
            }
            WGPUBufferDescriptor bufDesc = {};
            bufDesc.size = bufSize;
            bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
            _tileReadbackBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
            _tileReadbackBufferSize = bufSize;
        }

        // Copy texture to buffer
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, nullptr);
        WGPUTexelCopyTextureInfo src = {};
        src.texture = texture;
        WGPUTexelCopyBufferInfo dst = {};
        dst.buffer = _tileReadbackBuffer;
        dst.layout.bytesPerRow = alignedBytesPerRow;
        dst.layout.rowsPerImage = height;
        WGPUExtent3D copySize = {width, height, 1};
        wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &copySize);
        WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, nullptr);
        wgpuQueueSubmit(_queue, 1, &cmd);
        wgpuCommandBufferRelease(cmd);
        wgpuCommandEncoderRelease(encoder);

        // Blocking map and copy (TODO: make async too)
        std::atomic<bool> mapDone{false};
        WGPUBufferMapCallbackInfo cbInfo = {};
        cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        cbInfo.callback = [](WGPUMapAsyncStatus, WGPUStringView, void* ud, void*) {
            *static_cast<std::atomic<bool>*>(ud) = true;
        };
        cbInfo.userdata1 = &mapDone;
        wgpuBufferMapAsync(_tileReadbackBuffer, WGPUMapMode_Read, 0, bufSize, cbInfo);
        while (!mapDone) wgpuDeviceTick(_device);

        const uint8_t* mapped = static_cast<const uint8_t*>(
            wgpuBufferGetConstMappedRange(_tileReadbackBuffer, 0, bufSize));
        uint32_t unalignedRow = width * bytesPerPixel;
        for (uint32_t y = 0; y < height; y++) {
            std::memcpy(_gpuReadbackPixels.data() + y * unalignedRow,
                       mapped + y * alignedBytesPerRow, unalignedRow);
        }
        wgpuBufferUnmap(_tileReadbackBuffer);
    }

    // Count dirty tiles
    uint16_t numDirty = 0;
    for (size_t i = 0; i < _dirtyTiles.size(); i++) {
        if (_dirtyTiles[i]) numDirty++;
    }

    yinfo("VNC sendFrame: numDirty={}/{}", numDirty, _tilesX * _tilesY);

    if (numDirty == 0) return Ok();

    // Build frame data
    std::vector<uint8_t> frameData;
    frameData.reserve(64 * 1024);

    FrameHeader fh;
    fh.magic = FRAME_MAGIC;
    fh.width = width;
    fh.height = height;
    fh.tile_size = TILE_SIZE;
    fh.num_tiles = numDirty;

    frameData.insert(frameData.end(),
                     reinterpret_cast<uint8_t*>(&fh),
                     reinterpret_cast<uint8_t*>(&fh) + sizeof(fh));

    bool isFullUpdate = (numDirty == _tilesX * _tilesY);
    if (isFullUpdate) _stats.fullUpdates++;

    // Encode dirty tiles from CPU framebuffer
    for (uint16_t ty = 0; ty < _tilesY; ty++) {
        for (uint16_t tx = 0; tx < _tilesX; tx++) {
            if (!_dirtyTiles[ty * _tilesX + tx]) continue;

            std::vector<uint8_t> tileData;
            Encoding enc;
            if (auto res = encodeTile(tx, ty, tileData, enc); !res) continue;

            _stats.tilesSent++;
            if (enc == Encoding::JPEG) {
                _stats.tilesJpeg++;
                _stats.bytesJpeg += tileData.size();
            } else {
                _stats.tilesRaw++;
                _stats.bytesRaw += tileData.size();
            }

            TileHeader th;
            th.tile_x = tx;
            th.tile_y = ty;
            th.encoding = static_cast<uint8_t>(enc);
            th.data_size = tileData.size();

            frameData.insert(frameData.end(),
                            reinterpret_cast<uint8_t*>(&th),
                            reinterpret_cast<uint8_t*>(&th) + sizeof(th));
            frameData.insert(frameData.end(), tileData.begin(), tileData.end());
        }
    }

    // Send to all clients
    {
        ytimeit("vnc-send");
        std::lock_guard<std::mutex> lock(_clientsMutex);
        std::vector<int> deadClients;

        for (int fd : _clients) {
            if (auto res = sendToClient(fd, frameData.data(), frameData.size()); !res) {
                deadClients.push_back(fd);
            }
        }

        // Remove dead clients
        for (int fd : deadClients) {
            close(fd);
            _clients.erase(std::remove(_clients.begin(), _clients.end(), fd), _clients.end());
            _clientInputBuffers.erase(fd);
            yinfo("VNC client disconnected");
        }
        _clientCount = _clients.size();
    }

    // Track frame stats
    _stats.bytesSent += frameData.size();
    _stats.frames++;

    // Report stats every second
    auto now = std::chrono::duration<double>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    if (now - _stats.lastReportTime >= 1.0) {
        double elapsed = now - _stats.lastReportTime;
        if (_stats.lastReportTime > 0 && _stats.frames > 0) {
            uint32_t avgTileSize = _stats.tilesSent > 0
                ? static_cast<uint32_t>((_stats.bytesJpeg + _stats.bytesRaw) / _stats.tilesSent)
                : 0;
            // Save to public stats
            _lastStats.tilesSent = _stats.tilesSent;
            _lastStats.tilesJpeg = _stats.tilesJpeg;
            _lastStats.tilesRaw = _stats.tilesRaw;
            _lastStats.avgTileSize = avgTileSize;
            _lastStats.fullUpdates = _stats.fullUpdates;
            _lastStats.frames = _stats.frames;
            _lastStats.bytesPerSec = static_cast<uint64_t>(_stats.bytesSent / elapsed);
        }
        // Reset stats
        _stats = Stats{};
        _stats.lastReportTime = now;
    }

    return Ok();
}

void VncServer::pollClientInput(int clientFd) {
    auto it = _clientInputBuffers.find(clientFd);
    if (it == _clientInputBuffers.end()) {
        ydebug("VNC pollClientInput: no buffer for fd={}", clientFd);
        return;
    }

    ClientInputBuffer& buf = it->second;

    // Non-blocking read
    while (true) {
        size_t toRead = buf.needed - buf.buffer.size();
        if (toRead == 0) break;

        uint8_t tmp[256];
        ssize_t n = recv(clientFd, tmp, std::min(toRead, sizeof(tmp)), MSG_DONTWAIT);
        if (n <= 0) {
            if (n == 0) {
                ydebug("VNC pollClientInput: fd={} disconnected", clientFd);
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                ydebug("VNC pollClientInput: fd={} recv error errno={}", clientFd, errno);
            }
            break;  // No more data available
        }

        yinfo("VNC pollClientInput: fd={} received {} bytes", clientFd, n);
        buf.buffer.insert(buf.buffer.end(), tmp, tmp + n);

        // Check if we have enough data
        if (buf.buffer.size() >= buf.needed) {
            if (buf.readingHeader) {
                // Parse header
                std::memcpy(&buf.header, buf.buffer.data(), sizeof(InputHeader));
                ydebug("VNC pollClientInput: parsed header type={} data_size={}", buf.header.type, buf.header.data_size);
                buf.buffer.clear();
                buf.needed = buf.header.data_size;
                buf.readingHeader = false;

                if (buf.needed == 0) {
                    // No data, dispatch immediately
                    ydebug("VNC pollClientInput: dispatching event with no data");
                    dispatchInput(buf.header, nullptr);
                    buf.needed = sizeof(InputHeader);
                    buf.readingHeader = true;
                }
            } else {
                // Dispatch event
                ydebug("VNC pollClientInput: dispatching event with {} bytes data", buf.buffer.size());
                dispatchInput(buf.header, buf.buffer.data());
                buf.buffer.clear();
                buf.needed = sizeof(InputHeader);
                buf.readingHeader = true;
            }
        }
    }
}

void VncServer::dispatchInput(const InputHeader& hdr, const uint8_t* data) {
    ydebug("VNC dispatchInput: type={} size={} data={}", hdr.type, hdr.data_size, (void*)data);
    switch (static_cast<InputType>(hdr.type)) {
        case InputType::MOUSE_MOVE:
            ydebug("VNC dispatchInput: MOUSE_MOVE callback={}", (bool)onMouseMove);
            if (data && hdr.data_size >= sizeof(MouseMoveEvent) && onMouseMove) {
                const MouseMoveEvent* m = reinterpret_cast<const MouseMoveEvent*>(data);
                ydebug("VNC dispatchInput: calling onMouseMove x={} y={}", m->x, m->y);
                onMouseMove(m->x, m->y);
            }
            break;

        case InputType::MOUSE_BUTTON:
            ydebug("VNC dispatchInput: MOUSE_BUTTON callback={}", (bool)onMouseButton);
            if (data && hdr.data_size >= sizeof(MouseButtonEvent) && onMouseButton) {
                const MouseButtonEvent* m = reinterpret_cast<const MouseButtonEvent*>(data);
                ydebug("VNC dispatchInput: calling onMouseButton x={} y={} btn={} pressed={}", m->x, m->y, m->button, m->pressed);
                onMouseButton(m->x, m->y, static_cast<MouseButton>(m->button), m->pressed != 0);
            }
            break;

        case InputType::MOUSE_SCROLL:
            ydebug("VNC dispatchInput: MOUSE_SCROLL callback={}", (bool)onMouseScroll);
            if (data && hdr.data_size >= sizeof(MouseScrollEvent) && onMouseScroll) {
                const MouseScrollEvent* m = reinterpret_cast<const MouseScrollEvent*>(data);
                ydebug("VNC dispatchInput: calling onMouseScroll x={} y={} dx={} dy={}", m->x, m->y, m->delta_x, m->delta_y);
                onMouseScroll(m->x, m->y, m->delta_x, m->delta_y);
            }
            break;

        case InputType::KEY_DOWN:
            ydebug("VNC dispatchInput: KEY_DOWN callback={}", (bool)onKeyDown);
            if (data && hdr.data_size >= sizeof(KeyEvent) && onKeyDown) {
                const KeyEvent* k = reinterpret_cast<const KeyEvent*>(data);
                ydebug("VNC dispatchInput: calling onKeyDown keycode={} scancode={} mods={}", k->keycode, k->scancode, k->mods);
                onKeyDown(k->keycode, k->scancode, k->mods);
            }
            break;

        case InputType::KEY_UP:
            ydebug("VNC dispatchInput: KEY_UP callback={}", (bool)onKeyUp);
            if (data && hdr.data_size >= sizeof(KeyEvent) && onKeyUp) {
                const KeyEvent* k = reinterpret_cast<const KeyEvent*>(data);
                ydebug("VNC dispatchInput: calling onKeyUp keycode={} scancode={} mods={}", k->keycode, k->scancode, k->mods);
                onKeyUp(k->keycode, k->scancode, k->mods);
            }
            break;

        case InputType::TEXT_INPUT:
            ydebug("VNC dispatchInput: TEXT_INPUT callback={}", (bool)onTextInput);
            if (data && hdr.data_size > 0 && onTextInput) {
                std::string text(reinterpret_cast<const char*>(data), hdr.data_size);
                ydebug("VNC dispatchInput: calling onTextInput text={}", text);
                onTextInput(text);
            }
            break;

        case InputType::RESIZE:
            yinfo("VNC dispatchInput: RESIZE callback={}", (bool)onResize);
            if (data && hdr.data_size >= sizeof(ResizeEvent) && onResize) {
                const ResizeEvent* r = reinterpret_cast<const ResizeEvent*>(data);
                yinfo("VNC dispatchInput: calling onResize {}x{}", r->width, r->height);
                onResize(r->width, r->height);
            }
            break;

        case InputType::CELL_SIZE:
            yinfo("VNC dispatchInput: CELL_SIZE callback={}", (bool)onCellSize);
            if (data && hdr.data_size >= sizeof(CellSizeEvent) && onCellSize) {
                const CellSizeEvent* c = reinterpret_cast<const CellSizeEvent*>(data);
                yinfo("VNC dispatchInput: calling onCellSize cellHeight={}", c->cellHeight);
                onCellSize(c->cellHeight);
            }
            break;

        case InputType::CHAR_WITH_MODS:
            ydebug("VNC dispatchInput: CHAR_WITH_MODS callback={}", (bool)onCharWithMods);
            if (data && hdr.data_size >= sizeof(CharWithModsEvent) && onCharWithMods) {
                const CharWithModsEvent* c = reinterpret_cast<const CharWithModsEvent*>(data);
                ydebug("VNC dispatchInput: calling onCharWithMods codepoint={} mods={}", c->codepoint, c->mods);
                onCharWithMods(c->codepoint, c->mods);
            }
            break;
    }
}

bool VncServer::hasPendingInput() const {
    return false;  // Input is processed immediately now
}

void VncServer::processInput() {
    // Poll all clients for input (non-blocking)
    std::lock_guard<std::mutex> lock(_clientsMutex);
    static int callCount = 0;
    if (++callCount % 100 == 1) {
        yinfo("VNC processInput: {} clients", _clients.size());
    }
    for (int fd : _clients) {
        pollClientInput(fd);
    }
}

VncServer::FrameStats VncServer::getStats() const {
    return _lastStats;
}

} // namespace yetty::vnc
