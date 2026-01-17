#include "gpu-screen.h"
#include <yetty/font.h>
#include "grid.h"  // For GLYPH_WIDE_CONT constant
#include <algorithm>
#include <cstring>
#include <string>

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

    // Allocate buffers
    resize(rows, cols);
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
    if (rows == rows_ && cols == cols_ && !visibleGlyphs_.empty()) {
        return;  // No change
    }
    
    int oldRows = rows_;
    int oldCols = cols_;
    bool hasOldContent = !visibleGlyphs_.empty();
    
    // Save old buffers
    auto oldGlyphs = std::move(visibleGlyphs_);
    auto oldCodepoints = std::move(visibleCodepoints_);
    auto oldFgColors = std::move(visibleFgColors_);
    auto oldBgColors = std::move(visibleBgColors_);
    auto oldAttrs = std::move(visibleAttrs_);
    
    rows_ = rows;
    cols_ = cols;
    
    size_t numCells = static_cast<size_t>(rows * cols);
    
    // Allocate new buffers (clear first to ensure clean state)
    visibleGlyphs_.clear();
    visibleCodepoints_.clear();
    visibleFgColors_.clear();
    visibleBgColors_.clear();
    visibleAttrs_.clear();
    viewGlyphs_.clear();
    viewCodepoints_.clear();
    viewFgColors_.clear();
    viewBgColors_.clear();
    viewAttrs_.clear();
    
    visibleGlyphs_.resize(numCells);
    visibleCodepoints_.resize(numCells);
    visibleFgColors_.resize(numCells * 4);
    visibleBgColors_.resize(numCells * 4);
    visibleAttrs_.resize(numCells);
    
    viewGlyphs_.resize(numCells);
    viewCodepoints_.resize(numCells);
    viewFgColors_.resize(numCells * 4);
    viewBgColors_.resize(numCells * 4);
    viewAttrs_.resize(numCells);
    
    // Initialize with default background
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    colorToRGB(defaultFg_, fgR, fgG, fgB);
    colorToRGB(defaultBg_, bgR, bgG, bgB);
    uint16_t spaceGlyph = font_ ? font_->getGlyphIndex(' ') : ' ';
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            setCell(row, col, ' ', spaceGlyph, fgR, fgG, fgB, bgR, bgG, bgB, 0);
        }
    }
    
    // Copy old content (as much as fits) - only if there was old content
    if (hasOldContent && oldRows > 0 && oldCols > 0) {
        int copyRows = std::min(oldRows, rows);
        int copyCols = std::min(oldCols, cols);
        
        for (int row = 0; row < copyRows; row++) {
            for (int col = 0; col < copyCols; col++) {
                size_t oldIdx = static_cast<size_t>(row * oldCols + col);
                size_t newIdx = cellIndex(row, col);
                
                // Bounds check on old buffers
                if (oldIdx < oldGlyphs.size()) {
                    visibleGlyphs_[newIdx] = oldGlyphs[oldIdx];
                }
                if (oldIdx < oldCodepoints.size()) {
                    visibleCodepoints_[newIdx] = oldCodepoints[oldIdx];
                }
                if (oldIdx * 4 + 3 < oldFgColors.size()) {
                    std::memcpy(&visibleFgColors_[newIdx * 4], &oldFgColors[oldIdx * 4], 4);
                }
                if (oldIdx * 4 + 3 < oldBgColors.size()) {
                    std::memcpy(&visibleBgColors_[newIdx * 4], &oldBgColors[oldIdx * 4], 4);
                }
                if (oldIdx < oldAttrs.size()) {
                    visibleAttrs_[newIdx] = oldAttrs[oldIdx];
                }
            }
        }
    }
    
    // Clamp cursor to new dimensions
    if (cursorRow_ >= rows) cursorRow_ = rows - 1;
    if (cursorCol_ >= cols) cursorCol_ = cols - 1;
    if (cursorRow_ < 0) cursorRow_ = 0;
    if (cursorCol_ < 0) cursorCol_ = 0;
    
    // Reset scroll offset on resize (back to live view)
    scrollOffset_ = 0;
    
    hasDamage_ = true;
    viewBufferDirty_ = true;
}

void GPUScreen::reset() {
    // Clear all cells with space and default colors
    uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
    colorToRGB(defaultFg_, fgR, fgG, fgB);
    colorToRGB(defaultBg_, bgR, bgG, bgB);

    uint16_t spaceGlyph = font_ ? font_->getGlyphIndex(' ') : ' ';
    
    for (int row = 0; row < rows_; row++) {
        for (int col = 0; col < cols_; col++) {
            setCell(row, col, ' ', spaceGlyph, fgR, fgG, fgB, bgR, bgG, bgB, 0);
        }
    }
    
    cursorRow_ = 0;
    cursorCol_ = 0;
    cursorVisible_ = true;
    hasDamage_ = true;
    viewBufferDirty_ = true;
    scrollOffset_ = 0;
}

//=============================================================================
// Buffer access - returns view buffer (handles scrollback blending)
//=============================================================================

const uint16_t* GPUScreen::getGlyphData() const {
    if (scrollOffset_ == 0) {
        return visibleGlyphs_.data();
    }
    // When scrolled back, we need to compose the view
    const_cast<GPUScreen*>(this)->composeViewBuffer();
    return viewGlyphs_.data();
}

const uint8_t* GPUScreen::getFgColorData() const {
    if (scrollOffset_ == 0) {
        return visibleFgColors_.data();
    }
    const_cast<GPUScreen*>(this)->composeViewBuffer();
    return viewFgColors_.data();
}

const uint8_t* GPUScreen::getBgColorData() const {
    if (scrollOffset_ == 0) {
        return visibleBgColors_.data();
    }
    const_cast<GPUScreen*>(this)->composeViewBuffer();
    return viewBgColors_.data();
}

const uint8_t* GPUScreen::getAttrsData() const {
    if (scrollOffset_ == 0) {
        return visibleAttrs_.data();
    }
    const_cast<GPUScreen*>(this)->composeViewBuffer();
    return viewAttrs_.data();
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
    
    // Fill bottom rows from visible buffer
    for (int viewRow = sbLinesToShow; viewRow < rows_; viewRow++) {
        int visRow = viewRow - sbLinesToShow;  // Source row in visible buffer
        size_t numCells = static_cast<size_t>(cols_);
        size_t srcOffset = static_cast<size_t>(visRow * cols_);
        size_t dstOffset = static_cast<size_t>(viewRow * cols_);
        
        std::memcpy(&viewGlyphs_[dstOffset], &visibleGlyphs_[srcOffset], numCells * sizeof(uint16_t));
        std::memcpy(&viewCodepoints_[dstOffset], &visibleCodepoints_[srcOffset], numCells * sizeof(uint32_t));
        std::memcpy(&viewFgColors_[dstOffset * 4], &visibleFgColors_[srcOffset * 4], numCells * 4);
        std::memcpy(&viewBgColors_[dstOffset * 4], &visibleBgColors_[srcOffset * 4], numCells * 4);
        std::memcpy(&viewAttrs_[dstOffset], &visibleAttrs_[srcOffset], numCells);
    }
    
    viewBufferDirty_ = false;
}

void GPUScreen::decompressLine(const ScrollbackLineGPU& line, int viewRow) {
    size_t dstOffset = static_cast<size_t>(viewRow * cols_);
    
    // Copy glyphs and codepoints (chars)
    int lineCols = std::min(static_cast<int>(line.glyphs.size()), cols_);
    for (int col = 0; col < lineCols; col++) {
        viewGlyphs_[dstOffset + col] = line.glyphs[col];
        viewCodepoints_[dstOffset + col] = col < static_cast<int>(line.chars.size()) ? line.chars[col] : ' ';
    }
    // Fill remainder with spaces
    uint16_t spaceGlyph = font_ ? font_->getGlyphIndex(' ') : ' ';
    for (int col = lineCols; col < cols_; col++) {
        viewGlyphs_[dstOffset + col] = spaceGlyph;
        viewCodepoints_[dstOffset + col] = ' ';
    }
    
    // Decompress RLE styles
    int col = 0;
    for (const auto& run : line.styleRuns) {
        for (uint16_t i = 0; i < run.count && col < cols_; i++, col++) {
            size_t colorIdx = (dstOffset + col) * 4;
            viewFgColors_[colorIdx + 0] = run.style.fgR;
            viewFgColors_[colorIdx + 1] = run.style.fgG;
            viewFgColors_[colorIdx + 2] = run.style.fgB;
            viewFgColors_[colorIdx + 3] = 255;
            viewBgColors_[colorIdx + 0] = run.style.bgR;
            viewBgColors_[colorIdx + 1] = run.style.bgG;
            viewBgColors_[colorIdx + 2] = run.style.bgB;
            viewBgColors_[colorIdx + 3] = 255;
            viewAttrs_[dstOffset + col] = static_cast<uint8_t>(run.style.attrs);
        }
    }
    // Fill remainder with default colors
    uint8_t fgR = defaultFg_.rgb.red, fgG = defaultFg_.rgb.green, fgB = defaultFg_.rgb.blue;
    uint8_t bgR = defaultBg_.rgb.red, bgG = defaultBg_.rgb.green, bgB = defaultBg_.rgb.blue;
    for (; col < cols_; col++) {
        size_t colorIdx = (dstOffset + col) * 4;
        viewFgColors_[colorIdx + 0] = fgR;
        viewFgColors_[colorIdx + 1] = fgG;
        viewFgColors_[colorIdx + 2] = fgB;
        viewFgColors_[colorIdx + 3] = 255;
        viewBgColors_[colorIdx + 0] = bgR;
        viewBgColors_[colorIdx + 1] = bgG;
        viewBgColors_[colorIdx + 2] = bgB;
        viewBgColors_[colorIdx + 3] = 255;
        viewAttrs_[dstOffset + col] = 0;
    }
}

//=============================================================================
// Scrollback push (compress line and store)
//=============================================================================

void GPUScreen::pushLineToScrollback(int row) {
    ScrollbackLineGPU line;
    line.glyphs.resize(cols_);
    line.chars.resize(cols_);
    
    // Copy raw data
    size_t srcOffset = static_cast<size_t>(row * cols_);
    for (int col = 0; col < cols_; col++) {
        line.glyphs[col] = visibleGlyphs_[srcOffset + col];
        line.chars[col] = visibleCodepoints_[srcOffset + col];
    }
    
    // RLE compress styles
    ScrollbackStyle lastStyle;
    size_t colorIdx = srcOffset * 4;
    lastStyle.fgR = visibleFgColors_[colorIdx + 0];
    lastStyle.fgG = visibleFgColors_[colorIdx + 1];
    lastStyle.fgB = visibleFgColors_[colorIdx + 2];
    lastStyle.bgR = visibleBgColors_[colorIdx + 0];
    lastStyle.bgG = visibleBgColors_[colorIdx + 1];
    lastStyle.bgB = visibleBgColors_[colorIdx + 2];
    lastStyle.attrs = visibleAttrs_[srcOffset];
    uint16_t runCount = 1;
    
    for (int col = 1; col < cols_; col++) {
        colorIdx = (srcOffset + col) * 4;
        ScrollbackStyle style;
        style.fgR = visibleFgColors_[colorIdx + 0];
        style.fgG = visibleFgColors_[colorIdx + 1];
        style.fgB = visibleFgColors_[colorIdx + 2];
        style.bgR = visibleBgColors_[colorIdx + 0];
        style.bgG = visibleBgColors_[colorIdx + 1];
        style.bgB = visibleBgColors_[colorIdx + 2];
        style.attrs = visibleAttrs_[srcOffset + col];
        
        if (style == lastStyle && runCount < 65535) {
            runCount++;
        } else {
            line.styleRuns.push_back({lastStyle, runCount});
            lastStyle = style;
            runCount = 1;
        }
    }
    line.styleRuns.push_back({lastStyle, runCount});
    
    scrollback_.push_back(std::move(line));
    
    // If scrolled back, increment scroll offset to maintain view position
    // (otherwise the view would shift as new lines push into scrollback)
    if (scrollOffset_ > 0) {
        scrollOffset_++;
    }
    
    // Limit scrollback size
    while (scrollback_.size() > maxScrollback_) {
        scrollback_.pop_front();
        // Adjust scroll offset if we removed lines from the beginning
        if (scrollOffset_ > static_cast<int>(scrollback_.size())) {
            scrollOffset_ = static_cast<int>(scrollback_.size());
        }
    }
    
    // Notify callback (for widget position updates)
    if (scrollCallback_) {
        scrollCallback_(1);
    }
}

//=============================================================================
// Cell manipulation (writes to visible buffer)
//=============================================================================

void GPUScreen::setCell(int row, int col, uint32_t codepoint, uint16_t glyph,
                        uint8_t fgR, uint8_t fgG, uint8_t fgB,
                        uint8_t bgR, uint8_t bgG, uint8_t bgB,
                        uint8_t attrsByte) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) return;
    
    size_t idx = cellIndex(row, col);
    
    // Bounds check on buffers
    if (idx >= visibleGlyphs_.size()) return;
    
    visibleGlyphs_[idx] = glyph;
    visibleCodepoints_[idx] = codepoint;
    
    size_t colorIdx = idx * 4;
    if (colorIdx + 3 >= visibleFgColors_.size()) return;
    
    visibleFgColors_[colorIdx + 0] = fgR;
    visibleFgColors_[colorIdx + 1] = fgG;
    visibleFgColors_[colorIdx + 2] = fgB;
    visibleFgColors_[colorIdx + 3] = 255;
    
    visibleBgColors_[colorIdx + 0] = bgR;
    visibleBgColors_[colorIdx + 1] = bgG;
    visibleBgColors_[colorIdx + 2] = bgB;
    visibleBgColors_[colorIdx + 3] = 255;
    
    visibleAttrs_[idx] = attrsByte;
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
    
    uint16_t spaceGlyph = font_ ? font_->getGlyphIndex(' ') : ' ';
    setCell(row, col, ' ', spaceGlyph, fgR, fgG, fgB, bgR, bgG, bgB, 0);
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
    
    // Get glyph index from font
    uint16_t glyphIdx = self->font_ 
        ? self->font_->getGlyphIndex(cp, self->pen_.bold, self->pen_.italic)
        : static_cast<uint16_t>(cp);
    
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
    // Bit 5 is emoji flag - check if codepoint is emoji
    // (simplified - real implementation should check Unicode emoji ranges)
    
    self->setCell(pos.row, pos.col, cp, glyphIdx, fgR, fgG, fgB, bgR, bgG, bgB, attrsByte);
    
    // Handle wide characters (width > 1)
    for (int i = 1; i < info->width; i++) {
        // Wide char continuation - store 0xFFFFFFFF as codepoint marker
        self->setCell(pos.row, pos.col + i, 0xFFFFFFFF, GLYPH_WIDE_CONT, 
                      fgR, fgG, fgB, bgR, bgG, bgB, attrsByte);
    }
    
    self->hasDamage_ = true;
    return 1;
}

int GPUScreen::onMoveCursor(VTermPos pos, VTermPos, int visible, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    self->cursorRow_ = pos.row;
    self->cursorCol_ = pos.col;
    self->cursorVisible_ = visible != 0;
    self->hasDamage_ = true;
    return 1;
}

int GPUScreen::onScrollRect(VTermRect rect, int downward, int rightward, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    (void)rightward;
    
    // Save lines to scrollback BEFORE vterm moves content (via moverect/erase)
    if (downward < 0) {
        // Scroll up: top lines will be overwritten
        int upAmount = -downward;
        if (rect.start_row == 0 && rect.start_col == 0 && rect.end_col == self->cols_) {
            for (int i = 0; i < upAmount && i < rect.end_row; i++) {
                self->pushLineToScrollback(i);
            }
        }
    }
    
    // Return 0 to let vterm handle the actual scroll via moverect/erase callbacks
    return 0;
}

int GPUScreen::onMoveRect(VTermRect dest, VTermRect src, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    
    // Copy rectangle from src to dest
    int height = src.end_row - src.start_row;
    int width = src.end_col - src.start_col;
    
    // Need to handle overlapping regions carefully
    // For simplicity, use temporary buffer
    std::vector<uint16_t> tmpGlyphs(height * width);
    std::vector<uint32_t> tmpCodepoints(height * width);
    std::vector<uint8_t> tmpFg(height * width * 4);
    std::vector<uint8_t> tmpBg(height * width * 4);
    std::vector<uint8_t> tmpAttrs(height * width);
    
    // Copy to temp
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            size_t srcIdx = self->cellIndex(src.start_row + row, src.start_col + col);
            size_t tmpIdx = row * width + col;
            
            tmpGlyphs[tmpIdx] = self->visibleGlyphs_[srcIdx];
            tmpCodepoints[tmpIdx] = self->visibleCodepoints_[srcIdx];
            std::memcpy(&tmpFg[tmpIdx * 4], &self->visibleFgColors_[srcIdx * 4], 4);
            std::memcpy(&tmpBg[tmpIdx * 4], &self->visibleBgColors_[srcIdx * 4], 4);
            tmpAttrs[tmpIdx] = self->visibleAttrs_[srcIdx];
        }
    }
    
    // Copy from temp to dest
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            size_t dstIdx = self->cellIndex(dest.start_row + row, dest.start_col + col);
            size_t tmpIdx = row * width + col;
            
            self->visibleGlyphs_[dstIdx] = tmpGlyphs[tmpIdx];
            self->visibleCodepoints_[dstIdx] = tmpCodepoints[tmpIdx];
            std::memcpy(&self->visibleFgColors_[dstIdx * 4], &tmpFg[tmpIdx * 4], 4);
            std::memcpy(&self->visibleBgColors_[dstIdx * 4], &tmpBg[tmpIdx * 4], 4);
            self->visibleAttrs_[dstIdx] = tmpAttrs[tmpIdx];
        }
    }
    
    self->hasDamage_ = true;
    self->viewBufferDirty_ = true;
    return 1;
}

int GPUScreen::onErase(VTermRect rect, int, void* user) {
    auto* self = static_cast<GPUScreen*>(user);
    
    for (int row = rect.start_row; row < rect.end_row; row++) {
        for (int col = rect.start_col; col < rect.end_col; col++) {
            self->clearCell(row, col);
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

std::string GPUScreen::getText(int startRow, int startCol, int endRow, int endCol) const {
    std::string result;
    
    // Clamp to valid range
    startRow = std::max(0, startRow);
    startCol = std::max(0, startCol);
    endRow = std::min(endRow, rows_ - 1);
    endCol = std::min(endCol, cols_ - 1);
    
    // For now, only get text from visible buffer (not scrollback)
    // TODO: support getText across scrollback
    
    for (int row = startRow; row <= endRow; row++) {
        int sc = (row == startRow) ? startCol : 0;
        int ec = (row == endRow) ? endCol : cols_ - 1;
        
        for (int col = sc; col <= ec; col++) {
            uint32_t cp = visibleCodepoints_[cellIndex(row, col)];
            
            // Skip wide char continuations
            if (cp == 0xFFFFFFFF) continue;
            
            // Convert codepoint to UTF-8
            if (cp == 0 || cp == ' ') {
                result += ' ';
            } else if (cp < 0x80) {
                result += static_cast<char>(cp);
            } else if (cp < 0x800) {
                result += static_cast<char>(0xC0 | (cp >> 6));
                result += static_cast<char>(0x80 | (cp & 0x3F));
            } else if (cp < 0x10000) {
                result += static_cast<char>(0xE0 | (cp >> 12));
                result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                result += static_cast<char>(0x80 | (cp & 0x3F));
            } else {
                result += static_cast<char>(0xF0 | (cp >> 18));
                result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
                result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
                result += static_cast<char>(0x80 | (cp & 0x3F));
            }
        }
        
        // Add newline between rows (except for last row)
        if (row < endRow) {
            result += '\n';
        }
    }
    
    return result;
}

} // namespace yetty
