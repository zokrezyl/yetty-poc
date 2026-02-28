#pragma once

#include "protocol.h"
#include <yetty/result.hpp>
#include <webgpu/webgpu.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include <unordered_map>

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

    // Poll for incoming input events (call from main thread)
    // Returns true if there are pending events
    bool hasPendingInput() const;

    // Input event callbacks (set these to receive input from clients)
    std::function<void(int16_t x, int16_t y)> onMouseMove;
    std::function<void(int16_t x, int16_t y, MouseButton button, bool pressed)> onMouseButton;
    std::function<void(int16_t x, int16_t y, int16_t dx, int16_t dy)> onMouseScroll;
    std::function<void(uint32_t keycode, uint32_t scancode, uint8_t mods)> onKeyDown;
    std::function<void(uint32_t keycode, uint32_t scancode, uint8_t mods)> onKeyUp;
    std::function<void(const std::string& text)> onTextInput;

    // Process all pending input events (call from main thread)
    void processInput();

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

    // Async capture state machine
    enum class CaptureState { IDLE, WAITING_QUEUE, WAITING_MAP };
    CaptureState _captureState = CaptureState::IDLE;
    std::atomic<bool> _queueDone{false};
    std::atomic<bool> _mapDone{false};
    WGPUMapAsyncStatus _mapStatus = WGPUMapAsyncStatus_Success;
    uint32_t _pendingWidth = 0;
    uint32_t _pendingHeight = 0;

    // JPEG compression
    void* _jpegCompressor = nullptr;

    // Dirty tile tracking
    std::vector<bool> _dirtyTiles;
    uint16_t _tilesX = 0;
    uint16_t _tilesY = 0;

    Result<void> ensureResources(uint32_t width, uint32_t height);
    void detectDirtyTiles();
    Result<void> encodeTile(uint16_t tx, uint16_t ty, std::vector<uint8_t>& outData, Encoding& outEncoding);

    // Input receiving (non-blocking, called from main thread)
    void pollClientInput(int clientFd);
    void dispatchInput(const InputHeader& hdr, const uint8_t* data);

    // Per-client input buffer for partial reads
    struct ClientInputBuffer {
        std::vector<uint8_t> buffer;
        size_t needed = sizeof(InputHeader);
        bool readingHeader = true;
        InputHeader header;
    };
    std::unordered_map<int, ClientInputBuffer> _clientInputBuffers;
};

} // namespace yetty::vnc
