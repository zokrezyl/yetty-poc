#pragma once

#include "Decorator.h"
#include "DecoratorPlugin.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

namespace yetty {

// Forward declarations
class Grid;
struct WebGPUContext;

// Our vendor ID for OSC sequences (to avoid conflicts with other terminals)
// Format: ESC ] 99999 ; <plugin_id> ; <mode> ; <params> ; <payload> ST
constexpr int YETTY_OSC_VENDOR_ID = 99999;

class DecoratorManager {
public:
    DecoratorManager();
    ~DecoratorManager();

    // Plugin management
    void loadPluginsFromDirectory(const std::string& path);
    void registerPlugin(std::unique_ptr<DecoratorPlugin> plugin);
    DecoratorPlugin* getPlugin(const std::string& id) const;
    const auto& getPlugins() const { return plugins_; }

    // Decorator lifecycle
    // Returns decorator ID, or 0 on failure
    uint32_t createDecorator(const std::string& pluginId,
                             PositionMode mode,
                             int32_t x, int32_t y,
                             uint32_t widthCells, uint32_t heightCells,
                             const std::string& payload,
                             Grid* grid,
                             uint32_t cellWidth, uint32_t cellHeight);

    bool updateDecorator(uint32_t id, const std::string& payload);
    bool removeDecorator(uint32_t id, Grid* grid);
    Decorator* getDecorator(uint32_t id);
    const std::vector<std::unique_ptr<Decorator>>& getDecorators() const { return decorators_; }

    // Parse OSC sequence and create/update/delete decorator
    // Returns true if sequence was handled
    bool handleOSCSequence(const std::string& sequence,
                          Grid* grid,
                          int cursorCol, int cursorRow,
                          uint32_t cellWidth, uint32_t cellHeight);

    // Update all decorators (called each frame)
    void update(double deltaTime);

    // Render all decorators directly to the target
    void render(WebGPUContext& ctx, WGPUTextureView targetView,
                uint32_t screenWidth, uint32_t screenHeight,
                float cellWidth, float cellHeight);

    // Handle terminal resize
    void onTerminalResize(uint32_t newCols, uint32_t newRows,
                          uint32_t cellWidth, uint32_t cellHeight);

    // Handle scroll (for relative positioned decorators)
    void onScroll(int lines);

    // Utility: Base94 decode
    static std::string base94Decode(const std::string& encoded);
    static std::string base94Encode(const std::string& data);

private:
    void markGridCells(Grid* grid, Decorator* decorator);
    void clearGridCells(Grid* grid, Decorator* decorator);

    std::unordered_map<std::string, std::unique_ptr<DecoratorPlugin>> plugins_;
    std::vector<std::unique_ptr<Decorator>> decorators_;
    uint32_t nextDecoratorId_ = 1;  // 0 is reserved for "no decorator"
};

} // namespace yetty
