#pragma once

#include "protocol.h"
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

namespace yetty::vnc {

class VncClient {
public:
    VncClient(WGPUDevice device, WGPUQueue queue);
    ~VncClient();

    // Connect to server
    Result<void> connect(const std::string& host, uint16_t port);
    void disconnect();
    bool isConnected() const { return _connected; }

    // Get current frame dimensions
    uint16_t width() const { return _width; }
    uint16_t height() const { return _height; }

    // Update texture with received tiles (call from main thread)
    Result<void> updateTexture();

    // Render the frame (fullscreen quad)
    Result<void> render(WGPURenderPassEncoder pass);

    // Get the texture view for external rendering
    WGPUTextureView getTextureView() const { return _textureView; }

private:
    void receiveLoop();
    Result<void> processFrame(const FrameHeader& header, const uint8_t* data, size_t size);
    Result<void> decodeTile(const TileHeader& tile, const uint8_t* data);
    Result<void> ensureResources(uint16_t width, uint16_t height);
    Result<void> createPipeline();

    WGPUDevice _device;
    WGPUQueue _queue;

    // Network
    int _socket = -1;
    std::atomic<bool> _connected{false};
    std::thread _receiveThread;

    // Frame state
    uint16_t _width = 0;
    uint16_t _height = 0;
    std::vector<uint8_t> _pixels;  // CPU-side pixel buffer

    // Pending tile updates (thread-safe queue)
    struct TileUpdate {
        uint16_t tile_x, tile_y;
        std::vector<uint8_t> pixels;  // TILE_SIZE * TILE_SIZE * 4
    };
    std::mutex _tileMutex;
    std::queue<TileUpdate> _pendingTiles;

    // GPU resources
    WGPUTexture _texture = nullptr;
    WGPUTextureView _textureView = nullptr;
    WGPUSampler _sampler = nullptr;
    WGPUBindGroup _bindGroup = nullptr;
    WGPUBindGroupLayout _bindGroupLayout = nullptr;
    WGPURenderPipeline _pipeline = nullptr;
    WGPUBuffer _vertexBuffer = nullptr;
};

} // namespace yetty::vnc
