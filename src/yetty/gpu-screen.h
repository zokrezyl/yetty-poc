#pragma once

#include <cstdint>
#include <vector>
#include <deque>
#include <functional>
#include <string>
#include <unordered_map>
#include "terminal-backend.h"  // For ScrollbackStyle, ScrollbackLine
#include "card.h"              // For CardPtr
#include <yetty/osc-command.h>
#include <yetty/widget.h>
#include <yetty/yetty-font.h>

extern "C" {
#include <vterm.h>
}

namespace yetty {

class WidgetFactory;
class CardBufferManager;
class CardFactory;
class Card;

//=============================================================================
// Cell structure - GPU-ready format, uploaded directly to shader (zero-copy)
// 12 bytes per cell, aligned for efficient GPU access
//=============================================================================
struct Cell {
    uint32_t glyph;     // 4 bytes - glyph index (room for shader-based glyphs, extended features)
    uint8_t fgR;        // 1 byte
    uint8_t fgG;        // 1 byte
    uint8_t fgB;        // 1 byte
    uint8_t alpha;      // 1 byte - for future transparent text effects
    uint8_t bgR;        // 1 byte
    uint8_t bgG;        // 1 byte
    uint8_t bgB;        // 1 byte
    uint8_t style;      // 1 byte - packed attributes (bold, italic, underline, strike)
};
static_assert(sizeof(Cell) == 12, "Cell must be 12 bytes for GPU alignment");

//=============================================================================
// GPUScreen scrollback line - stores cells directly (no separate arrays)
//=============================================================================
struct ScrollbackLineGPU {
    std::vector<Cell> cells;  // direct cell storage
};

//=============================================================================
// Widget position found by scanning glyph buffer
//=============================================================================
struct WidgetPosition {
    uint16_t widgetId;
    int row;
    int col;
};

//=============================================================================
// Widget whose marker has scrolled into scrollback buffer
//=============================================================================
struct ScrolledOutWidget {
    uint16_t widgetId;
    int y;    // Current Y position (negative, decremented on each scroll)
    int col;  // Column position of the marker
};

//=============================================================================
// GPUScreen - Direct vterm State layer to GPU buffer with integrated scrollback
//
// Architecture:
//   - visibleBuffer_: GPU-ready arrays where vterm State callbacks write
//   - scrollback_: compressed storage of scrolled-off lines
//   - viewBuffer_: GPU-ready arrays for rendering (== visibleBuffer_ when not scrolling)
//
// When scrollOffset_ == 0: render from visibleBuffer_ (zero-copy from vterm)
// When scrollOffset_ > 0: compose viewBuffer_ from scrollback + visible
//
// Buffer layout: Cell struct (12 bytes per cell) uploaded directly to GPU
//   - glyph: uint32 (4 bytes) - glyph index, room for shader features
//   - fgR, fgG, fgB, alpha: 4 bytes - foreground color with alpha
//   - bgR, bgG, bgB, style: 4 bytes - background color and attributes
//=============================================================================

class GPUScreen {
public:
    GPUScreen(int rows, int cols, YettyFont::Ptr terminalFont, size_t maxScrollback = 10000);
    ~GPUScreen();

    // Attach to vterm (registers State callbacks)
    void attach(VTerm* vt);

    // Resize buffers
    void resize(int rows, int cols);

    // Reset screen (clear all)
    void reset();

    //=========================================================================
    // Buffer access for GPU upload - returns Cell buffer directly (zero-copy)
    //=========================================================================
    const Cell* getCellData() const;
    size_t getCellDataSize() const { return static_cast<size_t>(rows_ * cols_) * sizeof(Cell); }
    size_t getCellCount() const { return static_cast<size_t>(rows_ * cols_); }

    // Dimensions
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    //=========================================================================
    // Scrollback control
    //=========================================================================
    void scrollUp(int lines);      // Scroll back into history
    void scrollDown(int lines);    // Scroll toward live
    void scrollToTop();            // View oldest scrollback
    void scrollToBottom();         // Return to live view
    
    int getScrollOffset() const { return scrollOffset_; }
    size_t getScrollbackSize() const { return scrollback_.size(); }
    bool isScrolledBack() const { return scrollOffset_ > 0; }

    //=========================================================================
    // Cursor state (updated by movecursor callback)
    //=========================================================================
    int getCursorRow() const { return cursorRow_; }
    int getCursorCol() const { return cursorCol_; }
    bool isCursorVisible() const { return cursorVisible_ && scrollOffset_ == 0; }

    //=========================================================================
    // Widget markers - encode widget ID in glyph=0xFFFF cells, ID in fg bytes
    // For shader glyphs (cards), all cells in the widget region share the same
    // glyph and attributes
    //=========================================================================
    void setWidgetMarker(int row, int col, uint16_t widgetId);
    void setWidgetMarker(int row, int col, uint16_t widgetId, uint32_t width, uint32_t height,
                         uint32_t shaderGlyph, uint32_t fg, uint32_t bg);
    void clearWidgetMarker(int row, int col);
    void clearWidgetMarker(int row, int col, uint32_t width, uint32_t height);

    // Scan for widget markers (SIMD optimized) - returns all found positions
    std::vector<WidgetPosition> scanWidgetPositions() const;

    // Track widget whose marker has scrolled into scrollback
    // Called by Terminal when widget Y goes from 0 to -1
    void trackScrolledOutWidget(uint16_t widgetId, int col);

    //=========================================================================
    // Widget management (legacy plugin-based widgets)
    //=========================================================================
    void setWidgetFactory(WidgetFactory* factory) { widgetFactory_ = factory; }
    void setCardBufferManager(CardBufferManager* mgr) { cardBufferManager_ = mgr; }
    void setCardFactory(CardFactory* factory) { cardFactory_ = factory; }
    void setVTerm(VTerm* vt) { vterm_ = vt; }
    void setPtyWriteCallback(std::function<void(const char*, size_t)> cb) { ptyWriteCallback_ = cb; }
    void setVTermInputCallback(std::function<void(const char*, size_t)> cb) { vtermInputCallback_ = cb; }
    void setIsAltScreen(bool alt) { isAltScreenExternal_ = alt; }

    // OSC sequence handling (widget creation/management)
    // Handles both legacy plugins (YETTY_OSC_VENDOR_ID) and cards (YETTY_OSC_VENDOR_CARD_ID)
    bool handleOSCSequence(const std::string& sequence,
                           std::string* response = nullptr,
                           uint32_t* linesToAdvance = nullptr);

    // Card-specific OSC handling (called by handleOSCSequence for YETTY_OSC_VENDOR_CARD_ID)
    bool handleCardOSCSequence(const std::string& sequence,
                               std::string* response = nullptr,
                               uint32_t* linesToAdvance = nullptr);

    //=========================================================================
    // Card management (new shader-glyph based system)
    //=========================================================================

    // Get card at a cell position (for input routing)
    // Returns nullptr if no card at that position
    Card* getCardAtCell(int row, int col) const;

    // Get card by metadata offset (unique identifier)
    Card* getCardByMetadataOffset(uint32_t offset) const;

    // Register a card in the registry (called after card creation)
    void registerCard(Card* card);

    // Unregister a card from the registry (called before card disposal)
    void unregisterCard(Card* card);

    // Get all cards
    const std::unordered_map<uint32_t, Card*>& getCards() const { return cardRegistry_; }

    // Child widget management
    void addChildWidget(WidgetPtr widget);
    Result<void> removeChildWidget(uint32_t id);
    Result<void> removeChildWidgetByHashId(const std::string& hashId);
    WidgetPtr getChildWidget(uint32_t id) const;
    WidgetPtr getChildWidgetByHashId(const std::string& hashId) const;
    const std::vector<WidgetPtr>& getChildWidgets() const { return childWidgets_; }

    // Mark/clear grid cells for widget region
    void markWidgetGridCells(Widget* widget);
    void clearWidgetGridCells(Widget* widget);

    // Damage tracking
    bool hasDamage() const { return hasDamage_; }
    void clearDamage() { hasDamage_ = false; }
    void markDamage() { hasDamage_ = true; }

    //=========================================================================
    // Callbacks
    //=========================================================================
    using TermPropCallback = std::function<void(VTermProp prop, VTermValue* val)>;
    void setTermPropCallback(TermPropCallback cb) { termPropCallback_ = cb; }

    using BellCallback = std::function<void()>;
    void setBellCallback(BellCallback cb) { bellCallback_ = cb; }
    
    using ScrollCallback = std::function<void(int lines)>;
    void setScrollCallback(ScrollCallback cb) { scrollCallback_ = cb; }

    // Called when a widget's marker is removed from scrollback (widget should be disposed)
    using WidgetDisposalCallback = std::function<void(uint16_t widgetId)>;
    void setWidgetDisposalCallback(WidgetDisposalCallback cb) { widgetDisposalCallback_ = cb; }

    //=========================================================================
    // State callbacks (public for C callback struct initialization)
    //=========================================================================
    static int onPutglyph(VTermGlyphInfo* info, VTermPos pos, void* user);
    static int onMoveCursor(VTermPos pos, VTermPos oldpos, int visible, void* user);
    static int onScrollRect(VTermRect rect, int downward, int rightward, void* user);
    static int onMoveRect(VTermRect dest, VTermRect src, void* user);
    static int onErase(VTermRect rect, int selective, void* user);
    static int onInitPen(void* user);
    static int onSetPenAttr(VTermAttr attr, VTermValue* val, void* user);
    static int onSetTermProp(VTermProp prop, VTermValue* val, void* user);
    static int onBell(void* user);
    static int onResize(int rows, int cols, VTermStateFields* fields, void* user);
    static int onSetLineInfo(int row, const VTermLineInfo* newinfo, 
                             const VTermLineInfo* oldinfo, void* user);

private:
    //=========================================================================
    // Internal helpers
    //=========================================================================
    void setCell(int row, int col, uint32_t glyph,
                 uint8_t fgR, uint8_t fgG, uint8_t fgB,
                 uint8_t bgR, uint8_t bgG, uint8_t bgB,
                 uint8_t attrs);
    void clearCell(int row, int col);
    size_t cellIndex(int row, int col) const {
        return static_cast<size_t>(row * cols_ + col);
    }
    void colorToRGB(const VTermColor& color, uint8_t& r, uint8_t& g, uint8_t& b);

    // Switch between primary and alternate screen
    void switchToScreen(bool alt);
    void clearBuffer(std::vector<Cell>& buffer);

    // Check if cell is a protected widget marker - INLINE for performance
    // Marker pattern: glyph=GLYPH_PLUGIN, fgB=0xFF, alpha=0xFF, bgR=0xAA, bgG=0xAA
    bool isWidgetMarkerCell(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return false;
        if (!visibleBuffer_) return false;
        size_t idx = static_cast<size_t>(row * cols_ + col);
        if (idx >= visibleBuffer_->size()) return false;
        const Cell& cell = (*visibleBuffer_)[idx];
        if (cell.glyph != 0xFFFF) return false;  // GLYPH_PLUGIN
        return cell.fgB == 0xFF && cell.alpha == 0xFF && cell.bgR == 0xAA && cell.bgG == 0xAA;
    }

    // Scrollback helpers
    void pushLineToScrollback(int row);
    void composeViewBuffer();
    void decompressLine(const ScrollbackLineGPU& line, int viewRow);

    //=========================================================================
    // Primary screen buffer - where vterm State callbacks write for main screen
    // Uses Cell struct for single-memcpy scroll operations
    // Widget markers: glyph=0xFFFF, widgetId in fg[0..1]
    //=========================================================================
    std::vector<Cell> primaryBuffer_;

    //=========================================================================
    // Alternate screen buffer - for apps like vim, htop, less
    // No scrollback for alternate screen (traditional terminal behavior)
    //=========================================================================
    std::vector<Cell> altBuffer_;

    //=========================================================================
    // Pointer to current active screen (primary or alternate)
    //=========================================================================
    std::vector<Cell>* visibleBuffer_ = nullptr;

    bool isAltScreen_ = false;

    //=========================================================================
    // View buffer - what gets rendered (== visible when scrollOffset_==0)
    //=========================================================================
    std::vector<Cell> viewBuffer_;

    //=========================================================================
    // Scrollback - compressed line storage (primary screen only)
    //=========================================================================
    std::deque<ScrollbackLineGPU> scrollback_;
    size_t maxScrollback_;
    int scrollOffset_ = 0;  // 0 = live view, >0 = viewing history

    int rows_;
    int cols_;
    YettyFont::Ptr terminalFont_;
    VTerm* vterm_ = nullptr;
    VTermState* state_ = nullptr;

    // Current pen state (updated by setpenattr)
    struct Pen {
        VTermColor fg;
        VTermColor bg;
        bool bold = false;
        bool italic = false;
        uint8_t underline = 0;
        bool strike = false;
        bool reverse = false;
        bool blink = false;
    } pen_;

    VTermColor defaultFg_;
    VTermColor defaultBg_;

    int cursorRow_ = 0;
    int cursorCol_ = 0;
    bool cursorVisible_ = true;

    bool hasDamage_ = true;
    bool viewBufferDirty_ = true;  // Need to recompose view buffer

    // Callbacks
    TermPropCallback termPropCallback_;
    BellCallback bellCallback_;
    ScrollCallback scrollCallback_;
    WidgetDisposalCallback widgetDisposalCallback_;

    // Track widgets whose markers have scrolled into scrollback
    std::vector<ScrolledOutWidget> scrolledOutWidgets_;

    // Pre-allocated scratch buffer for onMoveRect (avoid allocation per call)
    std::vector<Cell> scratchBuffer_;

    // Cached space glyph index (initialized from font in constructor)
    uint32_t cachedSpaceGlyph_ = 0;

    //=========================================================================
    // Widget management (legacy plugin-based widgets)
    //=========================================================================
    WidgetFactory* widgetFactory_ = nullptr;
    CardBufferManager* cardBufferManager_ = nullptr;

    std::vector<WidgetPtr> childWidgets_;
    std::unordered_map<std::string, WidgetPtr> childWidgetsByHashId_;
    OscCommandParser oscParser_;
    uint32_t nextChildWidgetId_ = 1;

    // External state from Terminal (needed for widget creation)
    bool isAltScreenExternal_ = false;
    std::function<void(const char*, size_t)> ptyWriteCallback_;
    std::function<void(const char*, size_t)> vtermInputCallback_;

    //=========================================================================
    // Card management (new shader-glyph based system)
    //=========================================================================
    CardFactory* cardFactory_ = nullptr;

    // Storage for created cards (owns the cards)
    std::vector<CardPtr> cards_;

    // Registry: metadataOffset -> Card* for input routing
    std::unordered_map<uint32_t, Card*> cardRegistry_;
};

} // namespace yetty
