#pragma once

#include "result.hpp"
#include <webgpu/webgpu.h>
#include <string>
#include <cstdint>
#include <memory>
#include <vector>

namespace yetty {

// Forward declarations
struct WebGPUContext;
class Grid;
class Plugin;
class PluginLayer;

using PluginPtr = std::shared_ptr<Plugin>;
using PluginLayerPtr = std::shared_ptr<PluginLayer>;

// Position mode for plugin layers
enum class PositionMode {
    Absolute,  // Fixed position, doesn't scroll
    Relative   // Relative to cursor when created, scrolls with content
};

//-----------------------------------------------------------------------------
// PluginLayer - represents a specific layer at a position in the terminal
// Each layer has its own position, size, payload, and per-layer state
//-----------------------------------------------------------------------------
class PluginLayer : public std::enable_shared_from_this<PluginLayer> {
public:
    PluginLayer() = default;
    virtual ~PluginLayer() = default;

    // Initialize this layer with its payload
    virtual Result<void> init(const std::string& payload) = 0;

    // Dispose layer-specific resources
    virtual void dispose() {}

    // Called every frame (for animation, etc.)
    virtual void update(double deltaTime) { (void)deltaTime; }

    // Input handling - coordinates are relative to layer's top-left (in screen pixels)
    // Return true if event was consumed
    virtual bool onMouseMove(float localX, float localY) { (void)localX; (void)localY; return false; }
    virtual bool onMouseButton(int button, bool pressed) { (void)button; (void)pressed; return false; }
    virtual bool onMouseScroll(float xoffset, float yoffset, int mods) { (void)xoffset; (void)yoffset; (void)mods; return false; }
    virtual bool onKey(int key, int scancode, int action, int mods) {
        (void)key; (void)scancode; (void)action; (void)mods; return false;
    }
    virtual bool onChar(unsigned int codepoint) { (void)codepoint; return false; }

    // Query if layer wants input
    virtual bool wantsKeyboard() const { return false; }
    virtual bool wantsMouse() const { return false; }

    // Focus state
    bool hasFocus() const { return _has_focus; }
    virtual void setFocus(bool f) { _has_focus = f; }

    // Handle resize (called when cell dimensions change)
    virtual void onResize(uint32_t newPixelWidth, uint32_t newPixelHeight) {
        _pixel_width = newPixelWidth;
        _pixel_height = newPixelHeight;
    }

    // Accessors
    uint32_t getId() const { return _id; }
    void setId(uint32_t id) { _id = id; }

    Plugin* getParent() const { return _parent; }
    void setParent(Plugin* p) { _parent = p; }

    PositionMode getPositionMode() const { return _position_mode; }
    void setPositionMode(PositionMode mode) { _position_mode = mode; }

    int32_t getX() const { return _x; }
    int32_t getY() const { return _y; }
    void setPosition(int32_t x, int32_t y) { _x = x; _y = y; }

    uint32_t getWidthCells() const { return _width_cells; }
    uint32_t getHeightCells() const { return _height_cells; }
    void setCellSize(uint32_t w, uint32_t h) { _width_cells = w; _height_cells = h; }

    uint32_t getPixelWidth() const { return _pixel_width; }
    uint32_t getPixelHeight() const { return _pixel_height; }
    void setPixelSize(uint32_t w, uint32_t h) { _pixel_width = w; _pixel_height = h; }

    bool isVisible() const { return _visible; }
    void setVisible(bool v) { _visible = v; }

    const std::string& getPayload() const { return _payload; }
    void setPayload(const std::string& p) { _payload = p; }

    bool needsRender() const { return _needs_render; }
    void setNeedsRender(bool v) { _needs_render = v; }

protected:
    uint32_t _id = 0;
    Plugin* _parent = nullptr;
    PositionMode _position_mode = PositionMode::Absolute;
    int32_t _x = 0;
    int32_t _y = 0;
    uint32_t _width_cells = 1;
    uint32_t _height_cells = 1;
    uint32_t _pixel_width = 0;
    uint32_t _pixel_height = 0;
    bool _visible = true;
    bool _needs_render = true;
    bool _has_focus = false;
    std::string _payload;
};

//-----------------------------------------------------------------------------
// Plugin - represents a plugin TYPE (e.g., "ymery", "shadertoy")
// Holds shared resources (e.g., single ImGui context for all ymery layers)
// Lazily initialized when first layer is created
//-----------------------------------------------------------------------------
class Plugin : public std::enable_shared_from_this<Plugin> {
public:
    Plugin() = default;
    virtual ~Plugin() = default;

    // Plugin identification
    virtual const char* pluginName() const = 0;

    // Initialize shared resources (called lazily on first layer creation)
    // ctx may be nullptr if called before first render
    virtual Result<void> init(WebGPUContext* ctx) { (void)ctx; return Ok(); }

    // Dispose shared resources
    virtual void dispose() {
        for (auto& layer : _layers) {
            if (layer) layer->dispose();
        }
        _layers.clear();
    }

    // Create a new layer for this plugin
    virtual Result<PluginLayerPtr> createLayer(const std::string& payload) = 0;

    // Add a layer to this plugin (called by PluginManager after createLayer)
    void addLayer(PluginLayerPtr layer) {
        layer->setParent(this);
        _layers.push_back(layer);
    }

    // Remove a layer by ID
    bool removeLayer(uint32_t id) {
        for (auto it = _layers.begin(); it != _layers.end(); ++it) {
            if ((*it)->getId() == id) {
                (*it)->dispose();
                _layers.erase(it);
                return true;
            }
        }
        return false;
    }

    // Get a layer by ID
    PluginLayerPtr getLayer(uint32_t id) {
        for (auto& layer : _layers) {
            if (layer->getId() == id) return layer;
        }
        return nullptr;
    }

    // Get all layers
    const std::vector<PluginLayerPtr>& getLayers() const { return _layers; }

    // Update all layers
    virtual void update(double deltaTime) {
        for (auto& layer : _layers) {
            if (layer->isVisible()) {
                layer->update(deltaTime);
            }
        }
    }

    // Render all layers of this plugin
    // Subclasses can override to batch rendering (e.g., single ImGui frame for all ymery layers)
    virtual void renderAll(WebGPUContext& ctx,
                           WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                           uint32_t screenWidth, uint32_t screenHeight,
                           float cellWidth, float cellHeight,
                           int scrollOffset, uint32_t termRows) = 0;

    // Handle terminal resize - notify all layers
    virtual void onTerminalResize(uint32_t cellWidth, uint32_t cellHeight) {
        for (auto& layer : _layers) {
            uint32_t newW = layer->getWidthCells() * cellWidth;
            uint32_t newH = layer->getHeightCells() * cellHeight;
            layer->onResize(newW, newH);
            layer->setNeedsRender(true);
        }
    }

    // Check if plugin is initialized
    bool isInitialized() const { return _initialized; }
    void setInitialized(bool v) { _initialized = v; }

protected:
    std::vector<PluginLayerPtr> _layers;
    bool _initialized = false;
};

// C function types for dynamic loading
using PluginNameFn = const char* (*)();
using PluginCreateFn = Result<PluginPtr> (*)();

} // namespace yetty
