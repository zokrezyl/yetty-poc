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

namespace yetty::vnc {

using yetty::Result;
using yetty::Ok;
using yetty::Err;

VncServer::VncServer(WGPUDevice device, WGPUQueue queue)
    : _device(device), _queue(queue) {
    _jpegCompressor = tjInitCompress();
}

VncServer::~VncServer() {
    stop();
    if (_readbackBuffer) wgpuBufferRelease(_readbackBuffer);
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
    while (remaining > 0) {
        ssize_t sent = send(clientFd, ptr, remaining, MSG_NOSIGNAL | MSG_DONTWAIT);
        if (sent <= 0) {
            if (sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                // Buffer full, skip this frame for this client
                return Ok();
            }
            return Err<void>("Send failed");
        }
        ptr += sent;
        remaining -= sent;
    }
    return Ok();
}

Result<void> VncServer::ensureResources(uint32_t width, uint32_t height) {
    if (_lastWidth == width && _lastHeight == height && _readbackBuffer) {
        return Ok();
    }

    // Release old buffer
    if (_readbackBuffer) {
        wgpuBufferRelease(_readbackBuffer);
        _readbackBuffer = nullptr;
    }

    _lastWidth = width;
    _lastHeight = height;

    // Calculate aligned size
    uint32_t bytesPerPixel = 4;
    uint32_t alignedBytesPerRow = (width * bytesPerPixel + 255) & ~255;
    _readbackSize = alignedBytesPerRow * height;

    // Create readback buffer
    WGPUBufferDescriptor bufDesc = {};
    bufDesc.size = _readbackSize;
    bufDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_MapRead;
    _readbackBuffer = wgpuDeviceCreateBuffer(_device, &bufDesc);
    if (!_readbackBuffer) return Err<void>("Failed to create readback buffer");

    // Resize pixel buffers
    _pixels.resize(width * height * bytesPerPixel);
    _prevPixels.resize(width * height * bytesPerPixel);
    std::memset(_prevPixels.data(), 0, _prevPixels.size());

    // Setup tile tracking
    _tilesX = tiles_x(width);
    _tilesY = tiles_y(height);
    _dirtyTiles.resize(_tilesX * _tilesY);

    yinfo("VNC server resources: {}x{}, {} tiles", width, height, _tilesX * _tilesY);
    return Ok();
}

void VncServer::detectDirtyTiles() {
    // Check if we need periodic full refresh
    _framesSinceFullRefresh++;
    if (_framesSinceFullRefresh >= FULL_REFRESH_INTERVAL) {
        _forceFullFrame = true;
        _framesSinceFullRefresh = 0;
    }

    // Force full frame: mark all tiles dirty
    if (_forceFullFrame) {
        std::fill(_dirtyTiles.begin(), _dirtyTiles.end(), true);
        _forceFullFrame = false;
        return;
    }

    // Compare current pixels to previous, mark dirty tiles
    std::fill(_dirtyTiles.begin(), _dirtyTiles.end(), false);

    uint32_t bytesPerPixel = 4;
    uint32_t stride = _lastWidth * bytesPerPixel;

    for (uint16_t ty = 0; ty < _tilesY; ty++) {
        for (uint16_t tx = 0; tx < _tilesX; tx++) {
            uint32_t startX = tx * TILE_SIZE;
            uint32_t startY = ty * TILE_SIZE;
            uint32_t endX = std::min(startX + TILE_SIZE, _lastWidth);
            uint32_t endY = std::min(startY + TILE_SIZE, _lastHeight);

            bool dirty = false;
            for (uint32_t y = startY; y < endY && !dirty; y++) {
                const uint8_t* curr = _pixels.data() + y * stride + startX * bytesPerPixel;
                const uint8_t* prev = _prevPixels.data() + y * stride + startX * bytesPerPixel;
                uint32_t rowBytes = (endX - startX) * bytesPerPixel;
                if (std::memcmp(curr, prev, rowBytes) != 0) {
                    dirty = true;
                }
            }
            _dirtyTiles[ty * _tilesX + tx] = dirty;
        }
    }
}

Result<void> VncServer::encodeTile(uint16_t tx, uint16_t ty,
                                    std::vector<uint8_t>& outData, Encoding& outEncoding) {
    uint32_t startX = tx * TILE_SIZE;
    uint32_t startY = ty * TILE_SIZE;
    uint32_t tileW = std::min((uint32_t)TILE_SIZE, _lastWidth - startX);
    uint32_t tileH = std::min((uint32_t)TILE_SIZE, _lastHeight - startY);

    // Extract tile pixels
    std::vector<uint8_t> tilePixels(TILE_SIZE * TILE_SIZE * 4);
    uint32_t srcStride = _lastWidth * 4;
    for (uint32_t y = 0; y < tileH; y++) {
        const uint8_t* src = _pixels.data() + (startY + y) * srcStride + startX * 4;
        uint8_t* dst = tilePixels.data() + y * TILE_SIZE * 4;
        std::memcpy(dst, src, tileW * 4);
    }

    // Check if tile is simple (solid color or few colors)
    // For now, use heuristic: if raw size is small enough, send raw
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
        TJSAMP_420, 80,  // Quality 80
        TJFLAG_FASTDCT
    );

    if (result == 0 && jpegSize < rawSize * 0.8) {
        // JPEG is smaller, use it
        outData.assign(jpegBuf, jpegBuf + jpegSize);
        outEncoding = Encoding::JPEG;
        tjFree(jpegBuf);
    } else {
        // Use RAW
        if (jpegBuf) tjFree(jpegBuf);
        outData = std::move(tilePixels);
        outEncoding = Encoding::RAW;
    }

    return Ok();
}

Result<void> VncServer::sendFrame(WGPUTexture texture, uint32_t width, uint32_t height) {
    if (_clientCount == 0) return Ok();  // No clients, skip

    // Single tick to process any pending callbacks
    wgpuDeviceTick(_device);

    // State machine for async capture
    ydebug("VNC sendFrame: state={} queueDone={} mapDone={}",
           static_cast<int>(_captureState), _queueDone.load(), _mapDone.load());
    switch (_captureState) {
        case CaptureState::IDLE: {
            // Start new capture
            if (auto res = ensureResources(width, height); !res) {
                return res;
            }

            uint32_t bytesPerPixel = 4;
            uint32_t alignedBytesPerRow = (width * bytesPerPixel + 255) & ~255;

            WGPUCommandEncoderDescriptor encDesc = {};
            WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(_device, &encDesc);

            WGPUTexelCopyTextureInfo src = {};
            src.texture = texture;

            WGPUTexelCopyBufferInfo dst = {};
            dst.buffer = _readbackBuffer;
            dst.layout.bytesPerRow = alignedBytesPerRow;
            dst.layout.rowsPerImage = height;

            WGPUExtent3D copySize = {width, height, 1};
            wgpuCommandEncoderCopyTextureToBuffer(encoder, &src, &dst, &copySize);

            WGPUCommandBufferDescriptor cmdDesc = {};
            WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(encoder, &cmdDesc);
            wgpuQueueSubmit(_queue, 1, &cmdBuf);
            wgpuCommandBufferRelease(cmdBuf);
            wgpuCommandEncoderRelease(encoder);

            // Register queue done callback
            _queueDone = false;
            WGPUQueueWorkDoneCallbackInfo queueCbInfo = {};
            queueCbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
            queueCbInfo.callback = [](WGPUQueueWorkDoneStatus, WGPUStringView, void* ud, void*) {
                auto* done = static_cast<std::atomic<bool>*>(ud);
                *done = true;
            };
            queueCbInfo.userdata1 = &_queueDone;
            wgpuQueueOnSubmittedWorkDone(_queue, queueCbInfo);

            _pendingWidth = width;
            _pendingHeight = height;
            _captureState = CaptureState::WAITING_QUEUE;
            return Ok();  // Will process on next frame
        }

        case CaptureState::WAITING_QUEUE: {
            if (!_queueDone) {
                return Ok();  // Still waiting
            }

            // Queue done, start mapping
            _mapDone = false;
            WGPUBufferMapCallbackInfo cbInfo = {};
            cbInfo.mode = WGPUCallbackMode_AllowSpontaneous;
            cbInfo.callback = [](WGPUMapAsyncStatus status, WGPUStringView, void* ud1, void* ud2) {
                auto* done = static_cast<std::atomic<bool>*>(ud1);
                auto* statusOut = static_cast<WGPUMapAsyncStatus*>(ud2);
                *statusOut = status;
                *done = true;
            };
            cbInfo.userdata1 = &_mapDone;
            cbInfo.userdata2 = &_mapStatus;
            wgpuBufferMapAsync(_readbackBuffer, WGPUMapMode_Read, 0, _readbackSize, cbInfo);

            _captureState = CaptureState::WAITING_MAP;
            return Ok();  // Will process on next frame
        }

        case CaptureState::WAITING_MAP: {
            if (!_mapDone) {
                return Ok();  // Still waiting
            }

            if (_mapStatus != WGPUMapAsyncStatus_Success) {
                ywarn("VNC buffer map failed, resetting");
                _captureState = CaptureState::IDLE;
                return Ok();
            }

            // Buffer is mapped, copy data
            uint32_t bytesPerPixel = 4;
            uint32_t alignedBytesPerRow = (_pendingWidth * bytesPerPixel + 255) & ~255;

            const uint8_t* mapped = static_cast<const uint8_t*>(
                wgpuBufferGetConstMappedRange(_readbackBuffer, 0, _readbackSize));

            uint32_t unalignedBytesPerRow = _pendingWidth * bytesPerPixel;
            for (uint32_t y = 0; y < _pendingHeight; y++) {
                std::memcpy(_pixels.data() + y * unalignedBytesPerRow,
                           mapped + y * alignedBytesPerRow,
                           unalignedBytesPerRow);
            }
            wgpuBufferUnmap(_readbackBuffer);

            _captureState = CaptureState::IDLE;
            break;  // Continue to send the frame
        }
    }

    // Data is ready, process and send
    // Detect dirty tiles
    detectDirtyTiles();

    // Count dirty tiles
    uint16_t numDirty = 0;
    for (bool d : _dirtyTiles) if (d) numDirty++;

    if (numDirty == 0) {
        // No changes, skip sending
        return Ok();
    }

    // Build frame data
    std::vector<uint8_t> frameData;
    frameData.reserve(64 * 1024);  // Pre-allocate

    // Frame header
    FrameHeader fh;
    fh.width = _pendingWidth;
    fh.height = _pendingHeight;
    fh.tile_size = TILE_SIZE;
    fh.num_tiles = numDirty;

    frameData.insert(frameData.end(),
                     reinterpret_cast<uint8_t*>(&fh),
                     reinterpret_cast<uint8_t*>(&fh) + sizeof(fh));

    // Encode and append dirty tiles
    {
        ytimeit("vnc-encode");
        for (uint16_t ty = 0; ty < _tilesY; ty++) {
            for (uint16_t tx = 0; tx < _tilesX; tx++) {
                if (!_dirtyTiles[ty * _tilesX + tx]) continue;

                std::vector<uint8_t> tileData;
                Encoding enc;
                if (auto res = encodeTile(tx, ty, tileData, enc); !res) {
                    continue;
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
    }

    // Save current frame as previous
    std::swap(_pixels, _prevPixels);

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

        ydebug("VNC pollClientInput: fd={} received {} bytes", clientFd, n);
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
    }
}

bool VncServer::hasPendingInput() const {
    return false;  // Input is processed immediately now
}

void VncServer::processInput() {
    // Poll all clients for input (non-blocking)
    std::lock_guard<std::mutex> lock(_clientsMutex);
    ydebug("VNC processInput: {} clients", _clients.size());
    for (int fd : _clients) {
        ydebug("VNC: polling client fd={}", fd);
        pollClientInput(fd);
    }
}

} // namespace yetty::vnc
