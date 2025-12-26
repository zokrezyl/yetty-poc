#pragma once

namespace yetty {

struct Config {
    // Rendering options
    bool useDamageTracking = true;  // Only update changed cells (from libvterm damage rects)
    bool showFps = true;            // Print FPS to stdout

    // Scrollback
    uint32_t scrollbackLines = 10000;  // Number of lines to keep in scrollback buffer

    // Debug options
    bool debugDamageRects = false;  // Log damage rect updates
};

} // namespace yetty
