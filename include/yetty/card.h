#pragma once

#include <yetty/card-buffer-manager.h>
#include <yetty/base/event-listener.h>
#include <yetty/gpu-context.h>
#include <yetty/result.hpp>
#include <memory>
#include <string>
#include <vector>

namespace yetty {

/**
 * Card - Base class for card-based widgets rendered via shader glyphs
 *
 * Cards are lightweight widgets that:
 * - Allocate metadata in CardBufferManager
 * - Are rendered entirely by GPU shaders
 * - Receive mouse/keyboard input via EventListener interface
 *
 * Two subclasses:
 *   TextureCard - produces RGBA pixels, written to atlas texture
 *   BufferCard  - produces linear data, written to GPU storage buffer
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

    // Is this a texture card? (for gpu-screen two-pass rendering)
    virtual bool isTextureCard() const { return false; }

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

/**
 * TextureCard - Card that produces RGBA pixels written to the atlas texture.
 *
 * Two-pass rendering driven by gpu-screen:
 *   Pass 1: producePixels() - card renders into its local CPU pixel buffer
 *   Pass 2: writeToAtlas()  - card writes pixels to atlas via handle
 *
 * Between passes, gpu-screen calls cardManager->packAtlas() to assign positions.
 */
class TextureCard : public Card {
public:
    bool isTextureCard() const override { return true; }

    // Pass 1: Render/prepare pixels into local CPU buffer, link to texture handle.
    // Called by gpu-screen when atlas needs rebuilding.
    // Default: no-op (cards that do everything in update() need not override).
    virtual Result<void> producePixels() { return Ok(); }

    // Pass 2: Write pixels to atlas and update metadata with atlas position.
    // Called by gpu-screen after packAtlas().
    // Default: no-op (atlas writes are driven by the manager's writeAllToAtlas()).
    virtual Result<void> writeToAtlas() { return Ok(); }

    // Get the texture handle (for gpu-screen to check validity)
    TextureHandle textureHandle() const { return _textureHandle; }

protected:
    TextureCard(CardBufferManager::Ptr mgr, const GPUContext& gpu,
                int32_t x, int32_t y,
                uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}

    TextureHandle _textureHandle = TextureHandle::invalid();
    bool _metadataDirty = true;
    uint32_t _cellWidth = 0;
    uint32_t _cellHeight = 0;
};

/**
 * BufferCard - Card that produces linear data written to GPU storage buffer.
 * Used by plot, ydraw, kdraw, hdraw cards.
 */
class BufferCard : public Card {
public:
    bool isTextureCard() const override { return false; }

protected:
    BufferCard(CardBufferManager::Ptr mgr, const GPUContext& gpu,
               int32_t x, int32_t y,
               uint32_t widthCells, uint32_t heightCells)
        : Card(std::move(mgr), gpu, x, y, widthCells, heightCells)
    {}
};

using CardPtr = Card::Ptr;

} // namespace yetty
