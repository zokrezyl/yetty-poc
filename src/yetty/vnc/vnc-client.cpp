#include "vnc-client.h"
#include <ytrace/ytrace.hpp>
#include <turbojpeg.h>

#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

namespace yetty::vnc {

using yetty::Result;
using yetty::Ok;
using yetty::Err;

// Fullscreen quad shader
static const char* SHADER_SOURCE = R"(
struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    var positions = array<vec2<f32>, 6>(
        vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0),
        vec2(-1.0, 1.0), vec2(1.0, -1.0), vec2(1.0, 1.0)
    );
    var uvs = array<vec2<f32>, 6>(
        vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(0.0, 0.0),
        vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)
    );
    var out: VertexOutput;
    out.position = vec4(positions[idx], 0.0, 1.0);
    out.uv = uvs[idx];
    return out;
}

@group(0) @binding(0) var frame_texture: texture_2d<f32>;
@group(0) @binding(1) var frame_sampler: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return textureSample(frame_texture, frame_sampler, in.uv);
}
)";

VncClient::VncClient(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat surfaceFormat)
    : _device(device), _queue(queue), _surfaceFormat(surfaceFormat) {
}

VncClient::~VncClient() {
    disconnect();

    if (_pipeline) wgpuRenderPipelineRelease(_pipeline);
    if (_bindGroup) wgpuBindGroupRelease(_bindGroup);
    if (_bindGroupLayout) wgpuBindGroupLayoutRelease(_bindGroupLayout);
    if (_sampler) wgpuSamplerRelease(_sampler);
    if (_textureView) wgpuTextureViewRelease(_textureView);
    if (_texture) wgpuTextureRelease(_texture);
    if (_vertexBuffer) wgpuBufferRelease(_vertexBuffer);
}

Result<void> VncClient::connect(const std::string& host, uint16_t port) {
    if (_connected) {
        return Err<void>("Already connected");
    }

    // Resolve host
    struct addrinfo hints = {}, *result;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);
    if (getaddrinfo(host.c_str(), portStr.c_str(), &hints, &result) != 0) {
        return Err<void>("Failed to resolve host: " + host);
    }

    // Create socket
    _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (_socket < 0) {
        freeaddrinfo(result);
        return Err<void>("Failed to create socket");
    }

    // Connect
    if (::connect(_socket, result->ai_addr, result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to connect to " + host + ":" + portStr);
    }
    freeaddrinfo(result);

    // Disable Nagle's algorithm for lower latency
    int flag = 1;
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    _connected = true;
    yinfo("VNC client connected to {}:{}", host, port);

    // Start receive thread
    _receiveThread = std::thread(&VncClient::receiveLoop, this);

    return Ok();
}

void VncClient::disconnect() {
    _connected = false;

    if (_socket >= 0) {
        shutdown(_socket, SHUT_RDWR);
        close(_socket);
        _socket = -1;
    }

    if (_receiveThread.joinable()) {
        _receiveThread.join();
    }
}

// Helper to read exactly n bytes with poll timeout
static bool recvExact(int fd, void* buf, size_t len, int timeoutMs) {
    uint8_t* ptr = static_cast<uint8_t*>(buf);
    size_t remaining = len;
    while (remaining > 0) {
        struct pollfd pfd = {fd, POLLIN, 0};
        int ret = poll(&pfd, 1, timeoutMs);
        if (ret <= 0) return false;  // Timeout or error

        ssize_t n = recv(fd, ptr, remaining, 0);
        if (n <= 0) return false;
        ptr += n;
        remaining -= n;
    }
    return true;
}

void VncClient::receiveLoop() {
    ydebug("VNC receiveLoop: started, socket={}", _socket);
    std::vector<uint8_t> recvBuffer(16 * 1024 * 1024);  // 16MB buffer
    tjhandle jpegDecompressor = tjInitDecompress();

    while (_connected) {
        // Read frame header with timeout
        FrameHeader header;
        if (!recvExact(_socket, &header, sizeof(header), 100)) {
            continue;  // Timeout, keep looping
        }

        // Validate magic number
        if (header.magic != FRAME_MAGIC) {
            ywarn("VNC: Invalid frame magic 0x{:08X}, expected 0x{:08X}",
                  header.magic, FRAME_MAGIC);
            // Drain socket and resync
            uint8_t drain[1024];
            recv(_socket, drain, sizeof(drain), MSG_DONTWAIT);
            continue;
        }

        // Sanity check dimensions
        if (header.width == 0 || header.height == 0 || header.width > 8192 || header.height > 8192) {
            ywarn("VNC: Invalid frame header {}x{}", header.width, header.height);
            continue;
        }

        // Sanity check tile count
        uint16_t maxTiles = ((header.width + TILE_SIZE - 1) / TILE_SIZE) *
                           ((header.height + TILE_SIZE - 1) / TILE_SIZE);
        if (header.num_tiles > maxTiles) {
            ywarn("VNC: Too many tiles {} (max {})", header.num_tiles, maxTiles);
            continue;
        }

        ydebug("VNC receiveLoop: frame {}x{} tiles={}", header.width, header.height, header.num_tiles);

        // Ensure resources match frame size
        if (header.width != _width || header.height != _height) {
            _width = header.width;
            _height = header.height;
            _pixels.resize(_width * _height * 4);
            std::memset(_pixels.data(), 0, _pixels.size());
            yinfo("VNC: Frame size {}x{}", _width, _height);
        }

        // Read all tiles
        for (uint16_t i = 0; i < header.num_tiles; i++) {
            TileHeader tile;
            if (!recvExact(_socket, &tile, sizeof(tile), 1000)) {
                ywarn("VNC: Failed to read tile header");
                break;
            }

            // Sanity check
            if (tile.data_size > 16 * 1024 * 1024) {
                ywarn("VNC: Tile data too large: {}", tile.data_size);
                break;
            }

            // Read tile data
            if (tile.data_size > recvBuffer.size()) {
                recvBuffer.resize(tile.data_size);
            }
            if (!recvExact(_socket, recvBuffer.data(), tile.data_size, 1000)) {
                ywarn("VNC: Failed to read tile data");
                break;
            }

            // Decode tile
            TileUpdate update;
            update.tile_x = tile.tile_x;
            update.tile_y = tile.tile_y;
            update.pixels.resize(TILE_SIZE * TILE_SIZE * 4);

            switch (static_cast<Encoding>(tile.encoding)) {
                case Encoding::RAW:
                    if (tile.data_size == TILE_SIZE * TILE_SIZE * 4) {
                        std::memcpy(update.pixels.data(), recvBuffer.data(), tile.data_size);
                    }
                    break;

                case Encoding::JPEG: {
                    int width, height, subsamp, colorspace;
                    if (tjDecompressHeader3(jpegDecompressor, recvBuffer.data(), tile.data_size,
                                           &width, &height, &subsamp, &colorspace) == 0) {
                        tjDecompress2(jpegDecompressor, recvBuffer.data(), tile.data_size,
                                     update.pixels.data(), TILE_SIZE, 0, TILE_SIZE,
                                     TJPF_BGRA, TJFLAG_FASTDCT);
                    }
                    break;
                }

                case Encoding::RLE:
                    // TODO: implement RLE decoding
                    break;
            }

            // Queue tile update
            {
                std::lock_guard<std::mutex> lock(_tileMutex);
                _pendingTiles.push(std::move(update));
            }
        }
    }

    tjDestroy(jpegDecompressor);
}

Result<void> VncClient::ensureResources(uint16_t width, uint16_t height) {
    if (_texture && _width == width && _height == height) {
        return Ok();
    }

    // Cleanup old resources
    if (_bindGroup) { wgpuBindGroupRelease(_bindGroup); _bindGroup = nullptr; }
    if (_textureView) { wgpuTextureViewRelease(_textureView); _textureView = nullptr; }
    if (_texture) { wgpuTextureRelease(_texture); _texture = nullptr; }

    // Create texture
    WGPUTextureDescriptor texDesc = {};
    texDesc.size = {width, height, 1};
    texDesc.format = WGPUTextureFormat_BGRA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    texDesc.dimension = WGPUTextureDimension_2D;

    _texture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_texture) return Err<void>("Failed to create VNC texture");

    _textureView = wgpuTextureCreateView(_texture, nullptr);
    if (!_textureView) return Err<void>("Failed to create VNC texture view");

    // Create sampler (once)
    if (!_sampler) {
        WGPUSamplerDescriptor samplerDesc = {};
        samplerDesc.magFilter = WGPUFilterMode_Linear;
        samplerDesc.minFilter = WGPUFilterMode_Linear;
        samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
        samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
        samplerDesc.maxAnisotropy = 1;
        _sampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    }

    // Create pipeline (once)
    if (!_pipeline) {
        if (auto res = createPipeline(); !res) {
            return res;
        }
    }

    // Create bind group
    WGPUBindGroupEntry entries[2] = {};
    entries[0].binding = 0;
    entries[0].textureView = _textureView;
    entries[1].binding = 1;
    entries[1].sampler = _sampler;

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = _bindGroupLayout;
    bgDesc.entryCount = 2;
    bgDesc.entries = entries;

    _bindGroup = wgpuDeviceCreateBindGroup(_device, &bgDesc);
    if (!_bindGroup) return Err<void>("Failed to create VNC bind group");

    return Ok();
}

Result<void> VncClient::createPipeline() {
    // Create shader module
    WGPUShaderSourceWGSL wgslDesc = {};
    wgslDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslDesc.code = {SHADER_SOURCE, strlen(SHADER_SOURCE)};

    WGPUShaderModuleDescriptor shaderDesc = {};
    shaderDesc.nextInChain = &wgslDesc.chain;

    WGPUShaderModule shader = wgpuDeviceCreateShaderModule(_device, &shaderDesc);
    if (!shader) return Err<void>("Failed to create VNC shader");

    // Bind group layout
    WGPUBindGroupLayoutEntry layoutEntries[2] = {};
    layoutEntries[0].binding = 0;
    layoutEntries[0].visibility = WGPUShaderStage_Fragment;
    layoutEntries[0].texture.sampleType = WGPUTextureSampleType_Float;
    layoutEntries[0].texture.viewDimension = WGPUTextureViewDimension_2D;

    layoutEntries[1].binding = 1;
    layoutEntries[1].visibility = WGPUShaderStage_Fragment;
    layoutEntries[1].sampler.type = WGPUSamplerBindingType_Filtering;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 2;
    bglDesc.entries = layoutEntries;
    _bindGroupLayout = wgpuDeviceCreateBindGroupLayout(_device, &bglDesc);

    // Pipeline layout
    WGPUPipelineLayoutDescriptor plDesc = {};
    plDesc.bindGroupLayoutCount = 1;
    plDesc.bindGroupLayouts = &_bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(_device, &plDesc);

    // Render pipeline
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc.layout = pipelineLayout;

    pipelineDesc.vertex.module = shader;
    pipelineDesc.vertex.entryPoint = {.data = "vs_main", .length = 7};

    WGPUColorTargetState colorTarget = {};
    colorTarget.format = _surfaceFormat;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragment = {};
    fragment.module = shader;
    fragment.entryPoint = {.data = "fs_main", .length = 7};
    fragment.targetCount = 1;
    fragment.targets = &colorTarget;
    pipelineDesc.fragment = &fragment;

    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.multisample.count = 1;
    pipelineDesc.multisample.mask = ~0u;

    _pipeline = wgpuDeviceCreateRenderPipeline(_device, &pipelineDesc);

    wgpuShaderModuleRelease(shader);
    wgpuPipelineLayoutRelease(pipelineLayout);

    if (!_pipeline) return Err<void>("Failed to create VNC pipeline");

    return Ok();
}

Result<void> VncClient::updateTexture() {
    ydebug("VNC updateTexture: width={} height={}", _width, _height);
    if (_width == 0 || _height == 0) return Ok();

    // Ensure GPU resources
    if (auto res = ensureResources(_width, _height); !res) {
        ywarn("VNC updateTexture: ensureResources failed");
        return res;
    }

    // Process pending tile updates
    std::queue<TileUpdate> tiles;
    {
        std::lock_guard<std::mutex> lock(_tileMutex);
        std::swap(tiles, _pendingTiles);
    }
    ydebug("VNC updateTexture: {} pending tiles", tiles.size());

    while (!tiles.empty()) {
        TileUpdate& update = tiles.front();

        // Calculate pixel coordinates
        uint32_t px = update.tile_x * TILE_SIZE;
        uint32_t py = update.tile_y * TILE_SIZE;
        uint32_t tw = std::min((uint32_t)TILE_SIZE, _width - px);
        uint32_t th = std::min((uint32_t)TILE_SIZE, _height - py);

        // Upload tile to texture
        WGPUTexelCopyTextureInfo dst = {};
        dst.texture = _texture;
        dst.origin = {px, py, 0};

        WGPUTexelCopyBufferLayout layout = {};
        layout.bytesPerRow = TILE_SIZE * 4;
        layout.rowsPerImage = TILE_SIZE;

        WGPUExtent3D size = {tw, th, 1};

        wgpuQueueWriteTexture(_queue, &dst, update.pixels.data(),
                              update.pixels.size(), &layout, &size);

        tiles.pop();
    }

    return Ok();
}

Result<void> VncClient::render(WGPURenderPassEncoder pass) {
    ydebug("VNC render: pipeline={} bindGroup={}", (void*)_pipeline, (void*)_bindGroup);
    if (!_pipeline || !_bindGroup) {
        ydebug("VNC render: skipping - no pipeline or bindGroup");
        return Ok();
    }

    wgpuRenderPassEncoderSetPipeline(pass, _pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    ydebug("VNC render: draw complete");

    return Ok();
}

void VncClient::sendInput(const void* data, size_t size) {
    if (!_connected || _socket < 0) {
        ywarn("VNC client sendInput: not connected! _socket={}", _socket);
        return;
    }

    ydebug("VNC client sendInput: sending {} bytes to socket {}", size, _socket);
    const uint8_t* ptr = static_cast<const uint8_t*>(data);
    size_t remaining = size;
    while (remaining > 0 && _connected) {
        ssize_t sent = send(_socket, ptr, remaining, MSG_NOSIGNAL);
        if (sent <= 0) {
            ywarn("VNC client sendInput: send failed errno={}", errno);
            return;
        }
        ydebug("VNC client sendInput: sent {} bytes", sent);
        ptr += sent;
        remaining -= sent;
    }
    ydebug("VNC client sendInput: complete");
}

void VncClient::sendMouseMove(int16_t x, int16_t y) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::MOUSE_MOVE);
    hdr.data_size = sizeof(MouseMoveEvent);

    MouseMoveEvent evt = {};
    evt.x = x;
    evt.y = y;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendMouseButton(int16_t x, int16_t y, MouseButton button, bool pressed) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::MOUSE_BUTTON);
    hdr.data_size = sizeof(MouseButtonEvent);

    MouseButtonEvent evt = {};
    evt.x = x;
    evt.y = y;
    evt.button = static_cast<uint8_t>(button);
    evt.pressed = pressed ? 1 : 0;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendMouseScroll(int16_t x, int16_t y, int16_t deltaX, int16_t deltaY) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::MOUSE_SCROLL);
    hdr.data_size = sizeof(MouseScrollEvent);

    MouseScrollEvent evt = {};
    evt.x = x;
    evt.y = y;
    evt.delta_x = deltaX;
    evt.delta_y = deltaY;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendKeyDown(uint32_t keycode, uint32_t scancode, uint8_t mods) {
    ydebug("VNC client sendKeyDown: keycode={} scancode={} mods={}", keycode, scancode, mods);
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::KEY_DOWN);
    hdr.data_size = sizeof(KeyEvent);

    KeyEvent evt = {};
    evt.keycode = keycode;
    evt.scancode = scancode;
    evt.mods = mods;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendKeyUp(uint32_t keycode, uint32_t scancode, uint8_t mods) {
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::KEY_UP);
    hdr.data_size = sizeof(KeyEvent);

    KeyEvent evt = {};
    evt.keycode = keycode;
    evt.scancode = scancode;
    evt.mods = mods;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendTextInput(const char* text, size_t len) {
    if (len == 0 || len > 1024) return; // Sanity check

    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
    hdr.data_size = static_cast<uint16_t>(len);

    std::vector<uint8_t> buf(sizeof(hdr) + len);
    std::memcpy(buf.data(), &hdr, sizeof(hdr));
    std::memcpy(buf.data() + sizeof(hdr), text, len);
    sendInput(buf.data(), buf.size());
}

} // namespace yetty::vnc
