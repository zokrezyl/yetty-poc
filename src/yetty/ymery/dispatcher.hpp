#pragma once

#include "types.hpp"
#include <yetty/base/object.h>
#include <map>
#include <vector>
#include <functional>
#include <memory>

namespace yetty::ymery {

// Event handler callback type
using EventHandler = std::function<Result<void>(const Dict&)>;

// Dispatcher - pub/sub for events and actions
class Dispatcher : public base::Object {
public:
    using Ptr = std::shared_ptr<Dispatcher>;

    static Result<Ptr> create();

    // Event handlers (fire-and-forget, key = "source/name")
    Result<void> registerEventHandler(const std::string& key, EventHandler handler);
    Result<void> unregisterEventHandler(const std::string& key);
    Result<void> dispatchEvent(const Dict& event);

    // Action handlers (require responder)
    using ActionHandler = std::function<Result<void>(const Dict&)>;
    Result<void> registerActionHandler(ActionHandler handler);
    Result<void> dispatchAction(const Dict& action);

    const char* typeName() const override { return "Dispatcher"; }

private:
    Dispatcher() = default;

    std::map<std::string, std::vector<EventHandler>> _eventHandlers;
    std::vector<ActionHandler> _actionHandlers;
};

using DispatcherPtr = std::shared_ptr<Dispatcher>;

} // namespace yetty::ymery
