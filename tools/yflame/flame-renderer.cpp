#include "flame-renderer.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

namespace yetty::flame {

//=============================================================================
// FlameRenderer
//=============================================================================

FlameRenderer::FlameRenderer(const FlameConfig& config)
    : _config(config)
{
    _root.name = "root";
}

//=============================================================================
// Collapsed stack parsing
//=============================================================================

void FlameRenderer::loadCollapsedStacks(const std::string& data) {
    std::istringstream iss(data);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto lastSpace = line.rfind(' ');
        if (lastSpace == std::string::npos) continue;

        uint64_t count = 0;
        try {
            count = std::stoull(line.substr(lastSpace + 1));
        } catch (...) {
            continue;
        }

        std::string stackStr = line.substr(0, lastSpace);

        std::vector<std::string> frames;
        std::istringstream ss(stackStr);
        std::string frame;
        while (std::getline(ss, frame, ';')) {
            if (!frame.empty()) {
                frames.push_back(frame);
            }
        }

        if (!frames.empty()) {
            mergeStack(frames, count);
        }
    }

    computeTotals(_root);
}

void FlameRenderer::clearTree() {
    _root.children.clear();
    _root.self = 0;
    _root.total = 0;
    _maxDepth = 0;
    _zoomRoot = nullptr;
}

void FlameRenderer::mergeStack(const std::vector<std::string>& frames,
                                uint64_t count) {
    FrameNode* current = &_root;

    for (size_t i = 0; i < frames.size(); i++) {
        const auto& name = frames[i];

        FrameNode* found = nullptr;
        for (auto& child : current->children) {
            if (child.name == name) {
                found = &child;
                break;
            }
        }

        if (!found) {
            current->children.push_back({name, 0, 0, {}});
            found = &current->children.back();
        }

        if (i == frames.size() - 1) {
            found->self += count;
        }

        current = found;
    }

    int depth = static_cast<int>(frames.size());
    if (depth > _maxDepth) _maxDepth = depth;
}

void FlameRenderer::computeTotals(FrameNode& node) {
    node.total = node.self;
    for (auto& child : node.children) {
        computeTotals(child);
        node.total += child.total;
    }

    std::sort(node.children.begin(), node.children.end(),
              [](const FrameNode& a, const FrameNode& b) {
                  return a.total > b.total;
              });
}

//=============================================================================
// Color scheme — classic warm flamegraph palette
//=============================================================================

uint32_t FlameRenderer::fnv1a(const std::string& s) {
    uint32_t hash = 2166136261u;
    for (char c : s) {
        hash ^= static_cast<uint32_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

uint32_t FlameRenderer::hslToRgb(float h, float s, float l) {
    float c = (1.0f - std::fabs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    float r1, g1, b1;

    if      (h < 60)  { r1 = c; g1 = x; b1 = 0; }
    else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
    else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
    else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
    else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
    else              { r1 = c; g1 = 0; b1 = x; }

    uint8_t r = static_cast<uint8_t>((r1 + m) * 255.0f);
    uint8_t g = static_cast<uint8_t>((g1 + m) * 255.0f);
    uint8_t b = static_cast<uint8_t>((b1 + m) * 255.0f);

    return 0xFF000000u | (static_cast<uint32_t>(b) << 16)
                       | (static_cast<uint32_t>(g) << 8)
                       | static_cast<uint32_t>(r);
}

uint32_t FlameRenderer::flameColor(const std::string& name) {
    uint32_t hash = fnv1a(name);
    float hue = 10.0f + static_cast<float>(hash % 50);
    float sat = 0.70f + static_cast<float>((hash >> 8) % 20) * 0.01f;
    float lit = 0.50f + static_cast<float>((hash >> 16) % 15) * 0.01f;
    return hslToRgb(hue, sat, lit);
}

//=============================================================================
// Rendering
//=============================================================================

void FlameRenderer::render(std::shared_ptr<YDrawBuffer> buffer,
                            const ViewState& view) {
    buffer->clear();
    buffer->setBgColor(_config.bgColor);

    if (_root.total == 0) {
        buffer->setSceneBounds(0, 0, _config.sceneWidth, _config.sceneHeight);
        buffer->addText(20.0f, _config.sceneHeight / 2.0f,
                        "No samples collected. Command may have finished too quickly.",
                        _config.fontSize, 0xFFAAAAAAu, 0, -1);
        return;
    }

    float topPadding = 30.0f;
    float frameH = dynamicFrameHeight();
    float fontSize = dynamicFontSize();

    // Always use fixed scene bounds — zooming is done by re-laying out
    buffer->setSceneBounds(0, 0, _config.sceneWidth, _config.sceneHeight);

    const FrameNode& effectiveRoot = _zoomRoot ? *_zoomRoot : _root;
    uint64_t effectiveTotal = effectiveRoot.total;

    uint32_t layer = 0;
    float topY = topPadding;

    // Root bar
    float rootW = _config.sceneWidth;
    float rootCx = rootW / 2.0f;
    float rootCy = topY + frameH / 2.0f;
    buffer->addBox(layer++, rootCx, rootCy,
                   rootW / 2.0f, frameH / 2.0f - 0.5f,
                   0xFF333333u, 0, 0.0f, 1.0f);
    std::string rootLabel = _zoomRoot
        ? effectiveRoot.name + " (" + std::to_string(effectiveTotal) + " samples)"
        : "all (" + std::to_string(effectiveTotal) + " samples)";
    if (rootW > _config.minWidthForText) {
        buffer->addText(2.0f, rootCy + fontSize * 0.3f,
                        rootLabel, fontSize, 0xFFFFFFFFu, layer++, -1);
    }

    float x = 0.0f;
    for (const auto& child : effectiveRoot.children) {
        float w = (static_cast<float>(child.total) / static_cast<float>(effectiveTotal))
                * _config.sceneWidth;
        renderNode(buffer, child, x, topY + frameH, w, layer, 1, frameH, fontSize,
                   effectiveTotal);
        x += w;
    }
}

void FlameRenderer::renderNode(std::shared_ptr<YDrawBuffer>& buffer,
                                const FrameNode& node, float x, float y,
                                float width, uint32_t& layer, int depth,
                                float frameH, float fontSize,
                                uint64_t effectiveTotal) {
    if (width < 0.5f) return;

    float hw = width / 2.0f;
    float hh = frameH / 2.0f - 0.5f;
    float cx = x + hw;
    float cy = y + frameH / 2.0f;

    uint32_t fillColor = flameColor(node.name);
    buffer->addBox(layer++, cx, cy, hw, hh, fillColor, 0, 0.0f, 1.0f);

    if (width > _config.minWidthForText) {
        float charWidth = fontSize * 0.55f;
        int maxChars = static_cast<int>((width - 4.0f) / charWidth);
        std::string label = node.name;
        if (maxChars > 0 && static_cast<int>(label.size()) > maxChars) {
            if (maxChars > 2) {
                label = label.substr(0, maxChars - 2) + "..";
            } else {
                label = label.substr(0, maxChars);
            }
        }

        buffer->addText(x + 2.0f, cy + fontSize * 0.3f,
                        label, fontSize, 0xFF000000u, layer++, -1);
    }

    float childX = x;
    float childY = y + frameH;
    for (const auto& child : node.children) {
        float childW = (static_cast<float>(child.total) / static_cast<float>(effectiveTotal))
                      * _config.sceneWidth;
        renderNode(buffer, child, childX, childY, childW, layer, depth + 1, frameH, fontSize,
                   effectiveTotal);
        childX += childW;
    }
}

//=============================================================================
// Hit testing
//=============================================================================

HitFrame FlameRenderer::hitTest(float sceneX, float sceneY) const {
    if (_root.total == 0) return {};

    const FrameNode& effectiveRoot = _zoomRoot ? *_zoomRoot : _root;
    uint64_t effectiveTotal = effectiveRoot.total;

    float topY = 30.0f;
    float frameH = dynamicFrameHeight();

    // Check root bar
    if (sceneY >= topY && sceneY < topY + frameH &&
        sceneX >= 0 && sceneX < _config.sceneWidth) {
        return {&effectiveRoot, 0, topY, _config.sceneWidth, 0};
    }

    // Check children
    float x = 0.0f;
    HitFrame best;
    for (const auto& child : effectiveRoot.children) {
        float w = (static_cast<float>(child.total) / static_cast<float>(effectiveTotal))
                * _config.sceneWidth;
        auto hit = hitTestNode(child, x, topY + frameH, w, 1,
                               sceneX, sceneY, frameH);
        if (hit.node && hit.depth > best.depth) {
            best = hit;
        }
        x += w;
    }
    return best;
}

HitFrame FlameRenderer::hitTestNode(const FrameNode& node, float x, float y,
                                     float width, int depth,
                                     float sceneX, float sceneY,
                                     float frameH) const {
    if (width < 0.5f) return {};

    HitFrame best;

    // Check if click is in this node's box
    if (sceneX >= x && sceneX < x + width &&
        sceneY >= y && sceneY < y + frameH) {
        best = {&node, x, y, width, depth};
    }

    // Check children (may find a deeper match)
    const FrameNode& effectiveRoot = _zoomRoot ? *_zoomRoot : _root;
    float childX = x;
    float childY = y + frameH;
    for (const auto& child : node.children) {
        float childW = (static_cast<float>(child.total) / static_cast<float>(effectiveRoot.total))
                      * _config.sceneWidth;
        auto hit = hitTestNode(child, childX, childY, childW, depth + 1,
                               sceneX, sceneY, frameH);
        if (hit.node && hit.depth > best.depth) {
            best = hit;
        }
        childX += childW;
    }
    return best;
}

//=============================================================================
// PerfProfiler
//=============================================================================

PerfProfiler::~PerfProfiler() {
    stop();
}

std::string PerfProfiler::loadPerfData(const std::string& path) {
    // Don't set _perfDataPath — it's not ours to delete.
    std::string cmd = "perf script -i " + path + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};
    std::string output;
    char buf[4096];
    while (fgets(buf, sizeof(buf), pipe)) output += buf;
    pclose(pipe);
    return collapseStacks(output);
}

bool PerfProfiler::startCommand(const std::vector<std::string>& cmd) {
    if (cmd.empty()) return false;

    _perfDataPath = "/tmp/yflame-" + std::to_string(getpid()) + ".perf.data";
    _commandMode = true;

    // Use perf record -- command, so perf manages the child process directly.
    // This works correctly for both short-lived and long-lived commands.
    _perfPid = fork();
    if (_perfPid == 0) {
        // Child: exec perf record -- command args...
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0) { dup2(devnull, STDOUT_FILENO); dup2(devnull, STDERR_FILENO); close(devnull); }

        // Build argv: perf record -g --freq=997 -o datapath -- cmd args...
        std::vector<const char*> argv;
        argv.push_back("perf");
        argv.push_back("record");
        argv.push_back("-g");
        argv.push_back("--freq=997");
        argv.push_back("-o");
        argv.push_back(_perfDataPath.c_str());
        argv.push_back("--");
        for (const auto& s : cmd) argv.push_back(s.c_str());
        argv.push_back(nullptr);
        execvp("perf", const_cast<char* const*>(argv.data()));
        _exit(127);
    }
    return _perfPid > 0;
}

bool PerfProfiler::attachPid(pid_t pid) {
    _targetPid = pid;
    _commandMode = false;
    _perfDataPath = "/tmp/yflame-" + std::to_string(getpid()) + ".perf.data";
    return startPerfAttach();
}

bool PerfProfiler::startPerfAttach() {
    _perfPid = fork();
    if (_perfPid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0) { dup2(devnull, STDOUT_FILENO); dup2(devnull, STDERR_FILENO); close(devnull); }

        std::string pidStr = std::to_string(_targetPid);
        execlp("perf", "perf", "record", "-g", "--freq=997",
               "-p", pidStr.c_str(),
               "-o", _perfDataPath.c_str(),
               nullptr);
        _exit(127);
    }
    return _perfPid > 0;
}

void PerfProfiler::stopPerf() {
    if (_perfPid > 0) {
        kill(_perfPid, SIGINT);
        int status;
        waitpid(_perfPid, &status, 0);
        _perfPid = 0;
    }
}

bool PerfProfiler::waitForPerf(int timeoutMs) {
    if (_perfPid <= 0) return true;

    int elapsed = 0;
    int step = 50;
    while (elapsed < timeoutMs) {
        int status;
        pid_t ret = waitpid(_perfPid, &status, WNOHANG);
        if (ret > 0) {
            _perfPid = 0;
            return true;
        }
        usleep(step * 1000);
        elapsed += step;
    }
    return false;
}

std::string PerfProfiler::snapshot() {
    // Stop perf record so it flushes the data file
    stopPerf();

    // Run perf script and collapse
    auto scriptOutput = runPerfScript();
    auto collapsed = collapseStacks(scriptOutput);

    // In attach mode, restart perf if target is still alive
    if (!_commandMode && targetAlive()) {
        startPerfAttach();
    }

    return collapsed;
}

void PerfProfiler::stop() {
    stopPerf();
    _targetPid = 0;

    if (!_perfDataPath.empty()) {
        unlink(_perfDataPath.c_str());
    }
}

bool PerfProfiler::perfAlive() const {
    if (_perfPid <= 0) return false;
    return kill(_perfPid, 0) == 0;
}

bool PerfProfiler::targetAlive() const {
    if (_targetPid <= 0) return false;
    return kill(_targetPid, 0) == 0;
}

std::string PerfProfiler::runPerfScript() {
    std::string cmd = "perf script -i " + _perfDataPath + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return {};

    std::string output;
    char buf[4096];
    while (fgets(buf, sizeof(buf), pipe)) {
        output += buf;
    }
    pclose(pipe);
    return output;
}

std::string PerfProfiler::collapseStacks(const std::string& perfScriptOutput) {
    // Parse perf script output into collapsed stacks.
    // Format:
    //   command pid/tid timestamp: event:
    //       addr function (module)
    //       addr function (module)
    //       ...
    //   <blank line>
    //
    // We collect function names per sample (bottom-up), reverse them,
    // join with ';', and count occurrences.

    std::unordered_map<std::string, uint64_t> stacks;
    std::vector<std::string> currentStack;
    bool inStack = false;

    std::istringstream iss(perfScriptOutput);
    std::string line;

    while (std::getline(iss, line)) {
        if (line.empty()) {
            // End of sample
            if (!currentStack.empty()) {
                // Reverse: perf script outputs leaf-first, we want root-first
                std::string key;
                for (int i = static_cast<int>(currentStack.size()) - 1; i >= 0; i--) {
                    if (!key.empty()) key += ';';
                    key += currentStack[i];
                }
                stacks[key]++;
                currentStack.clear();
            }
            inStack = false;
            continue;
        }

        // Stack frame lines start with whitespace
        if (line[0] == '\t' || line[0] == ' ') {
            // Parse: "    addr function (module)" or "    addr function+0xoff (module)"
            // Find the function name between the address and the module
            size_t start = line.find_first_not_of(" \t");
            if (start == std::string::npos) continue;

            // Skip the hex address
            size_t addrEnd = line.find(' ', start);
            if (addrEnd == std::string::npos) continue;

            size_t funcStart = line.find_first_not_of(" \t", addrEnd);
            if (funcStart == std::string::npos) continue;

            // Function name ends at ' (' or '+' or end of line
            std::string func;
            size_t funcEnd = line.find(" (", funcStart);
            if (funcEnd != std::string::npos) {
                func = line.substr(funcStart, funcEnd - funcStart);
            } else {
                func = line.substr(funcStart);
            }

            // Strip +0x offset
            size_t plus = func.find('+');
            if (plus != std::string::npos) {
                func = func.substr(0, plus);
            }

            // Skip [unknown] frames
            if (!func.empty() && func != "[unknown]") {
                currentStack.push_back(func);
            }
            inStack = true;
        } else {
            // Header line of a new sample — if we had a pending stack, flush it
            if (!currentStack.empty()) {
                std::string key;
                for (int i = static_cast<int>(currentStack.size()) - 1; i >= 0; i--) {
                    if (!key.empty()) key += ';';
                    key += currentStack[i];
                }
                stacks[key]++;
                currentStack.clear();
            }
            inStack = false;
        }
    }

    // Flush last sample
    if (!currentStack.empty()) {
        std::string key;
        for (int i = static_cast<int>(currentStack.size()) - 1; i >= 0; i--) {
            if (!key.empty()) key += ';';
            key += currentStack[i];
        }
        stacks[key]++;
    }

    // Build collapsed output
    std::string result;
    for (const auto& [stack, count] : stacks) {
        result += stack + " " + std::to_string(count) + "\n";
    }
    return result;
}

} // namespace yetty::flame
