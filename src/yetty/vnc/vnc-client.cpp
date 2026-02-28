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
    let tex = textureSample(frame_texture, frame_sampler, in.uv);
    // Debug: return magenta if texture alpha is zero, otherwise return texture
    if (tex.a < 0.01) {
        return vec4<f32>(1.0, 0.0, 1.0, 1.0);  // magenta = no texture data
    }
    return tex;
}
)";

VncClient::VncClient(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat surfaceFormat)
    : _device(device), _queue(queue), _surfaceFormat(surfaceFormat) {
    _jpegDecompressor = tjInitDecompress();
}

VncClient::~VncClient() {
    disconnect();

    if (_jpegDecompressor) {
        tjDestroy(static_cast<tjhandle>(_jpegDecompressor));
    }

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

    // Set non-blocking
    int flags = fcntl(_socket, F_GETFL, 0);
    fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

    _connected = true;
    yinfo("VNC client connected to {}:{}", host, port);

    // Initialize receive state machine
    _recvState = RecvState::FRAME_HEADER;
    _recvNeeded = sizeof(FrameHeader);
    _recvOffset = 0;
    _recvBuffer.resize(_recvNeeded);

    // Register with event loop for async receive
    auto loopResult = base::EventLoop::instance();
    if (!loopResult) {
        return Err<void>("No event loop available");
    }
    auto loop = *loopResult;

    auto pollRes = loop->createPoll();
    if (!pollRes) {
        return Err<void>("Failed to create poll", pollRes);
    }
    _pollId = *pollRes;

    if (auto res = loop->configPoll(_pollId, _socket); !res) {
        return Err<void>("Failed to configure poll", res);
    }

    auto listener = sharedAs<base::EventListener>();
    if (!listener) {
        return Err<void>("Failed to get shared_ptr<EventListener> - sharedAs returned nullptr");
    }

    if (auto res = loop->registerPollListener(_pollId, listener); !res) {
        return Err<void>("Failed to register poll listener", res);
    }

    if (auto res = loop->startPoll(_pollId); !res) {
        return Err<void>("Failed to start poll", res);
    }

    yinfo("VNC client: registered poll id={} for socket={}", _pollId, _socket);
    return Ok();
}

void VncClient::disconnect() {
    _connected = false;

    // Deregister from event loop
    if (_pollId >= 0) {
        auto loopResult = base::EventLoop::instance();
        if (loopResult) {
            auto loop = *loopResult;
            loop->stopPoll(_pollId);
            loop->destroyPoll(_pollId);
        }
        _pollId = -1;
    }

    if (_socket >= 0) {
        ::shutdown(_socket, SHUT_RDWR);
        ::close(_socket);
        _socket = -1;
    }
}

Result<bool> VncClient::onEvent(const base::Event& event) {
    if (event.type == base::Event::Type::PollReadable) {
        if (event.poll.fd == _socket) {
            onSocketReadable();
            return Ok(true);
        }
    }
    return Ok(false);
}

void VncClient::onSocketReadable() {
    if (!_connected || _socket < 0) return;

    yinfo("VNC client: onSocketReadable called, state={}", static_cast<int>(_recvState));

    // Read as much as possible
    while (true) {
        ssize_t n = recv(_socket, _recvBuffer.data() + _recvOffset,
                         _recvNeeded - _recvOffset, MSG_DONTWAIT);

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more data available
                return;
            }
            ywarn("VNC client: recv error: {}", strerror(errno));
            disconnect();
            return;
        }

        if (n == 0) {
            yinfo("VNC client: server closed connection");
            disconnect();
            return;
        }

        _recvOffset += n;

        // Check if we have enough data
        if (_recvOffset < _recvNeeded) {
            continue;  // Need more data
        }

        // Process based on current state
        switch (_recvState) {
            case RecvState::FRAME_HEADER: {
                std::memcpy(&_currentFrame, _recvBuffer.data(), sizeof(FrameHeader));

                // Validate magic number
                if (_currentFrame.magic != FRAME_MAGIC) {
                    ywarn("VNC: Invalid frame magic 0x{:08X}, expected 0x{:08X}",
                          _currentFrame.magic, FRAME_MAGIC);
                    // Drain and resync
                    uint8_t drain[1024];
                    while (recv(_socket, drain, sizeof(drain), MSG_DONTWAIT) > 0) {}
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                    return;
                }

                // Sanity check dimensions
                if (_currentFrame.width == 0 || _currentFrame.height == 0 ||
                    _currentFrame.width > 8192 || _currentFrame.height > 8192) {
                    ywarn("VNC: Invalid frame header {}x{}", _currentFrame.width, _currentFrame.height);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    return;
                }

                ydebug("VNC client: frame {}x{} tiles={}", _currentFrame.width, _currentFrame.height, _currentFrame.num_tiles);

                // Update frame dimensions
                if (_currentFrame.width != _width || _currentFrame.height != _height) {
                    _width = _currentFrame.width;
                    _height = _currentFrame.height;
                    _pixels.resize(_width * _height * 4);
                    std::memset(_pixels.data(), 0, _pixels.size());
                    yinfo("VNC: Frame size {}x{}", _width, _height);
                }

                _tilesReceived = 0;

                if (_currentFrame.num_tiles == 0) {
                    // No tiles, wait for next frame
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                } else {
                    // Read first tile header
                    _recvState = RecvState::TILE_HEADER;
                    _recvNeeded = sizeof(TileHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                }
                break;
            }

            case RecvState::TILE_HEADER: {
                std::memcpy(&_currentTile, _recvBuffer.data(), sizeof(TileHeader));

                // Sanity check
                if (_currentTile.data_size > 16 * 1024 * 1024) {
                    ywarn("VNC: Tile data too large: {}", _currentTile.data_size);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                    return;
                }

                // Read tile data
                _recvState = RecvState::TILE_DATA;
                _recvNeeded = _currentTile.data_size;
                _recvOffset = 0;
                _recvBuffer.resize(_recvNeeded);
                break;
            }

            case RecvState::TILE_DATA: {
                // Decode tile
                TileUpdate update;
                update.tile_x = _currentTile.tile_x;
                update.tile_y = _currentTile.tile_y;
                update.pixels.resize(TILE_SIZE * TILE_SIZE * 4);

                switch (static_cast<Encoding>(_currentTile.encoding)) {
                    case Encoding::RAW:
                        if (_currentTile.data_size == TILE_SIZE * TILE_SIZE * 4) {
                            std::memcpy(update.pixels.data(), _recvBuffer.data(), _currentTile.data_size);
                        }
                        break;

                    case Encoding::JPEG: {
                        int width, height, subsamp, colorspace;
                        if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                               _recvBuffer.data(), _currentTile.data_size,
                                               &width, &height, &subsamp, &colorspace) == 0) {
                            tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                         _recvBuffer.data(), _currentTile.data_size,
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
                uint16_t tx = update.tile_x, ty = update.tile_y;
                _pendingTiles.push(std::move(update));
                yinfo("VNC client: queued tile ({},{}) total pending={}", tx, ty, _pendingTiles.size());

                _tilesReceived++;

                if (_tilesReceived >= _currentFrame.num_tiles) {
                    // All tiles received, wait for next frame
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                } else {
                    // Read next tile header
                    _recvState = RecvState::TILE_HEADER;
                    _recvNeeded = sizeof(TileHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                }
                break;
            }
        }
    }
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

Result<bool> VncClient::updateTexture() {
    ydebug("VNC updateTexture: width={} height={}", _width, _height);
    if (_width == 0 || _height == 0) return Ok(false);

    // Ensure GPU resources
    if (auto res = ensureResources(_width, _height); !res) {
        ywarn("VNC updateTexture: ensureResources failed");
        return Err<bool>("ensureResources failed", res);
    }

    // Process pending tile updates
    size_t tilesProcessed = 0;
    while (!_pendingTiles.empty()) {
        TileUpdate& update = _pendingTiles.front();

        // Calculate pixel coordinates
        uint32_t px = update.tile_x * TILE_SIZE;
        uint32_t py = update.tile_y * TILE_SIZE;

        // Validate tile is within frame bounds
        if (px >= _width || py >= _height) {
            ywarn("VNC: Tile ({},{}) at pixel ({},{}) outside frame {}x{}",
                  update.tile_x, update.tile_y, px, py, _width, _height);
            _pendingTiles.pop();
            continue;
        }

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

        _pendingTiles.pop();
        tilesProcessed++;
    }

    if (tilesProcessed > 0) {
        ydebug("VNC updateTexture: processed {} tiles", tilesProcessed);
    }

    return Ok(tilesProcessed > 0);
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
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Would block, try again
                continue;
            }
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

void VncClient::sendResize(uint16_t width, uint16_t height) {
    yinfo("VNC client sendResize: {}x{}", width, height);
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::RESIZE);
    hdr.data_size = sizeof(ResizeEvent);

    ResizeEvent evt = {};
    evt.width = width;
    evt.height = height;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendCellSize(uint8_t cellHeight) {
    yinfo("VNC client sendCellSize: cellHeight={}", (int)cellHeight);
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::CELL_SIZE);
    hdr.data_size = sizeof(CellSizeEvent);

    CellSizeEvent evt = {};
    evt.cellHeight = cellHeight;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

} // namespace yetty::vnc
