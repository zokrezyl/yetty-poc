#include "dispatcher.h"

namespace yetty::ymery {

Result<Dispatcher::Ptr> Dispatcher::create() {
    auto dispatcher = Ptr(new Dispatcher());
    return dispatcher;
}

Result<void> Dispatcher::registerEventHandler(const std::string& key, EventHandler handler) {
    _eventHandlers[key].push_back(std::move(handler));
    return Ok();
}

Result<void> Dispatcher::unregisterEventHandler(const std::string& key) {
    _eventHandlers.erase(key);
    return Ok();
}

Result<void> Dispatcher::dispatchEvent(const Dict& event) {
    // Extract source and name from event
    std::string source, name;

    auto sourceIt = event.find("source");
    if (sourceIt != event.end()) {
        if (auto s = getAs<std::string>(sourceIt->second)) {
            source = *s;
        }
    }

    auto nameIt = event.find("name");
    if (nameIt != event.end()) {
        if (auto n = getAs<std::string>(nameIt->second)) {
            name = *n;
        }
    }

    // Build key
    std::string key = source + "/" + name;

    // Find and call handlers
    auto it = _eventHandlers.find(key);
    if (it != _eventHandlers.end()) {
        for (auto& handler : it->second) {
            handler(event);
        }
    }

    // Also try wildcard handlers
    auto wildIt = _eventHandlers.find("*/" + name);
    if (wildIt != _eventHandlers.end()) {
        for (auto& handler : wildIt->second) {
            handler(event);
        }
    }

    return Ok();
}

Result<void> Dispatcher::registerActionHandler(ActionHandler handler) {
    _actionHandlers.push_back(std::move(handler));
    return Ok();
}

Result<void> Dispatcher::dispatchAction(const Dict& action) {
    for (auto& handler : _actionHandlers) {
        auto res = handler(action);
        if (res) {
            return res;
        }
    }
    return Err<void>("Dispatcher::dispatchAction: no handler responded");
}

} // namespace yetty::ymery
