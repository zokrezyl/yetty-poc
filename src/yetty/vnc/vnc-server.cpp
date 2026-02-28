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

namespace yetty::vnc {

using yetty::Result;
using yetty::Ok;
using yetty::Err;

VncServer::VncServer(WGPUDevice device, WGPUQueue queue)
    : _device(device), _queue(queue) {
    _jpegCompressor = tjInitCompress();
}

// Compute shader for tile diff detection
// Each workgroup handles one tile, compares pixels, outputs 1 if any differ
static const char* DIFF_SHADER = R"(
@group(0) @binding(0) var currTex: texture_2d<f32>;
@group(0) @binding(1) var prevTex: texture_2d<f32>;
@group(0) @binding(2) var<storage, read_write> dirtyFlags: array<u32>;

override TILE_SIZE: u32 = 64;
override TILES_X: u32 = 20;

@compute @workgroup_size(8, 8)
fn main(@builtin(global_invocation_id) gid: vec3<u32>,
        @builtin(workgroup_id) wgid: vec3<u32>) {
    let tileIdx = wgid.y * TILES_X + wgid.x;
    let px = wgid.x * TILE_SIZE + gid.x % TILE_SIZE;
    let py = wgid.y * TILE_SIZE + gid.y % TILE_SIZE;

    let dims = textureDimensions(currTex);
    if (px >= dims.x || py >= dims.y) {
        return;
    }

    let curr = textureLoad(currTex, vec2<u32>(px, py), 0);
    let prev = textureLoad(prevTex, vec2<u32>(px, py), 0);

    // If any channel differs, mark tile dirty (atomic OR)
    if (any(curr != prev)) {
        atomicOr(&dirtyFlags[tileIdx], 1u);
    }
}
)";

VncServer::~VncServer() {
    stop();
    if (_prevTexture) wgpuTextureRelease(_prevTexture);
    if (_dirtyFlagsBuffer) wgpuBufferRelease(_dirtyFlagsBuffer);
    if (_dirtyFlagsReadback) wgpuBufferRelease(_dirtyFlagsReadback);
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
    uint32_t numTiles = _tilesX * _tilesY;
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = numTiles * sizeof(uint32_t);
    bufDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopySrc;
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

    wgpuDeviceTick(_device);

    ydebug("VNC sendFrame: state={} computeDone={} mapDone={}",
           static_cast<int>(_captureState), _computeDone.load(), _mapDone.load());

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

            if (_forceFullFrame) {
                // Skip GPU diff, mark all dirty
                std::fill(_dirtyTiles.begin(), _dirtyTiles.end(), true);
                _forceFullFrame = false;
                _captureState = CaptureState::IDLE;
                break;  // Go to encoding
            }

            // Upload current frame to GPU for comparison
            WGPUTexelCopyTextureInfo dst = {};
            dst.texture = _prevTexture;
            WGPUTexelCopyBufferLayout layout = {};
            layout.bytesPerRow = width * 4;
            layout.rowsPerImage = height;
            WGPUExtent3D extent = {width, height, 1};

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

            // Clear dirty flags buffer, run compute, copy to readback
            WGPUCommandEncoderDescriptor encDesc = {};
            WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

            // Clear dirty flags to 0
            wgpuCommandEncoderClearBuffer(encoder, _dirtyFlagsBuffer, 0, _tilesX * _tilesY * sizeof(uint32_t));

            // Run compute shader
            WGPUComputePassDescriptor cpDesc = {};
            WGPUComputePassEncoder cpass = wgpuCommandEncoderBeginComputePass(encoder, &cpDesc);
            wgpuComputePassEncoderSetPipeline(cpass, _diffPipeline);
            wgpuComputePassEncoderSetBindGroup(cpass, 0, _diffBindGroup, 0, nullptr);
            // Dispatch one workgroup per tile (8x8 threads per workgroup covers 64x64 tile)
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
            srcCopy.texture = texture;
            WGPUTexelCopyTextureInfo dstCopy = {};
            dstCopy.texture = _prevTexture;
            wgpuCommandEncoderCopyTextureToTexture(copyEnc, &srcCopy, &dstCopy, &extent);
            WGPUCommandBuffer copyCmd = wgpuCommandEncoderFinish(copyEnc, nullptr);
            wgpuQueueSubmit(_queue, 1, &copyCmd);
            wgpuCommandBufferRelease(copyCmd);
            wgpuCommandEncoderRelease(copyEnc);

            // Wait for GPU work to complete (synchronous)
            {
                std::atomic<bool> done{false};
                WGPUQueueWorkDoneCallbackInfo cbInfo = {};
                cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
                cbInfo.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud, void*) {
                    *static_cast<std::atomic<bool>*>(ud) = true;
                };
                cbInfo.userdata1 = &done;
                wgpuQueueOnSubmittedWorkDone(_queue, cbInfo);
                while (!done) wgpuDeviceTick(_device);
            }

            // Map dirty flags buffer and read (synchronous)
            {
                std::atomic<bool> done{false};
                WGPUMapAsyncStatus status = WGPUMapAsyncStatus_Success;
                WGPUBufferMapCallbackInfo cbInfo = {};
                cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
                cbInfo.callback = [](WGPUMapAsyncStatus s, WGPUStringView, void* ud1, void* ud2) {
                    *static_cast<std::atomic<bool>*>(ud1) = true;
                    *static_cast<WGPUMapAsyncStatus*>(ud2) = s;
                };
                cbInfo.userdata1 = &done;
                cbInfo.userdata2 = &status;
                wgpuBufferMapAsync(_dirtyFlagsReadback, WGPUMapMode_Read, 0,
                    _tilesX * _tilesY * sizeof(uint32_t), cbInfo);
                while (!done) wgpuDeviceTick(_device);

                if (status != WGPUMapAsyncStatus_Success) {
                    ywarn("VNC dirty flags map failed");
                    break;
                }

                const uint32_t* flags = static_cast<const uint32_t*>(
                    wgpuBufferGetConstMappedRange(_dirtyFlagsReadback, 0, _tilesX * _tilesY * sizeof(uint32_t)));

                for (uint32_t i = 0; i < _tilesX * _tilesY; i++) {
                    _dirtyTiles[i] = (flags[i] != 0);
                }
                wgpuBufferUnmap(_dirtyFlagsReadback);
            }
            break;  // Fall through to encoding
        }
    }

    // If no CPU pixels, read back from GPU (synchronous for simplicity)
    if (!_cpuPixels && !_gpuReadbackPixels.empty()) {
        uint32_t bytesPerPixel = 4;
        uint32_t alignedBytesPerRow = (width * bytesPerPixel + 255) & ~255;
        uint32_t bufSize = alignedBytesPerRow * height;

        // Create temp readback buffer if needed
        if (!_tileReadbackBuffer) {
            WGPUBufferDescriptor bufDesc = {};
            bufDesc.size = bufSize;
            bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
            _tileReadbackBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
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

        // Blocking map and copy
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
