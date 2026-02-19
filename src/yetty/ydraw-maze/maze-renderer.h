#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <random>
#include <vector>
#include <utility>

namespace yetty { class YDrawBuffer; }

namespace yetty::maze {

struct MazeConfig {
    uint32_t cols = 15;
    uint32_t rows = 10;
    float actorSpeed = 4.0f;
    float wallWidth = 1.5f;
    uint32_t wallColor = 0xFF808080;
    uint32_t actorColor = 0xFF00CCFF;
    uint32_t startColor = 0xFF00FF00;
    uint32_t endColor = 0xFF0000FF;
    uint32_t bgColor = 0xFF1A1A2E;
    float sceneWidth = 600.0f;
    float sceneHeight = 400.0f;
};

MazeConfig parseArgs(const std::string& args);

class MazeRenderer {
public:
    explicit MazeRenderer(const MazeConfig& config, uint32_t seed = 0);

    /// Generate a new maze and solve it.
    void generate();

    /// Render current frame into buffer (clears buffer, sets scene bounds/bg).
    /// Auto-regenerates when actor finishes the path.
    /// Returns true if a new maze was generated this frame.
    bool render(std::shared_ptr<YDrawBuffer> buffer, float time);

    const MazeConfig& config() const { return _config; }
    bool isFinished(float time) const;

private:
    static constexpr uint8_t WALL_N = 1;
    static constexpr uint8_t WALL_S = 2;
    static constexpr uint8_t WALL_E = 4;
    static constexpr uint8_t WALL_W = 8;

    void generateMaze();
    void solveMaze();
    void buildPrims(std::shared_ptr<YDrawBuffer>& buffer, float time);

    uint8_t& cell(uint32_t col, uint32_t row);
    uint8_t  cell(uint32_t col, uint32_t row) const;
    float cellX(uint32_t col) const;
    float cellY(uint32_t row) const;

    MazeConfig _config;
    std::mt19937 _rng;

    // Maze state
    std::vector<uint8_t> _grid;
    std::vector<std::pair<uint32_t, uint32_t>> _solutionPath;
    uint32_t _startCol = 0, _startRow = 0;
    uint32_t _endCol = 0, _endRow = 0;
    float _cellW = 0, _cellH = 0;
    float _originX = 0, _originY = 0;

    // Animation
    float _solveStartTime = 0.0f;
    bool _initialized = false;
};

} // namespace yetty::maze
