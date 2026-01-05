// Stub implementations of PluginManager for Android
// Full plugin support will be added later

#include "yetty/plugin-manager.h"

namespace yetty {

PluginManager::PluginManager() noexcept {}
PluginManager::~PluginManager() = default;

Result<PluginManager::Ptr> PluginManager::create() noexcept {
    auto pm = Ptr(new PluginManager());
    return Ok(pm);
}

Result<void> PluginManager::init() noexcept {
    return Ok();
}

uint16_t PluginManager::onCellSync(uint32_t, uint32_t, uint32_t, uint32_t) { return 0; }
void PluginManager::onAltScreenChange(bool) {}
void PluginManager::onScroll(int, Grid*) {}

bool PluginManager::handleOSCSequence(
    const std::string&, Grid*, int, int,
    uint32_t, uint32_t, std::string*, uint32_t*) {
    return false;
}

// Stub remaining methods that may be called
void PluginManager::registerPlugin(const std::string&, BuiltinPluginFactory) {}
void PluginManager::loadPluginsFromDirectory(const std::string&) {}

Result<PluginLayerPtr> PluginManager::createLayer(const std::string&,
                                                   PositionMode, int32_t, int32_t,
                                                   int32_t, int32_t,
                                                   const std::string&, Grid*,
                                                   uint32_t, uint32_t) {
    return Err<PluginLayerPtr>("Plugins not supported on Android");
}

Result<void> PluginManager::updateLayer(const std::string&, const std::string&) {
    return Ok();
}

Result<void> PluginManager::removeLayer(const std::string&, Grid*) {
    return Ok();
}

PluginLayerPtr PluginManager::getLayer(const std::string&) {
    return nullptr;
}

PluginLayerPtr PluginManager::getLayerById(uint32_t) {
    return nullptr;
}

std::vector<PluginLayerPtr> PluginManager::getAllLayers() const {
    return {};
}

Result<void> PluginManager::update(double) {
    return Ok();
}

Result<void> PluginManager::render(WebGPUContext&, WGPUTextureView,
                                    uint32_t, uint32_t, float, float, int, uint32_t) {
    return Ok();
}

Result<void> PluginManager::onTerminalResize(uint32_t, uint32_t, uint32_t, uint32_t) {
    return Ok();
}

bool PluginManager::onMouseMove(float, float, const Grid*, float, float, int) {
    return false;
}

bool PluginManager::onMouseButton(int, bool, float, float, const Grid*, float, float, int) {
    return false;
}

bool PluginManager::onMouseScroll(float, float, int, float, float, const Grid*, float, float, int) {
    return false;
}

bool PluginManager::onKey(int, int, int, int) {
    return false;
}

bool PluginManager::onChar(unsigned int) {
    return false;
}

void PluginManager::clearFocus() {}

std::string PluginManager::base94Decode(const std::string&) {
    return "";
}

std::string PluginManager::base94Encode(const std::string&) {
    return "";
}

void PluginManager::registerCustomGlyphPlugin(CustomGlyphPluginPtr) {}

CustomGlyphPluginPtr PluginManager::getCustomGlyphPluginForCodepoint(uint32_t) {
    return nullptr;
}

void PluginManager::onCellClear(uint32_t, uint32_t) {}

Result<void> PluginManager::renderCustomGlyphs(WebGPUContext&, WGPUTextureView,
                                                uint32_t, uint32_t, float, float, int) {
    return Ok();
}

void PluginManager::updateCustomGlyphs(double) {}

Result<void> PluginManager::initFrameRenderer(WGPUDevice, WGPUTextureFormat) {
    return Ok();
}

void PluginManager::renderFrame(WebGPUContext&, WGPUTextureView,
                                 uint32_t, uint32_t, float, float, float, float,
                                 float, float, float, float) {}

Result<PluginPtr> PluginManager::getOrCreatePlugin(const std::string&) {
    return Err<PluginPtr>("Plugins not supported on Android");
}

PluginLayerPtr PluginManager::layerAtCell(int, int, const Grid*) {
    return nullptr;
}

void PluginManager::markGridCells(Grid*, PluginLayer*) {}
void PluginManager::clearGridCells(Grid*, PluginLayer*) {}

} // namespace yetty
