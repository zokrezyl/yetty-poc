#pragma once

namespace yetty {

struct Config {
    // Rendering options
    bool _useDamageTracking = true;  // Only update changed cells (from libvterm damage rects)
    bool _showFps = true;            // Print FPS to stdout

    // Scrollback
    uint32_t _scrollbackLines = 10000;  // Number of lines to keep in scrollback buffer

    // Debug options
    bool _debugDamageRects = false;  // Log damage rect updates
};

} // namespace yetty
