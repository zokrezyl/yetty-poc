#include "vnc-client.h"
#include <ytrace/ytrace.hpp>
#include <turbojpeg.h>

#include <cstring>

#ifdef __EMSCRIPTEN__
#include <emscripten/websocket.h>
#include <emscripten/emscripten.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

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

#ifdef __EMSCRIPTEN__
// WebSocket callbacks for Emscripten
static EM_BOOL onWsOpen(int eventType, const EmscriptenWebSocketOpenEvent* wsEvent, void* userData) {
    VncClient* client = static_cast<VncClient*>(userData);
    yinfo("WebSocket connected");
    client->_wsConnected = true;
    client->_connected = true;
    client->_connecting = false;
    if (client->onConnected) {
        client->onConnected();
    }
    return EM_TRUE;
}

static EM_BOOL onWsError(int eventType, const EmscriptenWebSocketErrorEvent* wsEvent, void* userData) {
    VncClient* client = static_cast<VncClient*>(userData);
    yerror("WebSocket error");
    client->_wsConnected = false;
    client->_connected = false;
    client->_connecting = false;
    client->_wantsReconnect = true;
    if (client->onDisconnected) {
        client->onDisconnected();
    }
    return EM_TRUE;
}

static EM_BOOL onWsClose(int eventType, const EmscriptenWebSocketCloseEvent* wsEvent, void* userData) {
    VncClient* client = static_cast<VncClient*>(userData);
    yinfo("WebSocket closed: code={} reason={}", wsEvent->code, wsEvent->reason);
    client->_wsConnected = false;
    client->_connected = false;
    client->_connecting = false;
    client->_wantsReconnect = true;
    if (client->onDisconnected) {
        client->onDisconnected();
    }
    return EM_TRUE;
}

static EM_BOOL onWsMessage(int eventType, const EmscriptenWebSocketMessageEvent* wsEvent, void* userData) {
    VncClient* client = static_cast<VncClient*>(userData);
    if (wsEvent->isText) {
        ywarn("WebSocket received text message, expected binary");
        return EM_TRUE;
    }
    // Append binary data to receive buffer and process
    client->onWebSocketData(wsEvent->data, wsEvent->numBytes);
    return EM_TRUE;
}
#endif

VncClient::VncClient(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat surfaceFormat, uint16_t width, uint16_t height)
    : _device(device), _queue(queue), _surfaceFormat(surfaceFormat), _width(width), _height(height) {
    _jpegDecompressor = tjInitDecompress();
    // Create texture immediately with view bounds
    ensureResources(width, height);
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

    // Initialize receive state machine
    _recvState = RecvState::FRAME_HEADER;
    _recvNeeded = sizeof(FrameHeader);
    _recvOffset = 0;
    _recvBuffer.resize(_recvNeeded);

#ifdef __EMSCRIPTEN__
    // WebSocket connection for Emscripten
    // host may be either:
    // - a hostname (legacy): build ws://host:port
    // - a full WebSocket URL (from web UI): use as-is
    std::string wsUrl;
    if (host.find("ws://") == 0 || host.find("wss://") == 0) {
        // Full URL provided (from YETTY_VNC_CLIENT env var)
        wsUrl = host;
    } else {
        // Legacy: build URL from host:port
        wsUrl = "ws://" + host + ":" + std::to_string(port);
    }
    yinfo("VNC client connecting via WebSocket to {}", wsUrl);

    EmscriptenWebSocketCreateAttributes attrs;
    emscripten_websocket_init_create_attributes(&attrs);
    attrs.url = wsUrl.c_str();
    attrs.protocols = nullptr;
    attrs.createOnMainThread = EM_TRUE;

    _wsSocket = emscripten_websocket_new(&attrs);
    if (_wsSocket <= 0) {
        return Err<void>("Failed to create WebSocket");
    }

    // Set up callbacks
    emscripten_websocket_set_onopen_callback(_wsSocket, this, onWsOpen);
    emscripten_websocket_set_onerror_callback(_wsSocket, this, onWsError);
    emscripten_websocket_set_onclose_callback(_wsSocket, this, onWsClose);
    emscripten_websocket_set_onmessage_callback(_wsSocket, this, onWsMessage);

    _connecting = true;
    yinfo("VNC client: WebSocket created, waiting for connection...");
    return Ok();

#else
    // POSIX socket connection
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

    // Set non-blocking BEFORE connect for async connection
    int flags = fcntl(_socket, F_GETFL, 0);
    fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

    // Disable Nagle's algorithm for lower latency
    int flag = 1;
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

    // Non-blocking connect - will return immediately with EINPROGRESS
    int connectResult = ::connect(_socket, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    if (connectResult < 0 && errno != EINPROGRESS) {
        close(_socket);
        _socket = -1;
        return Err<void>("Failed to connect to " + host + ":" + portStr + ": " + strerror(errno));
    }

    // Connection is in progress - we'll complete it asynchronously
    _connecting = (errno == EINPROGRESS);
    _connected = !_connecting;  // If connect succeeded immediately (localhost)
    bool immediateConnect = _connected;

    if (_connected) {
        yinfo("VNC client connected to {}:{} (immediate)", host, port);
    } else {
        yinfo("VNC client connecting to {}:{} (async)...", host, port);
    }

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

    // Start polling - if connecting async, need writable to detect connect completion
    // Otherwise just readable
    int pollEvents = base::EventLoop::POLL_READABLE;
    if (_connecting) {
        pollEvents |= base::EventLoop::POLL_WRITABLE;
    }

    if (auto res = loop->startPoll(_pollId, pollEvents); !res) {
        return Err<void>("Failed to start poll", res);
    }

    ydebug("VNC client: registered poll id={} for socket={} events={}", _pollId, _socket, pollEvents);

    // If connect succeeded immediately, call onConnected callback now
    if (immediateConnect && onConnected) {
        ydebug("VNC client: calling onConnected (immediate connect)");
        onConnected();
    }

    return Ok();
#endif
}

void VncClient::disconnect() {
    _connected = false;
    _connecting = false;

#ifdef __EMSCRIPTEN__
    if (_wsSocket > 0) {
        emscripten_websocket_close(_wsSocket, 1000, "disconnect");
        emscripten_websocket_delete(_wsSocket);
        _wsSocket = 0;
        _wsConnected = false;
    }
#else
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
#endif
}

void VncClient::setReconnectParams(const std::string& host, uint16_t port) {
    _reconnectHost = host;
    _reconnectPort = port;
}

Result<void> VncClient::reconnect() {
    if (_reconnectHost.empty()) {
        return Err<void>("No reconnection parameters set");
    }
    yinfo("VNC client reconnecting to {}:{}", _reconnectHost, _reconnectPort);
    _wantsReconnect = false;

    // Reset receive state
    _recvState = RecvState::FRAME_HEADER;
    _recvNeeded = sizeof(FrameHeader);
    _recvOffset = 0;
    _recvBuffer.resize(_recvNeeded);

    // Clear send queue
    _sendQueue.clear();
    _sendOffset = 0;

    return connect(_reconnectHost, _reconnectPort);
}

#ifndef __EMSCRIPTEN__
Result<bool> VncClient::onEvent(const base::Event& event) {
    if (event.poll.fd != _socket) {
        return Ok(false);
    }

    // Handle async connect completion
    if (_connecting && event.type == base::Event::Type::PollWritable) {
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0) {
            yerror("VNC async connect failed: {}", error ? strerror(error) : "getsockopt failed");
            disconnect();
            return Ok(true);
        }
        _connecting = false;
        _connected = true;
        yinfo("VNC client connected (async complete)");
        // Switch to readable-only polling now that we're connected
        updatePollEvents();
        // Call onConnected callback - client should send resize here
        if (onConnected) {
            ydebug("VNC client: calling onConnected (async connect complete)");
            onConnected();
        }
        return Ok(true);
    }

    // Handle writable event - drain send queue
    if (event.type == base::Event::Type::PollWritable) {
        drainSendQueue();
        return Ok(true);
    }

    // Handle readable event
    if (event.type == base::Event::Type::PollReadable) {
        ydebug("VNC onEvent: PollReadable fd={}", event.poll.fd);
        onSocketReadable();
        return Ok(true);
    }

    return Ok(false);
}
#else
Result<bool> VncClient::onEvent(const base::Event& event) {
    // WebSocket uses callbacks, not event loop polling
    return Ok(false);
}
#endif

#ifndef __EMSCRIPTEN__
void VncClient::onSocketReadable() {
    if (!_connected || _socket < 0) {
        ywarn("VNC onSocketReadable: NOT CONNECTED! _connected={} _socket={}", _connected, _socket);
        return;
    }

    ydebug("VNC onSocketReadable: state={} offset={} needed={}", static_cast<int>(_recvState), _recvOffset, _recvNeeded);

    bool tilesReceived = false;
    int loopCount = 0;

    // Read as much as possible
    while (true) {
        loopCount++;
        if (loopCount > 10000) {
            yerror("VNC onSocketReadable: INFINITE LOOP DETECTED! Breaking out.");
            break;
        }
        ssize_t n = recv(_socket, _recvBuffer.data() + _recvOffset,
                         _recvNeeded - _recvOffset, MSG_DONTWAIT);

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No more data available - trigger render if we got tiles
                ydebug("VNC onSocketReadable: EAGAIN after {} loops, tilesReceived={}", loopCount, tilesReceived);
                if (tilesReceived && onFrameReceived) {
                    auto t = std::chrono::high_resolution_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
                    yinfo("[TIME] CLIENT frame received at {}ms, calling onFrameReceived", ms);
                    onFrameReceived();
                }
                return;
            }
            ywarn("VNC onSocketReadable: recv error: {} (errno={})", strerror(errno), errno);
            disconnect();
            return;
        }

        if (n == 0) {
            yinfo("VNC onSocketReadable: server closed connection");
            disconnect();
            return;
        }

        _recvOffset += n;
        ydebug("VNC onSocketReadable: recv {} bytes, offset now {}/{}", n, _recvOffset, _recvNeeded);

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

                // Update frame dimensions and ensure GPU resources
                if (_currentFrame.width != _width || _currentFrame.height != _height) {
                    _width = _currentFrame.width;
                    _height = _currentFrame.height;
                    yinfo("VNC: Frame size {}x{}", _width, _height);
                }
                // Ensure texture exists for immediate tile uploads
                ensureResources(_width, _height);

                _tilesReceived = 0;

                if (_currentFrame.num_tiles == 0) {
                    // No tiles, wait for next frame
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                } else if (_currentFrame.tile_size == 0) {
                    // Rectangle mode (merged tiles) - read RectHeader
                    _recvState = RecvState::RECT_HEADER;
                    _recvNeeded = sizeof(RectHeader);
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
                // Decode tile directly into temp buffer
                std::vector<uint8_t> pixels(TILE_SIZE * TILE_SIZE * 4);

                switch (static_cast<Encoding>(_currentTile.encoding)) {
                    case Encoding::RAW:
                        if (_currentTile.data_size == TILE_SIZE * TILE_SIZE * 4) {
                            std::memcpy(pixels.data(), _recvBuffer.data(), _currentTile.data_size);
                        }
                        break;

                    case Encoding::JPEG: {
                        int width, height, subsamp, colorspace;
                        if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                               _recvBuffer.data(), _currentTile.data_size,
                                               &width, &height, &subsamp, &colorspace) == 0) {
                            if (width <= TILE_SIZE && height <= TILE_SIZE) {
                                tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                             _recvBuffer.data(), _currentTile.data_size,
                                             pixels.data(), TILE_SIZE, 0, TILE_SIZE,
                                             TJPF_BGRA, TJFLAG_FASTDCT);
                            }
                        }
                        break;
                    }

                    case Encoding::RLE:
                        break;

                    case Encoding::FULL_FRAME: {
                        // Full frame JPEG - decompress entire frame
                        int width, height, subsamp, colorspace;
                        if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                               _recvBuffer.data(), _currentTile.data_size,
                                               &width, &height, &subsamp, &colorspace) == 0) {
                            if (width == static_cast<int>(_width) && height == static_cast<int>(_height)) {
                                // Decompress directly into full-frame buffer
                                std::vector<uint8_t> fullFrame(_width * _height * 4);
                                tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                             _recvBuffer.data(), _currentTile.data_size,
                                             fullFrame.data(), _width, 0, _height,
                                             TJPF_BGRA, TJFLAG_FASTDCT);

                                // Upload entire frame to GPU
                                if (_texture) {
                                    WGPUTexelCopyTextureInfo dst = {};
                                    dst.texture = _texture;
                                    dst.origin = {0, 0, 0};

                                    WGPUTexelCopyBufferLayout layout = {};
                                    layout.bytesPerRow = _width * 4;
                                    layout.rowsPerImage = _height;

                                    WGPUExtent3D size = {_width, _height, 1};
                                    wgpuQueueWriteTexture(_queue, &dst, fullFrame.data(), fullFrame.size(), &layout, &size);
                                    ydebug("VNC FULL_FRAME: {}x{} ({} bytes)", _width, _height, _currentTile.data_size);
                                }
                            }
                        }
                        tilesReceived = true;
                        _tilesReceived++;
                        if (_tilesReceived >= _currentFrame.num_tiles) {
                            _recvState = RecvState::FRAME_HEADER;
                            _recvNeeded = sizeof(FrameHeader);
                            _recvOffset = 0;
                            _recvBuffer.resize(_recvNeeded);
                            sendFrameAck();  // Flow control: notify server
                        }
                        break;
                    }
                }

                // Upload tile IMMEDIATELY to GPU (skip for FULL_FRAME - already handled)
                if (static_cast<Encoding>(_currentTile.encoding) == Encoding::FULL_FRAME) {
                    break;  // Already uploaded full frame above
                }
                if (_texture && _width > 0 && _height > 0) {
                    uint32_t px = _currentTile.tile_x * TILE_SIZE;
                    uint32_t py = _currentTile.tile_y * TILE_SIZE;
                    if (px < _width && py < _height) {
                        uint32_t tw = std::min((uint32_t)TILE_SIZE, _width - px);
                        uint32_t th = std::min((uint32_t)TILE_SIZE, _height - py);

                        WGPUTexelCopyTextureInfo dst = {};
                        dst.texture = _texture;
                        dst.origin = {px, py, 0};

                        WGPUTexelCopyBufferLayout layout = {};
                        layout.bytesPerRow = TILE_SIZE * 4;
                        layout.rowsPerImage = TILE_SIZE;

                        WGPUExtent3D size = {tw, th, 1};
                        wgpuQueueWriteTexture(_queue, &dst, pixels.data(), pixels.size(), &layout, &size);
                        ydebug("VNC TILE UPLOAD: tile({},{}) px({},{}) size({}x{}) texSize({}x{})",
                              _currentTile.tile_x, _currentTile.tile_y, px, py, tw, th, _textureWidth, _textureHeight);
                    } else {
                        ywarn("VNC TILE SKIP: tile({},{}) px({},{}) OUT OF BOUNDS frame({}x{})",
                              _currentTile.tile_x, _currentTile.tile_y, px, py, _width, _height);
                    }
                } else {
                    ywarn("VNC TILE SKIP: no texture! _texture={} _width={} _height={}", (void*)_texture, _width, _height);
                }

                tilesReceived = true;
                _tilesReceived++;

                if (_tilesReceived >= _currentFrame.num_tiles) {
                    // All tiles received, wait for next frame
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                    sendFrameAck();  // Flow control: notify server
                } else {
                    // Read next tile header
                    _recvState = RecvState::TILE_HEADER;
                    _recvNeeded = sizeof(TileHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                }
                break;
            }

            case RecvState::RECT_HEADER: {
                std::memcpy(&_currentRect, _recvBuffer.data(), sizeof(RectHeader));

                // Sanity check
                if (_currentRect.data_size > 16 * 1024 * 1024) {
                    ywarn("VNC: Rect data too large: {}", _currentRect.data_size);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                    return;
                }

                // Read rectangle data
                _recvState = RecvState::RECT_DATA;
                _recvNeeded = _currentRect.data_size;
                _recvOffset = 0;
                _recvBuffer.resize(_recvNeeded);
                break;
            }

            case RecvState::RECT_DATA: {
                // Decode rectangle
                uint16_t px = _currentRect.px_x;
                uint16_t py = _currentRect.px_y;
                uint16_t rectW = _currentRect.width;
                uint16_t rectH = _currentRect.height;

                std::vector<uint8_t> pixels(rectW * rectH * 4);

                switch (static_cast<Encoding>(_currentRect.encoding)) {
                    case Encoding::RECT_RAW:
                        if (_currentRect.data_size == static_cast<uint32_t>(rectW * rectH * 4)) {
                            std::memcpy(pixels.data(), _recvBuffer.data(), _currentRect.data_size);
                        }
                        break;

                    case Encoding::RECT_JPEG: {
                        int width, height, subsamp, colorspace;
                        if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                               _recvBuffer.data(), _currentRect.data_size,
                                               &width, &height, &subsamp, &colorspace) == 0) {
                            if (width == rectW && height == rectH) {
                                tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                             _recvBuffer.data(), _currentRect.data_size,
                                             pixels.data(), rectW, 0, rectH,
                                             TJPF_BGRA, TJFLAG_FASTDCT);
                            }
                        }
                        break;
                    }

                    default:
                        ywarn("VNC: Unknown rect encoding {}", _currentRect.encoding);
                        break;
                }

                // Upload rectangle to GPU
                if (_texture && _width > 0 && _height > 0) {
                    if (px < _width && py < _height) {
                        uint32_t uploadW = std::min(static_cast<uint32_t>(rectW), static_cast<uint32_t>(_width - px));
                        uint32_t uploadH = std::min(static_cast<uint32_t>(rectH), static_cast<uint32_t>(_height - py));

                        WGPUTexelCopyTextureInfo dst = {};
                        dst.texture = _texture;
                        dst.origin = {px, py, 0};

                        WGPUTexelCopyBufferLayout layout = {};
                        layout.bytesPerRow = rectW * 4;
                        layout.rowsPerImage = rectH;

                        WGPUExtent3D size = {uploadW, uploadH, 1};
                        wgpuQueueWriteTexture(_queue, &dst, pixels.data(), pixels.size(), &layout, &size);
                        ydebug("VNC RECT UPLOAD: pos({},{}) size({}x{})", px, py, uploadW, uploadH);
                    }
                }

                tilesReceived = true;
                _tilesReceived++;

                if (_tilesReceived >= _currentFrame.num_tiles) {
                    // All rectangles received, wait for next frame
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                    sendFrameAck();  // Flow control: notify server
                } else {
                    // Read next rectangle header
                    _recvState = RecvState::RECT_HEADER;
                    _recvNeeded = sizeof(RectHeader);
                    _recvOffset = 0;
                    _recvBuffer.resize(_recvNeeded);
                }
                break;
            }
        }
    }
}
#endif  // !__EMSCRIPTEN__

Result<void> VncClient::ensureResources(uint16_t width, uint16_t height) {
    if (_texture && _textureWidth == width && _textureHeight == height) {
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

    _textureWidth = width;
    _textureHeight = height;

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

    // Set viewport to VNC content area (top portion of window, excluding client's statusbar)
    // The VNC texture size (_width x _height) matches this area exactly - no stretching
    if (_width > 0 && _height > 0) {
        wgpuRenderPassEncoderSetViewport(pass, 0, 0,
            static_cast<float>(_width), static_cast<float>(_height), 0.0f, 1.0f);
        wgpuRenderPassEncoderSetScissorRect(pass, 0, 0, _width, _height);
    }

    wgpuRenderPassEncoderSetPipeline(pass, _pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, _bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);
    ydebug("VNC render: draw complete, viewport={}x{}", _width, _height);

    return Ok();
}

void VncClient::sendInput(const void* data, size_t size) {
#ifdef __EMSCRIPTEN__
    if (!_connected || !_wsConnected || _wsSocket <= 0) {
        ywarn("VNC client sendInput: not connected!");
        return;
    }

    // Send binary data via WebSocket
    EMSCRIPTEN_RESULT result = emscripten_websocket_send_binary(_wsSocket, const_cast<void*>(data), size);
    if (result != EMSCRIPTEN_RESULT_SUCCESS) {
        ywarn("VNC client sendInput: WebSocket send failed: {}", result);
    } else {
        ydebug("VNC client sendInput: sent {} bytes via WebSocket", size);
    }
#else
    if (!_connected || _socket < 0) {
        ywarn("VNC client sendInput: not connected! _socket={}", _socket);
        return;
    }

    const uint8_t* ptr = static_cast<const uint8_t*>(data);

    // If send queue is empty, try to send directly first
    if (_sendQueue.empty()) {
        ssize_t sent = send(_socket, ptr, size, MSG_NOSIGNAL | MSG_DONTWAIT);
        if (sent > 0) {
            if (static_cast<size_t>(sent) == size) {
                ydebug("VNC client sendInput: sent {} bytes directly", sent);
                return;  // All sent!
            }
            // Partial send - queue the rest
            ptr += sent;
            size -= sent;
        } else if (sent < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            ywarn("VNC client sendInput: send failed errno={}", errno);
            return;
        }
        // EAGAIN/EWOULDBLOCK - queue the data
    }

    // Queue remaining data for async send
    _sendQueue.insert(_sendQueue.end(), ptr, ptr + size);
    ydebug("VNC client sendInput: queued {} bytes, queue size now {}", size, _sendQueue.size());

    // Enable writable polling to drain queue
    updatePollEvents();
#endif
}

#ifndef __EMSCRIPTEN__
void VncClient::drainSendQueue() {
    if (_sendQueue.empty() || _socket < 0) {
        return;
    }

    while (_sendOffset < _sendQueue.size()) {
        size_t remaining = _sendQueue.size() - _sendOffset;
        ssize_t sent = send(_socket, _sendQueue.data() + _sendOffset, remaining, MSG_NOSIGNAL | MSG_DONTWAIT);

        if (sent <= 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Can't send more right now, wait for next writable event
                ydebug("VNC drainSendQueue: EAGAIN, {} bytes remaining", remaining);
                return;
            }
            ywarn("VNC drainSendQueue: send failed errno={}", errno);
            _sendQueue.clear();
            _sendOffset = 0;
            updatePollEvents();
            return;
        }

        _sendOffset += sent;
        ydebug("VNC drainSendQueue: sent {} bytes, {} remaining", sent, _sendQueue.size() - _sendOffset);
    }

    // All sent - clear queue and disable writable polling
    _sendQueue.clear();
    _sendOffset = 0;
    updatePollEvents();
    ydebug("VNC drainSendQueue: queue drained");
}

void VncClient::updatePollEvents() {
    if (_pollId < 0) return;

    auto loopResult = base::EventLoop::instance();
    if (!loopResult) return;
    auto loop = *loopResult;

    // Always poll readable when connected
    int events = base::EventLoop::POLL_READABLE;

    // Poll writable if:
    // 1. We're in async connect and need to detect completion
    // 2. We have data queued to send
    if (_connecting || !_sendQueue.empty()) {
        events |= base::EventLoop::POLL_WRITABLE;
    }

    loop->setPollEvents(_pollId, events);
}
#endif

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

void VncClient::sendCharWithMods(uint32_t codepoint, uint8_t mods) {
    ydebug("VNC client sendCharWithMods: codepoint={} ('{}') mods={}", codepoint, (char)codepoint, mods);
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::CHAR_WITH_MODS);
    hdr.data_size = sizeof(CharWithModsEvent);

    CharWithModsEvent evt = {};
    evt.codepoint = codepoint;
    evt.mods = mods;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

void VncClient::sendTextInput(const char* text, size_t len) {
    if (len == 0 || len > 1024) return; // Sanity check

    auto t = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
    std::string textStr(text, len);
    yinfo("[TIME] CLIENT sendTextInput at {}ms: '{}'", ms, textStr);

    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::TEXT_INPUT);
    hdr.data_size = static_cast<uint16_t>(len);

    std::vector<uint8_t> buf(sizeof(hdr) + len);
    std::memcpy(buf.data(), &hdr, sizeof(hdr));
    std::memcpy(buf.data() + sizeof(hdr), text, len);
    sendInput(buf.data(), buf.size());
}

void VncClient::sendResize(uint16_t width, uint16_t height) {
    ydebug("VNC client sendResize: {}x{}", width, height);
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
    ydebug("VNC client sendCellSize: cellHeight={}", (int)cellHeight);
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

void VncClient::sendFrameAck() {
    ydebug("VNC client sendFrameAck");
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::FRAME_ACK);
    hdr.data_size = 0;  // No payload

    sendInput(&hdr, sizeof(hdr));
}

void VncClient::sendCompressionConfig(bool forceRaw, uint8_t quality) {
    yinfo("VNC client sendCompressionConfig: forceRaw={} quality={}", forceRaw, quality);
    InputHeader hdr = {};
    hdr.type = static_cast<uint8_t>(InputType::COMPRESSION_CONFIG);
    hdr.data_size = sizeof(CompressionConfigEvent);

    CompressionConfigEvent evt = {};
    evt.forceRaw = forceRaw ? 1 : 0;
    evt.quality = quality;

    uint8_t buf[sizeof(hdr) + sizeof(evt)];
    std::memcpy(buf, &hdr, sizeof(hdr));
    std::memcpy(buf + sizeof(hdr), &evt, sizeof(evt));
    sendInput(buf, sizeof(buf));
}

#ifdef __EMSCRIPTEN__
void VncClient::onWebSocketData(const uint8_t* data, size_t size) {
    ydebug("VNC WebSocket received {} bytes", size);

    // Append incoming data to receive buffer
    size_t dataOffset = 0;
    while (dataOffset < size) {
        // How much more data do we need?
        size_t toRead = _recvNeeded - _recvOffset;
        size_t available = size - dataOffset;
        size_t toCopy = std::min(toRead, available);

        // Copy to buffer
        if (_recvBuffer.size() < _recvNeeded) {
            _recvBuffer.resize(_recvNeeded);
        }
        std::memcpy(_recvBuffer.data() + _recvOffset, data + dataOffset, toCopy);
        _recvOffset += toCopy;
        dataOffset += toCopy;

        // Check if we have enough data
        if (_recvOffset < _recvNeeded) {
            continue;  // Need more data
        }

        // Process based on current state - reuse the logic from onSocketReadable
        // but inline it here for WebSocket
        switch (_recvState) {
            case RecvState::FRAME_HEADER: {
                std::memcpy(&_currentFrame, _recvBuffer.data(), sizeof(FrameHeader));

                if (_currentFrame.magic != FRAME_MAGIC) {
                    ywarn("VNC: Invalid frame magic 0x{:08X}", _currentFrame.magic);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    return;
                }

                if (_currentFrame.width == 0 || _currentFrame.height == 0 ||
                    _currentFrame.width > 8192 || _currentFrame.height > 8192) {
                    ywarn("VNC: Invalid frame header {}x{}", _currentFrame.width, _currentFrame.height);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    return;
                }

                if (_currentFrame.width != _width || _currentFrame.height != _height) {
                    _width = _currentFrame.width;
                    _height = _currentFrame.height;
                    yinfo("VNC: Frame size {}x{}", _width, _height);
                }
                ensureResources(_width, _height);

                _tilesReceived = 0;

                if (_currentFrame.num_tiles == 0) {
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                } else if (_currentFrame.tile_size == 0) {
                    _recvState = RecvState::RECT_HEADER;
                    _recvNeeded = sizeof(RectHeader);
                    _recvOffset = 0;
                } else {
                    _recvState = RecvState::TILE_HEADER;
                    _recvNeeded = sizeof(TileHeader);
                    _recvOffset = 0;
                }
                break;
            }

            case RecvState::TILE_HEADER: {
                std::memcpy(&_currentTile, _recvBuffer.data(), sizeof(TileHeader));

                if (_currentTile.data_size > 16 * 1024 * 1024) {
                    ywarn("VNC: Tile data too large: {}", _currentTile.data_size);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    return;
                }

                _recvState = RecvState::TILE_DATA;
                _recvNeeded = _currentTile.data_size;
                _recvOffset = 0;
                _recvBuffer.resize(_recvNeeded);
                break;
            }

            case RecvState::TILE_DATA: {
                // Decode and upload tile (simplified - full implementation in onSocketReadable)
                std::vector<uint8_t> pixels(TILE_SIZE * TILE_SIZE * 4);

                if (static_cast<Encoding>(_currentTile.encoding) == Encoding::FULL_FRAME) {
                    // Full frame JPEG
                    int width, height, subsamp, colorspace;
                    if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                           _recvBuffer.data(), _currentTile.data_size,
                                           &width, &height, &subsamp, &colorspace) == 0) {
                        if (width == static_cast<int>(_width) && height == static_cast<int>(_height)) {
                            std::vector<uint8_t> fullFrame(_width * _height * 4);
                            tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                         _recvBuffer.data(), _currentTile.data_size,
                                         fullFrame.data(), _width, 0, _height,
                                         TJPF_BGRA, TJFLAG_FASTDCT);

                            if (_texture) {
                                WGPUTexelCopyTextureInfo dst = {};
                                dst.texture = _texture;
                                dst.origin = {0, 0, 0};
                                WGPUTexelCopyBufferLayout layout = {};
                                layout.bytesPerRow = _width * 4;
                                layout.rowsPerImage = _height;
                                WGPUExtent3D sz = {_width, _height, 1};
                                wgpuQueueWriteTexture(_queue, &dst, fullFrame.data(), fullFrame.size(), &layout, &sz);
                            }
                        }
                    }
                } else if (static_cast<Encoding>(_currentTile.encoding) == Encoding::RAW) {
                    if (_currentTile.data_size == TILE_SIZE * TILE_SIZE * 4) {
                        std::memcpy(pixels.data(), _recvBuffer.data(), _currentTile.data_size);
                    }
                } else if (static_cast<Encoding>(_currentTile.encoding) == Encoding::JPEG) {
                    int width, height, subsamp, colorspace;
                    if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                           _recvBuffer.data(), _currentTile.data_size,
                                           &width, &height, &subsamp, &colorspace) == 0) {
                        if (width <= TILE_SIZE && height <= TILE_SIZE) {
                            tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                         _recvBuffer.data(), _currentTile.data_size,
                                         pixels.data(), TILE_SIZE, 0, TILE_SIZE,
                                         TJPF_BGRA, TJFLAG_FASTDCT);
                        }
                    }
                }

                // Upload tile to texture (skip for FULL_FRAME)
                if (static_cast<Encoding>(_currentTile.encoding) != Encoding::FULL_FRAME &&
                    _texture && _width > 0 && _height > 0) {
                    uint32_t px = _currentTile.tile_x * TILE_SIZE;
                    uint32_t py = _currentTile.tile_y * TILE_SIZE;
                    if (px < _width && py < _height) {
                        uint32_t tw = std::min((uint32_t)TILE_SIZE, (uint32_t)(_width - px));
                        uint32_t th = std::min((uint32_t)TILE_SIZE, (uint32_t)(_height - py));

                        WGPUTexelCopyTextureInfo dst = {};
                        dst.texture = _texture;
                        dst.origin = {px, py, 0};
                        WGPUTexelCopyBufferLayout layout = {};
                        layout.bytesPerRow = TILE_SIZE * 4;
                        layout.rowsPerImage = TILE_SIZE;
                        WGPUExtent3D sz = {tw, th, 1};
                        wgpuQueueWriteTexture(_queue, &dst, pixels.data(), pixels.size(), &layout, &sz);
                    }
                }

                _tilesReceived++;
                if (_tilesReceived >= _currentFrame.num_tiles) {
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    sendFrameAck();
                    if (onFrameReceived) {
                        onFrameReceived();
                    }
                } else {
                    _recvState = RecvState::TILE_HEADER;
                    _recvNeeded = sizeof(TileHeader);
                    _recvOffset = 0;
                }
                break;
            }

            case RecvState::RECT_HEADER: {
                std::memcpy(&_currentRect, _recvBuffer.data(), sizeof(RectHeader));

                if (_currentRect.data_size > 16 * 1024 * 1024) {
                    ywarn("VNC: Rect data too large: {}", _currentRect.data_size);
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    return;
                }

                _recvState = RecvState::RECT_DATA;
                _recvNeeded = _currentRect.data_size;
                _recvOffset = 0;
                _recvBuffer.resize(_recvNeeded);
                break;
            }

            case RecvState::RECT_DATA: {
                uint16_t px = _currentRect.px_x;
                uint16_t py = _currentRect.px_y;
                uint16_t rectW = _currentRect.width;
                uint16_t rectH = _currentRect.height;

                std::vector<uint8_t> pixels(rectW * rectH * 4);

                if (static_cast<Encoding>(_currentRect.encoding) == Encoding::RECT_RAW) {
                    if (_currentRect.data_size == static_cast<uint32_t>(rectW * rectH * 4)) {
                        std::memcpy(pixels.data(), _recvBuffer.data(), _currentRect.data_size);
                    }
                } else if (static_cast<Encoding>(_currentRect.encoding) == Encoding::RECT_JPEG) {
                    int width, height, subsamp, colorspace;
                    if (tjDecompressHeader3(static_cast<tjhandle>(_jpegDecompressor),
                                           _recvBuffer.data(), _currentRect.data_size,
                                           &width, &height, &subsamp, &colorspace) == 0) {
                        if (width == rectW && height == rectH) {
                            tjDecompress2(static_cast<tjhandle>(_jpegDecompressor),
                                         _recvBuffer.data(), _currentRect.data_size,
                                         pixels.data(), rectW, 0, rectH,
                                         TJPF_BGRA, TJFLAG_FASTDCT);
                        }
                    }
                }

                // Upload rectangle to texture
                if (_texture && _width > 0 && _height > 0) {
                    if (px < _width && py < _height) {
                        uint32_t uploadW = std::min(static_cast<uint32_t>(rectW), static_cast<uint32_t>(_width - px));
                        uint32_t uploadH = std::min(static_cast<uint32_t>(rectH), static_cast<uint32_t>(_height - py));

                        WGPUTexelCopyTextureInfo dst = {};
                        dst.texture = _texture;
                        dst.origin = {px, py, 0};
                        WGPUTexelCopyBufferLayout layout = {};
                        layout.bytesPerRow = rectW * 4;
                        layout.rowsPerImage = rectH;
                        WGPUExtent3D sz = {uploadW, uploadH, 1};
                        wgpuQueueWriteTexture(_queue, &dst, pixels.data(), pixels.size(), &layout, &sz);
                    }
                }

                _tilesReceived++;
                if (_tilesReceived >= _currentFrame.num_tiles) {
                    _recvState = RecvState::FRAME_HEADER;
                    _recvNeeded = sizeof(FrameHeader);
                    _recvOffset = 0;
                    sendFrameAck();
                    if (onFrameReceived) {
                        onFrameReceived();
                    }
                } else {
                    _recvState = RecvState::RECT_HEADER;
                    _recvNeeded = sizeof(RectHeader);
                    _recvOffset = 0;
                }
                break;
            }
        }
        _recvBuffer.resize(_recvNeeded);
    }
}
#endif

} // namespace yetty::vnc
