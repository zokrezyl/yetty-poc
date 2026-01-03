#include <yetty/renderable.h>
#include <spdlog/spdlog.h>

namespace yetty {

//=============================================================================
// RenderableFactory
//=============================================================================

RenderableFactory& RenderableFactory::instance() {
    static RenderableFactory factory;
    return factory;
}

void RenderableFactory::registerType(const std::string& typeName, CreateFn createFn) {
    if (factories_.find(typeName) != factories_.end()) {
        spdlog::warn("RenderableFactory: overwriting existing type '{}'", typeName);
    }
    factories_[typeName] = std::move(createFn);
    spdlog::debug("RenderableFactory: registered type '{}'", typeName);
}

Renderable::Ptr RenderableFactory::create(const std::string& typeName, uint32_t id,
                                           const std::string& config) {
    auto it = factories_.find(typeName);
    if (it == factories_.end()) {
        spdlog::error("RenderableFactory: unknown type '{}'", typeName);
        return nullptr;
    }
    return it->second(id, config);
}

bool RenderableFactory::hasType(const std::string& typeName) const {
    return factories_.find(typeName) != factories_.end();
}

} // namespace yetty
