#pragma once

#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <webgpu/webgpu.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

struct ImGuiContext;

namespace yetty {

struct YettyContext;

//-----------------------------------------------------------------------------
// ContextMenuItem - represents a single menu item with the event to dispatch
//-----------------------------------------------------------------------------
struct ContextMenuItem {
    std::string label;
    base::Event event;                    // The event to dispatch when this item is clicked
    std::function<void()> callback;       // Optional: direct callback instead of event dispatch
};

//-----------------------------------------------------------------------------
// ImguiManager - manages ImGui context and context menus
//-----------------------------------------------------------------------------
class ImguiManager : public base::EventListener, public base::ObjectFactory<ImguiManager> {
public:
    using Ptr = std::shared_ptr<ImguiManager>;

    // Factory method - creates ImguiManagerImpl
    static Result<Ptr> createImpl(ContextType& ctx, const YettyContext& yettyCtx) noexcept;

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override = 0;

    virtual ~ImguiManager() = default;

    // Update display size (call on window resize)
    virtual void updateDisplaySize(uint32_t width, uint32_t height) = 0;

    // Context menu API - multiple sources add items during render, menu opens automatically
    virtual void beginContextMenu(float x, float y) = 0;  // Called on right-click to set position and clear items
    virtual void addContextMenuItem(const ContextMenuItem& item) = 0;  // Add item (call during render cycle)
    virtual void clearContextMenu() = 0;

    // Statusbar API
    virtual void setStatusText(const std::string& text) = 0;
    virtual void setFps(uint32_t fps) = 0;
    virtual float getStatusbarHeight() const = 0;

    // Render ImGui - call after main scene render, in same render pass
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // GPU Stats dialog - provider is called every frame for live data
    virtual void showGpuStatsDialog(std::function<std::string()> provider) = 0;

    // Check if context menu is open
    virtual bool isContextMenuOpen() const = 0;

    // Get ImGui context (for advanced usage)
    virtual ImGuiContext* context() const = 0;

protected:
    ImguiManager() = default;
};

} // namespace yetty
