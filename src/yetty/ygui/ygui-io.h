#pragma once

#include <string>

namespace yetty::ygui {

//=============================================================================
// WidgetEvent — structured event emitted by widget interactions
//=============================================================================
struct WidgetEvent {
    std::string widgetId;
    std::string eventType;  // "click", "change", "scroll"
    std::string key;        // "value", "checked", "selected", "text", "open", "color"
    std::string value;      // serialized value
};

//=============================================================================
// YGuiIO — abstraction for event output
//
// In-process: formats as OSC and logs/writes to stdout
// Remote:     serializes and sends over network (future)
//=============================================================================
class YGuiIO {
public:
    virtual ~YGuiIO() = default;
    virtual void emitEvent(const WidgetEvent& event) = 0;
};

} // namespace yetty::ygui
