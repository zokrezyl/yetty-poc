#pragma once

#include <yetty/gpu-context.h>
#include <yetty/shader-manager.h>
#include <yetty/font-manager.h>

namespace yetty {

// Application-level context for Yetty components
// Contains GPUContext plus shared managers and application state
struct YettyContext {
    // Low-level GPU handles
    GPUContext gpu;

    // Shared managers
    ShaderManager::Ptr shaderManager;
    FontManager::Ptr fontManager;
};

} // namespace yetty
