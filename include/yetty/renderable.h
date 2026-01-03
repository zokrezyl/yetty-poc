#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace yetty {

class WebGPUContext;

//=============================================================================
// Renderable - interface for anything that can render itself
//
// Each Renderable:
//   - Has a unique ID and a zOrder for rendering priority
//   - Can optionally run a worker thread (for data processing)
//   - Manages its own GPU resources (shaders, buffers, textures)
//   - Renders itself when render() is called by the main loop
//
// Threading model:
//   - start() is called once after creation (may spawn worker thread)
//   - render() is called by main thread each frame
//   - If using worker thread, renderable handles synchronization internally
//=============================================================================

class Renderable {
public:
    using Ptr = std::shared_ptr<Renderable>;

    virtual ~Renderable() = default;

    //-------------------------------------------------------------------------
    // Identity
    //-------------------------------------------------------------------------

    // Unique identifier for this renderable instance
    virtual uint32_t id() const = 0;

    // Rendering order. Lower values render first (background).
    // Typical values:
    //   0-99:    Terminals (background)
    //   100-199: UI overlays
    //   200+:    Plugin overlays
    virtual uint32_t zOrder() const = 0;

    // Human-readable name for debugging
    virtual const std::string& name() const = 0;

    //-------------------------------------------------------------------------
    // Lifecycle
    //-------------------------------------------------------------------------

    // Called once after creation. May spawn worker thread.
    virtual void start() = 0;

    // Called before destruction. Stop worker thread if any.
    virtual void stop() = 0;

    // Check if renderable is running
    virtual bool isRunning() const = 0;

    //-------------------------------------------------------------------------
    // Rendering
    //-------------------------------------------------------------------------

    // Render this renderable. Called by main loop each frame.
    // The renderable manages its own GPU resources and does all WebGPU calls.
    // Returns true if something was rendered, false if skipped (no damage).
    virtual bool render(WebGPUContext& ctx) = 0;
};

//=============================================================================
// RenderableFactory - creates Renderables by type name
//=============================================================================

class RenderableFactory {
public:
    using CreateFn = std::function<Renderable::Ptr(uint32_t id, const std::string& config)>;

    static RenderableFactory& instance();

    void registerType(const std::string& typeName, CreateFn createFn);
    Renderable::Ptr create(const std::string& typeName, uint32_t id,
                           const std::string& config = "");
    bool hasType(const std::string& typeName) const;

private:
    RenderableFactory() = default;
    std::unordered_map<std::string, CreateFn> factories_;
};

#define REGISTER_RENDERABLE(TypeName, ClassName) \
    static bool _registered_##ClassName = []() { \
        RenderableFactory::instance().registerType(TypeName, \
            [](uint32_t id, const std::string& config) -> Renderable::Ptr { \
                return std::make_shared<ClassName>(id, config); \
            }); \
        return true; \
    }()

} // namespace yetty
