#include "gpu-screen.h"
#include <yetty/gpu-allocator.h>
#include <yetty/card-manager.h>
#include <yetty/card-factory.h>
#include <yetty/card.h>
#include <algorithm>
#include <cstdlib>
#include <unordered_set>
#include <chrono>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <yetty/osc-command.h>
#include <yetty/result.hpp>
#include <yetty/shader-manager.h>
#include <yetty/wgpu-compat.h>
#include <yetty/font-manager.h>
#include <yetty/vector-sdf-font.h>
#include <yetty/vector-coverage-font.h>
#include <yetty/raster-font.h>
#include <ytrace/ytrace.hpp>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR "."
#endif

#if defined(__SSE2__)
#include <emmintrin.h>
#endif

#include <deque>
#include <unordered_map>
#include <vterm.h>

namespace yetty {

// CardPtr and Card come from <yetty/card.h> via card-factory.h

// =============================================================================
// Glyph constants
// =============================================================================
constexpr uint32_t GLYPH_WIDE_CONT = 0xFFFE; // Wide char continuation

// Font type encoding for cell attributes (bits 5-7 of style byte)
constexpr uint8_t FONT_TYPE_MSDF = 0;   // Default text rendering
constexpr uint8_t FONT_TYPE_BITMAP = 1; // Bitmap fonts (emoji, color fonts)
constexpr uint8_t FONT_TYPE_SHADER = 2; // Single-cell shader glyphs
constexpr uint8_t FONT_TYPE_CARD = 3;   // Multi-cell card glyphs
constexpr uint8_t FONT_TYPE_VECTOR = 4;   // Vector font (SDF curves)
constexpr uint8_t FONT_TYPE_COVERAGE = 5; // Vector font (coverage-based)
constexpr uint8_t FONT_TYPE_RASTER = 6;   // Raster font (texture atlas)

// Card glyph range - multi-cell widgets (U+100000 - U+100FFF)
constexpr uint32_t CARD_GLYPH_BASE = 0x100000;
constexpr uint32_t CARD_GLYPH_END = 0x100FFF;

// Shader glyph range - single-cell procedural glyphs (U+101000 - U+10FFFD)
constexpr uint32_t SHADER_GLYPH_BASE = 0x101000;
constexpr uint32_t SHADER_GLYPH_END = 0x10FFFD;

// Vector SDF font glyph range - Plane 15 PUA-A (U+F0000 - U+F0FFF)
constexpr uint32_t VECTOR_GLYPH_BASE = 0xF0000;
constexpr uint32_t VECTOR_GLYPH_END = 0xF0FFF;

// Vector Coverage font glyph range - Plane 15 PUA-A (U+F1000 - U+F1FFF)
constexpr uint32_t COVERAGE_GLYPH_BASE = 0xF1000;
constexpr uint32_t COVERAGE_GLYPH_END = 0xF1FFF;

// Raster font glyph range - Plane 15 PUA-A (U+F2000 - U+F2FFF)
constexpr uint32_t RASTER_GLYPH_BASE = 0xF2000;
constexpr uint32_t RASTER_GLYPH_END = 0xF2FFF;

// =============================================================================
// Copy mode key bindings (tmux vi-mode style)
// These will be configurable in the future
// =============================================================================

// Navigation
constexpr uint32_t KEY_COPY_LEFT = 'h';
constexpr uint32_t KEY_COPY_DOWN = 'j';
constexpr uint32_t KEY_COPY_UP = 'k';
constexpr uint32_t KEY_COPY_RIGHT = 'l';
constexpr uint32_t KEY_COPY_WORD_NEXT = 'w';
constexpr uint32_t KEY_COPY_WORD_PREV = 'b';
constexpr uint32_t KEY_COPY_WORD_END = 'e';
constexpr uint32_t KEY_COPY_WORD_NEXT_BIG = 'W';   // WORD next (whitespace delimited)
constexpr uint32_t KEY_COPY_WORD_PREV_BIG = 'B';   // WORD prev
constexpr uint32_t KEY_COPY_WORD_END_BIG = 'E';    // WORD end
constexpr uint32_t KEY_COPY_LINE_START = '0';
constexpr uint32_t KEY_COPY_LINE_END = '$';
constexpr uint32_t KEY_COPY_LINE_FIRST_NONBLANK = '^';
constexpr uint32_t KEY_COPY_TOP = 'g';        // gg = top of buffer
constexpr uint32_t KEY_COPY_BOTTOM = 'G';     // G = bottom of buffer
constexpr uint32_t KEY_COPY_HALFPAGE_UP = 'u';   // Ctrl-u
constexpr uint32_t KEY_COPY_HALFPAGE_DOWN = 'd'; // Ctrl-d
constexpr uint32_t KEY_COPY_PAGE_UP = 'b';       // Ctrl-b (page up)
constexpr uint32_t KEY_COPY_PAGE_DOWN = 'f';     // Ctrl-f (page down)
constexpr uint32_t KEY_COPY_SCREEN_TOP = 'H';    // H = top of screen
constexpr uint32_t KEY_COPY_SCREEN_MID = 'M';    // M = middle of screen
constexpr uint32_t KEY_COPY_SCREEN_BOT = 'L';    // L = bottom of screen
constexpr uint32_t KEY_COPY_PARA_PREV = '{';     // { = previous paragraph
constexpr uint32_t KEY_COPY_PARA_NEXT = '}';     // } = next paragraph
constexpr uint32_t KEY_COPY_FIND_CHAR = 'f';     // f = find char forward
constexpr uint32_t KEY_COPY_FIND_CHAR_BACK = 'F'; // F = find char backward
constexpr uint32_t KEY_COPY_TILL_CHAR = 't';     // t = till char forward
constexpr uint32_t KEY_COPY_TILL_CHAR_BACK = 'T'; // T = till char backward
constexpr uint32_t KEY_COPY_REPEAT_FIND = ';';   // ; = repeat last f/F/t/T
constexpr uint32_t KEY_COPY_REPEAT_FIND_REV = ','; // , = repeat last f/F/t/T reverse

// Search
constexpr uint32_t KEY_COPY_SEARCH_FWD = '/';
constexpr uint32_t KEY_COPY_SEARCH_BWD = '?';
constexpr uint32_t KEY_COPY_SEARCH_NEXT = 'n';
constexpr uint32_t KEY_COPY_SEARCH_PREV = 'N';

// Selection and copy
constexpr uint32_t KEY_COPY_START_SELECTION = 'v';  // v = start/toggle visual selection (like vim)
constexpr uint32_t KEY_COPY_VISUAL_LINE = 'V';      // V = visual line mode
constexpr uint32_t KEY_COPY_YANK = 'y';             // y = yank (copy) selection
constexpr uint32_t KEY_COPY_RECT_TOGGLE = 22;       // Ctrl-v = rectangle mode (like vim)

// Exit copy mode
constexpr uint32_t KEY_COPY_QUIT = 'q';
constexpr int KEY_COPY_ESCAPE = 256;  // GLFW_KEY_ESCAPE
constexpr int KEY_COPY_ENTER = 257;   // GLFW_KEY_ENTER (also yanks in tmux)

// GLFW cursor shapes (avoid including glfw3.h)
constexpr int CURSOR_DEFAULT = 0;             // Reset to default arrow
constexpr int CURSOR_CROSSHAIR = 0x00036003;  // GLFW_CROSSHAIR_CURSOR
constexpr int CURSOR_MOVE = 0x00036009;       // GLFW_RESIZE_ALL_CURSOR

// Helper functions for glyph type detection
inline bool isCardGlyph(uint32_t codepoint) {
  return codepoint >= CARD_GLYPH_BASE && codepoint <= CARD_GLYPH_END;
}

inline bool isShaderGlyph(uint32_t codepoint) {
  return codepoint >= SHADER_GLYPH_BASE && codepoint <= SHADER_GLYPH_END;
}

inline bool isVectorGlyph(uint32_t codepoint) {
  return codepoint >= VECTOR_GLYPH_BASE && codepoint <= VECTOR_GLYPH_END;
}

inline bool isCoverageGlyph(uint32_t codepoint) {
  return codepoint >= COVERAGE_GLYPH_BASE && codepoint <= COVERAGE_GLYPH_END;
}

inline bool isRasterGlyph(uint32_t codepoint) {
  return codepoint >= RASTER_GLYPH_BASE && codepoint <= RASTER_GLYPH_END;
}

// Check if codepoint is an emoji (common ranges)
inline bool isEmoji(uint32_t cp) {
  if (cp >= 0x1F600 && cp <= 0x1F64F)
    return true; // Emoticons
  if (cp >= 0x1F300 && cp <= 0x1F5FF)
    return true; // Misc Symbols/Pictographs
  if (cp >= 0x1F680 && cp <= 0x1F6FF)
    return true; // Transport/Map
  if (cp >= 0x1F900 && cp <= 0x1F9FF)
    return true; // Supplemental Symbols
  if (cp >= 0x1FA00 && cp <= 0x1FAFF)
    return true; // Symbols Extended
  if (cp >= 0x2600 && cp <= 0x26FF)
    return true; // Misc Symbols
  if (cp >= 0x2700 && cp <= 0x27BF)
    return true; // Dingbats
  if (cp >= 0x1F1E0 && cp <= 0x1F1FF)
    return true; // Regional Indicators
  return false;
}

// Scrollback line storage
struct ScrollbackLineGPU {
  std::vector<Cell> cells;
};


// Pen state for current text attributes
struct Pen {
  VTermColor fg;
  VTermColor bg;
  bool bold = false;
  bool italic = false;
  uint8_t underline = 0;
  bool strike = false;
  bool reverse = false;
  bool blink = false;
};

// OSC parsing is included via osc-command.h

class GPUScreenImpl : public GPUScreen {
public:
  explicit GPUScreenImpl(const YettyContext &ctx);
  ~GPUScreenImpl() override;

  Result<void> onShutdown() override;

  // Initialize fonts and cell metrics - called from factory after construction
  Result<void> init() noexcept;

  // GPUScreen interface
  void write(const char *data, size_t len) override;
  void resize(uint32_t cols, uint32_t rows) override;
  uint32_t getCols() const override;
  uint32_t getRows() const override;
  bool hasDamage() const override { return _hasDamage; }
  void clearDamage() override { _hasDamage = false; }
  void setOutputCallback(OutputCallback cb) override;
  void setResizeCallback(ResizeCallback cb) override;
  Result<void> render(WGPURenderPassEncoder pass) override;
  void setViewport(float x, float y, float width, float height) override;
  uint32_t getCellWidth() const override { return static_cast<uint32_t>(_baseCellWidth * _zoomLevel); }
  uint32_t getCellHeight() const override { return static_cast<uint32_t>(_baseCellHeight * _zoomLevel); }

  // EventListener interface
  Result<bool> onEvent(const base::Event &event) override;

  // GPUScreen interface - test methods
  const Cell *getCellData() const override;
  Cell getCell(int row, int col) const override;
  int getCursorCol() const { return _cursorCol; }
  int getCursorRow() const { return _cursorRow; }
  bool isCursorVisible() const { return _cursorVisible; }
  void scrollUp(int lines);
  void scrollDown(int lines);
  void scrollToBottom();
  bool isScrolledBack() const { return _scrollOffset > 0; }

  // Text search (tmux-style)
  bool search(const std::string& query);  // Search and scroll to first match
  bool searchNext();                       // Find next occurrence
  bool searchPrevious();                   // Find previous occurrence
  void clearSearch();                      // Clear search state
  bool hasSearchMatch() const { return _searchActive && _searchMatchRow >= 0; }

  void sendKey(uint32_t codepoint, int mods);
  void sendSpecialKey(int key, int mods);
  base::ObjectId id() const { return _id; }
  bool isFocused() const { return _focused; }
  void registerForFocus();

  // VTerm state callbacks (static)
  static int onPutglyph(VTermGlyphInfo *info, VTermPos pos, void *user);
  static int onMoveCursor(VTermPos pos, VTermPos oldpos, int visible,
                          void *user);
  static int onScrollRect(VTermRect rect, int downward, int rightward,
                          void *user);
  static int onMoveRect(VTermRect dest, VTermRect src, void *user);
  static int onErase(VTermRect rect, int selective, void *user);
  static int onInitPen(void *user);
  static int onSetPenAttr(VTermAttr attr, VTermValue *val, void *user);
  static int onSetTermProp(VTermProp prop, VTermValue *val, void *user);
  static int onBell(void *user);
  static int onResize(int rows, int cols, VTermStateFields *fields, void *user);
  static int onOSC(int command, VTermStringFragment frag, void *user);
  static int onSetLineInfo(int row, const VTermLineInfo *newinfo,
                           const VTermLineInfo *oldinfo, void *user);

private:
  Result<void> initPipeline();
  void createVTerm();
  void attach(VTerm *vt);
  void resizeInternal(int rows, int cols, VTermStateFields *fields);
  void resizeBuffer(int bufidx, int newRows, int newCols, bool active,
                    VTermStateFields *fields);
  int linePopcount(const std::vector<Cell> &buffer, int row, int cols) const;
  void clearBuffer(std::vector<Cell> &buffer);
  void switchToScreen(bool alt);
  void reset();
  void composeViewBuffer();
  void decompressLine(const ScrollbackLineGPU &line, int viewRow);
  void pushLineToScrollback(int row);
  void scrollToTop();
  void clampVisualZoomOffset();  // Clamp visual zoom pan offset to valid range

  void setCell(int row, int col, uint32_t glyph, uint8_t fgR, uint8_t fgG,
               uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB,
               uint8_t attrsByte);
  void clearCell(int row, int col);
  void colorToRGB(const VTermColor &color, uint8_t &r, uint8_t &g, uint8_t &b);

  size_t cellIndex(int row, int col) const {
    return static_cast<size_t>(row * _cols + col);
  }

  // Card dormancy — scan visible buffer for a given slotIndex
  bool isCardSlotVisibleInBuffer(uint32_t slotIndex, int excludeRow = -1) const;
  void suspendOffscreenCards(int scrolledRow);
  void suspendNonVisibleCards();
  void reactivateVisibleCards();
  void gcInactiveCards();

  // OSC handling
  bool handleOSCSequence(const std::string &sequence, std::string *response,
                         uint32_t *linesToAdvance);
  bool handleCardOSCSequence(const std::string &sequence, std::string *response,
                             uint32_t *linesToAdvance);
  void handleEffectOSC(int command, const std::string &payload);
  std::string buildGpuStatsText();

  // Text selection
  void clearSelection();
  std::string extractSelectedText() const;

  // Text search helpers
  std::string extractRowText(int totalRow) const;  // Extract text from scrollback or visible row
  bool searchInRow(int totalRow, int startCol, bool forward, int& matchCol) const;
  void scrollToRow(int totalRow);  // Scroll to make row visible
  void findAllMatches();  // Populate _searchMatches with all occurrences
  void applySearchHighlighting();  // Swap fg/bg for all match positions
  void clearSearchHighlighting();  // Restore original colors

  // Copy mode (tmux-style vi mode)
  void enterCopyMode();
  void exitCopyMode();
  void updateCopyModeStatus();
  bool handleCopyModeKey(uint32_t codepoint, int key, int mods);
  void copyModeMoveCursor(int dRow, int dCol);
  void copyModeStartSelection();
  void copyModeYank();
  void copyModeScrollHalfPage(bool up);
  void copyModeScrollFullPage(bool up);
  void copyModeGotoTop();
  void copyModeGotoBottom();
  void copyModeGotoLineStart();
  void copyModeGotoLineEnd();
  void copyModeWordNext();
  void copyModeWordPrev();
  void copyModeWordEnd();
  void copyModeGotoScreenTop();
  void copyModeGotoScreenMid();
  void copyModeGotoScreenBot();
  void copyModeParaNext();
  void copyModeParaPrev();
  void copyModeFindChar(char ch, bool forward, bool till);
  int getTotalRows() const;  // scrollback + visible rows

  // Status line (tmux-style bar at bottom)
  void updateStatusLine();
  void setStatusText(const std::string& text);
  void clearStatusLine();

  // Card registry
  void registerCard(CardPtr card);
  void unregisterCard(uint32_t metadataOffset);
  Card *getCardBySlotIndex(uint32_t offset) const;
  Card *getCardAtCell(int row, int col) const;

  // Compute card-local pixel coords from viewport-local position and cell row/col.
  // Uses bg encoding to find card's actual top-left corner in the grid.
  void cardLocalCoords(float viewLocalX, float viewLocalY, int row, int col,
                       float &cardX, float &cardY) const {
    uint32_t cellW = getCellWidth();
    uint32_t cellH = getCellHeight();
    // bg encodes relative position within card: (relRow << 12) | relCol
    size_t idx = cellIndex(row, col);
    const Cell &cell = (*_visibleBuffer)[idx];
    uint32_t bg24 = static_cast<uint32_t>(cell.bgR) |
                    (static_cast<uint32_t>(cell.bgG) << 8) |
                    (static_cast<uint32_t>(cell.bgB) << 16);
    int relCol = bg24 & 0xFFF;
    int relRow = (bg24 >> 12) & 0xFFF;
    int cardTopCol = col - relCol;
    int cardTopRow = row - relRow;
    cardX = viewLocalX - static_cast<float>(cardTopCol) * cellW;
    cardY = viewLocalY - static_cast<float>(cardTopRow) * cellH;
  }

  // VTerm
  VTerm *_vterm = nullptr;
  VTermState *state_ = nullptr;

  // Buffers
  std::vector<Cell> _primaryBuffer;
  std::vector<Cell> _altBuffer;
  std::vector<Cell> viewBuffer_;
  std::vector<Cell> scratchBuffer_;
  std::vector<Cell> *_visibleBuffer = nullptr;

  // Scrollback
  std::deque<ScrollbackLineGPU> _scrollback;
  size_t maxScrollback_;
  int _scrollOffset = 0;
  bool viewBufferDirty_ = false;

  // Screen state
  int _rows = 0;
  int _cols = 0;
  int _cursorRow = 0;
  int _cursorCol = 0;
  bool _cursorVisible = true;
  bool _cursorBlink = true;
  int _cursorShape = 1; // VTERM_PROP_CURSORSHAPE_BLOCK
  bool _isAltScreen = false;
  bool isAltScreenExternal_ = false;
  bool _hasDamage = false;
  bool _reflow = true; // Enable reflow on resize (like screen.c)

  // Pen state
  Pen _pen;
  VTermColor defaultFg_;
  VTermColor defaultBg_;

  // Fonts
  MsMsdfFont::Ptr _msdfFont;
  BmFont::Ptr _bitmapFont;
  ShaderFont::Ptr _shaderGlyphFont;
  ShaderFont::Ptr _cardFont;
  VectorSdfFont::Ptr _vectorFont;
  VectorCoverageFont::Ptr _coverageFont;
  RasterFont::Ptr _rasterFont;
  uint32_t _cachedSpaceGlyph = 0;

  // Callbacks
  OutputCallback _outputCallback;
  ResizeCallback _resizeCallback;
  std::function<void(VTermProp, VTermValue *)> termPropCallback_;
  std::function<void()> bellCallback_;
  std::function<void(const char *, size_t)> vtermInputCallback_;

  // Cards — active (visible, buffers allocated) and inactive (suspended)
  std::unordered_map<uint32_t, CardPtr> _cards;
  std::unordered_map<uint32_t, CardPtr> _inactiveCards;
  CardManager::Ptr _cardManager;
  base::ObjectId _cardMouseTarget = 0;  // card that received last CardMouseDown
  uint32_t _gcFrameCounter = 0;
  bool _bufferLayoutChanged = false;   // A buffer card entered or left
  bool _textureLayoutChanged = false;  // A texture card entered or left

  // Text selection state
  bool _selecting = false;
  int _selStartCol = 0, _selStartRow = 0;
  int _selEndCol = 0, _selEndRow = 0;
  bool _hasSelection = false;

  // Copy mode state (tmux-style)
  bool _copyMode = false;
  bool _commandMode = false;      // Waiting for command key after Ctrl+\ prefix
  int _copyCursorRow = 0;         // Cursor position in copy mode (view row)
  int _copyCursorCol = 0;
  bool _copySelecting = false;    // Selection in progress
  bool _copyRectMode = false;     // Rectangle selection mode
  bool _searchInputMode = false;  // Waiting for search input
  bool _searchForward = true;     // Search direction
  std::string _searchInputBuffer; // Buffer for search input
  int _gPending = 0;              // For 'gg' command (g pressed count)
  
  // f/F/t/T char find state
  bool _findCharMode = false;     // Waiting for char input for f/F/t/T
  char _findCharCmd = 0;          // Which command: 'f', 'F', 't', 'T'
  char _lastFindChar = 0;         // Last char used for f/F/t/T
  char _lastFindCmd = 0;          // Last f/F/t/T command for ; and ,

  // Text search state
  std::string _searchQuery;
  int _searchMatchRow = -1;    // Row of current match (-1 = no match)
  int _searchMatchCol = -1;    // Column of current match
  bool _searchActive = false;  // Whether a search is active
  // Store all match positions as (row, col) pairs for highlighting
  std::vector<std::pair<int, int>> _searchMatches;
  int _currentMatchIndex = -1; // Index into _searchMatches for current match

  // Status line (tmux-style bar at bottom, not managed by vterm)
  std::vector<Cell> _statusLine;
  std::string _statusText;
  bool _statusLineEnabled = true;

  // OSC parsing
  OscCommandParser _oscParser;
  std::string _oscBuffer;
  int _oscCommand = -1;

  // Rendering - GPU resources (shared resources come from ShaderManager)
  YettyContext _ctx;
  GpuAllocator::Ptr _allocator;
  // Per-instance resources only:
  WGPUBindGroup _bindGroup = nullptr;
  WGPUBuffer _uniformBuffer = nullptr;
  WGPUBuffer _cellBuffer = nullptr;
  size_t _cellBufferSize = 0;
  uint32_t _textureCols = 0;
  uint32_t _textureRows = 0;
  bool _pipelineInitialized = false;
  bool _needsBindGroupRecreation = false;
  uint32_t _lastFontResourceVersion = 0;

  // Grid uniforms (must match shader GridUniforms - group 1 binding 0)
  struct Uniforms {
    glm::mat4 projection;     // 64 bytes
    glm::vec2 screenSize;     // 8 bytes
    glm::vec2 cellSize;       // 8 bytes
    glm::vec2 gridSize;       // 8 bytes (cols, rows)
    float pixelRange;         // 4 bytes
    float scale;              // 4 bytes
    glm::vec2 cursorPos;      // 8 bytes (col, row)
    float cursorVisible;      // 4 bytes
    float cursorShape;        // 4 bytes (1=block, 2=underline, 3=bar)
    glm::vec2 viewportOrigin; // 8 bytes
    float cursorBlink;        // 4 bytes (0=no blink, 1=blink)
    float hasSelection;       // 4 bytes (0=no, 1=yes)
    glm::vec2 selStart;       // 8 bytes (col, row)
    glm::vec2 selEnd;         // 8 bytes (col, row)
    // Effects (per gpu-screen instance)
    uint32_t preEffectIndex = 0;   // 4 bytes (0 = no effect)
    uint32_t postEffectIndex = 0;  // 4 bytes (0 = no effect)
    float preEffectParams[6] = {}; // 24 bytes
    float postEffectParams[6] = {}; // 24 bytes
    uint32_t defaultFg = 0;         // 4 bytes - packed RGBA like cell.fg
    uint32_t defaultBg = 0;         // 4 bytes - packed RGB like cell.bg
    uint32_t spaceGlyph = 0;        // 4 bytes - glyph index for space character
    // Coord distortion effects
    uint32_t effectIndex = 0;       // 4 bytes (0 = no effect)
    float effectParams[6] = {};     // 24 bytes
    // Visual Zoom (shader-based, no terminal resize)
    float visualZoomScale = 1.0f;   // 4 bytes: 1.0 = off, >1.0 = zoomed
    float visualZoomOffsetX = 0.0f; // 4 bytes: pan X in pixels
    float visualZoomOffsetY = 0.0f; // 4 bytes: pan Y in pixels
    float _pad0 = 0.0f;             // 4 bytes: padding for 16-byte alignment
  }; // Total: 256 bytes (16-byte aligned)

  Uniforms _uniforms;

  // Cell size from font metrics
  float _baseCellWidth = 10.0f;  // from font advance
  float _baseCellHeight = 20.0f; // from font line height
  float _zoomLevel = 0.5f;       // adjustable with ctrl+wheel

  // Visual Zoom state (shader-based magnification)
  bool _visualZoomActive = false;
  float _visualZoomScale = 1.0f;     // 1.0 to 100.0
  float _visualZoomOffsetX = 0.0f;   // Pan in source pixels
  float _visualZoomOffsetY = 0.0f;
  bool _visualZoomDragging = false;   // Mouse drag-to-pan active
  float _visualZoomDragLastX = 0.0f;  // Last drag position (screen pixels)
  float _visualZoomDragLastY = 0.0f;

  // Viewport (pixel coordinates within window)
  float _viewportX = 0.0f;
  float _viewportY = 0.0f;
  float _viewportWidth = 0.0f;
  float _viewportHeight = 0.0f;

  // Focus management
  base::ObjectId _id = 0;
  bool _focused = false;
  static base::ObjectId _nextId;
};

// State callbacks struct
static VTermStateCallbacks stateCallbacks = {
    .putglyph = GPUScreenImpl::onPutglyph,
    .movecursor = GPUScreenImpl::onMoveCursor,
    .scrollrect = GPUScreenImpl::onScrollRect,
    .moverect = GPUScreenImpl::onMoveRect,
    .erase = GPUScreenImpl::onErase,
    .initpen = GPUScreenImpl::onInitPen,
    .setpenattr = GPUScreenImpl::onSetPenAttr,
    .settermprop = GPUScreenImpl::onSetTermProp,
    .bell = GPUScreenImpl::onBell,
    .resize = GPUScreenImpl::onResize,
    .setlineinfo = GPUScreenImpl::onSetLineInfo,
    .sb_clear = nullptr,
};

// State fallbacks for unrecognized sequences (OSC, etc.)
static VTermStateFallbacks stateFallbacks = {
    .control = nullptr,
    .csi = nullptr,
    .osc = GPUScreenImpl::onOSC,
    .dcs = nullptr,
    .apc = nullptr,
    .pm = nullptr,
    .sos = nullptr,
};

// Static ID counter for GPUScreen instances
base::ObjectId GPUScreenImpl::_nextId = 1;

GPUScreenImpl::GPUScreenImpl(const YettyContext &ctx)
    : maxScrollback_(10000), _ctx(ctx), _id(_nextId++) {
  // Initialize default colors
  vterm_color_rgb(&defaultFg_, 204, 204, 204); // Light gray
  vterm_color_rgb(&defaultBg_, 15, 15, 35);    // Dark blue-ish

  _pen.fg = defaultFg_;
  _pen.bg = defaultBg_;

  // Start on primary screen
  _isAltScreen = false;

  // Fonts, cell size, vterm and buffers initialized in init()
}

Result<void> GPUScreenImpl::init() noexcept {
  // Get fonts from context
  if (_ctx.fontManager) {
    _msdfFont = _ctx.fontManager->getDefaultMsMsdfFont();
    _bitmapFont = _ctx.fontManager->getDefaultBmFont();
    _shaderGlyphFont = _ctx.fontManager->getDefaultShaderGlyphFont();
    _cardFont = _ctx.fontManager->getDefaultCardFont();
    _vectorFont = _ctx.fontManager->getDefaultVectorSdfFont();
    _coverageFont = _ctx.fontManager->getDefaultVectorCoverageFont();
    _rasterFont = _ctx.fontManager->getDefaultRasterFont();
  }

  // Cache space glyph index to avoid repeated lookups in hot paths
  _cachedSpaceGlyph = _msdfFont ? _msdfFont->getGlyphIndex(' ') : 0;

  // Calculate base cell size from font metrics
  if (_msdfFont) {
    _baseCellHeight = _msdfFont->atlas()->getLineHeight();
    const auto &metadata = _msdfFont->atlas()->getGlyphMetadata();
    uint32_t mIndex = _msdfFont->getGlyphIndex('M');
    if (mIndex > 0 && mIndex < metadata.size()) {
      _baseCellWidth = metadata[mIndex]._advance;
    } else {
      _baseCellWidth = _baseCellHeight * 0.5f;
    }
  }

  // Update raster font with actual cell size
  if (_rasterFont) {
    _rasterFont->setCellSize(getCellWidth(), getCellHeight());
    _rasterFont->uploadToGpu();
  }

  // Create per-screen GpuAllocator for tracking per-terminal GPU resources
  if (_ctx.gpu.device) {
    _allocator = std::make_shared<GpuAllocator>(_ctx.gpu.device);
  }

  // Create per-screen CardManager (owns CardBufferManager, CardTextureManager, and bind group).
  // Starts with tiny placeholder buffers (~20KB for metadata pool + 4 bytes each
  // for buffer/atlas). Real buffers and atlas are allocated lazily
  // on first card creation, avoiding ~81MB GPU allocation when no cards are used.
  if (_ctx.gpu.device && _ctx.gpu.sharedUniformBuffer && _allocator) {
    auto cmResult = CardManager::create(&_ctx.gpu, _allocator, _ctx.gpu.sharedUniformBuffer, _ctx.gpu.sharedUniformSize);
    if (!cmResult) {
      ywarn("GPUScreen: failed to create CardManager: {}", error_msg(cmResult));
    } else {
      _cardManager = *cmResult;
      _ctx.cardManager = _cardManager;
      yinfo("GPUScreen[{}]: created per-screen CardManager (lazy)", _id);
    }
  }

  // Initialize status line with default text
  _statusText = "[0] yetty";

  return Ok();
}

GPUScreenImpl::~GPUScreenImpl() {
  if (_vterm) {
    vterm_free(_vterm);
    _vterm = nullptr;
  }
}

Result<void> GPUScreenImpl::onShutdown() {
  Result<void> result = Ok();

  // Deregister from all EventLoop events while shared_ptr is still alive
  auto loop = *base::EventLoop::instance();
  loop->deregisterListener(sharedAs<base::EventListener>());

  // Dispose active cards
  for (auto& [slotIndex, card] : _cards) {
    if (auto res = card->dispose(); !res) {
      result = Err<void>("Failed to dispose active card", res);
    }
  }
  _cards.clear();

  // Dispose inactive (suspended) cards
  for (auto& [slotIndex, card] : _inactiveCards) {
    if (auto res = card->dispose(); !res) {
      result = Err<void>("Failed to dispose inactive card", res);
    }
  }
  _inactiveCards.clear();

  // Release CardManager while shared_ptr is still alive
  _cardManager.reset();

  yinfo("GPUScreen[{}]: onShutdown complete", _id);
  return result;
}

void GPUScreenImpl::attach(VTerm *vt) {
  yinfo(">>> GPUScreenImpl::attach: ENTERED vt={}", (void*)vt);
  _vterm = vt;
  state_ = vterm_obtain_state(vt);
  yinfo(">>> GPUScreenImpl::attach: state_={}", (void*)state_);

  // Register our callbacks with State layer
  vterm_state_set_callbacks(state_, &stateCallbacks, this);
  yinfo(">>> GPUScreenImpl::attach: state callbacks registered");

  // Register fallbacks for unrecognized sequences (OSC, etc.)
  vterm_state_set_unrecognised_fallbacks(state_, &stateFallbacks, this);
  yinfo(">>> GPUScreenImpl::attach: OSC fallbacks registered (stateFallbacks.osc={})",
        (void*)stateFallbacks.osc);

  // Get default colors from vterm (white fg, black bg)
  vterm_state_get_default_colors(state_, &defaultFg_, &defaultBg_);

  yinfo("attach: defaultFg_ type={} idx={} rgb=({},{},{})",
        VTERM_COLOR_IS_DEFAULT_FG(&defaultFg_) ? "DEFAULT_FG" :
        VTERM_COLOR_IS_DEFAULT_BG(&defaultFg_) ? "DEFAULT_BG" :
        VTERM_COLOR_IS_INDEXED(&defaultFg_) ? "INDEXED" : "RGB",
        defaultFg_.indexed.idx,
        defaultFg_.rgb.red, defaultFg_.rgb.green, defaultFg_.rgb.blue);
  yinfo("attach: defaultBg_ type={} idx={} rgb=({},{},{})",
        VTERM_COLOR_IS_DEFAULT_BG(&defaultBg_) ? "DEFAULT_BG" :
        VTERM_COLOR_IS_DEFAULT_FG(&defaultBg_) ? "DEFAULT_FG" :
        VTERM_COLOR_IS_INDEXED(&defaultBg_) ? "INDEXED" : "RGB",
        defaultBg_.indexed.idx,
        defaultBg_.rgb.red, defaultBg_.rgb.green, defaultBg_.rgb.blue);

  // Convert to RGB if they're indexed
  if (VTERM_COLOR_IS_INDEXED(&defaultFg_)) {
    vterm_state_convert_color_to_rgb(state_, &defaultFg_);
    yinfo("attach: converted defaultFg_ to RGB: ({},{},{})",
          defaultFg_.rgb.red, defaultFg_.rgb.green, defaultFg_.rgb.blue);
  }
  if (VTERM_COLOR_IS_INDEXED(&defaultBg_)) {
    vterm_state_convert_color_to_rgb(state_, &defaultBg_);
    yinfo("attach: converted defaultBg_ to RGB: ({},{},{})",
          defaultBg_.rgb.red, defaultBg_.rgb.green, defaultBg_.rgb.blue);
  }

  _pen.fg = defaultFg_;
  _pen.bg = defaultBg_;

  // Reset state (triggers initpen, clears screen)
  vterm_state_reset(state_, 1);
}

void GPUScreenImpl::createVTerm() {
  if (_vterm)
    return; // Already have vterm

  _vterm = vterm_new(_rows, _cols);
  if (!_vterm)
    return;

  vterm_set_utf8(_vterm, 1);

  // Attach to our own vterm
  attach(_vterm);

  // Set output callback if it was provided before vterm was created
  if (_outputCallback) {
    vterm_output_set_callback(
        _vterm,
        [](const char *data, size_t len, void *user) {
          auto *self = static_cast<GPUScreenImpl *>(user);
          if (self->_outputCallback) {
            self->_outputCallback(data, len);
          }
        },
        this);
  }

  // Set up vterm input callback for card ANSI output
  // This allows cards to inject their ANSI-encoded cells into vterm
  vtermInputCallback_ = [this](const char *data, size_t len) {
    yinfo(">>> vtermInputCallback_: injecting {} bytes into vterm", len);
    if (_vterm) {
      vterm_input_write(_vterm, data, len);
    }
  };
  yinfo(">>> GPUScreenImpl::createVTerm: vtermInputCallback_ SET");
}

void GPUScreenImpl::write(const char *data, size_t len) {
  ydebug("GPUScreenImpl::write: len={} _vterm={}", len, (void *)_vterm);
  if (_vterm && len > 0) {
    vterm_input_write(_vterm, data, len);
  }
}

void GPUScreenImpl::resize(uint32_t cols, uint32_t rows) {
  // Create vterm on first resize if needed
  if (!_vterm) {
    resizeInternal(static_cast<int>(rows), static_cast<int>(cols), nullptr);
    createVTerm();
    return;
  }

  // Only resize if size actually changed
  if (static_cast<int>(rows) != _rows || static_cast<int>(cols) != _cols) {
    // vterm_set_size triggers our onResize callback which handles everything
    vterm_set_size(_vterm, static_cast<int>(rows), static_cast<int>(cols));
    // Notify Terminal so it can update PTY window size (TIOCSWINSZ)
    if (_resizeCallback) {
      _resizeCallback(cols, rows);
    }
  }
}

void GPUScreenImpl::setViewport(float x, float y, float width, float height) {
  ydebug("GPUScreen {} setViewport({}, {}, {}, {})", _id, x, y, width, height);
  _viewportX = x;
  _viewportY = y;
  _viewportWidth = width;
  _viewportHeight = height;

  // Calculate cols/rows from viewport size and float cell size
  // Must use the same float cell dimensions the shader uses (baseCellSize * zoom)
  // to avoid integer truncation giving more cols than actually fit in the viewport
  float cellWidthF = _baseCellWidth * _zoomLevel;
  float cellHeightF = _baseCellHeight * _zoomLevel;

  uint32_t totalCols = cellWidthF > 0 ? static_cast<uint32_t>(width / cellWidthF) : 0;
  uint32_t totalRows = cellHeightF > 0 ? static_cast<uint32_t>(height / cellHeightF) : 0;

  // Reserve 1 row for status line (not managed by vterm)
  uint32_t vtermRows = (_statusLineEnabled && totalRows > 1) ? totalRows - 1 : totalRows;

  // Only resize if size actually changed
  if (totalCols > 0 && vtermRows > 0 && (totalCols != _cols || vtermRows != _rows)) {
    resize(totalCols, vtermRows);
    // Resize status line buffer to match columns
    _statusLine.resize(totalCols);
    updateStatusLine();
  }

  // Update cards with viewport origin
  for (auto& [slotIndex, card] : _cards) {
    card->setScreenOrigin(x, y);
  }
}

// Helper: count non-blank cells from end of row (like screen.c line_popcount)
int GPUScreenImpl::linePopcount(const std::vector<Cell> &buffer, int row,
                                int cols) const {
  int col = cols - 1;
  while (col >= 0) {
    size_t idx = static_cast<size_t>(row * cols + col);
    if (idx < buffer.size() && buffer[idx].glyph != 0 &&
        buffer[idx].glyph != _cachedSpaceGlyph) {
      break;
    }
    col--;
  }
  return col + 1;
}

// Resize a single buffer with proper reflow (mirrors screen.c resize_buffer)
void GPUScreenImpl::resizeBuffer(int bufidx, int newRows, int newCols,
                                 bool active, VTermStateFields *fields) {
  yfunc();
  int oldRows = _rows;
  int oldCols = _cols;

  std::vector<Cell> &oldBuffer = (bufidx == 0) ? _primaryBuffer : _altBuffer;
  VTermLineInfo *oldLineinfo = fields ? fields->lineinfos[bufidx] : nullptr;

  // Allocate new buffer
  std::vector<Cell> newBuffer(static_cast<size_t>(newRows * newCols));
  clearBuffer(newBuffer);

  // Allocate new lineinfo
  VTermLineInfo *newLineinfo = nullptr;
  if (fields) {
    newLineinfo = static_cast<VTermLineInfo *>(
        calloc(static_cast<size_t>(newRows), sizeof(VTermLineInfo)));
  }

  int oldRow = oldRows - 1;
  int newRow = newRows - 1;

  VTermPos oldCursor = {_cursorRow, _cursorCol};
  VTermPos newCursor = {-1, -1};

  int finalBlankRow = newRows;

  // Walk backwards through old buffer, processing logical lines
  while (oldRow >= 0 && !oldBuffer.empty()) {
    int oldRowEnd = oldRow;

    // Find start of logical line by following continuation markers backwards
    while (_reflow && oldLineinfo && oldRow >= 0 &&
           oldLineinfo[oldRow].continuation) {
      oldRow--;
    }
    int oldRowStart = oldRow;

    // Calculate total width of logical line
    int width = 0;
    for (int row = oldRowStart; row <= oldRowEnd; row++) {
      if (_reflow && row < (oldRows - 1) && oldLineinfo &&
          oldLineinfo[row + 1].continuation) {
        width += oldCols; // Full row if next row is continuation
      } else {
        width += linePopcount(oldBuffer, row, oldCols);
      }
    }

    if (finalBlankRow == (newRow + 1) && width == 0) {
      finalBlankRow = newRow;
    }

    // Calculate how many new rows needed for this logical line
    int newHeight = _reflow ? (width ? (width + newCols - 1) / newCols : 1) : 1;

    int newRowEnd = newRow;
    int newRowStart = newRow - newHeight + 1;

    oldRow = oldRowStart;
    int oldCol = 0;

    int spareRows = newRows - finalBlankRow;

    // If we'd fall off top, try to scroll content down into blank rows
    if (newRowStart < 0 && spareRows >= 0 &&
        (!active || newCursor.row == -1 ||
         (newCursor.row - newRowStart) < newRows)) {
      int downwards = -newRowStart;
      if (downwards > spareRows)
        downwards = spareRows;
      int rowcount = newRows - downwards;

      // Move existing content down
      for (int r = rowcount - 1; r >= 0; r--) {
        for (int c = 0; c < newCols; c++) {
          newBuffer[(r + downwards) * newCols + c] = newBuffer[r * newCols + c];
        }
        if (newLineinfo) {
          newLineinfo[r + downwards] = newLineinfo[r];
        }
      }

      newRow += downwards;
      newRowStart += downwards;
      newRowEnd += downwards;

      if (newCursor.row >= 0)
        newCursor.row += downwards;
      finalBlankRow += downwards;
    }

    if (newRowStart < 0) {
      // Content falls off top - update cursor if it was in this logical line
      if (oldRowStart <= oldCursor.row && oldCursor.row <= oldRowEnd) {
        newCursor.row = 0;
        newCursor.col = oldCursor.col;
        if (newCursor.col >= newCols)
          newCursor.col = newCols - 1;
      }
      break;
    }

    // Copy content with reflow
    for (newRow = newRowStart, oldRow = oldRowStart; newRow <= newRowEnd;
         newRow++) {
      int count = (width >= newCols) ? newCols : width;
      width -= count;

      int newCol = 0;

      while (count > 0) {
        size_t oldIdx = static_cast<size_t>(oldRow * oldCols + oldCol);
        size_t newIdx = static_cast<size_t>(newRow * newCols + newCol);

        if (oldIdx < oldBuffer.size() && newIdx < newBuffer.size()) {
          newBuffer[newIdx] = oldBuffer[oldIdx];
        }

        // Track cursor
        if (oldCursor.row == oldRow && oldCursor.col == oldCol) {
          newCursor.row = newRow;
          newCursor.col = newCol;
        }

        oldCol++;
        if (oldCol == oldCols) {
          oldRow++;
          if (!_reflow) {
            newCol++;
            break;
          }
          oldCol = 0;
        }

        newCol++;
        count--;
      }

      // Handle cursor at end of copied content
      if (oldCursor.row == oldRow && oldCursor.col >= oldCol) {
        newCursor.row = newRow;
        newCursor.col = oldCursor.col - oldCol + newCol;
        if (newCursor.col >= newCols)
          newCursor.col = newCols - 1;
      }

      // Set continuation marker for new lineinfo
      if (newLineinfo) {
        newLineinfo[newRow].continuation = (newRow > newRowStart) ? 1 : 0;
      }
    }

    oldRow = oldRowStart - 1;
    newRow = newRowStart - 1;
  }

  // Handle cursor that would be off top
  if (oldCursor.row <= oldRow) {
    newCursor.row = 0;
    newCursor.col = oldCursor.col;
    if (newCursor.col >= newCols)
      newCursor.col = newCols - 1;
  }

  // Fallback cursor position
  if (active && (newCursor.row == -1 || newCursor.col == -1)) {
    newCursor.row = 0;
    newCursor.col = 0;
  }

  // Handle scrollback for primary buffer
  if (oldRow >= 0 && bufidx == 0) {
    // Push remaining top lines to scrollback
    _visibleBuffer = &oldBuffer;
    for (int row = 0; row <= oldRow; row++) {
      pushLineToScrollback(row);
    }
    if (active) {
      newCursor.row -= (oldRow + 1);
      if (newCursor.row < 0)
        newCursor.row = 0;
    }
  }

  // If there's space at top, scroll content up
  if (newRow >= 0) {
    int moveRows = newRows - newRow - 1;
    for (int r = 0; r < moveRows; r++) {
      for (int c = 0; c < newCols; c++) {
        newBuffer[r * newCols + c] = newBuffer[(r + newRow + 1) * newCols + c];
      }
      if (newLineinfo) {
        newLineinfo[r] = newLineinfo[r + newRow + 1];
      }
    }

    newCursor.row -= (newRow + 1);
    if (newCursor.row < 0)
      newCursor.row = 0;

    // Clear bottom rows
    for (int r = moveRows; r < newRows; r++) {
      for (int c = 0; c < newCols; c++) {
        size_t idx = static_cast<size_t>(r * newCols + c);
        newBuffer[idx].glyph = _cachedSpaceGlyph;
      }
      if (newLineinfo) {
        newLineinfo[r] = VTermLineInfo{};
      }
    }
  }

  // Store new buffer
  if (bufidx == 0) {
    _primaryBuffer = std::move(newBuffer);
  } else {
    _altBuffer = std::move(newBuffer);
  }

  // Update lineinfo in fields
  if (fields && oldLineinfo) {
    free(oldLineinfo);
    fields->lineinfos[bufidx] = newLineinfo;
  }

  // Update cursor if this is active buffer
  if (active) {
    _cursorRow = newCursor.row;
    _cursorCol = newCursor.col;
  }
}

void GPUScreenImpl::resizeInternal(int rows, int cols,
                                   VTermStateFields *fields) {
  // Check if this is a no-op (same size AND buffers already allocated)
  if (rows == _rows && cols == _cols && !_primaryBuffer.empty()) {
    return;
  }

  yinfo("GPUScreenImpl::resize: {}x{} -> {}x{} isAltScreen={}", _rows, _cols,
        rows, cols, _isAltScreen);

  int oldRows = _rows;
  int oldCols = _cols;
  bool altscreenActive = _isAltScreen;

  // Resize primary buffer (active if not on altscreen)
  resizeBuffer(0, rows, cols, !altscreenActive, fields);

  // Resize alt buffer if it exists
  if (!_altBuffer.empty()) {
    resizeBuffer(1, rows, cols, altscreenActive, fields);
  } else if (rows != oldRows && fields && fields->lineinfos[1]) {
    // Alt buffer not in use but we need to resize its lineinfo
    free(fields->lineinfos[1]);
    VTermLineInfo *newLineinfo = static_cast<VTermLineInfo *>(
        calloc(static_cast<size_t>(rows), sizeof(VTermLineInfo)));
    fields->lineinfos[1] = newLineinfo;
  }

  // Update dimensions
  _rows = rows;
  _cols = cols;

  // Resize view buffer and scratch buffer
  size_t numCells = static_cast<size_t>(rows * cols);
  viewBuffer_.clear();
  viewBuffer_.resize(numCells);
  scratchBuffer_.resize(cols);

  // Re-establish visible buffer pointer
  _visibleBuffer = altscreenActive ? &_altBuffer : &_primaryBuffer;

  // Reset scroll offset
  if (!altscreenActive) {
    _scrollOffset = 0;
  }

  _hasDamage = true;
  viewBufferDirty_ = true;
}

void GPUScreenImpl::clearBuffer(std::vector<Cell> &buffer) {
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  colorToRGB(defaultFg_, fgR, fgG, fgB);
  colorToRGB(defaultBg_, bgR, bgG, bgB);

  Cell defaultCell;
  defaultCell.glyph = _cachedSpaceGlyph;
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

void GPUScreenImpl::switchToScreen(bool alt) {
  if (_isAltScreen == alt)
    return; // Already on the requested screen

  yinfo("GPUScreenImpl::switchToScreen: {} -> {}",
        _isAltScreen ? "alt" : "primary", alt ? "alt" : "primary");

  _isAltScreen = alt;

  if (alt) {
    // Allocate alt buffer if not yet allocated
    if (_altBuffer.empty()) {
      size_t numCells = static_cast<size_t>(_rows * _cols);
      _altBuffer.resize(numCells);
    }

    // Switch to alternate screen
    _visibleBuffer = &_altBuffer;

    // Clear alternate screen on entry (traditional terminal behavior)
    clearBuffer(_altBuffer);

    // Alternate screen doesn't use scrollback
    _scrollOffset = 0;
  } else {
    // Switch back to primary screen
    _visibleBuffer = &_primaryBuffer;

    // Primary screen content is preserved (was just hidden)
  }

  _hasDamage = true;
  viewBufferDirty_ = true;
}

void GPUScreenImpl::reset() {
  // Clear current screen with space and default colors
  if (_visibleBuffer) {
    clearBuffer(*_visibleBuffer);
  }

  _cursorRow = 0;
  _cursorCol = 0;
  _cursorVisible = true;
  _hasDamage = true;
  viewBufferDirty_ = true;
  if (!_isAltScreen) {
    _scrollOffset = 0;
  }
}

//=============================================================================
// Buffer access - returns Cell buffer directly for GPU upload (zero-copy)
//=============================================================================

const Cell *GPUScreenImpl::getCellData() const {
  if (_scrollOffset > 0 || (_searchActive && !_searchMatches.empty())) {
    const_cast<GPUScreenImpl *>(this)->composeViewBuffer();
    return viewBuffer_.data();
  }
  if (_visibleBuffer) {
    return _visibleBuffer->data();
  }
  return nullptr;
}

Cell GPUScreenImpl::getCell(int row, int col) const {
  if (!_visibleBuffer || row < 0 || col < 0 || row >= _rows || col >= _cols) {
    Cell empty = {};
    return empty;
  }
  size_t idx = static_cast<size_t>(row) * static_cast<size_t>(_cols) +
               static_cast<size_t>(col);
  return (*_visibleBuffer)[idx];
}

//=============================================================================
// Scrollback control
//=============================================================================

void GPUScreenImpl::scrollUp(int lines) {
  int maxOffset = static_cast<int>(_scrollback.size());
  int newOffset = std::min(_scrollOffset + lines, maxOffset);
  if (newOffset != _scrollOffset) {
    _scrollOffset = newOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollDown(int lines) {
  int newOffset = std::max(_scrollOffset - lines, 0);
  if (newOffset != _scrollOffset) {
    _scrollOffset = newOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollToTop() {
  int maxOffset = static_cast<int>(_scrollback.size());
  if (_scrollOffset != maxOffset) {
    _scrollOffset = maxOffset;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::scrollToBottom() {
  if (_scrollOffset != 0) {
    _scrollOffset = 0;
    viewBufferDirty_ = true;
    _hasDamage = true;
  }
}

void GPUScreenImpl::clampVisualZoomOffset() {
  if (_visualZoomScale <= 1.0f) return;

  float cellW = static_cast<float>(getCellWidth());
  float cellH = static_cast<float>(getCellHeight());
  int totalRows = static_cast<int>(_rows) + 1;  // +1 for status line
  float contentW = static_cast<float>(_cols) * cellW;
  float contentH = static_cast<float>(totalRows) * cellH;

  // Shader centers zoom on content center (screenSize/2), so offset range is symmetric
  // At zoom S, visible region is contentSize/S, max offset allows reaching edges
  float maxOffsetX = std::max(0.0f, contentW / 2.0f * (1.0f - 1.0f / _visualZoomScale));
  float maxOffsetY = std::max(0.0f, contentH / 2.0f * (1.0f - 1.0f / _visualZoomScale));

  _visualZoomOffsetX = std::clamp(_visualZoomOffsetX, -maxOffsetX, maxOffsetX);
  _visualZoomOffsetY = std::clamp(_visualZoomOffsetY, -maxOffsetY, maxOffsetY);
}

//=============================================================================
// View buffer composition (when scrolled back)
//=============================================================================

void GPUScreenImpl::composeViewBuffer() {
  bool needsHighlighting = _searchActive && !_searchMatches.empty();
  
  // If highlighting is needed, we always need fresh data (to avoid double-swapping)
  if (!viewBufferDirty_ && !needsHighlighting)
    return;

  // Always rebuild when highlighting is active to avoid double-swapping colors
  {
    // _scrollOffset lines come from scrollback, rest from visible
    // _scrollback[size-1] is newest, _scrollback[0] is oldest
    // When _scrollOffset = N, we show scrollback lines [size-N, size-1] at top
    // plus visible lines [0, rows-N-1] shifted down

    int sbSize = static_cast<int>(_scrollback.size());
    int sbLinesToShow = std::min(_scrollOffset, _rows);

    // Fill top rows from scrollback (newest first in view)
    for (int viewRow = 0; viewRow < sbLinesToShow; viewRow++) {
      // scrollback index: newest lines are at end
      // viewRow 0 should get _scrollback[sbSize - _scrollOffset]
      int sbIndex = sbSize - _scrollOffset + viewRow;
      if (sbIndex >= 0 && sbIndex < sbSize) {
        decompressLine(_scrollback[sbIndex], viewRow);
      }
    }

    // Fill bottom rows from visible buffer - single memcpy per row with Cell
    // struct
    if (_visibleBuffer) {
      for (int viewRow = sbLinesToShow; viewRow < _rows; viewRow++) {
        int visRow = viewRow - sbLinesToShow; // Source row in visible buffer
        size_t numCells = static_cast<size_t>(_cols);
        size_t srcOffset = static_cast<size_t>(visRow * _cols);
        size_t dstOffset = static_cast<size_t>(viewRow * _cols);

        // Single memcpy for entire row of cells
        std::memcpy(&viewBuffer_[dstOffset], &(*_visibleBuffer)[srcOffset],
                    numCells * sizeof(Cell));
      }
    }

    viewBufferDirty_ = false;
  }
  
  // Apply search highlighting to visible matches
  if (needsHighlighting) {
    int sbSize = static_cast<int>(_scrollback.size());
    // Calculate which total rows are currently visible
    // viewRow 0 = totalRow (sbSize - _scrollOffset)
    int firstVisibleTotalRow = sbSize - _scrollOffset;
    int lastVisibleTotalRow = firstVisibleTotalRow + _rows - 1;
    
    // Determine current match cells (for different highlight color)
    int currentMatchStartCol = _searchMatchCol;
    int currentMatchEndCol = _searchMatchCol + static_cast<int>(_searchQuery.length()) - 1;
    
    for (const auto& match : _searchMatches) {
      int totalRow = match.first;
      int col = match.second;
      
      // Check if this match is in visible range
      if (totalRow >= firstVisibleTotalRow && totalRow <= lastVisibleTotalRow && col < _cols) {
        int viewRow = totalRow - firstVisibleTotalRow;
        size_t idx = static_cast<size_t>(viewRow * _cols + col);
        if (idx < viewBuffer_.size()) {
          Cell& cell = viewBuffer_[idx];
          
          // Check if this is part of the current match
          bool isCurrentMatch = (totalRow == _searchMatchRow && 
                                 col >= currentMatchStartCol && 
                                 col <= currentMatchEndCol);
          
          if (isCurrentMatch) {
            // Current match: yellow background, black foreground
            cell.bgR = 255;
            cell.bgG = 255;
            cell.bgB = 0;
            cell.fgR = 0;
            cell.fgG = 0;
            cell.fgB = 0;
          } else {
            // Other matches: swap fg and bg colors
            std::swap(cell.fgR, cell.bgR);
            std::swap(cell.fgG, cell.bgG);
            std::swap(cell.fgB, cell.bgB);
          }
        }
      }
    }
  }
}

void GPUScreenImpl::decompressLine(const ScrollbackLineGPU &line, int viewRow) {
  size_t dstOffset = static_cast<size_t>(viewRow * _cols);

  // Copy cells directly from scrollback
  int lineCols = std::min(static_cast<int>(line.cells.size()), _cols);
  for (int col = 0; col < lineCols; col++) {
    viewBuffer_[dstOffset + col] = line.cells[col];
  }

  // Fill remainder with default cells
  uint8_t fgR = defaultFg_.rgb.red, fgG = defaultFg_.rgb.green,
          fgB = defaultFg_.rgb.blue;
  uint8_t bgR = defaultBg_.rgb.red, bgG = defaultBg_.rgb.green,
          bgB = defaultBg_.rgb.blue;

  Cell defaultCell;
  defaultCell.glyph = _cachedSpaceGlyph;
  defaultCell.fgR = fgR;
  defaultCell.fgG = fgG;
  defaultCell.fgB = fgB;
  defaultCell.bgR = bgR;
  defaultCell.bgG = bgG;
  defaultCell.bgB = bgB;
  defaultCell.alpha = 255;
  defaultCell.style = 0;

  for (int col = lineCols; col < _cols; col++) {
    viewBuffer_[dstOffset + col] = defaultCell;
  }
}

//=============================================================================
// Scrollback push (compress line and store)
//=============================================================================

void GPUScreenImpl::pushLineToScrollback(int row) {
  if (!_visibleBuffer)
    return; // Safety check

  ScrollbackLineGPU line;
  line.cells.resize(_cols);

  // Copy cell data directly
  size_t srcOffset = static_cast<size_t>(row * _cols);
  std::memcpy(line.cells.data(), &(*_visibleBuffer)[srcOffset],
              _cols * sizeof(Cell));

  // Scan this row for card glyphs — check if any card's top-left cell is
  // leaving the visible area.  Top-left is identified by bg=0 (relRow=0, relCol=0).
  suspendOffscreenCards(row);

  _scrollback.push_back(std::move(line));

  // If scrolled back, increment scroll offset to maintain view position
  if (_scrollOffset > 0) {
    _scrollOffset++;
  }

  // Limit scrollback size — dispose cards whose scrollback lines are gone
  while (_scrollback.size() > maxScrollback_) {
    _scrollback.pop_front();
    if (_scrollOffset > static_cast<int>(_scrollback.size())) {
      _scrollOffset = static_cast<int>(_scrollback.size());
    }
  }
}

//=============================================================================
// Text selection helpers
//=============================================================================

void GPUScreenImpl::clearSelection() {
  _selecting = false;
  _hasSelection = false;
  _hasDamage = true;
}

// Encode a Unicode codepoint as UTF-8 and append to string
static void appendUtf8(std::string& out, uint32_t cp) {
  if (cp < 0x80) {
    out += static_cast<char>(cp);
  } else if (cp < 0x800) {
    out += static_cast<char>(0xC0 | (cp >> 6));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else if (cp < 0x10000) {
    out += static_cast<char>(0xE0 | (cp >> 12));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  } else if (cp < 0x110000) {
    out += static_cast<char>(0xF0 | (cp >> 18));
    out += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
    out += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
    out += static_cast<char>(0x80 | (cp & 0x3F));
  }
}

std::string GPUScreenImpl::extractSelectedText() const {
  if (!_hasSelection || !_msdfFont) return "";

  // Normalize selection direction (start <= end)
  int r0 = _selStartRow, c0 = _selStartCol;
  int r1 = _selEndRow, c1 = _selEndCol;
  if (r0 > r1 || (r0 == r1 && c0 > c1)) {
    std::swap(r0, r1);
    std::swap(c0, c1);
  }

  std::string result;
  int sbSize = static_cast<int>(_scrollback.size());
  int sbLines = std::min(_scrollOffset, sbSize);

  for (int row = r0; row <= r1; row++) {
    int startCol = (row == r0) ? c0 : 0;
    int endCol = (row == r1) ? c1 : _cols - 1;
    std::string line;

    if (row < sbLines) {
      // Row comes from scrollback buffer
      int sbIdx = sbSize - sbLines + row;
      if (sbIdx >= 0 && sbIdx < sbSize) {
        const auto& sbLine = _scrollback[sbIdx];
        for (int col = startCol; col <= endCol; col++) {
          if (col < static_cast<int>(sbLine.cells.size())) {
            uint32_t glyphIdx = sbLine.cells[col].glyph;
            uint32_t cp = _msdfFont->atlas()->getCodepoint(glyphIdx);
            if (cp == 0) cp = ' ';
            appendUtf8(line, cp);
          }
        }
      }
    } else if (_visibleBuffer) {
      // Row comes from visible buffer - use cell glyph indices
      int bufRow = row - sbLines;
      if (bufRow >= 0 && bufRow < static_cast<int>(_rows)) {
        size_t rowOffset = static_cast<size_t>(bufRow * _cols);
        for (int col = startCol; col <= endCol; col++) {
          size_t idx = rowOffset + col;
          if (idx < _visibleBuffer->size()) {
            uint32_t glyphIdx = (*_visibleBuffer)[idx].glyph;
            uint32_t cp = _msdfFont->atlas()->getCodepoint(glyphIdx);
            if (cp == 0) cp = ' ';
            appendUtf8(line, cp);
          }
        }
      }
    }

    while (!line.empty() && line.back() == ' ') line.pop_back();
    if (row > r0) result += '\n';
    result += line;
  }

  return result;
}

//=============================================================================
// Text search (tmux-style)
//=============================================================================

std::string GPUScreenImpl::extractRowText(int totalRow) const {
  if (!_msdfFont) return "";

  int sbSize = static_cast<int>(_scrollback.size());
  std::string result;

  if (totalRow < sbSize) {
    // Row is in scrollback buffer
    const auto& sbLine = _scrollback[totalRow];
    for (int col = 0; col < static_cast<int>(sbLine.cells.size()); col++) {
      uint32_t glyphIdx = sbLine.cells[col].glyph;
      uint32_t cp = _msdfFont->atlas()->getCodepoint(glyphIdx);
      if (cp == 0) cp = ' ';
      appendUtf8(result, cp);
    }
  } else if (_visibleBuffer) {
    // Row is in visible buffer
    int bufRow = totalRow - sbSize;
    if (bufRow >= 0 && bufRow < _rows) {
      size_t rowOffset = static_cast<size_t>(bufRow * _cols);
      for (int col = 0; col < _cols; col++) {
        size_t idx = rowOffset + col;
        if (idx < _visibleBuffer->size()) {
          uint32_t glyphIdx = (*_visibleBuffer)[idx].glyph;
          uint32_t cp = _msdfFont->atlas()->getCodepoint(glyphIdx);
          if (cp == 0) cp = ' ';
          appendUtf8(result, cp);
        }
      }
    }
  }

  return result;
}

bool GPUScreenImpl::searchInRow(int totalRow, int startCol, bool forward, int& matchCol) const {
  std::string rowText = extractRowText(totalRow);
  if (rowText.empty() || _searchQuery.empty()) return false;

  if (forward) {
    size_t pos = rowText.find(_searchQuery, startCol);
    if (pos != std::string::npos) {
      matchCol = static_cast<int>(pos);
      return true;
    }
  } else {
    // Search backwards: find last occurrence before startCol
    size_t searchEnd = (startCol < 0) ? std::string::npos : static_cast<size_t>(startCol);
    size_t pos = rowText.rfind(_searchQuery, searchEnd);
    if (pos != std::string::npos) {
      matchCol = static_cast<int>(pos);
      return true;
    }
  }
  return false;
}

void GPUScreenImpl::scrollToRow(int totalRow) {
  int sbSize = static_cast<int>(_scrollback.size());

  if (totalRow < sbSize) {
    // Row is in scrollback - calculate offset to show this row at top of screen
    // _scrollOffset = N means we show scrollback lines [sbSize-N, sbSize-1] at top
    // To show totalRow at top: sbSize - _scrollOffset = totalRow
    // => _scrollOffset = sbSize - totalRow
    int newOffset = sbSize - totalRow;
    // Clamp to valid range
    newOffset = std::max(0, std::min(newOffset, sbSize));
    if (newOffset != _scrollOffset) {
      _scrollOffset = newOffset;
      viewBufferDirty_ = true;
      _hasDamage = true;
    }
  } else {
    // Row is in visible buffer - scroll to bottom
    scrollToBottom();
  }
}

bool GPUScreenImpl::search(const std::string& query) {
  // Search only works in copy mode
  if (!_copyMode) return false;

  if (query.empty()) {
    clearSearch();
    return false;
  }

  _searchQuery = query;
  _searchActive = true;
  _searchMatchRow = -1;
  _searchMatchCol = -1;

  // Find all matches for highlighting
  findAllMatches();
  _hasDamage = true;

  int sbSize = static_cast<int>(_scrollback.size());
  int totalRows = sbSize + _rows;

  ydebug("search: query='{}' totalRows={} sbSize={}", query, totalRows, sbSize);

  // Search from bottom (newest) to top (oldest), like tmux
  for (int row = totalRows - 1; row >= 0; row--) {
    int matchCol;
    if (searchInRow(row, 0, true, matchCol)) {
      _searchMatchRow = row;
      _searchMatchCol = matchCol;
      ydebug("search: found match at row={} col={}", row, matchCol);
      scrollToRow(row);
      return true;
    }
  }

  ydebug("search: no match found");
  return false;
}

bool GPUScreenImpl::searchNext() {
  // Search only works in copy mode
  if (!_copyMode) return false;
  if (!_searchActive || _searchQuery.empty()) return false;

  int sbSize = static_cast<int>(_scrollback.size());
  int totalRows = sbSize + _rows;

  // Start searching from current match position
  int startRow = (_searchMatchRow >= 0) ? _searchMatchRow : totalRows - 1;
  int startCol = (_searchMatchCol >= 0) ? _searchMatchCol + 1 : 0;

  // Search forward (upward in terminal = older content)
  // First, continue in current row from startCol
  if (startRow >= 0 && startRow < totalRows) {
    int matchCol;
    if (searchInRow(startRow, startCol, true, matchCol)) {
      _searchMatchRow = startRow;
      _searchMatchCol = matchCol;
      scrollToRow(startRow);
      return true;
    }
  }

  // Then search in previous rows (going up = older content)
  for (int row = startRow - 1; row >= 0; row--) {
    int matchCol;
    if (searchInRow(row, 0, true, matchCol)) {
      _searchMatchRow = row;
      _searchMatchCol = matchCol;
      scrollToRow(row);
      return true;
    }
  }

  // Wrap around: search from bottom
  for (int row = totalRows - 1; row > startRow; row--) {
    int matchCol;
    if (searchInRow(row, 0, true, matchCol)) {
      _searchMatchRow = row;
      _searchMatchCol = matchCol;
      scrollToRow(row);
      return true;
    }
  }

  return false;
}

bool GPUScreenImpl::searchPrevious() {
  // Search only works in copy mode
  if (!_copyMode) return false;
  if (!_searchActive || _searchQuery.empty()) return false;

  int sbSize = static_cast<int>(_scrollback.size());
  int totalRows = sbSize + _rows;

  // Start searching from current match position
  int startRow = (_searchMatchRow >= 0) ? _searchMatchRow : 0;
  int startCol = (_searchMatchCol > 0) ? _searchMatchCol - 1 : -1;

  // Search backward (downward in terminal = newer content)
  // First, continue in current row before startCol
  if (startRow >= 0 && startRow < totalRows && startCol >= 0) {
    int matchCol;
    if (searchInRow(startRow, startCol, false, matchCol)) {
      _searchMatchRow = startRow;
      _searchMatchCol = matchCol;
      scrollToRow(startRow);
      return true;
    }
  }

  // Then search in next rows (going down = newer content)
  for (int row = startRow + 1; row < totalRows; row++) {
    int matchCol;
    if (searchInRow(row, -1, false, matchCol)) {
      _searchMatchRow = row;
      _searchMatchCol = matchCol;
      scrollToRow(row);
      return true;
    }
  }

  // Wrap around: search from top
  for (int row = 0; row < startRow; row++) {
    int matchCol;
    if (searchInRow(row, -1, false, matchCol)) {
      _searchMatchRow = row;
      _searchMatchCol = matchCol;
      scrollToRow(row);
      return true;
    }
  }

  return false;
}

void GPUScreenImpl::clearSearch() {
  _searchQuery.clear();
  _searchMatchRow = -1;
  _searchMatchCol = -1;
  _searchActive = false;
  _searchMatches.clear();
  _currentMatchIndex = -1;
  _hasDamage = true;  // Need to redraw to remove highlighting
}

void GPUScreenImpl::findAllMatches() {
  _searchMatches.clear();
  _currentMatchIndex = -1;
  
  if (_searchQuery.empty()) return;
  
  int sbSize = static_cast<int>(_scrollback.size());
  int totalRows = sbSize + _rows;
  int queryLen = static_cast<int>(_searchQuery.length());
  
  // Find all matches in all rows
  for (int row = 0; row < totalRows; row++) {
    std::string rowText = extractRowText(row);
    size_t pos = 0;
    while ((pos = rowText.find(_searchQuery, pos)) != std::string::npos) {
      // Store starting position of each character in the match
      for (int i = 0; i < queryLen; i++) {
        _searchMatches.push_back({row, static_cast<int>(pos) + i});
      }
      pos += 1;  // Move forward to find overlapping matches
    }
  }
  
  ydebug("findAllMatches: found {} cells to highlight for query '{}'", 
         _searchMatches.size(), _searchQuery);
}

//=============================================================================
// Copy mode (tmux-style vi mode)
//=============================================================================

int GPUScreenImpl::getTotalRows() const {
  return static_cast<int>(_scrollback.size()) + _rows;
}

void GPUScreenImpl::enterCopyMode() {
  if (_copyMode) return;

  _copyMode = true;
  _copySelecting = false;
  _copyRectMode = false;
  _searchInputMode = false;
  _gPending = 0;

  // Position cursor at bottom of visible area (current view)
  int sbSize = static_cast<int>(_scrollback.size());
  _copyCursorRow = sbSize + _rows - 1;  // Last visible row in total row space
  _copyCursorCol = 0;

  // Scroll up by 1 to enter scrollback view if not already
  if (_scrollOffset == 0 && sbSize > 0) {
    scrollUp(1);
  }

  updateCopyModeStatus();
  ydebug("Entered copy mode at row={} col={}", _copyCursorRow, _copyCursorCol);
}

void GPUScreenImpl::exitCopyMode() {
  _copyMode = false;
  _copySelecting = false;
  _copyRectMode = false;
  _searchInputMode = false;
  _gPending = 0;
  clearSelection();
  clearSearch();
  scrollToBottom();
  _statusText = "[0] yetty";
  updateStatusLine();
  ydebug("Exited copy mode");
}

void GPUScreenImpl::updateCopyModeStatus() {
  if (!_copyMode) return;

  std::string status;

  if (_searchInputMode) {
    // In search mode, show only the search prompt and input
    status = _searchForward ? "/" : "?";
    status += _searchInputBuffer;
  } else {
    if (_copySelecting) {
      if (_copyRectMode) {
        status = "-- VISUAL BLOCK --";
      } else {
        status = "-- VISUAL --";
      }
    } else {
      status = "[copy mode]";
    }

    // Show cursor position
    status += " [" + std::to_string(_copyCursorRow) + "," + std::to_string(_copyCursorCol) + "]";
  }

  _statusText = status;
  updateStatusLine();
}

void GPUScreenImpl::copyModeMoveCursor(int dRow, int dCol) {
  int totalRows = getTotalRows();
  int sbSize = static_cast<int>(_scrollback.size());

  _copyCursorRow = std::max(0, std::min(_copyCursorRow + dRow, totalRows - 1));
  _copyCursorCol = std::max(0, std::min(_copyCursorCol + dCol, _cols - 1));

  // Update selection if selecting
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasSelection = true;
    _hasDamage = true;
  }

  // Calculate visible row range
  // When _scrollOffset = N, visible rows are [sbSize - N, sbSize - N + _rows - 1] in total row space
  int firstVisibleRow = sbSize - _scrollOffset;
  int lastVisibleRow = firstVisibleRow + _rows - 1;

  // Only scroll if cursor is outside visible area
  if (_copyCursorRow < firstVisibleRow) {
    // Cursor above visible area - scroll up to show it at top
    int newOffset = sbSize - _copyCursorRow;
    newOffset = std::max(0, std::min(newOffset, sbSize));
    if (newOffset != _scrollOffset) {
      _scrollOffset = newOffset;
      viewBufferDirty_ = true;
      _hasDamage = true;
    }
  } else if (_copyCursorRow > lastVisibleRow) {
    // Cursor below visible area - scroll down to show it at bottom
    int newOffset = sbSize - (_copyCursorRow - _rows + 1);
    newOffset = std::max(0, std::min(newOffset, sbSize));
    if (newOffset != _scrollOffset) {
      _scrollOffset = newOffset;
      viewBufferDirty_ = true;
      _hasDamage = true;
    }
  }

  updateCopyModeStatus();
  _gPending = 0;
}

void GPUScreenImpl::copyModeStartSelection() {
  _copySelecting = true;
  _selStartRow = _copyCursorRow;
  _selStartCol = _copyCursorCol;
  _selEndRow = _copyCursorRow;
  _selEndCol = _copyCursorCol;
  _hasSelection = true;
  _hasDamage = true;
  updateCopyModeStatus();
  ydebug("Started selection at row={} col={}", _copyCursorRow, _copyCursorCol);
}

void GPUScreenImpl::copyModeYank() {
  if (!_hasSelection) return;

  std::string text = extractSelectedText();
  if (!text.empty()) {
    auto loop = *base::EventLoop::instance();
    loop->dispatch(base::Event::copyEvent(std::make_shared<std::string>(text)));
    yinfo("Yanked {} bytes to clipboard", text.size());
  }

  // Exit copy mode after yank (like tmux)
  exitCopyMode();
}

void GPUScreenImpl::copyModeScrollHalfPage(bool up) {
  int delta = _rows / 2;
  copyModeMoveCursor(up ? -delta : delta, 0);
}

void GPUScreenImpl::copyModeScrollFullPage(bool up) {
  int delta = _rows - 1;
  copyModeMoveCursor(up ? -delta : delta, 0);
}

void GPUScreenImpl::copyModeGotoTop() {
  _copyCursorRow = 0;
  _copyCursorCol = 0;
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  scrollToRow(_copyCursorRow);
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoBottom() {
  _copyCursorRow = getTotalRows() - 1;
  _copyCursorCol = 0;
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  scrollToRow(_copyCursorRow);
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoLineStart() {
  _copyCursorCol = 0;
  if (_copySelecting) {
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoLineEnd() {
  // Find last non-blank character on line
  std::string rowText = extractRowText(_copyCursorRow);
  int lastNonBlank = static_cast<int>(rowText.size()) - 1;
  while (lastNonBlank >= 0 && rowText[lastNonBlank] == ' ') {
    lastNonBlank--;
  }
  _copyCursorCol = std::max(0, std::min(lastNonBlank, _cols - 1));
  if (_copySelecting) {
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeWordNext() {
  std::string rowText = extractRowText(_copyCursorRow);
  int col = _copyCursorCol;
  int len = static_cast<int>(rowText.size());

  // Skip current word
  while (col < len && rowText[col] != ' ') col++;
  // Skip whitespace
  while (col < len && rowText[col] == ' ') col++;

  if (col >= len) {
    // Move to next line
    if (_copyCursorRow < getTotalRows() - 1) {
      _copyCursorRow++;
      _copyCursorCol = 0;
    }
  } else {
    _copyCursorCol = col;
  }

  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  scrollToRow(_copyCursorRow);
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeWordPrev() {
  std::string rowText = extractRowText(_copyCursorRow);
  int col = _copyCursorCol;

  // Skip whitespace
  while (col > 0 && rowText[col - 1] == ' ') col--;
  // Skip current word
  while (col > 0 && rowText[col - 1] != ' ') col--;

  if (col == 0 && _copyCursorCol == 0) {
    // Move to previous line
    if (_copyCursorRow > 0) {
      _copyCursorRow--;
      rowText = extractRowText(_copyCursorRow);
      col = static_cast<int>(rowText.size());
      while (col > 0 && rowText[col - 1] == ' ') col--;
    }
  }

  _copyCursorCol = std::max(0, col);

  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  scrollToRow(_copyCursorRow);
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeWordEnd() {
  std::string rowText = extractRowText(_copyCursorRow);
  int col = _copyCursorCol;
  int len = static_cast<int>(rowText.size());

  // Move at least one character
  if (col < len - 1) col++;
  
  // Skip whitespace
  while (col < len && rowText[col] == ' ') col++;
  // Move to end of word
  while (col < len - 1 && rowText[col + 1] != ' ') col++;

  if (col >= len) {
    // Move to next line
    if (_copyCursorRow < getTotalRows() - 1) {
      _copyCursorRow++;
      rowText = extractRowText(_copyCursorRow);
      col = 0;
      while (col < static_cast<int>(rowText.size()) && rowText[col] == ' ') col++;
      while (col < static_cast<int>(rowText.size()) - 1 && rowText[col + 1] != ' ') col++;
    }
  }

  _copyCursorCol = std::min(col, _cols - 1);

  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  scrollToRow(_copyCursorRow);
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoScreenTop() {
  int sbSize = static_cast<int>(_scrollback.size());
  _copyCursorRow = sbSize - _scrollOffset;
  _copyCursorCol = 0;
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoScreenMid() {
  int sbSize = static_cast<int>(_scrollback.size());
  _copyCursorRow = sbSize - _scrollOffset + _rows / 2;
  _copyCursorCol = 0;
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeGotoScreenBot() {
  int sbSize = static_cast<int>(_scrollback.size());
  _copyCursorRow = sbSize - _scrollOffset + _rows - 1;
  _copyCursorCol = 0;
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

void GPUScreenImpl::copyModeParaNext() {
  int totalRows = getTotalRows();
  int row = _copyCursorRow;
  
  // Skip current non-empty lines
  while (row < totalRows - 1) {
    std::string text = extractRowText(row);
    bool empty = text.empty() || text.find_first_not_of(' ') == std::string::npos;
    if (empty) break;
    row++;
  }
  // Skip empty lines
  while (row < totalRows - 1) {
    std::string text = extractRowText(row);
    bool empty = text.empty() || text.find_first_not_of(' ') == std::string::npos;
    if (!empty) break;
    row++;
  }
  
  _copyCursorRow = row;
  _copyCursorCol = 0;
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  copyModeMoveCursor(0, 0);  // This will scroll if needed
}

void GPUScreenImpl::copyModeParaPrev() {
  int row = _copyCursorRow;
  
  // Skip current non-empty lines
  while (row > 0) {
    std::string text = extractRowText(row);
    bool empty = text.empty() || text.find_first_not_of(' ') == std::string::npos;
    if (empty) break;
    row--;
  }
  // Skip empty lines
  while (row > 0) {
    std::string text = extractRowText(row);
    bool empty = text.empty() || text.find_first_not_of(' ') == std::string::npos;
    if (!empty) break;
    row--;
  }
  
  _copyCursorRow = row;
  _copyCursorCol = 0;
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  copyModeMoveCursor(0, 0);  // This will scroll if needed
}

void GPUScreenImpl::copyModeFindChar(char ch, bool forward, bool till) {
  std::string rowText = extractRowText(_copyCursorRow);
  int col = _copyCursorCol;
  int len = static_cast<int>(rowText.size());
  
  _lastFindChar = ch;
  _lastFindCmd = forward ? (till ? 't' : 'f') : (till ? 'T' : 'F');
  
  if (forward) {
    for (int i = col + 1; i < len; i++) {
      if (rowText[i] == ch) {
        _copyCursorCol = till ? i - 1 : i;
        break;
      }
    }
  } else {
    for (int i = col - 1; i >= 0; i--) {
      if (rowText[i] == ch) {
        _copyCursorCol = till ? i + 1 : i;
        break;
      }
    }
  }
  
  _copyCursorCol = std::max(0, std::min(_copyCursorCol, _cols - 1));
  
  if (_copySelecting) {
    _selEndRow = _copyCursorRow;
    _selEndCol = _copyCursorCol;
    _hasDamage = true;
  }
  updateCopyModeStatus();
}

bool GPUScreenImpl::handleCopyModeKey(uint32_t codepoint, int key, int mods) {
  // Handle search input mode first
  if (_searchInputMode) {
    if (key == KEY_COPY_ENTER) {
      // Execute search
      _searchInputMode = false;
      if (!_searchInputBuffer.empty()) {
        // First search - find all matches and go to first one
        search(_searchInputBuffer);
        // Move cursor to match if found
        if (_searchMatchRow >= 0) {
          _copyCursorRow = _searchMatchRow;
          _copyCursorCol = _searchMatchCol;
          if (_copySelecting) {
            _selEndRow = _copyCursorRow;
            _selEndCol = _copyCursorCol;
            _hasDamage = true;
          }
        }
      }
      updateCopyModeStatus();
      return true;
    }
    if (key == KEY_COPY_ESCAPE) {
      // Cancel search
      _searchInputMode = false;
      _searchInputBuffer.clear();
      updateCopyModeStatus();
      return true;
    }
    if (key == 259) {  // GLFW_KEY_BACKSPACE
      if (!_searchInputBuffer.empty()) {
        _searchInputBuffer.pop_back();
      }
      updateCopyModeStatus();
      return true;
    }
    if (codepoint >= 32 && codepoint < 127) {
      _searchInputBuffer += static_cast<char>(codepoint);
      updateCopyModeStatus();
      return true;
    }
    return true;  // Consume all keys in search input mode
  }

  // Handle f/F/t/T char find mode
  if (_findCharMode) {
    _findCharMode = false;
    if (codepoint >= 32 && codepoint < 127) {
      bool forward = (_findCharCmd == 'f' || _findCharCmd == 't');
      bool till = (_findCharCmd == 't' || _findCharCmd == 'T');
      copyModeFindChar(static_cast<char>(codepoint), forward, till);
    }
    return true;
  }

  // Check for Ctrl modifiers
  bool ctrl = (mods & 0x0002) != 0;

  // Ctrl+u = half page up
  if (ctrl && codepoint == KEY_COPY_HALFPAGE_UP) {
    copyModeScrollHalfPage(true);
    return true;
  }
  // Ctrl+d = half page down
  if (ctrl && codepoint == KEY_COPY_HALFPAGE_DOWN) {
    copyModeScrollHalfPage(false);
    return true;
  }
  // Ctrl+b = page up
  if (ctrl && codepoint == KEY_COPY_PAGE_UP) {
    copyModeScrollFullPage(true);
    return true;
  }
  // Ctrl+f = page down
  if (ctrl && codepoint == KEY_COPY_PAGE_DOWN) {
    copyModeScrollFullPage(false);
    return true;
  }
  // Ctrl+v = rectangle selection mode
  if (ctrl && codepoint == 'v') {
    if (!_copySelecting) {
      copyModeStartSelection();
    }
    _copyRectMode = !_copyRectMode;
    updateCopyModeStatus();
    return true;
  }

  // Handle non-Ctrl keys
  switch (codepoint) {
    // Navigation
    case KEY_COPY_LEFT:
      copyModeMoveCursor(0, -1);
      return true;
    case KEY_COPY_DOWN:
      copyModeMoveCursor(1, 0);
      return true;
    case KEY_COPY_UP:
      copyModeMoveCursor(-1, 0);
      return true;
    case KEY_COPY_RIGHT:
      copyModeMoveCursor(0, 1);
      return true;

    case KEY_COPY_WORD_NEXT:
      copyModeWordNext();
      return true;
    case KEY_COPY_WORD_PREV:
      copyModeWordPrev();
      return true;
    case KEY_COPY_WORD_END:
      copyModeWordEnd();
      return true;
    case KEY_COPY_WORD_NEXT_BIG:  // W
      copyModeWordNext();  // Same as w for now (whitespace-based)
      return true;
    case KEY_COPY_WORD_PREV_BIG:  // B
      copyModeWordPrev();  // Same as b for now
      return true;
    case KEY_COPY_WORD_END_BIG:   // E
      copyModeWordEnd();   // Same as e for now
      return true;

    case KEY_COPY_LINE_START:
    case KEY_COPY_LINE_FIRST_NONBLANK:
      copyModeGotoLineStart();
      return true;
    case KEY_COPY_LINE_END:
      copyModeGotoLineEnd();
      return true;

    // Screen position
    case KEY_COPY_SCREEN_TOP:  // H
      copyModeGotoScreenTop();
      return true;
    case KEY_COPY_SCREEN_MID:  // M
      copyModeGotoScreenMid();
      return true;
    case KEY_COPY_SCREEN_BOT:  // L
      copyModeGotoScreenBot();
      return true;

    // Paragraph movement
    case KEY_COPY_PARA_PREV:  // {
      copyModeParaPrev();
      return true;
    case KEY_COPY_PARA_NEXT:  // }
      copyModeParaNext();
      return true;

    // Find char in line
    case KEY_COPY_FIND_CHAR:      // f
    case KEY_COPY_FIND_CHAR_BACK: // F
    case KEY_COPY_TILL_CHAR:      // t
    case KEY_COPY_TILL_CHAR_BACK: // T
      _findCharMode = true;
      _findCharCmd = static_cast<char>(codepoint);
      return true;
    case KEY_COPY_REPEAT_FIND:    // ;
      if (_lastFindChar != 0) {
        bool forward = (_lastFindCmd == 'f' || _lastFindCmd == 't');
        bool till = (_lastFindCmd == 't' || _lastFindCmd == 'T');
        copyModeFindChar(_lastFindChar, forward, till);
      }
      return true;
    case KEY_COPY_REPEAT_FIND_REV: // ,
      if (_lastFindChar != 0) {
        bool forward = !(_lastFindCmd == 'f' || _lastFindCmd == 't');
        bool till = (_lastFindCmd == 't' || _lastFindCmd == 'T');
        copyModeFindChar(_lastFindChar, forward, till);
      }
      return true;

    case KEY_COPY_TOP:  // 'g'
      if (_gPending > 0) {
        // gg = go to top
        copyModeGotoTop();
        _gPending = 0;
      } else {
        _gPending = 1;
      }
      return true;

    case KEY_COPY_BOTTOM:  // 'G'
      copyModeGotoBottom();
      _gPending = 0;
      return true;

    // Search
    case KEY_COPY_SEARCH_FWD:
      _searchInputMode = true;
      _searchForward = true;
      _searchInputBuffer.clear();
      updateCopyModeStatus();
      return true;
    case KEY_COPY_SEARCH_BWD:
      _searchInputMode = true;
      _searchForward = false;
      _searchInputBuffer.clear();
      updateCopyModeStatus();
      return true;
    case KEY_COPY_SEARCH_NEXT:
      if (_searchForward) {
        searchNext();
      } else {
        searchPrevious();
      }
      if (_searchMatchRow >= 0) {
        _copyCursorRow = _searchMatchRow;
        _copyCursorCol = _searchMatchCol;
        if (_copySelecting) {
          _selEndRow = _copyCursorRow;
          _selEndCol = _copyCursorCol;
          _hasDamage = true;
        }
        scrollToRow(_copyCursorRow);
      }
      updateCopyModeStatus();
      return true;
    case KEY_COPY_SEARCH_PREV:
      if (_searchForward) {
        searchPrevious();
      } else {
        searchNext();
      }
      if (_searchMatchRow >= 0) {
        _copyCursorRow = _searchMatchRow;
        _copyCursorCol = _searchMatchCol;
        if (_copySelecting) {
          _selEndRow = _copyCursorRow;
          _selEndCol = _copyCursorCol;
          _hasDamage = true;
        }
        scrollToRow(_copyCursorRow);
      }
      updateCopyModeStatus();
      return true;

    // Selection
    case KEY_COPY_START_SELECTION:  // 'v' - toggle visual selection mode
      if (_copySelecting) {
        // Already selecting - cancel selection
        _copySelecting = false;
        _hasSelection = false;
        _hasDamage = true;
      } else {
        // Start selection at current cursor
        copyModeStartSelection();
      }
      updateCopyModeStatus();
      return true;
    case KEY_COPY_VISUAL_LINE:  // 'V' - visual line mode (select whole lines)
      if (!_copySelecting) {
        _copySelecting = true;
        _selStartRow = _copyCursorRow;
        _selStartCol = 0;
        _selEndRow = _copyCursorRow;
        _selEndCol = _cols - 1;
        _hasSelection = true;
        _hasDamage = true;
      }
      updateCopyModeStatus();
      return true;
    case KEY_COPY_YANK:
      copyModeYank();
      return true;
    case KEY_COPY_RECT_TOGGLE:  // Ctrl-v - rectangle mode
      _copyRectMode = !_copyRectMode;
      updateCopyModeStatus();
      return true;

    // Exit
    case KEY_COPY_QUIT:
      exitCopyMode();
      return true;

    default:
      break;
  }

  // Handle special keys (non-character keys)
  switch (key) {
    case KEY_COPY_ESCAPE:
      exitCopyMode();
      return true;
    case KEY_COPY_ENTER:
      if (_hasSelection) {
        copyModeYank();
      } else {
        exitCopyMode();
      }
      return true;
    // Arrow keys
    case 265:  // GLFW_KEY_UP
      copyModeMoveCursor(-1, 0);
      return true;
    case 264:  // GLFW_KEY_DOWN
      copyModeMoveCursor(1, 0);
      return true;
    case 263:  // GLFW_KEY_LEFT
      copyModeMoveCursor(0, -1);
      return true;
    case 262:  // GLFW_KEY_RIGHT
      copyModeMoveCursor(0, 1);
      return true;
    // Page Up/Down
    case 266:  // GLFW_KEY_PAGE_UP
      copyModeScrollFullPage(true);
      return true;
    case 267:  // GLFW_KEY_PAGE_DOWN
      copyModeScrollFullPage(false);
      return true;
    // Home/End
    case 268:  // GLFW_KEY_HOME
      copyModeGotoLineStart();
      return true;
    case 269:  // GLFW_KEY_END
      copyModeGotoLineEnd();
      return true;
    default:
      break;
  }

  _gPending = 0;  // Reset g pending on any other key
  return false;
}

//=============================================================================
// Status line (tmux-style bar at bottom)
//=============================================================================

void GPUScreenImpl::updateStatusLine() {
  if (!_statusLineEnabled || _cols == 0) return;

  // Ensure status line buffer is sized correctly
  if (_statusLine.size() != static_cast<size_t>(_cols)) {
    _statusLine.resize(_cols);
  }

  // Default colors for status line (green on black, like tmux default)
  uint8_t fgR = 0, fgG = 255, fgB = 0;      // Green text
  uint8_t bgR = 0, bgG = 0, bgB = 0;        // Black background
  uint8_t style = 0;

  // Get space glyph
  uint32_t spaceGlyph = _cachedSpaceGlyph;
  if (spaceGlyph == 0 && _msdfFont) {
    spaceGlyph = _msdfFont->getGlyphIndex(' ');
  }

  // Clear status line with spaces
  for (int col = 0; col < _cols; col++) {
    _statusLine[col] = {spaceGlyph, fgR, fgG, fgB, 255, bgR, bgG, bgB, style};
  }

  // Render status text
  if (!_statusText.empty() && _msdfFont) {
    int col = 0;
    for (size_t i = 0; i < _statusText.size() && col < _cols; i++) {
      char ch = _statusText[i];
      uint32_t glyph = _msdfFont->getGlyphIndex(static_cast<uint32_t>(ch));
      _statusLine[col] = {glyph, fgR, fgG, fgB, 255, bgR, bgG, bgB, style};
      col++;
    }
  }

  _hasDamage = true;
}

void GPUScreenImpl::setStatusText(const std::string& text) {
  _statusText = text;
  updateStatusLine();
}

void GPUScreenImpl::clearStatusLine() {
  _statusText.clear();
  updateStatusLine();
}

//=============================================================================
// Cell manipulation (writes to visible buffer)
//=============================================================================

void GPUScreenImpl::setCell(int row, int col, uint32_t glyph, uint8_t fgR,
                            uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG,
                            uint8_t bgB, uint8_t attrsByte) {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols)
    return;
  if (!_visibleBuffer)
    return; // Safety check

  size_t idx = cellIndex(row, col);

  // Bounds check on buffer
  if (idx >= _visibleBuffer->size())
    return;

  Cell &cell = (*_visibleBuffer)[idx];
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

void GPUScreenImpl::clearCell(int row, int col) {
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  colorToRGB(_pen.fg, fgR, fgG, fgB);
  colorToRGB(_pen.bg, bgR, bgG, bgB);

  if (_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  setCell(row, col, _cachedSpaceGlyph, fgR, fgG, fgB, bgR, bgG, bgB, 0);
}

void GPUScreenImpl::colorToRGB(const VTermColor &color, uint8_t &r, uint8_t &g,
                               uint8_t &b) {
  const char* colorType = "UNKNOWN";
  if (VTERM_COLOR_IS_DEFAULT_FG(&color)) {
    colorType = "DEFAULT_FG";
    r = defaultFg_.rgb.red;
    g = defaultFg_.rgb.green;
    b = defaultFg_.rgb.blue;
  } else if (VTERM_COLOR_IS_DEFAULT_BG(&color)) {
    colorType = "DEFAULT_BG";
    r = defaultBg_.rgb.red;
    g = defaultBg_.rgb.green;
    b = defaultBg_.rgb.blue;
  } else if (VTERM_COLOR_IS_INDEXED(&color)) {
    colorType = "INDEXED";
    VTermColor rgb = color;
    if (state_) {
      vterm_state_convert_color_to_rgb(state_, &rgb);
    }
    r = rgb.rgb.red;
    g = rgb.rgb.green;
    b = rgb.rgb.blue;
  } else {
    colorType = "RGB";
    r = color.rgb.red;
    g = color.rgb.green;
    b = color.rgb.blue;
  }

  // Debug: log non-default color conversions
  static int colorDebugCount = 0;
  bool isNonDefault = !VTERM_COLOR_IS_DEFAULT_FG(&color) && !VTERM_COLOR_IS_DEFAULT_BG(&color);
  if (colorDebugCount < 50 && isNonDefault) {
    colorDebugCount++;
    yinfo("colorToRGB: type={} idx={} -> RGB({},{},{})",
          colorType, color.indexed.idx, r, g, b);
  }
}

//=============================================================================
// State callbacks
//=============================================================================

int GPUScreenImpl::onPutglyph(VTermGlyphInfo *info, VTermPos pos, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  uint32_t cp = info->chars[0];

  // Debug: log all high codepoints (above BMP)
  if (cp >= 0x10000) {
    yinfo(">>> onPutglyph: HIGH CODEPOINT cp={:#x} pos=({},{})", cp, pos.row, pos.col);
  }

  if (cp == 0)
    cp = ' ';

  // Determine font type and glyph index based on codepoint
  uint32_t glyphIdx = 0;
  uint8_t fontType = FONT_TYPE_MSDF; // Default to MSDF text

  if (isVectorGlyph(cp)) {
    // Vector SDF font glyphs
    fontType = FONT_TYPE_VECTOR;
    glyphIdx = cp;  // Shader uses codepoint directly
  } else if (isCoverageGlyph(cp)) {
    // Vector Coverage font glyphs
    fontType = FONT_TYPE_COVERAGE;
    glyphIdx = cp;  // Shader uses codepoint directly
  } else if (isRasterGlyph(cp)) {
    // Raster font glyphs (texture atlas)
    fontType = FONT_TYPE_RASTER;
    glyphIdx = cp;  // Shader uses codepoint directly, converts to index
  } else if (isCardGlyph(cp)) {
    // Card glyphs (multi-cell widgets)
    fontType = FONT_TYPE_CARD;
    glyphIdx = self->_cardFont ? self->_cardFont->getGlyphIndex(cp) : cp;
    yinfo(">>> CARD GLYPH: cp={:#x} glyphIdx={} _cardFont={} pos=({},{})",
          cp, glyphIdx, (void*)self->_cardFont.get(), pos.row, pos.col);
  } else if (isShaderGlyph(cp)) {
    // Shader glyphs (single-cell animated)
    fontType = FONT_TYPE_SHADER;
    glyphIdx =
        self->_shaderGlyphFont ? self->_shaderGlyphFont->getGlyphIndex(cp) : cp;
  } else if (isEmoji(cp) && self->_bitmapFont) {
    // Emoji - render from bitmap font atlas
    fontType = FONT_TYPE_BITMAP;
    glyphIdx = self->_bitmapFont->getGlyphIndex(cp);
  } else if (self->_msdfFont) {
    // Regular text - get glyph from MSDF font
    fontType = FONT_TYPE_MSDF;
    glyphIdx =
        self->_msdfFont->getGlyphIndex(cp, self->_pen.bold, self->_pen.italic);
  } else {
    fontType = FONT_TYPE_MSDF;
    glyphIdx = cp;
  }

  // Get colors from current pen
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  self->colorToRGB(self->_pen.fg, fgR, fgG, fgB);
  self->colorToRGB(self->_pen.bg, bgR, bgG, bgB);

  // Debug: log first few colored glyphs with bold state
  static int debugCount = 0;
  bool hasColor = (bgR != 0 || bgG != 0 || bgB != 0) ||
                  (fgR != 240 || fgG != 240 || fgB != 240);
  if (debugCount < 10 && hasColor) {
    debugCount++;
    yinfo("onPutglyph: '{}' glyph={} bold={} fg=({},{},{}) fontType={}",
          (cp >= 32 && cp < 127) ? static_cast<char>(cp) : '?', glyphIdx,
          self->_pen.bold, fgR, fgG, fgB, fontType);
  }

  if (self->_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  // Pack attributes (bits 0-4) with font type (bits 5-7)
  uint8_t attrsByte = 0;
  if (self->_pen.bold)
    attrsByte |= 0x01;
  if (self->_pen.italic)
    attrsByte |= 0x02;
  attrsByte |= (self->_pen.underline & 0x03) << 2;
  if (self->_pen.strike)
    attrsByte |= 0x10;
  attrsByte |= (fontType & 0x07) << 5; // Pack font type into bits 5-7

  // Debug card glyph cells
  if (fontType == FONT_TYPE_CARD) {
    uint32_t cellSlotIndex = fgR | (static_cast<uint32_t>(fgG) << 8) | (static_cast<uint32_t>(fgB) << 16);
    uint32_t bg24 = bgR | (static_cast<uint32_t>(bgG) << 8) | (static_cast<uint32_t>(bgB) << 16);
    int relCol = bg24 & 0xFFF;
    int relRow = (bg24 >> 12) & 0xFFF;
    ydebug("CARD CELL: pos=({},{}) glyph={:#x} glyphIdx={} slotIndex={} relPos=({},{}) attrs={:#x}",
           pos.row, pos.col, cp, glyphIdx, cellSlotIndex, relRow, relCol, attrsByte);
  }

  self->setCell(pos.row, pos.col, glyphIdx, fgR, fgG, fgB, bgR, bgG, bgB,
                attrsByte);

  // Handle wide characters (width > 1)
  for (int i = 1; i < info->width; i++) {
    self->setCell(pos.row, pos.col + i, GLYPH_WIDE_CONT, fgR, fgG, fgB, bgR,
                  bgG, bgB, attrsByte);
  }

  self->_hasDamage = true;
  return 1;
}

int GPUScreenImpl::onMoveCursor(VTermPos pos, VTermPos oldpos, int visible,
                                void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  ydebug("GPUScreenImpl::onMoveCursor: ({},{}) -> ({},{})", oldpos.row,
         oldpos.col, pos.row, pos.col);
  self->_cursorRow = pos.row;
  self->_cursorCol = pos.col;
  self->_cursorVisible = visible != 0;
  self->_hasDamage = true;
  return 1;
}

int GPUScreenImpl::onScrollRect(VTermRect rect, int downward, int rightward,
                                void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  (void)rightward;

  bool isFullWidth = (rect.start_col == 0 && rect.end_col == self->_cols);

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
    }
  }

  // Return 0 to let vterm handle the actual scroll via moverect/erase callbacks
  return 0;
}

int GPUScreenImpl::onMoveRect(VTermRect dest, VTermRect src, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  int height = src.end_row - src.start_row;
  int width = src.end_col - src.start_col;
  int cols = self->_cols;
  int rows = self->_rows;

  // Bounds checking to prevent memory corruption
  if (height <= 0 || width <= 0)
    return 1;
  if (src.start_row < 0 || src.end_row > rows)
    return 1;
  if (dest.start_row < 0 || dest.start_row + height > rows)
    return 1;
  if (src.start_col < 0 || src.end_col > cols)
    return 1;
  if (dest.start_col < 0 || dest.start_col + width > cols)
    return 1;

  if (!self->_visibleBuffer)
    return 1; // Safety check

  // Ultra-fast path: full-width move starting at column 0
  // Memory is fully contiguous - single memmove for entire region of Cells
  if (src.start_col == 0 && dest.start_col == 0 && width == cols) {
    size_t srcIdx = self->cellIndex(src.start_row, 0);
    size_t dstIdx = self->cellIndex(dest.start_row, 0);
    size_t totalCells = static_cast<size_t>(height) * cols;

    // Single memmove for all cell data
    std::memmove(&(*self->_visibleBuffer)[dstIdx],
                 &(*self->_visibleBuffer)[srcIdx], totalCells * sizeof(Cell));
  }
  // Fast path: full-width but not starting at column 0 (scroll regions)
  // Still contiguous per row, do row-by-row with single memmove per row
  else if (src.start_col == dest.start_col && width == cols) {
    if (dest.start_row < src.start_row) {
      for (int row = 0; row < height; row++) {
        size_t srcIdx = self->cellIndex(src.start_row + row, 0);
        size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

        std::memmove(&(*self->_visibleBuffer)[dstIdx],
                     &(*self->_visibleBuffer)[srcIdx], width * sizeof(Cell));
      }
    } else {
      for (int row = height - 1; row >= 0; row--) {
        size_t srcIdx = self->cellIndex(src.start_row + row, 0);
        size_t dstIdx = self->cellIndex(dest.start_row + row, 0);

        std::memmove(&(*self->_visibleBuffer)[dstIdx],
                     &(*self->_visibleBuffer)[srcIdx], width * sizeof(Cell));
      }
    }
  } else {
    // General case: use pre-allocated scratch buffer, copy row-by-row
    // Ensure scratch buffer is large enough
    if (static_cast<int>(self->scratchBuffer_.size()) < width) {
      self->scratchBuffer_.resize(width);
    }

    // Determine copy direction
    bool copyForward =
        (dest.start_row < src.start_row) ||
        (dest.start_row == src.start_row && dest.start_col <= src.start_col);

    if (copyForward) {
      for (int row = 0; row < height; row++) {
        size_t srcRowStart =
            self->cellIndex(src.start_row + row, src.start_col);
        size_t dstRowStart =
            self->cellIndex(dest.start_row + row, dest.start_col);

        // Copy to scratch, then to dest (single memcpy for all cell data)
        std::memcpy(self->scratchBuffer_.data(),
                    &(*self->_visibleBuffer)[srcRowStart],
                    width * sizeof(Cell));
        std::memcpy(&(*self->_visibleBuffer)[dstRowStart],
                    self->scratchBuffer_.data(), width * sizeof(Cell));
      }
    } else {
      for (int row = height - 1; row >= 0; row--) {
        size_t srcRowStart =
            self->cellIndex(src.start_row + row, src.start_col);
        size_t dstRowStart =
            self->cellIndex(dest.start_row + row, dest.start_col);

        // Copy to scratch, then to dest (single memcpy for all cell data)
        std::memcpy(self->scratchBuffer_.data(),
                    &(*self->_visibleBuffer)[srcRowStart],
                    width * sizeof(Cell));
        std::memcpy(&(*self->_visibleBuffer)[dstRowStart],
                    self->scratchBuffer_.data(), width * sizeof(Cell));
      }
    }
  }

  self->_hasDamage = true;
  self->viewBufferDirty_ = true;
  return 1;
}

int GPUScreenImpl::onErase(VTermRect rect, int, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (!self->_visibleBuffer)
    return 1; // Safety check

  // Bounds checking to prevent memory corruption
  int startRow = std::max(0, rect.start_row);
  int endRow = std::min(self->_rows, rect.end_row);
  int startCol = std::max(0, rect.start_col);
  int endCol = std::min(self->_cols, rect.end_col);
  int width = endCol - startCol;
  int cols = self->_cols;

  if (width <= 0 || startRow >= endRow)
    return 1;

  // Pre-compute colors once (not per cell)
  uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
  self->colorToRGB(self->_pen.fg, fgR, fgG, fgB);
  self->colorToRGB(self->_pen.bg, bgR, bgG, bgB);
  if (self->_pen.reverse) {
    std::swap(fgR, bgR);
    std::swap(fgG, bgG);
    std::swap(fgB, bgB);
  }

  // Build default cell
  Cell defaultCell;
  defaultCell.glyph = self->_cachedSpaceGlyph;
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
      std::fill(&(*self->_visibleBuffer)[rowOffset],
                &(*self->_visibleBuffer)[rowOffset + cols], defaultCell);
    }
  } else {
    // Partial width
    for (int row = startRow; row < endRow; row++) {
      size_t rowOffset = static_cast<size_t>(row * cols);
      for (int col = startCol; col < endCol; col++) {
        (*self->_visibleBuffer)[rowOffset + col] = defaultCell;
      }
    }
  }

  self->_hasDamage = true;
  if (self->_scrollOffset > 0) {
    self->viewBufferDirty_ = true;
  }

  // On significant erase (e.g., clear screen), immediately GC orphaned cards
  // to free up card manager resources. Cards may still be in scrollback.
  int erasedRows = endRow - startRow;
  int erasedCols = endCol - startCol;
  bool isLargeErase = (erasedRows >= self->_rows / 2) && (erasedCols >= self->_cols / 2);
  if (isLargeErase && !self->_cards.empty()) {
    self->gcInactiveCards();
  }

  return 1;
}

int GPUScreenImpl::onInitPen(void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  // Reset pen to defaults
  self->_pen.fg = self->defaultFg_;
  self->_pen.bg = self->defaultBg_;
  self->_pen.bold = false;
  self->_pen.italic = false;
  self->_pen.underline = 0;
  self->_pen.strike = false;
  self->_pen.reverse = false;
  self->_pen.blink = false;

  return 1;
}

int GPUScreenImpl::onSetPenAttr(VTermAttr attr, VTermValue *val, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  switch (attr) {
  case VTERM_ATTR_BOLD:
    self->_pen.bold = val->boolean != 0;
    yinfo("VTERM_ATTR_BOLD: val->boolean={} pen.bold={}", val->boolean, self->_pen.bold);
    break;
  case VTERM_ATTR_ITALIC:
    self->_pen.italic = val->boolean != 0;
    yinfo("VTERM_ATTR_ITALIC: val->boolean={} pen.italic={}", val->boolean, self->_pen.italic);
    break;
  case VTERM_ATTR_UNDERLINE:
    self->_pen.underline = static_cast<uint8_t>(val->number & 0x03);
    break;
  case VTERM_ATTR_STRIKE:
    self->_pen.strike = val->boolean != 0;
    break;
  case VTERM_ATTR_REVERSE:
    self->_pen.reverse = val->boolean != 0;
    yinfo("VTERM_ATTR_REVERSE: val->boolean={} pen.reverse={}", val->boolean,
          self->_pen.reverse);
    break;
  case VTERM_ATTR_BLINK:
    self->_pen.blink = val->boolean != 0;
    break;
  case VTERM_ATTR_FOREGROUND:
    self->_pen.fg = val->color;
    yinfo("VTERM_ATTR_FOREGROUND: type={} idx={} rgb=({},{},{})",
          VTERM_COLOR_IS_DEFAULT_FG(&val->color) ? "DEFAULT_FG" :
          VTERM_COLOR_IS_DEFAULT_BG(&val->color) ? "DEFAULT_BG" :
          VTERM_COLOR_IS_INDEXED(&val->color) ? "INDEXED" : "RGB",
          val->color.indexed.idx,
          val->color.rgb.red, val->color.rgb.green, val->color.rgb.blue);
    break;
  case VTERM_ATTR_BACKGROUND:
    self->_pen.bg = val->color;
    yinfo("VTERM_ATTR_BACKGROUND: type={} idx={} rgb=({},{},{})",
          VTERM_COLOR_IS_DEFAULT_BG(&val->color) ? "DEFAULT_BG" :
          VTERM_COLOR_IS_DEFAULT_FG(&val->color) ? "DEFAULT_FG" :
          VTERM_COLOR_IS_INDEXED(&val->color) ? "INDEXED" : "RGB",
          val->color.indexed.idx,
          val->color.rgb.red, val->color.rgb.green, val->color.rgb.blue);
    break;
  default:
    break;
  }

  return 1;
}

int GPUScreenImpl::onSetTermProp(VTermProp prop, VTermValue *val, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (prop == VTERM_PROP_CURSORVISIBLE) {
    self->_cursorVisible = val->boolean != 0;
  } else if (prop == VTERM_PROP_CURSORBLINK) {
    self->_cursorBlink = val->boolean != 0;
  } else if (prop == VTERM_PROP_CURSORSHAPE) {
    self->_cursorShape = val->number;
  } else if (prop == VTERM_PROP_ALTSCREEN) {
    // Switch between primary and alternate screen
    self->switchToScreen(val->boolean != 0);
  }

  if (self->termPropCallback_) {
    self->termPropCallback_(prop, val);
  }

  return 1;
}

int GPUScreenImpl::onBell(void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  if (self->bellCallback_) {
    self->bellCallback_();
  }

  return 1;
}

int GPUScreenImpl::onResize(int rows, int cols, VTermStateFields *fields,
                            void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);
  // resizeInternal handles buffer reflow AND lineinfo reallocation (like
  // screen.c)
  self->resizeInternal(rows, cols, fields);
  return 1;
}

int GPUScreenImpl::onSetLineInfo(int, const VTermLineInfo *,
                                 const VTermLineInfo *, void *) {
  // Line info (double-width, double-height) - not commonly used
  return 1;
}

int GPUScreenImpl::onOSC(int command, VTermStringFragment frag, void *user) {
  auto *self = static_cast<GPUScreenImpl *>(user);

  yinfo(">>> onOSC: command={} initial={} final={} len={}", command,
         (bool)frag.initial, (bool)frag.final, (size_t)frag.len);

  // Handle color query OSC commands (10=fg, 11=bg, 12=cursor)
  // Query format: OSC 10 ; ? ST  ->  Response: OSC 10 ; rgb:RRRR/GGGG/BBBB ST
  if (command == 10 || command == 11 || command == 12) {
    // Check if this is a query (payload is "?")
    if (frag.final && frag.len >= 1 && frag.str[0] == '?') {
      VTermColor color;
      if (command == 10) {
        color = self->defaultFg_;
      } else if (command == 11) {
        color = self->defaultBg_;
      } else {
        // OSC 12: cursor color - use foreground as fallback
        color = self->defaultFg_;
      }

      // Convert indexed colors to RGB if needed
      if (VTERM_COLOR_IS_INDEXED(&color) && self->state_) {
        vterm_state_convert_color_to_rgb(self->state_, &color);
      }

      // Format response: OSC N ; rgb:RRRR/GGGG/BBBB ST
      // Use 4-digit hex (RRRR = RR * 257 to scale 8-bit to 16-bit)
      char response[64];
      snprintf(response, sizeof(response),
               "\033]%d;rgb:%04x/%04x/%04x\033\\",
               command,
               color.rgb.red * 257,
               color.rgb.green * 257,
               color.rgb.blue * 257);

      yinfo(">>> onOSC: color query {} -> rgb:{:02x}{:02x}{:02x}",
            command, color.rgb.red, color.rgb.green, color.rgb.blue);

      if (self->_outputCallback) {
        self->_outputCallback(response, strlen(response));
      }
      return 1;
    }
    // If not a query, ignore (setting colors not implemented)
    return 0;
  }

  // Accept yetty vendor commands: 666666 (cards), 666667-669 (effects), 666670 (gpu stats)
  if (command != YETTY_OSC_VENDOR_ID && command != 666667 && command != 666668 && command != 666669 && command != 666670) {
    yinfo(">>> onOSC: ignoring non-yetty command {}", command);
    return 0;
  }

  yinfo(">>> onOSC: *** YETTY OSC DETECTED *** command={}", command);

  if (frag.initial) {
    self->_oscBuffer.clear();
    self->_oscCommand = command;
    yinfo(">>> onOSC: started new OSC buffer");
  }

  if (frag.len > 0) {
    self->_oscBuffer.append(frag.str, frag.len);
    yinfo(">>> onOSC: appended {} bytes, total={}", (size_t)frag.len,
           self->_oscBuffer.size());
  }

  if (frag.final) {
    // Handle effect OSC commands directly (666667 = pre-effect, 666668 = post-effect, 666669 = effect)
    if (command == 666667 || command == 666668 || command == 666669) {
      self->handleEffectOSC(command, self->_oscBuffer);
      self->_oscBuffer.clear();
      self->_oscCommand = -1;
      return 1;
    }

    // Handle GPU stats OSC command (666670)
    if (command == 666670) {
      std::string stats = self->buildGpuStatsText();
      if (self->_outputCallback) {
        self->_outputCallback(stats.c_str(), stats.size());
      }
      self->_oscBuffer.clear();
      self->_oscCommand = -1;
      return 1;
    }

    std::string fullSeq = std::to_string(command) + ";" + self->_oscBuffer;
    yinfo("onOSC: FINAL - fullSeq len={} first100chars='{}'",
          fullSeq.size(), fullSeq.substr(0, 100));

    std::string response;
    uint32_t linesToAdvance = 0;

    // Forward OSC handling to handleOSCSequence
    bool handled = self->handleOSCSequence(fullSeq, &response, &linesToAdvance);

    yinfo("onOSC: handled={} response_len={} linesToAdvance={}", handled,
           response.size(), linesToAdvance);

    if (handled) {
      self->viewBufferDirty_ = true;
      if (!response.empty() && self->_outputCallback) {
        // Send response back to the shell
        self->_outputCallback(response.c_str(), response.size());
      }
      // Inject newlines immediately to advance cursor past the widget
      if (linesToAdvance > 0 && self->_vterm) {
        std::string nl(linesToAdvance, '\n');
        vterm_input_write(self->_vterm, nl.c_str(), nl.size());
      }
    }
    self->_oscBuffer.clear();
    self->_oscCommand = -1;
  }

  return 1;
}

//=============================================================================
// Card dormancy — suspend off-screen cards, reactivate when scrolled back
//=============================================================================

bool GPUScreenImpl::isCardSlotVisibleInBuffer(uint32_t slotIndex, int excludeRow) const {
  if (!_visibleBuffer)
    return false;

  const Cell *cells = _visibleBuffer->data();
  const uint8_t targetR = static_cast<uint8_t>(slotIndex & 0xFF);
  const uint8_t targetG = static_cast<uint8_t>((slotIndex >> 8) & 0xFF);
  const uint8_t targetB = static_cast<uint8_t>((slotIndex >> 16) & 0xFF);

  for (int row = 0; row < _rows; row++) {
    if (row == excludeRow)
      continue;
    size_t base = static_cast<size_t>(row) * _cols;
    for (int col = 0; col < _cols; col++) {
      const Cell &c = cells[base + col];
      if (isCardGlyph(c.glyph) &&
          c.fgR == targetR &&
          c.fgG == targetG &&
          c.fgB == targetB) {
        return true;
      }
    }
  }
  return false;
}

void GPUScreenImpl::suspendOffscreenCards(int scrolledRow) {
  if (!_visibleBuffer)
    return;

  // Scan the row being pushed to scrollback for ANY card glyph cell.
  // Collect unique slotIndices found on this row.
  size_t srcOffset = static_cast<size_t>(scrolledRow * _cols);
  std::unordered_set<uint32_t> slotsOnRow;

  for (int col = 0; col < _cols; col++) {
    const Cell &cell = (*_visibleBuffer)[srcOffset + col];
    if (!isCardGlyph(cell.glyph))
      continue;

    uint32_t slotIndex = static_cast<uint32_t>(cell.fgR) |
                         (static_cast<uint32_t>(cell.fgG) << 8) |
                         (static_cast<uint32_t>(cell.fgB) << 16);
    slotsOnRow.insert(slotIndex);
  }

  for (uint32_t slotIndex : slotsOnRow) {
    // Only consider cards we're actually tracking
    if (_cards.find(slotIndex) == _cards.end())
      continue;

    // Check if this card still has cells visible on OTHER rows in the buffer
    // (exclude the row being scrolled out — it's about to leave)
    if (isCardSlotVisibleInBuffer(slotIndex, scrolledRow)) {
      // Card still has visible cells on other rows — don't suspend yet.
      continue;
    }

    // Card is fully off-screen — suspend and move to inactive
    auto it = _cards.find(slotIndex);
    if (it != _cards.end()) {
      yinfo("GPUScreen::suspendOffscreenCards: suspending card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      if (it->second->needsBuffer())  _bufferLayoutChanged = true;
      if (it->second->needsTexture()) _textureLayoutChanged = true;
      it->second->suspend();
      _inactiveCards[slotIndex] = std::move(it->second);
      _cards.erase(it);
    }
  }
}

void GPUScreenImpl::reactivateVisibleCards() {
  if (_inactiveCards.empty())
    return;

  // Scan the composed view buffer for card glyphs that belong to inactive cards.
  // If found, move them back to _cards so update() runs and reconstructs buffers.
  const Cell *cells = getCellData();
  if (!cells)
    return;

  const size_t numCells = static_cast<size_t>(_rows) * _cols;

  // Collect slotIndices to reactivate (can't modify _inactiveCards during iteration)
  std::vector<uint32_t> toReactivate;

  for (size_t i = 0; i < numCells; i++) {
    if (!isCardGlyph(cells[i].glyph))
      continue;

    uint32_t slotIndex = static_cast<uint32_t>(cells[i].fgR) |
                         (static_cast<uint32_t>(cells[i].fgG) << 8) |
                         (static_cast<uint32_t>(cells[i].fgB) << 16);

    if (_inactiveCards.count(slotIndex)) {
      toReactivate.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toReactivate) {
    auto it = _inactiveCards.find(slotIndex);
    if (it != _inactiveCards.end()) {
      yinfo("GPUScreen::reactivateVisibleCards: reactivating card slotIndex={}", slotIndex);
      if (it->second->needsBuffer())  _bufferLayoutChanged = true;
      if (it->second->needsTexture()) _textureLayoutChanged = true;
      _cards[slotIndex] = std::move(it->second);
      _inactiveCards.erase(it);
    }
  }
}

void GPUScreenImpl::suspendNonVisibleCards() {
  if (_cards.empty())
    return;

  // Collect all card slotIndices visible in the composed view
  const Cell *cells = getCellData();
  if (!cells)
    return;

  const size_t numCells = static_cast<size_t>(_rows) * _cols;
  std::unordered_set<uint32_t> visibleSlots;

  for (size_t i = 0; i < numCells; i++) {
    if (!isCardGlyph(cells[i].glyph))
      continue;
    uint32_t slotIndex = static_cast<uint32_t>(cells[i].fgR) |
                         (static_cast<uint32_t>(cells[i].fgG) << 8) |
                         (static_cast<uint32_t>(cells[i].fgB) << 16);
    visibleSlots.insert(slotIndex);
  }

  // Suspend active cards not in the composed view
  std::vector<uint32_t> toSuspend;
  for (const auto& [slotIndex, card] : _cards) {
    if (visibleSlots.find(slotIndex) == visibleSlots.end()) {
      toSuspend.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toSuspend) {
    auto it = _cards.find(slotIndex);
    if (it != _cards.end()) {
      yinfo("GPUScreen::suspendNonVisibleCards: suspending card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      if (it->second->needsBuffer())  _bufferLayoutChanged = true;
      if (it->second->needsTexture()) _textureLayoutChanged = true;
      it->second->suspend();
      _inactiveCards[slotIndex] = std::move(it->second);
      _cards.erase(it);
    }
  }
}

void GPUScreenImpl::gcInactiveCards() {
  // Note: removed early return on _inactiveCards.empty() because we also
  // need to GC orphaned active cards (e.g., after clear screen)

  // Collect all card slotIndices referenced in visible buffer + scrollback
  std::unordered_set<uint32_t> liveSlots;

  // Scan visible buffer
  if (_visibleBuffer) {
    const Cell *cells = _visibleBuffer->data();
    const size_t numCells = static_cast<size_t>(_rows) * _cols;
    for (size_t i = 0; i < numCells; i++) {
      if (isCardGlyph(cells[i].glyph)) {
        uint32_t slot = static_cast<uint32_t>(cells[i].fgR) |
                        (static_cast<uint32_t>(cells[i].fgG) << 8) |
                        (static_cast<uint32_t>(cells[i].fgB) << 16);
        liveSlots.insert(slot);
      }
    }
  }

  // Scan scrollback
  for (const auto &line : _scrollback) {
    for (const auto &cell : line.cells) {
      if (isCardGlyph(cell.glyph)) {
        uint32_t slot = static_cast<uint32_t>(cell.fgR) |
                        (static_cast<uint32_t>(cell.fgG) << 8) |
                        (static_cast<uint32_t>(cell.fgB) << 16);
        liveSlots.insert(slot);
      }
    }
  }

  // Dispose inactive cards not referenced anywhere
  std::vector<uint32_t> toDispose;
  for (const auto &[slotIndex, card] : _inactiveCards) {
    if (liveSlots.find(slotIndex) == liveSlots.end()) {
      toDispose.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toDispose) {
    auto it = _inactiveCards.find(slotIndex);
    if (it != _inactiveCards.end()) {
      yinfo("GPUScreen::gcInactiveCards: disposing orphaned inactive card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      it->second->dispose();
      _inactiveCards.erase(it);
    }
  }

  // Also dispose active cards not referenced anywhere
  std::vector<uint32_t> toDisposeActive;
  for (const auto &[slotIndex, card] : _cards) {
    if (liveSlots.find(slotIndex) == liveSlots.end()) {
      toDisposeActive.push_back(slotIndex);
    }
  }

  for (uint32_t slotIndex : toDisposeActive) {
    auto it = _cards.find(slotIndex);
    if (it != _cards.end()) {
      yinfo("GPUScreen::gcInactiveCards: disposing orphaned active card '{}' slotIndex={}",
            it->second->typeName(), slotIndex);
      it->second->dispose();
      _cards.erase(it);
    }
  }

  if (!toDispose.empty() || !toDisposeActive.empty()) {
    yinfo("GPUScreen::gcInactiveCards: disposed {} inactive + {} active orphaned cards",
          toDispose.size(), toDisposeActive.size());
  }
}

//=============================================================================
// OSC Sequence Handling
//=============================================================================

bool GPUScreenImpl::handleOSCSequence(const std::string &sequence,
                                      std::string *response,
                                      uint32_t *linesToAdvance) {
  yinfo("GPUScreenImpl::handleOSCSequence: ENTERED, sequence len={}",
        sequence.size());

  // Parse vendor ID from sequence (format: "vendorId;...")
  size_t semicolon = sequence.find(';');
  if (semicolon == std::string::npos) {
    yerror("GPUScreenImpl::handleOSCSequence: no semicolon found!");
    if (response)
      *response = OscResponse::error("Invalid OSC sequence: no vendor ID");
    return false;
  }

  int vendorId = 0;
  try {
    vendorId = std::stoi(sequence.substr(0, semicolon));
  } catch (...) {
    yerror("GPUScreenImpl::handleOSCSequence: failed to parse vendor ID");
    if (response)
      *response = OscResponse::error("Invalid vendor ID");
    return false;
  }

  if (vendorId != YETTY_OSC_VENDOR_ID) {
    yerror("GPUScreenImpl::handleOSCSequence: unknown vendor ID {}", vendorId);
    if (response)
      *response = OscResponse::error("Unknown vendor ID");
    return false;
  }

  return handleCardOSCSequence(sequence, response, linesToAdvance);
}

//=============================================================================
// Effect OSC Handling (666667 = pre-effect, 666668 = post-effect, 666669 = effect)
// Payload format: INDEX:p0:p1:p2:p3:p4:p5  (params optional, 0 = disable)
//=============================================================================

void GPUScreenImpl::handleEffectOSC(int command, const std::string &payload) {
  const char* label = (command == 666667) ? "pre-effect" :
                      (command == 666668) ? "post-effect" : "effect";

  // Parse colon-separated values
  std::vector<float> values;
  std::string token;
  std::istringstream stream(payload);
  while (std::getline(stream, token, ':')) {
    if (!token.empty()) {
      try { values.push_back(std::stof(token)); } catch (...) { values.push_back(0.0f); }
    }
  }

  if (values.empty()) {
    yinfo("Effect OSC {}: disabling {}", command, label);
    if (command == 666667) {
      _uniforms.preEffectIndex = 0;
    } else if (command == 666668) {
      _uniforms.postEffectIndex = 0;
    } else {
      _uniforms.effectIndex = 0;
    }
    return;
  }

  uint32_t effectIndex = static_cast<uint32_t>(values[0]);

  if (command == 666667) {
    _uniforms.preEffectIndex = effectIndex;
    for (int i = 0; i < 6; i++) {
      _uniforms.preEffectParams[i] = (i + 1 < static_cast<int>(values.size())) ? values[i + 1] : 0.0f;
    }
    yinfo("Effect OSC: {} index={} params=[{},{},{},{},{},{}]",
          label, effectIndex,
          _uniforms.preEffectParams[0], _uniforms.preEffectParams[1],
          _uniforms.preEffectParams[2], _uniforms.preEffectParams[3],
          _uniforms.preEffectParams[4], _uniforms.preEffectParams[5]);
  } else if (command == 666668) {
    _uniforms.postEffectIndex = effectIndex;
    for (int i = 0; i < 6; i++) {
      _uniforms.postEffectParams[i] = (i + 1 < static_cast<int>(values.size())) ? values[i + 1] : 0.0f;
    }
    yinfo("Effect OSC: {} index={} params=[{},{},{},{},{},{}]",
          label, effectIndex,
          _uniforms.postEffectParams[0], _uniforms.postEffectParams[1],
          _uniforms.postEffectParams[2], _uniforms.postEffectParams[3],
          _uniforms.postEffectParams[4], _uniforms.postEffectParams[5]);
  } else {
    _uniforms.effectIndex = effectIndex;
    for (int i = 0; i < 6; i++) {
      _uniforms.effectParams[i] = (i + 1 < static_cast<int>(values.size())) ? values[i + 1] : 0.0f;
    }
    yinfo("Effect OSC: {} index={} params=[{},{},{},{},{},{}]",
          label, effectIndex,
          _uniforms.effectParams[0], _uniforms.effectParams[1],
          _uniforms.effectParams[2], _uniforms.effectParams[3],
          _uniforms.effectParams[4], _uniforms.effectParams[5]);
  }
}

//=============================================================================
// GPU Stats (OSC 666670)
//=============================================================================

std::string GPUScreenImpl::buildGpuStatsText() {
  std::ostringstream ss;
  ss << "\r\n=== GPU Memory Stats ===\r\n";

  // Global allocator
  if (_ctx.globalAllocator) {
    ss << "\r\n[Global Allocator]\r\n";
    ss << _ctx.globalAllocator->dumpAllocationsToString();
  }

  // Per-terminal allocator
  if (_allocator) {
    ss << "\r\n[Terminal Allocator]\r\n";
    ss << _allocator->dumpAllocationsToString();
  }

  // CardBufferManager sub-allocations
  if (_cardManager && _cardManager->bufferManager()) {
    auto stats = _cardManager->bufferManager()->getStats();
    ss << "\r\n[Card Buffer]\r\n";
    ss << "  used=" << stats.bufferUsed
       << " capacity=" << stats.bufferCapacity
       << " pending_uploads=" << stats.pendingBufferUploads << "\r\n";
    ss << _cardManager->bufferManager()->dumpSubAllocationsToString();
  }

  // CardTextureManager stats
  if (_cardManager && _cardManager->textureManager()) {
    auto stats = _cardManager->textureManager()->getStats();
    ss << "\r\n[Card Texture Atlas]\r\n";
    ss << "  textures=" << stats.textureCount
       << " atlas=" << stats.atlasWidth << "x" << stats.atlasHeight
       << " used_pixels=" << stats.usedPixels << "\r\n";
  }

  // Summary
  uint64_t globalBytes = _ctx.globalAllocator ? _ctx.globalAllocator->totalAllocatedBytes() : 0;
  uint64_t terminalBytes = _allocator ? _allocator->totalAllocatedBytes() : 0;
  uint64_t totalBytes = globalBytes + terminalBytes;
  ss << "\r\n[Total] " << totalBytes << " bytes ("
     << std::fixed << std::setprecision(2) << (totalBytes / (1024.0 * 1024.0))
     << " MB)\r\n";

  return ss.str();
}

//=============================================================================
// Card OSC Handling
//=============================================================================

bool GPUScreenImpl::handleCardOSCSequence(const std::string &sequence,
                                          std::string *response,
                                          uint32_t *linesToAdvance) {
  yinfo("GPUScreenImpl::handleCardOSCSequence: ENTERED with sequence length={}",
        sequence.size());
  yinfo("GPUScreenImpl::handleCardOSCSequence: sequence={}",
        sequence.substr(0, 200));

  if (!_ctx.cardFactory) {
    yerror("GPUScreenImpl::handleCardOSCSequence: no CardFactory!");
    if (response)
      *response = OscResponse::error("No CardFactory");
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: cardFactory is available");

  // Parse using the same OSC parser (same argument format)
  auto parseResult = _oscParser.parse(sequence);
  if (!parseResult) {
    yerror("GPUScreenImpl::handleCardOSCSequence: parse FAILED: {}",
           error_msg(parseResult));
    if (response)
      *response = OscResponse::error(error_msg(parseResult));
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: parse succeeded");

  OscCommand cmd = *parseResult;
  if (!cmd.isValid()) {
    yerror("GPUScreenImpl::handleCardOSCSequence: cmd invalid: {}", cmd.error);
    if (response)
      *response = OscResponse::error(cmd.error);
    return false;
  }
  yinfo("GPUScreenImpl::handleCardOSCSequence: cmd valid, type={}, card='{}'",
        static_cast<int>(cmd.type), cmd.run.card);

  switch (cmd.type) {
  case OscCommandType::Run: {
    yinfo(
        "GPUScreenImpl::handleCardOSCSequence: RUN command for card '{}'",
        cmd.run.card);
    int32_t x = cmd.run.x;
    int32_t y = cmd.run.y;

    ydebug("Card OSC Run: cmd.x={} cmd.y={} relative={}", x, y,
           cmd.run.relative);

    if (cmd.run.relative) {
      x += getCursorCol();
      y += getCursorRow();
      ydebug("Card OSC Run: final position ({},{})", x, y);
    }

    // Run GC before creation to free resources from orphaned inactive cards
    gcInactiveCards();

    // Resolve "stretch to edge" dimensions (0 = fill remaining terminal)
    // vterm handles scrolling automatically when writing past the bottom
    uint32_t widthCells = static_cast<uint32_t>(cmd.run.width);
    uint32_t heightCells = static_cast<uint32_t>(cmd.run.height);
    if (widthCells == 0 && _cols > 0) {
      widthCells = static_cast<uint32_t>(_cols);
    }
    if (heightCells == 0 && _rows > 0) {
      heightCells = static_cast<uint32_t>(_rows);
    }
    ydebug("Card OSC Run: resolved size {}x{} (terminal {}x{})", widthCells, heightCells, _cols, _rows);

    // Create the card (pass full YettyContext for font access etc)
    auto result = _ctx.cardFactory->createCard(
        _ctx, cmd.run.card, x, y, widthCells, heightCells,
        cmd.cardArgs, cmd.payload);

    if (!result) {
      yerror("GPUScreen: createCard failed: {}", error_msg(result));
      if (response)
        *response = OscResponse::error(error_msg(result));
      return false;
    }

    CardPtr cardPtr = std::move(*result);
    Card *card = cardPtr.get();

    // =====================================================================
    // Generate ANSI escape sequences for card cells
    // =====================================================================
    // Instead of directly writing to the grid buffer (which breaks scrolling),
    // we generate standard ANSI true-color escape sequences that flow through
    // vterm's normal line handling. This ensures:
    //   - Proper scrolling behavior
    //   - Correct scrollback buffer management
    //   - Widget cells move naturally with terminal content
    //
    // Encoding (fits in 24-bit ANSI true-color):
    //   fg RGB: metadata slot index (offset/32) - addresses 512MB
    //   bg RGB: (relRow << 12) | relCol - supports 4096x4096 widgets
    // =====================================================================

    if (vtermInputCallback_) {
      std::string ansiOutput;
      ansiOutput.reserve(card->widthCells() * card->heightCells() *
                         40); // Estimate

      uint32_t slotIndex = card->metadataSlotIndex();
      uint32_t shaderGlyph = card->shaderGlyph();
      uint32_t metaOffset = card->metadataOffset();

      ydebug("Card ANSI gen: type='{}' slotIndex={} shaderGlyph={:#x} metaOffset={} size={}x{}",
             card->typeName(), slotIndex, shaderGlyph, metaOffset,
             card->widthCells(), card->heightCells());

      // UTF-8 encode the shader glyph (Plane 16: U+100000 range)
      // For codepoints >= 0x10000: 4-byte UTF-8
      char glyphUtf8[5];
      glyphUtf8[0] = static_cast<char>(0xF0 | ((shaderGlyph >> 18) & 0x07));
      glyphUtf8[1] = static_cast<char>(0x80 | ((shaderGlyph >> 12) & 0x3F));
      glyphUtf8[2] = static_cast<char>(0x80 | ((shaderGlyph >> 6) & 0x3F));
      glyphUtf8[3] = static_cast<char>(0x80 | (shaderGlyph & 0x3F));
      glyphUtf8[4] = '\0';

      // fg color from slot index (24 bits)
      uint8_t fgR = slotIndex & 0xFF;
      uint8_t fgG = (slotIndex >> 8) & 0xFF;
      uint8_t fgB = (slotIndex >> 16) & 0xFF;

      ydebug("Card ANSI gen: fg=({},{},{}) for slotIndex={}", fgR, fgG, fgB, slotIndex);

      // ESC character - use explicit char to avoid any fmt::format issues
      const char ESC = '\033'; // 0x1B

      // Start with newline + reset to ensure parser is in ground state
      // The OSC ST (ESC \) may leave parser expecting more, so we
      // need to ensure we start fresh. The newline puts us on a new
      // line and resets parser state.
      // ansiOutput += "\n";
      ansiOutput += ESC;
      ansiOutput += "[0m";

      for (uint32_t row = 0; row < card->heightCells(); row++) {
        // Carriage return to start at column 0
        ansiOutput += '\r';

        for (uint32_t col = 0; col < card->widthCells(); col++) {
          // bg encodes relative position: (row << 12) | col
          uint32_t bg24 = (row << 12) | col;
          uint8_t bgR = bg24 & 0xFF;
          uint8_t bgG = (bg24 >> 8) & 0xFF;
          uint8_t bgB = (bg24 >> 16) & 0xFF;

          // ANSI true-color escape sequences
          // ESC[38;2;R;G;Bm - set fg color
          // ESC[48;2;R;G;Bm - set bg color
          ansiOutput += ESC;
          ansiOutput += fmt::format("[38;2;{};{};{}m", fgR, fgG, fgB);
          ansiOutput += ESC;
          ansiOutput += fmt::format("[48;2;{};{};{}m", bgR, bgG, bgB);
          ansiOutput += glyphUtf8;
        }
        // Reset attributes and newline - vterm handles scrolling!
        ansiOutput += ESC;
        ansiOutput += "[0m\n";
      }

      yinfo("GPUScreen: Writing {} bytes of ANSI sequences to vterm for card "
            "'{}'",
            ansiOutput.size(), card->typeName());

      // Write to vterm input - this flows through normal terminal processing
      vtermInputCallback_(ansiOutput.c_str(), ansiOutput.size());
    } else {
      yerror("GPUScreen: No vtermInputCallback_ set, cannot create card");
      return false;
    }

    yinfo("GPUScreen: Created card '{}' at ({},{}) size {}x{} slotIndex={} "
          "shaderGlyph={:#x}",
          card->typeName(), x, y, card->widthCells(), card->heightCells(),
          card->metadataSlotIndex(), card->shaderGlyph());

    // Store and register the card (transfer ownership)
    registerCard(std::move(cardPtr));

    // Don't set linesToAdvance - vterm handles cursor movement via the newlines
    // we output The ANSI sequences include newlines which advance the cursor
    // naturally

    _hasDamage = true;
    return true;
  }

  case OscCommandType::Cards: {
    if (response) {
      *response = OscResponse::pluginList(_ctx.cardFactory->getRegisteredCards());
    }
    return true;
  }

  case OscCommandType::List: {
    if (response) {
      std::vector<
          std::tuple<std::string, std::string, int, int, int, int, bool>>
          cardList;
      for (const auto &[slotIndex, card] : _cards) {
        cardList.emplace_back(
            std::to_string(slotIndex), card->typeName(),
            card->x(), card->y(),
            static_cast<int>(card->widthCells()),
            static_cast<int>(card->heightCells()), true);
      }
      for (const auto &[slotIndex, card] : _inactiveCards) {
        cardList.emplace_back(
            std::to_string(slotIndex), card->typeName(),
            card->x(), card->y(),
            static_cast<int>(card->widthCells()),
            static_cast<int>(card->heightCells()), false);
      }
      *response = OscResponse::cardList(cardList);
    }
    return true;
  }

  case OscCommandType::Kill: {
    if (cmd.target.id.empty()) {
      if (response)
        *response = OscResponse::error("kill: --id required");
      return false;
    }
    uint32_t slotIndex = 0;
    try { slotIndex = static_cast<uint32_t>(std::stoul(cmd.target.id)); }
    catch (...) {
      if (response)
        *response = OscResponse::error("kill: invalid card id");
      return false;
    }
    auto it = _cards.find(slotIndex);
    if (it == _cards.end())
      it = _inactiveCards.find(slotIndex);
    if (it == _cards.end() && _inactiveCards.find(slotIndex) == _inactiveCards.end()) {
      if (response)
        *response = OscResponse::error("Card not found: " + cmd.target.id);
      return false;
    }
    // Find in active or inactive and dispose
    {
      auto ait = _cards.find(slotIndex);
      if (ait != _cards.end()) {
        ait->second->dispose();
        _cards.erase(ait);
      } else {
        auto iit = _inactiveCards.find(slotIndex);
        if (iit != _inactiveCards.end()) {
          iit->second->dispose();
          _inactiveCards.erase(iit);
        }
      }
    }
    return true;
  }

  default:
    if (response)
      *response = OscResponse::error("Card: unknown or unsupported command");
    return false;
  }
}

//=============================================================================
// Card Registry Management
//=============================================================================

void GPUScreenImpl::registerCard(CardPtr card) {
  if (!card)
    return;

  card->setScreenOrigin(_viewportX, _viewportY);
  card->setCellSize(getCellWidth(), getCellHeight());
  uint32_t slotIndex = card->metadataSlotIndex();
  ydebug("GPUScreenImpl::registerCard: registered card at slotIndex {}", slotIndex);
  if (card->needsBuffer())  _bufferLayoutChanged = true;
  if (card->needsTexture()) _textureLayoutChanged = true;
  _cards[slotIndex] = std::move(card);
}

void GPUScreenImpl::unregisterCard(uint32_t slotIndex) {
  _cards.erase(slotIndex);
  ydebug("GPUScreenImpl::unregisterCard: unregistered card at slotIndex {}",
         slotIndex);
}

Card *GPUScreenImpl::getCardBySlotIndex(uint32_t offset) const {
  auto it = _cards.find(offset);
  return (it != _cards.end()) ? it->second.get() : nullptr;
}

Card *GPUScreenImpl::getCardAtCell(int row, int col) const {
  if (row < 0 || row >= _rows || col < 0 || col >= _cols) {
    return nullptr;
  }

  const Cell *cells = getCellData();
  if (!cells)
    return nullptr;

  size_t idx = cellIndex(row, col);
  const Cell &cell = cells[idx];

  // Check if this cell has a shader glyph (Plane 16 PUA-B: U+100000 - U+10FFFD)
  if (cell.glyph < 0x100000 || cell.glyph > 0x10FFFD) {
    return nullptr;
  }

  // The fg RGB channels encode the metadata slot index (3 bytes = 24 bits)
  uint32_t slotIndex = static_cast<uint32_t>(cell.fgR) |
                       (static_cast<uint32_t>(cell.fgG) << 8) |
                       (static_cast<uint32_t>(cell.fgB) << 16);

  return getCardBySlotIndex(slotIndex);
}

//=============================================================================
// Interface method implementations
//=============================================================================

uint32_t GPUScreenImpl::getCols() const { return static_cast<uint32_t>(_cols); }

uint32_t GPUScreenImpl::getRows() const { return static_cast<uint32_t>(_rows); }

void GPUScreenImpl::sendKey(uint32_t codepoint, int mods) {
  if (!_vterm)
    return;

  // Convert GLFW modifiers to VTermModifier
  // GLFW: GLFW_MOD_SHIFT=0x0001, GLFW_MOD_CONTROL=0x0002, GLFW_MOD_ALT=0x0004
  VTermModifier vtMods = VTERM_MOD_NONE;
  if (mods & 0x0001)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_SHIFT);
  if (mods & 0x0002)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_CTRL);
  if (mods & 0x0004)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_ALT);

  vterm_keyboard_unichar(_vterm, codepoint, vtMods);
}

void GPUScreenImpl::sendSpecialKey(int key, int mods) {
  if (!_vterm)
    return;

  ydebug("sendSpecialKey: key={} mods={}", key, mods);

  // Convert GLFW modifiers to VTermModifier
  // GLFW: GLFW_MOD_SHIFT=0x0001, GLFW_MOD_CONTROL=0x0002, GLFW_MOD_ALT=0x0004
  VTermModifier vtMods = VTERM_MOD_NONE;
  if (mods & 0x0001)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_SHIFT);
  if (mods & 0x0002)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_CTRL);
  if (mods & 0x0004)
    vtMods = static_cast<VTermModifier>(vtMods | VTERM_MOD_ALT);

  // Convert GLFW key to VTermKey
  VTermKey vtKey = VTERM_KEY_NONE;
  switch (key) {
  case 257:
    vtKey = VTERM_KEY_ENTER;
    break; // GLFW_KEY_ENTER
  case 259:
    vtKey = VTERM_KEY_BACKSPACE;
    break; // GLFW_KEY_BACKSPACE
  case 258:
    vtKey = VTERM_KEY_TAB;
    break; // GLFW_KEY_TAB
  case 256:
    vtKey = VTERM_KEY_ESCAPE;
    break; // GLFW_KEY_ESCAPE
  case 265:
    vtKey = VTERM_KEY_UP;
    break; // GLFW_KEY_UP
  case 264:
    vtKey = VTERM_KEY_DOWN;
    break; // GLFW_KEY_DOWN
  case 263:
    vtKey = VTERM_KEY_LEFT;
    break; // GLFW_KEY_LEFT
  case 262:
    vtKey = VTERM_KEY_RIGHT;
    break; // GLFW_KEY_RIGHT
  case 268:
    vtKey = VTERM_KEY_HOME;
    break; // GLFW_KEY_HOME
  case 269:
    vtKey = VTERM_KEY_END;
    break; // GLFW_KEY_END
  case 266:
    vtKey = VTERM_KEY_PAGEUP;
    break; // GLFW_KEY_PAGE_UP
  case 267:
    vtKey = VTERM_KEY_PAGEDOWN;
    break; // GLFW_KEY_PAGE_DOWN
  case 260:
    vtKey = VTERM_KEY_INS;
    break; // GLFW_KEY_INSERT
  case 261:
    vtKey = VTERM_KEY_DEL;
    break; // GLFW_KEY_DELETE
  case 290:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(1));
    break; // GLFW_KEY_F1
  case 291:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(2));
    break; // GLFW_KEY_F2
  case 292:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(3));
    break; // GLFW_KEY_F3
  case 293:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(4));
    break; // GLFW_KEY_F4
  case 294:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(5));
    break; // GLFW_KEY_F5
  case 295:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(6));
    break; // GLFW_KEY_F6
  case 296:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(7));
    break; // GLFW_KEY_F7
  case 297:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(8));
    break; // GLFW_KEY_F8
  case 298:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(9));
    break; // GLFW_KEY_F9
  case 299:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(10));
    break; // GLFW_KEY_F10
  case 300:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(11));
    break; // GLFW_KEY_F11
  case 301:
    vtKey = static_cast<VTermKey>(VTERM_KEY_FUNCTION(12));
    break; // GLFW_KEY_F12
  default:
    // Handle Ctrl/Alt + character combinations
    // GLFW key codes: SPACE=32, 0-9=48-57, A-Z=65-90
    if (mods & (0x0002 | 0x0004)) { // CTRL or ALT
      uint32_t ch = 0;
      if (key == 32) { // GLFW_KEY_SPACE
        ch = ' ';
      } else if (key >= 48 && key <= 57) { // GLFW_KEY_0 - GLFW_KEY_9
        ch = static_cast<uint32_t>(key);
      } else if (key >= 65 && key <= 90) {    // GLFW_KEY_A - GLFW_KEY_Z
        ch = static_cast<uint32_t>(key + 32); // lowercase
      }
      if (ch != 0) {
        ydebug("sendSpecialKey: sending ctrl/alt char '{}' ({})", (char)ch, ch);
        vterm_keyboard_unichar(_vterm, ch, vtMods);
      }
    }
    return;
  }

  vterm_keyboard_key(_vterm, vtKey, vtMods);
}

void GPUScreenImpl::setOutputCallback(OutputCallback cb) {
  _outputCallback = std::move(cb);
  if (_vterm && _outputCallback) {
    vterm_output_set_callback(
        _vterm,
        [](const char *data, size_t len, void *user) {
          auto *self = static_cast<GPUScreenImpl *>(user);
          if (self->_outputCallback) {
            self->_outputCallback(data, len);
          }
        },
        this);
  }
}

void GPUScreenImpl::setResizeCallback(ResizeCallback cb) {
  _resizeCallback = std::move(cb);
}

void GPUScreenImpl::registerForFocus() {
  auto loop = *base::EventLoop::instance();
  loop->registerListener(base::Event::Type::SetFocus,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseDown,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseUp,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::MouseMove,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::Scroll,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::Paste,
                         sharedAs<base::EventListener>());
  loop->registerListener(base::Event::Type::CommandKey,
                         sharedAs<base::EventListener>());
  yinfo("GPUScreen {} registered for SetFocus, Mouse, Scroll, Paste and CommandKey events", _id);

  // Auto-focus on startup so keyboard works immediately
  loop->dispatch(base::Event::focusEvent(_id));
}

Result<bool> GPUScreenImpl::onEvent(const base::Event &event) {
  // Handle mouse click - dispatch SetFocus if clicked on our surface
  if (event.type == base::Event::Type::MouseDown) {
    float mx = event.mouse.x;
    float my = event.mouse.y;
    int button = event.mouse.button;
    ydebug("GPUScreen {} MouseDown at ({}, {}), button={}, viewport=({},{} {}x{})", _id,
           mx, my, button, _viewportX, _viewportY, _viewportWidth, _viewportHeight);

    // Only check bounds if viewport is set
    if (_viewportWidth > 0 && _viewportHeight > 0) {
      // Check if click is within our viewport
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        // Right-click: open context menu
        if (button == 1 && _ctx.imguiManager) {  // GLFW_MOUSE_BUTTON_RIGHT = 1
          // Calculate cell position from mouse coordinates
          float localX = mx - _viewportX;
          float localY = my - _viewportY;
          int col = static_cast<int>(localX / getCellWidth());
          int row = static_cast<int>(localY / getCellHeight());

          // Clamp to valid range
          col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
          row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

          // Begin context menu at mouse position, then add items
          _ctx.imguiManager->beginContextMenu(mx, my);
          _ctx.imguiManager->addContextMenuItem({
            "Copy cell info",
            base::Event::contextMenuAction(_id, "copy_cell_info", row, col)
          });
          _ctx.imguiManager->addContextMenuItem({
            "Inspect glyph",
            base::Event::contextMenuAction(_id, "inspect_glyph", row, col)
          });
          _ctx.imguiManager->addContextMenuItem({
            "GPU Stats", {}, [this]() {
              _ctx.imguiManager->showGpuStatsDialog([this]() -> std::string {
                std::string stats = buildGpuStatsText();
                std::string cleaned;
                cleaned.reserve(stats.size());
                for (size_t i = 0; i < stats.size(); i++) {
                  if (stats[i] != '\r') cleaned += stats[i];
                }
                return cleaned;
              });
            }
          });

          yinfo("GPUScreen {} right-click at cell ({}, {}), opening context menu", _id, row, col);
          return Ok(true);
        }

        // Left-click in visual zoom mode: start drag-to-pan
        if (button == 0 && _visualZoomActive) {
          _visualZoomDragging = true;
          _visualZoomDragLastX = mx;
          _visualZoomDragLastY = my;
          auto loop = *base::EventLoop::instance();
          loop->dispatch(base::Event::setCursorEvent(CURSOR_MOVE));
          return Ok(true);
        }

        // Left-click: focus + start selection or card interaction
        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        float cellWidthF = _baseCellWidth * _zoomLevel;
        float cellHeightF = _baseCellHeight * _zoomLevel;
        int col = static_cast<int>(localX / cellWidthF);
        int row = static_cast<int>(localY / cellHeightF);
        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

        auto loop = *base::EventLoop::instance();

        // Check if clicked cell is a card
        Card* card = getCardAtCell(row, col);
        if (card) {
          clearSelection();
          loop->dispatch(base::Event::focusEvent(card->id()));
          float cardX, cardY;
          cardLocalCoords(localX, localY, row, col, cardX, cardY);
          // Clamp to card's pixel dimensions
          float cardPixelW = static_cast<float>(card->widthCells() * getCellWidth());
          float cardPixelH = static_cast<float>(card->heightCells() * getCellHeight());
          cardX = std::max(0.0f, std::min(cardX, cardPixelW - 1.0f));
          cardY = std::max(0.0f, std::min(cardY, cardPixelH - 1.0f));
          ydebug("GPUScreen {} card click: localXY=({:.1f},{:.1f}) cell=({},{}) cardXY=({:.1f},{:.1f}) cellSize={}x{}",
                _id, localX, localY, col, row, cardX, cardY, getCellWidth(), getCellHeight());
          _cardMouseTarget = card->id();
          loop->dispatch(base::Event::cardMouseDown(card->id(), cardX, cardY, button));
        } else if (button == 0) {
          // Left-click on terminal: start text selection
          _selecting = true;
          _hasSelection = false;
          _selStartCol = col;
          _selStartRow = row;
          _selEndCol = col;
          _selEndRow = row;
          _hasDamage = true;
          loop->dispatch(base::Event::focusEvent(_id));
        } else {
          loop->dispatch(base::Event::focusEvent(_id));
        }
        return Ok(true);
      }
    }
    return Ok(false);
  }

  // Handle mouse move - update statusbar and forward to focused card
  if (event.type == base::Event::Type::MouseMove) {
    float mx = event.mouse.x;
    float my = event.mouse.y;

    // Visual zoom drag-to-pan
    if (_visualZoomDragging) {
      float dx = mx - _visualZoomDragLastX;
      float dy = my - _visualZoomDragLastY;
      // Convert screen delta to source coordinate delta (divide by zoom)
      _visualZoomOffsetX -= dx / _visualZoomScale;
      _visualZoomOffsetY -= dy / _visualZoomScale;
      clampVisualZoomOffset();
      _visualZoomDragLastX = mx;
      _visualZoomDragLastY = my;
      return Ok(true);
    }

    if (_viewportWidth > 0 && _viewportHeight > 0) {
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        int col = static_cast<int>(localX / getCellWidth());
        int row = static_cast<int>(localY / getCellHeight());

        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

        if (_ctx.imguiManager) {
          Cell cell = getCell(row, col);
          char statusBuf[256];
          snprintf(statusBuf, sizeof(statusBuf),
                   "Row:%d Col:%d Glyph:U+%04X FG:(%d,%d,%d) BG:(%d,%d,%d) Style:0x%02X",
                   row, col, cell.glyph,
                   cell.fgR, cell.fgG, cell.fgB,
                   cell.bgR, cell.bgG, cell.bgB,
                   cell.style);
          _ctx.imguiManager->setStatusText(statusBuf);
        }

        // Extend text selection while dragging
        if (_selecting) {
          _selEndCol = col;
          _selEndRow = row;
          _hasSelection = (_selStartCol != _selEndCol || _selStartRow != _selEndRow);
          _hasDamage = true;
        }

        // Forward mouse move to active card with local coordinates
        if (_cardMouseTarget != 0) {
          Card* card = getCardAtCell(row, col);
          if (card && card->id() == _cardMouseTarget) {
            float cardX, cardY;
            cardLocalCoords(localX, localY, row, col, cardX, cardY);
            // Clamp to card's pixel dimensions
            float cardPixelW = static_cast<float>(card->widthCells() * getCellWidth());
            float cardPixelH = static_cast<float>(card->heightCells() * getCellHeight());
            cardX = std::max(0.0f, std::min(cardX, cardPixelW - 1.0f));
            cardY = std::max(0.0f, std::min(cardY, cardPixelH - 1.0f));
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::cardMouseMove(_cardMouseTarget, cardX, cardY));
          } else {
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::cardMouseMove(_cardMouseTarget, localX, localY));
          }
        }

      }
    }
    return Ok(false);  // Don't consume
  }

  // Handle mouse up - finalize selection or dispatch CardMouseUp
  if (event.type == base::Event::Type::MouseUp) {
    // End visual zoom drag
    if (_visualZoomDragging && event.mouse.button == 0) {
      _visualZoomDragging = false;
      auto loop = *base::EventLoop::instance();
      loop->dispatch(base::Event::setCursorEvent(CURSOR_CROSSHAIR));
      return Ok(true);
    }
    // Finalize text selection on left button release
    if (_selecting && event.mouse.button == 0) {
      _selecting = false;
      if (_hasSelection) {
        std::string text = extractSelectedText();
        if (!text.empty()) {
          auto payload = std::make_shared<std::string>(std::move(text));
          auto loop = *base::EventLoop::instance();
          loop->dispatch(base::Event::copyEvent(std::move(payload)));
          ydebug("GPUScreen {}: selection copied ({} bytes)", _id, text.size());
        }
      }
      return Ok(true);
    }

    if (_cardMouseTarget != 0) {
      float mx = event.mouse.x;
      float my = event.mouse.y;
      int button = event.mouse.button;
      float localX = mx - _viewportX;
      float localY = my - _viewportY;
      int col = static_cast<int>(localX / getCellWidth());
      int row = static_cast<int>(localY / getCellHeight());
      col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
      row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));

      float cardX, cardY;
      Card* card = getCardAtCell(row, col);
      if (card && card->id() == _cardMouseTarget) {
        cardLocalCoords(localX, localY, row, col, cardX, cardY);
        // Clamp to card's pixel dimensions
        float cardPixelW = static_cast<float>(card->widthCells() * getCellWidth());
        float cardPixelH = static_cast<float>(card->heightCells() * getCellHeight());
        cardX = std::max(0.0f, std::min(cardX, cardPixelW - 1.0f));
        cardY = std::max(0.0f, std::min(cardY, cardPixelH - 1.0f));
      } else {
        cardX = localX;
        cardY = localY;
      }
      auto loop = *base::EventLoop::instance();
      loop->dispatch(base::Event::cardMouseUp(_cardMouseTarget, cardX, cardY, button));
      _cardMouseTarget = 0;
      return Ok(true);
    }
    return Ok(false);
  }

  // Handle scroll - scrollback or zoom (only when focused)
  if (_focused && event.type == base::Event::Type::Scroll) {
    float mx = event.scroll.x;
    float my = event.scroll.y;

    if (_viewportWidth > 0 && _viewportHeight > 0) {
      if (mx >= _viewportX && mx < _viewportX + _viewportWidth &&
          my >= _viewportY && my < _viewportY + _viewportHeight) {

        // Check if scroll is over a card - forward as CardScroll
        float localX = mx - _viewportX;
        float localY = my - _viewportY;
        int col = static_cast<int>(localX / getCellWidth());
        int row = static_cast<int>(localY / getCellHeight());
        col = std::max(0, std::min(col, static_cast<int>(_cols) - 1));
        row = std::max(0, std::min(row, static_cast<int>(_rows) - 1));
        Card* scrollCard = getCardAtCell(row, col);
        if (scrollCard) {
          float cardX, cardY;
          cardLocalCoords(localX, localY, row, col, cardX, cardY);
          // Clamp to card's pixel dimensions
          float cardPixelW = static_cast<float>(scrollCard->widthCells() * getCellWidth());
          float cardPixelH = static_cast<float>(scrollCard->heightCells() * getCellHeight());
          cardX = std::max(0.0f, std::min(cardX, cardPixelW - 1.0f));
          cardY = std::max(0.0f, std::min(cardY, cardPixelH - 1.0f));
          auto loop = *base::EventLoop::instance();
          loop->dispatch(base::Event::cardScrollEvent(
              scrollCard->id(), cardX, cardY, event.scroll.dx, event.scroll.dy, event.scroll.mods));
          return Ok(true);
        }

        bool ctrlPressed = (event.scroll.mods & 0x0002) != 0;   // GLFW_MOD_CONTROL
        bool shiftPressed = (event.scroll.mods & 0x0001) != 0; // GLFW_MOD_SHIFT

        if (ctrlPressed && shiftPressed) {
          // Ctrl+Shift+scroll: structural zoom (changes terminal grid)
          float newZoom = _zoomLevel + event.scroll.dy * 0.02f;
          newZoom = std::max(0.2f, std::min(newZoom, 5.0f));
          if (newZoom != _zoomLevel) {
            _zoomLevel = newZoom;
            // Recalculate cols/rows from viewport and new zoom
            setViewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
            // Notify cards of new cell size
            for (auto& [slotIndex, card] : _cards) {
              card->setCellSize(getCellWidth(), getCellHeight());
            }
            // Update raster font cell size (triggers re-rasterization)
            if (_rasterFont) {
              _rasterFont->setCellSize(getCellWidth(), getCellHeight());
              _rasterFont->uploadToGpu();
            }
            yinfo("GPUScreen {} zoom: {:.0f}%", _id, _zoomLevel * 100.0f);
          }
        } else if (ctrlPressed) {
          // Ctrl+scroll: Visual Zoom (shader-based)
          if (!_visualZoomActive && event.scroll.dy > 0) {
            _visualZoomActive = true;
            _visualZoomScale = 1.0f;
            _visualZoomOffsetX = 0.0f;
            _visualZoomOffsetY = 0.0f;
            auto loop = *base::EventLoop::instance();
            loop->dispatch(base::Event::setCursorEvent(CURSOR_CROSSHAIR));
          }
          if (_visualZoomActive) {
            _visualZoomScale += event.scroll.dy * 0.1f;
            _visualZoomScale = std::clamp(_visualZoomScale, 1.0f, 100.0f);
            if (_visualZoomScale <= 1.0f) {
              // Exit visual zoom when zoomed all the way out
              _visualZoomActive = false;
              _visualZoomScale = 1.0f;
              _visualZoomOffsetX = 0.0f;
              _visualZoomOffsetY = 0.0f;
              auto loop = *base::EventLoop::instance();
              loop->dispatch(base::Event::setCursorEvent(CURSOR_DEFAULT));
              yinfo("GPUScreen {} visual zoom: off", _id);
            } else {
              // Re-clamp offsets after scale change
              clampVisualZoomOffset();
              yinfo("GPUScreen {} visual zoom: {:.1f}x", _id, _visualZoomScale);
            }
          }
        } else if (_visualZoomActive) {
          // In visual zoom mode: scroll pans the view
          if (shiftPressed) {
            // Shift+scroll: horizontal pan
            _visualZoomOffsetX -= event.scroll.dy * getCellWidth();
          } else {
            // Plain scroll: vertical pan
            _visualZoomOffsetY -= event.scroll.dy * getCellHeight() * 3;
          }
          clampVisualZoomOffset();
        } else {
          // Normal scroll: enter copy mode and scroll
          if (!_copyMode && event.scroll.dy > 0) {
            enterCopyMode();
          }
          int lines = static_cast<int>(event.scroll.dy * 3);  // 3 lines per notch
          if (lines > 0) {
            scrollUp(lines);
          } else if (lines < 0) {
            scrollDown(-lines);
          }
        }
        return Ok(true);  // Consume
      }
    }
    return Ok(false);
  }

  if (event.type == base::Event::Type::SetFocus) {
    auto loop = *base::EventLoop::instance();
    if (event.setFocus.objectId == _id) {
      // This GPUScreen is being focused
      if (!_focused) {
        _focused = true;
        // Register for keyboard events
        loop->registerListener(base::Event::Type::Char,
                               sharedAs<base::EventListener>());
        loop->registerListener(base::Event::Type::KeyDown,
                               sharedAs<base::EventListener>());
        yinfo("GPUScreen {} gained focus, registered for keyboard events", _id);
      }
    } else {
      // Another element is being focused, we lose focus
      if (_focused) {
        _focused = false;
        // Deregister from keyboard events (but keep SetFocus registration)
        loop->deregisterListener(base::Event::Type::Char,
                                 sharedAs<base::EventListener>());
        loop->deregisterListener(base::Event::Type::KeyDown,
                                 sharedAs<base::EventListener>());
        yinfo("GPUScreen {} lost focus, deregistered from keyboard events",
              _id);
      }
    }
    // Don't consume - all GPUScreens need to see SetFocus
    return Ok(false);
  }

  // Handle Paste event — write pasted text to PTY
  if (event.type == base::Event::Type::Paste && _focused && event.payload) {
    auto text = std::static_pointer_cast<std::string>(event.payload);
    if (text && !text->empty() && _outputCallback) {
      _outputCallback(text->c_str(), text->size());
      ydebug("GPUScreen {}: pasted {} bytes", _id, text->size());
    }
    return Ok(true);
  }

  // Handle keyboard events (only when focused)
  if (_focused) {
    // Handle command mode (waiting for key after Ctrl+\) - check FIRST
    if (_commandMode) {
      if (event.type == base::Event::Type::Char) {
        uint32_t cp = event.chr.codepoint;
        ydebug("GPUScreen: Command mode Char: codepoint={} ('{}')", cp, (char)cp);
        _commandMode = false;

        // '[' - Enter copy mode
        if (cp == '[') {
          ydebug("GPUScreen: Entering copy mode via Char '[']");
          enterCopyMode();
          return Ok(true);
        }

        // Other commands can be added here
        return Ok(true);  // consume unknown command
      }
      if (event.type == base::Event::Type::KeyDown) {
        int key = event.key.key;
        ydebug("GPUScreen: Command mode KeyDown: key={}", key);

        // Escape cancels command mode
        if (key == 256) {  // GLFW_KEY_ESCAPE
          _commandMode = false;
          return Ok(true);
        }

        // For printable keys, just consume KeyDown - the Char event will handle the command
        // Don't reset _commandMode here, let Char event do it
        return Ok(true);
      }
    }

    // Check for Ctrl+\ prefix key (enters command mode)
    // Ctrl+\ sends ASCII 28 (File Separator) or codepoint 92 with Ctrl mod
    if (event.type == base::Event::Type::Char) {
      if ((event.chr.codepoint == 28) ||
          (event.chr.codepoint == '\\' && (event.chr.mods & 0x0002))) {
        ydebug("GPUScreen: Entering command mode (Ctrl+\\ pressed)");
        _commandMode = true;
        return Ok(true);  // consume
      }
    }
    if (event.type == base::Event::Type::KeyDown) {
      // Also check KeyDown for Ctrl+\ (key code 92)
      if (event.key.key == 92 && (event.key.mods & 0x0002) && !(event.key.mods & ~0x0002)) {
        ydebug("GPUScreen: Entering command mode (Ctrl+\\ pressed via KeyDown)");
        _commandMode = true;
        return Ok(true);  // consume
      }
      // Escape or Enter exits visual zoom mode (Enter first, then copy mode on next press)
      if ((event.key.key == 256 || event.key.key == 257) && _visualZoomActive) {  // GLFW_KEY_ESCAPE or GLFW_KEY_ENTER
        _visualZoomActive = false;
        _visualZoomScale = 1.0f;
        _visualZoomOffsetX = 0.0f;
        _visualZoomOffsetY = 0.0f;
        _visualZoomDragging = false;
        auto loop = *base::EventLoop::instance();
        loop->dispatch(base::Event::setCursorEvent(CURSOR_DEFAULT));
        yinfo("GPUScreen {} visual zoom: off", _id);
        return Ok(true);  // consume
      }
    }

    // Handle keyboard events in copy mode
    if (_copyMode) {
      if (event.type == base::Event::Type::Char) {
        if (handleCopyModeKey(event.chr.codepoint, 0, event.chr.mods)) {
          return Ok(true);
        }
      }
      if (event.type == base::Event::Type::KeyDown) {
        if (handleCopyModeKey(0, event.key.key, event.key.mods)) {
          return Ok(true);
        }
      }
      // Consume all keyboard input in copy mode
      return Ok(true);
    }

    // Normal mode: send keys to vterm
    // Clear selection on any keyboard input
    if (_hasSelection &&
        (event.type == base::Event::Type::Char || event.type == base::Event::Type::KeyDown)) {
      clearSelection();
    }

    if (event.type == base::Event::Type::Char) {
      ydebug("GPUScreen::onEvent: Char codepoint={} mods={}",
             event.chr.codepoint, event.chr.mods);
      sendKey(event.chr.codepoint, event.chr.mods);
      return Ok(true);
    }

    if (event.type == base::Event::Type::KeyDown) {
      ydebug("GPUScreen::onEvent: KeyDown key={} mods={}", event.key.key,
             event.key.mods);
      sendSpecialKey(event.key.key, event.key.mods);
      return Ok(true);
    }
  }

  return Ok(false);
}

Result<void> GPUScreenImpl::initPipeline() {
  if (_pipelineInitialized)
    return Ok();

  WGPUDevice device = _ctx.gpu.device;
  yinfo("GPUScreenImpl::initPipeline: initializing per-instance resources");

  // Verify ShaderManager has shared resources ready
  auto shaderMgr = _ctx.shaderManager;
  if (!shaderMgr->getGridPipeline() || !shaderMgr->getGridBindGroupLayout()) {
    return Err<void>("ShaderManager not initialized - shared pipeline "
                     "resources not available");
  }

  // Create per-instance uniform buffer only
  WGPUBufferDescriptor uniformDesc = {};
  uniformDesc.label = WGPU_STR("grid uniforms");
  uniformDesc.size = sizeof(Uniforms);
  uniformDesc.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
  _uniformBuffer = _allocator->createBuffer(uniformDesc);
  if (!_uniformBuffer) {
    return Err<void>("Failed to create uniform buffer");
  }

  _pipelineInitialized = true;
  yinfo("GPUScreenImpl::initPipeline: per-instance resources initialized");
  return Ok();
}

Result<void> GPUScreenImpl::render(WGPURenderPassEncoder pass) {
  // Suspend active cards no longer in the composed view (e.g., scrolled away in scrollback)
  suspendNonVisibleCards();

  // Reactivate any suspended cards whose cells are now visible (user scrolled back)
  reactivateVisibleCards();

  // Periodic GC: dispose inactive cards no longer referenced in any buffer
  if (!_inactiveCards.empty() && ++_gcFrameCounter >= 180) {
    _gcFrameCounter = 0;
    gcInactiveCards();
  }

  // Loop 1+2: Buffer cards — only when a buffer card entered or left
  if (_bufferLayoutChanged && _cardManager) {
    ydebug("GPUScreen::render: _bufferLayoutChanged=true, {} active cards", _cards.size());

    // Loop 1: Buffer cards declare their buffer needs
    for (auto& [slotIndex, card] : _cards) {
      if (card->needsBuffer()) {
        ydebug("GPUScreen::render: Loop1 declareBufferNeeds card='{}' slot={}", card->typeName(), slotIndex);
        card->declareBufferNeeds();
      }
    }
    // Pre-size buffer to fit all declared needs
    if (auto res = _cardManager->bufferManager()->commitReservations(); !res) {
      yerror("GPUScreen::render: commitReservations failed: {}", error_msg(res));
      return Err<void>("GPUScreen::render: commitReservations failed", res);
    }

    // Loop 2: Buffer cards allocate their buffers
    for (auto& [slotIndex, card] : _cards) {
      if (card->needsBuffer()) {
        ydebug("GPUScreen::render: Loop2 allocateBuffers card='{}' slot={}", card->typeName(), slotIndex);
        if (auto res = card->allocateBuffers(); !res) {
          yerror("GPUScreen::render: card '{}' allocateBuffers FAILED: {}", card->typeName(), error_msg(res));
        }
      }
    }

    _bufferLayoutChanged = false;
  }

  // Loop 3: Texture cards — only when a texture card entered or left
  if (_textureLayoutChanged && _cardManager && _cardManager->textureManager()) {
    ydebug("GPUScreen::render: _textureLayoutChanged=true, {} active cards", _cards.size());

    // Clear all old texture handles — cards will re-allocate fresh
    _cardManager->textureManager()->clearHandles();

    // All active texture cards re-allocate
    for (auto& [slotIndex, card] : _cards) {
      if (card->needsTexture()) {
        ydebug("GPUScreen::render: Loop3 allocateTextures card='{}' slot={}", card->typeName(), slotIndex);
        if (auto res = card->allocateTextures(); !res) {
          yerror("GPUScreen::render: card '{}' allocateTextures FAILED: {}", card->typeName(), error_msg(res));
        }
      }
    }

    // Pack and create atlas sized to current needs
    if (auto res = _cardManager->textureManager()->createAtlas(); !res) {
      yerror("GPUScreen::render: createAtlas FAILED: {}", error_msg(res));
    }

    // Write pixel data into the atlas (now that it exists)
    for (auto& [slotIndex, card] : _cards) {
      if (card->needsTexture()) {
        if (auto res = card->writeTextures(); !res) {
          yerror("GPUScreen::render: card '{}' writeTextures FAILED: {}", card->typeName(), error_msg(res));
        }
      }
    }

    _cardManager->invalidateBindGroup();

    _textureLayoutChanged = false;
  }

  auto _loopT1 = std::chrono::steady_clock::now();

  // Loop 4: Render all active cards (write data, no allocations)
  for (auto& [slotIndex, card] : _cards) {
    ydebug("GPUScreen::render: Loop4 render card='{}' slot={} metaOffset={}",
           card->typeName(), slotIndex, card->metadataOffset());
    {
      static auto t0 = std::chrono::steady_clock::now();
      auto now = std::chrono::steady_clock::now();
      float timeSec = std::chrono::duration<float>(now - t0).count();
      if (auto res = card->render(timeSec); !res) {
        yerror("GPUScreen::render: card '{}' render FAILED: {}", card->typeName(), error_msg(res));
        return Err<void>("GPUScreen::render: card render failed", res);
      }
    }
  }

  // Flush CardManager (packs atlas, updates bind group if needed, uploads buffers)
  if (_cardManager) {
    ydebug("GPUScreen::render: calling flush");
    if (auto res = _cardManager->flush(_ctx.gpu.queue); !res) {
      yerror("GPUScreen: CardManager flush FAILED: {}", error_msg(res));
      return Err<void>("GPUScreen: CardManager flush failed", res);
    }
  }

  const Cell *cells = getCellData();
  if (!cells || _cols == 0 || _rows == 0) {
    return Ok(); // Nothing to render
  }

  // Calculate total rows including status line
  int totalRows = _statusLineEnabled ? _rows + 1 : _rows;

  // Initialize pipeline on first render
  if (!_pipelineInitialized) {
    if (auto res = initPipeline(); !res) {
      return res;
    }
  }

  WGPUDevice device = _ctx.gpu.device;
  WGPUQueue queue = _ctx.gpu.queue;
  auto shaderMgr = _ctx.shaderManager;

  // Hot-recompile shader if providers are dirty (e.g. new glyph/card enabled)
  shaderMgr->update();

  // Recreate cell buffer if grid size changed (include status line row)
  size_t requiredSize = static_cast<size_t>(_cols) * totalRows * sizeof(Cell);
  if (_cols != _textureCols || totalRows != _textureRows || !_cellBuffer ||
      _cellBufferSize < requiredSize) {
    if (_cellBuffer) {
      _allocator->releaseBuffer(_cellBuffer);
      _cellBuffer = nullptr;
    }

    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.label = WGPU_STR("cell buffer");
    bufferDesc.size = requiredSize;
    bufferDesc.usage = WGPUBufferUsage_Storage | WGPUBufferUsage_CopyDst;
    _cellBuffer = _allocator->createBuffer(bufferDesc);
    if (!_cellBuffer) {
      return Err<void>("Failed to create cell buffer");
    }
    _cellBufferSize = requiredSize;
    _textureCols = _cols;
    _textureRows = totalRows;
    _needsBindGroupRecreation = true;
    yinfo("GPUScreenImpl::render: created cell buffer {}x{} ({} bytes)", _cols,
          totalRows, requiredSize);
  }

  // Upload lazily-loaded bitmap font glyphs to GPU
  if (_bitmapFont && _bitmapFont->isDirty()) {
    _bitmapFont->uploadToGpu();
    _needsBindGroupRecreation = true;
  }

  // Recreate bind group if needed
  if (_needsBindGroupRecreation || !_bindGroup ||
      (_msdfFont &&
       _msdfFont->atlas()->getResourceVersion() != _lastFontResourceVersion)) {
    if (_bindGroup) {
      wgpuBindGroupRelease(_bindGroup);
      _bindGroup = nullptr;
    }

    if (!_msdfFont || !_msdfFont->atlas()->getTextureView() ||
        !_msdfFont->atlas()->getSampler() || !_msdfFont->atlas()->getGlyphMetadataBuffer()) {
      return Err<void>("MSDF font resources not ready");
    }
    if (!_bitmapFont || !_bitmapFont->getTextureView() ||
        !_bitmapFont->getSampler() || !_bitmapFont->getMetadataBuffer()) {
      return Err<void>("Bitmap font resources not ready");
    }

    WGPUBindGroupEntry bgEntries[15] = {};  // 12 + 3 for raster font

    bgEntries[0].binding = 0;
    bgEntries[0].buffer = _uniformBuffer;
    bgEntries[0].size = sizeof(Uniforms);

    bgEntries[1].binding = 1;
    bgEntries[1].textureView = _msdfFont->atlas()->getTextureView();

    bgEntries[2].binding = 2;
    bgEntries[2].sampler = _msdfFont->atlas()->getSampler();

    bgEntries[3].binding = 3;
    bgEntries[3].buffer = _msdfFont->atlas()->getGlyphMetadataBuffer();
    bgEntries[3].size =
        _msdfFont->atlas()->getBufferGlyphCount() * sizeof(GlyphMetadataGPU);

    bgEntries[4].binding = 4;
    bgEntries[4].buffer = _cellBuffer;
    bgEntries[4].size = _cellBufferSize;

    bgEntries[5].binding = 5;
    bgEntries[5].textureView = _bitmapFont->getTextureView();

    bgEntries[6].binding = 6;
    bgEntries[6].sampler = _bitmapFont->getSampler();

    bgEntries[7].binding = 7;
    bgEntries[7].buffer = _bitmapFont->getMetadataBuffer();
    uint32_t glyphsPerRow =
        _bitmapFont->getAtlasWidth() / _bitmapFont->getGlyphSize();
    bgEntries[7].size =
        glyphsPerRow * glyphsPerRow * sizeof(BitmapGlyphMetadata);

    // Vector font bindings (8 and 9)
    if (!_vectorFont || !_vectorFont->getGlyphBuffer() || !_vectorFont->getOffsetBuffer()) {
      return Err<void>("Vector font resources not ready");
    }

    bgEntries[8].binding = 8;
    bgEntries[8].buffer = _vectorFont->getGlyphBuffer();
    bgEntries[8].size = _vectorFont->bufferSize();

    bgEntries[9].binding = 9;
    bgEntries[9].buffer = _vectorFont->getOffsetBuffer();
    bgEntries[9].size = _vectorFont->offsetTableSize();

    // Coverage font bindings (10 and 11)
    // Note: coverage font is optional, use vector font buffers as fallback if not available
    WGPUBuffer coverageGlyphBuffer = _coverageFont ? _coverageFont->getGlyphBuffer() : _vectorFont->getGlyphBuffer();
    WGPUBuffer coverageOffsetBuffer = _coverageFont ? _coverageFont->getOffsetBuffer() : _vectorFont->getOffsetBuffer();
    size_t coverageGlyphSize = _coverageFont ? _coverageFont->bufferSize() : _vectorFont->bufferSize();
    size_t coverageOffsetSize = _coverageFont ? _coverageFont->offsetTableSize() : _vectorFont->offsetTableSize();

    bgEntries[10].binding = 10;
    bgEntries[10].buffer = coverageGlyphBuffer;
    bgEntries[10].size = coverageGlyphSize;

    bgEntries[11].binding = 11;
    bgEntries[11].buffer = coverageOffsetBuffer;
    bgEntries[11].size = coverageOffsetSize;

    // Raster font bindings (12, 13, 14)
    // Use dummy resources if raster font not available
    if (_rasterFont && _rasterFont->getTextureView() && _rasterFont->getSampler() && _rasterFont->getMetadataBuffer()) {
        bgEntries[12].binding = 12;
        bgEntries[12].textureView = _rasterFont->getTextureView();

        bgEntries[13].binding = 13;
        bgEntries[13].sampler = _rasterFont->getSampler();

        bgEntries[14].binding = 14;
        bgEntries[14].buffer = _rasterFont->getMetadataBuffer();
        bgEntries[14].size = _rasterFont->getMetadataBufferSize();
    } else {
        // Fallback: use bitmap font resources as placeholder
        bgEntries[12].binding = 12;
        bgEntries[12].textureView = _bitmapFont->getTextureView();

        bgEntries[13].binding = 13;
        bgEntries[13].sampler = _bitmapFont->getSampler();

        bgEntries[14].binding = 14;
        bgEntries[14].buffer = _bitmapFont->getMetadataBuffer();
        bgEntries[14].size = 8;  // Minimum size
    }

    WGPUBindGroupDescriptor bindGroupDesc = {};
    bindGroupDesc.layout = shaderMgr->getGridBindGroupLayout();
    bindGroupDesc.entryCount = 15;
    bindGroupDesc.entries = bgEntries;
    _bindGroup = wgpuDeviceCreateBindGroup(device, &bindGroupDesc);
    if (!_bindGroup) {
      return Err<void>("Failed to create bind group");
    }

    _needsBindGroupRecreation = false;
    if (_msdfFont) {
      _lastFontResourceVersion = _msdfFont->atlas()->getResourceVersion();
    }
  }

  // Upload vterm cells to GPU
  wgpuQueueWriteBuffer(queue, _cellBuffer, 0, cells,
                       _cols * _rows * sizeof(Cell));

  // Upload status line cells (appended after vterm cells)
  if (_statusLineEnabled && !_statusLine.empty()) {
    size_t statusOffset = static_cast<size_t>(_cols * _rows) * sizeof(Cell);
    wgpuQueueWriteBuffer(queue, _cellBuffer, statusOffset, _statusLine.data(),
                         _statusLine.size() * sizeof(Cell));
  }

  // Debug: scan uploaded cells for card glyphs
  {
    int cardCellCount = 0;
    for (size_t i = 0; i < static_cast<size_t>(_cols * _rows); i++) {
      const Cell& c = cells[i];
      uint8_t fontType = (c.style >> 5) & 0x07;
      if (fontType == FONT_TYPE_CARD) {
        if (cardCellCount < 3) {
          uint32_t slotIdx = c.fgR | (static_cast<uint32_t>(c.fgG) << 8) | (static_cast<uint32_t>(c.fgB) << 16);
          uint32_t bg24 = c.bgR | (static_cast<uint32_t>(c.bgG) << 8) | (static_cast<uint32_t>(c.bgB) << 16);
          ydebug("render cell[{}]: CARD glyph={:#x} slotIdx={} bg24={:#x} style={:#x}",
                 i, c.glyph, slotIdx, bg24, c.style);
        }
        cardCellCount++;
      }
    }
    if (cardCellCount > 0) {
      ydebug("render: {} card cells in buffer ({}x{})", cardCellCount, _cols, _rows);
    }
  }


  // Precise float cell size for GPU shaders
  float cellWidthF = _baseCellWidth * _zoomLevel;
  float cellHeightF = _baseCellHeight * _zoomLevel;

  // Screen size is grid * cell size (including status line)
  float screenWidth = static_cast<float>(_cols) * cellWidthF;
  float screenHeight = static_cast<float>(totalRows) * cellHeightF;

  // Update uniforms
  _uniforms.projection =
      glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
  _uniforms.screenSize = {screenWidth, screenHeight};
  _uniforms.cellSize = {cellWidthF, cellHeightF};
  _uniforms.gridSize = {static_cast<float>(_cols), static_cast<float>(totalRows)};
  _uniforms.pixelRange = _msdfFont ? _msdfFont->atlas()->getPixelRange() : 2.0f;
  _uniforms.scale = _zoomLevel;
  
  // In copy mode, show cursor at copy mode position (converted to view row)
  if (_copyMode) {
    int sbSize = static_cast<int>(_scrollback.size());
    int firstVisibleRow = sbSize - _scrollOffset;
    int viewRow = _copyCursorRow - firstVisibleRow;
    _uniforms.cursorPos = {static_cast<float>(_copyCursorCol),
                           static_cast<float>(viewRow)};
    _uniforms.cursorVisible = 1.0f;  // Always show cursor in copy mode
  } else {
    _uniforms.cursorPos = {static_cast<float>(_cursorCol),
                           static_cast<float>(_cursorRow)};
    _uniforms.cursorVisible = _cursorVisible ? 1.0f : 0.0f;
  }
  _uniforms.cursorShape = static_cast<float>(_cursorShape);
  _uniforms.viewportOrigin = {_viewportX, _viewportY};
  _uniforms.cursorBlink = _cursorBlink ? 1.0f : 0.0f;

  // Selection uniforms — normalize direction so start <= end
  if (_hasSelection) {
    int r0 = _selStartRow, c0 = _selStartCol;
    int r1 = _selEndRow, c1 = _selEndCol;
    if (r0 > r1 || (r0 == r1 && c0 > c1)) {
      std::swap(r0, r1);
      std::swap(c0, c1);
    }
    _uniforms.hasSelection = 1.0f;
    _uniforms.selStart = {static_cast<float>(c0), static_cast<float>(r0)};
    _uniforms.selEnd = {static_cast<float>(c1), static_cast<float>(r1)};
  } else {
    _uniforms.hasSelection = 0.0f;
    _uniforms.selStart = {0.0f, 0.0f};
    _uniforms.selEnd = {0.0f, 0.0f};
  }

  // Default fg/bg colors (packed as RGBA u32, same format as cell.fg/bg)
  _uniforms.defaultFg = defaultFg_.rgb.red | (defaultFg_.rgb.green << 8) | (defaultFg_.rgb.blue << 16) | (0xFFu << 24);
  _uniforms.defaultBg = defaultBg_.rgb.red | (defaultBg_.rgb.green << 8) | (defaultBg_.rgb.blue << 16);
  _uniforms.spaceGlyph = _cachedSpaceGlyph;

  // Visual Zoom
  _uniforms.visualZoomScale = _visualZoomActive ? _visualZoomScale : 1.0f;
  _uniforms.visualZoomOffsetX = _visualZoomOffsetX;
  _uniforms.visualZoomOffsetY = _visualZoomOffsetY;

  wgpuQueueWriteBuffer(queue, _uniformBuffer, 0, &_uniforms, sizeof(Uniforms));

  // Set viewport and scissor to this terminal's tile bounds
  if (_viewportWidth > 0 && _viewportHeight > 0) {
    wgpuRenderPassEncoderSetViewport(pass, _viewportX, _viewportY,
                                     _viewportWidth, _viewportHeight, 0.0f,
                                     1.0f);
    wgpuRenderPassEncoderSetScissorRect(pass,
        static_cast<uint32_t>(_viewportX), static_cast<uint32_t>(_viewportY),
        static_cast<uint32_t>(_viewportWidth), static_cast<uint32_t>(_viewportHeight));
  }

  // Draw using shared pipeline and quad vertex buffer from ShaderManager
  wgpuRenderPassEncoderSetPipeline(pass, shaderMgr->getGridPipeline());
  // Use per-screen CardManager bind group (uniforms + card buffers),
  // falling back to the minimal shared bind group (uniforms only)
  WGPUBindGroup bindGroup0 = _cardManager
      ? _cardManager->sharedBindGroup()
      : nullptr;
  if (!bindGroup0) bindGroup0 = _ctx.gpu.sharedBindGroup;
  ydebug("render: bindGroup0={} (cardMgr={} sharedBG={} fallback={})",
         (void*)bindGroup0,
         (void*)(_cardManager ? _cardManager->sharedBindGroup() : nullptr),
         (void*)_ctx.gpu.sharedBindGroup,
         !_cardManager || !_cardManager->sharedBindGroup());
  wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup0, 0, nullptr);
  wgpuRenderPassEncoderSetBindGroup(pass, 1, _bindGroup, 0, nullptr);
  wgpuRenderPassEncoderSetVertexBuffer(
      pass, 0, shaderMgr->getQuadVertexBuffer(), 0, WGPU_WHOLE_SIZE);
  wgpuRenderPassEncoderDraw(pass, 6, 1, 0, 0);

  return Ok();
}

//=============================================================================
// Factory
//=============================================================================

Result<GPUScreen::Ptr>
GPUScreen::create(const YettyContext &ctx) noexcept {
  auto screen = std::make_shared<GPUScreenImpl>(ctx);
  if (auto res = screen->init(); !res) {
    return Err<Ptr>("GPUScreen init failed", res);
  }
  screen->registerForFocus();
  return Ok<Ptr>(screen);
}

Result<GPUScreen::Ptr> GPUScreen::createForTest(uint32_t cols,
                                                uint32_t rows) noexcept {
  YettyContext nullContext = {}; // Empty context - no GPU, no managers
  auto screen = std::make_shared<GPUScreenImpl>(nullContext);
  // Don't call init() - no font manager in test context
  // Don't register for focus - no event loop in tests
  // Resize to requested dimensions
  screen->resize(cols, rows);
  return Ok<Ptr>(screen);
}

} // namespace yetty
