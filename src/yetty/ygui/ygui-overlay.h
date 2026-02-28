#pragma once

#include <yetty/result.hpp>
#include <yetty/base/base.h>
#include <yetty/font-manager.h>
#include <yetty/gpu-allocator.h>
#include <webgpu/webgpu.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace yetty {

class YDrawBuffer;
class YDrawBuilder;

namespace ygui {
class YGuiEngine;
class YGuiIO;
struct WidgetEvent;
}

struct YettyContext;

struct ContextMenuItem {
    std::string label;
    base::Event event;
    std::function<void()> callback;
};

/// YGuiOverlay — renders ygui widgets as a transparent fullscreen overlay
/// on top of the terminal content. Replaces ImGui for statusbar, context
/// menus, and other overlay UI.
class YGuiOverlay : public base::EventListener,
                    public base::ObjectFactory<YGuiOverlay> {
public:
    using Ptr = std::shared_ptr<YGuiOverlay>;

    static Result<Ptr> createImpl(ContextType& ctx,
                                   const YettyContext& yettyCtx) noexcept;

    virtual ~YGuiOverlay() = default;

    // EventListener interface
    Result<bool> onEvent(const base::Event& event) override = 0;

    // Display management
    virtual void updateDisplaySize(uint32_t width, uint32_t height) = 0;

    // Statusbar API
    virtual void setStatusText(const std::string& text) = 0;
    virtual void setFps(uint32_t fps) = 0;
    virtual float getStatusbarHeight() const = 0;

    // Context menu API
    virtual void beginContextMenu(float x, float y) = 0;
    virtual void addContextMenuItem(const ContextMenuItem& item) = 0;
    virtual void clearContextMenu() = 0;
    virtual bool isContextMenuOpen() const = 0;

    // GPU Stats dialog
    virtual void showGpuStatsDialog(std::function<std::string()> provider) = 0;

    // Render overlay — call after main scene render, in same render pass
    virtual Result<void> render(WGPURenderPassEncoder pass) = 0;

    // Access ygui engine (for building custom UI)
    virtual ygui::YGuiEngine& engine() = 0;

    // Text measurement
    virtual float measureText(const std::string& text, float fontSize) = 0;

protected:
    YGuiOverlay() = default;
};

} // namespace yetty
