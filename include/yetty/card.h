#pragma once

#include <yetty/card-manager.h>
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
 * - Allocate metadata in CardManager
 * - Are rendered entirely by GPU shaders
 * - Receive mouse/keyboard input via EventListener interface
 *
 * Cards declare their resource needs via needsBuffer() / needsTexture().
 * Buffer cards use CardBufferManager for linear GPU storage.
 * Texture cards use CardTextureManager for atlas textures.
 * A card may need both.
 *
 * Per-frame lifecycle (orchestrated by gpu-screen):
 *
 *   Phase 1 — renderToStaging(time)    [every frame, all active cards]
 *   Phase 2 — allocation loops          [only when layout changes]
 *   Phase 3 — render()            [every frame, all active cards]
 *   Flush   — manager uploads to GPU
 */
class Card : public base::EventListener {
public:
    using Ptr = std::shared_ptr<Card>;

    virtual ~Card() = default;

    // Non-copyable
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;

    //=========================================================================
    // Resource declarations
    //=========================================================================

    /// Returns true if this card uses buffer storage (CardBufferManager).
    /// Buffer cards participate in the buffer allocation loops.
    /// Examples: ydraw (SDF primitives), plot (float data), qrcode (packed bits).
    virtual bool needsBuffer() const { return false; }

    /// Returns true if this card uses texture storage (CardTextureManager).
    /// Texture cards participate in the atlas allocation and packing loops.
    /// Examples: image (pixel data), pdf (rendered pages), thorvg (SVG renders).
    virtual bool needsTexture() const { return false; }

    //=========================================================================
    // Phase 1: Per-frame staging (called every frame for all active cards)
    //=========================================================================

    /// Perform all per-frame computation into card-owned staging data.
    ///
    /// This is the card's "think" phase — advance animations, recompute layouts,
    /// rebuild spatial structures, process external input, etc. All work happens
    /// in card-private memory; this method must NOT touch GPU handles or call
    /// any CardBufferManager/CardTextureManager write methods.
    ///
    /// After this call, the card knows:
    ///   - What its staging data looks like for this frame
    ///   - Whether its buffer/texture allocation needs changed (reported via
    ///     needsBufferRealloc/needsTextureRealloc)
    ///
    /// @param time  Wall-clock time in seconds since an arbitrary epoch,
    ///              consistent across all cards in a frame.
    virtual void renderToStaging(float time) { (void)time; }

    //=========================================================================
    // Phase 2: Allocation (runs only when buffer/texture layout changes)
    //=========================================================================

    /// Returns true if this card's buffer size needs changed since last allocation.
    /// Checked every frame after renderToStaging(). If any card returns true,
    /// the full buffer repack (declareBufferNeeds → commitReservations →
    /// allocateBuffers) is triggered for ALL buffer cards.
    virtual bool needsBufferRealloc() { return false; }

    /// Returns true if this card's texture size needs changed since last allocation.
    /// Checked every frame after renderToStaging(). If any card returns true,
    /// the full texture repack (clearHandles → allocateTextures → createAtlas →
    /// writeTextures) is triggered for ALL texture cards.
    virtual bool needsTextureRealloc() { return false; }

    /// Declare total buffer byte needs via bufferManager()->reserve().
    /// Called during buffer repack. The card should reserve space for all its
    /// sub-allocations (e.g., primitives, derived data, grid).
    /// Must NOT allocate — only call reserve().
    virtual void declareBufferNeeds() {}

    /// Obtain buffer handles from bufferManager()->allocateBuffer().
    /// Called after commitReservations() has sized the GPU buffer.
    /// The card receives BufferHandle with direct CPU pointers — it should
    /// copy staging data into these handles and call markBufferDirty().
    /// Handles remain valid until the next buffer repack or explicit release.
    virtual Result<void> allocateBuffers() { return Ok(); }

    /// Obtain texture handles from textureManager()->allocate().
    /// Called during texture repack after clearHandles(). The card declares
    /// the width/height it needs. Atlas positions are NOT yet assigned —
    /// that happens in createAtlas() after all cards have allocated.
    virtual Result<void> allocateTextures() { return Ok(); }

    /// Write pixel data into the atlas after createAtlas() has assigned positions.
    /// Called during texture repack. The card should call textureManager()->write()
    /// with its pixel data. Atlas positions are now final.
    virtual Result<void> writeTextures() { return Ok(); }

    //=========================================================================
    // Phase 3: Finalize (called every frame for all active cards)
    //=========================================================================

    /// Finalize staged data before the batch GPU write.
    ///
    /// IMPORTANT: This method does NOT write to GPU. It finalizes/packs data
    /// into CPU staging buffers. The actual GPU write happens later when
    /// gpu-screen calls CardManager::flush() after ALL cards have finalized.
    ///
    /// Typical work done here:
    ///   - Pack metadata (buffer offsets, scene bounds, grid dimensions, etc.)
    ///   - Copy primitives/glyphs into staging buffers
    ///   - Mark buffer regions as dirty for upcoming GPU upload
    ///   - Update animation state, zoom/pan values
    ///
    /// For cards with stable allocations (e.g., plot with external writers),
    /// this may only pack metadata. For animated cards, this may also
    /// update buffer contents in-place.
    ///
    /// Called after allocation is settled — handles are guaranteed valid.
    /// Called every frame, even if nothing changed (card should track _dirty).
    virtual Result<void> finalize() { return Ok(); }

    //=========================================================================
    // Updates (OSC update command)
    //=========================================================================

    /// Update the card with new arguments and/or payload data.
    /// Called when an OSC "update" command targets this card.
    ///
    /// For streaming cards like plot, the args can include:
    ///   - advance=N: Shift buffer left by N elements, append payload at end
    ///   - buffer=name: Target a specific named buffer
    ///
    /// @param args    Card-specific arguments string
    /// @param payload Decoded payload data (e.g., comma-separated floats)
    /// @return Ok() on success, Err on failure
    virtual Result<void> update(const std::string& args, const std::string& payload) {
        (void)args;
        (void)payload;
        return Ok();  // Default: no-op
    }

    //=========================================================================
    // Lifecycle
    //=========================================================================

    /// Release all resources (metadata, buffer handles, texture handles, events).
    /// Called when the card is permanently removed.
    virtual Result<void> dispose() = 0;

    /// Save GPU-resident data back to card-owned staging and release handles.
    /// Called when the card scrolls off-screen. The card must be able to
    /// fully reconstruct its GPU state from staging when reactivated
    /// (via the normal declareBufferNeeds → allocateBuffers path).
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

    // Called by gpu-screen when cell pixel size changes (e.g. zoom)
    virtual void setCellSize(float cellWidth, float cellHeight) {
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

    // User-assigned name for RPC identification (optional)
    const std::string& name() const { return _name; }
    void setName(const std::string& name) { _name = name; }
    bool hasName() const { return !_name.empty(); }

protected:
    Card(CardManager::Ptr mgr, const GPUContext& gpu,
         int32_t x, int32_t y,
         uint32_t widthCells, uint32_t heightCells)
        : _cardMgr(std::move(mgr))
        , _gpu(gpu)
        , _x(x)
        , _y(y)
        , _widthCells(widthCells)
        , _heightCells(heightCells)
    {}

    CardManager::Ptr _cardMgr;
    GPUContext _gpu;
    MetadataHandle _metaHandle = MetadataHandle::invalid();
    uint32_t _shaderGlyph = 0;
    int32_t _x;
    int32_t _y;
    uint32_t _widthCells;
    uint32_t _heightCells;
    float _screenOriginX = 0.0f;
    float _screenOriginY = 0.0f;
    std::string _name;  // User-assigned name for RPC identification
};

using CardPtr = Card::Ptr;

} // namespace yetty
