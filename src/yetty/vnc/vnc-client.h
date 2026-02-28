#pragma once

#include <yetty/result.hpp>
#include <yetty/base/event-loop.h>
#include <yetty/base/event-listener.h>
#include "protocol.h"
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <queue>

namespace yetty::vnc {

class VncClient : public base::EventListener {
public:
    using Ptr = std::shared_ptr<VncClient>;

    VncClient(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat surfaceFormat);
    ~VncClient() override;

    // Connect to server
    Result<void> connect(const std::string& host, uint16_t port);
    void disconnect();
    bool isConnected() const { return _connected; }

    // Get current frame dimensions
    uint16_t width() const { return _width; }
    uint16_t height() const { return _height; }

    // Update texture with received tiles (call from main thread)
    // Returns true if new tiles were processed
    Result<bool> updateTexture();

    // Render the frame (fullscreen quad)
    Result<void> render(WGPURenderPassEncoder pass);

    // Get the texture view for external rendering
    WGPUTextureView getTextureView() const { return _textureView; }

    // Input forwarding (call from main thread when events occur)
    void sendMouseMove(int16_t x, int16_t y);
    void sendMouseButton(int16_t x, int16_t y, MouseButton button, bool pressed);
    void sendMouseScroll(int16_t x, int16_t y, int16_t deltaX, int16_t deltaY);
    void sendKeyDown(uint32_t keycode, uint32_t scancode, uint8_t mods);
    void sendKeyUp(uint32_t keycode, uint32_t scancode, uint8_t mods);
    void sendTextInput(const char* text, size_t len);
    void sendResize(uint16_t width, uint16_t height);
    void sendCellSize(uint8_t cellHeight);

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override;

private:
    void sendInput(const void* data, size_t size);
    void onSocketReadable();
    Result<void> ensureResources(uint16_t width, uint16_t height);
    Result<void> createPipeline();

    WGPUDevice _device;
    WGPUQueue _queue;
    WGPUTextureFormat _surfaceFormat;

    // Network
    int _socket = -1;
    bool _connected = false;
    base::PollId _pollId = -1;

    // Async receive state machine
    enum class RecvState {
        FRAME_HEADER,   // Waiting for frame header
        TILE_HEADER,    // Waiting for tile header
        TILE_DATA       // Waiting for tile data
    };
    RecvState _recvState = RecvState::FRAME_HEADER;
    std::vector<uint8_t> _recvBuffer;
    size_t _recvOffset = 0;
    size_t _recvNeeded = 0;

    // Current frame being received
    FrameHeader _currentFrame;
    uint16_t _tilesReceived = 0;

    // Current tile being received
    TileHeader _currentTile;

    // JPEG decompressor (initialized once)
    void* _jpegDecompressor = nullptr;

    // Frame state
    uint16_t _width = 0;
    uint16_t _height = 0;
    std::vector<uint8_t> _pixels;  // CPU-side pixel buffer

    // Pending tile updates
    struct TileUpdate {
        uint16_t tile_x, tile_y;
        std::vector<uint8_t> pixels;  // TILE_SIZE * TILE_SIZE * 4
    };
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
