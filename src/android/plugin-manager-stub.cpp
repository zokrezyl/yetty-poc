// Stub implementations of PluginManager for Android
// Full plugin support will be added later

#include "yetty/plugin-manager.h"

namespace yetty {

PluginManager::PluginManager() {}
PluginManager::~PluginManager() {}

uint16_t PluginManager::onCellSync(uint32_t, uint32_t, uint32_t, uint32_t) { return 0; }
void PluginManager::onAltScreenChange(bool) {}
void PluginManager::onScroll(int, Grid*) {}

bool PluginManager::handleOSCSequence(
    const std::string&, Grid*, int, int,
    uint32_t, uint32_t, std::string*, uint32_t*) {
    return false;
}

} // namespace yetty
