#pragma once

#include <yetty/card-buffer-manager.h>
#include <yetty/base/event-listener.h>
#include <yetty/gpu-context.h>
#include <yetty/result.hpp>
#include <memory>
#include <string>

namespace yetty {

/**
 * Card - Base class for card-based widgets rendered via shader glyphs
 *
 * Cards are lightweight widgets that:
 * - Allocate metadata/storage in CardBufferManager
 * - Are rendered entirely by GPU shaders (no texture uploads)
 * - Receive mouse/keyboard input via EventListener interface
 *
 * The metadataOffset serves as the unique identifier for the card's lifetime.
 */
class Card : public base::EventListener {
public:
    using Ptr = std::shared_ptr<Card>;

    virtual ~Card() = default;

    // Non-copyable
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;

    //=========================================================================
    // Lifecycle
    //=========================================================================
    virtual Result<void> dispose() = 0;
    virtual void suspend() = 0;

    //=========================================================================
    // EventListener interface - for mouse/keyboard input
    // Default implementation returns Ok(false) - event not handled
    //=========================================================================
    Result<bool> onEvent(const base::Event& event) override;

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
    virtual Result<void> update(float time) { (void)time; return Ok(); }

    // Called by gpu-screen when cell pixel size changes (e.g. zoom)
    virtual void setCellSize(uint32_t cellWidth, uint32_t cellHeight) {
        (void)cellWidth; (void)cellHeight;
    }

    // Called by gpu-screen to tell the card its viewport origin (window coordinates)
    void setScreenOrigin(float x, float y) { _screenOriginX = x; _screenOriginY = y; }

    //=========================================================================
    // Accessors
    //=========================================================================

    // Raw byte offset in metadata buffer
    uint32_t metadataOffset() const { return _metaHandle.offset; }

    // Slot index for ANSI encoding - must be implemented by each card
    virtual uint32_t metadataSlotIndex() const = 0;

    // Shader glyph codepoint
    uint32_t shaderGlyph() const { return _shaderGlyph; }

    // Position in grid cells
    int32_t x() const { return _x; }
    int32_t y() const { return _y; }
    void setPosition(int32_t x, int32_t y) { _x = x; _y = y; }

    // Size in grid cells
    uint32_t widthCells() const { return _widthCells; }
    uint32_t heightCells() const { return _heightCells; }

    // Card type name
    virtual const char* typeName() const = 0;

protected:
    Card(CardBufferManager::Ptr mgr, const GPUContext& gpu,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr))
        , _gpu(gpu)
        , _x(x)
        , _y(y)
        , _widthCells(widthCells)
        , _heightCells(heightCells)
    {}

    CardBufferManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x;
    int32_t _y;
    uint32_t _widthCells;
    uint32_t _heightCells;
    float _screenOriginX = 0.0f;
    float _screenOriginY = 0.0f;
};

using CardPtr = Card::Ptr;

} // namespace yetty
