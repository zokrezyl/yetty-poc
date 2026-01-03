#pragma once

#include <yetty/yetty-command.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace yetty {

//=============================================================================
// Renderable - interface for anything that can produce render commands
//
// Renderables are the building blocks of the Yetty rendering system.
// Each Renderable:
//   - Has a unique ID and a zOrder for rendering priority
//   - Can optionally run a worker thread (decided in start())
//   - Produces commands via get_command_queue()
//   - Manages its own internal state and synchronization
//
// Threading model:
//   - start() is called once after creation
//   - get_command_queue() is called by the main thread each frame
//   - The Renderable decides internally whether to use a worker thread
//   - get_command_queue() should use try_lock to avoid blocking main thread
//
// Ownership model:
//   - When get_command_queue(old_queue) is called, ownership of old_queue
//     is transferred to the Renderable (for recycling)
//   - The returned queue is owned by the caller until next call
//   - If Renderable is busy (try_lock fails), it returns nullptr
//     and still takes ownership of old_queue
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
    // Renderables are kept sorted by this value in the render list.
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

    // Called once after creation. Renderable decides whether to spawn
    // a worker thread here. Called on main thread.
    virtual void start() = 0;

    // Called before destruction. Stop worker thread if any.
    // Called on main thread.
    virtual void stop() = 0;

    // Check if renderable is running
    virtual bool isRunning() const = 0;

    //-------------------------------------------------------------------------
    // Command queue
    //-------------------------------------------------------------------------

    // Get the current command queue for this renderable.
    //
    // Parameters:
    //   old_queue: The queue returned by this renderable in the previous frame.
    //              Ownership is transferred TO the renderable for recycling.
    //              May be nullptr on first call or after renderable was busy.
    //
    // Returns:
    //   - Pointer to the current command queue (caller takes ownership)
    //   - nullptr if the renderable is busy (try_lock failed)
    //
    // Implementation notes:
    //   - Should use try_lock internally to avoid blocking main thread
    //   - If busy, still accept old_queue for later recycling
    //   - Commands should be in the order the renderable wants them executed
    //   - GPU commands will be executed in order within this renderable's batch
    //   - Engine commands (create/delete renderable) are collected and
    //     processed at the start of the NEXT frame
    //
    virtual CommandQueue* get_command_queue(CommandQueue* old_queue) = 0;
};

//=============================================================================
// RenderableFactory - creates Renderables by type name
//=============================================================================

class RenderableFactory {
public:
    using CreateFn = std::function<Renderable::Ptr(uint32_t id, const std::string& config)>;

    static RenderableFactory& instance();

    // Register a renderable type
    void registerType(const std::string& typeName, CreateFn createFn);

    // Create a renderable by type name
    Renderable::Ptr create(const std::string& typeName, uint32_t id,
                           const std::string& config = "");

    // Check if type is registered
    bool hasType(const std::string& typeName) const;

private:
    RenderableFactory() = default;
    std::unordered_map<std::string, CreateFn> factories_;
};

// Macro to register a renderable type
#define REGISTER_RENDERABLE(TypeName, ClassName) \
    static bool _registered_##ClassName = []() { \
        RenderableFactory::instance().registerType(TypeName, \
            [](uint32_t id, const std::string& config) -> Renderable::Ptr { \
                return std::make_shared<ClassName>(id, config); \
            }); \
        return true; \
    }()

} // namespace yetty
