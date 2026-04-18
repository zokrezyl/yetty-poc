#pragma once

#include <memory>
#include <cstdint>
#include <string>
#include <random>
#include <vector>

namespace yetty { class YDrawBuffer; }

namespace yetty::zoo {

struct ZooConfig {
    uint32_t targetPoints = 20;
    uint32_t targetConnsPerCP = 3;
    float growthRate = 0.30f;
    float spawnRadiusMin = 20.0f;
    float spawnRadiusMax = 140.0f;
    float cpMarkerSize = 3.0f;
    float maxConnectionDist = 280.0f;
    float strokeMin = 0.5f;
    float strokeMax = 2.5f;
    float curveRatio = 0.80f;
    uint32_t bgColor = 0xFF1A1A2E;
    float sceneWidth = 600.0f;
    float sceneHeight = 400.0f;
};

ZooConfig parseArgs(const std::string& args);

struct ControlPoint {
    float angle;
    float spawnTime;
    float baseRadius;
    float animPhase;
    float animFreq;
    uint32_t color;
};

struct Connection {
    uint32_t cpA, cpB;
    int type;
    int shapeChoice;
    float curvature;
    float curveAnimPhase;
    float curveAnimFreq;
    float strokeWidth;
    uint32_t color;
};

class ZooRenderer {
public:
    explicit ZooRenderer(const ZooConfig& config, uint32_t seed = 0);

    /// Render one frame into buffer (clears buffer, sets scene bounds/bg).
    /// Handles spawn/cull/connect internally.
    void render(std::shared_ptr<YDrawBuffer> buffer, float time);

    const ZooConfig& config() const { return _config; }

private:
    struct Vec2f { float x, y; };

    void spawnControlPoint(float time);
    void removeControlPoint(uint32_t idx);
    void updateConnections(float time);
    void addShape(std::shared_ptr<YDrawBuffer>& buffer,
                  int choice, float cx, float cy, float size,
                  uint32_t color, uint32_t layer);
    uint32_t randomColor();
    uint32_t blendColors(uint32_t a, uint32_t b);
    Vec2f cpPosition(const ControlPoint& cp, float time) const;

    ZooConfig _config;
    std::mt19937 _rng;

    std::vector<ControlPoint> _controlPoints;
    std::vector<Connection> _connections;
    float _lastTime = -1.0f;
    bool _initialized = false;
};

} // namespace yetty::zoo
