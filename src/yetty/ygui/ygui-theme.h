#pragma once

#include <cstdint>

namespace yetty::ygui {

//=============================================================================
// Theme — centralised style constants for all ygui widgets
//=============================================================================
struct Theme {
    // Spacing / sizing
    float padSmall      = 2.0f;
    float padMedium     = 4.0f;
    float padLarge      = 8.0f;
    float radiusSmall   = 2.0f;
    float radiusMedium  = 4.0f;
    float radiusLarge   = 8.0f;
    float rowHeight     = 24.0f;
    float scrollbarSize = 12.0f;
    float scrollSpeed   = 20.0f;
    float fontSize      = 14.0f;
    float separatorSize = 1.0f;

    // Surface colours (ABGR)
    uint32_t bgPrimary   = 0xFF1A1A2E;  // deepest background
    uint32_t bgSecondary = 0xFF222233;  // scrollbar tracks, tooltip bg
    uint32_t bgSurface   = 0xFF2A2A3E;  // widget default bg, selected option
    uint32_t bgHover     = 0xFF333344;  // hover highlight
    uint32_t bgHeader    = 0xFF3A3A4E;  // table/popup header
    uint32_t bgDropdown  = 0xFF1E1E2E;  // dropdown menu

    // Border / muted colours
    uint32_t border      = 0xFF444455;
    uint32_t borderLight = 0xFF555566;
    uint32_t borderMuted = 0xFF666677;

    // Text colours
    uint32_t textPrimary = 0xFFFFFFFF;
    uint32_t textMuted   = 0xFFAAAAAA;

    // Overlay
    uint32_t overlayModal = 0x80000000;
    uint32_t shadow       = 0x40000000;
    uint32_t tooltipBg    = 0xF0222233;

    // Scrollbar thumb
    uint32_t thumbNormal  = 0xFF444455;
    uint32_t thumbHover   = 0xFF555566;

    // Text selection
    uint32_t selectionBg  = 0xFF2244AA;
};

/// Global default theme (returned by RenderContext when none is set).
inline const Theme& defaultTheme() {
    static Theme t;
    return t;
}

} // namespace yetty::ygui
