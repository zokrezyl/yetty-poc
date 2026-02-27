#pragma once

#include <algorithm>
#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <sys/types.h>

namespace yetty { class YDrawBuffer; }

namespace yetty::flame {

struct FlameConfig {
    float sceneWidth = 1200.0f;
    float sceneHeight = 800.0f;
    float frameHeight = 18.0f;
    float fontSize = 10.0f;
    float minWidthForText = 40.0f;
    uint32_t bgColor = 0xFF1A1A2E;
};

struct ViewState {
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float zoom = 1.0f;

    void panLeft(float amount)  { offsetX -= amount / zoom; }
    void panRight(float amount) { offsetX += amount / zoom; }
    void panUp(float amount)    { offsetY -= amount / zoom; }
    void panDown(float amount)  { offsetY += amount / zoom; }
    void zoomIn()               { zoom *= 1.3f; }
    void zoomOut()              { zoom = std::max(1.0f, zoom / 1.3f); }
    void reset()                { offsetX = 0; offsetY = 0; zoom = 1.0f; }

    void zoomToFrame(float frameX, float frameWidth, float sceneWidth) {
        zoom = sceneWidth / frameWidth;
        offsetX = frameX;
        offsetY = 0;
    }
};

struct FrameNode {
    std::string name;
    uint64_t self = 0;
    uint64_t total = 0;
    std::vector<FrameNode> children;
};

struct HitFrame {
    const FrameNode* node = nullptr;
    float x = 0, y = 0, width = 0;
    int depth = 0;
};

class FlameRenderer {
public:
    explicit FlameRenderer(const FlameConfig& config);

    /// Parse collapsed stack lines and merge into the frame tree.
    void loadCollapsedStacks(const std::string& data);

    /// Clear the frame tree (for re-loading).
    void clearTree();

    /// Render the flamegraph into buffer using the given view state.
    void render(std::shared_ptr<YDrawBuffer> buffer, const ViewState& view);

    const FlameConfig& config() const { return _config; }
    uint64_t totalSamples() const { return _root.total; }
    int maxDepth() const { return _maxDepth; }
    float contentHeight() const { return _config.sceneHeight; }

    float dynamicFrameHeight() const { return 20.0f; }
    float dynamicFontSize() const { return 11.0f; }

    HitFrame hitTest(float sceneX, float sceneY) const;

    void setZoomRoot(const FrameNode* node) { _zoomRoot = node; }
    const FrameNode* zoomRoot() const { return _zoomRoot; }

private:
    HitFrame hitTestNode(const FrameNode& node, float x, float y,
                         float width, int depth,
                         float sceneX, float sceneY,
                         float frameH) const;
    void mergeStack(const std::vector<std::string>& frames, uint64_t count);
    void computeTotals(FrameNode& node);
    void renderNode(std::shared_ptr<YDrawBuffer>& buffer,
                    const FrameNode& node, float x, float y,
                    float width, uint32_t& layer, int depth,
                    float frameH, float fontSize,
                    uint64_t effectiveTotal);

    static uint32_t flameColor(const std::string& name);
    static uint32_t hslToRgb(float h, float s, float l);
    static uint32_t fnv1a(const std::string& s);

    FlameConfig _config;
    FrameNode _root;
    const FrameNode* _zoomRoot = nullptr;
    int _maxDepth = 0;
};

//=============================================================================
// PerfProfiler — manages perf record/script for live profiling
//=============================================================================

class PerfProfiler {
public:
    /// Load an existing perf.data file: yflame -d perf.data
    std::string loadPerfData(const std::string& path);

    /// Start profiling a new command: yflame -- ./program args...
    /// Uses `perf record -- command` so perf manages the child.
    bool startCommand(const std::vector<std::string>& cmd);

    /// Attach to a running process: yflame -p PID
    bool attachPid(pid_t pid);

    /// Snapshot: stop current perf record, run perf script to get collapsed
    /// stacks, then restart perf record. Returns collapsed stack data.
    std::string snapshot();

    /// Stop profiling and clean up.
    void stop();

    /// Is perf (and thus the target in command mode) still running?
    bool perfAlive() const;

    /// Is the attached target still running? (attach mode only)
    bool targetAlive() const;

    /// Wait for perf to finish (for short-lived commands).
    /// Returns true if perf exited, false if timeout.
    bool waitForPerf(int timeoutMs);

    ~PerfProfiler();

private:
    bool startPerfAttach();
    void stopPerf();
    std::string runPerfScript();
    std::string collapseStacks(const std::string& perfScriptOutput);

    pid_t _targetPid = 0;     // only set in attach mode
    pid_t _perfPid = 0;
    bool _commandMode = false; // true = perf runs the command
    std::string _perfDataPath;
};

} // namespace yetty::flame
