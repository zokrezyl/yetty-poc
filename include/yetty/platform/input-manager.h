#pragma once

#include <yetty/base/factory.h>

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

protected:
    InputManager() = default;
};

} // namespace yetty
