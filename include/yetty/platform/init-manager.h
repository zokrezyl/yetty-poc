#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <functional>

#if defined(__ANDROID__)
struct android_app;
#endif

namespace yetty {

// InitManager - platform initialization and main thread event loop
//
// All platforms except WebASM. Created once in main(), not a singleton.
//
// Responsibilities:
// - Initialize platform (glfwInit, etc.)
// - Spawn render thread
// - Block on main thread event loop (glfwWaitEvents)
//
// Window creation happens on render thread via GlfwWindowSingleton.
// glfwWaitEvents() is GLOBAL - doesn't need window handle.
//
class InitManager : public base::Object, public base::ObjectFactory<InitManager> {
public:
    using Ptr = std::shared_ptr<InitManager>;

    static Result<Ptr> createImpl();

    virtual ~InitManager() = default;

    // Runs the application - creates Yetty, starts event loop, shuts down
    // argc/argv used for Config creation
    virtual Result<void> run(int argc = 0, char** argv = nullptr) = 0;

protected:
    InitManager() = default;
};

#if defined(__ANDROID__)
// Android-specific factory (takes android_app* from NativeActivity)
Result<InitManager::Ptr> createForAndroid(android_app* app);
#endif

} // namespace yetty
