#pragma once

#include "protocol.h"
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

namespace yetty::vnc {

class VncServer {
public:
    VncServer(WGPUDevice device, WGPUQueue queue);
    ~VncServer();

    // Start listening on port
    Result<void> start(uint16_t port);
    void stop();
    bool isRunning() const { return _running; }
    bool hasClients() const { return _clientCount > 0; }

    // Capture current frame and send to all clients
    // Call this after rendering to the capture texture
    Result<void> sendFrame(WGPUTexture texture, uint32_t width, uint32_t height);

private:
    void acceptLoop();
    void clientLoop(int clientFd);
    Result<void> sendToClient(int clientFd, const void* data, size_t size);

    WGPUDevice _device;
    WGPUQueue _queue;

    // Server socket
    int _serverFd = -1;
    uint16_t _port = 0;
    std::atomic<bool> _running{false};
    std::thread _acceptThread;

    // Connected clients
    std::mutex _clientsMutex;
    std::vector<int> _clients;
    std::atomic<int> _clientCount{0};

    // Frame capture resources
    WGPUBuffer _readbackBuffer = nullptr;
    uint32_t _readbackSize = 0;
    uint32_t _lastWidth = 0;
    uint32_t _lastHeight = 0;
    std::vector<uint8_t> _pixels;
    std::vector<uint8_t> _prevPixels;  // For dirty detection

    // JPEG compression
    void* _jpegCompressor = nullptr;

    // Dirty tile tracking
    std::vector<bool> _dirtyTiles;
    uint16_t _tilesX = 0;
    uint16_t _tilesY = 0;

    Result<void> ensureResources(uint32_t width, uint32_t height);
    void detectDirtyTiles();
    Result<void> encodeTile(uint16_t tx, uint16_t ty, std::vector<uint8_t>& outData, Encoding& outEncoding);
};

} // namespace yetty::vnc
