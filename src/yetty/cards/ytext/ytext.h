#pragma once

#include <yetty/card.h>
#include <yetty/gpu-context.h>
#include <yetty/yetty-context.h>
#include <yetty/ms-msdf-font.h>
#include <yetty/font-manager.h>
#include <yetty/base/factory.h>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

namespace yetty::card {

//=============================================================================
// YText - GPU-animated text card with shader-driven scrolling
//
// Unlike YDraw, animation (scrolling) happens entirely in the fragment shader
// using the time uniform - no CPU-side spatial hash rebuilding needed.
//
// Architecture:
//   - Text is organized into lines with cumulative Y offsets
//   - Each line contains glyphs with X offsets
//   - Shader uses binary search (Y) + linear search (X) for O(log N + M) lookup
//   - Scroll position computed from: time * scrollSpeed
//
// Supports:
//   - Variable font sizes within same font family
//   - Vertical and horizontal scrolling
//   - Loop, clamp, and ping-pong scroll modes
//   - Smooth edge fading
//=============================================================================

// Per-line metadata (16 bytes)
struct YTextLine {
    uint32_t startGlyph;    // Index into glyph array
    uint32_t glyphCount;    // Number of glyphs in this line
    float yOffset;          // Cumulative Y to this line's top
    float lineHeight;       // Height of this line (max ascender + descender)
};
static_assert(sizeof(YTextLine) == 16, "YTextLine must be 16 bytes");

// Per-glyph data (16 bytes)
struct YTextGlyph {
    float xOffset;          // X position from line start
    float width;            // Rendered width (for hit testing)
    uint32_t glyphIndex;    // Index into MSDF glyph metadata
    uint32_t colorAndScale; // Packed: color RGB (24-bit) + scale (8-bit)
};
static_assert(sizeof(YTextGlyph) == 16, "YTextGlyph must be 16 bytes");

// Scroll modes
enum class YTextScrollMode : uint32_t {
    Clamp = 0,      // Stop at content bounds
    Loop = 1,       // Wrap around
    PingPong = 2,   // Bounce back and forth
};

// Effect modes
enum class YTextEffectMode : uint32_t {
    None = 0,        // Flat text
    CylinderH = 1,   // Horizontal cylinder projection
    CylinderV = 2,   // Vertical cylinder projection
    Sphere = 3,      // Sphere projection
    WaveDispH = 4,   // Horizontal wave displacement (no foreshortening)
    WaveDispV = 5,   // Vertical wave displacement (no foreshortening)
    WaveProjH = 6,   // Horizontal wave projection (foreshortening)
    WaveProjV = 7,   // Vertical wave projection (foreshortening)
    Ripple = 8,      // Concentric water ripple projection
    Perspective = 9, // Star Wars crawl perspective
};

class YText : public Card,
              public base::ObjectFactory<YText> {
public:
    using Ptr = std::shared_ptr<YText>;

    // Shader glyph codepoint (U+100006)
    static constexpr uint32_t SHADER_GLYPH = 0x100006;

    //=========================================================================
    // Factory method (ObjectFactory provides create() automatically)
    //=========================================================================
    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    ~YText() override = default;

    const char* typeName() const override { return "ytext"; }

    //=========================================================================
    // Card interface
    //=========================================================================
    bool needsBuffer() const override { return true; }
    uint32_t metadataSlotIndex() const override { return _metaHandle.offset / 64; }
    Result<void> dispose() override;
    void suspend() override;
    void declareBufferNeeds() override;
    Result<void> allocateBuffers() override;
    Result<void> render(float time) override;

    //=========================================================================
    // Text building API
    //=========================================================================

    // Start a new line (call before adding glyphs to that line)
    void beginLine();

    // Add a glyph to the current line
    void addGlyph(uint32_t codepoint, float fontSize, uint32_t color);

    // Add text string to current line (convenience)
    void addText(const std::string& text, float fontSize, uint32_t color);

    // End current line (computes line height, advances Y cursor)
    void endLine();

    // Set content from plain text (auto line-wrapping)
    void setText(const std::string& text, float fontSize, uint32_t color);

    //=========================================================================
    // Animation settings
    //=========================================================================
    void setScrollSpeedX(float pixelsPerSecond) { _scrollSpeedX = pixelsPerSecond; }
    void setScrollSpeedY(float pixelsPerSecond) { _scrollSpeedY = pixelsPerSecond; }
    void setScrollMode(YTextScrollMode mode) { _scrollMode = mode; }
    void setBgColor(uint32_t color) { _bgColor = color; }

    // Content dimensions (computed after text is added)
    float contentWidth() const { return _contentWidth; }
    float contentHeight() const { return _contentHeight; }

    // Constructor (public for make_shared, use create() instead)
    YText(const YettyContext& ctx,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells,
          const std::string& args, const std::string& payload);

    Result<void> init();

private:
    // Metadata structure (matches shader layout - 64 bytes = 16 u32s)
    // Packed layout to maximize space efficiency
    struct Metadata {
        // Word 0: lineOffset(16) | lineCount(16)
        uint32_t lineOffsetAndCount;
        // Word 1: glyphOffset(16) | glyphCount(16)
        uint32_t glyphOffsetAndCount;
        // Words 2-5: floats that need full precision
        float contentWidth;
        float contentHeight;
        float scrollSpeedX;
        float scrollSpeedY;
        // Word 6: widthCells(12) | heightCells(12) | scrollMode(2) | effectMode(4) | reserved(2)
        uint32_t packedCellsAndModes;
        // Words 7-8: effect parameters
        float effectStrength;
        float frequency;
        // Words 9-10: colors (RGBA, 0 = use terminal default)
        uint32_t bgColor;
        uint32_t fgColor;
        // Words 11-12: tilt for Star Wars effect
        float tiltX;
        float tiltY;
        // Words 13-15: reserved for future use
        uint32_t reserved[3];
    };
    static_assert(sizeof(Metadata) == 64, "Metadata must be 64 bytes");

    void parseArgs(const std::string& args);
    Result<void> parsePayload(const std::string& payload);
    void computeLayout();
    Result<void> uploadData();
    Result<void> uploadMetadata();

    // Font
    FontManager::Ptr _fontManager;
    MsMsdfFont::Ptr _font;
    float _baseFontSize = 16.0f;

    // Text data (CPU staging)
    std::vector<YTextLine> _lines;
    std::vector<YTextGlyph> _glyphs;

    // Current line building state
    float _curLineY = 0.0f;
    float _curLineX = 0.0f;
    float _curLineMaxAscender = 0.0f;
    float _curLineMaxDescender = 0.0f;
    uint32_t _curLineStartGlyph = 0;

    // Content dimensions
    float _contentWidth = 0.0f;
    float _contentHeight = 0.0f;

    // Animation
    float _scrollSpeedX = 0.0f;
    float _scrollSpeedY = 0.0f;
    YTextScrollMode _scrollMode = YTextScrollMode::Clamp;

    // 3D Effects
    YTextEffectMode _effectMode = YTextEffectMode::None;
    float _effectStrength = 0.8f;   // Curvature intensity (0-1)
    float _frequency = 3.0f;        // Wave/ripple frequency
    float _tiltX = 0.0f;            // Tilt around X axis (radians)
    float _tiltY = 0.0f;            // Tilt around Y axis (radians)

    // Appearance (0 = use terminal default)
    uint32_t _fgColor = 0;  // 0 = use terminal default
    uint32_t _bgColor = 0;  // 0 = use terminal default

    static uint32_t parseColor(const std::string& str);

    // GPU storage
    StorageHandle _dataStorage = StorageHandle::invalid();
    uint32_t _lineOffset = 0;
    uint32_t _glyphOffset = 0;

    // State
    bool _dirty = true;
    bool _metadataDirty = true;

    std::string _argsStr;
    std::string _payloadStr;
};

} // namespace yetty::card
