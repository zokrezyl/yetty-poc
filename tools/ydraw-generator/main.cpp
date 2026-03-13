// ydraw-generator: Generate random shapes and output as OSC sequence
//
// Generates random 2D SDF primitives (circles, boxes, ellipses, triangles,
// segments) within specified boundaries and outputs an OSC sequence for
// ydraw or ypaint cards.

#include "../../src/yetty/ycat/osc.h"
#include "../../src/yetty/ydraw/ydraw-buffer.h"

#include <args.hxx>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>

using namespace yetty;

//=============================================================================
// Configuration
//=============================================================================

struct GeneratorConfig {
  uint32_t count = 10;
  std::string mode = "ypaint";
  float minX = 0.0f;
  float minY = 0.0f;
  float maxX = 800.0f;
  float maxY = 600.0f;
  uint32_t seed = 0;
};

//=============================================================================
// Random shape generation
//=============================================================================

class ShapeGenerator {
public:
  explicit ShapeGenerator(const GeneratorConfig &config) : _config(config) {
    if (config.seed == 0) {
      auto s =
          std::chrono::high_resolution_clock::now().time_since_epoch().count();
      _rng.seed(static_cast<uint32_t>(s));
    } else {
      _rng.seed(config.seed);
    }
  }

  void generate(std::shared_ptr<YDrawBuffer> buffer) {
    for (uint32_t i = 0; i < _config.count; ++i) {
      addRandomShape(buffer);
    }

    buffer->setSceneBounds(_config.minX, _config.minY, _config.maxX,
                           _config.maxY);
    buffer->setBgColor(randomColor(0xFF));
  }

private:
  GeneratorConfig _config;
  std::mt19937 _rng;

  float randomFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(_rng);
  }

  uint32_t randomUint(uint32_t min, uint32_t max) {
    return std::uniform_int_distribution<uint32_t>(min, max)(_rng);
  }

  uint32_t randomColor(uint8_t minAlpha = 0x80) {
    uint8_t r = static_cast<uint8_t>(randomUint(0, 255));
    uint8_t g = static_cast<uint8_t>(randomUint(0, 255));
    uint8_t b = static_cast<uint8_t>(randomUint(0, 255));
    uint8_t a = static_cast<uint8_t>(randomUint(minAlpha, 255));
    return (a << 24) | (b << 16) | (g << 8) | r; // ABGR format
  }

  float sceneWidth() const { return _config.maxX - _config.minX; }
  float sceneHeight() const { return _config.maxY - _config.minY; }

  void addRandomShape(std::shared_ptr<YDrawBuffer> buffer) {
    uint32_t shapeType = randomUint(0, 4);
    uint32_t layer = randomUint(0, 10);
    uint32_t fillColor = randomColor();
    uint32_t strokeColor = randomColor();
    float strokeWidth = randomFloat(0.0f, 3.0f);

    switch (shapeType) {
    case 0:
      addCircle(buffer, layer, fillColor, strokeColor, strokeWidth);
      break;
    case 1:
      addBox(buffer, layer, fillColor, strokeColor, strokeWidth);
      break;
    case 2:
      addEllipse(buffer, layer, fillColor, strokeColor, strokeWidth);
      break;
    case 3:
      addTriangle(buffer, layer, fillColor, strokeColor, strokeWidth);
      break;
    case 4:
      addSegment(buffer, layer, fillColor, strokeColor, strokeWidth);
      break;
    }
  }

  void addCircle(std::shared_ptr<YDrawBuffer> buffer, uint32_t layer,
                 uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
    float maxRadius = std::min(sceneWidth(), sceneHeight()) * 0.15f;
    float radius = randomFloat(5.0f, maxRadius);
    float cx = randomFloat(_config.minX + radius, _config.maxX - radius);
    float cy = randomFloat(_config.minY + radius, _config.maxY - radius);

    buffer->addCircle(layer, cx, cy, radius, fillColor, strokeColor,
                      strokeWidth, 0.0f);
  }

  void addBox(std::shared_ptr<YDrawBuffer> buffer, uint32_t layer,
              uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
    float maxHalf = std::min(sceneWidth(), sceneHeight()) * 0.15f;
    float hw = randomFloat(5.0f, maxHalf);
    float hh = randomFloat(5.0f, maxHalf);
    float cx = randomFloat(_config.minX + hw, _config.maxX - hw);
    float cy = randomFloat(_config.minY + hh, _config.maxY - hh);
    float round = randomFloat(0.0f, std::min(hw, hh) * 0.5f);

    buffer->addBox(layer, cx, cy, hw, hh, fillColor, strokeColor, strokeWidth,
                   round);
  }

  void addEllipse(std::shared_ptr<YDrawBuffer> buffer, uint32_t layer,
                  uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
    float maxRadius = std::min(sceneWidth(), sceneHeight()) * 0.15f;
    float rx = randomFloat(5.0f, maxRadius);
    float ry = randomFloat(5.0f, maxRadius);
    float cx = randomFloat(_config.minX + rx, _config.maxX - rx);
    float cy = randomFloat(_config.minY + ry, _config.maxY - ry);

    buffer->addEllipse(layer, cx, cy, rx, ry, fillColor, strokeColor,
                       strokeWidth, 0.0f);
  }

  void addTriangle(std::shared_ptr<YDrawBuffer> buffer, uint32_t layer,
                   uint32_t fillColor, uint32_t strokeColor,
                   float strokeWidth) {
    float margin = std::min(sceneWidth(), sceneHeight()) * 0.05f;
    float ax = randomFloat(_config.minX + margin, _config.maxX - margin);
    float ay = randomFloat(_config.minY + margin, _config.maxY - margin);
    float bx = randomFloat(_config.minX + margin, _config.maxX - margin);
    float by = randomFloat(_config.minY + margin, _config.maxY - margin);
    float vx = randomFloat(_config.minX + margin, _config.maxX - margin);
    float vy = randomFloat(_config.minY + margin, _config.maxY - margin);

    buffer->addTriangle(layer, ax, ay, bx, by, vx, vy, fillColor, strokeColor,
                        strokeWidth, 0.0f);
  }

  void addSegment(std::shared_ptr<YDrawBuffer> buffer, uint32_t layer,
                  uint32_t fillColor, uint32_t strokeColor, float strokeWidth) {
    float margin = std::min(sceneWidth(), sceneHeight()) * 0.05f;
    float x0 = randomFloat(_config.minX + margin, _config.maxX - margin);
    float y0 = randomFloat(_config.minY + margin, _config.maxY - margin);
    float x1 = randomFloat(_config.minX + margin, _config.maxX - margin);
    float y1 = randomFloat(_config.minY + margin, _config.maxY - margin);

    // Segments typically use stroke, not fill
    buffer->addSegment(layer, x0, y0, x1, y1, 0, strokeColor,
                       strokeWidth + 1.0f, 0.0f);
  }
};

//=============================================================================
// Parse bounds argument "minX,minY,maxX,maxY"
//=============================================================================

bool parseBounds(const std::string &s, float &minX, float &minY, float &maxX,
                 float &maxY) {
  float values[4];
  int idx = 0;
  size_t start = 0;

  for (size_t i = 0; i <= s.size() && idx < 4; ++i) {
    if (i == s.size() || s[i] == ',') {
      if (i > start) {
        try {
          values[idx++] = std::stof(s.substr(start, i - start));
        } catch (...) {
          return false;
        }
      }
      start = i + 1;
    }
  }

  if (idx != 4)
    return false;

  minX = values[0];
  minY = values[1];
  maxX = values[2];
  maxY = values[3];

  return minX < maxX && minY < maxY;
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char **argv) {
  args::ArgumentParser parser(
      "ydraw-generator - Generate random shapes as OSC sequence",
      "Outputs an OSC sequence for ydraw/ypaint cards.");
  args::HelpFlag help(parser, "help", "Show help", {'h', "help"});

  args::ValueFlag<uint32_t> countFlag(
      parser, "N", "Number of shapes to generate", {'n', "count"}, 10);
  args::ValueFlag<std::string> modeFlag(
      parser, "MODE", "Output mode: ydraw or ypaint", {'m', "mode"}, "ypaint");
  args::ValueFlag<std::string> boundsFlag(parser, "BOUNDS",
                                          "Bounds as minX,minY,maxX,maxY",
                                          {'b', "bounds"}, "0,0,800,600");
  args::ValueFlag<uint32_t> seedFlag(
      parser, "SEED", "Random seed (0 = use time)", {'s', "seed"}, 0);

  try {
    parser.ParseCLI(argc, argv);
  } catch (const args::Help &) {
    std::cout << parser;
    return 0;
  } catch (const args::ParseError &e) {
    std::cerr << e.what() << "\n" << parser;
    return 1;
  }

  GeneratorConfig config;
  config.count = args::get(countFlag);
  config.mode = args::get(modeFlag);
  config.seed = args::get(seedFlag);

  // Validate mode
  if (config.mode != "ydraw" && config.mode != "ypaint") {
    std::cerr << "Error: mode must be 'ydraw' or 'ypaint'\n";
    return 1;
  }

  // Parse bounds
  std::string boundsStr = args::get(boundsFlag);
  if (!parseBounds(boundsStr, config.minX, config.minY, config.maxX,
                   config.maxY)) {
    std::cerr << "Error: invalid bounds format. Use: minX,minY,maxX,maxY\n";
    return 1;
  }

  // Create buffer and generate shapes
  auto bufferResult = YDrawBuffer::create();
  if (!bufferResult) {
    std::cerr << "Failed to create YDrawBuffer\n";
    return 1;
  }
  auto buffer = *bufferResult;

  ShapeGenerator generator(config);
  generator.generate(buffer);

  // Serialize to binary
  auto binary = buffer->serialize();

  // Create OSC sequence
  auto b64 = ycat::base64Encode(binary);

  // Build OSC sequence
  // Format: ESC ] 666666 ; run -c <card> ;; <base64> ESC
  std::string osc;
  osc += "\033]666666;run -c ";
  osc += config.mode;
  osc += ";;";
  osc += b64;
  osc += "\033\\";

  // Output (wrapped for tmux if needed)
  std::string output = ycat::maybeWrapForTmux(osc);
  std::cout << output;

  return 0;
}
