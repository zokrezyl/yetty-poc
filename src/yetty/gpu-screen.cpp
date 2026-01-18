#include "gpu-screen.h"
#include <yetty/font.h>
#include <ytrace/ytrace.hpp>
#include "grid.h"  // For GLYPH_WIDE_CONT, GLYPH_PLUGIN constants
#include <algorithm>
#include <cstring>
#include <string>

#if defined(__SSE2__)
#include <emmintrin.h>
#endif

namespace yetty {

// State callbacks struct
static VTermStateCallbacks stateCallbacks = {
    .putglyph = GPUScreen::onPutglyph,
    .movecursor = GPUScreen::onMoveCursor,
    .scrollrect = GPUScreen::onScrollRect,
    .moverect = GPUScreen::onMoveRect,
    .erase = GPUScreen::onErase,
    .initpen = GPUScreen::onInitPen,
    .setpenattr = GPUScreen::onSetPenAttr,
    .settermprop = GPUScreen::onSetTermProp,
    .bell = GPUScreen::onBell,
    .resize = GPUScreen::onResize,
    .setlineinfo = GPUScreen::onSetLineInfo,
    .sb_clear = nullptr,
};

GPUScreen::GPUScreen(int rows, int cols, Font* font, size_t maxScrollback)
    : maxScrollback_(maxScrollback)
    , rows_(rows)
    , cols_(cols)
    , font_(font)
{
    // Initialize default colors
    vterm_color_rgb(&defaultFg_, 204, 204, 204);  // Light gray
    vterm_color_rgb(&defaultBg_, 15, 15, 35);     // Dark blue-ish

    pen_.fg = defaultFg_;
    pen_.bg = defaultBg_;

    // Cache space glyph index to avoid repeated lookups in hot paths
    cachedSpaceGlyph_ = font_ ? font_->getGlyphIndex(' ') : 0;

    // Start on primary screen
    isAltScreen_ = false;

    // Allocate buffers (this sets up both primary and alternate)
    resize(rows, cols);

    // Point to primary screen by default
    visibleBuffer_ = &primaryBuffer_;
}

GPUScreen::~GPUScreen() = default;

void GPUScreen::attach(VTerm* vt) {
    vterm_ = vt;
    state_ = vterm_obtain_state(vt);
    
    // Register our callbacks with State layer
    vterm_state_set_callbacks(state_, &stateCallbacks, this);
    
    // Get default colors from state
    vterm_state_get_default_colors(state_, &defaultFg_, &defaultBg_);
    pen_.fg = defaultFg_;
    pen_.bg = defaultBg_;

    // Reset state (triggers initpen, clears screen)
    vterm_state_reset(state_, 1);
}

void GPUScreen::resize(int rows, int cols) {
    // Check if this is a no-op (same size AND buffers already allocated)
    if (rows == rows_ && cols == cols_ && !primaryBuffer_.empty()) {
        return;  // No change
    }

    yinfo("GPUScreen::resize: {}x{} -> {}x{} isAltScreen={}", rows_, cols_, rows, cols, isAltScreen_);

    int oldRows = rows_;
    int oldCols = cols_;
    bool hasOldPrimaryContent = !primaryBuffer_.empty();
    bool hasOldAltContent = !altBuffer_.empty();

    // =========================================================================
    // CRITICAL: Push excess lines to scrollback BEFORE moving old buffers
    // This is how libvterm's screen.c handles resize - lines that would be
    // lost due to row count decrease go to scrollback first
    // =========================================================================
    if (hasOldPrimaryContent && rows < oldRows && !isAltScreen_) {
        int linesToPush = oldRows - rows;
        yinfo("GPUScreen::resize: pushing {} lines to scrollback (rows {} -> {})",
              linesToPush, oldRows, rows);

        // Temporarily point visibleBuffer_ to primary buffer for pushLineToScrollback
        visibleBuffer_ = &primaryBuffer_;

        // Push top lines to scrollback (they will scroll off the top)
        // We push from top because content shifts up when screen shrinks
        for (int i = 0; i < linesToPush; i++) {
            pushLineToScrollback(i);
        }
    }

    // Save old buffers for both screens
    auto oldPrimaryBuffer = std::move(primaryBuffer_);
    auto oldAltBuffer = std::move(altBuffer_);

    rows_ = rows;
    cols_ = cols;

    size_t numCells = static_cast<size_t>(rows * cols);

    // Allocate new buffers
    primaryBuffer_.clear();
    primaryBuffer_.resize(numCells);

    altBuffer_.clear();
    altBuffer_.resize(numCells);

    viewBuffer_.clear();
    viewBuffer_.resize(numCells);

    // Pre-allocate scratch buffer for onMoveRect (one full row)
    scratchBuffer_.resize(cols);

    // Re-establish pointer to current screen
    if (isAltScreen_) {
        visibleBuffer_ = &altBuffer_;
    } else {
        visibleBuffer_ = &primaryBuffer_;
    }

    // Initialize both screens with default background
    clearBuffer(primaryBuffer_);
    clearBuffer(altBuffer_);

    // Copy old content for primary screen
    // When rows decreased, we pushed top lines to scrollback, so skip them
    if (hasOldPrimaryContent && oldRows > 0 && oldCols > 0) {
        int linesToSkip = (rows < oldRows && !isAltScreen_) ? (oldRows - rows) : 0;
        int copyRows = std::min(oldRows - linesToSkip, rows);
        int copyCols = std::min(oldCols, cols);

        yinfo("GPUScreen::resize: copying primary content, skip={} copyRows={} copyCols={}",
              linesToSkip, copyRows, copyCols);

        for (int row = 0; row < copyRows; row++) {
            int srcRow = row + linesToSkip;
            for (int col = 0; col < copyCols; col++) {
                size_t oldIdx = static_cast<size_t>(srcRow * oldCols + col);
                size_t newIdx = cellIndex(row, col);

                if (oldIdx < oldPrimaryBuffer.size()) {
                    primaryBuffer_[newIdx] = oldPrimaryBuffer[oldIdx];
                }
            }
        }
    }

    // Copy old content for alternate screen (as much as fits)
    if (hasOldAltContent && oldRows > 0 && oldCols > 0) {
        int copyRows = std::min(oldRows, rows);
        int copyCols = std::min(oldCols, cols);

        for (int row = 0; row < copyRows; row++) {
            for (int col = 0; col < copyCols; col++) {
                size_t oldIdx = static_cast<size_t>(row * oldCols + col);
                size_t newIdx = cellIndex(row, col);

                if (oldIdx < oldAltBuffer.size()) {
                    altBuffer_[newIdx] = oldAltBuffer[oldIdx];
                }
            }
        }
    }

    // Adjust cursor position when rows decreased
    // Lines were pushed to scrollback, so cursor moves up by that amount
    if (rows < oldRows && !isAltScreen_) {
        int linesToSkip = oldRows - rows;
        cursorRow_ -= linesToSkip;
        yinfo("GPUScreen::resize: cursor adjusted by -{}, now at row {}", linesToSkip, cursorRow_);
    }

    // Clamp cursor to new dimensions
    if (cursorRow_ >= rows) cursorRow_ = rows - 1;
    if (cursorCol_ >= cols) cursorCol_ = cols - 1;
    if (cursorRow_ < 0) cursorRow_ = 0;
    if (cursorCol_ < 0) cursorCol_ = 0;

    // Reset scroll offset on resize (back to live view) - only for primary screen
    if (!isAltScreen_) {
        scrollOffset_ = 0;
    }

    hasDamage_ = true;
    viewBufferDirty_ = true;
}

void GPUScreen::clearBuffer(std::vector<Cell>& buffer) {
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    colorToRGB(defaultFg_, fgR, fgG, fgB);
    colorToRGB(defaultBg_, bgR, bgG, bgB);

    Cell defaultCell;
    defaultCell.glyph = cachedSpaceGlyph_;
    defaultCell.fgR = fgR;
    defaultCell.fgG = fgG;
    defaultCell.fgB = fgB;
    defaultCell.bgR = bgR;
    defaultCell.bgG = bgG;
    defaultCell.bgB = bgB;
    defaultCell.alpha = 255;
    defaultCell.style = 0;

    std::fill(buffer.begin(), buffer.end(), defaultCell);
}

void GPUScreen::switchToScreen(bool alt) {
    if (isAltScreen_ == alt) return;  // Already on the requested screen

    yinfo("GPUScreen::switchToScreen: {} -> {}", isAltScreen_ ? "alt" : "primary", alt ? "alt" : "primary");

    isAltScreen_ = alt;

    if (alt) {
        // Switch to alternate screen
        visibleBuffer_ = &altBuffer_;

        // Clear alternate screen on entry (traditional terminal behavior)
        clearBuffer(altBuffer_);

        // Alternate screen doesn't use scrollback
        scrollOffset_ = 0;
    } else {
        // Switch back to primary screen
        visibleBuffer_ = &primaryBuffer_;

        // Primary screen content is preserved (was just hidden)
    }

    hasDamage_ = true;
    viewBufferDirty_ = true;
}

void GPUScreen::reset() {
    // Clear current screen with space and default colors
    if (visibleBuffer_) {
        clearBuffer(*visibleBuffer_);
    }

    cursorRow_ = 0;
    cursorCol_ = 0;
    cursorVisible_ = true;
    hasDamage_ = true;
    viewBufferDirty_ = true;
    if (!isAltScreen_) {
        scrollOffset_ = 0;
    }
}

//=============================================================================
// Buffer access - returns Cell buffer directly for GPU upload (zero-copy)
//=============================================================================

const Cell* GPUScreen::getCellData() const {
    if (scrollOffset_ > 0) {
        const_cast<GPUScreen*>(this)->composeViewBuffer();
        return viewBuffer_.data();
    }
    if (visibleBuffer_) {
        return visibleBuffer_->data();
    }
    return nullptr;
}

//=============================================================================
// Scrollback control
//=============================================================================

void GPUScreen::scrollUp(int lines) {
    int maxOffset = static_cast<int>(scrollback_.size());
    int newOffset = std::min(scrollOffset_ + lines, maxOffset);
    if (newOffset != scrollOffset_) {
        scrollOffset_ = newOffset;
        viewBufferDirty_ = true;
            hasDamage_ = true;
    }
}

void GPUScreen::scrollDown(int lines) {
    int newOffset = std::max(scrollOffset_ - lines, 0);
    if (newOffset != scrollOffset_) {
        scrollOffset_ = newOffset;
        viewBufferDirty_ = true;
            hasDamage_ = true;
    }
}

void GPUScreen::scrollToTop() {
    int maxOffset = static_cast<int>(scrollback_.size());
    if (scrollOffset_ != maxOffset) {
        scrollOffset_ = maxOffset;
        viewBufferDirty_ = true;
            hasDamage_ = true;
    }
}

void GPUScreen::scrollToBottom() {
    if (scrollOffset_ != 0) {
        scrollOffset_ = 0;
        viewBufferDirty_ = true;
            hasDamage_ = true;
    }
}

//=============================================================================
// View buffer composition (when scrolled back)
//=============================================================================

void GPUScreen::composeViewBuffer() {
    if (!viewBufferDirty_) return;

    // scrollOffset_ lines come from scrollback, rest from visible
    // scrollback_[size-1] is newest, scrollback_[0] is oldest
    // When scrollOffset_ = N, we show scrollback lines [size-N, size-1] at top
    // plus visible lines [0, rows-N-1] shifted down

    int sbSize = static_cast<int>(scrollback_.size());
    int sbLinesToShow = std::min(scrollOffset_, rows_);
    int visibleLinesToShow = rows_ - sbLinesToShow;

    // Fill top rows from scrollback (newest first in view)
    for (int viewRow = 0; viewRow < sbLinesToShow; viewRow++) {
        // scrollback index: newest lines are at end
        // viewRow 0 should get scrollback_[sbSize - scrollOffset_]
        int sbIndex = sbSize - scrollOffset_ + viewRow;
        if (sbIndex >= 0 && sbIndex < sbSize) {
            decompressLine(scrollback_[sbIndex], viewRow);
        }
    }

    // Fill bottom rows from visible buffer - single memcpy per row with Cell struct
    if (visibleBuffer_) {
        for (int viewRow = sbLinesToShow; viewRow < rows_; viewRow++) {
            int visRow = viewRow - sbLinesToShow;  // Source row in visible buffer
            size_t numCells = static_cast<size_t>(cols_);
            size_t srcOffset = static_cast<size_t>(visRow * cols_);
            size_t dstOffset = static_cast<size_t>(viewRow * cols_);

            // Single memcpy for entire row of cells
            std::memcpy(&viewBuffer_[dstOffset], &(*visibleBuffer_)[srcOffset], numCells * sizeof(Cell));
        }
    }

    viewBufferDirty_ = false;
}

void GPUScreen::decompressLine(const ScrollbackLineGPU& line, int viewRow) {
    size_t dstOffset = static_cast<size_t>(viewRow * cols_);

    // Copy cells directly from scrollback
    int lineCols = std::min(static_cast<int>(line.cells.size()), cols_);
    for (int col = 0; col < lineCols; col++) {
        viewBuffer_[dstOffset + col] = line.cells[col];
    }

    // Fill remainder with default cells
    uint8_t fgR = defaultFg_.rgb.red, fgG = defaultFg_.rgb.green, fgB = defaultFg_.rgb.blue;
    uint8_t bgR = defaultBg_.rgb.red, bgG = defaultBg_.rgb.green, bgB = defaultBg_.rgb.blue;

    Cell defaultCell;
    defaultCell.glyph = cachedSpaceGlyph_;
    defaultCell.fgR = fgR;
    defaultCell.fgG = fgG;
    defaultCell.fgB = fgB;
    defaultCell.bgR = bgR;
    defaultCell.bgG = bgG;
    defaultCell.bgB = bgB;
    defaultCell.alpha = 255;
    defaultCell.style = 0;

    for (int col = lineCols; col < cols_; col++) {
        viewBuffer_[dstOffset + col] = defaultCell;
    }
}

//=============================================================================
// Scrollback push (compress line and store)
//=============================================================================

void GPUScreen::pushLineToScrollback(int row) {
    if (!visibleBuffer_) return;  // Safety check

    ScrollbackLineGPU line;
    line.cells.resize(cols_);

    // Copy cell data directly
    size_t srcOffset = static_cast<size_t>(row * cols_);
    std::memcpy(line.cells.data(), &(*visibleBuffer_)[srcOffset], cols_ * sizeof(Cell));

    // Scan this row for widget markers - if found, track in scrolledOutWidgets_
    yinfo("GPUScreen::pushLineToScrollback: scanning row {} for markers", row);
    for (int col = 0; col < cols_; col++) {
        const Cell& cell = (*visibleBuffer_)[srcOffset + col];
        if (cell.glyph == GLYPH_PLUGIN) {
            yinfo("GPUScreen::pushLineToScrollback: found GLYPH_PLUGIN at row={} col={}, checking validation", row, col);
            // Validate marker pattern: fgB=0xFF, bgR=0xAA, bgG=0xAA
            if (cell.fgB == 0xFF && cell.bgR == 0xAA && cell.bgG == 0xAA) {
                uint16_t widgetId = static_cast<uint16_t>(cell.fgR) |
                                   (static_cast<uint16_t>(cell.fgG) << 8);
                yinfo("GPUScreen::pushLineToScrollback: VALID marker! widget {} at row={} col={} -> scrolledOutWidgets_ with y=-1",
                       widgetId, row, col);
                // Add with Y = -1 (just scrolled out)
                scrolledOutWidgets_.push_back({widgetId, -1, col});
            } else {
                yinfo("GPUScreen::pushLineToScrollback: marker validation FAILED fgB={} bgR={} bgG={}",
                      cell.fgB, cell.bgR, cell.bgG);
            }
        }
    }

    scrollback_.push_back(std::move(line));

    // If scrolled back, increment scroll offset to maintain view position
    // (otherwise the view would shift as new lines push into scrollback)
    if (scrollOffset_ > 0) {
        scrollOffset_++;
    }

    // Decrement Y for all scrolled-out widgets (they move up with each scroll)
    int maxNegativeY = -static_cast<int>(maxScrollback_);
    if (!scrolledOutWidgets_.empty()) {
        yinfo("GPUScreen::pushLineToScrollback: decrementing Y for {} scrolled-out widgets", scrolledOutWidgets_.size());
    }
    auto it = scrolledOutWidgets_.begin();
    while (it != scrolledOutWidgets_.end()) {
        int oldY = it->y;
        it->y--;
        yinfo("GPUScreen::pushLineToScrollback: widget {} y: {} -> {}", it->widgetId, oldY, it->y);
        if (it->y < maxNegativeY) {
            // This widget's marker has been removed from scrollback
            yinfo("GPUScreen: widget {} y={} below threshold {}, disposing", it->widgetId, it->y, maxNegativeY);
            if (widgetDisposalCallback_) {
                widgetDisposalCallback_(it->widgetId);
            }
            it = scrolledOutWidgets_.erase(it);
        } else {
            ++it;
        }
    }

    // Limit scrollback size
    while (scrollback_.size() > maxScrollback_) {
        scrollback_.pop_front();
        // Adjust scroll offset if we removed lines from the beginning
        if (scrollOffset_ > static_cast<int>(scrollback_.size())) {
            scrollOffset_ = static_cast<int>(scrollback_.size());
        }
    }

    // Notify callback (for widget position updates - but we already updated Y above)
    if (scrollCallback_) {
        scrollCallback_(1);
    }
}

//=============================================================================
// Cell manipulation (writes to visible buffer)
//=============================================================================

void GPUScreen::setCell(int row, int col, uint32_t glyph,
                        uint8_t fgR, uint8_t fgG, uint8_t fgB,
                        uint8_t bgR, uint8_t bgG, uint8_t bgB,
                        uint8_t attrsByte) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return;
    if (!visibleBuffer_) return;  // Safety check

    size_t idx = cellIndex(row, col);

    // Bounds check on buffer
    if (idx >= visibleBuffer_->size()) return;

    Cell& cell = (*visibleBuffer_)[idx];
    cell.glyph = glyph;
    cell.fgR = fgR;
    cell.fgG = fgG;
    cell.fgB = fgB;
    cell.bgR = bgR;
    cell.bgG = bgG;
    cell.bgB = bgB;
    cell.alpha = 255;
    cell.style = attrsByte;
}

void GPUScreen::clearCell(int row, int col) {
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    colorToRGB(pen_.fg, fgR, fgG, fgB);
    colorToRGB(pen_.bg, bgR, bgG, bgB);

    if (pen_.reverse) {
        std::swap(fgR, bgR);
        std::swap(fgG, bgG);
        std::swap(fgB, bgB);
    }

    setCell(row, col, cachedSpaceGlyph_, fgR, fgG, fgB, bgR, bgG, bgB, 0);
}

void GPUScreen::colorToRGB(const VTermColor& color, uint8_t& r, uint8_t& g, uint8_t& b) {
    if (VTERM_COLOR_IS_DEFAULT_FG(&color)) {
        r = defaultFg_.rgb.red;
        g = defaultFg_.rgb.green;
        b = defaultFg_.rgb.blue;
    } else if (VTERM_COLOR_IS_DEFAULT_BG(&color)) {
        r = defaultBg_.rgb.red;
        g = defaultBg_.rgb.green;
        b = defaultBg_.rgb.blue;
    } else if (VTERM_COLOR_IS_INDEXED(&color)) {
        VTermColor rgb = color;
        if (state_) {
            vterm_state_convert_color_to_rgb(state_, &rgb);
        }
        r = rgb.rgb.red;
        g = rgb.rgb.green;
        b = rgb.rgb.blue;
    } else {
        r = color.rgb.red;
        g = color.rgb.green;
        b = color.rgb.blue;
    }
}

//=============================================================================
// State callbacks
//=============================================================================

int GPUScreen::onPutglyph(VTermGlyphInfo* info, VTermPos pos, void* user) {
    auto* self = static_cast<GPUScreen*>(user);

    uint32_t cp = info->chars[0];
    if (cp == 0) cp = ' ';

    // Get glyph index - shader glyphs (Plane 16 PUA) pass through directly
    uint32_t glyphIdx;
    if (isShaderGlyph(cp)) {
        glyphIdx = cp;  // Shader glyph codepoint IS the glyph index
    } else {
        glyphIdx = self->font_
            ? self->font_->getGlyphIndex(cp, self->pen_.bold, self->pen_.italic)
            : cp;
    }

    // Get colors from current pen
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    self->colorToRGB(self->pen_.fg, fgR, fgG, fgB);
    self->colorToRGB(self->pen_.bg, bgR, bgG, bgB);

    if (self->pen_.reverse) {
        std::swap(fgR, bgR);
        std::swap(fgG, bgG);
        std::swap(fgB, bgB);
    }

    // Pack attributes
    uint8_t attrsByte = 0;
    if (self->pen_.bold) attrsByte |= 0x01;
    if (self->pen_.italic) attrsByte |= 0x02;
    attrsByte |= (self->pen_.underline & 0x03) << 2;
    if (self->pen_.strike) attrsByte |= 0x10;

    self->setCell(pos.row, pos.col, glyphIdx, fgR, fgG, fgB, bgR, bgG, bgB, attrsByte);

    // Handle wide characters (width > 1)
    for (int i = 1; i < info->width; i++) {
        self->setCell(pos.row, pos.col + i, GLYPH_WIDE_CONT,
                      fgR, fgG, fgB, bgR, bgG, bgB, attrsByte);
    }

    self->hasDamage_ = true;
    return 1;
}

int GPUScreen::onMoveCursor(VTermPos pos, VTermPos oldpos, int visible, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    ydebug("GPUScreen::onMoveCursor: ({},{}) -> ({},{})", oldpos.row, oldpos.col, pos.row, pos.col);
    self->cursorRow_ = pos.row;
    self->cursorCol_ = pos.col;
    self->cursorVisible_ = visible != 0;
    self->hasDamage_ = true;
    return 1;
}

int GPUScreen::onScrollRect(VTermRect rect, int downward, int rightward, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    (void)rightward;

    bool isFullWidth = (rect.start_col == 0 && rect.end_col == self->cols_);

    // Save lines to scrollback BEFORE vterm moves content (via moverect/erase)
    // downward > 0: content moves down, top rows pushed to scrollback
    // downward < 0: content moves up, bottom rows cleared (less common)
    if (downward > 0 && rect.start_row == 0 && isFullWidth) {
        for (int i = 0; i < downward && i < rect.end_row; i++) {
            self->pushLineToScrollback(i);
        }
    } else if (downward < 0) {
        // Scroll up: top lines will be overwritten
        int upAmount = -downward;

        if (rect.start_row == 0 && isFullWidth) {
            // Full-screen scroll from top - push to scrollback
            for (int i = 0; i < upAmount && i < rect.end_row; i++) {
                self->pushLineToScrollback(i);
            }
        } else if (isFullWidth) {
            // Scroll within region (not from row 0) - still notify for widget position updates
            if (self->scrollCallback_) {
                self->scrollCallback_(upAmount);
            }
        }
    }

    // Return 0 to let vterm handle the actual scroll via moverect/erase callbacks
    return 0;
}

int GPUScreen::onMoveRect(VTermRect dest, VTermRect src, void* user) {
    auto* self = static_cast<GPUScreen*>(user);

    int height = src.end_row - src.start_row;
    int width = src.end_col - src.start_col;
    int cols = self->cols_;
    int rows = self->rows_;

    // Bounds checking to prevent memory corruption
    if (height <= 0 || width <= 0) return 1;
    if (src.start_row < 0 || src.end_row > rows) return 1;
    if (dest.start_row < 0 || dest.start_row + height > rows) return 1;
    if (src.start_col < 0 || src.end_col > cols) return 1;
    if (dest.start_col < 0 || dest.start_col + width > cols) return 1;

    if (!self->visibleBuffer_) return 1;  // Safety check

    // Ultra-fast path: full-width move starting at column 0
    // Memory is fully contiguous - single memmove for entire region of Cells
    if (src.start_col == 0 && dest.start_col == 0 && width == cols) {
        size_t srcIdx = self->cellIndex(src.start_row, 0);
        size_t dstIdx = self->cellIndex(dest.start_row, 0);
        size_t totalCells = static_cast<size_t>(height) * cols;

        // Single memmove for all cell data
        std::memmove(&(*self->visibleBuffer_)[dstIdx],
                    &(*self->visibleBuffer_)[srcIdx],
                    totalCells * sizeof(Cell));
    }
    // Fast path: full-width but not starting at column 0 (scroll regions)
    // Still contiguous per row, do row-by-row with single memmove per row
    else if (src.start_col == dest.start_col && width == cols) {
        if (dest.start_row < src.start_row) {
            for (int row = 0; row < height; row++) {
                size_t srcIdx = self->cellIndex(src.start_row + row, 0);
                size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

                std::memmove(&(*self->visibleBuffer_)[dstIdx],
                            &(*self->visibleBuffer_)[srcIdx],
                            width * sizeof(Cell));
            }
        } else {
            for (int row = height - 1; row >= 0; row--) {
                size_t srcIdx = self->cellIndex(src.start_row + row, 0);
                size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

                std::memmove(&(*self->visibleBuffer_)[dstIdx],
                            &(*self->visibleBuffer_)[srcIdx],
                            width * sizeof(Cell));
            }
        }
    } else {
        // General case: use pre-allocated scratch buffer, copy row-by-row
        // Ensure scratch buffer is large enough
        if (static_cast<int>(self->scratchBuffer_.size()) < width) {
            self->scratchBuffer_.resize(width);
        }

        // Determine copy direction
        bool copyForward = (dest.start_row < src.start_row) ||
                          (dest.start_row == src.start_row && dest.start_col <= src.start_col);

        if (copyForward) {
            for (int row = 0; row < height; row++) {
                size_t srcRowStart = self->cellIndex(src.start_row + row, src.start_col);
                size_t dstRowStart = self->cellIndex(dest.start_row + row, dest.start_col);

                // Copy to scratch, then to dest (single memcpy for all cell data)
                std::memcpy(self->scratchBuffer_.data(),
                           &(*self->visibleBuffer_)[srcRowStart],
                           width * sizeof(Cell));
                std::memcpy(&(*self->visibleBuffer_)[dstRowStart],
                           self->scratchBuffer_.data(),
                           width * sizeof(Cell));
            }
        } else {
            for (int row = height - 1; row >= 0; row--) {
                size_t srcRowStart = self->cellIndex(src.start_row + row, src.start_col);
                size_t dstRowStart = self->cellIndex(dest.start_row + row, dest.start_col);

                // Copy to scratch, then to dest (single memcpy for all cell data)
                std::memcpy(self->scratchBuffer_.data(),
                           &(*self->visibleBuffer_)[srcRowStart],
                           width * sizeof(Cell));
                std::memcpy(&(*self->visibleBuffer_)[dstRowStart],
                           self->scratchBuffer_.data(),
                           width * sizeof(Cell));
            }
        }
    }

    self->hasDamage_ = true;
    self->viewBufferDirty_ = true;
    return 1;
}

int GPUScreen::onErase(VTermRect rect, int, void* user) {
    auto* self = static_cast<GPUScreen*>(user);

    if (!self->visibleBuffer_) return 1;  // Safety check

    // Bounds checking to prevent memory corruption
    int startRow = std::max(0, rect.start_row);
    int endRow = std::min(self->rows_, rect.end_row);
    int startCol = std::max(0, rect.start_col);
    int endCol = std::min(self->cols_, rect.end_col);
    int width = endCol - startCol;
    int cols = self->cols_;

    if (width <= 0 || startRow >= endRow) return 1;

    // Pre-compute colors once (not per cell)
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    self->colorToRGB(self->pen_.fg, fgR, fgG, fgB);
    self->colorToRGB(self->pen_.bg, bgR, bgG, bgB);
    if (self->pen_.reverse) {
        std::swap(fgR, bgR);
        std::swap(fgG, bgG);
        std::swap(fgB, bgB);
    }

    // Build default cell
    Cell defaultCell;
    defaultCell.glyph = self->cachedSpaceGlyph_;
    defaultCell.fgR = fgR;
    defaultCell.fgG = fgG;
    defaultCell.fgB = fgB;
    defaultCell.bgR = bgR;
    defaultCell.bgG = bgG;
    defaultCell.bgB = bgB;
    defaultCell.alpha = 255;
    defaultCell.style = 0;

    // Fast path: full-width erase - use std::fill for entire row
    if (startCol == 0 && endCol == cols) {
        for (int row = startRow; row < endRow; row++) {
            size_t rowOffset = static_cast<size_t>(row * cols);
            std::fill(&(*self->visibleBuffer_)[rowOffset],
                     &(*self->visibleBuffer_)[rowOffset + cols], defaultCell);
        }
    } else {
        // Partial width
        for (int row = startRow; row < endRow; row++) {
            size_t rowOffset = static_cast<size_t>(row * cols);
            for (int col = startCol; col < endCol; col++) {
                (*self->visibleBuffer_)[rowOffset + col] = defaultCell;
            }
        }
    }

    self->hasDamage_ = true;
    if (self->scrollOffset_ > 0) {
        self->viewBufferDirty_ = true;
    }
    return 1;
}

int GPUScreen::onInitPen(void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    
    // Reset pen to defaults
    self->pen_.fg = self->defaultFg_;
    self->pen_.bg = self->defaultBg_;
    self->pen_.bold = false;
    self->pen_.italic = false;
    self->pen_.underline = 0;
    self->pen_.strike = false;
    self->pen_.reverse = false;
    self->pen_.blink = false;
    
    return 1;
}

int GPUScreen::onSetPenAttr(VTermAttr attr, VTermValue* val, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    
    switch (attr) {
        case VTERM_ATTR_BOLD:
            self->pen_.bold = val->boolean != 0;
            break;
        case VTERM_ATTR_ITALIC:
            self->pen_.italic = val->boolean != 0;
            break;
        case VTERM_ATTR_UNDERLINE:
            self->pen_.underline = static_cast<uint8_t>(val->number & 0x03);
            break;
        case VTERM_ATTR_STRIKE:
            self->pen_.strike = val->boolean != 0;
            break;
        case VTERM_ATTR_REVERSE:
            self->pen_.reverse = val->boolean != 0;
            break;
        case VTERM_ATTR_BLINK:
            self->pen_.blink = val->boolean != 0;
            break;
        case VTERM_ATTR_FOREGROUND:
            self->pen_.fg = val->color;
            break;
        case VTERM_ATTR_BACKGROUND:
            self->pen_.bg = val->color;
            break;
        default:
            break;
    }
    
    return 1;
}

int GPUScreen::onSetTermProp(VTermProp prop, VTermValue* val, void* user) {
    auto* self = static_cast<GPUScreen*>(user);

    if (prop == VTERM_PROP_CURSORVISIBLE) {
        self->cursorVisible_ = val->boolean != 0;
    } else if (prop == VTERM_PROP_ALTSCREEN) {
        // Switch between primary and alternate screen
        self->switchToScreen(val->boolean != 0);
    }

    if (self->termPropCallback_) {
        self->termPropCallback_(prop, val);
    }

    return 1;
}

int GPUScreen::onBell(void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    
    if (self->bellCallback_) {
        self->bellCallback_();
    }
    
    return 1;
}

int GPUScreen::onResize(int rows, int cols, VTermStateFields*, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    self->resize(rows, cols);
    return 1;
}

int GPUScreen::onSetLineInfo(int, const VTermLineInfo*, const VTermLineInfo*, void*) {
    // Line info (double-width, double-height) - not commonly used
    return 1;
}

//=============================================================================
// Widget marker methods
//=============================================================================

void GPUScreen::setWidgetMarker(int row, int col, uint16_t widgetId) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        ydebug("GPUScreen::setWidgetMarker: OUT OF BOUNDS row={} col={} (screen {}x{})",
               row, col, rows_, cols_);
        return;
    }
    if (!visibleBuffer_) return;  // Safety check

    size_t idx = cellIndex(row, col);
    if (idx >= visibleBuffer_->size()) return;

    ydebug("GPUScreen::setWidgetMarker: row={} col={} widgetId={} idx={}",
           row, col, widgetId, idx);

    Cell& cell = (*visibleBuffer_)[idx];

    // Set glyph to GLYPH_PLUGIN (0xFFFF) to mark as widget cell
    cell.glyph = GLYPH_PLUGIN;

    // Encode widget ID using fg RGB
    // Use: fgR=bits0-7, fgG=bits8-15, fgB=validation marker (0xFF)
    cell.fgR = static_cast<uint8_t>(widgetId & 0xFF);        // bits 0-7
    cell.fgG = static_cast<uint8_t>((widgetId >> 8) & 0xFF); // bits 8-15
    cell.fgB = 0xFF;  // Marker validation byte
    cell.alpha = 0xFF;  // Marker validation

    // Also set bg to distinguish from erased cells
    cell.bgR = 0xAA;  // Validation pattern
    cell.bgG = 0xAA;
    cell.bgB = 0xAA;
    cell.style = 0;

    hasDamage_ = true;
}

void GPUScreen::clearWidgetMarker(int row, int col) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return;
    if (!visibleBuffer_) return;  // Safety check

    size_t idx = cellIndex(row, col);
    if (idx >= visibleBuffer_->size()) return;

    // Only clear if it's actually a widget marker
    if ((*visibleBuffer_)[idx].glyph == GLYPH_PLUGIN) {
        clearCell(row, col);
    }
}

void GPUScreen::trackScrolledOutWidget(uint16_t widgetId, int col) {
    // Widget marker just scrolled from row 0 into scrollback
    // Start tracking with y = -1 (just above visible area)
    ydebug("GPUScreen::trackScrolledOutWidget: widget {} col={} y=-1", widgetId, col);
    scrolledOutWidgets_.push_back({widgetId, -1, col});
}

std::vector<WidgetPosition> GPUScreen::scanWidgetPositions() const {
    std::vector<WidgetPosition> positions;

    // When scrolled back, scan the composed view buffer (includes scrollback content)
    // Otherwise scan the visible buffer directly
    const std::vector<Cell>* buffer;

    if (scrollOffset_ > 0) {
        // Ensure view buffer is composed
        const_cast<GPUScreen*>(this)->composeViewBuffer();
        buffer = &viewBuffer_;
    } else if (visibleBuffer_) {
        buffer = visibleBuffer_;
    } else {
        return positions;  // No buffers available
    }

    int numCells = rows_ * cols_;

    // Helper lambda to validate and extract widget marker
    auto extractWidget = [&](int cellIdx) -> bool {
        const Cell& cell = (*buffer)[cellIdx];
        // Validate marker pattern: fgB=0xFF, bgR=0xAA, bgG=0xAA
        if (cell.fgB != 0xFF) return false;
        if (cell.bgR != 0xAA || cell.bgG != 0xAA) return false;

        int row = cellIdx / cols_;
        int col = cellIdx % cols_;
        uint16_t widgetId = static_cast<uint16_t>(cell.fgR) |
                           (static_cast<uint16_t>(cell.fgG) << 8);
        ydebug("GPUScreen::scanWidgetPositions: found widget {} at row={} col={}",
               widgetId, row, col);
        positions.push_back({widgetId, row, col});
        return true;
    };

    // Scan for widget markers
    for (int i = 0; i < numCells; i++) {
        if ((*buffer)[i].glyph == GLYPH_PLUGIN) {
            extractWidget(i);
        }
    }

    // Include widgets from helper struct (markers scrolled into scrollback)
    if (!scrolledOutWidgets_.empty()) {
        yinfo("GPUScreen::scanWidgetPositions: adding {} scrolled-out widgets", scrolledOutWidgets_.size());
    }
    for (const auto& sow : scrolledOutWidgets_) {
        yinfo("GPUScreen::scanWidgetPositions: scrolled-out widget {} at y={} col={}",
               sow.widgetId, sow.y, sow.col);
        positions.push_back({sow.widgetId, sow.y, sow.col});
    }

    yinfo("GPUScreen::scanWidgetPositions: returning {} total positions", positions.size());
    return positions;
}

} // namespace yetty
