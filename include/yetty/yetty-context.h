#pragma once

#include <yetty/base/types.h>
#include <yetty/gpu-context.h>
#include <yetty/gpu-allocator.h>
#include <yetty/shader-manager.h>
#include <yetty/font-manager.h>
#include <yetty/card-manager.h>
#include <yetty/card-factory.h>
#include <yetty/config.h>
#include <yetty/imgui-manager.h>

#include <memory>

namespace yetty::ymery { class Renderer; }

namespace yetty {

// Application-level context for Yetty components
// Contains GPUContext plus shared managers and application state
struct YettyContext {
    // Low-level GPU handles
    GPUContext gpu;

    // Global GPU allocator (tracks all shared GPU resource allocations)
    GpuAllocator::Ptr globalAllocator;

    // Shared managers
    ShaderManager::Ptr shaderManager;
    FontManager::Ptr fontManager;
    CardFactory::Ptr cardFactory;
    ImguiManager::Ptr imguiManager;

    // Per-screen card manager (set by GPUScreen, not globally)
    CardManager::Ptr cardManager;

    // ID of owning GPUScreen (set by GPUScreen, used for targeted events)
    base::ObjectId screenId = 0;

    // Ymery widget renderer (YAML-driven ImGui widgets)
    std::shared_ptr<ymery::Renderer> ymeryRenderer;

    // Application config (tree-like, runtime writable)
    Config::Ptr config;
};

} // namespace yetty
