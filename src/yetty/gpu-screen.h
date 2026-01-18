#pragma once

#include <cstdint>
#include <vector>
#include <deque>
#include <functional>
#include <string>
#include "terminal-backend.h"  // For ScrollbackStyle, ScrollbackLine

extern "C" {
#include <vterm.h>
}

namespace yetty {

class Font;

//=============================================================================
// Cell structure - contains all per-cell data in a single struct
// This enables single-memcpy scroll operations instead of 4 separate copies
//=============================================================================
struct Cell {
    uint16_t glyph;     // glyph index in font atlas
    uint8_t fg[4];      // RGBA foreground color
    uint8_t bg[4];      // RGBA background color
    uint8_t attrs;      // packed attributes (bold, italic, underline, strike)
    uint8_t _pad;       // padding to 12 bytes for alignment
};
static_assert(sizeof(Cell) == 12, "Cell must be 12 bytes for optimal alignment");

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
// Buffer layout (matches GridRenderer expectations):
//   - glyphIndices: uint16_t per cell (glyph index in font atlas)
//   - fgColors: 4 bytes per cell (RGBA)
//   - bgColors: 4 bytes per cell (RGBA)
//   - attrs: 1 byte per cell (packed attributes)
//=============================================================================

class GPUScreen {
public:
    GPUScreen(int rows, int cols, Font* font, size_t maxScrollback = 10000);
    ~GPUScreen();

    // Attach to vterm (registers State callbacks)
    void attach(VTerm* vt);

    // Resize buffers
    void resize(int rows, int cols);

    // Reset screen (clear all)
    void reset();

    //=========================================================================
    // Buffer access for GPU upload - returns VIEW buffer (handles scrollback)
    //=========================================================================
    const uint16_t* getGlyphData() const;
    const uint8_t* getFgColorData() const;
    const uint8_t* getBgColorData() const;
    const uint8_t* getAttrsData() const;

    // Buffer sizes
    size_t getGlyphDataSize() const { return static_cast<size_t>(rows_ * cols_) * sizeof(uint16_t); }
    size_t getFgColorDataSize() const { return static_cast<size_t>(rows_ * cols_) * 4; }
    size_t getBgColorDataSize() const { return static_cast<size_t>(rows_ * cols_) * 4; }
    size_t getAttrsDataSize() const { return static_cast<size_t>(rows_ * cols_); }

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
    //=========================================================================
    void setWidgetMarker(int row, int col, uint16_t widgetId);
    void clearWidgetMarker(int row, int col);

    // Scan for widget markers (SIMD optimized) - returns all found positions
    std::vector<WidgetPosition> scanWidgetPositions() const;

    // Track widget whose marker has scrolled into scrollback
    // Called by Terminal when widget Y goes from 0 to -1
    void trackScrolledOutWidget(uint16_t widgetId, int col);

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
    void setCell(int row, int col, uint16_t glyph,
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

    // Extract Cell buffer to separate GPU arrays
    void extractToGPUBuffers() const;

    // Check if cell is a protected widget marker - INLINE for performance
    bool isWidgetMarkerCell(int row, int col) const {
        if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return false;
        if (!visibleBuffer_) return false;
        size_t idx = static_cast<size_t>(row * cols_ + col);
        if (idx >= visibleBuffer_->size()) return false;
        const Cell& cell = (*visibleBuffer_)[idx];
        if (cell.glyph != 0xFFFF) return false;  // GLYPH_PLUGIN
        return cell.fg[2] == 0xFF && cell.fg[3] == 0xFF &&
               cell.bg[0] == 0xAA && cell.bg[1] == 0xAA;
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
    // GPU extraction buffers - extracted from Cell buffers for GPU upload
    // Only extracted when gpuBuffersDirty_ is true
    //=========================================================================
    mutable std::vector<uint16_t> gpuGlyphs_;
    mutable std::vector<uint8_t> gpuFgColors_;
    mutable std::vector<uint8_t> gpuBgColors_;
    mutable std::vector<uint8_t> gpuAttrs_;
    mutable bool gpuBuffersDirty_ = true;

    //=========================================================================
    // Scrollback - compressed line storage (primary screen only)
    //=========================================================================
    std::deque<ScrollbackLineGPU> scrollback_;
    size_t maxScrollback_;
    int scrollOffset_ = 0;  // 0 = live view, >0 = viewing history

    int rows_;
    int cols_;
    Font* font_;
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
    uint16_t cachedSpaceGlyph_ = 0;
};

} // namespace yetty
