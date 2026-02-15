#pragma once

#include <yetty/card.h>
#include <yetty/base/factory.h>
#include <yetty/yetty-context.h>
#include <string>
#include <memory>

// Re-export widget types from ygui library
#include "../../ygui/ygui-widgets.h"

namespace yetty::card {

// Re-export from ygui library namespace
using ygui::Widget;
using ygui::WidgetPtr;
using ygui::WidgetType;
using ygui::WidgetFlags;
using ygui::WIDGET_HOVER;
using ygui::WIDGET_PRESSED;
using ygui::WIDGET_FOCUSED;
using ygui::WIDGET_DISABLED;
using ygui::WIDGET_CHECKED;
using ygui::WIDGET_OPEN;

//=============================================================================
// YGui - Retained mode UI card using SDF rendering
//=============================================================================
class YGui : public Card,
             public base::ObjectFactory<YGui> {
public:
    using Ptr = std::shared_ptr<YGui>;

    static constexpr uint32_t SHADER_GLYPH = 0x100003;  // Same as ydraw

    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    ~YGui() override = default;
    const char* typeName() const override { return "ygui"; }
    bool needsBuffer() const override { return true; }

protected:
    using Card::Card;
};

} // namespace yetty::card
