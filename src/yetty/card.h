#pragma once

#include "card-buffer-manager.h"
#include <yetty/result.hpp>
#include <memory>
#include <string>

namespace yetty {

//=============================================================================
// Card - Base class for card-based widgets rendered via shader glyphs
//
// Cards are lightweight widgets that:
// - Allocate metadata/storage in CardBufferManager
// - Are rendered entirely by GPU shaders (no texture uploads)
// - Receive mouse/keyboard input via GPUScreen routing
//
// The metadataOffset serves as the unique identifier for the card's lifetime.
//=============================================================================

class Card {
public:
    virtual ~Card() = default;

    // Non-copyable
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;

    //=========================================================================
    // Lifecycle - init() has NO arguments, all passed via constructor
    //=========================================================================
    virtual Result<void> init() = 0;
    virtual void dispose() = 0;

    //=========================================================================
    // Input handling (coordinates in local UV space 0-1)
    //=========================================================================
    virtual void onMouseMove(float u, float v) { (void)u; (void)v; }
    virtual void onMouseDown(float u, float v, int button) { (void)u; (void)v; (void)button; }
    virtual void onMouseUp(float u, float v, int button) { (void)u; (void)v; (void)button; }
    virtual void onMouseScroll(float u, float v, float deltaX, float deltaY) {
        (void)u; (void)v; (void)deltaX; (void)deltaY;
    }
    virtual void onKeyDown(uint32_t codepoint, int mods) { (void)codepoint; (void)mods; }
    virtual void onKeyUp(uint32_t codepoint, int mods) { (void)codepoint; (void)mods; }

    //=========================================================================
    // Per-frame update (called if card is visible)
    //=========================================================================
    virtual void update(float time) { (void)time; }

    //=========================================================================
    // Accessors - GPUScreen needs these to mark cells and route input
    //=========================================================================

    // Raw byte offset in metadata buffer (for internal use)
    uint32_t metadataOffset() const { return metaHandle_.offset; }

    // Slot index for ANSI encoding (offset / 32)
    // Metadata is always 32-byte aligned, so we can address 512MB with 24 bits
    // This is what gets encoded in the fg color for ANSI true-color sequences
    uint32_t metadataSlotIndex() const { return metaHandle_.offset / 32; }

    // Shader glyph codepoint (in Plane 16 PUA-B range)
    uint32_t shaderGlyph() const { return shaderGlyph_; }

    // Position in grid cells
    int32_t x() const { return x_; }
    int32_t y() const { return y_; }
    void setPosition(int32_t x, int32_t y) { x_ = x; y_ = y; }

    // Size in grid cells
    uint32_t widthCells() const { return widthCells_; }
    uint32_t heightCells() const { return heightCells_; }

    // Card type name (for debugging/serialization)
    virtual const char* typeName() const = 0;

protected:
    //=========================================================================
    // Constructor - called by subclass create() factory method
    //=========================================================================
    Card(CardBufferManager* mgr, int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
        : cardMgr_(mgr)
        , x_(x)
        , y_(y)
        , widthCells_(widthCells)
        , heightCells_(heightCells)
    {}

    CardBufferManager* cardMgr_;
    MetadataHandle metaHandle_ = MetadataHandle::invalid();
    uint32_t shaderGlyph_ = 0;
    int32_t x_;
    int32_t y_;
    uint32_t widthCells_;
    uint32_t heightCells_;
};

using CardPtr = std::unique_ptr<Card>;

} // namespace yetty
