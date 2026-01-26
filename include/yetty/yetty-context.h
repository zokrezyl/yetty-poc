#pragma once

#include <yetty/gpu-context.h>
#include <yetty/shader-manager.h>
#include <yetty/font-manager.h>
#include <yetty/card-buffer-manager.h>
#include <yetty/card-factory.h>

namespace yetty {

// Application-level context for Yetty components
// Contains GPUContext plus shared managers and application state
struct YettyContext {
    // Low-level GPU handles
    GPUContext gpu;

    // Shared managers
    ShaderManager::Ptr shaderManager;
    FontManager::Ptr fontManager;
    CardBufferManager::Ptr cardBufferManager;
    CardFactory::Ptr cardFactory;
};

} // namespace yetty
