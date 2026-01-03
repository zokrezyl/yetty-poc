#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace yetty {

// Forward declarations
class Grid;
class WebGPUContext;
class Renderable;

//=============================================================================
// YettyCommand - base class for all commands
//=============================================================================
class YettyCommand {
public:
    virtual ~YettyCommand() = default;

    // Execute the command. Returns true on success.
    virtual bool execute(WebGPUContext& ctx) = 0;

    // Rendering order (lower = rendered first)
    virtual uint32_t zOrder() const = 0;

    // Command type for dispatch
    enum class Type {
        // GPU commands
        GridRender,
        OverlayRender,
        Clear,
        DeleteTexture,
        DeleteBuffer,

        // Engine commands (renderable lifecycle)
        CreateRenderable,
        DeleteRenderable,
        StopRenderable,
        StartRenderable,
    };

    virtual Type type() const = 0;

    // Helper to check if this is an engine command
    bool isEngineCommand() const {
        auto t = type();
        return t == Type::CreateRenderable ||
               t == Type::DeleteRenderable ||
               t == Type::StopRenderable ||
               t == Type::StartRenderable;
    }
};

//=============================================================================
// GPU Commands
//=============================================================================

// Render terminal grid
class GridRenderCmd : public YettyCommand {
public:
    GridRenderCmd(uint32_t terminalId, const Grid* grid,
                  int cursorCol, int cursorRow, bool cursorVisible,
                  uint32_t zOrder = 0)
        : terminalId_(terminalId), grid_(grid)
        , cursorCol_(cursorCol), cursorRow_(cursorRow)
        , cursorVisible_(cursorVisible), zOrder_(zOrder) {}

    bool execute(WebGPUContext& ctx) override;
    uint32_t zOrder() const override { return zOrder_; }
    Type type() const override { return Type::GridRender; }

    uint32_t terminalId() const { return terminalId_; }
    const Grid* grid() const { return grid_; }
    int cursorCol() const { return cursorCol_; }
    int cursorRow() const { return cursorRow_; }
    bool cursorVisible() const { return cursorVisible_; }

private:
    uint32_t terminalId_;
    const Grid* grid_;
    int cursorCol_;
    int cursorRow_;
    bool cursorVisible_;
    uint32_t zOrder_;
};

// Render overlay (plugin content)
class OverlayRenderCmd : public YettyCommand {
public:
    OverlayRenderCmd(uint32_t pluginId, float x, float y, float w, float h,
                     uint32_t zOrder = 100)
        : pluginId_(pluginId), x_(x), y_(y), w_(w), h_(h), zOrder_(zOrder) {}

    bool execute(WebGPUContext& ctx) override;
    uint32_t zOrder() const override { return zOrder_; }
    Type type() const override { return Type::OverlayRender; }

private:
    uint32_t pluginId_;
    float x_, y_, w_, h_;
    uint32_t zOrder_;
};

// Clear screen
class ClearCmd : public YettyCommand {
public:
    ClearCmd(float r, float g, float b, float a)
        : r_(r), g_(g), b_(b), a_(a) {}

    bool execute(WebGPUContext& ctx) override;
    uint32_t zOrder() const override { return 0; }  // Always first
    Type type() const override { return Type::Clear; }

private:
    float r_, g_, b_, a_;
};

// Delete GPU texture
class DeleteTextureCmd : public YettyCommand {
public:
    explicit DeleteTextureCmd(uint64_t textureHandle)
        : textureHandle_(textureHandle) {}

    bool execute(WebGPUContext& ctx) override;
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::DeleteTexture; }

private:
    uint64_t textureHandle_;
};

// Delete GPU buffer
class DeleteBufferCmd : public YettyCommand {
public:
    explicit DeleteBufferCmd(uint64_t bufferHandle)
        : bufferHandle_(bufferHandle) {}

    bool execute(WebGPUContext& ctx) override;
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::DeleteBuffer; }

private:
    uint64_t bufferHandle_;
};

//=============================================================================
// Engine Commands (Renderable lifecycle)
//=============================================================================

// Create a new renderable
class CreateRenderableCmd : public YettyCommand {
public:
    CreateRenderableCmd(const std::string& renderableType,
                        const std::string& config = "")
        : renderableType_(renderableType), config_(config) {}

    bool execute(WebGPUContext& ctx) override { return true; }  // Handled by engine
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::CreateRenderable; }

    const std::string& renderableType() const { return renderableType_; }
    const std::string& config() const { return config_; }

private:
    std::string renderableType_;
    std::string config_;
};

// Delete a renderable
class DeleteRenderableCmd : public YettyCommand {
public:
    explicit DeleteRenderableCmd(uint32_t renderableId)
        : renderableId_(renderableId) {}

    bool execute(WebGPUContext& ctx) override { return true; }  // Handled by engine
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::DeleteRenderable; }

    uint32_t renderableId() const { return renderableId_; }

private:
    uint32_t renderableId_;
};

// Stop (suspend) a renderable
class StopRenderableCmd : public YettyCommand {
public:
    explicit StopRenderableCmd(uint32_t renderableId)
        : renderableId_(renderableId) {}

    bool execute(WebGPUContext& ctx) override { return true; }  // Handled by engine
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::StopRenderable; }

    uint32_t renderableId() const { return renderableId_; }

private:
    uint32_t renderableId_;
};

// Start (resume) a stopped renderable
class StartRenderableCmd : public YettyCommand {
public:
    explicit StartRenderableCmd(uint32_t renderableId)
        : renderableId_(renderableId) {}

    bool execute(WebGPUContext& ctx) override { return true; }  // Handled by engine
    uint32_t zOrder() const override { return 0; }
    Type type() const override { return Type::StartRenderable; }

    uint32_t renderableId() const { return renderableId_; }

private:
    uint32_t renderableId_;
};

//=============================================================================
// CommandQueue - container for commands from a Renderable
//=============================================================================
class CommandQueue {
public:
    CommandQueue() = default;
    ~CommandQueue() = default;

    // Move-only
    CommandQueue(CommandQueue&&) = default;
    CommandQueue& operator=(CommandQueue&&) = default;
    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;

    // Add command (takes ownership)
    void push(std::unique_ptr<YettyCommand> cmd) {
        commands_.push_back(std::move(cmd));
    }

    // Add command (convenience for in-place construction)
    template<typename T, typename... Args>
    void emplace(Args&&... args) {
        commands_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    // Access commands
    std::vector<std::unique_ptr<YettyCommand>>& commands() { return commands_; }
    const std::vector<std::unique_ptr<YettyCommand>>& commands() const { return commands_; }

    // Clear and reuse
    void clear() { commands_.clear(); }

    // Check state
    bool empty() const { return commands_.empty(); }
    size_t size() const { return commands_.size(); }

    // Reserve capacity (to avoid reallocation)
    void reserve(size_t n) { commands_.reserve(n); }

private:
    std::vector<std::unique_ptr<YettyCommand>> commands_;
};

} // namespace yetty
