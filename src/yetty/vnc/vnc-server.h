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
    void forceFullFrame() { _forceFullFrame = true; }

    // Statistics (updated per second)
    struct FrameStats {
        uint32_t tilesSent = 0;
        uint32_t tilesJpeg = 0;
        uint32_t tilesRaw = 0;
        uint32_t avgTileSize = 0;
        uint32_t fullUpdates = 0;
        uint32_t frames = 0;
        uint64_t bytesPerSec = 0;
    };
    FrameStats getStats() const;

    // Capture current frame and send to all clients
    // texture: current frame on GPU (for diff detection)
    // cpuPixels: same frame in CPU memory (for JPEG encoding), or nullptr to read back from GPU
    Result<void> sendFrame(WGPUTexture texture, const uint8_t* cpuPixels, uint32_t width, uint32_t height);

    // Overload for GPU-only rendering (will read back dirty tiles for encoding)
    Result<void> sendFrame(WGPUTexture texture, uint32_t width, uint32_t height) {
        return sendFrame(texture, nullptr, width, height);
    }

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
    std::function<void(uint16_t width, uint16_t height)> onResize;
    std::function<void(uint8_t cellHeight)> onCellSize;

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

    // Frame dimensions
    uint32_t _lastWidth = 0;
    uint32_t _lastHeight = 0;

    // GPU tile diff resources
    WGPUTexture _prevTexture = nullptr;      // Previous frame for comparison
    WGPUBuffer _dirtyFlagsBuffer = nullptr;  // Output: 1 uint per tile (dirty=1, clean=0)
    WGPUBuffer _dirtyFlagsReadback = nullptr; // CPU-readable copy
    WGPUComputePipeline _diffPipeline = nullptr;
    WGPUBindGroup _diffBindGroup = nullptr;
    WGPUBindGroupLayout _diffBindGroupLayout = nullptr;

    // CPU framebuffer for encoding (passed in from caller or read back from GPU)
    const uint8_t* _cpuPixels = nullptr;
    uint32_t _cpuPixelsSize = 0;
    std::vector<uint8_t> _gpuReadbackPixels;  // For GPU-only rendering
    WGPUBuffer _tileReadbackBuffer = nullptr;

    // Async state machine
    enum class CaptureState { IDLE, WAITING_COMPUTE, WAITING_MAP };
    CaptureState _captureState = CaptureState::IDLE;
    std::atomic<bool> _computeDone{false};
    std::atomic<bool> _mapDone{false};
    WGPUMapAsyncStatus _mapStatus = WGPUMapAsyncStatus_Success;

    // Full frame refresh
    std::atomic<bool> _forceFullFrame{true};  // Start with full frame
    uint32_t _framesSinceFullRefresh = 0;
    static constexpr uint32_t FULL_REFRESH_INTERVAL = 300;  // Every 300 frames (~5 sec at 60fps)

    // JPEG compression
    void* _jpegCompressor = nullptr;

    // Dirty tile tracking
    std::vector<bool> _dirtyTiles;
    uint16_t _tilesX = 0;
    uint16_t _tilesY = 0;

    Result<void> ensureResources(uint32_t width, uint32_t height);
    Result<void> createDiffPipeline();
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

    // Statistics tracking (per second)
    struct Stats {
        uint32_t tilesSent = 0;
        uint32_t tilesJpeg = 0;
        uint32_t tilesRaw = 0;
        uint64_t bytesSent = 0;
        uint64_t bytesJpeg = 0;
        uint64_t bytesRaw = 0;
        uint32_t fullUpdates = 0;
        uint32_t frames = 0;
        double lastReportTime = 0;
    };
    Stats _stats;
    FrameStats _lastStats;  // Last reported stats for public access
};

} // namespace yetty::vnc
