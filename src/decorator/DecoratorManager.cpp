#include "DecoratorManager.h"
#include "../terminal/Grid.h"
#include "../renderer/WebGPUContext.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <dlfcn.h>

namespace yetty {

DecoratorManager::DecoratorManager() {
}

DecoratorManager::~DecoratorManager() {
    // Clean up decorators first (they reference plugins)
    for (auto& decorator : decorators_) {
        if (decorator && decorator->getPlugin()) {
            decorator->getPlugin()->cleanup(decorator.get());
        }
    }
    decorators_.clear();
    plugins_.clear();
}

void DecoratorManager::loadPluginsFromDirectory(const std::string& path) {
    namespace fs = std::filesystem;

    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "Plugin directory not found: " << path << std::endl;
        return;
    }

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            const auto& filePath = entry.path();
            std::string ext = filePath.extension().string();

            // Load shared libraries (.so on Linux, .dylib on macOS)
#ifdef __APPLE__
            if (ext != ".dylib") continue;
#else
            if (ext != ".so") continue;
#endif

            void* handle = dlopen(filePath.c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Failed to load plugin " << filePath << ": " << dlerror() << std::endl;
                continue;
            }

            auto createFunc = reinterpret_cast<CreatePluginFunc>(dlsym(handle, "createPlugin"));
            if (!createFunc) {
                std::cerr << "Plugin " << filePath << " missing createPlugin symbol" << std::endl;
                dlclose(handle);
                continue;
            }

            DecoratorPlugin* plugin = createFunc();
            if (plugin) {
                std::cout << "Loaded plugin: " << plugin->getName()
                          << " (id: " << plugin->getId() << ")" << std::endl;
                plugins_[plugin->getId()] = std::unique_ptr<DecoratorPlugin>(plugin);
            }
        }
    }
}

void DecoratorManager::registerPlugin(std::unique_ptr<DecoratorPlugin> plugin) {
    if (plugin) {
        std::string id = plugin->getId();
        plugins_[id] = std::move(plugin);
    }
}

DecoratorPlugin* DecoratorManager::getPlugin(const std::string& id) const {
    auto it = plugins_.find(id);
    return (it != plugins_.end()) ? it->second.get() : nullptr;
}

uint32_t DecoratorManager::createDecorator(const std::string& pluginId,
                                           PositionMode mode,
                                           int32_t x, int32_t y,
                                           uint32_t widthCells, uint32_t heightCells,
                                           const std::string& payload,
                                           Grid* grid,
                                           uint32_t cellWidth, uint32_t cellHeight) {
    DecoratorPlugin* plugin = getPlugin(pluginId);
    if (!plugin) {
        std::cerr << "Unknown plugin: " << pluginId << std::endl;
        return 0;
    }

    uint32_t id = nextDecoratorId_++;
    auto decorator = std::make_unique<Decorator>(id, plugin, mode, x, y, widthCells, heightCells);

    // Calculate pixel size
    decorator->setPixelSize(widthCells * cellWidth, heightCells * cellHeight);
    decorator->setPayload(payload);

    // Initialize with plugin
    if (!plugin->initialize(decorator.get(), payload)) {
        std::cerr << "Plugin " << pluginId << " failed to initialize decorator" << std::endl;
        return 0;
    }

    // Mark grid cells
    if (grid) {
        markGridCells(grid, decorator.get());
    }

    decorators_.push_back(std::move(decorator));
    return id;
}

bool DecoratorManager::updateDecorator(uint32_t id, const std::string& payload) {
    Decorator* decorator = getDecorator(id);
    if (!decorator) return false;

    decorator->setPayload(payload);
    decorator->setNeedsRender(true);

    // Re-initialize with new payload
    if (decorator->getPlugin()) {
        decorator->getPlugin()->cleanup(decorator);
        return decorator->getPlugin()->initialize(decorator, payload);
    }
    return false;
}

bool DecoratorManager::removeDecorator(uint32_t id, Grid* grid) {
    for (auto it = decorators_.begin(); it != decorators_.end(); ++it) {
        if ((*it)->getId() == id) {
            if (grid) {
                clearGridCells(grid, it->get());
            }
            if ((*it)->getPlugin()) {
                (*it)->getPlugin()->cleanup(it->get());
            }
            decorators_.erase(it);
            return true;
        }
    }
    return false;
}

Decorator* DecoratorManager::getDecorator(uint32_t id) {
    for (auto& decorator : decorators_) {
        if (decorator->getId() == id) {
            return decorator.get();
        }
    }
    return nullptr;
}

// Helper: find the Nth semicolon and return position after it (or npos if not found)
static size_t findNthSemicolon(const std::string& s, int n) {
    size_t pos = 0;
    for (int i = 0; i < n; i++) {
        pos = s.find(';', pos);
        if (pos == std::string::npos) return pos;
        pos++;  // Move past the semicolon
    }
    return pos;
}

// Helper: extract field between semicolon positions
static std::string getField(const std::string& s, int fieldIndex) {
    size_t start = (fieldIndex == 0) ? 0 : findNthSemicolon(s, fieldIndex);
    if (start == std::string::npos) return "";
    size_t end = s.find(';', start);
    if (end == std::string::npos) return s.substr(start);
    return s.substr(start, end - start);
}

bool DecoratorManager::handleOSCSequence(const std::string& sequence,
                                         Grid* grid,
                                         int cursorCol, int cursorRow,
                                         uint32_t cellWidth, uint32_t cellHeight) {
    // Format: <vendor_id>;<plugin_id>;<mode>;...
    // Base94 payload may contain ';', so we use fixed field positions.

    // Field 0: vendor_id
    // Field 1: plugin_id
    // Field 2: mode (A/R/U/D)
    // For A/R: fields 3-6 are x,y,w,h; field 7+ is payload
    // For U: field 3 is id; field 4+ is payload
    // For D: field 3 is id

    std::string vendorStr = getField(sequence, 0);
    if (vendorStr.empty()) return false;

    int vendorId = 0;
    try {
        vendorId = std::stoi(vendorStr);
    } catch (...) {
        return false;
    }

    if (vendorId != YETTY_OSC_VENDOR_ID) {
        return false;
    }

    std::string pluginId = getField(sequence, 1);
    std::string mode = getField(sequence, 2);

    if (pluginId.empty() || mode.empty()) {
        return false;
    }

    if (mode == "D") {
        // Delete: 99999;<plugin>;D;<id>
        // 4 fields, 3 semicolons
        std::string idStr = getField(sequence, 3);
        if (idStr.empty()) return false;
        uint32_t decoratorId = std::stoul(idStr);
        return removeDecorator(decoratorId, grid);
    }

    if (mode == "U") {
        // Update: 99999;<plugin>;U;<id>;<payload>
        // 5 fields, 4 semicolons. Payload is everything after 4th semicolon.
        std::string idStr = getField(sequence, 3);
        if (idStr.empty()) return false;
        uint32_t decoratorId = std::stoul(idStr);

        size_t payloadStart = findNthSemicolon(sequence, 4);
        std::string encodedPayload = (payloadStart != std::string::npos)
            ? sequence.substr(payloadStart) : "";
        std::string payload = base94Decode(encodedPayload);
        return updateDecorator(decoratorId, payload);
    }

    if (mode == "A" || mode == "R") {
        // Create: 99999;<plugin>;<A|R>;<x>;<y>;<w>;<h>;<payload>
        // 8 fields, 7 semicolons. Payload is everything after 7th semicolon.
        std::string xStr = getField(sequence, 3);
        std::string yStr = getField(sequence, 4);
        std::string wStr = getField(sequence, 5);
        std::string hStr = getField(sequence, 6);

        if (xStr.empty() || yStr.empty() || wStr.empty() || hStr.empty()) {
            return false;
        }

        PositionMode posMode = (mode == "A") ? PositionMode::Absolute : PositionMode::Relative;
        int32_t x = std::stoi(xStr);
        int32_t y = std::stoi(yStr);
        uint32_t w = std::stoul(wStr);
        uint32_t h = std::stoul(hStr);

        size_t payloadStart = findNthSemicolon(sequence, 7);
        std::string encodedPayload = (payloadStart != std::string::npos)
            ? sequence.substr(payloadStart) : "";
        std::string payload = base94Decode(encodedPayload);

        if (posMode == PositionMode::Relative) {
            x += cursorCol;
            y += cursorRow;
        }

        uint32_t id = createDecorator(pluginId, posMode, x, y, w, h, payload,
                                      grid, cellWidth, cellHeight);
        if (id > 0) {
            std::cout << "Created decorator id=" << id << " plugin=" << pluginId
                      << " at (" << x << "," << y << ") size " << w << "x" << h << std::endl;
            return true;
        }
    }

    return false;
}

void DecoratorManager::update(double deltaTime) {
    for (auto& decorator : decorators_) {
        if (decorator->isVisible() && decorator->getPlugin()) {
            decorator->getPlugin()->update(decorator.get(), deltaTime);
        }
    }
}

void DecoratorManager::render(WebGPUContext& ctx, WGPUTextureView targetView,
                              uint32_t screenWidth, uint32_t screenHeight,
                              float cellWidth, float cellHeight) {
    if (!targetView) return;

    for (auto& decorator : decorators_) {
        if (decorator->isVisible() && decorator->getPlugin()) {
            // Calculate pixel position and size
            float pixelX = decorator->getX() * cellWidth;
            float pixelY = decorator->getY() * cellHeight;
            float pixelW = decorator->getWidthCells() * cellWidth;
            float pixelH = decorator->getHeightCells() * cellHeight;

            decorator->getPlugin()->render(
                decorator.get(), ctx,
                targetView, ctx.getSurfaceFormat(),
                screenWidth, screenHeight,
                pixelX, pixelY, pixelW, pixelH
            );
        }
    }
}

void DecoratorManager::onTerminalResize(uint32_t newCols, uint32_t newRows,
                                        uint32_t cellWidth, uint32_t cellHeight) {
    for (auto& decorator : decorators_) {
        // Update pixel size based on new cell dimensions
        decorator->setPixelSize(decorator->getWidthCells() * cellWidth,
                                decorator->getHeightCells() * cellHeight);

        if (decorator->getPlugin()) {
            decorator->getPlugin()->onResize(decorator.get(),
                                             decorator->getPixelWidth(),
                                             decorator->getPixelHeight());
        }
        decorator->setNeedsRender(true);
    }
}

void DecoratorManager::onScroll(int lines) {
    for (auto& decorator : decorators_) {
        if (decorator->getPositionMode() == PositionMode::Relative) {
            // Adjust Y position when terminal scrolls
            decorator->setPosition(decorator->getX(), decorator->getY() - lines);
        }
    }
}

void DecoratorManager::markGridCells(Grid* grid, Decorator* decorator) {
    if (!grid || !decorator) return;

    int32_t startX = decorator->getX();
    int32_t startY = decorator->getY();
    uint32_t w = decorator->getWidthCells();
    uint32_t h = decorator->getHeightCells();
    uint32_t id = decorator->getId();

    // Encode decorator ID into fg color (24 bits)
    uint8_t idR = (id >> 16) & 0xFF;
    uint8_t idG = (id >> 8) & 0xFF;
    uint8_t idB = id & 0xFF;

    for (uint32_t row = 0; row < h; row++) {
        for (uint32_t col = 0; col < w; col++) {
            int32_t gridCol = startX + col;
            int32_t gridRow = startY + row;

            if (gridCol >= 0 && gridCol < (int32_t)grid->getCols() &&
                gridRow >= 0 && gridRow < (int32_t)grid->getRows()) {
                grid->setCell(gridCol, gridRow,
                              GLYPH_DECORATOR,
                              idR, idG, idB,    // fg = decorator ID
                              0, 0, 0);         // bg = reserved
            }
        }
    }
}

void DecoratorManager::clearGridCells(Grid* grid, Decorator* decorator) {
    if (!grid || !decorator) return;

    int32_t startX = decorator->getX();
    int32_t startY = decorator->getY();
    uint32_t w = decorator->getWidthCells();
    uint32_t h = decorator->getHeightCells();

    for (uint32_t row = 0; row < h; row++) {
        for (uint32_t col = 0; col < w; col++) {
            int32_t gridCol = startX + col;
            int32_t gridRow = startY + row;

            if (gridCol >= 0 && gridCol < (int32_t)grid->getCols() &&
                gridRow >= 0 && gridRow < (int32_t)grid->getRows()) {
                // Reset to space with default colors
                grid->setCell(gridCol, gridRow, ' ', 255, 255, 255, 0, 0, 0);
            }
        }
    }
}

// Base94 encoding uses printable ASCII from '!' (33) to '~' (126)
// Encoding: byte -> chr(33 + byte // 94) + chr(33 + byte % 94)
// Decoding: (c1 - 33) * 94 + (c2 - 33) = byte
std::string DecoratorManager::base94Decode(const std::string& encoded) {
    if (encoded.empty()) return "";

    std::string result;
    result.reserve(encoded.size() / 2);

    for (size_t i = 0; i + 1 < encoded.size(); i += 2) {
        int v1 = encoded[i] - '!';
        int v2 = encoded[i + 1] - '!';

        // Validate range
        if (v1 < 0 || v1 > 2 || v2 < 0 || v2 >= 94) {
            std::cerr << "base94Decode: invalid chars at " << i << ": "
                      << (int)encoded[i] << ", " << (int)encoded[i+1] << std::endl;
            continue;
        }

        int byte = v1 * 94 + v2;
        if (byte > 255) {
            std::cerr << "base94Decode: decoded value out of range: " << byte << std::endl;
            continue;
        }

        result.push_back(static_cast<char>(byte));
    }

    return result;
}

std::string DecoratorManager::base94Encode(const std::string& data) {
    std::string result;
    result.reserve(data.size() * 2);

    for (unsigned char byte : data) {
        int value = byte;
        char c1 = '!' + (value / 94);
        char c2 = '!' + (value % 94);
        result.push_back(c1);
        result.push_back(c2);
    }

    return result;
}

} // namespace yetty
