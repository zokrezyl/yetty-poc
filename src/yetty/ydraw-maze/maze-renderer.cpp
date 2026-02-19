#include "maze-renderer.h"
#include "../ydraw/ydraw-buffer.h"
#include <cmath>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>

namespace yetty::maze {

//=============================================================================
// Argument parsing
//=============================================================================

MazeConfig parseArgs(const std::string& args) {
    MazeConfig cfg;
    if (args.empty()) return cfg;

    std::istringstream iss(args);
    std::string token;

    while (iss >> token) {
        if (token == "--cols") {
            uint32_t v; if (iss >> v) cfg.cols = std::max(3u, std::min(v, 80u));
        } else if (token == "--rows") {
            uint32_t v; if (iss >> v) cfg.rows = std::max(3u, std::min(v, 50u));
        } else if (token == "--speed") {
            float v; if (iss >> v) cfg.actorSpeed = std::max(0.5f, std::min(v, 50.0f));
        } else if (token == "--wall-width") {
            float v; if (iss >> v) cfg.wallWidth = std::max(0.3f, std::min(v, 5.0f));
        } else if (token == "--wall-color" || token == "--actor-color" ||
                   token == "--start-color" || token == "--end-color" ||
                   token == "--bg-color") {
            std::string s;
            if (iss >> s) {
                if (s.substr(0, 2) == "0x" || s.substr(0, 2) == "0X")
                    s = s.substr(2);
                try {
                    uint32_t c = static_cast<uint32_t>(std::stoul(s, nullptr, 16));
                    if (token == "--wall-color") cfg.wallColor = c;
                    else if (token == "--actor-color") cfg.actorColor = c;
                    else if (token == "--start-color") cfg.startColor = c;
                    else if (token == "--end-color") cfg.endColor = c;
                    else if (token == "--bg-color") cfg.bgColor = c;
                } catch (...) {}
            }
        }
    }
    return cfg;
}

//=============================================================================
// MazeRenderer
//=============================================================================

MazeRenderer::MazeRenderer(const MazeConfig& config, uint32_t seed)
    : _config(config)
{
    if (seed == 0) {
        auto s = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        _rng.seed(static_cast<uint32_t>(s));
    } else {
        _rng.seed(seed);
    }
}

void MazeRenderer::generate() {
    generateMaze();
    solveMaze();
}

bool MazeRenderer::isFinished(float time) const {
    if (_solutionPath.empty()) return false;
    float elapsed = time - _solveStartTime;
    float totalTime = static_cast<float>(_solutionPath.size() - 1) / _config.actorSpeed;
    return elapsed > totalTime + 1.0f;
}

bool MazeRenderer::render(std::shared_ptr<YDrawBuffer> buffer, float time) {
    bool regenerated = false;

    if (!_initialized) {
        generate();
        _solveStartTime = time;
        _initialized = true;
        regenerated = true;
    }

    // Check if actor reached the end — generate new maze
    if (!_solutionPath.empty() && isFinished(time)) {
        generate();
        _solveStartTime = time;
        regenerated = true;
    }

    buffer->clear();
    buffer->setBgColor(_config.bgColor);
    buffer->setSceneBounds(0, 0, _config.sceneWidth, _config.sceneHeight);
    buildPrims(buffer, time);

    return regenerated;
}

//=============================================================================
// Cell accessors
//=============================================================================

uint8_t& MazeRenderer::cell(uint32_t col, uint32_t row) {
    return _grid[row * _config.cols + col];
}

uint8_t MazeRenderer::cell(uint32_t col, uint32_t row) const {
    return _grid[row * _config.cols + col];
}

float MazeRenderer::cellX(uint32_t col) const {
    return _originX + (static_cast<float>(col) + 0.5f) * _cellW;
}

float MazeRenderer::cellY(uint32_t row) const {
    return _originY + (static_cast<float>(row) + 0.5f) * _cellH;
}

//=============================================================================
// Maze generation — recursive backtracker (DFS)
//=============================================================================

void MazeRenderer::generateMaze() {
    uint32_t total = _config.cols * _config.rows;
    _grid.assign(total, WALL_N | WALL_S | WALL_E | WALL_W);

    float pad = 10.0f;
    float usableW = _config.sceneWidth - 2.0f * pad;
    float usableH = _config.sceneHeight - 2.0f * pad;
    _cellW = usableW / static_cast<float>(_config.cols);
    _cellH = usableH / static_cast<float>(_config.rows);
    _originX = pad;
    _originY = pad;

    _startCol = 0;
    _startRow = 0;
    _endCol = _config.cols - 1;
    _endRow = _config.rows - 1;

    // DFS with explicit stack
    std::vector<bool> visited(total, false);
    std::stack<std::pair<uint32_t, uint32_t>> stack;

    visited[0] = true;
    stack.push({0, 0});

    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {-1, 1, 0, 0};
    const uint8_t wallRemove[] = {WALL_N, WALL_S, WALL_E, WALL_W};
    const uint8_t wallOpposite[] = {WALL_S, WALL_N, WALL_W, WALL_E};

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();

        int neighbors[4];
        int ncount = 0;
        for (int d = 0; d < 4; d++) {
            int nx = static_cast<int>(cx) + dx[d];
            int ny = static_cast<int>(cy) + dy[d];
            if (nx >= 0 && nx < static_cast<int>(_config.cols) &&
                ny >= 0 && ny < static_cast<int>(_config.rows) &&
                !visited[ny * _config.cols + nx]) {
                neighbors[ncount++] = d;
            }
        }

        if (ncount == 0) {
            stack.pop();
            continue;
        }

        int d = neighbors[std::uniform_int_distribution<int>(0, ncount - 1)(_rng)];
        uint32_t nx = static_cast<uint32_t>(static_cast<int>(cx) + dx[d]);
        uint32_t ny = static_cast<uint32_t>(static_cast<int>(cy) + dy[d]);

        cell(cx, cy) &= ~wallRemove[d];
        cell(nx, ny) &= ~wallOpposite[d];

        visited[ny * _config.cols + nx] = true;
        stack.push({nx, ny});
    }
}

//=============================================================================
// Pathfinding — BFS
//=============================================================================

void MazeRenderer::solveMaze() {
    uint32_t total = _config.cols * _config.rows;
    std::vector<int> parent(total, -1);
    std::vector<bool> visited(total, false);
    std::queue<uint32_t> q;

    uint32_t startIdx = _startRow * _config.cols + _startCol;
    uint32_t endIdx = _endRow * _config.cols + _endCol;

    visited[startIdx] = true;
    q.push(startIdx);

    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {-1, 1, 0, 0};
    const uint8_t wallCheck[] = {WALL_N, WALL_S, WALL_E, WALL_W};

    while (!q.empty()) {
        uint32_t idx = q.front();
        q.pop();

        if (idx == endIdx) break;

        uint32_t cx = idx % _config.cols;
        uint32_t cy = idx / _config.cols;

        for (int d = 0; d < 4; d++) {
            if (_grid[idx] & wallCheck[d]) continue;

            int nx = static_cast<int>(cx) + dx[d];
            int ny = static_cast<int>(cy) + dy[d];
            if (nx < 0 || nx >= static_cast<int>(_config.cols) ||
                ny < 0 || ny >= static_cast<int>(_config.rows)) continue;

            uint32_t nIdx = static_cast<uint32_t>(ny) * _config.cols + static_cast<uint32_t>(nx);
            if (visited[nIdx]) continue;

            visited[nIdx] = true;
            parent[nIdx] = static_cast<int>(idx);
            q.push(nIdx);
        }
    }

    _solutionPath.clear();
    int cur = static_cast<int>(endIdx);
    while (cur >= 0) {
        uint32_t cx = static_cast<uint32_t>(cur) % _config.cols;
        uint32_t cy = static_cast<uint32_t>(cur) / _config.cols;
        _solutionPath.push_back({cx, cy});
        cur = parent[cur];
    }
    std::reverse(_solutionPath.begin(), _solutionPath.end());
}

//=============================================================================
// Prim generation
//=============================================================================

void MazeRenderer::buildPrims(std::shared_ptr<YDrawBuffer>& buffer, float time) {
    uint32_t layer = 0;

    // 1. Wall segments
    for (uint32_t row = 0; row < _config.rows; row++) {
        for (uint32_t col = 0; col < _config.cols; col++) {
            uint8_t w = cell(col, row);
            float x0 = _originX + static_cast<float>(col) * _cellW;
            float y0 = _originY + static_cast<float>(row) * _cellH;
            float x1 = x0 + _cellW;
            float y1 = y0 + _cellH;

            auto addWall = [&](float ax, float ay, float bx, float by) {
                buffer->addSegment(layer++, ax, ay, bx, by, 0, _config.wallColor, _config.wallWidth, 0.0f);
            };

            if ((w & WALL_N) && row == 0) addWall(x0, y0, x1, y0);
            if (w & WALL_S) addWall(x0, y1, x1, y1);
            if ((w & WALL_W) && col == 0) addWall(x0, y0, x0, y1);
            if (w & WALL_E) addWall(x1, y0, x1, y1);
        }
    }

    // 2. Start marker (circle)
    {
        float markerR = std::min(_cellW, _cellH) * 0.3f;
        buffer->addCircle(layer++, cellX(_startCol), cellY(_startRow), markerR, _config.startColor, 0, 0.0f, 0.0f);
    }

    // 3. End marker (star)
    {
        float markerR = std::min(_cellW, _cellH) * 0.35f;
        buffer->addStar(layer++, cellX(_endCol), cellY(_endRow), markerR, 5.0f, 2.2f, _config.endColor, 0, 0.0f, 0.0f);
    }

    // 4. Trail
    if (!_solutionPath.empty()) {
        float elapsed = time - _solveStartTime;
        float totalCells = elapsed * _config.actorSpeed;
        uint32_t cellsTraversed = static_cast<uint32_t>(totalCells);
        cellsTraversed = std::min(cellsTraversed, static_cast<uint32_t>(_solutionPath.size() - 1));

        uint32_t trailColor = (_config.actorColor & 0x00FFFFFFu) | 0x60000000u;
        float trailWidth = std::min(_cellW, _cellH) * 0.15f;
        for (uint32_t i = 0; i < cellsTraversed && i + 1 < _solutionPath.size(); i++) {
            auto [c0, r0] = _solutionPath[i];
            auto [c1, r1] = _solutionPath[i + 1];
            buffer->addSegment(layer++, cellX(c0), cellY(r0), cellX(c1), cellY(r1), 0, trailColor, trailWidth, 0.0f);
        }
    }

    // 5. Actor
    if (!_solutionPath.empty()) {
        float elapsed = time - _solveStartTime;
        float progress = elapsed * _config.actorSpeed;
        uint32_t idx = static_cast<uint32_t>(progress);
        float frac = progress - static_cast<float>(idx);

        float ax, ay;
        if (idx >= _solutionPath.size() - 1) {
            auto [c, r] = _solutionPath.back();
            ax = cellX(c);
            ay = cellY(r);
        } else {
            auto [c0, r0] = _solutionPath[idx];
            auto [c1, r1] = _solutionPath[idx + 1];
            ax = cellX(c0) + (cellX(c1) - cellX(c0)) * frac;
            ay = cellY(r0) + (cellY(r1) - cellY(r0)) * frac;
        }

        float actorR = std::min(_cellW, _cellH) * 0.28f;
        buffer->addCircle(layer++, ax, ay, actorR, _config.actorColor, 0, 0.0f, 0.0f);
    }
}

} // namespace yetty::maze
