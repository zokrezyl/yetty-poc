#pragma once

#include <yetty/base/factory.h>
#include <string>

namespace yetty {

// InputManager - translates platform input into base::Event
//
// Implementations hook into platform-specific input sources and inject
// events directly into EventLoop. No methods needed - just instantiate.
//
// Platform implementations:
// - glfw.cpp: GLFW callbacks -> EventLoop (Linux, macOS, Windows)
// - android.cpp: AInputEvent -> EventLoop
// - webasm.cpp: Emscripten callbacks -> EventLoop
//
class InputManager : public base::ThreadSingleton<InputManager> {
public:
    using Ptr = std::shared_ptr<InputManager>;

    static Result<Ptr> createImpl();

    virtual ~InputManager() = default;

    // Key name lookup (for displaying key bindings)
    virtual std::string getKeyName(int key, int scancode) const = 0;

protected:
    InputManager() = default;
};

} // namespace yetty
