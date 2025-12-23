#pragma once

#include <string>
#include <cstdint>
#include <webgpu/webgpu.h>

namespace yetty {

// Forward declarations
class Decorator;
struct WebGPUContext;

// Position mode for decorators
enum class PositionMode {
    Absolute,  // Fixed position, doesn't scroll
    Relative   // Relative to cursor when created, scrolls with content
};

// Plugin interface - implemented by shader, image, video plugins etc.
class DecoratorPlugin {
public:
    virtual ~DecoratorPlugin() = default;

    // Plugin identification
    virtual const char* getId() const = 0;
    virtual const char* getName() const = 0;
    virtual const char* getDescription() const = 0;

    // Called when a new decorator instance is created with this plugin
    // Returns true if payload is valid and decorator was initialized
    virtual bool initialize(Decorator* decorator, const std::string& payload) = 0;

    // Called every frame to update decorator state (animations, videos, etc.)
    virtual void update(Decorator* decorator, double deltaTime) = 0;

    // Render the decorator directly to the target view
    // pixelX, pixelY = top-left corner in pixels
    // pixelW, pixelH = size in pixels
    virtual void render(Decorator* decorator, WebGPUContext& ctx,
                       WGPUTextureView targetView, WGPUTextureFormat targetFormat,
                       uint32_t screenWidth, uint32_t screenHeight,
                       float pixelX, float pixelY, float pixelW, float pixelH) = 0;

    // Called when decorator is destroyed
    virtual void cleanup(Decorator* decorator) = 0;

    // Optional: handle resize
    virtual void onResize(Decorator* decorator, uint32_t newWidth, uint32_t newHeight) {}
};

// Plugin factory function type (for dynamic loading)
using CreatePluginFunc = DecoratorPlugin* (*)();
using DestroyPluginFunc = void (*)(DecoratorPlugin*);

} // namespace yetty
