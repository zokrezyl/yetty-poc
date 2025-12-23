#pragma once

#include "DecoratorPlugin.h"
#include <cstdint>
#include <string>
#include <any>

namespace yetty {

class Decorator {
public:
    Decorator(uint32_t id, DecoratorPlugin* plugin, PositionMode mode,
              int32_t x, int32_t y, uint32_t widthCells, uint32_t heightCells);
    ~Decorator();

    // Getters
    uint32_t getId() const { return id_; }
    DecoratorPlugin* getPlugin() const { return plugin_; }
    PositionMode getPositionMode() const { return positionMode_; }

    // Position (in cells for Relative, pixels for Absolute)
    int32_t getX() const { return x_; }
    int32_t getY() const { return y_; }
    void setPosition(int32_t x, int32_t y) { x_ = x; y_ = y; }

    // Size in cells (terminal grid cells this decorator occupies)
    uint32_t getWidthCells() const { return widthCells_; }
    uint32_t getHeightCells() const { return heightCells_; }
    void setSizeCells(uint32_t w, uint32_t h) { widthCells_ = w; heightCells_ = h; }

    // Pixel dimensions (for rendering)
    uint32_t getPixelWidth() const { return pixelWidth_; }
    uint32_t getPixelHeight() const { return pixelHeight_; }
    void setPixelSize(uint32_t w, uint32_t h) { pixelWidth_ = w; pixelHeight_ = h; }

    // For scrolling support (relative mode)
    int32_t getLogicalLine() const { return logicalLine_; }
    void setLogicalLine(int32_t line) { logicalLine_ = line; }

    // Visibility
    bool isVisible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }


    // Plugin-specific data storage
    template<typename T>
    void setPluginData(T&& data) { pluginData_ = std::forward<T>(data); }

    template<typename T>
    T* getPluginData() { return std::any_cast<T>(&pluginData_); }

    template<typename T>
    const T* getPluginData() const { return std::any_cast<T>(&pluginData_); }

    // Raw payload (base94 decoded)
    const std::string& getPayload() const { return payload_; }
    void setPayload(const std::string& p) { payload_ = p; }

    // Dirty flag for re-rendering
    bool needsRender() const { return needsRender_; }
    void setNeedsRender(bool v) { needsRender_ = v; }

private:
    uint32_t id_;
    DecoratorPlugin* plugin_;
    PositionMode positionMode_;

    int32_t x_, y_;              // Position
    uint32_t widthCells_;        // Width in terminal cells
    uint32_t heightCells_;       // Height in terminal cells
    uint32_t pixelWidth_ = 0;    // Actual pixel width
    uint32_t pixelHeight_ = 0;   // Actual pixel height

    int32_t logicalLine_ = 0;    // For scroll tracking in relative mode
    bool visible_ = true;
    bool needsRender_ = true;

    std::string payload_;        // Decoded payload data
    std::any pluginData_;        // Plugin-specific state
};

} // namespace yetty
